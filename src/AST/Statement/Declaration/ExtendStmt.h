//
// Created by Jonas Zell on 31.07.17.
//

#ifndef CDOT_EXTENDSTMT_H
#define CDOT_EXTENDSTMT_H

#include "../Statement.h"

class FieldDecl;
class MethodDecl;
class ConstrDecl;

class ExtendStmt : public Statement {
public:
    ExtendStmt(std::string, std::vector<std::shared_ptr<FieldDecl>>, std::vector<std::shared_ptr<MethodDecl>>,
        std::vector<std::shared_ptr<ConstrDecl>> constructors);

    typedef std::shared_ptr<ExtendStmt> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children() override;
    void __dump(int depth) override;

    NodeType get_type() override {
        return NodeType::FUNC_ARG_DECL;
    }

    llvm::Value* accept(CodeGen& v) override {
        return v.visit(this);
    }

    Type* accept(TypeCheckPass& v) override {
        return v.visit(this);
    }

   void accept(DeclPass &v) override {
      v.visit(this);
   }

    friend class ConstExprPass;
    friend class CodeGen;
    friend class TypeCheckPass;
   friend class DeclPass;

protected:
    std::string extended_class;
    std::vector<std::shared_ptr<FieldDecl>> fields;
    std::vector<std::shared_ptr<MethodDecl>> methods;
    std::vector<std::shared_ptr<ConstrDecl>> constructors;
};


#endif //CDOT_EXTENDSTMT_H
