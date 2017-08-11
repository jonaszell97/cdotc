//
// Created by Jonas Zell on 21.06.17.
//

#ifndef CDOT_ARRAYLITERAL_H
#define CDOT_ARRAYLITERAL_H


#include "../../AstNode.h"
#include "../Expression.h"

class ArrayLiteral : public Expression {
public:
    ArrayLiteral(TypeSpecifier = {});

    inline void set_length(int len) {
        _length = len;
    }
    void add_element(Expression::SharedPtr);

    typedef std::shared_ptr<ArrayLiteral> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    inline virtual NodeType get_type() {
        return NodeType::ARRAY_LITERAL;
    }

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }
    virtual inline CGValue accept(CodeGenVisitor& v) {
        return v.visit(this);
    }
    virtual TypeSpecifier accept(TypeCheckVisitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    friend class ConstExprVisitor;
    friend class CodeGenVisitor;
    friend class TypeCheckVisitor;

protected:
    std::vector<Expression::SharedPtr> _elements;
    Expression::SharedPtr _length_expr;
    TypeSpecifier type;
    int _length;
    bool var_length = false;

    // codegen
    llvm::Value* preinit = nullptr;
};


#endif //CDOT_ARRAYLITERAL_H
