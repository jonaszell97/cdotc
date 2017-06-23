//
// Created by Jonas Zell on 20.06.17.
//

#include <iostream>
#include "ObjectPropExpr.h"
#include "../../../Util.h"

ObjectPropExpr::ObjectPropExpr(std::string prop_name, Expression::SharedPtr prop_val, ValueType type) :
    _prop_name(prop_name),
    _prop_val(prop_val),
    _prop_type(type)
{

}

VariantPtr ObjectPropExpr::evaluate(VariantPtr) {
    return Variant::from(Variant{ std::make_shared<ObjectProperty>(_prop_name, _prop_val->evaluate(), _prop_type) });
}

std::vector<AstNode::SharedPtr> ObjectPropExpr::get_children() {
    std::vector<AstNode::SharedPtr> res;
    res.push_back(_prop_val);

    return res;
}

void ObjectPropExpr::__dump(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }
    std::cout << "ObjectPropExpr" << " [" << (_prop_type == AUTO_T ? "auto" : util::types[_prop_type])
              << " " << _prop_name << "]" << std::endl;

    _prop_val->__dump(depth + 1);
}