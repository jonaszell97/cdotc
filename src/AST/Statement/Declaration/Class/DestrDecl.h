//
// Created by Jonas Zell on 02.09.17.
//

#ifndef CDOT_DESTRDECL_H
#define CDOT_DESTRDECL_H


#include "../../Statement.h"

class CompoundStmt;

namespace cdot {
   namespace cl {
      struct Method;
   }
}

class DestrDecl: public Statement {
public:
   DestrDecl(std::shared_ptr<CompoundStmt> &&body);

   typedef std::shared_ptr<DestrDecl> SharedPtr;
   typedef std::unique_ptr<DestrDecl> UniquePtr;

   std::vector<std::shared_ptr<AstNode>> get_children() override;
   void __dump(int depth) override;

   NodeType get_type() override {
      return NodeType::DESTR_DECL;
   }

   llvm::Value* accept(CodeGen& v) override {
      return v.visit(this);
   }

   Type* accept(TypeCheckPass& v) override {
      return v.visit(this);
   }

   void accept(AbstractPass* v) override {
      v->visit(this);
   }

   Variant accept(ConstExprPass& v) override {
      return v.visit(this);
   }

   ADD_FRIEND_PASSES

protected:
   std::shared_ptr<CompoundStmt> body;

   // codegen
   cdot::cl::Method* declaredMethod;
   string selfBinding;
   string className;
};


#endif //CDOT_DESTRDECL_H
