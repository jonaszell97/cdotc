//
// Created by Jonas Zell on 06.10.17.
//

#include "CGException.h"
#include "CodeGen.h"

#include "llvm/IR/Module.h"

#include "../../../Variant/Type/ObjectType.h"
#include "../../Statement/Exception/ThrowStmt.h"
#include "../../Statement/Exception/TryStmt.h"
#include "../../Expression/Expression.h"
#include "../../Expression/TypeRef.h"
#include "CGMemory.h"
#include "CGCast.h"

namespace cdot {
namespace eh {

using namespace ::cdot::codegen;
   
   CGException::CGException(CodeGen &CGM) : CGM(CGM)
   {
      
   }

   llvm::Constant *CGException::getBeginCatchFn()
   {
      if (!BeginCatchFn) {
         BeginCatchFn = CGM.Module->getOrInsertFunction(
            "__cdot_begin_catch",
            CGM.Builder.getInt8PtrTy(),
            CGM.Builder.getInt8PtrTy(),
            nullptr
         );
      }
   
      return BeginCatchFn;
   }
   
   llvm::Constant *CGException::getEndCatchFn()
   {
      if (!EndCatchFn) {
         EndCatchFn = CGM.Module->getOrInsertFunction(
            "__cdot_end_catch",
            CGM.Builder.getVoidTy(),
            CGM.Builder.getInt8PtrTy(),
            nullptr
         );
      }
   
      return EndCatchFn;
   }
   
   llvm::Constant *CGException::getThrowFn()
   {
      if (!ThrowFn) {
         ThrowFn = CGM.Module->getOrInsertFunction(
            "__cdot_throw",
            CGM.Builder.getVoidTy(),
            CGM.Builder.getInt8PtrTy(),
            nullptr
         );
      }
   
      return ThrowFn;
   }
   
   llvm::Constant *CGException::getRethrowFn()
   {
      if (!RethrowFn) {
         RethrowFn = CGM.Module->getOrInsertFunction(
            "__cdot_rethrow",
            CGM.Builder.getVoidTy(),
            CGM.Builder.getInt8PtrTy(),
            nullptr
         );
      }
   
      return RethrowFn;
   }
   
   llvm::Constant *CGException::getAllocExcFn()
   {
      if (!AllocExcFn) {
         AllocExcFn = CGM.Module->getOrInsertFunction(
            "__cdot_allocate_exception",
            CGM.Builder.getInt8PtrTy(),
            CGM.WordTy,
            CGM.Builder.getInt8PtrTy(),
            CGM.Builder.getInt8PtrTy(),
            CGM.Builder.getInt8PtrTy(),
            nullptr
         );
      }
   
      return AllocExcFn;
   }
   
   llvm::Constant *CGException::getPersonalityFn()
   {
      if (!PersonalityFn) {
         PersonalityFn = CGM.Module->getOrInsertFunction(
            "__cdot_personality",
            CGM.Builder.getInt32Ty(),
            CGM.Builder.getInt32Ty(),
            CGM.Builder.getInt32Ty(),
            CGM.Builder.getInt64Ty(),
            CGM.Builder.getInt8PtrTy(),
            CGM.Builder.getInt8PtrTy(),
            nullptr
         );
      }
   
      return PersonalityFn;
   }

}
}

using namespace cdot::eh;

void CodeGen::visit(TryStmt *node)
{
   bool hasFinally = node->finallyBlock != nullptr;
   bool hasCatches = !node->catchBlocks.empty();

   assert(!functions.empty());

   auto& currentFunc = functions.back();
   if (!currentFunc->hasPersonalityFn()) {
      currentFunc->setPersonalityFn(Exc->getPersonalityFn());
   }

   auto landBB = CreateBasicBlock("try.catch");
   auto contBB = CreateBasicBlock("try.cont");

   auto ip = Builder.GetInsertBlock();
   Builder.SetInsertPoint(landBB);

   Builder.CreateLandingPad(
      llvm::StructType::get(Builder.getInt8PtrTy(), Builder.getInt32Ty(), nullptr),
      (unsigned int)node->catchBlocks.size()
   );

   llvm::BasicBlock *finallyBB = nullptr;
   llvm::AllocaInst *brAlloca = nullptr;

   if (node->finallyBlock) {
      finallyBB = CreateBasicBlock("try.finally");
      Builder.SetInsertPoint(finallyBB);

      brAlloca = llvm::cast<llvm::AllocaInst>(
         Mem->CreateAlloca(Builder.getInt8PtrTy())
      );
   }

   Builder.SetInsertPoint(ip);
   EHStack.emplace_back(landBB, finallyBB, contBB, brAlloca, std::vector<llvm::BasicBlock*>());

   node->body->accept(this);

   if (hasCatches) {
      EmitCatchClauses(node);
   }

   if (hasFinally) {
      EmitFinally(node);
   }

   if (!hasCatches && !hasFinally) {
      EmitTryNoClauses(node);
   }
}

void CodeGen::EmitTryNoClauses(
   TryStmt *node)
{
   auto eh = EHStack.back();
   EHStack.pop_back();

   auto landBB = eh.landingPad;
   auto& contBB = eh.contBB;

   if (!Builder.GetInsertBlock()->getTerminator()) {
      Builder.CreateBr(contBB);
   }

   Builder.SetInsertPoint(landBB);

   // catch-none
   auto landingPad = llvm::cast<llvm::LandingPadInst>(
      &landBB->getInstList().front());
   landingPad->addClause(
      llvm::ConstantArray::get(
         llvm::ArrayType::get(Builder.getInt8PtrTy(), 1),
         { llvm::ConstantPointerNull::get(Builder.getInt8PtrTy()) }
      )
   );

   Builder.CreateCall(TERMINATE);

   Builder.CreateUnreachable();
   Builder.SetInsertPoint(contBB);
}

void CodeGen::EmitFinally(
   TryStmt *node)
{
   auto hasCatches = !node->catchBlocks.empty();
   auto eh = EHStack.back();
   EHStack.pop_back();

   auto landingPad = llvm::cast<llvm::LandingPadInst>(
      &eh.landingPad->getInstList().front()
   );

   auto& contBB = eh.contBB;

   llvm::BasicBlock *rethrowBB;
   if (!hasCatches) {
      rethrowBB = CreateBasicBlock("try.rethrow");
   }

   auto tryBB = Builder.GetInsertBlock();
   if (tryBB->getTerminator() == nullptr && tryBB != contBB) {
      CreateStore(llvm::BlockAddress::get(tryBB), eh.branchTarget);
      eh.targets.push_back(tryBB);

      Builder.CreateBr(eh.finallyBB);
   }

   if (!hasCatches) {
      CreateStore(llvm::BlockAddress::get(eh.landingPad), eh.branchTarget);
      eh.targets.push_back(eh.landingPad);
   }

   Builder.SetInsertPoint(eh.finallyBB);
   node->finallyBlock->accept(this);

   if (!Builder.GetInsertBlock()->getTerminator()) {
      if (eh.targets.empty()) {
         eh.branchTarget->removeFromParent();
         if (!hasCatches) {
            Builder.CreateBr(rethrowBB);
         }
         else {
            Builder.CreateBr(contBB);
         }
      }
      else {
         // give control back to the appropriate return statement for each
         // incoming basic block
         auto indirectBr = Builder.CreateIndirectBr(CreateLoad(eh.branchTarget));
         for (const auto& target : eh.targets) {
            indirectBr->addDestination(target);
         }
      }
   }
   else {
      eh.branchTarget->removeFromParent();
   }

   // catch-all to guarantee execution of the finally
   Builder.SetInsertPoint(eh.landingPad);
   landingPad->addClause(llvm::ConstantPointerNull::get(Builder.getInt8PtrTy()));

   // if there are catches, EmitCatchClauses will handle branching to the finally block
   if (!hasCatches) {
      auto excnAlloc = Mem->CreateAlloca(Builder.getInt8PtrTy());
      auto excObj = Builder.CreateExtractValue(landingPad, 0);
      CreateStore(excObj, excnAlloc);

      Builder.CreateBr(eh.finallyBB);

      // rethrow the exception if the finally block was entered from the landing pad
      Builder.SetInsertPoint(rethrowBB);

      CreateCall(Exc->getRethrowFn(), {CreateLoad(excnAlloc)});

      Builder.CreateUnreachable();
   }

   Builder.SetInsertPoint(contBB);
}

void CodeGen::EmitCatchClauses(
   TryStmt *node)
{
   auto &eh = EHStack.back();
   auto contBB = eh.contBB;
   auto landBB = eh.landingPad;

   // remove landing pad so that calls are not transformed into invokes
   // but returns are still considering the finally
   eh.landingPad = nullptr;

   bool hasFinally = node->finallyBlock != nullptr;
   llvm::Value* excAlloc = Mem->CreateAlloca(Builder.getInt8PtrTy());;
   llvm::Value* selectorAlloc = Mem->CreateAlloca(Builder.getInt32Ty());

   if (!Builder.GetInsertBlock()->getTerminator()) {
      if (hasFinally) {
         CreateStore(llvm::BlockAddress::get(contBB), eh.branchTarget);
         eh.targets.push_back(contBB);

         Builder.CreateBr(eh.finallyBB);
      }
      else {
         Builder.CreateBr(contBB);
      }
   }

   Builder.SetInsertPoint(landBB);
   auto landingPad = llvm::cast<llvm::LandingPadInst>(&landBB->getInstList().front());

   auto excPtr = Builder.CreateExtractValue(landingPad, { 0 });
   auto givenSel = Builder.CreateExtractValue(landingPad, { 1 });
   CreateStore(excPtr, excAlloc);
   CreateStore(givenSel, selectorAlloc);

   std::vector<llvm::BasicBlock*> catchBBs;
   std::vector<llvm::BasicBlock*> compBBs;
   for (auto& catchBlock : node->catchBlocks) {
      auto caughtTy = catchBlock.caughtType->getType();
      landingPad->addClause(llvm::cast<llvm::Constant>(
         toInt8Ptr(getTypeInfo(*caughtTy))
      ));

      auto catchBB = CreateBasicBlock("catch." + caughtTy.toString());
      catchBBs.push_back(catchBB);
      compBBs.push_back(CreateBasicBlock("catch.comp."));

      Builder.SetInsertPoint(catchBB);
      llvm::Value *excObj = Builder.CreateCall(
         Exc->getBeginCatchFn(), { CreateLoad(excAlloc) }
      );

      llvm::Type *llvmTy = caughtTy->getLlvmType();
      if (!llvmTy->isPointerTy()) {
         llvmTy = llvmTy->getPointerTo();
      }

      excObj = Builder.CreateBitCast(
         excObj,
         llvmTy
      );

      llvm::Value *exc;
      if (caughtTy->isNumeric()) {
         auto ld = CreateLoad(excObj);
         auto alloc = Mem->CreateAlloca(ld->getType());
         CreateStore(ld, alloc);
         exc = alloc;
      }
      else if (caughtTy->needsMemCpy()) {
         auto alloc = Mem->CreateAlloca(caughtTy->getLlvmType());
         Builder.CreateMemCpy(alloc, excObj, GetStructSize(caughtTy->getLlvmType()),
            caughtTy->getAlignment());

         exc = alloc;
      }
      else {
         auto alloc = Mem->CreateAlloca(excObj->getType());
         CreateStore(excObj, alloc);
         exc = alloc;
      }

      declareVariable(catchBlock.identifier, exc);
      catchBlock.body->accept(this);

      if (!Builder.GetInsertBlock()->getTerminator()) {
         if (hasFinally) {
            CreateStore(llvm::BlockAddress::get(catchBB), eh.branchTarget);
            eh.targets.push_back(catchBB);

            Builder.CreateBr(eh.finallyBB);
         }
         else {
            Builder.CreateBr(contBB);
         }
      }
   }

   Builder.SetInsertPoint(landBB);
   Builder.CreateBr(compBBs.front());

   auto typeIdFor = llvm::Intrinsic::getDeclaration(
      Module,
      llvm::Intrinsic::ID::eh_typeid_for
   );

   size_t i = 0;
   for (const auto& compBB : compBBs) {
      auto typeInfo = toInt8Ptr(
         getTypeInfo(*node->catchBlocks[i].caughtType->getType())
      );

      Builder.SetInsertPoint(compBB);
      auto sel = Builder.CreateCall(typeIdFor, { typeInfo });
      auto next = i == compBBs.size() - 1 ? contBB : compBBs[i + 1];

      auto eq = Builder.CreateICmpEQ(CreateLoad(selectorAlloc), sel);
      Builder.CreateCondBr(eq, catchBBs[i], next);

      ++i;
   }

   Builder.SetInsertPoint(contBB);

   if (hasFinally) {
      eh.landingPad = landBB;
   }
}

void CodeGen::visit(ThrowStmt *node)
{
   auto thrownVal = getResult(node->getThrownVal());
   llvm::Function *descFn = node->descFn ? getOwnDecl(node->descFn)
                                         : nullptr;

   if (!node->thrownType->isRefcounted()) {
      auto size = node->thrownType->getSize();
      llvm::Value *errAlloc = Builder.CreateCall(MALLOC,
                                                 { wordSizedInt(size) });

      Builder.CreateMemCpy(errAlloc, thrownVal, size, 1);
      thrownVal = errAlloc;
   }
   else {
      IncrementRefCount(thrownVal);
   }

   auto alloc = Builder.CreateCall(
      Exc->getAllocExcFn(),
      {
         GetStructSize(thrownVal->getType()),
         toInt8Ptr(thrownVal),
         toInt8Ptr(getTypeInfo(node->getThrownType())),
         toInt8Ptr(descFn)
      }
   );

   CreateCall(Exc->getThrowFn(), {alloc});
   Builder.CreateUnreachable();
}