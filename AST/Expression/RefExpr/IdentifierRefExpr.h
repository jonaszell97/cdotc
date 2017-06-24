//
// Created by Jonas Zell on 19.06.17.
//

#ifndef MATHPARSER_IDENTIFIEREXPRESSION_H
#define MATHPARSER_IDENTIFIEREXPRESSION_H


#include <string>
#include "../Expression.h"
#include "RefExpr.h"

class MemberRefExpr;

class IdentifierRefExpr : public RefExpr {
public:
    IdentifierRefExpr(std::string);
    IdentifierRefExpr(Variant);
    Variant evaluate(Variant = {});
    void return_ref(bool);

    typedef std::shared_ptr<IdentifierRefExpr> SharedPtr;
    void set_member_expr(std::shared_ptr<RefExpr>);
    std::vector<AstNode::SharedPtr> get_children();

    void __dump(int);

protected:
    std::string _ident;
    std::string __class_name = "IdentifierRefExpr";
    std::shared_ptr<RefExpr> _member_expr;
    bool _return_ref = false;
};


#endif //MATHPARSER_IDENTIFIEREXPRESSION_H
