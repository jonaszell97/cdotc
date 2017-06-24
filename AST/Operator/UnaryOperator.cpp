//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "UnaryOperator.h"
#include "../../Variant/Variant.h"
#include "../../Objects/Object.h"
#include "../../Util.h"

UnaryOperator::UnaryOperator(std::string op, std::string fix) :
        _operator(op),
        prefix(fix == "prefix")
{

}

UnaryOperator::UnaryOperator(Variant v, std::string fix) : UnaryOperator(v.get<std::string>(), fix) {

}

std::string UnaryOperator::get_operator() {
    return _operator;
}

Variant UnaryOperator::evaluate(Variant) {
    if (_operator == "typeof") {
        return util::types[_child->evaluate().get_type()];
    }
    else if (_operator == "&") {
        IdentifierRefExpr::SharedPtr child = std::static_pointer_cast<IdentifierRefExpr>(_child);
        child->return_ref(true);

        return child->evaluate();

    }
    else if (_operator == "+") {
        return _child->evaluate();
    }
    else if (_operator == "-") {
        return -(_child->evaluate());
    }
    else if (_operator == "!") {
        return !(_child->evaluate());
    }
    else if (_operator == "++" && prefix) {
        auto fst = _child->evaluate();

        if (fst.get_type() == REF_T) {
            fst.strict_equals(fst + Variant(1));
        }
        else {
            RuntimeError::raise(ERR_OP_UNDEFINED, "Cannot apply increment operator to value that is not a reference");
        }

        return fst;
    }
    else if (_operator == "++") {
        auto fst = _child->evaluate();
        Variant return_val(fst);

        if (fst.get_type() == REF_T) {
            fst.strict_equals(fst + Variant(1));
        }
        else {
            RuntimeError::raise(ERR_OP_UNDEFINED, "Cannot apply increment operator to value that is not a reference");
        }

        return return_val;
    }
    else if (_operator == "--" && prefix) {
        auto fst = _child->evaluate();

        if (fst.get_type() == REF_T) {
            fst.strict_equals(fst - Variant(1));
        }
        else {
            RuntimeError::raise(ERR_OP_UNDEFINED, "Cannot apply increment operator to value that is not a reference");
        }

        return fst;
    }
    else if (_operator == "--") {
        auto fst = _child->evaluate();
        Variant return_val(fst);

        if (fst.get_type() == REF_T) {
            fst.strict_equals(fst - Variant(1));
        }
        else {
            RuntimeError::raise(ERR_OP_UNDEFINED, "Cannot apply increment operator to value that is not a reference");
        }

        return return_val;
    }
    else if (_operator == "~") {
        return ~_child->evaluate();
    }

    RuntimeError::raise(ERR_OP_UNDEFINED, "No definition found for unary operator " + _operator + " on type "
                            + std::to_string(_child->evaluate().get_type()));
}

std::vector<AstNode::SharedPtr> UnaryOperator::get_children() {
    std::vector<AstNode::SharedPtr> res;
    res.push_back(_child);

    return res;
}

void UnaryOperator::__dump(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }

    std::cout << "UnaryOperator ['" << _operator << "']" << std::endl;

    _child->__dump(depth + 1);
}