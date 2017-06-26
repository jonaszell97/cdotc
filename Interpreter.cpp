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
#include "StdLib/Objects/Object.h"
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
#include "AST/Visitor/Visitor.h"
#include "AST/Statement/ControlFlow/ContinueStmt.h"
#include "AST/Statement/ControlFlow/BreakStmt.h"
#include "AST/Expression/RefExpr/MethodCallExpr.h"
#include "StdLib/GlobalContext.h"
#include "AST/Visitor/ContextVisitor.h"
#include "AST/Visitor/EvaluatingVisitor.h"

/**
 * Creates a new interpreter for an Xtreme Jonas Script program.
 * @param program
 */
Interpreter::Interpreter(std::string program) :
    tokenizer(std::make_unique<Tokenizer>(program)),
    prog_root(std::make_shared<CompoundStmt>())
{
    prog_root->terminable(false);
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


/**
 * Parses a type specifier, like "int", "bool[3]", "any[18 * x]"
 * @return
 */
TypeSpecifier Interpreter::parse_type() {
    TypeSpecifier ts;
    tokenizer->advance();
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
RefExpr::SharedPtr Interpreter::parse_identifier() {
    RefExpr::SharedPtr ref_expr = __parse_identifier();

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
RefExpr::SharedPtr Interpreter::__parse_identifier() {
    Token _next = tokenizer->lookahead();

    // identifier
    if (_next.get_type() == T_IDENT) {
        tokenizer->advance(T_IDENT);
        std::string ident = tokenizer->s_val();

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
        std::string ident = tokenizer->s_val();

        // method call
        _next = tokenizer->lookahead();
        if (_next.is_punctuator(C_OPEN_PAREN)) {
            CallExpr::SharedPtr call = parse_function_call();
            MethodCallExpr::SharedPtr method_call = std::make_shared<MethodCallExpr>(*call, ident);

            return method_call;
        }

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
 *    type prop_name = value
 *    type2 name2 = value2
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
    else if (next.is_punctuator(C_OPEN_PAREN)) {
        return parse_expression();
    }
    else if (next.get_type() == T_IDENT) {
        return parse_identifier();
    }
    else if (next.get_type() == T_LITERAL) {
        tokenizer->advance();

        return std::make_shared<LiteralExpr>(tokenizer->current_token.get_value());
    }
    else {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected identifier or literal, got "
                                                + util::token_names[next.get_type()]);
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
Expression::SharedPtr Interpreter::parse_unary_expr(Expression::SharedPtr literal, bool postfix) {
    Token _next = tokenizer->lookahead();

    // prefix unary op
    if (_next.get_type() == T_OP && !postfix
            && util::in_vector<std::string>(util::unary_operators, _next.get_value().get<std::string>()))
    {
        tokenizer->advance();

        UnaryOperator::SharedPtr unary_op = std::make_shared<UnaryOperator>(tokenizer->s_val(), "prefix");

        unary_op->set_child(parse_unary_expr(literal, postfix));

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
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ')' after typecast operator");
            }

            cast_op->set_child(parse_unary_expr(literal, postfix));

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

    _next = tokenizer->lookahead();

    // postfix unary op
    if (_next.get_type() == T_OP && (_next.get_value().get<std::string>() == "++"
         || _next.get_value().get<std::string>() == "--"))
    {
        tokenizer->advance();
        UnaryOperator::SharedPtr unary_op = std::make_shared<UnaryOperator>(tokenizer->s_val(), "postfix");

        unary_op->set_child(literal);

        return unary_op;
    }

    return literal;
}

TertiaryOperator::SharedPtr Interpreter::parse_tertiary_operator(Expression::SharedPtr cond) {
    Expression::SharedPtr if_branch = parse_expression();
    tokenizer->advance();
    Expression::SharedPtr else_branch = parse_expression();

    return std::make_shared<TertiaryOperator>(cond, if_branch, else_branch);
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

            // possible unary operator
            return parse_unary_expr(res, true);
        }
    }

    // initial parsing of left hand side argument
    if (lhs == nullptr) {
        lhs = parse_unary_expr();

        next = tokenizer->lookahead();
    }

    // tertiary operator
    if (next.get_type() == T_OP && next.get_value().get<std::string>() == "?") {
        tokenizer->advance();
        return parse_tertiary_operator(lhs);
    }
    else if (next.get_type() == T_OP && next.get_value().get<std::string>() == ":") {
        return lhs;
    }

    // ...while the next operator has a higher precedence than the minimum
    while (next.get_type() == T_OP &&
            util::in_vector(util::binary_operators, next.get_value().get<std::string>()) &&
            util::op_precedence[next.get_value().get<std::string>()] >= min_precedence)
    {
        std::string op = next.get_value().get<std::string>();
        tokenizer->advance();

        next = tokenizer->lookahead();
        Expression::SharedPtr rhs;

        // parenthesis recursion
        if (next.is_punctuator(C_OPEN_PAREN)) {
            tokenizer->advance();
            next = tokenizer->lookahead();

            // typecast
            if (next.get_type() == T_TYPE) {
                tokenizer->backtrack();
                rhs = parse_unary_expr();
            }
            else {
                rhs = parse_expression({}, 0);

                next = tokenizer->lookahead();
                if (!next.is_punctuator(C_CLOSE_PAREN)) {
                    ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ')'");
                }

                tokenizer->advance();

                rhs = parse_unary_expr(rhs, true);
            }
        }
        else {
            // right hand side expression
            rhs = parse_unary_expr();
            next = tokenizer->lookahead();
        }

        // continue recursively while a higher precedence operator follows
        while (next.get_type() == T_OP &&
                util::op_precedence[next.get_value().get<std::string>()] > util::op_precedence[op])
        {
            rhs = parse_expression(rhs, util::op_precedence[next.get_value().get<std::string>()]);
            next = tokenizer->lookahead();
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
        tokenizer->advance();
        return parse_tertiary_operator(lhs);
    }
    else if (next.get_type() == T_OP && next.get_value().get<std::string>() == ":" &&
            util::op_precedence[":"] >= min_precedence) {
        return lhs;
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

    DeclStmt::SharedPtr decl_stmt;

    // equals sign
    tokenizer->advance();
    if (tokenizer->current_token.get_type() != T_OP || tokenizer->current_token.get_value().get<std::string>() != "=") {
       // only declaration with no value
        if (!ts.nullable) {
            ParseError::raise(ERR_UNINITIALIZED_VAR, "Non-nullable variable " + _ident + " must be initialized");
        }

        return std::make_shared<DeclStmt>(_ident, ts);
    }

    if (ts.is_array) {
        ArrayLiteral::SharedPtr arr = parse_array_literal();
        arr->set_type(ts.type);
        if (ts.is_var_length) {
            arr->is_var_length(true);
        }
        else {
            arr->set_length(ts.length);
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
 *  def func(x: number, y: string) => bool {
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
    if (_next.get_type() == T_OP && _next.get_value().get<std::string>() == "=") {
        tokenizer->advance();

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

    func_body->terminable(true);
    fun_dec->set_body(func_body);

    return fun_dec;
}

IfStmt::SharedPtr Interpreter::parse_if_stmt() {
    tokenizer->set_flag(TFLAG_IGNORE_NEWLINE);

    Token next = tokenizer->lookahead();
    if (!next.is_punctuator(C_OPEN_PAREN)) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '(' to begin if statement");
    }

    tokenizer->advance();

    Expression::SharedPtr if_cond = parse_expression();

    tokenizer->advance(); // last part of expression
    tokenizer->advance(); // closing parenthesis

    CompoundStmt::SharedPtr if_branch = std::static_pointer_cast<CompoundStmt>(parse_next_stmt(true));
    if_branch->terminable(false);

    IfStmt::SharedPtr if_stmt = std::make_shared<IfStmt>(if_cond, if_branch);

    next = tokenizer->lookahead();
    if (next.get_type() == T_KEYWORD && next.get_value().get<std::string>() == "else") {
        tokenizer->advance();
        tokenizer->advance();

        CompoundStmt::SharedPtr else_branch = std::static_pointer_cast<CompoundStmt>(parse_next_stmt(true));
        else_branch->terminable(false);
                        if_stmt->set_else_branch(else_branch);
    }

    tokenizer->set_flag(TFLAG_NONE);

    return if_stmt;
}

WhileStmt::SharedPtr Interpreter::parse_while_stmt() {
    tokenizer->set_flag(TFLAG_IGNORE_NEWLINE);

    Token next = tokenizer->lookahead();
    if (!next.is_punctuator(C_OPEN_PAREN)) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '(' after while keyword");
    }

    Expression::SharedPtr while_cond = parse_expression();

    tokenizer->advance();
    CompoundStmt::SharedPtr while_block = std::static_pointer_cast<CompoundStmt>(parse_next_stmt(true));
    while_block->terminable(false);
    while_block->continuable(true);


    WhileStmt::SharedPtr while_stmt = std::make_shared<WhileStmt>(while_cond, while_block);

    tokenizer->set_flag(TFLAG_NONE);

    return while_stmt;
}

ForStmt::SharedPtr Interpreter::parse_for_stmt() {
    Token next = tokenizer->lookahead();
    if (!next.is_punctuator(C_OPEN_PAREN)) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '(' after for keyword");
    }

    tokenizer->advance();
    tokenizer->advance();

    Statement::SharedPtr init = parse_next_stmt();
    tokenizer->advance();

    if (!tokenizer->current_token.is_punctuator(C_SEMICOLON)) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ';' to seperate for loop arguments");
    }

    tokenizer->advance();
    Statement::SharedPtr term = parse_next_stmt();
    tokenizer->advance();
    if (!tokenizer->current_token.is_punctuator(C_SEMICOLON)) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ';' to seperate for loop arguments");
    }

    tokenizer->advance();
    Statement::SharedPtr inc = parse_next_stmt();
    tokenizer->advance();
    if (!tokenizer->current_token.is_punctuator(C_CLOSE_PAREN)) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ';' to seperate for loop arguments");
    }

    tokenizer->advance();

    CompoundStmt::SharedPtr block = std::static_pointer_cast<CompoundStmt>(parse_next_stmt(true));
    block->terminable(false);
    block->continuable(true);

    ForStmt::SharedPtr for_stmt = std::make_shared<ForStmt>(init, term, inc);
    for_stmt->set_body(block);

    return for_stmt;
}

/**
 * Interprets a keyword statement
 */
AstNode::SharedPtr Interpreter::parse_keyword() {
    std::string keyword = tokenizer->current_token.get_value().get<std::string>();

    if (keyword == "def") {
        FunctionDecl::SharedPtr fun_dec = parse_function_decl();

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

        return out_stmt;
    }
    else if (keyword == "if") {
        IfStmt::SharedPtr if_stmt = parse_if_stmt();

        return if_stmt;
    }
    else if (keyword == "while") {
        WhileStmt::SharedPtr while_stmt = parse_while_stmt();

        return while_stmt;
    }
    else if (keyword == "for") {
        ForStmt::SharedPtr for_stmt = parse_for_stmt();

        return for_stmt;
    }
    else if (keyword == "continue") {
        ContinueStmt::SharedPtr cont_stmt = std::make_shared<ContinueStmt>();

        return cont_stmt;
    }
    else if (keyword == "break") {
        BreakStmt::SharedPtr break_stmt = std::make_shared<BreakStmt>();

        return break_stmt;
    }
    else if (keyword == "return") {
        Expression::SharedPtr expr = parse_expression();

        ReturnStmt::SharedPtr return_stmt = std::make_shared<ReturnStmt>(expr);

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


    return cmp_stmt;
}

Statement::SharedPtr Interpreter::parse_next_stmt(bool force_cmpnd) {
    if (tokenizer->current_token.is_punctuator('{')) {
        tokenizer->backtrack();
        CompoundStmt::SharedPtr cmp_stmt = parse_block();

        return cmp_stmt;
    }
    else if (tokenizer->current_token.get_type() == T_TYPE) {
        tokenizer->backtrack();
        Statement::SharedPtr assign = parse_assignment(false);
        if (force_cmpnd) {
            CompoundStmt::SharedPtr cmpnd = std::make_shared<CompoundStmt>();
            cmpnd->add_statement(assign);

            return cmpnd;
        }

        return assign;
    }
    else if (tokenizer->current_token.get_type() == T_KEYWORD) {
        AstNode::SharedPtr expr = parse_keyword();
        if (force_cmpnd) {
            CompoundStmt::SharedPtr cmpnd = std::make_shared<CompoundStmt>();
            cmpnd->add_statement(expr);

            return cmpnd;
        }

        return expr;
    }
    else {
        tokenizer->backtrack();
        Expression::SharedPtr expr = parse_expression();
        if (force_cmpnd) {
            CompoundStmt::SharedPtr cmpnd = std::make_shared<CompoundStmt>();
            cmpnd->add_statement(expr);

            return cmpnd;
        }

        return expr;
    }
}


/**
 * Runs the program by tokenizing the program, creating the AST and finally evaluating it.
 * @return
 */
void Interpreter::run(bool debug = false) {
    parse();

    GlobalContext::init();
    ContextVisitor v;
    v.visit(prog_root.get());

    if (debug) {
        prog_root->__dump(0);
        std::cout << std::endl << std::endl;
    }

    EvaluatingVisitor ev;
    ev.visit(prog_root.get());
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

        Statement::SharedPtr stmt = parse_next_stmt(false);

        prog_root->add_statement(stmt);
    }

    return prog_root;
}