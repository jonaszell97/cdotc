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

    inline void add_statements(std::vector<Statement::SharedPtr> stmts) {
        _statements.reserve(stmts.size());
        _statements.insert(_statements.begin(), stmts.begin(), stmts.end());
    }

    inline std::vector<Statement::SharedPtr> get_statements() {
        return _statements;
    }

    typedef std::shared_ptr<ModuleDecl> SharedPtr;
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
    friend class EvaluatingVisitor;

protected:
    std::string module_name;
    std::vector<std::shared_ptr<ImportStmt>> imports;
    std::vector<std::shared_ptr<ExportStmt>> exports;
};


#endif //CDOT_MODULEDECL_H
