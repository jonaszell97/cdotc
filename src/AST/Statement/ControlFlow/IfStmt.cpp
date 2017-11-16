//
// Created by Jonas Zell on 19.06.17.
//

#include "IfStmt.h"
#include "../../Expression/Expression.h"

IfStmt::IfStmt(Expression::SharedPtr cond, Statement::SharedPtr if_branch) :
    condition(cond),
    ifBranch(if_branch),
    elseBranch{}
{
    children.push_back(&cond);
}

IfStmt::IfStmt(Expression::SharedPtr cond) : condition(cond), ifBranch{}, elseBranch{} {
    children.push_back(&cond);
}

std::vector<AstNode::SharedPtr> IfStmt::get_children() {
    std::vector<AstNode::SharedPtr> children;
    children.push_back(condition);

    if (ifBranch != nullptr) {
        children.push_back(ifBranch);
    }
    if (elseBranch != nullptr) {
        children.push_back(elseBranch);
    }

    return children;
}