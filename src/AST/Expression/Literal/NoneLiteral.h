//
// Created by Jonas Zell on 30.08.17.
//

#ifndef CDOT_NONELITERAL_H
#define CDOT_NONELITERAL_H


#include "../Expression.h"

class NoneLiteral: public Expression {
public:
   NoneLiteral();

   bool needsContextualInformation() override {
      return true;
   }

   typedef std::shared_ptr<NoneLiteral> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int depth) override;

   NodeType get_type() override {
      return NodeType::NONE_LITERAL;
   }

   llvm::Value* accept(CodeGen& v) override{
      return v.visit(this);
   }

   Type* accept(TypeCheckPass& v) override {
      return v.visit(this);
   }

   Variant accept(ConstExprPass& v) override {
      return v.visit(this);
   }

   friend class ConstExprPass;
   friend class CodeGen;
   friend class TypeCheckPass;
   friend class DeclPass;
};


#endif //CDOT_NONELITERAL_H
