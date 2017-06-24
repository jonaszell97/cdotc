//
// Created by Jonas Zell on 20.06.17.
//

#ifndef MATHPARSER_FUNCTIONDECL_H
#define MATHPARSER_FUNCTIONDECL_H


#include "../Statement.h"
#include "FuncArgDecl.h"
#include "../CompoundStmt.h"

class FunctionDecl : public Statement {
public:
    FunctionDecl(std::string, ValueType = ANY_T);
    Variant evaluate(Variant = {});
    void set_return_type(ValueType);
    void set_body(CompoundStmt::SharedPtr);
    void add_arg(FuncArgDecl::SharedPtr);

    typedef std::shared_ptr<FunctionDecl> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

protected:
    std::string _func_name;
    ValueType _return_type;
    std::vector<FuncArgDecl::SharedPtr> _args;
    CompoundStmt::SharedPtr _body;
};


#endif //MATHPARSER_FUNCTIONDECL_H
