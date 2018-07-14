//
// Created by Jonas Zell on 30.04.18.
//

#include "MemoryLocation.h"

#include "IL/Constants.h"
#include "IL/Function.h"
#include "IL/Instructions.h"
#include "IL/Passes/InstructionVisitor.h"
#include "Support/Various.h"

using namespace cdot::support;

namespace cdot {
namespace il {
namespace {

class MemLocBuilder: public InstructionVisitor<MemLocBuilder> {
   uintptr_t &HashVal;
   bool Exact;
   bool LookThroughMove;

public:
   explicit MemLocBuilder(uintptr_t &HashVal, bool Exact, bool LookThroughMove)
      : HashVal(HashVal), Exact(Exact), LookThroughMove(LookThroughMove)
   {}

   void AddPointer(const void *Ptr)
   {
      hash_combine(HashVal, reinterpret_cast<uintptr_t>(Ptr));
   }

   template<class T>
   void AddInteger(T val)
   {
      hash_combine(HashVal, val);
   }

   // final memory locations
   void visitGlobalVariable(const GlobalVariable &GV);
   void visitAllocaInst(const AllocaInst &I);
   void visitArgument(const Argument &A);

   // memory access path instructions
   void visitLoadInst(const LoadInst &I);
   void visitMoveInst(const MoveInst &I);
   void visitBitCastInst(const BitCastInst &I);

   void visitGEPInst(const GEPInst &I);
   void visitFieldRefInst(const FieldRefInst &I);
   void visitTupleExtractInst(const TupleExtractInst &I);
   void visitEnumExtractInst(const EnumExtractInst &I);
};

} // anonymous namespace

void MemLocBuilder::visitGlobalVariable(const il::GlobalVariable &GV)
{
   // we've reached a final memory location, finalize the hash and stop
   HashVal = reinterpret_cast<uintptr_t>(&GV);
}

void MemLocBuilder::visitAllocaInst(const il::AllocaInst &I)
{
   // we've reached a final memory location, finalize the hash and stop
   HashVal = reinterpret_cast<uintptr_t>(&I);
}

void MemLocBuilder::visitArgument(const il::Argument &A)
{
   // we've reached a final memory location, finalize the hash and stop
   HashVal = reinterpret_cast<uintptr_t>(&A);
}

void MemLocBuilder::visitLoadInst(const il::LoadInst &I)
{
   // lvalues along the access path always need to be loaded, so we just look
   // through loads
   visit(I.getTarget());
}

void MemLocBuilder::visitMoveInst(const il::MoveInst &I)
{
   if (!LookThroughMove)
      return AddPointer(&I);

   // in terms of memory locations, moves don't affect anything, so just look
   // through them (this might not always be literally true but it's good
   // enough for memory loc analyses)
   visit(I.getOperand(0));
}

void MemLocBuilder::visitBitCastInst(const il::BitCastInst &I)
{
   if (I.getType()->isReferenceType()
         && I.getOperand(0)->getType()->isReferenceType())
      return visit(I.getOperand(0));
}

void MemLocBuilder::visitGEPInst(const il::GEPInst &I)
{
   visit(I.getOperand(0));

   auto CI = dyn_cast<ConstantInt>(I.getIndex());
   if (!CI) {
      // if we need an exact memory loc, we can't reason about a non-constant
      // GEP index
      if (Exact) {
         HashVal = 0;
         return;
      }

      // otherwise we need to be conservative - assume that the entire
      // location so far is accessed
   }
   else {
      AddInteger(CI->getZExtValue());
   }
}

void MemLocBuilder::visitFieldRefInst(const il::FieldRefInst &I)
{
   visit(I.getOperand(0));
   AddPointer(I.getFieldName().getIdentifierInfo());
}

void MemLocBuilder::visitTupleExtractInst(const il::TupleExtractInst &I)
{
   visit(I.getOperand(0));

   auto CI = dyn_cast<ConstantInt>(I.getIndex());
   assert(CI && "TupleExtract with non-constant index!");

   AddInteger(CI->getZExtValue());
}

void MemLocBuilder::visitEnumExtractInst(const il::EnumExtractInst &I)
{
   HashVal = reinterpret_cast<uintptr_t>(&I);
}

MemoryLocation MemoryLocation::get()
{
   return MemoryLocation();
}

MemoryLocation MemoryLocation::getImpl(il::Value *V, bool Exact,
                                       bool LookThroughMove) {
   // a global variable is a memory location, no need to look further
   if (auto GV = dyn_cast<GlobalVariable>(V))
      return MemoryLocation(reinterpret_cast<uintptr_t>(GV));

   // we can't trace an argument any further, use it as the memory loc
   if (auto Arg = dyn_cast<Argument>(V))
      return MemoryLocation(reinterpret_cast<uintptr_t>(Arg));

   // otherwise, only instructions can represent memory locations
   if (auto I = dyn_cast<Instruction>(V))
      return getImpl(I, Exact, LookThroughMove);

   // this value does not point to memory
   return MemoryLocation();
}

MemoryLocation MemoryLocation::getImpl(il::Instruction *I, bool Exact,
                                       bool LookThroughMove) {
   uintptr_t HashVal = 0;
   MemLocBuilder(HashVal, Exact, LookThroughMove).visit(I);

   return MemoryLocation(HashVal);
}

MemoryLocation MemoryLocation::getTupleField(il::MemoryLocation Tup,
                                             unsigned Idx) {
   uintptr_t HashVal = Tup.OpaqueValue;
   hash_combine(HashVal, Idx);

   return MemoryLocation(HashVal);
}

MemoryLocation MemoryLocation::getStructField(il::MemoryLocation S,
                                              DeclarationName FieldName) {
   uintptr_t HashVal = S.OpaqueValue;
   hash_combine(HashVal,
                reinterpret_cast<uintptr_t>(FieldName.getIdentifierInfo()));

   return MemoryLocation(HashVal);
}

} // namespace il
} // namespace cdot