//===- Timer.cpp ----------------------------------------------------------===//
//
//                     The pat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#include "Timer.h"
#include <pat/Config/Config.h>

//===----------------------------------------------------------------------===//
// Timer Implementation
//===----------------------------------------------------------------------===//
#if defined(PAT_ON_WIN32)
#include "Windows/Timer.inc"
#endif

#if defined(PAT_ON_UNIX)
#include "Unix/Timer.inc"
#endif

#if defined(PAT_ON_DRAGON)
#include "Dragon/Timer.inc"
#endif
