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
class Function;

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

   std::set<Statement*> captures;

   ast::Function *func;

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

   std::set<Statement*> &getCaptures()
   {
      return captures;
   }

   void setCaptures(std::set<Statement*> &&captures)
   {
      LambdaExpr::captures = captures;
   }

   Function *getFunc() const
   {
      return func;
   }

   void setFunc(Function *func)
   {
      LambdaExpr::func = func;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_LAMBDAEXPR_H
