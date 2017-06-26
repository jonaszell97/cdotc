//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_OPERATOR_H
#define CDOT_OPERATOR_H


#include <string>
#include "../AstNode.h"
#include "../Expression/Expression.h"
#include "../Statement/Statement.h"

class Operator : public Expression {
public:
    virtual std::string get_operator() = 0;
    virtual Variant evaluate(Variant = {}) = 0;
    virtual AstNode::SharedPtr clone() const = 0;

    typedef std::shared_ptr<Operator> SharedPtr;
    virtual void __dump(int) = 0;

protected:
    std::string _operator;
    std::string __class_name = "Operator";
};


#endif //CDOT_OPERATOR_H
