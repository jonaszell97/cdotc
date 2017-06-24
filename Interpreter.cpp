//
// Created by Jonas Zell on 13.06.17.
//

#include <string>
#include "Interpreter.h"
#include "Variant/Variant.h"
#include <vector>
#include "Util.h"
#include <iostream>
#include "Exceptions.h"
#include "Debug.h"
#include "Objects/Object.h"
#include "AST/Expression/RefExpr/IdentifierRefExpr.h"
#include "AST/Operator/UnaryOperator.h"
#include "AST/Expression/Literal/LiteralExpr.h"
#include "AST/Operator/BinaryOperator.h"
#include "AST/Statement/DeclStmt.h"
#include "AST/Statement/IO/InputStmt.h"
#include "AST/Statement/IO/OutputStmt.h"
#include "AST/Expression/Literal/ObjectLiteral.h"
#include "AST/Statement/CompoundStmt.h"
#include "AST/Statement/Function/FunctionDecl.h"
#include "AST/Expression/RefExpr/CallExpr.h"
#include "AST/Expression/RefExpr/MemberRefExpr.h"
#include "AST/Expression/RefExpr/ArrayAccessExpr.h"
#include "AST/Statement/Function/ReturnStmt.h"
#include "AST/Operator/ExplicitCastExpr.h"

/**
 * Creates a new interpreter for an Xtreme Jonas Script program.
 * @param program
 */
Interpreter::Interpreter(std::string program) :
    tokenizer(std::make_unique<Tokenizer>(program)),
    prog_root(std::make_shared<CompoundStmt>())
{

}

/**
 * Throws an unexpected token error
 */
void Interpreter::token_error() {
    ParseError::raise(ERR_UNEXPECTED_TOKEN, "Unexpected Token " + util::token_names[tokenizer->current_token.get_type()]
                                   + " on line " + std::to_string(tokenizer->current_line) + ":"
                                   + std::to_string(tokenizer->index_on_line));
}

/**
 * Throws an unexpected token error
 * @param expected The expected TokenType
 * @param found The found TokenType
 */
void Interpreter::token_error(TokenType expected, TokenType found) {
    ParseError::raise(ERR_UNEXPECTED_TOKEN, "Unexpected Token: Expected " + util::token_names[expected]
                                   + ", found: " + util::token_names[found]
                                   + " on line " + std::to_string(tokenizer->current_line) + ":"
                                   + std::to_string(tokenizer->index_on_line));
}


TypeSpecifier Interpreter::parse_type() {
    TypeSpecifier ts;
    tokenizer->advance(T_TYPE);
    ts.type = util::typemap[tokenizer->current_token.get_value().get<std::string>()];

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

    return ts;
}

RefExpr::SharedPtr Interpreter::parse_identifier() {
    RefExpr::SharedPtr ref_expr = __parse_identifier();

    // check if lvalue
    Token next = tokenizer->lookahead();
    if (next.get_type() == T_OP && next.get_value().get<std::string>() == "=") {
        ref_expr->return_ref(true);
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
RefExpr::SharedPtr Interpreter::__parse_identifier() {
    Token _next = tokenizer->lookahead();

    // identifier
    if (_next.get_type() == T_IDENT) {
        tokenizer->advance(T_IDENT);

        std::string ident = tokenizer->current_token.get_value().get<std::string>();
        IdentifierRefExpr::SharedPtr ident_expr = std::make_shared<IdentifierRefExpr>(ident);

        ident_expr->set_member_expr(__parse_identifier());
        return ident_expr;
    }

    // function call
    if (_next.is_punctuator(C_OPEN_PAREN)) {
        CallExpr::SharedPtr call = parse_function_call();

        call->set_member_expr(__parse_identifier());
        return call;
    }

    // member access
    if (_next.is_punctuator(C_DOT)) {
        tokenizer->advance(T_PUNCTUATOR);
        tokenizer->advance(T_IDENT);
        MemberRefExpr::SharedPtr mem_ref = std::make_shared<MemberRefExpr>(tokenizer->current_token.get_value());

        mem_ref->set_member_expr(__parse_identifier());
        return mem_ref;
    }

    // array access
    if (_next.is_punctuator(C_OPEN_SQUARE)) {
        tokenizer->advance();
        Expression::SharedPtr expr = parse_expression();
        ArrayAccessExpr::SharedPtr arr_acc = std::make_shared<ArrayAccessExpr>(expr);

        tokenizer->advance();
        if (!tokenizer->current_token.is_punctuator(C_CLOSE_SQUARE)) {
            token_error();
        }

        arr_acc->set_member_expr(__parse_identifier());
        return arr_acc;
    }

    return {};
}

/**
 * Parses an object literal in the form of
 * {
 *    prop_name: optional_type = value,
 *    prop2 = value2
 * }
 * @return
 */
ObjectLiteral::SharedPtr Interpreter::parse_object_literal() {
    // opening curly brace
    tokenizer->advance(T_PUNCTUATOR);
    if (tokenizer->c_val() != C_OPEN_CURLY) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '{' to begin an object literal");
    }

    ObjectLiteral::SharedPtr obj = std::make_shared<ObjectLiteral>();

    tokenizer->set_flag(TFLAG_IGNORE_NEWLINE);

    Token _next = tokenizer->lookahead();
    if (_next.is_punctuator(C_CLOSE_CURLY)) {
        tokenizer->advance();
        tokenizer->set_flag(TFLAG_NONE);

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
                arr->set_length(ts.length);
            }

            obj->add_prop(ObjectPropExpr(_prop_name, arr, ARRAY_T));
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

    tokenizer->set_flag(TFLAG_NONE);

    return obj;
}

/**
 * Parses an array literal in the form of
 * [val1, val2, val3, ..., valn]
 * @return
 */
ArrayLiteral::SharedPtr Interpreter::parse_array_literal() {
    // opening square brace
    tokenizer->advance(T_PUNCTUATOR);
    if (tokenizer->c_val() != C_OPEN_SQUARE) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '[' to begin an array literal");
    }

    ArrayLiteral::SharedPtr arr = std::make_shared<ArrayLiteral>();

    int el_count = 0;
    while (!tokenizer->current_token.is_punctuator(C_CLOSE_SQUARE)) {
        Expression::SharedPtr expr = parse_expression();
        arr->add_element(expr);

        tokenizer->advance();
        ++el_count;
    }

    arr->set_length(el_count);
    arr->set_root(prog_root);

    return arr;
}

/**
 * Parses an atomic part of an expression, like a single identifier or literal
 * @return
 */
Expression::SharedPtr Interpreter::parse_unary_expr_target() {
    Token next = tokenizer->lookahead();

    if (next.is_punctuator('{')) {
        return parse_object_literal();
    }
    else if (next.is_punctuator('[')) {
        return parse_array_literal();
    }
    else if (next.get_type() == T_IDENT) {
        return parse_identifier();
    }
    else if (next.get_type() == T_LITERAL) {
        tokenizer->advance();

        return std::make_shared<LiteralExpr>(tokenizer->current_token.get_value());
    }
    else {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected identifier or literal, got " + util::token_names[next.get_type()]);
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
Expression::SharedPtr Interpreter::parse_unary_expr(Expression::SharedPtr literal) {
    Token _next = tokenizer->lookahead();

    // prefix unary op
    if (_next.get_type() == T_OP) {
        tokenizer->advance();

        UnaryOperator::SharedPtr unary_op = std::make_shared<UnaryOperator>(tokenizer->s_val(), "prefix");
        unary_op->set_child(parse_unary_expr(literal));

        return unary_op;
    }

    // typecast
    if (_next.is_punctuator(C_OPEN_PAREN)) {
        tokenizer->advance();
        _next = tokenizer->lookahead();
        if (_next.get_type() == T_TYPE) {
            tokenizer->advance();
            ExplicitCastExpr::SharedPtr cast_op = std::make_shared<ExplicitCastExpr>(tokenizer->s_val());

            tokenizer->advance();
            if (!tokenizer->current_token.is_punctuator(C_CLOSE_PAREN)) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ')' after typecast operator");
            }

            cast_op->set_child(parse_unary_expr(literal));
            return cast_op;
        }
        else {
            tokenizer->backtrack();
        }
    }

    if (literal == nullptr) {
        literal = parse_unary_expr_target();
    }
    else {
        literal->set_child(parse_unary_expr_target());
    }

    _next = tokenizer->lookahead();

    // postfix unary op
    if (_next.get_type() == T_OP && (_next.get_value().get<std::string>() == "++" || _next.get_value().get<std::string>() == "--")) {
        tokenizer->advance();
        UnaryOperator::SharedPtr unary_op = std::make_shared<UnaryOperator>(tokenizer->s_val(), "postfix");
        unary_op->set_child(literal);

        return unary_op;
    }

    return literal;
}

/**
 * Recursively parses an arbitrary (semantically correct) combination of literals, operators and
 *  perenthesized expressions
 * @param lhs Left hand side of the current expression
 * @param min_precedence Minimum operator precedence for current expression
 * @return
 */
Expression::SharedPtr Interpreter::parse_expression(Expression::SharedPtr lhs, int min_precedence) {
    Token next = tokenizer->lookahead();

    // use recursion and assert that the next character is a closing parenthesis
    if (next.is_punctuator(C_OPEN_PAREN)) {
        tokenizer->advance();

        // check if typecast
        next = tokenizer->lookahead();
        if (next.get_type() == T_TYPE) {
            tokenizer->backtrack();
        }
        else {
            auto res = parse_expression(lhs, 0);

            next = tokenizer->lookahead();
            if (!next.is_punctuator(C_CLOSE_PAREN)) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ')'");
            }

            tokenizer->advance();
            return res;
        }
    }

    // initial parsing of left hand side argument
    if (lhs == nullptr) {
        lhs = parse_unary_expr();

        next = tokenizer->lookahead();
    }

    // ...while the next operator has a higher precedence than the minimum
    while (next.get_type() == T_OP && util::op_precedence[next.get_value().get<std::string>()] >= min_precedence) {
        std::string op = next.get_value().get<std::string>();
        tokenizer->advance();

        next = tokenizer->lookahead();
        Expression::SharedPtr rhs;

        // parenthesis recursion
        if (next.is_punctuator(C_OPEN_PAREN)) {
            tokenizer->advance();
            rhs = parse_expression({}, 0);

            next = tokenizer->lookahead();
            if (!next.is_punctuator(C_CLOSE_PAREN)) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ')'");
            }

            tokenizer->advance();
        }
        else {
            // right hand side expression
            rhs = parse_unary_expr();
            next = tokenizer->lookahead();
        }

        // continue recursively while a higher precedence operator follows
        while (next.get_type() == T_OP && util::op_precedence[next.get_value().get<std::string>()] > util::op_precedence[op]) {
            rhs = parse_expression(rhs, util::op_precedence[next.get_value().get<std::string>()]);
            next = tokenizer->lookahead();
        }

        // return the expression so far
        BinaryOperator::SharedPtr binary_op = std::make_shared<BinaryOperator>(op);
        binary_op->set_fst_child(lhs);
        binary_op->set_snd_child(rhs);

        lhs = binary_op;
    }

    return lhs;
}

/**
 * Parses a variable (re-) assigment, e.g.
 *  let x = 3, y = 4;
 *  x.num = 18;
 * @param reassign
 * @return
 */
Statement::SharedPtr Interpreter::parse_assignment(bool auto_type, CompoundStmt::SharedPtr cmp_stmt) {

    // type declaration
    ValueType _type = AUTO_T;
    TypeSpecifier ts;
    if (!auto_type) {
        ts = parse_type();
        _type = ts.type;
    }

    // identifier for assignment
    tokenizer->advance(T_IDENT);
    std::string _ident = tokenizer->current_token.get_value().get<std::string>();

    // equals sign
    tokenizer->advance(T_OP);
    if (tokenizer->current_token.get_value().get<std::string>() != "=") {
        token_error();
    }

    DeclStmt::SharedPtr decl_stmt;
    if (ts.is_array) {
        ArrayLiteral::SharedPtr arr = parse_array_literal();
        arr->set_type(ts.type);
        if (ts.is_var_length) {
            arr->is_var_length(true);
        }
        else {
            arr->set_length(ts.length);
        }

        decl_stmt = std::make_shared<DeclStmt>(_ident, arr, ARRAY_T);
    }
    else {
        Expression::SharedPtr expr = parse_expression();
        decl_stmt = std::make_shared<DeclStmt>(_ident, expr, _type);
    }

    if (cmp_stmt != nullptr) {
        cmp_stmt->add_statement(decl_stmt);
    }

    // multiple declarations
    Token _next = tokenizer->lookahead();
    if (_next.is_punctuator(C_COMMA)) {
        tokenizer->advance();
        while (tokenizer->current_token.get_type() == T_PUNCTUATOR) {
            tokenizer->advance();
        }
        tokenizer->backtrack();

        if (cmp_stmt == nullptr) {
            cmp_stmt = std::make_shared<CompoundStmt>();
            cmp_stmt->add_statement(decl_stmt);
        }

        return parse_assignment(auto_type, cmp_stmt);
    }
    else if (cmp_stmt == nullptr) {
        return decl_stmt;
    }

    return cmp_stmt;
}

/**
 * Parses a function declaration in the form of
 *  def func(x: number, y: string): bool {
 *      ... statements
 *  }
 */
FunctionDecl::SharedPtr Interpreter::parse_function_decl() {
    // function name
    tokenizer->advance(T_IDENT);

    FunctionDecl::SharedPtr fun_dec = std::make_shared<FunctionDecl>(tokenizer->current_token.get_value().get<std::string>());

    tokenizer->advance(T_PUNCTUATOR);
    if (!(tokenizer->current_token.is_punctuator('('))) {
        tokenizer->backtrack_c(1);
        ParseError::raise(ERR_UNEXPECTED_CHARACTER, "Unexpected character" + std::string(1, tokenizer->get_next_char()));
    }

    bool def_arg = false;

    // argument list
    parse_func_arg:
    tokenizer->advance();
    if (tokenizer->current_token.get_type() == T_TYPE) {
        FuncArgDecl::SharedPtr arg_dec = std::make_shared<FuncArgDecl>();

        // type declaration
        tokenizer->backtrack();
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
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Default values are only allowed at the end of a function declaration.");
        }

        fun_dec->add_arg(arg_dec);

        // end of argument list or next argument
        if (_next.is_punctuator(C_COMMA)) {
            tokenizer->advance();
            goto parse_func_arg;
        } else if (!_next.is_punctuator(C_CLOSE_PAREN)) {
            ParseError::raise(ERR_UNEXPECTED_CHARACTER, "Expected closing parenthesis after function definition.");
        }

        tokenizer->advance();
    }

    // optional return type
    Token _next = tokenizer->lookahead();
    ValueType _type = VOID_T;
    if (_next.is_punctuator(C_EQUALS)) {
        tokenizer->advance(T_PUNCTUATOR);

        _next = tokenizer->lookahead();
        if (_next.get_value().get<std::string>() != ">") {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '=>' after function argument list");
        }

        tokenizer->advance();
        TypeSpecifier ts = parse_type();

        _type = ts.type;
    }

    fun_dec->set_return_type(_type);

    // function body
    std::string body = tokenizer->get_next_block();
    Interpreter _int(body);
    CompoundStmt::SharedPtr func_body = _int.parse();
    func_body->set_root(prog_root, false);

    fun_dec->set_body(func_body);

    return fun_dec;
}

IfStmt::SharedPtr Interpreter::parse_if_stmt() {
    tokenizer->set_flag(TFLAG_IGNORE_NEWLINE);

    Token next = tokenizer->lookahead();
    if (!next.is_punctuator(C_OPEN_PAREN)) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '(' to begin if expression");
    }

    Expression::SharedPtr if_cond = parse_expression();

    tokenizer->advance();
    Statement::SharedPtr if_branch = parse_next_stmt();

    IfStmt::SharedPtr if_stmt = std::make_shared<IfStmt>(if_cond, if_branch);

    next = tokenizer->lookahead();
    if (next.get_type() == T_KEYWORD && next.get_value().get<std::string>() == "else") {
        tokenizer->advance();
        tokenizer->advance();

        Statement::SharedPtr else_branch = parse_next_stmt();
        if_stmt->set_else_branch(else_branch);
    }

    tokenizer->set_flag(TFLAG_NONE);

    return if_stmt;
}

/**
 * Interprets a keyword statement
 */
AstNode::SharedPtr Interpreter::parse_keyword() {
    std::string keyword = tokenizer->current_token.get_value().get<std::string>();

    if (keyword == "def") {
        FunctionDecl::SharedPtr fun_dec = parse_function_decl();
        fun_dec->set_root(prog_root, false);

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

        InputStmt::SharedPtr in_stmt(new InputStmt(_ident, _type));
        in_stmt->set_root(prog_root, true);

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
        out_stmt->set_root(prog_root, true);

        return out_stmt;
    }
    else if (keyword == "if") {
        IfStmt::SharedPtr if_stmt = parse_if_stmt();

        return if_stmt;
    }
    else if (keyword == "return") {
        Expression::SharedPtr expr = parse_expression();
        ReturnStmt::SharedPtr return_stmt = std::make_shared<ReturnStmt>(expr);
        return_stmt->set_root(prog_root, true);

        return return_stmt;
    }
}

/**
 * Parses a function call in the form func(arg1, arg2, ...)
 * @return
 */
CallExpr::SharedPtr Interpreter::parse_function_call() {
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
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected closing parenthesis after function call.");
        }
    }

    tokenizer->advance();

    return call;
}

CompoundStmt::SharedPtr Interpreter::parse_block() {
    std::string block = tokenizer->get_next_block();
    Interpreter interp(block);
    CompoundStmt::SharedPtr cmp_stmt = interp.parse();
    cmp_stmt->set_root(prog_root, false);

    return cmp_stmt;
}

Statement::SharedPtr Interpreter::parse_next_stmt() {
    if (tokenizer->current_token.is_punctuator('{')) {
        tokenizer->backtrack();
        CompoundStmt::SharedPtr cmp_stmt = parse_block();

        return cmp_stmt;
    }
    else if (tokenizer->current_token.get_type() == T_TYPE) {
        tokenizer->backtrack();
        Statement::SharedPtr assign = parse_assignment(false);

        assign->set_root(prog_root, true);

        return assign;
    }
    else if (tokenizer->current_token.get_type() == T_KEYWORD) {
        return parse_keyword();
    }
    else {
        tokenizer->backtrack();
        Expression::SharedPtr expr = parse_expression();

        expr->set_root(prog_root, true);
        return expr;
    }
}


/**
 * Runs the program by tokenizing the program, creating the AST and finally evaluating it.
 * @return
 */
void Interpreter::run(bool debug = false) {
    parse();

    if (debug) {
        prog_root->__dump(0);
        std::cout << std::endl << std::endl;
    }

    prog_root->evaluate();
}

/**
 * Parses the program into an AST
 * @return
 */
CompoundStmt::SharedPtr Interpreter::parse() {
    while(tokenizer->current_token.get_type() != T_EOF) {
        tokenizer->advance();
        while (tokenizer->current_token.is_punctuator('\n')) {
            tokenizer->advance();
        }

        if (tokenizer->current_token.get_type() == T_EOF) {
            break;
        }

        prog_root->add_statement(parse_next_stmt());
    }

    return prog_root;
}