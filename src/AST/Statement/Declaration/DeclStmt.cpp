//
// Created by Jonas Zell on 19.06.17.
//

#include "DeclStmt.h"
#include "../../Expression/TypeRef.h"

namespace cdot {
namespace ast {

DeclStmt::DeclStmt(string &&identifier,
                   TypeRef::SharedPtr &&type,
                   bool is_const,
                   Expression::SharedPtr &&val)
   : Statement(DeclStmtID), identifier(move(identifier)),
     type(move(type)),
     is_const(is_const),
     value(move(val))
{

}

} // namespace ast
} // namespace cdot