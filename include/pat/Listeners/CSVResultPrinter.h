//===- CSVResultPrinter.h -------------------------------------------------===//
//
//                     The pat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#ifndef PAT_LISTENERS_CSV_RESULT_PRINTER_H
#define PAT_LISTENERS_CSV_RESULT_PRINTER_H
#include <pat/pat.h>
#include <string>
#include <fstream>

namespace pat {
namespace testing {

class UnitTest;
class TestCase;
class TestInfo;

} // namespace of UnitTest

//===----------------------------------------------------------------------===//
// CSVResultPrinter
//===----------------------------------------------------------------------===//
class CSVResultPrinter : public pat::testing::Listener
{
public:
  CSVResultPrinter();

  ~CSVResultPrinter();

  bool open(const std::string& pFileName);

  void OnTestEnd(const testing::TestInfo& pTestInfo);

private:
  std::ofstream m_OStream;
};

} // namespace pat

#endif
