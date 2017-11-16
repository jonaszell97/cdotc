//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_CONTINUESTMT_H
#define CDOT_CONTINUESTMT_H

#include "../Statement.h"

class ContinueStmt : public Statement {
public:
   ContinueStmt();

   typedef std::shared_ptr<ContinueStmt> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;

   NodeType get_type() override {
      return NodeType::CONTINUE_STMT;
   }

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES
};


#endif //CDOT_CONTINUESTMT_H
