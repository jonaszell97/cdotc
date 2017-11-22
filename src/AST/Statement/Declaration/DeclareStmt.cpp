//
// Created by Jonas Zell on 13.08.17.
//

#include "DeclareStmt.h"
#include "../../Expression/TypeRef.h"
#include "FuncArgDecl.h"

namespace cdot {
namespace ast {

DeclareStmt::DeclareStmt(ExternKind kind) : Statement(DeclareStmtID)
{
   externKind = kind;
}

void DeclareStmt::addDeclaration(Statement::SharedPtr &&stmt)
{
   stmt->setExternKind(externKind);
   stmt->isDeclaration(true);

   declarations.push_back(move(stmt));
}

} // namespace ast
} // namespace cdot