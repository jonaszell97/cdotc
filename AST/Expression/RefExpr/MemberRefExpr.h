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
    Variant evaluate(Variant = {});

    typedef std::shared_ptr<MemberRefExpr> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();

    void __dump(int);

    virtual inline void visit(Visitor& v, VisitorFlag f = VisitorFlag::NONE) {
        v.accept(this, f);
    }

    friend class Visitor;

protected:
    std::string _ident;
    std::string __class_name = "MemberRefExpr";
};


#endif //CDOT_MEMBERREFEXPR_H
