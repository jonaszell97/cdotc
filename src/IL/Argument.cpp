//
// Created by Jonas Zell on 17.11.17.
//

#include "Argument.h"

#include "AST/ASTContext.h"
#include "BasicBlock.h"

namespace cdot {
namespace il {

Argument::Argument(ValueType type,
                   ArgumentConvention Conv,
                   BasicBlock *parent,
                   llvm::StringRef name)
   : Value(ArgumentID, type),
     parent(parent)
{
   setConvention(Conv);
   this->name = name.str();
}

Argument::Argument(const il::Argument &Arg, il::BasicBlock &Parent)
   : Value(ArgumentID, Arg.type),
     parent(&Parent)
{
   ArgBits = Arg.ArgBits;
   setName(Arg.getName());
}

} // namespace il
} // namespace cdot