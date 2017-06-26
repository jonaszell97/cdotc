//
// Created by Jonas Zell on 20.06.17.
//

#ifndef CDOT_OBJECTLITERAL_H
#define CDOT_OBJECTLITERAL_H


#include "../../../StdLib/Objects/Object.h"
#include "../Expression.h"
#include "ObjectPropExpr.h"

class ObjectLiteral : public Expression {
public:
    ObjectLiteral();
    ObjectLiteral(const ObjectLiteral& cp);
    virtual AstNode::SharedPtr clone() const;
    void add_prop(ObjectPropExpr);
    std::vector<ObjectPropExpr::SharedPtr> get_props();
    Variant evaluate(Variant = {});
    std::vector<AstNode::SharedPtr> get_children();

    inline void set_class_prototype(Class* proto) {
        _class_prototype = proto;
    }

    typedef std::shared_ptr<ObjectLiteral> SharedPtr;
    void __dump(int);

    virtual inline void visit(Visitor& v, VisitorFlag f = VisitorFlag::NONE) {
        v.accept(this, f);
    }

    friend class Visitor;

protected:
    Class* _class_prototype;
    std::vector<ObjectPropExpr::SharedPtr> _props;
    std::string _name;
    std::string __class_name = "ObjectLiteral";
};


#endif //CDOT_OBJECTLITERAL_H
