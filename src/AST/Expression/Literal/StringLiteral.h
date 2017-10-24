//
// Created by Jonas Zell on 30.06.17.
//

#ifndef CDOT_STRINGLITERAL_H
#define CDOT_STRINGLITERAL_H

#include "../Expression.h"

class StringLiteral : public Expression {
public:
   explicit StringLiteral(string);

   bool needsContextualInformation() override {
      return true;
   }

   typedef std::shared_ptr<StringLiteral> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int) override;

   llvm::Value* accept(CodeGen& v) override {
      return v.visit(this);
   }

   Type accept(SemaPass& v) override {
      return v.visit(this);
   }

   void accept(AbstractPass* v) override {
      v->visit(this);
   }

   Variant accept(ConstExprPass &v) override {
      return v.visit(this);
   }

   NodeType get_type() override {
      return NodeType::STRING_LITERAL;
   }

   bool createsTemporary() override
   {
      return true;
   }

   ADD_FRIEND_PASSES

protected:
   string value;

   bool raw = false;

public:
   const string &getValue() const
   {
      return value;
   }

   void setValue(const string &value)
   {
      StringLiteral::value = value;
   }

   bool isRaw() const
   {
      return raw;
   }

   void setRaw(bool raw)
   {
      StringLiteral::raw = raw;
   }
};


#endif //CDOT_STRINGLITERAL_H
