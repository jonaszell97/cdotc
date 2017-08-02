//
// Created by Jonas Zell on 07.07.17.
//

#include <fstream>
#include "SymbolTable.h"
#include "../Util.h"
#include "Visitor/StaticAnalysis/Class.h"
#include "Statement/Declaration/Class/ClassDecl.h"
#include "../Parser.h"

std::unordered_map<std::string, Variant::SharedPtr> SymbolTable::symbols = {};
std::unordered_map<std::string, cdot::cl::Class::UniquePtr> SymbolTable::classes = {};
std::unordered_map<std::string, cdot::cl::Interface::UniquePtr> SymbolTable::interfaces = {};
std::vector<std::string> SymbolTable::namespaces = {};

std::string SymbolTable::symbol_from_var(std::string id, std::string scope) {
    return "_V" + std::to_string(id.length()) + id + "_" + scope;
}

std::string SymbolTable::symbol_from_var(std::string id, TypeSpecifier type, bool is_static) {
    return "_V" + std::to_string(id.length()) + id + "_" + util::type_to_symbol(type) + (is_static ? ".st" : "");
}

std::string SymbolTable::symbol_from_fun(std::string id, std::vector<TypeSpecifier> args, std::string scope) {
    std::string symbol = "_F" + std::to_string(id.length()) + id + (args.size() > 0 ? "_" : "");
    for (auto arg : args) {
        symbol += util::type_to_symbol(arg);
    }

    return symbol + "_" + scope;
}

std::string SymbolTable::fun_with_ret_type(std::string name, std::vector<TypeSpecifier> args, TypeSpecifier ret_type) {
    std::string symb = name + util::type_to_symbol(ret_type) + ".";
    for (const auto& arg : args) {
        symb += util::type_to_symbol(arg);
    }

    return symb;
}

std::string SymbolTable::symbol_from_method(std::string &class_name, std::string &name, std::vector<TypeSpecifier> args) {
    std::string symbol = class_name + "." + name + std::to_string(args.size());
    for (const auto& arg : args) {
        symbol += util::type_to_symbol(arg);
    }

    return symbol;
}

void SymbolTable::insert(std::string id, TypeSpecifier type, std::string scope) {
    auto val = std::make_shared<Variant>();
    val->set_type(type);

    symbols.emplace(symbol_from_var(id, scope), val);
}

void SymbolTable::insert(std::string id, TypeSpecifier* ret_type, std::vector<TypeSpecifier> args, std::string scope) {
    TypeSpecifier ts(OBJECT_T);
    ts.is_function = true;
    ts.args = args;
    ts.return_type = ret_type;

    auto val = std::make_shared<Variant>();
    val->set_type(ts);

    symbols.emplace(symbol_from_fun(id, args, scope), val);
}

bool SymbolTable::has(std::string id) {
    return symbols.find(id) != symbols.end();
}

Variant::SharedPtr SymbolTable::lookup(std::string id) {
    return symbols.at(id);
}

cdot::cl::Class* SymbolTable::declare_class(std::string class_name, std::string extends, std::vector<std::string>
    implements, ClassDecl* decl)
{
    cdot::cl::Class* parent_class = nullptr;
    if (!extends.empty()) {
        if (classes.find(extends) == classes.end()) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Class " + extends + " does not exist", decl);
        }

        parent_class = classes.at(extends).get();
    }

    auto cl = std::make_unique<cdot::cl::Class>(class_name, parent_class, implements, decl);
    auto ptr = cl.get();
    classes.emplace(class_name, std::move(cl));

    return ptr;
}

void SymbolTable::declare_interface(std::string interface_name) {
    interfaces.emplace(interface_name, std::make_unique<cdot::cl::Interface>(interface_name));
}

cdot::cl::Class* SymbolTable::get_class(std::string name) {
    return classes.at(name).get();
}

cdot::cl::Interface* SymbolTable::get_interface(std::string name) {
    return interfaces.at(name).get();
}

bool SymbolTable::has_class(std::string class_name) {
    return classes.find(class_name) != classes.end();
}

bool SymbolTable::has_interface(std::string interface_name) {
    return interfaces.find(interface_name) != interfaces.end();
}

void SymbolTable::declare_namespace(std::string ns_name) {
    namespaces.push_back(ns_name);
}

bool SymbolTable::is_namespace(std::string ns_name) {
    return std::find(namespaces.begin(), namespaces.end(), ns_name) != namespaces.end();
}

std::string SymbolTable::finalize_class(std::string class_name) {
    return classes.at(class_name)->finalize();
}

void SymbolTable::dump()  {
    for (auto symbol : symbols) {
        std::cout << symbol.first << ": " << symbol.second->to_string() << " | " <<
             symbol.second->get_type().to_string() << std::endl;
    }
}

CompoundStmt::SharedPtr SymbolTable::import(std::string name) {
    string file_path = __FILE__;
    string dir_path = file_path.substr(0, file_path.rfind("/")) + "/../StdLib/" + name + ".dot";
    std::ifstream ifs(dir_path);
    std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

    return Parser(content).parse();
}