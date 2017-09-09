//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_CONTINUESTMT_H
#define CDOT_CONTINUESTMT_H

#include "../Statement.h"

class ContinueStmt : public Statement {
public:
    ContinueStmt();

    typedef std::shared_ptr<ContinueStmt> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children() override;
    void __dump(int depth) override;

    NodeType get_type() override {
        return NodeType::CONTINUE_STMT;
    }

    llvm::Value* accept(CodeGen& v) override {
        return v.visit(this);
    }

    Type* accept(TypeCheckPass& v) override {
        return v.visit(this);
    }

   void accept(DeclPass &v) override {
      v.visit(this);
   }

    friend class ConstExprPass;
    friend class CodeGen;
    friend class TypeCheckPass;
   friend class DeclPass;
};


#endif //CDOT_CONTINUESTMT_H
