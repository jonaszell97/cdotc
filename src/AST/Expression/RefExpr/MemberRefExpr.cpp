//
// Created by Jonas Zell on 20.06.17.
//

#include "MemberRefExpr.h"
#include "../../SymbolTable.h"
#include "../../Passes/StaticAnalysis/Class.h"
#include "../../Passes/StaticAnalysis/Enum.h"

MemberRefExpr::MemberRefExpr(string ident, bool pointerAccess) : isPointerAccess(pointerAccess) {
   this->ident = ident;
}

MemberRefExpr::MemberRefExpr(size_t index, bool pointerAccess) :
   tupleIndex(index),
   isTupleAccess(true),
   isPointerAccess(pointerAccess)
{

}

MemberRefExpr::~MemberRefExpr() {
   delete genericOriginTy;
   delete genericDestTy;
   delete fieldType;
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