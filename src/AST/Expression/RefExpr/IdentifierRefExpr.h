//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_IDENTIFIEREXPRESSION_H
#define CDOT_IDENTIFIEREXPRESSION_H

#include "../Expression.h"

class MemberRefExpr;

class IdentifierRefExpr : public Expression {
public:
    explicit IdentifierRefExpr(string);

    typedef std::shared_ptr<IdentifierRefExpr> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children() override;

    void __dump(int) override ;

    NodeType get_type() override {
        return NodeType::IDENTIFIER_EXPR;
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
    // codegen
    bool isNonMutableArg = false;

    bool isCapturedVar = false;
    bool isNamespace = false;
    bool isSuper = false;
    string superClassName;
};


#endif //CDOT_IDENTIFIEREXPRESSION_H
