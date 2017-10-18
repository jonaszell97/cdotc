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
   void __dump(int depth) override;


   llvm::Value* accept(CodeGen& v) override {
      return v.visit(this);
   }

   Type accept(TypeCheckPass& v) override {
      return v.visit(this);
   }

   void accept(AbstractPass* v) override {
      v->visit(this);
   }

   Variant accept(ConstExprPass &v) override {
      return v.visit(this);
   }

   NodeType get_type() override {
      return NodeType::LABEL_STMT;
   }

   ADD_FRIEND_PASSES

protected:
   string labelName;
};


#endif //CDOT_LABELSTMT_H
