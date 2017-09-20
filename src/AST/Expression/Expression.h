//
// Created by Jonas Zell on 19.06.17.
//

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <llvm/IR/GlobalVariable.h>
#include "../Statement/Statement.h"

namespace cdot {
   struct Variant;
}

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

   virtual void isFunctionArgument() {
      isFunctionArgument_ = true;
      if (memberExpr != nullptr) {
         memberExpr->isFunctionArgument();
      }
   }

   virtual void needsProtocolExtraction(bool p) {
      auto current = this;
      while (current->memberExpr != nullptr) {
         current = current->memberExpr.get();
      }

      current->needsProtocolExtraction_ = p;
   }

   void isReturnValue() override;

   virtual void isPartOfReturnValue(bool ret, bool initial = true) {
      auto current = this;
      while (current->memberExpr != nullptr) {
         current = current->memberExpr.get();
      }

      current->isPartOfReturnValue_ = ret;

      if (!initial) {
         return;
      }
      for (const auto& child : get_children()) {
         if (child == memberExpr) {
            continue;
         }
         if (auto expr = dynamic_cast<Expression*>(child.get())) {
            expr->isPartOfReturnValue(ret, false);
         }
      }
   }

   void isEnumCase_(bool b) {
      isEnumCase = b;
   }

   bool needsContextualInformation() override {
      return isEnumCase;
   }

   virtual bool canReturn(Type *ty) {
      return false;
   }

   void isHiddenReturnValue() override;

   NodeType get_type() override {
      return NodeType::EXPRESSION;
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
   Expression::SharedPtr memberExpr;
   Expression* parentExpr = nullptr;
   bool lvalue = false;
   string ident;

   bool needsCast = false;
   Type* castFrom = nullptr;
   Type* castTo = nullptr;
   bool castHandled = false;

   bool lvalueCast = false;
   bool needsProtocolExtraction_ = false;
   bool loadBeforeExtract = false;
   bool needsByValPass = false;

   bool isFunctionArgument_ = false;
   bool isLhsOfAssigment_ = false;
   bool isPartOfReturnValue_ = false;
   bool isSetterCall = false;
   string setterName;

   Variant staticVal;

   // codegen
   llvm::GlobalVariable* globalVar = nullptr;
   bool isEnumCase = false;
   long caseVal;
};


#endif //EXPRESSION_H
