//
// Created by Jonas Zell on 01.09.17.
//

#ifndef CDOT_FORINSTMT_H
#define CDOT_FORINSTMT_H


#include "../Statement.h"

class ForInStmt: public Statement {
public:
   ForInStmt(std::shared_ptr<DeclStmt>&& decl, std::shared_ptr<Expression>&& range,
      std::shared_ptr<Statement>&& body);

   typedef std::shared_ptr<ForInStmt> SharedPtr;
   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int depth) override;

   NodeType get_type() override {
      return NodeType::FOR_IN_STMT;
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
   std::shared_ptr<DeclStmt> decl;
   std::shared_ptr<Expression> rangeExpr;
   std::shared_ptr<Statement> body;

   // codegen
   string iteratorGetter;
   string iteratorClass;
   string nextFunc;
   Type* iteratedType = nullptr;
   bool rangeIsRefcounted = false;
   string rangeClassName;

   Type* protocolTy = nullptr;
};


#endif //CDOT_FORINSTMT_H
