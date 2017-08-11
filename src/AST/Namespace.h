//
// Created by Jonas Zell on 07.07.17.
//

#ifndef CDOT_SYMBOLTABLE_H
#define CDOT_SYMBOLTABLE_H

#include <string>
#include <unordered_map>
#include <iostream>
#include <map>

class CompoundStmt;
class ClassDecl;
struct TypeSpecifier;
class Variant;

using std::pair;

namespace cdot {
namespace cl {
    class Class;
    class Interface;
}
}

using std::string;

class Namespace {
public:
    Namespace(string, Namespace* = nullptr);

    std::unordered_map<string, std::shared_ptr<Variant>> symbols = {};
    void insert(string, TypeSpecifier, string);
    void insert(string, TypeSpecifier*, std::vector<TypeSpecifier>, string);
    bool has(string);
    std::shared_ptr<Variant> lookup(string);

    cdot::cl::Class* declare_class(string, string, std::vector<string>, std::vector<pair<string, TypeSpecifier>>, ClassDecl*, bool);
    void declare_interface(string);
    void declare_typedef(string, TypeSpecifier);
    void declare_var(string, TypeSpecifier);

    bool has_class(string);
    bool has_interface(string);
    bool has_typedef(string);
    bool has_namespace(string);
    bool has_var(string);

    static Namespace* global();

    cdot::cl::Class* get_class(string);
    cdot::cl::Interface* get_interface(string);
    TypeSpecifier& get_typedef(string);
    Namespace* get_namespace(string);
    TypeSpecifier& get_var(string);

    void dump();

    inline string get_name() {
        return name;
    }

    typedef std::unique_ptr<Namespace> UniquePtr;

    static Namespace* get(string ns);
    static Namespace* latest();

    static bool is_namespace(string);
    static Namespace* declare_namespace(string, Namespace* = nullptr);

    static string symbol_from_var(string&, TypeSpecifier&, bool);
    static string symbol_from_var(string&, string = "");
    static string symbol_from_fun(string&, std::vector<TypeSpecifier>&, string);
    static string fun_with_ret_type(string&, std::vector<TypeSpecifier>&, TypeSpecifier&);
    static string symbol_from_method(string&, string&, std::vector<TypeSpecifier>&);

    static cdot::cl::Class* get_class(TypeSpecifier);
    void resolve_typedef(TypeSpecifier&);

private:
    string name;
    Namespace* parent;
    std::unordered_map<string, Namespace*> children = {};

    std::unordered_map<string, std::unique_ptr<cdot::cl::Class>> classes;
    std::unordered_map<string, std::unique_ptr<cdot::cl::Interface>> interfaces;
    std::unordered_map<string, TypeSpecifier> typedefs = {};
    std::unordered_map<string, TypeSpecifier> variables = {};

    static string last_added_ns;
    static std::unordered_map<string, Namespace::UniquePtr> namespaces;
};


#endif //CDOT_SYMBOLTABLE_H
