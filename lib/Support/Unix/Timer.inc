//===- Timer.inc ----------------------------------------------------------===//
//
//                     The pat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#include <pat/Support/ManagedStatic.h>
#include <time.h>
#include <unistd.h>
#include <cassert>

#if defined(HAVE_SYS_TIMES_H)
#include <sys/times.h>
#endif

#if defined(HAVE_SYS_TIME_H) && defined(ENABLE_GETTIMEOFDAY)
#include <sys/time.h>
#endif

#if defined(HAVE_LINUX_PERF_EVENT_H)
#include <linux/perf_event.h>
#include <sys/ioctl.h>
#include <cstring>
#include <cstdlib>
#ifndef __NR_perf_event_open
#define __NR_perf_event_open 364
#endif
#endif 

namespace pat {
namespace testing {
namespace internal {

//===----------------------------------------------------------------------===//
// Timer Implementation
//===----------------------------------------------------------------------===//
class TimerImpl
{
public:
  TimerImpl() {
    if (-1 == g_ClkTick) {
      g_ClkTick = sysconf(_SC_CLK_TCK);
      assert((0 < g_ClkTick) && "sysconf error");
#if defined(HAVE_LINUX_PERF_EVENT_H)
     struct perf_event_attr attr;
     
     memset(&attr, 0, sizeof(attr));

     attr.inherit = 1;
     attr.disabled = 1;
     attr.type = PERF_TYPE_SOFTWARE;
     attr.config = PERF_COUNT_SW_TASK_CLOCK;
     attr.size = sizeof(attr);

     m_Fd = syscall(__NR_perf_event_open, &attr, 0, -1, -1, 0);
#endif
    }
  }
  ~TimerImpl() {
#if defined(HAVE_LINUX_PERF_EVENT_H)
    close(m_Fd);
#endif
  }

   testing::Interval ns() {
#if defined(HAVE_LINUX_PERF_EVENT_H)
     unsigned long long runtime;
     read(m_Fd, &runtime, sizeof(unsigned long long));
     return runtime;
#elif defined(HAVE_CLOCK_GETTIME) && defined(ENABLE_CLOCK_GETTIME)
     struct timespec ts;
     int r = clock_gettime(CLOCK_MONOTONIC, &ts);
     return r == -1 ? -1 : ts.tv_sec * 1000000000LL + ts.tv_nsec;
#elif defined(HAVE_GETTIMEOFDAY) && defined(ENABLE_GETTIMEOFDAY)
     struct timeval tv;
     int r = gettimeofday(&tv, NULL);
     return r == -1 ? -1 : tv.tv_sec * 1000000000LL + (tv.tv_usec * 1000LL);
#else
     struct tms tm;
     clock_t r = times(&tm);
     return r == -1 ? -1 : r * 1000000000LL / g_ClkTick;
#endif
  }

  void start() {
#if defined(HAVE_LINUX_PERF_EVENT_H)
    ioctl(m_Fd, PERF_EVENT_IOC_ENABLE);
#endif
    assert(-1 != (m_Start = ns()) && "fail to get starting time");
  }

  void stop() {
#if defined(HAVE_LINUX_PERF_EVENT_H)
    ioctl(m_Fd, PERF_EVENT_IOC_DISABLE);
#endif
    assert(-1 != (m_End = ns()) && "fail to get elapsed time");
  }

  testing::Interval clock() const {
    return (m_End - m_Start);
  }

private:
  testing::Interval m_Start;
  testing::Interval m_End;

  static long g_ClkTick;

  int m_Fd;
};

long TimerImpl::g_ClkTick = -1;

static ManagedStatic<TimerImpl> g_Timer;

//===----------------------------------------------------------------------===//
// Timer
//===----------------------------------------------------------------------===//
Timer::Timer()
  : m_Interval(0), m_bIsActive(false) {
}

Timer::~Timer()
{
}

void Timer::start()
{
  m_bIsActive = true;
  g_Timer->start();
}

void Timer::stop()
{
  g_Timer->stop();
  m_bIsActive = false;
  m_Interval = g_Timer->clock();
}

std::string Timer::unit()
{
  return "ns";
}

} // namespace of internal
} // namespace of testing
} // namespace of pat
