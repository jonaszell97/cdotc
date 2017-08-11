//
// Created by Jonas Zell on 07.07.17.
//

#include <fstream>
#include "Namespace.h"
#include "../Util.h"
#include "Visitor/StaticAnalysis/Class.h"
#include "Statement/Declaration/Class/ClassDecl.h"
#include "../Parser.h"

std::unordered_map<string, Namespace::UniquePtr> Namespace::namespaces = []() {
    std::unordered_map<string, Namespace::UniquePtr> ns;
    ns.emplace("Global", std::make_unique<Namespace>("Global"));

    return ns;
}();

string Namespace::last_added_ns = "Global";

Namespace::Namespace(string name, Namespace* parent) : name(name), classes{}, interfaces{}, parent(parent) {
    if (parent == nullptr && name != "Global") {
        parent = Namespace::get("Global");
    }

    if (parent != nullptr) {
        parent->children.emplace(name, this);
    }
}

cdot::cl::Class* Namespace::get_class(TypeSpecifier ts) {
    Namespace* current = Namespace::get("Global");

    for (const auto& ns : ts.ns_name) {
        if (ns != "Global") {
            current = current->children[ns];
        }
    }

    return current->get_class(ts.class_name);
}

void Namespace::resolve_typedef(TypeSpecifier& ts) {
    if (ts.type != OBJECT_T) {
        return;
    }
    if (ts.raw_array) {
        return;
    }

    if (ts.ns_name.empty()) {
        if (!has_typedef(ts.class_name)) {
            if (name != "Global") {
                return Namespace::get("Global")->resolve_typedef(ts);
            }
            else {
                ts.invalid_ns_ref = true;
                return;
            }
        }

        auto td = get_typedef(ts.class_name);
        ts.class_name = td.class_name;
        ts.type = td.type;
        return;
    }

    Namespace* current = this;

    for (const auto& ns : ts.ns_name) {
        if (current->children.find(ns) != current->children.end()) {
            current = current->children[ns];
        }
        else if (name != "Global") {
            return Namespace::get("Global")->resolve_typedef(ts);
        }
    }

    if (!current->has_typedef(ts.class_name) && util::typemap.find(ts.class_name) == util::typemap.end()) {
        ts.invalid_ns_ref = true;
        return;
    }

    ts.ns_name.clear();

    if (util::typemap.find(ts.class_name) != util::typemap.end()) {
        ts.type = util::typemap[ts.class_name];
    }
    else {
        auto& type = current->get_typedef(ts.class_name);
        ts.class_name = type.class_name;
        ts.type = type.type;
    }
}

string Namespace::symbol_from_var(string& id, string scope) {
    return "_V" + std::to_string(id.length()) + id + scope;
}

string Namespace::symbol_from_var(string& id, TypeSpecifier& type, bool is_static) {
    return "_V" + std::to_string(id.length()) + id + "_" + util::type_to_symbol(type) + (is_static ? ".st" : "");
}

string Namespace::symbol_from_fun(string& id, std::vector<TypeSpecifier>& args, string scope) {
    string symbol = "_F" + std::to_string(id.length()) + id;
    for (auto arg : args) {
        symbol += util::type_to_symbol(arg);
    }

    return symbol + scope;
}

string Namespace::fun_with_ret_type(string& name, std::vector<TypeSpecifier>& args, TypeSpecifier& ret_type) {
    string symb = name + util::type_to_symbol(ret_type) + ".";
    for (const auto& arg : args) {
        symb += util::type_to_symbol(arg);
    }

    return symb;
}

string Namespace::symbol_from_method(string &class_name, string &name, std::vector<TypeSpecifier> &args) {
    string symbol = class_name + "." + name;
    for (const auto& arg : args) {
        symbol += util::type_to_symbol(arg);
    }

    return symbol;
}

void Namespace::insert(string id, TypeSpecifier type, string scope) {
    auto val = std::make_shared<Variant>();
    val->set_type(type);

    symbols.emplace(symbol_from_var(id, scope), val);
}

void Namespace::insert(string id, TypeSpecifier* ret_type, std::vector<TypeSpecifier> args, string scope) {
    TypeSpecifier ts(OBJECT_T);
    ts.is_function = true;
    ts.args = args;
    ts.return_type = ret_type;

    auto val = std::make_shared<Variant>();
    val->set_type(ts);

    symbols.emplace(symbol_from_fun(id, args, scope), val);
}

bool Namespace::has(string id) {
    return symbols.find(id) != symbols.end();
}

Variant::SharedPtr Namespace::lookup(string id) {
    return symbols.at(id);
}

cdot::cl::Class* Namespace::declare_class(string class_name, string extends, std::vector<string>
    implements, std::vector<pair<string, TypeSpecifier>> generics, ClassDecl* decl, bool is_abstract)
{
    cdot::cl::Class* parent_class = nullptr;
    if (!extends.empty()) {
        if (classes.find(extends) == classes.end()) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Class " + extends + " does not exist", decl);
        }

        parent_class = classes.at(extends).get();
    }

    auto ns = Namespace::declare_namespace(class_name, this);

    auto cl = std::make_unique<cdot::cl::Class>(class_name, parent_class, implements, generics, decl, ns, is_abstract);
    auto ptr = cl.get();

    classes.emplace(class_name, std::move(cl));

    TypeSpecifier t(is_abstract ? INTERFACE_T : OBJECT_T);
    t.class_name = class_name;
    declare_typedef(class_name, t);

    return ptr;
}

void Namespace::declare_interface(string interface_name) {
    interfaces.emplace(interface_name, std::make_unique<cdot::cl::Interface>(interface_name));
}

void Namespace::declare_typedef(string alias, TypeSpecifier origin) {
    typedefs.emplace(alias, origin);
}

void Namespace::declare_var(string name, TypeSpecifier origin) {
    variables.emplace(name, origin);
}

cdot::cl::Class* Namespace::get_class(string name) {
    auto current = this;
    while (current != nullptr) {
        if (current->classes.find(name) != current->classes.end()) {
            return current->classes[name].get();
        }

        current = current->parent;
    }

    if (classes.find(name) == classes.end()) {
        int i = 0;
    }

    return classes.at(name).get();
}

cdot::cl::Interface* Namespace::get_interface(string name) {
    auto current = this;
    while (current != nullptr) {
        if (current->interfaces.find(name) != current->interfaces.end()) {
            return current->interfaces[name].get();
        }

        current = current->parent;
    }

    return interfaces.at(name).get();
}

TypeSpecifier& Namespace::get_typedef(string name) {
    auto current = this;
    while (current != nullptr) {
        if (current->typedefs.find(name) != current->typedefs.end()) {
            return current->typedefs[name];
        }

        current = current->parent;
    }

    return typedefs.at(name);
}

TypeSpecifier& Namespace::get_var(string name) {
    auto current = this;
    while (current != nullptr) {
        if (current->variables.find(name) != current->variables.end()) {
            return current->variables[name];
        }

        current = current->parent;
    }

    return variables.at(name);
}

Namespace* Namespace::get_namespace(string name) {
    if (name == this->name) {
        return this;
    }

    Namespace* ns;
    for (const auto& child : children) {
        ns = child.second->get_namespace(name);
        if (ns != nullptr) {
            return ns;
        }
    }

    return ns;
}

bool Namespace::has_class(string class_name) {
    auto current = this;
    while (current != nullptr) {
        if (current->classes.find(class_name) != current->classes.end()) {
            return true;
        }

        current = current->parent;
    }

    return false;
}

bool Namespace::has_interface(string interface_name) {
    auto current = this;
    while (current != nullptr) {
        if (current->interfaces.find(interface_name) != current->interfaces.end()) {
            return true;
        }

        current = current->parent;
    }

    return false;
}

bool Namespace::has_typedef(string name) {
    auto current = this;
    while (current != nullptr) {
        if (current->typedefs.find(name) != current->typedefs.end()) {
            return true;
        }

        current = current->parent;
    }

    return false;
}

bool Namespace::has_var(string name) {
    auto current = this;
    while (current != nullptr) {
        if (current->variables.find(name) != current->variables.end()) {
            return true;
        }

        current = current->parent;
    }

    return false;
}

bool Namespace::has_namespace(string name) {
    if (children.find(name) != children.end()) {
        return true;
    }

    for (const auto& child : children) {
        if (child.second->has_namespace(name)) {
            return true;
        }
    }

    return false;
}

Namespace* Namespace::declare_namespace(string ns_name, Namespace* parent) {
    auto ns = std::make_unique<Namespace>(ns_name, parent);
    auto ptr = ns.get();

    namespaces.emplace(ns_name, std::move(ns));
    last_added_ns = ns_name;

    return ptr;
}

Namespace* Namespace::get(string ns) {
    return namespaces[ns].get();
}

Namespace* Namespace::latest() {
    return namespaces[last_added_ns].get();
}

Namespace* Namespace::global() {
    return namespaces["Global"].get();
}

bool Namespace::is_namespace(string ns_name) {
    return namespaces.find(ns_name) != namespaces.end();
}

void Namespace::dump()  {
    for (auto symbol : symbols) {
        std::cout << symbol.first << ": " << symbol.second->to_string() << " | " <<
             symbol.second->get_type().to_string() << std::endl;
    }
}