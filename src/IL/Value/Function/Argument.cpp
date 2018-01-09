//
// Created by Jonas Zell on 17.11.17.
//

#include "Argument.h"

namespace cdot {
namespace il {

Argument::Argument(Type *type,
                   bool vararg,
                   BasicBlock *parent,
                   llvm::StringRef name)
   : Value(ArgumentID, type), parent(parent), vararg(vararg)
{
   new (&this->name) std::string(name.data(), name.size());
}

Argument::Argument(QualType type,
                   bool vararg,
                   BasicBlock *parent,
                   llvm::StringRef name)
   : Value(ArgumentID, type), parent(parent), vararg(vararg)
{
   new (&this->name) std::string(name.data(), name.size());
}

void Argument::setParent(BasicBlock *p)
{
   parent = p;
}

} // namespace il
} // namespace cdot