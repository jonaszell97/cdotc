//
// Created by Jonas Zell on 16.11.17.
//

#include "ConstantExpr.h"
#include "Constant.h"

namespace cdot {
namespace il {

ConstantExpr::ConstantExpr(TypeID id, Type *ty, BasicBlock *parent,
                           const std::string &name, const SourceLocation &loc)
   : Instruction(id, ty, parent, name, loc)
{

}

ConstantBitCastInst::ConstantBitCastInst(Constant *Val, Type *toType,
                                 BasicBlock *parent, const string &name,
                                 const SourceLocation &loc)
   : ConstantExpr(ConstantBitCastInstID, toType, parent, name, loc), target(Val)
{

}

} // namespace il
} // namespace cdot