//
// Created by Jonas Zell on 02.07.17.
//

#ifndef CDOT_MODULEDECL_H
#define CDOT_MODULEDECL_H


#include "../CompoundStmt.h"

class ImportStmt;
class ExportStmt;

class ModuleDecl : public CompoundStmt {
public:
    ModuleDecl(std::string);

    inline void add_import(std::shared_ptr<ImportStmt> import) {
        imports.push_back(import);
    }

    inline void add_export(std::shared_ptr<ExportStmt> _export) {
        exports.push_back(_export);
    }

    typedef std::shared_ptr<ModuleDecl> SharedPtr;
    std::vector<AstNode::SharedPtr> get_children();
    void __dump(int);

    virtual inline Variant accept(Visitor& v) {
        return v.visit(this);
    }

    friend class Visitor;
    friend class EvaluatingVisitor;

protected:
    std::string module_name;
    std::vector<std::shared_ptr<ImportStmt>> imports;
    std::vector<std::shared_ptr<ExportStmt>> exports;
};


#endif //CDOT_MODULEDECL_H
