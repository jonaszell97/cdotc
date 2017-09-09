//
// Created by Jonas Zell on 02.07.17.
//

#ifndef CDOT_EXPORTSTMT_H
#define CDOT_EXPORTSTMT_H

#include "Statement.h"

class EndOfFileStmt : public Statement {
public:
    EndOfFileStmt() = default;

    typedef std::shared_ptr<EndOfFileStmt> SharedPtr;

    std::vector<AstNode::SharedPtr> get_children() override {
        return {};
    }

    void __dump(int depth) override {}

    NodeType get_type() override {
        return NodeType::EOF_STMT;
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

    friend class TypeCheckPass;
   friend class DeclPass;
};


#endif //CDOT_EXPORTSTMT_H
