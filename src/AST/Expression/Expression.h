//
// Created by Jonas Zell on 19.06.17.
//

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <llvm/IR/GlobalVariable.h>
#include "../Statement/Statement.h"

class Expression : public Statement {
public:
    typedef std::shared_ptr<Expression> SharedPtr;
    typedef std::unique_ptr<Expression> UniquePtr;

    virtual inline void returnLvalue(bool lval) {
        lvalue = lval;
    }

    inline void setParent(AstNode* p) {
        parent = p;
    }

    virtual inline void setMemberExpr(std::shared_ptr<Expression> ref_expr) {
        memberExpr = ref_expr;
        if (memberExpr != nullptr) {
            memberExpr->parentExpr = this;
            memberExpr->lvalue = true;
            children.push_back(&memberExpr);
            memberExpr->parent = this;
        }
    }

    virtual inline void setGlobalVar(llvm::GlobalVariable* glob) {
        globalVar = glob;
        if (memberExpr != nullptr) {
            memberExpr->setGlobalVar(glob);
        }

        isGlobal(true);
        heapAllocate();
    }

    inline void isGlobal(bool gl) {
        isGlobal_ = gl;
        if (memberExpr != nullptr) {
            memberExpr->isGlobal(gl);
        }
    }

    inline void isLhsOfAssigment() {
        isLhsOfAssigment_ = true;
        if (memberExpr != nullptr) {
            memberExpr->isLhsOfAssigment();
        }
    }

    void isHiddenReturnValue() override;

    NodeType get_type() override {
        return NodeType::EXPRESSION;
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
    Expression::SharedPtr memberExpr;
    Expression* parentExpr = nullptr;
    bool lvalue = false;
    string ident;

    Type* castFrom = nullptr;
    Type* needsCastTo = nullptr;

    bool lvalueCast = false;
    bool needsLvalueToRvalueConversion = false;
    bool needsByValPass = false;

    bool isLhsOfAssigment_ = false;
    bool isSetterCall = false;
    string setterName;

    // codegen
    llvm::GlobalVariable* globalVar = nullptr;
};


#endif //EXPRESSION_H
