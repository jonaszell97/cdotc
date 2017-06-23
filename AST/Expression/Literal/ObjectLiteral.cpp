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

VariantPtr ObjectLiteral::evaluate(VariantPtr) {
    std::shared_ptr<Object> obj(new Object);
    for (auto prop : _props) {
        obj->add_property(prop.evaluate()->op_val);
    }

    return std::make_shared<Variant>(obj);
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