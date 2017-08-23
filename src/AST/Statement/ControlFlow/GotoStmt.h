//
// Created by Jonas Zell on 29.06.17.
//

#ifndef CDOT_GOTOSTMT_H
#define CDOT_GOTOSTMT_H

#include "../Statement.h"

class GotoStmt : public Statement {
public:
    explicit GotoStmt(string);

    typedef std::shared_ptr<GotoStmt> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children() override;
    void __dump(int depth) override;

    NodeType get_type() override {
        return NodeType::GOTO_STMT;
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

protected:
    string labelName;
};


#endif //CDOT_GOTOSTMT_H
