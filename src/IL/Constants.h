//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_CONSTANTINT_H
#define CDOT_CONSTANTINT_H

#include "Constant.h"

#include "Basic/CastKind.h"

#include <cstdint>
#include <string>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/APSInt.h>
#include <llvm/ADT/APFloat.h>

namespace cdot {
namespace ast {
   class ClassDecl;
} // namespace ast

namespace il {

class Module;
class Function;

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
   static ConstantPointer *get(ValueType ty);

private:
   ConstantPointer(ValueType ty);

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

class ConstantArray final: public Constant {
   explicit ConstantArray(ValueType ty, llvm::ArrayRef<Constant*> vec);

   unsigned NumElements;

public:
   static ConstantArray *get(ValueType ty, llvm::ArrayRef<Constant*> vec);

   llvm::ArrayRef<Constant*> getVec() const
   {
      return { reinterpret_cast<Constant* const*>(this + 1), NumElements };
   }

   unsigned getNumElements() const { return NumElements; }
   QualType getElementType() const
   {
      return type->asArrayType()->getElementType();
   }

   static inline bool classof(Value const* T)
   {
      return T->getTypeID() == ConstantArrayID;
   }
};

class ConstantStruct final: public Constant {
   ConstantStruct(ValueType Ty,
                  llvm::ArrayRef<Constant*> vec);

   ConstantStruct(TypeID id,
                  ValueType Ty,
                  llvm::ArrayRef<Constant*> vec);

   unsigned NumElements;

public:
   static ConstantStruct *get(ValueType Ty,
                              llvm::ArrayRef<Constant*> vec);

   llvm::ArrayRef<Constant*> getElements() const
   {
      return { reinterpret_cast<Constant* const*>(this + 1), NumElements };
   }

   static inline bool classof(Value const* T)
   {
     return T->getTypeID() == ConstantStructID;
   }
};

class VTable: public Constant {
   VTable(il::Context &Ctx,
          llvm::ArrayRef<il::Function*> Entries,
          ast::ClassDecl *C);

   unsigned NumFunctions;
   ast::ClassDecl *C;

public:
   static VTable *Create(il::Context &Ctx,
                         llvm::ArrayRef<il::Function*> Entries,
                         ast::ClassDecl *C);

   ast::ClassDecl *getClassDecl() const { return C; }
   unsigned getNumFunctions() const { return NumFunctions; }

   llvm::ArrayRef<Function*> getFunctions() const
   {
      return { reinterpret_cast<Function *const*>(this + 1), NumFunctions };
   }

   static inline bool classof(Value const* T)
   {
      return T->getTypeID() == VTableID;
   }
};

class TypeInfo: public Constant {
   TypeInfo(Module *M, QualType forType,
            llvm::ArrayRef<il::Constant*> Vals);

   QualType forType;
   il::Constant *Vals[MetaType::MemberCount];

public:
   static TypeInfo *get(Module *M, QualType forType,
                        llvm::ArrayRef<il::Constant*> Vals);

   llvm::ArrayRef<Constant*> getValues() const
   {
      return Vals;
   }

   il::Constant* getParentClass() const { return Vals[MetaType::BaseClass]; }
   il::Constant* getVTable() const { return Vals[MetaType::VTable]; }
   il::Constant* getPTable() const { return Vals[MetaType::PTable]; }
   il::Constant* getTypeName() const { return Vals[MetaType::Name]; }
   il::Constant* getDeinitializer()const {return Vals[MetaType::Deinitializer];}
   il::Constant* getConformances() const {return Vals[MetaType::Conformances];}

   QualType getForType() const { return forType; }

   static inline bool classof(Value const* T)
   {
      return T->getTypeID() == TypeInfoID;
   }
};

class ConstantBitCastInst;
class ConstantAddrOfInst;
class ConstantIntCastInst;

class ConstantExpr: public Constant {
public:
   static ConstantBitCastInst *getBitCast(Constant *Val, QualType toType);
   static ConstantAddrOfInst *getAddrOf(Constant *Val);
   static ConstantIntCastInst *getIntCast(CastKind kind,
                                          Constant *Target,
                                          QualType toType);

   static ConstantIntCastInst *getIntToPtr(Constant *Target,
                                           QualType toType) {
      return getIntCast(CastKind::IntToPtr, Target, toType);
   }

   static ConstantIntCastInst *getPtrToInt(Constant *Target,
                                           QualType toType) {
      return getIntCast(CastKind::PtrToInt, Target, toType);
   }

   static ConstantIntCastInst *getExt(Constant *Target,
                                      QualType toType) {
      return getIntCast(CastKind::Ext, Target, toType);
   }

   static ConstantIntCastInst *getTrunc(Constant *Target,
                                        QualType toType) {
      return getIntCast(CastKind::Trunc, Target, toType);
   }

   static ConstantIntCastInst *getSignCast(Constant *Target,
                                           QualType toType) {
      return getIntCast(CastKind::SignFlip, Target, toType);
   }

   static ConstantIntCastInst *getFPToInt(Constant *Target,
                                          QualType toType) {
      return getIntCast(CastKind::FPToInt, Target, toType);
   }

   static ConstantIntCastInst *getIntToFP(Constant *Target,
                                          QualType toType) {
      return getIntCast(CastKind::IntToFP, Target, toType);
   }

protected:
   ConstantExpr(TypeID id, ValueType ty);

   static inline bool classof(Value const* T)
   {
      auto kind = T->getTypeID();
      return kind > _firstConstExpr && kind < _lastConstExpr;
   }
};

class ConstantBitCastInst: public ConstantExpr {
private:
   Constant *target;

   ConstantBitCastInst(Constant *Val, ValueType toType);

public:
   friend class ConstantExpr;

   Constant *getTarget() const
   {
      return target;
   }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == ConstantBitCastInstID;
   }
};

class ConstantAddrOfInst: public ConstantExpr {
private:
   Constant *target;

   ConstantAddrOfInst(Constant *Val, ValueType PtrTy);

public:
   friend class ConstantExpr;

   Constant *getTarget() const
   {
      return target;
   }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == ConstantAddrOfInstID;
   }
};

class ConstantIntCastInst: public ConstantExpr {
   ConstantIntCastInst(CastKind kind,
                       Constant *Target,
                       QualType toType);

   Constant *Target;
   CastKind Kind;

public:
   friend class ConstantExpr;

   Constant *getTarget() const { return Target; }
   CastKind getKind() const { return Kind; }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == ConstantIntCastInstID;
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_CONSTANTINT_H
