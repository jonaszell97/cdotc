//
// Created by Jonas Zell on 30.06.17.
//

#ifndef CDOT_STRINGLITERAL_H
#define CDOT_STRINGLITERAL_H

#include "../Expression.h"

class StringLiteral : public Expression {
public:
   explicit StringLiteral(string, char = 'f');

   bool needsContextualInformation() override {
      return true;
   }

   bool canReturn(Type* ty) override;

   typedef std::shared_ptr<StringLiteral> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int) override;

   llvm::Value* accept(CodeGen& v) override {
      return v.visit(this);
   }

   Type* accept(TypeCheckPass& v) override {
      return v.visit(this);
   }

   void accept(DeclPass &v) override {
      v.visit(this);
   }

   Variant accept(ConstExprPass &v) override {
      return v.visit(this);
   }

   inline virtual NodeType get_type() override {
      return NodeType::STRING_LITERAL;
   }
   virtual inline char get_modifier() {
      return modifier;
   }

   friend class ConstExprPass;
   friend class CodeGen;
   friend class TypeCheckPass;
   friend class DeclPass;

protected:
   char modifier;
   string value;

   bool raw = false;
};


#endif //CDOT_STRINGLITERAL_H
