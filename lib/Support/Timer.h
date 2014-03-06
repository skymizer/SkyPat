//===- Timer.h ------------------------------------------------------------===//
//
//                     The pat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#ifndef PAT_SUPPORT_TIMER_H
#define PAT_SUPPORT_TIMER_H
#include <pat/pat.h>

namespace pat {
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
} // namespace of pat

#endif
