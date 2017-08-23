//
// Created by Jonas Zell on 21.06.17.
//

#include "CollectionLiteral.h"
#include "../TypeRef.h"

CollectionLiteral::CollectionLiteral(std::shared_ptr<TypeRef>  ts) :
    type(ts),
    elements(std::vector<Expression::SharedPtr>())
{

}

void CollectionLiteral::add_element(Expression::SharedPtr el) {
    elements.push_back(el);
    children.push_back(&elements.back());
}

std::vector<AstNode::SharedPtr> CollectionLiteral::get_children() {
    std::vector<AstNode::SharedPtr> res;

    for (auto el : elements) {
        res.push_back(el);
    }

    if (memberExpr != nullptr) {
        res.push_back(memberExpr);
    }

    return res;
}

void CollectionLiteral::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "CollectionLiteral [" << type->toString() << "]" << std::endl;

    for (auto c : elements) {
        c->__dump(depth + 1);
    }
}