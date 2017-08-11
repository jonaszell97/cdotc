//
// Created by Jonas Zell on 02.07.17.
//

#ifndef CDOT_IMPORTSTMT_H
#define CDOT_IMPORTSTMT_H


#include <string>
#include "Statement.h"

class ImportStmt : public Statement {
public:
    ImportStmt(std::vector<string>);

    typedef std::shared_ptr<ImportStmt> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    inline virtual NodeType get_type() {
        return NodeType::IMPORT_STMT;
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
    friend class EvaluatingVisitor;
    friend class TypeCheckVisitor;
    friend class CodeGenVisitor;

protected:
    std::vector<string> import;
};


#endif //CDOT_IMPORTSTMT_H
