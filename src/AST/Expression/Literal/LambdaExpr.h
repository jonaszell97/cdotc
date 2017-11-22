//
// Created by Jonas Zell on 01.07.17.
//

#ifndef CDOT_LAMBDAEXPR_H
#define CDOT_LAMBDAEXPR_H

#include <set>
#include "../Expression.h"

namespace cdot {

class FunctionType;

namespace ast {

class FuncArgDecl;
class TypeRef;

class LambdaExpr : public Expression {
public:
   LambdaExpr(std::shared_ptr<TypeRef> &&returnType,
              std::vector<std::shared_ptr<FuncArgDecl>> &&args,
              Statement::SharedPtr &&body);

   typedef std::shared_ptr<LambdaExpr> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == LambdaExprID;
   }

protected:
   std::shared_ptr<TypeRef> returnType;
   std::vector<std::shared_ptr<FuncArgDecl>> args;
   Statement::SharedPtr body;

   std::set<std::string> captures;

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

   std::set<string> &getCaptures()
   {
      return captures;
   }

   void setCaptures(const std::set<string> &captures)
   {
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

} // namespace ast
} // namespace cdot

#endif //CDOT_LAMBDAEXPR_H
