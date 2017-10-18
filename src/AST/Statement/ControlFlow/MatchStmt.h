//
// Created by Jonas Zell on 28.06.17.
//

#ifndef CDOT_SWITCHSTMT_H
#define CDOT_SWITCHSTMT_H

#include "../Statement.h"

class Expression;
class CaseStmt;

class MatchStmt : public Statement {
public:
   MatchStmt(std::shared_ptr<Expression> switchVal);

   inline void addCase(std::shared_ptr<CaseStmt> case_) {
      cases.push_back(case_);
   }

   typedef std::shared_ptr<MatchStmt> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int depth) override;

   NodeType get_type() override {
      return NodeType::SWITCH_STMT;
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
   std::shared_ptr<Expression> switchValue;
   BuiltinType *switchType = nullptr;
   std::vector<std::shared_ptr<CaseStmt>> cases;

   Method* operatorEquals = nullptr;

   // codegen
   bool hasDefault = false;
   unsigned defaultIndex = -1;
   
   bool isIntegralSwitch = false;
   bool allCasesReturn;
};


#endif //CDOT_SWITCHSTMT_H
