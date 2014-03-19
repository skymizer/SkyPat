//===- pat.h ----------------------------------------------------------===//
//
//                     The pat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#ifndef PAT_PAT_H
#define PAT_PAT_H
#include <stdint.h>
#include <string>
#include <vector>
#include <map>
#include <pat/Support/OStrStream.h>

#define PAT_VERNUM 0x24

#define PAT_PUBLIC __attribute__ ((visibility ("default")))
#define PAT_LOCAL  __attribute__ ((visibility ("hidden")))


namespace pat {
class Test;

namespace testing {
namespace internal {
class Timer;

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
#define PAT_TEST_CLASS_NAME_(case_name, test_name) \
  case_name##_##test_name##_Test

/// Helper macro for defining Cases.
#define PAT_TEST_CASE(case_name, test_name, parent_class)\
class PAT_TEST_CLASS_NAME_(case_name, test_name) : public parent_class {\
 public:\
  PAT_TEST_CLASS_NAME_(case_name, test_name)() {}\
 private:\
  virtual void TestBody();\
  static pat::testing::TestInfo* const m_TestInfo;\
};\
\
pat::testing::TestInfo* const PAT_TEST_CLASS_NAME_(case_name, test_name)\
  ::m_TestInfo =\
    pat::testing::MakeAndRegisterTestInfo(\
        #case_name, #test_name, \
        new pat::testing::TestFactory<\
            PAT_TEST_CLASS_NAME_(case_name, test_name)>);\
void PAT_TEST_CLASS_NAME_(case_name, test_name)::TestBody()

// The message handling macros.
#define PAT_MESSAGE_AT(file, line, message, result_type) \
  pat::testing::AssertHelper(result_type, file, line, message) = true\

#define PAT_MESSAGE(message, result_type) \
  PAT_MESSAGE_AT(__FILE__, __LINE__, message, result_type)

#define PAT_FATAL_FAILURE(message) \
  return PAT_MESSAGE(message, pat::testing::TestPartResult::kFatalFailure)

#define PAT_NONFATAL_FAILURE(message) \
  PAT_MESSAGE(message, pat::testing::TestPartResult::kNonFatalFailure)

#define PAT_SUCCESS(message) \
  PAT_MESSAGE(message, pat::testing::TestPartResult::kSuccess)

// The GNU compiler emits a warning if nested "if" statements are followed by
// an "else" statement and braces are not used to explicitly disambiguate the
// "else" binding.
#ifdef __INTEL_COMPILER
# define PAT_UNAMBIGUOUS_ELSE_BLOCKER
#else
# define PAT_UNAMBIGUOUS_ELSE_BLOCKER switch (0) case 0: default:  // NOLINT
#endif

// Implements Boolean test assertions such as EXPECT_TRUE. expression can be
// either a boolean expression or an AssertionResult. text is a textual
// represenation of expression as it was passed into the EXPECT_TRUE.
#define PAT_TEST_BOOLEAN(expression, text, actual, expected, fail) \
  PAT_UNAMBIGUOUS_ELSE_BLOCKER \
  if (const pat::testing::AssertionResult _ar_ = \
      pat::testing::AssertionResult(expression)) \
    PAT_SUCCESS(""); \
  else \
    fail(pat::testing::GetBoolAssertionFailureMessage(\
        _ar_, text, #actual, #expected))

// Implements Predicate test assertions such as EXPECT_EQ.
#define PAT_TEST_PREDICATE(expression, text, actual, expected, fail) \
  PAT_UNAMBIGUOUS_ELSE_BLOCKER \
  if (const pat::testing::AssertionResult _ar_ = \
      pat::testing::AssertionResult(expression)) \
    PAT_SUCCESS(""); \
  else \
    fail(pat::testing::GetPredAssertionFailureMessage(\
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
  virtual pat::Test* CreateTest() = 0;
};

template<typename SingleTest> struct TestFactory : public TestFactoryBase {
  virtual pat::Test* CreateTest() { return new SingleTest; }
};

/** \class PerfIterator
 *  \brief PerfIterator is used to calculate the computing time of a
 *  performance test.
 */
class PAT_PUBLIC PerfIterator
{
public:
  /// @param pFileName the source file name.
  /// @param pLoC the line of code.
  PerfIterator(const char* pFileName, int pLoC);

  /// Destructor. The place to sum up the time.
  ~PerfIterator();

  /// increase counter
  /// @return true if we should go to the next step.
  bool next();

private:
  int m_Counter;
  internal::Timer* m_pTimer;
  PerfPartResult* m_pPerfResult;
};

/** \class PartResult
 *  \brief The partial result of a single test
 */
class PAT_PUBLIC PartResult
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
class PAT_PUBLIC TestPartResult : public PartResult
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

private:
  Type m_Type;
};

/** \class TestPerfPartResult
 *  \brief The performance result
 */
class PAT_PUBLIC PerfPartResult : public PartResult
{
public:
  PerfPartResult(const std::string& pFileName, int pLoC);

  Interval getPerformance() const;

  void setPerformance(Interval pNum);

private:
  Interval m_PerfNum;
};

/** \class TestResult
 *  \brief The result of a single test.
 *
 *  TestResult concludes the result of a single test in summary.
 */
class PAT_PUBLIC TestResult : private pat::testing::internal::Uncopyable
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
class PAT_PUBLIC TestCase
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

  void addTestInfo(const std::string& pTestName,
                   testing::TestFactoryBase& pFactory);

  const std::string& getCaseName() const { return m_CaseName; }

  unsigned int getNumOfTests() const { return m_InfoList.size(); }

private:
  InfoList m_InfoList;
  std::string m_CaseName;
};

/** \class TestInfo
 *  \brief The information of a single test
 *
 *  TestInfo stores the information of a single test. A test case contains
 *  multiple tests which is represented by TestInfos.
 */
class PAT_PUBLIC TestInfo
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
class PAT_PUBLIC AssertionResult
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

/** \class AssertHelper
 *  \brief AssertHelper carries all information to UnitTest.
 */
class PAT_PUBLIC AssertHelper
{
public:
  AssertHelper(TestPartResult::Type pType,
               const std::string& pFile,
               int pLineOfCode,
               const std::string& pMessage);

  // Message assignment is a semantic trick to enable assertion
  // streaming; see the PAT_MESSAGE_AT macro below.
  void operator=(bool pValue) const;

private:
  TestPartResult m_Result;
};

PAT_PUBLIC
TestInfo* MakeAndRegisterTestInfo(
    const char* pCaseName,
    const char* pTestName,
    TestFactoryBase* pFactory);

PAT_PUBLIC
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
class PAT_PUBLIC Listener
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

class PAT_PUBLIC Log
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
class PAT_PUBLIC Repeater : public Listener
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
class PAT_PUBLIC UnitTest
{
private:
  // TODO: Performance
  typedef std::map<std::string, testing::TestCase*> CaseMap;

public:
  static UnitTest* self() {
    static UnitTest instance;
    return &instance;
  }

  void RunAll();

  void addTestInfo(const std::string& pCaseName,
                   const std::string& pTestName,
                   testing::TestFactoryBase& pFactory);

  void addTestPartResult(const testing::TestPartResult& pPartResult);
  testing::PerfPartResult* addPerfPartResult(const char* pFile, int pLine);

  const testing::TestInfo* getCurrentInfo() const { return m_pCurrentInfo; }
  testing::TestInfo*       getCurrentInfo()       { return m_pCurrentInfo; }

  void setCurrentInfo(testing::TestInfo& pInfo) { m_pCurrentInfo = &pInfo; }

  const Repeater& repeater() const { return m_Repeater; }
  Repeater&       repeater()       { return m_Repeater; }

  unsigned int getNumOfCases() const { return m_CaseMap.size(); }
  unsigned int getNumOfTests() const { return m_NumOfTests; }
  unsigned int getNumOfFails() const { return m_NumOfFails; }

private:
  UnitTest();
  ~UnitTest();
  UnitTest(const UnitTest& pCopy); // DO NOT IMPLEMENT
  UnitTest& operator=(const UnitTest& pCopy); // DO NOT IMPLEMENT

private:
  CaseMap m_CaseMap;
  testing::Repeater m_Repeater;
  testing::TestInfo* m_pCurrentInfo;
  unsigned int m_NumOfTests;
  unsigned int m_NumOfFails;
};

} // namespace of testing

/** \class Test
 *  \brief Test is the abstract class that all tests inherit from.
 *
 *  In pat, a performance test program contains one or many TestCases, and
 *  each TestCase contains one or many Tests.
 *
 *  When you define a test using the TEST macro, you don't need to explicitly
 *  derive from Test - the PERFORM macro automatically does this for you.
 *
 *  The only one time you derive from Test is when defining a test fixture
 *  to be used a PERFORM_F. For example:
 *
 *  class FooTest : public pat::Test
 *  {
 *  public:
 *    virtual void SetUp() { ... }
 *    virtual void TearDown() { ... }
 *    ...
 *  };
 *
 *  PAT_C( FooTest, Bar1) { ... }
 *  PAT_C( FooTest, Bar2) { ... }
 */
class PAT_PUBLIC Test : private pat::testing::internal::Uncopyable
{
friend class pat::testing::TestInfo;
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
  static void Initialize(int* pArgc, char* pArgv[]);

  /// RunAll - run all TestCases.
  static void RunAll();

  /// Sleep - sleep for micro seconds
  static void Sleep(int pMS);
  /// @}

  virtual void TestBody() = 0;
};

// Defines a test that uses a test fixture.
//
// PAT_C defines a pat case.
// PAT_F defines a pat function.
//
// The first parameter is the name of the test fixture class, which
// also doubles as the test case name.  The second parameter is the
// name of the test within the test case.
//
// A test fixture class must be declared earlier.  The user should put
// his test code between braces after using this macro.  Example:
//
//   class FooTest : public pat::Test {
//    protected:
//     virtual void SetUp() { m_B.AddElement(3); }
//
//     Foo m_A;
//     Foo m_B;
//   };
//
//   PAT_C(FooTest, Initializes) {
//     m_A.StatusIsOK();
//   }
//
#define PAT_C(test_fixture, test_name) \
  PAT_TEST_CASE(test_fixture, test_name, test_fixture)

#define PAT_F(test_fixture, test_name) \
  PAT_TEST_CASE(test_fixture, test_name, pat::Test)

// Boolean assertions.
#define EXPECT_TRUE(condition) \
  PAT_TEST_BOOLEAN(condition, #condition, false, true, \
                      PAT_NONFATAL_FAILURE)
#define EXPECT_FALSE(condition) \
  PAT_TEST_BOOLEAN(!(condition), #condition, true, false, \
                      PAT_NONFATAL_FAILURE)
#define ASSERT_TRUE(condition) \
  PAT_TEST_BOOLEAN(condition, #condition, false, true, \
                      PAT_FATAL_FAILURE)
#define ASSERT_FALSE(condition) \
  PAT_TEST_BOOLEAN(!(condition), #condition, true, false, \
                      PAT_FATAL_FAILURE)

#define PAT_EXPECT_PRED(condition, actual, expected) \
  PAT_TEST_PREDICATE(condition, #condition, \
                     actual, expected, \
                     PAT_NONFATAL_FAILURE)

#define PAT_ASSERT_PRED(condition, actual, expected) \
  PAT_TEST_PREDICATE(condition, #condition, \
                     actual, expected, \
                     PAT_FATAL_FAILURE)

#define EXPECT_EQ(actual, expected) \
  PAT_EXPECT_PRED((actual == expected), actual, expected)
#define EXPECT_NE(actual, expected) \
  PAT_EXPECT_PRED((actual != expected), actual, expected)
#define EXPECT_LE(actual, expected) \
  PAT_EXPECT_PRED((actual <= expected), actual, expected)
#define EXPECT_LT(actual, expected) \
  PAT_EXPECT_PRED((actual < expected), actual, expected)
#define EXPECT_GE(actual, expected) \
  PAT_EXPECT_PRED((actual >= expected), actual, expected)
#define EXPECT_GT(actual, expected) \
  PAT_EXPECT_PRED((actual > expected), actual, expected)

#define ASSERT_EQ(actual, expected) \
  PAT_ASSERT_PRED((actual == expected), actual, expected)
#define ASSERT_NE(actual, expected) \
  PAT_ASSERT_PRED((actual != expected), actual, expected)
#define ASSERT_LE(actual, expected) \
  PAT_ASSERT_PRED((actual <= expected), actual, expected)
#define ASSERT_LT(actual, expected) \
  PAT_ASSERT_PRED((actual < expected), actual, expected)
#define ASSERT_GE(actual, expected) \
  PAT_ASSERT_PRED((actual >= expected), actual, expected)
#define ASSERT_GT(actual, expected) \
  PAT_ASSERT_PRED((actual > expected), actual, expected)

#define PERFORM for(pat::testing::PerfIterator __loop(__FILE__, __LINE__) \
                        ; __loop.next() ; )

} // namespace of pat

#endif
