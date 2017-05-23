//===- ThreadTest.h -----------------------------------------------------===//
//
//                              The SkyPat team 
//
//  This file is distributed under the New BSD License.
//  See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/skypat.h>
#include <skypat/Thread/Thread.h>
#include <skypat/Thread/Mutex.h>

using namespace skypat;

class Counter
{
public:
  static Counter* self() {
    static Counter instance;
    return &instance;
  }

  unsigned int value() const {
    ScopedLock locker(m_Mutex);
    return m_Value;
  }

  void increase() {
    ScopedLock locker(m_Mutex);
    ++m_Value;
  }

private:
  unsigned int m_Value;
  mutable Mutex m_Mutex;
};

class WriterThread : public Thread
{
public:
  void run() {
    while(20 != Counter::self()->value())
      Counter::self()->increase();
  }
};

class MonitorThread : public Thread
{
public:
  void run() {
    while (20 != Counter::self()->value())
      ;
    get_ten = true;
  }

  bool getTen() const { return get_ten; }

public:
  static bool get_ten;
};

bool MonitorThread::get_ten = false;
