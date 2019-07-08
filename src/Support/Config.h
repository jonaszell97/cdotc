//
// Created by Jonas Zell on 25.08.18.
//

#ifndef CDOT_CONFIG_H
#define CDOT_CONFIG_H

#ifndef __has_feature
#  define __has_feature(x) 0
#endif

#ifndef __has_attribute
#  define __has_attribute(x) 0
#endif

#if __has_feature(address_sanitizer)
#  define CDOT_NO_SANITIZE(KIND) __attribute__((no_sanitize(#KIND)))
#else
#  define CDOT_NO_SANITIZE(KIND)
#endif

#if __has_attribute(abi_tag)
#  define CDOT_LLDB_STEP_OVER __attribute__((abi_tag("lldb_step_over")))
#else
#  define CDOT_LLDB_STEP_OVER
#endif

#define ASSERT_NOEXCEPT_MOVE_CONSTRUCTIBLE(TYPE)                  \
static_assert(std::is_nothrow_move_constructible<TYPE>::value,    \
              #TYPE " should be noexcept move constructible")

#endif //CDOT_CONFIG_H
