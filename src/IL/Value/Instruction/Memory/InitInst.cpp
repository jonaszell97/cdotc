//
// Created by Jonas Zell on 16.11.17.
//

#include "InitInst.h"
#include "../../Function/Method.h"
#include "../../Record/AggregateType.h"

using namespace cdot::support;

namespace cdot {
namespace il {

InitInst::InitInst(StructType *InitializedType,
                   Method *Init,
                   llvm::ArrayRef<Value *> args,
                   BasicBlock *parent)
   : CallInst(InitInstID, Init, args, parent),
     InitializedType(InitializedType)
{
   type = Init->getRecordType()->getType();
   setIsLvalue(!isa<ClassType>(InitializedType));
}

UnionInitInst::UnionInitInst(UnionType *UnionTy,
                             Value *InitializerVal,
                             BasicBlock *parent)
   : CallInst(UnionInitInstID, UnionTy->getCtx(), { InitializerVal }, parent),
     UnionTy(UnionTy)
{
   type = UnionTy->getType();
   setIsLvalue(true);
}

EnumInitInst::EnumInitInst(EnumType *EnumTy,
                           std::string const& caseName,
                           llvm::ArrayRef<Value *> args,
                           BasicBlock *parent)
   : CallInst(EnumInitInstID, EnumTy->getCtx(), args, parent),
     EnumTy(EnumTy), caseName(caseName)
{
   type = EnumTy->getType();
   setIsLvalue(true);
}

LambdaInitInst::LambdaInitInst(il::Function *F,
                               QualType LambdaTy,
                               llvm::ArrayRef<Value*> Captures,
                               BasicBlock *parent)
   : Instruction(LambdaInitInstID, ValueType(F->getCtx(), LambdaTy), parent),
     MultiOperandInst(Captures),
     F(F)
{
   setIsLvalue(true);
}

} // namespace il
} // namespace cdot