//
// Created by Jonas Zell on 28.06.17.
//

#ifndef CDOT_CASESTMT_H
#define CDOT_CASESTMT_H

#include "LabelStmt.h"
class Expression;

class CaseStmt : public LabelStmt {
public:
    explicit CaseStmt(std::shared_ptr<Expression>);
    CaseStmt();

    typedef std::shared_ptr<CaseStmt> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children() override;
    void __dump(int depth) override;

    NodeType get_type() override {
        return NodeType::CASE_STMT;
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
    bool isDefault = false;
    std::shared_ptr<Expression> caseVal;

    // codegen
    bool fallthrough = false;
};


#endif //CDOT_CASESTMT_H
