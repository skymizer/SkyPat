# The SkyPat Team

-----------------------------------------------------------------------------

## Introduction
  "SkyPat" is a C++ performance analyzing and testing framework on Android and
  Linux platforms. It combines perf_events and unit tests together and gives
  programmers the power of fine-grained performance analysis via writing
  typical test cases. 

## Latest release

* v3.1.1 Bourbon
  - Tarball: https://github.com/skymizer/SkyPat/archive/v3.1.1.tar.gz
  - Zipball: https://github.com/skymizer/SkyPat/archive/v3.1.1.zip

## Directory Structure

```
share/SkyPat/README    - this document
share/SkyPat/LICENSE   - the License
share/SkyPat/examples/ - the place of examples
include/            - include file, exported interface
lib/                - libraries of exported interface
```

## Supported platforms

SkyPat supports macOS, Ubuntu/x86_64, Debian/arm, Fedora/x86_64, Fedora/arm,
 and CentOS/x86_64.

Here is a list of verified versions:

* Ubuntu/x86_64
  - 14.04 and later versions
* Fedora/x86_64
  - 20 and later versions
* CentOS/x86_64
  - 7 and later versions
* Debian/arm
  - 8.6 and later versions

## Getting Started

## Download and build SkyPat


You could download **SkyPat** either from the latest release tar-ball or from
 our `git` repository.

### Method 1: Download **SkyPat** from the latest release tar-ball
Download the source code from the latest release tar-ball.

    $ wget -O SkyPat-3.1.1.tar.gz https://github.com/skymizer/SkyPat/archive/v3.1.1.tar.gz
    $ tar zxvf SkyPat-3.1.1.tar.gz

### Method 2: Download **SkyPat** from our `git` repository
Download the source code from our `git` repository into ${SKYPAT_SRC} directory.
 
    $ git clone https://github.com/skymizer/SkyPat.git ${SKYPAT_SRC}

### Install required packages

#### Ubuntu/Debian users

Install following packages before you install **SkyPat**.

    $ sudo apt-get install wget automake autoconf libtool build-essential

#### Fedora/CentOS users

Install following packages before you install **SkyPat**.

    $ sudo yum install wget automake autoconf libtool make automake gcc gcc-c++
    $ sudo yum install kernel-devel 

### Build by Auto-tools

If there are no `configure` shell script in the directory,
 use `./autogen.sh` to generate `configure` script.

    $ ./autogen.sh

Use `./configure` to configure the package for your system. 
See [INSTALL](https://github.com/skymizer/SkyPat/blob/master/INSTALL INSTALL) for 
more details.

    $ ./configure --prefix=${SKYPAT_INSTALL}

Use `make` to compile the package

    $ make

Type `make install` to install the package into your system.

    $ make install

### Run Examples

Examples of **SkyPat** are installed at ${SKYPAT_INSTALL}/share/skypat/examples. 
You can go to any sub-directory and build it. For example:

    cd ${SKYPAT_INSTALL}/share/skypat/examples/assertion
    make

Then, if **SkyPat** is successfully installed, you should see:

    c++ -I../../../../include -L../../../../lib main.cpp my_case.cpp -lskypat -o assertion
      LD_LIBRARY_PATH=../../../../lib ./assertion
      [  SkyPat  ] Running 2 tests from 1 cases.
      [----------] 2 tests from MyCase.
      [ RUN      ] MyCase.fibonacci_test
      [       OK ]
      [ RUN      ] MyCase.factorial_test
      [       OK ]
      [==========] 2 tests from 1 cases ran.
      [  PASSED  ] 2 tests.

## User Guide

For those who need more information about **SkyPat**, please check out our
 [UserGuide](https://github.com/skymizer/SkyPat/wiki/HowToUse) on Github.
