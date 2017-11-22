//
// Created by Jonas Zell on 27.06.17.
//

#include "FieldDecl.h"
#include "../../../Expression/TypeRef.h"

namespace cdot {
namespace ast {

FieldDecl::FieldDecl(std::string &&fieldName,
                     std::shared_ptr<TypeRef> &&type,
                     AccessModifier am,
                     bool isStatic,
                     bool isConst,
                     Expression::SharedPtr &&defaultVal)
   : RecordSubDecl(FieldDeclID, std::move(fieldName), am, isStatic), type(type),
     is_const(isConst), defaultVal(std::move(defaultVal))
{

}

} // namespace ast
} // namespace cdot