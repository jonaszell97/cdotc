//
// Created by Jonas Zell on 01.08.17.
//

#include "TypedefDecl.h"
#include "../../Expression/TypeRef.h"

TypedefDecl::TypedefDecl(
   AccessModifier access,
   string &&alias,
   std::shared_ptr<TypeRef> &&origin,
   std::vector<GenericConstraint> &&generics)
   : alias(alias), origin(origin), generics(generics), access(access)
{
   
}

std::vector<AstNode::SharedPtr> TypedefDecl::get_children() {
   return {};
}

void TypedefDecl::__dump(int depth) {
   AstNode::__tab(depth);

   std::cout << "TypedefDecl [" << origin->toString() << " -> " << alias << "]" << std::endl;
}