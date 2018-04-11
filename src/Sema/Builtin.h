//
// Created by Jonas Zell on 20.08.17.
//

#ifndef CDOT_BUILTIN_H
#define CDOT_BUILTIN_H

namespace cdot {

enum class BuiltinFn : unsigned char {
   None = 0,
   DeclType,
   MEMCPY,
   MEMSET,
   MemCmp,
   ISNULL,
   BuiltinSizeof,
   CtfePrintStackTrace,

   SIZEOF,
   ALIGNOF,
   NULLPTR,
   DefaultVal,

   STACK_ALLOC,
   BITCAST
};

enum class KnownFunction: unsigned char {
   Unchecked = 0, None,

   Malloc, Free, Printf, MemCpy, MemSet, MemCmp, Exit, Abort, System,
   Srand, Rand, Time, Sleep,

   IsAlpha, IsDigit, IsPrint,

   llvm_sqrt_f32, llvm_sqrt_f64,
   llvm_powi_f32, llvm_powi_f64,
   llvm_pow_f32,  llvm_pow_f64,
   llvm_log10_f64, llvm_log2_f64,
   llvm_log_f64,

   llvm_floor_f64, llvm_ceil_f64,
   llvm_floor_f32, llvm_ceil_f32,
};

} // namespace cdot

#endif //CDOT_BUILTIN_H
