//===- my_case.cpp --------------------------------------------------------===//
//
//                               The SkyPat Team
//
// This file is distributed under the New BSD License.
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
//
// A sample program demonstrating using SkyPat performance testing framework.
//
// Author: Luba Tang <luba@skymizer.com>
//===----------------------------------------------------------------------===//
#include "my_case.h"

// Returns the Nth fibonacci number. For negative n, fibonacci(n) is defined to
// be 1.
int fibonacci(int n)
{
  if (n <= 1)
    return 1;

  if (n == 2)
    return 2;

  return fibonacci(n-1) + fibonacci(n-2);
}

// Returns n! (the factorial of n). For negative n, n! is defined to be 1.
int factorial(int n)
{
  if (n <= 1)
    return 1;

  return n * factorial(n-1);
}
