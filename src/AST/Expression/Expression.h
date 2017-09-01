//
// Created by Jonas Zell on 19.06.17.
//

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <llvm/IR/GlobalVariable.h>
#include "../Statement/Statement.h"

class Expression : public Statement {
public:
   typedef std::shared_ptr<Expression> SharedPtr;
   typedef std::unique_ptr<Expression> UniquePtr;

   inline void setParent(AstNode* p) {
      parent = p;
   }

   virtual inline void setMemberExpr(std::shared_ptr<Expression> ref_expr) {
      if (ref_expr == nullptr) {
        return;
      }
      
      if (memberExpr == nullptr) {
         memberExpr = ref_expr;
         children.push_back(&memberExpr);
         memberExpr->parent = this;
         memberExpr->parentExpr = this;
      }
      else {
         memberExpr->setMemberExpr(ref_expr);
      }
   }

   virtual inline void setGlobalVar(llvm::GlobalVariable* glob) {
      globalVar = glob;
      if (memberExpr != nullptr) {
         memberExpr->setGlobalVar(glob);
      }

      isGlobal(true);
      heapAllocate();
   }

   inline void isGlobal(bool gl) {
      isGlobal_ = gl;
      if (memberExpr != nullptr) {
         memberExpr->isGlobal(gl);
      }
   }

   virtual void isLhsOfAssigment() {
      isLhsOfAssigment_ = true;
      if (memberExpr != nullptr) {
         memberExpr->isLhsOfAssigment();
      }
   }

   void isEnumCase_(bool b) {
      isEnumCase = b;
   }

   bool needsContextualInformation() override {
      return isEnumCase;
   }

   void isHiddenReturnValue() override;

   NodeType get_type() override {
      return NodeType::EXPRESSION;
   }

   llvm::Value* accept(CodeGenVisitor& v) override {
      return v.visit(this);
   }

   Type* accept(TypeCheckVisitor& v) override {
      return v.visit(this);
   }

   friend class ConstExprVisitor;
   friend class CodeGenVisitor;
   friend class TypeCheckVisitor;

protected:
   Expression::SharedPtr memberExpr;
   Expression* parentExpr = nullptr;
   bool lvalue = false;
   string ident;

   bool needsCast = false;
   Type* castFrom = nullptr;
   Type* castTo = nullptr;
   bool castHandled = false;

   bool lvalueCast = false;
   bool needsLvalueToRvalueConversion = false;
   bool needsByValPass = false;

   bool isLhsOfAssigment_ = false;
   bool isSetterCall = false;
   string setterName;

   // codegen
   llvm::GlobalVariable* globalVar = nullptr;
   bool isEnumCase = false;
   long caseVal;
};


#endif //EXPRESSION_H
