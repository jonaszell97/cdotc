//
// Created by Jonas Zell on 02.07.17.
//

#ifndef CDOT_MODULEDECL_H
#define CDOT_MODULEDECL_H

#include "../Statement.h"

class CompoundStmt;

class NamespaceDecl : public Statement {
public:
    NamespaceDecl(string, std::shared_ptr<CompoundStmt>);

    typedef std::shared_ptr<NamespaceDecl> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children() override;
    void __dump(int depth) override;

    NodeType get_type() override {
        return NodeType::NAMESPACE_DECL;
    }

    llvm::Value* accept(CodeGenVisitor& v) override {
        return v.visit(this);
    }

    Type* accept(TypeCheckVisitor& v) override {
        return v.visit(this);
    }

    friend class ConstExprVisitor;
    friend class TypeCheckVisitor;
    friend class CodeGenVisitor;


protected:
    string nsName;
    std::shared_ptr<CompoundStmt> contents;
};


#endif //CDOT_MODULEDECL_H
