//
// Created by Jonas Zell on 27.06.17.
//

#ifndef CDOT_CONSTRDECL_H
#define CDOT_CONSTRDECL_H

#include "../../Statement.h"

class FuncArgDecl;
class CompoundStmt;

namespace cdot {
    namespace cl {
        struct Method;
    }
}

class ConstrDecl : public Statement {
public:
    ConstrDecl();
    ConstrDecl(std::vector<std::shared_ptr<FuncArgDecl>>, std::shared_ptr<CompoundStmt>, AccessModifier);

    typedef std::shared_ptr<ConstrDecl> SharedPtr;
    typedef std::unique_ptr<ConstrDecl> UniquePtr;

    std::vector<std::shared_ptr<AstNode>> get_children() override;
    void __dump(int depth) override;

    NodeType get_type() override {
        return NodeType::CONSTR_DECL;
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
    bool memberwise = false;
    AccessModifier am;
    std::vector<std::shared_ptr<FuncArgDecl>> args;
    std::shared_ptr<CompoundStmt> body;

    // codegen
    std::string selfBinding;
    string className;
    cdot::cl::Method* method;
};


#endif //CDOT_CONSTRDECL_H