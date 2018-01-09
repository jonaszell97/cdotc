//
// Created by Jonas Zell on 16.11.17.
//

#include "Constant.h"

namespace cdot {
namespace il {

Constant::Constant(TypeID id, Type *ty)
   : Constant(id, QualType(ty))
{

}

Constant::Constant(TypeID id, QualType ty)
   : Value(id, ty)
{

}

void Constant::handleReplacement(Value *with)
{

}

} // namespace il
} // namespace cdot