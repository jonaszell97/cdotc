#include "ILGenPass.h"

#include "IL/Constants.h"
#include "IL/Instructions.h"
#include "Sema/SemaPass.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::il;
using namespace cdot::support;

il::Value* ILGenPass::visitAwaitExpr(AwaitExpr *Expr)
{
   auto *Awaiter = visit(Expr->getExpr());
   if (Expr->isImmediateReturn())
      return Awaiter;

   return EmitCoroutineAwait(Awaiter);
}

void ILGenPass::CreateEndCleanupBlocks(CoroutineInfo &Info)
{
   auto IP = Builder.saveIP();

   if (!Info.EndBB) {
      Info.EndBB = Builder.CreateBasicBlock("coro.end");
      Builder.SetInsertPoint(Info.EndBB);

      Builder.CreateIntrinsicCall(Intrinsic::coro_end, {Info.Handle});
      Builder.CreateIntrinsicCall(Intrinsic::coro_return, {Info.Handle});
      Builder.CreateUnreachable();
   }

   if (!Info.CleanupBB) {
      Info.CleanupBB = Builder.CreateBasicBlock("coro.cleanup");
      Builder.SetInsertPoint(Info.CleanupBB);

      Builder.CreateIntrinsicCall(Intrinsic::coro_free, {Info.ID, Info.Handle});
      // Fall through to the end block.
      Builder.CreateBr(Info.EndBB);
   }

   Builder.restoreIP(IP);
}

il::Value *ILGenPass::EmitCoroutineAwait(il::Value *Awaitable)
{
   auto *Fn = getCurrentFn();
   auto &Info = SP.getCoroutineInfo(Awaitable->getType());
   auto &CoroInfo = CoroInfoMap[Fn];

   // Basic block to resume the current coroutine at.
   auto *ResumeBB = Builder.CreateBasicBlock("await.resume");

   // Block in which we suspend the current coroutine.
   auto *SuspendBB = Builder.CreateBasicBlock("await.suspend");

   // Get the Awaiter from the Awaitable.
   auto *Awaiter = Builder.CreateCall(getFunc(Info.AwaitableGetAwaiter),
                                      {Awaitable});

   // Cleanup the awaiter at the end of the coroutine.
   pushDefaultCleanup(Awaiter);

   // Check whether the value is ready immediately.
   auto *Ready = Builder.CreateCall(getFunc(Info.AwaitReady), Awaiter);
   Builder.CreateCondBr(Ready, ResumeBB, SuspendBB);

   // Save the coroutine state.
   Builder.SetInsertPoint(SuspendBB);

   auto *HandlePtr = CoroInfo.Handle;
   auto *Save = Builder.CreateIntrinsicCall(Intrinsic::coro_save, HandlePtr);

   // Create the coroutine handle to pass the awaiter.
   auto *Handle = Builder.CreateStructInit(
      cast<StructDecl>(Info.CoroHandleInit->getRecord()),
      getFunc(Info.CoroHandleInit),
      HandlePtr);

   Ready = Builder.CreateCall(getFunc(Info.AwaitSuspend), {Awaiter, Handle});
   auto *SuspendRealBB = Builder.CreateBasicBlock("await.suspend");

   Builder.CreateCondBr(Ready, ResumeBB, SuspendRealBB);

   // Suspend the current coroutine.
   Builder.SetInsertPoint(SuspendRealBB);
   auto *Discrim = Builder.CreateIntrinsicCall(Intrinsic::coro_suspend,
                                               {Save, Builder.GetFalse()});

   // Default means the coroutine was suspended.
   auto *EndBB = CoroInfo.EndBB;

   // 0 means the coroutine was resumed.
   BasicBlock *ResumeDst = ResumeBB;

   // 1 means the coroutine has ended.
   auto *CleanupBB = CoroInfo.CleanupBB;

   // Act according to the value returned by suspend.
   auto *Switch = Builder.CreateSwitch(Discrim, EndBB);
   Switch->addCase(Builder.GetConstantInt(Discrim->getType(), 0), ResumeDst);
   Switch->addCase(Builder.GetConstantInt(Discrim->getType(), 1), CleanupBB);

   // Get the promise value from the awaiter after resuming.
   Builder.SetInsertPoint(ResumeBB);
   return Builder.CreateCall(getFunc(Info.AwaitResume), {Awaiter});
}

il::Value* ILGenPass::EmitCoroutineReturn(il::Value *Value)
{
   auto *Fn = getCurrentFn();
   auto &Info = SP.getCoroutineInfo(cast<CallableDecl>(getDeclForValue(Fn)));
   auto &CoroInfo = CoroInfoMap[Fn];

   // Resolve the awaitable with the returned value.
   il::Value *Awaitable = Builder.CreateLoad(CoroInfo.Awaitable);
   Builder.CreateCall(getFunc(Info.AwaitableResolve),{Awaitable, Value});

   // Suspend the coroutine one final time.
   auto *Discrim = Builder.CreateIntrinsicCall(
      Intrinsic::coro_suspend,
      {Builder.GetTokenNone(), Builder.GetTrue()});

   // Default means the coroutine was suspended.
   auto *EndBB = CoroInfo.EndBB;

   // 0 means the coroutine was resumed.
   BasicBlock *ResumeDst = makeUnreachableBB();

   // 1 means the coroutine has ended.
   auto *CleanupBB = CoroInfo.CleanupBB;

   // Act according to the value returned by suspend.
   auto *Switch = Builder.CreateSwitch(Discrim, EndBB);
   Switch->addCase(Builder.GetConstantInt(Discrim->getType(), 0), ResumeDst);
   Switch->addCase(Builder.GetConstantInt(Discrim->getType(), 1), CleanupBB);

   Builder.SetInsertPoint(EndBB);
   return nullptr;
}

void ILGenPass::EmitCoroutinePrelude(CallableDecl *C, il::Function &F)
{
   auto &Info = SP.getCoroutineInfo(C);
   auto &CoroInfo = CoroInfoMap[getCurrentFn()];

   // Allocate the coroutine awaitable.
   auto *Awaitable = Builder.CreateStructInit(
      cast<StructDecl>(Info.AwaitableType->getRecord()),
      getFunc(Info.AwaitableInit), {});

   auto *AwaitableAlloc = Builder.CreateAlloca(Info.AwaitableType);
   Builder.CreateInit(Awaitable, AwaitableAlloc);

   // Get the coroutine ID.
   auto *ID = Builder.CreateIntrinsicCall(Intrinsic::coro_id, AwaitableAlloc);

   // Check whether we need to allocate space for the coroutine.
   auto *NeedAlloc = Builder.CreateIntrinsicCall(Intrinsic::coro_alloc, ID);
   auto *NeedAllocBB = Builder.CreateBasicBlock("coro.alloc");

   auto *MergeBB = Builder.CreateBasicBlock("coro.merge");
   MergeBB->addBlockArg(UInt8PtrTy, "mem");

   Builder.CreateCondBr(NeedAlloc, NeedAllocBB, MergeBB, {},
                        Builder.GetConstantNull(UInt8PtrTy));

   Builder.SetInsertPoint(NeedAllocBB);

   // Heap allocate memory for the coroutine.
   auto *CoroSize = Builder.CreateIntrinsicCall(Intrinsic::coro_size);
   auto *Mem = Builder.CreateAlloca(SP.getContext().getUInt8Ty(), CoroSize, 1,
                                    true, true);

   Builder.CreateBr(MergeBB, Builder.CreateAddrOf(Mem));
   Builder.SetInsertPoint(MergeBB);

   // Begin the coroutine.
   auto *Handle = Builder.CreateIntrinsicCall(Intrinsic::coro_begin,
                                              { ID, MergeBB->getBlockArg(0) });

   // Remember the promise and coroutine handle.
   CoroInfo.ID = ID;
   CoroInfo.Handle = Handle;
   CoroInfo.Awaitable = AwaitableAlloc;

   CreateEndCleanupBlocks(CoroInfo);
}