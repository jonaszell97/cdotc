//
// Created by Jonas Zell on 10.10.17.
//

#include "UnionDecl.h"

#include "../../../Expression/TypeRef.h"

UnionDecl::UnionDecl(string &&name, UnionTypes &&types, bool isConst)
   : name(name), containedTypes(types), is_const(isConst)
{

}

std::vector<std::shared_ptr<AstNode>>
UnionDecl::get_children()
{
   std::vector<std::shared_ptr<AstNode>> children;
   for (const auto & ty : containedTypes) {
      children.push_back(ty.second);
   }

   return children;
}

bool UnionDecl::isConst() const {
   return is_const;
}

void UnionDecl::isConst(bool is_const) {
   UnionDecl::is_const = is_const;
}
