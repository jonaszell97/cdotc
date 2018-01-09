//
// Created by Jonas Zell on 19.11.17.
//

#ifndef CDOT_CASTING_H
#define CDOT_CASTING_H

#include <cassert>
#include <llvm/Support/Casting.h>
#include <memory>

namespace cdot {
namespace support {

using llvm::dyn_cast;
using llvm::isa;
using llvm::cast;

template<class T, class U>
bool isa(const std::shared_ptr<U> &v)
{
   return T::classof(v.get());
};

template<class T, class U>
T *cast(const std::shared_ptr<U> &v)
{
   assert(T::classof(v.get()) && "Check isa<> before casting");
   return static_cast<T *>(v.get());
}

template<class T, class U>
T *dyn_cast(const std::shared_ptr<U> &v)
{
   if (!T::classof(v.get())) {
      return nullptr;
   }

   return static_cast<T *>(v.get());
}

template<class T, class U>
T *dyn_cast_if_not_null(const std::shared_ptr<U> &v)
{
   if (!v || !T::classof(v.get())) {
      return nullptr;
   }

   return static_cast<T *>(v.get());
}

template<class T, class U>
T *dyn_cast_if_not_null(U* v)
{
   if (!v || !T::classof(v)) {
      return nullptr;
   }

   return static_cast<T *>(v);
}

} // namespace support
} // namespace cdot

#endif //CDOT_CASTING_H
