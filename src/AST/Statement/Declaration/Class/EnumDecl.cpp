//
// Created by Jonas Zell on 29.08.17.
//

#include "EnumDecl.h"
#include "MethodDecl.h"
#include "EnumCaseDecl.h"

#include "../../../Passes/SemanticAnalysis/Record/Enum.h"

EnumDecl::EnumDecl(AccessModifier am,
                   string &&enumName,
                   std::shared_ptr<TypeRef>& rawType,
                   std::vector<std::shared_ptr<Statement>> &&methods,
                   std::vector<std::shared_ptr<PropDecl>> &&properties,
                   std::vector<std::shared_ptr<TypeRef>> &&conformsTo,
                   std::vector<std::shared_ptr<EnumCaseDecl>> &&cases,
                   std::vector<Statement::SharedPtr> &&innerDeclarations) :
   RecordDecl(
      am,
      std::move(enumName),
      std::move(conformsTo),
      std::move(methods),
      {},
      move(properties),
      std::move(innerDeclarations)
   ),
   rawType(move(rawType)),
   cases(move(cases))
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

cl::Enum* EnumDecl::getDeclaredEnum() const
{
   return record->getAs<Enum>();
}