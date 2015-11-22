//===- Perf.h ------------------------------------------------------------===//
//
//                     The pat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#ifndef PAT_SUPPORT_PERF_H
#define PAT_SUPPORT_PERF_H
#include <pat/pat.h>

namespace pat {
namespace testing {
namespace internal {

//===----------------------------------------------------------------------===//
// Perf
//===----------------------------------------------------------------------===//
class Perf
{
public:
  Perf();
  Perf(enum PerfFlavor pFlavor, enum PerfType pType, enum PerfEvent pEvent);
  ~Perf();

  bool isActive() const { return m_bIsActive; }
  testing::Interval interval() const { return m_Interval; }
  testing::Interval interval2() const { return m_Interval2; }

  void start();
  void stop();

  static std::string unit();

private:
  testing::Interval m_Interval, m_Interval2;
  bool m_bIsActive;
};

} // namespace of internal
} // namespace of testing
} // namespace of pat

#endif
