//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_DECLARESTMT_H
#define CDOT_DECLARESTMT_H


#include "../Statement.h"

class FuncArgDecl;
class TypeRef;

enum class DeclarationType {
    CLASS_DECL,
    FUNC_DECL,
    VAR_DECL
};

class DeclareStmt : public Statement {
public:
    DeclareStmt(string, std::shared_ptr<TypeRef>, bool);
    DeclareStmt(string, std::shared_ptr<TypeRef>, std::vector<std::shared_ptr<FuncArgDecl>>, std::vector<GenericType*>);
    DeclareStmt(AccessModifier, string, ObjectType*, std::vector<ObjectType*>, bool, std::vector<GenericType*>);

    std::vector<AstNode::SharedPtr> get_children() override;

    typedef std::shared_ptr<DeclareStmt> SharedPtr;
    void __dump(int depth) override;

    NodeType get_type() override {
        return NodeType::DECLARE_STMT;
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
    DeclarationType declKind;
    string declaredName;

    // class declaration
    std::vector<GenericType*> generics;
    ObjectType* extends;
    std::vector<ObjectType*> conformsTo;
    AccessModifier am;
    bool is_abstract;

    // function declaration
    std::vector<std::shared_ptr<FuncArgDecl>> args;
    std::shared_ptr<TypeRef> type = nullptr;

    // variable
    bool isConst;
};


#endif //CDOT_DECLARESTMT_H
