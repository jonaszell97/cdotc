//
// Created by Jonas Zell on 09.07.17.
//

#ifndef CDOT_INTERFACEDECL_H
#define CDOT_INTERFACEDECL_H


#include "../../Statement.h"
#include "FieldDecl.h"
#include "MethodDecl.h"
#include "OperatorDecl.h"

class InterfaceDecl : public Statement {
public:
    InterfaceDecl(std::string);
    void add_field(FieldDecl::SharedPtr);
    void add_method(MethodDecl::SharedPtr);
    void add_operator(OperatorDecl::SharedPtr);

    typedef std::shared_ptr<InterfaceDecl> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    inline virtual NodeType get_type() {
        return NodeType::INTERFACE_DECL;
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
    friend class TypeCheckVisitor;

protected:
    std::string name;

    std::vector<FieldDecl::SharedPtr> fields = {};
    std::vector<MethodDecl::SharedPtr> methods = {};
    std::vector<OperatorDecl::SharedPtr> operators = {};
};


#endif //CDOT_INTERFACEDECL_H
