//
// Created by Jonas Zell on 10.09.17.
//

#include "StringInterpolation.h"

StringInterpolation::StringInterpolation(std::vector<Expression::SharedPtr>&& strings) :
   strings(strings)
{

}

std::vector<AstNode::SharedPtr> StringInterpolation::get_children() {
   std::vector<AstNode::SharedPtr> children;
   for (const auto& expr : strings) {
      children.push_back(expr);
   }

   return children;
}

void StringInterpolation::__dump(int depth) {

}