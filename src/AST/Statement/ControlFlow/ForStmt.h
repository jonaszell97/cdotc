//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_FORSTMT_H
#define CDOT_FORSTMT_H

#include "../Statement.h"

class IdentifierRefExpr;

class ForStmt : public Statement {
public:
    ForStmt(Statement::SharedPtr, Statement::SharedPtr, Statement::SharedPtr);

    void setBody(Statement::SharedPtr _body) {
        body = _body;
    }

    typedef std::shared_ptr<ForStmt> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children() override;
    void __dump(int depth) override;

    NodeType get_type() override {
        return NodeType::FOR_STMT;
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
    bool rangeBased  = false;
    std::shared_ptr<Expression> range;

    Statement::SharedPtr initialization;
    Statement::SharedPtr termination;
    Statement::SharedPtr increment;
    Statement::SharedPtr body;
};


#endif //CDOT_FORSTMT_H
