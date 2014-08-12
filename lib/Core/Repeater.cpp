//===- Repeater.cpp --------------------------------------------------------===//
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
