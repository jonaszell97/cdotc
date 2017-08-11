//
// Created by Jonas Zell on 01.07.17.
//

#ifndef CDOT_OPERATORDECL_H
#define CDOT_OPERATORDECL_H


#include "../../Statement.h"
#include "../../Block/CompoundStmt.h"
#include "../FuncArgDecl.h"

class OperatorDecl : public Statement {
public:
    OperatorDecl(std::string, std::vector<FuncArgDecl::SharedPtr>, AccessModifier, bool);

    inline void set_body(CompoundStmt::SharedPtr _body) {
        body = _body;
    }
    inline std::string get_operator() {
        return _operator;
    }
    inline void set_return_type(TypeSpecifier ts) {
        return_type = ts;
    }
    bool is_binary;

    typedef std::shared_ptr<OperatorDecl> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    inline virtual NodeType get_type() {
        return NodeType::OPERATOR_DECL;
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
    TypeSpecifier return_type;
    AccessModifier am;
    std::string _operator;
    std::vector<FuncArgDecl::SharedPtr> args;
    CompoundStmt::SharedPtr body;
};


#endif //CDOT_OPERATORDECL_H
