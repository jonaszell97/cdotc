//
// Created by Jonas Zell on 01.08.17.
//

#include "TypedefDecl.h"

TypedefDecl::TypedefDecl(string alias, TypeRef::SharedPtr origin) : alias(alias), origin(origin) {

}

std::vector<AstNode::SharedPtr> TypedefDecl::get_children() {
    return {};
}

void TypedefDecl::__dump(int depth) {
    AstNode::__tab(depth);

    std::cout << "TypedefDecl [" << origin->to_string() << " -> " << alias << "]" << std::endl;
}