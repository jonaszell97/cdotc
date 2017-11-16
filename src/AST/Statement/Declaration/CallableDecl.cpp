//
// Created by Jonas Zell on 09.10.17.
//

#include "CallableDecl.h"

CallableDecl::CallableDecl(
   AccessModifier am,
   string &&name,
   std::shared_ptr<TypeRef> &&returnType,
   std::vector<std::shared_ptr<FuncArgDecl>> &&args)
   : am(am), name(name), returnType(returnType),
     args(args)
{

}