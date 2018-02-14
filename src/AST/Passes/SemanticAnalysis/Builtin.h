//
// Created by Jonas Zell on 20.08.17.
//

#ifndef CDOT_BUILTIN_H
#define CDOT_BUILTIN_H

namespace cdot {

enum class BuiltinFn : unsigned char {
   None = 0,
   TYPEOF,
   MEMCPY,
   MEMSET,
   MemCmp,
   ISNULL,
   UNWRAP_PROTO,
   BuiltinSizeof,
   CtfePrintStackTrace,

   SIZEOF,
   ALIGNOF,
   NULLPTR,
   DefaultVal,

   STACK_ALLOC,
   BITCAST
};

} // namespace cdot

#endif //CDOT_BUILTIN_H
