//
// Created by Jonas Zell on 01.07.17.
//

#include "LambdaExpr.h"
#include "../../Statement/Declaration/FuncArgDecl.h"

LambdaExpr::LambdaExpr(std::shared_ptr<TypeRef> return_type, std::vector<std::shared_ptr<FuncArgDecl>> args) :
    _return_type(return_type),
    _args(args)
{

}

std::vector<AstNode::SharedPtr> LambdaExpr::get_children() {
    std::vector<AstNode::SharedPtr> children;
    for (auto arg : _args) {
        children.push_back(arg);
    }
    if (_body != nullptr) {
        children.push_back(_body);
    }
    if (memberExpr != nullptr) {
        children.push_back(memberExpr);
    }

    return children;
}

void LambdaExpr::__dump(int depth) {

}