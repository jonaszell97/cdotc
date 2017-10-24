//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_WHILESTATEMENT_H
#define CDOT_WHILESTATEMENT_H

#include "../Statement.h"
class Expression;

class WhileStmt : public Statement {
public:
   WhileStmt(std::shared_ptr<Expression> cond, Statement::SharedPtr body, bool atLeastOnce);

   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int depth) override;

   NodeType get_type() override {
      return NodeType::WHILE_STMT;
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

   typedef std::shared_ptr<WhileStmt> SharedPtr;

   ADD_FRIEND_PASSES

protected:
   std::shared_ptr<Expression> condition;
   Statement::SharedPtr body;

   bool atLeastOnce = false;
};


#endif //CDOT_WHILESTATEMENT_H
