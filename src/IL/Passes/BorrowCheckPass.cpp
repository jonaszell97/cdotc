#include "cdotc/IL/Passes/BorrowCheckPass.h"

#include "cdotc/IL/Analysis/AccessPathDescriptor.h"
#include "cdotc/IL/Analysis/AccessPathIterator.h"
#include "cdotc/IL/Analysis/Dominance.h"
#include "cdotc/IL/Analysis/MemoryUse.h"
#include "cdotc/IL/Analysis/UnsafeAnalysis.h"
#include "cdotc/IL/BasicBlock.h"
#include "cdotc/IL/Function.h"
#include "cdotc/IL/Instructions.h"
#include "cdotc/IL/Passes/DataflowProblem.h"
#include "cdotc/ILGen/ILGenPass.h"
#include "cdotc/Sema/SemaPass.h"

#include <llvm/ADT/BitVector.h>

using llvm::BitVector;
using namespace cdot::support;

namespace cdot {
namespace il {

static il::Value* LookThroughLoad(il::Value* V)
{
   if (auto Ld = dyn_cast<LoadInst>(V))
      return Ld->getOperand(0);

   // also look through MutRefToRef casts
   if (auto BC = dyn_cast<BitCastInst>(V))
      if (BC->getType()->isReferenceType()
          && BC->getOperand(0)->getType()->isReferenceType()) {
         return BC->getOperand(0);
      }

   return V;
}

void BorrowCheckPass::computeMemoryAccesses(il::Function& F)
{
   for (auto& B : F) {
      for (auto& I : B) {
         if (UA->isUnsafe(I))
            continue;

         if (auto Move = dyn_cast<MoveInst>(&I)) {
            buildMemoryUse(*Move);
         }
         else if (auto Borrow = dyn_cast<BeginBorrowInst>(&I)) {
            buildMemoryUse(*Borrow);
         }
      }
   }
}

void BorrowCheckPass::buildMemoryUse(il::MoveInst& I)
{
   // get the moved memory location
   auto Loc = MemoryLocation::get(I.getOperand(0));
   if (!Loc) {
      // can't reason about this memory use
      return;
   }

   // compute further restrictions
   llvm::SmallVector<MemoryRestriction, 8> Restrictions;
   buildRestrictions(I.getOperand(0), MemoryUse::Move, Restrictions);

   auto* Mem = Allocator.Allocate<MemoryRestriction>(Restrictions.size());
   std::copy(Restrictions.begin(), Restrictions.end(), Mem);

   // remember this memory use
   MemoryUse Use(MemoryUse::Move, I.getSourceLoc(), Loc,
                 llvm::ArrayRef<MemoryRestriction>(Mem, Restrictions.size()));

   InstUseMap.try_emplace(&I, MemoryUses.size());
   LocUseMap[Loc].push_back(MemoryUses.size());

   MemoryUses.push_back(Use);
}

void BorrowCheckPass::buildMemoryUse(il::BeginBorrowInst& I)
{
   // get the borrowed memory location
   auto Loc = MemoryLocation::get(I.getOperand(0));
   if (!Loc) {
      // can't reason about this memory use
      return;
   }

   MemoryUse::Kind UseKind = I.isMutableBorrow() ? MemoryUse::MutableBorrow
                                                 : MemoryUse::ImmutableBorrow;

   // compute further restrictions
   llvm::SmallVector<MemoryRestriction, 8> Restrictions;
   buildRestrictions(I.getOperand(0), UseKind, Restrictions);

   auto* Mem = Allocator.Allocate<MemoryRestriction>(Restrictions.size());
   std::copy(Restrictions.begin(), Restrictions.end(), Mem);

   // remember this memory use
   MemoryUse Use(UseKind,
                 SourceRange(I.getBeginBorrowLoc(), I.getEndBorrowLoc()), Loc,
                 llvm::ArrayRef<MemoryRestriction>(Mem, Restrictions.size()));

   InstUseMap.try_emplace(&I, MemoryUses.size());

   for (auto EndUse : I.getUses()) {
      auto End = dyn_cast<EndBorrowInst>(EndUse->getUser());
      if (End)
         InstUseMap.try_emplace(End, MemoryUses.size());
   }

   LocUseMap[Loc].push_back(MemoryUses.size());
   MemoryUses.push_back(Use);
}

MemoryRestriction BorrowCheckPass::getRestriction(il::Value* V,
                                                  MemoryUse::Kind UseKind)
{
   auto Loc = MemoryLocation::get(V);
   if (auto Decl = ILGen.getDeclForValue(V)) {
      MemDeclMap.try_emplace(Loc, Decl);
   }
   else if (auto FieldRef = dyn_cast<FieldRefInst>(V)) {
      auto F = FieldRef->getAccessedType()->getField(FieldRef->getFieldName());
      if (F)
         MemDeclMap.try_emplace(Loc, F);
   }

   switch (UseKind) {
   case MemoryUse::Move: {
      // a move prevents any further use of the values along the access path
      return MemoryRestriction(MemoryRestriction::Use, Loc);
   }
   case MemoryUse::ImmutableBorrow: {
      // an immutable borrow prevents mutable borrows and moves
      return MemoryRestriction(
          MemoryRestriction::MutableBorrow | MemoryRestriction::Move, Loc);
   }
   case MemoryUse::MutableBorrow: {
      // a mutable borrow prevents all uses
      return MemoryRestriction(MemoryRestriction::Use, Loc);
   }
   }
}

void BorrowCheckPass::buildRestrictions(
    il::Value* V, MemoryUse::Kind UseKind,
    llvm::SmallVectorImpl<MemoryRestriction>& Vec)
{
   V = LookThroughLoad(V);

   while (true) {
      if (auto Load = dyn_cast<LoadInst>(V)) {
         V = Load->getTarget();
      }
      else if (auto GEP = dyn_cast<GEPInst>(V)) {
         Vec.push_back(getRestriction(GEP, UseKind));
         V = LookThroughLoad(GEP->getOperand(0));
      }
      else if (auto FieldRef = dyn_cast<FieldRefInst>(V)) {
         Vec.push_back(getRestriction(FieldRef, UseKind));
         V = LookThroughLoad(FieldRef->getOperand(0));
      }
      else if (auto TupleExtract = dyn_cast<TupleExtractInst>(V)) {
         Vec.push_back(getRestriction(TupleExtract, UseKind));
         V = LookThroughLoad(TupleExtract->getOperand(0));
      }
      else if (auto Alloca = dyn_cast<AllocaInst>(V)) {
         Vec.push_back(getRestriction(Alloca, UseKind));
         break;
      }
      else if (auto Arg = dyn_cast<Argument>(V)) {
         Vec.push_back(getRestriction(Arg, UseKind));
         break;
      }
      else {
         break;
      }
   }
}

void BorrowCheckPass::visitIntrinsicCallInst(const IntrinsicCallInst& I,
                                             BitVector& Gen, BitVector& Kill)
{
   switch (I.getCalledIntrinsic()) {
   case Intrinsic::lifetime_begin:
   case Intrinsic::lifetime_end: {
      auto Loc = MemoryLocation::get(I.getArgs().front());
      if (!Loc)
         return;

      auto IdxIt = LocUseMap.find(Loc);
      if (IdxIt == LocUseMap.end())
         return;

      // Lifetime markers void all memory uses.
      for (auto UseIdx : IdxIt->getSecond()) {
         Gen.reset(UseIdx);
         Kill.set(UseIdx);
      }

      break;
   }
   default:
      break;
   }
}

void BorrowCheckPass::visitMoveInst(const il::MoveInst& I, BitVector& Gen,
                                    BitVector& Kill)
{
   auto IdxIt = InstUseMap.find(&I);
   if (IdxIt == InstUseMap.end())
      return;

   Gen.set(IdxIt->getSecond());
   Kill.reset(IdxIt->getSecond());
}

void BorrowCheckPass::visitBeginBorrowInst(const il::BeginBorrowInst& I,
                                           BitVector& Gen, BitVector& Kill)
{
   auto IdxIt = InstUseMap.find(&I);
   if (IdxIt == InstUseMap.end())
      return;

   Gen.set(IdxIt->getSecond());
   Kill.reset(IdxIt->getSecond());
}

void BorrowCheckPass::visitEndBorrowInst(const il::EndBorrowInst& I,
                                         BitVector& Gen, BitVector& Kill)
{

   auto IdxIt = InstUseMap.find(&I);
   if (IdxIt == InstUseMap.end())
      return;

   Gen.reset(IdxIt->getSecond());
   Kill.set(IdxIt->getSecond());
}

void BorrowCheckPass::checkConflictingUse(const Instruction& I,
                                          MemoryLocation Loc,
                                          const MemoryUse& Use,
                                          bool IsDirectlyAccessedMem)
{
   for (auto& R : Use.getRestrictions()) {
      checkConflictingRestriction(I, Loc, Use, R);

      if (!IsDirectlyAccessedMem) {
         // only check the first restriction for things up the access path
         return;
      }
   }
}

void BorrowCheckPass::checkConflictingRestriction(const Instruction& I,
                                                  MemoryLocation Loc,
                                                  const MemoryUse& Use,
                                                  const MemoryRestriction& R)
{
   if (R.getRestrictedMem() != Loc || R.getKind() == MemoryRestriction::None)
      return;

   if (DiagnosedLocs.find(I.getSourceLoc()) != DiagnosedLocs.end())
      return;

   std::string Name = getAccessPathDescriptor(ILGen, *I.getOperand(0));

   if (isa<MoveInst>(I)) {
      // check if a move is allowed by this restriction
      if ((R.getKind() & MemoryRestriction::Move) == 0) {
         return;
      }

      F->setInvalid(true);

      // previous use was also a move
      if (Use.isMove()) {
         bool Partial = Use.getAccessedMem() != Loc;
         ILGen.getSema().diagnose(diag::err_already_moved, I.getSourceLoc(),
                                  Name, Partial);

         ILGen.getSema().diagnose(diag::note_moved_here, Use.getSourceLoc());
      }
      else {
         bool Partial = Use.getAccessedMem() != Loc;
         ILGen.getSema().diagnose(diag::err_move_after_borrow, I.getSourceLoc(),
                                  Name, Partial);

         ILGen.getSema().diagnose(diag::note_borrow_begins_here,
                                  Use.getSourceLoc().getStart());

         if (auto EndLoc = Use.getSourceLoc().getEnd())
            ILGen.getSema().diagnose(diag::note_borrow_ends_here, EndLoc);
      }

      DiagnosedLocs.insert(I.getSourceLoc());
      return;
   }

   if (auto Borrow = dyn_cast<BeginBorrowInst>(&I)) {
      if (Borrow->isMutableBorrow()) {
         if ((R.getKind() & MemoryRestriction::MutableBorrow) == 0) {
            return;
         }
      }
      else {
         if ((R.getKind() & MemoryRestriction::ImmutableBorrow) == 0) {
            return;
         }
      }

      F->setInvalid(true);

      // previous use was a move
      if (Use.isMove()) {
         bool Partial = Use.getAccessedMem() != Loc;
         ILGen.getSema().diagnose(diag::err_borrow_after_move, I.getSourceLoc(),
                                  Borrow->isMutableBorrow(), Name, Partial);

         ILGen.getSema().diagnose(diag::note_moved_here, Use.getSourceLoc());
      }
      else {
         ILGen.getSema().diagnose(
             diag::err_cannot_borrow, Borrow->getBeginBorrowLoc(),
             Borrow->isMutableBorrow(), Name, Use.isMutableBorrow());

         ILGen.getSema().diagnose(diag::note_borrow_begins_here,
                                  Use.getSourceLoc().getStart());

         if (auto EndLoc = Use.getSourceLoc().getEnd())
            ILGen.getSema().diagnose(diag::note_borrow_ends_here, EndLoc);
      }

      DiagnosedLocs.insert(I.getSourceLoc());
      return;
   }

   F->setInvalid(true);

   // previous use was a move
   if (Use.isMove()) {
      bool Partial = Use.getAccessedMem() != Loc;
      ILGen.getSema().diagnose(diag::err_use_after_move, I.getSourceLoc(), Name,
                               Partial);

      ILGen.getSema().diagnose(diag::note_moved_here, Use.getSourceLoc());
   }
   else {
      ILGen.getSema().diagnose(diag::err_cannot_borrow, I.getSourceLoc(), true,
                               Name, Use.isMutableBorrow());

      ILGen.getSema().diagnose(diag::note_borrow_begins_here,
                               Use.getSourceLoc());

      if (auto EndLoc = Use.getSourceLoc().getEnd())
         ILGen.getSema().diagnose(diag::note_borrow_ends_here, EndLoc);
   }

   DiagnosedLocs.insert(I.getSourceLoc());
}

void BorrowCheckPass::checkOverlappingMemoryUse(const il::Instruction& I,
                                                const BitVector& Gen)
{
   if ((!isa<MoveInst>(I) && !isa<BeginBorrowInst>(I)) || UA->isUnsafe(I))
      return;

   bool IsDirectlyAccessedMem = true;
   for (auto AccessedMem : AccessPath(I.getOperand(0))) {
      auto MemLoc = MemoryLocation::get(AccessedMem);
      if (!MemLoc)
         continue;

      // check all active memory uses for overlapping access
      for (auto Idx : Gen.set_bits()) {
         auto& Use = MemoryUses[Idx];
         checkConflictingUse(I, MemLoc, Use, IsDirectlyAccessedMem);
      }

      IsDirectlyAccessedMem = false;
   }
}

void BorrowCheckPass::run()
{
   UA = PM->getAnalysis<UnsafeAnalysis>()->get(F);

   computeMemoryAccesses(*F);
   unsigned NumMemUses = (unsigned)MemoryUses.size();

   // fast-path if there is nothing to check
   if (!NumMemUses)
      return;

   // set up data flow problem
   solveForwardMayProblem(*F, NumMemUses, GenMap, KillMap, InMap, OutMap,
                          [&](Instruction& I, BitVector& Gen, BitVector& Kill) {
                             visit(I, Gen, Kill);
                          },
                          false);

   // check for overlapping borrows
   for (auto& B : *F) {
      if (B.hasNoPredecessors())
         continue;

      BitVector Gen(NumMemUses);
      Gen |= InMap[&B];

      BitVector Kill(NumMemUses);

      for (auto& I : B) {
         checkOverlappingMemoryUse(I, Gen);
         visit(I, Gen, Kill);

         detail::andNot(Gen, Kill, NumMemUses);
      }
   }

   MemoryUses.clear();
   GenMap.clear();
   KillMap.clear();
   InMap.clear();
   OutMap.clear();
   InstUseMap.clear();
   LocUseMap.clear();
   MemoryUses.clear();
   DiagnosedLocs.clear();
   Allocator.Reset();
}

} // namespace il
} // namespace cdot