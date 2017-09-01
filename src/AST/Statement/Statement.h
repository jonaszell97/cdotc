//
// Created by Jonas Zell on 19.06.17.
//

#ifndef STATEMENT_H
#define STATEMENT_H

#include "../AstNode.h"

class Statement : public AstNode {
public:
   typedef std::shared_ptr<Statement> SharedPtr;

   llvm::Value* accept(CodeGenVisitor& v) override {
      return v.visit(this);
   }

   Type* accept(TypeCheckVisitor& v) override {
      return v.visit(this);
   }
};


#endif //STATEMENT_H
