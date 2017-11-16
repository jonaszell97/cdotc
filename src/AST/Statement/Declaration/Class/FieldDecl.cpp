//
// Created by Jonas Zell on 27.06.17.
//

#include "FieldDecl.h"
#include "../../../Expression/TypeRef.h"

FieldDecl::FieldDecl(std::string &&fieldName,
                     std::shared_ptr<TypeRef> &&type,
                     AccessModifier am,
                     bool isStatic,
                     bool isConst,
                     Expression::SharedPtr &&defaultVal)
   : RecordSubDecl(std::move(fieldName), am, isStatic), type(type),
     is_const(isConst), defaultVal(std::move(defaultVal))
{

}

std::vector<AstNode::SharedPtr> FieldDecl::get_children() {
   return (defaultVal != nullptr) ? std::vector<AstNode::SharedPtr>{defaultVal}
                                  : std::vector<AstNode::SharedPtr>{};
}