//
// Created by Jonas Zell on 17.11.17.
//

#include "Argument.h"
#include "BasicBlock.h"

namespace cdot {
namespace il {

Argument::Argument(ValueType type,
                   Convention Conv,
                   BasicBlock *parent,
                   llvm::StringRef name)
   : Value(ArgumentID, type),
     parent(parent)
{
   setConvention(Conv);
   this->name = name.str();
}

} // namespace il
} // namespace cdot