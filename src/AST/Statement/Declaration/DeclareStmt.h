//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_DECLARESTMT_H
#define CDOT_DECLARESTMT_H


#include "../Statement.h"

namespace cdot {
namespace ast {

class FuncArgDecl;
class TypeRef;

class DeclareStmt : public Statement {
public:
   explicit DeclareStmt(ExternKind kind);
   void addDeclaration(Statement::SharedPtr&& stmt);

   typedef std::shared_ptr<DeclareStmt> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == DeclareStmtID;
   }

protected:
   std::vector<Statement::SharedPtr> declarations;

public:
   const std::vector<Statement::SharedPtr> &getDeclarations() const
   {
      return declarations;
   }

   void setDeclarations(const std::vector<Statement::SharedPtr> &declarations)
   {
      DeclareStmt::declarations = declarations;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_DECLARESTMT_H
