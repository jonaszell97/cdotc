//
// Created by Jonas Zell on 04.07.17.
//

#include <iostream>
#include <fstream>
#include "TypeCheckVisitor.h"
#include "../Visitor.cpp"
#include "../../Namespace.h"
#include "../../Statement/Declaration/Class/InterfaceDecl.h"
#include "Class.h"
#include "../../../Message/Warning.h"
#include "../CodeGen/CGType.h"
#include "../../Statement/Declaration/ExtendStmt.h"

std::vector<std::string> TypeCheckVisitor::classes = {};
std::vector<std::string> TypeCheckVisitor::interfaces = {};
std::vector<Namespace*> TypeCheckVisitor::namespaces = {};
std::unordered_multimap<std::string, std::pair<std::string, TypeSpecifier>> TypeCheckVisitor::class_props = {};
std::unordered_multimap<std::string, std::pair<std::string, TypeSpecifier>> TypeCheckVisitor::static_class_props = {};
std::unordered_multimap<string, TypeSpecifier> TypeCheckVisitor::GlobalVariables = {};
string this_str = "this";

TypeCheckVisitor::TypeCheckVisitor(TypeCheckVisitor *parent, bool copy_ret_type) : parent(parent), variables{} {
    if (namespaces.empty()) {
        namespaces.push_back(Namespace::get("Global"));
    }

    if (parent != nullptr) {
        parent->add_child(this);
        continuable = parent->continuable;
        breakable = parent->breakable;
        scope = parent->scope + std::to_string(parent->children.size());
        current_class = parent->current_class;
        current_this = parent->current_this;
        if (copy_ret_type) {
            check_return_type = parent->check_return_type;
            declared_return_type = parent->declared_return_type;
        }
    }
}

void TypeCheckVisitor::dump() {
    int tab = 0;
    auto current = parent;
    while (current != nullptr) {
        ++tab; current = current->parent;
    }
    for (auto symb : variables) {
        for (int i = 0; i < tab; ++i) {
            std::cout << "\t";
        }
        std::cout << symb.first << ": " << symb.second.to_string() << std::endl;
    }
}

/**
 * Declares a variable in the current context
 * @param name
 * @param type
 * @param cause
 */
void TypeCheckVisitor::declare_var(std::string& name, TypeSpecifier& type, bool global, AstNode *cause) {
    auto& container = global ? GlobalVariables : variables;

    if (container.find(name) != container.end()) {
        RuntimeError::raise(ERR_REDECLARED_VAR, "Redeclaration of variable " + name, cause);
    }

    namespaces.back()->insert(name, type, scope);
    container.emplace(name, type);
}

/**
 * Declares a function (overload) in the current context
 * @param fun
 * @param args
 * @param ret
 * @param cause
 */
TypeSpecifier& TypeCheckVisitor::declare_fun(std::string& fun, std::vector<TypeSpecifier>& args, TypeSpecifier& ret,
        AstNode *cause) {
    auto overloads = variables.equal_range(fun);
    auto score = util::func_score(args);

    for (auto it = overloads.first; it != overloads.second; ++it) {
        auto& overload = it->second;
        std::vector<TypeSpecifier> generics;
        std::vector<pair<string, TypeSpecifier>> generics2;
        auto res = util::func_call_compatible(args, overload.args, generics, generics2);
        if (res.is_compatible && res.compat_score > score) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot redeclare function " + fun + " with a similar name to a "
                "previous declaration", cause);
        }
    }

    TypeSpecifier ts(OBJECT_T);
    ts.is_function = true;
    ts.args = args;
    ts.return_type = new TypeSpecifier(ret);

    namespaces.back()->insert(fun, ts.return_type, args, scope);
    variables.emplace(fun, ts);

    return *ts.return_type;
}

/**
 * Pushes a type on the type stack
 * @param type
 */
void TypeCheckVisitor::push_type(TypeSpecifier& type) {
    type_refs.push_back(&type);
}

/**
 * Removes a type from the type stack
 * @return
 */
void TypeCheckVisitor::pop_type() {
    type_refs.pop_back();
}

/**
 * Returns the latest type from the type stack
 * @return
 */
TypeSpecifier& TypeCheckVisitor::latest_type() {
    assert(!type_refs.empty() && "Reference to undeclared identifier");

    return *type_refs.back();
}

/**
 * Returns a variable if it exists, throws otherwise
 * @param ident
 * @param cause
 * @return
 */
std::pair<TypeSpecifier, std::string> TypeCheckVisitor::get_var(std::string& ident, AstNode* cause) {
    if (std::find(classes.begin(), classes.end(), ident) != classes.end()) {
        TypeSpecifier ts(CLASS_T);
        ts.class_name = ident;

        return { ts, "0" };
    }

    TypeCheckVisitor *current = this;
    std::string context = scope;

    while (current != nullptr && current->variables.find(ident) == current->variables.end()) {
        current = current->parent;
    }

    if (current == nullptr || current->variables.find(ident) == current->variables.end()) {
        if (GlobalVariables.find(ident) != GlobalVariables.end()) {
            auto var = GlobalVariables.equal_range(ident);
            return { var.first->second, "" };
        }

        RuntimeError::raise(ERR_UNDECLARED_VARIABLE, "Reference to undeclared identifier " + ident, cause);
    }

    context = current->scope;

    auto var = current->variables.equal_range(ident);
    return { var.first->second, context };
}

bool TypeCheckVisitor::has_var(string ident) {
    if (std::find(classes.begin(), classes.end(), ident) != classes.end()) {
        return true;
    }

    TypeCheckVisitor *current = this;
    std::string context = scope;

    while (current != nullptr && current->variables.find(ident) == current->variables.end()) {
        current = current->parent;
    }

    if (current == nullptr || current->variables.find(ident) == current->variables.end()) {
        return false;
    }

    return true;
}

/**
 * Checks for a matching function overload, throws if none or more than one is found
 * @param fun
 * @param args
 * @param cause
 * @return
 */
std::pair<TypeSpecifier, std::string> TypeCheckVisitor::get_fun(std::string& fun, std::vector<Expression::SharedPtr>&args) {

    TypeCheckVisitor* current = this;
    std::string context;
    while (current != nullptr && current->variables.find(fun) == current->variables.end()) {
        current = current->parent;
    }

    if (current == nullptr || current->variables.find(fun) == current->variables.end()) {
        return { TypeSpecifier(VOID_T), "-1" };
    }

    context = current->scope;

    pair<int, TypeSpecifier> best_match;

    auto overloads = current->variables.equal_range(fun);
    TypeSpecifier match;
    bool found_match = false;

    for (auto it = overloads.first; it != overloads.second; ++it) {
        auto& overload = it->second;
        std::vector<TypeSpecifier> generics;
        std::vector<pair<string, TypeSpecifier>> generics2;

        auto res = util::func_call_compatible(args, overload.args, *this, generics, generics2);
        if (res.perfect_match) {
            return { overload, context };
        }
        if (res.is_compatible && res.compat_score >= best_match.first) {
            found_match = true;
            best_match.first = res.compat_score;
            best_match.second = overload;
        }
    }

    if (!found_match) {
        return { TypeSpecifier(VOID_T), "-1" };
    }

    return { best_match.second, context };
}

/**
 * Returns from a context
 * @param ret_type
 * @param cause
 */
void TypeCheckVisitor::return_(TypeSpecifier& ret_type, AstNode *cause, bool increase_return) {
    if (is_function_body_visitor) {
        if (declared_return_type != AUTO_T) {
            if (!val::is_compatible(ret_type, declared_return_type)) {
                RuntimeError::raise(ERR_TYPE_ERROR, "Returning value of type " + ret_type.to_string() +
                    " from function with declared return type " + declared_return_type.to_string(), cause);
            }
            else if (ret_type != declared_return_type) {
                Warning::issue("Implicit cast from " + ret_type.to_string() + " to " + declared_return_type.to_string(),
                    cause);
            }
        }
        else {
            declared_return_type = ret_type;
        }

        return_type = ret_type;
        if (increase_return) {
            returned++;
        }
    }
    else if (parent != nullptr) {
        parent->declared_return_type = declared_return_type;
        parent->return_(ret_type, cause, increase_return && check_return_type);
    }
    else {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "Keyword 'return' is only allowed in function bodies", cause);
    }
}

void TypeCheckVisitor::update_ret_type(TypeSpecifier ts) {
    auto *current = this;
    while (current) {
        current->declared_return_type = ts;

        if (current->is_function_body_visitor) {
            break;
        }

        current = current->parent;
    }
}

void TypeCheckVisitor::import_builtin(std::vector<string> imp) {
    if (imp.size() == 2 && imp.at(1) == "Primitive") {
        auto ns = Namespace::declare_namespace("Primitive");
        ns->declare_typedef("int", TypeSpecifier(INT_T));
        ns->declare_typedef("bool", TypeSpecifier(BOOL_T));
        ns->declare_typedef("char", TypeSpecifier(CHAR_T));
    }
}

void TypeCheckVisitor::resolve(TypeSpecifier &ts) {
    ts.resolve(namespaces.back());
}

/**
 * Visits all the nodes children
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(NamespaceDecl *node) {
    Namespace::declare_namespace(node->ns_name, namespaces.back());
    namespaces.push_back(namespaces.back()->get_namespace(node->ns_name));

    node->contents->accept(*this);

    namespaces.pop_back();

    return {};
}

/**
 * Visits all the nodes children
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(CompoundStmt *node) {
    if (newly_created || node->keep_scope) {
        newly_created = false;
        for (const auto& child : node->get_children()) {
            child->accept(*this);
        }
    }
    else {
        TypeCheckVisitor t(this);
        for (const auto& child : node->get_children()) {
            child->accept(t);
        }
    }

    return {};
}

/**
 * Checks if all code paths in the function return a value of the correct type and places the function in the symbol
 * table
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(FunctionDecl *node) {
    if (namespaces.back()->has_class(node->_func_name)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot declare a function with the same name as a class declaration",
            node);
    }
    
    node->_return_type->accept(*this);
    auto& return_type = node->_return_type->getTypeSpecifier();

    std::vector<TypeSpecifier> arg_types;
    TypeCheckVisitor func_visitor(this);
    func_visitor.check_return_type = true;
    func_visitor.declared_return_type = return_type;
    func_visitor.returnable = true;
    func_visitor.is_function_body_visitor = true;

    // if the current scope is not global scope, we are in a nested function
    if (scope != "0") {
        func_visitor.is_nested_function = true;
    }

    for (auto arg : node->_args) {
        TypeSpecifier ts = arg->accept(*this);
        arg->bind(Namespace::symbol_from_var(arg->_arg_name, func_visitor.scope));
        arg_types.push_back(ts);
        func_visitor.declare_var(arg->_arg_name, arg->_arg_type->getTypeSpecifier());
    }

    TypeSpecifier& ret_type = declare_fun(node->_func_name, arg_types, return_type, node);
    current_fun = node->_func_name;

    attributes = node->attributes;

    node->_body->accept(func_visitor);

    attributes.clear();
    return_type = func_visitor.declared_return_type;
    ret_type = return_type;
    node->captures = func_visitor.captures;
    node->capture_types = func_visitor.capture_types;
    node->copy_targets = func_visitor.copy_targets;

    if (return_type.type == AUTO_T) {
        return_type = TypeSpecifier(VOID_T);
    }

    if (func_visitor.branches - func_visitor.returned > 0 && return_type.type != VOID_T &&
        !func_visitor.declared_return_type.nullable && node->_func_name != "Main")
    {
        RuntimeError::raise(ERR_TYPE_ERROR, "Not all code paths return a value", node);
    }
    // implicit 0 return for main function
    else if (func_visitor.branches - func_visitor.returned > 0 && node->_func_name == "Main") {
        return_type = TypeSpecifier(INT_T);
        node->_body->add_statement(std::make_shared<ReturnStmt>(
                std::make_shared<LiteralExpr>(Variant(0))
        ));
    }
    // implicit void return
    else if (func_visitor.branches - func_visitor.returned > 0) {
        node->_body->add_statement(std::make_shared<ReturnStmt>());
    }

    node->bind(Namespace::symbol_from_fun(node->_func_name, arg_types, scope));

    return {};
}

/**
 * Returns the type of a variable, if it exists
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(IdentifierRefExpr *node) {
    bool is_ns = false;

    // nested namespace
    if (namespaces.back()->has_namespace(node->_ident)) {
        namespaces.push_back(namespaces.back()->get_namespace(node->_ident));
        is_ns = true;
    }
    // global namespace
    else if (Namespace::is_namespace(node->_ident)) {
        namespaces.push_back(Namespace::get(node->_ident));
        is_ns = true;
    }

    node->is_ns = is_ns;

    if (is_ns) {
        if (node->_member_expr == nullptr) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot use a namespace as a value", node);
        }

        if (node->_member_expr->get_type() == NodeType::MEMBER_EXPR) {
            std::static_pointer_cast<MemberRefExpr>(node->_member_expr)->is_ns_member = true;
        }
        else if (node->_member_expr->get_type() == NodeType::CALL_EXPR) {
            std::static_pointer_cast<CallExpr>(node->_member_expr)->is_ns_member = true;
        }
        else {
            RuntimeError::raise(ERR_TYPE_ERROR, "Unknown namespace operation", node);
        }

        return node->_member_expr->accept(*this);
    }

    bool implicit_this = node->_ident != "this" && !has_var(node->_ident) && current_class != "";
    if (node->_ident == "this" || implicit_this) {
        if (current_class.empty() || current_method_is_static) {
            RuntimeError::raise(ERR_CONTEXT_ERROR, "'this' can only be used in instance methods", node);
        }

        if (implicit_this) {
            auto mem_ref = std::make_shared<MemberRefExpr>(node->_ident);

            mem_ref->set_member_expr(node->_member_expr);
            node->set_member_expr(mem_ref);
        }

        node->binding = current_this;
        node->is_this = true;
        node->_ident = "this";

        TypeSpecifier t(OBJECT_T);
        t.class_name = current_class;

        if (node->_member_expr == nullptr) {
            return t;
        }
        else {
            push_type(t);
            return node->_member_expr->accept(*this);
        }
    }

    auto res = get_var(node->_ident, node);
    node->target_type = res.first;

    if (res.first.type == CLASS_T) {
        node->is_class = true;
    }

    // add capture for this variable
    if (is_nested_function && variables.find(node->_ident) == variables.end()) {
        capture_types.push_back(res.first);
        captures.push_back({node->_ident, Namespace::symbol_from_var(node->_ident, res.second)});
        copy_targets.push_back(Namespace::symbol_from_var(node->_ident, res.second));
        node->is_captured_var = true;

        node->binding = current_this;

        return res.first;
    }
    else {
        node->bind(Namespace::symbol_from_var(node->_ident, res.second));
    }

    node->declaration = declarations[node->binding];

    if (node->_member_expr == nullptr) {
        return res.first;
    }
    else {
        push_type(res.first);
        return node->_member_expr->accept(*this);
    }
}

/**
 * Declares one or more variables
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(DeclStmt *node) {
    auto raw_array = false;
    for (const auto& attr : node->attributes) {
        if (attr == "RawArray") {
            raw_array = true;
        }
        else {
            RuntimeError::raise(ERR_TYPE_ERROR, "Attribute " + attr + " is not applicable on declarations", node);
        }
    }

    auto& ident = node->identifier;

    node->type->accept(*this);
    auto& decl_type = node->type->getTypeSpecifier();
    auto& val = node->value;

    decl_type.raw_array = raw_array;
    if (raw_array) {
        decl_type.class_name = "";
    }

    if (decl_type.is_pointer && !has_attribute("UnsafePtr")) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Pointer types not allowed by default. Use @UnsafePtr to allow "
            "pointer types", node);
    }

    if (val != nullptr) {

        push_type(decl_type);
        val->set_inferred_type(decl_type);

        if (val->get_type() == NodeType::LAMBDA_EXPR) {
            std::static_pointer_cast<LambdaExpr>(val)->self_ident = ident;
        }

        TypeSpecifier given_type = val->accept(*this);
        decl_type.static_const = given_type.static_const;

        if (decl_type.type == AUTO_T) {
            if (given_type.type == VOID_T) {
                RuntimeError::raise(ERR_TYPE_ERROR, "Cannot assign value of type void to variable with inferred "
                    "type", val.get());
            }

            decl_type = given_type;
        }
        else if (!val::is_compatible(given_type, decl_type)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Incompatible types " + given_type.to_string() + " and " +
                decl_type.to_string(), val.get());
        }
        else if (decl_type != given_type && (given_type.raw_array || given_type.is_lambda)) {
            decl_type = given_type;
        }

        pop_type();
    }
    else if (!decl_type.nullable && !val::has_default(decl_type)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Expected non-nullable variable " + ident + " to be defined",
            node);
    }

    auto binding = node->is_global ? Namespace::symbol_from_var(ident) : Namespace::symbol_from_var(ident, scope);
    node->binding = binding;
    declare_var(ident, decl_type, node->is_global, node);

    declarations.emplace(binding, node);

    return {};
}

/**
 * Checks a for statement
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(ForStmt *node) {
    TypeCheckVisitor init_visitor(this, true);
    node->_initialization->accept(init_visitor);
    node->_increment->accept(init_visitor);
    node->_termination->accept(init_visitor);

    TypeCheckVisitor body_visitor(&init_visitor, true);
    body_visitor.continuable = true;
    body_visitor.breakable = true;

    node->_body->accept(body_visitor);

    return {};
}

/**
 * Checks a while statement
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(WhileStmt *node) {
    node->_condition->accept(*this);
    TypeCheckVisitor body_visitor(this);
    body_visitor.continuable = true;
    body_visitor.breakable = true;

    node->_while_block->accept(body_visitor);

    return {};
}

/**
 * Checks an array literal
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(ArrayLiteral *node) {

    for (const auto& attr : node->attributes) {
        if (attr == "RawArray") {
            node->inferred_type.raw_array = true;
            node->inferred_type.class_name = "";
        }
        else {
            RuntimeError::raise(ERR_TYPE_ERROR, "Attribute " + attr + " is not applicable on list literals", node);
        }
    }

    TypeSpecifier ts(OBJECT_T);
    ts.raw_array = node->inferred_type.raw_array;
    ts.class_name = ts.raw_array ? "" : "Array";
    ts.arr_length = node->inferred_type.arr_length;

    if (node->inferred_type.type != AUTO_T) {
        ts.element_type = node->inferred_type.element_type;
    }
    else if (!node->_elements.empty()) {
        ts.element_type = new TypeSpecifier(node->_elements.front()->accept(*this));
    }
    else {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot infer type of empty array literal", node);
    }

    ts.concrete_generic_types.emplace("T", *ts.element_type);

    for (int i = 1; i < node->_elements.size(); ++i) {
        const auto& el = node->_elements[i];
        auto el_type = el->accept(*this);
        if (!val::is_compatible(*ts.element_type, el_type)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Incompatible types " + ts.element_type->to_string() + " and " +
                el_type.to_string());
        }
    }

    node->type = ts;

    if (node->_member_expr == nullptr) {
        return ts;
    }
    else {
        push_type(ts);
        return node->_member_expr->accept(*this);
    }
}

/**
 * Returns the type of the literal
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(LiteralExpr *node) {
    auto ret_type = node->_type;

    if (node->inferred_type.type == OBJECT_T) {
        auto& ns = namespaces.back();

        string constr_name = ns->get_class(node->_type.class_name)->has_method("init", { node->_type });
        ret_type = node->inferred_type;

        if (!node->preserve_state) {
            node->autobox_constr = constr_name;
            node->autobox = true;
        }
    }
    else if (!node->preserve_state) {
        node->autobox = false;
    }

    if (node->_member_expr == nullptr) {
        return ret_type;
    }
    else {
        push_type(ret_type);
        return node->_member_expr->accept(*this);
    }
}

/**
 * Returns type string
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(StringLiteral *node) {
    TypeSpecifier res = TypeSpecifier(OBJECT_T);
    res.class_name = "String";

    for (const auto& attr : node->attributes) {
        if (attr == "CString") {
            res.cstring = true;
            node->inferred_type.cstring = true;
        }
        else {
            RuntimeError::raise(ERR_TYPE_ERROR, "Attribute " + attr + " is not applicable on string literals", node);
        }
    }

    if (node->_member_expr == nullptr) {
        return res;
    }
    else {
        push_type(res);
        return node->_member_expr->accept(*this);
    }
}

/**
 * Returns the arrays type
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(ArrayAccessExpr *node) {
    if (!node->_index) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Expected array index", node);
    }

    const auto& ns = namespaces.back();
    auto& ts = latest_type();
    resolve(ts);

    if (ts.type == OBJECT_T && ns->has_class(ts.class_name)) {
        auto cl = ns->get_class(ts.class_name);
        std::string op;
        CallExpr::SharedPtr call = nullptr;
        bool final_return_lval = false;

        auto current = node->_parent_expr;
        while (current != nullptr) {
            final_return_lval = current->lvalue;
            current = current->_parent_expr;
        }

        if (final_return_lval && !(op = cl->has_method("operator[]&", { node->_index }, *this)).empty()) {
            call = std::make_shared<CallExpr>(CallType::METHOD_CALL, std::vector<Expression::SharedPtr>{node->_index },
                "operator[]&");
        }
        else if (!(op = cl->has_method("operator[]", { node->_index }, *this)).empty()) {
            call = std::make_shared<CallExpr>(CallType::METHOD_CALL, std::vector<Expression::SharedPtr>{node->_index },
                "operator[]");

        }

        if (call != nullptr) {
            node->overriden_call = call;
            node->is_subscript_op = true;

            push_type(ts);
            auto type = call->accept(*this);
            pop_type();

            if (node->_member_expr == nullptr) {
                return type;
            }
            else {
                push_type(type);
                auto ret = node->_member_expr->accept(*this);
                pop_type();

                return ret;
            }
        }
    }

    auto index_type = node->_index->accept(*this);
    if (!ts.raw_array) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Operator [" + index_type.to_string() + "] is not defined on type " +
            ts.to_string(), node);
    }

    node->arr_type = ts;

    if (node->_member_expr == nullptr) {
        return *ts.element_type;
    }
    else {
        push_type(*ts.element_type);
        return node->_member_expr->accept(*this);
    }
}

/**
 * Checks a function call for the validity of the arguments
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(CallExpr *node) {
    std::vector<AstNode*> arg_nodes;
    for (auto arg : node->args) {
        arg_nodes.push_back(arg.get());
    }

    TypeSpecifier ret_type;
    const auto ns = namespaces.back();

    if (node->type == CallType::FUNC_CALL) {
        // call operator
        if (node->ident.empty() && !type_refs.empty()) {
            auto &latest = latest_type();
            if (latest.type == OBJECT_T && ns->has_class(latest.class_name) && !latest.is_function) {
                auto cl = ns->get_class(latest.class_name);
                std::string op;
                if (!(op = cl->has_method("operator()", node->args, *this)).empty()) {
                    node->type = CallType::METHOD_CALL;
                    node->ident = "operator()";
                    node->binding = op;

                    ret_type = cl->get_method(op)->return_type;
                    goto end;
                }
                else {
                    RuntimeError::raise(ERR_TYPE_ERROR, "Wrong arguments supplied for function call", node);
                }
            }
        }

        if (util::builtin_functions.find(node->ident) != util::builtin_functions.end()) {
            for (const auto& arg : node->args) {
                arg->accept(*this);
            }

            if (node->_member_expr == nullptr) {
                return TypeSpecifier(util::builtin_functions[node->ident]);
            } else {
                push_type(util::builtin_functions[node->ident]);
                return node->_member_expr->accept(*this);
            }
        }
        else if (node->ident == "") {
            auto func = latest_type();

            if (!func.is_function) {
                RuntimeError::raise(ERR_TYPE_ERROR, "Cannot call value of type " + func.to_string(), node);
            }
            else if (!util::func_call_compatible(node->args, func.args, *this).is_compatible) {
                RuntimeError::raise(ERR_TYPE_ERROR, "Trying to call function " + func.to_string() + " with "
                    "incompatible arguments", node);
            }

            node->type = CallType::ANON_CALL;
            ret_type = *func.return_type;
        }
        else {
            // constructor call
            if (ns->has_class(node->ident)) {
                string constr_name;
                auto cl = ns->get_class(node->ident);

                if ((constr_name = cl->has_method("init", node->args, node->generics, *this, node, arg_nodes)) == "") {
                    if (node->generics.size() != cl->get_generics().size()) {
                        RuntimeError::raise(ERR_TYPE_ERROR, "Mismatched generic type count", node);
                    }

                    RuntimeError::raise(ERR_TYPE_ERROR, "Class " + node->ident + " does not define a constructor with "
                        "given signature", node);
                }

                auto method = cl->get_method(constr_name);

                if (method->access_modifier == AccessModifier::PROTECTED &&
                        !cl->protected_prop_accessible_from(current_class)) {
                    RuntimeError::raise(ERR_CONTEXT_ERROR, "Protected method " + method->method_name + " of class " +
                        node->ident + " is not accessible", node);
                }

                if (method->access_modifier == AccessModifier::PRIVATE &&
                        !cl->private_prop_accessible_from(current_class)) {
                    RuntimeError::raise(ERR_CONTEXT_ERROR, "Private method " + method->method_name + " of class " +
                        node->ident + " is not accessible", node);
                }

                if (cl->isAbstract()) {
                    RuntimeError::raise(ERR_TYPE_ERROR, "Cannot instantiate abstract class " + node->ident, node);
                }

                if (cl->is_generic()) {
                    node->return_type = method->return_type;
                }
                else {
                    node->return_type = method->return_type;
                }

                if (node->inferred_type.type == INTERFACE_T) {
                    node->interface_shift = true;
                }

                node->type = CallType::CONSTR_CALL;
                node->binding = constr_name;

                ret_type = node->return_type;
            }
            else {
                auto func = get_fun(node->ident, node->args);

                // implicit this
                if (!current_this.empty() && func.second == "-1") {
                    node->type = CallType::METHOD_CALL;
                    node->implicit_this_call = true;
                    node->this_val = current_this;

                    TypeSpecifier t(OBJECT_T);
                    t.class_name = current_class;

                    push_type(t);
                    auto ret = visit(node);
                    pop_type();

                    return ret;
                }

                if (func.second == "-1") {
                    RuntimeError::raise(ERR_UNDECLARED_VARIABLE, "No matching call for function " + node->ident + " "
                        "found", node);
                }

                node->ret_type = *func.first.return_type;

                if (node->ident == current_fun && declared_return_type.type == AUTO_T) {
                    RuntimeError::raise(ERR_TYPE_ERROR, "Cannot infer type of recursive function", node);
                }

                size_t i = 0;
                for (const auto& arg : node->args) {
                    arg->set_inferred_type(func.first.args[i]);
                    arg->accept(*this);
                }

                if (func.first.is_lambda) {
                    node->is_lambda_call = true;
                    node->type = CallType::ANON_CALL;
                    node->binding = Namespace::symbol_from_var(node->ident, func.second);
                }
                else {
                    node->binding = Namespace::symbol_from_fun(node->ident, func.first.args, func.second);
                }

                ret_type = *func.first.return_type;
            }
        }
    }
    else if (node->type == CallType::METHOD_CALL) {
        if (type_refs.empty()) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot call method on given value", node);
        }
        if (!ns->has_class(latest_type().class_name)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Class " + latest_type().class_name + " does not exist", node);
        }
        
        auto& target = latest_type();

        bool static_ = target.type == CLASS_T;
        node->is_static = static_;
        
        cdot::cl::Class* cl = ns->get_class(target.class_name);
        node->class_name = target.class_name;

        if (target.type != OBJECT_T) {
            string constr_name = ns->get_class(target.class_name)->has_method("init", { target });
            node->autobox_constr = constr_name;
        }

        std::string symbol_name;
        if (!(symbol_name = cl->has_method(node->ident, node->args, *this, node, arg_nodes)).empty()) {
            cdot::cl::Method* method = cl->get_method(symbol_name);

            if (method->return_type.is_generic) {
                TypeSpecifier t = method->return_type;
                TypeSpecifier::resolveGeneric(t, latest_type().concrete_generic_types);

                node->return_type = t;
                node->generic_return_type = t;
                node->needs_generic_cast = true;
            }
            else {
                node->return_type = method->return_type;
            }

            node->bind(method->symbol_name);
            node->method = method;
            node->is_virtual = cl->isAbstract() || cl->is_virtual(method);

            if (method->access_modifier == AccessModifier::PROTECTED && !cl->protected_prop_accessible_from(current_class)) {
                RuntimeError::raise(ERR_CONTEXT_ERROR, "Protected method " + method->method_name + " of class " +
                    target.class_name + " is not accessible", node);
            }
            else if (method->access_modifier == AccessModifier::PRIVATE && !cl->private_prop_accessible_from(current_class))
            {
                RuntimeError::raise(ERR_CONTEXT_ERROR, "Private method " + method->method_name + " of class " +
                    target.class_name + " is not accessible", node);
            }

            ret_type = node->return_type;
        }
        else {
            RuntimeError::raise(ERR_TYPE_ERROR, "Method " + node->ident + " does not exist on class " +
                target.class_name, node);
        }
    }
    else if (node->type == CallType::CONSTR_CALL) {
        ret_type.type = OBJECT_T;
        ret_type.class_name = node->ident;
    }

    end:
    if (node->_member_expr == nullptr) {
        return ret_type;
    }
    else {
        push_type(ret_type);
        return node->_member_expr->accept(*this);
    }
}

/**
 * Checks if a class member exists and is of correct type
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(MemberRefExpr *node) {
    // namespace
    if (node->is_ns_member) {
        if (!namespaces.back()->has_var(node->_ident)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Reference to undeclared namespace member " + node->_ident, node);
        }

        auto ts = namespaces.back()->get_var(node->_ident);
        node->binding = "." + namespaces.back()->get_name() + "." + node->_ident;

        if (node->_member_expr == nullptr) {
            return ts;
        }
        else {
            push_type(ts);
            return node->_member_expr->accept(*this);
        }
    }

    if (latest_type().type != OBJECT_T && latest_type().type != CLASS_T) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot access property on primitive value " + node->_ident + " of type "
            + latest_type().to_string(), node);
    }

    bool static_ = latest_type().type == CLASS_T;
    cdot::cl::Class* cl = namespaces.back()->get_class(latest_type().class_name);

    node->class_name = latest_type().class_name;
    node->is_static = static_;

    if (cl->has_field(node->_ident)) {
        cdot::cl::Field *field = cl->get_field(node->_ident);

        if (field->field_type.is_generic && latest_type().concrete_generic_types.find(field->field_type.generic_class_name) !=
                latest_type().concrete_generic_types.end()) {
            TypeSpecifier t = latest_type().concrete_generic_types[field->field_type.generic_class_name];
            node->field_type = t;
            node->generic_return_type = t;
            node->needs_generic_cast = true;
        }
        else {
            node->field_type = field->field_type;
        }

        node->binding = field->symbol_name;

        if (field->access_modifier == AccessModifier::PROTECTED && !cl->protected_prop_accessible_from(current_class)) {
            RuntimeError::raise(ERR_CONTEXT_ERROR, "Protected field " + field->field_name + " of class " +
                latest_type().class_name + " is not accessible", node);
        }
        else if (field->access_modifier == AccessModifier::PRIVATE && !cl->private_prop_accessible_from(current_class)) {
            RuntimeError::raise(ERR_CONTEXT_ERROR, "Private field " + field->field_name + " of class " +
                latest_type().class_name + " is not accessible", node);
        }

        if (node->_member_expr == nullptr) {
            return node->field_type;
        }
        else {
            push_type(node->field_type);
            return node->_member_expr->accept(*this);
        }
    }

    RuntimeError::raise(ERR_TYPE_ERROR, "Field " + node->_ident + " does not exist on class " + latest_type()
        .class_name, node);
}

/**
 * Returns the type of a binary expression based on the input types
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(BinaryOperator *node) {
    TypeSpecifier fst = node->_first_child->accept(*this);
    node->_second_child->set_inferred_type(fst);

    TypeSpecifier snd = node->_second_child->accept(*this);
    std::string op = node->_operator;

    if (fst.type == AUTO_T && snd.type == AUTO_T) {
        return TypeSpecifier(AUTO_T);
    }

    if (fst.is_pointer) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Pointer arithmetic is not allowed. Use * to dereference the pointer",
            node->_first_child.get());
    }

    if (snd.is_pointer) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Pointer arithmetic is not allowed. Use * to dereference the pointer",
            node->_second_child.get());
    }

    bool is_overload = false;
    std::string class_name;
    std::vector<TypeSpecifier> overload_args;

    if ((fst.type == OBJECT_T || fst.type == INTERFACE_T) && !fst.is_function) {
        class_name = fst.class_name;
        overload_args.push_back(snd);
        is_overload = true;
    }
    else if (util::is_reversible(op) && (snd.type == OBJECT_T || snd.type == INTERFACE_T) && !snd.is_function) {
        class_name = snd.class_name;
        overload_args.push_back(fst);
        is_overload = true;
    }

    if (is_overload && class_name != "") {
        std::string bin_op;
        cdot::cl::Class* cl = namespaces.back()->get_class(class_name);
        if (!(bin_op = cl->has_method("operator" + op, { node->_second_child }, *this)).empty()) {
            push_type(fst);
            auto call = std::make_shared<CallExpr>(CallType::METHOD_CALL,
                std::vector<Expression::SharedPtr>{ node->_second_child }, "operator" + op);

            node->overriden_call = call;
            node->is_overriden = true;

            return call->accept(*this);
        }
    }

    if (op == "+=" || op == "-=" || op == "*=") {
        TypeSpecifier ret_type = val::simple_arithmetic_return_type(fst, snd);
        if (ret_type.type == VOID_T) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '" + op + "' values of type " + fst
                    .to_string() + " and " + snd.to_string(), node);
        }

        if (!val::is_compatible(fst, ret_type)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot assign value of type " + TypeSpecifier(ret_type).to_string() +
                " to variable of type " + fst.to_string(), node);
        }
        if (snd.type != ret_type.type) {
            auto cast = new ImplicitCastExpr(snd, ret_type, node->_second_child);
            node->_second_child.reset(cast);
        }

        node->operand_type = ret_type;

        op = "=";
        snd = TypeSpecifier(ret_type);
    }
    else if (op == "/=") {
        ValueType ret_type = val::division_return_type(fst, snd);
        if (ret_type == VOID_T) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot divide values of type " + TypeSpecifier(ret_type).to_string()
                + " and " + snd.to_string(), node);
        }

        if (fst.type != ret_type) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot assign value of type " + TypeSpecifier(ret_type).to_string()
                    + " to variable of type " + fst.to_string(), node);
        }
        if (snd.type != ret_type) {
            auto cast = new ImplicitCastExpr(snd, ret_type, node->_second_child);
            node->_second_child.reset(cast);
        }

        node->operand_type = TypeSpecifier { ret_type };

        op = "=";
        snd = TypeSpecifier(ret_type);
    }

    if (op == "=") {
        if (fst.is_const) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Trying to reassign variable declared 'const'", node->_first_child.get());
        }
        else if (!val::is_compatible(snd, fst)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot assign value of type " + snd.to_string() + " to variable of "
                    "type " + fst.to_string(), node);
        }

        auto type = node->_first_child->get_type();
        if (type != NodeType::IDENTIFIER_EXPR && type != NodeType::MEMBER_EXPR && type != NodeType::ARRAY_ACCESS_EXPR) {
            string deref = "";
            if (type == NodeType::UNARY_OPERATOR) {
                auto op = std::static_pointer_cast<UnaryOperator>(node->_first_child);
                if (op->_operator == "*") {
                    op->return_lvalue(true);
                }
                else {
                    RuntimeError::raise(ERR_TYPE_ERROR, "Value must be lvalue", node->_first_child.get());
                }
            }
            else {
                RuntimeError::raise(ERR_TYPE_ERROR, "Value must be lvalue", node->_first_child.get());
            }
        }

        node->operand_type = fst;

        return snd;
    }
    else if (op == "+" || op == "-" || op == "*") {
        if ((fst.raw_array || snd.raw_array) && op != "+") {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '" + op + "' values of type " + fst
                .to_string() + " and " + snd.to_string(), node);
        }
        if (fst.raw_array || snd.raw_array) {
            node->return_lvalue(true);
        }

        TypeSpecifier ret_type = val::simple_arithmetic_return_type(fst, snd);

        if (ret_type.type == VOID_T) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '" + op + "' values of type " + fst
                .to_string() + " and " + snd.to_string(), node);
        }

        if (fst.type != ret_type.type || (fst.is_unsigned && !ret_type.is_unsigned) || (fst.bitwidth < ret_type.bitwidth)) {
            auto cast = new ImplicitCastExpr(fst, ret_type, node->_first_child);
            node->_first_child.reset(cast);
        }
        if (snd.type != ret_type.type || (snd.is_unsigned && !ret_type.is_unsigned) || (snd.bitwidth < ret_type.bitwidth)) {
            auto cast = new ImplicitCastExpr(snd, ret_type, node->_second_child);
            node->_second_child.reset(cast);
        }

        ret_type.static_const = fst.static_const && snd.static_const;

        node->operand_type = ret_type;

        return ret_type;
    }
    else if (op == "**") {
        TypeSpecifier ret_type(fst.type == FLOAT_T ? FLOAT_T : DOUBLE_T);
        if (!val::implicitly_castable(fst.type, FLOAT_T) && !val::implicitly_castable(fst.type, DOUBLE_T)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Left hand side of '**' must be a floating point number",
                node->_first_child.get());
        }
        if (!val::implicitly_castable(snd.type, INT_T)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Right hand side of '**' must be of type integer",
                node->_second_child.get());
        }
        if (fst.type != ret_type.type) {
            auto cast = new ImplicitCastExpr(fst, ret_type, node->_first_child);
            node->_first_child.reset(cast);
        }
        if (snd.type != INT_T) {
            auto cast = new ImplicitCastExpr(snd, TypeSpecifier(INT_T), node->_second_child);
            node->_second_child.reset(cast);
        }

        node->operand_type = ret_type;

        return ret_type;
    }
    else if (op == "/" || op == "%") {
        ValueType ret_type = op == "%" ? fst.type : val::division_return_type(fst, snd);
        if (ret_type == VOID_T) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot divide values of type " + fst.to_string() + " and " +
                snd.to_string(), node);
        }

        if (fst.type != ret_type) {
            auto cast = new ImplicitCastExpr(fst, ret_type, node->_first_child);
            node->_first_child.reset(cast);
        }

        TypeSpecifier res = TypeSpecifier(ret_type);
        res.static_const = fst.static_const && snd.static_const;

        node->operand_type = ret_type;

        return res;
    }
    // only applicable to int and long
    else if (op == "&" || op == "|" || op == "^" || op == "<<" || op == ">>") {
        if (!val::implicitly_castable(fst.type, INT_T) && !val::implicitly_castable(fst.type, LONG_T)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '" + op + "' values of type " + fst
                .to_string() + " and " + snd.to_string(), node);
        }
        if (!val::implicitly_castable(snd.type, INT_T) && !val::implicitly_castable(snd.type, LONG_T)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '" + op + "' values of type " + fst
                .to_string() + " and " + snd.to_string(), node);
        }

        TypeSpecifier res = TypeSpecifier((fst.type == LONG_T || snd.type == LONG_T) ? LONG_T : INT_T);
        res.static_const = fst.static_const && snd.static_const;

        if (fst.type != res.type) {
            auto cast = new ImplicitCastExpr(fst, res, node->_first_child);
            node->_first_child.reset(cast);
        }

        node->operand_type = res;

        return res;
    }
    // only applicable to bool
    else if (op == "&&" || op == "||") {
        if (!val::implicitly_castable(fst.type, BOOL_T) && !val::implicitly_castable(snd.type, BOOL_T)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '" + op + "' values of type " + fst
                .to_string() + " and " + snd.to_string(), node);
        }

        TypeSpecifier res = TypeSpecifier(BOOL_T);
        res.static_const = fst.static_const && snd.static_const;

        if (fst.type != res.type) {
            auto cast = new ImplicitCastExpr(fst, res, node->_first_child);
            node->_first_child.reset(cast);
        }
        if (snd.type != res.type) {
            auto cast = new ImplicitCastExpr(snd, res, node->_second_child);
            node->_second_child.reset(cast);
        }

        return res;
    }
    else if (op == "..") {
        if (!val::implicitly_castable(fst.type, INT_T) && !val::implicitly_castable(fst.type, LONG_T)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '" + op + "' values of type " + fst
                    .to_string() + " and " + snd.to_string(), node);
        }
        if (!val::implicitly_castable(snd.type, INT_T) && !val::implicitly_castable(snd.type, LONG_T)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '" + op + "' values of type " + fst
                    .to_string() + " and " + snd.to_string(), node);
        }

        TypeSpecifier ts;
        ts.type = (fst.type == LONG_T || snd.type == LONG_T) ? LONG_T : INT_T;
        ts.static_const = fst.static_const && snd.static_const;

        if (!ts.static_const) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Ranges can only be created from compile-time constant values", node);
        }

        if (fst.type != ts.type) {
            auto cast = new ImplicitCastExpr(fst, ts, node->_first_child);
            node->_first_child.reset(cast);
        }

        ts.class_name = "Array";

        return ts;
    }
    else if (op == "??") {
        if (fst != snd) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '??' to values of different types", node);
        }

        return fst;
    }
    else if (op == "==" || op == "!=") {
        if (!val::implicitly_castable(fst.type, snd.type)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot compare values of type " + fst
                .to_string() + " and " + snd.to_string() + " for equality", node);
        }

        if (fst.type != snd.type) {
            auto cast = new ImplicitCastExpr(fst, snd, node->_first_child);
            node->_first_child.reset(cast);
        }

        TypeSpecifier res = TypeSpecifier(BOOL_T);
        res.static_const = fst.static_const && snd.static_const;

        node->operand_type = fst.type;

        return res;
    }
    else if (op == "<=" || op == ">=" || op == "<" || op == ">") {
        if (!fst.is_primitive || !snd.is_primitive) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot compare values of type " + fst
                    .to_string() + " and " + snd.to_string(), node);
        }

        if (fst.type != snd.type) {
            auto cast = new ImplicitCastExpr(fst, snd, node->_first_child);
            node->_first_child.reset(cast);
        }

        TypeSpecifier res = TypeSpecifier(BOOL_T);
        res.static_const = fst.static_const && snd.static_const;

        node->operand_type = fst.type;

        return res;
    }

    RuntimeError::raise(ERR_TYPE_ERROR, "Unknown binary operator '" + op + "'", node);

    return {};
}

/**
 * Chechs if an explicit cast is possible
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(ExplicitCastExpr *node) {
    node->from = node->_child->accept(*this);
    resolve(node->to);
    resolve(node->from);

    if (!val::is_castable(node->from, node->to)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot cast from " + node->from.to_string() + " to " + node->to.to_string(),
            node->_child.get());
    }

    return node->to;
}

/**
 * Checks if the condition is boolean and the values are of the same type
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(TertiaryOperator *node) {
    TypeSpecifier cond = node->condition->accept(*this);

    if (!val::implicitly_castable(cond.type, BOOL_T)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Condition of tertiary operator '?:' must be boolean or implement "
            "instance method 'toBool() -> bool'", node);
    }

    if (cond.type != BOOL_T) {
        Warning::issue("Implicit cast to boolean", node->condition.get());
    }

    TypeSpecifier fst = node->lhs->accept(*this);
    TypeSpecifier snd = node->rhs->accept(*this);

    if (fst.type == AUTO_T && snd.type != AUTO_T) {
        fst = snd;
    }
    if (snd.type == AUTO_T && fst.type != AUTO_T) {
        snd = fst;
    }

    if (!val::is_compatible(fst, snd) || !val::is_compatible(snd, fst)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply tertiary operator '?:' to values of type " + fst.to_string() +
            " and " + snd.to_string(), node);
    }
    else if (fst != snd) {
        Warning::issue("Implicit cast from " + snd.to_string() + " to " + fst.to_string(), node->rhs.get());

        auto cast = new ImplicitCastExpr(snd, fst, node->rhs);
        node->rhs.reset(cast);
    }

    node->result_type = fst;
    
    return fst;
}

/**
 * Returns the result type of a unary expression depending on the input type
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(UnaryOperator *node) {
    const auto ns = namespaces.back();

    std::string op = node->_operator;

    TypeSpecifier target = node->_child->accept(*this);

    if (node->declaration != nullptr) {
        auto declared_type = node->declaration->type->accept(*this);
        if (declared_type.type != AUTO_T) {
            target = declared_type;
        }
    }

    node->operand_type = target;

    if (target.type == OBJECT_T || target.type == INTERFACE_T) {
        auto class_decl = ns->get_class(target.class_name);
        std::string bin_op;
        if (!(bin_op = class_decl->has_method("operator" + op, {}, *this)).empty()) {
            push_type(target);
            auto call = std::make_shared<CallExpr>(CallType::METHOD_CALL, std::vector<Expression::SharedPtr>{ }, "operator" + op);

            node->overriden_call = call;
            node->is_overriden = true;

            return call->accept(*this);
        }
    }

    if (op == "typeof") {
        TypeSpecifier res = TypeSpecifier(STRING_T);
        res.static_const = true;

        node->immediate_return_val = std::make_shared<StringLiteral>(target.to_string());

        return res;
    }

    if (target.type == OBJECT_T) {
        auto props = class_props.equal_range(target.class_name);
        for (auto it = props.first; it != props.second; ++it) {
            if (it->second.first == op && it->second.second.args.empty()) {
                auto name = target.class_name + "_" + op;
                node->bind(Namespace::symbol_from_fun(name, it->second.second.args, scope));
                return *it->second.second.return_type;
            }
        }
    }

    if (op == "++" || op == "--" || op == "+" || op == "-") {
        if (target.type != INT_T && target.type != LONG_T && target.type != DOUBLE_T && target.type != FLOAT_T) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Unary operator " + op + " is not applicable to type " + target.to_string(),
                node->_child.get());
        }

        
        return target;
    }
    else if (op == "~") {
        if (target != INT_T && target != LONG_T) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Unary operator '~' is only applicable to types int and long",
                node->_child.get());
        }

        
        return target;
    }
    else if (op == "!") {
        if (!val::implicitly_castable(target.type, BOOL_T)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Unary operator '!' is not applicable to type " + target
                .to_string(), node->_child.get());
        }
        if (target.type != BOOL_T) {
            Warning::issue("Implicit cast to boolean", node->_child.get());
        }

        TypeSpecifier res = TypeSpecifier(BOOL_T);
        res.static_const = target.static_const;

                if (target.type != BOOL_T) {
            node->cast_to = res;
            node->cast_needed = true;
        }

        return res;
    }
    else if (op == "*") {
        if (!has_attribute("UnsafePtr")) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Pointer operators are only allowed in functions with the UnsafePtr "
                "attribute", node);
        }
        if (!target.is_pointer) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Trying to dereference non-pointer type", node->_child.get());
        }

        TypeSpecifier ts(target);
        ts.is_pointer = false;
        node->operand_type = target;

        return ts;
    }
    else if (op == "&") {
        if (!has_attribute("UnsafePtr")) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Pointer operators are only allowed in functions with the UnsafePtr "
                "attribute", node);
        }
        if (node->_child->get_type() == NodeType::IDENTIFIER_EXPR) {
            IdentifierRefExpr::SharedPtr ref_expr = std::dynamic_pointer_cast<IdentifierRefExpr>(node->_child);
            node->operand_type.is_reference = true;
            return node->operand_type;
        }
        else {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply unary operator '&' to non-reference value", node->_child.get());
        }
    }

    RuntimeError::raise(ERR_TYPE_ERROR, "Unknown unary operator '" + op + "'", node);

    return {};
}

/**
 * Checks if a break statement is valid
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(BreakStmt *node) {
    if (!breakable) {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "Keyword 'break' is only allowed in switch, for and while statements",
            node);
    }

    broken = true;

    return {};
}

/**
 * Checks if a continue statement is valid
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(ContinueStmt *node) {
    if (!continuable) {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "Keyword 'continue' is only allowed in for and while statements",
            node);
    }

    continued = true;

    return {};
}

/**
 * Checks if the condition is boolean (or convertible), and if all branches return
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(IfStmt *node) {
    TypeSpecifier t = node->_condition->accept(*this);
    if (!val::implicitly_castable(t.type, BOOL_T)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Condition must be boolean", node->_condition.get());
    }

    // if there's no else, the remaining code path needs to return either way
    if (check_return_type && node->_else_branch) {

        TypeCheckVisitor if_visitor(this, true);
        if_visitor.returnable = true;

        node->_if_branch->accept(if_visitor);

        TypeCheckVisitor else_visitor(this, true);
        else_visitor.returnable = true;

        node->_else_branch->accept(else_visitor);

        // all branches return
        if (if_visitor.branches - if_visitor.returned <= 0 && declared_return_type.type != VOID_T
                && else_visitor.branches - else_visitor.returned <= 0) {
            returned++;
        }
    }
    else {
        TypeCheckVisitor if_visitor(this, true);
        if_visitor.returnable = returnable;

        node->_if_branch->accept(if_visitor);
    }

    return {};
}

/**
 * Checks if the switch and case types are compatible, and if all branches return
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(SwitchStmt *node) {
    TypeSpecifier switch_type = node->switch_val->accept(*this);

    if (check_return_type) {
        TypeCheckVisitor case_visitor(this);
        case_visitor.returnable = true;
        case_visitor.breakable = true;
        case_visitor.declared_return_type = declared_return_type;
        case_visitor.check_return_type = true;
        bool all_branches_return = true;
        bool default_exists = false;
        //TODO check if exhausted

        int i = 0;
        for (auto case_ : node->cases) {
            if (!case_->is_default) {
                TypeSpecifier case_type = case_->case_val->accept(*this);

                if (!val::is_compatible(switch_type, case_type)) {
                    RuntimeError::raise(ERR_TYPE_ERROR, "Incompatible types in switch statement: " + switch_type
                            .to_string() + " and " + case_type.to_string(), case_.get());
                } else if (case_type != switch_type) {
                    Warning::issue("Implicit cast from " + case_type.to_string() + " to " + switch_type.to_string(),
                            case_.get());
                }
            }
            else {
                default_exists = true;
                node->has_default = true;
                node->default_index = i;
            }

            case_->accept(case_visitor);

            if (case_visitor.branches - case_visitor.returned > 0 && declared_return_type.type != VOID_T) {
                all_branches_return = false;
            }
            case_visitor.reset();
            i++;
        }

        if (all_branches_return && default_exists) {
            returned++;
        }
    }
    else {
        TypeCheckVisitor case_visitor(this);
        case_visitor.breakable = true;
        for (const auto& case_ : node->cases) {
            TypeSpecifier case_type = case_->case_val->accept(*this);
            if (!val::is_compatible(switch_type, case_type)) {
                RuntimeError::raise(ERR_TYPE_ERROR, "Incompatible types in switch statement: " + switch_type
                    .to_string() + " and " + case_type.to_string(), case_.get());
            }
            else if (case_type != switch_type) {
                Warning::issue("Implicit cast from " + case_type.to_string() + " to " + switch_type.to_string(),
                    case_.get());
            }

            case_->accept(case_visitor);
        }
    }

    return {};
}

/**
 * Iterates over all children
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(CaseStmt *node) {
    node->fallthrough = true;
    for (const auto& child : node->get_children()) {
        if (child->get_type() == NodeType::BREAK_STMT) {
            node->fallthrough = false;
        }
        child->accept(*this);
    }

    return {};
}

/**
 * Does nothing
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(LabelStmt *node) {
    if (std::find(labels.begin(), labels.end(), node->label_name) != labels.end()) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Label '" + node->label_name + "' already exists in the same scope", node);
    }

    labels.push_back(node->label_name);

    return {};
}

/**
 * Does nothing
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(GotoStmt *node) {
    if (!has_label(node->label_name)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "No label '" + node->label_name + "' to go to", node);
    }

    return {};
}

/**
 * Checks a function argument declaration for type consistency of default value and existence of declared type
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(FuncArgDecl *node) {
    node->_arg_type->accept(*this);
    TypeSpecifier& ts = node->_arg_type->getTypeSpecifier();

    for (const auto& attr : node->attributes) {
        if (attr == "RawArray") {
            ts.raw_array = true;
            ts.class_name = "";
        }
        else {
            RuntimeError::raise(ERR_TYPE_ERROR, "Attribute " + attr + " is not applicable on list literals", node);
        }
    }

    if (node->_default_val) {
        TypeSpecifier def_type = node->_default_val->accept(*this);
        if (!val::is_compatible(def_type, ts)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Default value for parameter " + node->_arg_name + " must be of type "
                "" + node->_arg_type->to_string(), node->_default_val.get());
        }
        else if (def_type != node->_arg_type->getTypeSpecifier()) {
            Warning::issue("Implicit cast from " + def_type.to_string() + " to " + node->_arg_type->to_string(),
                    node->_default_val.get());
        }

        ts.nullable = true;
    }

    return ts;
}

/**
 * Returns the current context
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(ReturnStmt *node) {

    if (check_return_type && node->_return_val) {
        node->_return_val->inferred_type = declared_return_type;

        TypeSpecifier ret_type = node->_return_val->accept(*this);

        if (ret_type.type == OBJECT_T && node->_return_val->get_type() == NodeType::IDENTIFIER_EXPR) {
            auto ident = std::static_pointer_cast<IdentifierRefExpr>(node->_return_val);
            if (declarations.find(ident->_ident) != declarations.end()) {
                auto decl = declarations[ident->_ident];
                decl->alloc_on_heap();
            }
        }

        if (node->_return_val->declaration != nullptr) {
            node->_return_val->declaration->alloc_on_heap();
        }
        else {
            node->_return_val->alloc_on_heap();
        }

        node->ret_type = ret_type;

        // return type inference
        if (declared_return_type.type == VOID_T) {
            update_ret_type(ret_type);
        }

        return_(ret_type, node->_return_val.get());
    }
    else {
        TypeSpecifier ret_type(VOID_T);
        node->ret_type = ret_type;
        return_(ret_type, node);
    }

    return {};
}

/**
 * Does nothing
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(Expression *node) {
    if (node->_child != nullptr) {
        return node->_child->accept(*this);
    }

    return {};
}

/**
 * Declares a new class
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(ClassDecl *node) {
    classes.push_back(node->class_name);

    TypeSpecifier t(node->is_abstract ? INTERFACE_T : CLASS_T);
    t.class_name = node->class_name;
    push_type(t);

    if (node->extends == "" && node->class_name != "Any") {
        node->extends = "Any";
    }

    auto cl = namespaces.back()->declare_class(node->class_name, node->extends, node->implements, node->generics, node,
        node->is_abstract);
    auto ns = cl->get_namespace();
    ns->insert(node->class_name, t, scope);
    node->class_decl = cl;

    namespaces.push_back(ns);

    for (const auto& generic : node->generics) {
        ns->declare_typedef(generic.first, generic.second);
    }

    for (const auto& td : node->typedefs) {
        td->accept(*this);
    }

    for (const auto& field : node->fields) {
        field->accept(*this);
    }

    // first run declares methods & constructors
    for (const auto& method : node->methods) {
        method->accept(*this);
    }
    if (!node->constructors.empty()) {
        for (const auto& constr : node->constructors) {
            constr->accept(*this);
        }
    }
    else if (!node->is_abstract) {
        TypeSpecifier* ts = new TypeSpecifier(OBJECT_T);
        ts->class_name = latest_type().class_name;
        ns->get_class(latest_type().class_name)->declare_method("init", *ts, AccessModifier::PUBLIC, {}, {}, {});

        node->this_binding = ns->symbol_from_var(this_str, TypeCheckVisitor(this).scope);
    }

    // second run checks them
    for (const auto& method : node->methods) {
        method->accept(*this);
    }
    for (const auto& constr : node->constructors) {
        constr->accept(*this);
    }

    std::string res = cl->finalize();
    if (res != "") {
        RuntimeError::raise(ERR_TYPE_ERROR, res, node);
    }

    namespaces.pop_back();

    return {};
}

/**
 * Declares a class constructor
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(ConstrDecl *node) {
    const auto ns = namespaces.back();

    std::vector<TypeSpecifier> arg_types;
    std::vector<std::string> arg_names;
    std::vector<Expression::SharedPtr> arg_defaults;
    for (auto arg : node->args) {
        arg_types.push_back(arg->accept(*this));
        arg_names.push_back(arg->_arg_name);
        arg_defaults.push_back(arg->_default_val);
    }

    if (node->declared) {
        TypeCheckVisitor t(this);
        t.current_class = latest_type().class_name;
        t.current_this = Namespace::symbol_from_var(this_str, t.scope);
        node->this_binding = t.current_this;

        for (auto arg : node->args) {
            arg->bind(Namespace::symbol_from_var(arg->_arg_name, t.scope));
            t.declare_var(arg->_arg_name, arg->_arg_type->getTypeSpecifier());
        }

        // constructor is not returnable
        node->body->accept(t);
    }
    else {
        auto cl = Namespace::global()->get_class(latest_type().class_name);
        auto& ts = cl->getType();

        node->class_name = ts.class_name;
        string method_name = "init";
        node->bind(Namespace::symbol_from_method(ts.class_name, method_name, arg_types));

        ns->insert(latest_type().class_name, &ts, std::vector<TypeSpecifier>(), scope);
        node->method = ns->get_class(latest_type().class_name)->declare_method(method_name, ts, node->am, arg_names,
            arg_types, arg_defaults);
    }

    node->declared = !node->declared;
    return {};
}

/**
 * Declares a class field and checks for correct type
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(FieldDecl *node) {
    auto props = (node->is_static) ? static_class_props.equal_range(latest_type().class_name)
                                   : class_props.equal_range(latest_type().class_name);
    for (auto it = props.first; it != props.second; ++it) {
        if (it->second.first == node->field_name) {
            RuntimeError::raise(ERR_REDECLARED_VAR, "Field " + node->field_name + " is declared twice", node);
        }

    }

    node->type->accept(*this);
    auto& field_type = node->type->getTypeSpecifier();

    for (const auto& attr : node->attributes) {
        if (attr == "RawArray") {
            field_type.raw_array = true;
            field_type.class_name = "";
        }
        else {
            RuntimeError::raise(ERR_TYPE_ERROR, "Attribute " + attr + " is not applicable on list literals", node);
        }
    }

    node->class_name = latest_type().class_name;

    if (node->default_val != nullptr) {
        TypeSpecifier def_type = node->default_val->accept(*this);

        if (field_type.type == AUTO_T) {
            field_type = def_type;
        }
        else if (!val::is_compatible(def_type, field_type)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Default value for field " + node->field_name + " must be of type " +
                node->type->to_string(), node->default_val.get());
        }
        else if (def_type != field_type) {
            Warning::issue("Implicit cast from " + def_type.to_string() + " to " + node->type->to_string(),
                node->default_val.get());
        }
    }

    const auto ns = namespaces.back();

    if (!current_decl_is_interface) {
        if (node->is_static) {
            node->binding = "." + latest_type().class_name + "." + node->field_name;
            ns->insert(latest_type().class_name + "." + node->field_name, field_type, scope);
            ns->declare_var(node->field_name, field_type);
            static_class_props.emplace(latest_type().class_name, pair<string, TypeSpecifier>(node->field_name,
                field_type));

            return {};
        } else {
            class_props.emplace(latest_type().class_name, pair<string, TypeSpecifier>(node->field_name, field_type));
        }

        auto cl = ns->get_class(latest_type().class_name);
        cl->declare_field(node->field_name, field_type, node->am, node->default_val);

        if (node->generate_getter) {
            std::vector<TypeSpecifier> arg_types;
            std::string getter_name = util::generate_getter_name(node->field_name);
            node->getter_binding = Namespace::symbol_from_method(node->class_name, getter_name, arg_types);

            cl->declare_method(getter_name, field_type, AccessModifier::PUBLIC, {}, {}, {}, node);
        }

        if (node->generate_setter) {
            std::vector<TypeSpecifier> arg_types{ field_type };
            std::string setter_name = util::generate_setter_name(node->field_name);
            node->setter_binding = Namespace::symbol_from_method(node->class_name, setter_name, arg_types);

            cl->declare_method(setter_name, TypeSpecifier(VOID_T), AccessModifier::PUBLIC, {node->field_name},
                arg_types, {}, node);
        }
    }
    else {
        ns->get_interface(latest_type().class_name)->add_field(node->field_name, field_type);
    }

    return {};
}

/**
 * Declares a method and checks if all code paths return
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(MethodDecl *node) {
    const auto ns = namespaces.back();
    std::vector<TypeSpecifier> arg_types;
    std::vector<std::string> arg_names;
    std::vector<Expression::SharedPtr> arg_defaults;
    std::vector<AstNode *> args;

    for (const auto &arg : node->args) {
        arg_types.push_back(arg->accept(*this));
        arg_names.push_back(arg->_arg_name);
        arg_defaults.push_back(arg->_default_val);
        args.push_back(arg.get());
    }

    node->return_type->accept(*this);
    auto& return_type = node->return_type->getTypeSpecifier();

    if (node->declared) {
        TypeCheckVisitor method_visitor(this);
        method_visitor.returnable = true;
        method_visitor.check_return_type = true;
        method_visitor.declared_return_type = return_type;
        method_visitor.current_class = latest_type().class_name;
        method_visitor.current_method_is_static = node->is_static;
        method_visitor.is_function_body_visitor = true;

        method_visitor.current_this = Namespace::symbol_from_var(this_str, method_visitor.scope);
        node->this_binding = method_visitor.current_this;

        attributes = node->attributes;

        if (node->body) {
            for (const auto &arg : node->args) {
                arg->bind(Namespace::symbol_from_var(arg->_arg_name, method_visitor.scope));
                method_visitor.declare_var(arg->_arg_name, arg->_arg_type->getTypeSpecifier());
            }

            node->body->accept(method_visitor);

            if (!method_visitor.returned) {
                return_type = TypeSpecifier(VOID_T);
            }
            else {
                return_type = method_visitor.declared_return_type;
            }

            node->method->return_type = return_type;
            *node->method->ret_type_ref = return_type;

            if (return_type.type == AUTO_T) {
                RuntimeError::raise(ERR_TYPE_ERROR, "Could not infer return type of method " + node->method_name, node);
            }

            if (method_visitor.branches - method_visitor.returned > 0 &&
                    return_type.type != VOID_T &&
                    !method_visitor.declared_return_type.nullable) {
                RuntimeError::raise(ERR_TYPE_ERROR, "Not all code paths return a value", node->body.get());
            }
        }
        else if (return_type.type == AUTO_T) {
            return_type.type = VOID_T;
            *node->method->ret_type_ref = return_type;
        }

        attributes.clear();
    }
    else {
        node->class_name = latest_type().class_name;
        node->binding = Namespace::symbol_from_method(node->class_name, node->method_name, arg_types);

        bool is_virtual = false;
        int vpos;
        if (!current_decl_is_interface) {
            auto class_decl = ns->get_class(latest_type().class_name);
            if (class_decl->has_method(node->method_name, arg_types, node, args, false) != "") {
                RuntimeError::raise(ERR_TYPE_ERROR, "Method " + node->method_name + " cannot be redeclared with a "
                    "similar signature to a previous declaration", node);
            }

            // check if method is virtual
            std::string symbol_name;
            if ((symbol_name = class_decl->ancestor_has_method(node->method_name, arg_types)) != "") {
                is_virtual = true;
            }
        }

        node->is_virtual = is_virtual;
        TypeSpecifier method_type(OBJECT_T);
        method_type.is_function = true;
        method_type.args = arg_types;
        method_type.return_type = new TypeSpecifier(return_type);

        if (!current_decl_is_interface) {
            if (node->is_static) {
                ns->insert(latest_type().class_name + "_" + node->method_name + "!st",
                    new TypeSpecifier(return_type), arg_types, scope);
                static_class_props.emplace(latest_type().class_name,
                    std::pair<std::string, TypeSpecifier>(node->method_name, method_type));
            } else {
                ns->insert(latest_type().class_name + "_" + node->method_name,
                    new TypeSpecifier(return_type), arg_types, scope);
                class_props.emplace(latest_type().class_name,
                    std::pair<std::string, TypeSpecifier>(node->method_name, method_type));
            }

            node->method = ns->get_class(latest_type().class_name)->declare_method(node->method_name,
                return_type, node->am, arg_names, arg_types, arg_defaults, node);
        } else {
            ns->get_interface(latest_type().class_name)->add_method(node->method_name,
                return_type, arg_types, node);
        }
    }

    node->declared = !node->declared;
    return {};
}

/**
 * Declares an operator
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(OperatorDecl *node) {
    const auto ns = namespaces.back();

    std::vector<TypeSpecifier> arg_types;
    std::vector<std::string> arg_names;
    std::vector<Expression::SharedPtr> arg_defaults;
    for (auto arg : node->args) {
        arg_types.push_back(arg->accept(*this));
        arg_names.push_back(arg->_arg_name);
        arg_defaults.push_back(arg->_default_val);
    }

    auto overloads = class_props.equal_range(node->_operator);
    for (auto it = overloads.first; it != overloads.second; ++it) {
        auto& overload = it->second.second;
        if (overload.args.size() == arg_types.size()) {
            bool clash = true;
            for (int i = 0; i < arg_types.size(); ++i) {
                if (!val::is_compatible(overload.args.at(i), arg_types.at(i))) {
                    clash = false;
                }
            }

            if (clash) {
                RuntimeError::raise(ERR_TYPE_ERROR, "Operator " + node->_operator + " cannot be redeclared with a "
                    "similar signature to a previous declaration", node);
            }
        }
    }

    if (node->body) {
        TypeCheckVisitor method_visitor(this);
        method_visitor.returnable = true;
        method_visitor.check_return_type = true;
        method_visitor.declared_return_type = node->return_type;
        method_visitor.current_class = latest_type().class_name;

        node->body->accept(method_visitor);
    }

    TypeSpecifier method_type(OBJECT_T);
    method_type.is_function = true;
    method_type.args = arg_types;
    method_type.return_type = new TypeSpecifier(node->return_type);

    if (!current_decl_is_interface) {
        ns->insert(latest_type().class_name + "_" + node->_operator, new TypeSpecifier(node->return_type),
            arg_types, scope);
        class_props.emplace(latest_type().class_name, std::pair<std::string, TypeSpecifier>(node->_operator,
            method_type));

        ns->get_class(latest_type().class_name)->declare_method(node->_operator, node->return_type, node->am,
            arg_names, arg_types, arg_defaults);
    }
    else {
        ns->get_interface(latest_type().class_name)->add_method(node->_operator, node->return_type,
            arg_types);
    }

    return {};
}

/**
 * Checks if all code paths return the correct type
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(LambdaExpr *node) {

    TypeCheckVisitor func_visitor(this);
    func_visitor.current_this = Namespace::symbol_from_var(this_str, func_visitor.scope);
    func_visitor.check_return_type = true;
    func_visitor.declared_return_type = node->_return_type;
    func_visitor.returnable = true;
    func_visitor.is_nested_function = true;
    func_visitor.is_function_body_visitor = true;

    std::vector<TypeSpecifier> arg_types;
    for (auto arg : node->_args) {
        TypeSpecifier ts = arg->accept(*this);
        arg->bind(Namespace::symbol_from_var(arg->_arg_name, func_visitor.scope));
        arg_types.push_back(ts);
        func_visitor.declare_var(arg->_arg_name, arg->_arg_type->getTypeSpecifier());
    }

    resolve(node->_return_type);

    TypeSpecifier ts(OBJECT_T);
    ts.is_function = true;
    ts.is_lambda = true;
    ts.args = arg_types;
    ts.return_type = &node->_return_type;
    ts.class_name = node->env_binding;
    ts.lambda_id = lambda_count++;
    node->lambda_id = ts.lambda_id;

    if (node->self_ident != "") {
        func_visitor.declare_var(node->self_ident, ts);
    }

    if (node->is_single_expr) {
        node->_return_type = node->_body->accept(func_visitor);
    }
    else {
        node->_body->accept(func_visitor);
        node->_return_type = func_visitor.return_type;

        if (func_visitor.branches - func_visitor.returned > 0 && node->_return_type != VOID_T &&
            !func_visitor.declared_return_type.nullable) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Not all code paths return a value", node);
        }
    }

    node->captures = func_visitor.captures;
    node->capture_types = func_visitor.capture_types;
    node->env_binding = func_visitor.current_this;

    // type might be used before LambdaExpr Codegen, so declare it here
    std::vector<llvm::Type*> capture_types;
    for (auto& capture : node->capture_types) {
        auto type = CGType::getType(capture);
        if (!type->isPointerTy()) {
            type = type->getPointerTo();
        }

        capture_types.push_back(type);
    }

    auto env_type = llvm::StructType::create(CodeGenVisitor::Context, capture_types, ".env");

    std::vector<llvm::Type*> larg_types{ env_type->getPointerTo() };
    for (const auto& arg : node->_args) {
        larg_types.push_back(CGType::getType(arg->_arg_type->getTypeSpecifier()));
    }

    auto func_type = llvm::FunctionType::get(CGType::getType(node->_return_type), larg_types, false);

    node->lambda_type = llvm::StructType::create(CodeGenVisitor::Context, { env_type->getPointerTo(),
        func_type->getPointerTo() }, ".lambda");
    CGType::declareEnvType(ts.lambda_id, node->lambda_type);

    if (node->_member_expr == nullptr) {
        return ts;
    }
    else if (node->_member_expr->get_type() != NodeType::CALL_EXPR) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Only call expressions can immediately follow a lambda expression", node);
    }
    else {
        push_type(ts);
        return node->_member_expr->accept(*this);
    }
}

/**
 * Does nothing
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(ImportStmt *node) {
    if (node->import.size() > 1 && node->import.front() == "System" && util::is_builtin_import(node->import.at(1))) {
        import_builtin(node->import);
        return {};
    }

    string file_path = __FILE__;
    string dir_path = file_path.substr(0, file_path.rfind("/")) + "/../../";

    std::ifstream f(dir_path.c_str());
    if(!f.good()) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot find module " + dir_path, node);
    }

    return {};
}

/**
 * Does nothing
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(ExportStmt *node) {
    for (const auto& child : node->get_children()) {
        child->accept(*this);
    }

    return {};
}

/**
 * Evaluates an interface declaration
 * @param node
 * @return
 */
TypeSpecifier TypeCheckVisitor::visit(InterfaceDecl *node) {
    namespaces.back()->declare_interface(node->name);
    current_decl_is_interface = true;
    interfaces.push_back(node->name);

    TypeSpecifier t(CLASS_T);
    t.class_name = node->name;
    push_type(t);

    for (auto field : node->fields) {
        field->accept(*this);
    }

    for (auto method : node->methods) {
        method->accept(*this);
    }

    for (auto op : node->operators) {
        op->accept(*this);
    }

    pop_type();
    current_decl_is_interface = false;

    return {};
}

TypeSpecifier TypeCheckVisitor::visit(ImplicitCastExpr *node) {
    resolve(node->to);
}

TypeSpecifier TypeCheckVisitor::visit(ExtendStmt *node) {
    const auto ns = namespaces.back();

    if (!ns->has_class(node->extended_class)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Class " + node->extended_class + " does not exist", node);
    }

    auto cl = ns->get_class(node->extended_class);
    TypeSpecifier t(CLASS_T);
    t.class_name = node->extended_class;
    push_type(t);

    for (const auto& field : node->fields) {
        field->accept(*this);
    }

    for (const auto& method : node->methods) {
        method->accept(*this);
    }

    for (const auto& method : node->methods) {
        method->accept(*this);
    }

    return {};
}

TypeSpecifier TypeCheckVisitor::visit(TypedefDecl *node) {
    const auto ns = namespaces.back();
    node->origin->accept(*this);
    ns->declare_typedef(node->alias, node->origin->getTypeSpecifier());

    return {};
}

TypeSpecifier TypeCheckVisitor::visit(TypeRef *node) {
    resolve(node->type);
    if (node->type.invalid_ns_ref) {
        RuntimeError::raise(ERR_TYPE_ERROR, string("Invalid ") + (node->type.ns_name.empty() ? "type" : "namespace") +
                " reference", node);
    }

    if (node->getTypeSpecifier().arr_length != nullptr) {
        node->getTypeSpecifier().arr_length->accept(*this);
    }

    if (util::in_vector<string>(node->attributes, "Boxed") && node->type.type != OBJECT_T) {
        node->type.type = OBJECT_T;
    }

    return node->type;
}