//
// Created by Jonas Zell on 30.04.18.
//

#include "AccessPathDescriptor.h"

#include "AST/Decl.h"
#include "IL/Constants.h"
#include "IL/GlobalVariable.h"
#include "IL/Instructions.h"
#include "IL/Passes/InstructionVisitor.h"
#include "ILGen/ILGenPass.h"

#include <llvm/Support/raw_ostream.h>

using namespace cdot::support;

namespace cdot {
namespace il {
namespace {

class AccessPathBuilder: public InstructionVisitor<AccessPathBuilder> {
   ast::ILGenPass &ILGen;
   llvm::raw_ostream &OS;

public:
   AccessPathBuilder(ast::ILGenPass &ILGen, llvm::raw_ostream &OS)
      : ILGen(ILGen), OS(OS)
   {}

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
};

} // anonymous namespace

void AccessPathBuilder::visitGlobalVariable(const il::GlobalVariable &GV)
{
   if (auto Decl = ILGen.getDeclForValue(&GV)) {
      OS << Decl->getDeclName();
   }
   else{
      OS << "<unknown>";
   }
}

void AccessPathBuilder::visitAllocaInst(const il::AllocaInst &I)
{
   if (auto Decl = ILGen.getDeclForValue(&I)) {
      OS << Decl->getDeclName();
      return;
   }

   if (auto Store = dyn_cast<StoreInst>(I.getUses().begin()->getUser())) {
      if (Store->getSrc()->isSelf()) {
         OS << "self";
         return;
      }
   }

   OS << "<unknown>";
}

void AccessPathBuilder::visitArgument(const il::Argument &A)
{
   if (A.isSelf()) {
      OS << "self";
   }
   else if (auto Decl = ILGen.getDeclForValue(&A)) {
      OS << Decl->getDeclName();
   }
   else {
      OS << "<unknown>";
   }
}

void AccessPathBuilder::visitLoadInst(const il::LoadInst &I)
{
   visit(I.getTarget());
}

void AccessPathBuilder::visitMoveInst(const il::MoveInst &I)
{
   visit(I.getOperand(0));
}

void AccessPathBuilder::visitBitCastInst(const il::BitCastInst &I)
{
   if (I.getType()->isReferenceType()
       && I.getOperand(0)->getType()->isReferenceType())
      return visit(I.getOperand(0));
}

void AccessPathBuilder::visitGEPInst(const il::GEPInst &I)
{
   auto CI = dyn_cast<ConstantInt>(I.getIndex());
   if (!CI) {
      // otherwise we need to be conservative - assume that the entire
      // location so far is accessed
      return;
   }

   visit(I.getOperand(0));
   OS << "[" << CI->getValue() << "]";
}

void AccessPathBuilder::visitFieldRefInst(const il::FieldRefInst &I)
{
   visit(I.getOperand(0));
   OS << "." << I.getFieldName();
}

void AccessPathBuilder::visitTupleExtractInst(const il::TupleExtractInst &I)
{
   auto CI = dyn_cast<ConstantInt>(I.getIndex());
   if (!CI) {
      return;
   }

   visit(I.getOperand(0));
   OS << "." << CI->getValue();
}

std::string getAccessPathDescriptor(ast::ILGenPass &ILGen, const Value &I)
{
   std::string str;
   {
      llvm::raw_string_ostream OS(str);
      AccessPathBuilder(ILGen, OS).visit(I);
   }
   
   return str;
}

} // namespace il
} // namespace cdot