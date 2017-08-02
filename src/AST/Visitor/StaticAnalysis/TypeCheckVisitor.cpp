//
// Created by Jonas Zell on 04.07.17.
//

#include <iostream>
#include "TypeCheckVisitor.h"
#include "../Visitor.cpp"
#include "../../SymbolTable.h"

std::vector<std::string> TypeCheckVisitor::classes = {};
std::vector<std::string> TypeCheckVisitor::interfaces = {};
std::unordered_multimap<std::string, std::pair<std::string, TypeSpecifier>> TypeCheckVisitor::interface_props = {};
std::unordered_multimap<std::string, std::pair<std::string, TypeSpecifier>> TypeCheckVisitor::class_props = {};
std::unordered_multimap<std::string, std::pair<std::string, TypeSpecifier>> TypeCheckVisitor::static_class_props = {};

TypeCheckVisitor::TypeCheckVisitor(TypeCheckVisitor *parent) : parent(parent), variables{} {
    if (parent != nullptr) {
        parent->add_child(this);
        continuable = parent->continuable;
        breakable = parent->breakable;
        scope = parent->scope + std::to_string(parent->children.size());
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
void TypeCheckVisitor::declare_var(std::string name, TypeSpecifier type, AstNode *cause) {
    if (variables.find(name) != variables.end()) {
        RuntimeError::raise(ERR_REDECLARED_VAR, "Redeclaration of variable " + name, cause);
    }

    SymbolTable::insert(name, type, scope);
    variables.emplace(name, type);
}

/**
 * Declares a function (overload) in the current context
 * @param fun
 * @param args
 * @param ret
 * @param cause
 */
void TypeCheckVisitor::declare_fun(std::string fun, std::vector<TypeSpecifier> args, TypeSpecifier ret,
        AstNode *cause) {
    auto overloads = variables.equal_range(fun);
    for (auto it = overloads.first; it != overloads.second; ++it) {
        auto& overload = it->second;
        if (overload.args.size() == args.size()) {
            bool clash = true;
            for (size_t i = 0; i < args.size(); ++i) {
                if (!val::is_compatible(overload.args.at(i), args.at(i)) && !val::is_compatible(args.at(i), overload
                        .args.at(i))) {
                    clash = false;
                }
            }

            if (clash) {
                RuntimeError::raise(ERR_TYPE_ERROR, "Function " + fun + " cannot be redeclared with a similar "
                    "signature to a previous declaration", cause);
            }
        }
    }

    TypeSpecifier ts(OBJECT_T);
    ts.is_function = true;
    ts.args = args;
    ts.return_type = new TypeSpecifier(ret);

    SymbolTable::insert(fun, new TypeSpecifier(ret), args, scope);
    variables.emplace(fun, ts);
}

/**
 * Pushes a type on the type stack
 * @param type
 */
void TypeCheckVisitor::push_type(TypeSpecifier type) {
    type_refs.push_back(type);
}

/**
 * Removes a type from the type stack
 * @return
 */
TypeSpecifier TypeCheckVisitor::pop_type() {
    auto ret_val = type_refs.back();
    type_refs.pop_back();

    return ret_val;
}

/**
 * Returns the latest type from the type stack
 * @return
 */
TypeSpecifier TypeCheckVisitor::latest_type() {
    if (type_refs.size() == 0) {
        RuntimeError::raise(ERR_UNDECLARED_VARIABLE, "Reference to undeclared identifier");
    }

    return type_refs.back();
}

/**
 * Returns a variable if it exists, throws otherwise
 * @param ident
 * @param cause
 * @return
 */
std::pair<TypeSpecifier, std::string> TypeCheckVisitor::get_var(std::string ident, AstNode* cause) {
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
        RuntimeError::raise(ERR_UNDECLARED_VARIABLE, "Reference to undeclared identifier " + ident, cause);
    }

    context = current->scope;

    auto var = current->variables.equal_range(ident);
    return { var.first->second, context };
}

/**
 * Checks for a matching function overload, throws if none or more than one is found
 * @param fun
 * @param args
 * @param cause
 * @return
 */
std::pair<TypeSpecifier, std::string> TypeCheckVisitor::get_fun(std::string fun, std::vector<TypeSpecifier> args,
        AstNode* cause) {

    TypeCheckVisitor* current = this;
    std::string context;
    while (current != nullptr && current->variables.find(fun) == current->variables.end()) {
        current = current->parent;
    }

    if (current == nullptr || current->variables.find(fun) == current->variables.end()) {
        RuntimeError::raise(ERR_UNDECLARED_VARIABLE, "Function " + fun + " does not exist", cause);
    }

    context = current->scope;

    auto overloads = current->variables.equal_range(fun);
    TypeSpecifier match;
    bool found_match = false;
    for (auto it = overloads.first; it != overloads.second; ++it) {
        auto& overload = it->second;
        int broke = false;
        for (size_t i = 0; i < overload.args.size(); ++i) {
            if (i < args.size()) {
                if (!val::is_compatible(args.at(i), overload.args.at(i))) {
                    broke = true;
                    break;
                }
                else if (args.at(i) != overload.args.at(i)) {
                    Warning::issue("Implicit cast from " + args.at(i).to_string() + " to " +
                        overload.args.at(i).to_string(), cause);
                }
            }
            else if (!overload.args.at(i).nullable) {
                broke = true;
                break;
            }
        }

        if (found_match && !broke) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Call to function " + fun + " is ambiguous", cause);
        }

        if (!broke) {
            match = overload;
            found_match = true;
        }
    }

    if (!found_match) {
        RuntimeError::raise(ERR_UNDECLARED_VARIABLE, "No matching call for function " + fun + " found", cause);
    }

    return { match, context };
}

/**
 * Checks if a given function can be called with the supplied arguments
 * @param fun
 * @param fun_type
 * @param args
 * @param cause
 */
void TypeCheckVisitor::check_fun_call(std::string fun, TypeSpecifier fun_type, std::vector<TypeSpecifier> args,
        AstNode* cause) {
    if (args.size() > fun_type.args.size()) {
        RuntimeError::raise(ERR_UNDECLARED_VARIABLE, "No matching call for function " + fun + " found", cause);
    }

    for (int i = 0; i < fun_type.args.size(); ++i) {
        if (i < args.size()) {
            if (!val::is_compatible(args.at(i), fun_type.args.at(i))) {
                RuntimeError::raise(ERR_UNDECLARED_VARIABLE, "No matching call for function " + fun + " found", cause);
            }
            else if (args.at(i) != fun_type.args.at(i)) {
                Warning::issue("Implicit cast from " + args.at(i).to_string() + " to " +
                        fun_type.args.at(i).to_string(), cause);
            }
        }
        else if (!fun_type.args.at(i).nullable) {
            RuntimeError::raise(ERR_UNDECLARED_VARIABLE, "No matching call for function " + fun + " found", cause);
        }
    }
}

/**
 * Checks whether the supplied arguments match a function signature
 * @param fun_type
 * @param args
 * @return
 */
bool TypeCheckVisitor::fun_call_compatible(TypeSpecifier fun_type, std::vector<TypeSpecifier> args) {
    if (args.size() > fun_type.args.size()) {
        return false;
    }

    for (int i = 0; i < fun_type.args.size(); ++i) {
        if (i < args.size()) {
            if (!val::is_compatible(args.at(i), fun_type.args.at(i))) {
                return false;
            }
        }
        else if (!fun_type.args.at(i).nullable) {
            return false;
        }
    }

    return true;
}

/**
 * Returns from a context
 * @param ret_type
 * @param cause
 */
void TypeCheckVisitor::return_(TypeSpecifier ret_type, AstNode *cause) {
    if (returnable) {
        if (!val::is_compatible(ret_type, declared_return_type)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Returning value of type " + ret_type.to_string() +
                    " from function with declared return type " + declared_return_type.to_string(), cause);
        }
        else if (ret_type != declared_return_type) {
            Warning::issue("Implicit cast from " + ret_type.to_string() + " to " + declared_return_type.to_string(),
                    cause);
        }

        return_type = ret_type;
        returned++;
    }
    else if (parent != nullptr) {
        parent->return_(ret_type, cause);
    }
    else {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "Keyword 'return' is only allowed in function bodies", cause);
    }
}

/**
 * Visits all the nodes children
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(ModuleDecl *node) {
    newly_created = false;
    for (auto child : node->get_children()) {
        child->accept(*this);
    }

    return {};
}

/**
 * Visits all the nodes children
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(CompoundStmt *node) {
    if (newly_created) {
        newly_created = false;
        for (auto child : node->get_children()) {
            child->accept(*this);
            if (returned) {
                break;
            }
        }
    }
    else {
        TypeCheckVisitor t(this);
        for (auto child : node->get_children()) {
            child->accept(t);
            if (returned) {
                break;
            }
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
Variant TypeCheckVisitor::visit(FunctionDecl *node) {
    std::vector<TypeSpecifier> arg_types;
    TypeCheckVisitor t(this);
    t.check_return_type = true;
    t.declared_return_type = node->_return_type;
    t.returnable = true;

    for (auto arg : node->_args) {
        TypeSpecifier ts = arg->accept(*this).get_type();
        arg_types.push_back(ts);
        t.declare_var(arg->_arg_name, arg->_arg_type);
    }

    declare_fun(node->_func_name, arg_types, node->_return_type, node);

    node->_body->accept(t);

    if (t.branches - t.returned > 0 && t.declared_return_type.type != VOID_T && !t.declared_return_type.nullable) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Not all code paths return a value", node);
    }

    return {};
}

/**
 * Returns the type of a variable, if it exists
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(IdentifierRefExpr *node) {
    auto res = get_var(node->_ident, node);
    node->bind(SymbolTable::symbol_from_var(node->_ident, res.second));
    if (node->_member_expr == nullptr) {
        return res.first;
    }
    else {
        push_type(res.first);
        decl_ident = node->_ident;

        return node->_member_expr->accept(*this);
    }
}

/**
 * Declares one or more variables
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(DeclStmt *node) {
    for (int i = 0; i < node->decl_identifiers.size(); ++i) {
        auto& ident = node->decl_identifiers[i];
        auto& decl = node->declarations[i];

        if (decl.first.is_array) {
            if (decl.first.length == nullptr) {
                RuntimeError::raise(ERR_TYPE_ERROR, "Array length must be a static constant", node);
            }

            TypeSpecifier length = decl.first.length->accept(*this).get_type();
            if (!length.static_const) {
                RuntimeError::raise(ERR_TYPE_ERROR, "Array length must be a static constant", node);
            }
        }

        if (decl.second != nullptr) {

            push_type(decl.first);

            decl_ident = ident;
            TypeSpecifier given_type = decl.second->accept(*this).get_type();
            decl.first.static_const = given_type.static_const;

            if (decl.first.type == AUTO_T) {
                decl.first = given_type;
            }
            else if (given_type != decl.first) {
                RuntimeError::raise(ERR_TYPE_ERROR, "Incompatible types " + given_type.to_string() + " and " +
                    decl.first.to_string(), node);
            }

            pop_type();
        }
        else if (!decl.first.nullable && !decl.first.is_primitive) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Expected non-nullable variable " + ident + " to be defined",
                node);
        }

        node->bind(SymbolTable::symbol_from_var(ident, scope));
        declare_var(ident, decl.first, node);
    }

    return {};
}

/**
 * Checks a for statement
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(ForStmt *node) {
    TypeCheckVisitor init_visitor(this);
    node->_initialization->accept(init_visitor);
    node->_increment->accept(init_visitor);
    node->_termination->accept(init_visitor);

    TypeCheckVisitor body_visitor(init_visitor);
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
Variant TypeCheckVisitor::visit(WhileStmt *node) {
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
Variant TypeCheckVisitor::visit(ArrayLiteral *node) {
    // current ref is array name
    int i = 0;
    for (auto el : node->_elements) {
        TypeSpecifier el_type = el->accept(*this).get_type();

        declare_var(latest_type().to_string() + "[" + std::to_string(i++) + "]", el_type, node);
    }

    TypeSpecifier ts = node->type;
    ts.type = OBJECT_T;

    if (node->_member_expr == nullptr) {
        return { ts };
    }
    else {
        return node->_member_expr->accept(*this).get_type();
    }
}

/**
 * Returns the type of the literal
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(LiteralExpr *node) {
    return { node->_type };
}

/**
 * Returns type string
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(StringLiteral *node) {
    TypeSpecifier res = TypeSpecifier(STRING_T);
    res.static_const = true;

    return { res };
}

/**
 * Returns the arrays type
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(ArrayAccessExpr *node) {
    return latest_type();
}

/**
 * Checks a function call for the validity of the arguments
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(CallExpr *node) {
    std::vector<TypeSpecifier> arg_types;
    for (auto arg : node->_arguments) {
        arg_types.push_back(arg->accept(*this).get_type());
    }

    if (!latest_type().is_function) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot call value of type " + latest_type().to_string(), node);
    }

    check_fun_call("'anonymous closure'", latest_type(), arg_types, node);

    if (node->_member_expr == nullptr) {
        return { *latest_type().return_type };
    }
    else {
        push_type(*latest_type().return_type);
        return node->_member_expr->accept(*this);
    }
}

/**
 * Checks a function call for validity
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(FunctionCallExpr *node) {
    std::vector<TypeSpecifier> arg_types;
    for (auto arg : node->args) {
        arg_types.push_back(arg->accept(*this).get_type());
    }

    auto func = get_fun(node->_ident, arg_types, node);
    node->bind(SymbolTable::symbol_from_fun(node->_ident, func.first.args, func.second));
    if (node->_member_expr == nullptr) {
        return { func.first };
    }
    else {
        push_type(func.first);

        return node->_member_expr->accept(*this);
    }
}

/**
 * Checks if a class member exists and is of correct type
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(MemberRefExpr *node) {
    if (latest_type().type != OBJECT_T && latest_type().type != CLASS_T) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot access property on primitive value " + node->_ident + " of type "
            + latest_type().to_string(), node);
    }

    auto props = latest_type().type == CLASS_T ? static_class_props.equal_range(latest_type().class_name)
                                               : class_props.equal_range(latest_type().class_name);

    std::string symbol = SymbolTable::symbol_from_var(latest_type().type == CLASS_T ? latest_type().class_name :
          decl_ident + "_" + node->_ident, scope);
    node->bind(symbol);

    decl_ident += "_" + node->_ident;
    for (auto it = props.first; it != props.second; ++it) {
        if (it->second.first == node->_ident) {
            if (node->_member_expr == nullptr) {
                return it->second.second;
            }
            else {
                push_type(it->second.second);
                return node->_member_expr->accept(*this);
            }
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
Variant TypeCheckVisitor::visit(BinaryOperator *node) {
    TypeSpecifier fst = node->_first_child->accept(*this).get_type();
    TypeSpecifier snd = node->_second_child->accept(*this).get_type();
    std::string op = node->_operator;

    if (fst.type == OBJECT_T) {
        auto props = class_props.equal_range(fst.class_name);
        std::vector<TypeSpecifier> arg_types{ snd };
        for (auto it = props.first; it != props.second; ++it) {
            if (it->second.first == op &&  fun_call_compatible(it->second.second, arg_types)) {
                node->bind(SymbolTable::symbol_from_fun(fst.class_name + "_" + op, it->second.second.args, scope));
                return *it->second.second.return_type;
            }
        }
    }
    else if (util::is_reversible(op) && snd.type == OBJECT_T) {
        auto props = class_props.equal_range(snd.class_name);
        std::vector<TypeSpecifier> arg_types{ fst };
        for (auto it = props.first; it != props.second; ++it) {
            if (it->second.first == op &&  fun_call_compatible(it->second.second, arg_types)) {
                node->bind(SymbolTable::symbol_from_fun(snd.class_name + "_" + op, it->second.second.args, scope));
                return *it->second.second.return_type;
            }
        }
    }

    if (op == "+=" || op == "-=" || op == "*=") {
        ValueType ret_type = val::simple_arithmetic_return_type(fst, snd);
        if (ret_type == VOID_T) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '" + op + "' values of type " + fst
                    .to_string() + " and " + snd.to_string(), node);
        }

        op = "=";
        snd = TypeSpecifier(ret_type);
    }
    else if (op == "/=") {
        ValueType ret_type = val::division_return_type(fst, snd);
        if (ret_type == VOID_T) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot divide values of type " + fst.to_string() + " and " +
                    snd.to_string(), node);
        }

        op = "=";
        snd = TypeSpecifier(ret_type);
    }

    if (op == "=") {
        if (!val::is_compatible(snd, fst)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot assign value of type " + snd.to_string() + " to variable of "
                    "type " + fst.to_string(), node);
        }

        return { snd };
    }
    else if (op == "+" || op == "-" || op == "*" || op == "**") {
        ValueType ret_type = val::simple_arithmetic_return_type(fst, snd);
        if (ret_type == VOID_T) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '" + op + "' values of type " + fst
                .to_string() + " and " + snd.to_string(), node);
        }

        TypeSpecifier res = TypeSpecifier(ret_type);
        res.static_const = fst.static_const && snd.static_const;

        return { res };
    }
    else if (op == "/") {
        ValueType ret_type = val::division_return_type(fst, snd);
        if (ret_type == VOID_T) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot divide values of type " + fst.to_string() + " and " +
                snd.to_string(), node);
        }

        TypeSpecifier res = TypeSpecifier(ret_type);
        res.static_const = fst.static_const && snd.static_const;

        return { res };
    }
    // only applicable to int and long
    else if (op == "%" || op == "&" || op == "|" || op == "^" || op == "<<" || op == ">>") {
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

        return { res };
    }
    // only applicable to bool
    else if (op == "&&" || op == "||") {
        if (!val::implicitly_castable(fst.type, BOOL_T) && !val::implicitly_castable(snd.type, BOOL_T)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '" + op + "' values of type " + fst
                .to_string() + " and " + snd.to_string(), node);
        }

        TypeSpecifier res = TypeSpecifier(BOOL_T);
        res.static_const = fst.static_const && snd.static_const;

        return { res };
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
        ts.is_array = true;
        ts.static_const = fst.static_const && snd.static_const;

        return ts;
    }
    else if (op == "??") {
        if (fst != snd) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply binary operator '??' to values of different types", node);
        }

        return { fst };
    }
    else if (op == "==" || op == "!=") {
        if (!val::implicitly_castable(fst.type, snd.type)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot compare values of type " + fst
                .to_string() + " and " + snd.to_string() + " for equality", node);
        }

        TypeSpecifier res = TypeSpecifier(BOOL_T);
        res.static_const = fst.static_const && snd.static_const;

        return { res };
    }
    else if (op == "<=" || op == ">=" || op == "<" || op == ">") {
        if (!fst.is_primitive || !snd.is_primitive) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot compare values of type " + fst
                    .to_string() + " and " + snd.to_string(), node);
        }

        TypeSpecifier res = TypeSpecifier(BOOL_T);
        res.static_const = fst.static_const && snd.static_const;

        return { res };
    }

    RuntimeError::raise(ERR_TYPE_ERROR, "Unknown binary operator '" + op + "'", node);

    return {};
}

/**
 * Chechs if an explicit cast is possible
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(ExplicitCastExpr *node) {
    TypeSpecifier target;
    // primitive
    if (util::typemap.find(node->_operator) != util::typemap.end()) {
        target.type = util::typemap[node->_operator];
    }
    else {
        target.type = OBJECT_T;
        target.class_name = node->_operator;
    }

    TypeSpecifier current = node->_child->accept(*this).get_type();
    if (!val::is_castable(current, target)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot cast from " + current.to_string() + " to " + target.to_string());
    }

    return { target };
}

/**
 * Checks if the condition is boolean and the values are of the same type
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(TertiaryOperator *node) {
    TypeSpecifier cond = node->_condition->accept(*this).get_type();

    if (cond.type == OBJECT_T && class_props.find(cond.class_name) != class_props.end()) {
        auto props = class_props.equal_range(cond.class_name);
        std::vector<TypeSpecifier> arg_types{};
        for (auto it = props.first; it != props.second; ++it) {
            if (it->second.first == "toBool" && fun_call_compatible(it->second.second, arg_types)) {
                node->bind(SymbolTable::symbol_from_fun(cond.class_name + "_toBool", it->second.second.args, scope));
                return *it->second.second.return_type;
            }
        }
    }

    if (!val::implicitly_castable(cond.type, BOOL_T)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Condition of tertiary operator '?:' must be boolean or implement "
            "instance method 'toBool() -> bool'", node);
    }

    if (cond.type != BOOL_T) {
        Warning::issue("Implicit cast to boolean", node->_condition.get());
    }

    TypeSpecifier fst = node->_if_branch->accept(*this).get_type();
    TypeSpecifier snd = node->_else_branch->accept(*this).get_type();

    if (!val::is_compatible(fst, snd)) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot apply tertiary operator '?:' to values of different types", node);
    }
    else if (fst != snd) {
        Warning::issue("Implicit cast from " + snd.to_string() + " to " + fst.to_string(), node->_condition.get());
    }

    return { fst };
}

/**
 * Returns the result type of a unary expression depending on the input type
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(UnaryOperator *node) {
    std::string op = node->_operator;

    if (op == "new") {
        IdentifierRefExpr::SharedPtr ref_expr = std::dynamic_pointer_cast<IdentifierRefExpr>(node->_child);
        if (!ref_expr) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Expected class name after 'new'", node->_child.get());
            return {};
        }
        else if (std::find(classes.begin(), classes.end(), ref_expr->_ident) == classes.end()) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Class " + ref_expr->_ident + " does not exist", node->_child.get());
        }

        TypeSpecifier given_type = ref_expr->accept(*this).get_type();
        if (given_type.type != CLASS_T) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Cannot instantiate value of type " + given_type.to_string(),
                ref_expr.get());
        }

        if (decl_ident != "") {
            auto props = class_props.equal_range(ref_expr->_ident);
            for (auto it = props.first; it != props.second; ++it) {
                if (it->second.second.is_method) {
                    continue;
                }
                SymbolTable::insert(decl_ident + "_" + it->second.first, it->second.second, scope);
            }
        }

        TypeSpecifier ts(OBJECT_T);
        ts.class_name = ref_expr->_ident;

        if (node->_member_expr == nullptr) {
            return {ts};
        }
        else {
            push_type(ts);
            return node->_member_expr->accept(*this).get_type();
        }
    }
    else if (op == "typeof") {
        TypeSpecifier res = TypeSpecifier(STRING_T);
        res.static_const = true;

        return { res };
    }

    Variant target = node->_child->accept(*this);

    if (target.get_type().type == OBJECT_T) {
        auto props = class_props.equal_range(target.get_type().class_name);
        std::vector<TypeSpecifier> arg_types{};
        for (auto it = props.first; it != props.second; ++it) {
            if (it->second.first == op && fun_call_compatible(it->second.second, arg_types)) {
                node->bind(SymbolTable::symbol_from_fun(target.get_type().class_name + "_" + op,
                    it->second.second.args, scope));
                return *it->second.second.return_type;
            }
        }
    }

    if (op == "++" || op == "--" || op == "+" || op == "-") {
        if (!target.get_type().is_primitive) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Unary operator " + op + " is not applicable to type " + target
                .get_type().to_string(), node->_child.get());
        }
        if (op == "+") {
            Warning::issue("Unary operator '+' has no effect", node->_child.get());
        }

        return target.get_type();
    }
    else if (op == "~") {
        if (target.get_type() != INT_T && target.get_type() != LONG_T) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Unary operator '~' is only applicable to types int and long",
                node->_child.get());
        }

        return { target.get_type() };
    }
    else if (op == "!") {
        if (!val::implicitly_castable(target.get_type().type, BOOL_T)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Unary operator '!' is not applicable to type " + target.get_type()
                .to_string(), node->_child.get());
        }
        if (target.get_type().type != BOOL_T) {
            Warning::issue("Implicit cast to boolean", node->_child.get());
        }

        TypeSpecifier res = TypeSpecifier(BOOL_T);
        res.static_const = target.get_type().static_const;

        return { res };
    }
    else if (op == "*") {
        return { target.get_type() };
    }
    else if (op == "&") {
        return { TypeSpecifier(REF_T) };
    }

    RuntimeError::raise(ERR_TYPE_ERROR, "Unknown unary operator '" + op + "'", node);

    return {};
}

/**
 * Checks if a break statement is valid
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(BreakStmt *node) {
    if (!breakable) {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "Keyword 'break' is only allowed in switch, for and while statements",
            node);
    }

    return {};
}

/**
 * Checks if a continue statement is valid
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(ContinueStmt *node) {
    if (!continuable) {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "Keyword 'continue' is only allowed in for and while statements",
            node);
    }

    return {};
}

/**
 * Checks if the condition is boolean (or convertible), and if all branches return
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(IfStmt *node) {
    if (check_return_type) {
        if (node->_else_branch) {
            TypeCheckVisitor if_visitor(this);
            if_visitor.check_return_type = true;
            if_visitor.returnable = true;
            if_visitor.declared_return_type = declared_return_type;

            node->_if_branch->accept(if_visitor);

            TypeCheckVisitor else_visitor(this);
            else_visitor.check_return_type = true;
            else_visitor.returnable = true;
            else_visitor.declared_return_type = declared_return_type;

            node->_else_branch->accept(else_visitor);

            // all branches return
            if (if_visitor.branches - if_visitor.returned <= 0 && declared_return_type.type != VOID_T
                    && else_visitor.branches - else_visitor.returned <= 0) {
                returned++;
            }
        }
    }
    else {
        for (auto child : node->get_children()) {
            child->accept(*this);
        }
    }

    return {};
}

/**
 * Checks if the switch and case types are compatible, and if all branches return
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(SwitchStmt *node) {
    TypeSpecifier switch_type = node->switch_val->accept(*this).get_type();

    if (check_return_type) {
        TypeCheckVisitor case_visitor(this);
        case_visitor.returnable = true;
        case_visitor.breakable = true;
        case_visitor.declared_return_type = declared_return_type;
        case_visitor.check_return_type = true;
        bool all_branches_return = true;
        bool default_exists = false;
        //TODO check if exhausted

        for (auto case_ : node->cases) {
            TypeSpecifier case_type = case_->case_val->accept(*this).get_type();
            if (!val::is_compatible(switch_type, case_type)) {
                RuntimeError::raise(ERR_TYPE_ERROR, "Incompatible types in switch statement: " + switch_type
                    .to_string() + " and " + case_type.to_string(), case_.get());
            }
            else if (case_type != switch_type) {
                Warning::issue("Implicit cast from " + case_type.to_string() + " to " + switch_type.to_string(),
                    case_.get());
            }

            case_->accept(case_visitor).get_type();
            if (case_->is_default) {
                default_exists = true;
            }
            if (case_visitor.branches - case_visitor.returned > 0 && declared_return_type.type != VOID_T) {
                all_branches_return = false;
            }
            case_visitor.reset();
        }

        if (all_branches_return && default_exists) {
            returned++;
        }
    }
    else {
        TypeCheckVisitor case_visitor(this);
        case_visitor.breakable = true;
        for (auto case_ : node->cases) {
            TypeSpecifier case_type = case_->case_val->accept(*this).get_type();
            if (!val::is_compatible(switch_type, case_type)) {
                RuntimeError::raise(ERR_TYPE_ERROR, "Incompatible types in switch statement: " + switch_type
                    .to_string() + " and " + case_type.to_string(), case_.get());
            }
            else if (case_type != switch_type) {
                Warning::issue("Implicit cast from " + case_type.to_string() + " to " + switch_type.to_string(),
                    case_.get());
            }

            case_->accept(case_visitor).get_type();
        }
    }

    return {};
}

/**
 * Iterates over all children
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(CaseStmt *node) {
    for (auto child : node->get_children()) {
        child->accept(*this);
    }

    return {};
}

/**
 * Does nothing
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(LabelStmt *node) {
    return {};
}

/**
 * Does nothing
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(GotoStmt *node) {
    return {};
}

/**
 * Checks a function argument declaration for type consistency of default value and existence of declared type
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(FuncArgDecl *node) {
    TypeSpecifier ts = node->get_type();
    if (ts.type == OBJECT_T) {
        if (std::find(classes.begin(), classes.end(), ts.class_name) == classes.end()) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Class " + ts.class_name + " does not exist", node);
        }
    }

    if (node->_default_val) {
        TypeSpecifier def_type = node->_default_val->accept(*this).get_type();
        if (!val::is_compatible(def_type, node->_arg_type)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Default value for parameter " + node->_arg_name + " must be of type "
                "" + node->_arg_type.to_string(), node->_default_val.get());
        }
        else if (def_type != node->_arg_type) {
            Warning::issue("Implicit cast from " + def_type.to_string() + " to " + node->_arg_type.to_string(),
                    node->_default_val.get());
        }

        ts.nullable = true;
    }

    return { ts };
}

/**
 * Returns the current context
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(ReturnStmt *node) {
    if (node->_return_val) {
        return_(node->_return_val->accept(*this).get_type(), node);
    }
    else {
        return_(TypeSpecifier(VOID_T), node);
    }

    return {};
}

/**
 * Does nothing
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(InputStmt *node) {
    for (auto child : node->get_children()) {
        child->accept(*this);
    }

    return {};
}

/**
 * Does nothing
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(OutputStmt *node) {
    for (auto child : node->get_children()) {
        child->accept(*this);
    }

    return {};
}

/**
 * Does nothing
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(Expression *node) {
    if (node->_child != nullptr) {
        return {node->_child->accept(*this).get_type()};
    }

    return {};
}

/**
 * Returns the return type of a method call, and checks if the method exists
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(MethodCallExpr *node) {
    if (latest_type().type != OBJECT_T && latest_type().type != CLASS_T) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot call method on primitive type "+ latest_type().to_string(), node);
    }

    std::vector<TypeSpecifier> arg_types;
    for (auto arg : node->_arguments) {
        arg_types.push_back(arg->accept(*this).get_type());
    }

    bool static_ = latest_type().type == CLASS_T;
    auto props = static_ ? static_class_props.equal_range(latest_type().class_name)
                                               : class_props.equal_range(latest_type().class_name);

    for (auto it = props.first; it != props.second; ++it) {
        if (it->second.first == node->_ident) {
            node->bind(SymbolTable::symbol_from_fun(latest_type().class_name + "_" + node->_ident + (static_ ? "!st"
                    : ""), it->second.second.args, scope));
            if (node->_member_expr == nullptr) {
                return *it->second.second.return_type;
            }
            else {
                push_type(*it->second.second.return_type);
                return node->_member_expr->accept(*this);
            }
        }
    }

    RuntimeError::raise(ERR_TYPE_ERROR, "Method " + node->_ident + " does not exist on class " +
        latest_type().class_name);
}

/**
 * Declares a new class
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(ClassDecl *node) {
    classes.push_back(node->class_name);

    TypeSpecifier t(CLASS_T);
    t.class_name = node->class_name;
    push_type(t);

    SymbolTable::declare_class(node->class_name);

    if (node->constr) {
        node->constr->accept(*this);
    }

    for (auto field : node->fields) {
        field->accept(*this);
    }

    for (auto method : node->methods) {
        method->accept(*this);
    }

    for (auto op : node->unary_operators) {
        op.second->accept(*this);
    }

    for (auto op : node->binary_operators) {
        op.second->accept(*this);
    }

    SymbolTable::insert(node->class_name, t, scope);

    return {};
}

/**
 * Declares a class constructor
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(ConstrDecl *node) {
    TypeCheckVisitor t(this);
    // constructor is not returnable
    node->body->accept(t);

    TypeSpecifier* ts = new TypeSpecifier(OBJECT_T);
    ts->class_name = latest_type().class_name;

    SymbolTable::insert(latest_type().class_name, ts, std::vector<TypeSpecifier>(), scope);
    SymbolTable::get_class(latest_type().class_name)->add_method("construct", ts, node->am, false, {}, {}, {});

    return {};
}

/**
 * Declares a class field and checks for correct type
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(FieldDecl *node) {
    auto props = (node->is_static) ? static_class_props.equal_range(latest_type().class_name)
                                   : class_props.equal_range(latest_type().class_name);
    for (auto it = props.first; it != props.second; ++it) {
        if (it->second.first == node->field_name) {
            RuntimeError::raise(ERR_REDECLARED_VAR, "Field " + node->field_name + " is declared twice", node);
        }

    }

    if (node->default_val != nullptr) {
        TypeSpecifier def_type = node->default_val->accept(*this).get_type();
        if (!val::is_compatible(def_type, node->type)) {
            RuntimeError::raise(ERR_TYPE_ERROR, "Default value for field " + node->field_name + " must be of type " +
                node->type.to_string(), node->default_val.get());
        }
        else if (def_type != node->type) {
            Warning::issue("Implicit cast from " + def_type.to_string() + " to " + node->type.to_string(),
                node->default_val.get());
        }
    }

    if (node->is_static) {
        SymbolTable::insert(latest_type().class_name + "_" + node->field_name + "!st", node->type, scope);
        static_class_props.emplace(latest_type().class_name,
            std::pair<std::string, TypeSpecifier>(node->field_name, node->type));
    }
    else {
        class_props.emplace(latest_type().class_name,
            std::pair<std::string, TypeSpecifier>(node->field_name, node->type));
    }

    SymbolTable::get_class(latest_type().class_name)->add_field(node->field_name, node->type, node->am,
        node->is_static);

    return {};
}

/**
 * Declares a method and checks if all code paths return
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(MethodDecl *node) {
    std::vector<TypeSpecifier> arg_types;
    std::vector<std::string> arg_names;
    std::vector<Expression::SharedPtr> arg_defaults;
    for (auto arg : node->args) {
        arg_types.push_back(arg->accept(*this).get_type());
        arg_names.push_back(arg->_arg_name);
        arg_defaults.push_back(arg->_default_val);
    }

    auto overloads = (node->is_static) ? static_class_props.equal_range(node->method_name)
                                       : class_props.equal_range(node->method_name);
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
                RuntimeError::raise(ERR_TYPE_ERROR, "Method " + node->method_name + " cannot be redeclared with a "
                    "similar signature to a previous declaration", node);
            }
        }
    }

    TypeCheckVisitor method_visitor(this);
    method_visitor.returnable = true;
    method_visitor.check_return_type = true;
    method_visitor.declared_return_type = node->return_type;

    node->body->accept(method_visitor);

    TypeSpecifier method_type(OBJECT_T);
    method_type.is_function = true;
    method_type.args = arg_types;
    method_type.return_type = new TypeSpecifier(node->return_type);
    method_type.is_method = true;

    if (node->is_static) {
        SymbolTable::insert(latest_type().class_name + "_" + node->method_name + "!st",
            new TypeSpecifier(node->return_type), arg_types, scope);
        static_class_props.emplace(latest_type().class_name,
            std::pair<std::string, TypeSpecifier>(node->method_name, method_type));
    }
    else {
        SymbolTable::insert(latest_type().class_name + "_" + node->method_name, new TypeSpecifier(node->return_type),
            arg_types, scope);
        class_props.emplace(latest_type().class_name,
            std::pair<std::string, TypeSpecifier>(node->method_name, method_type));
    }

    SymbolTable::get_class(latest_type().class_name)->add_method(node->method_name, node->return_type, node->am,
        node->is_static, arg_names, arg_types, arg_defaults);

    return {};
}

/**
 * Declares an operator
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(OperatorDecl *node) {
    std::vector<TypeSpecifier> arg_types;
    std::vector<std::string> arg_names;
    std::vector<Expression::SharedPtr> arg_defaults;
    for (auto arg : node->args) {
        arg_types.push_back(arg->accept(*this).get_type());
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

    TypeCheckVisitor method_visitor(this);
    method_visitor.returnable = true;
    method_visitor.check_return_type = true;
    method_visitor.declared_return_type = node->return_type;

    node->body->accept(method_visitor);

    TypeSpecifier method_type(OBJECT_T);
    method_type.is_function = true;
    method_type.args = arg_types;
    method_type.return_type = new TypeSpecifier(node->return_type);
    method_type.is_method = true;

    SymbolTable::insert(latest_type().class_name + "_" + node->_operator, new TypeSpecifier(node->return_type),
            arg_types, scope);
    class_props.emplace(latest_type().class_name, std::pair<std::string, TypeSpecifier>(node->_operator, method_type));

    SymbolTable::get_class(latest_type().class_name)->add_method(node->_operator, node->return_type, node->am,
        false, arg_names, arg_types, arg_defaults);
    
    return {};
}

/**
 * Checks if all code paths return the correct type
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(LambdaExpr *node) {
    std::vector<TypeSpecifier> arg_types;
    for (auto arg : node->_args) {
        arg_types.push_back(arg->accept(*this).get_type());
    }

    TypeCheckVisitor t(this);
    t.check_return_type = true;
    t.declared_return_type = node->_return_type;
    t.returnable = true;

    node->_body->accept(t);

    if (t.branches - t.returned > 0 && t.declared_return_type.type != VOID_T && !t.declared_return_type.nullable) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Not all code paths return a value", node);
    }

    TypeSpecifier ts(OBJECT_T);
    ts.is_function = true;
    ts.args = arg_types;
    ts.return_type = &node->_return_type;

    if (node->_member_expr == nullptr) {
        return ts;
    }
    else {
        push_type(ts);
        return node->_member_expr->accept(*this).get_type();
    }
}

/**
 * Does nothing
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(ImportStmt *node) {
    for (auto child : node->get_children()) {
        child->accept(*this);
    }

    return {};
}

/**
 * Does nothing
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(ExportStmt *node) {
    for (auto child : node->get_children()) {
        child->accept(*this);
    }

    return {};
}

/**
 * Checks a struct declaration
 * @param node
 * @return
 */
Variant TypeCheckVisitor::visit(StructDecl *node) {
    classes.push_back(node->class_name);

    TypeSpecifier t(CLASS_T);
    t.class_name = node->class_name;
    push_type(t);

    if (node->constr) {
        node->constr->accept(*this);
    }

    for (auto field : node->fields) {
        field->accept(*this);
    }

    for (auto method : node->methods) {
        method->accept(*this);
    }

    for (auto op : node->unary_operators) {
        op.second->accept(*this);
    }

    for (auto op : node->binary_operators) {
        op.second->accept(*this);
    }

    SymbolTable::insert(node->class_name, t, scope);

    return {};
}