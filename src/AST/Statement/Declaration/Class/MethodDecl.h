//
// Created by Jonas Zell on 27.06.17.
//

#ifndef CDOT_MEMBERDECL_H
#define CDOT_MEMBERDECL_H


#include "../../Statement.h"
#include "../FuncArgDecl.h"
#include "../../Block/CompoundStmt.h"
#include "../../../Visitor/StaticAnalysis/Class.h"

class MethodDecl : public Statement {
public:
    MethodDecl(std::string, TypeRef::SharedPtr, std::vector<FuncArgDecl::SharedPtr>, CompoundStmt::SharedPtr,
               AccessModifier = AccessModifier::PUBLIC, bool = false);
    MethodDecl(std::string, TypeRef::SharedPtr, std::vector<FuncArgDecl::SharedPtr>, AccessModifier =
    AccessModifier::PUBLIC, bool = false);

    typedef std::shared_ptr<MethodDecl> SharedPtr;
    typedef std::unique_ptr<MethodDecl> UniquePtr;

    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    inline virtual NodeType get_type() {
        return NodeType::METHOD_DECL;
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
    bool is_static;
    bool is_abstract;
    AccessModifier am;
    std::string method_name;
    TypeRef::SharedPtr return_type;
    std::vector<FuncArgDecl::SharedPtr> args;
    CompoundStmt::SharedPtr body;

    bool declared = false;

    // codegen
    std::string class_name;
    std::string this_binding;
    bool is_virtual = false;
    llvm::Function* cg_function;
    cdot::cl::Method* method;
};


#endif //CDOT_MEMBERDECL_H
