//
// Created by Jonas Zell on 19.06.17.
//

#ifndef STATEMENT_H
#define STATEMENT_H

#include "../AstNode.h"

enum class ExternKind : unsigned int {
   NONE,
   C,
   CPP
};

class Statement : public AstNode {
public:
   void isDeclaration(bool decl) {
      is_declaration = decl;
   }

   bool isDeclaration() {
      return is_declaration;
   }

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

   void accept(AbstractPass* v) override {
      v->visit(this);
   }

   void setExternKind(ExternKind kind) {
      externKind = kind;
   }

   ExternKind getExternKind() const {
      return externKind;
   }

   ADD_FRIEND_PASSES

protected:
   ExternKind externKind = ExternKind::NONE;
   bool is_declaration = false;
};


#endif //STATEMENT_H
