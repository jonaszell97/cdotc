//
// Created by Jonas Zell on 27.04.18.
//

#include "Cleanup.h"

#include "AST/Decl.h"
#include "IL/Instructions.h"
#include "IL/Function.h"
#include "ILGen/ILGenPass.h"
#include "Sema/SemaPass.h"

using namespace cdot::il;
using namespace cdot::support;

namespace cdot {

void Cleanup::anchor() {}

void DefaultCleanup::Emit(ast::ILGenPass &ILGen)
{
   if (Flags & Ignored)
      return;

   ast::ILGenPass::TerminatorRAII terminatorRAII(ILGen);
   deinitializeValue(ILGen, Val);
}

void DefaultCleanup::deinitializeValue(ast::ILGenPass &ILGen,
                                       il::Value *Val) {
   auto &Builder = ILGen.Builder;
   auto ty = Val->getType()->stripReference();

   if (Val->isLvalue())
      Builder.CreateLifetimeEnd(Val);

   auto &Sema = ILGen.getSema();
   if (!Sema.NeedsDeinitilization(ty))
      return;

   ILBuilder::SynthesizedRAII SR(Builder);

   if (Val->isLvalue())
      Val = Builder.CreateLoad(Val);

   if (ty->isRefcounted() || ty->isLambdaType() || ty->isBoxType()) {
      Builder.CreateRelease(Val);
      return;
   }

   if (RecordType *Obj = ty->asRecordType()) {
      if (Obj->isRawEnum())
         return;

      if (Obj->isProtocol()) {
         Builder.CreateIntrinsicCall(Intrinsic::deinit_existential, Val);
         return;
      }

      auto deinit = ty->getRecord()->getDeinitializer();
      assert(deinit && "trivially deinitializable type!");

      deinit = ILGen.getSema().maybeInstantiateTemplateMember(
         ty->getRecord(), deinit);

      if (ILGen.inCTFE()) {
         if (!ILGen.registerCalledFunction(deinit, deinit)) {
            return;
         }
      }

      ILGen.CreateCall(deinit, { Val });
      return;
   }

   if (ArrayType *Arr = ty->asArrayType()) {
      auto NumElements = Arr->getNumElements();
      for (unsigned i = 0; i < NumElements; ++i) {
         auto GEP = Builder.CreateGEP(Val, i);
         auto Ld = Builder.CreateLoad(GEP);

         deinitializeValue(ILGen, Ld);
      }

      return;
   }

   if (TupleType *Tup = ty->asTupleType()) {
      size_t i = 0;
      auto Cont = Tup->getContainedTypes();
      size_t numTys = Cont.size();

      while (i < numTys) {
         if (!Sema.NeedsDeinitilization(Cont[i])) {
            ++i;
            continue;
         }

         auto val = Builder.CreateTupleExtract(Val, i);
         auto Ld = Builder.CreateLoad(val);

         deinitializeValue(ILGen, Ld);
         ++i;
      }

      return;
   }
}

void BorrowCleanup::Emit(ast::ILGenPass &ILGen)
{
   ast::ILGenPass::TerminatorRAII terminatorRAII(ILGen);
   ILGen.Builder.CreateEndBorrow(BeginBorrow, EndLoc,
                                 BeginBorrow->isMutableBorrow());
}

CleanupStack& CleanupStack::operator=(CleanupStack &&other) noexcept
{
   this->~CleanupStack();
   new(this) CleanupStack(std::move(other));

   return *this;
}

namespace {

/// A CleanupBuffer is a location to which to temporarily copy a
/// cleanup.
class CleanupBuffer {
   llvm::SmallVector<char, sizeof(Cleanup) + 10 * sizeof(void *)> data;

public:
   CleanupBuffer(const Cleanup &cleanup) {
      size_t size = cleanup.allocated_size();
      data.set_size(size);
      memcpy(data.data(), reinterpret_cast<const void *>(&cleanup), size);
   }

   Cleanup &getCopy() { return *reinterpret_cast<Cleanup *>(data.data()); }
};

} // anonymous namespace

void CleanupStack::emitCleanups(CleanupsDepth depth, bool popCleanups,
                                bool doEmitCleanups) {
   auto begin = stack.stable_begin();
   while (begin != depth) {
      auto iter = stack.find(begin);

      Cleanup &stackCleanup = *iter;

      // Copy it off the cleanup stack in case the cleanup pushes a new cleanup
      // and the backing storage is re-allocated.
      CleanupBuffer buffer(stackCleanup);
      Cleanup &cleanup = buffer.getCopy();

      // Advance stable iterator.
      begin = stack.stabilize(++iter);

      // Pop now.
      if (popCleanups)
         stack.pop();

      if (doEmitCleanups)
         cleanup.Emit(ILGen);

      stack.checkIterator(begin);
   }
}

void CleanupStack::endScope(CleanupsDepth depth, bool doEmitCleanups)
{
   stack.checkIterator(depth);

   if (stack.begin() == stack.find(depth)) {
      return;
   }

   emitCleanups(depth, true, doEmitCleanups);
}

bool CleanupStack::ignoreValue(CleanupsDepth depth, il::Value *Val)
{
   auto begin = stack.stable_begin();
   while (begin != depth) {
      auto iter = stack.find(begin);
      Cleanup &cleanup = *iter;

      // Advance stable iterator.
      begin = stack.stabilize(++iter);

      if (cleanup.ignoreValue(Val))
         return true;

      stack.checkIterator(begin);
   }

   return false;
}

void CleanupStack::emitUntilWithoutPopping(CleanupsDepth depth)
{
   emitCleanups(depth, false, true);
}

void CleanupScope::popImpl(bool emitCleanups)
{
   Cleanups.stack.checkIterator(depth);
   Cleanups.stack.checkIterator(Cleanups.innermostScope);
   assert(Cleanups.innermostScope == depth && "popping scopes out of order");

   Cleanups.innermostScope = savedInnermostScope;
   Cleanups.endScope(depth, emitCleanups);

   Cleanups.stack.checkIterator(Cleanups.innermostScope);
}

} // namespace cdot