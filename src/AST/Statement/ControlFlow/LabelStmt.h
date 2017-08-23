//
// Created by Jonas Zell on 28.06.17.
//

#ifndef CDOT_LABELSTMT_H
#define CDOT_LABELSTMT_H

#include "../Statement.h"

class LabelStmt : public Statement {
public:
    explicit LabelStmt(string);

    typedef std::shared_ptr<LabelStmt> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children() override;
    void __dump(int depth) override;


    llvm::Value* accept(CodeGenVisitor& v) override {
        return v.visit(this);
    }

    Type* accept(TypeCheckVisitor& v) override {
        return v.visit(this);
    }

    NodeType get_type() override {
        return NodeType::LABEL_STMT;
    }

    friend class ConstExprVisitor;
    friend class CodeGenVisitor;
    friend class TypeCheckVisitor;

protected:
    string labelName;
};


#endif //CDOT_LABELSTMT_H
