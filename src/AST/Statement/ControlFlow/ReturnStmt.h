//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_RETURNSTATEMENT_H
#define CDOT_RETURNSTATEMENT_H

#include "../Statement.h"

class Expression;

class ReturnStmt : public Statement {
public:
    ReturnStmt();
    explicit ReturnStmt(std::shared_ptr<Expression>);

    ~ReturnStmt() override {
//        delete returnType;
    }

    typedef std::shared_ptr<ReturnStmt> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children() override;
    void __dump(int depth) override;

    NodeType get_type() override {
        return NodeType::RETURN_STMT;
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
    std::shared_ptr<Expression> returnValue;

    bool interfaceShift = false;
    string originTy;

    bool hiddenParamReturn = false;

    // codegen
    Type* returnType;
};


#endif //CDOT_RETURNSTATEMENT_H
