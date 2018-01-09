//
// Created by Jonas Zell on 16.11.17.
//

#include "InitInst.h"
#include "../../Function/Method.h"
#include "../../Record/AggregateType.h"

#include "../../../../Variant/Type/ObjectType.h"
#include "../../../../Variant/Type/FunctionType.h"

namespace cdot {
namespace il {

InitInst::InitInst(StructType *InitializedType,
                   Method *Init,
                   llvm::ArrayRef<Value *> args,
                   BasicBlock *parent)
   : CallInst(InitInstID, Init, args, parent),
     InitializedType(InitializedType)
{
   *type = ObjectType::get(Init->getRecordType()->getName());
   setIsLvalue(!support::isa<ClassType>(InitializedType));
}

UnionInitInst::UnionInitInst(UnionType *UnionTy,
                             Value *InitializerVal,
                             BasicBlock *parent)
   : CallInst(UnionInitInstID, { InitializerVal }, parent),
     UnionTy(UnionTy)
{
   *type = ObjectType::get(UnionTy->getName());
   setIsLvalue(true);
}

EnumInitInst::EnumInitInst(EnumType *EnumTy,
                           std::string const& caseName,
                           llvm::ArrayRef<Value *> args,
                           BasicBlock *parent)
   : CallInst(EnumInitInstID, args, parent),
     EnumTy(EnumTy), caseName(caseName)
{
   *type = ObjectType::get(EnumTy->getName());
   setIsLvalue(true);
}

LambdaInitInst::LambdaInitInst(il::Function *F,
                               llvm::ArrayRef<Value*> Captures,
                               BasicBlock *parent)
   : Instruction(LambdaInitInstID, nullptr, parent),
     MultiOperandInst(Captures),
     F(F)
{
   auto FuncTy = support::cast<FunctionType>(*F->getType());
   *type = FunctionType::get(FuncTy->getReturnType(), FuncTy->getArgTypes(),
                             false);

   setIsLvalue(true);
}

} // namespace il
} // namespace cdot