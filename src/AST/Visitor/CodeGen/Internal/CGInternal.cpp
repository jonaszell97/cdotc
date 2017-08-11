//
// Created by Jonas Zell on 30.07.17.
//

#include "CGInternal.h"
#include "../CodeGenVisitor.h"


InternalClass::InternalClass(CodeGenVisitor& Visitor, string name) : name(name), Visitor(Visitor) {

}

InternalClass& InternalClass::DeclareProperty(string name, llvm::Type *type, bool is_static) {
    fields.emplace(is_static ? name + "!st" : name, type);
    return *this;
}

InternalClass& InternalClass::Get() {
    finalized = true;
    std::vector<llvm::Type*> lfields;
    lfields.reserve(fields.size());
    for (const auto& field : fields) {
        lfields.push_back(field.second);
    }

    type = llvm::StructType::create(CodeGenVisitor::Context, lfields, "class." + name);

    return *this;
}

llvm::Function* InternalClass::DeclareMethod(string name, llvm::Type *ret, std::vector<llvm::Type*> args, bool
    is_static, bool vararg)
{
    args.insert(args.begin(), type->getPointerTo());
    auto func_type = llvm::FunctionType::get(ret, args, vararg);
    auto func = llvm::Function::Create(func_type, llvm::Function::ExternalLinkage, this->name + "." + name,
        Visitor.Module.get());

    methods.emplace(name, func);
    llvm::BasicBlock::Create(Visitor.Context, "", func);

    return func;
}

llvm::Value* InternalClass::AccessField(string name, llvm::Value *structure, bool is_static) {
    return Visitor.Builder.CreateStructGEP(type, structure, std::distance(fields.begin(), fields.find(name)));
}

llvm::Value* InternalClass::CallMethod(string name, std::vector<llvm::Value*> args, llvm::Value *structure, bool
    is_static)
{
    args.insert(args.begin(), structure);
    return Visitor.Builder.CreateCall(methods[is_static ? name + "!st" : name], args);
}


void CGInternal::CreateString(CodeGenVisitor &Visitor) {
    auto& Builder = Visitor.Builder;
    auto& Context = Visitor.Context;

    auto String = InternalClass(Visitor, "XString")
        .DeclareProperty("__str", llvm::Type::getInt8PtrTy(Context), false)
        .Get();

    auto constr = String.DeclareMethod("construct", llvm::Type::getVoidTy(Context), {llvm::Type::getInt8PtrTy(Context)}, false);

    auto ip = Builder.GetInsertBlock();

    Builder.SetInsertPoint(&constr->getEntryBlock());
    Builder.CreateStore(&*(++constr->arg_begin()), String.AccessField("__str", &*constr->arg_begin(), false));

    Builder.CreateRetVoid();
    Builder.SetInsertPoint(ip);
}

llvm::GlobalVariable* CGInternal::CreateInt(CodeGenVisitor &Visitor) {
    auto& Builder = Visitor.Builder;
    auto& Context = Visitor.Context;

    auto Int32Max = new llvm::GlobalVariable(*Visitor.Module, Builder.getInt32Ty(), true, llvm::GlobalVariable::ExternalLinkage,
        llvm::ConstantInt::get(Builder.getInt32Ty(), (uint64_t )std::pow(2, 31) - 1));

    Visitor.MutableValues.emplace("class.Int.Max", CGValue{ Int32Max });
}