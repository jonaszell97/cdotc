//
// Created by Jonas Zell on 28.06.17.
//

#ifndef CDOT_LABELSTMT_H
#define CDOT_LABELSTMT_H

#include "../Statement.h"

class LabelStmt : public Statement {
public:
   explicit LabelStmt(string);

   typedef std::shared_ptr<LabelStmt> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;

   NodeType get_type() override {
      return NodeType::LABEL_STMT;
   }

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES

protected:
   string labelName;
};


#endif //CDOT_LABELSTMT_H
