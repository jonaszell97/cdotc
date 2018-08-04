//
// Created by Jonas Zell on 16.11.17.
//

#include "Constants.h"

#include "AST/ASTContext.h"
#include "AST/Decl.h"
#include "AST/Type.h"
#include "Context.h"
#include "Module.h"

using std::string;

namespace cdot {
namespace il {

ConstantInt::ConstantInt(const ValueType &ty, llvm::APSInt &&value)
   : Constant(ConstantIntID, ty),
     Val(std::move(value))
{
   assert(ty->isUnsigned() == Val.isUnsigned());
   assert(ty->isIntegerType());
}

ConstantInt* ConstantInt::get(ValueType ty, llvm::APSInt &&value)
{
   assert(ty->isUnsigned() == value.isUnsigned());

   auto &Ctx = ty.getCtx();
   auto it = Ctx.IntConstants.find(value);
   if (it != Ctx.IntConstants.end()) {
      return it->getSecond().get();
   }

   auto Val = new ConstantInt(ty, std::move(value));
   if (Val->getValue().isNullValue()) {
      Val->ConstBits.AllZeros = true;
   }

   Ctx.IntConstants[Val->getValue()] = std::unique_ptr<ConstantInt>(Val);
   return Val;
}

ConstantInt* ConstantInt::get(ValueType ty, uint64_t value)
{
   return get(ty, llvm::APSInt(llvm::APInt(ty->getBitwidth(), value,
                                           !ty->isUnsigned()),
                               ty->isUnsigned()));
}

ConstantInt* ConstantInt::getTrue(Context &Ctx)
{
   if (!Ctx.TrueVal)
      Ctx.TrueVal = new ConstantInt(ValueType(Ctx, Ctx.getASTCtx().getInt1Ty()),
                                    llvm::APSInt(llvm::APInt(1, 1, false),
                                                 true));

   return Ctx.TrueVal;
}

ConstantInt* ConstantInt::getFalse(Context &Ctx)
{
   if (!Ctx.FalseVal) {
      Ctx.FalseVal = new ConstantInt(
         ValueType(Ctx, Ctx.getASTCtx().getInt1Ty()),
         llvm::APSInt(llvm::APInt(1, 0, false),
                      true));

      Ctx.FalseVal->ConstBits.AllZeros = true;
   }

   return Ctx.FalseVal;
}

ConstantFloat* ConstantFloat::get(ValueType Ty, float val)
{
   return get(Ty, llvm::APFloat(val));
}

ConstantFloat* ConstantFloat::get(ValueType Ty, double val)
{
   return get(Ty, llvm::APFloat(val));
}

ConstantFloat* ConstantFloat::get(ValueType Ty, llvm::APFloat &&APF)
{
   auto &Ctx = Ty.getCtx();

   Context::FPMapTy *Map;
   if (Ty->isFloatTy()) {
      Map = &Ctx.FP32Constants;
   }
   else if (Ty->isDoubleTy()) {
      Map = &Ctx.FP64Constants;
   }
   else {
      llvm_unreachable("bad floating point type!");
   }

   auto it = Map->find(APF);
   if (it != Map->end()) {
      return it->getSecond().get();
   }

   auto Val = new ConstantFloat(Ty, std::move(APF));
   if (Val->getValue().isZero()) {
      Val->ConstBits.AllZeros = true;
   }

   (*Map)[Val->getValue()] = std::unique_ptr<ConstantFloat>(Val);

   return Val;
}

ConstantString* ConstantString::get(Context &Ctx,
                                    llvm::StringRef val) {
   auto it = Ctx.StringConstants.find(val);
   if (it != Ctx.StringConstants.end())
      return it->getValue().get();

   auto Str = new ConstantString(ValueType(Ctx, Ctx.getASTCtx().getUInt8PtrTy()),
                                 val);

   Ctx.StringConstants.try_emplace(val, std::unique_ptr<ConstantString>(Str));
   return Str;
}

ConstantArray::ConstantArray(ValueType ty,
                             llvm::ArrayRef<Constant *> vec)
   : Constant(ConstantArrayID, ty), NumElements((unsigned)vec.size())
{
   assert(ty->isArrayType() && "ConstantArray must have array type!");
   std::copy(vec.begin(), vec.end(), reinterpret_cast<Constant**>(this + 1));

   for (auto *C : vec)
      C->addUse(this);
}

ConstantArray *ConstantArray::get(ValueType ty,
                                  llvm::ArrayRef<Constant*> vec) {
   auto &Ctx = ty.getCtx();
   llvm::FoldingSetNodeID ID;
   ConstantArray::Profile(ID, ty, vec);

   void *InsertPos;
   if (auto *Val = Ctx.ArrayConstants.FindNodeOrInsertPos(ID, InsertPos)) {
      return Val;
   }

   void *Mem = new char[sizeof(ConstantArray) + vec.size() * sizeof(Constant*)];
   auto Val = new(Mem) ConstantArray(ty, vec);

   Ctx.ArrayConstants.InsertNode(Val, InsertPos);
   return Val;
}

ConstantArray* ConstantArray::getAllZeros(il::ValueType Ty)
{
   auto &Ctx = Ty.getCtx();
   auto It = Ctx.AllZeroArrayConstants.find(Ty);

   if (It != Ctx.AllZeroArrayConstants.end())
      return It->getSecond();

   auto Val = new ConstantArray(Ty, {});
   Val->ConstBits.AllZeros = true;

   Ctx.AllZeroArrayConstants[Ty] = Val;
   return Val;
}

void ConstantArray::Profile(llvm::FoldingSetNodeID &ID,
                            QualType ElementTy,
                            llvm::ArrayRef<Constant *> Values) {
   ElementTy.Profile(ID);
   for (auto &V : Values)
      ID.AddPointer(V);
}

ConstantTuple::ConstantTuple(ValueType ty,
                             llvm::ArrayRef<Constant *> vec)
   : Constant(ConstantTupleID, ty),
     NumElements((unsigned)vec.size())
{
   assert(ty->isTupleType() && "ConstantTuple must have tuple type!");
   std::copy(vec.begin(), vec.end(), reinterpret_cast<Constant**>(this + 1));

   for (auto *C : vec)
      C->addUse(this);
}

ConstantTuple* ConstantTuple::get(ValueType ty,
                                  llvm::ArrayRef<Constant *> vec) {
   auto &Ctx = ty.getCtx();
   llvm::FoldingSetNodeID ID;
   ConstantTuple::Profile(ID, ty, vec);

   void *InsertPos;
   if (auto *Val = Ctx.TupleConstants.FindNodeOrInsertPos(ID, InsertPos)) {
      return Val;
   }

   void *Mem = new char[sizeof(ConstantTuple)
                        + vec.size() * sizeof(Constant*)];

   auto Val = new(Mem) ConstantTuple(ty, vec);
   Ctx.TupleConstants.InsertNode(Val, InsertPos);

   return Val;
}

ConstantTuple* ConstantTuple::getEmpty(il::Context &Ctx)
{
   if (Ctx.EmptyTuple)
      return Ctx.EmptyTuple;

   auto Ty = Ctx.getASTCtx().getTupleType({});
   auto Tup = get(ValueType(Ctx, Ty), {});

   Ctx.EmptyTuple = Tup;
   return Tup;
}

ConstantTuple* ConstantTuple::getAllZeros(il::ValueType Ty)
{
   auto &Ctx = Ty.getCtx();
   auto It = Ctx.AllZeroTupleConstants.find(Ty);

   if (It != Ctx.AllZeroTupleConstants.end())
      return It->getSecond();

   auto Val = new ConstantTuple(Ty, {});
   Val->ConstBits.AllZeros = true;

   Ctx.AllZeroTupleConstants[Ty] = Val;
   return Val;
}

void ConstantTuple::Profile(llvm::FoldingSetNodeID &ID,
                            QualType Ty,
                            llvm::ArrayRef<Constant *> Values) {
   Ty.Profile(ID);
   for (auto &V : Values)
      ID.AddPointer(V);
}

ConstantStruct::ConstantStruct(ValueType Ty,
                               llvm::ArrayRef<Constant *> vec)
   : ConstantStruct(ConstantStructID, Ty, vec)
{

}

ConstantStruct::ConstantStruct(TypeID id, ValueType Ty,
                               llvm::ArrayRef<Constant *> vec)
   : Constant(id, Ty),
     NumElements((unsigned)vec.size())
{
   std::copy(vec.begin(), vec.end(), reinterpret_cast<Constant**>(this + 1));
   for (auto *C : vec)
      C->addUse(this);
}

ConstantStruct* ConstantStruct::get(ValueType Ty,
                                    llvm::ArrayRef<Constant *> vec) {
   auto &Ctx = Ty.getCtx();

   llvm::FoldingSetNodeID ID;
   ConstantStruct::Profile(ID, Ty, vec);

   void *InsertPos;
   if (auto *Val = Ctx.StructConstants.FindNodeOrInsertPos(ID, InsertPos)) {
      return Val;
   }

   void *Mem = new char[sizeof(ConstantStruct)
                        + vec.size() * sizeof(Constant*)];
   auto Val = new(Mem) ConstantStruct(Ty, vec);

   Ctx.StructConstants.InsertNode(Val, InsertPos);
   return Val;
}

ConstantStruct* ConstantStruct::getAllZeros(il::ValueType Ty)
{
   auto &Ctx = Ty.getCtx();
   auto It = Ctx.AllZeroStructConstants.find(Ty);

   if (It != Ctx.AllZeroStructConstants.end())
      return It->getSecond();

   auto Val = new ConstantStruct(Ty, {});
   Val->ConstBits.AllZeros = true;

   Ctx.AllZeroStructConstants[Ty] = Val;
   return Val;
}

void ConstantStruct::Profile(llvm::FoldingSetNodeID &ID,
                             QualType Type,
                             llvm::ArrayRef<Constant *> Elements) {
   Type.Profile(ID);
   for (auto &V : Elements)
      ID.AddPointer(V);
}

ConstantClass::ConstantClass(ConstantStruct *StructVal,
                             GlobalVariable *TI,
                             ConstantClass *Base)
   : Constant(ConstantClassID, StructVal->getType()),
     Vals{ TI, StructVal, Base }
{
   for (auto Op : getOperands())
      Op->addUse(this);
}

ConstantClass::ConstantClass(ValueType Ty,
                             GlobalVariable *TI)
   : Constant(ConstantClassID, Ty),
     Vals{ TI, nullptr, nullptr }
{
   TI->addUse(this);
}

ConstantClass* ConstantClass::ForwardDeclare(ValueType Ty,
                                             GlobalVariable *TI) {
   return new ConstantClass(Ty, TI);
}

ConstantClass *ConstantClass::get(ConstantStruct *StructVal,
                                  GlobalVariable *TI,
                                  ConstantClass *Base) {
   auto &Ctx = StructVal->getCtx();

   llvm::FoldingSetNodeID ID;
   ConstantClass::Profile(ID, StructVal, Base);

   void *InsertPos;
   if (auto *Val = Ctx.ClassConstants.FindNodeOrInsertPos(ID, InsertPos)) {
      return Val;
   }

   auto Val = new ConstantClass(StructVal, TI, Base);
   Ctx.ClassConstants.InsertNode(Val, InsertPos);

   return Val;
}

ConstantClass*
ConstantClass::ReplaceForwardDecl(ConstantStruct *StructVal,
                                  ConstantClass *Base) {
   assert(isForwardDecl() && "not a forward declared class!");
   auto &Ctx = getCtx();

   llvm::FoldingSetNodeID ID;
   ConstantClass::Profile(ID, StructVal, Base);

   void *InsertPos;
   if (auto *Val = Ctx.ClassConstants.FindNodeOrInsertPos(ID, InsertPos)) {
      replaceAllUsesWith(Val);
      delete this;

      return Val;
   }

   Vals[ConstantClass::StructVal] = StructVal;
   Vals[ConstantClass::Base] = Base;

   StructVal->addUse(this);
   Base->addUse(this);

   Ctx.ClassConstants.InsertNode(this, InsertPos);
   return this;
}

GlobalVariable *ConstantClass::getTypeInfo() const
{
   return support::cast<GlobalVariable>(Vals[TypeInfo]);
}

void ConstantClass::Profile(llvm::FoldingSetNodeID &ID,
                            ConstantStruct *StructVal,
                            ConstantClass *Base) {
   ID.AddPointer(StructVal);
   ID.AddPointer(Base);
}

ConstantUnion::ConstantUnion(ValueType Ty,
                             Constant *InitVal)
   : Constant(ConstantUnionID, Ty),
     InitVal(InitVal)
{
   InitVal->addUse(this);
}

ConstantUnion* ConstantUnion::get(ValueType Ty,
                                  Constant *InitVal) {
   auto &Ctx = Ty.getCtx();

   llvm::FoldingSetNodeID ID;
   ConstantUnion::Profile(ID, Ty, InitVal);

   void *InsertPos;
   if (auto *Val = Ctx.UnionConstants.FindNodeOrInsertPos(ID, InsertPos)) {
      return Val;
   }

   auto Val = new ConstantUnion(Ty, InitVal);
   Ctx.UnionConstants.InsertNode(Val, InsertPos);

   return Val;
}

void ConstantUnion::Profile(llvm::FoldingSetNodeID &ID,
                            QualType Type,
                            Constant *InitVal) {
   Type.Profile(ID);
   ID.AddPointer(InitVal);
}

ConstantEnum::ConstantEnum(Context &Ctx,
                           ast::EnumCaseDecl *Case,
                           llvm::ArrayRef<Constant*> vec)
   : Constant(ConstantEnumID,
              ValueType(Ctx, Ctx.getASTCtx().getRecordType(Case->getRecord()))),
     Case(Case), NumValues((unsigned)vec.size())
{
   std::copy(vec.begin(), vec.end(), reinterpret_cast<Constant**>(this + 1));

   for (auto *C : vec)
      C->addUse(this);
}

ConstantEnum *ConstantEnum::get(Context &Ctx,
                                ast::EnumCaseDecl *Case,
                                llvm::ArrayRef<Constant*> vec) {
   llvm::FoldingSetNodeID ID;
   ConstantEnum::Profile(ID, Case, vec);

   void *InsertPos;
   if (auto *Val = Ctx.EnumConstants.FindNodeOrInsertPos(ID, InsertPos)) {
      return Val;
   }

   void *Mem = new char[sizeof(ConstantEnum)
                        + vec.size() * sizeof(Constant*)];
   auto Val = new(Mem) ConstantEnum(Ctx, Case, vec);

   Ctx.EnumConstants.InsertNode(Val, InsertPos);
   return Val;
}

Constant* ConstantEnum::getDiscriminator() const
{
   return Case->getILValue();
}

void ConstantEnum::Profile(llvm::FoldingSetNodeID &ID,
                           ast::EnumCaseDecl *Case,
                           llvm::ArrayRef<Constant *> Elements) {
   ID.AddPointer(Case);
   for (auto &V : Elements)
      ID.AddPointer(V);
}

VTable::VTable(il::Context &Ctx,
               llvm::ArrayRef<il::Function *> Entries,
               ast::ClassDecl *C)
   : Constant(VTableID, ValueType(Ctx, QualType())),
     NumFunctions((unsigned)Entries.size()), C(C)
{
   auto &ASTCtx = Ctx.getASTCtx();
   QualType Ty = ASTCtx.getArrayType(ASTCtx.getInt8PtrTy(), Entries.size());
   type = ValueType(Ctx, Ty);

   auto Ptr = reinterpret_cast<Function**>(this + 1);
   for (auto &Val : Entries) {
      Val->addUse(this);
      *Ptr = Val;
      ++Ptr;
   }
}

VTable* VTable::Create(il::Context &Ctx,
                       llvm::ArrayRef<il::Function *> Entries,
                       ast::ClassDecl *C) {
   auto It = Ctx.VTableMap.find(C);
   if (It != Ctx.VTableMap.end()) {
      return It->getSecond();
   }

   void *Mem = new char[sizeof(VTable) + Entries.size() * sizeof(Function*)];
   auto *VT = new(Mem) VTable(Ctx, Entries, C);

   Ctx.VTableMap[C] = VT;
   return VT;
}

TypeInfo::TypeInfo(Module *M, QualType forType,
                   llvm::ArrayRef<il::Constant*> Vals)
   : Constant(TypeInfoID, ValueType(M->getContext(), M->getContext()
                                                      .getASTCtx()
                                                      .getMetaType(forType))),
     forType(forType)
{
   assert(Vals.size() == MetaType::MemberCount);

   auto Ptr = (Constant**)this->Vals;
   for (auto &Val : Vals) {
      Val->addUse(this);
      *Ptr = Val;
      ++Ptr;
   }
}

TypeInfo* TypeInfo::get(Module *M, QualType forType,
                        llvm::ArrayRef<il::Constant*> Vals) {
   auto &Map = M->getContext().TypeInfoMap;
   auto It = Map.find(forType);
   if (It != Map.end())
      return It->getSecond();

   auto *TI = new TypeInfo(M, forType, Vals);
   Map.try_emplace(forType, TI);

   return TI;
}

ConstantPointer::ConstantPointer(ValueType ty)
   : Constant(ConstantPointerID, ty)
{
   assert(ty->isRefcounted() || ty->isPointerType() || ty->isThinFunctionTy());
}

ConstantPointer* ConstantPointer::get(ValueType ty)
{
   auto &Ctx = ty.getCtx();
   auto it = Ctx.NullConstants.find(ty);
   if (it != Ctx.NullConstants.end())
      return it->getSecond().get();

   auto Ptr = new ConstantPointer(ty);
   Ptr->ConstBits.AllZeros = true;

   Ctx.NullConstants.try_emplace(ty, std::unique_ptr<ConstantPointer>(Ptr));
   return Ptr;
}

ConstantTokenNone::ConstantTokenNone(Context &Ctx)
   : Constant(ConstantTokenNoneID,
              ValueType(Ctx, Ctx.getASTCtx().getTokenType()))
{

}

ConstantTokenNone* ConstantTokenNone::get(il::Context &Ctx)
{
   if (!Ctx.TokNone) {
      Ctx.TokNone = new ConstantTokenNone(Ctx);
   }

   return Ctx.TokNone;
}

UndefValue::UndefValue(ValueType Ty)
   : Constant(UndefValueID, Ty)
{

}

UndefValue* UndefValue::get(ValueType Ty)
{
   auto &Ctx = Ty.getCtx();
   auto it = Ctx.UndefConstants.find(Ty);
   if (it != Ctx.UndefConstants.end())
      return it->getSecond().get();

   auto Ptr = new UndefValue(Ty);
   Ctx.UndefConstants.try_emplace(Ty, std::unique_ptr<UndefValue>(Ptr));

   return Ptr;
}

MagicConstant::MagicConstant(ValueType Ty,
                             Kind MagicConstantKind)
   : Constant(MagicConstantID, Ty),
     MagicConstantKind(MagicConstantKind)
{

}

MagicConstant* MagicConstant::get(ValueType Ty,
                                  Kind MagicConstantKind) {
   auto &C = Ty.getCtx().MagicConstants[MagicConstantKind];
   if (C)
      return C;

   C = new MagicConstant(Ty, MagicConstantKind);
   return C;
}

ConstantExpr::ConstantExpr(TypeID id, ValueType ty)
   : Constant(id, ty)
{

}

ConstantAddrOfInst::ConstantAddrOfInst(Constant *Val, ValueType PtrTy)
   : ConstantExpr(ConstantAddrOfInstID, PtrTy),
     target(Val)
{
   assert(Val->getType()->getReferencedType() == PtrTy->getPointeeType());
   assert(Val->isLvalue());
   Val->addUse(this);
}

void ConstantAddrOfInst::Profile(llvm::FoldingSetNodeID &ID,
                                 Constant *Target) {
   ID.AddPointer(Target);
}

ConstantBitCastInst::ConstantBitCastInst(Constant *Val, ValueType toType)
   : ConstantExpr(ConstantBitCastInstID, toType),
     target(Val)
{
   Val->addUse(this);
}

void ConstantBitCastInst::Profile(llvm::FoldingSetNodeID &ID,
                                  QualType Type,
                                  Constant *Target) {
   Type.Profile(ID);
   ID.AddPointer(Target);
}

LLVM_ATTRIBUTE_UNUSED
static bool isBitCastable(QualType Ty)
{
   switch (Ty->getTypeID()) {
   case Type::PointerTypeID:
   case Type::MutablePointerTypeID:
   case Type::ReferenceTypeID:
   case Type::MutableReferenceTypeID:
   case Type::FunctionTypeID:
      return true;
   case Type::RecordTypeID:
      return Ty->isClass();
   default:
      return false;
   }
}

ConstantBitCastInst* ConstantExpr::getBitCast(Constant *Val,
                                              QualType toType) {
   auto &Ctx = Val->getCtx();

   llvm::FoldingSetNodeID ID;
   ConstantBitCastInst::Profile(ID, toType, Val);

   void *InsertPos;
   if (auto *Ptr = Ctx.BitCastConstants.FindNodeOrInsertPos(ID, InsertPos)) {
      return Ptr;
   }

   assert(isBitCastable(Val->getType()) && "invalid bitcast!");
   auto Ptr = new ConstantBitCastInst(Val, ValueType(Val->getType().getCtx(),
                                                     toType));

   Ctx.BitCastConstants.InsertNode(Ptr, InsertPos);
   return Ptr;
}

ConstantAddrOfInst* ConstantExpr::getAddrOf(Constant *Val)
{
   auto &Ctx = Val->getCtx();

   llvm::FoldingSetNodeID ID;
   ConstantAddrOfInst::Profile(ID, Val);

   void *InsertPos;
   if (auto *Ptr = Ctx.AddrOfConstants.FindNodeOrInsertPos(ID, InsertPos)) {
      return Ptr;
   }

   assert(Val->isLvalue());
   auto Ptr = new ConstantAddrOfInst(
      Val,
      ValueType(Val->getCtx(), Val->getType()->getReferencedType())
         .getPointerTo());

   Ctx.AddrOfConstants.InsertNode(Ptr, InsertPos);
   return Ptr;
}

ConstantIntCastInst *ConstantExpr::getIntCast(CastKind kind,
                                              il::Constant *Target,
                                              QualType toType) {
   auto &Ctx = Target->getCtx();

   llvm::FoldingSetNodeID ID;
   ConstantIntCastInst::Profile(ID, toType, kind, Target);

   void *InsertPos;
   if (auto *Ptr = Ctx.IntCastConstants.FindNodeOrInsertPos(ID, InsertPos)) {
      return Ptr;
   }

   auto Ptr = new ConstantIntCastInst(kind, Target, toType);

   Ctx.IntCastConstants.InsertNode(Ptr, InsertPos);
   return Ptr;
}

ConstantIntCastInst::ConstantIntCastInst(CastKind kind,
                                         il::Constant *Target,
                                         QualType toType)
   : ConstantExpr(ConstantIntCastInstID, ValueType(Target->getCtx(), toType)),
     Target(Target), Kind(kind)
{
   Target->addUse(this);
}

void ConstantIntCastInst::Profile(llvm::FoldingSetNodeID &ID,
                                  QualType Type,
                                  CastKind Kind,
                                  Constant *Target) {
   Type.Profile(ID);
   ID.AddInteger((int)Kind);
   ID.AddPointer(Target);
}

ConstantOperatorInst::ConstantOperatorInst(OpCode OPC,
                                           Constant *LHS,
                                           Constant *RHS)
   : ConstantExpr(ConstantOperatorInstID, LHS->getType()),
     OPC(OPC), Ops{LHS, RHS}
{
   LHS->addUse(this);
   RHS->addUse(this);
}

void ConstantOperatorInst::Profile(llvm::FoldingSetNodeID &ID,
                                   OpCode OPC,
                                   Constant *LHS,
                                   Constant *RHS) {
   ID.AddInteger(OPC);
   ID.AddPointer(LHS);
   ID.AddPointer(RHS);
}

ConstantGEPInst::ConstantGEPInst(Constant *Target,
                                 ConstantInt *Idx)
   : ConstantExpr(ConstantGEPInstID, ValueType(Target->getCtx(), QualType())),
     Target(Target), Idx(Idx)
{
   Target->addUse(this);

   QualType valTy = Target->getType()->stripReference();

   assert(valTy->isRecordType() && "struct GEP on non-record type!");
   auto R = support::cast<ast::StructDecl>(valTy->getRecord());

   size_t i = 0;
   size_t IdxVal = Idx->getZExtValue();

   for (auto F : R->getFields()) {
      if (i++ == IdxVal) {
         type = F->getType();
         break;
      }
   }

   type = getASTCtx().getReferenceType(type);
}

ConstantOperatorInst* ConstantExpr::getOperator(unsigned OPC,
                                                Constant *LHS,
                                                Constant *RHS) {
   auto &Ctx = LHS->getCtx();

   llvm::FoldingSetNodeID ID;
   ConstantOperatorInst::Profile(ID, (ConstantOperatorInst::OpCode)OPC,
                                 LHS, RHS);

   void *InsertPos;
   if (auto Val = Ctx.OperatorConstants.FindNodeOrInsertPos(ID, InsertPos)) {
      return Val;
   }

   auto Val = new ConstantOperatorInst((ConstantOperatorInst::OpCode)OPC,
                                       LHS, RHS);

   Ctx.OperatorConstants.InsertNode(Val, InsertPos);
   return Val;
}

ConstantGEPInst* ConstantExpr::getGEP(Constant *Target,
                                      ConstantInt *Idx) {
   auto &Ctx = Target->getCtx();

   llvm::FoldingSetNodeID ID;
   ConstantGEPInst::Profile(ID, Target, Idx);

   void *InsertPos;
   if (auto Val = Ctx.GEPConstants.FindNodeOrInsertPos(ID, InsertPos)) {
      return Val;
   }

   auto Val = new ConstantGEPInst(Target, Idx);
   Ctx.GEPConstants.InsertNode(Val, InsertPos);

   return Val;
}

ConstantLoadInst* ConstantExpr::getLoad(Constant *Target)
{
   auto &Ctx = Target->getCtx();

   llvm::FoldingSetNodeID ID;
   ConstantLoadInst::Profile(ID, Target);

   void *InsertPos;
   if (auto *Ptr = Ctx.LoadConstants.FindNodeOrInsertPos(ID, InsertPos)) {
      return Ptr;
   }

   auto Ptr = new ConstantLoadInst(Target);

   Ctx.LoadConstants.InsertNode(Ptr, InsertPos);
   return Ptr;
}

ConstantLoadInst::ConstantLoadInst(Constant *Target)
   : ConstantExpr(ConstantLoadInstID, ValueType(Target->getCtx(), QualType())),
     Target(Target)
{
   Target->addUse(this);

   if (Target->getType()->isReferenceType()) {
      type = Target->getType()->getReferencedType();
   }
   else {
      type = Target->getType()->getPointeeType();
   }
}

} // namespace il
} // namespace cdot