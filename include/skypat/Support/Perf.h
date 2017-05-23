//===- Perf.h ------------------------------------------------------------===//
//
//                     The SkyPat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#ifndef SKYPAT_SUPPORT_PERF_H
#define SKYPAT_SUPPORT_PERF_H
#include <skypat/skypat.h>

namespace skypat {
namespace testing {
namespace internal {

//===----------------------------------------------------------------------===//
// Perf
//===----------------------------------------------------------------------===//
class Perf
{
public:
  Perf();
  Perf(enum PerfEvent pEvent);
  ~Perf();

  bool isActive() const { return m_bIsActive; }
  testing::Interval interval() const { return m_Interval; }
  testing::Interval eventType() const { return m_EventType; }

  void start();
  void stop();

  static std::string unit();

private:
  testing::Interval m_Interval, m_EventType;
  bool m_bIsActive;
};

} // namespace of internal
} // namespace of testing
} // namespace of skypat

#endif
