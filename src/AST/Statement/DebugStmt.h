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

   llvm::Value* accept(CodeGenVisitor& v) override {
      return v.visit(this);
   }

   Type* accept(TypeCheckVisitor& v) override {
      return v.visit(this);
   }

   friend class TypeCheckVisitor;
   friend class CodeGenVisitor;

   typedef std::shared_ptr<DebugStmt> SharedPtr;

protected:
   bool isUnreachable;
};


#endif //CDOT_DEBUGSTMT_H
