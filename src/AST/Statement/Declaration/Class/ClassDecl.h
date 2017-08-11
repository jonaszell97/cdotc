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
#include "../TypedefDecl.h"

class ClassDecl : public Expression {
public:
    ClassDecl(std::string, std::vector<FieldDecl::SharedPtr>, std::vector<MethodDecl::SharedPtr>,
        std::vector<ConstrDecl::SharedPtr>, std::vector<TypedefDecl::SharedPtr>, std::vector<pair<string, TypeSpecifier>>,
        AccessModifier = AccessModifier::PUBLIC, bool = false, std::string = "", std::vector<std::string> = {});

    typedef std::shared_ptr<ClassDecl> SharedPtr;
    typedef std::unique_ptr<ClassDecl> UniquePtr;

    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    inline virtual NodeType get_type() {
        return NodeType::CLASS_DECL;
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
    friend class cdot::cl::Class;

protected:
    std::string extends;
    std::vector<std::string> implements;

    bool is_abstract;
    AccessModifier am;
    std::string class_name;

    std::vector<ConstrDecl::SharedPtr> constructors;
    std::vector<FieldDecl::SharedPtr> fields;
    std::vector<MethodDecl::SharedPtr> methods;
    std::vector<TypedefDecl::SharedPtr> typedefs;

    std::vector<pair<string, TypeSpecifier>> generics;

    // codegen
    std::string this_binding;
    bool needs_vtable;
    cdot::cl::Class* class_decl;
};


#endif //CDOT_CLASSDECLEXPR_H
