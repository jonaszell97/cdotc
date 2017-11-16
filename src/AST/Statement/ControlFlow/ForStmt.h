//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_FORSTMT_H
#define CDOT_FORSTMT_H

#include "../Statement.h"

class IdentifierRefExpr;

class ForStmt : public Statement {
public:
   ForStmt(Statement::SharedPtr, std::shared_ptr<Expression>, Statement::SharedPtr);

   void setBody(Statement::SharedPtr _body) {
      body = _body;
   }

   typedef std::shared_ptr<ForStmt> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;

   NodeType get_type() override {
      return NodeType::FOR_STMT;
   }

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES

protected:
   std::shared_ptr<Statement> initialization;
   std::shared_ptr<Expression> termination;
   Statement::SharedPtr increment;
   Statement::SharedPtr body;
};


#endif //CDOT_FORSTMT_H
