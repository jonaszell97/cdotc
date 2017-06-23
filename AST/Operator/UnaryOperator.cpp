//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "UnaryOperator.h"
#include "../../Variant.h"
#include "../../Objects/Object.h"
#include "../../Util.h"

UnaryOperator::UnaryOperator(std::string op, std::string fix) :
        _operator(op),
        prefix(fix == "prefix")
{

}

UnaryOperator::UnaryOperator(Variant v, std::string fix) : UnaryOperator(v.s_val, fix) {

}

std::string UnaryOperator::get_operator() {
    return _operator;
}

VariantPtr UnaryOperator::evaluate(VariantPtr) {
    if (_operator == "typeof") {
        return Variant::from(util::types[_child->evaluate()->type]);
    }
    else if (_operator == "+") {
        return _child->evaluate();
    }
    else if (_operator == "-") {
        return Variant::from(-(*_child->evaluate()));
    }
    else if (_operator == "!") {
        return Variant::from(!(*_child->evaluate()));
    }
    else if (_operator == "++" && prefix) {
        auto fst = _child->evaluate(Variant::from(true));

        fst->strict_equals(*fst + Variant(1));
        /*if (fst->type == REF_T) {
            fst->ref->strict_equals(*fst + Variant(1.0));
        } else if (fst->type == OBJECT_PROP_T) {
            fst->op_val->set(*fst + Variant(1.0));
        }
        else {
            RuntimeError::raise(ERR_OP_UNDEFINED, "Cannot apply increment operator to value that is not a reference");
        }*/

        return fst;
    }
    else if (_operator == "++") {
        auto fst = _child->evaluate(Variant::from(true));
        Variant return_val;

        fst->strict_equals(*fst + Variant(1));
        /*if (fst->type == REF_T) {
            fst->ref->strict_equals(*fst->ref + Variant(1.0));
            return_val = *fst->ref;
        } else if (fst->type == OBJECT_PROP_T) {
            fst->op_val->set(*fst + Variant(1.0));
            return_val = fst->op_val->get();
        }
        else {
            RuntimeError::raise(ERR_OP_UNDEFINED, "Cannot apply increment operator to value that is not a reference");
        }*/

        return Variant::from(return_val);
    }
    else if (_operator == "--" && prefix) {
        auto fst = _child->evaluate(Variant::from(true));

        fst->strict_equals(*fst - Variant(1));
        /*if (fst->type == REF_T) {
            fst->ref->strict_equals(*fst->ref - Variant(1.0));
        } else if (fst->type == OBJECT_PROP_T) {
            fst->op_val->set(*fst - Variant(1.0));
        }
        else {
            RuntimeError::raise(ERR_OP_UNDEFINED, "Cannot apply decrement operator to value that is not a reference");
        }*/

        return fst;
    }
    else if (_operator == "--") {
        auto fst = _child->evaluate(Variant::from(true));
        Variant return_val;

        fst->strict_equals(*fst - Variant(1));
        /*if (fst->type == REF_T) {
            fst->ref->strict_equals(*fst - Variant(1.0));
            return_val = *fst->ref;
        } else if (fst->type == OBJECT_PROP_T) {
            fst->op_val->set(*fst - Variant(1.0));
            return_val = fst->op_val->get();
        }
        else {
            RuntimeError::raise(ERR_OP_UNDEFINED, "Cannot apply decrement operator to value that is not a reference");
        }*/

        return Variant::from(return_val);
    }
    else if (_operator == "~") {
        return Variant::from(_child->evaluate()->operator~());
    }

    RuntimeError::raise(ERR_OP_UNDEFINED, "No definition found for unary operator " + _operator + " on type "
                            + std::to_string(_child->evaluate()->type));
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