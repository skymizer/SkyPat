//===- ThreadImpl.cpp -----------------------------------------------------===//
//
//                              The "pat" team 
//
// This file is distributed under the New BSD License.
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#include <pat/Thread/ThreadImpl.h>
#include <pat/Config/Config.h>

using namespace pat;

//===----------------------------------------------------------------------===//
// ThreadData
//===----------------------------------------------------------------------===//
void ThreadData::SetUp(ThreadData*& pData, Thread& pParent)
{
  if (NULL != pData)
    return;

  pData = ThreadData::current();
  pData->thread = &pParent;
}

//===----------------------------------------------------------------------===//
// ThreadImpl
//===----------------------------------------------------------------------===//
ThreadImpl::ThreadImpl(Thread* pParent)
  : data(NULL), parent(pParent) {
}

ThreadImpl::~ThreadImpl()
{
}

void ThreadImpl::finish(void* pArg)
{
}

// Include the truly platform-specific parts. 
#if defined(HAVE_PTHREAD)
#include "Pthread/ThreadImpl.inc"
#else
#include "Quick/ThreadImpl.inc"
#endif
