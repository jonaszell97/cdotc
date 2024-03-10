#ifndef CDOT_OPTIONAL_H
#define CDOT_OPTIONAL_H

#include <optional>

namespace cdot {

template<class T> using Optional = std::optional<T>;
const std::nullopt_t None = std::nullopt;

namespace detail {

template<class T> struct remove_reference {
   using type = T;
};

template<class T> struct remove_reference<T&> {
   using type = T;
};

template<class T> struct remove_reference<T&&> {
   using type = T;
};

} // namespace detail

template<class T>
Optional<typename detail::remove_reference<T>::type> Some(T&& t)
{
   return Optional<typename detail::remove_reference<T>::type>(
       std::forward<T&&>(t));
}

} // namespace cdot

#endif // CDOT_OPTIONAL_H
