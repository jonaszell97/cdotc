//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_BINARYOPERATOR_H
#define CDOT_BINARYOPERATOR_H


#include "../AstNode.h"
#include <string>
#include "Operator.h"
#include "../Expression/Expression.h"
#include "../Expression/RefExpr/CallExpr.h"

namespace cdot {
namespace cl {
    class Method;
}
}

class BinaryOperator : public Operator {
public:
    BinaryOperator(std::string);

    std::string get_operator();
    std::vector<AstNode::SharedPtr> get_children();
    void set_fst_child(Expression::SharedPtr);
    void set_snd_child(Expression::SharedPtr);

    typedef std::shared_ptr<BinaryOperator> SharedPtr;
    void __dump(int);

    inline virtual NodeType get_type() {
        return NodeType::BINARY_OPERATOR;
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
    Expression::SharedPtr _first_child;
    Expression::SharedPtr _second_child;
    TypeSpecifier operand_type;
    std::string _operator;

    unsigned long lhs_arr_size;
    unsigned long rhs_arr_size;

    // codegen
    bool is_overriden = false;
    CallExpr::SharedPtr overriden_call = nullptr;
    bool is_virtual = false;
    string class_name;
    cdot::cl::Method* method;
};


#endif //CDOT_BINARYOPERATOR_H
