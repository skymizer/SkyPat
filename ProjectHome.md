&lt;wiki:gadget url="http://file.skymizer.com/pat/facebook.xml" height="20" width="120" border="0"/&gt;

**pat** is a C++ performance analyzing and testing framework on Android and Linux platforms.
It combines **perf\_events** and **unit tests** together and gives programmers the power of _fine-grained performance analysis_ via _writing typical test cases_.

# Current Status #
<div>
Date: 2014/03/31<br>
<br>
Release: 2.7.0, Black-throated, <a href='http://file.skymizer.com/pat/pat-2-7-0-bt.tar.gz'>http://file.skymizer.com/pat/pat-2-7-0-bt.tar.gz</a>
</div>
## New features ##
<div>
In this release, there are several interesting feature added, predicate assertion macros, ability to integrate with the other projects, building system.<br>
<br>
First, "pat" has added predicate assertion macros, ASSERT_EQ/NE/LT/LE/GT/GE, to have ability to observe the value of two parameters. For example, <code>ASSERT_LT(foo(), bar())</code> means that <code>foo()</code> should be less than <code>bar()</code>. If the assert fails, "pat" will show the value of <code>foo()</code> and <code>bar()</code> to let programmer know more details about this fail.<br>
<br>
Second, "pat" has enhanced the ability to integrate with the other projects via providing m4 scripts. User can use these scripts to integrate "pat" into their project as test and performance analysis framework.<br>
<br>
Finally, "pat" has added a building script to help user build "pat" by autotool more easier. User can execute the building script to build "pat" without setting configure file.<br>
<br>
Enjoy.<br>
</div>
# Introduction #
A typical test case is like the following.
<div>
<pre><code><br>
#include &lt;pat/pat.h&gt;<br>
PAT_F(SerialNumbersCase, FibonacciTest)<br>
{<br>
int result = 0;<br>
PERFORM {<br>
result = fibonacci(7);<br>
}<br>
ASSERT_EQ(result, 13);<br>
}<br>
</code></pre>
</div>


`PERFORM` macro wraps the code for performance test, and `ASSERT_TRUE` macro verifies the results. The result is like that:
<div>
<pre><code>[    pat   ] Running 1 tests from 1 cases.<br>
[----------] 1 test from SerialNumbersCase.<br>
[ RUN      ] SerialNumbersCase.FibonacciTest<br>
[       OK ] <br>
[ TIME (ns)]    3000<br>
[==========] 1 test from 1 cases ran.<br>
[  PASSED  ] 1 test.<br>
</code></pre>
</div>

# Support Platforms #
Pat is verified on these platforms:
  * Mac OS X 10.9.2
  * Fedora 20
  * Ubuntu 13.10