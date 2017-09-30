//
// Created by Jonas Zell on 30.06.17.
//

#include "StringLiteral.h"

StringLiteral::StringLiteral(std::string value, char modifier) : value(value), modifier(modifier) {

}

void StringLiteral::saveOrResetState()
{
   if (prevState == nullptr) {
      prevState = new StringLiteral(*this);
   }
   else {
      assert(prevState->get_type() == NodeType::STRING_LITERAL && "Not a string literal");
      *this = *static_cast<StringLiteral *>(prevState);
   }
}

std::vector<AstNode::SharedPtr> StringLiteral::get_children() {
   return memberExpr == nullptr ? std::vector<AstNode::SharedPtr>() : std::vector<AstNode::SharedPtr>{memberExpr };
}

void StringLiteral::__dump(int depth) {
   AstNode::__tab(depth);
   std::cout << "StringLiteral [\"" << util::str_escape(value) << "\"" << std::string(1, modifier) << "]" << std::endl;

   for (auto c : get_children()) {
      c->__dump(depth + 1);
   }
}