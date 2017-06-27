//
// Created by Jonas Zell on 27.06.17.
//

#ifndef CDOT_CLASSDECLEXPR_H
#define CDOT_CLASSDECLEXPR_H


#include "../../../Expression/Expression.h"
#include "FieldDecl.h"
#include "MethodDecl.h"
#include "ConstrDecl.h"

class ClassDecl : public Expression {
public:
    ClassDecl(std::string, std::vector<FieldDecl::SharedPtr>, std::vector<MethodDecl::SharedPtr>, ConstrDecl::SharedPtr = {},
        AccessModifier = AccessModifier::PUBLIC);
    ClassDecl(const ClassDecl& cp);
    virtual AstNode::SharedPtr clone() const;

    typedef std::shared_ptr<ClassDecl> SharedPtr;
    typedef std::unique_ptr<ClassDecl> UniquePtr;

    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class ContextVisitor;

protected:
    AccessModifier am;
    std::string class_name;
    ConstrDecl::SharedPtr constr;
    std::vector<FieldDecl::SharedPtr> fields;
    std::vector<MethodDecl::SharedPtr> methods;
};


#endif //CDOT_CLASSDECLEXPR_H
