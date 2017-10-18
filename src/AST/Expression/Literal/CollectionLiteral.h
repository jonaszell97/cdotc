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
      std::vector<Expression::SharedPtr> keys,
      std::vector<Expression::SharedPtr> values
   );
   explicit CollectionLiteral(
      std::vector<Expression::SharedPtr> values
   );

   void addElement(Expression::SharedPtr value);
   void addElement(Expression::SharedPtr key, Expression::SharedPtr value);

   typedef std::shared_ptr<CollectionLiteral> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int) override;

   NodeType get_type() override {
      return NodeType::COLLECTION_LITERAL;
   }

   llvm::Value* accept(CodeGen& v) override {
      return v.visit(this);
   }

   Type accept(TypeCheckPass& v) override {
      return v.visit(this);
   }

   void accept(AbstractPass* v) override {
      v->visit(this);
   }

   Variant accept(ConstExprPass &v) override {
      return v.visit(this);
   }

   bool createsTemporary() override
   {
      return true;
   }

   void replaceChildWith(AstNode *child, Expression *replacement) override;

   ADD_FRIEND_PASSES

protected:
   std::vector<Expression::SharedPtr> keys;
   std::vector<Expression::SharedPtr> values;
   std::shared_ptr<TypeRef> type;

   bool isDictionary = false;
};


#endif //CDOT_ARRAYLITERAL_H
