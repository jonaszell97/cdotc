//
// Created by Jonas Zell on 18.06.17.
//

#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "Token.h"
#include "Tokenizer.h"
#include "AST/Statement/CompoundStmt.h"
#include "AST/Expression/Expression.h"
#include "AST/Operator/UnaryOperator.h"
#include "AST/Expression/Literal/LiteralExpr.h"
#include "AST/Expression/Literal/ObjectLiteral.h"
#include "AST/Expression/Literal/ArrayLiteral.h"
#include "AST/Expression/RefExpr/IdentifierRefExpr.h"
#include "AST/Expression/RefExpr/CallExpr.h"
#include "Util.h"
#include "AST/Statement/Function/FunctionDecl.h"
#include "AST/Statement/ControlFlow/IfStmt.h"
#include "AST/Statement/ControlFlow/WhileStmt.h"
#include "AST/Statement/ControlFlow/ForStmt.h"
#include "AST/Operator/TertiaryOperator.h"

class Tokenizer;

class Interpreter {
public:
    Interpreter(std::string);
    void run(bool);
    friend class Tokenizer;

protected:
    std::unique_ptr<Tokenizer> tokenizer;
    CompoundStmt::SharedPtr prog_root;

    void token_error();
    void token_error(TokenType, TokenType);

    CompoundStmt::SharedPtr parse();
    Statement::SharedPtr parse_next_stmt(bool = false);

    Statement::SharedPtr parse_assignment(bool, CompoundStmt::SharedPtr = {});
    AstNode::SharedPtr parse_keyword();
    Expression::SharedPtr parse_expression(Expression::SharedPtr = {}, int = 0);
    CompoundStmt::SharedPtr parse_block();

    TertiaryOperator::SharedPtr parse_tertiary_operator(Expression::SharedPtr);

    ObjectLiteral::SharedPtr parse_object_literal();
    ArrayLiteral::SharedPtr parse_array_literal();

    FunctionDecl::SharedPtr parse_function_decl();
    CallExpr::SharedPtr parse_function_call();

    TypeSpecifier parse_type();
    RefExpr::SharedPtr parse_identifier();
    RefExpr::SharedPtr __parse_identifier();

    Expression::SharedPtr parse_unary_expr(Expression::SharedPtr = {}, bool = false);
    Expression::SharedPtr parse_unary_expr_target();

    IfStmt::SharedPtr parse_if_stmt();
    WhileStmt::SharedPtr parse_while_stmt();
    ForStmt::SharedPtr parse_for_stmt();
};

#endif //INTERPRETER_H
