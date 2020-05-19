#ifndef CDOT_BUILTINS_H
#define CDOT_BUILTINS_H

namespace cdot {
namespace builtin {

enum Builtin : unsigned {
   InvalidBuiltin,

   allocStack,
   addressOf,

   loadFromPointer,
   storeToPointer,
   reinterpretCast,
   unsafeConstCast,
   constructInPlace,

   move,
   consume,
   copy,
   deinit,
   retainValue,
   releaseValue,

   printCTFEStacktrace,

   memcpy,
   memset,
   likely,
   unlikely,
   llvm_intrinsic,

   atomic_load,
   atomic_store,
   atomic_cmpxchg,
   atomic_rmw,

   _lastBuiltin,
};

enum BuiltinAlias : unsigned {
   InvalidAlias = _lastBuiltin,
   TokenType,
   CVoid,
   RawPointer,
   MutableRawPointer,
   Int1,
   Int8,
   UInt8,
   Int16,
   UInt16,
   Int32,
   UInt32,
   Int64,
   UInt64,
   Int128,
   UInt128,
   Float32,
   Float64,
   undefValue,
};

} // namespace builtin
} // namespace cdot

#endif // CDOT_BUILTINS_H
