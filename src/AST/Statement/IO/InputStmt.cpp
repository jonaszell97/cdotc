//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "InputStmt.h"
#include "../../../Variant/Variant.h"
#include "../CompoundStmt.h"
#include "../../Expression/Literal/LiteralExpr.h"
#include "../../../Util.h"
#include "../../Visitor/Visitor.h"

InputStmt::InputStmt(std::string s, ValueType type) : _ident(s), _type(type) {
    _literal = std::make_shared<LiteralExpr>();
    _decl = std::make_shared<DeclStmt>(s, _literal, TypeSpecifier());
}

InputStmt::InputStmt(Variant v, ValueType type) : InputStmt(v.get<std::string>(), type) {
}

InputStmt::InputStmt(const InputStmt& cp) {
    _literal = std::static_pointer_cast<LiteralExpr>(cp._literal->clone());
    _decl = std::static_pointer_cast<DeclStmt>(cp._decl->clone());
    //set_root(cp._root, true);
    set_parent(cp._parent);
}

AstNode::SharedPtr InputStmt::clone() const {
    return std::make_shared<InputStmt>(*this);
}

std::vector<AstNode::SharedPtr> InputStmt::get_children() {
    return { _literal, _decl };
}

void InputStmt::__dump(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }

    std::cout << "InputStmt ['" << val::typetostr(_type) << " " << _ident << "']" << std::endl;

    _decl->__dump(depth + 1);
}