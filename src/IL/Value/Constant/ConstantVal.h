//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_CONSTANTINT_H
#define CDOT_CONSTANTINT_H


#include <string>
#include <llvm/ADT/ArrayRef.h>
#include "Constant.h"
#include "../Record/AggregateType.h"

namespace cdot {
namespace il {

class ConstantInt: public Constant {
public:
   ConstantInt(Type *ty, uint64_t value);

   uint64_t getU64() const;
   uint32_t getU32() const;
   uint16_t getU16() const;
   uint8_t getU8() const;
   bool getU1() const;
   bool isUnsigned() const;

protected:
   union {
      uint64_t u64;
      uint32_t u32;
      uint16_t u16;
      uint8_t u8;
      bool u1;
   };

public:
   static inline bool classof(Value const* T) {
      return T->getTypeID() == ConstantIntID;
   }
};

class ConstantFloat: public Constant {
public:
   explicit ConstantFloat(double val);
   explicit ConstantFloat(float val);

   double getDoubleVal() const;
   float getFloatVal() const;

protected:
   union {
      double doubleVal;
      float floatVal;
   };

public:
   static inline bool classof(Value const* T) {
      return T->getTypeID() == ConstantFloatID;
   }
};

class ConstantString: public Constant {
public:
   explicit ConstantString(const std::string &val);

   const std::string &getValue() const;

protected:
   std::string value;

public:
   static inline bool classof(Value const* T) {
      return T->getTypeID() == ConstantStringID;
   }
};

class ConstantArray: public Constant {
public:
   typedef llvm::SmallVector<Constant*, 4> ArrayTy;

   explicit ConstantArray(ArrayTy &&Arr);
   explicit ConstantArray(llvm::ArrayRef<Constant*> vec);
   ConstantArray(Type *ty, size_t numElements);

   const ArrayTy &getVec() const;
   size_t getNumElements() const;

   Type *getElementType() const;

protected:
   ArrayTy vec;
   Type *elementType;
   size_t numElements;

public:
   static inline bool classof(Value const* T) {
      switch (T->getTypeID()) {
         case ConstantArrayID:
         case VTableID:
            return true;
         default:
            return false;
      }
   }
};

class VTable: public ConstantArray {
public:
   explicit VTable(ArrayTy &&Arr, ClassType *Owner);

   ClassType *getOwner() const
   {
      return Owner;
   }

protected:
   ClassType *Owner;

public:
   static inline bool classof(Value const* T) {
      return T->getTypeID() == VTableID;
   }
};

class PTable: public ConstantArray {
public:
   typedef llvm::DenseMap<size_t, llvm::StringRef> PositionMap;
   explicit PTable(ArrayTy &&Arr, PositionMap &&PosMap, ClassType *Owner);

   AggregateType *getOwner() const
   {
      return Owner;
   }

   const PositionMap &getContainedProtocols() const
   {
      return ContainedProtocols;
   }

protected:
   AggregateType *Owner;
   PositionMap ContainedProtocols;

public:
   static inline bool classof(Value const* T) {
      return T->getTypeID() == PTableID;
   }
};

class ConstantStruct: public Constant {
public:
   typedef llvm::SmallVector<Constant*, 8> ElementTy;
   explicit ConstantStruct(AggregateType *structTy,
                           llvm::ArrayRef<Constant*> vec);

   AggregateType *getStructTy() const
   {
      return structTy;
   }

   const ElementTy &getElements() const;

protected:
   AggregateType *structTy;
   ElementTy elements;

public:
   static inline bool classof(Value const* T) {
      return T->getTypeID() == ConstantStructID;
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_CONSTANTINT_H
