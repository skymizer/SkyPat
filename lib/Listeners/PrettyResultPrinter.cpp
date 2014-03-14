//===- PrettyResultPrinter.cpp ---------------------------------------------===//
//
//                     The pat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#include <pat/Listeners/PrettyResultPrinter.h>
#include <pat/ADT/Color.h>
#include <iostream>

using namespace pat;

//===----------------------------------------------------------------------===//
// PrettyResultPrinter
//===----------------------------------------------------------------------===//
void PrettyResultPrinter::PrintCaseName(const std::string& pCase,
                                        const std::string& pTest)
{
  testing::Log::getOStream() << pCase << "." << pTest;
}

void PrettyResultPrinter::OnTestProgramStart(const testing::UnitTest& pUnitTest)
{
  testing::Log::getOStream() << Color::CYAN << "[    pat   ] "
    << "Running " << pUnitTest.getNumOfTests()
    << " tests from " << pUnitTest.getNumOfCases() << " cases." << std::endl;
}

void PrettyResultPrinter::OnTestCaseStart(const testing::TestCase& pTestCase)
{
  testing::Log::getOStream() << Color::GREEN << "[----------] " << Color::YELLOW;
  int num_tests = pTestCase.getNumOfTests();
  if (1 == num_tests)
    testing::Log::getOStream() << "1 test from ";
  else if (1 < num_tests)
    testing::Log::getOStream() << num_tests << " tests from ";

  testing::Log::getOStream() << pTestCase.getCaseName() << "."
                             << Color::RESET << std::endl;
}

void PrettyResultPrinter::OnTestStart(const testing::TestInfo& pTestInfo)
{
  testing::Log::getOStream() << Color::GREEN
                             << "[ RUN      ] " << Color::WHITE;
  PrintCaseName(pTestInfo.getCaseName(), pTestInfo.getTestName());
  testing::Log::getOStream() << Color::RESET << std::endl;
}

void PrettyResultPrinter::OnTestEnd(const testing::TestInfo& pTestInfo)
{
  // unit-test results
  if (pTestInfo.result().isPassed()) {
    testing::Log::getOStream() << Color::GREEN
                               << "[       OK ] \n";
  }
  if (pTestInfo.result().isFailed()) {
    testing::Log::getOStream() << Color::RED
                               << "[  FAILED  ] ";
    testing::TestResult::Reliability::const_iterator test =
                                    pTestInfo.result().reliability().begin();
    testing::TestResult::Reliability::const_iterator tEnd =
                                      pTestInfo.result().reliability().end();
    while (test != tEnd) {
      testing::Log::getOStream() << "\n" << Color::Bold(Color::YELLOW)
                                         << (*test)->filename()
                                         << ':'
                                         << (*test)->lineNumber()
                                         << ": ";
      switch ((*test)->type()) {
        case testing::TestPartResult::kFatalFailure: {
          testing::Log::getOStream()
                               << Color::RED
                               << "fatal: "
                               << Color::RESET << Color::Bold(Color::YELLOW)
                               << "failed to assert\n";
          break;
        }
        case testing::TestPartResult::kNonFatalFailure: {
          testing::Log::getOStream()
                               << Color::MAGENTA
                               << "error: "
                               << Color::RESET << Color::Bold(Color::YELLOW)
                               << "failed to expect\n";
          break;
        }
        default:
          break;
      }

      testing::Log::getOStream() << Color::RESET
                                 << (*test)->message();
      ++test;
    }
    testing::Log::getOStream() << std::endl;
  }

  // performance test results
  if (!pTestInfo.result().performance().empty()) {
    testing::Log::getOStream() << Color::Bold(Color::BLUE)
                               << "[ TIME (ns)]";

    testing::TestResult::Performance::const_iterator perf =
                                      pTestInfo.result().performance().begin();
    testing::TestResult::Performance::const_iterator pEnd =
                                      pTestInfo.result().performance().end();

    while (perf != pEnd) {
      testing::Log::getOStream() << " " << std::setw(7)
                                 << (*perf)->getPerformance();
      ++perf;
    }
    testing::Log::getOStream() << Color::RESET << std::endl;
  }
}

void PrettyResultPrinter::OnTestProgramEnd(const testing::UnitTest& pUnitTest)
{
  testing::Log::getOStream() << Color::CYAN << "[==========] "
                             << pUnitTest.getNumOfTests();
  if (1 == pUnitTest.getNumOfTests())
    testing::Log::getOStream() << " test from ";
  if (1 < pUnitTest.getNumOfTests())
    testing::Log::getOStream() << " tests from ";

  testing::Log::getOStream() << pUnitTest.getNumOfCases()
                             << " cases ran.\n";

  if (0 == pUnitTest.getNumOfFails()) {
    testing::Log::getOStream() << Color::GREEN << "[  PASSED  ] "
                               << Color::RESET
                               << pUnitTest.getNumOfTests();

    if (1 == pUnitTest.getNumOfTests()) {
      testing::Log::getOStream() << " test.\n"
                                 << std::flush;
    }

    if (1 < pUnitTest.getNumOfTests()) {
      testing::Log::getOStream() << " tests.\n"
                                 << std::flush;
    }
  }
  else {
    testing::Log::getOStream() << Color::RED << "[  FAILED  ] "
                               << Color::RESET
                               << pUnitTest.getNumOfFails();
    if (1 == pUnitTest.getNumOfFails()) {
      testing::Log::getOStream() << " test.\n"
                                 << std::flush;
    }

    if (1 < pUnitTest.getNumOfFails()) {
      testing::Log::getOStream() << " tests.\n"
                                 << std::flush;
    }
  }
}
