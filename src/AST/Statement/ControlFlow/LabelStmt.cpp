//
// Created by Jonas Zell on 28.06.17.
//

#include "LabelStmt.h"

namespace cdot {
namespace ast {

LabelStmt::LabelStmt(std::string &&name)
   : Statement(LabelStmtID), labelName(move(name))
{

}

const string &LabelStmt::getLabelName() const
{
   return labelName;
}

} // namespace ast
} // namespace cdot