//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "BinaryOperator.h"
#include "../../Objects/Object.h"
#include "../../Util.h"
#include "../Statement/CompoundStmt.h"

BinaryOperator::BinaryOperator(std::string op) : _operator(op) {

}

void BinaryOperator::set_fst_child(Expression::SharedPtr exp) {
    _first_child = exp;
}

void BinaryOperator::set_snd_child(Expression::SharedPtr exp) {
    _second_child = exp;
}

std::string BinaryOperator::get_operator() {
    return _operator;
}

Variant BinaryOperator::evaluate(Variant) {
    if (!(util::in_vector(util::binary_operators, _operator))) {
        RuntimeError::raise(ERR_OP_UNDEFINED, "Undefined binary operator " + _operator);
    }

    if (_operator == "=") {
        auto fst = _first_child->evaluate();
        auto snd = _second_child->evaluate();

        fst.strict_equals(snd);

        return fst;
    }
    else if (_operator == "+=") {
        auto fst = _first_child->evaluate();
        auto snd = _second_child->evaluate();

        fst.strict_equals(fst + snd);

        return fst;
    }
    else if (_operator == "-=") {
        auto fst = _first_child->evaluate();
        auto snd = _second_child->evaluate();

        fst.strict_equals(fst - snd);

        return fst;
    }
    else if (_operator == "*=") {
        auto fst = _first_child->evaluate();
        auto snd = _second_child->evaluate();

        fst.strict_equals(fst * snd);

        return fst;
    }
    else if (_operator == "/=") {
        auto fst = _first_child->evaluate();
        auto snd = _second_child->evaluate();

        fst.strict_equals(fst - snd);

        return fst;
    }

    auto fst = _first_child->evaluate();
    auto snd = _second_child->evaluate();

    if (_operator == "==") {
        return fst == snd;
    }
    else if (_operator == "!=") {
        return fst != snd;
    }
    else if (_operator == "<=") {
        return fst <= snd;
    }
    else if (_operator == ">=") {
        return fst >= snd;
    }
    else if (_operator == ">>") {
        return fst >> snd;
    }
    else if (_operator == ">>") {
        return fst << snd;
    }
    else if (_operator == "**") {
        return fst.pow(snd);
    }
    else switch (_operator[0]) {
        case '+': {
            return fst + snd;
        }
        case '-': {
            return fst - snd;
        }
        case '*': {
            return fst * snd;
        }
        case '/': {
            return fst / snd;
        }
        case '%': {
            return fst % snd;
        }
        case '<': {
            return fst < snd;
        }
        case '>': {
            return fst > snd;
        }
        case '&': {
            return fst & snd;
        }
        case '|': {
            return fst | snd;
        }
        case '^': {
            return fst ^ snd;
        }
        default:
            RuntimeError::raise(ERR_OP_UNDEFINED, "Undefined binary operator " + _operator);
    }
}

std::vector<AstNode::SharedPtr> BinaryOperator::get_children() {
    std::vector<AstNode::SharedPtr> res;
    res.push_back(_first_child);
    res.push_back(_second_child);

    return res;
}

void BinaryOperator::__dump(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }

    std::cout << "BinaryOperator ['" + _operator << "']" << std::endl;

    _first_child->__dump(depth + 1);
    _second_child->__dump(depth + 1);
}