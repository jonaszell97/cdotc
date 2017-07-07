//
// Created by Jonas Zell on 21.06.17.
//

#ifndef CDOT_REFEXPR_H
#define CDOT_REFEXPR_H


#include "../../../Variant/Variant.h"
#include "../../AstNode.h"
#include "../Expression.h"

class RefExpr : public Expression {
public:
    virtual inline void return_ref(bool ref) {
        _return_ref = ref;
    }
    virtual inline void implicit_ref(bool implicit) {
        _implicit_ref = implicit;
    }
    virtual inline void set_member_expr(std::shared_ptr<RefExpr> ref_expr) {
        _member_expr = ref_expr;
    }

    typedef std::shared_ptr<RefExpr> SharedPtr;
    virtual std::vector<AstNode::SharedPtr> get_children() = 0;
    virtual void __dump(int) = 0;

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    friend class TypeCheckVisitor;

protected:
    bool _implicit_ref = false;
    RefExpr::SharedPtr _member_expr;
    bool _return_ref = false;
};


#endif //CDOT_REFEXPR_H
