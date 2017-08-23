//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_WHILESTATEMENT_H
#define CDOT_WHILESTATEMENT_H

#include "../Statement.h"
class Expression;

class WhileStmt : public Statement {
public:
    WhileStmt(std::shared_ptr<Expression>, Statement::SharedPtr);

    std::vector<AstNode::SharedPtr> get_children() override;
    void __dump(int depth) override;

    NodeType get_type() override {
        return NodeType::WHILE_STMT;
    }

    llvm::Value* accept(CodeGenVisitor& v) override {
        return v.visit(this);
    }

    Type* accept(TypeCheckVisitor& v) override {
        return v.visit(this);
    }

    typedef std::shared_ptr<WhileStmt> SharedPtr;

    friend class ConstExprVisitor;
    friend class CodeGenVisitor;
    friend class TypeCheckVisitor;

protected:
    std::shared_ptr<Expression> condition;
    Statement::SharedPtr body;
};


#endif //CDOT_WHILESTATEMENT_H
