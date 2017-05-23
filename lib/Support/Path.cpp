//===- Path.cpp -----------------------------------------------------------===//
//
//                     The SkyPat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#include <skypat/Support/Path.h>
#include <skypat/Config/Config.h>
#include <cstring>
#include <cassert>

// Include the truly platform-specific parts. 
#if defined(SKYPAT_ON_UNIX)
#include "Unix/Path.inc"
#endif
#if defined(SKYPAT_ON_WIN32)
#include "Windows/Path.inc"
#endif
#if defined(SKYPAT_ON_DRAGON)
#include "Dragon/Path.inc"
#endif

using namespace skypat;

const char Path::colon = ':';                                      
const char Path::dot   = '.';

//===--------------------------------------------------------------------===//
// Path
//===--------------------------------------------------------------------===//
Path::Path()
  : m_PathName() {
}

Path::Path(const Path::ValueType* pStr)
  : m_PathName() {
  if (NULL != pStr)
    m_PathName = pStr;
}

Path::Path(const Path::StringType &s )
  : m_PathName(s) {
}

Path::Path(const Path& pCopy)
 : m_PathName(pCopy.m_PathName) {
}

Path::~Path()
{
}

bool Path::isFromRoot() const
{
  if (m_PathName.empty())
    return false;
  return (separator == m_PathName[0]);
}

bool Path::isFromPWD() const
{
  if (2 > m_PathName.size())
    return false;
  return ('.' == m_PathName[0] && separator == m_PathName[1]);
}

int Path::compare(const char* pString) const
{
  return ::strcmp(pString, m_PathName.c_str());
}

Path& Path::assign(const Path::StringType &s)
{
  m_PathName.assign(s);
  return *this;
}

Path& Path::assign(const Path::ValueType* s, unsigned int length)
{
  if (0 == s || 0 == length)
    assert(0 && "assign a null or empty string to Path");
  m_PathName.assign(s, length);
  return *this;
}

//a,/b a/,b a/,b/ a,b is a/b
Path& Path::append(const Path& pPath)
{
  //first path is a/,second path is /b
  if(m_PathName[m_PathName.length()-1] == separator &&
     pPath.native()[0] == separator) {
    unsigned int old_size = m_PathName.size()-1;
    unsigned int new_size = old_size + pPath.native().size();

    m_PathName.resize(new_size);
    strcpy(const_cast<ValueType*>(m_PathName.data()+old_size), pPath.native().data());
  }
  //first path is a,second path is b
  else if(this->native()[this->native().size()-1] != separator &&
          pPath.native()[0] != separator) {
    m_PathName.append(separator_str);
    m_PathName.append(pPath.native());
  }
  // a/,b or a,/b just append
  else {
    m_PathName.append(pPath.native());
  }
  return *this;
}

bool Path::empty() const
{
  return m_PathName.empty();
}

Path::StringType Path::generic_string() const
{
  StringType result = m_PathName;
  sys::canonicalize(result);
  return result;
}

bool Path::canonicalize()
{
  return sys::canonicalize(m_PathName);
}

Path::StringType::size_type Path::AppendSeparatorAsNeeded()
{
#if defined(SKYMIZER_ON_WIN32)
  // On Windows platform, path can not append separator.
  return 0;
#endif

  StringType::value_type last_char = m_PathName[m_PathName.size() - 1];
  if (!m_PathName.empty() &&
      !sys::is_separator(last_char)) {
    StringType::size_type tmp(m_PathName.size());
    m_PathName += separator_str;
    return tmp;
  }
  return 0;
}

void Path::EraseRedundantSeparator(Path::StringType::size_type pSepPos)
{
  size_t begin=pSepPos;
  // skip '/' or '\\'
  while(separator == m_PathName[pSepPos]) {
#if defined(SKYMIZER_ON_WIN32)
    pSepPos += 2;
#else
    ++pSepPos;
#endif
  }

  if(begin!=pSepPos)
    m_PathName.erase(begin+1,pSepPos-begin-1);
}

Path Path::parent_path() const
{
  size_t end_pos = m_PathName.find_last_of(separator);
  if (end_pos != StringType::npos)
    return Path(m_PathName.substr(0, end_pos));
  return Path();
}

Path Path::filename() const
{
  size_t pos = m_PathName.find_last_of(separator);
  if (pos != StringType::npos) {
    ++pos;
    return Path(m_PathName.substr(pos));
  }
  return Path(*this);
}

Path Path::stem() const
{
  size_t begin_pos = m_PathName.find_last_of(separator)+1;
  size_t end_pos   = m_PathName.find_last_of(dot);
  Path result_path(m_PathName.substr(begin_pos, end_pos - begin_pos));
  return result_path;
}

Path Path::extension() const
{
  size_t begin_pos = m_PathName.find_last_of('.');
  Path result_path(m_PathName.substr(begin_pos));
  return result_path;
}

//===--------------------------------------------------------------------===//
// non-member functions
//===--------------------------------------------------------------------===//
bool skypat::operator==(const Path& pLHS,const Path& pRHS)
{
  return (pLHS.generic_string()==pRHS.generic_string());
}

bool skypat::operator!=(const Path& pLHS,const Path& pRHS)
{
  return !(pLHS==pRHS);
}

Path skypat::operator+(const Path& pLHS, const Path& pRHS)
{
  Path result = pLHS;
  result.append(pRHS);
  return result;
}

