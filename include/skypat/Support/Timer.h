//===- Timer.h ------------------------------------------------------------===//
//
//                     The SkyPat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#ifndef SKYPAT_SUPPORT_TIMER_H
#define SKYPAT_SUPPORT_TIMER_H
#include <skypat/skypat.h>

namespace skypat {
namespace testing {
namespace internal {

//===----------------------------------------------------------------------===//
// Timer
//===----------------------------------------------------------------------===//
class Timer
{
public:
  Timer();
  ~Timer();

  bool isActive() const { return m_bIsActive; }
  testing::Interval interval() const { return m_Interval; }

  void start();
  void stop();

  static std::string unit();

private:
  testing::Interval m_Interval;
  bool m_bIsActive;
};

} // namespace of internal
} // namespace of testing
} // namespace of skypat

#endif
