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
   void __dump(int depth) override ;

   NodeType get_type() override {
      return NodeType::BREAK_STMT;
   }

   llvm::Value* accept(CodeGen& v) override {
      return v.visit(this);
   }

   Type accept(SemaPass& v) override {
      return v.visit(this);
   }

   void accept(AbstractPass* v) override {
      v->visit(this);
   }

   Variant accept(ConstExprPass &v) override {
      return v.visit(this);
   }

   ADD_FRIEND_PASSES
};


#endif //CDOT_BREAKSTMT_H
