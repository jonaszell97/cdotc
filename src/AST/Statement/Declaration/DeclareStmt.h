//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_DECLARESTMT_H
#define CDOT_DECLARESTMT_H


#include "../Statement.h"

class FuncArgDecl;
class TypeRef;

class DeclareStmt : public Statement {
public:
   DeclareStmt(ExternKind kind);

   void addDeclaration(Statement::SharedPtr&& stmt) {
      stmt->setExternKind(externKind);
      declarations.push_back(stmt);
   }

   std::vector<AstNode::SharedPtr> get_children() override;

   typedef std::shared_ptr<DeclareStmt> SharedPtr;
   void __dump(int depth) override;

   NodeType get_type() override {
      return NodeType::DECLARE_STMT;
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

   Variant accept(ConstExprPass &v) override {
      return v.visit(this);
   }

   ADD_FRIEND_PASSES

protected:
   std::vector<Statement::SharedPtr> declarations;
};


#endif //CDOT_DECLARESTMT_H
