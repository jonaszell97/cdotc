//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_LITERALEXPRESSION_H
#define CDOT_LITERALEXPRESSION_H

#include "../Expression.h"
#include "../../../Variant/Variant.h"

class NumericLiteral : public Expression {
public:
   NumericLiteral(cdot::Variant);

   bool needsContextualInformation() override {
      return true;
   }

   typedef std::shared_ptr<NumericLiteral> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int) override;

   NodeType get_type() override {
      return NodeType::LITERAL_EXPR;
   }

   llvm::Value* accept(CodeGen& v) override{
      return v.visit(this);
   }

   Type accept(SemaPass& v) override {
      return v.visit(this);
   }

   Variant accept(ConstExprPass& v) override {
      return v.visit(this);
   }

   void accept(AbstractPass *v) override
   {
      v->visit(this);
   }

   bool createsTemporary() override
   {
      return true;
   }

   ADD_FRIEND_PASSES

protected:
   cdot::Variant value;
   BuiltinType* type = nullptr;
   string className;

   bool is_char = false;
   bool is_bool = false;

   // codegen
   bool primitive = false;

public:
   const Variant &getValue() const
   {
      return value;
   }

   void setValue(const Variant &value)
   {
      NumericLiteral::value = value;
   }

   BuiltinType *getType() const
   {
      return type;
   }

   void setType(BuiltinType *type)
   {
      NumericLiteral::type = type;
   }

   const string &getClassName() const
   {
      return className;
   }

   void setClassName(const string &className)
   {
      NumericLiteral::className = className;
   }

   bool isChar() const
   {
      return is_char;
   }

   void isChar(bool is_char)
   {
      NumericLiteral::is_char = is_char;
   }

   bool isBool() const
   {
      return is_bool;
   }

   void isBool(bool is_bool)
   {
      NumericLiteral::is_bool = is_bool;
   }

   bool isPrimitive() const
   {
      return primitive;
   }

   void isPrimitive(bool primitive)
   {
      NumericLiteral::primitive = primitive;
   }
};


#endif //CDOT_LITERALEXPRESSION_H
