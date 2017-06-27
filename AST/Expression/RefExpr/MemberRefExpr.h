//
// Created by Jonas Zell on 20.06.17.
//

#ifndef CDOT_MEMBERREFEXPR_H
#define CDOT_MEMBERREFEXPR_H


#include <string>
#include "../../../Variant/Variant.h"
#include "../Expression.h"
#include "IdentifierRefExpr.h"
#include "RefExpr.h"

class MemberRefExpr : public RefExpr {
public:
    MemberRefExpr(std::string);
    MemberRefExpr(Variant);
    MemberRefExpr(const MemberRefExpr& cp);
    virtual AstNode::SharedPtr clone() const;

    typedef std::shared_ptr<MemberRefExpr> SharedPtr;
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
    std::string __class_name = "MemberRefExpr";
};


#endif //CDOT_MEMBERREFEXPR_H
