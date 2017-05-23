//===- CSVResultPrinter.h -------------------------------------------------===//
//
//                     The skypat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#ifndef SKYPAT_LISTENERS_CSV_RESULT_PRINTER_H
#define SKYPAT_LISTENERS_CSV_RESULT_PRINTER_H
#include <skypat/skypat.h>
#include <string>
#include <fstream>

namespace skypat {
namespace testing {

class UnitTest;
class TestCase;
class TestInfo;

} // namespace of UnitTest

//===----------------------------------------------------------------------===//
// CSVResultPrinter
//===----------------------------------------------------------------------===//
class CSVResultPrinter : public skypat::testing::Listener
{
public:
  CSVResultPrinter();

  ~CSVResultPrinter();

  bool open(const std::string& pFileName);

  void OnTestEnd(const testing::TestInfo& pTestInfo);

private:
  std::ofstream m_OStream;
};

} // namespace skypat

#endif
