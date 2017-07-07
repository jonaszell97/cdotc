//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_METHODCALLEXPR_H
#define CDOT_METHODCALLEXPR_H


#include "../../Visitor/Visitor.h"
#include "RefExpr.h"
#include "CallExpr.h"

class MethodCallExpr : public CallExpr {
public:
    MethodCallExpr(std::string, std::vector<Expression::SharedPtr>);

    typedef std::shared_ptr<MethodCallExpr> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    friend class TypeCheckVisitor;

protected:
    std::vector<Expression::SharedPtr> _arguments;
    std::string _ident;

};


#endif //CDOT_METHODCALLEXPR_H
