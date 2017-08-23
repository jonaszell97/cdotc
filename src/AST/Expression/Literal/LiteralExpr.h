//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_LITERALEXPRESSION_H
#define CDOT_LITERALEXPRESSION_H


#include "../Expression.h"

class LiteralExpr : public Expression {
public:
    LiteralExpr(Variant);
    LiteralExpr(Variant, Type*);

    typedef std::shared_ptr<LiteralExpr> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children() override;
    void __dump(int) override;

    NodeType get_type() override {
        return NodeType::LITERAL_EXPR;
    }

    llvm::Value* accept(CodeGenVisitor& v) override{
        return v.visit(this);
    }

    Type* accept(TypeCheckVisitor& v) override {
        return v.visit(this);
    }

    friend class ConstExprVisitor;
    friend class CodeGenVisitor;
    friend class TypeCheckVisitor;

protected:
    Variant value;
    Type* type = nullptr;
    string className;
    string constructor;

    // codegen
    bool isPrimitive = false;
};


#endif //CDOT_LITERALEXPRESSION_H
