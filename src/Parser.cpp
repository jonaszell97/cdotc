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
#include "Variant/Type/GenericType.h"
#include "Template/TokenStore.h"

#include "AST/Passes/Declaration/DeclPass.h"
#include "AST/Passes/ASTIncludes.h"
#include "Variant/Type/Generic.h"
#include "Files/FileManager.h"
#include "Message/Diagnostics.h"

using namespace cdot::diag;
using namespace cdot::cl;

static const unsigned MAX_TEMPLATE_ARGS = 32;
static const unsigned MAX_FUNCTION_ARGS = 64;

Parser::Parser(std::unique_ptr<llvm::MemoryBuffer> &&buf,
               string &fileName,
               size_t sourceId)
   : lexer(new Lexer(buf.get(), fileName, sourceId)), source_id(sourceId),
     decl(new DeclPass), buf(std::move(buf))
{
   lexer->advance();
}

Parser::Parser(std::vector<Token> &&tokens)
   : lexer(new Lexer(std::move(tokens))), decl(new DeclPass)
{
   lexer->advance();
}

Parser::~Parser()
{
   delete lexer;
   delete decl;
}

void Parser::pushNamespace(const string &nsName)
{
   decl->pushNamespace(nsName);
}

void Parser::popNamespace()
{
   decl->popNamespace();
}

string Parser::currentNS()
{
   return decl->ns_prefix();
}

void Parser::setIndex(
   AstNode *node,
   const Token &start)
{
   node->setSourceLoc(
      SourceLocation(start.getCol(),
                     start.getLine(),
                     lexer->currentToken.getStart() - start.getStart() - 1,
                     lexer->currentToken.getSourceLoc().getSourceId()));
}

void Parser::isTemplateInstantiation(
   const unordered_map<string, TemplateArg> &templateArgs)
{
   for (const auto &arg : templateArgs) {
      if (arg.second.isVariadic()) {
         std::vector<Variant> vec;
         for (const auto &val : arg.second.getVariadicArgs()) {
            vec.emplace_back(val.toString());
         }

         lexer->addVariadicTemplateArg(arg.first, vec);
      }
      else switch (arg.second.getKind()) {
         case TemplateConstraint::TypeName:
            lexer->addTemplateArg(
               arg.first,
               arg.second.getGenericTy()->getActualType()->toString()
            );
            break;
         case TemplateConstraint::Value:
            lexer->addTemplateArg(
               arg.first,
               arg.second.getValue()
            );
            break;
         case TemplateConstraint::Arbitrary:
            lexer->addTemplateArg(
               arg.first,
               arg.second.getTokens()
            );
            break;
      }
   }
}

std::shared_ptr<TypeRef> Parser::parse_type(bool ignoreError, bool metaTy)
{
   bool isReference = false;
   if (lexer->currentToken.is_keyword("ref")) {
      lexer->advance();
      isReference = true;
   }

   auto attrs = parse_attributes();

   Token start = lexer->currentToken;
   auto typeref = parse_type_impl(ignoreError);

   if (typeref) {
      setIndex(typeref.get(), start);
      typeref->isReference(isReference);
      typeref->setAttributes(attrs);
      typeref->isMetaTy(metaTy);
   }

   return typeref;
}

/**
 * Parses a type specifier, like "int", "bool[3]", "any[18 * x]"
 * @return
 */
std::shared_ptr<TypeRef> Parser::parse_type_impl(bool ignoreError)
{
   std::vector<pair<string, TemplateArgList*>> ns;

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

         std::vector<TemplateArg> templateArgs = {
            TemplateArg(std::move(elType)), TemplateArg(std::move(valType))
         };

         ns.emplace_back("Dictionary", new ResolvedTemplateArgList(
            std::move(templateArgs)
         ));

         return std::make_shared<TypeRef>(std::move(ns));
      }

      if (!lexer->currentToken.is_punctuator(']')) {
         if (ignoreError) {
            return nullptr;
         }

         ParseError::raise("Expected ']' after array type", lexer);
      }

      std::vector<TemplateArg> templateArgs = {
         TemplateArg(std::move(elType))
      };

      ns.emplace_back("Array", new ResolvedTemplateArgList(
         std::move(templateArgs)
      ));

      return std::make_shared<TypeRef>(std::move(ns));
   }

   // function or tuple type
   if (lexer->currentToken.is_punctuator('(')) {
      auto argTypes = parse_tuple_type();
      lexer->advance();

      // tuple
      if (!lexer->currentToken.is_operator("->")) {
         lexer->backtrack();
         return std::make_shared<TypeRef>(std::move(argTypes));
      }

      lexer->advance();

      auto returnType = parse_type();
      return std::make_shared<TypeRef>(std::move(returnType),
                                       std::move(argTypes));
   }


   std::vector<pair<string, TypeRef::SharedPtr>> containedTypes;

   Token next;
   bool initial = true;

   while (initial || next.is_punctuator('.')) {
      if (!initial) {
         lexer->advance();
         lexer->advance();
      }
      else {
         initial = false;
      }

      if (lexer->currentToken.get_type() != T_IDENT) {
         if (ignoreError) {
            return nullptr;
         }
         ParseError::raise("Unexpected character in type reference", lexer);
      }

      string subName = lexer->strVal();
      TemplateArgList *subTemplateArgs = nullptr;

      next = lexer->lookahead(false);
      if (next.is_operator("<")) {
         lexer->advance();

         subTemplateArgs = parse_unresolved_template_args();
         next = lexer->lookahead(false);
      }

      ns.emplace_back(std::move(subName), std::move(subTemplateArgs));
   }

   auto typeref = std::make_shared<TypeRef>(std::move(ns));

   // pointer type
   while (next.get_type() == T_OP) {
      auto op = next.get_value().strVal;
      if (util::matches("\\*+", op)) {
         while (op.length() > 0) {
            typeref->incrementPointerDepth();
            op = op.substr(0, op.length() - 1);
         }

         lexer->advance();
         next = lexer->lookahead(false);
      }
      else {
         break;
      }
   }

   // optional type
   if (next.is_operator("?")) {
      lexer->advance();
      typeref->isOption(true);
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
   auto alias = lexer->strVal();
   auto generics = parse_template_constraints();

   auto td = std::make_shared<TypedefDecl>(am,
                                           std::move(alias),
                                           std::move(originTy),
                                           std::move(generics));

   td->setAttributes(attrs);
   setIndex(td.get(), start);

   return td;
}

namespace {

bool identifierTemplateDecider(char c)
{
   return c == '\n' || c == ')' || c == ',' || c == ']' || c == ';'
          || Lexer::is_operator_char(c);
}

}

Expression::SharedPtr Parser::parse_identifier()
{
   Token start = lexer->currentToken;
   string ident = lexer->strVal();

   if (ident=="self") {
       int i=3;
   }

   auto ident_expr = std::make_shared<IdentifierRefExpr>(std::move(ident));

   if (_is_generic_any(identifierTemplateDecider)) {
      lexer->advance();
      ident_expr->setTemplateArgs(parse_unresolved_template_args());
   }

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
      Lexer::ModeScopeGuard guard((Lexer::MemberAccess), lexer);
      lexer->advance(false);

      // tuple access
      if (lexer->currentToken.get_type() == T_LITERAL) {
         size_t index = lexer->currentToken.get_value().intVal;

         auto mem_ref = std::make_shared<MemberRefExpr>(index, pointerAccess);
         mem_ref->setMemberExpr(try_parse_member_expr());
         setIndex(mem_ref.get(), start);

         return mem_ref;
      }

      // method call
      Token next = lexer->lookahead();
      if (next.is_punctuator('(') || is_generic_call()) {
         auto call = parse_function_call(CallType::METHOD_CALL);
         call->isPointerAccess(pointerAccess);

         return call;
      }

      string ident = lexer->strVal();

      auto mem_ref = std::make_shared<MemberRefExpr>(std::move(ident),
                                                     pointerAccess);

      if (_is_generic_any(identifierTemplateDecider)) {
         lexer->advance();
         mem_ref->setTemplateArgs(parse_unresolved_template_args());
      }

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
      auto dict = std::make_shared<CollectionLiteral>(std::move(keys),
                                                      std::move(values));
      setIndex(dict.get(), start);

      return dict;
   }

   auto arr = std::make_shared<CollectionLiteral>(std::move(values));
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

      auto caseName = lexer->strVal();

      if (!lexer->lookahead().is_punctuator('(')) {
         auto expr = std::make_shared<MemberRefExpr>(std::move(caseName));
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
         auto argName = lexer->strVal();
         auto arg = std::make_shared<FuncArgDecl>(argName,
                                                  std::make_shared<TypeRef>());
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
         auto str = lexer->strVal();

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
               expr = std::make_shared<IdentifierRefExpr>(
                  std::move(lexer->strRef()));
            }

            lexer->continueInterpolation = true;
            lexer->advance();

            strings.push_back(expr);
            strings.push_back(
               std::make_shared<StringLiteral>(lexer->strVal()));

            lexer->continueInterpolation = false;
            if (!lexer->currentToken.isInterpolationStart) {
               break;
            }

            lexer->advance();
         }

         auto interp = std::make_shared<StringInterpolation>(
            std::move(strings));
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

      Expression::SharedPtr expr;
      if (val.type == VariantType::INT) {
         switch (val.bitwidth) {
            case 1:
               expr = std::make_shared<BoolLiteral>(val.intVal != 0);
               break;
            case CHAR_BIT:
               expr = std::make_shared<CharLiteral>(val.charVal);
               break;
            default:
               expr = std::make_shared<IntegerLiteral>(std::move(val));
               break;
         }
      }
      else if (val.type == VariantType::FLOAT) {
         expr = std::make_shared<FPLiteral>(std::move(val));
      }

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

bool followsPostfixUnaryOp(const Token &tok)
{
   return tok.is_separator() || tok.is_operator()
          || (tok.is_punctuator() && !tok.is_punctuator('.')
              && !tok.is_punctuator('('));
}

} // anonymous namespace

/**
 * Recursively parses a unary expression for example an identifier or a literal
 * with unary expressions applied to it, e.g.
 *     - ++3
 *     - x.foo()++
 *     - !bar[3]
 * @return
 */
Expression::SharedPtr Parser::parse_unary_expr(
   UnaryOperator::SharedPtr literal, bool postfix)
{
   Token start = lexer->currentToken;

   // prefix unary op
   if (lexer->currentToken.get_type() == T_OP) {
      auto unary_op = std::make_shared<UnaryOperator>(lexer->strVal(),
                                                      "prefix");
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

   Token next = lexer->lookahead();

   // postfix unary op
   if (next.get_type() == T_OP) {
      lexer->advance();
      next = lexer->lookahead();

      if (followsPostfixUnaryOp(next)) {
         auto unary_op = std::make_shared<UnaryOperator>(lexer->strVal(),
                                                         "postfix");
         unary_op->setMemberExpr(try_parse_member_expr());
         unary_op->setTarget(expr);
         setIndex(unary_op.get(), start);

         return unary_op;
      }
      else {
         next = lexer->currentToken;
         lexer->backtrack();
      }
   }

   // call
   if (next.is_punctuator('(') || is_generic_call()) {
      lexer->advance();

      auto generics = parse_unresolved_template_args();

      auto call = std::make_shared<CallExpr>(CallType::FUNC_CALL,
                                             parse_arguments());
      call->setTemplateArgs(generics);
      expr->setMemberExpr(call);
   }

   return expr;
}

TertiaryOperator::SharedPtr Parser::parse_tertiary_operator(
   Expression::SharedPtr cond)
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

   auto isLambda = false;
   bool isTuple = false;
   bool maybeFuncTy = false;

   Lexer::StateSaveGuard guard(lexer);
   assert(lexer->currentToken.is_punctuator('('));

   while (open_parens > closed_parens) {
      lexer->advance();
      if (!lexer->currentToken.is_punctuator()) {
         continue;
      }

      char c = lexer->currentToken._value.charVal;
      switch (c) {
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
   }

   if (open_parens != closed_parens) {
      ParseError::raise("Expression contains unmatched parentheses", lexer);
   }

   lexer->advance();

   if (lexer->currentToken.is_operator("->")) {
      maybeFuncTy = true;
      lexer->advance();
   }
   if (lexer->currentToken.is_operator("=>")) {
      isLambda = true;
   }

   // lambda
   if (isLambda) {
      return ParenExprType::LAMBDA;
   }
   else if (maybeFuncTy) {
      return ParenExprType::FunctionType;
   }
   else if (isTuple) {
      return ParenExprType::TUPLE;
   }

   return ParenExprType::EXPR;
}

namespace {

bool openParenDecider(char c)
{
   return c == '(';
}

bool periodDecider(char c)
{
   return c == '.';
}

}

bool Parser::is_generic_call()
{
   return _is_generic_any(openParenDecider);
}

bool Parser::is_generic_member_access()
{
   return _is_generic_any(periodDecider);
}

bool Parser::_is_generic_any(bool (*decider)(char))
{
   Lexer::StateSaveGuard guard(lexer);
   lexer->advance();

   if (!lexer->currentToken.is_punctuator('<')) {
      return false;
   }

   auto openBrackets = 1;
   auto closeBrackets = 0;

   bool maybeArrow = false;
   while (openBrackets > closeBrackets) {
      lexer->advance();

      char c;
      if (lexer->currentToken.is_operator()) {
         auto &op = lexer->currentToken._value.strVal;
         if (op == "<") {
            c = '<';
         }
         else if (op == ">") {
            c = '>';
         }
         else {
            continue;
         }
      }
      else if (lexer->currentToken.is_punctuator()) {
         c = lexer->currentToken._value.charVal;
      }
      else {
         continue;
      }

      switch (c) {
         case '-':
         case '=':
            maybeArrow = true;
            break;
         case '\n': return false;
         case '<':
            if (!maybeArrow) {
               ++openBrackets;
            }
            maybeArrow = false;
            break;
         case '>':
            if (!maybeArrow) {
               ++closeBrackets;
            }
            maybeArrow = false;
            break;
         default:
            maybeArrow = false;
      }
   }

   lexer->advance();
   if (!lexer->currentToken.is_punctuator()) {
      return false;
   }

   return decider(lexer->currentToken._value.charVal);
}

Expression::SharedPtr Parser::parse_paren_expr() {
   ParenExprType type = get_paren_expr_type();
   Expression::SharedPtr expr;
   switch (type) {
      case ParenExprType::LAMBDA:
         expr = parse_lambda_expr();
         break;
      case ParenExprType::FunctionType:
         return parse_type(false, true);
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

      if (lexer->currentToken.get_type() == T_IDENT
          && lexer->lookahead().is_operator(":")) {
         label = lexer->strVal();
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
      ParseError::raise("Expected closing parenthesis after tuple literal",
                        lexer);
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
 * Recursively parses an arbitrary (semantically correct) combination of
 * literals, operators and perenthesized expressions
 * @param lhs Left hand side of the current expression
 * @param min_precedence Minimum operator precedence for current expression
 * @return
 */
Expression::SharedPtr Parser::parse_expression(
   Expression::SharedPtr lhs, int min_precedence)
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
      if (next.is_identifier("self")) {
          int i=3;
      }

      // continue recursively while a higher precedence operator follows
      while (hasHigherPrecedence(next, util::op_precedence[op])) {
         rhs = parse_expression(rhs,
                                util::op_precedence[next.get_value().strVal]);
         next = lexer->lookahead(false);
      }

      // return the expression so far
      auto binary_op = std::make_shared<BinaryOperator>(op);
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

   setIndex(lhs.get(), start);
   return lhs;
}

Statement::SharedPtr Parser::parse_declaration(
   bool is_const, bool is_declaration)
{
   std::vector<DeclStmt::SharedPtr> declarations;
   auto access = AccessModifier::DEFAULT;

   while (lexer->currentToken.get_type() == T_KEYWORD) {
      auto keyword = lexer->strVal();
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
      std::string identifier = lexer->strVal();

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

      auto decl_stmt = std::make_shared<DeclStmt>(identifier, typeref,
                                                  is_const, value);

      decl_stmt->setAccess(access);
      decl_stmt->setAttributes(attributes);
      decl_stmt->isDeclaration(is_declaration);
      decl_stmt->isGlobal(top_level);

      setIndex(decl_stmt.get(), start);

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
      if (lexer->currentToken.get_type() == T_IDENT
          && lexer->lookahead().is_operator(":")) {
         label = lexer->strVal();
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
      if (!lexer->currentToken.is_punctuator(',')
          && !lexer->currentToken.is_punctuator(')')) {
         ParseError::raise("Expected closing parenthesis after argument list",
                           lexer);
      }
   }

   return tupleTypes;
}

/**
 * Parses a method's argument list
 * @return
 */
std::vector<FuncArgDecl::SharedPtr> Parser::parse_arg_list(
   bool optionalNames, bool optionalTypes, bool isTemplateArgList)
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

   while (!lexer->currentToken.is_punctuator(')')) {
      if (args.size() > MAX_FUNCTION_ARGS) {
         ParseError::raise("functions cannot have more than "
            + std::to_string(MAX_FUNCTION_ARGS) + " arguments", lexer);
      }
      if (var_arg) {
         ParseError::raise("Vararg arguments can only be the last argument"
                              "of a function", lexer);
      }

      std::vector<Attribute> attributes = parse_attributes();

      start = lexer->currentToken;
      FuncArgDecl::SharedPtr arg_dec = std::make_shared<FuncArgDecl>();
      arg_dec->setAttributes(attributes);

      if (!optionalNames || lexer->lookahead().is_operator(":")) {
         if (lexer->currentToken.get_type() != T_IDENT) {
            ParseError::raise("Expected identifier", lexer);
         }

         arg_dec->setName(lexer->strVal());
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
         argTy->setReturnDummyObjTy(isTemplateArgList);

         arg_dec->setType(argTy);
         lexer->advance();
      }
      else if (!optionalTypes) {
         ParseError::raise("Function arguments have to have a specified type",
                           lexer);
      }

      // optional default value
      if (lexer->currentToken.is_operator("=")) {
         if (var_arg) {
            ParseError::raise("Vararg arguments cannot have a default value",
                              lexer);
         }

         lexer->advance();

         Expression::SharedPtr _expr = parse_expression();
         arg_dec->setDefault(_expr);
         def_arg = true;

         lexer->advance();

      } else if (def_arg) {
         ParseError::raise("Default values are only allowed as last items of "
                              "an argument list", lexer);
      }

      setIndex(arg_dec.get(), start);
      args.push_back(arg_dec);

      // end of argument list or next argument
      if (lexer->currentToken.is_punctuator(',')) {
         lexer->advance();
      }
      else if (!lexer->currentToken.is_punctuator(')')) {
         ParseError::raise("Expected closing parenthesis after "
                              "argument list", lexer);
      }

      start = lexer->currentToken;
   }

   if (!lexer->currentToken.is_punctuator(')')) {
      ParseError::raise("Expected closing parenthesis after argument "
                           "list", lexer);
   }

   return args;
}

bool Parser::isValidOperatorChar(Token& next) {
   return next.get_type() != T_PUNCTUATOR
          && next.get_type() != T_EOF && !next.is_operator("->");
}

Statement::SharedPtr Parser::parse_function_decl(
   bool is_declaration, string *templateFuncName)
{
   Token start = lexer->currentToken;
   size_t beginIndex = lexer->tokens.size() - 1;

   bool isOperatorDecl = false;
   string opType;
   if (lexer->lookahead().get_type() == T_KEYWORD) {
      isOperatorDecl = true;
      lexer->advance();

      opType = lexer->strVal();
   }

   // function name
   lexer->advance();

   string funcName;
   string op;

   if (templateFuncName) {
      funcName = *templateFuncName;
      if (isOperatorDecl) {
         while (isValidOperatorChar(lexer->currentToken)) {
            op += lexer->strVal();
            lexer->advance(false, true);
         }

         lexer->backtrack();
      }
   }
   else if (isOperatorDecl) {
      while (isValidOperatorChar(lexer->currentToken)) {
         op += lexer->strVal();
         lexer->advance(false, true);
      }

      if (lexer->currentToken.is_punctuator(' ')) {
         lexer->advance();
      }

      if (!util::matches("(..+|[^.])*", op)) {
         ParseError::raise("Custom operators can only contain periods "
                              "in sequences of two or more", lexer);
      }

      lexer->backtrack();
      funcName = opType + " " + op;
   }
   else {
      funcName = lexer->strVal();
   }

   std::vector<Statement::SharedPtr> innerDecls;
   CurrentFuncDecls = &innerDecls;

   if (funcName == "main") {
      for (const auto& stmt : implicit_main_stmts) {
         if (stmt->get_type() != NodeType::DECLARATION) {
            ParseError::raise("Cannot define a main method when top-level "
                                 "non-declaration statements exist", lexer);
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

   bool isTemplate = false;

   std::vector<TemplateConstraint> generics;

   if (!templateFuncName) {
      generics = parse_template_constraints();
      lexer->advance();
      if (!generics.empty()) {
         isTemplate = true;
      }
   }
   else {
      lexer->advance();
      lexer->skip_until_even(Lexer::ANGLED);
      if (lexer->currentToken.is_punctuator('>')) {
         lexer->advance();
      }
   }

   // template
   if (isTemplate) {
      return parse_function_template(
         beginIndex,
         funcName,
         generics
      );
   }

   std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list(is_declaration);
   FunctionDecl::SharedPtr fun_dec = std::make_shared<FunctionDecl>(
      AccessModifier::PUBLIC, std::move(funcName)
   );

   fun_dec->setAttributes(attributes);
   attributes.clear();

   for (auto arg : args) {
      // TODO meta type arg
      fun_dec->addArgument(arg);
   }

   // optional return type
   if (lexer->lookahead().is_operator("->")) {
      lexer->advance();
      lexer->advance();
      fun_dec->setReturnType(parse_type());
   }
   else if (is_declaration) {
      ParseError::raise("Declared functions have to have a defined return type",
                        lexer);
   }
   else {
      fun_dec->setReturnType(std::make_shared<TypeRef>());
   }

   if (lexer->lookahead().is_punctuator('{')
            || lexer->lookahead().is_keyword("unsafe")) {
      if (is_declaration) {
         ParseError::raise("Declared functions cannot have a body", lexer);
      }

      // function body
      CompoundStmt::SharedPtr func_body = parse_block();
      fun_dec->setBody(func_body);
   }

   CurrentFuncDecls = nullptr;

   fun_dec->isDeclaration(is_declaration);
   fun_dec->setInnerDecls(std::move(innerDecls));

   setIndex(fun_dec.get(), start);

   return fun_dec;
}

std::shared_ptr<CallableTemplateDecl> Parser::parse_function_template(
   size_t beginIndex,
   string &name,
   std::vector<TemplateConstraint> &constraints)
{
   Lexer::IgnoreScope guard(lexer);
   std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list();

   auto returnType = std::make_shared<TypeRef>();
   lexer->advance();

   if (lexer->currentToken.is_operator("->")) {
      lexer->advance();
      returnType = parse_type();
      lexer->advance();
   }

   lexer->skip_until_even(Lexer::BRACE);

   std::vector<Token> tokens;
   tokens.reserve(lexer->tokens.size() - beginIndex - 1);

   auto current = lexer->tokens.size() - 1;
   for (;;) {
      tokens.push_back(std::move(lexer->tokens[current]));
      if (current == beginIndex) {
         break;
      }

      --current;
   }

   auto Store = std::make_unique<SimpleTokenStore>(move(tokens));
   auto Template = std::make_shared<CallableTemplateDecl>(
      std::move(name),
      std::move(Store),
      std::move(constraints),
      std::move(args),
      std::move(returnType)
   );

   return Template;
}

std::shared_ptr<MethodTemplateDecl> Parser::parse_method_template(
   size_t beginIndex,
   string &name,
   bool isStatic,
   bool isMutating,
   bool isOperator,
   bool isProtocol,
   std::vector<TemplateConstraint> &constraints)
{
   Lexer::IgnoreScope guard(lexer);
   std::vector<FuncArgDecl::SharedPtr> args = parse_arg_list();

   auto returnType = std::make_shared<TypeRef>();
   lexer->advance();

   if (lexer->currentToken.is_operator("->")) {
      lexer->advance();
      returnType = parse_type(true);
      lexer->advance();
   }

   lexer->skip_until_even(Lexer::BRACE);

   std::vector<Token> tokens;
   tokens.reserve(lexer->tokens.size() - beginIndex - 1);

   auto current = lexer->tokens.size() - 1;
   for (;;) {
      tokens.push_back(std::move(lexer->tokens[current]));
      if (current == beginIndex) {
         break;
      }

      --current;
   }

   if (isProtocol) {
      constraints.emplace_back(TemplateConstraint::TypeName, "Self", nullptr,
                               nullptr, nullptr);
   }

   auto Store = std::make_unique<SimpleTokenStore>(move(tokens));
   auto Template = std::make_shared<MethodTemplateDecl>(
      std::move(name),
      std::move(Store),
      std::move(constraints),
      std::move(args),
      std::move(returnType),
      isStatic
   );

   Template->isMutating(isMutating);
   Template->setOuterRecord(outerTemplate);
   Template->isOperator(isOperator);

   return Template;
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

std::vector<TemplateConstraint> Parser::parse_template_constraints()
{
   std::vector<TemplateConstraint> templateArgs;
   auto guard = lexer->makeModeGuard(Lexer::TemplateConstraints);

   if (!lexer->lookahead().is_operator("<")
       && !lexer->currentToken.is_operator("<")) {
      return templateArgs;
   }

   if (!lexer->currentToken.is_operator("<")) {
      lexer->advance();
   }

   bool defaultGiven = false;
   bool variadic = false;

   lexer->advance();
   while (!lexer->currentToken.is_operator(">")) {
      if (templateArgs.size() > MAX_TEMPLATE_ARGS) {
         ParseError::raise("templates cannot have more than "
            + std::to_string(MAX_TEMPLATE_ARGS) + " arguments", lexer);
      }

      TemplateConstraint::Kind kind = TemplateConstraint::TypeName;

      if (lexer->currentToken.get_type() == T_KEYWORD) {
         auto &keyword = lexer->currentToken._value.strVal;
         if (keyword == "typename") {
            // default
         }
         else if (keyword == "value") {
            kind = TemplateConstraint::Value;
         }
         else if (keyword == "any") {
            kind = TemplateConstraint::Arbitrary;
         }
         else {
            ParseError::raise("expected template arg kind", lexer);
         }

         lexer->advance();
      }

      if (lexer->currentToken.is_operator("...")) {
         if (variadic) {
            ParseError::raise("variadic arguments must be last in a template "
                                 "parameter list", lexer);
         }

         variadic = true;
         lexer->advance();
      }

      if (lexer->currentToken.get_type() != T_IDENT) {
         ParseError::raise("expected template argument name", lexer);
      }

      auto genericTypeName = std::move(lexer->currentToken._value.strVal);
      std::shared_ptr<TypeRef> unresolvedCovariance = nullptr;
      std::shared_ptr<TypeRef> unresolvedContravariance = nullptr;

      if (lexer->lookahead().is_operator(":")) {
         lexer->advance();
         lexer->advance();

         if (kind == TemplateConstraint::TypeName) {
            bool covarSet = false;
            bool contravarSet = false;

            for (;;) {
               auto isCovar = true;
               if (lexer->currentToken.is_operator("+")) {
                  isCovar = false;
                  lexer->advance();
               }
               else if (lexer->currentToken.is_operator("-")) {
                  // default
                  lexer->advance();
               }

               if (isCovar) {
                  if (covarSet) {
                     diag::err(err_generic_error) << "covariance already "
                        "specified" << lexer << diag::term;
                  }

                  covarSet = true;
                  unresolvedCovariance = parse_type();
               }
               else {
                  if (contravarSet) {
                     diag::err(err_generic_error) << "contravariance already "
                        "specified" << lexer << diag::term;
                  }

                  contravarSet = true;
                  unresolvedContravariance = parse_type();
               }

               if (lexer->lookahead().is_punctuator(',')
                   || lexer->lookahead().is_operator(">")) {
                  break;
               }

               lexer->advance();
            }
         }
         else if (kind == TemplateConstraint::Value) {
            unresolvedCovariance = parse_type();
         }
         else {
            ParseError::raise("cannot specify template argument", lexer);
         }
      }

      lexer->advance();

      std::shared_ptr<TemplateArg> defaultValue = nullptr;
      if (lexer->currentToken.is_operator("=")) {
         defaultGiven = true;
         lexer->advance();

         while (!lexer->currentToken.is_operator(">")
                && !lexer->currentToken.is_punctuator(',')) {
            switch (kind) {
               case TemplateConstraint::TypeName: {
                  defaultValue = std::make_shared<TemplateArg>(parse_type());
                  break;
               }
               case TemplateConstraint::Value: {
                  defaultValue = std::make_shared<TemplateArg>(
                     lexer->parseExpression({}, 0, true));
                  break;
               }
               case TemplateConstraint::Arbitrary: {
                  std::vector<Token> tokens;
                  while (!lexer->currentToken.is_punctuator(',')
                         && lexer->currentToken.get_type() != T_EOF) {
                     tokens.push_back(lexer->currentToken);
                     lexer->advance(false, true);
                  }

                  defaultValue = std::make_shared<TemplateArg>(
                     std::move(tokens));
                  break;
               }
            }

            lexer->advance();
         }
      }
      else if (defaultGiven) {
         lexer->backtrack();
         diag::err(err_generic_error) << "default template paremeter values "
            "can only appear at the end of a parameter list"
                                      << lexer << diag::term;
      }

      templateArgs.emplace_back(
         kind,
         std::move(genericTypeName),
         std::move(unresolvedCovariance),
         std::move(unresolvedContravariance),
         std::move(defaultValue),
         variadic
      );

      if (lexer->currentToken.is_punctuator(',')) {
         lexer->advance();
      }
   }

   return std::move(templateArgs);
}

TemplateArgList *Parser::parse_unresolved_template_args()
{
   if (!lexer->currentToken.is_operator("<")) {
      return new ResolvedTemplateArgList({});
   }

   Lexer::ModeScopeGuard guard((Lexer::TemplateArgMode), lexer);
   std::vector<Token> tokens;

   lexer->advance();

   unsigned openCount = 1;
   unsigned closeCount = 0;

   for (;;) {
      if (lexer->currentToken.is_punctuator('<')) {
         ++openCount;
      }
      else if (lexer->currentToken.is_punctuator('>')) {
         ++closeCount;
      }

      if (openCount == closeCount) {
         break;
      }

      tokens.push_back(std::move(lexer->currentToken));
      lexer->advance();
   }

   std::reverse(tokens.begin(), tokens.end());

   return new UnresolvedTemplateArgList(
      std::move(tokens),
      source_id, lexer->currentToken.getLine(), lexer->currentToken.getCol()
   );
}

ResolvedTemplateArgList* Parser::resolve_template_args(
   TemplateArgList *argList,
   std::vector<TemplateConstraint> &constraints)
{
   assert(!argList->isResolved());

   auto unresolved = static_cast<UnresolvedTemplateArgList*>(argList);
   auto &&tokens = unresolved->getTokens();

   Parser p(std::move(tokens));

   auto &lexer = *p.getLexer();
   lexer.setLine(unresolved->getLoc().line);
   lexer.setCol(unresolved->getLoc().col);

   unsigned i = 0;
   std::vector<TemplateArg> args;
   bool variadic = false;

   while (lexer.currentToken.get_type() != T_EOF) {
      TemplateConstraint::Kind kind;
      if (i == constraints.size() - 1 && constraints[i].isVariadic) {
         args.emplace_back(std::vector<TemplateArg>());
         variadic = true;
      }
      else if (i >= constraints.size()) {
         kind = constraints.back().kind;
      }
      else {
         kind = constraints[i].kind;
      }

      auto &target = variadic ? args.back().getVariadicArgs()
                              : args;

      if (kind == TemplateConstraint::TypeName) {
         target.emplace_back(p.parse_type());
      }
      else if (constraints[i].kind == TemplateConstraint::Value) {
         target.emplace_back(lexer.parseExpression({}, 0, true));
      }
      else {
         std::vector<Token> tokens;
         while (!lexer.currentToken.is_punctuator(',')
                && lexer.currentToken.get_type() != T_EOF) {
            tokens.push_back(lexer.currentToken);
            lexer.advance(false, true);
         }

         target.emplace_back(std::move(tokens));
      }

      lexer.advance();
      if (lexer.currentToken.is_punctuator(',')) {
         lexer.advance();
      }

      ++i;
   }

   return new ResolvedTemplateArgList(std::move(args));
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

      auto else_branch = std::static_pointer_cast<CompoundStmt>(
         parse_next_stmt());
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
   if (lexer->currentToken.is_keyword("case")
       || lexer->currentToken.is_punctuator('}')
       || lexer->currentToken.is_keyword("default")) {
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

      while (!lexer->currentToken.is_keyword("case")
             && !lexer->currentToken.is_keyword("default") &&
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

   Expression::SharedPtr cond = std::make_shared<BoolLiteral>(true);
   if (conditionBefore) {
      cond = parse_expression();
      lexer->advance();
   }

   auto body = std::static_pointer_cast<CompoundStmt>(parse_next_stmt());
   if (!conditionBefore && lexer->lookahead().is_keyword("while")) {
      lexer->advance();
      lexer->advance();

      cond = parse_expression();
   }

   auto whileStmt = std::make_shared<WhileStmt>(cond, body, !conditionBefore);
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
         ParseError::raise("Expected declaration before 'in' in range based"
                              "for loop", lexer);
      }

      lexer->advance();

      Expression::SharedPtr range = parse_expression();
      lexer->advance();

      if (lexer->currentToken.is_punctuator(')')) {
         lexer->advance();
      }

      auto body = std::static_pointer_cast<CompoundStmt>(parse_next_stmt());

      auto forInStmt = std::make_shared<ForInStmt>(std::move(decl),
                                                   std::move(range),
                                                   std::move(body));
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
      term = std::make_shared<BoolLiteral>(true);
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

   auto block = std::static_pointer_cast<CompoundStmt>(parse_next_stmt());

   ForStmt::SharedPtr for_stmt = std::make_shared<ForStmt>(init, term, inc);
   for_stmt->setBody(block);
   setIndex(for_stmt.get(), start);

   return for_stmt;
}

DeclareStmt::SharedPtr Parser::parse_declare_stmt(
   ExternKind externKind, std::shared_ptr<DeclareStmt> decl)
{
   Token start = lexer->currentToken;
   if (lexer->lookahead().get_type() == T_IDENT) {
      lexer->advance();
      auto str = lexer->strVal();

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
   string keyword = lexer->strVal();

   if (keyword == "public") {
      access = AccessModifier::PUBLIC;
      lexer->advance();
      keyword = lexer->strVal();
   }
   else if (keyword == "private") {
      access = AccessModifier::PRIVATE;
      lexer->advance();
      keyword = lexer->strVal();
   }
   else if (keyword == "protected") {
      access = AccessModifier::PROTECTED;
      lexer->advance();
      keyword = lexer->strVal();
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
      auto cl = parse_class_decl(isStruct, isProtocol, true);
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
      auto assign = std::static_pointer_cast<DeclStmt>(
         parse_declaration(keyword == "let", true));
      decl->addDeclaration(std::move(assign));
   }
   else {
      ParseError::raise("Unexpected keyword '" + keyword + "' after declare",
                        lexer);
   }

   if (wasNull) {
      setIndex(decl.get(), start);
   }

   decl->setAttributes(attrs);
   return decl;
}

DeclareStmt::SharedPtr Parser::parse_multiple_declare_stmt(
   ExternKind externKind)
{
   Token start = lexer->currentToken;
   auto decl = std::make_shared<DeclareStmt>(externKind);

   while (!lexer->lookahead().is_punctuator('}')
          && lexer->currentToken.get_type() != T_EOF) {
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
      auto keyword = lexer->strVal();

      if (keyword == "catch") {
         lexer->advance();
         CatchBlock catchBlock;

         if (lexer->currentToken.get_type() != T_IDENT) {
            ParseError::raise("expected identifier", lexer);
         }

         catchBlock.identifier = lexer->strVal();
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

   string &keyword = lexer->strRef();

   if (keyword == "var" || keyword == "let") {
      auto assignment = parse_declaration(keyword == "let");

      return assignment;
   }
   else if (keyword == "def") {
      auto fun_dec = parse_function_decl();
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
         return parse_declaration(next.is_keyword("let"));
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
      auto goto_stmt = std::make_shared<GotoStmt>(lexer->strVal());
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
      ParseError::raise("Keyword '" + keyword + "' is only allowed at the"
         "beginning of a file", lexer);
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

CallExpr::SharedPtr Parser::parse_function_call(
   CallType callTy, bool allowLet)
{
   Token start = lexer->currentToken;

   string funcName = lexer->strVal();
   lexer->advance();

   auto generics = parse_unresolved_template_args();
   if (!generics->isResolved()) {
      lexer->advance();
   }

   CallExpr::SharedPtr call;
   if (funcName == "sizeof" || funcName == "alignof"
       || funcName == "__nullptr") {
      lexer->advance();

      auto type = parse_type();
      lexer->advance();

      std::vector<pair<string, Expression::SharedPtr>> args;
      args.emplace_back("", type);

      call = std::make_shared<CallExpr>(callTy, std::move(args),
                                        std::move(funcName));
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

      call = std::make_shared<CallExpr>(callTy, std::move(args),
                                        std::move(funcName));
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

      call = std::make_shared<CallExpr>(callTy, std::move(args),
                                        std::move(funcName));
   }
   else {
      call = std::make_shared<CallExpr>(callTy, parse_arguments(allowLet),
                                        std::move(funcName));
   }

   call->setMemberExpr(try_parse_member_expr());
   setIndex(call.get(), start);
   call->setAttributes(attributes);
   call->setTemplateArgs(generics);

   return call;
}

std::vector<pair<string, std::shared_ptr<Expression>>>
Parser::parse_arguments(bool allowLet)
{
   std::vector<pair<string, std::shared_ptr<Expression>>> args;
   bool isLabeled = false;

   while (!lexer->currentToken.is_punctuator(')')) {
      if (args.size() > MAX_FUNCTION_ARGS) {
         ParseError::raise("functions cannot have more than "
            + std::to_string(MAX_FUNCTION_ARGS) + " arguments", lexer);
      }

      string label;

      if (lexer->currentToken.is_punctuator('(')
          || lexer->currentToken.is_punctuator(',')) {
         lexer->advance();
         if (lexer->currentToken.get_type() == T_IDENT
             && lexer->lookahead().is_operator(":")) {
            label = lexer->strVal();
            lexer->advance();
            lexer->advance();

            isLabeled = true;
         }
         else if (lexer->currentToken.is_punctuator(')')) {
            break;
         }
         else if (isLabeled) {
            ParseError::raise("Labeled arguments have to come last in a call",
                              lexer);
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
      if (!lexer->currentToken.is_punctuator(',')
          && !lexer->currentToken.is_punctuator(')')) {
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
      attr.name = lexer->strVal();
      if (std::find(foundAttrs.begin(), foundAttrs.end(), attr.name)
          != foundAttrs.end()) {
         ParseError::raise("Duplicate definition of attribute " + attr.name,
                           lexer);
      }

      attr.kind = AttributeMap[attr.name];

      if (lexer->lookahead().is_punctuator('(')) {
         lexer->advance();
         lexer->advance();

         while (!lexer->currentToken.is_punctuator(')')
                && lexer->currentToken.get_type() != T_EOF) {
            attr.args.push_back(lexer->parseExpression({}, 0, false, true));
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

   if (lexer->currentToken.get_type() == T_KEYWORD
       && lexer->strVal() == "unsafe") {
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

      if (lexer->currentToken.get_type() == T_EOF
          || lexer->currentToken.is_punctuator('}')) {
         break;
      }

      Statement::SharedPtr stmt = parse_next_stmt();

      if (CurrentFuncDecls != nullptr
          && (stmt->get_type() == NodeType::CLASS_DECL
         || stmt->get_type() == NodeType::ENUM_DECL
              || stmt->get_type() == NodeType::UNION_DECL))
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

void Parser::skip_block()
{
   auto openedBraces = 1;
   auto closedBraces = 0;

   if (!lexer->currentToken.is_punctuator('{')) {
      ParseError::raise("expected '{'", lexer);
   }

   Lexer::IgnoreScope guard(lexer);

   while (openedBraces > closedBraces) {
      lexer->advance();

      if (lexer->currentToken.is_punctuator('{')) {
         ++openedBraces;
      }
      else if (lexer->currentToken.is_punctuator('}')) {
         ++closedBraces;
      }
   }
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
   else if (lexer->currentToken.get_type() == T_IDENT
            && lexer->lookahead().is_operator(":")) {
      string label = lexer->strVal();
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
      nsName = decl->ns_prefix() + util::nextAnonymousNamespace();

      lexer->backtrack();
   }
   else {
      nsName = decl->ns_prefix() + lexer->strVal();
   }

   while (lexer->lookahead().is_punctuator('.')) {
      lexer->advance();
      lexer->advance();

      if (lexer->currentToken.get_type() != T_IDENT) {
         ParseError::raise("Expected identifier after 'namespace'", lexer);
      }

      nsName += "." + lexer->strVal();
   }

   pushNamespace(nsName);

   auto mod = std::make_shared<NamespaceDecl>(nsName, parse_block(true),
                                              anonymous);
   setIndex(mod.get(), start);

   popNamespace();

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

   while (lexer->currentToken.get_type() == T_IDENT
          || lexer->currentToken.is_operator("*")) {
      if (lexer->lookahead().is_punctuator('.')) {
         if (!importNs.empty()) {
            importNs += ".";
         }

         importNs += lexer->strVal();

         lexer->advance();
         if (lexer->lookahead().is_punctuator('{')) {
            lexer->advance();
            lexer->advance();

            while (!lexer->currentToken.is_punctuator('}')) {
               items.push_back(lexer->strVal());
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
         items.push_back(lexer->strVal());
         break;
      }
   }

   if (importNs.empty()) {
      importNs = items.back();
      items.pop_back();
   }

   decl->importNamespace(importNs);

   auto usingStmt = std::make_shared<UsingStmt>(std::move(importNs),
                                                std::move(items));
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
   while (lexer->currentToken.is_keyword("using")) {
      root->addStatement(parse_using_stmt());
      lexer->advance();
   }

   while(lexer->currentToken.get_type() != T_EOF) {
      while (lexer->currentToken.is_separator()
             || lexer->currentToken.is_punctuator(';')) {
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

   SemaPass::connectTree(root.get());
   return root;
}