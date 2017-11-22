//
// Created by Jonas Zell on 16.11.17.
//

#include "InitInst.h"
#include "../../Function/Method.h"
#include "../../Record/AggregateType.h"

#include "../../../../Variant/Type/ObjectType.h"

namespace cdot {
namespace il {

InitInst::InitInst(StructType *InitializedType,
                                 Method *Init,
                                 llvm::ArrayRef<Value *> args,
                                 BasicBlock *parent,
                                 const std::string &name,
                                 const SourceLocation &loc)
   : Instruction(InitInstID, nullptr, parent, name, loc),
     Init(Init), args(args.begin(), args.end()),
     InitializedType(InitializedType)
{
   *type = ObjectType::get(Init->getRecordType()->getName());
}

Method *InitInst::getInit() const
{
   return Init;
}

const llvm::SmallVector<Value *, 4> &InitInst::getArgs() const
{
   return args;
}

StructType *InitInst::getInitializedType() const
{
   return InitializedType;
}

UnionInitInst::UnionInitInst(UnionType *UnionTy,
                                 Value *InitializerVal,
                                 BasicBlock *parent,
                                 const std::string &name,
                                 const SourceLocation &loc)
   : Instruction(UnionInitInstID, ObjectType::get(UnionTy->getName()),
                 parent, name, loc), UnionTy(UnionTy),
     InitializerVal(InitializerVal)
{

}

EnumInitInst::EnumInitInst(EnumType *EnumTy,
                               llvm::StringRef caseName,
                               llvm::ArrayRef<Value *> args,
                               BasicBlock *parent,
                               const std::string &name,
                               const SourceLocation &loc)
   : Instruction(EnumInitInstID, ObjectType::get(EnumTy->getName()),
                 parent, name, loc), EnumTy(EnumTy),
     args(args.begin(), args.end()), caseName(caseName)
{

}

LambdaInitInst::LambdaInitInst(Constant *Function, CaptureList &&Captures,
                               BasicBlock *parent, const string &name,
                               const SourceLocation &loc)
   : Instruction(LambdaInitInstID, ObjectType::get("cdot.Lambda"), parent,
                 name, loc), Function(Function),
     Captures(std::move(Captures))
{

}

} // namespace il
} // namespace cdot