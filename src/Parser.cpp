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
#include "AST/Expression/Literal/NumericLiteral.h"
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
#include "AST/Visitor/StaticAnalysis/TypeCheckPass.h"
#include "AST/SymbolTable.h"
#include "AST/Visitor/CodeGen/CodeGen.h"
#include "Message/Warning.h"
#include "AST/Statement/Declaration/TypedefDecl.h"
#include "Variant/Type/Type.h"
#include "Variant/Type/PointerType.h"
#include "Variant/Type/IntegerType.h"
#include "Variant/Type/FPType.h"
#include "Variant/Type/FunctionType.h"
#include "Variant/Type/CollectionType.h"
#include "Variant/Type/ObjectType.h"
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
#include "AST/Statement/ControlFlow/IfStmt.h"
#include "AST/Statement/ControlFlow/CaseStmt.h"
#include "AST/Statement/ControlFlow/MatchStmt.h"
#include "AST/Statement/ControlFlow/WhileStmt.h"
#include "AST/Statement/ControlFlow/ForStmt.h"
#include "AST/Statement/Declaration/DeclareStmt.h"
#include "Variant/Type/TupleType.h"
#include "Variant/Type/AutoType.h"
#include "AST/Expression/Literal/TupleLiteral.h"
#include "AST/Statement/Declaration/Class/EnumCaseDecl.h"
#include "AST/Statement/Declaration/Class/EnumDecl.h"
#include "AST/Expression/Literal/NoneLiteral.h"
#include "AST/Statement/ControlFlow/ForInStmt.h"
#include "AST/Statement/Declaration/Class/DestrDecl.h"
#include "AST/Expression/Literal/StringInterpolation.h"

std::vector<string> Parser::type_names = {};
std::vector<string> Parser::namespaces = {};
std::vector<pair<string, string>> Parser::source_files = {};
unordered_map<string, ObjectType*>  Parser::CurrentClassGenerics =  {};
std::vector<ClassDecl::SharedPtr> Parser::class_declarations = {};
bool Parser::main_method_defined = false;

/**
 * Creates a new interpreter for an Xtreme Jonas Script program.
 * @param program
 */
Parser::Parser(string &fileName, string &src) :
   lexer(new Lexer(src, fileName)),
   source_id(source_files.size())
{
   source_files.emplace_back(fileName, src);
}

Parser::~Parser() {
   delete lexer;
}


TypeRef::SharedPtr Parser::parse_type() {
   int start = lexer->current_index;

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
      lexer->advance();

      if (lexer->current_token.is_operator(":")) {
         lexer->advance();

         auto valType = __parse_type();
         lexer->advance();

         if (!lexer->current_token.is_punctuator(']')) {
            ParseError::raise("Expected ']' after dictionary type", lexer);
         }

         auto dictType = new CollectionType(elType, valType);
         dictType->isLvalue(isReference);

         return dictType;
      }

      if (!lexer->current_token.is_punctuator(']')) {
         ParseError::raise("Expected ']' after array type", lexer);
      }

      Type* arrType = new CollectionType(elType);
      arrType->isLvalue(isReference);

      return arrType;
   }

   // function type
   if (lexer->current_token.is_punctuator('(')) {
      auto argTypes = parse_tuple_type();

      lexer->advance();
      if (!lexer->current_token.is_operator("->")) {
         if (argTypes.size() == 1) {
            return argTypes.front().second->getType(true);
         }

         Type* tuple = new TupleType(argTypes);
         tuple->isLvalue(isReference);

         lexer->backtrack();
         return tuple;
      }

      lexer->advance();
      auto returnType = parse_type();

      Type* fun = new FunctionType(returnType, argTypes);
      fun->isLvalue(isReference);

      return fun;
   }


   string typeName = lexer->s_val();
   Type* type;

   while (lexer->lookahead(false).is_punctuator('.')) {
      lexer->advance();
      lexer->advance();
      if (lexer->current_token.get_type() != T_IDENT) {
         ParseError::raise("Unexpected character in type reference", lexer);
      }

      typeName += "." + lexer->s_val();
   }


   if (CurrentClassGenerics.find(typeName) != CurrentClassGenerics.end()) {
      type = CurrentClassGenerics[typeName]->deepCopy();
   }
   else {
      type = ObjectType::get(typeName);
   }

   Token next = lexer->lookahead(false);

   // generic type requirements
   if (next.is_operator("<")) {
      if (!isa<ObjectType>(type)) {
         ParseError::raise("Generic class names are only allowed after object types", lexer);
      }

      lexer->advance();

      auto asObj = cast<ObjectType>(type);
      auto generics = parse_concrete_generics();

      asObj->setUnqualGenerics(generics);
      next = lexer->lookahead(false);
   }

   // optional type
   if (next.is_operator("?")) {
      lexer->advance();

      auto opt = ObjectType::get("Option");
      opt->isEnum(true);
      opt->getConcreteGenericTypes().emplace("T", type);

      return opt;
   }

   // pointer type
   while (next.get_type() == T_OP) {
      auto op = next.get_value().strVal;
      if (util::matches("\\*+", op)) {
         while (op.length() > 0) {
            type = new PointerType(type);
            op = op.substr(0, op.length() - 1);
         }

         lexer->advance();
         next = lexer->lookahead(false);
      }
      else {
         break;
      }
   }

   type->isLvalue(isReference);

   return type;
}

/**
 * Recursively parses an identifier. Can be a combination of a variable, property accesses,
 * array indices and method calls
 * e.g. foo.bar[3](), foo.bar, foo, foo.baz()[3]
 * @param ident_expr
 * @return
 */
Expression::SharedPtr Parser::parse_identifier()
{
   int start = lexer->current_index;
   string ident = lexer->s_val();

   IdentifierRefExpr::SharedPtr ident_expr = std::make_shared<IdentifierRefExpr>(ident);

   ident_expr->setMemberExpr(try_parse_member_expr());
   ident_expr->setIndex(start, lexer->current_index, source_id);

   return ident_expr;
}

std::shared_ptr<Expression> Parser::try_parse_member_expr()
{
   int start = lexer->current_index;
   lexer->advance(false);

   // member access
   bool pointerAccess = lexer->current_token.is_operator("->");
   if (lexer->current_token.is_punctuator('.') || pointerAccess) {
      lexer->advance(false);

      // tuple access
      if (lexer->current_token.get_type() == T_LITERAL) {
         size_t index = lexer->current_token.get_value().intVal;

         MemberRefExpr::SharedPtr mem_ref = std::make_shared<MemberRefExpr>(index, pointerAccess);
         mem_ref->setMemberExpr(try_parse_member_expr());
         mem_ref->setIndex(start, lexer->current_index, source_id);

         return mem_ref;
      }

      // method call
      Token next = lexer->lookahead();
      if (next.is_punctuator('(') || is_generic_call()) {
         auto calltype = std::find(namespaces.begin(), namespaces.end(), lexer->s_val()) != namespaces.end() ?
                         CallType::FUNC_CALL : CallType::METHOD_CALL;

         auto call = parse_function_call(calltype);
         call->isPointerAccess(pointerAccess);

         return call;
      }

      string ident = lexer->s_val();

      MemberRefExpr::SharedPtr mem_ref = std::make_shared<MemberRefExpr>(ident, pointerAccess);
      mem_ref->setMemberExpr(try_parse_member_expr());
      mem_ref->setIndex(start, lexer->current_index, source_id);

      return mem_ref;
   }

   // array access
   if (lexer->current_token.is_punctuator('[')) {
      lexer->advance();

      Expression::SharedPtr expr = parse_expression();
      expr->setIndex(start, lexer->current_index, source_id);

      SubscriptExpr::SharedPtr arr_acc = std::make_shared<SubscriptExpr>(expr);

      lexer->advance();
      if (!lexer->current_token.is_punctuator(']')) {
         ParseError::raise("Expected ']'", lexer);
      }

      arr_acc->setMemberExpr(try_parse_member_expr());
      arr_acc->setIndex(start, lexer->current_index, source_id);

      return arr_acc;
   }

   lexer->backtrack();
   return nullptr;
}

/**
 * Parses an array literal in the form of
 * [val1, val2, val3, ..., valn]
 * @return
 */
CollectionLiteral::SharedPtr Parser::parse_collection_literal()
{
   int start = lexer->current_index;

   bool isDictionary = false;
   bool first = true;
   std::vector<Expression::SharedPtr> keys;
   std::vector<Expression::SharedPtr> values;

   while (!lexer->current_token.is_punctuator(']')) {
      if (lexer->lookahead().is_punctuator(']')) {
         lexer->advance();
         break;
      }

      if (lexer->lookahead().is_punctuator(':')) {
         isDictionary = true;
         lexer->advance();
         continue;
      }

      lexer->advance();

      auto key = parse_expression();
      lexer->advance();

      if (lexer->current_token.is_operator(":")) {
         if (!first && !isDictionary) {
            ParseError::raise("Unexpected token ':'", lexer);
         }

         lexer->advance();
         auto value = parse_expression();
         keys.push_back(key);
         values.push_back(value);

         isDictionary = true;
      }
      else {
         if (isDictionary) {
            ParseError::raise("Expected ':'", lexer);
         }

         values.push_back(key);
      }

      first = false;
   }

   if (!lexer->current_token.is_punctuator(']')) {
      ParseError::raise("Expected ']' after array literal", lexer);
   }

   if (isDictionary) {
      auto dict = std::make_shared<CollectionLiteral>(keys, values);
      dict->setIndex(start, lexer->current_index, source_id);

      return dict;
   }

   auto arr = std::make_shared<CollectionLiteral>(values);
   arr->setIndex(start, lexer->current_index, source_id);

   return arr;
}

/**
 * Parses an atomic part of an expression, like a single identifier or literal
 * @return
 */
Expression::SharedPtr Parser::parse_unary_expr_target()
{
   std::vector<Attribute> attributes;

   if (lexer->current_token.is_punctuator('@')) {
      attributes = parse_attributes();
   }

   if (lexer->current_token.is_punctuator('[')) {
      auto arr = parse_collection_literal();
      arr->setAttributes(attributes);

      return arr;
   }
   if (lexer->current_token.is_punctuator('(')) {
      auto expr = parse_paren_expr();
      expr->setAttributes(attributes);
      expr->setMemberExpr(try_parse_member_expr());

      return expr;
   }
   // enum case with inferred type
   if (lexer->current_token.is_punctuator('.')) {
      int start = lexer->current_index;
      lexer->advance(T_IDENT);

      auto caseName = lexer->s_val();

      if (!lexer->lookahead().is_punctuator('(')) {
         auto expr = std::make_shared<MemberRefExpr>(caseName);
         expr->setAttributes(attributes);
         expr->setIndex(start, lexer->current_index, source_id);
         expr->isEnumCase_(true);

         return expr;
      }
      else {
         auto call = parse_function_call(CallType::METHOD_CALL, true);
         call->isEnumCase_(true);

         return call;
      }
   }
   if (lexer->current_token.get_type() == T_IDENT) {
      int start = lexer->current_index;
      Token next = lexer->lookahead(false);

      // function call
      if (next.is_punctuator('(') || is_generic_call()) {
         return parse_function_call(CallType::FUNC_CALL);
      }

      // single argument lambda
      if (next.is_operator("=>")) {
         auto argName = lexer->s_val();
         auto arg = std::make_shared<FuncArgDecl>(argName, std::make_shared<TypeRef>(new AutoType));
         arg->setIndex(start, lexer->current_index, source_id);
         start = lexer->current_index;

         lexer->advance();
         lexer->advance();

         auto body = parse_next_stmt();
         auto lambda = std::make_shared<LambdaExpr>(std::make_shared<TypeRef>(new AutoType),
            std::vector<FuncArgDecl::SharedPtr>{ arg }, body);
         lambda->setIndex(start, lexer->current_index, source_id);
         lambda->setMemberExpr(try_parse_member_expr());

         return lambda;
      }

      auto ident = parse_identifier();
      ident->setAttributes(attributes);

      return ident;
   }
   if (lexer->current_token.get_type() == T_LITERAL) {
      int start = lexer->last_token_index;
      auto val = lexer->current_token.get_value();

      if (val.type == VariantType::STRING) {
         Token next = lexer->lookahead(false);
         auto str = lexer->s_val();
         auto interpolations = util::findInterpolations(str);

         // possible string modifier
         if (!interpolations.empty()) {
            Lexer *lex = new Lexer;
            size_t i = 0;
            auto originalLexer = lexer;
            bool currentIsString = true;
            std::vector<Expression::SharedPtr> strings;

            for (auto& curr : interpolations) {
               if (currentIsString) {
                  strings.push_back(std::make_shared<StringLiteral>(curr.second));
                  currentIsString = false;
                  continue;
               }

               lex->reset(curr.second, originalLexer->fileName);
               lex->advance();

               lexer = lex;
               auto expr = parse_expression();
               expr->setIndex(start + curr.first, start + curr.first + curr.second.length(), source_id);
               strings.push_back(expr);

               ++i;
               currentIsString = true;
            }

            delete lex;
            lexer = originalLexer;

            auto interp = std::make_shared<StringInterpolation>(std::move(strings));
            interp->setIndex(start, lexer->current_index, source_id);
            interp->setMemberExpr(try_parse_member_expr());
            interp->setAttributes(attributes);

            return interp;
         }
         else {
            auto string_literal = std::make_shared<StringLiteral>(str);
            string_literal->setIndex(start, lexer->current_index, source_id);
            string_literal->setMemberExpr(try_parse_member_expr());
            string_literal->setAttributes(attributes);

            return string_literal;
         }
      }
      else if (val.type == VariantType::VOID) {
         auto noneLiteral = std::make_shared<NoneLiteral>();
         noneLiteral->setIndex(start, lexer->current_index, source_id);
         noneLiteral->setAttributes(attributes);

         return noneLiteral;
      }

      auto expr = std::make_shared<NumericLiteral>(val);
      expr->setMemberExpr(try_parse_member_expr());
      expr->setIndex(start, lexer->current_index, source_id);
      expr->setAttributes(attributes);

      return expr;
   }
   else {
      ParseError::raise("Expected expression but found " +
               util::token_names[lexer->current_token.get_type()], lexer);
   }

   return nullptr;
}

namespace {

   bool isPrefixUnaryOp(Token& next) {
      return next.get_type() == T_OP &&
         util::in_vector(util::PrefixUnaryOperators, next.get_value().strVal);
   }

   bool isPostfixUnaryOp(Token& next) {
      return next.get_type() == T_OP &&
         util::in_vector(util::PostfixUnaryOperators, next.get_value().strVal);
   }
}

/**
 * Recursively parses a unary expression for example an identifier or a literal with unary expressions
 * applied to it, e.g.
 *     - ++3
 *     - x.foo()++
 *     - !bar[3]
 * @return
 */
Expression::SharedPtr Parser::parse_unary_expr(UnaryOperator::SharedPtr literal, bool postfix)
{
   int start = lexer->current_index;

   // prefix unary op
   if (isPrefixUnaryOp(lexer->current_token)) {
      UnaryOperator::SharedPtr unary_op = std::make_shared<UnaryOperator>(lexer->s_val(), "prefix");
      lexer->advance();

      unary_op->setTarget(parse_unary_expr());
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

   Token next = lexer->lookahead(false);

   // postfix unary op
   if (isPostfixUnaryOp(next)) {
      lexer->advance();

      UnaryOperator::SharedPtr unary_op = std::make_shared<UnaryOperator>(lexer->s_val(), "postfix");
      unary_op->setMemberExpr(try_parse_member_expr());
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

TertiaryOperator::SharedPtr Parser::parse_tertiary_operator(Expression::SharedPtr cond)
{
   int start = lexer->current_index;

   lexer->advance();
   Expression::SharedPtr if_branch = parse_expression();

   lexer->advance();
   if (!lexer->current_token.is_operator(":")) {
      ParseError::raise("Expected ':' in tertiary expression", lexer);
   }

   lexer->advance();
   Expression::SharedPtr else_branch = parse_expression();

   auto op = std::make_shared<TertiaryOperator>(cond, if_branch, else_branch);
   op->setIndex(start, lexer->current_index, source_id);

   return op;
}

ParenExprType Parser::get_paren_expr_type() {
   int start_index = lexer->current_token_index - 1;
   int last_index = lexer->last_token_index - 1;

   int open_parens = 1;
   int closed_parens = 0;
   bool isTuple = false;

   while (open_parens != closed_parens && lexer->current_token.get_type() != T_EOF) {
      lexer->advance();
      if (lexer->current_token.is_punctuator('(')) {
         ++open_parens;
      }
      else if (lexer->current_token.is_punctuator(')')) {
         ++closed_parens;
      }
      else if (lexer->current_token.is_punctuator(',')) {
         isTuple = true;
      }
   }

   if (open_parens != closed_parens) {
      ParseError::raise("Expression contains unmatched parentheses", lexer);
   }

   Token next = lexer->lookahead();
   lexer->current_token_index = start_index;
   lexer->last_token_index = last_index;
   lexer->advance();

   // lambda
   if (next.is_operator("=>") || next.is_operator("->")) {
      return ParenExprType::LAMBDA;
   }
   else if (isTuple) {
      return ParenExprType::TUPLE;
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
         lexer->advance();

         expr = parse_expression();
         lexer->advance(); // last part of expr
         if (!lexer->current_token.is_punctuator(')')) {
            ParseError::raise("Expected ')'", lexer);
         }
         break;
      case ParenExprType::TUPLE:
         expr = parse_tuple_literal();
         break;
   }


   return expr;
}

std::shared_ptr<TupleLiteral> Parser::parse_tuple_literal() {

   int start = lexer->current_index;
   std::vector<pair<string, Expression::SharedPtr>> elements;

   lexer->advance();

   while (!lexer->current_token.is_punctuator(')'))
   {
      std::vector<Attribute> attributes = parse_attributes();
      string label;

      if (lexer->current_token.get_type() == T_IDENT && lexer->lookahead().is_operator(":")) {
         label = lexer->s_val();
         lexer->advance();
      }

      auto expr = parse_expression();
      expr->setAttributes(attributes);

      elements.emplace_back(label, expr);

      lexer->advance();

      if (lexer->current_token.is_punctuator(',')) {
         lexer->advance();
      }
      else if (!lexer->current_token.is_punctuator(')')) {
         ParseError::raise("Expected ',' or ')'", lexer);
      }
   }

   if (!lexer->current_token.is_punctuator(')')) {
      ParseError::raise("Expected closing parenthesis after tuple literal", lexer);
   }

   auto tuple = std::make_shared<TupleLiteral>(elements);
   tuple->setIndex(start, lexer->current_index, source_id);

   return tuple;
}

namespace {

   bool hasHigherPrecedence(Token& next, int& min_precedence) {
      if (next.get_type() == T_OP) {
         auto op = next.get_value().strVal;
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
         auto op = next.get_value().strVal;
         return (util::op_precedence.find(op) != util::op_precedence.end()
            && util::op_precedence[op] >= min_precedence)
            || (min_precedence == 0 && op != "?" && op != ":");
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
Expression::SharedPtr Parser::parse_expression(Expression::SharedPtr lhs, int min_precedence)
{
   int start = lexer->last_token_index;

   if (lhs == nullptr) {
      lhs = parse_unary_expr();
   }

   Token next = lexer->lookahead(false);

   // ...while the next operator has a higher precedence than the minimum
   while (hasHigherOrEqualPrecedence(next, min_precedence)) {
      string op = next.get_value().strVal;
      Expression::SharedPtr rhs;

      if (op == "as" || op == "as!" || op == "isa") {
         lexer->advance();
         lexer->advance();

         rhs = parse_type();
      }
      else {
         lexer->advance();
         lexer->advance();

         rhs = parse_unary_expr();
      }

      next = lexer->lookahead(false);

      // continue recursively while a higher precedence operator follows
      while (hasHigherPrecedence(next, util::op_precedence[op])) {
         rhs = parse_expression(rhs, util::op_precedence[next.get_value().strVal]);
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
Statement::SharedPtr Parser::parse_assignment(bool is_const, bool is_declaration)
{
   std::vector<DeclStmt::SharedPtr> declarations;
   auto access = AccessModifier::DEFAULT;

   while (lexer->current_token.get_type() == T_KEYWORD) {
      auto keyword = lexer->s_val();
      if (keyword == "var" || keyword == "let") {
         break;
      }
      else if (keyword == "public") {
         access = AccessModifier::PUBLIC;
      }
      else if (keyword == "private") {
         access = AccessModifier::PRIVATE;
      }
      else {
         llvm_unreachable("Unexpected keyword");
      }

      lexer->advance();
   }

   for (;;) {
      int start = lexer->current_index;
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

         typeref = parse_type();

         next = lexer->lookahead();
      }
      else if (is_declaration) {
         ParseError::raise("Declared variables must be type annotated", lexer);
      }

      typeref->getType(true)->isConst(is_const);

      if (next.is_operator("=")) {
         if (is_declaration) {
            ParseError::raise("Declared variables cannot be assigned", lexer);
         }

         lexer->advance(false);
         lexer->advance();

         value = parse_expression();
      }

      DeclStmt::SharedPtr decl_stmt = std::make_shared<DeclStmt>(identifier, typeref, is_const, top_level, value);
      decl_stmt->setAccess(access);
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
         ParseError::raise("Compound declarations not allowed here", lexer);
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

std::vector<pair<string, std::shared_ptr<TypeRef>>> Parser::parse_tuple_type() {
   std::vector<pair<string, std::shared_ptr<TypeRef>>> tupleTypes;

   while (!lexer->current_token.is_punctuator(')')) {
      lexer->advance();

      if (lexer->current_token.is_punctuator(')')) {
         break;
      }

      string label;
      if (lexer->current_token.get_type() == T_IDENT && lexer->lookahead().is_operator(":")) {
         label = lexer->s_val();
         lexer->advance();
         lexer->advance();
      }

      bool vararg = false;
      if (lexer->current_token.is_operator("...")) {
         vararg = true;
         lexer->advance();

         // c style vararg
         if (lexer->current_token.get_type() != T_IDENT) {
            auto argType = new AutoType();
            argType->isCStyleVararg(true);
            tupleTypes.emplace_back(label, std::make_shared<TypeRef>(argType));
            continue;
         }
      }

      auto argType = parse_type();
      argType->getType(true)->isVararg(vararg);
      tupleTypes.emplace_back(label, argType);

      lexer->advance();
      if (!lexer->current_token.is_punctuator(',') && !lexer->current_token.is_punctuator(')')) {
         ParseError::raise("Expected closing parenthesis after argument list", lexer);
      }
   }

   return tupleTypes;
}

/**
 * Parses a method's argument list
 * @return
 */
std::vector<FuncArgDecl::SharedPtr> Parser::parse_arg_list(bool optionalNames, bool optionalTypes) {

   int start = lexer->current_index;

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
         ParseError::raise("Vararg arguments can only be the last argument of a "
                     "function", lexer);
      }

      std::vector<Attribute> attributes = parse_attributes();

      start = lexer->current_index;
      FuncArgDecl::SharedPtr arg_dec = std::make_shared<FuncArgDecl>();
      arg_dec->setAttributes(attributes);

      if (!optionalNames || lexer->lookahead().is_operator(":")) {
         if (lexer->current_token.get_type() != T_IDENT) {
            ParseError::raise("Expected identifier", lexer);
         }

         arg_dec->setName(lexer->s_val());
         lexer->advance();
      }
      else if (lexer->current_token.is_operator("...")) {
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

      if (lexer->current_token.is_operator(":") || optionalNames) {
         if (lexer->current_token.is_operator(":")) {
            lexer->advance();
         }

         auto argTy = parse_type();
         if (argTy->getType(true)->isVararg()) {
            var_arg = true;
         }

         arg_dec->setType(argTy);
         lexer->advance();
      }
      else if (!optionalTypes) {
         ParseError::raise("Function arguments have to have a specified type", lexer);
      }

      // optional default value
      if (lexer->current_token.is_operator("=")) {
         if (var_arg) {
            ParseError::raise("Vararg arguments cannot have a default value", lexer);
         }

         lexer->advance();

         Expression::SharedPtr _expr = parse_expression();
         arg_dec->setDefault(_expr);
         def_arg = true;

         arg_dec->getArgType()->getType(true)->hasDefaultArgVal(true);

         lexer->advance();

      } else if (def_arg) {
         ParseError::raise("Default values are only allowed as last items of an argument list", lexer);
      }

      arg_dec->setIndex(start, lexer->current_index, source_id);
      args.push_back(arg_dec);

      // end of argument list or next argument
      if (lexer->current_token.is_punctuator(',')) {
         lexer->advance();
      }
      else if (!lexer->current_token.is_punctuator(')')) {
         ParseError::raise("Expected closing parenthesis after argument list", lexer);
      }

      start = lexer->current_index;
   }

   if (!lexer->current_token.is_punctuator(')')) {
      ParseError::raise("Expected closing parenthesis after argument list", lexer);
   }

   return args;
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
 * Parses a function declaration in the form of
 *  def func(x: number, y: string) => bool {
 *     ... statements
 *  }
 */
FunctionDecl::SharedPtr Parser::parse_function_decl(bool is_declaration)
{
   int start = lexer->current_index;
   bool isOperatorDecl = false;
   string opType;
   if (lexer->lookahead().get_type() == T_KEYWORD) {
      isOperatorDecl = true;
      lexer->advance();

      opType = lexer->s_val();
   }

   // function name
   lexer->advance();

   string funcName;
   string op;

   if (isOperatorDecl) {
      while (isValidOperatorChar(lexer->current_token)) {
         op += lexer->s_val();
         lexer->advance(false, true);
      }

      if (lexer->current_token.is_punctuator(' ')) {
         lexer->advance();
      }

      if (!util::matches("(..+|[^.])*", op)) {
         ParseError::raise("Custom operators can only contain periods in sequences of two or "
                     "more", lexer);
      }

      lexer->backtrack();
      funcName = opType + " " + op;
   }
   else {
      funcName = lexer->s_val();
   }

   FunctionDecl::SharedPtr fun_dec = std::make_shared<FunctionDecl>(funcName);
   fun_dec->isOperatorDecl(isOperatorDecl);

   if (funcName == "main") {
      for (const auto& stmt : implicit_main_stmts) {
         if (stmt->get_type() != NodeType::DECLARATION) {
            ParseError::raise("Cannot define a main method when top-level non-declaration "
                           "statements exist", lexer);
         }
      }

      main_method_defined = true;
   }

   if (isOperatorDecl) {
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
      ParseError::raise("Declared functions have to have a defined return type", lexer);
   }
   else {
      fun_dec->setReturnType(std::make_shared<TypeRef>(new VoidType()));
   }

   if (lexer->lookahead().is_punctuator('{') || lexer->lookahead().is_keyword("unsafe")) {
      if (is_declaration) {
         ParseError::raise("Declared functions cannot have a body", lexer);
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
   int start = lexer->current_index;

   std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list(false, true);

   lexer->advance();
   auto retType = std::make_shared<TypeRef>();

   if (lexer->current_token.is_operator("->")) {
      lexer->advance();
      retType = parse_type();
      lexer->advance();
   }

   if (lexer->current_token.is_operator("=>")) {
      lexer->advance(); // =>
   }
   else {
      llvm_unreachable("function should never be called in this case");
   }

   Statement::SharedPtr body = parse_next_stmt();

   auto lambda_expr = std::make_shared<LambdaExpr>(retType, args, body);
   if (body->get_type() == NodeType::COMPOUND_STMT) {
      auto cmpnd = std::static_pointer_cast<CompoundStmt>(body);
      cmpnd->returnable(true);
   }

   lambda_expr->setIndex(start, lexer->current_index, source_id);
   lambda_expr->setMemberExpr(try_parse_member_expr());

   return lambda_expr;
}

/**
 * Parses a class constructor declaration
 * @param am
 * @return
 */
ConstrDecl::SharedPtr Parser::parse_constr_decl(AccessModifier am, bool optionalNames)
{
   int start = lexer->current_index;

   lexer->advance();
   std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list(optionalNames);


   CompoundStmt::SharedPtr constr_body = nullptr;
   if (!optionalNames) {
      constr_body = parse_block();
   }


   auto constr = std::make_shared<ConstrDecl>(args, constr_body, am);
   constr->setIndex(start, lexer->current_index, source_id);
   return constr;
}

DestrDecl::SharedPtr Parser::parse_destr_decl()
{
   int start = lexer->current_index;

   lexer->advance();
   std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list();
   if (!args.empty()) {
      ParseError::raise("Deinitializers cannot have arguments", lexer);
   }

   CompoundStmt::SharedPtr body = parse_block();

   auto destr = std::make_shared<DestrDecl>(std::move(body));
   destr->setIndex(start, lexer->current_index, source_id);

   return destr;
}

/**
 * Parses a class field declaration
 * @param am
 * @param is_static
 * @return
 */
FieldDecl::SharedPtr Parser::parse_field_decl(AccessModifier am, bool is_static, bool isConst) {
   if (lexer->current_token.get_type() != T_IDENT) {
      ParseError::raise("Field name must be a valid identifier", lexer);
   }

   int start = lexer->current_index;
   string field_name = lexer->s_val();
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

   FieldDecl::SharedPtr field = std::make_shared<FieldDecl>(field_name, typeref, am, is_static, isConst);

   // getter and setter
   bool getter = false;
   bool setter = false;

   if (next.is_punctuator('{')) {
      lexer->advance();
      lexer->advance();

      while (!lexer->current_token.is_punctuator('}')) {
         if (lexer->current_token.get_type() == T_IDENT && lexer->s_val() == "get") {
            if (getter) {
               ParseError::raise("Getter already declared", lexer);
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
         else if (lexer->current_token.get_type() == T_IDENT && lexer->s_val() == "set") {
            if (setter) {
               ParseError::raise("Setter already declared", lexer);
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
            ParseError::raise("Expected 'get' or 'set'", lexer);
         }
      }

      next = lexer->lookahead();
   }

   // optional default value
   if (next.get_type() == T_OP && next.get_value().strVal == "=") {
      lexer->advance();
      lexer->advance();
      field->setDefault(parse_expression());
   }
   else if (!typeDeclared) {
      ParseError::raise("Fields have to have an annotated type or a default value", lexer);
   }

   lexer->advance(false);
   if (!lexer->current_token.is_separator()) {
      ParseError::raise("Field declarations must be on seperate lines", lexer);
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
MethodDecl::SharedPtr Parser::parse_method_decl(AccessModifier am, bool is_static, bool is_interface)
{
   if (lexer->current_token.get_type() != T_IDENT) {
      ParseError::raise("Method name must be a valid identifier", lexer);
   }

   int start = lexer->current_index;

   string method_name = lexer->s_val();
   lexer->advance();

   // method alias
   if (lexer->current_token.is_operator("=")) {
      lexer->advance();

      if (lexer->current_token.get_type() != T_IDENT && lexer->current_token.get_type() != T_OP) {
         ParseError::raise("Method aliasee must be a valid identifier", lexer);
      }

      string aliasee = lexer->s_val();
      lexer->advance();

      auto args = parse_arg_list(true);

      auto op_decl = std::make_shared<MethodDecl>(method_name, aliasee, args);
      op_decl->setIndex(start, lexer->current_index, source_id);

      return op_decl;
   }

   auto generics = parse_generics();
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
         ParseError::raise("Method declarations must be on seperate lines", lexer);
      }

      auto method = std::make_shared<MethodDecl>(method_name, typeref, args, method_body, am, is_static);
      method->setIndex(start, lexer->current_index, source_id);
      method->setGenerics(generics);

      return method;
   }
   else if (!is_interface) {
      ParseError::raise("Abstract methods can only be declared in a protocol or an abstract "
         "class", lexer);
   }
   else {
      auto method = std::make_shared<MethodDecl>(method_name, typeref, args, am, is_static);
      method->setIndex(start, lexer->current_index, source_id);
      method->setGenerics(generics);

      return method;
   }

   return nullptr;
}

/**
 * Parses an operator method declaration
 * @param am
 * @return
 */
MethodDecl::SharedPtr Parser::parse_operator_decl(AccessModifier am, bool is_interface)
{
   int start = lexer->current_index;
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
      op = "[]";
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

      if (!util::matches("(..+|[^.])*", op)) {
         ParseError::raise("Custom operators can only contain periods in sequences of two or "
            "more", lexer);
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
         ParseError::raise("Method aliasee must be a valid identifier", lexer);
      }

      string aliasee = lexer->s_val();
      lexer->advance();

      auto generics = parse_generics();
      auto args = parse_arg_list(true);

      auto op_decl = std::make_shared<MethodDecl>(opType + " " + op, aliasee, args);
      op_decl->setIndex(start, lexer->current_index, source_id);
      op_decl->setGenerics(generics);

      return op_decl;
   }

   std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list(is_interface);

   if (args.size() > 1) {
      ParseError::raise("Custom operators cannot have more than one argument", lexer);
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
         ParseError::raise("Expected infix function to have exactly one argument", lexer);
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
         ParseError::raise("Cast operator return type differs from declared cast target", lexer);
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
      ParseError::raise("Abstract methods can only be declared in an interface or an abstract "
               "class", lexer);
   }
   else {
      op_decl = std::make_shared<MethodDecl>(opType + " " + op, typeref, args, am, false);
   }

   op_decl->setIndex(start, lexer->current_index, source_id);
   return op_decl;
}

std::shared_ptr<EnumCaseDecl> Parser::parse_enum_case() {
   int start = lexer->current_index;
   if (lexer->current_token.get_type() != T_IDENT) {
      ParseError::raise("Expected valid identifier as case name", lexer);
   }

   string caseName = lexer->s_val();
   std::vector<pair<string, std::shared_ptr<TypeRef>>> associatedTypes;

   if (lexer->lookahead().is_punctuator('(')) {
      lexer->advance();
      associatedTypes = parse_tuple_type();
   }

   EnumCaseDecl::SharedPtr caseDecl;

   if (lexer->lookahead().is_operator("=")) {
      lexer->advance();
      lexer->advance();

      if (lexer->current_token.get_type() != T_LITERAL || lexer->current_token.get_value().type != VariantType::INT) {
         ParseError::raise("Expected integer literal for enum raw value", lexer);
      }

      caseDecl = std::make_shared<EnumCaseDecl>(std::move(caseName), lexer->current_token.get_value().intVal,
         std::move(associatedTypes));
   }
   else {
      caseDecl = std::make_shared<EnumCaseDecl>(std::move(caseName), std::move(associatedTypes));
   }

   caseDecl->setIndex(start, lexer->current_index, source_id);
   return caseDecl;
}

std::vector<ObjectType*> Parser::parse_generics() {
   std::vector<ObjectType*> generics;

   if (!lexer->current_token.is_operator("<")) {
      return generics;
   }

   lexer->advance();
   while (!lexer->current_token.is_operator(">")) {
      if (lexer->current_token.get_type() != T_IDENT) {
         ParseError::raise("Expected typename", lexer);
      }

      ObjectType* generic = nullptr;
      auto genericClassName = lexer->s_val();

      lexer->advance();

      // A: +B -> A is superclass of B (contravariance)
      // A: B or A: -B -> A is subclass of B (covariance)
      if (lexer->current_token.is_operator(":")) {
         unsigned covarianceDefined = 0;
         unsigned contravarianceDefined = 0;
         lexer->advance();

         generic_constraint:
         bool covariant = true;

         if (lexer->current_token.is_operator("+")) {
            covariant = false;
            lexer->advance();
         }
         else if (lexer->current_token.is_operator("-")) {
            lexer->advance();
         }

         if (lexer->current_token.get_type() != T_IDENT) {
            ParseError::raise("Expected class name", lexer);
         }

         if (covariant) {
            ++covarianceDefined;
            if (generic == nullptr) {
               generic = ObjectType::get(lexer->s_val());
            }
            else {
               generic->setClassName(lexer->s_val());
            }
         }
         else {
            if (generic == nullptr) {
               generic = ObjectType::get("Any");
            }
            ++contravarianceDefined;
            generic->setContravariance(ObjectType::get(lexer->s_val()));
         }

         if (covarianceDefined > 1 || contravarianceDefined > 1) {
            ParseError::raise("Duplicate generic constraint definition", lexer);
         }

         lexer->advance();
         if (!lexer->current_token.is_punctuator(',') && !lexer->current_token.is_operator(">")) {
            goto generic_constraint;
         }
      }
      else {
         generic = ObjectType::get("Any");
      }

      generic->isGeneric(true);
      generic->setGenericClassName(genericClassName);

      generics.push_back(generic);
      CurrentClassGenerics.emplace(genericClassName, generic);

      if (!lexer->current_token.is_operator(">") && !lexer->current_token.is_punctuator(',')) {
         ParseError::raise("Expected ',' or '>'", lexer);
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
         ParseError::raise("Expected typename", lexer);
      }

      auto type = __parse_type();
      lexer->advance();

      if (!lexer->current_token.is_operator(">") && !lexer->current_token.is_punctuator(',')) {
         ParseError::raise("Expected ',' or '>'", lexer);
      }
      if (lexer->current_token.is_punctuator(',')) {
         lexer->advance();
      }

      generics.push_back(type);
   }

   return generics;
}

std::vector<Statement::SharedPtr> Parser::parse_class_inner(
   bool isStruct,
   bool isProtocol,
   bool isExtension)
{
   std::vector<Statement::SharedPtr> declarations;
   std::vector<Attribute> attributes;
   bool declaration_finished = false;

   enum class DeclType {
      NONE,
      FIELD,
      METHOD,
      CONSTR,
      DESTR,
      OPERATOR,
      TYPEDEF,
      CASE,
      INNER_CLASS,
      INNER_ENUM,
      INNER_STRUCT
   };

   while (!declaration_finished) {
      lexer->advance();

      int start = lexer->current_token.getStart();
      AccessModifier current_am = AccessModifier::DEFAULT;
      bool am_set = false;
      bool is_static = false;
      auto type = DeclType::NONE;
      bool memberwiseInit = false;
      bool isConstField = false;
      bool isMutating = false;

      attributes = parse_attributes();

      while (lexer->current_token.get_type() == T_KEYWORD && lexer->s_val() != "as") {
         std::string keyword =lexer->s_val();
         if (am_set && (keyword == "public" || keyword == "protected" || keyword == "private")) {
            ParseError::raise("Field access modifier already declared", lexer);
         }
         else if (is_static && keyword == "static") {
            ParseError::raise("Field already declared static", lexer);
         }

         if (type != DeclType::NONE && (keyword == "var" || keyword == "def" || keyword == "init" ||
               keyword == "delete" || keyword == "typedef")) {
            ParseError::raise("Declaration type already defined", lexer);
         }

         if (keyword == "public") {
            current_am = AccessModifier::PUBLIC;
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
         else if (keyword == "var") {
            type = DeclType::FIELD;
         }
         else if (keyword == "let") {
            type = DeclType::FIELD;
            isConstField = true;
         }
         else if (keyword == "def") {
            type = DeclType::METHOD;
         }
         else if (keyword == "mutating") {
            isMutating = true;
         }
         else if (keyword == "memberwise") {
            lexer->advance();
            if (!lexer->current_token.is_keyword("init")) {
               ParseError::raise("Expected 'init' after 'memberwise'", lexer);
            }

            type = DeclType::CONSTR;
            memberwiseInit = true;
            break;
         }
         else if (keyword == "init") {
            type = DeclType::CONSTR;
         }
         else if (keyword == "deinit") {
            type = DeclType::DESTR;
         }
         else if (keyword == "infix" || keyword == "postfix" || keyword == "prefix") {
            break;
         }
         else if (keyword == "case") {
            type = DeclType::CASE;
         }
         else if (keyword == "class") {
            type = DeclType::INNER_CLASS;
         }
         else if (keyword == "enum") {
            type = DeclType::INNER_ENUM;
         }
         else if (keyword == "struct") {
            type = DeclType::INNER_STRUCT;
         }
         else {
            ParseError::raise("Unexpected keyword '" + keyword + "' in class declaration", lexer);
         }

         if (type == DeclType::TYPEDEF && is_static) {
            ParseError::raise("Typedefs are static by default", lexer);
         }

         lexer->advance();
      }

      if (isMutating && type != DeclType::METHOD) {
         ParseError::raise("'mutating' can only come before method definitions", lexer);
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
            ParseError::raise("Constructor cannot be declared static", lexer);
         }

         lexer->backtrack();

         if (memberwiseInit) {
            declarations.push_back(std::make_shared<ConstrDecl>());
            lexer->advance();
         }
         else {
            auto constr = parse_constr_decl(current_am, isProtocol);
            constr->setAttributes(attributes);

            declarations.push_back(constr);
         }
      }
      else if (type == DeclType::DESTR) {
         lexer->backtrack();

         auto destr = parse_destr_decl();
         destr->setAttributes(attributes);

         declarations.push_back(destr);
      }
      else if (type == DeclType::FIELD) {
         if (isExtension && !is_static) {
            ParseError::raise("Extensions can only contain static fields", lexer);
         }

         auto field = parse_field_decl(current_am, is_static, isConstField);
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
            auto op = parse_operator_decl(current_am, isProtocol);
            op->setAttributes(attributes);
            op->isMutating(isMutating);

            declarations.push_back(op);
         }
         else {
            auto method = parse_method_decl(current_am, is_static, isProtocol);
            method->setAttributes(attributes);
            method->isMutating(isMutating);

            declarations.push_back(method);
         }
      }
      else if (type == DeclType::CASE) {
         case_start:
         auto case_ = parse_enum_case();
         case_->setAttributes(attributes);

         declarations.push_back(case_);

         if (lexer->lookahead().is_punctuator(',')) {
            lexer->advance();
            lexer->advance();
            goto case_start;
         }
      }
      else if (type == DeclType::INNER_CLASS || type == DeclType::INNER_STRUCT) {
         lexer->backtrack();

         auto cl = parse_class_decl(type == DeclType::INNER_STRUCT, false);
         cl->setAttributes(attributes);

         declarations.push_back(cl);
      }
      else if (type == DeclType::INNER_ENUM) {
         lexer->backtrack();

         auto en = parse_enum_decl();
         en->setAttributes(attributes);

         declarations.push_back(en);
      }
      else {
         declaration_finished = true;
      }

      if (!declarations.empty()) {
         declarations.back()->setIndex(start, lexer->current_index, source_id);
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

ClassHead Parser::parse_class_head(bool isEnum)
{
   AccessModifier am = AccessModifier::DEFAULT;
   bool am_set = false;
   bool isAbstract = false;

   while (lexer->current_token.get_type() == T_KEYWORD) {
      string keyword = lexer->s_val();
      if (keyword == "public") {
         if (am_set) {
            ParseError::raise("The access modifier for this class was already set", lexer);
         }

         am = AccessModifier::PUBLIC;
         am_set = true;
         lexer->advance();
      }
      else if (keyword == "protected") {
         ParseError::raise("Classes cannot be declared 'protected'", lexer);
      }
      else if (lexer->s_val() == "private") {
         if (am_set) {
            ParseError::raise("The access modifier for this class was already set", lexer);
         }

         am = AccessModifier::PRIVATE;
         am_set = true;
         lexer->advance();
      }
      else if (keyword == "abstract") {
         if (isAbstract) {
            ParseError::raise("Class already declared abstract", lexer);
         }

         isAbstract = true;
         lexer->advance();
      }
      else if (keyword == "class") {
         break;
      }
      else {
         break;
      }
   }

   lexer->advance();
   if (lexer->current_token.get_type() != T_IDENT) {
      ParseError::raise("Expected class name", lexer);
   }

   string className = lexer->s_val();
   namespaces.push_back(className);
   type_names.push_back(className);

   TypeRef::SharedPtr enumRawType = nullptr;
   if (isEnum && lexer->lookahead().is_punctuator('(')) {
      lexer->advance();
      lexer->advance();

      enumRawType = parse_type();

      lexer->advance();
      if (!lexer->current_token.is_punctuator(')')) {
         ParseError::raise("Expected ')'", lexer);
      }
   }

   ObjectType* extends = nullptr;
   std::vector<ObjectType*> with;

   lexer->advance();
   auto generics = parse_generics();

   while (!lexer->current_token.is_punctuator('{') && lexer->current_token.get_type() != T_EOF) {
      if (lexer->current_token.is_operator(":")) {
         if (extends != nullptr) {
            ParseError::raise("Classes can only inherit from one other class", lexer);
         }

         lexer->advance();
         if (lexer->current_token.get_type() != T_IDENT) {
            ParseError::raise("Expected identifier after ':'", lexer);
         }

         auto ext = __parse_type();
         if (!isa<ObjectType>(ext)) {
            ParseError::raise("Only object types can be extended", lexer);
         }

         extends = cast<ObjectType>(ext);
         lexer->advance();
      }
      else if (lexer->current_token.is_keyword("with")) {
         lexer->advance();
         if (lexer->current_token.get_type() != T_IDENT) {
            ParseError::raise("Expected identifier after 'with'", lexer);
         }

         while (lexer->current_token.get_type() == T_IDENT && lexer->current_token.get_type() != T_EOF) {
            auto with_ = __parse_type();
            if (!isa<ObjectType>(with_)) {
               ParseError::raise("Only object types can be extended", lexer);
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

   return ClassHead { am, className, extends, with, generics, isAbstract, enumRawType };
}

/**
 * Parses a class declaration
 * @return
 */
ClassDecl::SharedPtr Parser::parse_class_decl(
   bool isStruct,
   bool isProtocol)
{
   int start = lexer->current_index;

   auto attrs = attributes;
   auto head = parse_class_head();

   if (isProtocol && head.isAbstract) {
      ParseError::raise("Protocols cannot be declared abstract", lexer);
   }
   if (isProtocol && head.extends != nullptr) {
      ParseError::raise("Protocols cannot inherit from classes or conform to protocols", lexer);
   }
   if (!lexer->current_token.is_punctuator('{')) {
      ParseError::raise("Expected '{' to start a class definition", lexer);
   }

   std::vector<ConstrDecl::SharedPtr> constructors;
   std::vector<FieldDecl::SharedPtr> fields;
   std::vector<MethodDecl::SharedPtr> methods;
   std::vector<TypedefDecl::SharedPtr> typedefs;
   std::vector<Statement::SharedPtr> innerDeclarations;
   DestrDecl::SharedPtr destructor = nullptr;

   auto declarations = parse_class_inner(isStruct, isProtocol);
   for (const auto& decl : declarations) {
      switch (decl->get_type()) {
         case NodeType::CONSTR_DECL:
            constructors.push_back(std::static_pointer_cast<ConstrDecl>(decl));
            break;
         case NodeType ::DESTR_DECL:
            destructor = std::static_pointer_cast<DestrDecl>(decl);
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
         case NodeType::ENUM_CASE_DECL:
            RuntimeError::raise("Cases can only appear in enum declarations", decl.get());
         case NodeType::CLASS_DECL:
         case NodeType::ENUM_DECL:
            innerDeclarations.push_back(decl);
            break;
         default:
            assert(false && "Unkown class declaration type");
      }
   }

   if (head.isAbstract && !constructors.empty()) {
      RuntimeError::raise("Abstract classes cannot define initializers", constructors.front().get());
   }
   if (head.isAbstract && destructor != nullptr) {
      RuntimeError::raise("Abstract classes cannot define deinitializers", constructors.front().get());
   }

   CurrentClassGenerics.clear();

   ClassDecl::SharedPtr class_decl;
   if (isProtocol) {
      class_decl = std::make_shared<ClassDecl>(head.class_name, std::move(fields), std::move(methods),
         std::move(constructors), std::move(typedefs), std::move(head.generics), head.am, std::move(head.conformsTo),
         std::move(destructor), std::move(innerDeclarations)
      );
   }
   else {
      class_decl = std::make_shared<ClassDecl>(head.class_name, std::move(fields), std::move(methods),
         std::move(constructors), std::move(typedefs), std::move(head.generics), head.am, head.isAbstract,
         head.extends, std::move(head.conformsTo), std::move(destructor), std::move(innerDeclarations)
      );
   }

   class_decl->isStruct(isStruct);
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
   auto cl = parse_class_decl(true);
   if (cl->getParentClass() != nullptr) {
      ParseError::raise("Structs cannot extend other structs or classes", lexer);
   }
   if (cl->getDestructor() != nullptr) {
      ParseError::raise("Structs cannot declare deinitializers", lexer);
   }

   return cl;
}

ClassDecl::SharedPtr Parser::parse_extend_stmt() {
   auto cl = parse_class_decl();
   if (cl->getParentClass() != nullptr) {
      ParseError::raise("Extensions cannot add base classes", lexer);
   }

   cl->isExtension(true);

   return cl;
}

std::shared_ptr<EnumDecl> Parser::parse_enum_decl() {
   int start = lexer->current_index;

   auto attrs = attributes;
   auto head = parse_class_head(true);

   if (head.isAbstract) {
      ParseError::raise("Enums cannot be declared abstract", lexer);
   }
   if (head.extends != nullptr) {
      ParseError::raise("Enums cannot inherit", lexer);
   }
   if (!lexer->current_token.is_punctuator('{')) {
      ParseError::raise("Expected '{' to start an enum definition", lexer);
   }

   std::vector<MethodDecl::SharedPtr> methods;
   std::vector<EnumCaseDecl::SharedPtr> cases;
   std::vector<Statement::SharedPtr> innerDeclarations;

   auto declarations = parse_class_inner(false, false, false);
   for (const auto& decl : declarations) {
      switch (decl->get_type()) {
         case NodeType::METHOD_DECL:
            methods.push_back(std::static_pointer_cast<MethodDecl>(decl));
            break;
         case NodeType::ENUM_CASE_DECL:
            cases.push_back(std::static_pointer_cast<EnumCaseDecl>(decl));
            break;
         case NodeType::CLASS_DECL:
         case NodeType::ENUM_DECL:
            innerDeclarations.push_back(decl);
            break;
         default:
            RuntimeError::raise("Enums can only contain methods and cases", decl.get());
      }
   }

   auto enumDecl = std::make_shared<EnumDecl>(head.am, std::move(head.class_name), head.enumRawType,
      std::move(methods), std::move(head.conformsTo), std::move(head.generics), std::move(cases),
      std::move(innerDeclarations)
   );

   enumDecl->setIndex(start, lexer->current_index, source_id);
   return enumDecl;
}

/**
 * Parses an if/else statement
 * @return
 */
IfStmt::SharedPtr Parser::parse_if_stmt()
{
   int start = lexer->current_index;
   lexer->advance();

   Expression::SharedPtr if_cond = parse_expression();
   lexer->advance(); // last part of expression

   Statement::SharedPtr if_branch = parse_next_stmt();
   IfStmt::SharedPtr if_stmt = std::make_shared<IfStmt>(if_cond, if_branch);

   Token next = lexer->lookahead();
   if (next.get_type() == T_KEYWORD && next.get_value().strVal == "else") {
      lexer->advance();
      lexer->advance();

      CompoundStmt::SharedPtr else_branch = std::static_pointer_cast<CompoundStmt>(parse_next_stmt());
      if_stmt->setElseBranch(else_branch);
   }

   if_stmt->setIndex(start, lexer->current_index, source_id);

   return if_stmt;
}

CaseStmt::SharedPtr Parser::parse_case_stmt(bool default_)
{
   int start = lexer->current_index;
   lexer->advance();

   CaseStmt::SharedPtr case_stmt;

   if (!default_) {
      Expression::SharedPtr case_val = parse_expression();
      case_stmt = std::make_shared<CaseStmt>(case_val);
      lexer->advance();
   }
   else {
      case_stmt = std::make_shared<CaseStmt>();
   }

   if (!lexer->current_token.is_operator(":")) {
      ParseError::raise("Expected ':' after case label", lexer);
   }

   lexer->advance();
   Statement::SharedPtr stmt = parse_next_stmt();

   if (stmt->get_type() != NodeType::COMPOUND_STMT) {
      CompoundStmt::SharedPtr compound = std::make_shared<CompoundStmt>();
      compound->addStatement(stmt);
      lexer->advance();

      while (!lexer->current_token.is_keyword("case") && !lexer->current_token.is_keyword("default") &&
         !lexer->current_token.is_punctuator('}'))
      {
         compound->addStatement(parse_next_stmt());
         lexer->advance();
      }

      case_stmt->setBody(compound);
   }
   else {
      case_stmt->setBody(stmt);
   }

   case_stmt->setIndex(start, lexer->current_index, source_id);
   return case_stmt;
}

MatchStmt::SharedPtr Parser::parse_match_stmt()
{
   int start = lexer->current_index;
   lexer->advance();

   Expression::SharedPtr switch_val = parse_expression();
   lexer->advance();
   if (!lexer->current_token.is_punctuator('{')) {
      ParseError::raise("Expected '{' after 'match'", lexer);
   }

   MatchStmt::SharedPtr switch_stmt = std::make_shared<MatchStmt>(switch_val);
   lexer->advance();

   bool isDefault = false;
   while (!lexer->current_token.is_punctuator('}')) {
      if (isDefault) {
         ParseError::raise("Expected no more cases after 'default'", lexer);
      }

      isDefault = lexer->current_token.is_keyword("default");
      if (!lexer->current_token.is_keyword("case") && !isDefault) {
         ParseError::raise("Expected 'case' or 'default'", lexer);
      }

      auto caseStmt = parse_case_stmt(isDefault);
      switch_stmt->addCase(caseStmt);
   }

   switch_stmt->setIndex(start, lexer->current_index, source_id);
   return switch_stmt;
}

WhileStmt::SharedPtr Parser::parse_while_stmt(bool conditionBefore)
{
   int start = lexer->current_index;
   lexer->advance();

   Expression::SharedPtr cond = std::make_shared<NumericLiteral>(Variant(true));
   if (conditionBefore) {
      cond = parse_expression();
      lexer->advance();
   }

   CompoundStmt::SharedPtr body = std::static_pointer_cast<CompoundStmt>(parse_next_stmt());
   if (!conditionBefore && lexer->lookahead().is_keyword("while")) {
      lexer->advance();
      lexer->advance();

      cond = parse_expression();
   }

   WhileStmt::SharedPtr whileStmt = std::make_shared<WhileStmt>(cond, body, !conditionBefore);
   whileStmt->setIndex(start, lexer->current_index, source_id);

   return whileStmt;
}

Statement::SharedPtr Parser::parse_for_stmt()
{
   int start = lexer->current_index;
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
      DeclStmt::SharedPtr decl = std::dynamic_pointer_cast<DeclStmt>(init);
      if (decl == nullptr) {
         ParseError::raise("Expected declaration before 'in' in range based for loop", lexer);
      }

      lexer->advance();

      Expression::SharedPtr range = parse_expression();
      lexer->advance();

      if (lexer->current_token.is_punctuator(')')) {
         lexer->advance();
      }

      CompoundStmt::SharedPtr body = std::static_pointer_cast<CompoundStmt>(parse_next_stmt());

      auto forInStmt = std::make_shared<ForInStmt>(std::move(decl), std::move(range), std::move(body));
      forInStmt->setIndex(start, lexer->current_index, source_id);
      forInStmt->setAttributes(attributes);

      return forInStmt;
   }

   if (!lexer->current_token.is_punctuator(';')) {
      ParseError::raise("Expected ';' to seperate for loop arguments", lexer);
   }

   Expression::SharedPtr term;
   lexer->advance();
   if (lexer->current_token.is_punctuator(';')) {
      term = std::make_shared<NumericLiteral>(Variant(true));
   }
   else {
      term = parse_expression();
      lexer->advance();
   }

   if (!lexer->current_token.is_punctuator(';')) {
      ParseError::raise("Expected ';' to seperate for loop arguments", lexer);
   }

   Statement::SharedPtr inc;
   lexer->advance();
   if (lexer->current_token.is_punctuator('{')) {
      inc = nullptr;
   }
   else {
      inc = parse_next_stmt();
      lexer->advance();
   }

   CompoundStmt::SharedPtr block = std::static_pointer_cast<CompoundStmt>(parse_next_stmt());

   ForStmt::SharedPtr for_stmt = std::make_shared<ForStmt>(init, term, inc);
   for_stmt->setBody(block);
   for_stmt->setIndex(start, lexer->current_index, source_id);

   return for_stmt;
}

Statement::SharedPtr Parser::parse_declare_stmt() {
   int start = lexer->current_index;

   if (lexer->lookahead().is_punctuator('{')) {
      lexer->advance();
      return parse_multiple_declare_stmt();
   }

   lexer->advance();
   if (lexer->current_token.get_type() != T_KEYWORD) {
      ParseError::raise("Expected 'def', 'let', 'const' or 'class'", lexer);
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
      decl = std::make_shared<DeclareStmt>(head.am, head.class_name, head.extends, head.conformsTo, false, head.generics);
   }
   else if (keyword == "var" || keyword == "let") {
      auto assign = std::static_pointer_cast<DeclStmt>(parse_assignment(keyword == "let", true));
      decl = std::make_shared<DeclareStmt>(assign->getIdentifier(), assign->getType(), keyword == "let");
   }
   else {
      assert(false && "Should not happen"); // TODO
   }

   decl->setIndex(start, lexer->current_index, source_id);
   return decl;
}

CompoundStmt::SharedPtr Parser::parse_multiple_declare_stmt() {
   int start = lexer->current_index;
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
   int start = lexer->current_index;

   string keyword = lexer->current_token.get_value().strVal;

   if (keyword == "var" || keyword == "let") {
      auto assignment = parse_assignment(keyword == "let");

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
   else if (keyword == "while" || keyword == "loop") {
      WhileStmt::SharedPtr while_stmt = parse_while_stmt(keyword == "while");
      while_stmt->setIndex(start, lexer->current_index, source_id);

      if (top_level) {
         implicit_main_stmts.push_back(while_stmt);
         return nullptr;
      }

      return while_stmt;
   }
   else if (keyword == "match") {
      auto switch_stmt = parse_match_stmt();
      switch_stmt->setIndex(start, lexer->current_index, source_id);

      if (top_level) {
         implicit_main_stmts.push_back(switch_stmt);
         return nullptr;
      }

      return switch_stmt;
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
      Statement::SharedPtr for_stmt = parse_for_stmt();
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
   else if (keyword == "class") {
      return parse_class_decl();
   }
   else if (keyword == "public" || keyword == "private") {
      auto next = lexer->lookahead();
      if (next.is_keyword("class") || next.is_keyword("struct")) {
         return parse_class_decl();
      }
      else if (next.is_keyword("enum")) {
         return parse_enum_decl();
      }
      else if (next.is_keyword("let") || next.is_keyword("var")) {
         return parse_assignment(next.is_keyword("let"));
      }
   }
   else if (keyword == "abstract") {
      auto next = lexer->lookahead();
      if (next.is_keyword("class")) {
         return parse_class_decl();
      }
      else {
         ParseError::raise("Unexpected keyword 'abstract'", lexer);
      }
   }
   else if (keyword == "enum") {
      return parse_enum_decl();
   }
   else if (keyword == "struct") {
      return parse_struct_decl();
   }
   else if (keyword == "protocol") {
      return parse_class_decl(false, true);
   }
   else if (keyword == "extend") {
      return parse_extend_stmt();
   }
   else if (keyword == "return") {
      Token next = lexer->lookahead(false);
      if (!next.is_separator()) {
         lexer->advance();

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
   else if (keyword == "using") {
      ParseError::raise("Keyword '" + keyword + "' is only allowed at the beginning of a "
         "file", lexer);
   }
   else if (keyword == "__debug") {
      return std::make_shared<DebugStmt>();
   }
   else if (keyword == "__unreachable") {
      return std::make_shared<DebugStmt>(true);
   }
   else {
      ParseError::raise("'" + keyword + "' is a reserved keyword", lexer);
   }

   return nullptr;
}

/**
 * Parses a function call in the form func(arg1, arg2, ...)
 * @return
 */
CallExpr::SharedPtr Parser::parse_function_call(CallType callTy, bool allowLet)
{
   int start = lexer->current_index;

   string funcName = lexer->s_val();
   lexer->advance();

   std::vector<Type*> generics = parse_concrete_generics();
   if (!generics.empty()) {
      lexer->advance();
   }

   CallExpr::SharedPtr call;
   if (funcName == "sizeof" || funcName == "alignof" || funcName == "__nullptr") {
      lexer->advance();

      auto type = parse_type();
      lexer->advance();

      std::vector<pair<string, Expression::SharedPtr>> args;
      args.emplace_back("", type);

      call = std::make_shared<CallExpr>(callTy, args, funcName);
   }
   else if (funcName == "__builtin_bitcast") {
      lexer->advance();

      auto target = parse_expression();
      lexer->advance(T_PUNCTUATOR);
      lexer->advance();

      auto type = parse_type();
      lexer->advance();

      std::vector<pair<string, Expression::SharedPtr>> args;
      args.emplace_back("", target);
      args.emplace_back("", type);

      call = std::make_shared<CallExpr>(callTy, args, funcName);
   }
   else {
      call = std::make_shared<CallExpr>(callTy, parse_arguments(allowLet), funcName);
   }

   call->setMemberExpr(try_parse_member_expr());
   call->setIndex(start, lexer->current_index, source_id);
   call->setAttributes(attributes);
   call->set_generics(generics);

   return call;
}

std::vector<pair<string, std::shared_ptr<Expression>>> Parser::parse_arguments(bool allowLet) {
   std::vector<pair<string, std::shared_ptr<Expression>>> args;
   bool isLabeled = false;

   while (!lexer->current_token.is_punctuator(')')) {
      string label;

      if (lexer->current_token.is_punctuator('(') || lexer->current_token.is_punctuator(',')) {
         lexer->advance();
         if (lexer->current_token.get_type() == T_IDENT && lexer->lookahead().is_operator(":")) {
            label = lexer->s_val();
            lexer->advance();
            lexer->advance();

            isLabeled = true;
         }
         else if (lexer->current_token.is_punctuator(')')) {
            break;
         }
         else if (isLabeled) {
            ParseError::raise("Labeled arguments have to come last in a call", lexer);
         }
      }

      bool isVar = false;
      bool isLet = false;
      if (lexer->current_token.is_keyword("let") && allowLet) {
         isLet = true;
         lexer->advance();
      }
      else if (lexer->current_token.is_keyword("var") && allowLet) {
         isVar = true;
         lexer->advance();
      }

      auto argVal = parse_expression();
      if ((isLet || isVar) && argVal->get_type() != NodeType::IDENTIFIER_EXPR) {
         ParseError::raise("Expected identifier after 'let' / 'var'", lexer);
      }
      else if (isLet) {
         std::static_pointer_cast<IdentifierRefExpr>(argVal)->isLetExpr(true);
      }
      else if (isVar) {
         std::static_pointer_cast<IdentifierRefExpr>(argVal)->isVarExpr(true);
      }

      args.emplace_back(label, argVal);

      lexer->advance();
      if (!lexer->current_token.is_punctuator(',') && !lexer->current_token.is_punctuator(')')) {
         ParseError::raise("Expected ',' or ')'", lexer);
      }
   }

   return args;
}

std::vector<Attribute> Parser::parse_attributes() {
   std::vector<Attribute> attributes;
   std::vector<string> foundAttrs;

   while (lexer->current_token.is_punctuator('@')) {
      lexer->advance();
      if (lexer->current_token.get_type() != T_IDENT) {
         ParseError::raise("Expected attribute name", lexer);
      }

      Attribute attr;
      attr.name = lexer->s_val();
      if (std::find(foundAttrs.begin(), foundAttrs.end(), attr.name) != foundAttrs.end()) {
         ParseError::raise("Duplicate definition of attribute " + attr.name, lexer);
      }

      attr.kind = AttributeMap[attr.name];

      if (lexer->lookahead().is_punctuator('(')) {
         lexer->advance();
         lexer->advance();

         while (!lexer->current_token.is_punctuator(')') && lexer->current_token.get_type() != T_EOF) {
            if (lexer->current_token.get_type() != T_IDENT) {
               ParseError::raise("Expected attribute argument", lexer);
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
         ParseError::raise(check, lexer);
      }

      attributes.push_back(attr);
      foundAttrs.push_back(attr.name);
      lexer->advance();
   }

   return attributes;
}

CompoundStmt::SharedPtr Parser::parse_block(bool preserveTopLevel) {
   int start = lexer->current_index;
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
      ParseError::raise("Expected '{' to start a block statement.", lexer);
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

   if (!lexer->current_token.is_punctuator('}')) {
      ParseError::raise("Expected '}' to end a block statement", lexer);
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
NamespaceDecl::SharedPtr Parser::parse_namespace_decl()
{
   int start = lexer->current_index;
   bool anonymous = false;
   string nsName;

   lexer->advance();
   if (lexer->current_token.get_type() != T_IDENT) {
      anonymous = true;
      lexer->backtrack();
   }
   else {
      nsName = lexer->s_val();
   }

   while (lexer->lookahead().is_punctuator('.')) {
      lexer->advance();
      lexer->advance();

      if (lexer->current_token.get_type() != T_IDENT) {
         ParseError::raise("Expected identifier after 'namespace'", lexer);
      }

      nsName += "." + lexer->s_val();
   }

   auto mod = std::make_shared<NamespaceDecl>(nsName, parse_block(true), anonymous);
   mod->setIndex(start, lexer->current_index, source_id);

   namespaces.push_back(nsName);
   return mod;
}

/**
 * Parses a module import statement
 * @return
 */
UsingStmt::SharedPtr Parser::parse_using_stmt() {
   int start = lexer->current_index;
   lexer->advance();

   string importNs;
   std::vector<string> items;

   while (lexer->current_token.get_type() == T_IDENT) {
      if (lexer->lookahead().is_punctuator('.')) {
         if (!importNs.empty()) {
            importNs += ".";
         }

         importNs += lexer->s_val();

         lexer->advance();
         if (lexer->lookahead().is_punctuator('{')) {
            lexer->advance();
            lexer->advance();

            while (!lexer->current_token.is_punctuator('}')) {
               items.push_back(lexer->s_val());
               lexer->advance();
               if (lexer->current_token.is_punctuator(',')) {
                  lexer->advance();
               }
            }

            break;
         }

         lexer->advance();
      }
      else {
         items.push_back(lexer->s_val());
         break;
      }
   }

   if (importNs.empty()) {
      importNs = items.back();
      items.pop_back();
   }

   UsingStmt::SharedPtr usingStmt = std::make_shared<UsingStmt>(std::move(importNs), std::move(items));
   usingStmt->setIndex(start, lexer->current_index, source_id);

   return usingStmt;
}

/**
 * Parses the program into an AST
 * @return
 */
CompoundStmt::SharedPtr Parser::parse() {
   CompoundStmt::SharedPtr root = std::make_shared<CompoundStmt>();

   lexer->advance(); // BOF
   while (lexer->current_token.is_keyword("using")) {
      root->addStatement(parse_using_stmt());
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
         ParseError::raise("Attributes not allowed here", lexer);
      }

      if (stmt != nullptr) {
         root->addStatement(stmt);
      }

      lexer->advance();
   }

   return root;
}