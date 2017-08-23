//
// Created by Jonas Zell on 06.08.17.
//

#ifndef CDOT_TYPEREF_H
#define CDOT_TYPEREF_H


#include "../Expression/Expression.h"

class TypeRef : public Expression {
public:
    explicit TypeRef();
    explicit TypeRef(Type*);

    ~TypeRef() override {

    }

    string toString();

    inline Type*& getType(bool force = false) {
        assert((force || resolved) && "Resolve type before accessing!");
        return type;
    }

    inline void setType(Type* t) {
        if (type == t) {
            return;
        }

        delete type;
        type = t;
    }

    typedef std::shared_ptr<TypeRef> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children() override;
    void __dump(int) override;

    NodeType get_type() override {
        return NodeType::TYPE_REF;
    }

    llvm::Value* accept(CodeGenVisitor& v) override {
        return v.visit(this);
    }

    Type* accept(TypeCheckVisitor& v) override {
        return v.visit(this);
    }

    friend class ConstExprVisitor;
    friend class CodeGenVisitor;
    friend class TypeCheckVisitor;

protected:
    bool resolved = false;
    Type* type = nullptr;
};


#endif //CDOT_TYPEREF_H
