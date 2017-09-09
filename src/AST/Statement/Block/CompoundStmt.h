//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_COMPOUNDSTATEMENT_H
#define CDOT_COMPOUNDSTATEMENT_H

#include "../Statement.h"

class CompoundStmt : public Statement {
public:
   explicit CompoundStmt(bool = false);

   void addStatement(Statement::SharedPtr stmt);

   void returnable(bool terminable) {
      returnable_ = terminable;
   }
   
   Statement::SharedPtr& at(size_t i) {
      return statements.at(i);
   }
   
   size_t size() {
      return statements.size();
   }

   inline void addStatements(std::vector<Statement::SharedPtr> stmts) {
      statements.reserve(stmts.size());
      statements.insert(statements.end(), stmts.begin(), stmts.end());
   }
   
   inline void insertAtBegin(std::vector<Statement::SharedPtr> stmts) {
      statements.insert(statements.begin(), stmts.begin(), stmts.end());
   }

   inline std::vector<Statement::SharedPtr>& getStatements() {
      return statements;
   }

   inline void clear() {
      statements.clear();
   }

   inline void isUnsafe(bool unsafe) {
      isUnsafe_ = unsafe;
   }

   typedef std::shared_ptr<CompoundStmt> SharedPtr;
   typedef std::unique_ptr<CompoundStmt> UniquePtr;
   
   std::vector<AstNode::SharedPtr> get_children() override;

   void __dump(int depth) override;

   NodeType get_type() override {
      return NodeType::COMPOUND_STMT;
   }
   
   llvm::Value* accept(CodeGen& v) override {
      return v.visit(this);
   }
   
   Type* accept(TypeCheckPass& v) override {
      return v.visit(this);
   }

   void accept(DeclPass& v) override {
      v.visit(this);
   }

   Variant accept(ConstExprPass& v) override {
      return v.visit(this);
   }
   
   friend class ConstExprPass;
   friend class CodeGen;
   friend class TypeCheckPass;
   friend class DeclPass;

protected:
   bool returnable_ = true;
   std::vector<Statement::SharedPtr> statements;
   bool preserveScope = false;

   bool isUnsafe_ = false;
   bool implicitZeroReturn = false;

   bool needsCleanup = false;
   std::vector<pair<string, string>> valuesToClean;
};


#endif //CDOT_COMPOUNDSTATEMENT_H
