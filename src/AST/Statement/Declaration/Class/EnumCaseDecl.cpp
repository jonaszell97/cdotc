//
// Created by Jonas Zell on 29.08.17.
//

#include "EnumCaseDecl.h"
#include "../../../Expression/TypeRef.h"

using std::string;
using std::pair;

namespace cdot {
namespace ast {

EnumCaseDecl::EnumCaseDecl(string &&name,
                           std::shared_ptr<Expression>&& rawValue,
                           AssociatedTypeVec &&associatedTypes)
   : Statement(EnumCaseDeclID), caseName(name), rawVal(move(rawValue)),
     has_raw_value(true), associatedTypes(move(associatedTypes))
{

}

EnumCaseDecl::EnumCaseDecl(string &&name,
                           AssociatedTypeVec &&associatedTypes)
   : Statement(EnumCaseDeclID), caseName(move(name)),
     has_raw_value(false), associatedTypes(move(associatedTypes))
{

}

} // namespace ast
} // namespace cdot