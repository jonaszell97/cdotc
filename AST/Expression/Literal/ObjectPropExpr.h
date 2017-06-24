//
// Created by Jonas Zell on 20.06.17.
//

#ifndef MATHPARSER_OBJECTPROPEXPR_H
#define MATHPARSER_OBJECTPROPEXPR_H


#include "../Expression.h"
#include "../../../Objects/Object.h"

struct ObjectProp {
    std::string name;
    Variant::SharedPtr value;
};

class ObjectPropExpr : public Expression {
public:
    ObjectPropExpr(std::string, Expression::SharedPtr, ValueType = ANY_T);
    Variant evaluate(Variant = {});
    ObjectProp specific_eval();

    typedef std::shared_ptr<ObjectPropExpr> SharedPtr;
    void __dump(int);
    std::vector<AstNode::SharedPtr> get_children();

protected:
    std::string _prop_name;
    Expression::SharedPtr _prop_val;
    ValueType _prop_type;
    std::string __class_name = "ObjectPropExpr";
};


#endif //MATHPARSER_OBJECTPROPEXPR_H
