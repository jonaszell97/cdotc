//
// Created by Jonas Zell on 17.07.17.
//

#include "CGMemory.h"
#include "CGType.h"
#include "CodeGenVisitor.h"

llvm::BasicBlock* CGMemory::CurrentEntryBlock = nullptr;

llvm::Value* CGMemory::CreateAlloca(llvm::Type *type, std::string name,
    llvm::Value* arr_size, unsigned short align, bool heap_alloc, llvm::BasicBlock* alloc_block)
{
//    heap_alloc = true;
    if (alloc_block == nullptr) {
        if (CurrentEntryBlock == nullptr) {
            auto glob = new llvm::GlobalVariable(*CodeGenVisitor::Module, type, false, llvm::GlobalVariable::ExternalLinkage,
                llvm::ConstantAggregateZero::get(type));

            return glob;
        }

        alloc_block = CurrentEntryBlock;
    }

    auto& Builder = CodeGenVisitor::Builder;

    if (heap_alloc && llvm::isa<llvm::StructType>(type)) {
        auto struct_type = llvm::cast<llvm::StructType>(type);

        // getelementptr hack to get type size
        auto type_size = Builder.CreatePtrToInt(
            Builder.CreateGEP(llvm::ConstantPointerNull::get(struct_type->getPointerTo()),
            { llvm::ConstantInt::get(Builder.getInt64Ty(), 1) }), Builder.getInt64Ty()
        );

        auto alloc = Builder.CreateCall(CodeGenVisitor::Functions["malloc"], { type_size });
        return Builder.CreateBitCast(alloc, struct_type->getPointerTo());
    }

    if (heap_alloc) {
        llvm::Value* alloc_size = llvm::ConstantInt::get(Builder.getInt64Ty(), 8);
        if (arr_size != nullptr) {
            alloc_size = Builder.CreateNSWMul(alloc_size, arr_size);
        }

        auto alloc = Builder.CreateCall(CodeGenVisitor::Functions["malloc"], { alloc_size });
        return Builder.CreateBitCast(alloc, type->getPointerTo());
    }

    auto ip = Builder.GetInsertBlock();

    // can't create it in alloc block because we need the arr_size variable
    if (arr_size == nullptr || llvm::isa<llvm::Constant>(arr_size)) {
        Builder.SetInsertPoint(alloc_block);
    }

    llvm::AllocaInst* alloca = Builder.CreateAlloca(type, arr_size, name);

    Builder.SetInsertPoint(ip);

    return alloca;
}

llvm::Value* CGMemory::CreateAlloca(TypeSpecifier &type, std::string name,
        llvm::Value* arr_size, bool heap_alloc, llvm::BasicBlock* alloc_block) {
    return CreateAlloca(CGType::getType(type), name, arr_size, CGType::getAlignment(type), heap_alloc, alloc_block);
}