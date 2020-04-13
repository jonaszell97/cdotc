#include "BasicBlock.h"

#include "AST/ASTContext.h"
#include "Argument.h"
#include "Context.h"
#include "Function.h"
#include "IL/Utils/BlockIterator.h"
#include "Writer/ModuleWriter.h"

#include <llvm/Support/ErrorHandling.h>
#include <llvm/Support/raw_ostream.h>

using namespace cdot::support;
using std::string;

namespace cdot {
namespace il {

BasicBlock::BasicBlock(Function *parent)
   : Constant(BasicBlockID, ValueType(parent->getCtx(),
                                      parent->getASTCtx().getLabelTy())),
     parent(parent),
     Instructions(parent && !parent->getBasicBlocks().empty()
                  ? std::move(InstList(this, parent->getBasicBlocks().front()
                                             .getInstructions().getSymTab()))
                  : std::move(InstList(this))),
     Args(this, Instructions.getSymTab())
{
   if (parent) {
      parent->getBasicBlocks().push_back(this);
   }
}

BasicBlock::BasicBlock(const il::BasicBlock &BB, il::Function &F)
   : Constant(BasicBlockID, BB.getType()),
     parent(&F),
     Instructions(parent && !parent->getBasicBlocks().empty()
                  ? std::move(InstList(this, parent->getBasicBlocks().front()
                                                   .getInstructions().getSymTab()))
                  : std::move(InstList(this))),
     Args(this, Instructions.getSymTab())
{
   InstBits = BB.InstBits;
   setName(BB.getName());

   for (auto &Arg : BB.getArgs()) {
      Args.push_back(new Argument(Arg, *this));
   }

   F.getBasicBlocks().push_back(this);
}

BasicBlock::~BasicBlock()
{

}

void BasicBlock::print(llvm::raw_ostream &OS) const
{
   ModuleWriter(this).WriteTo(OS);
}

void BasicBlock::dump() const
{
   print(llvm::outs());
}

TerminatorInst const* BasicBlock::getTerminator() const
{
   if (Instructions.empty())
      return nullptr;

   return dyn_cast<TerminatorInst>(&Instructions.back());
}

TerminatorInst* BasicBlock::getTerminator()
{
   if (Instructions.empty())
      return nullptr;

   return dyn_cast<TerminatorInst>(&Instructions.back());
}

const BasicBlock::InstList &BasicBlock::getInstructions() const
{
   return Instructions;
}

BasicBlock::InstList &BasicBlock::getInstructions()
{
   return Instructions;
}

Function *BasicBlock::getParent() const
{
   return parent;
}

Argument const* BasicBlock::getBlockArg(llvm::StringRef name) const
{
   for (auto &arg : Args) {
      if (arg.getName() == name) {
         return &arg;
      }
   }

   return nullptr;
}

Argument const* BasicBlock::getBlockArg(unsigned idx) const
{
   assert(Args.size() > idx);
   return &Args[idx];
}

Argument* BasicBlock::getBlockArg(llvm::StringRef name)
{
   for (auto &arg : Args) {
      if (arg.getName() == name) {
         return &arg;
      }
   }

   return nullptr;
}

Argument* BasicBlock::getBlockArg(unsigned idx)
{
   assert(Args.size() > idx);
   return &Args[idx];
}

bool BasicBlock::isEntryBlock() const
{
   return this == parent->getEntryBlock();
}

bool BasicBlock::hasNoPredecessors() const
{
   if (this == parent->getEntryBlock())
      return false;

   return pred_begin(this) == pred_end(this);
}

bool BasicBlock::isExitBlock() const
{
   auto Term = getTerminator();
   if (!Term)
      return false;

   switch (Term->getTypeID()) {
   case RetInstID:
   case ThrowInstID:
   case RethrowInstID:
   case InvokeInstID:
      return true;
   default:
      return false;
   }
}

Argument *BasicBlock::addBlockArg(QualType ty, llvm::StringRef name)
{
   Args.push_back(new Argument(ValueType(getCtx(), ty),
                               ArgumentConvention::Owned, this));

   if (!name.empty()) {
      Args.back().setName(name);
   }

   return &Args.back();
}

} // namespace il
} // namespace cdot