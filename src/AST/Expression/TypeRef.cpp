//
// Created by Jonas Zell on 06.08.17.
//

#include "TypeRef.h"

TypeRef::TypeRef() : type(new AutoType) {

}

TypeRef::TypeRef(Type* t) : type(t) {

}

string TypeRef::toString() {
    return type->toString();
}

std::vector<AstNode::SharedPtr> TypeRef::get_children() {
    std::vector<AstNode::SharedPtr> children;

    if (isa<PointerType>(type)) {
        auto asPtr = cast<PointerType>(type);
        if (asPtr->getLengthExpr() != nullptr) {
            children.push_back(asPtr->getLengthExpr());
        }
    }

    return children;
}

void TypeRef::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "TypeRef [" << type->toString() << "]" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}