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
class FunctionDecl;

class LambdaExpr : public Expression {
public:
   LambdaExpr(TypeRef* returnType,
              std::vector<FuncArgDecl* > &&args,
              Statement* body)
      : Expression(LambdaExprID, true), returnType(returnType),
        args(move(args)), body(body)
   {

   }

   friend class TransformImpl;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == LambdaExprID;
   }

protected:
   TypeRef* returnType;
   std::vector<FuncArgDecl* > args;
   Statement* body;

   std::set<Statement*> captures;

   FunctionDecl *func;

public:
   TypeRef* getReturnType() const
   {
      return returnType;
   }

   const std::vector<FuncArgDecl* > &getArgs() const
   {
      return args;
   }

   Statement* getBody() const
   {
      return body;
   }

   std::set<Statement*> &getCaptures()
   {
      return captures;
   }

   FunctionDecl *getFunc() const
   {
      return func;
   }

   void setFunc(FunctionDecl *func)
   {
      LambdaExpr::func = func;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_LAMBDAEXPR_H
