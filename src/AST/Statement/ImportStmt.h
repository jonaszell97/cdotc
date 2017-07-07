//
// Created by Jonas Zell on 02.07.17.
//

#ifndef CDOT_IMPORTSTMT_H
#define CDOT_IMPORTSTMT_H


#include <string>
#include "Statement.h"

class ImportStmt : public Statement {
public:
    ImportStmt(std::string, std::string = "", bool = false);

    virtual inline void add_import_ident(std::string ident) {
        import_identifiers.push_back(ident);
        is_full_import = false;
    }

    typedef std::shared_ptr<ImportStmt> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;

protected:
    bool is_full_import = true;
    std::string module_name;
    std::string path;
    std::vector<std::string> import_identifiers;
    bool is_lib_import;
};


#endif //CDOT_IMPORTSTMT_H
