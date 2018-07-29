//
// Created by Jonas Zell on 29.04.18.
//

#ifndef CDOT_BUILTINS_H
#define CDOT_BUILTINS_H

namespace cdot {
namespace builtin {

enum Builtin : unsigned {
   NoBuiltin,

   allocStack,
   addressOf,

   loadFromPointer, storeToPointer,
   reinterpretCast, unsafeConstCast,
   constructInPlace,

   copy, deinit, retainValue, releaseValue,

   printCTFEStacktrace,

   memcpy, memset,

   llvm_intrinsic,

   atomic_load, atomic_store, atomic_cmpxchg, atomic_rmw,

   _lastBuiltin,
};

enum BuiltinAlias : unsigned {
   NoAlias = _lastBuiltin,
   TokenType,
   CVoid,
   RawPointer,
   Int8, UInt8, Int16, UInt16, Int32, UInt32, Int64, UInt64,
   Int128, UInt128,
   undefValue,
};

} // namespace builtin
} // namespace cdot

#endif //CDOT_BUILTINS_H
