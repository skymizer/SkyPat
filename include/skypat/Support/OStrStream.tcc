//===- OStream.tcc --------------------------------------------------------===//
//
//                     The SkyPat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
// BasicOStringBuf
//===----------------------------------------------------------------------===//
template<typename CharT, typename Traits, typename Alloc>
skypat::BasicOStringBuf<CharT, Traits, Alloc>::BasicOStringBuf(string_type& pStr,
                                                  std::ios_base::openmode pMode)
  : streambuf_type(), m_Mode(), m_String(pStr) {
  BasicOStringBufInit(pMode);
}

template<typename CharT, typename Traits, typename Alloc> void
skypat::BasicOStringBuf<CharT, Traits, Alloc>::BasicOStringBufInit(
                                                  std::ios_base::openmode pMode)
{
  m_Mode = pMode;
  size_type len = 0;
  if (m_Mode & (std::ios_base::ate | std::ios_base::app))
    len = m_String.size();

  doSync(const_cast<char_type*>(m_String.data()), len);
}

/// doSync - Internal function for correctly updating the internal buffer
/// for particular m_String.
template<typename CharT, typename Traits, typename Alloc> void
skypat::BasicOStringBuf<CharT, Traits, Alloc>::doSync(char_type* pBase, 
                                                                   size_type pO)
{
  char_type* endp = pBase + m_String.capacity();
  this->setp(pBase, endp);
  this->pbump(pO);
}

template<typename CharT, typename Traits, typename Alloc>
typename skypat::BasicOStringBuf<CharT, Traits, Alloc>::string_type
skypat::BasicOStringBuf<CharT, Traits, Alloc>::str() const
{
  return m_String;
}

template<typename CharT, typename Traits, typename Alloc>
typename skypat::BasicOStringBuf<CharT, Traits, Alloc>::int_type
skypat::BasicOStringBuf<CharT, Traits, Alloc>::overflow(int_type pC)
{
  const bool testout = this->m_Mode & std::ios_base::out;
  if (!testout)
    return traits_type::eof();

  const bool testeof = traits_type::eq_int_type(pC, traits_type::eof());
  if (testeof)
    return traits_type::not_eof(pC);

  const size_type capacity = m_String.capacity();
  const size_type max_size = m_String.max_size();
  const bool testput = this->pptr() < this->epptr();
  if (!testput && capacity == max_size)
    return traits_type::eof();

  // try to append pC into output sequence
  const char_type conv = traits_type::to_char_type(pC);
  if (!testput) {
    string_type tmp;
    // magic number. 512 looks like the best configuration on Mac OSX, 64 bits.
    const size_type opt_len = std::max(size_type(2 * capacity), size_type(512));
    tmp.reserve(std::min(opt_len, max_size));
    if (this->pbase())
      tmp.assign(this->pbase(), this->epptr() - this->pbase());
    tmp.push_back(conv);
    m_String.swap(tmp);
    doSync(const_cast<char_type*>(m_String.data()), this->pptr()-this->pbase());
  }
  else {
    // Although pptr >= epptr, we still have capacity.
    *this->pptr() = conv;
  }
  this->pbump(1);
  return pC;
}

template<typename CharT, typename Traits, typename Alloc> std::streamsize
skypat::BasicOStringBuf<CharT, Traits, Alloc>::xsputn(const char_type* pS,
                                                   std::streamsize pN)
{
  std::streamsize result = 0;
  while (result < pN) {
    const std::streamsize buf_len = this->epptr() - this->pptr();
    if (buf_len) {
      const std::streamsize remaining = pN - result;
      const std::streamsize len = std::min(buf_len, remaining);
      // In order to correct m_String's bookkeeping, use append instead of
      // char_traits::copy.
      m_String.append(pS, len);
      result += len;
      pS += len;
      this->pbump(len);
    }

    if (result < pN) {
      int_type c = this->overflow(traits_type::to_int_type(*pS));
      if (!traits_type::eq_int_type(c, traits_type::eof())) {
        ++result;
        ++pS;
      }
      else
        break;
    }
  }
  return result;
}

//===----------------------------------------------------------------------===//
// BasicOStrStream
//===----------------------------------------------------------------------===//
template<typename CharT, typename Traits, typename Alloc>
skypat::BasicOStrStream<CharT, Traits, Alloc>::BasicOStrStream(string_type& pStr
                                                , std::ios_base::openmode pMode)
  : ostream_type(), m_StringBuf(pStr, pMode | std::ios_base::out) {
  this->init(&m_StringBuf);
}
