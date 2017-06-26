//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "DeclStmt.h"
#include "CompoundStmt.h"
#include "../../Util.h"
#include "../Expression/Literal/ArrayLiteral.h"
#include "../../StdLib/Objects/Array.h"

DeclStmt::DeclStmt(std::string ident, Expression::SharedPtr val, TypeSpecifier type) :
        _ident(ident),
        _val(val),
        _type(type)
{

}

DeclStmt::DeclStmt(std::string ident, TypeSpecifier type) : DeclStmt(ident, {}, type) {

}

DeclStmt::DeclStmt(const DeclStmt& cp) {
    _ident = cp._ident;
    _type = cp._type;
    if (cp._val != nullptr) {
        _val = std::static_pointer_cast<Expression>(cp._val->clone());
    }
    else {
        new (&_val) std::shared_ptr<Expression>();
    }
    context = std::make_shared<Context>(*cp.context);
    set_parent(cp._parent);
}

AstNode::SharedPtr DeclStmt::clone() const {
    return std::make_shared<DeclStmt>(*this);
}

Variant DeclStmt::evaluate(Variant) {
    if (context != nullptr) {

        Variant res = _val != nullptr ? _val->evaluate() : Variant();

        res.is_nullable(_type.nullable);

        if (res.get_type() == DOUBLE_T && _type.type != DOUBLE_T) {
            res.cast_to(FLOAT_T);
        }

        if (_type.type == AUTO_T) {
            _type.type = res.get_type();
        }
        else if (_type.type == ANY_T) {
            res.is_any_type();
        }

        if (!val::is_compatible(_type.type, res.get_type())) {
            RuntimeError::raise(ERR_BAD_CAST, "Trying to assign value of type " + val::typetostr(res.get_type())
                                                    + " to variable of type " + val::typetostr(_type.type));
        }

        context->set_variable(_ident, res);
    }
    else {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "No context to create variable " + _ident + " in.");
    }

    return { };
}

Expression::SharedPtr DeclStmt::get_expr() {
    return _val;
}

std::vector<AstNode::SharedPtr> DeclStmt::get_children() {
    return _val == nullptr ? std::vector<AstNode::SharedPtr>() : std::vector<AstNode::SharedPtr>{ _val };
}

void DeclStmt::__dump(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }

    std::cout << "DeclStmt [" << (_type.type == AUTO_T ? "auto" : val::typetostr(_type.type))
              << " " << _ident << "]" << std::endl;

    if (_val != nullptr) {
        _val->__dump(depth + 1);
    }
}