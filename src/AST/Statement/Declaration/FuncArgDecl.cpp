//
// Created by Jonas Zell on 20.06.17.
//

#include <iostream>
#include "FuncArgDecl.h"
#include "../../Expression/TypeRef.h"

namespace cdot {
namespace ast {

FuncArgDecl::FuncArgDecl(std::string &&name,
                         TypeRef::SharedPtr &&type,
                         Expression::SharedPtr &&def,
                         bool variadicArgPackExpansion,
                         bool isConst)
   : Statement(FuncArgDeclID), argName(move(name)),
     argType(move(type)), defaultVal(move(def)),
     variadicArgPackExpansion(variadicArgPackExpansion),
     is_const(isConst)
{

}

} // namespace ast
} // namespace cdot