//
// Created by Jonas Zell on 19.06.17.
//

#ifndef IFSTATEMENT_H
#define IFSTATEMENT_H

#include "../Statement.h"

class Expression;

class IfStmt : public Statement {
public:
   IfStmt(std::shared_ptr<Expression>, Statement::SharedPtr);
   IfStmt(std::shared_ptr<Expression>);

   inline void setElseBranch(Statement::SharedPtr else_branch) {
      elseBranch = else_branch;
   }

   typedef std::unique_ptr<IfStmt> UniquePtr;
   typedef std::shared_ptr<IfStmt> SharedPtr;

   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int depth) override;

   NodeType get_type() override {
      return NodeType::IF_STMT;
   }

   llvm::Value* accept(CodeGen& v) override {
      return v.visit(this);
   }

   Type* accept(TypeCheckPass& v) override {
      return v.visit(this);
   }

   friend class ConstExprPass;
   friend class CodeGen;
   friend class TypeCheckPass;
   friend class DeclPass;

protected:
   std::shared_ptr<Expression> condition;
   Statement::SharedPtr ifBranch;
   Statement::SharedPtr elseBranch;
   
   bool needsPrimitiveExtraction = false;
};


#endif //IFSTATEMENT_H
