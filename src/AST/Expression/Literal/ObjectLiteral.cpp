//
// Created by Jonas Zell on 20.06.17.
//

#include <iostream>
#include "ObjectLiteral.h"

ObjectLiteral::ObjectLiteral() {
    _props = std::vector<ObjectPropExpr::SharedPtr>();
}

void ObjectLiteral::add_prop(ObjectPropExpr::SharedPtr prop) {
    _props.push_back(prop);
}

std::vector<ObjectPropExpr::SharedPtr> ObjectLiteral::get_props() {
    return _props;
}

std::vector<AstNode::SharedPtr> ObjectLiteral::get_children() {
    std::vector<AstNode::SharedPtr> res;
    for (int i = 0; i < _props.size(); i++) {
        res.emplace_back(_props.at(i));
    }
    if (_member_expr != nullptr) {
        res.push_back(_member_expr);
    }

    return res;
}

void ObjectLiteral::__dump(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }

    std::cout << "ObjectLiteral" << std::endl;

    for (auto prop : _props) {
        prop->__dump(depth + 1);
    }
}