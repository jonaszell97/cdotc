//
// Created by Jonas Zell on 20.06.17.
//

#include <iostream>
#include "ObjectLiteral.h"

ObjectLiteral::ObjectLiteral() {
    _props = std::vector<ObjectPropExpr>();
}

void ObjectLiteral::add_prop(ObjectPropExpr prop) {
    _props.push_back(prop);
}

std::vector<ObjectPropExpr> ObjectLiteral::get_props() {
    return _props;
}

Variant ObjectLiteral::evaluate(Variant) {
    Object::SharedPtr obj = std::make_shared<Object>();
    for (auto prop : _props) {
        ObjectProp op = prop.specific_eval();
        obj->add_property(op.name, op.value);
    }

    return { obj };
}

std::vector<AstNode::SharedPtr> ObjectLiteral::get_children() {
    std::vector<AstNode::SharedPtr> res;
    for (int i = 0; i < _props.size(); i++) {
        res.emplace_back(std::make_shared<ObjectPropExpr>(_props.at(i)));
    }

    return res;
}

void ObjectLiteral::__dump(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }

    std::cout << "ObjectLiteral" << std::endl;

    for (auto prop : _props) {
        prop.__dump(depth + 1);
    }
}