//
// Created by Jonas Zell on 02.07.17.
//

#ifndef CDOT_IMPORTSTMT_H
#define CDOT_IMPORTSTMT_H

#include "Statement.h"

class UsingStmt : public Statement {
public:
    explicit UsingStmt(string);

    typedef std::shared_ptr<UsingStmt> SharedPtr;

    std::vector<AstNode::SharedPtr> get_children() override;
    void __dump(int depth) override;

    NodeType get_type() override {
        return NodeType::USING_STMT;
    }

    llvm::Value* accept(CodeGenVisitor& v) override {
        return v.visit(this);
    }

    Type* accept(TypeCheckVisitor& v) override {
        return v.visit(this);
    }

    friend class EvaluatingVisitor;
    friend class TypeCheckVisitor;
    friend class CodeGenVisitor;

protected:
    string nsName;
};


#endif //CDOT_IMPORTSTMT_H
