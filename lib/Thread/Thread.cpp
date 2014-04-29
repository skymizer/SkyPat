//===- Thread.cpp ---------------------------------------------------------===//
//
//                              The "pat" team 
//
// This file is distributed under the New BSD License.
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#include <pat/Thread/Thread.h>
#include <pat/Thread/ThreadImpl.h>
#include <pat/Config/Config.h>
#include <cassert>

using namespace pat; 

//===----------------------------------------------------------------------===//
// Thread
//===----------------------------------------------------------------------===//
Thread::Thread()
  : m_pThreadImpl(new ThreadImpl(this)) {
}

Thread::~Thread()
{
  delete m_pThreadImpl;
}

HANDLE Thread::getThreadID() const
{
  return (HANDLE)impl()->thread_id;
}

const ThreadData* Thread::data() const
{
  assert(NULL != m_pThreadImpl->data &&
         "There is no thread data before creating a thread");
  return m_pThreadImpl->data;
}

ThreadData* Thread::data()
{
  assert(NULL != m_pThreadImpl->data &&
         "There is no thread data before creating a thread");
  return m_pThreadImpl->data;
}

const ThreadImpl* Thread::impl() const
{
  return m_pThreadImpl;
}

ThreadImpl* Thread::impl()
{
  return m_pThreadImpl;
}

// Include the truly platform-specific parts.
// *.inc defines platform-specific implementation.
#if defined(HAVE_PTHREAD)
#include "Pthread/Thread.inc"
#else
#include "Quick/Thread.inc"
#endif
