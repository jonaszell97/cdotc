//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "UnaryOperator.h"
#include "../../Variant/Variant.h"
#include "../../StdLib/Objects/Object.h"
#include "../../Util.h"

UnaryOperator::UnaryOperator(std::string op, std::string fix) :
        _operator(op),
        prefix(fix == "prefix")
{

}

UnaryOperator::UnaryOperator(Variant v, std::string fix) : UnaryOperator(v.get<std::string>(), fix) {

}

UnaryOperator::UnaryOperator(const UnaryOperator& cp) {
    _operator = cp._operator;
    _child = std::static_pointer_cast<Expression>(cp._child->clone());
    set_parent(cp._parent);
}

AstNode::SharedPtr UnaryOperator::clone() const {
    return std::make_shared<UnaryOperator>(*this);
}

std::string UnaryOperator::get_operator() {
    return _operator;
}

Variant UnaryOperator::evaluate(Variant) {
    if (_operator == "typeof") {
        return val::type_name(_child->evaluate());
    }
    else if (_operator == "*") {
        return *_child->evaluate();
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
    else if (_operator == "++" || _operator == "--") {
        Variant fst;
        IdentifierRefExpr::SharedPtr ref = std::dynamic_pointer_cast<IdentifierRefExpr>(_child);
        if (ref != nullptr) {
            ref->return_ref(true);
            fst = ref->evaluate();
        }
        else {
            fst = _child->evaluate();
            if (!fst.is_ref()) {
                RuntimeError::raise(ERR_OP_UNDEFINED,
                                    "Cannot apply increment operator to value that is not a reference");
            }
        }

        if (prefix) {
            fst.strict_equals(*fst + Variant(_operator == "++" ? 1 : -1));

            return *fst;
        }
        else {
            Variant return_val(*fst);
            fst.strict_equals(*fst + Variant(_operator == "++" ? 1 : -1));

            return return_val;
        }
    }
    else if (_operator == "~") {
        return ~_child->evaluate();
    }

    RuntimeError::raise(ERR_OP_UNDEFINED, "No definition found for unary operator " + _operator + " on type "
                            + val::typetostr(_child->evaluate().get_type()));
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