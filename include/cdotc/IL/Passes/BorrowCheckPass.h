#ifndef CDOT_BORROWCHECKER_H
#define CDOT_BORROWCHECKER_H

#include "cdotc/IL/Analysis/MemoryLocation.h"
#include "cdotc/IL/Analysis/MemoryUse.h"
#include "cdotc/IL/Passes/InstructionVisitor.h"
#include "cdotc/IL/Passes/Passes.h"

#include <llvm/ADT/BitVector.h>

namespace cdot {
namespace ast {
class ILGenPass;
class NamedDecl;
} // namespace ast

namespace il {

class UnsafeAnalysisInfo;

class BorrowCheckPass
    : public FunctionPass,
      public InstructionVisitor<BorrowCheckPass, void, llvm::BitVector&,
                                llvm::BitVector&> {
   /// The ILGen object
   ast::ILGenPass& ILGen;

   /// Memory uses that are interesting for borrow checking, i.e. moves and
   /// (immutable) borrows
   SmallVector<MemoryUse, 8> MemoryUses;

   /// Mapping from instructions back to the memory use they cause
   llvm::DenseMap<const il::Instruction*, unsigned> InstUseMap;

   /// Mapping from memory locations back to their memory uses
   llvm::DenseMap<MemoryLocation, SmallVector<unsigned, 0>> LocUseMap;

   /// Mapping from MemoryLocations to the NamedDecl that they access
   llvm::DenseMap<MemoryLocation, ast::NamedDecl*> MemDeclMap;

   /// Allocator used for allocating MemoryRestrictions
   llvm::BumpPtrAllocator Allocator;

   /// Mapping from BasicBlocks to their Gen and Kill Sets
   llvm::DenseMap<BasicBlock*, llvm::BitVector> GenMap;
   llvm::DenseMap<BasicBlock*, llvm::BitVector> KillMap;

   /// Mapping from BasicBlocks to their In and Out Sets
   llvm::DenseMap<BasicBlock*, llvm::BitVector> InMap;
   llvm::DenseMap<BasicBlock*, llvm::BitVector> OutMap;

   /// Set of diagnosed source locations to avoid duplicate diagnostics
   llvm::DenseSet<SourceLocation> DiagnosedLocs;

   /// Info about unsafe instructions
   UnsafeAnalysisInfo* UA = nullptr;

   MemoryRestriction getRestriction(il::Value* V, MemoryUse::Kind UseKind);
   void buildRestrictions(il::Value* V, MemoryUse::Kind UseKind,
                          llvm::SmallVectorImpl<MemoryRestriction>& Vec);

   void buildMemoryUse(il::MoveInst& I);
   void buildMemoryUse(il::BeginBorrowInst& I);

   void computeMemoryAccesses(Function& F);

   void checkConflictingRestriction(const Instruction& I, MemoryLocation Loc,
                                    const MemoryUse& Use,
                                    const MemoryRestriction& Restriction);

   void checkConflictingUse(const Instruction& I, MemoryLocation Loc,
                            const MemoryUse& Use, bool IsDirectlyAccessedMem);

   void checkOverlappingMemoryUse(const Instruction& I,
                                  const llvm::BitVector& Gen);

   void visitIntrinsicCallInst(const IntrinsicCallInst& I, llvm::BitVector& Gen,
                               llvm::BitVector& Kill);
   void visitMoveInst(const MoveInst& I, llvm::BitVector& Gen,
                      llvm::BitVector& Kill);
   void visitBeginBorrowInst(const BeginBorrowInst& I, llvm::BitVector& Gen,
                             llvm::BitVector& Kill);
   void visitEndBorrowInst(const EndBorrowInst& I, llvm::BitVector& Gen,
                           llvm::BitVector& Kill);

public:
   explicit BorrowCheckPass(ast::ILGenPass& ILGen)
       : FunctionPass(PassKind::BorrowCheckPassID), ILGen(ILGen), Allocator()
   {
   }

   void run() override;

   static bool classof(ILPass const* P)
   {
      return P->getKind() == PassKind::BorrowCheckPassID;
   }

   friend InstructionVisitor;
};

} // namespace il
} // namespace cdot

#endif // CDOT_BORROWCHECKER_H
