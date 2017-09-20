//
// Created by Jonas Zell on 27.06.17.
//

#include "ConstrDecl.h"
#include "../../Block/CompoundStmt.h"
#include "../FuncArgDecl.h"

ConstrDecl::ConstrDecl() : args{}, body{}, memberwise(true), am(AccessModifier::PUBLIC) {

}

ConstrDecl::ConstrDecl(std::vector<std::shared_ptr<FuncArgDecl>> args, CompoundStmt::SharedPtr body, AccessModifier am) :
      args(args), body(body), am(am) {

}

std::vector<std::shared_ptr<AstNode>> ConstrDecl::get_children() {
   if (memberwise) {
      return {};
   }

   std::vector<std::shared_ptr<AstNode>> children;
   for (auto arg : args) {
      children.push_back(arg);
   }

   if (body != nullptr) {
      children.push_back(body);
   }

   return children;
}

void ConstrDecl::__dump(int depth) {
   AstNode::__tab(depth);
   std::cout << (memberwise ? "Implicit" : "") << "ConstrDecl" << std::endl;

   for (auto c : get_children()) {
      c->__dump(depth + 1);
   }
}