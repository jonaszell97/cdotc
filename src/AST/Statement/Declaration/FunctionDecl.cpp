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

FunctionDecl::FunctionDecl(
   string &&funcName,
   std::shared_ptr<TypeRef> &&returnType,
   std::vector<std::shared_ptr<FuncArgDecl>> &&args,
   std::shared_ptr<CompoundStmt> &&body,
   std::vector<GenericConstraint> &&generics,
   bool has_sret) : funcName(funcName),
                    returnType(returnType),
                    args(args),
                    body(body),
                    generics(generics),
                    has_sret(has_sret)
{

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

