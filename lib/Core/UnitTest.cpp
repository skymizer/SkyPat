//===- UnitTest.cpp -------------------------------------------------------===//
//
//                     The pat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#include <pat/pat.h>

using namespace pat;

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

testing::TestInfo*
testing::UnitTest::addTestInfo(const std::string& pCaseName,
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
  testing::TestInfo* info = test_case->addTestInfo(pTestName, pFactory);
  ++m_NumOfTests;
  return info;
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

  RunCases::iterator iCase, iEnd = m_RunCases.end();
  for (iCase = m_RunCases.begin(); iCase != iEnd; ++iCase) {
    TestCase* test_case = *iCase;
    m_Repeater.OnTestCaseStart(*test_case);

    TestCase::iterator it   = test_case->begin();
    TestCase::iterator iEnd = test_case->end();
    while (it != iEnd) {
      m_pCurrentInfo = *it;
      (*it)->run();
      ++it;
    }

    m_Repeater.OnTestCaseEnd(*test_case);
  }

  m_Repeater.OnTestProgramEnd(*this);
}
