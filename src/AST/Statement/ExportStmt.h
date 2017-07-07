//
// Created by Jonas Zell on 02.07.17.
//

#ifndef CDOT_EXPORTSTMT_H
#define CDOT_EXPORTSTMT_H


#include "../../Variant/Variant.h"
#include "Statement.h"

class ExportStmt : public Statement {
public:
    ExportStmt(std::string);

    typedef std::shared_ptr<ExportStmt> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;

protected:
    std::string target;
};


#endif //CDOT_EXPORTSTMT_H
