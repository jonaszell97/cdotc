//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_BREAKSTMT_H
#define CDOT_BREAKSTMT_H

#include "../Statement.h"

class BreakStmt : public Statement {
public:
    explicit BreakStmt();

    typedef std::shared_ptr<BreakStmt> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children() override;
    void __dump(int depth) override ;

    NodeType get_type() override {
        return NodeType::BREAK_STMT;
    }

    llvm::Value* accept(CodeGenVisitor& v) override {
        return v.visit(this);
    }

    Type* accept(TypeCheckVisitor& v) override {
        return v.visit(this);
    }

    friend class ConstExprVisitor;
    friend class CodeGenVisitor;
    friend class TypeCheckVisitor;
};


#endif //CDOT_BREAKSTMT_H
