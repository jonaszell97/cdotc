//
// Created by Jonas Zell on 09.07.17.
//

#ifndef CDOT_CLASS_H
#define CDOT_CLASS_H

#include <string>
#include <unordered_map>
#include "../../../Variant/Variant.h"
#include "../../../Util.h"
#include "../../Namespace.h"
#include "../../Expression/Expression.h"
#include "../../Statement/Declaration/FuncArgDecl.h"

class ClassDecl;
class Expression;

using std::string;

namespace cdot {
namespace cl {

    struct Field {
        Field(string name, TypeSpecifier type, AccessModifier access_modifier, Expression::SharedPtr = nullptr);
        typedef std::unique_ptr<Field> UniquePtr;

        string field_name;
        string symbol_name;
        TypeSpecifier field_type;
        AccessModifier  access_modifier;
        Expression::SharedPtr def_val;

        llvm::Type* llvm_type = nullptr;
    };

    struct Method {
        Method(string name, TypeSpecifier ret_type, AccessModifier access_modifier, std::vector<string>,
            std::vector<TypeSpecifier>, std::vector<std::shared_ptr<Expression>>);
        typedef std::unique_ptr<Method> UniquePtr;

        string method_name;
        string symbol_name;
        TypeSpecifier return_type;

        std::vector<string> argument_names;
        std::vector<TypeSpecifier> argument_types;
        std::vector<std::shared_ptr<Expression>> argument_def_values;
        AccessModifier  access_modifier;

        TypeSpecifier* ret_type_ref;

        llvm::Function* declared_func;
    };

    class Class {
    public:
        Class(string&, Class*, std::vector<string>&, std::vector<pair<string, TypeSpecifier>>&, ClassDecl*, Namespace*,
            bool = false);

        void declare_field(string, TypeSpecifier, AccessModifier, Expression::SharedPtr = nullptr);
        Method* declare_method(string, TypeSpecifier, AccessModifier, std::vector<string>,
            std::vector<TypeSpecifier>, std::vector<std::shared_ptr<Expression>>, AstNode * = nullptr);

        string finalize();
        void inherit();

        inline bool is_generic() {
            return !generics.empty();
        }
        inline std::vector<pair<string, TypeSpecifier>> get_generics() {
            return generics;
        }

        string has_method(string, std::vector<Expression::SharedPtr>, TypeCheckVisitor&, AstNode* = nullptr,
            std::vector<AstNode*> = {}, bool = true);
        string has_method(string, std::vector<Expression::SharedPtr>, std::vector<TypeSpecifier>&, TypeCheckVisitor&,
            AstNode* = nullptr, std::vector<AstNode*> = {}, bool = true);
        string has_method(string, std::vector<TypeSpecifier>, AstNode* = nullptr,
            std::vector<AstNode*> = {}, bool = true);
        string ancestor_has_method(string, std::vector<TypeSpecifier>);

        bool has_field(string);

        Method* get_method(string);
        Field* get_field(string);

        inline bool isAbstract() {
            return is_abstract;
        }

        inline bool implements(string name) {
            return std::find(implements_.begin(), implements_.end(), name) != implements_.end();
        }

        inline Namespace* get_namespace() {
            return _namespace;
        }

        bool protected_prop_accessible_from(string);
        bool private_prop_accessible_from(string);
        bool is_base_class_of(string);

        inline Class* get_parent() {
            return parent_class;
        }
        inline std::vector<pair<string, string>>& getVTable() {
            return virtual_methods;
        }

        void assign_vpos();
        bool is_virtual(Method*);
        void implement(std::vector<llvm::Type*>&, unordered_map<string, int>&);

        inline std::vector<pair<string, Field*>>& getFields() {
            return fields;
        }

        inline size_t methodCount() {
            return methods.size();
        }

        inline std::map<string, std::vector<string>>& getInterfMethods() {
            return interface_methods;
        }

        inline std::unordered_map<string, llvm::Value*>& getInterfVTables() {
            return interface_vtables;
        }

        inline unsigned int getVTableOffset(string interface_name) {
            return vtable_offset[interface_name];
        }

        inline unsigned int getDepth() {
            return depth;
        }

        inline std::vector<Method*>& getConstructors() {
            return constructors;
        }

        TypeSpecifier& getType();

        typedef std::unique_ptr<Class> UniquePtr;

    protected:
        unsigned int depth = 0;

        string class_name;

        Class* parent_class;
        std::vector<Class*> extended_by = {};

        ClassDecl* declaration;

        std::vector<string> implements_;
        std::map<string, std::vector<string>> interface_methods;
        std::vector<string> ignore_in_vtable;

        std::vector<pair<string, Field*>> fields = {};
        std::unordered_multimap<string, Method::UniquePtr> methods = {};

        std::vector<string> field_symbols;
        std::vector<pair<string, Method*>> method_symbols;
        std::unordered_map<string, TypeSpecifier> method_ret_types;

        std::vector<Method*> constructors;

        std::vector<string> field_names;
        std::vector<string> method_names;

        std::unordered_map<string, unsigned int> vtable_offset;
        std::unordered_map<string, llvm::Value*> interface_vtables;
        
        Namespace* _namespace;

        TypeSpecifier type;

        std::vector<pair<string, TypeSpecifier>> generics = {};
        bool is_abstract = false;

        std::vector<pair<string, string>> virtual_methods;
    };

    class Interface {
    public:
        Interface(string);
        void add_field(string, TypeSpecifier);
        void add_method(string, TypeSpecifier, std::vector<TypeSpecifier>, AstNode* = nullptr);

        typedef std::unique_ptr<Interface> UniquePtr;
        friend class Class;

    protected:
        string interface_name;

        std::vector<string> field_names;
        std::vector<string> method_names;
        std::vector<string> field_symbols;
        std::vector<string> method_symbols;
    };

} // namespace cl
} // namespace cdot

#endif //CDOT_CLASS_H
