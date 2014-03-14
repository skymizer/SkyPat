//===- pat.cpp --------------------------------------------------------===//
//
//                     The pat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#include <pat/pat.h>
#include <pat/Support/Timer.h>
#include <pat/Support/ManagedStatic.h>
#include <pat/Support/OStrStream.h>
#include <vector>
#include <cassert>
#include <cstdlib>
#include <ios>
#include <ostream>
#include <iostream>
#include <stdint.h>
#include <algorithm>
#include <cstdio>
#include <time.h>

#include <pat/Config/Config.h>

using namespace pat;

/* Define the numebr of iteration of performance loop */
#define PAT_PERFORM_LOOP_TIMES 1

//===----------------------------------------------------------------------===//
// Details of UnitTest
//===----------------------------------------------------------------------===//
testing::UnitTest::UnitTest()
  : m_pCurrentInfo(NULL), m_NumOfTests(0), m_NumOfFails(0) {
}

testing::UnitTest::~UnitTest()
{
  CaseMap::iterator iCase, iEnd = m_CaseMap.end();
  for (iCase = m_CaseMap.begin(); iCase != iEnd; ++iCase) {
    delete iCase->second;
  }
}

void testing::UnitTest::addTestInfo(const std::string& pCaseName,
                                    const std::string& pTestName,
                                    testing::TestFactoryBase& pFactory)
{
  CaseMap::iterator iCase = m_CaseMap.find(pCaseName);
  TestCase* test_case;
  if (iCase != m_CaseMap.end())
    test_case = iCase->second;
  else {
    test_case = new TestCase(pCaseName);
    m_CaseMap.insert(make_pair(pCaseName, test_case));
  }
  test_case->addTestInfo(pTestName, pFactory);
  ++m_NumOfTests;
}

void testing::UnitTest::addTestPartResult(const TestPartResult& pPartResult)
{
  m_pCurrentInfo->addTestPartResult(pPartResult);
  m_Repeater.OnTestPartResult(pPartResult);
  if (testing::TestPartResult::kSuccess != pPartResult.type())
    ++m_NumOfFails;
}

testing::PerfPartResult*
testing::UnitTest::addPerfPartResult(const char* pFile, int pLine)
{
  /* XXX: */
  m_Repeater.OnPerfPartResult(testing::PerfPartResult(pFile, pLine));
  return m_pCurrentInfo->addPerfPartResult(pFile, pLine);
}

void testing::UnitTest::RunAll()
{
  m_Repeater.OnTestProgramStart(*this);

  CaseMap::iterator iCase, iEnd = m_CaseMap.end();
  for (iCase = m_CaseMap.begin(); iCase != iEnd; ++iCase) {
    TestCase* test_case = iCase->second;
    m_Repeater.OnTestCaseStart(*test_case);

    TestCase::iterator it   = test_case->begin();
    TestCase::iterator iEnd = test_case->end();
    while (it != iEnd) {
      (*it)->run();
      ++it;
    }

    m_Repeater.OnTestCaseEnd(*test_case);
  }

  m_Repeater.OnTestProgramEnd(*this);
}

//===----------------------------------------------------------------------===//
// Non-member function
//===----------------------------------------------------------------------===//
testing::TestInfo*
testing::MakeAndRegisterTestInfo(const char* pCaseName, const char* pTestName,
                                 testing::TestFactoryBase* pFactory)
{
  testing::UnitTest::self()->addTestInfo(pCaseName, pTestName, *pFactory);
  return testing::UnitTest::self()->getCurrentInfo();
}

std::string testing::GetBoolAssertionFailureMessage(
    const pat::testing::AssertionResult& pAssertionResult,
    const char* pExpressionText,
    const char* pActualPredicateValue,
    const char* pExpectedPredicateValue)
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
// PerfIterator
//===----------------------------------------------------------------------===//
testing::PerfIterator::PerfIterator(const char* pFile, int pLine)
  : m_Counter(-1) // -1 for the for-loop initial test.
{
  m_pPerfResult = testing::UnitTest::self()->addPerfPartResult(pFile, pLine);
  m_pTimer = new internal::Timer();
  m_pTimer->start();
}

testing::PerfIterator::~PerfIterator()
{
  m_pTimer->stop();
  m_pPerfResult->setPerformance(m_pTimer->interval());
  delete m_pTimer;
}

bool testing::PerfIterator::next()
{
  ++m_Counter;
  if (m_Counter >= PAT_PERFORM_LOOP_TIMES) {
    return false;
  }
  return true;
}

//===----------------------------------------------------------------------===//
// PartResult
//===----------------------------------------------------------------------===//
testing::PartResult::PartResult(const std::string& pFileName, int pLoC)
  : m_FileName(pFileName), m_LoC(pLoC), m_Message() {
}

testing::PartResult::PartResult(const std::string& pFileName,
                                int pLoC,
                                const std::string& pMessage)
  : m_FileName(pFileName), m_LoC(pLoC), m_Message(pMessage) {
}

//===----------------------------------------------------------------------===//
// TestPartResult
//===----------------------------------------------------------------------===//
testing::TestPartResult::TestPartResult(Type pType,
                                        const std::string& pFileName,
                                        int pLoC,
                                        const std::string& pMessage)
  : PartResult(pFileName, pLoC, pMessage), m_Type(pType) {
}

//===----------------------------------------------------------------------===//
// PerfPartResult
//===----------------------------------------------------------------------===//
testing::PerfPartResult::PerfPartResult(const std::string& pFileName,
                                        int pLoC)
  : PartResult(pFileName, pLoC) {
}

testing::Interval testing::PerfPartResult::getPerformance() const
{
  return m_PerfNum;
}

void testing::PerfPartResult::setPerformance(testing::Interval pNum)
{
  m_PerfNum = pNum;
  OStrStream os(m_Message);
  os << pNum << " ns";
}

//===----------------------------------------------------------------------===//
// TestResult
//===----------------------------------------------------------------------===//
testing::TestResult::TestResult(const TestInfo& pInfo)
  : m_Info(pInfo), m_Conclusion(kNotTested) {
}

testing::TestResult::~TestResult()
{
}

bool testing::TestResult::isPassed() const
{
  return (kPassed == m_Conclusion);
}

bool testing::TestResult::isFailed() const
{
  return (kFailed == m_Conclusion);
}

const testing::TestResult::Reliability& testing::TestResult::reliability() const
{
  return m_Info.getTestResults();
}

const testing::TestResult::Performance& testing::TestResult::performance() const
{
  return m_Info.getPerfResults();
}

//===----------------------------------------------------------------------===//
// TestCase
//===----------------------------------------------------------------------===//
testing::TestCase::TestCase(const std::string& pCaseName)
  : m_CaseName(pCaseName)
{
}

testing::TestCase::~TestCase()
{
  InfoList::iterator info, iEnd = m_InfoList.end();
  for (info = m_InfoList.begin(); info != iEnd; ++info) {
    delete (*info);
  }
}

void
testing::TestCase::addTestInfo(const std::string& pTestName,
                               testing::TestFactoryBase& pFactory)
{
  m_InfoList.push_back(new testing::TestInfo(this, pTestName, pFactory));
}

//===----------------------------------------------------------------------===//
// TestInfo
//===----------------------------------------------------------------------===//
testing::TestInfo::TestInfo(TestCase* pTestCase,
                            const std::string& pTestName,
                            testing::TestFactoryBase& pFactory)
  : m_pTestCase(pTestCase),
    m_TestName(pTestName),
    m_Result(*this),
    m_pFactory(&pFactory) {
}

testing::TestInfo::~TestInfo()
{
  delete m_pFactory;
  TestPartResultList::iterator tt, tEnd = m_TestResultList.end();
  for (tt = m_TestResultList.begin(); tt != tEnd; ++tt) {
    delete (*tt);
  }
  PerfPartResultList::iterator pt, pEnd = m_PerfResultList.end();
  for (pt = m_PerfResultList.begin(); pt != pEnd; ++pt) {
    delete (*pt);
  }
}

void testing::TestInfo::run()
{
  UnitTest& unittest = *UnitTest::self();
  Repeater& repeater = unittest.repeater();
  unittest.setCurrentInfo(*this);
  pat::Test* test = m_pFactory->CreateTest();
  if (NULL != test) {
    repeater.OnSetUpStart(unittest);
    test->SetUp();
    repeater.OnSetUpEnd(unittest);

    repeater.OnTestStart(*this);
    test->run();
    repeater.OnTestEnd(*this);

    repeater.OnTearDownStart(unittest);
    test->TearDown();
    repeater.OnTearDownEnd(unittest);
  }
  delete test;
}

void testing::TestInfo::addTestPartResult(const TestPartResult& pResult)
{
  if (m_TestResultList.empty()) {
    m_Result.setConclusion(testing::TestResult::kPassed);
  }

  if (testing::TestPartResult::kSuccess != pResult.type()) {
    m_Result.setConclusion(testing::TestResult::kFailed);
    m_TestResultList.push_back(new TestPartResult(pResult));
  }
}

testing::PerfPartResult*
testing::TestInfo::addPerfPartResult(const char* pFile, int pLine)
{
  PerfPartResult* perf_pr = new PerfPartResult(pFile, pLine);
  m_PerfResultList.push_back(perf_pr);
  return perf_pr;
}

//===----------------------------------------------------------------------===//
// AssertionResult
//===----------------------------------------------------------------------===//
pat::testing::AssertionResult::AssertionResult(const AssertionResult& pOther)
  : m_bSuccess(pOther.m_bSuccess), m_Message(pOther.m_Message) {
}

pat::testing::AssertionResult::AssertionResult(bool pSuccess)
  : m_bSuccess(pSuccess) {
}

pat::testing::AssertionResult
pat::testing::AssertionResult::operator!() const
{
  AssertionResult negative(!m_bSuccess);
  negative << m_Message;
  return negative;
}

template <typename T> pat::testing::AssertionResult&
pat::testing::AssertionResult::operator<<(const T& pValue)
{
  pat::OStrStream OS(m_Message);
  OS << pValue;
  return *this;
}

pat::testing::AssertionResult&
pat::testing::AssertionResult::operator<<(
                  ::std::ostream& (*basic_manipulator)(::std::ostream& stream))
{
  pat::OStrStream OS(m_Message);
  OS << basic_manipulator;
  return *this;
}

//===----------------------------------------------------------------------===//
// AssertHelper
//===----------------------------------------------------------------------===//
pat::testing::AssertHelper::AssertHelper(TestPartResult::Type pType,
                                              const std::string& pFile,
                                              int pLineOfCode,
                                              const std::string& pMessage)
  : m_Result(pType, pFile, pLineOfCode, pMessage) {
}

void pat::testing::AssertHelper::operator=(bool pValue) const
{
  UnitTest::self()->addTestPartResult(m_Result);
}

//===----------------------------------------------------------------------===//
// Log
//===----------------------------------------------------------------------===//
testing::Log::Log(Severity pSeverity,
                  const std::string& pFileName,
                  int pLoC)
  : m_Severity(pSeverity) {
  const char* const mesg =
    kInfo    == pSeverity ? "[  INFO ]" :
    kWarning == pSeverity ? "[WARNING]" :
    kError   == pSeverity ? "[ ERROR ]" : "[ FATAL ]";

  getOStream() << std::endl << mesg << FormatFileLocation(pFileName, pLoC)
               << ": ";
}

testing::Log::~Log()
{
  getOStream() << std::endl;
  if (kFatal == m_Severity) {
    shutdown();
    fflush(stderr);
    exit(1);
  }
}

::std::ostream& testing::Log::getOStream()
{
  return ::std::cerr;
}

std::string
testing::Log::FormatFileLocation(const std::string& pFileName, int pLoC)
{
  std::string result;
  OStrStream OS(result);
  if (pFileName.empty())
    OS << "(unknown file)";
  else
    OS << pFileName;

  if (pLoC < 0) {
    OS << ":";
    return result;
  }
#ifdef _MSC_VER
  OS << "(" << pLoC << "):";
#else
  OS << ":" << pLoC << ":";
#endif  // _MSC_VER
  return result;
}

//===----------------------------------------------------------------------===//
// Repeater
//===----------------------------------------------------------------------===//
testing::Repeater::Repeater()
  : m_bForward(true) {
}

testing::Repeater::~Repeater()
{
  // Do nothing. Since we arn't the owner of Listeners.
}

void testing::Repeater::add(Listener* pListener)
{
  if (NULL != pListener) {
    m_Listeners.push_back(pListener);
  }
}

void testing::Repeater::release(Listener& pListener)
{
  ListenerList::iterator listener, lEnd = m_Listeners.end();
  for (listener = m_Listeners.begin(); listener != lEnd; ++listener) {
    if (*listener == &pListener) {
      m_Listeners.erase(listener);
      return;
    }
  }
}

#define PAT_REPEATER_METHOD(Name, ParamType) \
void testing::Repeater::Name(const ParamType& pParam) { \
  if (m_bForward) { \
    ListenerList::iterator listener, lEnd = m_Listeners.end(); \
    for (listener = m_Listeners.begin(); listener != lEnd; ++listener) { \
      (*listener)->Name(pParam); \
    } \
  } \
}

PAT_REPEATER_METHOD(OnTestProgramStart, testing::UnitTest)
PAT_REPEATER_METHOD(OnTestCaseStart, TestCase)
PAT_REPEATER_METHOD(OnSetUpStart, UnitTest)
PAT_REPEATER_METHOD(OnSetUpEnd, UnitTest)
PAT_REPEATER_METHOD(OnTestStart, TestInfo)
PAT_REPEATER_METHOD(OnTestPartResult, TestPartResult)
PAT_REPEATER_METHOD(OnPerfPartResult, PerfPartResult)
PAT_REPEATER_METHOD(OnTestEnd, TestInfo)
PAT_REPEATER_METHOD(OnTearDownStart, UnitTest)
PAT_REPEATER_METHOD(OnTearDownEnd, UnitTest)
PAT_REPEATER_METHOD(OnTestCaseEnd, TestCase)
PAT_REPEATER_METHOD(OnTestProgramEnd, UnitTest)
