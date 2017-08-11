//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_UNARYOPERATOR_H
#define CDOT_UNARYOPERATOR_H

#include "Operator.h"
#include "../Expression/Expression.h"
#include "../Expression/RefExpr/IdentifierRefExpr.h"
#include "../Expression/RefExpr/CallExpr.h"

namespace cdot {
namespace cl {
    struct Method;
}
}

class UnaryOperator : public Operator {
public:
    UnaryOperator(std::string, std::string);
    UnaryOperator(Variant, std::string);

    std::string get_operator();
    std::vector<AstNode::SharedPtr> get_children();

    typedef std::shared_ptr<UnaryOperator> SharedPtr;
    void __dump(int);

    inline virtual NodeType get_type() {
        return NodeType::UNARY_OPERATOR;
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
    std::string _operator;
    std::string __class_name = "UnaryOperator";

    // identifier for '&' operator
    TypeSpecifier operand_type;
    TypeSpecifier result_type;

    Expression::SharedPtr immediate_return_val;

    bool prefix;

    // codegen
    bool is_overriden = false;
    CallExpr::SharedPtr overriden_call;
    bool is_virtual = false;
    cdot::cl::Method* method;
    string class_name;
};


#endif //CDOT_UNARYOPERATOR_H
