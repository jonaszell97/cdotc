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
   : Statement(EnumCaseDeclID), caseName(name), rawVal(rawValue),
     associatedTypes(associatedTypes), has_raw_value(true)
{

}

EnumCaseDecl::EnumCaseDecl(string &&name,
                           AssociatedTypeVec &&associatedTypes)
   : Statement(EnumCaseDeclID), caseName(move(name)),
     associatedTypes(move(associatedTypes)), has_raw_value(false)
{

}

} // namespace ast
} // namespace cdot