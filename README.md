# The SkyPat Team

-----------------------------------------------------------------------------

## Introduction
  "SkyPat" is a C++ performance analyzing and testing framework on Android and
  Linux platforms. It combines perf_events and unit tests together and gives
  programmers the power of fine-grained performance analysis via writing
  typical test cases. 

## Directory Structure

```
share/pat/README    - this document
share/pat/LICENSE   - the License
share/pat/examples/ - the place of examples
include/            - include file, exported interface
lib/                - libraries of exported interface
```

## Getting Started

### Build SkyPat From `git` Repository

Download the source code into ${PAT_SRC} directory.

    $ git clone https://github.com/skymizer/pat.git

#### Build by Dancing with Auto-tools

If there are no `configure` shell script in the directory,
 use `./autogen.sh` to generate `configure` script.

    $ ./autogen.sh

Use `./configure` to configure the package for your system. 
See [INSTALL](https://github.com/skymizer/pat/blob/master/INSTALL INSTALL) for 
more details.

    $ ./configure --prefix=${PAT_INSTALL}

Use `make` to compile the package

    $ make

Type `make install` to install the package into your system.

    $ make install

### Run Examples

Examples of **SkyPat** are installed at ${PAT_INSTALL}/share/pat/examples. 
You can go to any sub-directory and build it. For example:

    cd ${PAT_INSTALL}/share/pat/examples/assertion
    make

Then, if **SkyPat** is successfully installed, you should see:

    c++ -I../../../../include -L../../../../lib main.cpp my_case.cpp -lpat -o assertion
      LD_LIBRARY_PATH=../../../../lib ./assertion
      [    pat   ] Running 2 tests from 1 cases.
      [----------] 2 tests from MyCase.
      [ RUN      ] MyCase.fibonacci_test
      [       OK ]
      [ RUN      ] MyCase.factorial_test
      [       OK ]
      [==========] 2 tests from 1 cases ran.
      [  PASSED  ] 2 tests.

## User Guide

For those who need more information about **SkyPat**, please check out our
 [UserGuide](https://github.com/skymizer/pat/wiki/HowToUse) on Github.
