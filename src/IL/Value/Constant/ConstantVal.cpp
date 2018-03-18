//
// Created by Jonas Zell on 16.11.17.
//

#include "ConstantVal.h"

#include "AST/ASTContext.h"
#include "IL/Module/Module.h"
#include "IL/Module/Context.h"
#include "Variant/Type/Type.h"

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

ConstantArray* ConstantArray::get(ValueType ty, ArrayTy &&Arr)
{
   return new ConstantArray(ty, std::move(Arr));
}

ConstantArray *ConstantArray::get(ValueType ty,
                                  llvm::ArrayRef<Constant*> vec) {
   return new ConstantArray(ty, vec);
}

ConstantArray *ConstantArray::get(ValueType ty)
{
   return new ConstantArray(ty);
}

ConstantStruct::ConstantStruct(AggregateType *structTy,
                               llvm::ArrayRef<Constant *> vec)
   : Constant(ConstantStructID, structTy->getType()),
     structTy(structTy), elements(vec.begin(), vec.end())
{

}

ConstantStruct::ConstantStruct(TypeID id, AggregateType *structTy,
                               llvm::ArrayRef<Constant *> vec)
   : Constant(id, structTy->getType()),
     structTy(structTy), elements(vec.begin(), vec.end())
{

}

ConstantStruct* ConstantStruct::get(AggregateType *structTy,
                                    llvm::ArrayRef<Constant *> vec) {
   return new ConstantStruct(structTy, vec);
}

const ConstantStruct::ElementTy &ConstantStruct::getElements() const
{
   return elements;
}

TypeInfo::TypeInfo(Module *M, Type* forType,
                   il::Constant *ParentClass, il::Constant *TypeID,
                   il::Constant *TypeName, il::Constant *Deinitializer,
                   il::Constant *NumConformances, il::Constant *Conformances)
   : ConstantStruct(TypeInfoID, M->getType("cdot.TypeInfo"),
                    { ParentClass, TypeID, TypeName, Deinitializer,
                       NumConformances, Conformances }),
     forType(forType)
{

}

TypeInfo* TypeInfo::get(Module *M, Type* forType,
                        il::Constant *ParentClass, il::Constant *TypeID,
                        il::Constant *TypeName, il::Constant *Deinitializer,
                        il::Constant *NumConformances,
                        il::Constant *Conformances) {
   return new TypeInfo(M, forType, ParentClass, TypeID, TypeName, Deinitializer,
                       NumConformances, Conformances);
}

ConstantPointer::ConstantPointer(ValueType ty, uintptr_t value)
   : Constant(ConstantPointerID, ty), value(value)
{
   assert(ty->isRefcounted() || ty->isPointerType() || ty->isRawFunctionTy());
}

ConstantPointer* ConstantPointer::get(ValueType ty, uintptr_t value)
{
   auto &Ctx = ty.getCtx();
   assert(value == 0 && "remove other options");
   if (!Ctx.NullVal) {
      Ctx.NullVal = new ConstantPointer(ty, value);
   }

   return Ctx.NullVal;
}

} // namespace il
} // namespace cdot