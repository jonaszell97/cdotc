//
// Created by Jonas Zell on 27.06.17.
//

#ifndef CDOT_CLASSMEMBERDECL_H
#define CDOT_CLASSMEMBERDECL_H


#include "../../Statement.h"
#include "../../../Expression/Expression.h"

class FieldDecl : public Statement {
public:
    FieldDecl(std::string, TypeSpecifier, AccessModifier = AccessModifier::PUBLIC, bool = false, Expression::SharedPtr = {});

    inline void set_default(Expression::SharedPtr expr) {
        default_val = expr;
    }

    typedef std::shared_ptr<FieldDecl> SharedPtr;
    typedef std::unique_ptr<FieldDecl> UniquePtr;

    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }
    virtual inline void generate(bool get, bool set) {
        generate_getter = get;
        generate_setter = set;
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    friend class TypeCheckVisitor;

protected:
    bool generate_getter = false;
    bool generate_setter = false;
    bool is_static;
    AccessModifier am;
    TypeSpecifier type;
    std::string field_name;
    Expression::SharedPtr default_val;
};


#endif //CDOT_CLASSMEMBERDECL_H
