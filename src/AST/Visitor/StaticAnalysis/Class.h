//
// Created by Jonas Zell on 09.07.17.
//

#ifndef CDOT_CLASS_H
#define CDOT_CLASS_H

#include <string>
#include <unordered_map>
#include "../../../../Variant/Variant.h"

class Expression;

namespace cdot {
namespace ast {
namespace cl {

    enum class AccessModifier {
        PUBLIC, PRIVATE, PROTECTED
    };

    class Field {
    public:
        Field(std::string name, TypeSpecifier type, AccessModifier access_modifier);
        typedef std::unique_ptr<Field> UniquePtr;

    private:
        std::string field_name;
        TypeSpecifier field_type;
        AccessModifier  access_modifier;
    };

    class Method {
    public:
        Method(std::string name, TypeSpecifier ret_type, AccessModifier access_modifier, std::vector<std::string>,
                std::vector<TypeSpecifier>, std::vector<std::shared_ptr<Expression>>);

        typedef std::unique_ptr<Method> UniquePtr;

    private:
        std::string method_name;
        TypeSpecifier return_type;
        std::vector<std::string> argument_names;
        std::vector<TypeSpecifier> argument_types;
        std::vector<std::shared_ptr<Expression>> argument_def_values;
        AccessModifier  access_modifier;
    };

    class Class;

    class Interface {
    public:
        Interface(std::string);
        void add_field(std::string, TypeSpecifier, bool);
        void add_method(std::string, TypeSpecifier, bool, std::vector<TypeSpecifier>);

        typedef std::unique_ptr<Interface> UniquePtr;
        friend class Class;

    protected:
        std::string interface_name;

        std::vector<std::string> field_symbols;
        std::vector<std::string> method_symbols;
    };

    class Class {
    public:
        Class(std::string, std::string, std::vector<std::string>);
        void add_field(std::string, TypeSpecifier, AccessModifier, bool);
        void add_method(std::string, TypeSpecifier, AccessModifier, bool, std::vector<std::string>,
            std::vector<TypeSpecifier>, std::vector<std::shared_ptr<Expression>>);
        void finalize();

        typedef std::unique_ptr<Class> UniquePtr;

    protected:
        std::string class_name;
        std::string extends;
        std::vector<std::string> implements;

        std::unordered_map<std::string, Field::UniquePtr> fields = {};
        std::unordered_map<std::string, Method::UniquePtr> methods = {};
        std::unordered_map<std::string, Field::UniquePtr> static_fields = {};
        std::unordered_map<std::string, Method::UniquePtr> static_methods = {};

        std::vector<std::string> field_symbols;
        std::vector<std::string> method_symbols;
    };

} // namespace cl
} // namespace ast
} // namespace cdot

#endif //CDOT_CLASS_H
