//
// Created by Jonas Zell on 28.08.17.
//

#include "TupleLiteral.h"
#include "../../../Variant/Type/TupleType.h"

TupleLiteral::TupleLiteral(
   std::vector<pair<string, Expression::SharedPtr>> elements)
   : elements(elements)
{

}

void TupleLiteral::replaceChildWith(
   AstNode *child,
   Expression *replacement)
{
   for (auto &el : elements) {
      if (el.second.get() == child) {
         el.second.reset(replacement);
         return;
      }
   }

   llvm_unreachable("child does not exist");
}

std::vector<AstNode::SharedPtr> TupleLiteral::get_children() {
   std::vector<AstNode::SharedPtr> children;
   for (auto& cont : elements) {
      children.push_back(cont.second);
   }
   if (memberExpr != nullptr) {
      children.push_back(memberExpr);
   }

   return children;
}

bool TupleLiteral::isMetaTy() const
{
   return is_meta_ty;
}

void TupleLiteral::isMetaTy(bool is_meta_ty)
{
   TupleLiteral::is_meta_ty = is_meta_ty;
}

const std::vector<pair<string, Expression::SharedPtr>> &
TupleLiteral::getElements() const
{
   return elements;
}

void TupleLiteral::setElements(
   const std::vector<pair<string, Expression::SharedPtr>> &elements)
{
   TupleLiteral::elements = elements;
}

TupleType *TupleLiteral::getTupleType() const
{
   return tupleType;
}

void TupleLiteral::setTupleType(TupleType *tupleType)
{
   TupleLiteral::tupleType = tupleType;
}
