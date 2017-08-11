//
// Created by Jonas Zell on 19.06.17.
//

#ifndef EXPRESSION_H
#define EXPRESSION_H


#include "../AstNode.h"
#include "../Statement/Statement.h"
#include "TypeRef.h"

class Expression : public Statement {
public:
    typedef std::shared_ptr<Expression> SharedPtr;

    Expression();
    Expression(Expression::SharedPtr);

    virtual void set_child(Expression::SharedPtr);

    virtual inline void return_lvalue(bool ref) {
        lvalue = ref;
    }
    virtual inline void implicit_ref(bool implicit) {
        _implicit_ref = implicit;
    }
    virtual inline void set_member_expr(std::shared_ptr<Expression> ref_expr) {
        _member_expr = ref_expr;
        if (_member_expr != nullptr) {
            _member_expr->_parent_expr = this;
            _member_expr->lvalue = true;
        }
    }
    inline virtual NodeType get_type() {
        return NodeType::EXPRESSION;
    }
    std::vector<AstNode::SharedPtr> get_children();

    virtual void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }
    virtual inline CGValue accept(CodeGenVisitor& v) {
        return v.visit(this);
    }
    virtual TypeSpecifier accept(TypeCheckVisitor& v) {
        return v.visit(this);
    }
    inline void is_lvalue() {
        lvalue = true;
        if (_member_expr) {
            _member_expr->is_lvalue();
        }
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    friend class ConstExprVisitor;
    friend class CodeGenVisitor;
    friend class TypeCheckVisitor;

protected:
    Expression::SharedPtr _child;
    bool _implicit_ref = false;
    Expression::SharedPtr _member_expr;
    Expression* _parent_expr = nullptr;
    bool lvalue = false;

    // codegen
    Variant::UniquePtr static_val;
    bool cast_needed = false;
    TypeSpecifier cast_to;
};


#endif //EXPRESSION_H
