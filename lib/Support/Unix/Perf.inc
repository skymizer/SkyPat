//===- Perf.inc ----------------------------------------------------------===//
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
// Perf Implementation
//===----------------------------------------------------------------------===//
class PerfImpl
{
public:
  PerfImpl() {
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
  ~PerfImpl() {
#if defined(HAVE_LINUX_PERF_EVENT_H)
    close(m_Fd);
#endif
  }

   testing::Interval ns() {
#if defined(HAVE_LINUX_PERF_EVENT_H)
     unsigned long long runtime;
     read(m_Fd, &runtime, sizeof(unsigned long long));
     return runtime;
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

long PerfImpl::g_ClkTick = -1;

static ManagedStatic<PerfImpl> g_Perf;

//===----------------------------------------------------------------------===//
// Perf
//===----------------------------------------------------------------------===//
Perf::Perf()
  : m_Interval(0), m_bIsActive(false) {
}

Perf::~Perf()
{
}

void Perf::start()
{
  m_bIsActive = true;
  g_Perf->start();
}

void Perf::stop()
{
  g_Perf->stop();
  m_bIsActive = false;
  m_Interval = g_Perf->clock();
}

std::string Perf::unit()
{
  return "ns";
}

} // namespace of internal
} // namespace of testing
} // namespace of pat
