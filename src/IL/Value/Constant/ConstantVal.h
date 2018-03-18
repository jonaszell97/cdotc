//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_CONSTANTINT_H
#define CDOT_CONSTANTINT_H

#include <cstdint>
#include <string>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/APSInt.h>
#include <llvm/ADT/APFloat.h>
#include "Constant.h"
#include "../Record/AggregateType.h"

namespace cdot {
namespace il {

class ConstantInt: public Constant {
public:
   static ConstantInt* get(ValueType ty, uint64_t value);
   static ConstantInt* get(ValueType ty, llvm::APSInt &&value);
   static ConstantInt* getTrue(Context &Ctx);
   static ConstantInt* getFalse(Context &Ctx);

   uint64_t getZExtValue() const
   {
      return Val.getZExtValue();
   }

   int64_t getSExtValue() const
   {
      return Val.getSExtValue();
   }

   const llvm::APSInt &getValue() const
   {
      return Val;
   }

   uint32_t getU32() const { return uint32_t(Val.getZExtValue()); }
   bool getBoolValue() const { return Val.getBoolValue(); }

   bool isUnsigned() const
   {
      return type->isUnsigned();
   }

   static ConstantInt *getCTFE(ValueType BoolTy)
   {
      return get(BoolTy, Flags::Ctfe);
   }

   bool isCTFE() const
   {
      if (!type->isInt1Ty())
         return false;

      return (Val.getZExtValue() & Flags::Ctfe) != 0;
   }

private:
   ConstantInt(const ValueType &ty, llvm::APSInt &&value);

   enum Flags {
      Ctfe = 1 << 2
   };

   llvm::APSInt Val;

public:
   static inline bool classof(Value const* T) {
      return T->getTypeID() == ConstantIntID;
   }
};

class ConstantPointer: public Constant {
public:
   static ConstantPointer *get(ValueType ty, uintptr_t value);
   static ConstantPointer *getNull(ValueType ty) { return get(ty, 0); }

   uintptr_t getValue() const
   {
      return value;
   }

private:
   ConstantPointer(ValueType ty, uintptr_t value);
   uintptr_t value;

public:
   static inline bool classof(Value const* T) {
      return T->getTypeID() == ConstantPointerID;
   }
};

class ConstantFloat: public Constant {
public:
   static ConstantFloat* get(ValueType Ty, float val);
   static ConstantFloat* get(ValueType Ty, double val);
   static ConstantFloat* get(ValueType Ty, llvm::APFloat &&APF);

   double getDoubleVal() const { return Val.convertToDouble(); }
   float getFloatVal() const { return Val.convertToFloat(); }

   const llvm::APFloat getValue() const { return Val; }

private:
   ConstantFloat(ValueType Ty, llvm::APFloat &&APF)
      : Constant(ConstantFloatID, Ty), Val(std::move(APF))
   {

   }

   llvm::APFloat Val;

public:
   static inline bool classof(Value const* T) {
      return T->getTypeID() == ConstantFloatID;
   }
};

class ConstantString: public Constant {
public:
   static ConstantString *get(Context &Ctx,
                              llvm::StringRef val);

   const std::string &getValue() const { return value; }

private:
   explicit ConstantString(ValueType Int8PtrTy, llvm::StringRef val)
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

   static ConstantArray *get(ValueType ty, ArrayTy &&Arr);
   static ConstantArray *get(ValueType ty, llvm::ArrayRef<Constant*> vec);
   static ConstantArray *get(ValueType ty);

   const ArrayTy &getVec() const { return vec; }

   size_t getNumElements() const
   {
      return type->asArrayType()->getNumElements();
   }

   QualType getElementType() const
   {
      return type->asArrayType()->getElementType();
   }

protected:
   explicit ConstantArray(ValueType ty, ArrayTy &&Arr)
      : Constant(ConstantArrayID, ty), vec(std::move(Arr))
   {}

   explicit ConstantArray(ValueType ty, llvm::ArrayRef<Constant*> vec = {})
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
