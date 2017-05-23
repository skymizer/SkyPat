//===- Mutex.cpp ----------------------------------------------------------===//
//
//                              The SkyPat team 
//
// This file is distributed under the New BSD License.
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/Thread/Mutex.h>
#include <skypat/Thread/MutexImpl.h>
#include <skypat/Config/Config.h>

using namespace skypat;

// Include the truly platform-specific parts.
// *.inc defines MutexData and platform-specific mutex.
#if defined(HAVE_PTHREAD)
#include "Pthread/Mutex.inc"
#else
#include "Quick/Mutex.inc"
#endif
