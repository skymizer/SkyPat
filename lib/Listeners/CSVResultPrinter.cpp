//===- CSVResultPrinter.cpp -----------------------------------------------===//
//
//                     The pat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#include <pat/Listeners/CSVResultPrinter.h>
#include <pat/ADT/Color.h>
#include <iostream>

using namespace pat;

//===----------------------------------------------------------------------===//
// CSVResultPrinter
//===----------------------------------------------------------------------===//
CSVResultPrinter::CSVResultPrinter()
  : m_OStream() {
}

CSVResultPrinter::~CSVResultPrinter()
{
  if (m_OStream.is_open())
    m_OStream.close();
}

bool CSVResultPrinter::open(const std::string& pFileName)
{
  if (m_OStream.is_open())
    return false;

  m_OStream.open(pFileName.c_str(), std::ostream::out | std::ostream::app);
  return m_OStream.good();
}

void CSVResultPrinter::OnTestEnd(const testing::TestInfo& pTestInfo)
{
  if (!pTestInfo.result().performance().empty()) {
    testing::TestResult::Performance::const_iterator perf =
                                      pTestInfo.result().performance().begin();
    testing::TestResult::Performance::const_iterator pEnd =
                                      pTestInfo.result().performance().end();
    m_OStream << pTestInfo.getTestName() << ",";
    while (perf != pEnd) {
      m_OStream << (*perf)->getTimerNum();
      ++perf;
      if (perf != pEnd)
        m_OStream << ",";
    }
    m_OStream << std::endl;
  }
}
