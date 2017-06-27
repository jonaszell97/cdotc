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
    MethodCallExpr(std::string);
    MethodCallExpr(const MethodCallExpr& cp);
    MethodCallExpr(const CallExpr& cp, std::string);
    virtual AstNode::SharedPtr clone() const;

    typedef std::shared_ptr<MethodCallExpr> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class ContextVisitor;

protected:
    std::string _ident;

};


#endif //CDOT_METHODCALLEXPR_H
