//===- TypeTraits.h -------------------------------------------------------===//
//
//                              The SkyPat team 
//
// This file is distributed under the New BSD License.
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#ifndef SKYPAT_TYPE_TRAITS_H
#define SKYPAT_TYPE_TRAITS_H

#include <cstdlib>

namespace skypat {

typedef void * HANDLE;

template<typename DataType>
struct NonConstTraits;

template<typename DataType>
struct ConstTraits
{
  typedef const DataType           value_type;
  typedef const DataType*          pointer;
  typedef const DataType&          reference;
  typedef size_t                   size_type;
  typedef ConstTraits<DataType>    const_traits;
  typedef NonConstTraits<DataType> nonconst_traits;

  template<typename T>
  struct rebind {
    typedef ConstTraits<T> other;
  };
};

template<typename DataType>
struct NonConstTraits
{
  typedef DataType                 value_type;
  typedef DataType*                pointer;
  typedef DataType&                reference;
  typedef size_t                   size_type;
  typedef ConstTraits<DataType>    const_traits;
  typedef NonConstTraits<DataType> nonconst_traits;

  template<typename T>
  struct rebind {
    typedef NonConstTraits<T> other;
  };
};

template<typename DataType>
struct ConstIteratorTraits
{
  typedef DataType                          value_type;
  typedef const DataType*                   pointer;
  typedef const DataType&                   reference;
  typedef size_t                            size_type;
  typedef ConstTraits<DataType>             const_traits;
  typedef NonConstTraits<DataType>          nonconst_traits;
  typedef typename DataType::const_iterator iterator;
};

template<typename DataType>
struct NonConstIteratorTraits
{
  typedef DataType                    value_type;
  typedef DataType*                   pointer;
  typedef DataType&                   reference;
  typedef size_t                      size_type;
  typedef ConstTraits<DataType>       const_traits;
  typedef NonConstTraits<DataType>    nonconst_traits;
  typedef typename DataType::iterator iterator;
};

} // namespace of skypat

#endif

