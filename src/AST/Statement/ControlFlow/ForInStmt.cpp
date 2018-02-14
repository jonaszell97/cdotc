//
// Created by Jonas Zell on 01.09.17.
//

#include "ForInStmt.h"
#include "../Declaration/LocalVarDecl.h"
#include "../../Expression/Expression.h"

namespace cdot {
namespace ast {

ForInStmt::ForInStmt(LocalVarDecl* decl,
                     Expression* range,
                     Statement* body)
   : Statement(ForInStmtID), decl(decl), rangeExpr(range),
     body(body)
{

}


} // namespace ast
} // namespace cdot