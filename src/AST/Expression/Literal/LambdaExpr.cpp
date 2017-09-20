//
// Created by Jonas Zell on 01.07.17.
//

#include "LambdaExpr.h"
#include "../../Statement/Declaration/FuncArgDecl.h"
#include "../../Statement/ControlFlow/ReturnStmt.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../TypeRef.h"

LambdaExpr::LambdaExpr(std::shared_ptr<TypeRef> return_type, std::vector<std::shared_ptr<FuncArgDecl>> args,
       Statement::SharedPtr body) :
   returnType(return_type),
   args(args),
   body(body)
{

}

LambdaExpr::~LambdaExpr() {
   delete lambdaType;
}

bool LambdaExpr::canReturn(Type *ty)
{
   if (!ty->isFunctionTy()) {
      return false;
   }

   auto fun = cast<FunctionType>(ty);
   auto& otherArgs = fun->getArgTypes();

   if (otherArgs.size() != otherArgs.size()) {
      return false;
   }

   size_t i = 0;
   for (const auto& arg : args) {
      if (!arg->getArgType()->getType(true)->implicitlyCastableTo(otherArgs[i])) {
         return false;
      }
   }

   return returnType->getType(true)->implicitlyCastableTo(fun->getReturnType());
}

std::vector<AstNode::SharedPtr> LambdaExpr::get_children() {
   std::vector<AstNode::SharedPtr> children;
   for (const auto& arg : args) {
      children.push_back(arg);
   }
   if (memberExpr != nullptr) {
      children.push_back(memberExpr);
   }

   children.push_back(body);
   return children;
}

void LambdaExpr::__dump(int depth) {
   AstNode::__tab(depth);
   std::cout << "LambdaExpr\n";

   for (const auto& child : get_children()) {
      child->__dump(depth + 1);
   }
}