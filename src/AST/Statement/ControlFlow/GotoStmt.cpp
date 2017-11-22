//
// Created by Jonas Zell on 29.06.17.
//

#include "GotoStmt.h"

namespace cdot {
namespace ast {

GotoStmt::GotoStmt(std::string &&name)
   : Statement(GotoStmtID), labelName(move(name))
{

}

const string &GotoStmt::getLabelName() const
{
   return labelName;
}

} // namespace ast
} // namespace cdot