//
// Created by Jonas Zell on 02.07.17.
//

#ifndef CDOT_EXPORTSTMT_H
#define CDOT_EXPORTSTMT_H

#include "Statement.h"

class EndOfFileStmt : public Statement {
public:
   EndOfFileStmt() = default;

   typedef std::shared_ptr<EndOfFileStmt> SharedPtr;

   std::vector<AstNode::SharedPtr> get_children() override {
      return {};
   }

   NodeType get_type() override {
      return NodeType::EOF_STMT;
   }

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES
};


#endif //CDOT_EXPORTSTMT_H
