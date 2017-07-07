//
// Created by Jonas Zell on 02.07.17.
//

#include "Module.h"

Module::Module(std::string name) :
    module_name(name),
    exports(std::unordered_map<std::string, Variant::SharedPtr>())
{

}

void Module::add_export(std::string ident, Variant::SharedPtr val) {
    exports.emplace(ident, val);
}

Variant::SharedPtr Module::get_export(std::string ident) {
    if (exports.find(ident) == exports.end()) {
        RuntimeError::raise(ERR_UNDECLARED_VARIABLE, "Export " + ident + " was not declared in module " + module_name);
    }

    return exports.at(ident);
}