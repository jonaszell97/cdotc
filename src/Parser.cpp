//
// Created by Jonas Zell on 13.06.17.
//

#include <string>
#include "Parser.h"
#include "Lexer.h"
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
#include "AST/Expression/RefExpr/SubscriptExpr.h"
#include "AST/Statement/ControlFlow/ReturnStmt.h"
#include "AST/Visitor/Visitor.h"
#include "AST/Statement/ControlFlow/ContinueStmt.h"
#include "AST/Statement/ControlFlow/BreakStmt.h"
#include "AST/Visitor/CaptureVisitor.h"
#include "AST/Visitor/EvaluatingVisitor.h"
#include "AST/Statement/ControlFlow/GotoStmt.h"
#include "AST/Expression/Literal/LambdaExpr.h"
#include "AST/Statement/Declaration/NamespaceDecl.h"
#include "AST/Statement/UsingStmt.h"
#include "AST/Statement/EndOfFileStmt.h"
#include "AST/Visitor/StaticAnalysis/TypeCheckVisitor.h"
#include "AST/SymbolTable.h"
#include "AST/Visitor/CodeGen/CodeGenVisitor.h"
#include "Message/Warning.h"
#include "AST/Statement/Declaration/TypedefDecl.h"
#include "Variant/Type/Type.h"
#include "Variant/Type/PointerType.h"
#include "Variant/Type/IntegerType.h"
#include "Variant/Type/FPType.h"
#include "Variant/Type/FunctionType.h"
#include "Variant/Type/CollectionType.h"
#include "Variant/Type/ObjectType.h"
#include "Variant/Type/GenericType.h"
#include "AST/Statement/DebugStmt.h"
#include "AST/Statement/Declaration/Class/ClassDecl.h"
#include "AST/Expression/TypeRef.h"
#include "AST/Statement/Declaration/FuncArgDecl.h"
#include "AST/Attribute/Attribute.h"
#include "AST/Expression/Literal/CollectionLiteral.h"
#include "Variant/Type/VoidType.h"
#include "AST/Operator/TertiaryOperator.h"
#include "AST/Statement/Declaration/Class/ConstrDecl.h"
#include "AST/Statement/Declaration/Class/FieldDecl.h"
#include "AST/Statement/Declaration/Class/MethodDecl.h"
#include "AST/Statement/Declaration/ExtendStmt.h"
#include "AST/Statement/ControlFlow/IfStmt.h"
#include "AST/Statement/ControlFlow/CaseStmt.h"
#include "AST/Statement/ControlFlow/SwitchStmt.h"
#include "AST/Statement/ControlFlow/WhileStmt.h"
#include "AST/Statement/ControlFlow/ForStmt.h"
#include "AST/Statement/Declaration/DeclareStmt.h"

std::vector<string> Parser::type_names = {};
std::vector<string> Parser::namespaces = {};
std::vector<string> Parser::source_files = {};
unordered_map<string, GenericType*>  Parser::CurrentClassGenerics =  {};
std::vector<ClassDecl::SharedPtr> Parser::class_declarations = {};
bool Parser::lib_imports_disabled = false;

/**
 * Creates a new interpreter for an Xtreme Jonas Script program.
 * @param program
 */
Parser::Parser(string program) :
    lexer(new Lexer(program)),
    source_id(source_files.size())
{
    source_files.push_back(program);
}


TypeRef::SharedPtr Parser::parse_type() {
    int start = lexer->last_token_index;

    auto type = __parse_type();
    auto typeref = std::make_shared<TypeRef>(type);
    typeref->setIndex(start, lexer->current_index, source_id);

    return typeref;

}

/**
 * Parses a type specifier, like "int", "bool[3]", "any[18 * x]"
 * @return
 */
Type* Parser::__parse_type() {

    bool isReference = false;
    if (lexer->current_token.is_keyword("ref")) {
        lexer->advance();
        isReference = true;
    }

    // collection type
    if (lexer->current_token.is_punctuator('[')) {
        lexer->advance();

        auto elType = __parse_type();
        Type* arrType = new CollectionType(elType);
        if (isReference) {
            arrType = new PointerType(arrType);
            arrType->isReference(true);
        }

        if (!lexer->lookahead().is_punctuator(']')) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ']' after array type", lexer);
        }

        lexer->advance();

        return arrType;
    }

    // function type
    if (lexer->current_token.is_punctuator('(')) {
        lexer->advance();

        auto argTypes = parse_tuple_type();

        lexer->advance();
        if (!lexer->current_token.is_operator("->")) {
            // TODO tuple
        }

        lexer->advance();
        auto returnType = __parse_type();

        Type* fun = new FunctionType(returnType, std::move(argTypes));
        if (isReference) {
            fun = new PointerType(fun);
            fun->isReference(true);
        }

        return fun;
    }


    string typeName = lexer->s_val();
    Type* type;

    while (lexer->lookahead(false).is_punctuator('.')) {
        lexer->advance();
        lexer->advance();
        if (lexer->current_token.get_type() != T_IDENT) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Unexpected character in type reference", lexer);
        }

        typeName += "." + lexer->s_val();
    }

    if (util::typemap.find(typeName) != util::typemap.end()) {
        bool isIntegral = false;
        bool isUnsigned = false;
        unsigned int bitWidth;

        if (typeName.substr(0, 4) == "UInt") {
            isUnsigned = true;
            typeName = typeName.substr(1);
        }

        if (typeName.substr(0, 3) == "Int") {
            isIntegral = true;
            std::string bitwidth;
            while (typeName.back() >= '0' && typeName.back() <= '9') {
                bitwidth = std::string(1, typeName.back()) + bitwidth;
                typeName = typeName.substr(0, typeName.length() - 1);
            }

            bitWidth = bitwidth.empty() ? sizeof(int*) * 8 : std::stoi(bitwidth);
        }

        if (typeName == "Bool") {
            isIntegral = true;
            bitWidth = 1;
        }

        if (isIntegral) {
            type = IntegerType::get(bitWidth, isUnsigned);
        }
        else if (typeName == "Void") {
            type = new VoidType;
        }
        else {
            type = typeName == "Double" ? FPType::getDoubleTy() : FPType::getFloatTy();
        }
    }
    else {
        if (CurrentClassGenerics.find(typeName) != CurrentClassGenerics.end()) {
            type = CurrentClassGenerics[typeName]->deepCopy();
        }
        else {
            type = ObjectType::get(typeName);
        }
    }

    Token next = lexer->lookahead(false);

    // generic type requirements
    if (next.is_operator("<")) {
        if (!isa<ObjectType>(type)) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Generic class names are only allowed after object types", lexer);
        }

        lexer->advance();

        auto asObj = cast<ObjectType>(type);
        auto generics = parse_concrete_generics();

        asObj->setUnqualGenerics(generics);
        next = lexer->lookahead(false);
    }

    // optional type
    if (next.is_operator("?")) {
        type->isNullable(true);
        lexer->advance();
        next = lexer->lookahead(false);
    }

    // pointer type
    while (next.get_type() == T_OP) {
        auto op = next.get_value().getString();
        if (util::matches("\\*+", op)) {
            while (op.length() > 0) {
                type = new PointerType(type);
                op = op.substr(0, op.length() - 1);
            }

            lexer->advance();
            next = lexer->lookahead();
        }
        else {
            break;
        }
    }

    // c style array
    while (lexer->lookahead().is_punctuator('[')) {
        lexer->advance();

        Expression::SharedPtr arrLength = nullptr;
        if (!lexer->lookahead().is_punctuator(']')) {
            arrLength = parse_expression();
        }

        lexer->advance();

        if (!lexer->current_token.is_punctuator(']')) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ']' after array type", lexer);
        }

        type = new PointerType(type, arrLength);
        type->isCStyleArray(true);
    }

    if (isReference) {
        type = new PointerType(type);
        type->isReference(true);
    }

    return type;
}

/**
 * Parses an identifier (wrapper function for lvalue check)
 * @return
 */
Expression::SharedPtr Parser::parse_identifier() {
    int start = lexer->last_token_index;
    Expression::SharedPtr ref_expr = __parse_identifier(true);
    ref_expr->setIndex(start, lexer->current_index, source_id);

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
        ident_expr->setMemberExpr(__parse_identifier());
        ident_expr->setIndex(start, lexer->current_index, source_id);

        return ident_expr;
    }

    // function call
    bool generic_func_call = is_generic_call();
    if (_next.is_punctuator('(') || generic_func_call) {
        CallExpr::SharedPtr call = parse_function_call();
        call->setMemberExpr(__parse_identifier());
        call->setIndex(start, lexer->current_index, source_id);

        return call;
    }

    // member access
    if (_next.is_punctuator('.')) {
        lexer->advance(false);
        lexer->advance(false);
        string ident = lexer->s_val();

        // method call
        _next = lexer->lookahead();
        if (_next.is_punctuator('(') || is_generic_call()) {
            lexer->advance();
            auto calltype = std::find(namespaces.begin(), namespaces.end(), ident) != namespaces.end() ?
                CallType::FUNC_CALL : CallType::METHOD_CALL;

            auto generics = parse_concrete_generics();
            if (!generics.empty()) {
                lexer->advance();
            }

            CallExpr::SharedPtr method_call = std::make_shared<CallExpr>(calltype, parse_arguments(), ident);
            method_call->setMemberExpr(__parse_identifier());
            method_call->setIndex(start, lexer->current_index, source_id);
            method_call->set_generics(generics);

            return method_call;
        }

        MemberRefExpr::SharedPtr mem_ref = std::make_shared<MemberRefExpr>(lexer->s_val());
        mem_ref->setMemberExpr(__parse_identifier());
        mem_ref->setIndex(start, lexer->current_index, source_id);

        return mem_ref;
    }

    // array access
    if (_next.is_punctuator('[')) {
        lexer->advance(false);
        Expression::SharedPtr expr = parse_expression();
        expr->setIndex(start, lexer->current_index, source_id);

        SubscriptExpr::SharedPtr arr_acc = std::make_shared<SubscriptExpr>(expr);

        lexer->advance();
        if (!lexer->current_token.is_punctuator(']')) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ']'", lexer);
        }

        arr_acc->setMemberExpr(__parse_identifier());
        arr_acc->setIndex(start, lexer->current_index, source_id);

        return arr_acc;
    }

    return {};
}

/**
 * Parses an array literal in the form of
 * [val1, val2, val3, ..., valn]
 * @return
 */
CollectionLiteral::SharedPtr Parser::parse_array_literal() {
    int start = lexer->last_token_index;

    lexer->advance();
    if (!lexer->current_token.is_punctuator('[')) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '[' to begin an array literal", lexer);
    }

    CollectionLiteral::SharedPtr arr = std::make_shared<CollectionLiteral>(
        std::make_shared<TypeRef>(ObjectType::get("Array"))
    );

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

    arr->setIndex(start, lexer->current_index, source_id);

    return arr;
}

/**
 * Parses an atomic part of an expression, like a single identifier or literal
 * @return
 */
Expression::SharedPtr Parser::parse_unary_expr_target() {
    Token next = lexer->lookahead(false);
    std::vector<Attribute> attributes;

    if (next.is_punctuator('@')) {
        lexer->advance();
        attributes = parse_attributes();
        lexer->backtrack();
        next = lexer->lookahead();
    }

    if (next.is_punctuator('[')) {
        auto arr = parse_array_literal();
        arr->setAttributes(attributes);

        return arr;
    }
    if (next.is_punctuator('(')) {
        lexer->advance();

        auto expr = parse_paren_expr();
        expr->setAttributes(attributes);
        expr->setMemberExpr(__parse_identifier(false));

        return expr;
    }
    if (next.get_type() == T_IDENT) {
        parse_identifier:
        int start = lexer->last_token_index;
        lexer->advance(false);
        next = lexer->lookahead(false);

        // function call
        if (next.is_punctuator('(') || is_generic_call()) {
            std::string func_name = lexer->s_val();
            lexer->advance();

            std::vector<Type*> generics = parse_concrete_generics();
            if (!generics.empty()) {
                lexer->advance();
            }

            auto args = parse_arguments();

            CallExpr::SharedPtr func_call = std::make_shared<CallExpr>(CallType::FUNC_CALL, args, func_name);
            func_call->setMemberExpr(__parse_identifier(false));
            func_call->setIndex(start, lexer->current_index, source_id);
            func_call->setAttributes(attributes);
            func_call->set_generics(generics);

            return func_call;
        }

        lexer->backtrack();
        auto ident = parse_identifier();
        ident->setAttributes(attributes);

        return ident;
    }
    if (next.get_type() == T_LITERAL) {
        int start = lexer->last_token_index;
        lexer->advance();

        if (isa<ObjectType>(lexer->current_token.get_value().getType())) {
            Token next = lexer->lookahead(false);

            // possible string modifier
            if (next.get_type() == T_IDENT) {
                std::string modifier = next.get_value().getString();
                if (util::string_modifiers.find(modifier) != util::string_modifiers.end()) {
                    auto string_literal = std::make_shared<StringLiteral>(lexer->s_val(), modifier[0]);
                    lexer->advance();

                    string_literal->setMemberExpr(__parse_identifier());
                    string_literal->setIndex(start, lexer->current_index, source_id);
                    string_literal->setAttributes(attributes);

                    return string_literal;
                }
                else {
                    lexer->advance();
                    ParseError::raise(ERR_UNEXPECTED_CHARACTER, "Unknown string modifier " + modifier, lexer);
                }
            }
            else {
                auto string_literal = std::make_shared<StringLiteral>(lexer->s_val());
                string_literal->setMemberExpr(__parse_identifier());
                string_literal->setIndex(start, lexer->current_index, source_id);
                string_literal->setAttributes(attributes);

                return string_literal;
            }
        }

        auto expr = std::make_shared<LiteralExpr>(lexer->current_token.get_value());
        expr->setMemberExpr(__parse_identifier());
        expr->setIndex(start, lexer->current_index, source_id);
        expr->setAttributes(attributes);

        return expr;
    }
    else {
        lexer->advance();
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected expression but found " + util::token_names[next.get_type()], lexer);
    }

    return nullptr;
}

namespace {

    bool isPrefixUnaryOp(Token& next) {
        return next.get_type() == T_OP &&
            util::in_vector(util::PrefixUnaryOperators, next.get_value().getString());
    }

    bool isPostfixUnaryOp(Token& next) {
        return next.get_type() == T_OP &&
            util::in_vector(util::PostfixUnaryOperators, next.get_value().getString());
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
Expression::SharedPtr Parser::parse_unary_expr(UnaryOperator::SharedPtr literal, bool postfix) {
    int start = lexer->last_token_index;
    Token next = lexer->lookahead(false);

    // prefix unary op
    if (isPrefixUnaryOp(next)) {
        lexer->advance(false);

        UnaryOperator::SharedPtr unary_op = std::make_shared<UnaryOperator>(lexer->s_val(), "prefix");

        unary_op->setTarget(parse_unary_expr(literal, postfix));
        unary_op->setIndex(start, lexer->current_index, source_id);

        return unary_op;
    }

    Expression::SharedPtr expr;

    if (literal == nullptr) {
        expr = parse_unary_expr_target();
    }
    else if (!postfix) {
        literal->setTarget(parse_unary_expr_target());
        expr = literal;
    }

    next = lexer->lookahead(false);

    // postfix unary op
    if (isPostfixUnaryOp(next)) {
        lexer->advance();
        UnaryOperator::SharedPtr unary_op = std::make_shared<UnaryOperator>(lexer->s_val(), "postfix");

        unary_op->setTarget(expr);
        unary_op->setIndex(start, lexer->current_index, source_id);

        return unary_op;
    }

    // call
    if (next.is_punctuator('(') || is_generic_call()) {
        lexer->advance();

        std::vector<Type*> generics = parse_concrete_generics();

        auto call = std::make_shared<CallExpr>(CallType::FUNC_CALL, parse_arguments());
        call->set_generics(generics);
        expr->setMemberExpr(call);
    }

    expr->setIndex(start, lexer->current_index, source_id);
    return expr;
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
    op->setIndex(start, lexer->current_index, source_id);

    return op;
}

ParenExprType Parser::get_paren_expr_type() {
    int start_index = lexer->current_token_index - 1;
    int last_index = lexer->last_token_index - 1;
    Token next = lexer->lookahead();

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
        if (lexer->current_token.get_type() == T_IDENT) {
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
        default:
            llvm_unreachable("Unknown paren expr type");
    }


    return expr;
}

namespace {

    bool hasHigherPrecedence(Token& next, int& min_precedence) {
        if (next.get_type() == T_OP) {
            auto op = next.get_value().getString();
            return util::op_precedence.find(op) != util::op_precedence.end()
                && util::op_precedence[op] > min_precedence;
        }

        if (next.get_type() == T_IDENT) {
            return util::op_precedence["infix"] > min_precedence;
        }

        return false;
    }

    bool hasHigherOrEqualPrecedence(Token& next, int& min_precedence) {
        if (next.get_type() == T_OP) {
            auto op = next.get_value().getString();
            return util::op_precedence.find(op) != util::op_precedence.end()
                && util::op_precedence[op] >= min_precedence;
        }

        if (next.get_type() == T_IDENT) {
            return util::op_precedence["infix"] >= min_precedence;
        }

        return false;
    }

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

    if (lhs == nullptr) {
        lhs = parse_unary_expr();
    }

    Token next = lexer->lookahead(false);

    // ...while the next operator has a higher precedence than the minimum
    while (hasHigherOrEqualPrecedence(next, min_precedence)) {
        string op = next.get_value().getString();
        lexer->advance(false);

        next = lexer->lookahead(false);
        Expression::SharedPtr rhs;

        if (op == "as" || op == "as!" || op == "isa") {
            lexer->advance();
            rhs = parse_type();
        }
        else {
            rhs = parse_unary_expr();
        }

        next = lexer->lookahead(false);

        // continue recursively while a higher precedence operator follows
        while (hasHigherPrecedence(next, util::op_precedence[op])) {
            rhs = parse_expression(rhs, util::op_precedence[next.get_value().getString()]);
            next = lexer->lookahead(false);
        }

        // return the expression so far
        BinaryOperator::SharedPtr binary_op = std::make_shared<BinaryOperator>(op);
        binary_op->setIndex(start, lexer->current_index, source_id);
        binary_op->setLhs(lhs);
        binary_op->setRhs(rhs);

        lhs = binary_op;
    }

    // tertiary operator
    if (next.is_operator("?")) {
        lexer->advance(false);
        auto op = parse_tertiary_operator(lhs);
        op->setIndex(start, lexer->current_index, source_id);

        return op;
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
Statement::SharedPtr Parser::parse_assignment(bool is_const, bool is_declaration) {

    std::vector<DeclStmt::SharedPtr> declarations;

    for (;;) {
        int start = lexer->last_token_index;
        lexer->advance();

        bool inferred = true;
        std::string identifier = lexer->s_val();

        Expression::SharedPtr value = nullptr;
        TypeRef::SharedPtr typeref = std::make_shared<TypeRef>();

        Token next = lexer->lookahead(false);

        if (next.is_operator(":")) {
            inferred = false;

            lexer->advance();
            lexer->advance();

            typeref->setType(__parse_type());

            next = lexer->lookahead();
        }
        else if (is_declaration) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Declared variables must be type annotated", lexer);
        }

        typeref->getType(true)->isConst(is_const);

        if (next.is_operator("=")) {
            if (is_declaration) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Declared variables cannot be assigned", lexer);
            }

            lexer->advance(false);
            value = parse_expression();
        }
        else if (inferred) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Type inferred declarations have to be initialized directly",
                lexer);
        }

        DeclStmt::SharedPtr decl_stmt = std::make_shared<DeclStmt>(identifier, typeref, is_const, top_level, value);
        decl_stmt->setAttributes(attributes);
        decl_stmt->setIndex(start, lexer->current_index, source_id);

        if (value != nullptr) {
            value->setDeclaration(decl_stmt.get());
        }

        declarations.push_back(decl_stmt);

        next = lexer->lookahead();
        if (!next.is_punctuator(',')) {
            break;
        }
        else if (is_declaration) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Compound declarations not allowed here", lexer);
        }
    }

    attributes.clear();

    if (declarations.size() == 1) {
        return declarations.front();
    }

    CompoundStmt::SharedPtr comp = std::make_shared<CompoundStmt>(true);
    for (const auto & decl : declarations) {
        comp->addStatement(decl);
    }

    return comp;
}

std::vector<Type*> Parser::parse_tuple_type() {
    std::vector<Type*> tupleTypes;

    while (!lexer->current_token.is_punctuator(')')) {
        lexer->advance();

        bool vararg = false;
        if (lexer->current_token.is_operator("...")) {
            vararg = true;
            lexer->advance();

            // c style vararg
            if (lexer->current_token.get_type() != T_IDENT) {
                auto argType = new AutoType();
                argType->isCStyleVararg(true);
                tupleTypes.push_back(argType);
                continue;
            }
        }

        auto argType = __parse_type();
        argType->isVararg(vararg);
        tupleTypes.push_back(argType);

        lexer->advance();
        if (lexer->current_token.is_punctuator(',')) {
            lexer->advance();
        }
        else if (!lexer->current_token.is_punctuator(')')) {
            ParseError::raise(ERR_UNEXPECTED_CHARACTER, "Expected closing parenthesis after argument list",
                lexer);
        }
    }

    return tupleTypes;
}

/**
 * Parses a method's argument list
 * @return
 */
std::vector<FuncArgDecl::SharedPtr> Parser::parse_arg_list(bool no_var_names) {

    int start = lexer->last_token_index;

    std::vector<FuncArgDecl::SharedPtr> args;
    if (!lexer->current_token.is_punctuator('(')) {
        lexer->backtrack();
        return args;
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

        std::vector<Attribute> attributes = parse_attributes();

        bool mut = false;
        if (lexer->current_token.is_keyword("ref")) {
            mut = true;
            lexer->advance();
        }

        FuncArgDecl::SharedPtr arg_dec = std::make_shared<FuncArgDecl>(mut);
        arg_dec->setAttributes(attributes);

        if (!no_var_names || lexer->lookahead().is_operator(":")) {
            if (lexer->current_token.get_type() != T_IDENT) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected identifier", lexer);
            }

            arg_dec->setName(lexer->s_val());
            lexer->advance();

            if (!lexer->current_token.is_operator(":")) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Function arguments have to have a specified type", lexer);
            }

            lexer->advance();
        }

        if (lexer->current_token.is_operator("...")) {
            var_arg = true;
            lexer->advance();
            if (lexer->lookahead().get_type() != T_IDENT) {
                auto argType = new AutoType();
                argType->isCStyleVararg(true);

                arg_dec->setType(std::make_shared<TypeRef>(argType));
                arg_dec->setIndex(start, lexer->current_index, source_id);

                args.push_back(arg_dec);
                break;
            }
        }

        auto argTy = parse_type();
        if (argTy->getType(true)->isVararg()) {
            var_arg = true;
        }

        arg_dec->setType(argTy);
        lexer->advance();

        // optional default value
        if (lexer->current_token.is_operator("=")) {
            if (var_arg) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Vararg arguments cannot have a default value", lexer);
            }

            Expression::SharedPtr _expr = parse_expression();
            arg_dec->setDefault(_expr);
            def_arg = true;

            arg_dec->getArgType()->getType(true)->hasDefaultArgVal(true);

            lexer->advance();

        } else if (def_arg) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Default values are only allowed as last items of an argument list",
                    lexer);
        }

        arg_dec->setIndex(start, lexer->current_index, source_id);
        args.push_back(arg_dec);

        // end of argument list or next argument
        if (lexer->current_token.is_punctuator(',')) {
            lexer->advance();
        } else if (!lexer->current_token.is_punctuator(')')) {
            ParseError::raise(ERR_UNEXPECTED_CHARACTER, "Expected closing parenthesis after argument list",
                    lexer);
        }

        start = lexer->current_index;
    }

    if (!lexer->current_token.is_punctuator(')')) {
        ParseError::raise(ERR_UNEXPECTED_CHARACTER, "Expected closing parenthesis after argument list", lexer);
    }

    return args;
}

/**
 * Parses a function declaration in the form of
 *  def func(x: number, y: string) => bool {
 *      ... statements
 *  }
 */
FunctionDecl::SharedPtr Parser::parse_function_decl(bool is_declaration) {

    int start = lexer->last_token_index;

    // function name
    lexer->advance(T_IDENT);
    FunctionDecl::SharedPtr fun_dec = std::make_shared<FunctionDecl>(lexer->s_val());

    if (lexer->s_val() == "main") {
        for (const auto& stmt : implicit_main_stmts) {
            if (stmt->get_type() != NodeType::DECLARATION) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Cannot define a main method when top-level non-declaration "
                    "statements exist", lexer);
            }
        }

        main_method_defined = true;
    }

    lexer->advance();

    auto generics = parse_generics();
    fun_dec->setGenerics(std::move(generics));

    // arguments
    std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list(is_declaration);
    for (auto arg : args) {
        fun_dec->addArgument(arg);
    }

    // optional return type
    if (lexer->lookahead().is_operator("->")) {
        lexer->advance();
        lexer->advance();
        fun_dec->setReturnType(parse_type());
    }
    else if (is_declaration) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Declared functions have to have a defined return type", lexer);
    }
    else {
        fun_dec->setReturnType(std::make_shared<TypeRef>(new VoidType()));
    }

    if (lexer->lookahead().is_punctuator('{') || lexer->lookahead().is_keyword("unsafe")) {
        if (is_declaration) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Declared functions cannot have a body", lexer);
        }

        // function body
        CompoundStmt::SharedPtr func_body = parse_block();
        func_body->returnable(true);
        fun_dec->setBody(func_body);
        fun_dec->setIndex(start, lexer->current_index, source_id);
        fun_dec->setAttributes(attributes);
        attributes.clear();
    }

    CurrentClassGenerics.clear();

    return fun_dec;
}

/**
 * Parses a lambda expression
 * @return
 */
LambdaExpr::SharedPtr Parser::parse_lambda_expr() {
    int start = lexer->last_token_index;

    std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list();

    Token next = lexer->lookahead();
    if (next.is_operator("=>")) {
        lexer->advance(); // =>
    }
    else {
        assert(false && "function should never be called in this case");
    }

    LambdaExpr::SharedPtr lambda_expr = std::make_shared<LambdaExpr>(std::make_shared<TypeRef>(new VoidType()), args);

    lexer->advance();
    Statement::SharedPtr body = parse_next_stmt();
    lambda_expr->isSingleExpr(true);

    if (body->get_type() == NodeType::COMPOUND_STMT) {
        auto cmpnd = std::static_pointer_cast<CompoundStmt>(body);
        cmpnd->returnable(true);
        lambda_expr->isSingleExpr(false);
    }

    lambda_expr->set_body(body);
    lambda_expr->setIndex(start, lexer->current_index, source_id);
    lambda_expr->setMemberExpr(__parse_identifier(false));

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
    bool typeDeclared = false;
    Token next = lexer->lookahead();
    if (next.is_operator(":")) {
        lexer->advance();
        lexer->advance();
        typeDeclared = true;

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
        lexer->advance();

        while (!lexer->current_token.is_punctuator('}')) {
            if (lexer->current_token.is_keyword("get")) {
                if (getter) {
                    ParseError::raise(ERR_UNEXPECTED_TOKEN, "Getter already declared", lexer);
                }

                getter = true;

                if (lexer->lookahead().is_punctuator('{')) {
                    auto body = parse_block();
                    field->addGetter(body);

                    lexer->advance();
                }
                else {
                    field->addGetter();
                    lexer->advance();
                }
            }
            else if (lexer->current_token.is_keyword("set")) {
                if (setter) {
                    ParseError::raise(ERR_UNEXPECTED_TOKEN, "Setter already declared", lexer);
                }

                setter = true;

                if (lexer->lookahead().is_punctuator('{')) {
                    auto body = parse_block();
                    field->addSetter(body);

                    lexer->advance();
                }
                else {
                    field->addSetter();
                    lexer->advance();
                }
            }
            else if (lexer->current_token.is_punctuator(',')) {
                lexer->advance();
            }
            else {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected 'get' or 'set'", lexer);
            }
        }

        next = lexer->lookahead();
    }

    // optional default value
    if (next.get_type() == T_OP && next.get_value().getString() == "=") {
        lexer->advance();
        field->setDefault(parse_expression());
    }
    else if (!typeDeclared) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Fields have to have an annotated type or a default value", lexer);
    }

    lexer->advance(false);
    if (!lexer->current_token.is_separator()) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Field declarations must be on seperate lines", lexer);
    }

    field->setIndex(start, lexer->current_index, source_id);
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

    string method_name = lexer->s_val();
    lexer->advance();

    // method alias
    if (lexer->current_token.is_operator("=")) {
        lexer->advance();

        if (lexer->current_token.get_type() != T_IDENT && lexer->current_token.get_type() != T_OP) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Method aliasee must be a valid identifier", lexer);
        }

        string aliasee = lexer->s_val();
        lexer->advance();

        auto args = parse_arg_list(true);

        auto op_decl = std::make_shared<MethodDecl>(method_name, aliasee, args);
        op_decl->setIndex(start, lexer->current_index, source_id);

        return op_decl;
    }

    std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list(is_interface);

    // optional return type
    Token next = lexer->lookahead();
    auto typeref = std::make_shared<TypeRef>(new VoidType);
    if (next.is_operator("->")) {
        lexer->advance();
        lexer->advance();
        typeref = parse_type();
    }


    if (lexer->lookahead().is_punctuator('{') || lexer->lookahead().is_keyword("unsafe")) {

        CompoundStmt::SharedPtr method_body = parse_block();
        method_body->returnable(true);

        lexer->advance(false);
        if (!lexer->current_token.is_separator()) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Method declarations must be on seperate lines", lexer);
        }

        auto method = std::make_shared<MethodDecl>(method_name, typeref, args, method_body, am, is_static);
        method->setIndex(start, lexer->current_index, source_id);

        return method;
    }
    else if (!is_interface) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Abstract methods can only be declared in an interface or an abstract "
                "class", lexer);
    }
    else {
        auto method = std::make_shared<MethodDecl>(method_name, typeref, args, am,
            is_static);
        method->setIndex(start, lexer->current_index, source_id);

        return method;
    }

    return nullptr;
}

namespace {

    bool isValidOperatorChar(Token& next) {
        return (
            next.get_type() != T_PUNCTUATOR &&
            next.get_type() != T_EOF
        );
    }

}

/**
 * Parses an operator method declaration
 * @param am
 * @return
 */
MethodDecl::SharedPtr Parser::parse_operator_decl(AccessModifier am, bool is_interface) {
    int start = lexer->last_token_index;
    auto opType = lexer->s_val();
    lexer->advance();

    string op;
    bool isCastOp = false;
    TypeRef::SharedPtr castTarget = nullptr;

    if (lexer->current_token.is_punctuator('(') && lexer->lookahead().is_punctuator(')')) {
        lexer->advance();
        lexer->advance();
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

        lexer->advance();
    }
    else if (lexer->current_token.is_operator("as")) {
        lexer->advance();
        castTarget = parse_type();
        isCastOp = true;

        lexer->advance();
        op = "as " + castTarget->getType(true)->toString();
    }
    else {
        while (isValidOperatorChar(lexer->current_token)) {
            op += lexer->s_val();
            lexer->advance(false, true);
        }

        if (lexer->current_token.is_punctuator(' ')) {
            lexer->advance();
        }
    }

    // method alias
    if (lexer->current_token.is_operator("=")) {
        if (opType != "infix") {
            if (opType == "prefix") {
                util::PrefixUnaryOperators.push_back(op);
            }
            else if (opType == "postfix") {
                util::PostfixUnaryOperators.push_back(op);
            }
        }
        else {
            util::binary_operators.push_back(op);
        }

        if (util::op_precedence.find(op) == util::op_precedence.end()) {
            util::op_precedence.emplace(op, util::op_precedence["="]);
        }

        lexer->advance();

        if (lexer->current_token.get_type() != T_IDENT && lexer->current_token.get_type() != T_OP) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Method aliasee must be a valid identifier", lexer);
        }

        string aliasee = lexer->s_val();
        lexer->advance();

        auto args = parse_arg_list(true);

        auto op_decl = std::make_shared<MethodDecl>(opType + " " + op, aliasee, args);
        op_decl->setIndex(start, lexer->current_index, source_id);

        return op_decl;
    }

    std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list(is_interface);

    if (args.size() > 1) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Custom operators cannot have more than one argument", lexer);
    }

    if (opType != "infix") {
        if (opType == "prefix") {
            util::PrefixUnaryOperators.push_back(op);
        }
        else if (opType == "postfix") {
            util::PostfixUnaryOperators.push_back(op);
        }
    }
    else {
        if (args.size() != 1 && !isCastOp) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected infix function to have exactly one argument", lexer);
        }

        util::binary_operators.push_back(op);
    }

    if (util::op_precedence.find(op) == util::op_precedence.end()) {
        util::op_precedence.emplace(op, util::op_precedence["="]);
    }

    // optional return type
    Token next = lexer->lookahead();
    TypeRef::SharedPtr typeref;

    if (next.is_operator("->")) {
        lexer->advance();
        lexer->advance();

        typeref = parse_type();

        if (castTarget != nullptr && *(typeref->getType(true)) != castTarget->getType(true)) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Cast operator return type differs from declared cast target",
                lexer);
        }
    }
    else {
        typeref = castTarget != nullptr ? castTarget : std::make_shared<TypeRef>(new VoidType);
        typeref->setIndex(start, lexer->current_index, source_id);
    }

    MethodDecl::SharedPtr op_decl;
    if (lexer->lookahead().is_punctuator('{') || lexer->lookahead().is_keyword("unsafe")) {
        CompoundStmt::SharedPtr body = parse_block();
        body->returnable(true);

        op_decl = std::make_shared<MethodDecl>(opType + " " + op, typeref, args, body, am, false);
    }
    else if (!is_interface) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Abstract methods can only be declared in an interface or an abstract "
            "class", lexer);
    }
    else {
        op_decl = std::make_shared<MethodDecl>(opType + " " + op, typeref, args, am, false);
    }

    op_decl->setIndex(start, lexer->current_index, source_id);
    return op_decl;
}

std::vector<GenericType*> Parser::parse_generics() {
    std::vector<GenericType*> generics;

    if (!lexer->current_token.is_operator("<")) {
        return generics;
    }

    lexer->advance();
    while (!lexer->current_token.is_operator(">")) {
        if (lexer->current_token.get_type() != T_IDENT) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected typename", lexer);
        }

        auto genericClassName = lexer->s_val();

        lexer->advance();

        // A: +B -> A is superclass of B (contravariance)
        // A: B or A: -B -> A is subclass of B (covariance)
        if (lexer->current_token.is_operator(":")) {
            lexer->advance();
            bool covariant = true;

            if (lexer->current_token.is_operator("+")) {
                covariant = false;
                lexer->advance();
            }
            else if (lexer->current_token.is_operator("-")) {
                lexer->advance();
            }

            if (lexer->current_token.get_type() != T_IDENT) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected class name", lexer);
            }

            if (covariant) {
                generics.push_back(new GenericType(std::move(genericClassName), __parse_type()));
            }
            else {
                generics.push_back(new GenericType(std::move(genericClassName), nullptr, __parse_type()));
            }

            lexer->advance();
        }
        else {
            generics.push_back(new GenericType(std::move(genericClassName), ObjectType::get("Any")));
        }

        CurrentClassGenerics.emplace(genericClassName, generics.back());

        if (!lexer->current_token.is_operator(">") && !lexer->current_token.is_punctuator(',')) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ',' or '>'", lexer);
        }
        if (lexer->current_token.is_punctuator(',')) {
            lexer->advance();
        }
    }

    lexer->advance();

    return generics;
}

std::vector<Type*> Parser::parse_concrete_generics() {
    std::vector<Type*> generics;

    if (!lexer->current_token.is_operator("<")) {
        return generics;
    }

    lexer->advance();
    while (!lexer->current_token.is_operator(">")) {
        if (lexer->current_token.get_type() != T_IDENT) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected typename", lexer);
        }

        auto type = __parse_type();
        lexer->advance();

        if (!lexer->current_token.is_operator(">") && !lexer->current_token.is_punctuator(',')) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ',' or '>'", lexer);
        }
        if (lexer->current_token.is_punctuator(',')) {
            lexer->advance();
        }

        generics.push_back(type);
    }

    return generics;
}

std::vector<Statement::SharedPtr> Parser::parse_class_inner(string class_name, bool is_abstract, bool is_extension) {
    std::vector<Statement::SharedPtr> declarations;
    std::vector<Attribute> attributes;
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
        bool memberwiseInit = false;

        attributes = parse_attributes();

        while (lexer->current_token.get_type() == T_KEYWORD && lexer->s_val() != "as") {
            std::string keyword =lexer->s_val();
            if (am_set && (keyword == "public" || keyword == "protected" || keyword == "private")) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Field access modifier already declared", lexer);
            }
            else if (is_static && keyword == "static") {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Field already declared static", lexer);
            }

            if (type != DeclType::NONE && (keyword == "let" || keyword == "def" || keyword == "init" ||
                    keyword == "delete" || keyword == "typedef")) {
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
            else if (keyword == "typedef") {
                type = DeclType::TYPEDEF;
            }
            else if (keyword == "let") {
                type = DeclType::FIELD;
            }
            else if (keyword == "def") {
                type = DeclType::METHOD;
            }
            else if (keyword == "memberwise") {
                lexer->advance();
                if (!lexer->current_token.is_keyword("init")) {
                    ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected 'init' after 'memberwise'", lexer);
                }

                type = DeclType::CONSTR;
                memberwiseInit = true;
                break;
            }
            else if (keyword == "init") {
                type = DeclType::CONSTR;
            }
            else if (keyword == "delete") {
                type = DeclType::DECONSTR;
            }
            else if (keyword == "infix" || keyword == "postfix" || keyword == "prefix") {
                break;
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
            lexer->backtrack();
            declaration_finished = true;
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

            if (memberwiseInit) {
                declarations.push_back(std::make_shared<ConstrDecl>());
                lexer->advance();
            }
            else {
                auto constr = parse_constr_decl(current_am);
                constr->setAttributes(attributes);

                declarations.push_back(constr);
            }
        }
        else if (type == DeclType::FIELD) {
            if (is_extension && !is_static) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Extensions can only contain static fields", lexer);
            }

            auto field = parse_field_decl(current_am, is_static, is_abstract);
            field->setAttributes(attributes);
            declarations.push_back(field);
        }
        else if (type == DeclType::METHOD) {
            bool isOperator = false;
            if (lexer->current_token.get_type() == T_KEYWORD) {
                auto keyword = lexer->s_val();
                if (keyword == "infix" || keyword == "prefix" || keyword == "postfix") {
                    isOperator = true;
                }
            }

            if (isOperator) {
                auto op = parse_operator_decl(current_am, is_abstract);
                op->setAttributes(attributes);

                declarations.push_back(op);
            }
            else {
                auto method = parse_method_decl(current_am, is_static, is_abstract);
                method->setAttributes(attributes);

                declarations.push_back(method);
            }
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

ClassHead Parser::parse_class_head() {
    AccessModifier am = AccessModifier::PRIVATE;
    bool am_set = false;
    bool isAbstract = false;

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
        else if (lexer->s_val() == "abstract") {
            if (isAbstract) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Class already declared abstract", lexer);
            }

            isAbstract = true;
        }
        else {
            break;
        }
    }

    lexer->advance();
    if (lexer->current_token.get_type() != T_IDENT) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected class name", lexer);
    }

    std::string class_name = lexer->s_val();
    namespaces.push_back(class_name);

    type_names.push_back(class_name);

    ObjectType* extends = nullptr;
    std::vector<ObjectType*> with;

    lexer->advance();
    auto generics = parse_generics();

    while (!lexer->current_token.is_punctuator('{') && lexer->current_token.get_type() != T_EOF) {
        if (lexer->current_token.is_operator(":")) {
            if (extends != nullptr) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Classes can only inherit from one other class", lexer);
            }

            lexer->advance();
            if (lexer->current_token.get_type() != T_IDENT) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected identifier after ':'", lexer);
            }

            auto ext = __parse_type();
            if (!isa<ObjectType>(ext)) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Only object types can be extended", lexer);
            }

            extends = cast<ObjectType>(ext);
            lexer->advance();
        }
        else if (lexer->current_token.is_keyword("with")) {
            lexer->advance();
            if (lexer->current_token.get_type() != T_IDENT) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected identifier after 'with'", lexer);
            }

            while (lexer->current_token.get_type() == T_IDENT && lexer->current_token.get_type() != T_EOF) {
                auto with_ = __parse_type();
                if (!isa<ObjectType>(with_)) {
                    ParseError::raise(ERR_UNEXPECTED_TOKEN, "Only object types can be extended", lexer);
                }

                with.push_back(cast<ObjectType>(with_));
                lexer->advance();

                if (lexer->current_token.is_punctuator(',')) {
                    lexer->advance();
                }
            }
        }
        else {
            lexer->backtrack();
            break;
        }
    }

    return ClassHead { am, class_name, extends, with, generics, isAbstract };
}

/**
 * Parses a class declaration
 * @return
 */
ClassDecl::SharedPtr Parser::parse_class_decl(bool isProtocol) {
    int start = lexer->last_token_index;

    auto attrs = attributes;
    auto head = parse_class_head();

    if (isProtocol && head.isAbstract) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Protocols cannot be declared abstract", lexer);
    }
    if (isProtocol && head.extends != nullptr) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Protocols cannot inherit from classes or conform to protocols",
            lexer);
    }
    if (!lexer->current_token.is_punctuator('{')) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '{' to start a class definition", lexer);
    }

    std::vector<ConstrDecl::SharedPtr> constructors;
    std::vector<FieldDecl::SharedPtr> fields;
    std::vector<MethodDecl::SharedPtr> methods;
    std::vector<TypedefDecl::SharedPtr> typedefs;

    auto declarations = parse_class_inner(head.class_name, isProtocol);
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

    CurrentClassGenerics.clear();

    ClassDecl::SharedPtr class_decl;
    if (isProtocol) {
        class_decl = std::make_shared<ClassDecl>(head.class_name, std::move(fields), std::move(methods),
            std::move(constructors), std::move(typedefs), std::move(head.generics), head.am, std::move(head.with)
        );
    }
    else {
        class_decl = std::make_shared<ClassDecl>(head.class_name, std::move(fields), std::move(methods),
            std::move(constructors), std::move(typedefs), std::move(head.generics), head.am, head.isAbstract,
            head.extends, std::move(head.with)
        );
    }

    class_decl->setAttributes(attrs);
    attributes.clear();

    class_decl->setIndex(start, lexer->current_index, source_id);
    class_declarations.push_back(class_decl);

    return class_decl;
}

/**
 * Parses a struct definition
 * @return
 */
ClassDecl::SharedPtr Parser::parse_struct_decl() {
    auto cl = parse_class_decl();
    if (cl->getParentClass() != nullptr) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Structs cannot extend other structs or classes", lexer);
    }

    cl->isStruct(true);

    return cl;
}

ClassDecl::SharedPtr Parser::parse_extend_stmt() {
    auto cl = parse_class_decl();
    if (cl->getParentClass() != nullptr) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Extensions cannot add base classes", lexer);
    }

    cl->isExtension(true);

    return cl;
}

/**
 * Parses an if/else statement
 * @return
 */
IfStmt::SharedPtr Parser::parse_if_stmt() {
    int start = lexer->last_token_index;

    Token next = lexer->lookahead();
    if (!next.is_punctuator('(')) {
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
    if (next.get_type() == T_KEYWORD && next.get_value().getString() == "else") {
        lexer->advance();
        lexer->advance();

        CompoundStmt::SharedPtr else_branch = std::static_pointer_cast<CompoundStmt>(parse_next_stmt());
        if_stmt->setElseBranch(else_branch);
    }

    if_stmt->setIndex(start, lexer->current_index, source_id);

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
    }

    case_stmt->setIndex(start, lexer->current_index, source_id);

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
            switch_stmt->addCase(case_stmt);
        }
    }

    switch_stmt->setIndex(start, lexer->current_index, source_id);

    return switch_stmt;
}

WhileStmt::SharedPtr Parser::parse_while_stmt() {
    int start = lexer->last_token_index;

    Token next = lexer->lookahead();
    if (!next.is_punctuator('(')) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '(' after while keyword",
                lexer);
    }

    Expression::SharedPtr while_cond = parse_expression();

    lexer->advance();

    CompoundStmt::SharedPtr while_block = std::static_pointer_cast<CompoundStmt>(parse_next_stmt());
    WhileStmt::SharedPtr while_stmt = std::make_shared<WhileStmt>(while_cond, while_block);

    while_block->setIndex(start, lexer->current_index, source_id);

    return while_stmt;
}

ForStmt::SharedPtr Parser::parse_for_stmt() {
    int start = lexer->last_token_index;

    Token next = lexer->lookahead();
    if (!next.is_punctuator('(')) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '(' after for keyword",
                lexer);
    }

    lexer->advance();
    lexer->advance();

    Statement::SharedPtr init;
    if (lexer->current_token.is_punctuator(';')) {
        init = nullptr;
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

        Expression::SharedPtr range = parse_expression();
        lexer->advance();
        if (!lexer->current_token.is_punctuator(')')) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ')' after loop arguments");
        }

        lexer->advance();
        CompoundStmt::SharedPtr block = std::static_pointer_cast<CompoundStmt>(parse_next_stmt());

        return nullptr;
    }

    if (!lexer->current_token.is_punctuator(';')) {
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

    if (!lexer->current_token.is_punctuator(';')) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected ';' to seperate for loop arguments",
                lexer);
    }

    Statement::SharedPtr inc;
    lexer->advance();
    if (lexer->current_token.is_punctuator(')')) {
        inc = nullptr;
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
    for_stmt->setBody(block);
    for_stmt->setIndex(start, lexer->current_index, source_id);

    return for_stmt;
}

Statement::SharedPtr Parser::parse_declare_stmt() {
    int start = lexer->last_token_index;

    if (lexer->lookahead().is_punctuator('{')) {
        lexer->advance();
        return parse_multiple_declare_stmt();
    }

    lexer->advance();
    if (lexer->current_token.get_type() != T_KEYWORD) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected 'def', 'let', 'const' or 'class'", lexer);
    }

    string keyword = lexer->s_val();

    DeclareStmt::SharedPtr decl = nullptr;

    if (keyword == "def") {
        auto func = parse_function_decl(true);
        decl = std::make_shared<DeclareStmt>(func->getName(), func->getReturnType(), func->getArgs(),
            func->getGenerics());
    }
    else if (keyword == "class") {
        auto head = parse_class_head();
        decl = std::make_shared<DeclareStmt>(head.am, head.class_name, head.extends, head.with, false, head.generics);
    }
    else if (keyword == "let" || keyword == "const") {
        auto assign = std::static_pointer_cast<DeclStmt>(parse_assignment(keyword == "const", true));
        decl = std::make_shared<DeclareStmt>(assign->getIdentifier(), assign->getType(), keyword == "const");
    }
    else {
        assert(false && "Should not happen"); // TODO
    }

    decl->setIndex(start, lexer->current_index, source_id);
    return decl;
}

CompoundStmt::SharedPtr Parser::parse_multiple_declare_stmt() {
    int start = lexer->last_token_index;
    CompoundStmt::SharedPtr declarations = std::make_shared<CompoundStmt>(true);

    while (!lexer->lookahead().is_punctuator('}') && lexer->current_token.get_type() != T_EOF) {
        auto declare = parse_declare_stmt();
        declarations->addStatement(declare);
    }

    lexer->advance();

    declarations->setIndex(start, lexer->current_index, source_id);
    return declarations;
}

/**
 * Interprets a keyword statement
 */
Statement::SharedPtr Parser::parse_keyword() {
    int start = lexer->last_token_index;

    std::string keyword = lexer->current_token.get_value().getString();

    if (keyword == "let" || keyword == "const") {
        auto assignment = parse_assignment(keyword == "const");

        return assignment;
    }
    else if (keyword == "def") {
        FunctionDecl::SharedPtr fun_dec = parse_function_decl();
        fun_dec->setIndex(start, lexer->current_index, source_id);

        return fun_dec;
    }
    else if (keyword == "if") {
        IfStmt::SharedPtr if_stmt = parse_if_stmt();
        if_stmt->setIndex(start, lexer->current_index, source_id);

        if (top_level) {
            implicit_main_stmts.push_back(if_stmt);
            return nullptr;
        }

        return if_stmt;
    }
    else if (keyword == "while") {
        WhileStmt::SharedPtr while_stmt = parse_while_stmt();
        while_stmt->setIndex(start, lexer->current_index, source_id);

        if (top_level) {
            implicit_main_stmts.push_back(while_stmt);
            return nullptr;
        }

        return while_stmt;
    }
    else if (keyword == "switch") {
        auto switch_stmt = parse_switch_stmt();
        switch_stmt->setIndex(start, lexer->current_index, source_id);

        if (top_level) {
            implicit_main_stmts.push_back(switch_stmt);
            return nullptr;
        }

        return switch_stmt;
    }
    else if (keyword == "case") {
        auto case_stmt = parse_case_stmt();
        case_stmt->setIndex(start, lexer->current_index, source_id);

        if (top_level) {
            implicit_main_stmts.push_back(case_stmt);
            return nullptr;
        }

        return case_stmt;
    }
    else if (keyword == "default") {
        auto def_stmt = parse_case_stmt(true);
        def_stmt->setIndex(start, lexer->current_index, source_id);

        if (top_level) {
            implicit_main_stmts.push_back(def_stmt);
            return nullptr;
        }

        return def_stmt;
    }
    else if (keyword == "for") {
        ForStmt::SharedPtr for_stmt = parse_for_stmt();
        for_stmt->setIndex(start, lexer->current_index, source_id);

        if (top_level) {
            implicit_main_stmts.push_back(for_stmt);
            return nullptr;
        }

        return for_stmt;
    }
    else if (keyword == "continue") {
        ContinueStmt::SharedPtr cont_stmt = std::make_shared<ContinueStmt>();
        cont_stmt->setIndex(start, lexer->current_index, source_id);

        if (top_level) {
            implicit_main_stmts.push_back(cont_stmt);
            return nullptr;
        }

        return cont_stmt;
    }
    else if (keyword == "typedef") {
        lexer->advance();

        auto origin = parse_type();

        lexer->advance(T_IDENT);
        std::string alias = lexer->s_val();

        return std::make_shared<TypedefDecl>(alias, origin);
    }
    else if (keyword == "unsafe") {
        lexer->advance();
        auto block = parse_block();
        block->isUnsafe(true);

        return block;
    }
    else if (keyword == "break") {
        BreakStmt::SharedPtr break_stmt = std::make_shared<BreakStmt>();
        break_stmt->setIndex(start, lexer->current_index, source_id);

        if (top_level) {
            implicit_main_stmts.push_back(break_stmt);
            return nullptr;
        }

        return break_stmt;
    }
    else if (keyword == "declare") {
        return parse_declare_stmt();
    }
    else if (keyword == "class" || keyword == "public" || keyword == "private" || keyword == "abstract") {
        return parse_class_decl();
    }
    else if (keyword == "struct") {
        return parse_struct_decl();
    }
    else if (keyword == "protocol") {
        return parse_class_decl(true);
    }
    else if (keyword == "extend") {
        return parse_extend_stmt();
    }
    else if (keyword == "return") {
        Token next = lexer->lookahead(false);
        if (!next.is_separator()) {
            Expression::SharedPtr expr = parse_expression();
            ReturnStmt::SharedPtr return_stmt = std::make_shared<ReturnStmt>(expr);
            return_stmt->setIndex(start, lexer->current_index, source_id);

            if (top_level) {
                implicit_main_stmts.push_back(return_stmt);
                return nullptr;
            }

            return return_stmt;
        }
        else {
            ReturnStmt::SharedPtr return_stmt = std::make_shared<ReturnStmt>();
            return_stmt->setIndex(start, lexer->current_index, source_id);
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
        goto_stmt->setIndex(start, lexer->current_index, source_id);

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
        export_stmt->setIndex(start, lexer->current_index, source_id);

        return export_stmt;
    }
    else if (keyword == "using") {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Keyword '" + keyword + "' is only allowed at the beginning of a "
                "file", lexer);
    }
    else if (keyword == "__debug") {
        return std::make_shared<DebugStmt>();
    }
    else {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "'" + keyword + "' is a reserved keyword", lexer);
    }

    return nullptr;
}

/**
 * Parses a function call in the form func(arg1, arg2, ...)
 * @return
 */
CallExpr::SharedPtr Parser::parse_function_call() {
    int start = lexer->last_token_index;

    lexer->advance();
    auto generics = parse_concrete_generics();

    if (!generics.empty()) {
        lexer->advance();
    }

    if (!lexer->current_token.is_punctuator('(')) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '(' before function arguments", lexer);
    }

    CallExpr::SharedPtr call = std::make_shared<CallExpr>(CallType::FUNC_CALL, parse_arguments());
    call->setIndex(start, lexer->current_index, source_id);
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

std::vector<Attribute> Parser::parse_attributes() {
    std::vector<Attribute> attributes;
    std::vector<string> foundAttrs;

    while (lexer->current_token.is_punctuator('@')) {
        lexer->advance();
        if (lexer->current_token.get_type() != T_IDENT) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected attribute name", lexer);
        }

        Attribute attr;
        attr.name = lexer->s_val();
        if (std::find(foundAttrs.begin(), foundAttrs.end(), attr.name) != foundAttrs.end()) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Duplicate definition of attribute " + attr.name, lexer);
        }

        attr.kind = AttributeMap[attr.name];

        if (lexer->lookahead().is_punctuator('(')) {
            lexer->advance();
            lexer->advance();

            while (!lexer->current_token.is_punctuator(')') && lexer->current_token.get_type() != T_EOF) {
                if (lexer->current_token.get_type() != T_IDENT) {
                    ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected attribute argument", lexer);
                }

                attr.args.push_back(lexer->s_val());
                if (lexer->lookahead().is_punctuator(',')) {
                    lexer->advance();
                }

                lexer->advance();
            }
        }

        auto check = isValidAttribute(attr);
        if (!check.empty()) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, check, lexer);
        }

        attributes.push_back(attr);
        foundAttrs.push_back(attr.name);
        lexer->advance();
    }

    return attributes;
}

CompoundStmt::SharedPtr Parser::parse_block(bool preserveTopLevel) {
    int start = lexer->last_token_index;
    bool last_top_level = top_level;
    bool unsafe = false;

    if (!preserveTopLevel) {
        top_level = false;
    }

    lexer->advance();

    if (lexer->current_token.get_type() == T_KEYWORD && lexer->s_val() == "unsafe") {
        lexer->advance();
        unsafe = true;
    }

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

        block->addStatement(stmt);

        lexer->advance();
    }

    if (lexer->current_token.get_type() != T_EOF && !lexer->current_token.is_punctuator('}')) {
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected '}' to end a block statement", lexer);
    }

    block->setIndex(start, lexer->current_index, source_id);
    block->isUnsafe(unsafe);

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
        ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected identifier after 'namespace'", lexer);
    }

    auto ns_name = lexer->s_val();
    while (lexer->lookahead().is_punctuator('.')) {
        lexer->advance();
        lexer->advance();

        if (lexer->current_token.get_type() != T_IDENT) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected identifier after 'namespace'", lexer);
        }

        ns_name += "." + lexer->s_val();
    }

    auto mod = std::make_shared<NamespaceDecl>(ns_name, parse_block(true));
    mod->setIndex(start, lexer->current_index, source_id);

    namespaces.push_back(ns_name);
    return mod;
}

/**
 * Parses a module import statement
 * @return
 */
UsingStmt::SharedPtr Parser::parse_import_stmt() {
    int start = lexer->last_token_index;
    lexer->advance();

    string ns_name = lexer->s_val();

    while (lexer->lookahead().is_punctuator('.')) {
        lexer->advance();
        lexer->advance();
        if (lexer->current_token.get_type() != T_IDENT) {
            ParseError::raise(ERR_UNEXPECTED_TOKEN, "Expected identifier", lexer);
        }

        ns_name += "." + lexer->s_val();
    }

    UsingStmt::SharedPtr imp_stmt = std::make_shared<UsingStmt>(ns_name);
    imp_stmt->setIndex(start, lexer->current_index, source_id);

    return imp_stmt;
}

/**
 * Parses an export statement
 * @return
 */
EndOfFileStmt::SharedPtr Parser::parse_export_stmt() {
    return nullptr;
}

/**
 * Runs the program by tokenizing the program, creating the AST and finally evaluating it.
 * @return
 */
void Parser::run(bool debug = false) {
    CompoundStmt::SharedPtr root = std::make_shared<CompoundStmt>();

    for (const auto& _import : util::stdLibImports) {
        string file_path = __FILE__;
        string dir_path = file_path.substr(0, file_path.rfind('/')) + "/StdLib/" + _import + ".dot";
        std::ifstream ifs(dir_path);
        std::string content((std::istreambuf_iterator<char>(ifs)), (std::istreambuf_iterator<char>()));

        auto Import = Parser(content).parse();
        root->addStatements(Import->getStatements());
        root->addStatement(std::make_shared<EndOfFileStmt>());
    }

    auto stmts = parse();
    if (lib_imports_disabled) {
        root->clear();
    }

    root->addStatements(stmts->getStatements());

    if (!main_method_defined) {
        auto Main = std::make_shared<FunctionDecl>("main");
        Main->setReturnType(std::make_shared<TypeRef>(new VoidType));

        auto body = std::make_shared<CompoundStmt>();
        body->returnable(true);
        body->addStatements(implicit_main_stmts);
        Main->setBody(body);

        root->addStatement(Main);
    }
    else {
        for (const auto& stmt : implicit_main_stmts) {
            if (stmt->get_type() != NodeType::DECLARATION) {
                ParseError::raise(ERR_UNEXPECTED_TOKEN, "Cannot define a main method when top-level non-declaration "
                    "statements exist", lexer);
            }
        }

        root->insertAtBegin(implicit_main_stmts);
    }

    delete lexer;

    TypeCheckVisitor::connectTree(root.get());

    TypeCheckVisitor tc;
    CodeGenVisitor cg;

    tc.DeclareClasses(root);
    tc.visit(root.get());

    if (debug) {
        root->__dump(0);
        std::cout << std::endl << std::endl;
    }

    cg.DeclareClasses(root);
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
        root->addStatement(parse_import_stmt());
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
            root->addStatement(stmt);
        }

        lexer->advance();
    }

    return root;
}