//===- Mutex.cpp ----------------------------------------------------------===//
//
//                              The "pat" team 
//
// This file is distributed under the New BSD License.
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#include <pat/Thread/Mutex.h>
#include <pat/Thread/MutexImpl.h>
#include <pat/Config/Config.h>

using namespace pat;

// Include the truly platform-specific parts.
// *.inc defines MutexData and platform-specific mutex.
#if defined(HAVE_PTHREAD)
#include "Pthread/Mutex.inc"
#else
#include "Quick/Mutex.inc"
#endif
