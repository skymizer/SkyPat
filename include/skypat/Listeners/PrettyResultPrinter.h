//===- PrettyResultPrinter.h ----------------------------------------------===//
//
//                     The skypat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#ifndef SKYPAT_LISTENERS_PRETTY_RESULT_PRINTER_H
#define SKYPAT_LISTENERS_PRETTY_RESULT_PRINTER_H
#include <string>
#include <skypat/skypat.h>

namespace skypat {
namespace testing {

class UnitTest;
class TestCase;
class TestInfo;

} // namespace of UnitTest

//===----------------------------------------------------------------------===//
// PrettyResultPrinter
//===----------------------------------------------------------------------===//
class PrettyResultPrinter : public skypat::testing::Listener
{
public:
  static void PrintCaseName(const std::string& pCase, const std::string& pTest);
  void OnTestProgramStart(const testing::UnitTest& pUnitTest);
  void OnTestCaseStart(const testing::TestCase& pTestCase);
  void OnTestStart(const testing::TestInfo& pTestInfo);
  void OnTestEnd(const testing::TestInfo& pTestInfo);
  void OnTestProgramEnd(const testing::UnitTest& pUnitTest);
};

} // namespace skypat

#endif
