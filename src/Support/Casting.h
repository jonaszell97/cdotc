//
// Created by Jonas Zell on 19.11.17.
//

#ifndef CDOT_CASTING_H
#define CDOT_CASTING_H

#include <cassert>

namespace cdot {

template <class T, class U>
bool isa(const U* v)
{
   return T::classof(v);
}

template<class T, class U>
bool isa(const std::shared_ptr<U> &v)
{
   return T::classof(v.get());
};

template <class T, class U>
const T* cast(const U* v)
{
   assert(T::classof(v) && "Check isa<> before casting");
   return static_cast<const T*>(v);
}

template <class T, class U>
T* cast(const std::shared_ptr<U> v)
{
   assert(T::classof(v.get()) && "Check isa<> before casting");
   return static_cast<T*>(v.get());
}

template <class T, class U>
T* cast(U* v)
{
   assert(T::classof(v) && "Check isa<> before casting");
   return static_cast<T*>(v);
}

template <class T, class U>
const T* dyn_cast(const U* v)
{
   if (!T::classof(v)) {
      return nullptr;
   }

   return static_cast<const T*>(v);
}

template <class T, class U>
T* dyn_cast(U* v)
{
   if (!T::classof(v)) {
      return nullptr;
   }

   return static_cast<T*>(v);
}

template <class T, class U>
T* dyn_cast(const std::shared_ptr<U> v)
{
   if (!T::classof(v.get())) {
      return nullptr;
   }

   return static_cast<T*>(v.get());
}

} // namespace cdot

#endif //CDOT_CASTING_H
