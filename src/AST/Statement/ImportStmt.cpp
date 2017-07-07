//
// Created by Jonas Zell on 02.07.17.
//

#include "ImportStmt.h"

#include <iostream>

ImportStmt::ImportStmt(std::string path, std::string module_name, bool lib_import) : path(path), module_name
        (module_name), is_lib_import(lib_import), import_identifiers(std::vector<std::string>()) {

}

std::vector<AstNode::SharedPtr> ImportStmt::get_children() {
    return { };
}

void ImportStmt::__dump(int depth) {
    AstNode::__tab(depth);
    std::string as = (module_name == "" ? "" : " as " + module_name);
    std::cout << "ImportStmt [\"" << path << "\"" << as << "]" << std::endl;

    for (auto c : get_children()) {
        c->__dump(depth + 1);
    }
}