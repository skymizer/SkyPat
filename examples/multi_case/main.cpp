//===- main.cpp -----------------------------------------------------------===//
//
//                              The pat Team 
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// If you're like us, you'd like to look at some "pat" sample code. The
// examples/ folder has a numebr of well-commented samples show you how to
// use "pat" features.
//
// This sample shows how to write a simple performance test for a function,
// using "pat" framework.
//
//===----------------------------------------------------------------------===//

// Step 1. Include necessary header files such that the stuff your test logic
// needs is declared.
#include "pat/pat.h"
#include "my_case.h"

// Step 2. Use the macro to define your performance test.
//
// PAT macro has two parameters: the case name and the test name.
// After using the macro, you must define your test logic between a pair of
// braces.
//
// In "pat", test logics are grouped into `cases`. Every case has a bunch of
// `tests`. This is how we organize the test codes. You should put logically
// related tests into the same test case.
//
// Every test contains two parts: performance test and unit-test.
// PERFORM macro is used to measure the performance of code within a test;
// ASSERT macro compares the actual value to an expected value by using the
// equals operator. pat remembers the performance number of a test only if
// all unit-tests in the test are passed.
//
// The names of cases and tests should be valid C++ identifiers.
//
// "pat" does NOT guarantee that every tests you define is run exactly once.
// In fact, "pat" runs a test repeatedly for getting precise performance
// numbers. Therefore, you should guarantee that in all runs the initial value
// of a variable of a test is fixed.
PAT_F(Fibonacci, fibonacci_result)
{
  int result = 0;

  PERFORM {
    result = fibonacci(5);
  }

  EXPECT_TRUE(result == 8);

  PERFORM {
    result = fibonacci(10);
  }

  EXPECT_TRUE(result == 89);
}

PAT_F(Fibonacci, fibonacci_special_case)
{
  int result = 0;

  PERFORM {
    result = fibonacci(-1);
  }

  EXPECT_TRUE(result == 1);
}


PAT_F(Factorial, factorial_result)
{
  int result = 0;

  PERFORM {
    result = factorial(5);
  }

  EXPECT_TRUE(result == 120);

  PERFORM {
    result = factorial(10);
  }

  EXPECT_TRUE(result == 3628800);
}

PAT_F(Factorial, factorial_special_case)
{
  int result = 0;

  PERFORM {
    result = factorial(-1);
  }

  EXPECT_TRUE(result == 1);
}

// Step 3. Call RunAll() in main().
//
// This runs all the tests you've defined, prints the result and
// return 0 if succeed, or 1 otherwise.
//
// Did you notice that we didn't register any tests? The RunAll()
// function magically knows about all the test we defined. Isn't it
// convenient?
int main(int argc, char* argv[])
{
  pat::Test::Initialize(&argc, argv);
  pat::Test::RunAll();
}
