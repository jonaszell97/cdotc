#ifndef CDOT_USEBEFOREINIT_H
#define CDOT_USEBEFOREINIT_H

#include "cdotc/IL/Analysis/MemoryLocation.h"
#include "cdotc/IL/Passes/InstructionVisitor.h"
#include "cdotc/IL/Passes/Passes.h"

#include <llvm/ADT/BitVector.h>

namespace cdot {
namespace ast {
class CallableDecl;
class FieldDecl;
class ILGenPass;
} // namespace ast

namespace il {
namespace detail {
struct LocalVariable;
}

class UnsafeAnalysisInfo;

class DefinitiveInitializationPass
    : public FunctionPass,
      public InstructionVisitor<DefinitiveInitializationPass, void,
                                llvm::BitVector&, llvm::BitVector&> {
   using LocalVariable = detail::LocalVariable;

   /// The ILGen object
   ast::ILGenPass& ILGen;

   /// Number of interesting memory locations
   unsigned MemoryLocCounter = 0;

   /// Used to verify that all fields are initialized in a constructor
   LocalVariable* SelfVal = nullptr;

   /// Memory location of the self value.
   MemoryLocation SelfLoc;

   /// Memory locations we track through the function
   llvm::DenseMap<MemoryLocation, LocalVariable*> MemoryLocs;

   /// Function arguments that are assumed to be initialized entering the entry
   /// block
   llvm::SmallVector<LocalVariable*, 4> InitializedArgs;

   /// Whitelist of loads that are allowed to access uninitialized memory
   llvm::SmallPtrSet<Instruction*, 16> Whitelist;

   /// Mapping from BasicBlocks to their Gen Set
   llvm::DenseMap<BasicBlock*, llvm::BitVector> GenMap;
   llvm::DenseMap<BasicBlock*, llvm::BitVector> KillMap;

   /// Mapping from BasicBlocks to their In and Out Sets
   llvm::DenseMap<BasicBlock*, llvm::BitVector> MustInMap;
   llvm::DenseMap<BasicBlock*, llvm::BitVector> MustOutMap;
   llvm::DenseMap<BasicBlock*, llvm::BitVector> MayInMap;
   llvm::DenseMap<BasicBlock*, llvm::BitVector> MayOutMap;

   /// Assign Instructions that are known to initialize, not store
   llvm::SmallPtrSet<AssignInst*, 8> InitAssigns;

   /// List of Instructions to remove
   llvm::SmallVector<Instruction*, 8> InstsToRemove;

   /// Set of uninitialized fields of self, used to track initialization in
   /// fallible initializers
   llvm::SmallPtrSet<MemoryLocation, 8> SelfFields;

   /// Info about unsafe instructions
   UnsafeAnalysisInfo* UA = nullptr;

   /// Whether or not we're doing the second pass over all instructions
   bool SecondPass = false;

   void prepareInitializer(ast::CallableDecl* FnDecl, Initializer& I);

   void verifyMemoryUse(Instruction& I, llvm::BitVector& MustGen,
                        llvm::BitVector& MayGen);

   void checkSelfInitializationState(llvm::BitVector& MustGen,
                                     llvm::BitVector& MayGen,
                                     SmallPtrSetImpl<ast::FieldDecl*> &InvalidFields);

   void checkDeinitilization(CallInst& I, llvm::BitVector& MustGen,
                             llvm::BitVector& MayGen);

   LocalVariable* getLocal(Value* Val);
   LocalVariable* getLocal(Value* Val, MemoryLocation Loc, QualType Ty,
                           bool NoClasses = true);

   LocalVariable* lookupLocal(MemoryLocation Loc)
   {
      auto It = MemoryLocs.find(Loc);
      if (It != MemoryLocs.end())
         return It->getSecond();

      return nullptr;
   }

   void solveDataFlowProblem(Function& F);

   void visitAssignInst(AssignInst& I, llvm::BitVector& Gen,
                        llvm::BitVector& Kill);
   void visitInitInst(InitInst& I, llvm::BitVector& Gen, llvm::BitVector& Kill);
   void visitMoveInst(MoveInst& I, llvm::BitVector& Gen, llvm::BitVector& Kill);
   void visitCallInst(CallInst& I, llvm::BitVector& Gen, llvm::BitVector& Kill);
   void visitLoadInst(LoadInst& I, llvm::BitVector& Gen, llvm::BitVector& Kill);

public:
   explicit DefinitiveInitializationPass(ast::ILGenPass& ILGen);

   void run() override;

   static bool classof(ILPass const* P)
   {
      return P->getKind() == PassKind::DefinitiveInitializationPassID;
   }

   friend InstructionVisitor;
};

} // namespace il
} // namespace cdot

#endif // CDOT_USEBEFOREINIT_H
