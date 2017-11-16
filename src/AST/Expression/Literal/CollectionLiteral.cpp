//
// Created by Jonas Zell on 21.06.17.
//

#include "CollectionLiteral.h"
#include "../TypeRef.h"

CollectionLiteral::CollectionLiteral(std::vector<Expression::SharedPtr> &&keys,
   std::vector<Expression::SharedPtr> &&values) :
   keys(keys), values(values), is_dictionary(true)
{
   type = std::make_shared<TypeRef>(
      TypeRef::NamespaceVec{ { "Dictionary", {} } }
   );
}

CollectionLiteral::CollectionLiteral(std::vector<Expression::SharedPtr> &&values) :
   values(values)
{
   type = std::make_shared<TypeRef>(
      TypeRef::NamespaceVec{ { "Array", {} } }
   );
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

const std::vector<Expression::SharedPtr> &CollectionLiteral::getKeys() const
{
   return keys;
}

void CollectionLiteral::setKeys(const std::vector<Expression::SharedPtr> &keys)
{
   CollectionLiteral::keys = keys;
}

const std::vector<Expression::SharedPtr> &CollectionLiteral::getValues() const
{
   return values;
}

void
CollectionLiteral::setValues(const std::vector<Expression::SharedPtr> &values)
{
   CollectionLiteral::values = values;
}

const std::shared_ptr<TypeRef> &CollectionLiteral::getType() const
{
   return type;
}

void CollectionLiteral::setType(const std::shared_ptr<TypeRef> &type)
{
   CollectionLiteral::type = type;
}

bool CollectionLiteral::isDictionary() const
{
   return is_dictionary;
}

void CollectionLiteral::isDictionary(bool is_dictionary)
{
   CollectionLiteral::is_dictionary = is_dictionary;
}

bool CollectionLiteral::isMetaTy() const
{
   return is_meta_ty;
}

void CollectionLiteral::isMetaTy(bool is_meta_ty)
{
   CollectionLiteral::is_meta_ty = is_meta_ty;
}
