//
// Created by Jonas Zell on 19.06.17.
//

#include <iostream>
#include "InputStmt.h"
#include "../../../Variant.h"
#include "../CompoundStmt.h"
#include "../../Expression/Literal/LiteralExpr.h"
#include "../../../Util.h"

InputStmt::InputStmt(std::string s, ValueType type) : _ident(s), _type(type) {
    _literal = std::make_shared<LiteralExpr>();
    _decl = std::make_shared<DeclStmt>(s, _literal);
}

InputStmt::InputStmt(Variant v, ValueType type) : InputStmt(v.s_val, type) {
}

VariantPtr InputStmt::evaluate(VariantPtr) {
    std::string s;
    std::cin >> s;

    _literal->set_val(Variant::from(Variant(s).cast_to(_type)));
    _decl->evaluate();

    return { };
}

void InputStmt::__dump(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }

    std::cout << "InputStmt ['" << util::types[_type] << " " << _ident << "']" << std::endl;

    _decl->__dump(depth + 1);
}