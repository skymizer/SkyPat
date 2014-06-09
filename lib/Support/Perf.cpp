//===- Perf.cpp ----------------------------------------------------------===//
//
//                     The pat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#include <pat/Support/Perf.h>
#include <pat/Config/Config.h>

//===----------------------------------------------------------------------===//
// Perf Implementation
//===----------------------------------------------------------------------===//
#if defined(PAT_ON_WIN32)
#include "Windows/Perf.inc"
#endif

#if defined(PAT_ON_UNIX)
#include "Unix/Perf.inc"
#endif

#if defined(PAT_ON_DRAGON)
#include "Dragon/Perf.inc"
#endif
