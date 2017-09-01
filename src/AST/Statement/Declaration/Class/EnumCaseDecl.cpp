//
// Created by Jonas Zell on 29.08.17.
//

#include "EnumCaseDecl.h"
#include "../../../Expression/TypeRef.h"

using std::string;
using std::pair;

EnumCaseDecl::EnumCaseDecl(string &&name, long rawValue,
      std::vector<pair<string, std::shared_ptr<TypeRef>>> &&associatedTypes) :
   caseName(name),
   rawValue(rawValue),
   associatedTypes(associatedTypes),
   hasRawValue(true)
{

}

EnumCaseDecl::EnumCaseDecl(string &&name, std::vector<pair<string, std::shared_ptr<TypeRef>>> &&associatedTypes) :
   caseName(name),
   associatedTypes(associatedTypes),
   hasRawValue(false)
{

}

std::vector<std::shared_ptr<AstNode>> EnumCaseDecl::get_children() {
   std::vector<std::shared_ptr<AstNode>> children;
   for (const auto& ty : associatedTypes) {
      children.push_back(ty.second);
   }

   return children;
}

void EnumCaseDecl::__dump(int depth) {
   AstNode::__tab(depth);
   std::cout << "EnumCaseDecl [" << caseName << "]\n";
}