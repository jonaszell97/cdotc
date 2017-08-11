//
// Created by Jonas Zell on 09.07.17.
//

#include "Class.h"
#include "../../Namespace.h"
#include "../../Expression/Expression.h"
#include "../../../Message/Warning.h"
#include "../../Statement/Declaration/Class/ClassDecl.h"
#include "../CodeGen/CGType.h"

namespace cdot {
namespace cl {

    /**
     * Instantiates a class field
     * @param name
     * @param type
     * @param access_modifier
     */
    Field::Field(std::string name, TypeSpecifier type, AccessModifier access_modifier, Expression::SharedPtr def) :
        field_name(name), field_type(type), access_modifier(access_modifier), def_val(def) {

    }

    /**
     * Instantiates a class method
     * @param name
     * @param ret_type
     * @param access_modifier
     * @param arg_names
     * @param arg_types
     * @param arg_defaults
     */
    Method::Method(std::string name, TypeSpecifier ret_type, AccessModifier access_modifier, std::vector<std::string>
        arg_names, std::vector<TypeSpecifier> arg_types, std::vector<Expression::SharedPtr> arg_defaults) :
            method_name(name), return_type(ret_type), access_modifier(access_modifier),
            argument_def_values(arg_defaults), argument_names(arg_names), argument_types(arg_types)
    {

    }

    /**
     * Instantiates an interface
     * @param name
     */
    Interface::Interface(std::string name) : interface_name(name), field_symbols{}, method_symbols{}, field_names{},
                                             method_names{} {

    }

    /**
     * Adds a field declaration to an interface
     * @param name
     * @param type
     * @param is_static
     */
    void Interface::add_field(std::string name, TypeSpecifier type) {
        field_symbols.push_back(Namespace::symbol_from_var(name, type, false));
        field_names.push_back(name);
    }

    /**
     * Adds a method declaration ot an interface
     * @param name
     * @param ret_type
     * @param is_static
     * @param arg_types
     */
    void Interface::add_method(std::string name, TypeSpecifier ret_type, std::vector<TypeSpecifier> arg_types, AstNode* caller)
    {
        auto symb = Namespace::fun_with_ret_type(name, arg_types, ret_type);
        if (std::find(method_symbols.begin(), method_symbols.end(), symb) != method_symbols.end()) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Method " + name + " cannot be redeclared with a similar "
                "signature to a previous declaration", caller);
        }

        method_symbols.push_back(symb);
        method_names.push_back(name);
    }

    /**
     * Instantiates a class
     * @param class_name
     * @param parent
     * @param implements
     */
    Class::Class(std::string& class_name, Class* parent, std::vector<std::string>& implements,
        std::vector<pair<string, TypeSpecifier>>& generics, ClassDecl* decl,
        Namespace* ns, bool is_abstract) :
        class_name(class_name), parent_class(parent), implements_(implements), field_symbols{}, method_symbols{},
        declaration(decl), _namespace(ns), is_abstract(is_abstract), generics(generics)
    {
        if (parent != nullptr) {
            inherit();
            depth = parent->depth + 1;
            auto current = parent;
            while (current != nullptr) {
                current->extended_by.push_back(this);
                current = current->parent_class;
            }
        }

        type.type = OBJECT_T;
        type.class_name = class_name;
        type.is_generic = !generics.empty();
    }

    /**
     * Adds a field to a class
     * @param name
     * @param type
     * @param access
     * @param is_static
     */
    void Class::declare_field(std::string name, TypeSpecifier type, AccessModifier access, Expression::SharedPtr
        def_val)
    {
        auto field = new Field(name, type, access, def_val);
        field->symbol_name = "." + class_name + "." + name;
        fields.emplace_back(name, field);
        field_symbols.push_back(Namespace::symbol_from_var(name, type, false));
        field_names.push_back(name);
    }

    /**
     * Adds a method to a class
     * @param name
     * @param ret_type
     * @param access
     * @param is_static
     * @param arg_names
     * @param arg_types
     * @param arg_defaults
     */
    Method* Class::declare_method(std::string name, TypeSpecifier ret_type, AccessModifier access,
        std::vector<std::string>
        arg_names, std::vector<TypeSpecifier> arg_types, std::vector<Expression::SharedPtr> arg_defaults, AstNode *caller)
    {
        auto overloads = methods.equal_range(name);
        auto score = util::func_score(arg_types);

        for (auto it = overloads.first; it != overloads.second; ++it) {
            auto& overload = it->second;
            auto res = util::func_call_compatible(arg_types, overload->argument_types);
            if (res.is_compatible && res.compat_score >= score) {
                RuntimeError::raise(ERR_TYPE_ERROR, "Cannot redeclare function " + name + " with a similar name to a "
                    "previous declaration", caller);
            }
        }

        auto symb = Namespace::symbol_from_method(class_name, name, arg_types);
        auto method = std::make_unique<Method>(name, ret_type, access, arg_names, arg_types, arg_defaults);
        method->symbol_name = symb;

        auto ptr = method.get();


        methods.emplace(name, std::move(method));
        method_names.push_back(name);

        if (name != "init") {
            auto symb_name = Namespace::symbol_from_fun(name, arg_types, "");
            method_symbols.emplace_back(symb_name, ptr);
            method_ret_types.emplace(symb_name, ret_type);
            ptr->ret_type_ref = &method_ret_types[symb_name];
        }
        else {
            constructors.push_back(ptr);
        }

        return ptr;
    }

    /**
     * Returns whether or not a class or its base class has a field
     * @param field_name
     * @param is_static
     * @return
     */
    bool Class::has_field(std::string field_name) {
        if (util::in_pair_vector(fields, field_name)) {
            return true;
        }

        if (parent_class != nullptr) {
            return parent_class->has_field(field_name);
        }

        return false;
    }

    /**
     * Returns whether or not a class or its base class has a method
     * @param method_name
     * @param is_static
     * @return
     */
    std::string Class::has_method(std::string method_name, std::vector<Expression::SharedPtr> args,
            TypeCheckVisitor& Visitor, AstNode* caller, std::vector<AstNode*> arg_nodes, bool check_parent) {
        auto overloads = methods.equal_range(method_name);
        bool found_match = false;
        for (auto it = overloads.first; it != overloads.second; ++it) {
            auto& overload = it->second;
            if (args.size() == 0 && overload->argument_types.size() == 0) {
                return overload->symbol_name;
            }
            if (args.size() > overload->argument_types.size()) {
                continue;
            }

            bool broke = false;
            for (size_t i = 0; i < overload->argument_types.size(); ++i) {
                broke = true;

                if (i < args.size()) {
                    args[i]->set_inferred_type(overload->argument_types.at(i));
                    auto arg = args[i]->accept(Visitor);
                    if (!val::is_compatible(arg, overload->argument_types.at(i))) {
                        break;
                    }
                    else if (arg != overload->argument_types.at(i)) {
                        Warning::issue("Implicit cast from " + arg.to_string() + " to " +
                                overload->argument_types.at(i).to_string(), arg_nodes.size() > i ? arg_nodes.at(i) : caller);
                    }
                }
                else if (!overload->argument_types.at(i).nullable) {
                    break;
                }

                broke = false;
            }

            if (broke) {
                continue;
            }

            return overload->symbol_name;
        }

        if (parent_class != nullptr && check_parent && method_name != "init") {
            return parent_class->has_method(method_name, args, Visitor, caller, arg_nodes, check_parent);
        }

        return "";
    }

    /**
     * Returns whether or not a class or its base class has a method
     * @param method_name
     * @param is_static
     * @return
     */
    std::string Class::has_method(std::string method_name, std::vector<Expression::SharedPtr> args,
        std::vector<TypeSpecifier>& concrete_generics, TypeCheckVisitor& Visitor, AstNode* caller,
        std::vector<AstNode*> arg_nodes, bool check_parent)
    {
        auto overloads = methods.equal_range(method_name);
        bool found_match = false;
        pair<int, string> best_match;

        for (auto it = overloads.first; it != overloads.second; ++it) {
            auto& overload = it->second;

            auto res = util::func_call_compatible(args, overload->argument_types, Visitor, concrete_generics, generics);
            if (res.is_compatible) {
                found_match = true;
                if (res.perfect_match) {
                    return overload->symbol_name;
                }
                if (res.compat_score >= best_match.first) {
                    best_match.first = res.compat_score;
                    best_match.second = overload->symbol_name;
                }
            }
        }

        if (found_match) {
            return best_match.second;
        }

        if (parent_class != nullptr && check_parent && method_name != "init") {
            return parent_class->has_method(method_name, args, Visitor, caller, arg_nodes, check_parent);
        }

        return "";
    }

    string Class::has_method(string method_name, std::vector<TypeSpecifier> args, AstNode *caller,
            std::vector<AstNode *> arg_nodes, bool check_parent) {
        auto overloads = methods.equal_range(method_name);
        bool found_match = false;
        for (auto it = overloads.first; it != overloads.second; ++it) {
            auto& overload = it->second;
            if (args.size() == 0 && overload->argument_types.size() == 0) {
                return overload->symbol_name;
            }
            if (args.size() > overload->argument_types.size()) {
                continue;
            }

            bool broke = false;
            for (size_t i = 0; i < overload->argument_types.size(); ++i) {
                broke = true;

                if (i < args.size()) {
                    auto arg = args.at(i);
                    if (!val::is_compatible(arg, overload->argument_types.at(i))) {
                        break;
                    }
                    else if (arg != overload->argument_types.at(i)) {
                        Warning::issue("Implicit cast from " + arg.to_string() + " to " +
                            overload->argument_types.at(i).to_string(), arg_nodes.size() > i ? arg_nodes.at(i) : caller);
                    }
                }
                else if (!overload->argument_types.at(i).nullable) {
                    break;
                }

                broke = false;
            }

            if (broke) {
                continue;
            }

            return overload->symbol_name;
        }

        if (parent_class != nullptr && check_parent) {
            return parent_class->has_method(method_name, args, caller, arg_nodes, check_parent);
        }

        return "";
    }

    std::string Class::ancestor_has_method(std::string name, std::vector<TypeSpecifier> args) {
        if (parent_class) {
            string method;
            if (!(method = parent_class->has_method(name, args)).empty()) {
                return method;
            }
        }
        for (const auto& impl : implements_) {
            Class* interface = Namespace::latest()->get_class(impl);
            string method;
            if (!(method = interface->has_method(name, args)).empty()) {
                return method;
            }
        }

        return "";
    }

    /**
     * Returns a method, if it exists
     * @param method_name
     * @param is_static
     * @return
     */
    Method* Class::get_method(std::string method_name) {

        for (auto& method : methods) {
            if (method.second->symbol_name == method_name) {
                return method.second.get();
            }
        }

        if (parent_class != nullptr) {
            return parent_class->get_method(method_name);
        }

        return nullptr;
    }

    /**
     * Returns a field, if it exists
     * @param field_name
     * @param is_static
     * @return
     */
    Field* Class::get_field(std::string field_name) {
        if (util::in_pair_vector(fields, field_name)) {
            return util::get_second(fields, field_name);
        }

        if (parent_class != nullptr) {
            return parent_class->get_field(field_name);
        }

        return nullptr;
    }

    TypeSpecifier& Class::getType() {
        return type;
    }

    /**
     * Checks a class for correct implementation of all interfaces
     * @return An error, if it occurs
     */
    std::string Class::finalize() {
        for (const auto& interface_name : implements_) {
            if (!Namespace::latest()->has_class(interface_name)) {
                return "Interface " + interface_name + " does not exist";
            }

            Class* interface = Namespace::latest()->get_class(interface_name);
            if (!interface->is_abstract) {
                return "Class " + interface_name + " is not an interface";
            }

//            int i = 0;
//            for (auto field : interface->field_symbols) {
//                if (std::find(field_symbols.begin(), field_symbols.end(), field) == field_symbols.end()) {
//                    return "Class " + class_name + " does not correctly implement "
//                        "interface " + interface_name + ": Required field " + interface->field_names[i] + " is missing";
//                }
//
//                ++i;
//            }

            interface_methods.emplace(interface_name, std::vector<string>());
            auto& int_methods = interface_methods[interface_name];

            int i = 0;
            for (const auto& method : interface->method_symbols) {
                auto current = this;
                bool found = false;

                while (current != nullptr && !util::in_pair_vector(current->method_symbols, method.first)) {
                    current = current->parent_class;
                }

                if (current == nullptr) {
                    return "Class " + class_name + " does not correctly implement interface " + interface_name +
                        ": Required method " + interface->method_names[i] + " is missing or has incompatible signature";
                }

                auto& given = current->method_ret_types[method.first];
                auto& needed = interface->method_ret_types[method.first];

                if (!val::is_compatible(given, needed)) {
                    return "Class " + class_name + " does not correctly implement interface " + interface_name +
                        ": Required method " + interface->method_names[i] + " has incompatible return type (Expected " +
                        needed.to_string() + ", found " + given.to_string() + ")";
                }

                int_methods.push_back(util::get_second(current->method_symbols, method.first)->symbol_name);
                ignore_in_vtable.push_back(method.first);

                ++i;
            }
        }

        assign_vpos();

        return "";
    }

    void Class::inherit() {
        for (const auto& field : parent_class->fields) {
            fields.push_back(field);
        }

        implements_.insert(implements_.begin(), parent_class->implements_.begin(), parent_class->implements_.end());
    }

    void Class::implement(std::vector<llvm::Type*>& prop_types, unordered_map<string, int>& field_info) {
        size_t i = prop_types.size() - 1;
        for (const auto& impl : implements_) {
            auto interf = _namespace->get_class(impl);
            prop_types.push_back(llvm::ArrayType::get(CodeGenVisitor::Builder.getInt8PtrTy(),
                interface_methods[impl].size())->getPointerTo());

            vtable_offset.emplace(impl, ++i);

            for (const auto& field : interf->fields) {
                prop_types.push_back(CGType::getType(field.second->field_type));
                field_info.emplace(field.first, i++);
            }
        }
    }

    /**
     * Returns whether or not a protected property of this class can be accessed from within the given class
     * @param class_context
     * @return
     */
    bool Class::protected_prop_accessible_from(std::string class_context) {
        if (class_context == class_name) {
            return true;
        }

        for (auto child : extended_by) {
            if (child->protected_prop_accessible_from(class_context)) {
                return true;
            }
        }

        return false;
    }

    /**
     * Returns whether or not a protected property of this class can be accessed from within the given class
     * @param class_context
     * @return
     */
    bool Class::private_prop_accessible_from(std::string class_context) {
        return class_context == class_name;
    }

    /**
     * Returns whether or not this class extends the given class
     * @return
     */
    bool Class::is_base_class_of(std::string child) {
        auto base = Namespace::global()->get_class(child);
        auto current = base;
        while (current != nullptr) {
            if (this == current) {
                return true;
            }
            current = current->parent_class;
        }

        return false;
    }

    void Class::assign_vpos() {
        if (is_abstract) {
            return;
        }

        Class* base = this;
        int depth = 0;
        while (base->parent_class != nullptr) {
            base = base->parent_class;
            ++depth;
        }

        while (base != this) {
            for (const auto &method : base->method_symbols) {
                if (util::in_vector(base->ignore_in_vtable, method.first)) {
                    continue;
                }

                auto current = this;
                while (current != nullptr) {
                    if (current == base) {
                        break;
                    }
                    if (util::in_pair_vector(current->method_symbols, method.first)) {
                        if (!util::in_pair_vector(virtual_methods, method.first)) {
                            auto symb = util::get_second(current->method_symbols, method.first)->symbol_name;
                            virtual_methods.emplace_back(method.first, symb);
                        }
                        if (!util::in_pair_vector(base->virtual_methods, method.first)) {
                            auto symb = util::get_second(base->method_symbols, method.first)->symbol_name;
                            base->virtual_methods.emplace_back(method.first, symb);
                        }
                        break;
                    }

                    current = current->parent_class;
                }
            }

            --depth;

            base = this;
            for (int i = 0; i < depth; ++i) {
                base = base->parent_class;
            }
        }
    }

    bool Class::is_virtual(Method* method) {
        if (util::in_pair_vector(virtual_methods, Namespace::symbol_from_fun(method->method_name,
                method->argument_types, ""))) {
            return true;
        }

        for (const auto& child : extended_by) {
            if (child->is_virtual(method)) {
                return true;
            }
        }

        return false;
    }

} // namespace cl
} // namespace cdot