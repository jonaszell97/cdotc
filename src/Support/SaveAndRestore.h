//
// Created by Jonas Zell on 02.08.18.
//

#ifndef CDOT_SAVEANDRESTORE_H
#define CDOT_SAVEANDRESTORE_H

#include <llvm/Support/SaveAndRestore.h>

namespace cdot {
namespace support {

template<class T>
llvm::SaveAndRestore<T> saveAndRestore(T &X)
{
   return llvm::SaveAndRestore<T>(X);
}

template<class T>
llvm::SaveAndRestore<T> saveAndRestore(T &X, const T &NewValue)
{
   return llvm::SaveAndRestore<T>(X, NewValue);
}

namespace detail
{
   template<class T, class Container>
   struct StackRestore {
      StackRestore(T &&el, Container &container) : container(container)
      {
         container.push_back(el);
      }

      ~StackRestore()
      {
         container.pop_back();
      }

   private:
      Container &container;
   };
}

template<class T, class Container>
detail::StackRestore<T, Container> stackRestore(T &&el, Container &container)
{
   return detail::StackRestore<T, Container>(std::move(el), container);
}

} // namespace support
} // namespace cdot

#endif //CDOT_SAVEANDRESTORE_H
