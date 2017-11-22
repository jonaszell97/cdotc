//
// Created by Jonas Zell on 29.08.17.
//

#include "EnumDecl.h"
#include "MethodDecl.h"
#include "EnumCaseDecl.h"

#include "../../../Passes/SemanticAnalysis/Record/Enum.h"

namespace cdot {
namespace ast {

EnumDecl::EnumDecl(AccessModifier am,
                   string &&enumName,
                   std::shared_ptr<TypeRef>& rawType,
                   std::vector<std::shared_ptr<Statement>> &&methods,
                   std::vector<std::shared_ptr<PropDecl>> &&properties,
                   std::vector<std::shared_ptr<TypeRef>> &&conformsTo,
                   std::vector<std::shared_ptr<EnumCaseDecl>> &&cases,
                   std::vector<Statement::SharedPtr> &&innerDeclarations) :
   RecordDecl(
      EnumDeclID,
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

cl::Enum* EnumDecl::getDeclaredEnum() const
{
   return record->getAs<Enum>();
}

} // namespace ast
} // namespace cdot