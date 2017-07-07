//
// Created by Jonas Zell on 25.06.17.
//

#ifndef CDOT_GLOBALCONTEXT_H
#define CDOT_GLOBALCONTEXT_H


#include <string>
#include "Class/Class.h"

class Module;

class GlobalContext {
public:
    static void init(std::string);

    static bool is_declared_class(std::string);
    static Class* get_class(std::string);
    static void declare_class(std::unique_ptr<Class>);
    static void declare_interface(std::unique_ptr<Interface>);
    static bool is_declared_interface(std::string);
    static Interface* get_interface(std::string);

    static Variant::SharedPtr get_global_var(std::string);
    static void set_global_var(std::string, Variant::SharedPtr);
    static bool has_global_var(std::string);

    static Module* get_module(std::string);
    static void import_module(std::string, std::string, bool, std::vector<std::string>);

    static std::string program;

    typedef std::shared_ptr<GlobalContext> SharedPtr;

private:
    GlobalContext();
    static bool initialized;
    static std::unordered_map<std::string, std::unique_ptr<Module>> modules;
    static std::unordered_map<std::string, std::unique_ptr<Class>> classes;
    static std::unordered_map<std::string, std::unique_ptr<Interface>> interfaces;
    static std::unordered_map<std::string, Variant::SharedPtr> global_variables;
};


#endif //CDOT_GLOBALCONTEXT_H
