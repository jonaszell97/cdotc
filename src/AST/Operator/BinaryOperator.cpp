//
// Created by Jonas Zell on 19.06.17.
//

#include "BinaryOperator.h"

namespace cdot {
    BinaryOperatorType getBinaryOpType(string op) {
        if (!util::isAssignmentOperator(op).empty()) {
            return BinaryOperatorType::ASSIGNMENT;
        }
        if (op == "+" || op == "-" || op == "*" || op == "/" || op == "%" || op == "**") {
            return BinaryOperatorType::ARITHMETIC;
        }
        if (op == "&" || op == "|" || op == "^" || op == "<<" || op == ">>" || op == ">>>") {
            return BinaryOperatorType::BITWISE;
        }
        if (op == "&&" || op == "||") {
            return BinaryOperatorType::LOGICAL;
        }
        if (op == "==" || op == "!=" || op == "===" || op == "!==") {
            return BinaryOperatorType::EQUALITY;
        }
        if (op == "<=" || op == ">=" || op == "<" || op == ">") {
            return BinaryOperatorType::COMPARISON;
        }
        if (op == "as" || op == "as!") {
            return BinaryOperatorType::CAST;
        }

        return BinaryOperatorType::OTHER;
    }

    BinaryOperator::BinaryOperator(string op) : op(op) {

    }

    BinaryOperator::BinaryOperator(string op, Expression::SharedPtr lhs, Expression::SharedPtr rhs) :
        op(op),
        lhs(lhs),
        rhs(rhs) {

    }

    void BinaryOperator::setLhs(Expression::SharedPtr exp) {
        lhs = exp;
        children.push_back(&lhs);
    }

    void BinaryOperator::setRhs(Expression::SharedPtr exp) {
        rhs = exp;
        children.push_back(&rhs);
    }

    std::vector<AstNode::SharedPtr> BinaryOperator::get_children() {
        std::vector<AstNode::SharedPtr> res;
        res.push_back(lhs);
        res.push_back(rhs);
        if (memberExpr != nullptr) {
            res.push_back(memberExpr);
        }

        return res;
    }

    void BinaryOperator::__dump(int depth) {
        AstNode::__tab(depth);

        std::cout << "BinaryOperator ['" + op << "']" << std::endl;

        for (auto child : get_children()) {
            child->__dump(depth + 1);
        }
    }

}