//
// Created by Jonas Zell on 25.06.17.
//

#include <iostream>
#include <fstream>
#include "GlobalContext.h"
#include "Types/Integer.h"
#include "Types/String.h"
#include "Module.h"
#include "../Parser.h"
#include "Types/Function.h"
#include "Types/Array.h"

GlobalContext::GlobalContext() {}

bool GlobalContext::initialized = false;
std::string GlobalContext::program = "";

std::unordered_map<std::string, std::unique_ptr<Class>> GlobalContext::classes =
        std::unordered_map<std::string, std::unique_ptr<Class>>();

std::unordered_map<std::string, std::unique_ptr<Module>> GlobalContext::modules = std::unordered_map<std::string,
        std::unique_ptr<Module>>();

std::unordered_map<std::string, Variant::SharedPtr> GlobalContext::global_variables = std::unordered_map<std::string,
        Variant::SharedPtr>();

std::unordered_map<std::string, std::unique_ptr<Interface>> GlobalContext::interfaces =
        std::unordered_map<std::string, std::unique_ptr<Interface>>();

void GlobalContext::init(std::string p) {
    if (initialized) {
        return;
    }

    program = p;
    initialized = true;

    Class::UniquePtr Object = std::make_unique<Class>("Object", Function(cdot::lib::obj::construct),
        AccessModifier::PUBLIC);

    declare_class(std::move(Object));

    Class::UniquePtr Integer = cdot::lib::intgr::init();
    declare_class(std::move(Integer));

    Class::UniquePtr String = cdot::lib::str::init();
    declare_class(std::move(String));

    Class::UniquePtr Function = cdot::lib::fun::init();
    declare_class(std::move(Function));

    Class::UniquePtr Array = cdot::lib::arr::init();
    declare_class(std::move(Array));
}

bool GlobalContext::is_declared_class(std::string class_name) {
    return classes.find(class_name) != classes.end();
}

void GlobalContext::declare_class(Class::UniquePtr class_) {
    if (is_declared_class(class_->class_name())) {
        RuntimeError::raise(ERR_REDECLARED_VAR, "Cannot redeclare class " + class_->class_name());
    }

    classes.emplace(class_->class_name(), std::move(class_));
}

void GlobalContext::declare_interface(std::unique_ptr<Interface> interface) {
    if (is_declared_interface(interface->get_name())) {
        RuntimeError::raise(ERR_REDECLARED_VAR, "Cannot redeclare interface " + interface->get_name());
    }

    interfaces.emplace(interface->get_name(), std::move(interface));
}

bool GlobalContext::is_declared_interface(std::string interface) {
    return interfaces.find(interface) != interfaces.end();
}

Interface* GlobalContext::get_interface(std::string interface) {
    if (!is_declared_interface(interface)) {
        RuntimeError::raise(ERR_UNDECLARED_VARIABLE, "Interface " + interface + " does not exist");
    }

    return interfaces.at(interface).get();
}

Class* GlobalContext::get_class(std::string class_name) {
    if (!is_declared_class(class_name)) {
        RuntimeError::raise(ERR_UNDECLARED_VARIABLE, "Class " + class_name + " does not exist");
    }

    return classes[class_name].get();
}

Variant::SharedPtr GlobalContext::get_global_var(std::string ident) {
    if (global_variables.find(ident) == global_variables.end()) {
        RuntimeError::raise(ERR_UNDECLARED_VARIABLE, "Global variable " + ident + " does not exist");
    }

    return global_variables.at(ident);
}

void GlobalContext::set_global_var(std::string ident, Variant::SharedPtr val) {
    if (global_variables.find(ident) != global_variables.end()) {
        RuntimeError::raise(ERR_REDECLARED_VAR, "Redeclaration of variable " + ident);
    }

    global_variables.emplace(ident, val);
}

bool GlobalContext::has_global_var(std::string ident) {
    return global_variables.find(ident) != global_variables.end();
}

Module* GlobalContext::get_module(std::string module_name) {
    if (modules.find(module_name) == modules.end()) {
        RuntimeError::raise(ERR_UNDECLARED_VARIABLE, "Module " + module_name + " does not exist");
    }

    return modules[module_name].get();
}

void GlobalContext::import_module(std::string path, std::string module_name, bool builtin, std::vector<std::string>
    imported_identifiers)
{
    if (builtin) {

    }
    else {
        std::ifstream t(path);
        if (!t.good()) {
            RuntimeError::raise(ERR_NULL_POINTER_EXC, "Module could not be imported: File " + path + " does not exist");
        }
        std::string str((std::istreambuf_iterator<char>(t)), std::istreambuf_iterator<char>());
        Parser x(str);

        Module::UniquePtr module = x.run(true);

        // partial import
        if (imported_identifiers.size() > 0) {
            for (auto ident : imported_identifiers) {
                set_global_var(ident, module->get_export(ident));
            }
        }
        // full import with identifier
        else if (module_name != "") {
            Object::SharedPtr obj = std::make_shared<Object>();
            for (auto exp : module->get_exports()) {
                obj->add_property(exp.first, exp.second);
            }

            set_global_var(module_name, std::make_shared<Variant>(obj));
        }
        // full import into global context
        else {
            for (auto exp : module->get_exports()) {
                set_global_var(exp.first, exp.second);
            }
        }
    }
}