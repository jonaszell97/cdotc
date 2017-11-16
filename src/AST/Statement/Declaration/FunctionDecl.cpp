//
// Created by Jonas Zell on 20.06.17.
//

#include "FunctionDecl.h"
#include "FuncArgDecl.h"
#include "../../Expression/TypeRef.h"
#include "../Block/CompoundStmt.h"

FunctionDecl::FunctionDecl(
   AccessModifier am, string &&func_name, TypeRef::SharedPtr type)
   : CallableDecl(am, std::move(func_name), std::move(type), {})
{

}

FunctionDecl::FunctionDecl(
   AccessModifier am,
   string &&funcName,
   std::shared_ptr<TypeRef> &&returnType,
   std::vector<std::shared_ptr<FuncArgDecl>> &&args,
   std::shared_ptr<CompoundStmt> &&body,
   bool has_sret) : CallableDecl(am, std::move(funcName), std::move(returnType),
                                 std::move(args))
{
   this->body = body;
   this->has_sret = has_sret;
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