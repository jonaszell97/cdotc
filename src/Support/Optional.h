#ifndef CDOT_OPTIONAL_H
#define CDOT_OPTIONAL_H

#include <llvm/ADT/Optional.h>

namespace cdot {

template<class T>
using Optional = llvm::Optional<T>;
const llvm::NoneType None = llvm::None;

namespace detail {

template<class T>
struct remove_reference { using type = T; };

template<class T>
struct remove_reference<T&> { using type = T; };

template<class T>
struct remove_reference<T&&> { using type = T; };

} // namespace detail

template<class T>
Optional<typename detail::remove_reference<T>::type> Some(T &&t)
{
   return Optional<typename detail::remove_reference<T>::type>(
      std::forward<T&&>(t));
}

} // namespace cdot

#endif //CDOT_OPTIONAL_H
