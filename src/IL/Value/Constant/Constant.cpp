//
// Created by Jonas Zell on 16.11.17.
//

#include "Constant.h"

namespace cdot {
namespace il {

Constant::Constant(TypeID id, Type *ty,
                   const std::string &name,
                   const SourceLocation &loc)
   : Value(id, ty, name, loc)
{

}

Constant::Constant(TypeID id, ILType ty,
                   const std::string &name,
                   const SourceLocation &loc)
   : Value(id, ty, name, loc)
{

}

} // namespace il
} // namespace cdot