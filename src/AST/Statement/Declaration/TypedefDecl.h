//
// Created by Jonas Zell on 01.08.17.
//

#ifndef CDOT_TYPEDEFDECL_H
#define CDOT_TYPEDEFDECL_H


#include "../Statement.h"
#include "../../Expression/TypeRef.h"

class TypedefDecl : public Statement {
public:
    TypedefDecl(string, TypeRef::SharedPtr);

    typedef std::shared_ptr<TypedefDecl> SharedPtr;
    typedef std::unique_ptr<TypedefDecl> UniquePtr;

    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    inline virtual NodeType get_type() {
        return NodeType::TYPEDEF_DECL;
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
    string alias;
    TypeRef::SharedPtr origin;
};


#endif //CDOT_TYPEDEFDECL_H
