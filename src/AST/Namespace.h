//
// Created by Jonas Zell on 07.07.17.
//

#ifndef CDOT_SYMBOLTABLE_H
#define CDOT_SYMBOLTABLE_H

#include <string>
#include <unordered_map>
#include <iostream>
#include "../Variant/Variant.h"
#include "../Util.h"

class CompoundStmt;
class ClassDecl;

namespace cdot {
namespace cl {
    class Class;
    class Interface;
}
}

class SymbolTable {
public:
    static std::unordered_map<std::string, Variant::SharedPtr> symbols;
    static void insert(std::string, TypeSpecifier, std::string);
    static void insert(std::string, TypeSpecifier*, std::vector<TypeSpecifier>, std::string);
    static bool has(std::string);
    static Variant::SharedPtr lookup(std::string);

    static std::shared_ptr<CompoundStmt> import(std::string);

    static cdot::cl::Class* declare_class(std::string, std::string, std::vector<std::string>, ClassDecl*);
    static void declare_interface(std::string);
    static void declare_namespace(std::string);

    static bool has_class(std::string);
    static bool has_interface(std::string);
    static bool is_namespace(std::string);

    static cdot::cl::Class* get_class(std::string);
    static cdot::cl::Interface* get_interface(std::string);

    static std::string finalize_class(std::string);

    static std::string symbol_from_var(std::string, TypeSpecifier, bool);
    static std::string symbol_from_var(std::string, std::string);
    static std::string symbol_from_fun(std::string, std::vector<TypeSpecifier>, std::string);
    static std::string fun_with_ret_type(std::string, std::vector<TypeSpecifier>, TypeSpecifier);
    static std::string symbol_from_method(std::string&, std::string&, std::vector<TypeSpecifier>);

    static void dump();

private:
    SymbolTable();

    static std::unordered_map<std::string, std::unique_ptr<cdot::cl::Class>> classes;
    static std::unordered_map<std::string, std::unique_ptr<cdot::cl::Interface>> interfaces;

    static std::vector<std::string> namespaces;
};


#endif //CDOT_SYMBOLTABLE_H
