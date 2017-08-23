//
// Created by Jonas Zell on 20.06.17.
//

#include <iostream>
#include "FuncArgDecl.h"
#include "../../Expression/TypeRef.h"

FuncArgDecl::FuncArgDecl(std::string name, TypeRef::SharedPtr type, Expression::SharedPtr def) :
    argName(name),
    argType(type),
    defaultVal(def)
{
    if (defaultVal != nullptr) {
        children.push_back(&defaultVal);
    }
}

FuncArgDecl::FuncArgDecl(bool mut) : mut(mut) {

}

std::vector<AstNode::SharedPtr> FuncArgDecl::get_children() {
    if (defaultVal != nullptr) {
        return { defaultVal };
    }

    return {};
}

void FuncArgDecl::__dump(int depth) {
    AstNode::__tab(depth);

    std::cout << "FuncArgDecl [" << argType->toString() << " " << argName << "]" << std::endl;

    if (defaultVal != nullptr) {
        defaultVal->__dump(depth + 1);
    }
}