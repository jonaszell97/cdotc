//
// Created by Jonas Zell on 19.06.17.
//

#ifndef EXPRESSION_H
#define EXPRESSION_H


#include "../AstNode.h"
#include "../Statement/Statement.h"

class Expression : public Statement {
public:
    typedef std::shared_ptr<Expression> SharedPtr;

    Expression();
    Expression(Expression::SharedPtr);
    void set_child(Expression::SharedPtr);
    Variant evaluate(Variant = {});
    std::vector<AstNode::SharedPtr> get_children();

    virtual void __dump(int) = 0;

protected:
    Expression::SharedPtr _child;
};


#endif //EXPRESSION_H
