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
#if defined(HAVE_ASM_UNISTD_H)
#include <asm/unistd.h>
#endif
#endif 

#ifndef PAT_PAT_H
#include <pat/pat.h>
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
  }
  ~PerfImpl() {
#if defined(HAVE_LINUX_PERF_EVENT_H)
    close(m_Fd);
#endif
  }

   testing::Interval getCounter() {
#if defined(HAVE_LINUX_PERF_EVENT_H)
     unsigned long long counter;
     read(m_Fd, &counter, sizeof(unsigned long long));
     return counter;
#endif
  }

  void init(enum PerfEvent pEvent) {
#if defined(HAVE_LINUX_PERF_EVENT_H)

    /* store the perf event numbers with the same order of pat:Perf_event_name */
    static const decltype(perf_event_attr::config)
        event_list[] = {
            PERF_COUNT_HW_CPU_CYCLES, PERF_COUNT_HW_INSTRUCTIONS,
            PERF_COUNT_HW_CACHE_REFERENCES, PERF_COUNT_HW_CACHE_MISSES,
            PERF_COUNT_HW_BRANCH_INSTRUCTIONS, PERF_COUNT_HW_BRANCH_MISSES,
            PERF_COUNT_HW_BUS_CYCLES, PERF_COUNT_HW_STALLED_CYCLES_FRONTEND,
            PERF_COUNT_HW_STALLED_CYCLES_BACKEND, PERF_COUNT_HW_REF_CPU_CYCLES,
            PERF_COUNT_SW_CPU_CLOCK, PERF_COUNT_SW_TASK_CLOCK,
            PERF_COUNT_SW_PAGE_FAULTS, PERF_COUNT_SW_CONTEXT_SWITCHES,
            PERF_COUNT_SW_CPU_MIGRATIONS, PERF_COUNT_SW_PAGE_FAULTS_MIN,
            PERF_COUNT_SW_PAGE_FAULTS_MAJ, PERF_COUNT_SW_ALIGNMENT_FAULTS,
            PERF_COUNT_SW_EMULATION_FAULTS,
#ifdef PERF_COUNT_SW_DUMMY
	    PERF_COUNT_SW_DUMMY
#else
	    0
#endif
    };

    struct perf_event_attr attr;
     
    memset(&attr, 0, sizeof(attr));

    attr.inherit = 1;
    attr.disabled = 1;

    attr.config = event_list[pEvent];

    if(pEvent < PerfEvent::CPU_CLOCK)
        attr.type = PERF_TYPE_HARDWARE;
    else
        attr.type = PERF_TYPE_SOFTWARE;

    attr.size = sizeof(attr);

    m_Fd = syscall(__NR_perf_event_open, &attr, 0, -1, -1, 0);
#endif
  }

  void start() {
#if defined(HAVE_LINUX_PERF_EVENT_H)
    ioctl(m_Fd, PERF_EVENT_IOC_ENABLE);
#endif
    m_Start = getCounter();
    assert(-1 != m_Start && "fail to get starting time");
  }

  void stop() {
#if defined(HAVE_LINUX_PERF_EVENT_H)
    ioctl(m_Fd, PERF_EVENT_IOC_DISABLE);
#endif
    m_End = getCounter();
    assert(-1 != m_End && "fail to get elapsed time");
  }

  testing::Interval getValue() const {
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
  : m_Interval(0), m_EventType(PerfEvent::CONTEXT_SWITCHES), m_bIsActive(false) {
  g_Perf->init(PerfEvent::CONTEXT_SWITCHES);
}

Perf::Perf(enum PerfEvent pEvent)
  : m_Interval(0), m_EventType(pEvent), m_bIsActive(false) {
  g_Perf->init(pEvent);
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
  m_Interval = g_Perf->getValue();
}

std::string Perf::unit()
{
  return "times";
}

} // namespace of internal
} // namespace of testing
} // namespace of pat
