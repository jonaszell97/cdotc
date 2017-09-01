//
// Created by Jonas Zell on 21.06.17.
//

#include "CallExpr.h"

CallExpr::CallExpr(CallType type, std::vector<pair<string, Expression::SharedPtr>> args, std::string _ident) :
   type(type),
   args(args)
{
   ident = _ident;
   for (auto& arg : this->args) {
      children.push_back(&arg.second);
   }
}

CallExpr::CallExpr(CallType type, std::vector<Expression::SharedPtr> args, string _ident) :
   type(type)
{
   ident = _ident;
   for (auto& arg : args) {
      this->args.emplace_back("", arg);
      children.push_back(&this->args.back().second);
   }
}

CallExpr::~CallExpr() {
   delete returnType;
   delete genericDestTy;
   delete genericOriginTy;

   for (const auto& gen : generics) {
      delete gen;
   }
}

std::vector<AstNode::SharedPtr> CallExpr::get_children() {
   std::vector<AstNode::SharedPtr> res;
   for (auto arg : args) {
      res.push_back(arg.second);
   }
   if (memberExpr != nullptr) {
      res.push_back(memberExpr);
   }

   return res;
}

void CallExpr::__dump(int depth) {
   AstNode::__tab(depth);
   std::cout << "CallExpr" << std::endl;

   for (auto c : get_children()) {
      c->__dump(depth + 1);
   }
}