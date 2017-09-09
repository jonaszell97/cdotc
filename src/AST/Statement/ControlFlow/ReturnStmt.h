//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_RETURNSTATEMENT_H
#define CDOT_RETURNSTATEMENT_H

#include "../Statement.h"

class Expression;

class ReturnStmt : public Statement {
public:
   ReturnStmt();
   explicit ReturnStmt(std::shared_ptr<Expression>);

   ~ReturnStmt() override;

   typedef std::shared_ptr<ReturnStmt> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int depth) override;

   NodeType get_type() override {
      return NodeType::RETURN_STMT;
   }

   llvm::Value* accept(CodeGen& v) override {
      return v.visit(this);
   }

   Type* accept(TypeCheckPass& v) override {
      return v.visit(this);
   }

   Variant accept(ConstExprPass& v) override {
      return v.visit(this);
   }

   friend class ConstExprPass;
   friend class CodeGen;
   friend class TypeCheckPass;
   friend class DeclPass;
protected:
   std::shared_ptr<Expression> returnValue;

   bool interfaceShift = false;
   string originTy;

   bool hiddenParamReturn = false;

   // codegen
   Type* returnType = nullptr;
};


#endif //CDOT_RETURNSTATEMENT_H
