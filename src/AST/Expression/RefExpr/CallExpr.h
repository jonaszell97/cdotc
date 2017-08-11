//
// Created by Jonas Zell on 21.06.17.
//

#ifndef CDOT_CALLEXPR_H
#define CDOT_CALLEXPR_H


#include "../Expression.h"
#include "RefExpr.h"
#include "../../Visitor/StaticAnalysis/Class.h"

enum class CallType {
    METHOD_CALL,
    FUNC_CALL,
    CONSTR_CALL,
    ANON_CALL
};

class CallExpr : public Expression {
public:
    CallExpr(CallType, std::vector<Expression::SharedPtr>, std::string = "");

    void add_argument(Expression::SharedPtr);

    inline void set_generics(std::vector<TypeSpecifier> generics) {
        this->generics = generics;
    }

    inline void isLamdbaCall(bool lambda) {
        is_lambda_call = lambda;
    }

    typedef std::shared_ptr<CallExpr> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children() override;
    void __dump(int) override;

    inline virtual NodeType get_type() override {
        return NodeType::CALL_EXPR;
    }
    virtual inline Variant accept(Visitor& v) override {
        return v.visit(this);
    }
    virtual inline CGValue accept(CodeGenVisitor& v) override {
        return v.visit(this);
    }
    virtual TypeSpecifier accept(TypeCheckVisitor& v) override {
        return v.visit(this);
    }


    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    friend class ConstExprVisitor;
    friend class CodeGenVisitor;
    friend class TypeCheckVisitor;
    friend class MethodCallExpr;

protected:
    CallType type;
    std::string ident;
    std::vector<Expression::SharedPtr> args;

    // codegen
    TypeSpecifier ret_type;
    bool is_call_op = false;
    std::string call_op_binding;

    bool is_lambda_call = false;

    // method call
    bool is_static = false;
    TypeSpecifier return_type;
    bool is_virtual = false;
    int vtable_pos;
    std::string class_name;
    cdot::cl::Method* method;

    TypeSpecifier generic_return_type;
    bool needs_generic_cast = false;

    std::vector<TypeSpecifier> generics;

    bool implicit_this_call = false;
    std::string this_val;
    bool is_ns_member = false;
    string autobox_constr;

    bool interface_shift = false;
};


#endif //CDOT_CALLEXPR_H
