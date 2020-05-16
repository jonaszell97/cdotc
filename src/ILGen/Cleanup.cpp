#include "cdotc/ILGen/Cleanup.h"

#include "cdotc/AST/Decl.h"
#include "cdotc/IL/Function.h"
#include "cdotc/IL/Instructions.h"
#include "cdotc/ILGen/ILGenPass.h"
#include "cdotc/Sema/SemaPass.h"

using namespace cdot::il;
using namespace cdot::support;

namespace cdot {

void Cleanup::anchor() {}

void DefaultCleanup::Emit(ast::ILGenPass& ILGen)
{
   if (Flags & Ignored)
      return;

   ast::ILGenPass::TerminatorRAII terminatorRAII(ILGen);
   deinitializeValue(ILGen, Val);
}

void DefaultCleanup::deinitializeValue(ast::ILGenPass& ILGen, il::Value* Val)
{
   auto& Builder = ILGen.Builder;
   auto ty = Val->getType()->removeReference();

   auto& Sema = ILGen.getSema();
   if (!Sema.NeedsDeinitilization(ty)) {
      return;
   }

   auto *DeinitFn = ILGen.GetDeinitFn(ty);

   ILBuilder::SynthesizedRAII Synthesized(Builder);
   if (!Val->isLvalue()) {
      auto *Alloc = Builder.CreateAlloca(Val->getType());
      Builder.CreateStore(Val, Alloc);
      Val = Alloc;
   }
   else if (!Val->getType()->isMutableReferenceType()) {
      Val = Builder.CreateBitCast(CastKind::BitCast,
          Val, DeinitFn->getEntryBlock()->getArgs().front().getType());
   }

   auto *Call = Builder.CreateCall(DeinitFn, Val);
   Call->setDeinitCall(true);
}

void BorrowCleanup::Emit(ast::ILGenPass& ILGen)
{
   ast::ILGenPass::TerminatorRAII terminatorRAII(ILGen);
   ILGen.Builder.CreateEndBorrow(BeginBorrow, EndLoc,
                                 BeginBorrow->isMutableBorrow());
}

CleanupStack& CleanupStack::operator=(CleanupStack&& other) noexcept
{
   this->~CleanupStack();
   new (this) CleanupStack(std::move(other));

   return *this;
}

namespace {

/// A CleanupBuffer is a location to which to temporarily copy a
/// cleanup.
class CleanupBuffer {
   llvm::SmallVector<char, sizeof(Cleanup) + 10 * sizeof(void*)> data;

public:
   CleanupBuffer(const Cleanup& cleanup)
   {
      size_t size = cleanup.allocated_size();
      data.set_size(size);
      memcpy(data.data(), reinterpret_cast<const void*>(&cleanup), size);
   }

   Cleanup& getCopy() { return *reinterpret_cast<Cleanup*>(data.data()); }
};

} // anonymous namespace

void CleanupStack::emitCleanups(CleanupsDepth depth, bool popCleanups,
                                bool doEmitCleanups)
{
   START_TIMER("IL Cleanups");

   auto begin = stack.stable_begin();
   while (begin != depth) {
      auto iter = stack.find(begin);

      Cleanup& stackCleanup = *iter;

      // Copy it off the cleanup stack in case the cleanup pushes a new cleanup
      // and the backing storage is re-allocated.
      CleanupBuffer buffer(stackCleanup);
      Cleanup& cleanup = buffer.getCopy();

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

bool CleanupStack::ignoreValue(il::Value *Val)
{
   return ignoreValue(stack.stable_end(), Val);
}

bool CleanupStack::ignoreValue(CleanupsDepth depth, il::Value* Val)
{
   auto begin = stack.stable_begin();
   while (begin != depth) {
      auto iter = stack.find(begin);
      Cleanup& cleanup = *iter;

      // Advance stable iterator.
      begin = stack.stabilize(++iter);

      if (cleanup.ignoreValue(Val))
         return true;

      stack.checkIterator(begin);
   }

   return false;
}

void CleanupStack::emitUntil(CleanupsDepth depth)
{
   emitCleanups(depth, true, true);
}

void CleanupStack::popUntil(CleanupsDepth depth)
{
   emitCleanups(depth, true, false);
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