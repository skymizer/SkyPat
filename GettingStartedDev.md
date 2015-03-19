# Developers Getting Started #
## Build pat From GIT Repository ##
  1. download the source code into ${PAT\_SRC} directory. See [Checkout](http://code.google.com/p/pat/source/checkout).
```
  git clone https://code.google.com/p/pat ${PAT_SRC}
```

### Build by Dancing with Auto-tools ###
  1. If there are no `configure' shell script in the directory, use `./autogen.sh' to generate `configure' script.
```
  ./autogen.sh
```
  1. Use `./configure` to configure the package for your system. See [INSTALL](http://code.google.com/p/pat/source/browse/INSTALL) for more details.
```
  ./configure --prefix=${PAT_INSTALL}
```
  1. Use `make` to compile the package
```
  make
```
  1. Type `make install` to install the package into your system.
```
  make install
```

## Run Examples ##
Examples of **pat** are installed at ${PAT\_INSTALL}/share/pat/examples. You can go to any subdirectory and build it. For example:
```
  cd ${PAT_INSTALL}/share/pat/examples/assertion
  make
```
Then, if **pat** is successfully installed, you should see:
```
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
```