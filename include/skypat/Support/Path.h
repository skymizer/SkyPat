//===- Path.h -------------------------------------------------------------===//
//
//                     The SkyPat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
// This file declares the skypat::Path. It follows TR2/boost
// filesystem (v3), but modified to remove exception handling and RTTI.
//===----------------------------------------------------------------------===//
#ifndef SKYPAT_SUPPORT_PATH_H
#define SKYPAT_SUPPORT_PATH_H
#include <stdint.h>
#include <iosfwd>
#include <string>

namespace skypat {

/** \class Path
 *  \brief Path provides an abstraction for the path to a file or directory in
 *   the operating system's filesystem.
 *
 *  Path is concerned only with the lexical and syntactic aspects of a path
 *  name. The path name does not necessarily exist in external storage, and
 *  path name is not necessarily valid for the current operating system or for
 *  a particular file system.
 *
 *  Native path name format is implementation-defined. Current storage type of
 *  native format is std::string.
 */
class Path
{
public:
  typedef char            ValueType;
  typedef std::string     StringType;

public:
  static const char       separator;
  static const char       preferred_separator;
  static const StringType separator_str;
  static const char       colon;
  static const char       dot;

public:
  /// empty path.
  Path();

  /// Stores the path name to the native format.
  Path(const ValueType* pPathName);

  /// Stores the path name to the native format.
  Path(const StringType &pPathName);

  /// Copy the native format from pCopy.
  Path(const Path& pCopy);

  /// Destructor
  virtual ~Path();

  /// Store the content [begin, end) in path name by native format.
  template <class InputIterator>
  Path& assign(InputIterator begin, InputIterator end);

  /// Assign \ref pPathName in native format.
  Path& assign(const StringType &pPathName);

  /// Assign \ref pPathName in native format.
  Path& assign(const ValueType* s, unsigned int length);

  /// Append the content [begin, end) in the native format. The append
  /// operations also appends the platform's preferred dictionary
  /// separator when needed. The preferred dictionary separator is
  /// implementation defined. In Unix platforms, it's '/' in general.
  template <class InputIterator>
  Path& append(InputIterator begin, InputIterator end);

  /// Append the \ref pPathName in the native format. The append
  /// operations also appends the platform's preferred dictionary
  /// separator when needed. The preferred dictionary separator is
  /// implementation defined. In Unix platforms, it's '/' in general.
  Path& append(const Path& pPathName);

  /// Check if the native format is empty.
  bool empty() const;

  /// Check if the path is absolutely starting from root.
  bool isFromRoot() const;

  /// Check if the path is starting from the current working folder.
  bool isFromPWD() const;

  /// Returns the native format.
  const StringType& native() const { return m_PathName; }

  /// Returns the native format.
  StringType&       native()       { return m_PathName; }

  /// Compares the value of the string object.
  /// @param [in] pString the target string.
  /// @return If they compare equal, then return 0;
  ///         If \ref pString is longer, then return > 0;
  ///         If \ref pString is shorter, then return < 0;
  int compare(const char* pString) const;

  /// Compares the value of the string object.
  /// @param [in] pString the target string.
  /// @return If they compare equal, then return 0;
  ///         If \ref pString is shorter, then return < 0;
  ///         If \ref pString is longer, then return > 0;
  int compare(const std::string& pString) const
  { return compare(pString.c_str()); }

  /// Compares the value of the string object.
  /// @param [in] pString the target string.
  /// @return If they compare equal, then return 0;
  ///         If \ref pString is shorter, then return < 0;
  ///         If \ref pString is longer, then return > 0;
  int compare(const Path& pString) const
  { return compare(pString.c_str()); }

  /// Returns the native format by C-style string.
  const ValueType* c_str() const
  { return m_PathName.c_str(); }

  /// @name Decomposition
  /// @{

  /// Returns the parent path. For example, if the path name is ./abc/XYZ/file,
  /// then its parent path is './abc/XYZ'
  Path parent_path() const;

  /// Returns the file name. For example, if the path name is ./abc/XYZ/file,
  /// then its parent path is 'file'.
  Path filename() const;

  /// Return the stem of the file name. For example, if the path name is
  /// ./abc/XYZ/stem.extension, then its stem is 'stem'.
  Path stem() const;

  /// Return the extension of the file name. For example, if the path name is
  /// ./abc/XYZ/stem.extension, then its extension is 'extension'.
  Path extension() const;

  /// @}

  /// Return the canonical form of the path name. A canonical form is a path
  /// that has no elements which are symbolic links, and not dot or dot dot
  /// elements.
  StringType generic_string() const;

  /// Change the native format to canonical form. A canonical form is a path
  /// that has no elements which are symbolic links, and not dot or dot dot
  bool canonicalize();

protected:
  StringType::size_type AppendSeparatorAsNeeded();
  void EraseRedundantSeparator(StringType::size_type sep_pos);

protected:
  StringType m_PathName;
};

bool operator==(const Path& pLHS,const Path& pRHS);
bool operator!=(const Path& pLHS,const Path& pRHS);
Path operator+(const Path& pLHS, const Path& pRHS);

//===----------------------------------------------------------------------===//
// System dependent non-member functions
//===----------------------------------------------------------------------===//
namespace sys {

/// Canonicalizes the path.
size_t canonicalize(Path::StringType& pPathName);

/// Check if \ref pValue is the separator of the path in the system.
bool is_separator(char pValue);

} // namespace of sys

//===----------------------------------------------------------------------===//
// Non-member functions
//===----------------------------------------------------------------------===//
template <class Char, class Traits>
inline std::basic_ostream<Char, Traits>&
operator<<(std::basic_ostream<Char, Traits>& pOS, const Path& pPath)
{
  return pOS << pPath.native();
}

template <class Char, class Traits>
inline std::basic_istream<Char, Traits>&
operator>>(std::basic_istream<Char, Traits>& pOS, Path& pPath)
{
  return pOS >> pPath.native();
}

//===----------------------------------------------------------------------===//
// class path member template implementation
//===----------------------------------------------------------------------===//
template <class InputIterator>
Path& Path::assign(InputIterator begin, InputIterator end)
{
  m_PathName.clear();
  if (begin != end)
    m_PathName.append<InputIterator>(begin, end);
  return *this;
}

template <class InputIterator>
Path& Path::append(InputIterator begin, InputIterator end)
{
  if (begin == end)
    return *this;
  StringType::size_type sep_pos(AppendSeparatorAsNeeded());
  m_PathName.append<InputIterator>(begin, end);
  if (sep_pos)
    EraseRedundantSeparator(sep_pos);
  return *this;
}

} // namespace of skypat

//===----------------------------------------------------------------------===//
// STL compatible functions
//===----------------------------------------------------------------------===//
namespace std {

template<>
struct less<skypat::Path> : public binary_function<skypat::Path,
                                                     skypat::Path,
                                                     bool>
{
  bool operator() (const skypat::Path& pX,const skypat::Path& pY) const {
    if (pX.generic_string().size() < pY.generic_string().size())
      return true;
    return (pX.generic_string() < pY.generic_string());
  }
};

} // namespace of std

#endif
