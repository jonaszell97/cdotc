//
// Created by Jonas Zell on 20.06.17.
//

#include <iostream>
#include "ObjectPropExpr.h"
#include "../../../Util.h"

ObjectPropExpr::ObjectPropExpr(std::string prop_name, Expression::SharedPtr prop_val, TypeSpecifier type) :
    _prop_name(prop_name),
    _prop_val(prop_val),
    _prop_type(type)
{

}

ObjectPropExpr::ObjectPropExpr(std::string prop_name, TypeSpecifier type) : _prop_name(prop_name), _prop_type(type),
                                                                            _prop_val{} {

}

std::vector<AstNode::SharedPtr> ObjectPropExpr::get_children() {
    return { _prop_val };
}

void ObjectPropExpr::__dump(int depth) {
    for (int i = 0; i < depth; i++) {
        std::cout << "\t";
    }
    std::cout << "ObjectPropExpr" << " [" << (_prop_type == AUTO_T ? "auto" : val::typetostr(_prop_type))
              << " " << _prop_name << "]" << std::endl;

    _prop_val->__dump(depth + 1);
}