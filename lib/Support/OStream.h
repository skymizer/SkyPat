//===- OStream.h --------------------------------------------------------===//
//
//                     The pat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#ifndef PAT_SUPPORT_OSTREAM_H
#define PAT_SUPPORT_OSTREAM_H

#include <ios>
#include <ostream>
#include <iostream>
#include "../ADT/Color.h"

namespace pat {

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

//===----------------------------------------------------------------------===//
// Support::OStrStream
//===----------------------------------------------------------------------===//
/** \class BasicOStringBuf
 *  \brief The actual work of input and output (for std::string)
 *
 *  Unlike std::basic_stringbuf, BasicOStringBuf doesn't own a string for the
 *  internal buffer. Otherwise, BasicOStringBuf uses the given string as the
 *  internal buffer. Every change to the BasicOStringBuf also changes the
 *  content of given string directly.
 */
template<typename CharT, typename Traits, typename Alloc>
class BasicOStringBuf : public std::basic_streambuf<CharT, Traits>
{
public:
  typedef CharT  char_type;
  typedef Traits traits_type;
  typedef Alloc  allocator_type;
  typedef typename traits_type::int_type    int_type;
  typedef typename traits_type::pos_type    pos_type;
  typedef typename traits_type::off_type    off_type;

  typedef std::basic_streambuf<char_type, traits_type> streambuf_type;
  typedef std::basic_string<char_type, Traits, Alloc>  string_type;
  typedef typename string_type::size_type              size_type;

public:
  explicit BasicOStringBuf(string_type& pStr,
                           std::ios_base::openmode pMode = std::ios_base::out);

  ~BasicOStringBuf() { }

  /// str - return a copy
  string_type str() const;

protected:
  void BasicOStringBufInit(std::ios_base::openmode pMode);

  void doSync(char_type* pBase, size_type pO);

  int_type overflow(int_type pC = traits_type::eof());

  std::streamsize xsputn(const char_type* pS, std::streamsize pN);

protected:
  std::ios_base::openmode m_Mode;
  string_type&            m_String;
};

/** \class BasicOStrStream
 *  \brief BasicOStrStream is an OStream that writes to an std::string.
 */
template<typename CharT, typename Traits, typename Alloc>
class BasicOStrStream : public std::basic_ostream<CharT, Traits>
{
public:
  typedef CharT  char_type;
  typedef Traits traits_type;
  typedef Alloc  allocator_type;
  typedef typename traits_type::int_type int_type;
  typedef typename traits_type::pos_type pos_type;
  typedef typename traits_type::off_type off_type;

  // Non-standard types:
  typedef std::basic_string<CharT, Traits, Alloc> string_type;
  typedef BasicOStringBuf<CharT, Traits, Alloc> stringbuf_type;
  typedef std::basic_ostream<CharT, Traits> ostream_type;

public:
  explicit BasicOStrStream(string_type& pStr,
                           std::ios_base::openmode pMode = std::ios_base::out);

  /// Destructor - does nothing
  ~BasicOStrStream() {}

  stringbuf_type* rdbuf() const {
    return const_cast<stringbuf_type*>(&m_StringBuf);
  }

  /// str - get a copy of output stream
  string_type str() const { return m_StringBuf.str(); }

private:
  stringbuf_type m_StringBuf;
};


} // namespace of pat
#endif