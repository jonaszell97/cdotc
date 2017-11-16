//
// Created by Jonas Zell on 18.08.17.
//

#ifndef CDOT_DEBUGSTMT_H
#define CDOT_DEBUGSTMT_H


#include "Statement.h"

class DebugStmt : public Statement {
public:
   DebugStmt(bool unreachable = false) : isUnreachable(unreachable) {

   }

   std::vector<AstNode::SharedPtr> get_children() override {
      return {};
   }

   NodeType get_type() override {
      return NodeType::DEBUG_STMT;
   }

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES

   typedef std::shared_ptr<DebugStmt> SharedPtr;

protected:
   bool isUnreachable;
};


#endif //CDOT_DEBUGSTMT_H
