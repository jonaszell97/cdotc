//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_UNARYOPERATOR_H
#define CDOT_UNARYOPERATOR_H

#include "../Expression/Expression.h"

class CallExpr;

namespace cdot {
namespace cl {
    struct Method;
}
}

class UnaryOperator : public Expression {
public:
    UnaryOperator(string, string);
    UnaryOperator(Variant, string);

    void setTarget(Expression::SharedPtr t) {
        target = t;
    }

    std::vector<AstNode::SharedPtr> get_children() override;

    typedef std::shared_ptr<UnaryOperator> SharedPtr;
    void __dump(int depth) override;

    NodeType get_type() override {
        return NodeType::UNARY_OPERATOR;
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
    Expression::SharedPtr target;
    string op;
    Type* operandType;
    bool prefix;

    // codegen
    std::shared_ptr<CallExpr> overridenCall;
    cdot::cl::Method* method;
    string className;
};


#endif //CDOT_UNARYOPERATOR_H
