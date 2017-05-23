//===- Perf.cpp ----------------------------------------------------------===//
//
//                     The SkyPat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/Support/Perf.h>
#include <skypat/Config/Config.h>

//===----------------------------------------------------------------------===//
// Perf Implementation
//===----------------------------------------------------------------------===//
#if defined(SKYPAT_ON_WIN32)
#include "Windows/Perf.inc"
#endif

#if defined(SKYPAT_ON_UNIX)
#include "Unix/Perf.inc"
#endif

#if defined(SKYPAT_ON_DRAGON)
#include "Dragon/Perf.inc"
#endif
