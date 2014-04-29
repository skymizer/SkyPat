//===- ThreadImpl.h -------------------------------------------------------===//
//
//                              The "pat" team 
//
// This file is distributed under the New BSD License.
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#ifndef PAT_THREAD_THREAD_IMPL_H
#define PAT_THREAD_THREAD_IMPL_H
#include <pat/PatNamespace.h>
#include <pat/ADT/TypeTraits.h>
#include <pat/Thread/Thread.h>
#include <pat/Thread/Mutex.h>
//#include <pat/Thread/WaitCondition.h>
#include <pat/Config/Config.h>

#if defined(HAVE_PTHREAD)
#include <pthread.h>
#endif

//
//  W A R N I N G
//  -------------
//
// This file is not part of the "pat" API.  It exists purely as an
// implementation detail.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//
//

namespace pat {

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

} // namespace of pat

#endif

