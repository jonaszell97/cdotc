//
// Created by Jonas Zell on 20.06.17.
//

#include <iostream>
#include "FuncArgDecl.h"
#include "../../Expression/TypeRef.h"

FuncArgDecl::FuncArgDecl(std::string name, TypeRef::SharedPtr type, Expression::SharedPtr def) :
   argName(name),
   argType(type),
   defaultVal(def)
{
   if (defaultVal != nullptr) {
      children.push_back(&defaultVal);
   }
}

FuncArgDecl::FuncArgDecl(bool mut) : mut(mut) {

}

std::shared_ptr<FuncArgDecl> FuncArgDecl::clone() const
{
   auto decl = std::make_shared<FuncArgDecl>(*this);
   decl->argType = std::make_shared<TypeRef>(argType->getType());

   return decl;
}

std::vector<AstNode::SharedPtr> FuncArgDecl::get_children() {
   if (defaultVal != nullptr) {
      return { defaultVal };
   }

   return {};
}