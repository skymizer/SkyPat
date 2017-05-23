//===- Thread.h -----------------------------------------------------------===//
//
//                              The SkyPat team 
//
// This file is distributed under the New BSD License.
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#ifndef SKYPAT_THREAD_THREAD_H
#define SKYPAT_THREAD_THREAD_H
#include <skypat/SkypatNamespace.h>

namespace skypat {

class ThreadData;
class ThreadImpl;

/** \class Thread
 *  \brief Thread provides the abstract interface for threads.
 */
class Thread
{
public:
  Thread();

  virtual ~Thread();

  /// start - Begins execution of the thread.
  void start();

  /// join - Wait the execution of thread complete. This is similar to
  /// the functionality to pthread_join.
  bool join();

  HANDLE getThreadID() const;

protected:
  friend class ThreadImpl;

  virtual void run() = 0;

  const ThreadData* data() const;
  ThreadData*       data();

  const ThreadImpl* impl() const;
  ThreadImpl*       impl();

protected:
  ThreadImpl* m_pThreadImpl;
};

} // namespace of skypat

#endif
