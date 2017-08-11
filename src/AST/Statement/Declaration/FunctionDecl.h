//
// Created by Jonas Zell on 20.06.17.
//

#ifndef CDOT_FUNCTIONDECL_H
#define CDOT_FUNCTIONDECL_H


#include "../Statement.h"
#include "FuncArgDecl.h"
#include "../Block/CompoundStmt.h"

class FunctionDecl : public Statement {
public:
    FunctionDecl(std::string, TypeRef::SharedPtr = nullptr);

    inline void set_return_type(TypeRef::SharedPtr type) {
        _return_type = type;
    }
    inline void set_body(CompoundStmt::SharedPtr body) {
        _body = body;
    }
    void add_arg(FuncArgDecl::SharedPtr);

    typedef std::shared_ptr<FunctionDecl> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    inline virtual NodeType get_type() {
        return NodeType::FUNCTION_DECL;
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
    std::string _func_name;
    TypeRef::SharedPtr _return_type;
    std::vector<FuncArgDecl::SharedPtr> _args;
    CompoundStmt::SharedPtr _body;

    // codegen
    std::vector<std::pair<std::string,std::string>> captures = {};
    std::vector<TypeSpecifier> capture_types = {};
    std::vector<std::string> copy_targets = {};
};


#endif //CDOT_FUNCTIONDECL_H
