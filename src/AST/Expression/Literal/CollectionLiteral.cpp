//
// Created by Jonas Zell on 21.06.17.
//

#include "CollectionLiteral.h"
#include "../TypeRef.h"

CollectionLiteral::CollectionLiteral(std::vector<Expression::SharedPtr> keys,
   std::vector<Expression::SharedPtr> values) :
   keys(keys), values(values), isDictionary(true)
{
   string name = "Dictionary";
   type = std::make_shared<TypeRef>(name, std::vector<pair<string, TypeRef::SharedPtr>>());
}

CollectionLiteral::CollectionLiteral(std::vector<Expression::SharedPtr> values) :
   values(values)
{
   string name = "Array";
   type = std::make_shared<TypeRef>(name, std::vector<pair<string, TypeRef::SharedPtr>>());
}

void CollectionLiteral::replaceChildWith(
   AstNode *child,
   Expression *replacement)
{
   for (auto &el : values) {
      if (el.get() == child) {
         el.reset(replacement);
         return;
      }
   }
   for (auto &el : keys) {
      if (el.get() == child) {
         el.reset(replacement);
         return;
      }
   }

   llvm_unreachable("child does not exist!");
}

void CollectionLiteral::addElement(Expression::SharedPtr value) {
   values.push_back(value);
   children.push_back(&values.back());
}

void CollectionLiteral::addElement(Expression::SharedPtr key, Expression::SharedPtr value) {
   values.push_back(value);
   keys.push_back(key);
   children.push_back(&values.back());
   children.push_back(&keys.back());

   isDictionary = true;
}

std::vector<AstNode::SharedPtr> CollectionLiteral::get_children() {
   std::vector<AstNode::SharedPtr> res;

   for (auto el : values) {
      res.push_back(el);
   }

   if (memberExpr != nullptr) {
      res.push_back(memberExpr);
   }

   return res;
}

void CollectionLiteral::__dump(int depth) {
   AstNode::__tab(depth);
   std::cout << "CollectionLiteral [" << type->toString() << "]" << std::endl;

   for (auto c : values) {
      c->__dump(depth + 1);
   }
}