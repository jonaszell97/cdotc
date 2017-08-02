//
// Created by Jonas Zell on 02.07.17.
//

#include "ModuleDecl.h"
#include "../ImportStmt.h"
#include "../ExportStmt.h"

#include <iostream>

ModuleDecl::ModuleDecl(std::string module_name) :
    module_name(module_name),
    imports(std::vector<ImportStmt::SharedPtr>()),
    exports(std::vector<ExportStmt::SharedPtr>())
{

}

std::vector<AstNode::SharedPtr> ModuleDecl::get_children() {
    std::vector<AstNode::SharedPtr> children;
    for (auto import : imports) {
        children.push_back(import);
    }
    for (auto stmt : _statements) {
        children.push_back(stmt);
    }

    return children;
}

void ModuleDecl::__dump(int depth) {
    AstNode::__tab(depth);
    std::cout << "ModuleDecl [" << module_name << "]" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}