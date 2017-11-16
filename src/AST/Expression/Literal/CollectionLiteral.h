//
// Created by Jonas Zell on 21.06.17.
//

#ifndef CDOT_ARRAYLITERAL_H
#define CDOT_ARRAYLITERAL_H

#include "../Expression.h"

class TypeRef;

class CollectionLiteral : public Expression {
public:
   explicit CollectionLiteral(
      std::vector<Expression::SharedPtr> &&keys,
      std::vector<Expression::SharedPtr> &&values
   );
   explicit CollectionLiteral(
      std::vector<Expression::SharedPtr> &&values
   );

   typedef std::shared_ptr<CollectionLiteral> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;

   NodeType get_type() override {
      return NodeType::COLLECTION_LITERAL;
   }

   bool createsTemporary() override
   {
      return true;
   }

   void replaceChildWith(AstNode *child, Expression *replacement) override;

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES

protected:
   std::vector<Expression::SharedPtr> keys;
   std::vector<Expression::SharedPtr> values;
   std::shared_ptr<TypeRef> type;

   bool is_dictionary = false;
   bool is_meta_ty = false;

public:
   const std::vector<std::shared_ptr<Expression>> &getKeys() const;
   void setKeys(const std::vector<std::shared_ptr<Expression>> &keys);

   const std::vector<std::shared_ptr<Expression>> &getValues() const;
   void setValues(const std::vector<std::shared_ptr<Expression>> &values);

   const std::shared_ptr<TypeRef> &getType() const;
   void setType(const std::shared_ptr<TypeRef> &type);

   bool isDictionary() const;
   void isDictionary(bool is_dictionary);

   bool isMetaTy() const;
   void isMetaTy(bool is_meta_ty);
};


#endif //CDOT_ARRAYLITERAL_H
