//
// Created by Jonas Zell on 27.06.17.
//

#ifndef CDOT_MEMBERDECL_H
#define CDOT_MEMBERDECL_H


#include "../../Statement.h"
#include "../FuncArgDecl.h"
#include "../../CompoundStmt.h"

class MethodDecl : public Statement {
public:
    MethodDecl(std::string, TypeSpecifier, std::vector<FuncArgDecl::SharedPtr>, CompoundStmt::SharedPtr,
               AccessModifier = AccessModifier::PUBLIC, bool = false);
    MethodDecl(const MethodDecl& cp);
    virtual AstNode::SharedPtr clone() const;

    typedef std::shared_ptr<MethodDecl> SharedPtr;
    typedef std::unique_ptr<MethodDecl> UniquePtr;

    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;

protected:
    bool is_static;
    AccessModifier am;
    std::string method_name;
    TypeSpecifier return_type;
    std::vector<FuncArgDecl::SharedPtr> args;
    CompoundStmt::SharedPtr body;
};


#endif //CDOT_MEMBERDECL_H
