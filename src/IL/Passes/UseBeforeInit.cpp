//
// Created by Jonas Zell on 05.03.18.
//

#include "UseBeforeInit.h"

#include "AST/Decl.h"
#include "ILGen/ILGenPass.h"
#include "IL/Utils/BlockIterator.h"
#include "Message/Diagnostics.h"
#include "Sema/SemaPass.h"

#include <llvm/ADT/BitVector.h>
#include <llvm/IR/ValueMap.h>

#include <string>

using namespace cdot::support;
using namespace cdot::diag;

using std::string;
using llvm::BitVector;

namespace cdot {
namespace il {

void UseBeforeInit::visitModule(Module &M)
{
   for (auto &F : M)
      visitFunction(F);
}

void DefinitiveInitilizationPass::visitModule(il::Module &M)
{
   for (auto &F : M) {
      if (auto I = dyn_cast<Initializer>(&F))
         visitInitializer(*I);
   }
}

static void checkStore(Instruction &I,
                       llvm::DenseMap<AllocaInst*, unsigned> &AllocaMap,
                       BitVector &Gen,
                       llvm::SmallPtrSetImpl<Instruction*> &Whitelist,
                       llvm::SmallPtrSetImpl<StoreInst*> *InitStores = nullptr){
   // whitelist any loads that exist to retain the old value (these will be
   // removed later)
   if (auto Ld = dyn_cast<LoadInst>(&I)) {
      auto Call = dyn_cast_or_null<IntrinsicCallInst>(Ld->getSingleUser());
      if (!Call)
         return;

      if (Call->getCalledIntrinsic() == Intrinsic::release) {
         Whitelist.insert(Ld);
      }

      return;
   }

   auto S = dyn_cast<StoreInst>(&I);
   if (!S)
      return;

   auto Alloca = dyn_cast<AllocaInst>(S->getDst());
   if (!Alloca)
      return;

   auto It = AllocaMap.find(Alloca);
   assert(It != AllocaMap.end());

   if (InitStores)
      InitStores->insert(S);

   auto Idx = It->second;
   Gen.set(Idx);
}

static bool checkSelfStore(Instruction &I,
                        llvm::DenseMap<DeclarationName, unsigned> &Fields,
                        BitVector &Gen,
                        llvm::SmallPtrSetImpl<Instruction*> &Whitelist,
                        llvm::SmallPtrSetImpl<StoreInst*> *InitStores = nullptr,
                        unsigned *IdxPtr = nullptr) {
   Instruction *Inst = &I;
   LoadInst *Load = nullptr;
   FieldRefInst *FieldRef = nullptr;

   // a 'self.init' call effectively initializes all fields
   if (auto Call = dyn_cast<CallInst>(&I)) {
      if (auto Init = dyn_cast_or_null<Initializer>(Call->getCalledFunction())){
         if (Init->getCtorKind() == ConstructorKind::Base) {
            Whitelist.insert(&I);

            Gen.set();
            if (IdxPtr)
               *IdxPtr = unsigned(-2);

            return false;
         }
      }
   }

   // in a mutating function, 'self' must be loaded first
   if ((Load = dyn_cast<LoadInst>(Inst))) {
      auto Self = dyn_cast<il::Argument>(Load->getOperand(0));
      if (!Self || !Self->isSelf())
         return false;

      Inst = dyn_cast_or_null<Instruction>(Load->getSingleUser());
      FieldRef = dyn_cast_or_null<FieldRefInst>(Inst);

      if (!FieldRef)
         return false;

      if (FieldRef->getOperand(0) != Load)
         return false;

      for (auto Use : FieldRef->getUses()) {
         auto User = Use->getUser();

         // a 'self.init' call is an allowed use of self
         if (auto Call = dyn_cast_or_null<CallInst>(User)) {
            if (auto Init
               = dyn_cast_or_null<Initializer>(Call->getCalledFunction())) {
               if (Init->getCtorKind() == ConstructorKind::Base) {
                  Whitelist.insert(Load);
                  return false;
               }
            }
         }

         // a retain or release can use it as well
         if (dyn_cast_or_null<Instruction>(User)
             && cast<Instruction>(User)->isRetainOrRelease()) {
            Whitelist.insert(Load);
            return false;
         }

         auto S = dyn_cast_or_null<StoreInst>(User);
         if (!S) {
            // if the accessed field was already initialized, whitelist the
            // instruction
            auto It = Fields.find(FieldRef->getFieldName());
            if (It == Fields.end())
               continue;

            auto Idx = It->second;
            if (Gen.test(Idx)) {
               Whitelist.insert(FieldRef);
               return false;
            }

            continue;
         }

         // found a matching store
         if (InitStores)
            InitStores->insert(S);

         break;
      }

      Whitelist.insert(Load);
   }
   else if ((FieldRef = dyn_cast<FieldRefInst>(Inst))) {
      // if the accessed field was already initialized, whitelist the
      // instruction
      auto It = Fields.find(FieldRef->getFieldName());
      if (It == Fields.end())
         return false;

      auto Idx = It->second;
      if (Gen.test(Idx))
         Whitelist.insert(FieldRef);

      return false;
   }
   else {
      auto S = dyn_cast<StoreInst>(Inst);
      if (!S)
         return false;

      Inst = dyn_cast_or_null<Instruction>(S->getDst());
      FieldRef = dyn_cast_or_null<FieldRefInst>(Inst);

      if (!FieldRef)
         return false;

      auto Self = dyn_cast<il::Argument>(FieldRef->getOperand(0));
      if (!Self || !Self->isSelf())
         return false;

      if (InitStores)
         InitStores->insert(S);

      Whitelist.insert(FieldRef);
   }

   auto It = Fields.find(FieldRef->getFieldName());
   if (It == Fields.end())
      return false;

   auto Idx = It->second;
   if (IdxPtr)
      *IdxPtr = Idx;

   bool WasSet = Gen.test(Idx);
   Gen.set(Idx);

   return WasSet;
}

template<BitVector&(BitVector::*meet)(const BitVector&), class Fn>
static void analyseFunction(Function &F, unsigned NumValues,
                            llvm::DenseMap<BasicBlock*, BitVector> &GenMap,
                            llvm::DenseMap<BasicBlock*, BitVector> &InMap,
                            llvm::DenseMap<BasicBlock*, BitVector> &OutMap,
                            const Fn &CalcFn) {
   // Calculate Gen and Kill sets for each BB
   // Kill always empty since a variable can't be "deinitialized"
   for (auto &B : F) {
      if (B.hasNoPredecessors())
         continue;

      auto &Gen = GenMap[&B];
      Gen.resize(NumValues);

      for (auto &I : B) {
         CalcFn(I, Gen);
      }
   }

   // Calculate In / Out until no more changes happen
   llvm::SmallPtrSet<BasicBlock*, 16> WorkList;

   // assume all are initialized at the beginning (except for entry block)
   for (auto &B : F) {
      auto &In = InMap[&B];
      auto &Out = OutMap[&B];

      In.resize(NumValues);
      Out.resize(NumValues);

      if (&B != F.getEntryBlock()) {
         Out.flip();
         WorkList.insert(&B);
      }
      else {
         Out = GenMap[&B];
      }
   }

   while (!WorkList.empty()) {
      auto BB = *WorkList.begin();
      WorkList.erase(BB);

      if (BB == F.getEntryBlock() || BB->hasNoPredecessors())
         continue;

      auto &In = InMap[BB];
      auto &Out = OutMap[BB];

      bool first = true;

      // recalculate In as intersection of all predecessors (a value is
      // initialized in B if it is intialized in all predecessors of B)
      for (auto &pred : getPredecessors(BB)) {
         if (first) {
            first = false;
            In = OutMap[&pred];
         }
         else {
            (In.*meet)(OutMap[&pred]);
         }
      }

      // Out(B) = Gen(B) | (In(B) & ~Kill(B))
      auto NewOut = GenMap[BB];
      NewOut |= In;

      if (NewOut != Out) {
         for (auto &Succ : getSuccessors(BB))
            WorkList.insert(&Succ);

         Out = NewOut;
      }
   }
}

void UseBeforeInit::visitFunction(Function &F)
{
   if (F.isDeclared())
      return;

   auto FnDecl = ILGen.getCallableDecl(&F);
   ast::SemaPass::DeclScopeRAII declScopeRAII(ILGen.getSema(), FnDecl);

   llvm::DenseMap<AllocaInst*, unsigned> AllocaMap;
   llvm::DenseMap<BasicBlock*, BitVector> GenMap;
   llvm::DenseMap<BasicBlock*, BitVector> InMap;
   llvm::DenseMap<BasicBlock*, BitVector> OutMap;
   llvm::SmallPtrSet<StoreInst*, 8> InitStores;
   llvm::SmallPtrSet<Instruction*, 16> Whitelist;

   unsigned i = 0;
   for (auto &B : F) {
      for (auto &I : B) {
         if (auto Alloca = dyn_cast<AllocaInst>(&I)) {
            AllocaMap.try_emplace(Alloca, i++);
         }
      }
   }

   analyseFunction<&BitVector::operator&=>(F, AllocaMap.size(), GenMap, InMap,
                                           OutMap,
                                           [&](Instruction &I, BitVector &Gen) {
                                              checkStore(I, AllocaMap, Gen,
                                                         Whitelist,
                                                         &InitStores);
                                           });

   // walk through all blocks again, this time checking any loads to see if
   // the value is initialized at that point
   for (auto &B : F) {
      if (B.hasNoPredecessors())
         continue;

      // this time track Gen on a per-instruction basis
      BitVector Gen;
      Gen.resize(AllocaMap.size());

      Gen |= InMap[&B];

      for (auto &I : B) {
         checkStore(I, AllocaMap, Gen, Whitelist);

         if (Whitelist.find(&I) != Whitelist.end())
            continue;

         if (auto Load = dyn_cast<LoadInst>(&I)) {
            auto Alloca = dyn_cast<AllocaInst>(Load->getTarget());
            if (!Alloca)
               continue;

            auto Idx = AllocaMap[Alloca];
            if (!Gen.test(Idx)) {
               if (auto Decl = ILGen.getDeclForValue(Alloca)) {
                  auto &SP = ILGen.getSema();

                  SP.diagnose(Decl, err_uninitialized_local,
                              Decl->getDeclName(), Load->getSourceLoc());

                  SP.diagnose(Decl, note_uninitialized_declared_here);
               }
            }
         }
      }
   }

   for (auto Store : InitStores) {
      Store->setIsInit(true);

      // erase any releases of the old value
      auto Val = Store->getDst();
      for (auto Use : Val->getUses()) {
         auto Ld = dyn_cast_or_null<LoadInst>(Use->getUser());
         if (!Ld)
            continue;

         auto Call = dyn_cast_or_null<IntrinsicCallInst>(Ld->getSingleUser());
         if (!Call)
            continue;

         if (Call->getCalledIntrinsic() == Intrinsic::release)
            Call->detachAndErase();
      }
   }

   // repeat analysis, this time with intersection instead of union to detect
   // possible double-inits
   InMap.clear();
   OutMap.clear();
   GenMap.clear();

   llvm::SmallDenseMap<ast::NamedDecl*, SourceLocation> StoreMap;
   analyseFunction<&BitVector::operator|=>(F, AllocaMap.size(), GenMap, InMap,
                                           OutMap,
                                           [&](Instruction &I, BitVector &Gen) {
                                              checkStore(I, AllocaMap, Gen,
                                                         Whitelist);
                                           });

   for (auto &B : F) {
      if (B.hasNoPredecessors())
         continue;

      // this time track Gen on a per-instruction basis
      BitVector Gen;
      Gen.resize(AllocaMap.size());

      Gen |= InMap[&B];

      for (auto &I : B) {
         if (auto Alloc = dyn_cast<AllocaInst>(&I)) {
            // an alloca kills a store again, this allows declaring and
            // initializing a variable in a loop
            Gen[AllocaMap[Alloc]] = 0;
         }
         else if (auto Store = dyn_cast<StoreInst>(&I)) {
            auto Alloca = dyn_cast<AllocaInst>(Store->getDst());
            if (!Alloca)
               continue;

            auto Decl = ILGen.getDeclForValue(Alloca);
            if (!Decl || !Decl->isConst())
               continue;

            auto Idx = AllocaMap[Alloca];
            if (Gen.test(Idx)) {
               auto &SP = ILGen.getSema();

               SP.diagnose(Decl, err_initialized_more_than_once,
                           Decl->getDeclName(), Store->getSourceLoc());

               SourceLocation Loc;

               // the same store might be in a loop and thus called multiple
               // times
               auto it = StoreMap.find(Decl);
               if (it == StoreMap.end()) {
                  Loc = Decl->getSourceLoc();
               }
               else {
                  Loc = StoreMap[Decl];
               }

               SP.diagnose(Decl, note_previous_init_here, Loc);
            }

            StoreMap[Decl] = Store->getSourceLoc();
         }

         checkStore(I, AllocaMap, Gen, Whitelist);
      }
   }
}

void DefinitiveInitilizationPass::visitInitializer(il::Initializer &F)
{
   auto FnDecl = cast<ast::InitDecl>(ILGen.getDeclForValue(&F));
   if (FnDecl->isSynthesized())
      return;

   ast::SemaPass::DeclScopeRAII declScopeRAII(ILGen.getSema(), FnDecl);

   auto Rec = F.getRecordType();
   auto S = dyn_cast<ast::StructDecl>(Rec);
   if (!S)
      return;

   bool IsUnion = isa<ast::UnionDecl>(Rec);

   // add all fields without a default value to the uninitialized list
   unsigned NumFields = 0;
   llvm::DenseMap<DeclarationName, unsigned> FieldMap;

   for (auto Field : S->getFields()) {
      if (!Field->getDefaultVal()) {
         FieldMap.try_emplace(Field->getDeclName(), NumFields++);
      }
   }

   llvm::DenseMap<BasicBlock*, BitVector> GenMap;
   llvm::DenseMap<BasicBlock*, BitVector> InMap;
   llvm::DenseMap<BasicBlock*, BitVector> OutMap;
   llvm::SmallPtrSet<StoreInst*, 8> InitStores;

   // list of instructions that are known to only use 'self' to initialize a
   // property
   llvm::SmallPtrSet<Instruction*, 16> Whitelist;

   analyseFunction<&BitVector::operator&=>(F, FieldMap.size(), GenMap, InMap,
                                           OutMap,
                                           [&](Instruction &I, BitVector &Gen) {
                                              checkSelfStore(I, FieldMap,
                                                             Gen, Whitelist,
                                                             &InitStores);
                                           });

   // report an error if any stored property is not initialized by the end of
   // the constructor
   auto &Sema = ILGen.getSema();
   for (auto &B : F) {
      if (!B.isExitBlock())
         continue;

      auto &Out = OutMap[&B];
      if (!IsUnion) {
         for (unsigned i = 0; i < NumFields; ++i) {
            if (Out.test(i))
               continue;

            auto Decl = S->getFields()[i];
            Sema.diagnose(Decl, err_field_must_be_initialized,
                          Decl->getDeclName(), F.getSourceLoc());
         }
      }
      else if (!Out.any()) {
         auto Decl = S->getFields()[0];
         Sema.diagnose(Decl, err_field_must_be_initialized,
                       Decl->getDeclName(), F.getSourceLoc());
      }
   }

   for (auto Store : InitStores) {
      Store->setIsInit(true);

      // erase any releases of the old value
      auto Val = Store->getDst();
      for (auto Use : Val->getUses()) {
         auto Ld = dyn_cast_or_null<LoadInst>(Use->getUser());
         if (!Ld)
            continue;

         auto Call = dyn_cast_or_null<IntrinsicCallInst>(Ld->getSingleUser());
         if (!Call)
            continue;

         if (Call->getCalledIntrinsic() == Intrinsic::release)
            Call->detachAndErase();
      }
   }

   il::Argument *Self = &*F.getEntryBlock()->arg_begin();

   // now check that 'self' is not accessed (except to initialize a property)
   // before it is fully initialized
   for (auto &B : F) {
      if (B.hasNoPredecessors())
         continue;

      // this time track Gen on a per-instruction basis
      BitVector Gen;
      Gen.resize(FieldMap.size());

      Gen |= InMap[&B];

      for (auto &I : B) {
         checkSelfStore(I, FieldMap, Gen, Whitelist);

         if (Gen.all())
            break;

         if (Whitelist.find(&I) != Whitelist.end())
            continue;

         if (I.isSanctionedSelfUse() || I.isRetainOrRelease())
            continue;

         for (auto Operand : I.getOperands()) {
            if (Operand == Self) {
               Sema.diagnose(FnDecl, err_self_used_before_initialized,
                             I.getSourceLoc());

               break;
            }
         }
      }
   }

   // now check for duplicate initilization
   InMap.clear();
   OutMap.clear();
   GenMap.clear();

   analyseFunction<&BitVector::operator|=>(F, FieldMap.size(), GenMap, InMap,
                                           OutMap,
                                           [&](Instruction &I, BitVector &Gen) {
                                              checkSelfStore(I, FieldMap,
                                                             Gen, Whitelist);
                                           });

   llvm::SmallVector<SourceLocation, 4> StoreLocs;
   StoreLocs.resize(FieldMap.size());

   for (auto &B : F) {
      if (B.hasNoPredecessors())
         continue;

      // this time track Gen on a per-instruction basis
      BitVector Gen;
      Gen.resize(FieldMap.size());
      Gen |= InMap[&B];

      for (auto &I : B) {
         unsigned FieldIdx = static_cast<unsigned>(-1);

         bool WasSet = checkSelfStore(I, FieldMap, Gen, Whitelist, nullptr,
                                      &FieldIdx);

         if (WasSet) {
            auto Decl = S->getFields()[FieldIdx];
            if (Decl->isConst()) {
               Sema.diagnose(Decl, err_initialized_more_than_once,
                             Decl->getDeclName(), I.getSourceLoc());
               Sema.diagnose(Decl, note_previous_init_here,
                             StoreLocs[FieldIdx]);
            }
         }
         // base initializer call
         else if (FieldIdx == -2) {
            std::fill(StoreLocs.begin(), StoreLocs.end(), I.getSourceLoc());
         }
         else if (FieldIdx != -1) {
            StoreLocs[FieldIdx] = I.getSourceLoc();
         }
      }
   }
}

} // namespace il
} // namespace cdot