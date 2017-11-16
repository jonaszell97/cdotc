//
// Created by Jonas Zell on 27.06.17.
//

#include "ConstrDecl.h"
#include "../../Block/CompoundStmt.h"
#include "../FuncArgDecl.h"
#include "../../../../Util.h"

ConstrDecl::ConstrDecl()
   : CallableDecl(AccessModifier::PUBLIC, {}, {}, {}), memberwise(true)
{

}

ConstrDecl::ConstrDecl(
   std::vector<std::shared_ptr<FuncArgDecl>> &&args,
   AccessModifier am) : CallableDecl(am, {}, {}, std::move(args))
{

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

cl::Record *ConstrDecl::getRecord() const
{
   return record;
}

void ConstrDecl::setRecord(cl::Record *record)
{
   ConstrDecl::record = record;
}
