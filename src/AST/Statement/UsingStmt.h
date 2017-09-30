//
// Created by Jonas Zell on 02.07.17.
//

#ifndef CDOT_IMPORTSTMT_H
#define CDOT_IMPORTSTMT_H

#include "Statement.h"

enum class UsingKind {
   FUNCTION,
   CLASS,
   VARIABLE,
   TYPEDEF,
   NAMESPACE
};

class UsingStmt : public Statement {
public:
   explicit UsingStmt(string&& importNamespace, std::vector<string>&& item);

   typedef std::shared_ptr<UsingStmt> SharedPtr;

   std::vector<AstNode::SharedPtr> get_children() override;
   void __dump(int depth) override;

   NodeType get_type() override {
      return NodeType::USING_STMT;
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

   ADD_FRIEND_PASSES

protected:
   string importNamespace;
   std::vector<string> importedItems;

   bool isWildcardImport = false;

   std::vector<string> fullNames;
   UsingKind kind;
};


#endif //CDOT_IMPORTSTMT_H
