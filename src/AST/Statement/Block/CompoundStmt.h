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

   void isUnsafe(bool unsafe)
   {
      isUnsafe_ = unsafe;
   }

   bool isUnsafe() const
   {
      return isUnsafe_;
   }

   bool preservesScope() const
   {
      return preserveScope;
   }
   
   Statement::SharedPtr& at(size_t i) {
      return statements.at(i);
   }
   
   size_t size() {
      return statements.size();
   }

   inline void addStatements(std::vector<Statement::SharedPtr> stmts) {
      statements.reserve(stmts.size());
      statements.insert(statements.end(), stmts.begin(), stmts.end());
   }
   
   inline void insertAtBegin(std::vector<Statement::SharedPtr> stmts) {
      statements.insert(statements.begin(), stmts.begin(), stmts.end());
   }

   inline std::vector<Statement::SharedPtr>& getStatements() {
      return statements;
   }

   inline void clear() {
      statements.clear();
   }

   typedef std::shared_ptr<CompoundStmt> SharedPtr;
   typedef std::unique_ptr<CompoundStmt> UniquePtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == CompoundStmtID;
   }

protected:
   std::vector<Statement::SharedPtr> statements;
   bool preserveScope = false;

   bool isUnsafe_ = false;
};

} // namespace ast
} // namespace cdot

#endif //CDOT_COMPOUNDSTATEMENT_H
