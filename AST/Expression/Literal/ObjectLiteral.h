//
// Created by Jonas Zell on 20.06.17.
//

#ifndef MATHPARSER_OBJECTLITERAL_H
#define MATHPARSER_OBJECTLITERAL_H


#include "../../../Objects/Object.h"
#include "../Expression.h"
#include "ObjectPropExpr.h"

class ObjectLiteral : public Expression {
public:
    ObjectLiteral();
    void add_prop(ObjectPropExpr);
    std::vector<ObjectPropExpr> get_props();
    Variant evaluate(Variant = {});
    std::vector<AstNode::SharedPtr> get_children();

    typedef std::shared_ptr<ObjectLiteral> SharedPtr;
    void __dump(int);

protected:
    std::vector<ObjectPropExpr> _props;
    std::string _name;
    std::string __class_name = "ObjectLiteral";
};


#endif //MATHPARSER_OBJECTLITERAL_H
