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
   static ConstantInt *get(Type *ty, uint64_t value);
   static ConstantInt *getCTFE();

   uint64_t getU64() const;
   uint32_t getU32() const;
   uint16_t getU16() const;
   uint8_t getU8() const;
   bool getU1() const;
   bool isUnsigned() const;

   bool isCTFE() const;

private:
   ConstantInt(Type *ty, uint64_t value);

   enum Flags {
      Ctfe = 1 << 2
   };

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

class ConstantPointer: public Constant {
public:
   static ConstantPointer *get(Type *ty, uintptr_t value);
   static ConstantPointer *getNull(Type *ty) { return get(ty, 0); }

   uintptr_t getValue() const
   {
      return value;
   }

private:
   ConstantPointer(Type *ty, uintptr_t value);
   uintptr_t value;

public:
   static inline bool classof(Value const* T) {
      return T->getTypeID() == ConstantPointerID;
   }
};

class ConstantFloat: public Constant {
public:
   static ConstantFloat *get(double val);
   static ConstantFloat *get(float val);

   double getDoubleVal() const;
   float getFloatVal() const;

private:
   explicit ConstantFloat(double val);
   explicit ConstantFloat(float val);

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
   static ConstantString *get(const std::string &val);

   const std::string &getValue() const;

private:
   explicit ConstantString(const std::string &val);
   std::string value;

public:
   static inline bool classof(Value const* T) {
      return T->getTypeID() == ConstantStringID;
   }
};

class ConstantArray: public Constant {
public:
   typedef llvm::SmallVector<Constant*, 4> ArrayTy;

   static ConstantArray *get(ArrayTy &&Arr);
   static ConstantArray *get(llvm::ArrayRef<Constant*> vec);
   static ConstantArray *get(Type *ty, size_t numElements);

   const ArrayTy &getVec() const;
   size_t getNumElements() const;

   Type *getElementType() const;

protected:
   explicit ConstantArray(ArrayTy &&Arr, bool useInt8PtrTy = false);
   explicit ConstantArray(llvm::ArrayRef<Constant*> vec);
   ConstantArray(Type *ty, size_t numElements);

   ArrayTy vec;
   Type *elementType;
   size_t numElements;

public:
   static inline bool classof(Value const* T) {
      switch (T->getTypeID()) {
         case ConstantArrayID:
         case VTableID:
         case PTableID:
            return true;
         default:
            return false;
      }
   }
};

class VTable: public ConstantArray {
public:
   static VTable *get(ArrayTy &&Arr, ClassType *Owner);

   ClassType *getOwner() const
   {
      return Owner;
   }

public:
   explicit VTable(ArrayTy &&Arr, ClassType *Owner);
   ClassType *Owner;

public:
   static inline bool classof(Value const* T) {
      return T->getTypeID() == VTableID;
   }
};

class PTable: public ConstantArray {
public:
   typedef llvm::StringMap<size_t> PositionMap;

   static PTable *get(ArrayTy &&Arr, PositionMap &&PosMap, AggregateType *Owner);

   AggregateType *getOwner() const
   {
      return Owner;
   }

   const PositionMap &getContainedProtocols() const
   {
      return ContainedProtocols;
   }

private:
   explicit PTable(ArrayTy &&Arr, PositionMap &&PosMap, AggregateType *Owner);

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

   static ConstantStruct *get(AggregateType *structTy,
                              llvm::ArrayRef<Constant*> vec);

   AggregateType *getStructTy() const
   {
      return structTy;
   }

   const ElementTy &getElements() const;

protected:
   ConstantStruct(AggregateType *structTy,
                  llvm::ArrayRef<Constant*> vec);

   ConstantStruct(TypeID id,
                  AggregateType *structTy,
                  llvm::ArrayRef<Constant*> vec);

   AggregateType *structTy;
   ElementTy elements;

public:
   static inline bool classof(Value const* T)
   {
      switch (T->getTypeID()) {
         case TypeID::ConstantStructID:
         case TypeID::TypeInfoID:
            return true;
         default:
            return false;
      }
   }
};

class TypeInfo: public ConstantStruct {
public:
   static TypeInfo *get(Module *M, Type* forType,
                        il::Constant *ParentClass,
                        il::Constant *TypeID,
                        il::Constant *TypeName,
                        il::Constant *Deinitializer,
                        il::Constant *NumConformances,
                        il::Constant *Conformances);

   il::Constant* getParentClass()     const { return elements[0]; }
   il::Constant* getTypeID()          const { return elements[1]; }
   il::Constant* getTypeName()        const { return elements[2]; }
   il::Constant* getDeinitializer()   const { return elements[3]; }
   il::Constant* getNumConformances() const { return elements[4]; }
   il::Constant* getConformances()    const { return elements[5]; }

   Type *getForType() const { return forType; }

private:
   TypeInfo(Module *M, Type* forType,
            il::Constant *ParentClass,
            il::Constant *TypeID,
            il::Constant *TypeName,
            il::Constant *Deinitializer,
            il::Constant *NumConformances,
            il::Constant *Conformances);

   Type* forType;

public:
   static inline bool classof(Value const* T)
   {
      return T->getTypeID() == TypeInfoID;

   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_CONSTANTINT_H
