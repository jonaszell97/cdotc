//
// Created by Jonas Zell on 17.01.18.
//

#ifndef CDOT_LLVM_H
#define CDOT_LLVM_H

#include "Config.h"

namespace llvm {
   class StringRef;
   class Twine;
   template <typename T> class SmallPtrSetImpl;
   template <typename T, unsigned N> class SmallPtrSet;
   template <typename T> class SmallVectorImpl;
   template <typename T, unsigned N> class SmallVector;
   template <unsigned N> class SmallString;
   template<typename T> class ArrayRef;
   template<typename T> class MutableArrayRef;
   template<typename T> class TinyPtrVector;
   template<typename T> class Optional;
   template <typename PT1, typename PT2> class PointerUnion;
   template <typename BaseTy, typename... TrailingTys> class TrailingObjects;
   template <typename EltTy> class TinyPtrVector;

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

using llvm::MemoryBuffer;
using llvm::APInt;
using llvm::APSInt;
using llvm::APFloat;

using llvm::raw_ostream;

} // namespace cdot

#endif //CDOT_LLVM_H
