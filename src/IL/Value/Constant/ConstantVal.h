//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_CONSTANTINT_H
#define CDOT_CONSTANTINT_H

#include <cstdint>
#include <string>
#include <llvm/ADT/ArrayRef.h>
#include "Constant.h"
#include "../Record/AggregateType.h"

namespace cdot {
namespace il {

class ConstantInt: public Constant {
public:
   static ConstantInt* get(Type *ty, uint64_t value)
   {
      return new ConstantInt(ty, value);
   }

   uint64_t getU64() const
   {
      return u64;
   }

   uint32_t getU32() const
   {
      return u32;
   }

   uint16_t getU16() const
   {
      return u16;
   }

   uint8_t getU8() const
   {
      return u8;
   }

   bool getU1() const
   {
      return u1;
   }

   bool isUnsigned() const
   {
      return type->isUnsigned();
   }

   static ConstantInt *getCTFE(IntegerType *BoolTy)
   {
      auto I = get(BoolTy, 0);
      I->u64 = Flags::Ctfe;

      return I;
   }

   bool isCTFE() const
   {
      if (!type->isInt1Ty())
         return false;

      return (u64 & Flags::Ctfe) != 0;
   }

private:
   ConstantInt(Type *ty, uint64_t value)
      : Constant(ConstantIntID, ty), u64(value)
   {
      assert(ty->isIntegerType());
   }

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
   static ConstantFloat* get(FPType *Ty, float val)
   {
      return new ConstantFloat(Ty, val);
   }

   static ConstantFloat* get(FPType *Ty, double val)
   {
      return new ConstantFloat(Ty, val);
   }

   double getDoubleVal() const
   {
      return doubleVal;
   }

   float getFloatVal() const
   {
      return floatVal;
   }

private:
   explicit ConstantFloat(FPType *Ty, double val)
      : Constant(ConstantFloatID, Ty), doubleVal(val)
   {

   }

   explicit ConstantFloat(FPType *Ty, float val)
      : Constant(ConstantFloatID, Ty), floatVal(val)
   {

   }

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
   static ConstantString *get(PointerType *Int8PtrTy,
                              const std::string &val);

   const std::string &getValue() const { return value; }

private:
   explicit ConstantString(PointerType *Int8PtrTy, llvm::StringRef val)
      : Constant(ConstantStringID, Int8PtrTy),
        value(val)
   {

   }

   std::string value;

public:
   static inline bool classof(Value const* T) {
      return T->getTypeID() == ConstantStringID;
   }
};

class ConstantArray: public Constant {
public:
   typedef std::vector<Constant*> ArrayTy;

   static ConstantArray *get(ArrayType *ty, ArrayTy &&Arr);
   static ConstantArray *get(ArrayType *ty, llvm::ArrayRef<Constant*> vec);
   static ConstantArray *get(ArrayType *ty);

   const ArrayTy &getVec() const { return vec; }

   size_t getNumElements() const
   {
      return support::cast<ArrayType>(type)->getNumElements();
   }

   QualType getElementType() const
   {
      return support::cast<ArrayType>(type)->getElementType();
   }

protected:
   explicit ConstantArray(ArrayType *ty, ArrayTy &&Arr)
      : Constant(ConstantArrayID, ty), vec(std::move(Arr))
   {}

   explicit ConstantArray(ArrayType *ty, llvm::ArrayRef<Constant*> vec = {})
      : Constant(ConstantArrayID, ty), vec(vec.begin(), vec.end())
   {}

   ArrayTy vec;

public:
   static inline bool classof(Value const* T) {
      switch (T->getTypeID()) {
         case ConstantArrayID:
            return true;
         default:
            return false;
      }
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
