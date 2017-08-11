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

class MemberRefExpr : public Expression {
public:
    MemberRefExpr(std::string);
    MemberRefExpr(Variant);

    typedef std::shared_ptr<MemberRefExpr> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();

    void __dump(int);

    inline virtual NodeType get_type() {
        return NodeType::MEMBER_EXPR;
    }
    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }
    virtual inline CGValue accept(CodeGenVisitor& v) {
        return v.visit(this);
    }
    virtual TypeSpecifier accept(TypeCheckVisitor& v) {
        return v.visit(this);
    }


    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    friend class ConstExprVisitor;
    friend class CodeGenVisitor;
    friend class TypeCheckVisitor;

protected:
    std::string _ident;

    // codegen
    std::string class_name;
    TypeSpecifier field_type;
    bool is_static = false;
    bool is_ns_member = false;
    TypeSpecifier generic_return_type;
    bool needs_generic_cast = false;


};


#endif //CDOT_MEMBERREFEXPR_H
