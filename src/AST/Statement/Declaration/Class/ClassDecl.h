//
// Created by Jonas Zell on 27.06.17.
//

#ifndef CDOT_CLASSDECLEXPR_H
#define CDOT_CLASSDECLEXPR_H


#include "../../../Expression/Expression.h"
#include "FieldDecl.h"
#include "MethodDecl.h"
#include "ConstrDecl.h"
#include "OperatorDecl.h"

class ClassDecl : public Expression {
public:
    ClassDecl(std::string, std::vector<FieldDecl::SharedPtr>, std::vector<MethodDecl::SharedPtr>, ConstrDecl::SharedPtr = {},
        AccessModifier = AccessModifier::PUBLIC, std::vector<std::pair<std::string, OperatorDecl::SharedPtr>> = {},
            std::vector<std::pair<std::string, OperatorDecl::SharedPtr>> = {}, bool = false, bool = false, std::string =
    "",
            std::vector<std::string> = {});

    typedef std::shared_ptr<ClassDecl> SharedPtr;
    typedef std::unique_ptr<ClassDecl> UniquePtr;

    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    friend class TypeCheckVisitor;

protected:
    std::string extends;
    std::vector<std::string> implements;

    bool is_interface;
    bool is_abstract;
    AccessModifier am;
    std::string class_name;
    ConstrDecl::SharedPtr constr;
    std::vector<FieldDecl::SharedPtr> fields;
    std::vector<MethodDecl::SharedPtr> methods;
    std::vector<std::pair<std::string, OperatorDecl::SharedPtr>> unary_operators;
    std::vector<std::pair<std::string, OperatorDecl::SharedPtr>> binary_operators;
};


#endif //CDOT_CLASSDECLEXPR_H
