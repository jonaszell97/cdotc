//
// Created by Jonas Zell on 29.08.17.
//

#include "EnumCaseDecl.h"
#include "../../../Expression/TypeRef.h"

using std::string;
using std::pair;

EnumCaseDecl::EnumCaseDecl(string &&name,
                           std::shared_ptr<Expression>&& rawValue,
      std::vector<pair<string, std::shared_ptr<TypeRef>>> &&associatedTypes) :
   caseName(name),
   rawVal(rawValue),
   associatedTypes(associatedTypes),
   has_raw_value(true)
{

}

EnumCaseDecl::EnumCaseDecl(string &&name,
                           std::vector<pair<string, std::shared_ptr<TypeRef>>>
                           &&associatedTypes)
   : caseName(name),
   associatedTypes(associatedTypes),
   has_raw_value(false)
{

}

std::vector<std::shared_ptr<AstNode>> EnumCaseDecl::get_children() {
   std::vector<std::shared_ptr<AstNode>> children;
   for (const auto& ty : associatedTypes) {
      children.push_back(ty.second);
   }

   return children;
}