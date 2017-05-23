//===- Uncopyable.h -------------------------------------------------------===//
//
//                              The SkyPat team 
//
// This file is distributed under the New BSD License.
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#ifndef SKYPAT_ADT_UNCOPYABLE_H
#define SKYPAT_ADT_UNCOPYABLE_H

namespace skypat {

/** \class Uncopyable
 *  \brief Uncopyable provides the base class to forbit copy operations.
 *
 */
class Uncopyable
{
protected:
  Uncopyable() { }
  ~Uncopyable() { }

private:
  Uncopyable(const Uncopyable&); /// NOT TO IMPLEMENT
  Uncopyable& operator=(const Uncopyable&); /// NOT TO IMPLEMENT
};

} // namespace of skypat

#endif

