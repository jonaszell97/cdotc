//
// Created by Jonas Zell on 17.07.17.
//

#include "CGMemory.h"
#include "CodeGenVisitor.h"

#define POINTER_SIZE sizeof(int*)

llvm::BasicBlock* CGMemory::CurrentEntryBlock = nullptr;

llvm::Value* CGMemory::CreateAlloca(
    llvm::Type *type,
    bool heapAlloc,
    string name,
    llvm::Value* arr_size
) {
    assert(CodeGenVisitor::MALLOC != nullptr && "No malloc?");

    if (CurrentEntryBlock == nullptr) {
        auto glob = new llvm::GlobalVariable(*CodeGenVisitor::Module, type, false, llvm::GlobalVariable::ExternalLinkage,
            llvm::ConstantAggregateZero::get(type), name);

        return glob;
    }

    auto alloc_block = CurrentEntryBlock;
    auto& Builder = CodeGenVisitor::Builder;
    auto ip = Builder.GetInsertBlock();
    bool isClass = false;
    llvm::StructType* structTy = nullptr;
    auto ptrSize = Builder.getInt64(POINTER_SIZE);

    if (heapAlloc && alloc_block->getNextNode() != nullptr) {
        alloc_block = alloc_block->getNextNode();
    }

    if (llvm::isa<llvm::StructType>(type)) {
        isClass = true;
        structTy = llvm::cast<llvm::StructType>(type);
    }

    // can't create it in alloc block because we need the arr_size variable
    if (arr_size == nullptr || llvm::isa<llvm::Constant>(arr_size)) {
        Builder.SetInsertPoint(alloc_block);
    }

    llvm::Value* alloca;

    if (heapAlloc && isClass) {
        auto type_size = Builder.CreatePtrToInt(
            Builder.CreateGEP(llvm::ConstantPointerNull::get(structTy->getPointerTo()), Builder.getInt64(1)),
            Builder.getIntNTy(POINTER_SIZE * 8)
        );

        auto allocSize = arr_size == nullptr ? type_size : Builder.CreateMul(type_size, arr_size);
        llvm::Value* struct_alloc = Builder.CreateBitCast(
            Builder.CreateCall(CodeGenVisitor::MALLOC, { allocSize }),
            structTy->getPointerTo(),
            name
        );

        alloca = struct_alloc;
    }
    else if (heapAlloc) {
        auto allocSize = arr_size == nullptr ? ptrSize : Builder.CreateMul(ptrSize, arr_size);
        auto alloc = Builder.CreateBitCast(
            Builder.CreateCall(CodeGenVisitor::MALLOC, { allocSize }),
            type->getPointerTo(),
            name
        );

        alloca = alloc;
    }
    else {
        auto alloc = Builder.CreateAlloca(type, arr_size, name);
        alloc->setAlignment(CodeGenVisitor::getAlignment(type));

        alloca = alloc;
    }

    Builder.SetInsertPoint(ip);
    return alloca;
}