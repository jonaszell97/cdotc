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
    FunctionDecl(std::string, ValueType = ANY_T);
    FunctionDecl(const FunctionDecl& cp);
    virtual AstNode::SharedPtr clone() const;

    Variant evaluate(Variant = {});
    void set_return_type(ValueType);
    void set_body(CompoundStmt::SharedPtr);
    void add_arg(FuncArgDecl::SharedPtr);

    inline void set_context(Context::SharedPtr ctx) {
        context = ctx;
    }

    typedef std::shared_ptr<FunctionDecl> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class ContextVisitor;

protected:
    Context::SharedPtr context;
    std::string _func_name;
    ValueType _return_type;
    std::vector<FuncArgDecl::SharedPtr> _args;
    CompoundStmt::SharedPtr _body;
};


#endif //CDOT_FUNCTIONDECL_H
