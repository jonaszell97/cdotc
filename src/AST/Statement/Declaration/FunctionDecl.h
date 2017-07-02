//
// Created by Jonas Zell on 20.06.17.
//

#ifndef CDOT_FUNCTIONDECL_H
#define CDOT_FUNCTIONDECL_H


#include "../Statement.h"
#include "FuncArgDecl.h"
#include "../CompoundStmt.h"

class FunctionDecl : public Statement {
public:
    FunctionDecl(std::string, TypeSpecifier = { ANY_T });
    FunctionDecl(const FunctionDecl& cp);
    virtual AstNode::SharedPtr clone() const;

    inline void set_return_type(TypeSpecifier type) {
        _return_type = type;
    }
    inline void set_body(CompoundStmt::SharedPtr body) {
        _body = body;
    }
    void add_arg(FuncArgDecl::SharedPtr);

    typedef std::shared_ptr<FunctionDecl> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;

protected:
    std::string _func_name;
    TypeSpecifier _return_type;
    std::vector<FuncArgDecl::SharedPtr> _args;
    CompoundStmt::SharedPtr _body;
};


#endif //CDOT_FUNCTIONDECL_H
