//===- Timer.inc ----------------------------------------------------------===//
//
//                     The SkyPat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/Support/ManagedStatic.h>
#include <time.h>
#include <unistd.h>
#include <cassert>
#include <iostream>

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
#if defined(HAVE_ASM_UNISTD_H)
#include <asm/unistd.h>
#endif
#endif 

namespace skypat {
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
    ioctl(m_Fd, PERF_EVENT_IOC_DISABLE);
    close(m_Fd);
#endif
  }

   testing::Interval clock() {
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
    if (false == g_bIsEnabled) {
#if defined(HAVE_LINUX_PERF_EVENT_H)
      ioctl(m_Fd, PERF_EVENT_IOC_ENABLE);
#endif
      g_bIsEnabled = true;
      m_Start = clock();
      assert(-1 != m_Start && "fail to get starting time");
    }
  }

  void stop() {
    m_End = clock();
    assert(-1 != m_End && "fail to get elapsed time");
  }

private:
  testing::Interval m_Start;
  testing::Interval m_End;
  static bool g_bIsEnabled;

  static long g_ClkTick;

  int m_Fd;
};

long TimerImpl::g_ClkTick = -1;
bool TimerImpl::g_bIsEnabled = false;

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
  g_Timer->start();
  m_Interval = g_Timer->clock();
  m_bIsActive = true;
}

void Timer::stop()
{
  m_Interval = g_Timer->clock() - m_Interval;
  m_bIsActive = false;
}

std::string Timer::unit()
{
  return "ns";
}

} // namespace of internal
} // namespace of testing
} // namespace of skypat
