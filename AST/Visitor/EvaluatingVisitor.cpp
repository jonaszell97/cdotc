//
// Created by Jonas Zell on 26.06.17.
//

#include "EvaluatingVisitor.h"
#include <iostream>
#include "Visitor.h"
#include "../AstNode.h"
#include "../Statement/CompoundStmt.h"
#include "../Expression/RefExpr/IdentifierRefExpr.h"
#include "../Expression/RefExpr/MethodCallExpr.h"
#include "../Expression/RefExpr/MemberRefExpr.h"
#include "../Statement/DeclStmt.h"
#include "../Statement/ControlFlow/ForStmt.h"
#include "../Statement/ControlFlow/WhileStmt.h"
#include "../Statement/Function/FunctionDecl.h"
#include "../Expression/Expression.h"
#include "../Expression/Literal/LiteralExpr.h"
#include "../Expression/Literal/ObjectLiteral.h"
#include "../Expression/RefExpr/ArrayAccessExpr.h"
#include "../Expression/RefExpr/CallExpr.h"
#include "../Operator/BinaryOperator.h"
#include "../Operator/UnaryOperator.h"
#include "../Operator/ExplicitCastExpr.h"
#include "../Operator/TertiaryOperator.h"
#include "../Statement/ControlFlow/ContinueStmt.h"
#include "../Statement/ControlFlow/BreakStmt.h"
#include "../Statement/ControlFlow/IfStmt.h"
#include "../Statement/IO/OutputStmt.h"
#include "../Statement/Function/ReturnStmt.h"
#include "../Statement/IO/InputStmt.h"
#include "../Expression/Literal/ArrayLiteral.h"
#include "../../StdLib/Objects/Array.h"
#include "../../StdLib/Objects/Object.h"
#include "../../StdLib/Objects/Function.h"
#include "../../Util.h"

EvaluatingVisitor::EvaluatingVisitor() {

}

EvaluatingVisitor::EvaluatingVisitor(const EvaluatingVisitor &v) : EvaluatingVisitor() {

}

/**
 * Evaluates a compound statement by evaluating all of its enclosed statements, until a ReturnStmt is reached
 * @param node
 * @return Variant
 */
Variant EvaluatingVisitor::visit(CompoundStmt *node) {
    for (auto stmt : node->_statements) {
        stmt->accept(*this);
        if (current_context_continuable && loop_continued) {
            break;
        }
    }

    return_reached = false;

    return return_val;
}

/**
 * Evaluates a function declaration and makes it available in the current context
 * @param node
 * @return
 */
Variant EvaluatingVisitor::visit(FunctionDecl *node) {
    Context::SharedPtr ctx = node->context;

    if (ctx == nullptr) {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "Cannot create function in current context");
    }

    Function::SharedPtr func = std::make_shared<Function>(node->_func_name, node->_return_type);
    func->set_body(node->_body);
    func->set_context(ctx);

    for (auto arg : node->_args) {
        arg->accept(*this);
        func->add_argument(current_arg.name, current_arg.type, current_arg.default_val);
        ctx->declare_variable(current_arg.name);
    }

    ctx->set_variable(node->_func_name, Variant(func));

    return nullptr;
}

/**
 * Evaluates an identifier reference by returning the associated variable
 * @param node
 * @return
 */
Variant EvaluatingVisitor::visit(IdentifierRefExpr *node) {
    Context::SharedPtr ctx = node->context;

    if (ctx != nullptr) {
        Variant var;
        if (GlobalContext::is_declared_class(node->_ident)) {
            var = { GlobalContext::get_class(node->_ident) };
        }
        else {
            Variant::SharedPtr ref = ctx->get_variable(node->_ident);
            var = node->_return_ref ? ref : *ref;
        }

        if (node->_member_expr == nullptr) {
            return var;
        } else {
            node->_member_expr->return_ref(node->_return_ref);
            current_obj_ref = var;
            return node->_member_expr->accept(*this);
        }
    }
    else {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "No context to get variable " + node->_ident + " from.");
    }

    return nullptr;
}

/**
 * Evaluates a variable declaration in the current context
 * @param node
 * @return
 */
Variant EvaluatingVisitor::visit(DeclStmt *node) {
    Context::SharedPtr ctx = node->context;
    if (ctx != nullptr) {

        Variant res = node->_val != nullptr ? node->_val->accept(*this) : Variant();
        res.is_nullable(node->_type.nullable);

        // use float as default when type is not specified
        if (res.get_type() == DOUBLE_T && node->_type.type != DOUBLE_T) {
            res.cast_to(FLOAT_T);
        }

        // type inference
        if (node->_type.type == AUTO_T) {
            node->_type.type = res.get_type();
        }
        else if (node->_type.type == ANY_T) {
            res.is_any_type();
        }

        if (!val::is_compatible(node->_type.type, res.get_type())) {
            RuntimeError::raise(ERR_BAD_CAST, "Trying to assign value of type " + val::typetostr(res.get_type())
                + " to variable of type " + val::typetostr(node->_type.type));
        }

        ctx->set_variable(node->_ident, res);
    }
    else {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "No context to create variable " + node->_ident + " in");
    }

    return nullptr;
}

/**
 * Evaluates a for loop
 * @param node 
 * @return 
 */
Variant EvaluatingVisitor::visit(ForStmt *node) {
    node->_initialization->accept(*this);

    if (node->_body == nullptr) {
        return {};
    }

    EvaluatingVisitor loop_evaluator;
    loop_evaluator.current_context_breakable = true;
    loop_evaluator.current_context_continuable = true;

    while (!loop_evaluator.loop_broken && node->_termination->accept(loop_evaluator).get<bool>()) {
        node->_body->accept(loop_evaluator);
        node->_increment->accept(loop_evaluator);
        loop_evaluator.loop_continued = false;
    }

    return nullptr;
}

/**
 * Evaluates a while loop
 * @param node
 * @return
 */
Variant EvaluatingVisitor::visit(WhileStmt *node) {
    EvaluatingVisitor loop_evaluator;
    loop_evaluator.current_context_breakable = true;
    loop_evaluator.current_context_continuable = true;

    while (!loop_evaluator.loop_broken && node->_condition->accept(loop_evaluator).get<bool>()) {
        node->_while_block->accept(loop_evaluator);
        loop_evaluator.loop_continued = false;
    }

    return nullptr;
}

/**
 * Creates an Array from an ArrayLiteral node
 * @param node
 * @return
 */
Variant EvaluatingVisitor::visit(ArrayLiteral *node) {
    int length = node->_length;

    if (node->_length_expr != nullptr) {
        int _length = node->_length_expr->accept(*this).get<int>();
        if (_length > INT32_MAX) {
            RuntimeError::raise(ERR_VAL_TOO_LARGE, "An array can hold a maxium of " + std::to_string(INT32_MAX)
                + " values, tried to allocate " + std::to_string(_length));
        }

       length = _length;
    }

    std::shared_ptr<Array> arr = std::make_shared<Array>(node->_type, length);
    for (auto el : node->_elements) {
        auto res = el->accept(*this);

        if (res.get_type() == DOUBLE_T && node->_type != DOUBLE_T) {
            res.cast_to(FLOAT_T);
        }

        arr->push(res);
    }

    return { arr };
}

/**
 * Evaluates a literal expression
 * @param node
 * @return
 */
Variant EvaluatingVisitor::visit(LiteralExpr *node) {
    return node->_value;
}

/**
 * Creates a generic object from an ObjectLiteral expression
 * @param node
 * @return
 */
Variant EvaluatingVisitor::visit(ObjectLiteral *node) {
    Object::SharedPtr obj = std::make_shared<Object>();

    for (auto prop : node->_props) {
        prop->accept(*this);
        obj->set_property(current_prop.name, current_prop.value);
    }

    current_prop = {};

    return { obj };
}

/**
 * Evaluates an object property expression
 * @param node
 * @return
 */
Variant EvaluatingVisitor::visit(ObjectPropExpr *node) {
    ObjectProp op;
    op.name = node->_prop_name;
    op.value = std::make_shared<Variant>(node->_prop_val->accept(*this));
    if (node->_prop_type == ANY_T) {
        op.value->is_any_type();
    }

    current_prop = op;
    return nullptr;
}

/**
 * Evaluates an array access expression
 * @param node
 * @return
 */
Variant EvaluatingVisitor::visit(ArrayAccessExpr *node) {
    Array::SharedPtr arr = std::dynamic_pointer_cast<Array>(current_obj_ref.get<Object::SharedPtr>());
    if (arr == nullptr) {
        RuntimeError::raise(ERR_TYPE_ERROR, "Cannot access index of non-array element");
    }

    Variant v = arr->at(node->_index->accept(*this).get<int>());
    current_obj_ref = v;

    if (node->_member_expr != nullptr) {
        node->_member_expr->return_ref(node->_return_ref);
        return node->_member_expr->accept(*this);
    }
    else {
        return (node->_return_ref) ? v : *v;
    }
}

/**
 * Evaluate a function call on an object
 * @param node
 * @return
 */
Variant EvaluatingVisitor::visit(CallExpr *node) {
    Function::SharedPtr fun = std::dynamic_pointer_cast<Function>(current_obj_ref.get<Object::SharedPtr>());
    if (fun == nullptr) {
        RuntimeError::raise(ERR_BAD_ACCESS, "Cannot call value of type " + val::typetostr(current_obj_ref.get_type()));
    }

    std::vector<Variant> _real_args;
    for (auto arg : node->_arguments) {
        auto arg_val = arg->accept(*this);
        _real_args.push_back(arg_val);
    }

    Variant res = fun->call(_real_args);

    if (node->_member_expr != nullptr) {
        node->_member_expr->return_ref(node->_return_ref);
        current_obj_ref = res;
        return node->_member_expr->accept(*this);
    }
    else {
        return node->_return_ref ? res : *res;
    }
}

/**
 * Evaluates a method call expression
 * @param node
 * @return
 */
Variant EvaluatingVisitor::visit(MethodCallExpr *node) {
    Variant v;
    std::vector<Variant> _real_args;
    for (auto arg : node->_arguments) {
        auto arg_val = arg->accept(*this);
        _real_args.push_back(arg_val);
    }

    if (current_obj_ref.get_type() == OBJECT_T) {
        v = current_obj_ref.get<Object::SharedPtr>()->call_method(node->_ident, _real_args);
    }
    else if (current_obj_ref.get_type() == CLASS_T) {
        v = current_obj_ref.get<Class*>()->call_static_method(node->_ident, _real_args);
    }
    else if (val::base_class(current_obj_ref.get_type()) != "") {
        Class* cl = GlobalContext::get_class(val::base_class(current_obj_ref.get_type()));
        Object::SharedPtr class_instance = cl->instantiate({current_obj_ref});
        v = class_instance->call_method(node->_ident, _real_args);
    }
    else {
        RuntimeError::raise(ERR_BAD_ACCESS, "Cannot call method on primitve value.");
    }

    if (node->_member_expr == nullptr) {
        return node->_return_ref ? v : *v;
    }
    else {
        node->_member_expr->return_ref(node->_return_ref);
        current_obj_ref = v;
        return node->_member_expr->accept(*this);
    }
}

/**
 * Evaluate a member access expression on an object
 * @param node
 * @return
 */
Variant EvaluatingVisitor::visit(MemberRefExpr *node) {
    Variant v;

    // object property access
    if (current_obj_ref.get_type() == OBJECT_T) {
        auto obj = current_obj_ref.get<Object::SharedPtr>();
        v = obj->access_property(node->_ident);
    }
    // static method call
    else if (current_obj_ref.get_type() == CLASS_T) {
        auto cl = current_obj_ref.get<Class*>();
        v = cl->access_static_property(node->_ident);
    }
    // autoboxing method call
    else if (val::base_class(current_obj_ref.get_type()) != "") {
        v = GlobalContext::get_class(val::base_class(current_obj_ref.get_type()))
                ->access_static_property(node->_ident);
    }
    else {
        RuntimeError::raise(ERR_BAD_ACCESS, "Cannot access property on given value.");
    }

    if (node->_member_expr == nullptr) {
        return node->_return_ref ? v : *v;
    }
    else {
        node->_member_expr->return_ref(node->_return_ref);
        current_obj_ref = v;
        return node->_member_expr->accept(*this);
    }
}

/**
 * Evaluates an explicit type cast
 * @param node
 * @return
 */
Variant EvaluatingVisitor::visit(ExplicitCastExpr *node) {
    return node->_child->accept(*this).cast_to(util::typemap[node->_operator]);
}

/**
 * Evaluates a break statement in a for, switch or while loop
 * @param node
 * @return
 */
Variant EvaluatingVisitor::visit(BreakStmt *node) {
    if (!current_context_breakable) {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "Keyword 'break' is only allowed in for, while and switch statements");
    }

    loop_broken = true;
    loop_continued = true;

    return {};
}

/**
 * Evaluates a continue statement in a for or while loop
 * @param node
 * @return
 */
Variant EvaluatingVisitor::visit(ContinueStmt *node) {
    if (!current_context_continuable) {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "Keyword 'continue' is only allowed in for and while statements");
    }

    loop_continued = true;

    return {};
}

Variant EvaluatingVisitor::visit(IfStmt *node) {
    bool cond = node->_condition->accept(*this).get<bool>();
    if (cond && node->_if_branch != nullptr) {
        return node->_if_branch->accept(*this);
    }
    else if (!cond && node->_else_branch != nullptr) {
        return node->_else_branch->accept(*this);
    }

    return {};
}

/**
 * Evaluates a function argument declaration expression
 * @param node
 * @return
 */
Variant EvaluatingVisitor::visit(FuncArgDecl *node) {
    FuncArg fa;
    fa.type = node->_arg_type;
    fa.name = node->_arg_name;
    fa.default_val = node->_default_val == nullptr ? Variant() : node->_default_val->accept(*this);

    current_arg = fa;

    return {};
}

/**
 * Evaluates a return statement
 * @param node
 * @return
 */
Variant EvaluatingVisitor::visit(ReturnStmt *node) {
    if (!current_context_returnable) {
        RuntimeError::raise(ERR_CONTEXT_ERROR, "Cannot return from global context");
    }

    return_reached = true;
    return_val = node->_return_val->accept(*this);

    return {};
}

/**
 * Evaluates an input statement
 * @param node
 * @return
 */
Variant EvaluatingVisitor::visit(InputStmt *node) {
    std::string s;
    std::cin >> s;

    DeclStmt decl(node->_ident, std::make_shared<LiteralExpr>(Variant(s).cast_to(node->_type)), TypeSpecifier());
    decl.accept(*this);

    return { };
}

/**
 * Evaluates an output statement
 * @param node
 * @return
 */
Variant EvaluatingVisitor::visit(OutputStmt *node) {
    std::cout << node->_value->accept(*this).to_string(true) << std::endl;

    return { };
}

/**
 * Evaluates a generic expression
 * @param node
 * @return
 */
Variant EvaluatingVisitor::visit(Expression *node) {
    return node->_child->accept(*this);
}

/**
 * Evaluates a unary operator expression
 * @param node 
 * @return 
 */
Variant EvaluatingVisitor::visit(UnaryOperator *node) {
    std::string _operator = node->_operator;
    if (_operator == "typeof") {
        return val::type_name(node->_child->accept(*this));
    }
    else if (_operator == "*") {
        return *node->_child->accept(*this);
    }
    else if (_operator == "&") {
        IdentifierRefExpr::SharedPtr child = std::static_pointer_cast<IdentifierRefExpr>(node->_child);
        child->return_ref(true);

        return child->accept(*this);

    }
    else if (_operator == "+") {
        return node->_child->accept(*this);
    }
    else if (_operator == "-") {
        return -(node->_child->accept(*this));
    }
    else if (_operator == "!") {
        return !(node->_child->accept(*this));
    }
    else if (_operator == "++" || _operator == "--") {
        Variant fst;
        IdentifierRefExpr::SharedPtr ref = std::dynamic_pointer_cast<IdentifierRefExpr>(node->_child);
        if (ref != nullptr) {
            ref->return_ref(true);
            fst = ref->accept(*this);
        }
        else {
            fst = node->_child->accept(*this);
            if (!fst.is_ref()) {
                RuntimeError::raise(ERR_OP_UNDEFINED,
                    "Cannot apply increment operator to value that is not a reference");
            }
        }

        if (node->prefix) {
            fst.strict_equals(*fst + Variant(_operator == "++" ? 1 : -1));

            return *fst;
        }
        else {
            Variant return_val(*fst);
            fst.strict_equals(*fst + Variant(_operator == "++" ? 1 : -1));

            return return_val;
        }
    }
    else if (_operator == "~") {
        return ~node->_child->accept(*this);
    }

    RuntimeError::raise(ERR_OP_UNDEFINED, "No definition found for unary operator " + _operator + " on type "
                                          + val::typetostr(node->_child->accept(*this).get_type()));
}

/**
 * Evaluates a binary operator epxression
 * @param node 
 * @return 
 */
Variant EvaluatingVisitor::visit(BinaryOperator *node) {
    std::string _operator = node->_operator;
    if (!(util::in_vector(util::binary_operators, _operator))) {
        RuntimeError::raise(ERR_OP_UNDEFINED, "Undefined binary _operator " + _operator);
    }

    if (_operator == "=") {
        auto fst = node->_first_child->accept(*this);
        auto snd = node->_second_child->accept(*this);

        fst.strict_equals(snd);

        return fst;
    }
    else if (_operator == "+=") {
        auto fst = node->_first_child->accept(*this);
        auto snd = node->_second_child->accept(*this);

        fst.strict_equals(fst + snd);

        return fst;
    }
    else if (_operator == "-=") {
        auto fst = node->_first_child->accept(*this);
        auto snd = node->_second_child->accept(*this);

        fst.strict_equals(fst - snd);

        return fst;
    }
    else if (_operator == "*=") {
        auto fst = node->_first_child->accept(*this);
        auto snd = node->_second_child->accept(*this);

        fst.strict_equals(fst * snd);

        return fst;
    }
    else if (_operator == "/=") {
        auto fst = node->_first_child->accept(*this);
        auto snd = node->_second_child->accept(*this);

        fst.strict_equals(fst - snd);

        return fst;
    }
    // lazy evaluation
    else if (_operator == "&&") {
        auto fst = node->_first_child->accept(*this);
        if (!fst.get<bool>()) {
            return false;
        }

        return node->_second_child->accept(*this);
    }
    else if (_operator == "||") {
        auto fst = node->_first_child->accept(*this);
        if (fst.get<bool>()) {
            return true;
        }

        return node->_second_child->accept(*this);
    }

    auto fst = node->_first_child->accept(*this);
    auto snd = node->_second_child->accept(*this);

    if (_operator == "==") {
        return fst == snd;
    }
    else if (_operator == "!=") {
        return fst != snd;
    }
    else if (_operator == "<=") {
        return fst <= snd;
    }
    else if (_operator == ">=") {
        return fst >= snd;
    }
    else if (_operator == ">>") {
        return fst >> snd;
    }
    else if (_operator == "<<") {
        return fst << snd;
    }
    else if (_operator == "**") {
        return fst.pow(snd);
    }
    else switch (_operator[0]) {
            case '+': {
                return fst + snd;
            }
            case '-': {
                return fst - snd;
            }
            case '*': {
                return fst * snd;
            }
            case '/': {
                return fst / snd;
            }
            case '%': {
                return fst % snd;
            }
            case '<': {
                return fst < snd;
            }
            case '>': {
                return fst > snd;
            }
            case '&': {
                return fst & snd;
            }
            case '|': {
                return fst | snd;
            }
            case '^': {
                return fst ^ snd;
            }
            default:
                RuntimeError::raise(ERR_OP_UNDEFINED, "Undefined binary _operator " + _operator);
        }
}

/**
 * Evaluates a tertiary operator expression (aka ?:)
 * @param node
 * @return
 */
Variant EvaluatingVisitor::visit(TertiaryOperator *node) {
    if (node->_condition->accept(*this).get<bool>()) {
        return node->_if_branch->accept(*this);
    }
    else {
        return node->_else_branch->accept(*this);
    }
}