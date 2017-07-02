//
// Created by Jonas Zell on 13.06.17.
//

#include <string>
#include "Parser.h"
#include "Variant/Variant.h"
#include <vector>
#include "Util.h"
#include <iostream>
#include "Exceptions.h"
#include "Debug.h"
#include "StdLib/Objects/Object.h"
#include "AST/Expression/RefExpr/IdentifierRefExpr.h"
#include "AST/Operator/UnaryOperator.h"
#include "AST/Expression/Literal/LiteralExpr.h"
#include "AST/Expression/Literal/StringLiteral.h"
#include "AST/Operator/BinaryOperator.h"
#include "AST/Statement/Declaration/DeclStmt.h"
#include "AST/Statement/IO/InputStmt.h"
#include "AST/Statement/IO/OutputStmt.h"
#include "AST/Expression/Literal/ObjectLiteral.h"
#include "AST/Statement/CompoundStmt.h"
#include "AST/Statement/Declaration/FunctionDecl.h"
#include "AST/Expression/RefExpr/CallExpr.h"
#include "AST/Expression/RefExpr/MemberRefExpr.h"
#include "AST/Expression/RefExpr/ArrayAccessExpr.h"
#include "AST/Statement/ControlFlow/ReturnStmt.h"
#include "AST/Operator/ExplicitCastExpr.h"
#include "AST/Visitor/Visitor.h"
#include "AST/Statement/ControlFlow/ContinueStmt.h"
#include "AST/Statement/ControlFlow/BreakStmt.h"
#include "AST/Expression/RefExpr/MethodCallExpr.h"
#include "StdLib/GlobalContext.h"
#include "AST/Visitor/CaptureVisitor.h"
#include "AST/Visitor/EvaluatingVisitor.h"
#include "AST/Statement/ControlFlow/GotoStmt.h"
#include "AST/Expression/Literal/LambdaExpr.h"

/**
 * Creates a new interpreter for an Xtreme Jonas Script program.
 * @param program
 */
Parser::Parser(std::string program) :
    tokenizer(new Tokenizer(program)),
    prog_root(std::make_shared<CompoundStmt>())
{
    prog_root->returnable(false);
    GlobalContext::init(program);
}

/**
 * Throws an unexpected token error
 */
void Parser::token_error() {
    ParseError::raise(ERR_UNEXPECTED_TOKEN, "Unexpected Token " + util::token_names[tokenizer->current_token.get_type()]
            + " on line " + std::to_string(tokenizer->current_line) + ":" + std::to_string(tokenizer->index_on_line),
            tokenizer);
}

/**
 * Throws an unexpected token error
 * @param expected The expected TokenType
 * @param found The found TokenType
 */
void Parser::token_error(TokenType expected, TokenType found) {
    ParseError::raise(ERR_UNEXPECTED_TOKEN, "Unexpected Token: Expected " + util::token_names[expected]
            + ", found: " + util::token_names[found] + " on line " + std::to_string(tokenizer->current_line) + ":"
            + std::to_string(tokenizer->index_on_line), tokenizer);
}


/**
 * Parses a type specifier, like "int", "bool[3]", "any[18 * x]"
 * @return
 */
TypeSpecifier Parser::parse_type() {
    TypeSpecifier ts;
    std::string type = tokenizer->current_token.get_value().get<std::string>();
    if (util::typemap.find(type) != util::typemap.end()) {
        ts.type = util::typemap[type];
        ts.is_primitive = true;
    }
    else {
        ts.class_name = type;
        ts.is_primitive = false;
    }

    // check for array type
    Token _next = tokenizer->lookahead();
    if (_next.is_punctuator(C_OPEN_SQUARE)) {
        tokenizer->advance();
        _next = tokenizer->lookahead();
        if (!_next.is_punctuator(C_CLOSE_SQUARE)) {
            ts.length = parse_expression();
            tokenizer->advance();
            if (!tokenizer->current_token.is_punctuator(C_CLOSE_SQUARE)) {
                token_error();
            }
        }
        else {
            ts.is_var_length = true;
            tokenizer->advance();
        }

        ts.is_array = true;
    }

    // nullable type
    if (_next.get_type() == T_OP && _next.get_value().get<std::string>() == "?") {
        ts.nullable = true;
        tokenizer->advance();
    }

    return ts;
}

/**
 * Parses an identifier (wrapper function for lvalue check)
 * @return
 */
RefExpr::SharedPtr Parser::parse_identifier() {
    int start = tokenizer->last_token_index;
    RefExpr::SharedPtr ref_expr = __parse_identifier(true);
    ref_expr->set_index(start, tokenizer->current_index);

    // check if lvalue
    Token next = tokenizer->lookahead();
    if (next.get_type() == T_OP && next.get_value().get<std::string>() == "=") {
        ref_expr->return_ref(true);
        ref_expr->implicit_ref(true);
    }


    return ref_expr;
}

/**
 * Recursively parses an identifier. Can be a combination of a variable, property accesses,
 * array indices and method calls
 * e.g. foo.bar[3](), foo.bar, foo, foo.baz()[3]
 * @param ident_expr
 * @return
 */
RefExpr::SharedPtr Parser::__parse_identifier(bool initial) {
    Token _next = tokenizer->lookahead(false);
    int start = tokenizer->last_token_index;

    // identifier
    if (_next.get_type() == T_IDENT && initial) {
        tokenizer->advance(false);
        std::string ident = tokenizer->s_val();

        IdentifierRefExpr::SharedPtr ident_expr = std::make_shared<IdentifierRefExpr>(ident);
        ident_expr->set_member_expr(__parse_identifier());
        ident_expr->set_index(start, tokenizer->current_index);

        return ident_expr;
    }

    // function call
    if (_next.is_punctuator(C_OPEN_PAREN)) {
        CallExpr::SharedPtr call = parse_function_call();
        call->set_member_expr(__parse_identifier());
        call->set_index(start, tokenizer->current_index);

        return call;
    }

    // member access
    if (_next.is_punctuator(C_DOT)) {
        tokenizer->advance(false);
        tokenizer->advance(false);
        Token ident_t = tokenizer->current_token;
        std::string ident = tokenizer->s_val();

        // method call
        _next = tokenizer->lookahead();
        if (_next.is_punctuator(C_OPEN_PAREN)) {
            CallExpr::SharedPtr call = parse_function_call();
            call->set_index(start, tokenizer->current_index);
            MethodCallExpr::SharedPtr method_call = std::make_shared<MethodCallExpr>(*call, ident);
            method_call->set_member_expr(__parse_identifier());

            return method_call;
        }

        MemberRefExpr::SharedPtr mem_ref = std::make_shared<MemberRefExpr>(tokenizer->current_token.get_value());
        mem_ref->set_member_expr(__parse_identifier());
        mem_ref->set_index(start, tokenizer->current_index);

        return mem_ref;
    }

    // array access
    if (_next.is_punctuator(C_OPEN_SQUARE)) {
        tokenizer->advance(false);
        Expression::SharedPtr expr = parse_expression();
        expr->set_index(start, tokenizer->current_index);
        ArrayAccessExpr::SharedPtr arr_acc = std::make_shared<ArrayAccessExpr>(expr);

        tokenizer->advance();
        if (!tokenizer->current_token.is_punctuator(C_CLOSE_SQUARE)) {
            token_error();
        }

        arr_acc->set_member_expr(__parse_identifier());
        arr_acc->set_index(start, tokenizer->current_index);

        return arr_acc;
    }

    return {};
}


/**
 * Parses an object literal in the form of
 * {
 *    type prop_name = value
 *    type2 name2 = value2
 * }
 * @return
 */
ObjectLiteral::SharedPtr Parser::parse_object_literal() {
    int start = tokenizer->last_token_index;

    // opening curly brace
    tokenizer->advance();
    if (tokenizer->current_token.is_punctuator('{')) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '{' to begin an object literal", tokenizer);
    }

    ObjectLiteral::SharedPtr obj = std::make_shared<ObjectLiteral>();

    Token _next = tokenizer->lookahead();
    if (_next.is_punctuator(C_CLOSE_CURLY)) {
        tokenizer->advance();
        obj->set_index(start, tokenizer->current_index);

        return obj;
    }

    while (!tokenizer->current_token.is_punctuator(C_CLOSE_CURLY)) {

        // type definition
        TypeSpecifier ts = parse_type();

        // property name
        tokenizer->advance(T_IDENT);

        std::string _prop_name = tokenizer->current_token.get_value().get<std::string>();

        // equals sign
        tokenizer->advance(T_OP);
        if (tokenizer->current_token.get_value().get<std::string>() != "=") {
            token_error();
        }

        // value
        if (ts.is_array) {
            ArrayLiteral::SharedPtr arr = parse_array_literal();
            arr->set_type(ts.type);
            if (ts.is_var_length) {
                arr->is_var_length(true);
            }
            else {
                arr->set_length_expr(ts.length);
            }

            obj->add_prop(ObjectPropExpr(_prop_name, arr, OBJECT_T));
        }
        else {
            Expression::SharedPtr val_node = parse_expression();

            obj->add_prop(ObjectPropExpr(_prop_name, val_node, ts.type));
        }

        Token next = tokenizer->lookahead();
        if (next.is_punctuator(C_CLOSE_CURLY)) {
            tokenizer->advance();
            break;
        }
    }

    obj->set_index(start, tokenizer->current_index);

    return obj;
}

/**
 * Parses an array literal in the form of
 * [val1, val2, val3, ..., valn]
 * @return
 */
ArrayLiteral::SharedPtr Parser::parse_array_literal() {
    int start = tokenizer->last_token_index;

    // opening square brace
    tokenizer->advance(T_PUNCTUATOR);
    if (tokenizer->c_val() != C_OPEN_SQUARE) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '[' to begin an array literal", tokenizer);
    }

    TypeSpecifier type;
    type.type = AUTO_T;
    ArrayLiteral::SharedPtr arr = std::make_shared<ArrayLiteral>(type);

    int el_count = 0;
    bool var_length = false;
    while (!tokenizer->current_token.is_punctuator(C_CLOSE_SQUARE)) {
        Token next = tokenizer->lookahead();
        if (next.is_operator("..")) {
            tokenizer->advance();
            next = tokenizer->lookahead();
            if (next.is_punctuator('.')) {
                var_length = true;
                tokenizer->advance();
                tokenizer->advance();
                break;
            }
            else {
                tokenizer->backtrack();
            }
        }

        Expression::SharedPtr expr = parse_expression();
        arr->add_element(expr);

        tokenizer->advance();
        ++el_count;
    }

    if (!tokenizer->current_token.is_punctuator(']')) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ']' after array literal", tokenizer);
    }

    arr->set_length(var_length ? -1 : el_count);
    arr->set_index(start, tokenizer->current_index);

    return arr;
}

/**
 * Parses an atomic part of an expression, like a single identifier or literal
 * @return
 */
Expression::SharedPtr Parser::parse_unary_expr_target() {
    Token next = tokenizer->lookahead(false);

    if (next.is_punctuator('{')) {
        return parse_object_literal();
    }
    else if (next.is_punctuator('[')) {
        return parse_array_literal();
    }
    else if (next.is_punctuator('\\')) {
        return parse_lambda_expr();
    }
    else if (next.is_punctuator(C_OPEN_PAREN)) {
        return parse_expression();
    }
    else if (next.get_type() == T_IDENT) {
        return parse_identifier();
    }
    else if (next.get_type() == T_LITERAL) {
        int start = tokenizer->last_token_index;
        tokenizer->advance();

        if (tokenizer->current_token.get_value().get_type() == STRING_T) {
            Token next = tokenizer->lookahead();

            // possible string modifier
            if (next.get_type() == T_IDENT) {
                std::string modifier = next.get_value().get<std::string>();
                if (util::string_modifiers.find(modifier) != util::string_modifiers.end()) {
                    auto string_literal = std::make_shared<StringLiteral>(tokenizer->s_val(), modifier[0]);
                    tokenizer->advance();

                    string_literal->set_member_expr(__parse_identifier());
                    string_literal->set_index(start, tokenizer->current_index);

                    return string_literal;
                }
                else {
                    tokenizer->advance();
                    ParseError::raise(ERR_UNEXPECTED_CHARACTER, "Unknown string modifier " + modifier, tokenizer);
                }
            }
            else {
                auto string_literal = std::make_shared<StringLiteral>(tokenizer->s_val());
                string_literal->set_member_expr(__parse_identifier());
                string_literal->set_index(start, tokenizer->current_index);

                return string_literal;
            }
        }

        auto expr = std::make_shared<LiteralExpr>(tokenizer->current_token.get_value());
        expr->set_member_expr(__parse_identifier());
        expr->set_index(start, tokenizer->current_index);

        return expr;
    }
    else {
        tokenizer->advance();
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected T_IDENT or T_LITERAL, got "
                + util::token_names[next.get_type()], tokenizer);
    }
}

/**
 * Recursively parses a unary expression for example an identifier or a literal with unary expressions
 * applied to it, e.g.
 *      - ++3
 *      - x.foo()++
 *      - !bar[3]
 * @return
 */
Expression::SharedPtr Parser::parse_unary_expr(Expression::SharedPtr literal, bool postfix) {
    int start = tokenizer->last_token_index;
    Token _next = tokenizer->lookahead(false);

    // prefix unary op
    if (_next.get_type() == T_OP && !postfix
            && util::in_vector<std::string>(util::unary_operators, _next.get_value().get<std::string>()))
    {
        tokenizer->advance(false);

        UnaryOperator::SharedPtr unary_op = std::make_shared<UnaryOperator>(tokenizer->s_val(), "prefix");

        unary_op->set_child(parse_unary_expr(literal, postfix));
        unary_op->set_index(start, tokenizer->current_index);

        return unary_op;
    }

    // typecast
    if (_next.is_punctuator(C_OPEN_PAREN) && !postfix) {
        tokenizer->advance();
        _next = tokenizer->lookahead();
        if (_next.get_type() == T_TYPE) {
            tokenizer->advance();
            ExplicitCastExpr::SharedPtr cast_op = std::make_shared<ExplicitCastExpr>(tokenizer->s_val());

            tokenizer->advance();
            if (!tokenizer->current_token.is_punctuator(C_CLOSE_PAREN)) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ')' after typecast operator",
                        tokenizer);
            }

            cast_op->set_child(parse_expression({}, util::op_precedence["typecast"]));
            cast_op->set_index(start, tokenizer->current_index);

            return cast_op;
        }
        else {
            tokenizer->backtrack();
        }
    }

    if (literal == nullptr) {
        literal = parse_unary_expr_target();
    }
    else if (!postfix) {
        literal->set_child(parse_unary_expr_target());
    }

    _next = tokenizer->lookahead(false);

    // postfix unary op
    if (_next.get_type() == T_OP && (_next.get_value().get<std::string>() == "++"
         || _next.get_value().get<std::string>() == "--"))
    {
        tokenizer->advance();
        UnaryOperator::SharedPtr unary_op = std::make_shared<UnaryOperator>(tokenizer->s_val(), "postfix");

        unary_op->set_child(literal);
        unary_op->set_index(start, tokenizer->current_index);

        return unary_op;
    }

    literal->set_index(start, tokenizer->current_index);

    return literal;
}

TertiaryOperator::SharedPtr Parser::parse_tertiary_operator(Expression::SharedPtr cond) {
    int start = tokenizer->last_token_index;

    Expression::SharedPtr if_branch = parse_expression();
    tokenizer->advance();
    if (!tokenizer->current_token.is_operator(":")) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ':' in tertiary expression", tokenizer);
    }
    Expression::SharedPtr else_branch = parse_expression();

    auto op = std::make_shared<TertiaryOperator>(cond, if_branch, else_branch);
    op->set_index(start, tokenizer->current_index);

    return op;
}

/**
 * Recursively parses an arbitrary (semantically correct) combination of literals, operators and
 *  perenthesized expressions
 * @param lhs Left hand side of the current expression
 * @param min_precedence Minimum operator precedence for current expression
 * @return
 */
Expression::SharedPtr Parser::parse_expression(Expression::SharedPtr lhs, int min_precedence) {
    int start = tokenizer->last_token_index;
    Token next = tokenizer->lookahead(false);

    // use recursion and assert that the next character is a closing parenthesis
    if (next.is_punctuator(C_OPEN_PAREN)) {
        tokenizer->advance(false);

        // check if typecast
        next = tokenizer->lookahead(false);
        if (next.get_type() == T_TYPE) {
            tokenizer->backtrack();
        }
        else {
            auto res = parse_expression(lhs, 0);

            next = tokenizer->lookahead();
            if (!next.is_punctuator(C_CLOSE_PAREN)) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ')'", tokenizer);
            }

            tokenizer->advance(false);

            // possible unary operator
            auto expr = parse_expression(res, min_precedence);
            expr->set_index(start, tokenizer->current_index);

            return expr;
        }
    }

    // initial parsing of left hand side argument
    if (lhs == nullptr) {
        lhs = parse_unary_expr();

        next = tokenizer->lookahead(false);
    }

    // tertiary operator
    if (next.get_type() == T_OP && next.get_value().get<std::string>() == "?") {
        tokenizer->advance(false);
        auto op = parse_tertiary_operator(lhs);
        op->set_index(start, tokenizer->current_index);

        return op;
    }
    else if (next.get_type() == T_OP && next.get_value().get<std::string>() == ":") {
        lhs->set_index(start, tokenizer->current_index);

        return lhs;
    }

    // ...while the next operator has a higher precedence than the minimum
    while (next.get_type() == T_OP &&
            util::in_vector(util::binary_operators, next.get_value().get<std::string>()) &&
            util::op_precedence[next.get_value().get<std::string>()] >= min_precedence)
    {
        std::string op = next.get_value().get<std::string>();
        tokenizer->advance(false);

        next = tokenizer->lookahead(false);
        Expression::SharedPtr rhs;

        // parenthesis recursion
        if (next.is_punctuator(C_OPEN_PAREN)) {
            tokenizer->advance(false);
            next = tokenizer->lookahead(false);

            // typecast
            if (next.get_type() == T_TYPE) {
                tokenizer->backtrack();
                rhs = parse_unary_expr();
            }
            else {
                rhs = parse_expression({}, 0);

                next = tokenizer->lookahead();
                if (!next.is_punctuator(C_CLOSE_PAREN)) {
                    ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ')'",
                            tokenizer);
                }

                tokenizer->advance(false);

                rhs = parse_unary_expr(rhs, true);
            }
        }
        else {
            // right hand side expression
            rhs = parse_unary_expr();
            next = tokenizer->lookahead(false);
        }

        // continue recursively while a higher precedence operator follows
        while (next.get_type() == T_OP &&
                util::op_precedence[next.get_value().get<std::string>()] > util::op_precedence[op])
        {
            rhs = parse_expression(rhs, util::op_precedence[next.get_value().get<std::string>()]);
            next = tokenizer->lookahead(false);
        }

        // return the expression so far
        BinaryOperator::SharedPtr binary_op = std::make_shared<BinaryOperator>(op);
        binary_op->set_fst_child(lhs);
        binary_op->set_snd_child(rhs);

        lhs = binary_op;
    }

    // tertiary operator
    if (next.get_type() == T_OP && next.get_value().get<std::string>() == "?" &&
            util::op_precedence["?"] >= min_precedence) {
        tokenizer->advance(false);
        auto op = parse_tertiary_operator(lhs);
        op->set_index(start, tokenizer->current_index);

        return op;
    }

    lhs->set_index(start, tokenizer->current_index);

    return lhs;
}

/**
 * Parses a variable (re-) assigment, e.g.
 *  let x = 3, y = 4;
 *  x.num = 18;
 * @param reassign
 * @return
 */
Statement::SharedPtr Parser::parse_assignment(bool auto_type, CompoundStmt::SharedPtr cmp_stmt) {

    int start = tokenizer->last_token_index;

    // type declaration
    TypeSpecifier ts;
    if (!auto_type) {
        ts = parse_type();
    }

    // identifier for assignment
    tokenizer->advance(T_IDENT);
    std::string _ident = tokenizer->current_token.get_value().get<std::string>();

    Token next = tokenizer->lookahead(false);
    if (!ts.nullable && !next.is_operator("=")) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '=' after non-nullable variable declaration",
                tokenizer);
    }
    else if (!next.is_operator("=")) {
        auto decl_stmt = std::make_shared<DeclStmt>(_ident, ts);
        decl_stmt->set_index(start, tokenizer->current_index);

        return decl_stmt;
    }
    else if (!next.is_operator("=")) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '=' after variable declaration",
                tokenizer);
    }

    // equals sign
    tokenizer->advance(false);
    DeclStmt::SharedPtr decl_stmt;
    if (ts.is_array) {
        ArrayLiteral::SharedPtr arr = parse_array_literal();
        arr->set_type(ts.type);
        if (ts.is_var_length) {
            arr->is_var_length(true);
        }
        else {
            arr->set_length_expr(ts.length);
        }

        ts.type = OBJECT_T;
        decl_stmt = std::make_shared<DeclStmt>(_ident, arr, ts);
    }
    else {
        Expression::SharedPtr expr = parse_expression();
        decl_stmt = std::make_shared<DeclStmt>(_ident, expr, ts);
    }

    if (cmp_stmt != nullptr) {
        cmp_stmt->add_statement(decl_stmt);
    }

    // multiple declarations
    Token _next = tokenizer->lookahead();
    if (_next.is_punctuator(',')) {
        tokenizer->advance();

        if (cmp_stmt == nullptr) {
            cmp_stmt = std::make_shared<CompoundStmt>();
            cmp_stmt->add_statement(decl_stmt);
        }

        auto assign = parse_assignment(auto_type, cmp_stmt);
        assign->set_index(start, tokenizer->current_index);

        return assign;
    }
    else if (cmp_stmt == nullptr) {
        decl_stmt->set_index(start, tokenizer->current_index);

        return decl_stmt;
    }

    cmp_stmt->set_index(start, tokenizer->current_index);

    return cmp_stmt;
}

/**
 * Parses a method's argument list
 * @return
 */
std::vector<FuncArgDecl::SharedPtr> Parser::parse_arg_list() {

    int start = tokenizer->last_token_index;

    std::vector<FuncArgDecl::SharedPtr> args;
    if (!tokenizer->current_token.is_punctuator('(')) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '(' to start an argument list",
                tokenizer);
    }

    bool def_arg = false;
    tokenizer->advance();

    while (tokenizer->current_token.get_type() == T_TYPE) {
        FuncArgDecl::SharedPtr arg_dec = std::make_shared<FuncArgDecl>();

        // type declaration
        TypeSpecifier ts = parse_type();
        arg_dec->set_type(ts.type);

        // identifier
        tokenizer->advance(T_IDENT);
        arg_dec->set_name(tokenizer->current_token.get_value().get<std::string>());

        // optional default value
        Token _next = tokenizer->lookahead();
        if (_next.get_type() == T_OP && _next.get_value().get<std::string>() == "=") {
            tokenizer->advance();
            Expression::SharedPtr _expr = parse_expression();
            arg_dec->set_default(_expr);
            def_arg = true;
            _next = tokenizer->lookahead();
        } else if (def_arg) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Default values are only allowed as last items of an argument list",
                    tokenizer);
        }

        arg_dec->set_index(start, tokenizer->current_index);

        args.push_back(arg_dec);

        // end of argument list or next argument
        if (_next.is_punctuator(C_COMMA)) {
            tokenizer->advance();
        } else if (!_next.is_punctuator(C_CLOSE_PAREN)) {
            ParseError::raise(ERR_UNEXPECTED_CHARACTER, "Expected closing parenthesis after argument list.",
                    tokenizer);
        }

        tokenizer->advance();
        start = tokenizer->current_index;
    }

    return args;
}

/**
 * Parses a function declaration in the form of
 *  def func(x: number, y: string) => bool {
 *      ... statements
 *  }
 */
FunctionDecl::SharedPtr Parser::parse_function_decl() {

    int start = tokenizer->last_token_index;

    // function name
    tokenizer->advance(T_IDENT);
    FunctionDecl::SharedPtr fun_dec = std::make_shared<FunctionDecl>(tokenizer->s_val());

    // arguments
    tokenizer->advance();
    std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list();
    for (auto arg : args) {
        fun_dec->add_arg(arg);
    }

    // optional return type
    Token _next = tokenizer->lookahead();
    ValueType _type = VOID_T;
    if (_next.get_type() == T_OP && _next.get_value().get<std::string>() == "-") {
        tokenizer->advance();

        _next = tokenizer->lookahead();
        if (_next.get_value().get<std::string>() != ">") {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '->' after function argument list",
                    tokenizer);
        }

        tokenizer->advance();
        tokenizer->advance();
        TypeSpecifier ts = parse_type();

        _type = ts.type;
    }

    fun_dec->set_return_type(_type);

    // function body
    std::string body = tokenizer->get_next_block();
    Parser _int(body);
    CompoundStmt::SharedPtr func_body = _int.parse();
    func_body->returnable(true);
    fun_dec->set_body(func_body);
    fun_dec->set_index(start, tokenizer->current_index);

    return fun_dec;
}

/**
 * Parses a lambda expression
 * @return
 */
LambdaExpr::SharedPtr Parser::parse_lambda_expr() {
    int start = tokenizer->last_token_index;

    tokenizer->advance(); // backslash
    tokenizer->advance();
    std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list();

    TypeSpecifier return_type = { VOID_T };
    Token next = tokenizer->lookahead();
    if (next.get_type() == T_OP && next.get_value().get<std::string>() == "-") {
        tokenizer->advance(); // -

        next = tokenizer->lookahead();
        if (next.get_value().get<std::string>() != ">") {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '->' after function argument list",
                    tokenizer);
        }

        tokenizer->advance(); // >
        next = tokenizer->lookahead();
        if (!next.is_punctuator('{')) {
            tokenizer->advance();
            return_type = parse_type();
        }
    }

    LambdaExpr::SharedPtr lambda_expr = std::make_shared<LambdaExpr>(return_type, args);
    CompoundStmt::SharedPtr body = parse_block();
    body->returnable(true);
    body->is_lambda_body(true);

    lambda_expr->set_body(body);
    lambda_expr->set_index(start, tokenizer->current_index);
    lambda_expr->set_member_expr(__parse_identifier(false));

    return lambda_expr;
}

/**
 * Parses a class constructor declaration
 * @param am
 * @return
 */
ConstrDecl::SharedPtr Parser::parse_constr_decl(AccessModifier am) {

    tokenizer->advance();
    std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list();

    std::string body = tokenizer->get_next_block();
    Parser _int(body);
    CompoundStmt::SharedPtr constr_body = _int.parse();


    auto constr = std::make_shared<ConstrDecl>(args, constr_body, am);

    return constr;
}

/**
 * Parses a class field declaration
 * @param am
 * @param is_static
 * @return
 */
FieldDecl::SharedPtr Parser::parse_field_decl(AccessModifier am, bool is_static, TypeSpecifier ts) {

    std::string field_name = tokenizer->s_val();
    Token next = tokenizer->lookahead();
    FieldDecl::SharedPtr field = std::make_shared<FieldDecl>(field_name, ts, am, is_static);

    // getter and setter
    bool getter = false;
    bool setter = false;
    if (next.is_punctuator('{')) {
        tokenizer->advance();

        get_and_set:
        tokenizer->advance();
        if (tokenizer->current_token.get_type() == T_KEYWORD && tokenizer->s_val() == "get") {
            if (getter) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Cannot redeclare 'get' on the same field",
                        tokenizer);
            }

            getter = true;
            tokenizer->advance();
        }
        else if (tokenizer->current_token.get_type() == T_KEYWORD && tokenizer->s_val() == "set") {
            if (setter) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Cannot redeclare 'set' on the same field",
                        tokenizer);
            }

            setter = true;
            tokenizer->advance();
        }

        if (tokenizer->current_token.is_punctuator(',')) {
            goto get_and_set;
        }

        if (!tokenizer->current_token.is_punctuator('}')) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '}");
        }

        next = tokenizer->lookahead();
    }

    field->generate(getter, setter);

    // optional default value
    if (next.get_type() == T_OP && next.get_value().get<std::string>() == "=") {
        tokenizer->advance();
        field->set_default(parse_expression());
    }
    else if (is_static && !ts.nullable) {
        ParseError::raise(ERR_UNINITIALIZED_VAR, "Non-nullable static field " + field_name + " must have a default value",
                tokenizer);
    }

    tokenizer->advance(false);
    if (!tokenizer->current_token.is_punctuator('\n')) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Field declarations must be on seperate lines", tokenizer);
    }

    return field;
}

/**
 * Parses a method declaration
 * @param am
 * @param is_static
 * @param ts
 * @return
 */
MethodDecl::SharedPtr Parser::parse_method_decl(AccessModifier am, bool is_static) {
    std::string method_name = tokenizer->s_val();
    tokenizer->advance();
    std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list();

    // optional return type
    Token next = tokenizer->lookahead();
    TypeSpecifier ts;
    if (next.is_operator("-")) {
        tokenizer->advance();

        next = tokenizer->lookahead();
        if (next.get_value().get<std::string>() != ">") {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '->' after function argument list",
                    tokenizer);
        }

        tokenizer->advance();
        tokenizer->advance();
        ts = parse_type();
    }

    std::string body = tokenizer->get_next_block();
    Parser _int(body);
    CompoundStmt::SharedPtr method_body = _int.parse();
    method_body->returnable(true);

    tokenizer->advance(false);
    if (!tokenizer->current_token.is_punctuator('\n')) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Method declarations must be on seperate lines", tokenizer);
    }

    auto method = std::make_shared<MethodDecl>(method_name, ts, args, method_body, am, is_static);

    return method;
}

/**
 * Parses an operator method declaration
 * @param am
 * @return
 */
OperatorDecl::SharedPtr Parser::parse_operator_decl(AccessModifier am, std::string class_name) {
    int start = tokenizer->last_token_index;

    std::string op = tokenizer->s_val();
    if (!util::in_vector<std::string>(util::unary_operators, op) && !util::in_vector<std::string>
        (util::binary_operators, op) )
    {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Unknown operator " + op, tokenizer);
    }

    tokenizer->advance();
    std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list();
    bool is_binary = args.size() == 1;

    // could be binary or unary
    if (op == "+" || op == "-") {
        if (args.size() == 1) {
            is_binary = true;
        }
    }
    if (util::in_vector<std::string>(util::binary_operators, op) || is_binary) {
        if (args.size() != 1) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Binary operator " + op + " requires exactly one argument",
                    tokenizer);
        }
    }
    if (util::in_vector<std::string>(util::unary_operators, op)) {
        if (args.size() != 0) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Unary operator " + op + " requires exactly zero arguments",
                    tokenizer);
        }
    }

    // optional return type
    Token next = tokenizer->lookahead();
    TypeSpecifier ts;
    bool type_specified = false;
    if (next.is_operator("-")) {
        tokenizer->advance();

        next = tokenizer->lookahead();
        if (next.get_value().get<std::string>() != ">") {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '->' after function argument list",
                    tokenizer);
        }

        tokenizer->advance();
        tokenizer->advance();
        ts = parse_type();
        type_specified = true;
    }

    CompoundStmt::SharedPtr body = parse_block();
    body->returnable(true);
    OperatorDecl::SharedPtr op_decl = std::make_shared<OperatorDecl>(op, args, am, is_binary);
    if (type_specified) {
        op_decl->set_return_type(ts);
    }

    op_decl->set_body(body);
    op_decl->set_index(start, tokenizer->current_index);

    return op_decl;

}

/**
 * Parses a class declaration
 * @return
 */
ClassDecl::SharedPtr Parser::parse_class_decl() {
    int start = tokenizer->last_token_index;

    AccessModifier am = AccessModifier::PRIVATE;
    if (tokenizer->s_val() == "public") {
        am = AccessModifier::PUBLIC;
        tokenizer->advance();
    }
    else if (tokenizer->s_val() == "protected") {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Classes cannot be declared 'protected'", tokenizer);
    }
    else if (tokenizer->s_val() == "private") {
        tokenizer->advance();
    }

    if (tokenizer->current_token.get_type() != T_KEYWORD || tokenizer->s_val() != "class") {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected keyword 'class' to start class declaration",
                tokenizer);
    }

    tokenizer->advance(T_IDENT);
    std::string class_name = tokenizer->s_val();

    //TODO extends, implements

    tokenizer->advance();
    if (!tokenizer->current_token.is_punctuator('{')) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '{' to start a block statement",
                tokenizer);
    }

    ConstrDecl::SharedPtr constr;
    std::vector<FieldDecl::SharedPtr> fields;
    std::vector<MethodDecl::SharedPtr> methods;
    std::unordered_map<std::string, OperatorDecl::SharedPtr> unary_operators;
    std::unordered_map<std::string, OperatorDecl::SharedPtr> binary_operators;
    bool declaration_finished = false;

    while (!declaration_finished) {
        int field_start = tokenizer->current_index;
        tokenizer->advance();
        AccessModifier current_am = AccessModifier::PUBLIC;
        bool am_set = false;
        bool is_static = false;
        bool is_operator = false;

        while (tokenizer->current_token.get_type() == T_KEYWORD) {
            std::string keyword =tokenizer->s_val();
            if (am_set && (keyword == "public" || keyword == "protected" || keyword == "private")) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Field access modifier already declared", tokenizer);
            }
            else if (is_static && keyword == "static") {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Field already declared static", tokenizer);
            }
            else if (is_static && keyword == "operator") {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Operator cannot be declared static", tokenizer);
            }
            else if (is_operator && keyword == "operator") {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Cannot redeclare operator keyword on the same field", tokenizer);
            }

            if (keyword == "public") {
                am_set = true;
            }
            else if (keyword == "protected") {
                current_am = AccessModifier::PROTECTED;
                am_set = true;
            }
            else if (keyword == "private") {
                current_am = AccessModifier::PRIVATE;
                am_set = true;
            }
            else if (keyword == "static") {
                is_static = true;
            }
            else if (keyword == "operator") {
                is_operator = true;
            }

            tokenizer->advance();
        }

        if (is_operator) {
            auto op = parse_operator_decl(current_am, class_name);
            op->set_index(field_start, tokenizer->current_index);

            if (op->is_binary) {
                binary_operators.emplace(op->get_operator(), op);
            }
            else {
                unary_operators.emplace(op->get_operator(), op);
            }
        }
        else if (tokenizer->current_token.get_type() == T_IDENT && tokenizer->s_val() == class_name) {
            if (is_static) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Constructor cannot be declared static",
                        tokenizer);
            }

            constr = parse_constr_decl(current_am);
            constr->set_index(field_start, tokenizer->current_index);
        }
        else if (!tokenizer->current_token.is_punctuator('}')) {

            if (tokenizer->current_token.get_type() == T_TYPE) {
                TypeSpecifier ts = parse_type();
                tokenizer->advance();

                auto field = parse_field_decl(current_am, is_static, ts);
                field->set_index(field_start, tokenizer->current_index);
                fields.push_back(field);
            }
            else if (tokenizer->current_token.get_type() == T_IDENT) {
                Token next = tokenizer->lookahead();
                if (next.is_punctuator('(')) {
                    auto method = parse_method_decl(current_am, is_static);
                    method->set_index(field_start, tokenizer->current_index);
                    methods.push_back(method);
                }
                else {
                    TypeSpecifier ts = parse_type();
                    auto field = parse_field_decl(current_am, is_static, ts);
                    field->set_index(field_start, tokenizer->current_index);
                    fields.push_back(field);
                }
            }
        }
        else {
            declaration_finished = true;
        }

        Token next = tokenizer->lookahead();
        if (next.is_punctuator('}')) {
            tokenizer->advance();
            declaration_finished = true;
        }
    }

    auto class_dec = std::make_shared<ClassDecl>(class_name, fields, methods, constr, am, unary_operators,
            binary_operators);
    class_dec->set_index(start, tokenizer->current_index);

    return class_dec;
}

/**
 * Parses an if/else statement
 * @return
 */
IfStmt::SharedPtr Parser::parse_if_stmt() {
    int start = tokenizer->last_token_index;

    Token next = tokenizer->lookahead();
    if (!next.is_punctuator(C_OPEN_PAREN)) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '(' to begin if statement",
                tokenizer);
    }

    tokenizer->advance();

    Expression::SharedPtr if_cond = parse_expression();

    tokenizer->advance(); // last part of expression
    tokenizer->advance(); // closing parenthesis

    CompoundStmt::SharedPtr if_branch = std::static_pointer_cast<CompoundStmt>(parse_next_stmt());

    IfStmt::SharedPtr if_stmt = std::make_shared<IfStmt>(if_cond, if_branch);

    next = tokenizer->lookahead();
    if (next.get_type() == T_KEYWORD && next.get_value().get<std::string>() == "else") {
        tokenizer->advance();
        tokenizer->advance();

        CompoundStmt::SharedPtr else_branch = std::static_pointer_cast<CompoundStmt>(parse_next_stmt());
        if_stmt->set_else_branch(else_branch);
    }

    if_stmt->set_index(start, tokenizer->current_index);

    return if_stmt;
}

CaseStmt::SharedPtr Parser::parse_case_stmt(bool default_) {
    int start = tokenizer->last_token_index;

    CaseStmt::SharedPtr case_stmt;
    if (!default_) {
        Expression::SharedPtr case_val = parse_expression();
        case_stmt = std::make_shared<CaseStmt>(case_val);
    }
    else {
        case_stmt = std::make_shared<CaseStmt>();
    }

    tokenizer->advance();
    if (!tokenizer->current_token.is_operator(":")) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ':' after case label",
                tokenizer);
    }

    bool case_finished = false;
    while (!case_finished) {
        tokenizer->advance();
        if (tokenizer->current_token.is_punctuator('}')) {
            case_finished = true;
            break;
        }
        else if (tokenizer->current_token.is_keyword("case") || tokenizer->current_token.is_keyword("default")) {
            case_finished = true;
            break;
        }

        Statement::SharedPtr stmt = parse_next_stmt();
        case_stmt->add_statement(stmt);
    }

    case_stmt->set_index(start, tokenizer->current_index);

    return case_stmt;
}

SwitchStmt::SharedPtr Parser::parse_switch_stmt() {
    int start = tokenizer->last_token_index;

    tokenizer->advance();
    if (!tokenizer->current_token.is_punctuator('(')) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '(' after switch",
                tokenizer);
    }

    Expression::SharedPtr switch_val = parse_expression();
    tokenizer->advance(); // closing paren
    tokenizer->advance(); // opening curly
    if (!tokenizer->current_token.is_punctuator('{')) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '{' before switch block",
                tokenizer);
    }

    SwitchStmt::SharedPtr switch_stmt = std::make_shared<SwitchStmt>(switch_val);

    tokenizer->advance();
    while (!tokenizer->current_token.is_punctuator('}')) {
        Statement::SharedPtr stmt = parse_next_stmt();
        CaseStmt::SharedPtr case_stmt = std::dynamic_pointer_cast<CaseStmt>(stmt);
        if (case_stmt != nullptr) {
            switch_stmt->add_case(case_stmt);
        }
    }

    switch_stmt->set_index(start, tokenizer->current_index);

    return switch_stmt;
}

WhileStmt::SharedPtr Parser::parse_while_stmt() {
    int start = tokenizer->last_token_index;

    Token next = tokenizer->lookahead();
    if (!next.is_punctuator(C_OPEN_PAREN)) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '(' after while keyword",
                tokenizer);
    }

    Expression::SharedPtr while_cond = parse_expression();

    tokenizer->advance();

    CompoundStmt::SharedPtr while_block = std::static_pointer_cast<CompoundStmt>(parse_next_stmt());
    WhileStmt::SharedPtr while_stmt = std::make_shared<WhileStmt>(while_cond, while_block);

    while_block->set_index(start, tokenizer->current_index);

    return while_stmt;
}

ForStmt::SharedPtr Parser::parse_for_stmt() {
    int start = tokenizer->last_token_index;

    Token next = tokenizer->lookahead();
    if (!next.is_punctuator(C_OPEN_PAREN)) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '(' after for keyword",
                tokenizer);
    }

    tokenizer->advance();
    tokenizer->advance();

    Statement::SharedPtr init = parse_next_stmt();
    tokenizer->advance();

    // range based for loop
    if (tokenizer->current_token.is_keyword("in")) {
        IdentifierRefExpr::SharedPtr ident = std::dynamic_pointer_cast<IdentifierRefExpr>(init);
        if (ident == nullptr) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected identifier before 'in' in range based for loop",
                    tokenizer);
        }

        ident->implicit_ref(false);

        Expression::SharedPtr range = parse_expression();
        tokenizer->advance();
        if (!tokenizer->current_token.is_punctuator(C_CLOSE_PAREN)) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ')' after loop arguments");
        }

        tokenizer->advance();
        CompoundStmt::SharedPtr block = std::static_pointer_cast<CompoundStmt>(parse_next_stmt());

        ForStmt::SharedPtr for_stmt = std::make_shared<ForStmt>(ident, range);
        for_stmt->set_body(block);
        for_stmt->set_index(start, tokenizer->current_index);

        return for_stmt;
    }

    if (!tokenizer->current_token.is_punctuator(C_SEMICOLON)) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ';' to seperate for loop arguments",
                tokenizer);
    }

    tokenizer->advance();
    Statement::SharedPtr term = parse_next_stmt();
    tokenizer->advance();
    if (!tokenizer->current_token.is_punctuator(C_SEMICOLON)) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ';' to seperate for loop arguments",
                tokenizer);
    }

    tokenizer->advance();
    Statement::SharedPtr inc = parse_next_stmt();
    tokenizer->advance();
    if (!tokenizer->current_token.is_punctuator(C_CLOSE_PAREN)) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ')' after loop arguments",
                tokenizer);
    }

    tokenizer->advance();

    CompoundStmt::SharedPtr block = std::static_pointer_cast<CompoundStmt>(parse_next_stmt());

    ForStmt::SharedPtr for_stmt = std::make_shared<ForStmt>(init, term, inc);
    for_stmt->set_body(block);
    for_stmt->set_index(start, tokenizer->current_index);

    return for_stmt;
}

/**
 * Interprets a keyword statement
 */
Statement::SharedPtr Parser::parse_keyword() {
    int start = tokenizer->last_token_index;

    std::string keyword = tokenizer->current_token.get_value().get<std::string>();

    if (keyword == "def") {
        FunctionDecl::SharedPtr fun_dec = parse_function_decl();
        fun_dec->set_index(start, tokenizer->current_index);

        return fun_dec;
    }
    else if (keyword == "in") {
        tokenizer->advance(T_IDENT);

        Variant _ident = tokenizer->current_token.get_value();

        Token _next = tokenizer->lookahead();
        ValueType _type = ANY_T;
        if (_next.is_punctuator(':')) {
            tokenizer->advance();
            tokenizer->advance();
            std::string type = tokenizer->current_token.get_value().get<std::string>();

            if (!tokenizer->is_type_keyword(type)) {
                token_error();
            }

            _type = val::strtotype(type);
        }

        InputStmt::SharedPtr in_stmt = std::make_shared<InputStmt>(_ident, _type);
        in_stmt->set_index(start, tokenizer->current_index);

        return in_stmt;
    }
    else if (keyword == "out" || keyword == "outln") {

        // _tokenizer->advance identifier or get_value
        Expression::SharedPtr _expr = parse_expression();

        bool _newline = false;
        if (keyword == "outln") {
            _newline = true;
        }

        OutputStmt::SharedPtr out_stmt = std::make_shared<OutputStmt>(_expr, _newline);
        out_stmt->set_index(start, tokenizer->current_index);

        return out_stmt;
    }
    else if (keyword == "if") {
        IfStmt::SharedPtr if_stmt = parse_if_stmt();
        if_stmt->set_index(start, tokenizer->current_index);

        return if_stmt;
    }
    else if (keyword == "while") {
        WhileStmt::SharedPtr while_stmt = parse_while_stmt();
        while_stmt->set_index(start, tokenizer->current_index);

        return while_stmt;
    }
    else if (keyword == "switch") {
        auto switch_stmt = parse_switch_stmt();
        switch_stmt->set_index(start, tokenizer->current_index);

        return switch_stmt;
    }
    else if (keyword == "case") {
        auto case_stmt = parse_case_stmt();
        case_stmt->set_index(start, tokenizer->current_index);

        return case_stmt;
    }
    else if (keyword == "default") {
        auto def_stmt = parse_case_stmt(true);
        def_stmt->set_index(start, tokenizer->current_index);

        return def_stmt;
    }
    else if (keyword == "for") {
        ForStmt::SharedPtr for_stmt = parse_for_stmt();
        for_stmt->set_index(start, tokenizer->current_index);

        return for_stmt;
    }
    else if (keyword == "continue") {
        ContinueStmt::SharedPtr cont_stmt = std::make_shared<ContinueStmt>();
        cont_stmt->set_index(start, tokenizer->current_index);

        return cont_stmt;
    }
    else if (keyword == "break") {
        BreakStmt::SharedPtr break_stmt = std::make_shared<BreakStmt>();
        break_stmt->set_index(start, tokenizer->current_index);

        return break_stmt;
    }
    else if (keyword == "public" || keyword == "private" || keyword == "protected" || keyword == "class") {
        return parse_class_decl();
    }
    else if (keyword == "return") {
        Expression::SharedPtr expr = parse_expression();
        ReturnStmt::SharedPtr return_stmt = std::make_shared<ReturnStmt>(expr);
        return_stmt->set_index(start, tokenizer->current_index);

        return return_stmt;
    }
    else if (keyword == "goto") {
        tokenizer->advance();
        auto goto_stmt = std::make_shared<GotoStmt>(tokenizer->s_val());
        goto_stmt->set_index(start, tokenizer->current_index);

        return goto_stmt;
    }
    else {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Unknown keyword '" + keyword + "'", tokenizer);
    }
}

/**
 * Parses a function call in the form func(arg1, arg2, ...)
 * @return
 */
CallExpr::SharedPtr Parser::parse_function_call() {
    int start = tokenizer->last_token_index;

    CallExpr::SharedPtr call = std::make_shared<CallExpr>();
    tokenizer->advance(T_PUNCTUATOR);

    // collect arguments
    Token _next = tokenizer->lookahead();
    if (!_next.is_punctuator(C_CLOSE_PAREN)) {
        parse_func_call_arg:
        Expression::SharedPtr arg = parse_expression();
        call->add_argument(arg);

        _next = tokenizer->lookahead();
        if (_next.is_punctuator(C_COMMA)) {
            tokenizer->advance();
            goto parse_func_call_arg;
        } else if (!_next.is_punctuator(C_CLOSE_PAREN)) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected closing parenthesis after function call", tokenizer);
        }
    }

    tokenizer->advance();
    call->set_index(start, tokenizer->current_index);

    return call;
}

CompoundStmt::SharedPtr Parser::parse_block() {
    int start = tokenizer->last_token_index;
    std::string block = tokenizer->get_next_block();

    Parser interp(block);
    CompoundStmt::SharedPtr cmp_stmt = interp.parse();
    cmp_stmt->set_index(start, tokenizer->current_index);

    return cmp_stmt;
}

Statement::SharedPtr Parser::parse_next_stmt() {
    if (tokenizer->current_token.is_punctuator('{')) {
        tokenizer->backtrack();
        CompoundStmt::SharedPtr cmp_stmt = parse_block();

        return cmp_stmt;
    }
    else if (tokenizer->current_token.get_type() == T_TYPE) {
        Statement::SharedPtr assign = parse_assignment(false);

        return assign;
    }
    else if (tokenizer->current_token.get_type() == T_KEYWORD) {
        Statement::SharedPtr expr = parse_keyword();

        return expr;
    }
    else if (tokenizer->current_token.get_type() == T_IDENT && tokenizer->lookahead().is_operator(":")) {
        std::string label = tokenizer->s_val();
        tokenizer->advance();

        return std::make_shared<LabelStmt>(label);
    }
    else {
        tokenizer->backtrack();
        Expression::SharedPtr expr = parse_expression();

        return expr;
    }
}

/**
 * Runs the program by tokenizing the program, creating the AST and finally evaluating it.
 * @return
 */
void Parser::run(bool debug = false) {
    parse();
    delete tokenizer;

    if (debug) {
        prog_root->__dump(0);
        std::cout << std::endl << std::endl;
    }

    EvaluatingVisitor ev;
    ev.evaluate(prog_root.get());
}

/**
 * Parses the program into an AST
 * @return
 */
CompoundStmt::SharedPtr Parser::parse() {
    while(tokenizer->current_token.get_type() != T_EOF) {
        tokenizer->advance();
        while (tokenizer->current_token.is_punctuator('\n')) {
            tokenizer->advance();
        }

        if (tokenizer->current_token.get_type() == T_EOF) {
            break;
        }

        Statement::SharedPtr stmt = parse_next_stmt();

        prog_root->add_statement(stmt);
    }

    return prog_root;
}