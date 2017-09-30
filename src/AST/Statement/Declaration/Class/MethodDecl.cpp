//
// Created by Jonas Zell on 27.06.17.
//

#include "MethodDecl.h"
#include "../../../Expression/TypeRef.h"
#include "../../Block/CompoundStmt.h"
#include "../../Declaration/FuncArgDecl.h"

MethodDecl::MethodDecl(std::string method_name, TypeRef::SharedPtr return_type, std::vector<FuncArgDecl::SharedPtr> args,
      CompoundStmt::SharedPtr body, AccessModifier am, bool is_static) :
   methodName(method_name),
   returnType(return_type),
   args(args),
   body(body),
   am(am),
   isStatic(is_static),
   isAbstract(false),
   hasDefinition_(body != nullptr)
{

}

MethodDecl::MethodDecl(std::string method_name, TypeRef::SharedPtr return_type, std::vector<FuncArgDecl::SharedPtr> args,
      AccessModifier am, bool is_static) :
   methodName(method_name),
   returnType(return_type),
   args(args),
   am(am),
   isStatic(is_static),
   isAbstract(true),
   hasDefinition_(body != nullptr)
{

}

MethodDecl::MethodDecl(string alias, string originMethod, std::vector<std::shared_ptr<FuncArgDecl>> args) :
   methodName(originMethod),
   alias(alias),
   args(args),
   isAlias(true),
   hasDefinition_(body != nullptr)
{

}

std::vector<AstNode::SharedPtr> MethodDecl::get_children() {
   std::vector<AstNode::SharedPtr> children;
   for (auto arg : args) {
      children.push_back(arg);
   }

   if (body != nullptr) {
      children.push_back(body);
   }

   return children;
}

void MethodDecl::__dump(int depth) {
   AstNode::__tab(depth);
   if (!isAlias) {
      std::cout << (isStatic ? "Static" : "") << "MethodDecl [" << (isAbstract ? "abstract " : "") << util::am_map[am]
              << " " << methodName << " => " << returnType->toString() << "]" << std::endl;
   }
   else {
      std::cout << "MethodAlias [" + methodName + " = " + alias + "]\n";
   }

   for (auto c : get_children()) {
      c->__dump(depth + 1);
   }
}