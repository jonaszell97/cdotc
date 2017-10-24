//
// Created by Jonas Zell on 13.06.17.
//

#include "Parser.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>

#include "Lexer.h"
#include "Message/Exceptions.h"
#include "Util.h"
#include "Variant/Variant.h"

#include "AST/Passes/ASTIncludes.h"

std::vector<string> Parser::type_names = {};
std::vector<string> Parser::namespaces = {};
unordered_map<string, ObjectType*>  Parser::CurrentClassGenerics =  {};
std::vector<ClassDecl::SharedPtr> Parser::class_declarations = {};

Parser::Parser(llvm::MemoryBuffer *buf, string &fileName, size_t sourceId) :
   lexer(new Lexer(buf, fileName, sourceId)), source_id(sourceId)
{

}

Parser::~Parser() {
   delete lexer;
}

void Parser::setIndex(
   AstNode *node,
   const Token &start)
{
   node->setSourceLoc(
      SourceLocation(start.getCol(), start.getLine(),
         lexer->currentIndex - start.getStart() - 1, source_id)
   );
}

TypeRef::SharedPtr Parser::parse_type()
{
   bool isReference = false;
   if (lexer->currentToken.is_keyword("ref")) {
      lexer->advance();
      isReference = true;
   }

   auto attrs = parse_attributes();

   Token start = lexer->currentToken;
   auto typeref = __parse_type();

   setIndex(typeref.get(), start);
   typeref->isReference(isReference);
   typeref->setAttributes(attrs);

   return typeref;
}

/**
 * Parses a type specifier, like "int", "bool[3]", "any[18 * x]"
 * @return
 */
std::shared_ptr<TypeRef> Parser::__parse_type()
{
   // collection type
   if (lexer->currentToken.is_punctuator('[')) {
      lexer->advance();

      auto elType = parse_type();
      lexer->advance();

      if (lexer->currentToken.is_operator(":")) {
         lexer->advance();

         auto valType = parse_type();
         lexer->advance();

         if (!lexer->currentToken.is_punctuator(']')) {
            ParseError::raise("Expected ']' after dictionary type", lexer);
         }

         std::vector<pair<string, TypeRef::SharedPtr>> generics{ { "", elType }, { "", valType } };
         string className = "Dictionary";

         return std::make_shared<TypeRef>(className, generics);
      }

      if (!lexer->currentToken.is_punctuator(']')) {
         ParseError::raise("Expected ']' after array type", lexer);
      }

      std::vector<pair<string, TypeRef::SharedPtr>> generics{ { "", elType } };
      string className = "Array";

      return std::make_shared<TypeRef>(className, generics);
   }

   // function or tuple type
   if (lexer->currentToken.is_punctuator('(')) {
      auto argTypes = parse_tuple_type();
      lexer->advance();

      // tuple
      if (!lexer->currentToken.is_operator("->")) {
         if (argTypes.size() == 1) {
            return argTypes.front().second;
         }

         lexer->backtrack();
         return std::make_shared<TypeRef>(argTypes);
      }

      lexer->advance();

      auto returnType = parse_type();
      return std::make_shared<TypeRef>(returnType, argTypes);
   }


   string typeName = lexer->s_val();
   std::vector<pair<string, TypeRef::SharedPtr>> generics;

   while (lexer->lookahead(false).is_punctuator('.')) {
      lexer->advance();
      lexer->advance();
      if (lexer->currentToken.get_type() != T_IDENT) {
         ParseError::raise("Unexpected character in type reference", lexer);
      }

      typeName += "." + lexer->s_val();
   }

   Token next = lexer->lookahead(false);

   // generic type requirements
   if (next.is_operator("<")) {
      lexer->advance();

      auto _generics = parse_concrete_generics();
      for (const auto& gen : _generics) {
         generics.emplace_back("", gen);
      }

      next = lexer->lookahead(false);
   }

   auto typeref = std::make_shared<TypeRef>(typeName, generics);

   // optional type
   if (next.is_operator("?")) {
      lexer->advance();

      string className = "Option";
      generics.emplace_back("", typeref);

      return std::make_shared<TypeRef>(className, generics);
   }

   // pointer type
   while (next.get_type() == T_OP) {
      auto op = next.get_value().strVal;
      if (util::matches("\\*+", op)) {
         while (op.length() > 0) {
            ++typeref->getPointerDepth();
            op = op.substr(0, op.length() - 1);
         }

         lexer->advance();
         next = lexer->lookahead(false);
      }
      else {
         break;
      }
   }

   return typeref;
}

std::shared_ptr<TypedefDecl> Parser::parse_typedef(AccessModifier am)
{
   Token start = lexer->currentToken;
   auto attrs = attributes;
   attributes.clear();

   auto originTy = parse_type();
   lexer->advance();

   if (!lexer->currentToken.is_operator("=")) {
      ParseError::raise("Expected =", lexer);
   }

   lexer->advance();
   auto alias = lexer->s_val();
   auto generics = parse_generics();
   if (!generics.empty()) {
      lexer->backtrack();
   }

   auto td = std::make_shared<TypedefDecl>(am, std::move(alias), std::move(originTy), std::move(generics));
   td->setAttributes(attrs);

   setIndex(td.get(), start);

   return td;
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
   Token start = lexer->currentToken;
   string ident = lexer->s_val();

   IdentifierRefExpr::SharedPtr ident_expr = std::make_shared<IdentifierRefExpr>(ident);

   ident_expr->setMemberExpr(try_parse_member_expr());
   setIndex(ident_expr.get(), start);

   return ident_expr;
}

std::shared_ptr<Expression> Parser::try_parse_member_expr()
{
   Token start = lexer->currentToken;
   lexer->advance(false);

   // member access
   bool pointerAccess = lexer->currentToken.is_operator("->");
   if (lexer->currentToken.is_punctuator('.') || pointerAccess) {
      lexer->advance(false);

      // tuple access
      if (lexer->currentToken.get_type() == T_LITERAL) {
         size_t index = lexer->currentToken.get_value().intVal;

         MemberRefExpr::SharedPtr mem_ref = std::make_shared<MemberRefExpr>(index, pointerAccess);
         mem_ref->setMemberExpr(try_parse_member_expr());
         setIndex(mem_ref.get(), start);

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
      setIndex(mem_ref.get(), start);

      return mem_ref;
   }

   // array access
   if (lexer->currentToken.is_punctuator('[')) {
      lexer->advance();

      Expression::SharedPtr expr = parse_expression();
      setIndex(expr.get(), start);

      SubscriptExpr::SharedPtr arr_acc = std::make_shared<SubscriptExpr>(expr);

      lexer->advance();
      if (!lexer->currentToken.is_punctuator(']')) {
         ParseError::raise("Expected ']'", lexer);
      }

      arr_acc->setMemberExpr(try_parse_member_expr());
      setIndex(arr_acc.get(), start);

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
   Token start = lexer->currentToken;

   bool isDictionary = false;
   bool first = true;
   std::vector<Expression::SharedPtr> keys;
   std::vector<Expression::SharedPtr> values;

   while (!lexer->currentToken.is_punctuator(']')) {
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

      if (lexer->currentToken.is_operator(":")) {
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

   if (!lexer->currentToken.is_punctuator(']')) {
      ParseError::raise("Expected ']' after array literal", lexer);
   }

   if (isDictionary) {
      auto dict = std::make_shared<CollectionLiteral>(keys, values);
      setIndex(dict.get(), start);

      return dict;
   }

   auto arr = std::make_shared<CollectionLiteral>(values);
   setIndex(arr.get(), start);

   return arr;
}

/**
 * Parses an atomic part of an expression, like a single identifier or literal
 * @return
 */
Expression::SharedPtr Parser::parse_unary_expr_target()
{
   std::vector<Attribute> attributes;

   if (lexer->currentToken.is_punctuator('@')) {
      attributes = parse_attributes();
   }

   if (lexer->currentToken.is_punctuator('[')) {
      auto arr = parse_collection_literal();
      arr->setAttributes(attributes);

      return arr;
   }
   if (lexer->currentToken.is_punctuator('(')) {
      auto expr = parse_paren_expr();
      expr->setAttributes(attributes);
      expr->setMemberExpr(try_parse_member_expr());

      return expr;
   }
   // enum case with inferred type
   if (lexer->currentToken.is_punctuator('.')) {
      Token start = lexer->currentToken;
      lexer->advance(T_IDENT);

      auto caseName = lexer->s_val();

      if (!lexer->lookahead().is_punctuator('(')) {
         auto expr = std::make_shared<MemberRefExpr>(caseName);
         expr->setAttributes(attributes);
         setIndex(expr.get(), start);
         expr->isEnumCase_(true);

         return expr;
      }
      else {
         auto call = parse_function_call(CallType::METHOD_CALL, true);
         call->isEnumCase_(true);

         return call;
      }
   }
   if (lexer->currentToken.get_type() == T_IDENT) {
      Token start = lexer->currentToken;
      Token next = lexer->lookahead(false);

      // function call
      if (next.is_punctuator('(') || is_generic_call()) {
         return parse_function_call(CallType::FUNC_CALL);
      }

      // single argument lambda
      if (next.is_operator("=>")) {
         auto argName = lexer->s_val();
         auto arg = std::make_shared<FuncArgDecl>(argName, std::make_shared<TypeRef>());
         setIndex(arg.get(), start);
         start = lexer->currentToken;

         lexer->advance();
         lexer->advance();

         auto body = parse_next_stmt();
         auto lambda = std::make_shared<LambdaExpr>(std::make_shared<TypeRef>(),
            std::vector<FuncArgDecl::SharedPtr>{ arg }, body);
         setIndex(lambda.get(), start);
         lambda->setMemberExpr(try_parse_member_expr());

         return lambda;
      }

      auto ident = parse_identifier();
      ident->setAttributes(attributes);

      return ident;
   }
   if (lexer->currentToken.get_type() == T_LITERAL) {
      Token start = lexer->currentToken;
      auto val = lexer->currentToken.get_value();

      if (val.type == VariantType::STRING) {
         Token next = lexer->lookahead(false);
         auto str = lexer->s_val();

         auto strLiteral = std::make_shared<StringLiteral>(str);
         setIndex(strLiteral.get(), start);
         strLiteral->setMemberExpr(try_parse_member_expr());
         strLiteral->setAttributes(attributes);

         if (!lexer->currentToken.isInterpolationStart) {
            return strLiteral;
         }

         std::vector<Expression::SharedPtr> strings{ strLiteral };
         lexer->advance();

         for (;;) {
            Expression::SharedPtr expr;

            if (lexer->currentToken.is_punctuator('{')) {
               lexer->advance();
               expr = parse_expression();

               lexer->advance();
               if (!lexer->currentToken.is_punctuator('}')) {
                  ParseError::raise("Expected '}", lexer);
               }
            }
            else {
               expr = std::make_shared<IdentifierRefExpr>(lexer->s_val());
            }

            lexer->continueInterpolation = true;
            lexer->advance();

            strings.push_back(expr);
            strings.push_back(std::make_shared<StringLiteral>(lexer->s_val()));

            lexer->continueInterpolation = false;
            if (!lexer->currentToken.isInterpolationStart) {
               break;
            }

            lexer->advance();
         }

         auto interp = std::make_shared<StringInterpolation>(std::move(strings));
         setIndex(interp.get(), start);
         interp->setMemberExpr(try_parse_member_expr());
         interp->setAttributes(attributes);

         return interp;
      }
      else if (val.type == VariantType::VOID) {
         auto noneLiteral = std::make_shared<NoneLiteral>();
         setIndex(noneLiteral.get(), start);
         noneLiteral->setAttributes(attributes);

         return noneLiteral;
      }

      auto expr = std::make_shared<NumericLiteral>(val);
      expr->setMemberExpr(try_parse_member_expr());
      setIndex(expr.get(), start);
      expr->setAttributes(attributes);

      return expr;
   }
   else {
      ParseError::raise("Expected expression but found " +
               util::token_names[lexer->currentToken.get_type()], lexer);
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
   Token start = lexer->currentToken;

   // prefix unary op
   if (isPrefixUnaryOp(lexer->currentToken)) {
      UnaryOperator::SharedPtr unary_op = std::make_shared<UnaryOperator>(lexer->s_val(), "prefix");
      lexer->advance();

      unary_op->setTarget(parse_unary_expr());
      setIndex(unary_op.get(), start);

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
      setIndex(unary_op.get(), start);

      return unary_op;
   }

   // call
   if (next.is_punctuator('(') || is_generic_call()) {
      lexer->advance();

      auto generics = parse_concrete_generics();

      auto call = std::make_shared<CallExpr>(CallType::FUNC_CALL, parse_arguments());
      call->set_generics(generics);
      expr->setMemberExpr(call);
   }

   return expr;
}

TertiaryOperator::SharedPtr Parser::parse_tertiary_operator(Expression::SharedPtr cond)
{
   Token start = lexer->currentToken;

   lexer->advance();
   Expression::SharedPtr if_branch = parse_expression();

   lexer->advance();
   if (!lexer->currentToken.is_operator(":")) {
      ParseError::raise("Expected ':' in tertiary expression", lexer);
   }

   lexer->advance();
   Expression::SharedPtr else_branch = parse_expression();

   auto op = std::make_shared<TertiaryOperator>(cond, if_branch, else_branch);
   setIndex(op.get(), start);

   return op;
}

ParenExprType Parser::get_paren_expr_type()
{
   int open_parens = 1;
   int closed_parens = 0;
   bool isTuple = false;

   auto index = lexer->currentToken.getStart();
   auto buf = lexer->getSrc() + index;
   auto len = lexer->srcLen;

   assert(*buf == '(');
   ++buf;

   while (index < len && open_parens > closed_parens) {
      switch (*buf) {
         case '(':
            ++open_parens;
            break;
         case ')':
            ++closed_parens;
            break;
         case ',':
            isTuple = true;
            break;
         default:
            break;
      }

      ++buf;
      ++index;
   }

   if (open_parens != closed_parens) {
      ParseError::raise("Expression contains unmatched parentheses", lexer);
   }

   auto isLambda = false;
   while (index < len) {
      switch (*buf) {
         case ' ':
            ++buf;
            continue;
         case '-':
         case '=':
            if (*(++buf) == '>') {
               isLambda = true;
            }
         default:
            goto end;
      }
   }

   end:

   // lambda
   if (isLambda) {
      return ParenExprType::LAMBDA;
   }
   else if (isTuple) {
      return ParenExprType::TUPLE;
   }

   return ParenExprType::EXPR;
}

bool Parser::is_generic_call()
{
   auto buf = lexer->getSrc() + lexer->lookahead().getStart();
   if (*buf != '<') {
      return false;
   }

   auto index = lexer->currentToken.getStart();
   auto len = lexer->srcLen;

   while (*buf != '>' && index < len) {
      ++buf;
      ++index;

      if (*buf == '\n') {
         return false;
      }
   }

   return (index < len && *(++buf) == '(');
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
         if (!lexer->currentToken.is_punctuator(')')) {
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

   Token start = lexer->currentToken;
   std::vector<pair<string, Expression::SharedPtr>> elements;

   lexer->advance();

   while (!lexer->currentToken.is_punctuator(')'))
   {
      std::vector<Attribute> attributes = parse_attributes();
      string label;

      if (lexer->currentToken.get_type() == T_IDENT && lexer->lookahead().is_operator(":")) {
         label = lexer->s_val();
         lexer->advance();
      }

      auto expr = parse_expression();
      expr->setAttributes(attributes);

      elements.emplace_back(label, expr);

      lexer->advance();

      if (lexer->currentToken.is_punctuator(',')) {
         lexer->advance();
      }
      else if (!lexer->currentToken.is_punctuator(')')) {
         ParseError::raise("Expected ',' or ')'", lexer);
      }
   }

   if (!lexer->currentToken.is_punctuator(')')) {
      ParseError::raise("Expected closing parenthesis after tuple literal", lexer);
   }

   auto tuple = std::make_shared<TupleLiteral>(elements);
   setIndex(tuple.get(), start);

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
   Token start = lexer->currentToken;

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
      setIndex(binary_op.get(), start);
      binary_op->setLhs(lhs);
      binary_op->setRhs(rhs);

      lhs = binary_op;
   }

   // tertiary operator
   if (next.is_operator("?")) {
      lexer->advance(false);
      auto op = parse_tertiary_operator(lhs);
      setIndex(op.get(), start);

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

   while (lexer->currentToken.get_type() == T_KEYWORD) {
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
      Token start = lexer->currentToken;
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

      typeref->getType(true).isConst(is_const);

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
      setIndex(decl_stmt.get(), start);
      decl_stmt->isDeclaration(is_declaration);

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

std::vector<pair<string, std::shared_ptr<TypeRef>>> Parser::parse_tuple_type()
{
   std::vector<pair<string, std::shared_ptr<TypeRef>>> tupleTypes;

   while (!lexer->currentToken.is_punctuator(')')) {
      lexer->advance();

      if (lexer->currentToken.is_punctuator(')')) {
         break;
      }

      string label;
      if (lexer->currentToken.get_type() == T_IDENT && lexer->lookahead().is_operator(":")) {
         label = lexer->s_val();
         lexer->advance();
         lexer->advance();
      }

      bool vararg = false;
      if (lexer->currentToken.is_operator("...")) {
         vararg = true;
         lexer->advance();

         // c style vararg
         if (lexer->currentToken.get_type() != T_IDENT) {
            auto typeref = std::make_shared<TypeRef>();
            typeref->isCStyleVararg(true);

            tupleTypes.emplace_back(label, typeref);
            continue;
         }
      }

      auto argType = parse_type();
      argType->isVararg(vararg);

      tupleTypes.emplace_back(label, argType);
      lexer->advance();
      if (!lexer->currentToken.is_punctuator(',') && !lexer->currentToken.is_punctuator(')')) {
         ParseError::raise("Expected closing parenthesis after argument list", lexer);
      }
   }

   return tupleTypes;
}

/**
 * Parses a method's argument list
 * @return
 */
std::vector<FuncArgDecl::SharedPtr> Parser::parse_arg_list(bool optionalNames, bool optionalTypes)
{
   Token start = lexer->currentToken;

   std::vector<FuncArgDecl::SharedPtr> args;
   if (!lexer->currentToken.is_punctuator('(')) {
      lexer->backtrack();
      return args;
   }

   bool def_arg = false;
   bool var_arg = false;

   lexer->advance();

   while (!lexer->currentToken.is_punctuator(')'))
   {
      if (var_arg) {
         ParseError::raise("Vararg arguments can only be the last argument of a "
                     "function", lexer);
      }

      std::vector<Attribute> attributes = parse_attributes();

      start = lexer->currentToken;
      FuncArgDecl::SharedPtr arg_dec = std::make_shared<FuncArgDecl>();
      arg_dec->setAttributes(attributes);

      if (!optionalNames || lexer->lookahead().is_operator(":")) {
         if (lexer->currentToken.get_type() != T_IDENT) {
            ParseError::raise("Expected identifier", lexer);
         }

         arg_dec->setName(lexer->s_val());
         lexer->advance();
      }
      else if (lexer->currentToken.is_operator("...")) {
         var_arg = true;
         lexer->advance();
         if (lexer->lookahead().get_type() != T_IDENT) {
            auto typeref = std::make_shared<TypeRef>();
            typeref->isCStyleVararg(true);

            arg_dec->setType(typeref);
            setIndex(arg_dec.get(), start);

            args.push_back(arg_dec);
            break;
         }
      }

      if (lexer->currentToken.is_operator(":") || optionalNames) {
         if (lexer->currentToken.is_operator(":")) {
            lexer->advance();
         }

         auto argTy = parse_type();
         argTy->isVararg(var_arg);

         arg_dec->setType(argTy);
         lexer->advance();
      }
      else if (!optionalTypes) {
         ParseError::raise("Function arguments have to have a specified type", lexer);
      }

      // optional default value
      if (lexer->currentToken.is_operator("=")) {
         if (var_arg) {
            ParseError::raise("Vararg arguments cannot have a default value", lexer);
         }

         lexer->advance();

         Expression::SharedPtr _expr = parse_expression();
         arg_dec->setDefault(_expr);
         def_arg = true;

         lexer->advance();

      } else if (def_arg) {
         ParseError::raise("Default values are only allowed as last items of an argument list", lexer);
      }

      setIndex(arg_dec.get(), start);
      args.push_back(arg_dec);

      // end of argument list or next argument
      if (lexer->currentToken.is_punctuator(',')) {
         lexer->advance();
      }
      else if (!lexer->currentToken.is_punctuator(')')) {
         ParseError::raise("Expected closing parenthesis after argument list", lexer);
      }

      start = lexer->currentToken;
   }

   if (!lexer->currentToken.is_punctuator(')')) {
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
   Token start = lexer->currentToken;
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
      while (isValidOperatorChar(lexer->currentToken)) {
         op += lexer->s_val();
         lexer->advance(false, true);
      }

      if (lexer->currentToken.is_punctuator(' ')) {
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
   fun_dec->setAttributes(attributes);
   attributes.clear();

   std::vector<Statement::SharedPtr> innerDecls;
   CurrentFuncDecls = &innerDecls;

   if (funcName == "main") {
      for (const auto& stmt : implicit_main_stmts) {
         if (stmt->get_type() != NodeType::DECLARATION) {
            ParseError::raise("Cannot define a main method when top-level non-declaration "
                           "statements exist", lexer);
         }
      }
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

   auto generics = parse_generics();
   if (generics.empty()) {
      lexer->advance();
   }

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
      fun_dec->setReturnType(std::make_shared<TypeRef>());
   }

   if (lexer->lookahead().is_punctuator('{') || lexer->lookahead().is_keyword("unsafe")) {
      if (is_declaration) {
         ParseError::raise("Declared functions cannot have a body", lexer);
      }

      // function body
      CompoundStmt::SharedPtr func_body = parse_block();
      fun_dec->setBody(func_body);
   }

   CurrentClassGenerics.clear();
   CurrentFuncDecls = nullptr;

   fun_dec->isDeclaration(is_declaration);
   fun_dec->setInnerDecls(std::move(innerDecls));

   setIndex(fun_dec.get(), start);
   return fun_dec;
}

/**
 * Parses a lambda expression
 * @return
 */
LambdaExpr::SharedPtr Parser::parse_lambda_expr()
{
   Token start = lexer->currentToken;
   auto args = parse_arg_list(false, true);

   lexer->advance();
   auto retType = std::make_shared<TypeRef>();

   if (lexer->currentToken.is_operator("->")) {
      lexer->advance();
      retType = parse_type();
      lexer->advance();
   }

   if (lexer->currentToken.is_operator("=>")) {
      lexer->advance(); // =>
   }
   else {
      llvm_unreachable("function should never be called in this case");
   }

   Statement::SharedPtr body = parse_next_stmt();

   auto lambda_expr = std::make_shared<LambdaExpr>(retType, args, body);

   setIndex(lambda_expr.get(), start);
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
   Token start = lexer->currentToken;

   lexer->advance();
   std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list(optionalNames);

   std::vector<Statement::SharedPtr> innerDecls;
   CurrentFuncDecls = &innerDecls;

   CompoundStmt::SharedPtr constr_body = nullptr;
   if (!optionalNames) {
      constr_body = parse_block();
   }

   auto constr = std::make_shared<ConstrDecl>(args, constr_body, am);
   setIndex(constr.get(), start);
   constr->setInnerDecls(std::move(innerDecls));

   CurrentFuncDecls = nullptr;

   return constr;
}

DestrDecl::SharedPtr Parser::parse_destr_decl()
{
   Token start = lexer->currentToken;

   lexer->advance();
   std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list();
   if (!args.empty()) {
      ParseError::raise("Deinitializers cannot have arguments", lexer);
   }

   std::vector<Statement::SharedPtr> innerDecls;
   CurrentFuncDecls = &innerDecls;

   CompoundStmt::SharedPtr body = parse_block();

   auto destr = std::make_shared<DestrDecl>(std::move(body));
   setIndex(destr.get(), start);
   destr->setInnerDecls(std::move(innerDecls));

   CurrentFuncDecls = nullptr;

   return destr;
}

/**
 * Parses a class field declaration
 * @param am
 * @param is_static
 * @return
 */
FieldDecl::SharedPtr Parser::parse_field_decl(
   AccessModifier am,
   bool is_static,
   bool isConst,
   bool isDeclaration,
   bool isProp)
{
   if (lexer->currentToken.get_type() != T_IDENT) {
      ParseError::raise("Field name must be a valid identifier", lexer);
   }

   Token start = lexer->currentToken;
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

      while (!lexer->currentToken.is_punctuator('}')) {
         if (lexer->currentToken.get_type() == T_IDENT && lexer->s_val() == "get") {
            if (getter) {
               ParseError::raise("Getter already declared", lexer);
            }

            getter = true;

            if (lexer->lookahead().is_punctuator('{')) {
               if (isDeclaration) {
                  ParseError::raise("Fields of declared classes cannot define a getter method", lexer);
               }

               auto body = parse_block();
               field->addGetter(body);

               lexer->advance();
            }
            else {
               field->addGetter();
               lexer->advance();
            }
         }
         else if (lexer->currentToken.get_type() == T_IDENT && lexer->s_val() == "set") {
            if (setter) {
               ParseError::raise("Setter already declared", lexer);
            }

            setter = true;

            if (lexer->lookahead().is_punctuator('{')) {
               if (isDeclaration) {
                  ParseError::raise("Fields of declared classes cannot define a setter method", lexer);
               }

               auto body = parse_block();
               field->addSetter(body);

               lexer->advance();
            }
            else {
               field->addSetter();
               lexer->advance();
            }
         }
         else if (lexer->currentToken.is_punctuator(',')) {
            lexer->advance();
         }
         else {
            ParseError::raise("Expected 'get' or 'set'", lexer);
         }
      }

      next = lexer->lookahead();
   }

   if (isProp && !getter && !setter) {
      ParseError::raise("property must define a getter or a setter", lexer);
   }

   // optional default value
   if (next.is_operator("=")) {
      if (isDeclaration) {
         ParseError::raise("Fields of declared classes cannot define a default value", lexer);
      }
      if (isProp) {
         ParseError::raise("properties cannot have default values", lexer);
      }

      lexer->advance();
      lexer->advance();
      field->setDefault(parse_expression());
   }
   else if (!typeDeclared) {
      ParseError::raise("Fields have to have an annotated type or a default value", lexer);
   }

   setIndex(field.get(), start);

   field->isProperty(isProp);
   return field;
}

/**
 * Parses a method declaration
 * @param am
 * @param isStatic
 * @param ts
 * @return
 */
MethodDecl::SharedPtr Parser::parse_method_decl(
   AccessModifier am,
   bool isStatic,
   bool allowOmittedBody)
{
   if (lexer->currentToken.get_type() != T_IDENT) {
      ParseError::raise("Method name must be a valid identifier", lexer);
   }

   Token start = lexer->currentToken;
   string method_name = lexer->s_val();

   lexer->advance();

   // method alias
   if (lexer->currentToken.is_operator("=")) {
      lexer->advance();

      if (lexer->currentToken.get_type() != T_IDENT && lexer->currentToken.get_type() != T_OP) {
         ParseError::raise("Method aliasee must be a valid identifier", lexer);
      }

      string aliasee = lexer->s_val();
      lexer->advance();

      auto args = parse_arg_list(true);

      auto op_decl = std::make_shared<MethodDecl>(method_name, aliasee, args);
      setIndex(op_decl.get(), start);

      return op_decl;
   }

   auto generics = parse_generics();
   if (!generics.empty()) {
      lexer->advance();
   }

   std::vector<Statement::SharedPtr> innerDecls;
   CurrentFuncDecls = &innerDecls;

   std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list(allowOmittedBody);

   // optional return type
   Token next = lexer->lookahead();
   auto typeref = std::make_shared<TypeRef>();
   if (next.is_operator("->")) {
      lexer->advance();
      lexer->advance();
      typeref = parse_type();
   }


   MethodDecl::SharedPtr method;
   if (lexer->lookahead().is_punctuator('{') || lexer->lookahead().is_keyword("unsafe")) {
      CompoundStmt::SharedPtr method_body = parse_block();

      lexer->advance(false);
      if (!lexer->currentToken.is_separator()) {
         ParseError::raise("Method declarations must be on seperate lines", lexer);
      }

      method = std::make_shared<MethodDecl>(method_name, typeref, args, method_body,
         am, isStatic);
   }
   else if (!allowOmittedBody) {
      ParseError::raise("Abstract methods can only be declared in a protocol or an abstract "
         "class", lexer);
   }
   else {
      method = std::make_shared<MethodDecl>(method_name, typeref, args, am, isStatic);
   }

   setIndex(method.get(), start);
   method->setGenerics(generics);
   method->setInnerDecls(std::move(innerDecls));

   CurrentFuncDecls = nullptr;

   return method;
}

/**
 * Parses an operator method declaration
 * @param am
 * @return
 */
MethodDecl::SharedPtr Parser::parse_operator_decl(AccessModifier am, bool is_interface)
{
   Token start = lexer->currentToken;
   auto opType = lexer->s_val();
   lexer->advance();

   string op;
   bool isCastOp = false;
   TypeRef::SharedPtr castTarget = nullptr;

   if (lexer->currentToken.is_punctuator('(') && lexer->lookahead().is_punctuator(')')) {
      lexer->advance();
      lexer->advance();
      op = "()";
   }
   else if (lexer->currentToken.is_punctuator('[') && lexer->lookahead().is_punctuator(']')) {
      lexer->advance();
      op = "[]";
      lexer->advance();
   }
   else if (lexer->currentToken.is_operator("as")) {
      lexer->advance();
      castTarget = parse_type();
      isCastOp = true;

      lexer->advance();
   }
   else {
      while (isValidOperatorChar(lexer->currentToken)) {
         op += lexer->s_val();
         lexer->advance(false, true);
      }

      if (lexer->currentToken.is_punctuator(' ')) {
         lexer->advance();
      }

      if (!util::matches("(..+|[^.])*", op)) {
         ParseError::raise("Custom operators can only contain periods in sequences of two or "
            "more", lexer);
      }
   }

   // method alias
   if (lexer->currentToken.is_operator("=")) {
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

      if (lexer->currentToken.get_type() != T_IDENT && lexer->currentToken.get_type() != T_OP) {
         ParseError::raise("Method aliasee must be a valid identifier", lexer);
      }

      string aliasee = lexer->s_val();
      auto generics = parse_generics();
      lexer->advance();

      auto args = parse_arg_list(true);

      auto op_decl = std::make_shared<MethodDecl>(opType + " " + op, aliasee, args);
      setIndex(op_decl.get(), start);
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
   }
   else {
      typeref = castTarget != nullptr ? castTarget : std::make_shared<TypeRef>();
      setIndex(typeref.get(), start);
   }

   MethodDecl::SharedPtr op_decl;
   if (lexer->lookahead().is_punctuator('{') || lexer->lookahead().is_keyword("unsafe")) {
      CompoundStmt::SharedPtr body = parse_block();
      op_decl = std::make_shared<MethodDecl>(opType + " " + op, typeref, args, body, am, false);
   }
   else if (!is_interface) {
      ParseError::raise("Abstract methods can only be declared in an interface or an abstract "
               "class", lexer);
   }
   else {
      op_decl = std::make_shared<MethodDecl>(opType + " " + op, typeref, args, am, false);
   }

   op_decl->isCastOp(isCastOp);
   setIndex(op_decl.get(), start);

   return op_decl;
}

std::shared_ptr<EnumCaseDecl> Parser::parse_enum_case()
{
   Token start = lexer->currentToken;
   
   if (lexer->currentToken.get_type() != T_IDENT) {
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

      auto expr = parse_expression();
      caseDecl = std::make_shared<EnumCaseDecl>(std::move(caseName), std::move(expr),
         std::move(associatedTypes));
   }
   else {
      caseDecl = std::make_shared<EnumCaseDecl>(std::move(caseName), std::move(associatedTypes));
   }

   setIndex(caseDecl.get(), start);
   return caseDecl;
}

std::vector<ExtensionConstraint> Parser::parse_ext_constraints()
{
   std::vector<ExtensionConstraint> constraints;

   for (;;) {
      if (lexer->currentToken.get_type() != T_IDENT) {
         ParseError::raise("Expected identifier", lexer);
      }

      ExtensionConstraint constraint;
      constraint.constrainedGenericTypeName = lexer->s_val();
      lexer->advance();

      bool expectType = false;
      if (lexer->currentToken.is_operator("==")) {
         constraint.kind = ExtensionConstraint::TYPE_EQUALITY;
         expectType = true;
         lexer->advance();
      }
      else if (lexer->currentToken.is_operator("!=")) {
         constraint.kind = ExtensionConstraint::TYPE_INEQUALITY;
         expectType = true;
         lexer->advance();
      }
      else if (lexer->currentToken.is_operator(":")) {
         lexer->advance();
         if (lexer->currentToken.is_keyword("struct")) {
            constraint.kind = ExtensionConstraint::IS_STRUCT;
         }
         else if (lexer->currentToken.is_keyword("class")) {
            constraint.kind = ExtensionConstraint::IS_CLASS;
         }
         else if (lexer->currentToken.is_keyword("enum")) {
            constraint.kind = ExtensionConstraint::IS_ENUM;
         }
         else if (lexer->currentToken.is_keyword("protocol")) {
            constraint.kind = ExtensionConstraint::IS_PROTOCOL;
         }
         else if (lexer->currentToken.is_keyword("default")) {
            constraint.kind = ExtensionConstraint::DEFAULT_CONSTRUCTIBLE;
         }
         else {
            constraint.kind = ExtensionConstraint::CONFORMANCE;
            expectType = true;
         }
      }

      if (expectType) {
         constraint.typeConstraint = parse_type();
      }

      constraints.push_back(constraint);
      if (lexer->lookahead().is_punctuator(',')) {
         lexer->advance();
      }
      else {
         break;
      }
   }

   lexer->advance();

   return constraints;
}

string Parser::parse_nested_type_name()
{
   string typeName;
   while (lexer->currentToken.get_type() == T_IDENT) {
      typeName += lexer->s_val();

      if (lexer->lookahead().is_punctuator('.')) {
         lexer->advance();
         lexer->advance();
      }
      else {
         break;
      }
   }

   return typeName;
}

std::vector<GenericConstraint> Parser::parse_generics()
{
   std::vector<GenericConstraint> generics;

   if (!lexer->lookahead().is_operator("<")) {
      return generics;
   }

   lexer->advance();
   lexer->advance();

   while (!lexer->currentToken.is_operator(">")) {
      if (lexer->currentToken.get_type() != T_IDENT) {
         ParseError::raise("Expected typename", lexer);
      }

      GenericConstraint gen;
      gen.genericTypeName = lexer->s_val();

      lexer->advance();

      // A: +B -> A is superclass of B (contravariance)
      // A: B or A: -B -> A is subclass of B (covariance)
      if (lexer->currentToken.is_operator(":")) {
         lexer->advance();

         for (;;) {
            bool isCovar = false;
            if (lexer->currentToken.is_operator("-")) {
               isCovar = true;
               lexer->advance();
            }

            if (isCovar || lexer->currentToken.get_type() == T_IDENT) {
               if (!gen.covarName.empty()) {
                  ParseError::raise("Duplicate definition of covariance", lexer);
               }

               gen.covarName = parse_nested_type_name();
               lexer->advance();
            }
            else if (lexer->currentToken.is_operator("+")) {
               if (!gen.contravarName.empty()) {
                  ParseError::raise("Duplicate definition of contravariance", lexer);
               }

               lexer->advance();
               gen.contravarName = parse_nested_type_name();
               lexer->advance();
            }
            else {
               break;
            }
         }
      }

      generics.push_back(gen);

      if (!lexer->currentToken.is_operator(">") && !lexer->currentToken.is_punctuator(',')) {
         ParseError::raise("Expected ',' or '>'", lexer);
      }
      if (lexer->currentToken.is_punctuator(',')) {
         lexer->advance();
      }
   }

   lexer->advance();

   return generics;
}

std::vector<std::shared_ptr<TypeRef>> Parser::parse_concrete_generics()
{
   std::vector<std::shared_ptr<TypeRef>> generics;

   if (!lexer->currentToken.is_operator("<")) {
      return generics;
   }

   lexer->advance();
   while (!lexer->currentToken.is_operator(">")) {
      if (lexer->currentToken.get_type() != T_IDENT) {
         ParseError::raise("Expected typename", lexer);
      }

      auto type = parse_type();
      lexer->advance();

      if (!lexer->currentToken.is_operator(">") && !lexer->currentToken.is_punctuator(',')) {
         ParseError::raise("Expected ',' or '>'", lexer);
      }
      if (lexer->currentToken.is_punctuator(',')) {
         lexer->advance();
      }

      generics.push_back(type);
   }

   return generics;
}

std::vector<Statement::SharedPtr> Parser::parse_class_inner(
   bool isStruct,
   bool isProtocol,
   bool isExtension,
   bool isDeclaration)
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

      Token start = lexer->currentToken;
      AccessModifier current_am = AccessModifier::DEFAULT;
      bool am_set = false;
      bool is_static = false;
      auto type = DeclType::NONE;
      bool memberwiseInit = false;
      bool isConstField = false;
      bool isMutating = false;
      bool isInnerDeclaration = false;
      bool isProp = false;

      attributes = parse_attributes();

      while (lexer->currentToken.get_type() == T_KEYWORD && lexer->s_val() != "as") {
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
         else if (keyword == "prop") {
            type = DeclType::FIELD;
            isProp = true;
         }
         else if (keyword == "def") {
            type = DeclType::METHOD;
         }
         else if (keyword == "mutating") {
            isMutating = true;
         }
         else if (keyword == "memberwise") {
            lexer->advance();
            if (!lexer->currentToken.is_keyword("init")) {
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
            if (isProtocol) {
               ParseError::raise("Protocols cannot contain deinitializers", lexer);
            }
            if (isDeclaration) {
               ParseError::raise("Declarations cannot contain deinitializers", lexer);
            }

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
         else if (keyword == "declare") {
            isInnerDeclaration = true;
         }
         else {
            ParseError::raise("Unexpected keyword '" + keyword + "' in class declaration", lexer);
         }

         if (type == DeclType::TYPEDEF && is_static) {
            ParseError::raise("Typedefs are static by default", lexer);
         }

         start = lexer->currentToken;
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
         declarations.push_back(parse_typedef(current_am));
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
            auto constr = parse_constr_decl(current_am,
               isProtocol || isDeclaration || isInnerDeclaration);
            declarations.push_back(constr);
         }
      }
      else if (type == DeclType::DESTR) {
         lexer->backtrack();

         auto destr = parse_destr_decl();
         declarations.push_back(destr);
      }
      else if (type == DeclType::FIELD) {
         if (isExtension && !is_static) {
            ParseError::raise("Extensions can only contain static fields", lexer);
         }

         auto field = parse_field_decl(current_am, is_static, isConstField,
            isDeclaration || isInnerDeclaration, isProp);
         declarations.push_back(field);
      }
      else if (type == DeclType::METHOD) {
         bool isOperator = false;
         if (lexer->currentToken.get_type() == T_KEYWORD) {
            auto keyword = lexer->s_val();
            if (keyword == "infix" || keyword == "prefix" || keyword == "postfix") {
               isOperator = true;
            }
         }

         if (isOperator) {
            auto op = parse_operator_decl(current_am,
               isProtocol || isDeclaration || isInnerDeclaration);
            op->isMutating(isMutating);

            declarations.push_back(op);
         }
         else {
            auto method = parse_method_decl(current_am, is_static,
               isProtocol || isDeclaration || isInnerDeclaration);
            method->isMutating(isMutating);

            declarations.push_back(method);
         }
      }
      else if (type == DeclType::CASE) {
         case_start:
         auto case_ = parse_enum_case();
         declarations.push_back(case_);

         if (lexer->lookahead().is_punctuator(',')) {
            lexer->advance();
            lexer->advance();
            goto case_start;
         }
      }
      else if (type == DeclType::INNER_CLASS || type == DeclType::INNER_STRUCT) {
         lexer->backtrack();

         auto cl = parse_class_decl(type == DeclType::INNER_STRUCT, false,
            isDeclaration || isInnerDeclaration);

         declarations.push_back(cl);
      }
      else if (type == DeclType::INNER_ENUM) {
         lexer->backtrack();

         auto en = parse_enum_decl(isDeclaration || isInnerDeclaration);
         declarations.push_back(en);
      }
      else {
         declaration_finished = true;
      }

      if (!declarations.empty()) {
         setIndex(declarations.back().get(), start);
         declarations.back()->setAttributes(attributes);
         declarations.back()->isDeclaration(declarations.back()->isDeclaration() || isDeclaration ||
            isInnerDeclaration);
      }

      Token next = lexer->lookahead();
      if (next.is_punctuator(';')) {
         lexer->advance();
         next = lexer->lookahead();
      }
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

   while (lexer->currentToken.get_type() == T_KEYWORD) {
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
   if (lexer->currentToken.get_type() != T_IDENT) {
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
      if (!lexer->currentToken.is_punctuator(')')) {
         ParseError::raise("Expected ')'", lexer);
      }
   }

   TypeRef::SharedPtr extends = nullptr;
   std::vector<TypeRef::SharedPtr> with;
   std::vector<ExtensionConstraint> constraints;

   auto generics = parse_generics();
   if (generics.empty()) {
      lexer->advance();
   }

   while (!lexer->currentToken.is_punctuator('{') && lexer->currentToken.get_type() != T_EOF) {
      if (lexer->currentToken.is_operator(":")) {
         if (extends != nullptr) {
            ParseError::raise("Classes can only inherit from one other class", lexer);
         }

         lexer->advance();
         if (lexer->currentToken.get_type() != T_IDENT) {
            ParseError::raise("Expected identifier after ':'", lexer);
         }

         extends = parse_type();
         lexer->advance();
      }
      else if (lexer->currentToken.is_keyword("with")) {
         lexer->advance();
         if (lexer->currentToken.get_type() != T_IDENT) {
            ParseError::raise("Expected identifier after 'with'", lexer);
         }

         while (lexer->currentToken.get_type() == T_IDENT && lexer->currentToken.get_type() != T_EOF) {
            auto proto = parse_type();
            with.push_back(proto);
            lexer->advance();

            if (lexer->currentToken.is_punctuator(',')) {
               lexer->advance();
            }
         }
      }
      else if (lexer->currentToken.is_keyword("where")) {
         lexer->advance();
         constraints = parse_ext_constraints();
      }
      else {
         lexer->backtrack();
         break;
      }
   }

   return ClassHead { am, className, extends, with, generics, isAbstract, enumRawType, constraints };
}

/**
 * Parses a class declaration
 * @return
 */
ClassDecl::SharedPtr Parser::parse_class_decl(
   bool isStruct,
   bool isProtocol,
   bool isDeclaration)
{
   Token start = lexer->currentToken;

   auto attrs = attributes;
   auto head = parse_class_head();

   if (isProtocol && head.isAbstract) {
      ParseError::raise("Protocols cannot be declared abstract", lexer);
   }
   if (isProtocol && head.extends != nullptr) {
      ParseError::raise("Protocols cannot inherit from classes or conform to protocols", lexer);
   }
   if (!lexer->currentToken.is_punctuator('{')) {
      ParseError::raise("Expected '{' to start a class definition", lexer);
   }

   std::vector<ConstrDecl::SharedPtr> constructors;
   std::vector<FieldDecl::SharedPtr> fields;
   std::vector<MethodDecl::SharedPtr> methods;
   std::vector<TypedefDecl::SharedPtr> typedefs;
   std::vector<Statement::SharedPtr> innerDeclarations;
   DestrDecl::SharedPtr destructor = nullptr;

   auto declarations = parse_class_inner(isStruct, isProtocol, false, isDeclaration);
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
         head.extends, std::move(head.conformsTo), std::move(destructor), std::move(innerDeclarations),
         std::move(head.constraints)
      );
   }

   class_decl->isDeclaration(isDeclaration);
   class_decl->isStruct(isStruct);
   class_decl->setAttributes(attrs);
   attributes.clear();

   setIndex(class_decl.get(), start);
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

ClassDecl::SharedPtr Parser::parse_extend_stmt(bool isDeclaration)
{
   auto cl = parse_class_decl(false, false, isDeclaration);
   if (cl->getParentClass() != nullptr) {
      ParseError::raise("Extensions cannot add base classes", lexer);
   }

   cl->isExtension(true);
   return cl;
}

std::shared_ptr<EnumDecl> Parser::parse_enum_decl(bool isDeclaration)
{
   Token start = lexer->currentToken;
   auto attrs = attributes;
   auto head = parse_class_head(true);

   if (head.isAbstract) {
      ParseError::raise("Enums cannot be declared abstract", lexer);
   }
   if (head.extends != nullptr) {
      ParseError::raise("Enums cannot inherit", lexer);
   }
   if (!lexer->currentToken.is_punctuator('{')) {
      ParseError::raise("Expected '{' to start an enum definition", lexer);
   }

   std::vector<MethodDecl::SharedPtr> methods;
   std::vector<EnumCaseDecl::SharedPtr> cases;
   std::vector<Statement::SharedPtr> innerDeclarations;

   auto declarations = parse_class_inner(false, false, false, isDeclaration);
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

   enumDecl->isDeclaration(isDeclaration);
   setIndex(enumDecl.get(), start);
   return enumDecl;
}

/**
 * Parses an if/else statement
 * @return
 */
IfStmt::SharedPtr Parser::parse_if_stmt()
{
   Token start = lexer->currentToken;
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

   setIndex(if_stmt.get(), start);

   return if_stmt;
}

CaseStmt::SharedPtr Parser::parse_case_stmt(bool default_)
{
   Token start = lexer->currentToken;
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

   if (!lexer->currentToken.is_operator(":")) {
      ParseError::raise("Expected ':' after case label", lexer);
   }

   lexer->advance();
   Statement::SharedPtr stmt;
   if (lexer->currentToken.is_keyword("case") || lexer->currentToken.is_punctuator('}')
      || lexer->currentToken.is_keyword("default"))
   {
      // implicit fallthrough
      stmt = nullptr;
   }
   else {
      stmt = parse_next_stmt();
   }

   if (stmt && stmt->get_type() != NodeType::COMPOUND_STMT) {
      CompoundStmt::SharedPtr compound = std::make_shared<CompoundStmt>();
      compound->addStatement(stmt);
      lexer->advance();

      while (!lexer->currentToken.is_keyword("case") && !lexer->currentToken.is_keyword("default") &&
         !lexer->currentToken.is_punctuator('}'))
      {
         compound->addStatement(parse_next_stmt());
         lexer->advance();
      }

      case_stmt->setBody(compound);
   }
   else {
      case_stmt->setBody(stmt);
   }

   setIndex(case_stmt.get(), start);
   return case_stmt;
}

MatchStmt::SharedPtr Parser::parse_match_stmt()
{
   Token start = lexer->currentToken;
   lexer->advance();

   Expression::SharedPtr switch_val = parse_expression();
   lexer->advance();
   if (!lexer->currentToken.is_punctuator('{')) {
      ParseError::raise("Expected '{' after 'match'", lexer);
   }

   MatchStmt::SharedPtr switch_stmt = std::make_shared<MatchStmt>(switch_val);
   lexer->advance();

   bool isDefault = false;
   while (!lexer->currentToken.is_punctuator('}')) {
      if (isDefault) {
         ParseError::raise("Expected no more cases after 'default'", lexer);
      }

      isDefault = lexer->currentToken.is_keyword("default");
      if (!lexer->currentToken.is_keyword("case") && !isDefault) {
         ParseError::raise("Expected 'case' or 'default'", lexer);
      }

      auto caseStmt = parse_case_stmt(isDefault);
      switch_stmt->addCase(caseStmt);
   }

   setIndex(switch_stmt.get(), start);
   return switch_stmt;
}

WhileStmt::SharedPtr Parser::parse_while_stmt(bool conditionBefore)
{
   Token start = lexer->currentToken;
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
   setIndex(whileStmt.get(), start);

   return whileStmt;
}

Statement::SharedPtr Parser::parse_for_stmt()
{
   Token start = lexer->currentToken;
   lexer->advance();

   Statement::SharedPtr init;
   if (lexer->currentToken.is_punctuator(';')) {
      init = nullptr;
   }
   else {
      init = parse_next_stmt();
      lexer->advance();
   }

   // range based for loop
   if (lexer->currentToken.is_identifier("in")) {
      DeclStmt::SharedPtr decl = std::dynamic_pointer_cast<DeclStmt>(init);
      if (decl == nullptr) {
         ParseError::raise("Expected declaration before 'in' in range based for loop", lexer);
      }

      lexer->advance();

      Expression::SharedPtr range = parse_expression();
      lexer->advance();

      if (lexer->currentToken.is_punctuator(')')) {
         lexer->advance();
      }

      CompoundStmt::SharedPtr body = std::static_pointer_cast<CompoundStmt>(parse_next_stmt());

      auto forInStmt = std::make_shared<ForInStmt>(std::move(decl), std::move(range), std::move(body));
      setIndex(forInStmt.get(), start);
      forInStmt->setAttributes(attributes);

      return forInStmt;
   }

   if (!lexer->currentToken.is_punctuator(';')) {
      ParseError::raise("Expected ';' to seperate for loop arguments", lexer);
   }

   Expression::SharedPtr term;
   lexer->advance();
   if (lexer->currentToken.is_punctuator(';')) {
      term = std::make_shared<NumericLiteral>(Variant(true));
   }
   else {
      term = parse_expression();
      lexer->advance();
   }

   if (!lexer->currentToken.is_punctuator(';')) {
      ParseError::raise("Expected ';' to seperate for loop arguments", lexer);
   }

   Statement::SharedPtr inc;
   lexer->advance();
   if (lexer->currentToken.is_punctuator('{')) {
      inc = nullptr;
   }
   else {
      inc = parse_next_stmt();
      lexer->advance();
   }

   CompoundStmt::SharedPtr block = std::static_pointer_cast<CompoundStmt>(parse_next_stmt());

   ForStmt::SharedPtr for_stmt = std::make_shared<ForStmt>(init, term, inc);
   for_stmt->setBody(block);
   setIndex(for_stmt.get(), start);

   return for_stmt;
}

DeclareStmt::SharedPtr Parser::parse_declare_stmt(ExternKind externKind, std::shared_ptr<DeclareStmt> decl)
{
   Token start = lexer->currentToken;
   if (lexer->lookahead().get_type() == T_IDENT) {
      lexer->advance();
      auto str = lexer->s_val();

      if (str == "C") {
         externKind = ExternKind::C;
      }
      else if (str == "C++" || str == "CPP") {
         externKind = ExternKind::CPP;
      }
      else {
         ParseError::raise("Unknown extern kind " + str, lexer);
      }
   }

   if (lexer->lookahead().is_punctuator('{')) {
      lexer->advance();
      return parse_multiple_declare_stmt(externKind);
   }

   lexer->advance();
   auto attrs = parse_attributes();

   if (lexer->currentToken.get_type() != T_KEYWORD) {
      ParseError::raise("Unexpected token", lexer);
   }

   AccessModifier access = AccessModifier::DEFAULT;
   string keyword = lexer->s_val();

   if (keyword == "public") {
      access = AccessModifier::PUBLIC;
      lexer->advance();
      keyword = lexer->s_val();
   }
   else if (keyword == "private") {
      access = AccessModifier::PRIVATE;
      lexer->advance();
      keyword = lexer->s_val();
   }
   else if (keyword == "protected") {
      access = AccessModifier::PROTECTED;
      lexer->advance();
      keyword = lexer->s_val();
   }

   bool wasNull = decl == nullptr;
   if (wasNull) {
      decl = std::make_shared<DeclareStmt>(externKind);
   }

   bool isAbstract = false;
   bool isClass = keyword == "class";
   bool isStruct = !isClass && keyword == "struct";
   bool isProtocol = !isStruct && keyword == "protocol";

   if (keyword == "extend") {
      auto ext = parse_extend_stmt(true);
      decl->addDeclaration(std::move(ext));
   }
   else if (keyword == "abstract") {
      auto cl = parse_class_decl(false, false, true);
      decl->addDeclaration(std::move(cl));
   }
   else if (keyword == "def") {
      auto func = parse_function_decl(true);
      decl->addDeclaration(std::move(func));
   }
   else if (isClass || isStruct || isProtocol) {
      auto cl = parse_class_decl(isStruct, isProtocol, true);
      decl->addDeclaration(std::move(cl));
   }
   else if (keyword == "enum") {
      auto en = parse_enum_decl(true);
      decl->addDeclaration(std::move(en));
   }
   else if (keyword == "var" || keyword == "let") {
      auto assign = std::static_pointer_cast<DeclStmt>(parse_assignment(keyword == "let", true));
      decl->addDeclaration(std::move(assign));
   }
   else {
      ParseError::raise("Unexpected keyword '" + keyword + "' after declare", lexer);
   }

   if (wasNull) {
      setIndex(decl.get(), start);
   }

   decl->setAttributes(attrs);
   return decl;
}

DeclareStmt::SharedPtr Parser::parse_multiple_declare_stmt(ExternKind externKind)
{
   Token start = lexer->currentToken;
   auto decl = std::make_shared<DeclareStmt>(externKind);

   while (!lexer->lookahead().is_punctuator('}') && lexer->currentToken.get_type() != T_EOF) {
      parse_declare_stmt(externKind, decl);
   }

   lexer->advance();

   setIndex(decl.get(), start);
   return decl;
}

TryStmt::SharedPtr Parser::parse_try_stmt()
{
   Token start = lexer->currentToken;
   lexer->advance();

   auto tryBody = parse_next_stmt();
   TryStmt::SharedPtr tryStmt = std::make_shared<TryStmt>(std::move(tryBody));

   while (lexer->lookahead().get_type() == T_KEYWORD) {
      lexer->advance();

      bool finallySet = false;
      auto keyword = lexer->s_val();

      if (keyword == "catch") {
         lexer->advance();
         CatchBlock catchBlock;

         if (lexer->currentToken.get_type() != T_IDENT) {
            ParseError::raise("expected identifier", lexer);
         }

         catchBlock.identifier = lexer->s_val();
         lexer->advance();

         if (!lexer->currentToken.is_operator(":")) {
            ParseError::raise("expected ':'", lexer);
         }

         lexer->advance();
         catchBlock.caughtType = parse_type();

         lexer->advance();
         catchBlock.body = parse_next_stmt();

         tryStmt->addCatch(std::move(catchBlock));
      }
      else if (keyword == "finally") {
         if (finallySet) {
            ParseError::raise("finally block already defined", lexer);
         }

         lexer->advance();

         auto finallyBody = parse_next_stmt();
         tryStmt->setFinally(std::move(finallyBody));

         finallySet = true;
      }
      else {
         lexer->backtrack();
         break;
      }
   }

   setIndex(tryStmt.get(), start);
   return tryStmt;
}

ThrowStmt::SharedPtr Parser::parse_throw_stmt()
{
   Token start = lexer->currentToken;
   lexer->advance();

   auto thrownVal = parse_expression();
   auto throwStmt = std::make_shared<ThrowStmt>(std::move(thrownVal));

   setIndex(throwStmt.get(), start);
   return throwStmt;
}

/**
 * Interprets a keyword statement
 */
Statement::SharedPtr Parser::parse_keyword() {
   Token start = lexer->currentToken;

   string keyword = lexer->currentToken.get_value().strVal;

   if (keyword == "var" || keyword == "let") {
      auto assignment = parse_assignment(keyword == "let");

      return assignment;
   }
   else if (keyword == "def") {
      FunctionDecl::SharedPtr fun_dec = parse_function_decl();
      setIndex(fun_dec.get(), start);

      return fun_dec;
   }
   else if (keyword == "if") {
      IfStmt::SharedPtr if_stmt = parse_if_stmt();
      setIndex(if_stmt.get(), start);

      if (top_level) {
         implicit_main_stmts.push_back(if_stmt);
         return nullptr;
      }

      return if_stmt;
   }
   else if (keyword == "while" || keyword == "loop") {
      WhileStmt::SharedPtr while_stmt = parse_while_stmt(keyword == "while");
      setIndex(while_stmt.get(), start);

      if (top_level) {
         implicit_main_stmts.push_back(while_stmt);
         return nullptr;
      }

      return while_stmt;
   }
   else if (keyword == "match") {
      auto switch_stmt = parse_match_stmt();
      setIndex(switch_stmt.get(), start);

      if (top_level) {
         implicit_main_stmts.push_back(switch_stmt);
         return nullptr;
      }

      return switch_stmt;
   }
   else if (keyword == "default") {
      auto def_stmt = parse_case_stmt(true);
      setIndex(def_stmt.get(), start);

      if (top_level) {
         implicit_main_stmts.push_back(def_stmt);
         return nullptr;
      }

      return def_stmt;
   }
   else if (keyword == "for") {
      Statement::SharedPtr for_stmt = parse_for_stmt();
      setIndex(for_stmt.get(), start);

      if (top_level) {
         implicit_main_stmts.push_back(for_stmt);
         return nullptr;
      }

      return for_stmt;
   }
   else if (keyword == "continue") {
      ContinueStmt::SharedPtr cont_stmt = std::make_shared<ContinueStmt>();
      setIndex(cont_stmt.get(), start);

      if (top_level) {
         implicit_main_stmts.push_back(cont_stmt);
         return nullptr;
      }

      return cont_stmt;
   }
   else if (keyword == "typedef") {
      lexer->advance();
      return parse_typedef(AccessModifier::PUBLIC);
   }
   else if (keyword == "unsafe") {
      lexer->advance();
      auto block = parse_block();
      block->isUnsafe(true);

      return block;
   }
   else if (keyword == "break") {
      BreakStmt::SharedPtr break_stmt = std::make_shared<BreakStmt>();
      setIndex(break_stmt.get(), start);

      if (top_level) {
         implicit_main_stmts.push_back(break_stmt);
         return nullptr;
      }

      return break_stmt;
   }
   else if (keyword == "declare") {
      return parse_declare_stmt(ExternKind::NONE);
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
      else if (next.is_keyword("typedef")) {
         if (keyword == "public") {
            return parse_typedef(AccessModifier::PUBLIC);
         }

         return parse_typedef(AccessModifier::PRIVATE);
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
   else if (keyword == "union") {
      return parse_union_decl();
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
         setIndex(return_stmt.get(), start);

         if (top_level) {
            implicit_main_stmts.push_back(return_stmt);
            return nullptr;
         }

         return return_stmt;
      }
      else {
         ReturnStmt::SharedPtr return_stmt = std::make_shared<ReturnStmt>();
         setIndex(return_stmt.get(), start);
         lexer->advance(false);

         if (top_level) {
            implicit_main_stmts.push_back(return_stmt);
            return nullptr;
         }

         return return_stmt;
      }
   }
   else if (keyword == "try") {
      return parse_try_stmt();
   }
   else if (keyword == "throw") {
      return parse_throw_stmt();
   }
   else if (keyword == "goto") {
      lexer->advance();
      auto goto_stmt = std::make_shared<GotoStmt>(lexer->s_val());
      setIndex(goto_stmt.get(), start);

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
   Token start = lexer->currentToken;

   string funcName = lexer->s_val();
   lexer->advance();

   auto generics = parse_concrete_generics();
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
   else if (funcName == "stackalloc") {
      lexer->advance();

      auto type = parse_type();
      lexer->advance(T_PUNCTUATOR);
      lexer->advance();

      auto target = parse_expression();
      lexer->advance();

      std::vector<pair<string, Expression::SharedPtr>> args;
      args.emplace_back("", type);
      args.emplace_back("", target);

      call = std::make_shared<CallExpr>(callTy, args, funcName);
   }
   else {
      call = std::make_shared<CallExpr>(callTy, parse_arguments(allowLet), funcName);
   }

   call->setMemberExpr(try_parse_member_expr());
   setIndex(call.get(), start);
   call->setAttributes(attributes);
   call->set_generics(generics);

   return call;
}

std::vector<pair<string, std::shared_ptr<Expression>>> Parser::parse_arguments(bool allowLet)
{
   std::vector<pair<string, std::shared_ptr<Expression>>> args;
   bool isLabeled = false;

   while (!lexer->currentToken.is_punctuator(')')) {
      string label;

      if (lexer->currentToken.is_punctuator('(') || lexer->currentToken.is_punctuator(',')) {
         lexer->advance();
         if (lexer->currentToken.get_type() == T_IDENT && lexer->lookahead().is_operator(":")) {
            label = lexer->s_val();
            lexer->advance();
            lexer->advance();

            isLabeled = true;
         }
         else if (lexer->currentToken.is_punctuator(')')) {
            break;
         }
         else if (isLabeled) {
            ParseError::raise("Labeled arguments have to come last in a call", lexer);
         }
      }

      bool isVar = false;
      bool isLet = false;
      if (lexer->currentToken.is_keyword("let") && allowLet) {
         isLet = true;
         lexer->advance();
      }
      else if (lexer->currentToken.is_keyword("var") && allowLet) {
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
      if (!lexer->currentToken.is_punctuator(',') && !lexer->currentToken.is_punctuator(')')) {
         ParseError::raise("Expected ',' or ')'", lexer);
      }
   }

   return args;
}

std::vector<Attribute> Parser::parse_attributes()
{
   std::vector<Attribute> attributes;
   std::vector<string> foundAttrs;

   while (lexer->currentToken.is_punctuator('@')) {
      lexer->advance();
      if (lexer->currentToken.get_type() != T_IDENT) {
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

         while (!lexer->currentToken.is_punctuator(')') && lexer->currentToken.get_type() != T_EOF) {
            attr.args.push_back(lexer->parseExpression());
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

CompoundStmt::SharedPtr Parser::parse_block(bool preserveTopLevel)
{
   Token start = lexer->currentToken;
   bool last_top_level = top_level;
   bool unsafe = false;

   if (!preserveTopLevel) {
      top_level = false;
   }

   lexer->advance();

   if (lexer->currentToken.get_type() == T_KEYWORD && lexer->s_val() == "unsafe") {
      lexer->advance();
      unsafe = true;
   }

   if (!(lexer->currentToken.is_punctuator('{'))) {
      ParseError::raise("Expected '{' to start a block statement.", lexer);
   }

   lexer->advance();

   CompoundStmt::SharedPtr block = std::make_shared<CompoundStmt>();
   while (!lexer->currentToken.is_punctuator('}')) {
      while (lexer->currentToken.is_separator()) {
         lexer->advance();
      }

      if (lexer->currentToken.get_type() == T_EOF || lexer->currentToken.is_punctuator('}')) {
         break;
      }

      Statement::SharedPtr stmt = parse_next_stmt();

      if (CurrentFuncDecls != nullptr && (stmt->get_type() == NodeType::CLASS_DECL
         || stmt->get_type() == NodeType::ENUM_DECL || stmt->get_type() == NodeType::UNION_DECL))
      {
         CurrentFuncDecls->push_back(stmt);
      }
      else {
         block->addStatement(stmt);
      }

      lexer->commit();
      lexer->advance();
   }

   if (!lexer->currentToken.is_punctuator('}')) {
      ParseError::raise("Expected '}' to end a block statement", lexer);
   }

   setIndex(block.get(), start);
   block->isUnsafe(unsafe);

   top_level = last_top_level;

   return block;
}

Statement::SharedPtr Parser::parse_next_stmt()
{
   if (lexer->currentToken.is_punctuator('{')) {
      lexer->backtrack();
      CompoundStmt::SharedPtr cmp_stmt = parse_block();

      if (top_level) {
         implicit_main_stmts.push_back(cmp_stmt);
         return nullptr;
      }

      return cmp_stmt;
   }
   else if (lexer->currentToken.get_type() == T_KEYWORD) {
      Statement::SharedPtr expr = parse_keyword();

      return expr;
   }
   else if (lexer->currentToken.get_type() == T_IDENT && lexer->lookahead().is_operator(":")) {
      std::string label = lexer->s_val();
      lexer->advance();

      auto label_stmt = std::make_shared<LabelStmt>(label);

      if (top_level) {
         implicit_main_stmts.push_back(label_stmt);
         return nullptr;
      }

      return label_stmt;
   }
   else if (lexer->currentToken.is_punctuator('@')) {
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
   Token start = lexer->currentToken;
   bool anonymous = false;
   string nsName;

   lexer->advance();
   if (lexer->currentToken.get_type() != T_IDENT) {
      anonymous = true;
      lexer->backtrack();
   }
   else {
      nsName = lexer->s_val();
   }

   while (lexer->lookahead().is_punctuator('.')) {
      lexer->advance();
      lexer->advance();

      if (lexer->currentToken.get_type() != T_IDENT) {
         ParseError::raise("Expected identifier after 'namespace'", lexer);
      }

      nsName += "." + lexer->s_val();
   }

   auto mod = std::make_shared<NamespaceDecl>(nsName, parse_block(true), anonymous);
   setIndex(mod.get(), start);

   namespaces.push_back(nsName);
   return mod;
}

/**
 * Parses a module import statement
 * @return
 */
UsingStmt::SharedPtr Parser::parse_using_stmt()
{
   Token start = lexer->currentToken;
   lexer->advance();

   string importNs;
   std::vector<string> items;

   while (lexer->currentToken.get_type() == T_IDENT || lexer->currentToken.is_operator("*")) {
      if (lexer->lookahead().is_punctuator('.')) {
         if (!importNs.empty()) {
            importNs += ".";
         }

         importNs += lexer->s_val();

         lexer->advance();
         if (lexer->lookahead().is_punctuator('{')) {
            lexer->advance();
            lexer->advance();

            while (!lexer->currentToken.is_punctuator('}')) {
               items.push_back(lexer->s_val());
               lexer->advance();
               if (lexer->currentToken.is_punctuator(',')) {
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
   setIndex(usingStmt.get(), start);

   return usingStmt;
}

/**
 * Parses the program into an AST
 * @return
 */
CompoundStmt::SharedPtr Parser::parse()
{
   CompoundStmt::SharedPtr root = std::make_shared<CompoundStmt>();

   lexer->advance(); // BOF
   while (lexer->currentToken.is_keyword("using")) {
      root->addStatement(parse_using_stmt());
      lexer->advance();
   }

   while(lexer->currentToken.get_type() != T_EOF) {
      while (lexer->currentToken.is_separator() || lexer->currentToken.is_punctuator(';')) {
         lexer->advance();
      }

      if (lexer->currentToken.get_type() == T_EOF) {
         break;
      }

      Statement::SharedPtr stmt = parse_next_stmt();

      if (!attributes.empty()) {
         ParseError::raise("Attributes not allowed here", lexer);
      }

      if (stmt != nullptr) {
         root->addStatement(stmt);
      }

      lexer->commit();
      lexer->advance();
   }

   root->addStatement(std::make_shared<EndOfFileStmt>());

   return root;
}