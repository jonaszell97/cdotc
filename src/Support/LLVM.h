//
// Created by Jonas Zell on 17.01.18.
//

#ifndef CDOT_LLVM_H
#define CDOT_LLVM_H

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

   class raw_ostream;
   class APInt;
   class APFloat;
   class MemoryBuffer;
} // end namespace llvm

#endif //CDOT_LLVM_H
