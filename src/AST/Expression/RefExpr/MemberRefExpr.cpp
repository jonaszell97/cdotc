//
// Created by Jonas Zell on 20.06.17.
//

#include "MemberRefExpr.h"
#include "../../SymbolTable.h"
#include "../../Passes/SemanticAnalysis/Record/Class.h"
#include "../../Passes/SemanticAnalysis/Record/Enum.h"

MemberRefExpr::MemberRefExpr(string ident, bool pointerAccess) : isPointerAccess(pointerAccess) {
   this->ident = ident;
}

MemberRefExpr::MemberRefExpr(size_t index, bool pointerAccess) :
   tupleIndex(index),
   isTupleAccess(true),
   isPointerAccess(pointerAccess)
{

}

void MemberRefExpr::replaceChildWith(
   AstNode *child,
   Expression *replacement)
{
   if (memberExpr.get() == child) {
      memberExpr.reset(replacement);
   }

   llvm_unreachable("child does not exist");
}

std::vector<AstNode::SharedPtr> MemberRefExpr::get_children() {
   if (memberExpr != nullptr) {
      return { memberExpr };
   }
   else {
      return { };
   }
}

void MemberRefExpr::__dump(int depth) {
   AstNode::__tab(depth);
   std::cout << "MemberRefExpr [" << ident << "]\n";

   if (memberExpr != nullptr) {
      memberExpr->__dump(depth + 1);
   }
}

bool MemberRefExpr::isUnionAccess() const {
   return unionAccess;
}

void MemberRefExpr::setUnionAccess(bool unionAccess) {
   MemberRefExpr::unionAccess = unionAccess;
}
