//
// Created by Jonas Zell on 17.11.17.
//

#include "Argument.h"

namespace cdot {
namespace il {

Argument::Argument(Type *type,
                   bool vararg,
                   Function *parent,
                   const std::string &name,
                   const SourceLocation &loc)
   : Value(ArgumentID, type, name), parent(parent), vararg(vararg)
{

}

Argument::Argument(ILType type,
                   bool vararg,
                   Function *parent,
                   const std::string &name,
                   const SourceLocation &loc)
   : Value(ArgumentID, type, name), parent(parent), vararg(vararg)
{

}

bool Argument::isVararg() const
{
   return vararg;
}

} // namespace il
} // namespace cdot