#ifndef CDOT_LLVM_H
#define CDOT_LLVM_H

#include "cdotc/Support/Config.h"

namespace llvm {
class StringRef;
class Twine;
template<typename T> class SmallPtrSetImpl;
template<typename T, unsigned N> class SmallPtrSet;
template<typename T> class SmallVectorImpl;
template<typename T, unsigned N> class SmallVector;
template<unsigned N> class SmallString;
template<typename T> class ArrayRef;
template<typename T> class MutableArrayRef;
template<typename T> class TinyPtrVector;
template<typename T> class Optional;
template<typename ...PTs> class PointerUnion;
template<typename BaseTy, typename... TrailingTys> class TrailingObjects;
template<typename EltTy> class TinyPtrVector;
template<typename ValueT, typename ValueInfoT> class DenseSet;
template<typename ValueT, unsigned InlineBuckets, typename ValueInfoT>
class SmallDenseSet;
template<typename ValueT, typename MapTy, typename ValueInfoT>
class DenseSetImpl;

class raw_ostream;
class APInt;
class APSInt;
class APFloat;
class MemoryBuffer;
} // end namespace llvm

namespace cdot {

using llvm::SmallVector;
using llvm::SmallVectorImpl;
using llvm::SmallString;
using llvm::SmallPtrSet;
using llvm::SmallPtrSetImpl;
using llvm::StringRef;
using llvm::ArrayRef;
using llvm::MutableArrayRef;
using llvm::TrailingObjects;
using llvm::TinyPtrVector;

using llvm::DenseSet;
using llvm::SmallDenseSet;
using llvm::DenseSetImpl;

using llvm::MemoryBuffer;
using llvm::APInt;
using llvm::APSInt;
using llvm::APFloat;

using llvm::raw_ostream;

} // namespace cdot

#define ENUM_DENSE_MAP_INFO(TYPE)                                              \
   template<> struct DenseMapInfo<TYPE> {                                      \
      static TYPE getEmptyKey()                                                \
      {                                                                        \
         return (TYPE)DenseMapInfo<__underlying_type(TYPE)>::getEmptyKey();    \
      }                                                                        \
                                                                               \
      static TYPE getTombstoneKey()                                            \
      {                                                                        \
         return (                                                              \
             TYPE)DenseMapInfo<__underlying_type(TYPE)>::getTombstoneKey();    \
      }                                                                        \
                                                                               \
      static int getHashValue(const TYPE& V)                                   \
      {                                                                        \
         return DenseMapInfo<__underlying_type(TYPE)>::getHashValue(           \
             (__underlying_type(TYPE))V);                                      \
      }                                                                        \
                                                                               \
      static bool isEqual(const TYPE& LHS, const TYPE& RHS)                    \
      {                                                                        \
         return DenseMapInfo<__underlying_type(TYPE)>::isEqual(                \
             (__underlying_type(TYPE))LHS, (__underlying_type(TYPE))RHS);      \
      }                                                                        \
   }

#endif // CDOT_LLVM_H
