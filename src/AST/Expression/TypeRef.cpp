//
// Created by Jonas Zell on 06.08.17.
//

#include "TypeRef.h"


TypeRef::TypeRef() : type(TypeSpecifier(AUTO_T)) {

}

TypeRef::TypeRef(TypeSpecifier t) : type(t) {

}

string TypeRef::to_string() {
    return type.to_string();
}

std::vector<AstNode::SharedPtr> TypeRef::get_children() {
    return {};
}

void TypeRef::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "TypeRef [" << type.to_string() << "]" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}