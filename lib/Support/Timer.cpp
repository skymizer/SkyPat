//===- Timer.cpp ----------------------------------------------------------===//
//
//                     The SkyPat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/Support/Timer.h>
#include <skypat/Config/Config.h>

//===----------------------------------------------------------------------===//
// Timer Implementation
//===----------------------------------------------------------------------===//
#if defined(SKYPAT_ON_WIN32)
#include "Windows/Timer.inc"
#endif

#if defined(SKYPAT_ON_UNIX)
#include "Unix/Timer.inc"
#endif

#if defined(SKYPAT_ON_DRAGON)
#include "Dragon/Timer.inc"
#endif
