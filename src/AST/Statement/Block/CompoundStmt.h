//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_COMPOUNDSTATEMENT_H
#define CDOT_COMPOUNDSTATEMENT_H

#include "../Statement.h"

class CompoundStmt : public Statement {
public:
   explicit CompoundStmt(bool preserveScope = false);

   void addStatement(Statement::SharedPtr stmt);

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

   void replaceChildWith(AstNode *child, Expression *replacement) override;

   typedef std::shared_ptr<CompoundStmt> SharedPtr;
   typedef std::unique_ptr<CompoundStmt> UniquePtr;
   
   std::vector<AstNode::SharedPtr> get_children() override;

   NodeType get_type() override {
      return NodeType::COMPOUND_STMT;
   }

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES

protected:
   std::vector<Statement::SharedPtr> statements;
   bool preserveScope = false;

   std::vector<AbstractPass*> passes;

   bool isUnsafe_ = false;
};


#endif //CDOT_COMPOUNDSTATEMENT_H
