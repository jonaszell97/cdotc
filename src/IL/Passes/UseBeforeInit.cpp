//
// Created by Jonas Zell on 05.03.18.
//

#include "UseBeforeInit.h"

#include "AST/NamedDecl.h"
#include "AST/Passes/ILGen/ILGenPass.h"
#include "AST/Passes/SemanticAnalysis/SemaPass.h"
#include "IL/Utils/BlockIterator.h"
#include "Message/Diagnostics.h"

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

static void checkStore(Instruction &I,
                       llvm::DenseMap<AllocaInst*, unsigned> &AllocaMap,
                       BitVector &Gen) {
   auto S = dyn_cast<StoreInst>(&I);
   if (!S)
      return;

   auto Alloca = dyn_cast<AllocaInst>(S->getDst());
   if (!Alloca)
      return;

   auto It = AllocaMap.find(Alloca);
   assert(It != AllocaMap.end());

   auto Idx = It->second;
   Gen.set(Idx);
}

static void analyseFunction(Function &F,
                            llvm::DenseMap<AllocaInst*, unsigned> &AllocaMap,
                            llvm::DenseMap<BasicBlock*, BitVector> &GenMap,
                            llvm::DenseMap<BasicBlock*, BitVector> &InMap,
                            BitVector&(BitVector::*meet)(const BitVector&)) {
   unsigned i = 0;
   for (auto &B : F) {
      for (auto &I : B) {
         if (auto Alloca = dyn_cast<AllocaInst>(&I)) {
            AllocaMap.try_emplace(Alloca, i++);
         }
      }
   }

   // Calculate Gen and Kill sets for each BB
   // Kill always empty since a variable can't be "deinitialized"
   for (auto &B : F) {
      auto &Gen = GenMap[&B];
      Gen.resize(AllocaMap.size());

      for (auto &I : B) {
         checkStore(I, AllocaMap, Gen);
      }
   }

   // Calculate In / Out until no more changes happen
   llvm::DenseMap<BasicBlock*, BitVector> OutMap;
   llvm::SmallPtrSet<BasicBlock*, 16> WorkList;

   // assume all are initialized at the beginning (except for entry block)
   for (auto &B : F) {
      auto &In = InMap[&B];
      auto &Out = OutMap[&B];

      In.resize(AllocaMap.size());
      Out.resize(AllocaMap.size());

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

      if (BB == F.getEntryBlock())
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

   llvm::DenseMap<AllocaInst*, unsigned> AllocaMap;
   llvm::DenseMap<BasicBlock*, BitVector> GenMap;
   llvm::DenseMap<BasicBlock*, BitVector> InMap;

   analyseFunction(F, AllocaMap, GenMap, InMap, &BitVector::operator&=);

   // walk through all blocks again, this time checking any loads to see if
   // the value is initialized at that point
   for (auto &B : F) {
      // this time track Gen on a per-instruction basis
      BitVector Gen;
      Gen.resize(AllocaMap.size());

      Gen |= InMap[&B];

      for (auto &I : B) {
         checkStore(I, AllocaMap, Gen);

         if (auto Load = dyn_cast<LoadInst>(&I)) {
            auto Alloca = dyn_cast<AllocaInst>(Load->getTarget());
            if (!Alloca)
               continue;

            auto Idx = AllocaMap[Alloca];
            if (!Gen.test(Idx)) {
               if (auto Decl = ILGen.getDeclForValue(Alloca)) {
                  auto &SP = ILGen.getSema();

                  SP.diagnose(Decl, err_uninitialized_local, Decl->getName(),
                              Load->getSourceLoc());

                  SP.diagnose(Decl, note_uninitialized_declared_here);
               }
            }
         }
      }
   }

   // repeat analysis, this time with intersection instead of union to detect
   // possible double-inits
   AllocaMap.clear();
   InMap.clear();
   GenMap.clear();

   llvm::SmallDenseMap<ast::NamedDecl*, SourceLocation> StoreMap;

   analyseFunction(F, AllocaMap, GenMap, InMap, &BitVector::operator|=);

   for (auto &B : F) {
      // this time track Gen on a per-instruction basis
      BitVector Gen;
      Gen.resize(AllocaMap.size());

      Gen |= InMap[&B];

      for (auto &I : B) {
         if (auto Store = dyn_cast<StoreInst>(&I)) {
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
                           Decl->getName(), Store->getSourceLoc());

               SP.diagnose(Decl, note_previous_init_here, StoreMap[Decl]);
            }

            StoreMap[Decl] = Store->getSourceLoc();
         }

         checkStore(I, AllocaMap, Gen);
      }
   }
}

} // namespace il
} // namespace cdot