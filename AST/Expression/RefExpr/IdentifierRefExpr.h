//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_IDENTIFIEREXPRESSION_H
#define CDOT_IDENTIFIEREXPRESSION_H


#include <string>
#include "../Expression.h"
#include "RefExpr.h"
#include "../../Context.h"

class MemberRefExpr;

class IdentifierRefExpr : public RefExpr {
public:
    IdentifierRefExpr(std::string);
    IdentifierRefExpr(Variant);
    IdentifierRefExpr(const IdentifierRefExpr& cp);
    virtual AstNode::SharedPtr clone() const;
    Variant evaluate(Variant = {});

    inline void set_context(Context::SharedPtr ctx) {
        context = ctx;
    }

    typedef std::shared_ptr<IdentifierRefExpr> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();

    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class ContextVisitor;

protected:
    Context::SharedPtr context;
    std::string _ident;
};


#endif //CDOT_IDENTIFIEREXPRESSION_H
