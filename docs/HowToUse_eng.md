This document explains how to use **SkyPat** to test your program and evaluate its performance. 

### Introduction 

**SkyPat** is a C++ performance analyzing and testing framework on Android and Linux platforms. We refer the concept of Google Test and extend its scope from testing framework into **Performance Analysis Framework**.

With the help of **SkyPat**, programmers who wants to analyze their program, only need to write test cases via **SkyPat**, and **SkyPat** can analyze programs' correctness and performance.

The following section provides examples to show how to use **SkyPat**.

### Basic concept

The easiest steps to use SkyPat are:

1. Writing test cases including _ASSERT_, _EXPECT_, and _PERFORM_ macros.
1. Compiling these cases with your main program and the shared library of pat.
1. Finally, to execute the binary file and you can get its correctness and performance. 

Like other unit test libraries, such as Google Test, **SkyPat** provides several macro to integrate your test cases and your main program. The next section introduces to each macro and explains how to use these macros to write your test-cases. 

### Macros

There are three kinds of macros in **SkyPat**, _ASSERT_, _EXPECT_, and _PERFORM_. _ASSERT_ and _EXPECT_ are assertions for condition testing and _PERFORM_ wraps a block of code for performance testing. The difference between _ASSERT_ and _EXPECT_ is whether it is fatal or not. 

_ASSERT_ is assertion for fatal condition testing. That is to say, if the condition of  _ASSERT__* fails, the test fails and stops immediately.

_EXPECT_ is non-fatal assertion. When the condition of _EXPECT__* fails, it displays on screen to indicate that is a non-fatal failure. The whole test keeps running and isn't considered as a failure. 

_PERFORM_ evaluates performance rather than correctness. With the help of _PERFORM_, you could measure the performance of code within a test.

The following sections give you examples to use these macros in your test program and explains the meanings of the output results.

### Examples

All examples listed here are also in pat's source code, _${PAT_SRC}/examples_.

#### Include PAT Stuff

Let's start from the most useful macro: _ASSERT_. All macros and definitions used in pat are located in the file <pat/pat.h>

For example, in `assertion/main.cpp`, it includes:

      #include "pat/pat.h"

#### Declare A Test Function via _PAT_F_
_PAT_F_ macro declare a test.
It has two parameters: test case's name and test function's name.
In **SkyPat**, test logic is grouped into `test cases`. Every test case has a bunch of `test functions`. Here is an example to declare a test function.

    PAT_F(AircraftCase, take_off_test)
    {
       // Your Test Code
    }
    PAT_F(AircraftCase, landing_test)
    {
       // Your Test Code
    }

This example defines a test case _AircraftCase_ who has two test functions _take_off_test_ and _landing_test_.
This is how does the **SkyPat** organize the test codes.
You should put logically related tests into the same test case.

#### Write A Test Function
By writing test functions, users can evaluate performance and reliability.
For reliability, users can assert fatal or non-fatal conditions in a block of code; For performance, users can evaluate performance by using pat PERFORM macros.
The following examples explains:
  * How to assert one fatal condition
  * How to assert multiple fatal conditions
  * How to assert non-fatal conditions
  * Evaluate performance

##### Assert one fatal condition
This example shows you how to use _ASSERT\_TRUE_ and _ASSERT\_FALSE_ to write a test function.

Here is an example of _ASSERT\_TRUE_:

    PAT_F(SeriesCase, fibonacci_test)
    {
      int result = 0;
    
      ASSERT_TRUE(0 != fibonacci(10));
    }


_ASSERT_ is a macro resembling function calls. You test a class or a function by making assertions about its behavior. There are two kinds of _ASSERT_ macros, _ASSERT\_TRUE_ and _ASSERT\_FALSE_.

First of all, _ASSERT\_TRUE_ means that it will PASS only if the condition is TRUE. In this case, our condition is `0 != fibonacci(10)` that is obviously TRUE. So this test is executed successfully.

If the condition of _ASSERT\_TRUE_ fails, the test will stop and exit the test immediately.

Here is an example to show a false condition:

    PAT_F(SeriesCase, factorial_test_fail)
    {
      ASSERT_TRUE(360 == factorial(5));
    
      cout<<"end of the test"<<endl;
    }

Since the condition, `360 == factorial(5)`, obviously fails, `ASSERT_TRUE(360 == factorial(5))` should be considered as a failure. As a result, you can see these on shell in run-time:

    [ RUN      ] MyCase.factorial_test_fail
    [  FAILED  ] 
    main.cpp:62: fatal: failed to assert
    Value of: 360 == factorial(5)
      Actual:   false
      Expected: true

Because _ASSERT\_TRUE_ is considered as a failure, the test stops and exits the test immediately without executing `cout<<"end of the test"<<endl`.

Second, _ASSERT\_FALSE_ means that it will PASS only if the condition is FALSE and vice versa. 

    PAT_F(SeriesCase, factorial_test)
    {
      ASSERT_FALSE(360 == factorial(5));
    }

In this test, the condition, `360 == factorial(5)`, obviously fails. Thus, it runs successfully.

In your test case, you can change the condition as you want. Pat provides rich testing macros, such as conditional assertions - ASSERT_TRUE/FALSE, and predicated assertions - ASSERT_EQ/NE/LT/LE/GT/GE.

##### Assert multiple fatal conditions

If you have more than one fatal conditions, you can use multiple _ASSERT__* in one test.
If there is any fatal condition considered as failure, the test will stop and exit immediately without executing the subsequent code.

Here is an example for multiple _ASSERT__*:

    PAT_F(MyCase, fibonacci_test)
    {
      ASSERT_TRUE(0 != fibonacci(10));
      ASSERT_TRUE(2 == fibonacci(10));
      ASSERT_TRUE(0 != fibonacci(10));
    }

Since the condition, `2 == fibonacci(10)`, is obviously FALSE, `ASSERT_TRUE(2 == fibonacci(10))` should be considered as a failure. As a result, you can see these on shell in runtime:

    [ RUN      ] MyCase.fibonacci_test
    [  FAILED  ] 
    main.cpp:53: fatal: failed to assert
    Value of: 2 == fibonacci(10)
      Actual:   false
      Expected: true

Because `ASSERT_TRUE(2 == fibonacci(10))` is considered as a failure, the test stops and exits the test immediately without executing the second `ASSERT_TRUE(0 != fibonacci(10))`.

##### Assert non-fatal conditions

_EXPECT_ is non-fatal assertion. When the condition of _EXPECT__* fails, it displays the result on screen and keeps running. 

There are two kinds of _EXPECT_ macros, _EXPECT\_TRUE_ and _EXPECT\_FALSE_. _EXPECT\_TRUE_ means that it will PASS only if the condition is TRUE. _EXPECT\_FALSE_ means that it will PASS only if the condition is FALSE and vice versa. Like fatal conditions, you can insert multiple _EXPECT__* macros in a test, too.

Here is an example for _EXPECT__*:

    PAT_F(MyCase, fibonacci_test)
    {
      EXPECT_TRUE(0 != fibonacci(10));
      EXPECT_TRUE(2 == fibonacci(10));
      EXPECT_TRUE(10 == fibonacci(10));
      EXPECT_FALSE(10 != fibonacci(10));
    }

In this example, there are three non-fatal assertions, `EXPECT_TRUE(2 == fibonacci(10))`, `EXPECT_TRUE(10 == fibonacci(10))`, and `EXPECT_FALSE(10 != fibonacci(10))`, are considered as a failure, the program executes all without stopping and show these three unsuccessful expectations on screen:

    [ RUN      ] MyCase.fibonacci_test
    [  FAILED  ] 
    main.cpp:53: error: failed to expect
    Value of: 2 == fibonacci(10)
      Actual:   false
      Expected: true
    main.cpp:54: error: failed to expect
    Value of: 10 == fibonacci(10)
      Actual:   false
      Expected: true
    main.cpp:55: error: failed to expect
    Value of: 10 != fibonacci(10)
      Actual:   true
      Expected: false

##### Evaluate performance

_PERFORM_ macro measures the performance of a block of code wrapped by it. Here is an example of _PERFROM_:

    PERFORM(pat::CPU_CLOCK) {
      fibonacci(10);
    }

In this example, _PERFORM_ macro measures the execution time and the times of context switching when executing `fibonacci(10)` .

_SkyPat_ shows the time information as:

    [ RUN      ] MyCase.fibonacci_test
    [ TIME (ns)]         1063
    [EVENT TYPE] [CPU  CLOCK]
    [RESULT NUM]         1086

Here is another example of _PERFORM_:

    PAT_F(MyCase, fibonacci_test)
    {
      PERFORM(pat::CONTEXT_SWITCHES) {
        fibonacci(10);
      } 
      PERFORM(pat::CPU_CLOCK) {
        fibonacci(10);
      } 
      PERFORM(pat::TASK_CLOCK) {
        fibonacci(10);
      } 
      PERFORM(pat::CPU_CYCLES) {
        fibonacci(10);
      } 
      PERFORM(pat::INSTRUCTIONS) {
        fibonacci(10);
      } 
    }

This example shows how you can put five _PERFORM_ macros, which monitors different perf_events, in one test. By doing so, you can observe execution time and the value of pointed perf_events of the five regions of code in one test.

Here is what shows the information respectively on screen:

    [ RUN      ] MyCase.fibonacci_test
    [ TIME (ns)]         4380         1950         1577         3279         2548
    [EVENT TYPE] [CTX SWITCH] [CPU  CLOCK] [TASK CLOCK] [CPU CYCLES] [INST   NUM]
    [RESULT NUM]            0         1718         1505         3269         4338

You can also replace the parameter of _PERFORM_ macros to monitor perf_events which you prefer to monitor by SkyPat _PERFORM_ keywords. Here are all SkyPat _PERFORM_ keywords:

|  SkyPat PERFORM keywords | 
|:-------------------------------------:|
|  CPU_CYCLES  |
|  INSTRUCTIONS |
|  CACHE_REFERENCES |
|  CACHE_MISSES |
|  BRANCH_INSTRUCTIONS | 
|  BRANCH_MISSES |
|  BUS_CYCLES |
|  STALLED_CYCLES_FRONTEND |
|  STALLED_CYCLES_BACKEND |
|  REF_CPU_CYCLES |
|  CPU_CLOCK |
|  TASK_CLOCK |
|  PAGE_FAULTS |
|  CONTEXT_SWITCHES |
|  CPU_MIGRATIONS |
|  PAGE_FAULTS_MIN |
|  PAGE_FAULTS_MAJ |
|  ALIGNMENT_FAULTS |
|  EMULATION_FAULTS |



#### Run All Test Functions via `RunAll()`

Call `Initialize(&argc, argv)` and `RunAll()`. 

    int main(int argc, char* argv[])
    {
      pat::Test::Initialize(&argc, argv);
      pat::Test::RunAll();
    }

`Initialize(&argc, argv)` helps **SkyPat** to initial the output interface of **SkyPat**. If you do not call `Initialize`, you can not see the output of **SkyPat**.

The `RunAll()` function magically knows about all the tests you defined.
`RunAll()` runs all the tests you've declared, prints the results and returns zero only if all tests are succeed. Otherwise, it returns one.