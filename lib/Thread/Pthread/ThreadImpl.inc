//===- ThreadImpl.cpp -----------------------------------------------------===//
//
//                            The "pat" team 
//
// This file is distributed under the New BSD License.
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#include <pthread.h>

//===----------------------------------------------------------------------===//
// Thread-local storage
//===----------------------------------------------------------------------===//
static pthread_once_t per_thread_once = PTHREAD_ONCE_INIT;
static pthread_key_t  per_thread_key;

static void thread_key_create()
{
  pthread_key_create(&per_thread_key, NULL);
}

//===----------------------------------------------------------------------===//
// ThreadData
//===----------------------------------------------------------------------===//
ThreadData::ThreadData()
  : thread(NULL) {
}

ThreadData::~ThreadData()
{
  // avoid from deleting a uninitial key
  pthread_once(&per_thread_once, thread_key_create);

  pthread_key_delete(per_thread_key);
}

ThreadData* ThreadData::current()
{
  // create the key of thread-local-storage if we never create the key before.
  pthread_once(&per_thread_once, thread_key_create);

  ThreadData* data =
             reinterpret_cast<ThreadData*>(pthread_getspecific(per_thread_key));

  if (NULL == data) {
    data = new ThreadData();

    pthread_setspecific(per_thread_key, data);
  }
  return data;
}

#include <iostream>
using namespace std;
//===----------------------------------------------------------------------===//
// ThreadImpl - Leverage system pthread
//===----------------------------------------------------------------------===//
void* ThreadImpl::start(void* pArg)
{
  pthread_cleanup_push(ThreadImpl::finish, pArg);

  Thread* thread = reinterpret_cast<Thread*>(pArg);
  ThreadData::SetUp(thread->impl()->data,
                    *thread->impl()->parent);
  thread->impl()->thread_id = pthread_self();

  cerr << "(" << endl;
  thread->run();
  cerr << ")" << endl;

  pthread_cleanup_pop(1);
  return 0;
}
