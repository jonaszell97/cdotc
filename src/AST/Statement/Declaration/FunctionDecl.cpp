//
// Created by Jonas Zell on 20.06.17.
//

#include "FunctionDecl.h"
#include "FuncArgDecl.h"
#include "../../Expression/TypeRef.h"
#include "../Block/CompoundStmt.h"

FunctionDecl::FunctionDecl(std::string func_name, TypeRef::SharedPtr type) :
   funcName(func_name),
   returnType(type),
   args(std::vector<FuncArgDecl::SharedPtr>())
{

}

FunctionDecl::~FunctionDecl() {
   for (const auto& gen : generics) {
      delete gen;
   }
}

std::vector<AstNode::SharedPtr> FunctionDecl::get_children() {
   std::vector<AstNode::SharedPtr> res;
   if (body != nullptr) {
      res.push_back(body);
   }
   for (auto arg : args) {
      res.push_back(arg);
   }

   return res;
}

void FunctionDecl::__dump(int depth) {
   AstNode::__tab(depth);
   std::cout << "FunctionDecl ["<< funcName << " => " << returnType->toString()  << "]" << std::endl;

   for (auto arg : args) {
      arg->__dump(depth + 1);
   }

   body->__dump(depth + 1);
}

