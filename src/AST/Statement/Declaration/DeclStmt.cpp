//
// Created by Jonas Zell on 19.06.17.
//

#include "DeclStmt.h"
#include "../../Expression/TypeRef.h"


DeclStmt::DeclStmt(
   string identifier, TypeRef::SharedPtr type, bool is_const,
   Expression::SharedPtr val)
   : identifier(identifier),
     type(type),
     is_const(is_const),
     value(val)
{

}

std::vector<AstNode::SharedPtr> DeclStmt::get_children() {
   std::vector<AstNode::SharedPtr> children{ type };
   if (value) {
      children.push_back(value);
   }

   return children;
}