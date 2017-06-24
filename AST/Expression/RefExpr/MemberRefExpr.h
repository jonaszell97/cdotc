//
// Created by Jonas Zell on 20.06.17.
//

#ifndef MATHPARSER_MEMBERREFEXPR_H
#define MATHPARSER_MEMBERREFEXPR_H


#include <string>
#include "../../../Variant/Variant.h"
#include "../Expression.h"
#include "IdentifierRefExpr.h"
#include "RefExpr.h"

class MemberRefExpr : public RefExpr {
public:
    MemberRefExpr(std::string);
    MemberRefExpr(Variant);
    Variant evaluate(Variant = {});
    void return_ref(bool);

    typedef std::shared_ptr<MemberRefExpr> SharedPtr;
    void set_member_expr(std::shared_ptr<RefExpr>);
    std::vector<AstNode::SharedPtr> get_children();

    void __dump(int);

protected:
    std::string _ident;
    std::string __class_name = "MemberRefExpr";
    RefExpr::SharedPtr _member_expr;
    std::shared_ptr<Object> _parent_obj;
    bool _return_ref = false;
};


#endif //MATHPARSER_MEMBERREFEXPR_H
