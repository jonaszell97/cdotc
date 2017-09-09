//
// Created by Jonas Zell on 17.07.17.
//

#include "CGMemory.h"
#include "CodeGen.h"

#define POINTER_SIZE sizeof(int*)

llvm::BasicBlock* CGMemory::CurrentEntryBlock = nullptr;

llvm::Value* CGMemory::CreateAlloca(
   llvm::Type *type,
   bool heapAlloc,
   string name,
   llvm::Value* arr_size
) {
   assert(CodeGen::MALLOC != nullptr && "No malloc?");

   if (CurrentEntryBlock == nullptr) {
      auto glob = new llvm::GlobalVariable(*CodeGen::Module, type, false, llvm::GlobalVariable::ExternalLinkage,
         llvm::ConstantAggregateZero::get(type), name);

      return glob;
   }

   auto alloc_block = CurrentEntryBlock;
   auto& Builder = CodeGen::Builder;
   auto ip = Builder.GetInsertBlock();
   bool isClass = false;
   llvm::Type* structTy = nullptr;
   auto ptrSize = Builder.getInt64(POINTER_SIZE);

   if (heapAlloc && alloc_block->getNextNode() != nullptr) {
      alloc_block = alloc_block->getNextNode();
   }

   if (type->isStructTy()) {
      isClass = true;
      structTy = llvm::cast<llvm::StructType>(type);
   }

   // can't create it in alloc block because we need the arr_size variable
   if (arr_size == nullptr || llvm::isa<llvm::Constant>(arr_size)) {
      Builder.SetInsertPoint(alloc_block);
   }

   llvm::Value* alloca;

   if (heapAlloc && isClass) {
//      auto type_size = Builder.CreatePtrToInt(
//         Builder.CreateGEP(llvm::ConstantPointerNull::get(structTy->getPointerTo()), Builder.getInt64(1)),
//         Builder.getIntNTy(POINTER_SIZE * 8)
//      );

      auto typeSize = CodeGen::GetStructSize(type);

      auto allocSize = arr_size == nullptr ? typeSize : Builder.CreateMul(typeSize, arr_size);
      llvm::Value* struct_alloc = Builder.CreateBitCast(
         Builder.CreateCall(CodeGen::MALLOC, { allocSize }),
         structTy->getPointerTo(),
         name
      );

      alloca = struct_alloc;
   }
   else if (heapAlloc) {
      auto allocSize = arr_size == nullptr ? ptrSize : Builder.CreateMul(ptrSize, arr_size);

      auto alloc = Builder.CreateBitCast(
         Builder.CreateCall(CodeGen::MALLOC, { allocSize }),
         type->getPointerTo(),
         name
      );

      alloca = alloc;
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
         alloca = Builder.CreateBitCast(alloc, type->getPointerTo());
      }
      else {
         alloca = alloc;
      }
   }

   Builder.SetInsertPoint(ip);
   return alloca;
}