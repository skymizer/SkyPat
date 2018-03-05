//===- skypat.h -----------------------------------------------------------===//
//
//                     The SkyPat Team
//
// This file is distributed under the New BSD License.
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#ifndef SKYPAT_SKYPAT_H
#define SKYPAT_SKYPAT_H
#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include <skypat/Support/OStrStream.h>

#define SKYPAT_VERNUM 0x24

namespace skypat {
enum PerfEvent {
// type is PERF_TYPE_HARDWARE
  CPU_CYCLES, // = 0
  INSTRUCTIONS,
  CACHE_REFERENCES,
  CACHE_MISSES,
  BRANCH_INSTRUCTIONS,
  BRANCH_MISSES,
  BUS_CYCLES,
  STALLED_CYCLES_FRONTEND,
  STALLED_CYCLES_BACKEND,
  REF_CPU_CYCLES, // = 9
// type is PERF_TYPE_SOFTWARE
  CPU_CLOCK, // = 10
  TASK_CLOCK,
  PAGE_FAULTS,
  CONTEXT_SWITCHES,
  CPU_MIGRATIONS,
  PAGE_FAULTS_MIN,
  PAGE_FAULTS_MAJ,
  ALIGNMENT_FAULTS,
  EMULATION_FAULTS // = 18
};

enum PerfEventCache{
// type is PERF_TYPE_HW_CACHE
// TODO: add CPU cache events
};

extern char const *Perf_event_name[];

class Test;

namespace testing {
namespace internal {
class Timer;
class Perf;

//===----------------------------------------------------------------------===//
// ADT
//===----------------------------------------------------------------------===//
class Uncopyable
{
protected:
  Uncopyable() { }
  ~Uncopyable() { }

private:
  Uncopyable(const Uncopyable&); /// NOT TO IMPLEMENT
  Uncopyable& operator=(const Uncopyable&); /// NOT TO IMPLEMENT
};

} // namespace of internal

//===----------------------------------------------------------------------===//
// Forward Declaration
//===----------------------------------------------------------------------===//
class AssertionResult;
class TestCase;
class TestInfo;
class TestResult;
class TestFactoryBase;
class PartResult;
class TestPartResult;
class PerfPartResult;
class UnitTest;

// Expands to the name of the class that implements the given test.
#define SKYPAT_TEST_CLASS_NAME_(case_name, test_name) \
  case_name##_##test_name##_Test

/// Helper macro for defining Cases.
#define SKYPAT_TEST_CASE(case_name, test_name, parent_class)\
class SKYPAT_TEST_CLASS_NAME_(case_name, test_name) : public parent_class {\
 public:\
  SKYPAT_TEST_CLASS_NAME_(case_name, test_name)() {}\
 private:\
  virtual void TestBody();\
  static skypat::testing::TestInfo* const m_TestInfo;\
};\
\
skypat::testing::TestInfo* const SKYPAT_TEST_CLASS_NAME_(case_name, test_name)\
  ::m_TestInfo =\
    skypat::testing::MakeAndRegisterTestInfo(\
        #case_name, #test_name, \
        new skypat::testing::TestFactory<\
            SKYPAT_TEST_CLASS_NAME_(case_name, test_name)>);\
void SKYPAT_TEST_CLASS_NAME_(case_name, test_name)::TestBody()

// The message handling macros. The assignment is used to trigger recording a
// partial result.
#define SKYPAT_MESSAGE_AT(file, line, message, result_type) \
  skypat::testing::AssertHelper(result_type, file, line, message) = \
      skypat::testing::Message()

#define SKYPAT_MESSAGE(message, result_type) \
  SKYPAT_MESSAGE_AT(__FILE__, __LINE__, message, result_type)

// SKYPAT_FATAL_FAILURE is a run-time result of fatal error
#define SKYPAT_FATAL_FAILURE(message) \
  return SKYPAT_MESSAGE(message, skypat::testing::TestPartResult::kFatalFailure)

// SKYPAT_NON_FATAL_FAILURE is a run-time result of expectation error
#define SKYPAT_NONFATAL_FAILURE(message) \
  SKYPAT_MESSAGE(message, skypat::testing::TestPartResult::kNonFatalFailure)

// SKYPAT_SUCCESS is a run-time result of success.
#define SKYPAT_SUCCESS(message) \
  SKYPAT_MESSAGE(message, skypat::testing::TestPartResult::kSuccess)

// The GNU compiler emits a warning if nested "if" statements are followed by
// an "else" statement and braces are not used to explicitly disambiguate the
// "else" binding.
#ifdef __INTEL_COMPILER
# define SKYPAT_UNAMBIGUOUS_ELSE_BLOCKER
#else
# define SKYPAT_UNAMBIGUOUS_ELSE_BLOCKER switch (0) case 0: default:  // NOLINT
#endif

// Implements Boolean test assertions such as EXPECT_TRUE. expression can be
// either a boolean expression or an AssertionResult. text is a textual
// represenation of expression as it was passed into the EXPECT_TRUE.
//
// The last parameter 'fail' is the run-time result of the testing
#define SKYPAT_TEST_BOOLEAN(expression, text, actual, expected, fail) \
  SKYPAT_UNAMBIGUOUS_ELSE_BLOCKER \
  if (const skypat::testing::AssertionResult _ar_ = \
      skypat::testing::AssertionResult(expression)) \
    SKYPAT_SUCCESS(""); \
  else \
    fail(skypat::testing::GetBoolAssertionFailureMessage(\
        _ar_, text, #actual, #expected))

// Implements Predicate test assertions such as EXPECT_EQ.
//
// The last parameter 'fail' is the run-time result of the testing
#define SKYPAT_TEST_PREDICATE(expression, text, actual, expected, fail) \
  SKYPAT_UNAMBIGUOUS_ELSE_BLOCKER \
  if (const skypat::testing::AssertionResult _ar_ = \
      skypat::testing::AssertionResult(expression)) \
    SKYPAT_SUCCESS(""); \
  else \
    fail(skypat::testing::GetPredAssertionFailureMessage(\
        _ar_, text, actual, #actual, expected, #expected))

//===----------------------------------------------------------------------===//
// Supports
//===----------------------------------------------------------------------===//
/// Interval - the unit of time.
typedef uint64_t Interval;

//===----------------------------------------------------------------------===//
// Core
//===----------------------------------------------------------------------===//
class TestFactoryBase
{
public:
  virtual ~TestFactoryBase() { }
  virtual skypat::Test* CreateTest() = 0;
};

template<typename SingleTest> struct TestFactory : public TestFactoryBase {
  virtual skypat::Test* CreateTest() { return new SingleTest; }
};

/** \class PerfIterator
 *  \brief PerfIterator is used to calculate the computing time of a
 *  performance test.
 */
class PerfIterator
{
public:
  /// @param pFileName the source file name.
  /// @param pLoC the line of code.
  PerfIterator(const char* pFileName, int pLoC);

  /// @param pFileName the source file name.
  /// @param pLoC the line of code.
  /// @param pFlavor the flavor of event (COUNT / SAMPLE)
  /// @param pType the type of event (HW / SW)
  /// @param pEvent the name of event
  PerfIterator(const char* pFileName, int pLoC,\
               enum PerfEvent pEvent);

  /// Destructor. The place to sum up the time.
  ~PerfIterator();

  /// increase counter
  PerfIterator& next();

  /// @return true if we should go to the next step.
  bool hasNext() const;

private:
  int m_Counter;
  internal::Timer* m_pTimer;
  internal::Perf* m_pPerf;
  PerfPartResult* m_pPerfResult;
};

/** \class PartResult
 *  \brief The partial result of a single test
 */
class PartResult
{
public:
  PartResult(const std::string& pFileName, int pLoC);

  PartResult(const std::string& pFileName, int pLoC,
             const std::string& pMessage);

  virtual ~PartResult() { }

  const std::string& filename() const { return m_FileName; }

  int lineNumber() const { return m_LoC; }

  const std::string& message() const { return m_Message; }

protected:
  void update_message(const std::string& pMessage) { m_Message = pMessage; }

protected:
  std::string m_FileName;
  int m_LoC;
  std::string m_Message;
};

/** \class TestPartResult
 *  \brief The partial result of a single test
 */
class TestPartResult : public PartResult
{
public:
  enum Type {
    kSuccess,          ///< Succeeded.
    kNonFatalFailure,  ///< Failed but the test can continue.
    kFatalFailure      ///< Failed and the test should be terminated.
  };

public:
  TestPartResult(Type pType, const std::string& pFileName,
                 int pLoC, const std::string& pMessage);

  Type type() const { return m_Type; }

  TestPartResult& appendUserMessage(const std::string& pMessage);

private:
  Type m_Type;
};

/** \class TestPerfPartResult
 *  \brief The performance result
 */
class PerfPartResult : public PartResult
{
public:
  PerfPartResult(const std::string& pFileName, int pLoC);

  Interval getTimerNum() const;
  Interval getPerfEventNum() const;
  Interval getPerfEventType() const;

  void setTimerNum(Interval pTime);
  void setPerfEventNum(Interval pEventNum);
  void setPerfEventType(Interval pEventType);

private:
  Interval m_PerfTimerNum;
  Interval m_PerfEventNum;
  Interval m_PerfEventType;
};

/** \class TestResult
 *  \brief The result of a single test.
 *
 *  TestResult concludes the result of a single test in summary.
 */
class TestResult : private skypat::testing::internal::Uncopyable
{
public:
  typedef std::vector<const TestPartResult*> Reliability;
  typedef std::vector<const PerfPartResult*> Performance;

public:
  enum Conclusion {
    kPassed,   ///< we pass all assertions
    kFailed,   ///< we fail some assertions
    kNotTested ///< we do not have any assertions
  };

public:
  TestResult(const TestInfo& pInfo);

  ~TestResult();

  bool isPassed() const;

  bool isFailed() const;

  Conclusion conclusion() const { return m_Conclusion; }

  void setConclusion(Conclusion pConclusion) { m_Conclusion = pConclusion; }

  const Performance& performance() const;

  const Reliability& reliability() const;

private:
  const TestInfo& m_Info;
  Conclusion m_Conclusion;
};

/** \class TestCase
 *  \brief The information of a test case (a set of tests)
 */
class TestCase
{
private:
  typedef std::vector<testing::TestInfo*> InfoList;

public:
  typedef InfoList::iterator iterator;
  typedef InfoList::const_iterator const_iterator;

  const_iterator begin() const { return m_InfoList.begin(); }
  iterator       begin()       { return m_InfoList.begin(); }
  const_iterator end()   const { return m_InfoList.end();   }
  iterator       end()         { return m_InfoList.end();   }

public:
  TestCase(const std::string& pCaseName);

  ~TestCase();

  testing::TestInfo* addTestInfo(const std::string& pTestName,
                                 testing::TestFactoryBase& pFactory);

  const std::string& getCaseName() const { return m_CaseName; }

  unsigned int getNumOfTests() const { return m_InfoList.size(); }

private:
  InfoList m_InfoList;
  std::string m_CaseName;
};

/** \class TestInfo
 *  \brief The information of a single test.
 *
 *  TestInfo stores the information of a single test. A test case contains
 *  multiple tests which is represented by TestInfos.
 *
 *  TestInfo is created at static time and gathers partial results at run-time.
 */
class TestInfo
{
public:
  TestInfo(TestCase* pTestCase,
           const std::string& pTestName,
           TestFactoryBase& pFactory);

  ~TestInfo();

  const TestCase* testCase() const { return m_pTestCase; }
  const std::string& getCaseName() const { return m_pTestCase->getCaseName(); }
  const std::string& getTestName() const { return m_TestName; }
  const TestResult& result() const { return m_Result; }

  /// run - run a single test function and notifiy repeater.
  void run();

  void addTestPartResult(const TestPartResult& pResult);
  PerfPartResult* addPerfPartResult(const char* pFile, int pLine);

private:
  friend class TestResult;

  typedef std::vector<const TestPartResult*> TestPartResultList;
  typedef std::vector<const PerfPartResult*> PerfPartResultList;

private:
  const TestPartResultList& getTestResults() const { return m_TestResultList; }
  TestPartResultList&       getTestResults()       { return m_TestResultList; }

  const PerfPartResultList& getPerfResults() const { return m_PerfResultList; }
  PerfPartResultList&       getPerfResults()       { return m_PerfResultList; }

private:
  TestCase* m_pTestCase;
  std::string m_TestName;
  TestResult m_Result;
  TestFactoryBase* m_pFactory;
  TestPartResultList m_TestResultList;
  PerfPartResultList m_PerfResultList;
};

/** \class AssertionResult
 *  \brief The result of an assertion.
 */
class AssertionResult
{
public:
  AssertionResult(const AssertionResult& other);

  explicit AssertionResult(bool pSuccess);

  operator bool() const { return m_bSuccess; }  // NOLINT

  AssertionResult operator!() const;

  const std::string& message() const { return m_Message; }

  template <typename T> AssertionResult& operator<<(const T& pValue);

  AssertionResult& operator<<(
      ::std::ostream& (*basic_manipulator)(::std::ostream& stream));

  bool hasMessage() const { return !m_Message.empty(); }

private:
  bool m_bSuccess;
  std::string m_Message;
};

/** \class Message
 *  \brief Message is a carrier to pass user-defined message to AssertionResult
 */
class Message
{
public:
  Message();

  template<typename T>
  inline Message& operator <<(const T& pValue) {
    m_OSS << pValue;
    return *this;
  }

  template<typename T>
  inline Message& operator <<(T* const& pPointer) {
    if (NULL == pPointer)
      m_OSS << "(null)";
    else
      m_OSS << pPointer;
    return *this;
  }

  const std::string& str() const { return m_Message; }

private:
  std::string m_Message;
  OStrStream m_OSS;
};

/** \class AssertHelper
 *  \brief AssertHelper carries all information to UnitTest.
 */
class AssertHelper
{
public:
  AssertHelper(TestPartResult::Type pType,
               const std::string& pFile,
               int pLineOfCode,
               const std::string& pMessage);

  // Message assignment is a semantic trick to enable assertion
  // streaming; see the SKYPAT_MESSAGE_AT macro below.
  // This method may update the messages stored in TestPartResult.
  void operator=(const Message& pMesg);

private:
  TestPartResult m_Result;
};

TestInfo* MakeAndRegisterTestInfo(
    const char* pCaseName,
    const char* pTestName,
    TestFactoryBase* pFactory);

std::string GetBoolAssertionFailureMessage(
    const AssertionResult& pAssertionResult,
    const char* pExpressionText,
    const char* pActualPredicateValue,
    const char* pExpectedPredicateValue);

template<typename T1, typename T2>
std::string GetPredAssertionFailureMessage(
    const AssertionResult& pAssertionResult,
    const char* pExpressionText,
    const T1& pActualPredicateValue,
    const char* pActualPredicate,
    const T2& pExpectedPredicateValue,
    const char* pExpectedPredicate)
{
  std::string result;
  OStrStream OS(result);
  OS << "Value of: " << pExpressionText
     << "\n  Actual:   " << pActualPredicateValue;
  if (pAssertionResult.hasMessage())
    OS << "(" << pAssertionResult.message() << ")";
  OS << "\n  Expected: " << pExpectedPredicateValue;
  return result;
}
//===----------------------------------------------------------------------===//
// Listener
//===----------------------------------------------------------------------===//
/** \class Listener
 *  \brief Listener provides interfaces for objects who wants UnitTest's events.
 */
class Listener
{
public:
  virtual ~Listener() { }

  // Fired before any test activity starts.
  virtual void OnTestProgramStart(const testing::UnitTest& pUnitTest) {}

  // Fired before each test case (a set of tests) of test start.
  virtual void OnTestCaseStart(const TestCase& pTestCase) {}

  // Fired before set-up for each iteration of test start.
  virtual void OnSetUpStart(const UnitTest& pUnitTest) {}

  // Fired before set-up for each iteration of test end.
  virtual void OnSetUpEnd(const UnitTest& pUnitTest) {}

  // Fired before the test starts.
  virtual void OnTestStart(const TestInfo& pTestInfo) {}

  // Fired after a failed assertion or a SUCCEED() invocation.
  virtual void OnTestPartResult(const TestPartResult& pTestPartResult) {}

  // Fired after a PERFORM invocation
  virtual void OnPerfPartResult(const PerfPartResult& pPerfPartResult) {}

  // Fired after the test ends.
  virtual void OnTestEnd(const TestInfo& pTestInfo) {}

  // Fired before environment tear-down for each iteration of tests starts.
  virtual void OnTearDownStart(const UnitTest& pUnitTest) {}

  // Fired after environment tear-down for each iteration of tests ends.
  virtual void OnTearDownEnd(const UnitTest& pUnitTest) {}

  // Fired after each test case (a set of tests) of test ends.
  virtual void OnTestCaseEnd(const TestCase& pTestCase) {}

  // Fired after all test activities have ended.
  virtual void OnTestProgramEnd(const UnitTest& pUnitTest) {}
};

class Log
{
public:
  enum Severity {
    kInfo,
    kWarning,
    kError,
    kFatal
  };

public:
  Log(Severity pSeverity, const std::string& pFileName, int pLoC);

  /// Destructor. Flush the buffers and if the Severity is fatal, aborts the
  /// program.
  ~Log();

  static ::std::ostream& getOStream();

  static std::string FormatFileLocation(const std::string& pFileName, int pLoC);

private:
  Severity m_Severity;
};

//===----------------------------------------------------------------------===//
// Repeater
//===----------------------------------------------------------------------===//
/** \class Repeater
 *  \brief Repeater dispatches events to all listeners.
 */
class Repeater : public Listener
{
public:
  typedef std::vector<Listener*> ListenerList;

public:
  Repeater();

  ~Repeater();

  void add(Listener* pListener);

  void release(Listener& pListener);

  bool isForward() const { return m_bForward; }

  void setForward(bool pEnable = true) { m_bForward = pEnable; }

  void OnTestProgramStart(const testing::UnitTest& pUnitTest);
  void OnTestCaseStart(const TestCase& pTestCase);
  void OnSetUpStart(const UnitTest& pUnitTest);
  void OnSetUpEnd(const UnitTest& pUnitTest);
  void OnTestStart(const TestInfo& pTestInfo);
  void OnTestPartResult(const TestPartResult& pTestPartResult);
  void OnPerfPartResult(const PerfPartResult& pPerfPartResult);
  void OnTestEnd(const TestInfo& pTestInfo);
  void OnTearDownStart(const UnitTest& pUnitTest);
  void OnTearDownEnd(const UnitTest& pUnitTest);
  void OnTestCaseEnd(const TestCase& pTestCase);
  void OnTestProgramEnd(const UnitTest& pUnitTest);

private:
  ListenerList m_Listeners;
  bool m_bForward;
};

//===----------------------------------------------------------------------===//
// UnitTest
//===----------------------------------------------------------------------===//
class UnitTest
{
private:
  typedef std::map<std::string, testing::TestCase*> CaseMap;

  // RunCases stores all runnable cases
  typedef std::vector<testing::TestCase*> RunCases;

public:
  static UnitTest* self() {
    static UnitTest instance;
    return &instance;
  }

  void RunAll();

  /// addRunCase - add the test case to run
  /// @return true if the case exists (be stored at static-time).
  bool addRunCase(const std::string& pCaseName);

  /// addAllRunCase - add all test cases to run
  void addAllRunCases();

  /// addTestInfo - store a TestInfo at static-time
  testing::TestInfo* addTestInfo(const std::string& pCaseName,
                                 const std::string& pTestName,
                                 testing::TestFactoryBase& pFactory);

  /// addTestPartResult - add partial test result at run-time.
  void addTestPartResult(const testing::TestPartResult& pPartResult);

  /// addPerfPartResult - add partial performance result at run-time.
  testing::PerfPartResult* addPerfPartResult(const char* pFile, int pLine);

  const Repeater& repeater() const { return m_Repeater; }
  Repeater&       repeater()       { return m_Repeater; }

  unsigned int getNumOfCases() const { return m_CaseMap.size(); }
  unsigned int getNumOfTests() const { return m_NumOfTests; }
  unsigned int getNumOfFails() const { return m_NumOfFails; }

  unsigned int getNumOfRunCases() const { return m_RunCases.size(); }

private:
  UnitTest();
  ~UnitTest();
  UnitTest(const UnitTest& pCopy); // DO NOT IMPLEMENT
  UnitTest& operator=(const UnitTest& pCopy); // DO NOT IMPLEMENT

private:
  CaseMap m_CaseMap;
  RunCases m_RunCases;
  testing::Repeater m_Repeater;
  testing::TestInfo* m_pCurrentInfo;
  unsigned int m_NumOfTests;
  unsigned int m_NumOfFails;
};

} // namespace of testing

/** \class Test
 *  \brief Test is the abstract class that all tests inherit from.
 *
 *  In skypat, a performance test program contains one or many TestCases, and
 *  each TestCase contains one or many Tests.
 *
 *  When you define a test using the TEST macro, you don't need to explicitly
 *  derive from Test - the PERFORM macro automatically does this for you.
 *
 *  The only one time you derive from Test is when defining a test fixture
 *  to be used a PERFORM_F. For example:
 *
 *  class FooTest : public skypat::Test
 *  {
 *  public:
 *    virtual void SetUp() { ... }
 *    virtual void TearDown() { ... }
 *    ...
 *  };
 *
 *  SKYPAT_C( FooTest, Bar1) { ... }
 *  SKYPAT_C( FooTest, Bar2) { ... }
 */
class Test : private skypat::testing::internal::Uncopyable
{
friend class skypat::testing::TestInfo;
private:
  void run();

public:
  /// Destructor
  virtual ~Test() = 0;

  /// SetUp - set up the test fixture.
  virtual void SetUp() { }

  /// TearDown - tears down the test fixture.
  virtual void TearDown() { }

public:
  /// @name Static Member Functions
  /// @{
  /// Initialize - initialize environment for TestCases.
  static void Initialize(const int& pArgc, char* pArgv[]);

  /// Initialize - use the pretty printer
  /// @param[out] pProgName The program name.
  static void Initialize(const std::string& pProgName);

  /// Initialize - initialize environment for TestCases.
  /// @param[out] pProgName The program name.
  /// @param[out] pCSVResult The resulting CSV file.
  static void
  Initialize(const std::string& pProgName, const std::string& pCSVResult);

  /// RunAll - run all TestCases.
  static void RunAll();

  /// Sleep - sleep for micro seconds
  static void Sleep(int pMS);
  /// @}

  virtual void TestBody() = 0;
};

// Defines a test that uses a test fixture.
//
// SKYPAT_C defines a skypat case.
// SKYPAT_F defines a skypat function.
//
// The first parameter is the name of the test fixture class, which
// also doubles as the test case name.  The second parameter is the
// name of the test within the test case.
//
// A test fixture class must be declared earlier.  The user should put
// his test code between braces after using this macro.  Example:
//
//   class FooTest : public skypat::Test {
//    protected:
//     virtual void SetUp() { m_B.AddElement(3); }
//
//     Foo m_A;
//     Foo m_B;
//   };
//
//   SKYPAT_C(FooTest, Initializes) {
//     m_A.StatusIsOK();
//   }
//
#define SKYPAT_C(test_fixture, test_name) \
  SKYPAT_TEST_CASE(test_fixture, test_name, test_fixture)

#define SKYPAT_F(test_fixture, test_name) \
  SKYPAT_TEST_CASE(test_fixture, test_name, skypat::Test)

// Boolean assertions.
#define EXPECT_TRUE(condition) \
  SKYPAT_TEST_BOOLEAN(condition, #condition, false, true, \
                      SKYPAT_NONFATAL_FAILURE)
#define EXPECT_FALSE(condition) \
  SKYPAT_TEST_BOOLEAN(!(condition), #condition, true, false, \
                      SKYPAT_NONFATAL_FAILURE)
#define ASSERT_TRUE(condition) \
  SKYPAT_TEST_BOOLEAN(condition, #condition, false, true, \
                      SKYPAT_FATAL_FAILURE)
#define ASSERT_FALSE(condition) \
  SKYPAT_TEST_BOOLEAN(!(condition), #condition, true, false, \
                      SKYPAT_FATAL_FAILURE)

// Boolean assertions.
#define EXPECT_TRUE_MSG(condition, msg) \
  SKYPAT_TEST_BOOLEAN(condition, #condition, false, true, \
                      SKYPAT_NONFATAL_FAILURE) << msg
#define EXPECT_FALSE_MSG(condition, msg) \
  SKYPAT_TEST_BOOLEAN(!(condition), #condition, true, false, \
                      SKYPAT_NONFATAL_FAILURE) << msg
#define ASSERT_TRUE_MSG(condition, msg) \
  SKYPAT_TEST_BOOLEAN(condition, #condition, false, true, \
                      SKYPAT_FATAL_FAILURE) << msg
#define ASSERT_FALSE_MSG(condition, msg) \
  SKYPAT_TEST_BOOLEAN(!(condition), #condition, true, false, \
                      SKYPAT_FATAL_FAILURE) << msg

#define SKYPAT_EXPECT_PRED(condition, actual, expected) \
  SKYPAT_TEST_PREDICATE(condition, #condition, \
                     actual, expected, \
                     SKYPAT_NONFATAL_FAILURE)

#define SKYPAT_ASSERT_PRED(condition, actual, expected) \
  SKYPAT_TEST_PREDICATE(condition, #condition, \
                     actual, expected, \
                     SKYPAT_FATAL_FAILURE)

#define EXPECT_EQ(actual, expected) \
  SKYPAT_EXPECT_PRED((actual == expected), actual, expected)
#define EXPECT_NE(actual, expected) \
  SKYPAT_EXPECT_PRED((actual != expected), actual, expected)
#define EXPECT_LE(actual, expected) \
  SKYPAT_EXPECT_PRED((actual <= expected), actual, expected)
#define EXPECT_LT(actual, expected) \
  SKYPAT_EXPECT_PRED((actual < expected), actual, expected)
#define EXPECT_GE(actual, expected) \
  SKYPAT_EXPECT_PRED((actual >= expected), actual, expected)
#define EXPECT_GT(actual, expected) \
  SKYPAT_EXPECT_PRED((actual > expected), actual, expected)

#define ASSERT_EQ(actual, expected) \
  SKYPAT_ASSERT_PRED((actual == expected), actual, expected)
#define ASSERT_NE(actual, expected) \
  SKYPAT_ASSERT_PRED((actual != expected), actual, expected)
#define ASSERT_LE(actual, expected) \
  SKYPAT_ASSERT_PRED((actual <= expected), actual, expected)
#define ASSERT_LT(actual, expected) \
  SKYPAT_ASSERT_PRED((actual < expected), actual, expected)
#define ASSERT_GE(actual, expected) \
  SKYPAT_ASSERT_PRED((actual >= expected), actual, expected)
#define ASSERT_GT(actual, expected) \
  SKYPAT_ASSERT_PRED((actual > expected), actual, expected)

#define EXPECT_EQ_MSG(actual, expected, mesg) \
  SKYPAT_EXPECT_PRED((actual == expected), actual, expected) << mesg
#define EXPECT_NE_MSG(actual, expected, mesg) \
  SKYPAT_EXPECT_PRED((actual != expected), actual, expected) << mesg
#define EXPECT_LE_MSG(actual, expected, mesg) \
  SKYPAT_EXPECT_PRED((actual <= expected), actual, expected) << mesg
#define EXPECT_LT_MSG(actual, expected, mesg) \
  SKYPAT_EXPECT_PRED((actual < expected), actual, expected) << mesg
#define EXPECT_GE_MSG(actual, expected, mesg) \
  SKYPAT_EXPECT_PRED((actual >= expected), actual, expected) << mesg
#define EXPECT_GT_MSG(actual, expected, mesg) \
  SKYPAT_EXPECT_PRED((actual > expected), actual, expected) << mesg

#define ASSERT_EQ_MSG(actual, expected, mesg) \
  SKYPAT_ASSERT_PRED((actual == expected), actual, expected) << mesg
#define ASSERT_NE_MSG(actual, expected, mesg) \
  SKYPAT_ASSERT_PRED((actual != expected), actual, expected) << mesg
#define ASSERT_LE_MSG(actual, expected, mesg) \
  SKYPAT_ASSERT_PRED((actual <= expected), actual, expected) << mesg
#define ASSERT_LT_MSG(actual, expected, mesg) \
  SKYPAT_ASSERT_PRED((actual < expected), actual, expected) << mesg
#define ASSERT_GE_MSG(actual, expected, mesg) \
  SKYPAT_ASSERT_PRED((actual >= expected), actual, expected) << mesg
#define ASSERT_GT_MSG(actual, expected, mesg) \
  SKYPAT_ASSERT_PRED((actual > expected), actual, expected) << mesg

#define PERFORM(event) \
  for (skypat::testing::PerfIterator __loop(__FILE__, __LINE__, event); \
                                                __loop.hasNext(); \
                                                __loop.next() )

} // namespace of skypat

#endif
