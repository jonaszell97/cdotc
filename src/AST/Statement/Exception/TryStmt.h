//
// Created by Jonas Zell on 05.10.17.
//

#ifndef CDOT_TRYSTMT_H
#define CDOT_TRYSTMT_H

#include "../Statement.h"

namespace cdot {
   struct CatchBlock {
      std::shared_ptr<TypeRef> caughtType;
      string identifier;

      Statement::SharedPtr body;

      bool needsCast = false;
      Type castTo;
   };
}

using cdot::CatchBlock;

class TryStmt: public Statement {
public:
   TryStmt(Statement::SharedPtr &&body);

   void addCatch(CatchBlock&& catchBlock)
   {
      catchBlocks.push_back(catchBlock);
   }

   void setFinally(Statement::SharedPtr&& finallyBlock)
   {
      this->finallyBlock = finallyBlock;
   }

   std::vector<AstNode::SharedPtr> get_children() override;

   typedef std::shared_ptr<TryStmt> SharedPtr;
   void __dump(int depth) override;

   NodeType get_type() override {
      return NodeType::TRY_STMT;
   }

   llvm::Value* accept(CodeGen& v) override {
      return v.visit(this);
   }

   Type accept(SemaPass& v) override {
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
   Statement::SharedPtr body;

   std::vector<CatchBlock> catchBlocks;
   Statement::SharedPtr finallyBlock = nullptr;
};


#endif //CDOT_TRYSTMT_H
