#ifndef CDOT_CONSTANTINT_H
#define CDOT_CONSTANTINT_H

#include "cdotc/Basic/CastKind.h"
#include "cdotc/IL/Constant.h"

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APSInt.h>
#include <llvm/ADT/FoldingSet.h>

namespace cdot {
namespace ast {
class ClassDecl;
class EnumCaseDecl;
} // namespace ast

namespace il {

class Module;
class Function;
class GlobalVariable;

class ConstantInt : public Constant {
public:
   static ConstantInt* get(ValueType ty, uint64_t value);
   static ConstantInt* get(ValueType ty, llvm::APSInt&& value);
   static ConstantInt* getTrue(Context& Ctx);
   static ConstantInt* getFalse(Context& Ctx);

   uint64_t getZExtValue() const { return Val.getZExtValue(); }

   int64_t getSExtValue() const { return Val.getSExtValue(); }

   const llvm::APSInt& getValue() const { return Val; }

   uint32_t getU32() const { return uint32_t(Val.getZExtValue()); }
   bool getBoolValue() const { return Val.getBoolValue(); }

   bool isUnsigned() const { return type->isUnsigned(); }

   static ConstantInt* getCTFE(ValueType BoolTy)
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
   ConstantInt(const ValueType& ty, llvm::APSInt&& value);

   enum Flags { Ctfe = 1 << 2 };

   llvm::APSInt Val;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == ConstantIntID;
   }
};

class ConstantPointer : public Constant {
   explicit ConstantPointer(ValueType ty);

public:
   static ConstantPointer* get(ValueType ty);

   static bool classof(Value const* T)
   {
      return T->getTypeID() == ConstantPointerID;
   }
};

class ConstantTokenNone : public Constant {
   ConstantTokenNone(Context& Ctx);

public:
   static ConstantTokenNone* get(Context& Ctx);

   static bool classof(Value const* T)
   {
      return T->getTypeID() == ConstantTokenNoneID;
   }
};

class UndefValue : public Constant {
   UndefValue(ValueType Ty);

public:
   static UndefValue* get(ValueType Ty);

   static bool classof(Value const* T)
   {
      return T->getTypeID() == UndefValueID;
   }
};

class ConstantFloat : public Constant {
   ConstantFloat(ValueType Ty, llvm::APFloat&& APF)
       : Constant(ConstantFloatID, Ty), Val(std::move(APF))
   {
   }

   llvm::APFloat Val;

public:
   static ConstantFloat* get(ValueType Ty, float val);
   static ConstantFloat* get(ValueType Ty, double val);
   static ConstantFloat* get(ValueType Ty, llvm::APFloat&& APF);

   double getDoubleVal() const { return Val.convertToDouble(); }
   float getFloatVal() const { return (float)Val.convertToDouble(); }

   const llvm::APFloat getValue() const { return Val; }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == ConstantFloatID;
   }
};

class ConstantString : public Constant {
   explicit ConstantString(ValueType Int8PtrTy, llvm::StringRef val)
       : Constant(ConstantStringID, Int8PtrTy), value(val)
   {
   }

   std::string value;

public:
   static ConstantString* get(Context& Ctx, llvm::StringRef val);

   llvm::StringRef getValue() const { return value; }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == ConstantStringID;
   }
};

class ConstantArray final : public Constant, public llvm::FoldingSetNode {
   ConstantArray(ValueType ty, llvm::ArrayRef<Constant*> vec);

   unsigned NumElements;

public:
   static ConstantArray* get(ValueType ty, llvm::ArrayRef<Constant*> vec);
   static ConstantArray* getAllZeros(ValueType ty);

   llvm::ArrayRef<Constant*> getVec() const
   {
      return {reinterpret_cast<Constant* const*>(this + 1), NumElements};
   }

   unsigned getNumElements() const { return NumElements; }
   QualType getElementType() const
   {
      return type->asArrayType()->getElementType();
   }

   op_iterator op_begin_impl()
   {
      return reinterpret_cast<Constant**>(this + 1);
   }
   op_const_iterator op_begin_impl() const
   {
      return reinterpret_cast<Constant* const*>(this + 1);
   }

   unsigned getNumOperandsImpl() const { return NumElements; }

   void Profile(llvm::FoldingSetNodeID& ID) const
   {
      Profile(ID, getElementType(), getVec());
   }

   static void Profile(llvm::FoldingSetNodeID& ID, QualType ElementTy,
                       llvm::ArrayRef<Constant*> Values);

   static inline bool classof(Value const* T)
   {
      return T->getTypeID() == ConstantArrayID;
   }
};

class ConstantTuple final : public Constant, public llvm::FoldingSetNode {
   ConstantTuple(ValueType ty, llvm::ArrayRef<Constant*> vec);

   unsigned NumElements;

public:
   static ConstantTuple* get(ValueType ty, llvm::ArrayRef<Constant*> vec);
   static ConstantTuple* getEmpty(il::Context& Ctx);
   static ConstantTuple* getAllZeros(ValueType ty);

   llvm::ArrayRef<Constant*> getVec() const
   {
      return {reinterpret_cast<Constant* const*>(this + 1), NumElements};
   }

   unsigned getNumElements() const { return NumElements; }

   op_iterator op_begin_impl()
   {
      return reinterpret_cast<Constant**>(this + 1);
   }
   op_const_iterator op_begin_impl() const
   {
      return reinterpret_cast<Constant* const*>(this + 1);
   }

   unsigned getNumOperandsImpl() const { return NumElements; }

   void Profile(llvm::FoldingSetNodeID& ID) const
   {
      Profile(ID, getType(), getVec());
   }

   static void Profile(llvm::FoldingSetNodeID& ID, QualType Ty,
                       llvm::ArrayRef<Constant*> Values);

   static inline bool classof(Value const* T)
   {
      return T->getTypeID() == ConstantTupleID;
   }
};

class ConstantStruct final : public Constant, public llvm::FoldingSetNode {
   ConstantStruct(ValueType Ty, ArrayRef<Constant*> vec);

   ConstantStruct(TypeID id, ValueType Ty, ArrayRef<Constant*> vec);

   unsigned NumElements;

public:
   static ConstantStruct* get(ValueType Ty, ArrayRef<Constant*> vec);

   static ConstantStruct* getAllZeros(ValueType ty);

   unsigned int getNumElements() const { return NumElements; }

   llvm::ArrayRef<Constant*> getElements() const
   {
      return {reinterpret_cast<Constant* const*>(this + 1), NumElements};
   }

   op_iterator op_begin_impl()
   {
      return reinterpret_cast<Constant**>(this + 1);
   }
   op_const_iterator op_begin_impl() const
   {
      return reinterpret_cast<Constant* const*>(this + 1);
   }

   unsigned getNumOperandsImpl() const { return NumElements; }

   void Profile(llvm::FoldingSetNodeID& ID) const
   {
      Profile(ID, getType(), getElements());
   }

   static void Profile(llvm::FoldingSetNodeID& ID, QualType Type,
                       llvm::ArrayRef<Constant*> Elements);

   static inline bool classof(Value const* T)
   {
      return T->getTypeID() == ConstantStructID;
   }
};

class ConstantClass final : public Constant, public llvm::FoldingSetNode {
   ConstantClass(ConstantStruct* StructVal, GlobalVariable* TI,
                 ConstantClass* Base);

   ConstantClass(ValueType Ty, GlobalVariable* TI);

   enum {
      TypeInfo = 0,
      StructVal,
      Base,
   };

   Constant* Vals[3];

public:
   static ConstantClass* get(ConstantStruct* StructVal, GlobalVariable* TI,
                             ConstantClass* Base = nullptr);

   static ConstantClass* ForwardDeclare(ValueType Ty, GlobalVariable* TI);

   bool isForwardDecl() const { return !StructVal; }
   ConstantClass* ReplaceForwardDecl(ConstantStruct* Value,
                                     ConstantClass* Base = nullptr);

   GlobalVariable* getTypeInfo() const;
   ConstantClass* getBase() const
   {
      return support::cast_or_null<ConstantClass>(Vals[Base]);
   }

   ConstantStruct* getStructVal() const
   {
      return support::cast_or_null<ConstantStruct>(Vals[StructVal]);
   }

   llvm::ArrayRef<Constant*> getElements() const
   {
      assert(!isForwardDecl() && "forward declared ConstantClass!");
      return getStructVal()->getElements();
   }

   op_iterator op_begin_impl() { return Vals; }
   unsigned getNumOperandsImpl() const { return Vals[Base] ? 3 : 2; }

   void Profile(llvm::FoldingSetNodeID& ID) const
   {
      Profile(ID, getStructVal(), getBase());
   }

   static void Profile(llvm::FoldingSetNodeID& ID, ConstantStruct* StructVal,
                       ConstantClass* Base);

   static bool classof(Value const* T)
   {
      return T->getTypeID() == ConstantClassID;
   }
};

class ConstantUnion : public Constant, public llvm::FoldingSetNode {
   ConstantUnion(ValueType Ty, Constant* InitVal);

   Constant* InitVal;

public:
   static ConstantUnion* get(ValueType Ty, Constant* InitVal);

   Constant* getInitVal() const { return InitVal; }

   op_iterator op_begin_impl() { return &InitVal; }
   unsigned getNumOperandsImpl() const { return 1; }

   void Profile(llvm::FoldingSetNodeID& ID) const
   {
      Profile(ID, getType(), getInitVal());
   }

   static void Profile(llvm::FoldingSetNodeID& ID, QualType Type,
                       Constant* InitVal);

   static bool classof(Value const* T)
   {
      return T->getTypeID() == ConstantUnionID;
   }
};

class ConstantEnum : public Constant, public llvm::FoldingSetNode {
   ConstantEnum(il::Context& Ctx, ast::EnumCaseDecl* Case,
                llvm::ArrayRef<Constant*> vec);

   ast::EnumCaseDecl* Case;
   unsigned NumValues;

public:
   static ConstantEnum* get(il::Context& Ctx, ast::EnumCaseDecl* Case,
                            llvm::ArrayRef<Constant*> vec);

   Constant* getDiscriminator() const;
   unsigned getNumValues() const { return NumValues; }

   ast::EnumCaseDecl* getCase() const { return Case; }
   llvm::ArrayRef<Constant*> getCaseValues() const
   {
      return {reinterpret_cast<Constant* const*>(this + 1), NumValues};
   }

   op_iterator op_begin_impl()
   {
      return reinterpret_cast<Constant**>(this + 1);
   }
   unsigned getNumOperandsImpl() const { return NumValues; }

   void Profile(llvm::FoldingSetNodeID& ID) const
   {
      Profile(ID, getCase(), getCaseValues());
   }

   static void Profile(llvm::FoldingSetNodeID& ID, ast::EnumCaseDecl* Case,
                       llvm::ArrayRef<Constant*> Elements);

   static bool classof(Value const* T)
   {
      return T->getTypeID() == ConstantEnumID;
   }
};

class MagicConstant : public Constant {
public:
   enum Kind : unsigned char {
      __ctfe = 0,
   };

private:
   MagicConstant(ValueType Ty, Kind MagicConstantKind);

   Kind MagicConstantKind;

public:
   static MagicConstant* get(ValueType Ty, Kind MagicConstantKind);

   Kind getMagicConstantKind() const { return MagicConstantKind; }

   static inline bool classof(Value const* T)
   {
      return T->getTypeID() == MagicConstantID;
   }
};

class ConstantBitCastInst;
class ConstantAddrOfInst;
class ConstantIntCastInst;
class ConstantOperatorInst;
class ConstantGEPInst;
class ConstantLoadInst;

class ConstantExpr : public Constant, public llvm::FoldingSetNode {
public:
   static ConstantBitCastInst* getBitCast(Constant* Val, QualType toType);
   static ConstantAddrOfInst* getAddrOf(Constant* Val);
   static ConstantIntCastInst* getIntCast(CastKind kind, Constant* Target,
                                          QualType toType);

   static ConstantIntCastInst* getIntToPtr(Constant* Target, QualType toType)
   {
      return getIntCast(CastKind::IntToPtr, Target, toType);
   }

   static ConstantIntCastInst* getPtrToInt(Constant* Target, QualType toType)
   {
      return getIntCast(CastKind::PtrToInt, Target, toType);
   }

   static ConstantIntCastInst* getExt(Constant* Target, QualType toType)
   {
      return getIntCast(CastKind::Ext, Target, toType);
   }

   static ConstantIntCastInst* getTrunc(Constant* Target, QualType toType)
   {
      return getIntCast(CastKind::Trunc, Target, toType);
   }

   static ConstantIntCastInst* getSignCast(Constant* Target, QualType toType)
   {
      return getIntCast(CastKind::SignFlip, Target, toType);
   }

   static ConstantIntCastInst* getFPToInt(Constant* Target, QualType toType)
   {
      return getIntCast(CastKind::FPToInt, Target, toType);
   }

   static ConstantIntCastInst* getIntToFP(Constant* Target, QualType toType)
   {
      return getIntCast(CastKind::IntToFP, Target, toType);
   }

   static ConstantOperatorInst* getOperator(unsigned OPC, Constant* LHS,
                                            Constant* RHS);

   inline static ConstantOperatorInst* getAdd(Constant* LHS, Constant* RHS);
   inline static ConstantOperatorInst* getSub(Constant* LHS, Constant* RHS);
   inline static ConstantOperatorInst* getMul(Constant* LHS, Constant* RHS);
   inline static ConstantOperatorInst* getDiv(Constant* LHS, Constant* RHS);
   inline static ConstantOperatorInst* getShl(Constant* LHS, Constant* RHS);
   inline static ConstantOperatorInst* getShr(Constant* LHS, Constant* RHS);
   inline static ConstantOperatorInst* getMod(Constant* LHS, Constant* RHS);
   inline static ConstantOperatorInst* getAnd(Constant* LHS, Constant* RHS);
   inline static ConstantOperatorInst* getOr(Constant* LHS, Constant* RHS);
   inline static ConstantOperatorInst* getXor(Constant* LHS, Constant* RHS);

   static ConstantGEPInst* getGEP(Constant* Target, ConstantInt* Idx);

   static ConstantLoadInst* getLoad(Constant* Target);

protected:
   ConstantExpr(TypeID id, ValueType ty);

   static inline bool classof(Value const* T)
   {
      auto kind = T->getTypeID();
      return kind > _firstConstExpr && kind < _lastConstExpr;
   }
};

class ConstantBitCastInst : public ConstantExpr {
private:
   Constant* target;

   ConstantBitCastInst(Constant* Val, ValueType toType);

public:
   friend class ConstantExpr;

   Constant* getTarget() const { return target; }

   op_iterator op_begin_impl() { return &target; }
   op_const_iterator op_begin_impl() const { return &target; }
   unsigned getNumOperandsImpl() const { return 1; }

   void Profile(llvm::FoldingSetNodeID& ID) const
   {
      Profile(ID, getType(), getTarget());
   }

   static void Profile(llvm::FoldingSetNodeID& ID, QualType Type,
                       Constant* Target);

   static bool classof(Value const* T)
   {
      return T->getTypeID() == ConstantBitCastInstID;
   }
};

class ConstantAddrOfInst : public ConstantExpr {
private:
   Constant* target;

   ConstantAddrOfInst(Constant* Val, ValueType PtrTy);

public:
   friend class ConstantExpr;

   Constant* getTarget() const { return target; }

   op_iterator op_begin_impl() { return &target; }
   op_const_iterator op_begin_impl() const { return &target; }
   unsigned getNumOperandsImpl() const { return 1; }

   void Profile(llvm::FoldingSetNodeID& ID) const { Profile(ID, getTarget()); }

   static void Profile(llvm::FoldingSetNodeID& ID, Constant* Target);

   static bool classof(Value const* T)
   {
      return T->getTypeID() == ConstantAddrOfInstID;
   }
};

class ConstantIntCastInst : public ConstantExpr {
   ConstantIntCastInst(CastKind kind, Constant* Target, QualType toType);

   Constant* Target;
   CastKind Kind;

public:
   friend class ConstantExpr;

   Constant* getTarget() const { return Target; }
   CastKind getKind() const { return Kind; }

   op_iterator op_begin_impl() { return &Target; }
   op_const_iterator op_begin_impl() const { return &Target; }
   unsigned getNumOperandsImpl() const { return 1; }

   void Profile(llvm::FoldingSetNodeID& ID) const
   {
      Profile(ID, getType(), getKind(), getTarget());
   }

   static void Profile(llvm::FoldingSetNodeID& ID, QualType Type, CastKind Kind,
                       Constant* Target);

   static bool classof(Value const* T)
   {
      return T->getTypeID() == ConstantIntCastInstID;
   }
};

class ConstantOperatorInst : public ConstantExpr {
public:
   enum OpCode : unsigned char {
#define CDOT_BINARY_OP(Name, OP) Name,
#include "cdotc/IL/Instructions.def"
   };

private:
   ConstantOperatorInst(OpCode OPC, Constant* LHS, Constant* RHS);

   OpCode OPC;
   Constant* Ops[2];

public:
   friend class ConstantExpr;

   OpCode getOpCode() const { return OPC; }
   Constant* getLHS() const { return Ops[0]; }
   Constant* getRHS() const { return Ops[1]; }

   op_iterator op_begin_impl() { return &Ops[0]; }
   op_const_iterator op_begin_impl() const { return &Ops[0]; }
   unsigned getNumOperandsImpl() const { return 2; }

   void Profile(llvm::FoldingSetNodeID& ID) const
   {
      Profile(ID, getOpCode(), getLHS(), getRHS());
   }

   static void Profile(llvm::FoldingSetNodeID& ID, OpCode OPC, Constant* LHS,
                       Constant* RHS);

   static bool classof(Value const* T)
   {
      return T->getTypeID() == ConstantOperatorInstID;
   }
};

class ConstantGEPInst : public ConstantExpr {
   ConstantGEPInst(Constant* Target, ConstantInt* Idx);

   Constant* Target;
   ConstantInt* Idx;

public:
   friend class ConstantExpr;

   Constant* getTarget() const { return Target; }
   ConstantInt* getIdx() const { return Idx; }

   void Profile(llvm::FoldingSetNodeID& ID) const
   {
      Profile(ID, getTarget(), getIdx());
   }

   static void Profile(llvm::FoldingSetNodeID& ID, Constant* Target,
                       ConstantInt* Idx)
   {
      ID.AddPointer(Target);
      ID.AddPointer(Idx);
   }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == ConstantGEPInstID;
   }
};

class ConstantLoadInst : public ConstantExpr {
   ConstantLoadInst(Constant* Target);

   Constant* Target;

public:
   friend class ConstantExpr;

   Constant* getTarget() const { return Target; }

   op_iterator op_begin_impl() { return &Target; }
   op_const_iterator op_begin_impl() const { return &Target; }
   unsigned getNumOperandsImpl() const { return 1; }

   void Profile(llvm::FoldingSetNodeID& ID) const { Profile(ID, getTarget()); }

   static void Profile(llvm::FoldingSetNodeID& ID, Constant* Target)
   {
      ID.AddPointer(Target);
   }

   static bool classof(Value const* T)
   {
      return T->getTypeID() == ConstantLoadInstID;
   }
};

ConstantOperatorInst* ConstantExpr::getAdd(Constant* LHS, Constant* RHS)
{
   return getOperator(ConstantOperatorInst::Add, LHS, RHS);
}

ConstantOperatorInst* ConstantExpr::getSub(Constant* LHS, Constant* RHS)
{
   return getOperator(ConstantOperatorInst::Sub, LHS, RHS);
}

ConstantOperatorInst* ConstantExpr::getMul(Constant* LHS, Constant* RHS)
{
   return getOperator(ConstantOperatorInst::Mul, LHS, RHS);
}

ConstantOperatorInst* ConstantExpr::getDiv(Constant* LHS, Constant* RHS)
{
   return getOperator(ConstantOperatorInst::Div, LHS, RHS);
}

ConstantOperatorInst* ConstantExpr::getMod(Constant* LHS, Constant* RHS)
{
   return getOperator(ConstantOperatorInst::Mod, LHS, RHS);
}

ConstantOperatorInst* ConstantExpr::getShl(Constant* LHS, Constant* RHS)
{
   return getOperator(ConstantOperatorInst::Shl, LHS, RHS);
}

ConstantOperatorInst* ConstantExpr::getShr(Constant* LHS, Constant* RHS)
{
   return getOperator(ConstantOperatorInst::AShr, LHS, RHS);
}

ConstantOperatorInst* ConstantExpr::getAnd(Constant* LHS, Constant* RHS)
{
   return getOperator(ConstantOperatorInst::And, LHS, RHS);
}

ConstantOperatorInst* ConstantExpr::getOr(Constant* LHS, Constant* RHS)
{
   return getOperator(ConstantOperatorInst::Or, LHS, RHS);
}

ConstantOperatorInst* ConstantExpr::getXor(Constant* LHS, Constant* RHS)
{
   return getOperator(ConstantOperatorInst::Xor, LHS, RHS);
}

} // namespace il
} // namespace cdot

#endif // CDOT_CONSTANTINT_H
