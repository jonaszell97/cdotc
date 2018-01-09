//
// Created by Jonas Zell on 19.06.17.
//

#include "LocalVarDecl.h"
#include "../../Expression/TypeRef.h"

namespace cdot {
namespace ast {

LocalVarDecl::LocalVarDecl(std::vector<string> &&identifiers,
                           TypeRef::SharedPtr &&type,
                           bool is_const,
                           Expression::SharedPtr &&val)
   : VarDecl(LocalVarDeclID, AccessModifier::PUBLIC, is_const,
             move(identifiers), move(type), move(val)),
     declared(false), captured(false)
{

}

LocalVarDecl::~LocalVarDecl() = default;

GlobalVarDecl::GlobalVarDecl(AccessModifier access,
                             std::vector<string> &&identifiers,
                             std::shared_ptr<TypeRef> &&type, bool isConst,
                             std::shared_ptr<Expression> &&val)
   : VarDecl(GlobalVarDeclID, access, isConst, move(identifiers),
             move(type), move(val))
{

}

} // namespace ast
} // namespace cdot