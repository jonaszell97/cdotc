//
// Created by Jonas Zell on 18.08.17.
//

#ifndef CDOT_DEBUGSTMT_H
#define CDOT_DEBUGSTMT_H


#include "Statement.h"

class DebugStmt : public Statement {
public:
   DebugStmt(bool unreachable = false) : isUnreachable(unreachable) {

   }

   std::vector<AstNode::SharedPtr> get_children() override {
      return {};
   }

   void __dump(int) override {}

   NodeType get_type() override {
      return NodeType::DEBUG_STMT;
   }

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

   friend class TypeCheckPass;
   friend class DeclPass;
   friend class CodeGen;
   friend class ConstExprPass;

   typedef std::shared_ptr<DebugStmt> SharedPtr;

protected:
   bool isUnreachable;
};


#endif //CDOT_DEBUGSTMT_H
