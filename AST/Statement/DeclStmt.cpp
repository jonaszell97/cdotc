//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "DeclStmt.h"
#include "CompoundStmt.h"
#include "../../Util.h"

DeclStmt::DeclStmt(std::string ident, Expression::SharedPtr val, ValueType type) :
        _ident(ident),
        _val(val),
        _type(type)
{

}

Variant DeclStmt::evaluate(Variant) {
    if (auto root = _root.lock()) {
        Variant res = _val->evaluate();

        if (res.get_type() == DOUBLE_T && _type != DOUBLE_T) {
            res.cast_to(FLOAT_T);
        }

        if (_type == AUTO_T) {
            _type = res.get_type();
        }
        else if (_type == ANY_T) {
            res.is_any_type();
        }

        if (!val::is_compatible(_type, res.get_type())) {
            RuntimeError::raise(ERR_BAD_CAST, "Trying to assign value of type " + util::types[res.get_type()]
                                                    + " to variable of type " + util::types[_type]);
        }

        root->set_variable(_ident, res);
    }
    else {
        RuntimeError::raise(ERR_MISSING_CONTEXT, "No context to create variable " + _ident + " in.");
    }

    return { };
}

Expression::SharedPtr DeclStmt::get_expr() {
    return _val;
}

std::vector<AstNode::SharedPtr> DeclStmt::get_children() {
    std::vector<AstNode::SharedPtr> res;
    res.emplace_back(_val);

    return res;
}

void DeclStmt::__dump(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }

    std::cout << "DeclStmt [" << (_type == AUTO_T ? "auto" : util::types[_type]) << " " << _ident << "]" << std::endl;

    _val->__dump(depth + 1);
}