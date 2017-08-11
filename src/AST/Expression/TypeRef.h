//
// Created by Jonas Zell on 06.08.17.
//

#ifndef CDOT_TYPEREF_H
#define CDOT_TYPEREF_H


#include "../Statement/Statement.h"

class TypeRef : public Statement {
public:
    TypeRef();
    TypeRef(TypeSpecifier);

    string to_string();

    inline TypeSpecifier& getTypeSpecifier() {
        return type;
    }

    inline void setTypeSpeicifer(TypeSpecifier t) {
        type = t;
    }

    typedef std::shared_ptr<TypeRef> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children() override;
    void __dump(int) override;

    inline virtual NodeType get_type() override {
        return NodeType::TYPE_REF;
    }
    virtual inline Variant accept(Visitor& v) override {
        return v.visit(this);
    }
    virtual inline CGValue accept(CodeGenVisitor& v) override {
        return v.visit(this);
    }
    virtual TypeSpecifier accept(TypeCheckVisitor& v) override {
        return v.visit(this);
    }


    friend class Visitor;
    friend class EvaluatingVisitor;
    friend class CaptureVisitor;
    friend class ConstExprVisitor;
    friend class CodeGenVisitor;
    friend class TypeCheckVisitor;
    friend class MethodCallExpr;

protected:
    TypeSpecifier type;
};


#endif //CDOT_TYPEREF_H
