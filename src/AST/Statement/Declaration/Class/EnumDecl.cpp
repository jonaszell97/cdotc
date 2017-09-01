//
// Created by Jonas Zell on 29.08.17.
//

#include "EnumDecl.h"
#include "MethodDecl.h"
#include "EnumCaseDecl.h"

EnumDecl::EnumDecl(AccessModifier am, string &&enumName, std::vector<std::shared_ptr<MethodDecl>>&& methods,
   std::vector<ObjectType *> &&conformsTo, std::vector<ObjectType *> &&generics,
   std::vector<std::shared_ptr<EnumCaseDecl>> &&cases) :
      className(enumName),
      am(am),
      methods(methods),
      conformsTo(conformsTo),
      generics(generics),
      cases(cases)
{

}

std::vector<std::shared_ptr<AstNode>> EnumDecl::get_children() {
   std::vector<std::shared_ptr<AstNode>> children;
   for (const auto& method : methods) {
      children.push_back(method);
   }
   for (const auto& case_ : cases) {
      children.push_back(case_);
   }

   return children;
}

void EnumDecl::__dump(int depth) {
   AstNode::__tab(depth);
   std::cout << "EnumDecl [" << className << "]\n";

   for (const auto& child : get_children()) {
      child->__dump(depth + 1);
   }
}