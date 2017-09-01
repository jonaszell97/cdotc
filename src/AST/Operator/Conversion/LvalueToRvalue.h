//
// Created by Jonas Zell on 15.08.17.
//

#ifndef CDOT_LVALUETORVALUECONV_H
#define CDOT_LVALUETORVALUECONV_H


#include "../../Expression/Expression.h"
using namespace cdot;

class LvalueToRvalue : public Expression {
public:
    explicit LvalueToRvalue(Expression::SharedPtr expr);

    typedef std::shared_ptr<LvalueToRvalue> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children() override;
    void __dump(int depth) override;

    llvm::Value* accept(CodeGenVisitor& v) override {
        return v.visit(this);
    }

    Type* accept(TypeCheckVisitor& v) override {
        return v.visit(this);
    }

    NodeType get_type() override {
        return NodeType::LVALUE_TO_RVALUE;
    }

    friend class CodeGenVisitor;
    friend class TypeCheckVisitor;
    friend class ConstExprVisitor;

protected:
    Expression::SharedPtr target;
};


#endif //CDOT_LVALUETORVALUECONV_H
