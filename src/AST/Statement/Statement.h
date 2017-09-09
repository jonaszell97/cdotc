//
// Created by Jonas Zell on 19.06.17.
//

#ifndef STATEMENT_H
#define STATEMENT_H

#include "../AstNode.h"

class Statement : public AstNode {
public:
   typedef std::shared_ptr<Statement> SharedPtr;

   llvm::Value* accept(CodeGen& v) override {
      return v.visit(this);
   }

   Type* accept(TypeCheckPass& v) override {
      return v.visit(this);
   }

   Variant accept(ConstExprPass& v) override {
      return v.visit(this);
   }

   void accept(DeclPass& v) override {
      v.visit(this);
   }
};


#endif //STATEMENT_H
