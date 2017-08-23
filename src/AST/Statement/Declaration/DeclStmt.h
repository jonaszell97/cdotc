//
// Created by Jonas Zell on 19.06.17.
//

#ifndef CDOT_DECLSTATEMENT_H
#define CDOT_DECLSTATEMENT_H


#include "../Statement.h"

class TypeRef;
class Expression;

class DeclStmt : public Statement {
public:
    DeclStmt(string, std::shared_ptr<TypeRef>, bool, bool, std::shared_ptr<Expression> = nullptr);

    ~DeclStmt() override {

    }

    std::vector<AstNode::SharedPtr> get_children() override;

    typedef std::shared_ptr<DeclStmt> SharedPtr;
    void __dump(int) override;

    NodeType get_type() override {
        return NodeType::DECLARATION;
    }

    llvm::Value* accept(CodeGenVisitor& v) override {
        return v.visit(this);
    }

    Type* accept(TypeCheckVisitor& v) override {
        return v.visit(this);
    }

    string getIdentifier() {
        return identifier;
    }

    std::shared_ptr<TypeRef> getType() {
        return type;
    }

    friend class ConstExprVisitor;
    friend class CodeGenVisitor;
    friend class TypeCheckVisitor;

protected:
    string identifier;
    std::shared_ptr<TypeRef> type;
    std::shared_ptr<Expression> value = nullptr;
    bool is_const;
    bool is_global;
    bool isStructAlloca = false;
    bool declared = false;
};


#endif //CDOT_DECLSTATEMENT_H
