//
// Created by Jonas Zell on 21.06.17.
//

#ifndef CDOT_CALLEXPR_H
#define CDOT_CALLEXPR_H


#include "../Expression.h"

namespace cdot {
namespace cl {
    class Class;
    class Method;
}
}

enum class CallType {
    METHOD_CALL,
    FUNC_CALL,
    CONSTR_CALL,
    ANON_CALL
};

class CallExpr : public Expression {
public:
    CallExpr(CallType, std::vector<Expression::SharedPtr>, string = "");

    virtual ~CallExpr() {
//        delete genericReturnType;
//        delete protocolType;
    }

    inline void set_generics(std::vector<Type*> generics) {
        this->generics = generics;
    }

    inline void isLamdbaCall(bool lambda) {
        isLambdaCall = lambda;
    }

    typedef std::shared_ptr<CallExpr> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children() override;
    void __dump(int) override;

    NodeType get_type() override {
        return NodeType::CALL_EXPR;
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
    CallType type;
    std::vector<Expression::SharedPtr> args;

    // codegen
    bool isCallOp = false;
    string callOpBinding;

    bool isLambdaCall = false;

    bool hasHiddenParamReturn = false;
    Type* hiddenParamType = nullptr;

    // method call
    bool isStatic = false;
    Type* returnType = nullptr;
    bool is_virtual = false;

    string class_name;
    cdot::cl::Method* method;

    Type* genericReturnType = nullptr;
    bool needsGenericCast = false;

    std::vector<Type*> generics;
    std::vector<Type*> argTypes;

    bool implicitSelfCall = false;
    string selfBinding;

    string autoboxConstr;

    bool protocolShift = false;
    bool reverseProtoShift = false;
    Type* protocolType = nullptr;

    bool castToBase = false;
};


#endif //CDOT_CALLEXPR_H
