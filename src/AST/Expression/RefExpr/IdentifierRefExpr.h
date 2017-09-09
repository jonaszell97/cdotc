//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_IDENTIFIEREXPRESSION_H
#define CDOT_IDENTIFIEREXPRESSION_H

#include "../Expression.h"

class MemberRefExpr;

class IdentifierRefExpr : public Expression {
public:
   explicit IdentifierRefExpr(string);

   void isLetExpr(bool letExpr) {
      isLetExpr_ = letExpr;
   }

   void isVarExpr(bool varExpr) {
      isVarExpr_ = varExpr;
   }

   bool isUnderscore() override {
      return ident == "_";
   }

   typedef std::shared_ptr<IdentifierRefExpr> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;

   void __dump(int) override ;

   NodeType get_type() override {
      return NodeType::IDENTIFIER_EXPR;
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
   // codegen
   bool isNonMutableArg = false;

   bool isCapturedVar = false;
   Type* capturedType; // unowned

   bool isLetExpr_ = false;
   bool isVarExpr_ = false;

   bool isNamespace = false;
   bool isSuper = false;
   bool isFunction = false;
   string superClassName;
};


#endif //CDOT_IDENTIFIEREXPRESSION_H
