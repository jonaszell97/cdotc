//
// Created by Jonas Zell on 17.07.17.
//

#include <iostream>
#include "CGMemory.h"
#include "CodeGen.h"

#define POINTER_SIZE sizeof(int*)

std::stack<llvm::BasicBlock*> CGMemory::StackAllocBlock = {};
std::stack<llvm::BasicBlock*> CGMemory::HeapAllocBlock = {};

llvm::Value* CGMemory::CreateAlloca(
   llvm::Type *type,
   bool heapAlloc,
   string name,
   llvm::Value* arr_size
) {
   assert(CodeGen::MALLOC != nullptr && "No malloc?");

   if (StackAllocBlock.empty() || HeapAllocBlock.empty()) {
      auto glob = new llvm::GlobalVariable(*CodeGen::Module, type, false, llvm::GlobalVariable::ExternalLinkage,
         llvm::ConstantAggregateZero::get(type), name);

      return glob;
   }

   auto& Builder = CodeGen::Builder;
   bool isClass = false;
   llvm::Type* structTy = nullptr;
   auto ptrSize = Builder.getInt64(POINTER_SIZE);

   auto allocBlock = Builder.GetInsertBlock();
   if (arr_size == nullptr || llvm::isa<llvm::Constant>(arr_size)) {
      if (heapAlloc) {
         allocBlock = HeapAllocBlock.top();
      }
      else {
         allocBlock = StackAllocBlock.top();
      }
   }

   auto prevInsert = Builder.GetInsertBlock();
   if (!allocBlock->getInstList().empty()) {
      Builder.SetInsertPoint(&allocBlock->getInstList().front());
   }
   else {
      Builder.SetInsertPoint(allocBlock);
   }

   if (type->isStructTy()) {
      isClass = true;
      structTy = llvm::cast<llvm::StructType>(type);
   }

   llvm::Value* val;
   if (heapAlloc && isClass) {
      auto typeSize = CodeGen::GetStructSize(type);
      auto allocSize = arr_size == nullptr ? typeSize : Builder.CreateMul(typeSize, arr_size);

      val = Builder.CreateBitCast(
         Builder.CreateCall(CodeGen::MALLOC, { allocSize }),
         structTy->getPointerTo(),
         name
      );
   }
   else if (heapAlloc) {
      auto allocSize = arr_size == nullptr ? ptrSize : Builder.CreateMul(ptrSize, arr_size);

      val = Builder.CreateBitCast(
         Builder.CreateCall(CodeGen::MALLOC, { allocSize }),
         type->getPointerTo(),
         name
      );
   }
   else {
      llvm::AllocaInst *alloc;
      bool constSize = false;
      if (arr_size != nullptr && llvm::isa<llvm::ConstantInt>(arr_size)) {
         llvm::Type* arrType = llvm::ArrayType::get(type,
            llvm::cast<llvm::ConstantInt>(arr_size)->getSExtValue()
         );

         alloc = Builder.CreateAlloca(arrType);
         constSize = true;
      }
      else {
         alloc = Builder.CreateAlloca(type, arr_size);
      }

      alloc->setName(name);
      alloc->setAlignment(CodeGen::getAlignment(type));

      if (constSize) {
         // later code never expects an array type
         val = Builder.CreateBitCast(alloc, type->getPointerTo());
      }
      else {
         val = alloc;
      }
   }

   Builder.SetInsertPoint(prevInsert);
   return val;
}