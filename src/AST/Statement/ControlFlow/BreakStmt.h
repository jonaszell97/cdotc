//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_BREAKSTMT_H
#define CDOT_BREAKSTMT_H

#include "../Statement.h"

class BreakStmt : public Statement {
public:
   explicit BreakStmt();

   typedef std::shared_ptr<BreakStmt> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;

   NodeType get_type() override {
      return NodeType::BREAK_STMT;
   }

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES
};


#endif //CDOT_BREAKSTMT_H
