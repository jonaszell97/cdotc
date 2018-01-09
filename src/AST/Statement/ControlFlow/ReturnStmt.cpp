//
// Created by Jonas Zell on 19.06.17.
//

#include "ReturnStmt.h"
#include "../../Expression/Expression.h"

namespace cdot {
namespace ast {

ReturnStmt::ReturnStmt(Expression::SharedPtr &&retVal)
   : Statement(ReturnStmtID), returnValue(move(retVal))
{

}

ReturnStmt::ReturnStmt() : Statement(ReturnStmtID)
{

}

} // namespace ast
} // namespace cdot
