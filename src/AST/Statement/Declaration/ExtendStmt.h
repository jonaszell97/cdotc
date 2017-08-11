//
// Created by Jonas Zell on 31.07.17.
//

#ifndef CDOT_EXTENDSTMT_H
#define CDOT_EXTENDSTMT_H


#include "../../AstNode.h"
#include "Class/MethodDecl.h"
#include "Class/FieldDecl.h"

class ExtendStmt : public Statement {
public:
    ExtendStmt(std::string, std::vector<FieldDecl::SharedPtr>, std::vector<MethodDecl::SharedPtr>);

    typedef std::shared_ptr<ExtendStmt> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    inline virtual NodeType get_type() {
        return NodeType::FUNC_ARG_DECL;
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
    std::string extended_class;
    std::vector<FieldDecl::SharedPtr> fields;
    std::vector<MethodDecl::SharedPtr> methods;
};


#endif //CDOT_EXTENDSTMT_H
