//
// Created by Jonas Zell on 21.06.17.
//

#ifndef MATHPARSER_CALLEXPR_H
#define MATHPARSER_CALLEXPR_H


#include "../Expression.h"
#include "RefExpr.h"

class CallExpr : public RefExpr {
public:
    CallExpr();
    Variant evaluate(Variant = {});
    void add_argument(Expression::SharedPtr);
    void return_ref(bool);
    void set_member_expr(std::shared_ptr<RefExpr>);

    typedef std::shared_ptr<CallExpr> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);
protected:
    std::vector<Expression::SharedPtr> _arguments;
    std::shared_ptr<RefExpr> _member_expr;
    bool _return_ref;
};


#endif //MATHPARSER_CALLEXPR_H
