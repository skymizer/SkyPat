//===- Test.cpp -----------------------------------------------------------===//
//
//                     The pat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#include <pat/pat.h>
#include <pat/Listeners/PrettyResultPrinter.h>
#include <pat/Listeners/CSVResultPrinter.h>
#include <time.h>
#include <cassert>
#include <unistd.h>
#include <string>
#include <cstdlib>

using namespace pat;

//===----------------------------------------------------------------------===//
// Helper Functions
//===----------------------------------------------------------------------===//
static inline void help(int pArgc, char* pArgv[])
{
  testing::Log::getOStream() << "Usage:\n"
                             << "\t" << pArgv[0] << " [options...]\n\n"
                             << "Options:\n"
                             << "\t-c [file]  toutput CSV to [file]\n"
                             << "\t-h         Show this help manual\n";
}

//===----------------------------------------------------------------------===//
// Test
//===----------------------------------------------------------------------===//
Test::~Test()
{
  // MUST KEEP THIS DESTRUCTOR
}

void Test::run()
{
  this->TestBody();
}

void Test::Initialize(int* pArgc, char* pArgv[])
{
  // Choice printer
  int opt;
  std::string csv_file;
  while ((opt = getopt(*pArgc, pArgv, "c:h")) != -1 ) {
    switch (opt) {
      case 'c':
        csv_file = optarg;
        break;
      case 'h':
      default:
        help(*pArgc, pArgv);
        return;
    }
  }

  if (!csv_file.empty()) {
    CSVResultPrinter* printer = new CSVResultPrinter();
    if (printer->open(csv_file)) {
      testing::UnitTest::self()->repeater().add(printer);
    }
    else {
      testing::Log::getOStream() << "Failed to open file `" << csv_file << "`\n";
      delete printer;
    }
  }
  else
    testing::UnitTest::self()->repeater().add(new PrettyResultPrinter());

  // Choice runnable tests
}

void Test::RunAll()
{
  testing::UnitTest::self()->RunAll();
}

void Test::Sleep(int pMS)
{
  assert(pMS > 0 && "Cannot sleep zero milliseconds");

  struct timespec ts = { pMS / 1000, (pMS % 1000) * 1000 * 1000 };
  nanosleep(&ts, NULL);
}

