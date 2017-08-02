//
// Created by Jonas Zell on 09.07.17.
//

#include "Class.h"
#include "../../../SymbolTable.h"
#include "../../../Expression/Expression.h"

namespace cdot {
namespace ast {
namespace cl {


    Field::Field(std::string name, TypeSpecifier type, AccessModifier access_modifier) :
        field_name(name), field_type(type), access_modifier(access_modifier) {

    }

    Method::Method(std::string name, TypeSpecifier ret_type, AccessModifier access_modifier, std::vector<std::string>
        arg_names, std::vector<TypeSpecifier> arg_types, std::vector<Expression::SharedPtr> arg_defaults) :
            method_name(name), return_type(ret_type), access_modifier(access_modifier),
            argument_def_values(arg_defaults), argument_names(arg_names), argument_types(arg_types)
    {

    }

    Interface::Interface(std::string name) : interface_name(name), field_symbols{}, method_symbols{} {

    }

    void Interface::add_field(std::string name, TypeSpecifier type, bool is_static) {
        field_symbols.push_back(SymbolTable::symbol_from_var(name, type, is_static));
    }

    void Interface::add_method(std::string name, TypeSpecifier ret_type, bool is_static,
            std::vector<TypeSpecifier> arg_types) {
        method_symbols.push_back(SymbolTable::symbol_from_fun(name, ret_type, arg_types, is_static));
    }

    Class::Class(std::string class_name, std::string extends, std::vector<std::string> implements) :
        class_name(class_name), extends(extends), implements(implements), field_symbols{}, method_symbols{}
    {

    }

    void Class::add_field(std::string name, TypeSpecifier type, AccessModifier access, bool is_static) {
        if (is_static) {
            static_fields.emplace(name, std::make_unique<Field>(name, type, access));
        }
        else {
            fields.emplace(name, std::make_unique<Field>(name, type, access));
        }

        field_symbols.push_back(SymbolTable::symbol_from_var(name, type, is_static));
    }

    void Class::add_method(std::string name, TypeSpecifier ret_type, AccessModifier access, bool is_static,
        std::vector<std::string> arg_names, std::vector<TypeSpecifier> arg_types,
        std::vector<Expression::SharedPtr> arg_defaults)
    {
        if (is_static) {
            static_methods.emplace(name, std::make_unique<Method>(name, ret_type, access, arg_names, arg_types,
                arg_defaults));
        }
        else {
            methods.emplace(name, std::make_unique<Method>(name, ret_type, access, arg_names, arg_types,
                arg_defaults));
        }

        method_symbols.push_back(SymbolTable::symbol_from_fun(name, ret_type, arg_types, is_static));
    }

    void Class::finalize() {
        for (auto interface_name : implements) {
            Interface* interface = SymbolTable::get_interface(interface_name);
            for (auto field : interface->field_symbols) {
                if (std::find(field_symbols.begin(), field_symbols.end(), field) == field_symbols.end()) {
                    RuntimeError::raise(ERR_TYPE_ERROR, "Class " + class_name + " does not correctly implement "
                        "interface " + interface_name + ": Required field is missing");
                }
            }

            for (auto method : interface->method_symbols) {
                if (std::find(method_symbols.begin(), method_symbols.end(), method) == method_symbols.end()) {
                    RuntimeError::raise(ERR_TYPE_ERROR, "Class " + class_name + " does not correctly implement "
                        "interface " + interface_name + ": Required method is missing or has differing signature");
                }
            }
        }
    }

} // namespace cl
} // namespace ast
} // namespace cdot