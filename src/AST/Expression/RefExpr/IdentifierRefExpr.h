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
   ~IdentifierRefExpr() {
      delete builtinType;
   }

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

   void accept(AbstractPass* v) override {
      v->visit(this);
   }

   Variant accept(ConstExprPass &v) override {
      return v.visit(this);
   }

   ADD_FRIEND_PASSES

protected:
   // codegen
   bool isNonMutableArg = false;

   bool isCapturedVar = false;
   Type* capturedType; // unowned

   bool isLetExpr_ = false;
   bool isVarExpr_ = false;

   Variant builtinValue;
   Type* builtinType = nullptr;

   bool isNamespace = false;
   bool isSuper = false;
   bool isFunction = false;
   string superClassName;
};


#endif //CDOT_IDENTIFIEREXPRESSION_H
