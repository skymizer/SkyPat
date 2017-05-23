//===- Mutex.h ------------------------------------------------------------===//
//
//                              The SkyPat team 
//
// This file is distributed under the New BSD License.
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#ifndef SKYPAT_MUTEX_H
#define SKYPAT_MUTEX_H
#include <skypat/ADT/Uncopyable.h>

namespace skypat {

class MutexData;
class WaitCondition;

/** \class Mutex
 *  \brief The Mutex class provides access serialization between threads.
 */
class Mutex : private Uncopyable
{
public:
  enum Status {
    Success,
    Busy,
    Invalid,
    UnknownError
  };

public:
  Mutex();

  ~Mutex();

  void lock() throw();

  void unlock() throw();

  Status tryLock() throw();

private:
  friend class WaitCondition;

  const MutexData* data() const { return m_pData; }
  MutexData*       data()       { return m_pData; }

private:
  mutable MutexData* m_pData;
};

/** \class ScopedLock
 *  \brief ScopedLock defines a scope as a mutual-execlusive region.
 */
class ScopedLock : private Uncopyable
{
public:
  ScopedLock(Mutex& pMutex) : m_Mutex(pMutex) { m_Mutex.lock(); }

  ~ScopedLock() { m_Mutex.unlock(); }

private:
  Mutex& m_Mutex;
};

} // namespace of skypat

#endif
