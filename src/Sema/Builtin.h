#ifndef CDOT_BUILTIN_H
#define CDOT_BUILTIN_H

namespace cdot {

enum class KnownFunction: unsigned char {
   Unchecked = 0, None,

   Malloc, Free, Realloc, Printf, MemCpy, MemSet, MemCmp, Exit, Abort, System,
   Srand, Rand, Time, Sleep, PutChar,

   IsAlpha, IsDigit, IsPrint,

   llvm_sqrt_f32, llvm_sqrt_f64,
   llvm_powi_f32, llvm_powi_f64,
   llvm_pow_f32,  llvm_pow_f64,
   llvm_log10_f64, llvm_log2_f64,
   llvm_log_f64,

   llvm_floor_f64, llvm_ceil_f64,
   llvm_floor_f32, llvm_ceil_f32,

   llvm_ctlz_i32, llvm_ctlz_i64,
   llvm_cttz_i32, llvm_cttz_i64,
};

enum class BuiltinIdentifier: unsigned char {
   None, NULLPTR, FUNC, MANGLED_FUNC, FLOAT_QNAN, DOUBLE_QNAN,
   FLOAT_SNAN, DOUBLE_SNAN, __ctfe, defaultValue, __builtin_void
};

} // namespace cdot

#endif //CDOT_BUILTIN_H
