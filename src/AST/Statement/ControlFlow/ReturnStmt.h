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

   typedef std::shared_ptr<ReturnStmt> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int depth) override;

   NodeType get_type() override {
      return NodeType::RETURN_STMT;
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

protected:
   std::shared_ptr<Expression> returnValue;

   bool interfaceShift = false;
   string originTy;

   bool hiddenParamReturn = false;

   // codegen
   Type returnType;

public:
   std::shared_ptr<Expression> &getReturnValue();
   void setReturnValue(const std::shared_ptr<Expression> &returnValue);

   bool isHiddenParamReturn() const;
   void setHiddenParamReturn(bool hiddenParamReturn);

   const Type &getReturnType() const;
   void setReturnType(const Type &returnType);
};


#endif //CDOT_RETURNSTATEMENT_H
