//
// Created by Jonas Zell on 29.08.17.
//

#include "EnumDecl.h"
#include "MethodDecl.h"
#include "EnumCaseDecl.h"

EnumDecl::EnumDecl(AccessModifier am, string &&enumName, std::shared_ptr<TypeRef>& rawType,
   std::vector<std::shared_ptr<MethodDecl>>&&methods, std::vector<std::shared_ptr<TypeRef>> &&conformsTo,
   std::vector<GenericConstraint> &&generics,
   std::vector<std::shared_ptr<EnumCaseDecl>> &&cases, std::vector<Statement::SharedPtr>&& innerDeclarations) :
      className(enumName),
      am(am),
      rawType(rawType),
      methods(methods),
      conformsTo(conformsTo),
      generics(generics),
      cases(cases),
      innerDeclarations(innerDeclarations)
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
   for (const auto& inner : innerDeclarations) {
      children.push_back(inner);
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