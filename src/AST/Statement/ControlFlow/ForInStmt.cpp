//
// Created by Jonas Zell on 01.09.17.
//

#include "ForInStmt.h"
#include "../Declaration/LocalVarDecl.h"
#include "../../Expression/Expression.h"

namespace cdot {
namespace ast {

ForInStmt::ForInStmt(std::shared_ptr<LocalVarDecl>&& decl,
                     std::shared_ptr<Expression>&& range,
                     std::shared_ptr<Statement>&& body)
   : Statement(ForInStmtID), decl(move(decl)), rangeExpr(move(range)),
     body(move(body))
{

}


} // namespace ast
} // namespace cdot