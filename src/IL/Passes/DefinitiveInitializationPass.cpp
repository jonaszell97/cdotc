#include "cdotc/IL/Passes/DefinitiveInitializationPass.h"

#include "cdotc/AST/Decl.h"
#include "cdotc/IL/Analysis/AccessPathDescriptor.h"
#include "cdotc/IL/Analysis/AccessPathIterator.h"
#include "cdotc/IL/Analysis/Dominance.h"
#include "cdotc/IL/Analysis/UnsafeAnalysis.h"
#include "cdotc/IL/Passes/BorrowCheckPass.h"
#include "cdotc/IL/Passes/DataflowProblem.h"
#include "cdotc/IL/Passes/FinalizeFunctionPass.h"
#include "cdotc/IL/Transforms/StackPromotion.h"
#include "cdotc/IL/Utils/BlockIterator.h"
#include "cdotc/ILGen/ILGenPass.h"
#include "cdotc/Diagnostics/Diagnostics.h"
#include "cdotc/Sema/SemaPass.h"

#ifndef NDEBUG
#include "cdotc/IL/Instructions.h"
#endif

using namespace cdot::support;
using namespace cdot::diag;

using cdot::il::detail::LocalVariable;
using llvm::BitVector;
using std::string;

namespace cdot {
namespace il {
namespace detail {

struct LocalVariable final
    : llvm::TrailingObjects<LocalVariable, LocalVariable*> {
private:
   LocalVariable(Value* Val, MemoryLocation Loc, unsigned BitVectorIdx,
                 llvm::ArrayRef<LocalVariable*> ContainedVars)
       : NumContainedVars((unsigned)ContainedVars.size()), Val(Val), Loc(Loc),
         BitVectorIdx(BitVectorIdx)
   {
      std::copy(ContainedVars.begin(), ContainedVars.end(),
                getTrailingObjects<LocalVariable*>());
   }

   unsigned NumContainedVars;

public:
   Value* Val;
   MemoryLocation Loc;
   unsigned BitVectorIdx;

   static LocalVariable* Create(ast::ASTContext& C, Value* Val,
                                MemoryLocation Loc, unsigned BitVectorIdx,
                                llvm::ArrayRef<LocalVariable*> ContainedVars)
   {
      void* Mem = C.TmpAllocator.Allocate(
          totalSizeToAlloc<LocalVariable*>(ContainedVars.size()),
          alignof(LocalVariable));

      return new (Mem) LocalVariable(Val, Loc, BitVectorIdx, ContainedVars);
   }

   enum InitializationState {
      FullyInitialized,
      MaybeUninitialized,
      NotInitialized,
      PartiallyInitialized,
   };

   InitializationState getInitializationState(BitVector& Must,
                                              BitVector& May) const
   {
      if (!NumContainedVars)
         return Must.test(BitVectorIdx)
                    ? FullyInitialized
                    : May.test(BitVectorIdx) ? MaybeUninitialized
                                             : NotInitialized;

      bool FoundInitialized = false;
      bool FoundUninitialized = false;

      for (auto Var : getContainedVars()) {
         switch (Var->getInitializationState(Must, May)) {
         case PartiallyInitialized:
            return PartiallyInitialized;
         case MaybeUninitialized:
            return MaybeUninitialized;
         case NotInitialized:
            FoundUninitialized = true;
            break;
         case FullyInitialized:
            FoundInitialized = true;
            break;
         }

         if (FoundInitialized && FoundUninitialized)
            return PartiallyInitialized;
      }

      return FoundInitialized ? FullyInitialized : NotInitialized;
   }

   void initializeAll(BitVector& BV)
   {
      BV.set(BitVectorIdx);
      for (auto& Var : getContainedVars()) {
         Var->initializeAll(BV);
      }
   }

   void uninitializeAll(BitVector& BV)
   {
      BV.reset(BitVectorIdx);
      for (auto& Var : getContainedVars()) {
         Var->uninitializeAll(BV);
      }
   }

   llvm::ArrayRef<LocalVariable*> getContainedVars() const
   {
      return {getTrailingObjects<LocalVariable*>(), NumContainedVars};
   }
};

} // namespace detail

DefinitiveInitializationPass::DefinitiveInitializationPass(
    ast::ILGenPass& ILGen)
    : FunctionPass(PassKind::DefinitiveInitializationPassID), ILGen(ILGen),
      SelfLoc(MemoryLocation::get())
{
}

void DefinitiveInitializationPass::solveDataFlowProblem(il::Function& F)
{
   unsigned NumFacts = MemoryLocs.size();

   // Calculate Gen and Kill sets for each BB
   // Kill always empty since a variable can't be "deinitialized"
   for (auto& B : F) {
      if (B.hasNoPredecessors())
         continue;

      auto& Gen = GenMap[&B];
      Gen.resize(NumFacts);

      auto& Kill = KillMap[&B];
      Kill.resize(NumFacts);

      for (auto& I : B) {
         visit(I, Gen, Kill);
      }
   }

   // Calculate In / Out until no more changes happen
   llvm::SmallPtrSet<BasicBlock*, 16> WorkList;

   for (auto& B : F) {
      auto& MustIn = MustInMap[&B];
      auto& MustOut = MustOutMap[&B];

      auto& MayIn = MayInMap[&B];
      auto& MayOut = MayOutMap[&B];

      MustIn.resize(NumFacts);
      MustOut.resize(NumFacts);

      MayIn.resize(NumFacts);
      MayOut.resize(NumFacts);

      if (&B != F.getEntryBlock()) {
         MustOut.flip();
         MayOut.flip();

         WorkList.insert(&B);
      }
      else {
         MustOut |= GenMap[&B];
         MustOut |= MustInMap[&B];

         MayOut |= GenMap[&B];
         MayOut |= MayInMap[&B];
      }
   }

   while (!WorkList.empty()) {
      auto BB = *WorkList.begin();
      WorkList.erase(BB);

      if (BB == F.getEntryBlock() || BB->hasNoPredecessors())
         continue;

      auto& MustIn = MustInMap[BB];
      auto& MustOut = MustOutMap[BB];

      auto& MayIn = MayInMap[BB];
      auto& MayOut = MayOutMap[BB];

      bool first = true;

      // recalculate In as intersection of all predecessors (a value is
      // initialized in B if it is intialized in all predecessors of B)
      for (auto pred : getPredecessors(BB)) {
         if (first) {
            first = false;
            MayIn = MayOutMap[pred];
            MustIn = MustOutMap[pred];
         }
         else {
            MayIn |= MayOutMap[pred];
            MustIn &= MustOutMap[pred];
         }
      }

      // Out(B) = (Gen(B) | In(B)) & ~Kill(B)
      auto& Kill = KillMap[BB];

      auto NewMayOut = GenMap[BB];
      NewMayOut |= MayIn;
      detail::andNot(NewMayOut, Kill, NumFacts);

      auto NewMustOut = GenMap[BB];
      NewMustOut |= MustIn;
      detail::andNot(NewMustOut, Kill, NumFacts);

      if (NewMayOut != MayOut || NewMustOut != MustOut) {
         for (auto Succ : getSuccessors(BB))
            WorkList.insert(Succ);

         MayOut = NewMayOut;
         MustOut = NewMustOut;
      }
   }
}

LocalVariable* DefinitiveInitializationPass::getLocal(il::Value* Val)
{
   if (!isa<Argument>(Val) && !isa<AllocaInst>(Val) && !isa<MoveInst>(Val))
      return nullptr;

   auto Decl = ILGen.getDeclForValue(Val);
   if (!Decl)
      return nullptr;

   if (auto Arg = dyn_cast<ast::FuncArgDecl>(Decl)) {
      if (Arg->getConvention() == ArgumentConvention::Borrowed)
         return nullptr;
   }

   MemoryLocation Loc = MemoryLocation::get(Val, false);
   if (!Loc)
      return nullptr;

   auto Var = getLocal(Val, Loc, Val->getType()->removeReference());
   if (!Var)
      return nullptr;

   if (isa<ast::FuncArgDecl>(Decl)) {
      // assume arguments start as initialized
      InitializedArgs.push_back(Var);
   }

   return Var;
}

LocalVariable* DefinitiveInitializationPass::getLocal(Value* Val,
                                                      MemoryLocation Loc,
                                                      QualType Ty,
                                                      bool NoClasses)
{
   llvm::SmallVector<LocalVariable*, 4> Vec;

   unsigned Offset = 0;
   if (auto* Tup = Ty->asTupleType()) {
      for (auto ContTy : Tup->getContainedTypes()) {
         auto NewLoc = MemoryLocation::getTupleField(Loc, Offset++);
         auto NewVal = getLocal(nullptr, NewLoc, ContTy);
         if (NewVal)
            Vec.push_back(NewVal);
      }
   }
   else if (auto* R = Ty->asRecordType()) {
      if (R->getRecord()->getDeclKind() == ast::Decl::StructDeclID
          || (!NoClasses && isa<ast::ClassDecl>(R->getRecord()))) {
         auto Fields = cast<ast::StructDecl>(R->getRecord())->getFields();
         for (auto Field : Fields) {
            auto NewLoc
                = MemoryLocation::getStructField(Loc, Field->getDeclName());

            auto NewVal = getLocal(nullptr, NewLoc, Field->getType());
            if (NewVal)
               Vec.push_back(NewVal);
         }
      }
   }

   auto Var = LocalVariable::Create(ILGen.getSema().getContext(), Val, Loc,
                                    MemoryLocCounter++, Vec);

   auto Result = MemoryLocs.try_emplace(Var->Loc, Var);
   if (!Result.second) {
      return Result.first->getSecond();
   }

   return Var;
}

void DefinitiveInitializationPass::visitAssignInst(il::AssignInst& I,
                                                   BitVector& Gen,
                                                   BitVector& Kill)
{
   if (auto *FR = dyn_cast<FieldRefInst>(I.getDst())) {
      auto LocRef = MemoryLocation::get(FR->getOperand(0));
      if (LocRef == SelfLoc) {
         I.setTagged(true);
      }
   }

   auto Loc = MemoryLocation::get(I.getDst(), false);
   if (!Loc)
      return;

   auto* Var = lookupLocal(Loc);
   if (!Var)
      return;

   if (!Gen.test(Var->BitVectorIdx)) {
      if (SecondPass) {
         // this assign is an initilization
         InitAssigns.insert(&I);
      }

      // whitelist all instructions along the access path
      for (auto Val : AccessPath(I.getDst(), true)) {
         auto Inst = dyn_cast<Instruction>(Val);
         if (!Inst)
            continue;

         Whitelist.insert(Inst);
      }
   }

   Var->initializeAll(Gen);
   Var->uninitializeAll(Kill);
}

void DefinitiveInitializationPass::visitInitInst(il::InitInst& I,
                                                 BitVector& Gen,
                                                 BitVector& Kill)
{
   if (I.isSynthesized())
      return;

   if (auto *FR = dyn_cast<FieldRefInst>(I.getDst())) {
      auto LocRef = MemoryLocation::get(FR->getOperand(0));
      if (LocRef == SelfLoc) {
         I.setTagged(true);
      }
   }

   auto Loc = MemoryLocation::get(I.getDst(), false);
   if (!Loc)
      return;

   auto* Var = lookupLocal(Loc);
   if (!Var)
      return;

   if (!Gen.test(Var->BitVectorIdx)) {
      // whitelist all instructions along the access path
      for (auto Val : AccessPath(I.getDst(), true)) {
         auto Inst = dyn_cast<Instruction>(Val);
         if (!Inst)
            continue;

         Whitelist.insert(Inst);
      }
   }

   Var->initializeAll(Gen);
   Var->uninitializeAll(Kill);
}

static void verifyMove(Function& F, il::MoveInst& I, ast::ILGenPass& ILGen)
{
   if (isa<GlobalVariable>(I.getOperand(0))) {
      F.setInvalid(true);
      ILGen.getSema().diagnose(diag::err_cannot_move, 0, I.getSourceLoc());
      return;
   }

   auto* FieldRef = dyn_cast<FieldRefInst>(I.getOperand(0));
   if (!FieldRef)
      return;

   int diagIdx = -1;
   if (isa<GlobalVariable>(FieldRef->getOperand(0))) {
      diagIdx = 0;
   }
   else if (FieldRef->getOperand(0)->getType()->removeReference()->isClass()) {
      diagIdx = 1;
   }

   if (diagIdx == -1)
      return;

   F.setInvalid(true);
   ILGen.getSema().diagnose(diag::err_cannot_move_field, diagIdx,
                            I.getSourceLoc());
}

void DefinitiveInitializationPass::visitMoveInst(il::MoveInst& I,
                                                 BitVector& Gen,
                                                 BitVector& Kill)
{
   if (SecondPass && !UA->isUnsafe(I))
      verifyMove(*this->F, I, ILGen);

   // deinitialize the moved-from memory
   auto Loc = MemoryLocation::get(I.getOperand(0), false);
   if (!Loc)
      return;

   if (SelfFields.find(Loc) != SelfFields.end())
      I.setTagged(true);

   auto* Var = lookupLocal(Loc);
   if (Var) {
      Var->uninitializeAll(Gen);
      Var->initializeAll(Kill);
   }

   // initialize the moved-to memory
   Loc = MemoryLocation::get(&I, false);
   if (!Loc)
      return;

   Var = lookupLocal(Loc);
   if (!Var)
      return;

   Var->initializeAll(Gen);
   Var->uninitializeAll(Kill);
}

void DefinitiveInitializationPass::visitCallInst(il::CallInst& I,
                                                 BitVector& Gen,
                                                 BitVector& Kill)
{
   // a 'self.init' call effectively initializes all fields
   if (auto Init = dyn_cast_or_null<Initializer>(I.getCalledFunction())) {
      if (Init->getCtorKind() == ConstructorKind::Base) {
         Whitelist.insert(&I);

         // whitelist all instructions along the access path
         for (auto Val : AccessPath(I.getArgs().front(), true)) {
            auto Inst = dyn_cast<Instruction>(Val);
            if (!Inst)
               continue;

            Whitelist.insert(Inst);
         }

         auto* SelfTy = Init->getRecordType();
         auto* Fn
             = cast_or_null<ast::CallableDecl>(ILGen.getDeclForValue(Init));

         if (Fn && Fn->isFallibleInit())
            SelfTy = SelfTy->getTemplateArgs().front().getType()->getRecord();

         // this might be a super.init, so only initialize the fields of the
         // class this method belongs to
         for (auto* F : cast<ast::StructDecl>(SelfTy)->getStoredFields()) {
            auto Loc = MemoryLocation::getStructField(SelfVal->Loc,
                                                      F->getDeclName());

            auto* Var = lookupLocal(Loc);
            assert(Var && "untracked stored field!");

            Var->initializeAll(Gen);
            Var->uninitializeAll(Kill);
         }
      }
   }
}

void DefinitiveInitializationPass::visitLoadInst(il::LoadInst& I,
                                                 BitVector& Gen,
                                                 BitVector& Kill)
{
   auto Loc = MemoryLocation::get(I.getTarget());
   if (SelfFields.find(Loc) != SelfFields.end()) {
      I.setTagged(true);
   }
}

void DefinitiveInitializationPass::prepareInitializer(ast::CallableDecl* FnDecl,
                                                      il::Initializer& I)
{
   auto S = dyn_cast<ast::StructDecl>(FnDecl->getRecord());
   if (!S) {
      return;
   }

   auto Val = I.getSelf();

   MemoryLocation Loc = MemoryLocation::get(Val, false);
   if (!Loc)
      return;

   SelfVal = getLocal(Val, Loc, Val->getType()->removeReference(), false);
   SelfLoc = Loc;

   if (cast<ast::InitDecl>(FnDecl)->isBaseInitializer()) {
      return;
   }

   bool IsFallibleInit = FnDecl->isFallibleInit();

   // mark fields with default values as initialized
   for (auto Field : S->getFields()) {
      if (!Field->getDefaultVal())
         continue;

      auto FieldLoc = MemoryLocation::getStructField(Loc, Field->getDeclName());
      auto Var = lookupLocal(FieldLoc);
      if (Var) {
         InitializedArgs.push_back(Var);

         if (IsFallibleInit)
            SelfFields.insert(FieldLoc);
      }
   }
}

void DefinitiveInitializationPass::run()
{
   UA = PM->getAnalysis<UnsafeAnalysis>()->get(F);

   auto FnDecl = cast_or_null<ast::CallableDecl>(ILGen.getDeclForValue(F));
   if (!FnDecl)
      return;

   bool IsInit = false;

   // in an initializer, all properties of 'self' need to be initialized
   bool IsBaseInit = false;
   if (auto I = dyn_cast<Initializer>(F)) {
      IsInit = true;
      IsBaseInit = cast<ast::InitDecl>(FnDecl)->isBaseInitializer();
      prepareInitializer(FnDecl, *I);
   }

   // add memory locations for owned or mutably borrowed arguments
   for (auto& Arg : F->getEntryBlock()->getArgs()) {
      if (Arg.getConvention() == ArgumentConvention::Borrowed)
         continue;

      if ((IsInit && Arg.isSelf()) || !Arg.isLvalue())
         continue;

      getLocal(&Arg);
   }

   // collect local variables
   for (auto& B : *F) {
      for (auto& I : B) {
         if (I.isSynthesized())
            continue;

         getLocal(&I);
      }
   }

   if (!MemoryLocCounter) {
      // nothing to check
      return;
   }

   auto& MustEntryVec = MustInMap[F->getEntryBlock()];
   MustEntryVec.resize(MemoryLocCounter);

   auto& KillEntryVec = KillMap[F->getEntryBlock()];
   KillEntryVec.resize(MemoryLocCounter);

   for (auto& Arg : InitializedArgs) {
      Arg->initializeAll(MustEntryVec);
      Arg->uninitializeAll(KillEntryVec);
   }

   MayInMap[F->getEntryBlock()] |= MustEntryVec;

   solveDataFlowProblem(*F);
   SecondPass = true;

   SmallPtrSet<ast::FieldDecl*, 4> InvalidFields;
   for (auto& B : *F) {
      BitVector MustGen(MemoryLocCounter);
      MustGen |= MustInMap[&B];

      BitVector MayGen(MemoryLocCounter);
      MayGen |= MayInMap[&B];

      BitVector Kill(MemoryLocCounter);

      for (auto& I : B) {
         if (Whitelist.find(&I) == Whitelist.end()) {
            verifyMemoryUse(I, MustGen, MayGen);
         }

         if (auto *Call = dyn_cast<CallInst>(&I)) {
            checkDeinitilization(*Call, MustGen, MayGen);

            // 'self' may not be partially initialized before a 'self.init' call.
            if (!IsBaseInit) {
               auto *CalledFn = Call->getCalledFunction();
               if (auto Init = dyn_cast_or_null<Initializer>(CalledFn)) {
                  if (Init->getCtorKind() == ConstructorKind::Base) {
                     auto State = SelfVal->getInitializationState(MustGen, MayGen);
                     if (State != LocalVariable::NotInitialized) {
                        ILGen.getSema().diagnose(
                            err_generic_error,
                            "'self' must be fully uninitialized before a call to a "
                            "delegating initializer",
                            Call->getSourceLoc());
                     }
                  }
               }
            }
         }

         visit(I, MustGen, Kill);
         MayGen |= MustGen;

         detail::andNot(MayGen, Kill, MemoryLocCounter);
         detail::andNot(MustGen, Kill, MemoryLocCounter);
      }

      // If this is an exit block, verify that no locals or arguments are
      // left partially initialized.
      if (!B.isExitBlock())
         continue;

      for (auto& Var : MemoryLocs) {
         if (!Var.getSecond()->Val)
            continue;

         if (Var.getSecond()->Val->isSelf()
             || !Var.getSecond()->Val->getType()->isRecordType())
            continue;

         auto State = Var.getSecond()->getInitializationState(MustGen, MayGen);
         if (State != LocalVariable::PartiallyInitialized)
            continue;

         auto Mem = AccessedMem(Var.getSecond()->Val);
         auto Desc = getAccessPathDescriptor(ILGen, *Mem);

         F->setInvalid(true);
         ILGen.getSema().diagnose(err_left_uninitialized, Desc,
                                  F->getSourceLoc());
      }

      if (SelfVal && !IsBaseInit) {
         auto Term = dyn_cast_or_null<RetInst>(B.getTerminator());
         if (!Term)
            continue;

         // an early return in a fallible init may leave things uninitialized
         if (Term->IsFallibleInitNoneRet())
            continue;

         checkSelfInitializationState(MustGen, MayGen, InvalidFields);
      }
   }

   for (auto Assign : InitAssigns) {
      auto Init = new InitInst(Assign->getDst(), Assign->getSrc(), nullptr);
      Init->setTagged(Assign->isTagged());

      Assign->replaceAllUsesWith(Init);
      Assign->detachAndErase();

      ILGen.Builder.SetInsertPoint(Init->getIterator());
      ILGen.Builder.CreateLifetimeBegin(Assign->getDst());
   }

   for (auto I : InstsToRemove) {
      if (I->use_begin() == I->use_end()) {
         I->detachAndErase();
      }
   }

   MemoryLocCounter = 0;
   SelfVal = nullptr;
   SelfLoc = MemoryLocation::get();

   MemoryLocs.clear();
   InitializedArgs.clear();
   Whitelist.clear();
   GenMap.clear();
   KillMap.clear();
   MustInMap.clear();
   MustOutMap.clear();
   MayInMap.clear();
   MayOutMap.clear();
   InitAssigns.clear();
   InstsToRemove.clear();
   SelfFields.clear();
   SecondPass = false;
}

void DefinitiveInitializationPass::verifyMemoryUse(il::Instruction& I,
                                                   BitVector& MustGen,
                                                   BitVector& MayGen)
{
   if (I.isSynthesized()) {
      return;
   }

   if (!isa<LoadInst>(I) && !isa<MoveInst>(I) && !isa<BeginBorrowInst>(I)) {
      return;
   }

   if (UA->isUnsafe(I)) {
      return;
   }

   auto Mem = MemoryLocation::get(I.getOperand(0), false);
   if (!Mem)
      return;

   auto It = MemoryLocs.find(Mem);
   if (It == MemoryLocs.end())
      return;

   int selector = -1;
   auto Status = It->getSecond()->getInitializationState(MustGen, MayGen);

   bool IsValidUse = true;
   switch (Status) {
   case LocalVariable::FullyInitialized:
      break;
   case LocalVariable::NotInitialized:
      if (I.isUnused() && !isa<AssignInst>(I)) {
         break;
      }

      selector = 2;
      IsValidUse = false;
      break;
   case LocalVariable::MaybeUninitialized:
      if (I.isUnused() && !isa<AssignInst>(I)) {
         break;
      }

      selector = 0;
      IsValidUse = false;
      break;
   case LocalVariable::PartiallyInitialized: {
      if (I.isUnused() && !isa<AssignInst>(I)) {
         break;
      }

      // a partially initialized value may only be indexed further
      auto Use = I.getSingleUser();
      if (!Use) {
         IsValidUse = false;
         break;
      }

      selector = 3;
      IsValidUse = Use->isIndexingInstruction();
      break;
   }
   }

   // Verify that the memory has been initialized before this use.
   if (!IsValidUse) {
      auto& Sema = ILGen.getSema();
      auto Decl = ILGen.getDeclForValue(I.getOperand(0)->ignoreBitCast());
      if (!Decl)
         return;

      F->setInvalid(true);

      Sema.diagnose(Decl, err_uninitialized_local, Decl->getDeclName(),
                    selector, I.getSourceLoc());

      if (!isa<ast::FuncArgDecl>(Decl) || !cast<ast::FuncArgDecl>(Decl)->isSelf()) {
         Sema.diagnose(note_uninitialized_declared_here, Decl->getSourceLoc());
      }
   }
}

static il::Value* LookThroughLoad(il::Value* V)
{
   if (auto Ld = dyn_cast<LoadInst>(V))
      return Ld->getTarget();

   return V;
}

void DefinitiveInitializationPass::checkSelfInitializationState(
    BitVector& MustGen,
    BitVector& MayGen,
    llvm::SmallPtrSetImpl<ast::FieldDecl*> &InvalidFields)
{
   auto State = SelfVal->getInitializationState(MustGen, MayGen);
   if (State == LocalVariable::FullyInitialized)
      return;

   auto S = cast<ast::StructDecl>(
       SelfVal->Val->getType()->removeReference()->getRecord());

   if (S->getStoredFields().empty())
      return;

   auto FieldIt = S->stored_field_begin();
   for (auto& Val : SelfVal->getContainedVars()) {
      auto FieldState = Val->getInitializationState(MustGen, MayGen);
      if (FieldState != LocalVariable::FullyInitialized) {
         auto Field = *FieldIt;

         // don't error twice for the same field
         if (!InvalidFields.insert(Field).second)
            continue;

         F->setInvalid(true);
         ILGen.getSema().diagnose(
             Field, err_field_must_be_initialized,
             Field->getDeclName(), F->getSourceLoc());

         ILGen.getSema().diagnose(
             note_declared_here, Field->getSourceLoc());
      }

      ++FieldIt;
   }
}

void DefinitiveInitializationPass::checkDeinitilization(il::CallInst& I,
                                                        BitVector& MustGen,
                                                        BitVector& MayGen)
{
   if (!I.isDeinitCall())
      return;

   auto Val = LookThroughLoad(I.getArgs().front());
   if (Val->isTagged()) {
      I.setTagged(true);
      return;
   }

   auto Loc = MemoryLocation::get(I.getArgs().front(), false);
   if (!Loc)
      return;

   auto* Var = lookupLocal(Loc);
   if (!Var)
      return;

   auto State = Var->getInitializationState(MustGen, MayGen);
   switch (State) {
   case LocalVariable::NotInitialized: {
      InstsToRemove.push_back(&I);

      // Remove any loads or indexing instructions that are used for this
      // deinitializer call.
      Instruction* Curr = dyn_cast<Instruction>(I.getOperand(0));
      while (Curr && (isa<LoadInst>(Curr) || Curr->isIndexingInstruction())) {
         InstsToRemove.push_back(Curr);
         Curr = dyn_cast<Instruction>(Curr->getOperand(0));
      }

      break;
   }
   case LocalVariable::MaybeUninitialized:
      I.setTagged(true);
      Val->setTagged(true);
      break;
   default:
      break;
   }
}

} // namespace il
} // namespace cdot