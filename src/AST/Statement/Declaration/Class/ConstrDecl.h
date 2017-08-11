//
// Created by Jonas Zell on 27.06.17.
//

#ifndef CDOT_CONSTRDECL_H
#define CDOT_CONSTRDECL_H


#include "../../Statement.h"
#include "../FuncArgDecl.h"
#include "../../../Visitor/StaticAnalysis/Class.h"

class ConstrDecl : public Statement {
public:
    ConstrDecl();
    ConstrDecl(std::vector<FuncArgDecl::SharedPtr>, std::shared_ptr<CompoundStmt>, AccessModifier);

    typedef std::shared_ptr<ConstrDecl> SharedPtr;
    typedef std::unique_ptr<ConstrDecl> UniquePtr;

    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    inline virtual NodeType get_type() {
        return NodeType::CONSTR_DECL;
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
    bool implicit = false;
    AccessModifier am;
    std::vector<FuncArgDecl::SharedPtr> args;
    std::shared_ptr<CompoundStmt> body;
    bool declared = false;

    // codegen
    std::string this_binding;
    string class_name;
    cdot::cl::Method* method;
};


#endif //CDOT_CONSTRDECL_H
