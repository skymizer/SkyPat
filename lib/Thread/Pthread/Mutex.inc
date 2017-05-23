//===- Mutex.inc ----------------------------------------------------------===//
//
//                              The SkyPat team 
//
// This file is distributed under the New BSD License.
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#include <pthread.h>
#include <errno.h>
#include <skypat/Thread/MutexImpl.h>
#include <iostream>

//===----------------------------------------------------------------------===//
// Mutex
//===----------------------------------------------------------------------===//
Mutex::Mutex()
  : m_pData(new MutexData()) {
  int code = pthread_mutex_init(&m_pData->mutex, NULL);
  if (0 != code)
    std::cerr<< code;
}

Mutex::~Mutex()
{
  int code = pthread_mutex_destroy(&m_pData->mutex);
  if (0 != code)
    std::cerr<< code;
  delete m_pData;
}

void Mutex::lock() throw()
{
  int code = pthread_mutex_lock(&m_pData->mutex);
  if (0 != code)
    std::cerr<< code;
}

void Mutex::unlock() throw()
{
  int code = pthread_mutex_unlock(&m_pData->mutex);
  if (0 != code)
    std::cerr<< code;
}

Mutex::Status Mutex::tryLock()
{
  switch(pthread_mutex_trylock(&m_pData->mutex)) {
    case 0:
      return Success;
    case EBUSY:
      return Busy;
    case EINVAL:
      return Invalid;
  }
  return UnknownError;
}
