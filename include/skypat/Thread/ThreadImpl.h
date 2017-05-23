//===- ThreadImpl.h -------------------------------------------------------===//
//
//                              The SkyPat team 
//
// This file is distributed under the New BSD License.
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#ifndef SKYPAT_THREAD_THREAD_IMPL_H
#define SKYPAT_THREAD_THREAD_IMPL_H
#include <skypat/SkypatNamespace.h>
#include <skypat/ADT/TypeTraits.h>
#include <skypat/Thread/Thread.h>
#include <skypat/Thread/Mutex.h>
//#include <skypat/Thread/WaitCondition.h>
#include <skypat/Config/Config.h>

#if defined(HAVE_PTHREAD)
#include <pthread.h>
#endif

//
//  W A R N I N G
//  -------------
//
// This file is not part of the "skypat" API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//
//

namespace skypat {

/** \class ThreadData
 *  \brief ThreadData stores thread specific data
 */
class ThreadData
{
public:
  ThreadData();

  ~ThreadData();

  static ThreadData* current();

  static void SetUp(ThreadData*& pData, Thread& pThread);

public:
  Thread* thread;
};

/** \class ThreadImpl
 *  \brief ThreadImpl provides the interface for the implementation of
 *  Thread.
 */
class ThreadImpl
{
public:
  ThreadImpl(Thread* pParent = NULL);

  ~ThreadImpl();

  static void* start(void* pArg);

  static void finish(void* pArg);

public:
#if defined(HAVE_PTHREAD)
  pthread_t thread_id;
#else
  HANDLE thread_id;
#endif

  ThreadData* data;
  Thread* parent;
};

} // namespace of skypat

#endif

