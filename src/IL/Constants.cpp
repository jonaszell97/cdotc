//
// Created by Jonas Zell on 16.11.17.
//

#include "Constants.h"

#include "AST/ASTContext.h"
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
                                    llvm::APSInt(llvm::APInt(1, 1, true),
                                                 false));

   return Ctx.TrueVal;
}

ConstantInt* ConstantInt::getFalse(Context &Ctx)
{
   if (!Ctx.FalseVal)
      Ctx.FalseVal = new ConstantInt(ValueType(Ctx, Ctx.getASTCtx().getInt1Ty()),
                                     llvm::APSInt(llvm::APInt(1, 0, true),
                                                  false));

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
   auto it = Ctx.FPConstants.find(APF);
   if (it != Ctx.FPConstants.end()) {
      return it->getSecond().get();
   }

   auto Val = new ConstantFloat(Ty, std::move(APF));
   Ctx.FPConstants[Val->getValue()] = std::unique_ptr<ConstantFloat>(Val);

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
   std::copy(vec.begin(), vec.end(), reinterpret_cast<Constant**>(this + 1));
}

ConstantArray *ConstantArray::get(ValueType ty,
                                  llvm::ArrayRef<Constant*> vec) {
   void *Mem = new char[sizeof(ConstantArray)
                        + vec.size() * sizeof(Constant*)];

   return new(Mem) ConstantArray(ty, vec);
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
}

ConstantStruct* ConstantStruct::get(ValueType Ty,
                                    llvm::ArrayRef<Constant *> vec) {
   void *Mem = new char[sizeof(ConstantStruct)
                        + vec.size() * sizeof(Constant*)];

   return new(Mem) ConstantStruct(Ty, vec);
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

   std::copy(Entries.begin(), Entries.end(),
             reinterpret_cast<Function**>(this + 1));
}

VTable* VTable::Create(il::Context &Ctx,
                       llvm::ArrayRef<il::Function *> Entries,
                       ast::ClassDecl *C) {
   void *Mem = new char[sizeof(VTable) + Entries.size() * sizeof(Function*)];
   return new(Mem) VTable(Ctx, Entries, C);
}

TypeInfo::TypeInfo(Module *M, QualType forType,
                   llvm::ArrayRef<il::Constant*> Vals)
   : Constant(TypeInfoID, ValueType(M->getContext(), M->getContext()
                                                      .getASTCtx()
                                                      .getMetaType(forType))),
     forType(forType)
{
   assert(Vals.size() == MetaType::MemberCount);
   std::copy(Vals.begin(), Vals.end(), this->Vals);
}

TypeInfo* TypeInfo::get(Module *M, QualType forType,
                        llvm::ArrayRef<il::Constant*> Vals) {
   return new TypeInfo(M, forType, Vals);
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

   Ctx.NullConstants.try_emplace(ty, std::unique_ptr<ConstantPointer>(Ptr));
   return Ptr;
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
}

ConstantBitCastInst::ConstantBitCastInst(Constant *Val, ValueType toType)
   : ConstantExpr(ConstantBitCastInstID, toType),
     target(Val)
{

}

static bool isBitCastable(QualType Ty)
{
   switch (Ty->getTypeID()) {
   case Type::PointerTypeID:
   case Type::ReferenceTypeID:
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
   assert(isBitCastable(Val->getType()) && "invalid bitcast!");
   return new ConstantBitCastInst(Val, ValueType(Val->getType().getCtx(),
                                                 toType));
}

ConstantAddrOfInst* ConstantExpr::getAddrOf(Constant *Val)
{
   assert(Val->isLvalue());
   return new ConstantAddrOfInst(
      Val, ValueType(Val->getCtx(), Val->getType()->getReferencedType())
         .getPointerTo());
}

ConstantIntCastInst *ConstantExpr::getIntCast(CastKind kind,
                                              il::Constant *Target,
                                              QualType toType) {
   return new ConstantIntCastInst(kind, Target, toType);
}

ConstantIntCastInst::ConstantIntCastInst(CastKind kind,
                                         il::Constant *Target,
                                         QualType toType)
   : ConstantExpr(ConstantIntCastInstID, ValueType(Target->getCtx(), toType)),
     Target(Target), Kind(kind)
{
}

} // namespace il
} // namespace cdot