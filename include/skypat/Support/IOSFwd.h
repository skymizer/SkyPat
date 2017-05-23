//===- IOSFwd.h -----------------------------------------------------------===//
//
//                     The SkyPat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#ifndef SKYPAT_SUPPORT_IOSFWD_H
#define SKYPAT_SUPPORT_IOSFWD_H
#include <string>
#include <ios>

namespace skypat {

//===----------------------------------------------------------------------===//
// Stream Forwarding
//===----------------------------------------------------------------------===//
template<typename CharT, typename Traits = std::char_traits<CharT> >
  class BasicOStream;

template<typename CharT, typename Traits = std::char_traits<CharT> >
  class BasicFileBuf;

template<typename CharT, typename Traits = std::char_traits<CharT> >
  class BasicOFStream;

template<typename CharT, typename Traits = std::char_traits<CharT>,
         typename Alloc = std::allocator<CharT> >
  class BasicOStringBuf;

template<typename CharT, typename Traits = std::char_traits<CharT>,
         typename Alloc = std::allocator<CharT> >
  class BasicOStrStream;

/// @defgroup I/O Forward Declaration
/// @{
typedef BasicOStream<char>    OStream;
typedef BasicFileBuf<char>    FileBuf;
typedef BasicOFStream<char>   OFStream;
typedef BasicOStringBuf<char> OStringBuf;
typedef BasicOStrStream<char> OStrStream;
/// @}

} // namespace of skypat

#endif
