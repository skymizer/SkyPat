//===- Thread.inc ---------------------------------------------------------===//
//
//                              The SkyPat team 
//
// This file is distributed under the New BSD License.
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#include <iostream>
#include <pthread.h>

//===----------------------------------------------------------------------===//
// Thread
//===----------------------------------------------------------------------===//
void Thread::start()
{
  pthread_attr_t attr;
  pthread_attr_init(&attr);

  // TODO: set up attributes

  // create a thread
  int code =
      pthread_create(&m_pThreadImpl->thread_id, &attr, ThreadImpl::start, this);
  if (0 != code) {
    std::cerr << code;
  }

  pthread_attr_destroy(&attr);
}

bool Thread::join()
{
  // TODO: can not wait on itself
  // TODO: if the thread is finished or not running, then return false
  // TODO: pthread_join, then return code.
  void* status;
  int code = pthread_join(impl()->thread_id, &status);
  if (0 != code) {
    std::cerr << code;
    return false;
  }
  return true;
}
