//
// Created by Jonas Zell on 01.07.17.
//

#ifndef CDOT_LAMBDAEXPR_H
#define CDOT_LAMBDAEXPR_H

#include "../Expression.h"

class FuncArgDecl;
class TypeRef;

namespace cdot {
   class FunctionType;
}

class LambdaExpr : public Expression {
public:
   LambdaExpr(std::shared_ptr<TypeRef>, std::vector<std::shared_ptr<FuncArgDecl>>, Statement::SharedPtr);

   bool needsContextualInformation() override {
      return true;
   }

   typedef std::shared_ptr<LambdaExpr> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int) override;

   NodeType get_type() override {
      return NodeType::LAMBDA_EXPR;
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
   std::shared_ptr<TypeRef> returnType;
   std::vector<std::shared_ptr<FuncArgDecl>> args;
   Statement::SharedPtr body;
   std::vector<pair<string, Type>> captures;

   // codegen
   FunctionType* lambdaType = nullptr;
   llvm::Function* lambdaFunc = nullptr;

public:
   const std::shared_ptr<TypeRef> &getReturnType() const {
      return returnType;
   }

   void setReturnType(const std::shared_ptr<TypeRef> &returnType) {
      LambdaExpr::returnType = returnType;
   }

   const std::vector<std::shared_ptr<FuncArgDecl>> &getArgs() const {
      return args;
   }

   void setArgs(const std::vector<std::shared_ptr<FuncArgDecl>> &args) {
      LambdaExpr::args = args;
   }

   const Statement::SharedPtr &getBody() const {
      return body;
   }

   void setBody(const Statement::SharedPtr &body) {
      LambdaExpr::body = body;
   }

   const std::vector<pair<string, Type>> &getCaptures() const {
      return captures;
   }

   void setCaptures(const std::vector<pair<string, Type>> &captures) {
      LambdaExpr::captures = captures;
   }

   FunctionType *getLambdaType() const {
      return lambdaType;
   }

   void setLambdaType(FunctionType *lambdaType) {
      LambdaExpr::lambdaType = lambdaType;
   }

   llvm::Function *getLambdaFunc() const {
      return lambdaFunc;
   }

   void setLambdaFunc(llvm::Function *lambdaFunc) {
      LambdaExpr::lambdaFunc = lambdaFunc;
   }
};


#endif //CDOT_LAMBDAEXPR_H
