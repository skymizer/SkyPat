//===- ManagedStatic.h -----------------------------------------------------===//
//
//                     The SkyPat Team
//
// This file is distributed under the New BSD License. 
// See LICENSE for details.
//
//===----------------------------------------------------------------------===//
#ifndef SKYPAT_SUPPORT_MANAGED_STATIC_H
#define SKYPAT_SUPPORT_MANAGED_STATIC_H
#include <algorithm>
#include <skypat/skypat.h>

namespace skypat {

template <class Container, typename Functor>
void ForEach(const Container& c, Functor functor) {
  std::for_each(c.begin(), c.end(), functor);
}

template<class C>
void* object_creator() {
  return new C();
}

template<typename T> struct object_deleter {
  static void call(void * Ptr) { delete (T*)Ptr; }
};

/// ManagedStaticBase - Common base class for ManagedStatic instances.
class ManagedStaticBase
{
public:
  bool isConstructed() const { return (NULL != m_Ptr); }

  void destroy() const;

protected:
  typedef void (*DeleterFuncType)(void*);

protected:
  // This should only be used as a static variable, which guarantees that this
  // will be zero initialized.
  mutable void* m_Ptr;
  mutable DeleterFuncType m_pDeleter;
  mutable const ManagedStaticBase* m_pNext;

  void RegisterManagedStatic(void *(*creator)(), void (*deleter)(void*)) const;

};

/** \class ManagedStatic
 *  \brief ManagedStatic changes the behavior of global static variables to
 *  be lazily constructed on demand and explicitly destructed by Shutdown()
 *  function call.
 */
template<class C>
class ManagedStatic : public ManagedStaticBase
{
public:

  // Accessors.
  C &operator*() {
    void* tmp = m_Ptr;
    if (NULL == tmp)
      RegisterManagedStatic(object_creator<C>, object_deleter<C>::call);
    return *static_cast<C*>(m_Ptr);
  }

  const C &operator*() const {
    void* tmp = m_Ptr;
    if (NULL == tmp)
      RegisterManagedStatic(object_creator<C>, object_deleter<C>::call);
    return *static_cast<C*>(m_Ptr);
  }

  C *operator->() {
    void* tmp = m_Ptr;
    if (NULL == tmp)
      RegisterManagedStatic(object_creator<C>, object_deleter<C>::call);

    return static_cast<C*>(m_Ptr);
  }

  const C *operator->() const {
    void* tmp = m_Ptr;
    if (NULL == tmp)
      RegisterManagedStatic(object_creator<C>, object_deleter<C>::call);

    return static_cast<C*>(m_Ptr);
  }
};

/// shutdown - Deallocate and destroy all ManagedStatic variables.
void shutdown();

/** \class Shutdown
 *  \brief Shutdown is a helper class that call shutdown() when it's destroyed.
 */
class Shutdown : private testing::internal::Uncopyable
{
public:
  Shutdown() { }

  ~Shutdown() { shutdown(); }
};

} // namespace of skypat

#endif
