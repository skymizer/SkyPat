//===- Color.h --------------------------------------------------------===//
//
//                     The pat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#ifndef PAT_SUPPORT_COLOR_H
#define PAT_SUPPORT_COLOR_H
#include <iosfwd>
#include <iomanip>

namespace pat {
//===----------------------------------------------------------------------===//
// Color
//===----------------------------------------------------------------------===//
/** \class Color
 *  \brief Color is a std::ostream manipulator for changing the color.
 */
class Color
{
public:
  // color order matches ANSI escape sequence, don't change
  enum kColor {
    BLACK,
    RED,
    GREEN,
    YELLOW,
    BLUE,
    MAGENTA,
    CYAN,
    WHITE,

    RESET
  };

  enum kType {
    NORMAL,
    BOLD,
    BACKGROUND
  };

public:
  Color(kColor pColor = BLACK, kType pType = NORMAL)
    : m_Color(pColor), m_Type(pType)
  {}

  static Color Bold(kColor pColor) {
    return Color(pColor, BOLD);
  }
  static Color BackGround(kColor pColor) {
    return Color(pColor, BACKGROUND);
  }

  template<typename CharT, typename Traits>
  void setColor(::std::basic_ostream<CharT, Traits>& pOS) {
    pOS << "\033[";
    if (m_Color != RESET) {
      if (m_Type == BOLD) {
        pOS << "1;";
      }
      // TODO: Turn off bold mode;
      int base_color = m_Type == BACKGROUND ? 40 : 30;
      pOS << (base_color + m_Color);
    }
    pOS << 'm';
  }

private:
  kColor m_Color : 24;
  kType m_Type : 8;
};

template<typename CharT, typename Traits>
static inline
::std::basic_ostream<CharT, Traits>&
operator<<(::std::basic_ostream<CharT, Traits>& pOS, Color pColor) {
  pColor.setColor(pOS);
  return pOS;
}

template<typename CharT, typename Traits>
static inline
::std::basic_ostream<CharT, Traits>&
operator<<(::std::basic_ostream<CharT, Traits>& pOS, Color::kColor pColor) {
  Color(pColor).setColor(pOS);
  return pOS;
}

} // namespace of pat

#endif
