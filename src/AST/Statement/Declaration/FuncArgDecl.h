//
// Created by Jonas Zell on 20.06.17.
//

#ifndef CDOT_FUNCARGDECL_H
#define CDOT_FUNCARGDECL_H

#include "../../Expression/Expression.h"

class TypeRef;

class FuncArgDecl : public Expression {
public:
    explicit FuncArgDecl(bool = false);
    FuncArgDecl(std::string, std::shared_ptr<TypeRef>, Expression::SharedPtr = {});
    
    void setName(string name_) {
        argName = name_;
    }

    void setType(std::shared_ptr<TypeRef> type) {
        argType = type;
    }

    std::shared_ptr<TypeRef> getArgType() {
        return argType;
    }

    void setDefault(Expression::SharedPtr def) {
        defaultVal = def;
    }

    typedef std::shared_ptr<FuncArgDecl> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children() override;
    void __dump(int depth) override;

    NodeType get_type() override {
        return NodeType::FUNC_ARG_DECL;
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
    std::string argName;
    std::shared_ptr<TypeRef> argType;
    std::shared_ptr<Expression> defaultVal;
    bool mut = false;
    bool isStruct = false;
    bool needsAlloca = false; // for passing a struct by reference
};


#endif //CDOT_FUNCARGDECL_H
