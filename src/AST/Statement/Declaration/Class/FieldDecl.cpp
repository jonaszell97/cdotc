//
// Created by Jonas Zell on 27.06.17.
//

#include "FieldDecl.h"
#include "../../../Expression/TypeRef.h"

FieldDecl::FieldDecl(std::string field_name, std::shared_ptr<TypeRef> type, AccessModifier am, bool is_static, bool 
      isConst, Expression::SharedPtr def_val):
   fieldName(field_name),
   type(type),
   am(am),
   is_static(is_static),
   is_const(isConst),
   defaultVal(def_val)
{

}

std::vector<AstNode::SharedPtr> FieldDecl::get_children() {
   return (defaultVal != nullptr) ? std::vector<AstNode::SharedPtr>{ defaultVal } : std::vector<AstNode::SharedPtr>{};
}

void FieldDecl::__dump(int depth) {
   AstNode::__tab(depth);

   std::string get_set = "";
   if (has_getter || has_setter) {
      get_set = " (";
      if (has_getter) {
         get_set += "get,";
      }
      if (has_setter) {
         get_set += "set,";
      }
      get_set.pop_back();
      get_set += ")";
   }

   std::cout << (is_static ? "Static" : "") << "FieldDecl [" << util::am_map[am] << " " << type->toString()
           << " " << fieldName << get_set << "]" << std::endl;

   for (auto c : get_children()) {
      c->__dump(depth + 1);
   }
}