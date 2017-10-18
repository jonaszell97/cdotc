//
// Created by Jonas Zell on 29.06.17.
//

#ifndef CDOT_GOTOSTMT_H
#define CDOT_GOTOSTMT_H

#include "../Statement.h"

class GotoStmt : public Statement {
public:
   explicit GotoStmt(string);

   typedef std::shared_ptr<GotoStmt> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int depth) override;

   NodeType get_type() override {
      return NodeType::GOTO_STMT;
   }

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

   ADD_FRIEND_PASSES

protected:
   string labelName;
};


#endif //CDOT_GOTOSTMT_H
