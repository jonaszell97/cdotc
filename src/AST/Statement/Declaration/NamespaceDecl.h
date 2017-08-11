//
// Created by Jonas Zell on 02.07.17.
//

#ifndef CDOT_MODULEDECL_H
#define CDOT_MODULEDECL_H

#include "../Statement.h"
#include "../Block/CompoundStmt.h"

class NamespaceDecl : public Statement {
public:
    NamespaceDecl(string, CompoundStmt::SharedPtr);

    typedef std::shared_ptr<NamespaceDecl> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    inline virtual NodeType get_type() {
        return NodeType::MODULE_DECL;
    }
    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }
    virtual inline CGValue accept(CodeGenVisitor& v) {
        return v.visit(this);
    }
    virtual TypeSpecifier accept(TypeCheckVisitor& v) {
        return v.visit(this);
    }


    friend class Visitor;
    friend class TypeCheckVisitor;
    friend class CodeGenVisitor;


protected:
    string ns_name;
    CompoundStmt::SharedPtr contents;
};


#endif //CDOT_MODULEDECL_H
