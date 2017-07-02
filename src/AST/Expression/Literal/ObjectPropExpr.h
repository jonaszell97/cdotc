//
// Created by Jonas Zell on 20.06.17.
//

#ifndef CDOT_OBJECTPROPEXPR_H
#define CDOT_OBJECTPROPEXPR_H


#include "../Expression.h"
#include "../../../StdLib/Objects/Object.h"

struct ObjectProp {
    std::string name;
    Variant::SharedPtr value;
};

class ObjectPropExpr : public Expression {
public:
    ObjectPropExpr(std::string, Expression::SharedPtr, TypeSpecifier);
    ObjectPropExpr(const ObjectPropExpr& cp);
    virtual AstNode::SharedPtr clone() const;

    typedef std::shared_ptr<ObjectPropExpr> SharedPtr;
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    std::vector<AstNode::SharedPtr> get_children();

protected:
    std::string _prop_name;
    Expression::SharedPtr _prop_val;
    TypeSpecifier _prop_type;
    std::string __class_name = "ObjectPropExpr";
};


#endif //CDOT_OBJECTPROPEXPR_H
