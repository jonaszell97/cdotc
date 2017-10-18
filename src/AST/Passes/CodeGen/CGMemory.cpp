//
// Created by Jonas Zell on 17.07.17.
//

#include <iostream>
#include "CGMemory.h"
#include "CodeGen.h"

#define POINTER_SIZE sizeof(int*)

llvm::Value* CGMemory::CreateAlloca(
   llvm::Type *type,
   bool heapAlloc,
   string name,
   llvm::Value* arr_size
) {
   assert(CGM.MALLOC != nullptr && "No malloc?");
   assert(!StackAllocBlock.empty());

   auto& Builder = CGM.Builder;
   bool isClass = false;
   llvm::Type* structTy = nullptr;
   auto ptrSize = Builder.getInt64(POINTER_SIZE);

   llvm::IRBuilder<>::InsertPointGuard guard(Builder);

   auto allocBlock = Builder.GetInsertBlock();
   if (!heapAlloc && (arr_size == nullptr || llvm::isa<llvm::Constant>(arr_size))) {
      allocBlock = StackAllocBlock.top();
   }

   if (allocBlock->getInstList().empty()) {
      Builder.SetInsertPoint(allocBlock);
   }
   else if (allocBlock->getInstList().size() > 1) {
      Builder.SetInsertPoint(allocBlock->getInstList().back().getPrevNode());
   }
   else {
      Builder.SetInsertPoint(&allocBlock->getInstList().back());
   }

   if (type->isStructTy()) {
      isClass = true;
      structTy = llvm::cast<llvm::StructType>(type);
   }

   llvm::Value* val;
   if (heapAlloc && isClass) {
      auto typeSize = CGM.GetStructSize(type);
      auto align = CGM.wordSizedInt(CGM.getAlignment(type));
      auto allocSize = arr_size == nullptr ? typeSize : Builder.CreateMul(align, arr_size);

      val = Builder.CreateBitCast(
         Builder.CreateCall(CGM.MALLOC, { allocSize }),
         structTy->getPointerTo(),
         name
      );
   }
   else if (heapAlloc) {
      auto align = CGM.wordSizedInt(CGM.getAlignment(type));
      auto allocSize = arr_size == nullptr ? ptrSize : Builder.CreateMul(align, arr_size);

      val = Builder.CreateBitCast(
         Builder.CreateCall(CGM.MALLOC, { allocSize }),
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
      alloc->setAlignment(CGM.getAlignment(type));

      if (constSize) {
         // later code never expects an array type
         val = Builder.CreateBitCast(alloc, type->getPointerTo());
      }
      else {
         val = alloc;
      }
   }

   return val;
}