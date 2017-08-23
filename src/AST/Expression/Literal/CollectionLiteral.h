//
// Created by Jonas Zell on 21.06.17.
//

#ifndef CDOT_ARRAYLITERAL_H
#define CDOT_ARRAYLITERAL_H

#include "../Expression.h"

class TypeRef;

class CollectionLiteral : public Expression {
public:
    explicit CollectionLiteral(std::shared_ptr<TypeRef>);

    void add_element(Expression::SharedPtr);

    typedef std::shared_ptr<CollectionLiteral> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children() override;
    void __dump(int) override;

    NodeType get_type() override {
        return NodeType::COLLECTION_LITERAL;
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
    std::vector<Expression::SharedPtr> elements;
    std::shared_ptr<TypeRef> type;
};


#endif //CDOT_ARRAYLITERAL_H
