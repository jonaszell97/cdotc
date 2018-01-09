//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_COMPOUNDSTATEMENT_H
#define CDOT_COMPOUNDSTATEMENT_H

#include "../Statement.h"

namespace cdot {
namespace ast {

class CompoundStmt : public Statement {
public:
   explicit CompoundStmt(bool preserveScope = false);
   void addStatement(Statement::SharedPtr &&stmt);

   bool preservesScope() const
   {
      return preserveScope;
   }
   
   Statement::SharedPtr& at(size_t i)
   {
      return statements.at(i);
   }
   
   size_t size()
   {
      return statements.size();
   }

   std::vector<Statement::SharedPtr>& getStatements()
   {
      return statements;
   }

   std::vector<Statement::SharedPtr> const& getStatements() const
   {
      return statements;
   }

   void clear()
   {
      statements.clear();
   }

   typedef std::shared_ptr<CompoundStmt> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == CompoundStmtID;
   }

protected:
   std::vector<Statement::SharedPtr> statements;
   bool preserveScope = false;
};

} // namespace ast
} // namespace cdot

#endif //CDOT_COMPOUNDSTATEMENT_H
