//
// Created by Jonas Zell on 13.06.17.
//

#include <string>
#include "Parser.h"
#include "Variant/Variant.h"
#include <vector>
#include "Util.h"
#include <iostream>
#include <fstream>
#include "Message/Exceptions.h"
#include "AST/Expression/RefExpr/IdentifierRefExpr.h"
#include "AST/Operator/UnaryOperator.h"
#include "AST/Expression/Literal/LiteralExpr.h"
#include "AST/Expression/Literal/StringLiteral.h"
#include "AST/Operator/BinaryOperator.h"
#include "AST/Statement/Declaration/DeclStmt.h"
#include "AST/Statement/Block/CompoundStmt.h"
#include "AST/Statement/Declaration/FunctionDecl.h"
#include "AST/Expression/RefExpr/CallExpr.h"
#include "AST/Expression/RefExpr/MemberRefExpr.h"
#include "AST/Expression/RefExpr/ArrayAccessExpr.h"
#include "AST/Statement/ControlFlow/ReturnStmt.h"
#include "AST/Operator/ExplicitCastExpr.h"
#include "AST/Visitor/Visitor.h"
#include "AST/Statement/ControlFlow/ContinueStmt.h"
#include "AST/Statement/ControlFlow/BreakStmt.h"
#include "AST/Visitor/CaptureVisitor.h"
#include "AST/Visitor/EvaluatingVisitor.h"
#include "AST/Statement/ControlFlow/GotoStmt.h"
#include "AST/Expression/Literal/LambdaExpr.h"
#include "AST/Statement/Declaration/NamespaceDecl.h"
#include "AST/Statement/ImportStmt.h"
#include "AST/Statement/ExportStmt.h"
#include "AST/Visitor/StaticAnalysis/TypeCheckVisitor.h"
#include "AST/Namespace.h"
#include "AST/Visitor/CodeGen/CodeGenVisitor.h"
#include "Message/Warning.h"
#include "AST/Statement/Declaration/TypedefDecl.h"

std::vector<string> Parser::type_names = {};
std::vector<string> Parser::infix_functions = {};
std::vector<string> Parser::namespaces = {};
std::vector<string> Parser::source_files = {};
std::unordered_map<string, TypeSpecifier> Parser::current_generics = {};
bool Parser::lib_imports_disabled = false;

/**
 * Creates a new interpreter for an Xtreme Jonas Script program.
 * @param program
 */
Parser::Parser(std::string program) :
    lexer(new Lexer(program)),
    source_id(source_files.size())
{
    source_files.push_back(program);
}


/**
 * Parses a type specifier, like "int", "bool[3]", "any[18 * x]"
 * @return
 */
TypeRef::SharedPtr Parser::parse_type(bool allow_vararg) {

    int start = lexer->last_token_index;
    TypeSpecifier ts;
    auto attributes = parse_attributes();
    bool var_arg = false;

    if (lexer->current_token.is_operator("...")) {
        if (!allow_vararg) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Vararg not allowed here", lexer);
        }

        var_arg = true;
        lexer->advance();
    }

    ts.is_vararg = var_arg;

    // array type
    if (lexer->current_token.is_punctuator('[')) {
        lexer->advance();
        ts.type = OBJECT_T;
        ts.class_name = "Array";
        ts.is_primitive = false;
        ts.element_type = new TypeSpecifier(parse_type()->getTypeSpecifier());
        ts.is_generic = ts.element_type->is_generic;

        if (!lexer->lookahead().is_punctuator(']')) {
            ts.arr_length = parse_expression();
        }

        if (!lexer->lookahead().is_punctuator(']')) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ']' after array type", lexer);
        }

        lexer->advance();

        auto typeref = std::make_shared<TypeRef>(ts);
        typeref->set_index(start, lexer->current_index, source_id);
        typeref->set_attributes(attributes);

        return typeref;
    }

    // function type
    if (lexer->current_token.is_punctuator('(')) {
        lexer->advance();
        while (!lexer->current_token.is_punctuator(')')) {
            ts.args.push_back(parse_type()->getTypeSpecifier());
            ts.is_generic = ts.args.back().is_generic;
            lexer->advance();
            if (lexer->current_token.is_punctuator(',')) {
                lexer->advance();
            }
            else if (!lexer->current_token.is_punctuator(')')) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ')' after function arguments", lexer);
            }
        }

        lexer->advance();
        if (!lexer->current_token.is_operator("->")) {
            if (ts.args.size() == 1) {
                lexer->backtrack();
                auto typeref = std::make_shared<TypeRef>(ts.args[0]);
                typeref->set_index(start, lexer->current_index, source_id);
                typeref->set_attributes(attributes);

                return typeref;
            }

            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '->' after function argument list", lexer);
        }

        ts.is_function = true;
        ts.type = OBJECT_T;

        lexer->advance();
        ts.return_type = new TypeSpecifier(parse_type()->getTypeSpecifier());
        ts.is_generic = ts.is_generic || ts.return_type->is_generic;

        auto typeref = std::make_shared<TypeRef>(ts);
        typeref->set_index(start, lexer->current_index, source_id);
        typeref->set_attributes(attributes);

        return typeref;
    }

    string type = lexer->s_val();

    Token _next = lexer->lookahead();
    if (_next.is_punctuator('.')) {
        lexer->advance();
        lexer->advance();
        auto ts = parse_type();
        ts->getTypeSpecifier().ns_name.insert(ts->getTypeSpecifier().ns_name.begin(), type);
        ts->set_attributes(attributes);
        ts->getTypeSpecifier().is_vararg = var_arg;

        return ts;
    }
    else {
        if (util::typemap.find(type) != util::typemap.end()) {
            ts.type = util::typemap[type];
            ts.is_primitive = true;
            ts.class_name = type;

            if (type.substr(0, 4) == "UInt") {
                ts.is_unsigned = true;
                type = type.substr(1);
            }

            if (type.substr(0, 3) == "Int") {
                std::string bitwidth;
                while (type.back() >= '0' && type.back() <= '9') {
                    bitwidth = std::string(1, type.back()) + bitwidth;
                    type = type.substr(0, type.length() - 1);
                }

                ts.bitwidth = bitwidth == "" ? 32 : std::stoi(bitwidth);
            }
        }
        else {
            ts.type = OBJECT_T;
            ts.class_name = type;
            ts.is_primitive = false;

            if (current_generics.find(type) != current_generics.end()) {
                ts.is_generic = true;
                ts.generic_class_name = type;
            }
        }
    }

    // nullable type
    if (_next.is_operator("?")) {
        ts.nullable = true;
        lexer->advance();
    }
    // pointer type
    else if (_next.is_operator("*")) {
        ts.is_pointer = true;
        lexer->advance();
    }
    // reference
    else if (_next.is_operator("&")) {
        ts.is_reference = true;
        lexer->advance();
    }

    auto typeref = std::make_shared<TypeRef>(ts);
    typeref->set_index(start, lexer->current_index, source_id);
    typeref->set_attributes(attributes);

    return typeref;
}

/**
 * Parses an identifier (wrapper function for lvalue check)
 * @return
 */
Expression::SharedPtr Parser::parse_identifier() {
    int start = lexer->last_token_index;
    Expression::SharedPtr ref_expr = __parse_identifier(true);
    ref_expr->set_index(start, lexer->current_index, source_id);

    // check if lvalue
    Token next = lexer->lookahead();
    if (next.is_operator("=") || next.is_operator("+=") || next.is_operator("-=") || next.is_operator("*=") ||
            next.is_operator("/=")) {
        ref_expr->is_lvalue();
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
Expression::SharedPtr Parser::__parse_identifier(bool initial) {
    Token _next = lexer->lookahead(false);
    int start = lexer->last_token_index;

    // identifier
    if (_next.get_type() == T_IDENT && initial) {
        lexer->advance(false);
        std::string ident = lexer->s_val();

        IdentifierRefExpr::SharedPtr ident_expr = std::make_shared<IdentifierRefExpr>(ident);
        ident_expr->set_member_expr(__parse_identifier());
        ident_expr->set_index(start, lexer->current_index, source_id);

        return ident_expr;
    }

    // function call
    bool generic_func_call = is_generic_call();
    if (_next.is_punctuator('(') || generic_func_call) {
        CallExpr::SharedPtr call = parse_function_call();
        call->set_member_expr(__parse_identifier());
        call->set_index(start, lexer->current_index, source_id);

        return call;
    }

    // member access
    if (_next.is_punctuator(C_DOT)) {
        lexer->advance(false);
        lexer->advance(false);
        Token ident_t = lexer->current_token;
        std::string ident = lexer->s_val();

        // method call
        _next = lexer->lookahead();
        if (_next.is_punctuator('(') || is_generic_call()) {
            lexer->advance();
            auto calltype = std::find(namespaces.begin(), namespaces.end(), ident) != namespaces.end() ?
                CallType::FUNC_CALL : CallType::METHOD_CALL;

            auto generics = parse_concrete_generics();

            CallExpr::SharedPtr method_call = std::make_shared<CallExpr>(calltype, parse_arguments(), ident);
            method_call->set_member_expr(__parse_identifier());
            method_call->set_index(start, lexer->current_index, source_id);
            method_call->set_generics(generics);

            return method_call;
        }

        start = lexer->last_token_index;
        MemberRefExpr::SharedPtr mem_ref = std::make_shared<MemberRefExpr>(lexer->current_token.get_value());
        mem_ref->set_member_expr(__parse_identifier());
        mem_ref->set_index(start, lexer->current_index, source_id);

        return mem_ref;
    }

    // array access
    if (_next.is_punctuator(C_OPEN_SQUARE)) {
        lexer->advance(false);
        Expression::SharedPtr expr = parse_expression();
        expr->set_index(start, lexer->current_index, source_id);

        ArrayAccessExpr::SharedPtr arr_acc = std::make_shared<ArrayAccessExpr>(expr);

        lexer->advance();
        if (!lexer->current_token.is_punctuator(C_CLOSE_SQUARE)) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ']'", lexer);
        }

        arr_acc->set_member_expr(__parse_identifier());
        arr_acc->set_index(start, lexer->current_index, source_id);

        return arr_acc;
    }

    return {};
}

/**
 * Parses an array literal in the form of
 * [val1, val2, val3, ..., valn]
 * @return
 */
ArrayLiteral::SharedPtr Parser::parse_array_literal() {
    int start = lexer->last_token_index;

    lexer->advance();
    if (!lexer->current_token.is_punctuator('[')) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '[' to begin an array literal", lexer);
    }

    TypeSpecifier type;
    type.type = AUTO_T;
    ArrayLiteral::SharedPtr arr = std::make_shared<ArrayLiteral>(type);

    int el_count = 0;
    bool var_length = false;
    while (!lexer->current_token.is_punctuator(']')) {
        if (lexer->lookahead().is_punctuator(']')) {
            lexer->advance();
            break;
        }

        Expression::SharedPtr expr = parse_expression();
        arr->add_element(expr);

        lexer->advance();
        ++el_count;
    }

    if (!lexer->current_token.is_punctuator(']')) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ']' after array literal", lexer);
    }

    arr->set_length(var_length ? -1 : el_count);
    arr->set_index(start, lexer->current_index, source_id);

    return arr;
}

/**
 * Parses an atomic part of an expression, like a single identifier or literal
 * @return
 */
Expression::SharedPtr Parser::parse_unary_expr_target() {
    Token next = lexer->lookahead(false);
    std::vector<string> attributes;

    if (next.is_punctuator('@')) {
        lexer->advance();
        attributes = parse_attributes();
        lexer->backtrack();
        next = lexer->lookahead();
    }

    if (next.is_punctuator('[')) {
        auto arr = parse_array_literal();
        arr->set_attributes(attributes);

        return arr;
    }
    if (next.is_punctuator('(')) {
        lexer->advance();

        auto expr = parse_paren_expr();
        expr->set_attributes(attributes);

        return expr;
    }
    if (next.get_type() == T_IDENT || next.get_type() == T_TYPE) {
        int start = lexer->last_token_index;
        lexer->advance(false);
        next = lexer->lookahead(false);

        // function call
        if (next.is_punctuator('(') || is_generic_call()) {
            std::string func_name = lexer->s_val();
            lexer->advance();

            std::vector<TypeSpecifier> generics = parse_concrete_generics();

            auto args = parse_arguments();

            CallExpr::SharedPtr func_call = std::make_shared<CallExpr>(CallType::FUNC_CALL, args, func_name);
            func_call->set_member_expr(__parse_identifier(false));
            func_call->set_index(start, lexer->current_index, source_id);
            func_call->set_attributes(attributes);
            func_call->set_generics(generics);

            return func_call;
        }

        lexer->backtrack();
        auto ident = parse_identifier();
        ident->set_attributes(attributes);

        return ident;
    }
    if (next.get_type() == T_LITERAL) {
        int start = lexer->last_token_index;
        lexer->advance();

        if (lexer->current_token.get_value().get_type().class_name == "String") {
            Token next = lexer->lookahead(false);

            // possible string modifier
            if (next.get_type() == T_IDENT) {
                std::string modifier = next.get_value().get<std::string>();
                if (util::string_modifiers.find(modifier) != util::string_modifiers.end()) {
                    auto string_literal = std::make_shared<StringLiteral>(lexer->s_val(), modifier[0]);
                    lexer->advance();

                    string_literal->set_member_expr(__parse_identifier());
                    string_literal->set_index(start, lexer->current_index, source_id);
                    string_literal->set_attributes(attributes);

                    return string_literal;
                }
                else {
                    lexer->advance();
                    ParseError::raise(ERR_UNEXPECTED_CHARACTER, "Unknown string modifier " + modifier, lexer);
                }
            }
            else {
                auto string_literal = std::make_shared<StringLiteral>(lexer->s_val());
                string_literal->set_member_expr(__parse_identifier());
                string_literal->set_index(start, lexer->current_index, source_id);
                string_literal->set_attributes(attributes);

                return string_literal;
            }
        }

        auto expr = std::make_shared<LiteralExpr>(lexer->current_token.get_value());
        expr->set_member_expr(__parse_identifier());
        expr->set_index(start, lexer->current_index, source_id);
        expr->set_attributes(attributes);

        return expr;
    }
    else {
        lexer->advance();
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected expression but found " + util::token_names[next.get_type()], lexer);
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
    int start = lexer->last_token_index;
    Token _next = lexer->lookahead(false);

    // prefix unary op
    if (_next.get_type() == T_OP && !postfix
        && util::in_vector<std::string>(util::unary_operators, _next.get_value().get<std::string>()))
    {
        lexer->advance(false);

        UnaryOperator::SharedPtr unary_op = std::make_shared<UnaryOperator>(lexer->s_val(), "prefix");

        unary_op->set_child(parse_unary_expr(literal, postfix));
        unary_op->set_index(start, lexer->current_index, source_id);

        return unary_op;
    }

    if (literal == nullptr) {
        literal = parse_unary_expr_target();
    }
    else if (!postfix) {
        literal->set_child(parse_unary_expr_target());
    }

    _next = lexer->lookahead(false);

    // postfix unary op
    if (_next.is_operator("++") || _next.is_operator("--")) {
        lexer->advance();
        UnaryOperator::SharedPtr unary_op = std::make_shared<UnaryOperator>(lexer->s_val(), "postfix");

        unary_op->set_child(literal);
        unary_op->set_index(start, lexer->current_index, source_id);

        return unary_op;
    }

    // call
    if (_next.is_punctuator('(') || is_generic_call()) {
        lexer->advance();

        std::vector<TypeSpecifier> generics = parse_concrete_generics();

        auto call = std::make_shared<CallExpr>(CallType::FUNC_CALL, parse_arguments());
        call->set_generics(generics);
        literal->set_member_expr(call);
    }

    literal->set_index(start, lexer->current_index, source_id);

    return literal;
}

ExplicitCastExpr::SharedPtr Parser::parse_typecast() {
    int start = lexer->last_token_index;

    lexer->advance(); // (
    auto ts = parse_type()->getTypeSpecifier();
    ExplicitCastExpr::SharedPtr cast_op = std::make_shared<ExplicitCastExpr>(ts);

    lexer->advance(); // type
    if (!lexer->current_token.is_punctuator(C_CLOSE_PAREN)) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ')' after typecast operator", lexer);
    }

    cast_op->set_child(parse_expression({}, util::op_precedence["typecast"]));
    cast_op->set_index(start, lexer->current_index, source_id);

    return cast_op;
}

TertiaryOperator::SharedPtr Parser::parse_tertiary_operator(Expression::SharedPtr cond) {
    int start = lexer->last_token_index;

    Expression::SharedPtr if_branch = parse_expression();
    lexer->advance();
    if (!lexer->current_token.is_operator(":")) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ':' in tertiary expression", lexer);
    }
    Expression::SharedPtr else_branch = parse_expression();

    auto op = std::make_shared<TertiaryOperator>(cond, if_branch, else_branch);
    op->set_index(start, lexer->current_index, source_id);

    return op;
}

ParenExprType Parser::get_paren_expr_type() {
    int start_index = lexer->current_token_index - 1;
    int last_index = lexer->last_token_index - 1;
    Token next = lexer->lookahead();

    if (next.get_type() == T_IDENT && (is_declared_type(next.get_value().get<std::string>()) ||
            util::typemap.find(next.get_value().get<std::string>()) != util::typemap.end())) {
        lexer->advance();
        if (lexer->lookahead().is_punctuator(')')) {
            lexer->current_token_index = start_index;
            lexer->last_token_index = last_index;
            lexer->advance();

            return ParenExprType::TYPECAST;
        }
        lexer->backtrack();
    }

    int open_parens = 1;
    int closed_parens = 0;

    while (open_parens != closed_parens && lexer->current_token.get_type() != T_EOF) {
        lexer->advance();
        if (lexer->current_token.is_punctuator('(')) {
            ++open_parens;
        }
        else if (lexer->current_token.is_punctuator(')')) {
            ++closed_parens;
        }
    }

    if (open_parens != closed_parens) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expression contains unmatched parentheses", lexer);
    }

    next = lexer->lookahead();
    lexer->current_token_index = start_index;
    lexer->last_token_index = last_index;
    lexer->advance();

    // lambda
    if (next.is_operator("=>")) {
        return ParenExprType::LAMBDA;
    }

    return ParenExprType::EXPR;
}

bool Parser::is_generic_call() {
    int start_index = lexer->current_token_index - 1;
    int last_index = lexer->last_token_index - 1;

    Token next = lexer->lookahead();
    if (!next.is_operator("<")) {
        return false;
    }

    lexer->advance();
    lexer->advance();
    while (!lexer->current_token.is_operator(">")) {
        if (lexer->current_token.get_type() == T_IDENT && is_declared_type(lexer->s_val())) {
            lexer->advance();
            continue;
        }
        if (lexer->current_token.is_punctuator(',')) {
            lexer->advance();
            continue;
        }

        lexer->current_token_index = start_index;
        lexer->last_token_index = last_index;
        lexer->advance();

        return false;
    }

    lexer->current_token_index = start_index;
    lexer->last_token_index = last_index;
    lexer->advance();

    return true;
}

Expression::SharedPtr Parser::parse_paren_expr() {
    ParenExprType type = get_paren_expr_type();
    Expression::SharedPtr expr;
    switch (type) {
        case ParenExprType::LAMBDA:
            expr = parse_lambda_expr();
            break;
        case ParenExprType::EXPR:
            expr = parse_expression();
            lexer->advance(); // last part of expr
            if (!lexer->current_token.is_punctuator(')')) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ')'", lexer);
            }
            break;
        case ParenExprType::TYPECAST:
            expr = parse_typecast();
            break;
    }


    return expr;
}

/**
 * Recursively parses an arbitrary (semantically correct) combination of literals, operators and
 *  perenthesized expressions
 * @param lhs Left hand side of the current expression
 * @param min_precedence Minimum operator precedence for current expression
 * @return
 */
Expression::SharedPtr Parser::parse_expression(Expression::SharedPtr lhs, int min_precedence) {
    int start = lexer->current_index;
    Token next = lexer->lookahead(false);

    // initial parsing of left hand side argument
    if (lhs == nullptr) {
        lhs = parse_unary_expr();

        next = lexer->lookahead(false);
    }

    // tertiary operator
    if (next.get_type() == T_OP && next.get_value().get<std::string>() == "?") {
        lexer->advance(false);
        auto op = parse_tertiary_operator(lhs);
        op->set_index(start, lexer->current_index, source_id);

        return op;
    }
    else if (next.get_type() == T_OP && next.get_value().get<std::string>() == ":") {
        lhs->set_index(start, lexer->current_index, source_id);

        return lhs;
    }

    // ...while the next operator has a higher precedence than the minimum
    while (((next.get_type() == T_OP && util::in_vector(util::binary_operators, next.get_value().get<std::string>())
            && util::op_precedence[next.get_value().get<std::string>()] >= min_precedence) ||
        (next.get_type() == T_IDENT && is_infix_function(next.get_value().get<std::string>()))
            && util::op_precedence["infix"] >= min_precedence)
    )
    {
        std::string op = next.get_value().get<std::string>();
        lexer->advance(false);

        next = lexer->lookahead(false);
        Expression::SharedPtr rhs;

        // right hand side expression
        rhs = parse_unary_expr();
        next = lexer->lookahead(false);

        // continue recursively while a higher precedence operator follows
        while (((next.get_type() == T_OP && util::in_vector(util::binary_operators, next.get_value().get<std::string>())
                && util::op_precedence[next.get_value().get<std::string>()] > util::op_precedence[op]) ||
            (next.get_type() == T_IDENT && is_infix_function(next.get_value().get<std::string>()))
                && util::op_precedence["infix"] > util::op_precedence[op])
        )
        {
            rhs = parse_expression(rhs, util::op_precedence[next.get_value().get<std::string>()]);
            next = lexer->lookahead(false);
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
        lexer->advance(false);
        auto op = parse_tertiary_operator(lhs);
        op->set_index(start, lexer->current_index, source_id);

        return op;
    }

    lhs->set_index(start, lexer->current_index, source_id);

    return lhs;
}

/**
 * Parses a variable (re-) assigment, e.g.
 *  let x = 3, y = 4;
 *  x.num = 18;
 * @param reassign
 * @return
 */
Statement::SharedPtr Parser::parse_assignment(bool is_const) {

    std::vector<DeclStmt::SharedPtr> declarations;

    for (;;) {
        int start = lexer->last_token_index;
        lexer->advance();

        std::string identifier = lexer->s_val();
        Expression::SharedPtr value = nullptr;
        TypeRef::SharedPtr typeref = std::make_shared<TypeRef>(TypeSpecifier(AUTO_T));

        Token next = lexer->lookahead(false);

        if (next.is_operator(":")) {
            lexer->advance();
            lexer->advance();
            typeref = parse_type();
            next = lexer->lookahead();
        }

        auto& ts = typeref->getTypeSpecifier();

        ts.is_const = is_const;

        if (next.is_operator("=")) {
            lexer->advance(false);
            value = parse_expression();
        }
        else if (ts.type == AUTO_T) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Type inferred declarations have to be initialized directly",
                lexer);
        }

        DeclStmt::SharedPtr decl_stmt = std::make_shared<DeclStmt>(identifier, typeref, is_const, top_level, value);
        decl_stmt->set_attributes(attributes);
        decl_stmt->set_index(start, lexer->current_index, source_id);

        if (value != nullptr) {
            value->set_decl(decl_stmt.get());
        }

        declarations.push_back(decl_stmt);

        next = lexer->lookahead();
        if (!next.is_punctuator(',')) {
            break;
        }
    }

    attributes.clear();

    if (declarations.size() == 1) {
        return declarations.front();
    }

    CompoundStmt::SharedPtr comp = std::make_shared<CompoundStmt>(true);
    for (const auto & decl : declarations) {
        comp->add_statement(decl);
    }

    return comp;
}

/**
 * Parses a method's argument list
 * @return
 */
std::vector<FuncArgDecl::SharedPtr> Parser::parse_arg_list(bool no_var_names) {

    int start = lexer->last_token_index;

    std::vector<FuncArgDecl::SharedPtr> args;
    if (!lexer->current_token.is_punctuator('(')) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '(' to start an argument list",
                lexer);
    }

    bool def_arg = false;
    bool var_arg = false;
    lexer->advance();

    while (!lexer->current_token.is_punctuator(')'))
    {
        if (var_arg) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Vararg arguments can only be the last argument of a "
                "function", lexer);
        }

        FuncArgDecl::SharedPtr arg_dec = std::make_shared<FuncArgDecl>();
        std::vector<string> attributes = parse_attributes();
        arg_dec->set_attributes(attributes);

        // identifier
        if (no_var_names) {
            auto type = parse_type(true);
            if (type->getTypeSpecifier().is_vararg) {
                var_arg = true;
            }

            arg_dec->set_type(type);
        }
        else {
            if (lexer->current_token.get_type() != T_IDENT) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected identifier", lexer);
            }

            arg_dec->set_name(lexer->s_val());

            lexer->advance();
            if (!lexer->current_token.is_operator(":")) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Function arguments have to have a specified type", lexer);
            }

            lexer->advance();
            auto type = parse_type(true);
            if (type->getTypeSpecifier().is_vararg) {
                var_arg = true;
            }

            arg_dec->set_type(type);
        }

        // optional default value
        Token _next = lexer->lookahead();
        if (_next.get_type() == T_OP && _next.get_value().get<std::string>() == "=") {
            lexer->advance();
            Expression::SharedPtr _expr = parse_expression();
            arg_dec->set_default(_expr);
            def_arg = true;
            _next = lexer->lookahead();
            if (var_arg) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Vararg arguments cannot have a default value", lexer);
            }
        } else if (def_arg) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Default values are only allowed as last items of an argument list",
                    lexer);
        }

        arg_dec->set_index(start, lexer->current_index, source_id);

        args.push_back(arg_dec);

        // end of argument list or next argument
        if (_next.is_punctuator(C_COMMA)) {
            lexer->advance();
        } else if (!_next.is_punctuator(C_CLOSE_PAREN)) {
            ParseError::raise(ERR_UNEXPECTED_CHARACTER, "Expected closing parenthesis after argument list",
                    lexer);
        }

        lexer->advance();
        start = lexer->current_index;
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

    int start = lexer->last_token_index;

    // function name
    lexer->advance(T_IDENT);
    FunctionDecl::SharedPtr fun_dec = std::make_shared<FunctionDecl>(lexer->s_val());

    if (lexer->s_val() == "Main") {
        for (const auto& stmt : implicit_main_stmts) {
            if (stmt->get_type() != NodeType::DECLARATION) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Cannot define a main method when top-level non-declaration "
                    "statements exist", lexer);
            }
        }

        main_method_defined = true;
    }

    // arguments
    lexer->advance();
    std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list();
    for (auto arg : args) {
        fun_dec->add_arg(arg);
    }

    // optional return type
    if (lexer->lookahead().is_operator("->")) {
        lexer->advance();
        lexer->advance();
        fun_dec->set_return_type(parse_type());
    }
    else {
        fun_dec->set_return_type(std::make_shared<TypeRef>(TypeSpecifier(AUTO_T)));
    }

    // function body
    CompoundStmt::SharedPtr func_body = parse_block();
    func_body->returnable(true);
    fun_dec->set_body(func_body);
    fun_dec->set_index(start, lexer->current_index, source_id);
    fun_dec->set_attributes(attributes);
    attributes.clear();

    return fun_dec;
}

/**
 * Parses a lambda expression
 * @return
 */
LambdaExpr::SharedPtr Parser::parse_lambda_expr() {
    int start = lexer->last_token_index;

    std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list();

    TypeSpecifier return_type(AUTO_T);
    Token next = lexer->lookahead();
    if (next.is_operator("=>")) {
        lexer->advance(); // =>
    }
    else {
        assert(false && "function should never be called in this case");
    }

    LambdaExpr::SharedPtr lambda_expr = std::make_shared<LambdaExpr>(return_type, args);

    lexer->advance();
    Statement::SharedPtr body = parse_next_stmt();
    lambda_expr->isSingleExpr(true);

    if (body->get_type() == NodeType::COMPOUND_STMT) {
        auto cmpnd = std::static_pointer_cast<CompoundStmt>(body);
        cmpnd->returnable(true);
        cmpnd->is_lambda_body(true);
        lambda_expr->isSingleExpr(false);
    }

    lambda_expr->set_body(body);
    lambda_expr->set_index(start, lexer->current_index, source_id);
    lambda_expr->set_member_expr(__parse_identifier(false));

    return lambda_expr;
}

/**
 * Parses a class constructor declaration
 * @param am
 * @return
 */
ConstrDecl::SharedPtr Parser::parse_constr_decl(AccessModifier am) {

    lexer->advance();
    std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list();

    CompoundStmt::SharedPtr constr_body = parse_block();


    auto constr = std::make_shared<ConstrDecl>(args, constr_body, am);

    return constr;
}

/**
 * Parses a class field declaration
 * @param am
 * @param is_static
 * @return
 */
FieldDecl::SharedPtr Parser::parse_field_decl(AccessModifier am, bool is_static, bool is_interface) {
    int start = lexer->last_token_index;

    if (lexer->current_token.get_type() != T_IDENT) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Field name must be a valid identifier", lexer);
    }

    std::string field_name = lexer->s_val();

    TypeRef::SharedPtr typeref;
    Token next = lexer->lookahead();
    if (next.is_operator(":")) {
        lexer->advance();
        lexer->advance();
        typeref = parse_type();
        next = lexer->lookahead();
    }
    else {
        typeref = std::make_shared<TypeRef>();
    }

    FieldDecl::SharedPtr field = std::make_shared<FieldDecl>(field_name, typeref, am, is_static);

    // getter and setter
    bool getter = false;
    bool setter = false;
    if (next.is_punctuator('{')) {
        lexer->advance();

        get_and_set:
        lexer->advance();
        if (lexer->current_token.get_type() == T_KEYWORD && lexer->s_val() == "get") {
            if (getter) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Cannot redeclare 'get' on the same field",
                        lexer);
            }

            getter = true;
            lexer->advance();
        }
        else if (lexer->current_token.get_type() == T_KEYWORD && lexer->s_val() == "set") {
            if (setter) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Cannot redeclare 'set' on the same field",
                        lexer);
            }

            setter = true;
            lexer->advance();
        }

        if (lexer->current_token.is_punctuator(',')) {
            goto get_and_set;
        }

        if (!lexer->current_token.is_punctuator('}')) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '}");
        }

        next = lexer->lookahead();
    }

    field->generate(getter, setter);

    // optional default value
    if (next.get_type() == T_OP && next.get_value().get<std::string>() == "=") {
        lexer->advance();
        field->set_default(parse_expression());
    }

    lexer->advance(false);
    if (!lexer->current_token.is_separator()) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Field declarations must be on seperate lines", lexer);
    }

    field->set_index(start, lexer->current_index, source_id);
    return field;
}

/**
 * Parses a method declaration
 * @param am
 * @param is_static
 * @param ts
 * @return
 */
MethodDecl::SharedPtr Parser::parse_method_decl(AccessModifier am, bool is_static, bool is_interface) {
    if (lexer->current_token.get_type() != T_IDENT) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Method name must be a valid identifier", lexer);
    }

    int start = lexer->last_token_index;

    std::string method_name = lexer->s_val();
    lexer->advance();
    std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list(is_interface);

    // optional return type
    Token next = lexer->lookahead();
    auto typeref = std::make_shared<TypeRef>();
    if (next.is_operator("->")) {
        lexer->advance();
        lexer->advance();
        typeref = parse_type();
    }


    if (lexer->lookahead().is_punctuator('{')) {

        CompoundStmt::SharedPtr method_body = parse_block();
        method_body->returnable(true);

        lexer->advance(false);
        if (!lexer->current_token.is_separator()) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Method declarations must be on seperate lines", lexer);
        }

        auto method = std::make_shared<MethodDecl>(method_name, typeref, args, method_body, am, is_static);
        method->set_index(start, lexer->current_index, source_id);

        return method;
    }
    else if (!is_interface) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Abstract methods can only be declared in an interface or an abstract "
                "class", lexer);
    }
    else {
        auto method = std::make_shared<MethodDecl>(method_name, typeref, args, am,
            is_static);
        method->set_index(start, lexer->current_index, source_id);

        return method;
    }
}

/**
 * Parses an operator method declaration
 * @param am
 * @return
 */
MethodDecl::SharedPtr Parser::parse_operator_decl(AccessModifier am, std::string class_name, bool is_interface) {
    int start = lexer->last_token_index;

    std::string op;
    bool allow_unary = false;
    if (lexer->current_token.is_punctuator('(') && lexer->lookahead().is_punctuator(')')) {
        lexer->advance();
        allow_unary = true;
        op = "()";
    }
    else if (lexer->current_token.is_punctuator('[') && lexer->lookahead().is_punctuator(']')) {
        lexer->advance();
        if (lexer->lookahead().is_operator("&")) {
            lexer->advance();
            op = "[]&";
        }
        else {
            op = "[]";
        }
    }
    else {
        op = lexer->s_val();
    }

    bool is_unary = util::in_vector<std::string>(util::unary_operators, op);
    bool is_binary = util::in_vector<std::string>(util::binary_operators, op);
    if (!is_unary && !is_binary) {
        infix_functions.push_back(op);
    }

    lexer->advance();
    std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list(is_interface);

    if (args.size() > 1) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Operators cannot have more than one argument", lexer);
    }

    if (!is_unary && !is_binary && args.size() == 0 && !allow_unary) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Custom operators can only be binary", lexer);
    }

    if (is_unary && !is_binary && args.size() > 0) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Unary operator '" + op + "' has no arguments", lexer);
    }

    // optional return type
    Token next = lexer->lookahead();
    TypeRef::SharedPtr typeref;
    bool type_specified = false;
    if (next.is_operator("->")) {
        lexer->advance();
        lexer->advance();
        typeref = parse_type();
    }
    else {
        typeref = std::make_shared<TypeRef>();
    }

    MethodDecl::SharedPtr op_decl;
    if (lexer->lookahead().is_punctuator('{')) {
        CompoundStmt::SharedPtr body = parse_block();
        body->returnable(true);

        op_decl = std::make_shared<MethodDecl>("operator" + op, typeref, args, body, am, false);
    }
    else if (!is_interface) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Abstract methods can only be declared in an interface or an abstract "
                "class");
    }
    else {
        op_decl = std::make_shared<MethodDecl>("operator" + op, typeref, args, am, false);
    }

    op_decl->set_index(start, lexer->current_index, source_id);
    return op_decl;
}

std::vector<pair<string, TypeSpecifier>> Parser::parse_generics() {
    std::vector<pair<string, TypeSpecifier>> generics;
    current_generics.clear();

    if (!lexer->current_token.is_operator("<")) {
        return generics;
    }

    lexer->advance();
    while (!lexer->current_token.is_operator(">")) {
        if (lexer->current_token.get_type() != T_IDENT) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected typename", lexer);
        }

        auto generic_class_name = lexer->s_val();

        TypeSpecifier cov(OBJECT_T);
        cov.class_name = "Any";

        if (lexer->lookahead().is_operator(":")) {
            lexer->advance();
            lexer->advance();

            if (lexer->current_token.get_type() != T_IDENT) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected class name", lexer);
            }

            cov = parse_type()->getTypeSpecifier();
        }

        lexer->advance();

        if (!lexer->current_token.is_operator(">") && !lexer->current_token.is_punctuator(',')) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ',' or '>'", lexer);
        }
        if (lexer->current_token.is_punctuator(',')) {
            lexer->advance();
        }

        generics.emplace_back(generic_class_name, cov);
        current_generics.emplace(generic_class_name, cov);
    }

    lexer->advance();
    return generics;
}

std::vector<TypeSpecifier> Parser::parse_concrete_generics() {
    std::vector<TypeSpecifier> generics;
    current_generics.clear();

    if (!lexer->current_token.is_operator("<")) {
        return generics;
    }

    lexer->advance();
    while (!lexer->current_token.is_operator(">")) {
        if (lexer->current_token.get_type() != T_IDENT) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected typename", lexer);
        }

        auto type = parse_type()->getTypeSpecifier();
        lexer->advance();

        if (!lexer->current_token.is_operator(">") && !lexer->current_token.is_punctuator(',')) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ',' or '>'", lexer);
        }
        if (lexer->current_token.is_punctuator(',')) {
            lexer->advance();
        }

        generics.push_back(type);
    }

    lexer->advance();
    return generics;
}

std::vector<Statement::SharedPtr> Parser::parse_class_inner(string class_name, bool is_abstract, bool is_extension) {
    std::vector<Statement::SharedPtr> declarations;
    std::vector<string> attributes;
    bool declaration_finished = false;

    enum class DeclType {
        NONE,
        FIELD,
        METHOD,
        CONSTR,
        DECONSTR,
        OPERATOR,
        TYPEDEF
    };

    while (!declaration_finished) {
        lexer->advance();
        AccessModifier current_am = AccessModifier::PUBLIC;
        bool am_set = false;
        bool is_static = false;
        auto type = DeclType::NONE;

        attributes = parse_attributes();

        while (lexer->current_token.get_type() == T_KEYWORD) {
            std::string keyword =lexer->s_val();
            if (am_set && (keyword == "public" || keyword == "protected" || keyword == "private")) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Field access modifier already declared", lexer);
            }
            else if (is_static && keyword == "static") {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Field already declared static", lexer);
            }

            if (type != DeclType::NONE && (keyword == "let" || keyword == "def" || keyword == "init" ||
                    keyword == "delete" || keyword == "operator" || keyword == "typedef")) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Declaration type already defined", lexer);
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
                type = DeclType::OPERATOR;
            }
            else if (keyword == "typedef") {
                type = DeclType::TYPEDEF;
            }
            else if (keyword == "let") {
                type = DeclType::FIELD;
            }
            else if (keyword == "def") {
                type = DeclType::METHOD;
            }
            else if (keyword == "init") {
                type = DeclType::CONSTR;
            }
            else if (keyword == "delete") {
                type = DeclType::DECONSTR;
            }
            else {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Unexpected keyword '" + keyword + "' in class declaration",
                    lexer);
            }

            if (type == DeclType::TYPEDEF && is_static) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Typedefs are static by default", lexer);
            }

            lexer->advance();
        }

        if (type == DeclType::NONE) {
            declaration_finished = true;
        }
        else if (type == DeclType::OPERATOR) {
            auto op = parse_operator_decl(current_am, class_name, is_abstract);
            op->set_attributes(attributes);

            declarations.push_back(op);
        }
        else if (type == DeclType::TYPEDEF) {
            auto origin = parse_type();

            lexer->advance(T_IDENT);
            std::string alias = lexer->s_val();

            declarations.push_back(std::make_shared<TypedefDecl>(alias, origin));
        }
        else if (type == DeclType::CONSTR) {
            if (is_static) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Constructor cannot be declared static", lexer);
            }
            if (is_abstract) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Abstract classes cannot declare a constructor", lexer);
            }
            if (is_extension) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Extensions cannot contain a constructor", lexer);
            }

            lexer->backtrack();

            auto constr = parse_constr_decl(current_am);
            constr->set_attributes(attributes);

            declarations.push_back(constr);
        }
        else if (type == DeclType::FIELD) {
            if (is_extension && !is_static) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Extensions can only contain static fields", lexer);
            }

            auto field = parse_field_decl(current_am, is_static, is_abstract);
            field->set_attributes(attributes);
            declarations.push_back(field);
        }
        else if (type == DeclType::METHOD) {
            auto method = parse_method_decl(current_am, is_static, is_abstract);
            method->set_attributes(attributes);
            declarations.push_back(method);
        }
        else {
            declaration_finished = true;
        }

        Token next = lexer->lookahead();
        if (next.is_punctuator('}')) {
            lexer->advance();
            declaration_finished = true;
        }

        attributes.clear();
    }

    return declarations;
}

/**
 * Parses a class declaration
 * @return
 */
ClassDecl::SharedPtr Parser::parse_class_decl(bool is_interface) {
    int start = lexer->last_token_index;

    AccessModifier am = AccessModifier::PRIVATE;
    bool am_set = false;

    while (lexer->current_token.get_type() == T_KEYWORD) {
        if (lexer->s_val() == "public") {
            if (am_set) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "The access modifier for this class was already set",
                        lexer);
            }

            am = AccessModifier::PUBLIC;
            am_set = true;
            lexer->advance();
        }
        else if (lexer->s_val() == "protected") {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Classes cannot be declared 'protected'", lexer);
        }
        else if (lexer->s_val() == "private") {
            if (am_set) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "The access modifier for this class was already set", lexer);
            }

            am_set = true;
            lexer->advance();
        }
        else {
            break;
        }
    }

    lexer->advance();
    if (lexer->current_token.get_type() != T_TYPE && lexer->current_token.get_type() != T_IDENT) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected class name", lexer);
    }

    std::string class_name = lexer->s_val();
    namespaces.push_back(class_name);

    type_names.push_back(class_name);

    std::string extends = "";
    std::vector<std::string> implements;

    lexer->advance();
    auto generics = parse_generics();

    while (!lexer->current_token.is_punctuator('{') && lexer->current_token.get_type() != T_EOF) {
        if (lexer->current_token.is_operator(":")) {
            if (extends != "") {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Classes can only inherit from one other class", lexer);
            }

            lexer->advance();
            if (lexer->current_token.get_type() != T_IDENT) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected identifier after 'extends'", lexer);
            }

            extends = lexer->s_val();
            lexer->advance();
        }
        else if (lexer->current_token.is_keyword("with")) {
            lexer->advance();
            if (lexer->current_token.get_type() != T_IDENT) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected identifier after 'implements'", lexer);
            }

            implements.push_back(lexer->s_val());
            lexer->advance();
            while (lexer->current_token.is_punctuator(',')) {
                lexer->advance();
                if (lexer->current_token.get_type() != T_IDENT) {
                    ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected identifier after 'implements'", lexer);
                }
                implements.push_back(lexer->s_val());
                lexer->advance();
            }
        }
        else {
            break;
        }
    }


    if (!lexer->current_token.is_punctuator('{')) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '{' to start a block statement",
                lexer);
    }

    std::vector<ConstrDecl::SharedPtr> constructors;
    std::vector<FieldDecl::SharedPtr> fields;
    std::vector<MethodDecl::SharedPtr> methods;
    std::vector<TypedefDecl::SharedPtr> typedefs;

    auto declarations = parse_class_inner(class_name, is_interface);
    for (const auto& decl : declarations) {
        switch (decl->get_type()) {
            case NodeType::CONSTR_DECL:
                constructors.push_back(std::static_pointer_cast<ConstrDecl>(decl));
                break;
            case NodeType::FIELD_DECL:
                fields.push_back(std::static_pointer_cast<FieldDecl>(decl));
                break;
            case NodeType::METHOD_DECL:
            case NodeType::OPERATOR_DECL:
                methods.push_back(std::static_pointer_cast<MethodDecl>(decl));
                break;
            case NodeType::TYPEDEF_DECL:
                typedefs.push_back(std::static_pointer_cast<TypedefDecl>(decl));
                break;
            default:
                assert(false && "Unkown class declaration type");
        }
    }

    auto class_dec = std::make_shared<ClassDecl>(class_name, fields, methods, constructors, typedefs, generics, am,
        is_interface, extends, implements);
    class_dec->set_attributes(attributes);
    attributes.clear();


    class_dec->set_index(start, lexer->current_index, source_id);
    return class_dec;
}

/**
 * Parses a struct definition
 * @return
 */
ClassDecl::SharedPtr Parser::parse_struct_decl() {
    int start = lexer->last_token_index;
    auto cl = parse_class_decl();

    return cl;
}

/**
 * Parses an interface declaration
 * @return
 */
ClassDecl::SharedPtr Parser::parse_interface_decl() {
    auto interface = parse_class_decl(true);
    return interface;
}

ExtendStmt::SharedPtr Parser::parse_extend_stmt() {
    lexer->advance();

    if (lexer->current_token.get_type() != T_IDENT && lexer->current_token.get_type() != T_TYPE) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected name of class to extend", lexer);
    }

    int start = lexer->last_token_index;
    auto extended_class = lexer->s_val();

    lexer->advance();

    std::vector<FieldDecl::SharedPtr> fields;
    std::vector<MethodDecl::SharedPtr> methods;

    auto declarations = parse_class_inner(extended_class, false, true);
    for (const auto& decl : declarations) {
        switch (decl->get_type()) {
            case NodeType::CONSTR_DECL:
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Extensions cannot contain constructors", lexer);
                break;
            case NodeType::FIELD_DECL:
                fields.push_back(std::static_pointer_cast<FieldDecl>(decl));
                break;
            case NodeType::METHOD_DECL:
            case NodeType::OPERATOR_DECL:
                methods.push_back(std::static_pointer_cast<MethodDecl>(decl));
                break;
            default:
                assert(false && "No idea how that happened!");
        }
    }

    auto extend_stmt = std::make_shared<ExtendStmt>(extended_class, fields, methods);
    extend_stmt->set_index(start, lexer->current_index, source_id);

    return extend_stmt;
}

/**
 * Parses an if/else statement
 * @return
 */
IfStmt::SharedPtr Parser::parse_if_stmt() {
    int start = lexer->last_token_index;

    Token next = lexer->lookahead();
    if (!next.is_punctuator(C_OPEN_PAREN)) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '(' to begin if statement",
                lexer);
    }

    lexer->advance();

    Expression::SharedPtr if_cond = parse_expression();

    lexer->advance(); // last part of expression
    lexer->advance(); // closing parenthesis

    CompoundStmt::SharedPtr if_branch = std::static_pointer_cast<CompoundStmt>(parse_next_stmt());

    IfStmt::SharedPtr if_stmt = std::make_shared<IfStmt>(if_cond, if_branch);

    next = lexer->lookahead();
    if (next.get_type() == T_KEYWORD && next.get_value().get<std::string>() == "else") {
        lexer->advance();
        lexer->advance();

        CompoundStmt::SharedPtr else_branch = std::static_pointer_cast<CompoundStmt>(parse_next_stmt());
        if_stmt->set_else_branch(else_branch);
    }

    if_stmt->set_index(start, lexer->current_index, source_id);

    return if_stmt;
}

CaseStmt::SharedPtr Parser::parse_case_stmt(bool default_) {
    int start = lexer->last_token_index;

    CaseStmt::SharedPtr case_stmt;
    if (!default_) {
        Expression::SharedPtr case_val = parse_expression();
        case_stmt = std::make_shared<CaseStmt>(case_val);
    }
    else {
        case_stmt = std::make_shared<CaseStmt>();
    }

    lexer->advance();
    if (!lexer->current_token.is_operator(":")) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ':' after case label",
                lexer);
    }

    bool case_finished = false;
    while (!case_finished) {
        lexer->advance();
        if (lexer->current_token.is_punctuator('}')) {
            case_finished = true;
            break;
        }
        else if (lexer->current_token.is_keyword("case") || lexer->current_token.is_keyword("default")) {
            case_finished = true;
            break;
        }

        Statement::SharedPtr stmt = parse_next_stmt();
        case_stmt->add_statement(stmt);
    }

    case_stmt->set_index(start, lexer->current_index, source_id);

    return case_stmt;
}

SwitchStmt::SharedPtr Parser::parse_switch_stmt() {
    int start = lexer->last_token_index;

    lexer->advance();
    if (!lexer->current_token.is_punctuator('(')) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '(' after switch",
                lexer);
    }

    Expression::SharedPtr switch_val = parse_expression();
    lexer->advance(); // closing paren
    lexer->advance(); // opening curly
    if (!lexer->current_token.is_punctuator('{')) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '{' before switch block",
                lexer);
    }

    SwitchStmt::SharedPtr switch_stmt = std::make_shared<SwitchStmt>(switch_val);

    lexer->advance();
    while (!lexer->current_token.is_punctuator('}')) {
        Statement::SharedPtr stmt = parse_next_stmt();
        CaseStmt::SharedPtr case_stmt = std::dynamic_pointer_cast<CaseStmt>(stmt);
        if (case_stmt != nullptr) {
            switch_stmt->add_case(case_stmt);
        }
    }

    switch_stmt->set_index(start, lexer->current_index, source_id);

    return switch_stmt;
}

WhileStmt::SharedPtr Parser::parse_while_stmt() {
    int start = lexer->last_token_index;

    Token next = lexer->lookahead();
    if (!next.is_punctuator(C_OPEN_PAREN)) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '(' after while keyword",
                lexer);
    }

    Expression::SharedPtr while_cond = parse_expression();

    lexer->advance();

    CompoundStmt::SharedPtr while_block = std::static_pointer_cast<CompoundStmt>(parse_next_stmt());
    WhileStmt::SharedPtr while_stmt = std::make_shared<WhileStmt>(while_cond, while_block);

    while_block->set_index(start, lexer->current_index, source_id);

    return while_stmt;
}

ForStmt::SharedPtr Parser::parse_for_stmt() {
    int start = lexer->last_token_index;

    Token next = lexer->lookahead();
    if (!next.is_punctuator(C_OPEN_PAREN)) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '(' after for keyword",
                lexer);
    }

    lexer->advance();
    lexer->advance();

    Statement::SharedPtr init;
    if (lexer->current_token.is_punctuator(';')) {
        init = std::make_shared<Expression>();
    }
    else {
        init = parse_next_stmt();
        lexer->advance();
    }

    // range based for loop
    if (lexer->current_token.is_keyword("in")) {
        IdentifierRefExpr::SharedPtr ident = std::dynamic_pointer_cast<IdentifierRefExpr>(init);
        if (ident == nullptr) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected identifier before 'in' in range based for loop",
                    lexer);
        }

        ident->implicit_ref(false);

        Expression::SharedPtr range = parse_expression();
        lexer->advance();
        if (!lexer->current_token.is_punctuator(C_CLOSE_PAREN)) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ')' after loop arguments");
        }

        lexer->advance();
        CompoundStmt::SharedPtr block = std::static_pointer_cast<CompoundStmt>(parse_next_stmt());

        ForStmt::SharedPtr for_stmt = std::make_shared<ForStmt>(ident, range);
        for_stmt->set_body(block);
        for_stmt->set_index(start, lexer->current_index, source_id);

        return for_stmt;
    }

    if (!lexer->current_token.is_punctuator(C_SEMICOLON)) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ';' to seperate for loop arguments",
                lexer);
    }

    Statement::SharedPtr term;
    lexer->advance();
    if (lexer->current_token.is_punctuator(';')) {
        term = std::make_shared<LiteralExpr>(Variant(true));
    }
    else {
        term = parse_next_stmt();
        lexer->advance();
    }

    if (!lexer->current_token.is_punctuator(C_SEMICOLON)) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ';' to seperate for loop arguments",
                lexer);
    }

    Statement::SharedPtr inc;
    lexer->advance();
    if (lexer->current_token.is_punctuator(')')) {
        inc = std::make_shared<Expression>();
    }
    else {
        inc = parse_next_stmt();
        lexer->advance();
    }

    if (!lexer->current_token.is_punctuator(')')) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ')' after loop arguments",
                lexer);
    }

    lexer->advance();

    CompoundStmt::SharedPtr block = std::static_pointer_cast<CompoundStmt>(parse_next_stmt());

    ForStmt::SharedPtr for_stmt = std::make_shared<ForStmt>(init, term, inc);
    for_stmt->set_body(block);
    for_stmt->set_index(start, lexer->current_index, source_id);

    return for_stmt;
}

/**
 * Interprets a keyword statement
 */
Statement::SharedPtr Parser::parse_keyword() {
    int start = lexer->last_token_index;

    std::string keyword = lexer->current_token.get_value().get<std::string>();

    if (keyword == "let" || keyword == "const") {
        auto assignment = parse_assignment(keyword == "const");

        return assignment;
    }
    else if (keyword == "def") {
        FunctionDecl::SharedPtr fun_dec = parse_function_decl();
        fun_dec->set_index(start, lexer->current_index, source_id);

        return fun_dec;
    }
    else if (keyword == "if") {
        IfStmt::SharedPtr if_stmt = parse_if_stmt();
        if_stmt->set_index(start, lexer->current_index, source_id);

        if (top_level) {
            implicit_main_stmts.push_back(if_stmt);
            return nullptr;
        }

        return if_stmt;
    }
    else if (keyword == "while") {
        WhileStmt::SharedPtr while_stmt = parse_while_stmt();
        while_stmt->set_index(start, lexer->current_index, source_id);

        if (top_level) {
            implicit_main_stmts.push_back(while_stmt);
            return nullptr;
        }

        return while_stmt;
    }
    else if (keyword == "switch") {
        auto switch_stmt = parse_switch_stmt();
        switch_stmt->set_index(start, lexer->current_index, source_id);

        if (top_level) {
            implicit_main_stmts.push_back(switch_stmt);
            return nullptr;
        }

        return switch_stmt;
    }
    else if (keyword == "case") {
        auto case_stmt = parse_case_stmt();
        case_stmt->set_index(start, lexer->current_index, source_id);

        if (top_level) {
            implicit_main_stmts.push_back(case_stmt);
            return nullptr;
        }

        return case_stmt;
    }
    else if (keyword == "default") {
        auto def_stmt = parse_case_stmt(true);
        def_stmt->set_index(start, lexer->current_index, source_id);

        if (top_level) {
            implicit_main_stmts.push_back(def_stmt);
            return nullptr;
        }

        return def_stmt;
    }
    else if (keyword == "for") {
        ForStmt::SharedPtr for_stmt = parse_for_stmt();
        for_stmt->set_index(start, lexer->current_index, source_id);

        if (top_level) {
            implicit_main_stmts.push_back(for_stmt);
            return nullptr;
        }

        return for_stmt;
    }
    else if (keyword == "continue") {
        ContinueStmt::SharedPtr cont_stmt = std::make_shared<ContinueStmt>();
        cont_stmt->set_index(start, lexer->current_index, source_id);

        if (top_level) {
            implicit_main_stmts.push_back(cont_stmt);
            return nullptr;
        }

        return cont_stmt;
    }
    else if (keyword == "typedef") {
        auto origin = parse_type();

        lexer->advance(T_IDENT);
        std::string alias = lexer->s_val();

        return std::make_shared<TypedefDecl>(alias, origin);
    }
    else if (keyword == "break") {
        BreakStmt::SharedPtr break_stmt = std::make_shared<BreakStmt>();
        break_stmt->set_index(start, lexer->current_index, source_id);

        if (top_level) {
            implicit_main_stmts.push_back(break_stmt);
            return nullptr;
        }

        return break_stmt;
    }
    else if (keyword == "class" || keyword == "public" || keyword == "private" || keyword
            == "abstract") {
        return parse_class_decl();
    }
    else if (keyword == "struct") {
        return parse_struct_decl();
    }
    else if (keyword == "interface") {
        return parse_interface_decl();
    }
    else if (keyword == "extend") {
        return parse_extend_stmt();
    }
    else if (keyword == "return") {
        Token next = lexer->lookahead(false);
        if (!next.is_separator()) {
            Expression::SharedPtr expr = parse_expression();
            ReturnStmt::SharedPtr return_stmt = std::make_shared<ReturnStmt>(expr);
            return_stmt->set_index(start, lexer->current_index, source_id);

            if (top_level) {
                implicit_main_stmts.push_back(return_stmt);
                return nullptr;
            }

            return return_stmt;
        }
        else {
            ReturnStmt::SharedPtr return_stmt = std::make_shared<ReturnStmt>();
            return_stmt->set_index(start, lexer->current_index, source_id);
            lexer->advance(false);

            if (top_level) {
                implicit_main_stmts.push_back(return_stmt);
                return nullptr;
            }

            return return_stmt;
        }
    }
    else if (keyword == "goto") {
        lexer->advance();
        auto goto_stmt = std::make_shared<GotoStmt>(lexer->s_val());
        goto_stmt->set_index(start, lexer->current_index, source_id);

        if (top_level) {
            implicit_main_stmts.push_back(goto_stmt);
            return nullptr;
        }

        return goto_stmt;
    }
    else if (keyword == "namespace") {
        return parse_namespace_decl();
    }
    else if (keyword == "export") {
        lexer->advance();
        auto export_stmt = parse_export_stmt();
        export_stmt->set_index(start, lexer->current_index, source_id);

        return export_stmt;
    }
    else if (keyword == "using") {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Keyword '" + keyword + "' is only allowed at the beginning of a "
                "file", lexer);
    }
    else {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "'" + keyword + "' is a reserved keyword", lexer);
    }
}

/**
 * Parses a function call in the form func(arg1, arg2, ...)
 * @return
 */
CallExpr::SharedPtr Parser::parse_function_call() {
    int start = lexer->last_token_index;

    lexer->advance();
    auto generics = parse_concrete_generics();

    if (!lexer->current_token.is_punctuator('(')) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '(' before function arguments", lexer);
    }

    CallExpr::SharedPtr call = std::make_shared<CallExpr>(CallType::FUNC_CALL, parse_arguments());
    call->set_index(start, lexer->current_index, source_id);
    call->set_generics(generics);

    return call;
}

std::vector<Expression::SharedPtr> Parser::parse_arguments() {
    std::vector<Expression::SharedPtr> args;

    // collect arguments
    Token _next = lexer->lookahead();
    if (!_next.is_punctuator(')')) {
        parse_func_call_arg:
        args.push_back(parse_expression());

        _next = lexer->lookahead();
        if (_next.is_punctuator(',')) {
            lexer->advance();
            goto parse_func_call_arg;
        } else if (!_next.is_punctuator(')')) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ')' after function call", lexer);
        }
    }
    lexer->advance();

    return args;
}

std::vector<string> Parser::parse_attributes() {
    std::vector<string> attributes;

    while (lexer->current_token.is_punctuator('@')) {
        lexer->advance();
        if (lexer->current_token.get_type() != T_IDENT) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected attribute name", lexer);
        }

        string attr = lexer->s_val();

        if (std::find(util::attributes.begin(), util::attributes.end(), attr) == util::attributes.end()) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Unknown attribute @" + attr, lexer);
        }

        if (attr == "NoStdLib") {
            lib_imports_disabled = true;
        }

        attributes.push_back(attr);
        lexer->advance();
    }

    return attributes;
}

CompoundStmt::SharedPtr Parser::parse_block() {
    int start = lexer->last_token_index;
    bool last_top_level = top_level;
    top_level = false;

    lexer->advance();
    if (!(lexer->current_token.is_punctuator('{'))) {
        ParseError::raise(ERR_UNEXPECTED_CHARACTER, "Expected '{' to start a block statement.", lexer);
    }
    lexer->advance();

    CompoundStmt::SharedPtr block = std::make_shared<CompoundStmt>();
    while (!lexer->current_token.is_punctuator('}')) {
        while (lexer->current_token.is_separator()) {
            lexer->advance();
        }

        if (lexer->current_token.get_type() == T_EOF || lexer->current_token.is_punctuator('}')) {
            break;
        }

        Statement::SharedPtr stmt = parse_next_stmt();

        block->add_statement(stmt);

        lexer->advance();
    }

    if (lexer->current_token.get_type() != T_EOF && !lexer->current_token.is_punctuator('}')) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '}' to end a block statement", lexer);
    }

    block->set_index(start, lexer->current_index, source_id);
    top_level = last_top_level;

    return block;
}

Statement::SharedPtr Parser::parse_next_stmt() {
    if (lexer->current_token.is_punctuator('{')) {
        lexer->backtrack();
        CompoundStmt::SharedPtr cmp_stmt = parse_block();

        if (top_level) {
            implicit_main_stmts.push_back(cmp_stmt);
            return nullptr;
        }

        return cmp_stmt;
    }
    else if (lexer->current_token.get_type() == T_KEYWORD) {
        Statement::SharedPtr expr = parse_keyword();

        return expr;
    }
    else if (lexer->current_token.get_type() == T_IDENT && lexer->lookahead().is_operator(":")) {
        std::string label = lexer->s_val();
        lexer->advance();

        auto label_stmt = std::make_shared<LabelStmt>(label);

        if (top_level) {
            implicit_main_stmts.push_back(label_stmt);
            return nullptr;
        }

        return label_stmt;
    }
    else if (lexer->current_token.is_punctuator('@')) {
        attributes = parse_attributes();
        return parse_next_stmt();
    }
    else {
        lexer->backtrack();
        Expression::SharedPtr expr = parse_expression();

        if (top_level) {
            implicit_main_stmts.push_back(expr);
            return nullptr;
        }

        return expr;
    }
}

/**
 * Parses the module declaration for the current file
 * @return
 */
NamespaceDecl::SharedPtr Parser::parse_namespace_decl() {
    int start = lexer->last_token_index;

    lexer->advance();
    if (lexer->current_token.get_type() != T_IDENT) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected T_IDENT after 'namespace'", lexer);
    }

    auto ns_name = lexer->s_val();

    auto mod = std::make_shared<NamespaceDecl>(ns_name, parse_block());
    mod->set_index(start, lexer->current_index, source_id);

    namespaces.push_back(ns_name);
    return mod;
}

/**
 * Parses a module import statement
 * @return
 */
ImportStmt::SharedPtr Parser::parse_import_stmt() {
    int start = lexer->last_token_index;
    lexer->advance();

    std::vector<string> _import { lexer->s_val() };
    while (lexer->lookahead().is_punctuator('.')) {
        lexer->advance();
        lexer->advance();
        if (lexer->current_token.get_type() != T_IDENT && lexer->current_token.get_type() != T_TYPE) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected identifier", lexer);
        }

        _import.push_back(lexer->s_val());
    }

    ImportStmt::SharedPtr imp_stmt = std::make_shared<ImportStmt>(_import);

    imp_stmt->set_index(start, lexer->current_index, source_id);
    return imp_stmt;
}

/**
 * Parses an export statement
 * @return
 */
ExportStmt::SharedPtr Parser::parse_export_stmt() {
    int start = lexer->last_token_index;

    if (lexer->current_token.get_type() != T_IDENT) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected T_IDENT after 'export'", lexer);
    }

    auto export_stmt = std::make_shared<ExportStmt>(lexer->s_val());
    export_stmt->set_index(start, lexer->current_index, source_id);

    return export_stmt;
}

/**
 * Runs the program by tokenizing the program, creating the AST and finally evaluating it.
 * @return
 */
void Parser::run(bool debug = false) {
    CompoundStmt::SharedPtr root = std::make_shared<CompoundStmt>();

    std::vector<std::string> imports { "Any", "Interface/Number", "Int", "Float", "Double", "Bool", "Char",
        "Array", "String", "Print" };
    for (const auto& _import : imports) {
        string file_path = __FILE__;
        string dir_path = file_path.substr(0, file_path.rfind("/")) + "/StdLib/" + _import + ".dot";
        std::ifstream ifs(dir_path);
        std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

        auto Import = Parser(content).parse();
        root->add_statements(Import->get_statements());
    }

    auto stmts = parse();
    if (lib_imports_disabled) {
        root->clear();
    }

    root->add_statements(stmts->get_statements());

    if (!main_method_defined) {
        auto Main = std::make_shared<FunctionDecl>("Main");
        Main->set_return_type(std::make_shared<TypeRef>(TypeSpecifier(INT_T)));

        auto body = std::make_shared<CompoundStmt>();
        body->returnable(true);
        body->add_statements(implicit_main_stmts);
        Main->set_body(body);

        root->add_statement(Main);
    }
    else {
        for (const auto& stmt : implicit_main_stmts) {
            if (stmt->get_type() != NodeType::DECLARATION) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Cannot define a main method when top-level non-declaration "
                    "statements exist", lexer);
            }
        }

        root->add_at_begin(implicit_main_stmts);
    }

    delete lexer;

    if (debug) {
        root->__dump(0);
        std::cout << std::endl << std::endl;
    }

    TypeCheckVisitor tc;
    tc.visit(root.get());

    CodeGenVisitor cg;
    cg.visit(root.get());

    cg.finalize();
}

/**
 * Parses the program into an AST
 * @return
 */
CompoundStmt::SharedPtr Parser::parse() {
    lexer->advance();
    CompoundStmt::SharedPtr root = std::make_shared<CompoundStmt>();

    while (lexer->current_token.is_keyword("using")) {
        root->add_statement(parse_import_stmt());
        lexer->advance();
    }

    while(lexer->current_token.get_type() != T_EOF) {
        while (lexer->current_token.is_separator() || lexer->current_token.is_punctuator(';')) {
            lexer->advance();
        }

        if (lexer->current_token.get_type() == T_EOF) {
            break;
        }

        Statement::SharedPtr stmt = parse_next_stmt();

        if (attributes.size() > 0) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Attributes not allowed here", lexer);
        }

        if (stmt != nullptr) {
            root->add_statement(stmt);
        }

        lexer->advance();
    }

    return root;
}