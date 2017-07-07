//
// Created by Jonas Zell on 02.07.17.
//

#include "Interface.h"
#include "../../Util.h"

Interface::Interface(std::string name) :
    name(name),
    properties(std::vector<std::pair<std::string, TypeSpecifier>>()),
    static_properties(std::vector<std::pair<std::string, TypeSpecifier>>()),
    methods(std::vector<std::pair<std::string, TypeSpecifier>>()),
    static_methods(std::vector<std::pair<std::string, TypeSpecifier>>()),
    method_args(std::vector<std::vector<TypeSpecifier>>()),
    static_method_args(std::vector<std::vector<TypeSpecifier>>())
{

}

void Interface::declare_field(std::string name, TypeSpecifier type, bool is_static) {
    if (is_static) {
        static_properties.push_back({name, type});
    }
    else {
        properties.push_back({name, type});
    }
}

void Interface::declare_method(std::string name, std::vector<TypeSpecifier> args, TypeSpecifier return_type, bool
        is_static) {
    if (is_static) {
        static_methods.push_back({name, return_type});
        static_method_args.push_back(args);
    }
    else {
        methods.push_back({name, return_type});
        method_args.push_back(args);
    }
}