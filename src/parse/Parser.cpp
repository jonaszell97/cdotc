//
// Created by Jonas Zell on 13.06.17.
//

#include "Parser.h"

#include <cassert>
#include <vector>
#include <string>
#include <llvm/ADT/StringSwitch.h>

#include "../lex/Lexer.h"
#include "../Message/Exceptions.h"
#include "../Util.h"

#include "../Variant/Variant.h"
#include "../Variant/Type/GenericType.h"
#include "../Template/TokenStore.h"

#include "../AST/Passes/Declaration/DeclPass.h"
#include "../AST/Passes/ASTIncludes.h"
#include "../Variant/Type/Generic.h"
#include "../Files/FileManager.h"
#include "../Message/Diagnostics.h"
#include "../AST/Passes/SemanticAnalysis/Builtin.h"

using namespace cdot::diag;
using namespace cdot::cl;
using namespace cdot::support;
using namespace cdot::lex;

namespace cdot {
namespace parse {

static const unsigned MAX_TEMPLATE_ARGS = 32;

Parser::Parser(lex::Lexer<lex::LexerTraits> *lexer, DeclPass *decl)
   : source_id(lexer->getSourceId()),
     lexer(lexer),
     decl(decl)
{
   lexer->lex();
   decl->importFileImports(lexer->getSourceId());
}

Parser::~Parser() = default;

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

Token Parser::lookahead(bool ignoreNewline, bool sw)
{
   return lexer->lookahead(ignoreNewline, sw);
}

void Parser::advance(bool ignoreNewline, bool sw)
{
   lexer->advance(ignoreNewline, sw);
}

const lex::Token& Parser::currentTok() const
{
   return lexer->currentTok();
}

AccessModifier Parser::maybeParseAccessModifier()
{
   switch (currentTok().getKind()) {
      case tok::kw_public:
         advance();
         return AccessModifier::PUBLIC;
      case tok::kw_private:
         advance();
         return AccessModifier::PRIVATE;
      case tok::kw_protected:
         advance();
         return AccessModifier::PROTECTED;
      default:
         return AccessModifier::DEFAULT;
   }
}

TemplateParameter const* Parser::getTemplateParam(llvm::StringRef name)
{
   for (const auto &Params : templateParamStack) {
      for (const auto &Param : *Params) {
         if (name.equals(Param.genericTypeName)) {
            return &Param;
         }
      }
   }

   return nullptr;
}

namespace {

void markAllowUnexpandedParams(TypeRef *ty)
{
   ty->setAllowUnexpandedTemplateArgs(true);
}

} // anonymous namespace

std::shared_ptr<TypeRef> Parser::parse_type(bool allowVariadic)
{
   auto start = currentTok().getSourceLoc();

   bool isReference = false;
   if (currentTok().is(tok::kw_ref)) {
      advance();
      isReference = true;
   }

   auto attrs = parse_attributes();
   auto typeref = parse_type_impl();

   // pointer type
   auto next = lookahead();
   while (1) {
      if (next.oneOf(tok::times, tok::times_times) || next.is_identifier()) {
         if (next.is(tok::times)) {
            advance();
            typeref = makeExpr<TypeRef>(start, move(typeref), TypeRef::Pointer);
            break;
         }
         if (next.is(tok::times_times)) {
            advance();
            typeref = makeExpr<TypeRef>(start, move(typeref), TypeRef::Pointer);
            typeref = makeExpr<TypeRef>(start, move(typeref), TypeRef::Pointer);
            break;
         }

         auto op = next.getValue().strVal;
         if (util::matches("\\*+", op)) {
            while (op.length() > 0) {
               typeref = makeExpr<TypeRef>(start, move(typeref),
                                           TypeRef::Pointer);
               op = op.substr(0, op.length() - 1);
            }

            advance();
         }
         else {
            break;
         }
      }
      // optional type
      else if (next.is(tok::question)) {
         advance();
         typeref = makeExpr<TypeRef>(start, move(typeref), TypeRef::Option);
      }
      // array type
      else if (next.is(tok::open_square)) {
         advance();
         advance();

         auto arraySize = parse_static_expr();
         lexer->expect(tok::close_square);

         typeref = makeExpr<TypeRef>(start, move(typeref), move(arraySize));
      }
      else {
         break;
      }

      next = lookahead();
   }

   if (typeref) {
      typeref->isReference(isReference);
      typeref->setAttributes(std::move(attrs));
   }

   if (typeref->isVariadicArgPackExpansion()) {
      if (!allowVariadic) {
         diag::err(err_generic_error)
            << "variadic expansion is not allowed here"
            << lexer << diag::term;
      }

      typeref->forEachContainedType(markAllowUnexpandedParams);
   }

   return typeref;
}

std::shared_ptr<TypeRef> Parser::parse_type_impl()
{
   auto start = currentTok().getSourceLoc();

   if (currentTok().isContextualKeyword("decltype")) {
      lexer->expect(tok::open_paren);
      advance();

      auto expr = parse_expr_sequence();
      lexer->expect(tok::close_paren);

      return makeExpr<TypeRef>(start, move(expr));
   }

   std::vector<pair<string, std::vector<TemplateArg>>> ns;

   // collection type
   if (currentTok().is(tok::open_square)) {
      advance();

      auto elType = parse_type();
      advance();

      if (currentTok().is(tok::colon)) {
         advance();

         auto valType = parse_type();
         advance();

         if (!currentTok().is(tok::close_square)) {
            ParseError::raise("Expected ']' after dictionary type", lexer);
         }

         std::vector<TemplateArg> templateArgs;
         templateArgs.emplace_back(std::move(elType));
         templateArgs.emplace_back(std::move(valType));

         ns.emplace_back("Dictionary", move(templateArgs));

         return makeExpr<TypeRef>(start, std::move(ns));
      }

      if (!currentTok().is(tok::close_square)) {
         ParseError::raise("Expected ']' after array type", lexer);
      }

      std::vector<TemplateArg> templateArgs;
      templateArgs.emplace_back(std::move(elType));

      ns.emplace_back("Array", std::move(templateArgs));
      return makeExpr<TypeRef>(start, std::move(ns));
   }

   // function or tuple type
   if (currentTok().is(tok::open_paren)) {
      auto argTypes = parse_tuple_type();
      advance();

      // tuple
      if (!currentTok().is(tok::arrow_single)) {
         lexer->backtrack();
         return makeExpr<TypeRef>(start, std::move(argTypes));
      }

      advance();

      auto returnType = parse_type();
      return makeExpr<TypeRef>(start, std::move(returnType),
                                       std::move(argTypes));
   }


   std::vector<pair<string, TypeRef::SharedPtr>> containedTypes;

   Token next;
   bool initial = true;

   while (initial || next.is(tok::period)) {
      if (!initial) {
         advance();
         advance();
      }
      else {
         initial = false;
      }

      if (currentTok().getKind() != tok::ident) {
         ParseError::raise("Unexpected character in type reference", lexer);
      }

      string subName = lexer->strVal();
      std::vector<TemplateArg> subTemplateArgs;

      next = lookahead(false);
      if (next.is(tok::smaller)) {
         advance();

         subTemplateArgs = parse_unresolved_template_args();
         next = lookahead(false);
      }

      ns.emplace_back(std::move(subName), std::move(subTemplateArgs));
   }

   bool variadic = false;
   if (next.is(tok::triple_period)) {
      if (ns.size() != 1)
         diag::err(err_generic_error)
            << "parameter pack expansion operator must be applied to "
               "variadic template arg"
            << lexer << diag::term;

      advance();
      variadic = true;
   }

   auto typeref = makeExpr<TypeRef>(start, std::move(ns));
   typeref->setIsVariadicArgPackExpansion(variadic);

   return typeref;
}

std::shared_ptr<TypedefDecl> Parser::parse_typedef(AccessModifier am,
                                                   bool inRecord) {
   auto start = currentTok().getSourceLoc();

   if (am == AccessModifier::DEFAULT) {
      am = maybeParseAccessModifier();
   }

   if (currentTok().is(tok::kw_typedef)) {
      advance();
   }

   auto originTy = parse_type();

   lexer->expect(tok::as);
   lexer->expect(tok::ident);

   auto alias = move(lexer->strRef());

   auto generics = try_parse_template_parameters();
   auto td = makeExpr<TypedefDecl>(start, am, std::move(alias),
                                   std::move(originTy), std::move(generics));

   decl->DeclareTypedef(td.get());
   DeclPass::addGlobalStatement(td.get());

   return td;
}

std::shared_ptr<AliasDecl> Parser::parse_alias()
{
   auto start = currentTok().getSourceLoc();
   lexer->expect(tok::ident);

   string name = move(lexer->strRef());
   auto params = try_parse_template_parameters();

   std::vector<std::shared_ptr<StaticExpr>> constraints;
   while (lookahead().is(tok::kw_where)) {
      advance();
      advance();

      constraints.push_back(parse_static_expr());
   }

   lexer->expect(tok::equals);
   advance();

   auto aliasExpr = parse_static_expr();

   return makeExpr<AliasDecl>(start, move(name), move(params),
                              move(constraints), move(aliasExpr));
}

namespace {

bool identifierTemplateDecider(const Token &t)
{
   return t.oneOf(tok::newline, tok::close_paren, tok::comma, tok::period,
                  tok::close_square, tok::semicolon, tok::open_brace)
          || t.is_operator();
}

} // anonymous namespace

Expression::SharedPtr Parser::parse_identifier()
{
   auto start = currentTok().getSourceLoc();
   string ident;

   switch (currentTok().getKind()) {
      case tok::ident:
         ident = move(lexer->strRef());
         break;
      case tok::kw_self:
         ident = "self";
         break;
      case tok::kw_super:
         ident = "super";
         break;
      default:
         diag::err(err_generic_error) << "expected identifier"
                                      << lexer << diag::term;
   }

   auto expr = makeExpr<IdentifierRefExpr>(start, std::move(ident));

   if (_is_generic_any(identifierTemplateDecider)) {
      advance();
      expr->setTemplateArgs(parse_unresolved_template_args());
   }

   expr->setMemberExpr(try_parse_member_expr());

   return expr;
}

std::shared_ptr<Expression> Parser::try_parse_member_expr()
{
   auto start = currentTok().getSourceLoc();
   advance(false);

   // member access
   bool pointerAccess = currentTok().is(tok::arrow_single);
   if (currentTok().is(tok::period) || pointerAccess) {
      Lexer<>::ModeScopeGuard guard((Lexer<>::MemberAccess), lexer);
      advance(false);

      // tuple access
      if (currentTok().is(tok::integerliteral)) {
         size_t index = currentTok().getValue().intVal;

         auto mem_ref = makeExpr<MemberRefExpr>(start, index, pointerAccess);
         mem_ref->setMemberExpr(try_parse_member_expr());

         return mem_ref;
      }

      // method call
      Token next = lookahead();
      if (next.is(tok::open_paren) || is_generic_call()) {
         auto call = parse_function_call();
         call->isPointerAccess(pointerAccess);

         return call;
      }

      string ident = move(lexer->strRef());

      auto mem_ref = makeExpr<MemberRefExpr>(start, std::move(ident),
                                             pointerAccess);

      if (_is_generic_any(identifierTemplateDecider)) {
         advance();
         mem_ref->setTemplateArgs(parse_unresolved_template_args());
      }

      mem_ref->setMemberExpr(try_parse_member_expr());

      return mem_ref;
   }

   // call
   if (currentTok().is(tok::open_paren)) {
      auto args = parse_arguments();
      auto call = makeExpr<CallExpr>(start, std::move(args.args));

      call->setMemberExpr(try_parse_member_expr());

      return call;
   }

   // array access
   if (currentTok().is(tok::open_square)) {
      advance();

      Expression::SharedPtr expr = parse_expr_sequence();
      SubscriptExpr::SharedPtr arr_acc = makeExpr<SubscriptExpr>(start,
                                                                 move(expr));

      lexer->expect(tok::close_square);

      arr_acc->setMemberExpr(try_parse_member_expr());

      return arr_acc;
   }

   lexer->backtrack();
   return nullptr;
}

std::shared_ptr<Expression> Parser::parse_collection_literal()
{
   auto start = currentTok().getSourceLoc();

   bool isDictionary = false;
   bool first = true;
   std::vector<Expression::SharedPtr> keys;
   std::vector<Expression::SharedPtr> values;

   while (!currentTok().is(tok::close_square)) {
      if (lookahead().is(tok::close_square)) {
         advance();
         break;
      }

      if (lookahead().is(tok::colon)) {
         isDictionary = true;
         advance();
         continue;
      }

      advance();

      if (currentTok().is(tok::comma))
         advance();

      auto key = parse_expr_sequence(false, true);
      advance();

      if (currentTok().is(tok::colon)) {
         if (!first && !isDictionary) {
            ParseError::raise("Unexpected token ':'", lexer);
         }

         advance();
         auto value = parse_expr_sequence();
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

   if (!currentTok().is(tok::close_square)) {
      ParseError::raise("Expected ']' after array literal", lexer);
   }

   if (isDictionary)
      return makeExpr<DictionaryLiteral>(start, std::move(keys),
                                         std::move(values));

   return makeExpr<ArrayLiteral>(start, std::move(values));
}

Expression::SharedPtr Parser::parse_unary_expr_target()
{
   std::vector<Attribute> attributes;

   if (currentTok().is(tok::at)) {
      attributes = parse_attributes();
   }

   if (currentTok().isContextualKeyword("__traits")) {
      auto expr = parse_traits_expr();
      expr->setAttributes(move(attributes));

      return expr;
   }

   if (currentTok().is(tok::kw_static)) {
      lexer->expect(tok::open_paren);
      advance();

      auto expr = parse_static_expr();

      lexer->expect(tok::close_paren);
      return expr;
   }

   if (currentTok().is(tok::open_square)) {
      auto arr = parse_collection_literal();
      arr->setAttributes(std::move(attributes));

      return arr;
   }

   if (currentTok().is(tok::open_paren)) {
      auto expr = parse_paren_expr();
      expr->setAttributes(std::move(attributes));

      return expr;
   }

   // enum case with inferred type
   if (currentTok().is(tok::period)) {
      advance(tok::ident);
      return parse_enum_case_expr();
   }

   if (currentTok().oneOf(tok::ident, tok::kw_self, tok::kw_super)) {
      auto start = currentTok().getSourceLoc();
      Token next = lookahead(false);

      // function call
      bool isVariadicSizeof = currentTok().isContextualKeyword("sizeof")
                              && lookahead().is(tok::triple_period);
      if (isVariadicSizeof || next.is(tok::open_paren) || is_generic_call()) {
         return parse_function_call();
      }

      // single argument lambda
      if (next.is(tok::arrow_double)) {
         auto argName = lexer->strVal();
         auto arg = makeExpr<FuncArgDecl>(start, move(argName),
                                          makeExpr<TypeRef>(start), nullptr,
                                          false, true);

         start = currentTok().getSourceLoc();

         advance();
         advance();

         auto body = parse_next_stmt();
         auto lambda = makeExpr<LambdaExpr>(start, makeExpr<TypeRef>(start),
            std::vector<FuncArgDecl::SharedPtr>{ arg }, std::move(body));

         lambda->setMemberExpr(try_parse_member_expr());

         return lambda;
      }

      auto ident = parse_identifier();
      ident->setAttributes(std::move(attributes));

      return ident;
   }

   auto start = currentTok().getSourceLoc();

   if (currentTok().is(tok::kw_none)) {
      auto noneLiteral = makeExpr<NoneLiteral>(start);
      noneLiteral->setAttributes(std::move(attributes));

      return noneLiteral;
   }

   if (currentTok().oneOf(tok::kw_true, tok::kw_false)) {
      auto expr = makeExpr<BoolLiteral>(start, currentTok().is(tok::kw_true));
      expr->setAttributes(move(attributes));

      return expr;
   }

   if (currentTok().oneOf(tok::integerliteral, tok::fpliteral,
                          tok::charliteral)) {
      auto &val = currentTok()._value;
      Expression::SharedPtr expr;

      if (val.kind == VariantType::INT) {
         switch (val.getBitwidth()) {
            case CHAR_BIT:
               expr = makeExpr<CharLiteral>(start, val.charVal);
               break;
            default:
               expr = makeExpr<IntegerLiteral>(start, Variant(val));
               break;
         }
      }
      else if (val.kind == VariantType::FLOAT) {
         expr = makeExpr<FPLiteral>(start, Variant(val));
      }

      expr->setMemberExpr(try_parse_member_expr());
      expr->setAttributes(std::move(attributes));

      return expr;
   }

   if (currentTok().is(tok::stringliteral)) {
      auto val = currentTok().getValue();

      if (val.kind == VariantType::STRING) {
         Token next = lookahead(false);

         auto strLiteral = makeExpr<StringLiteral>(start,
                                                   move(lexer->strRef()));

         strLiteral->setAttributes(std::move(attributes));

         if (!lookahead().is(tok::sentinel)) {
            // concatenate adjacent string literals
            if (lookahead().is(tok::stringliteral)) {
               string s = strLiteral->getValue();
               while (lookahead().is(tok::stringliteral)) {
                  advance();
                  s += currentTok()._value.getString();
               }

               strLiteral = makeExpr<StringLiteral>(start, move(s));
            }

            strLiteral->setMemberExpr(try_parse_member_expr());
            return strLiteral;
         }

         std::vector<std::shared_ptr<Expression>> strings{ strLiteral };
         advance();

         while (1) {
            advance();
            strings.push_back(parse_expr_sequence());

            lexer->expect(tok::sentinel);

            if (!lookahead().is(tok::stringliteral)) {
               break;
            }
            else {
               advance();
               auto lit = makeExpr<StringLiteral>(start, move(lexer->strRef()));
               strings.push_back(move(lit));

               if (!lookahead().is(tok::sentinel)) {
                  break;
               }
               else {
                  advance();
               }
            }
         }

         auto interp = makeExpr<StringInterpolation>(start, move(strings));
         interp->setMemberExpr(try_parse_member_expr());
         interp->setAttributes(std::move(attributes));

         return interp;
      }
   }

   ParseError::raise("Expected expression but found " +
                     currentTok().rawRepr(), lexer);
   llvm_unreachable(0);
}

Expression::SharedPtr Parser::parse_unary_expr(UnaryOperator::SharedPtr literal,
                                               bool postfix) {
   auto start = currentTok().getSourceLoc();

   // prefix unary op
   if (currentTok().is_operator()) {
      auto unary_op = makeExpr<UnaryOperator>(start, currentTok().toString(),
                                              "prefix");

      advance();
      unary_op->setTarget(parse_unary_expr());

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

   Token next = lookahead(false);

   // postfix unary op
   if (next.oneOf(tok::plus_plus, tok::minus_minus)) {
      advance();
      auto unary_op = makeExpr<UnaryOperator>(start, currentTok().toString(),
                                              "postfix");

      unary_op->setMemberExpr(try_parse_member_expr());
      unary_op->setTarget(expr);

      return unary_op;
   }

   // call
   if (next.is(tok::open_paren) || is_generic_call()) {
      advance();

      auto generics = parse_unresolved_template_args();

      auto call = makeExpr<CallExpr>(start, parse_arguments().args);

      call->setTemplateArgs(std::move(generics));
      expr->setMemberExpr(call);
   }

   return expr;
}

std::shared_ptr<TertiaryOperator>
Parser::parse_tertiary_operator(std::shared_ptr<Expression> cond)
{
   auto start = currentTok().getSourceLoc();

   advance();
   auto lhs = parse_static_expr(false, true);

   advance();
   if (!currentTok().is(tok::colon)) {
      ParseError::raise("expected ':' in tertiary expression", lexer);
   }

   advance();
   auto rhs = parse_expr_sequence();

   return makeExpr<TertiaryOperator>(start, move(cond), move(lhs), move(rhs));
}

ParenExprType Parser::get_paren_expr_type()
{
   int open_parens = 1;
   int closed_parens = 0;

   auto isLambda = false;
   bool isTuple = false;
   bool maybeFuncTy = false;

   Lexer<>::StateSaveGuard guard(lexer);
   assert(currentTok().is(tok::open_paren));

   if (lookahead().is(tok::close_paren)) {
      isTuple = true;
   }

   auto begin = currentTok().getSourceLoc();

   while (open_parens > closed_parens) {
      advance();
      switch (currentTok().getKind()) {
         case tok::open_paren:
            ++open_parens;
            break;
         case tok::close_paren:
            ++closed_parens;
            break;
         case tok::comma:
            isTuple |= (open_parens - closed_parens) == 1;
            break;
         case tok::eof:
            diag::err(err_generic_error)
               << "unexpected end of file, expecting ')'"
               << lexer->tokens[lexer->tokens.size() - 2].getSourceLoc()
               << diag::cont;

            diag::note(note_generic_note)
               << "to match this" << begin << diag::term;
         default:
            break;
      }
   }

   if (open_parens != closed_parens) {
      ParseError::raise("Expression contains unmatched parentheses", lexer);
   }

   advance();

   if (currentTok().is(tok::arrow_single)) {
      maybeFuncTy = true;
      advance();
   }
   if (currentTok().is(tok::arrow_double)) {
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

bool openParenDecider(const lex::Token &tok)
{
   return tok.is(tok::open_paren);
}

bool periodDecider(const lex::Token &tok)
{
   return tok.is(tok::period);
}

} // anonymous namespace

bool Parser::is_generic_call()
{
   return _is_generic_any(openParenDecider);
}

bool Parser::is_generic_member_access()
{
   return _is_generic_any(periodDecider);
}

bool Parser::_is_generic_any(bool (*decider)(const lex::Token &))
{
   Lexer<>::IgnoreScope ignore(lexer);
   Lexer<>::StateSaveGuard guard(lexer);
   advance();

   if (!currentTok().is(tok::smaller)) {
      return false;
   }

   auto openBrackets = 1;
   auto closeBrackets = 0;

   while (openBrackets > closeBrackets) {
      advance(false);

      switch (currentTok().getKind()) {
         case tok::smaller:
            ++openBrackets;
            break;
         case tok::greater:
            ++closeBrackets;
            break;
         case tok::newline:
            return false;
         case tok::eof:
            return false;
         default:
            break;
      }
   }

   if (!currentTok().is(tok::greater))
      return false;

   advance(false);

   return decider(currentTok());
}

Expression::SharedPtr Parser::parse_paren_expr()
{
   ParenExprType type = get_paren_expr_type();
   Expression::SharedPtr expr;
   switch (type) {
      case ParenExprType::LAMBDA:
         expr = parse_lambda_expr();
         break;
      case ParenExprType::FunctionType:
         return parse_type();
      case ParenExprType::EXPR:
         advance();

         expr = parse_expr_sequence(true);
         advance(); // last part of expr
         if (!currentTok().is(tok::close_paren)) {
            ParseError::raise("Expected ')'", lexer);
         }

         expr->setMemberExpr(try_parse_member_expr());

         break;
      case ParenExprType::TUPLE:
         expr = parse_tuple_literal();
         break;
   }

   return expr;
}

std::shared_ptr<TupleLiteral> Parser::parse_tuple_literal()
{
   auto start = currentTok().getSourceLoc();
   std::vector<pair<string, Expression::SharedPtr>> elements;

   advance();

   while (!currentTok().is(tok::close_paren)) {
      std::vector<Attribute> attributes = parse_attributes();
      string label;

      if (currentTok().getKind() == tok::ident
          && lookahead().is(tok::colon)) {
         label = lexer->strVal();
         advance();
      }

      auto expr = parse_expr_sequence();
      expr->setAttributes(std::move(attributes));

      elements.emplace_back(label, expr);

      advance();

      if (currentTok().is(tok::comma)) {
         advance();
      }
      else if (!currentTok().is(tok::close_paren)) {
         ParseError::raise("Expected ',' or ')'", lexer);
      }
   }

   if (!currentTok().is(tok::close_paren)) {
      ParseError::raise("Expected closing parenthesis after tuple literal",
                        lexer);
   }

   auto tup = makeExpr<TupleLiteral>(start, move(elements));
   tup->setMemberExpr(try_parse_member_expr());

   return tup;
}

namespace {

using SeqVec = std::vector<ExprSequence::SequenceElement>;
using iterator = SeqVec::iterator;

bool match(SeqVec const& fragments, iterator const& it) { return true; }

template <class ...Rest>
bool match(SeqVec const& fragments,
           iterator const& it,
           ExprSequence::SequenceElement::Kind kind,
           Rest... rest) {
   if (it == fragments.end())
      return false;

   if (it->getKind() != kind)
      return false;

   return match(fragments, it + 1, rest...);
}

template <class ...Rest>
bool match(SeqVec const& fragments, size_t idx, Rest... rest)
{
   assert(fragments.size() > idx);
   return match(fragments, fragments.begin() + idx, rest...);
}

LLVM_ATTRIBUTE_UNUSED
std::shared_ptr<Expression> getExpr(ExprSequence::SequenceElement &El)
{
   std::shared_ptr<Expression> expr;
   switch (El.getKind()) {
      case ExprSequence::SequenceElement::EF_Operator:
         expr = std::make_shared<IdentifierRefExpr>(
            string(operatorToString(El.getOperatorKind())));
         break;
      case ExprSequence::SequenceElement::EF_Expression:
         expr = El.getExpr();
         break;
      case ExprSequence::SequenceElement::EF_PossibleOperator:
         expr = std::make_shared<IdentifierRefExpr>(
            string(El.getOp()));
         break;
   }

   expr->setSourceLoc(El.getLoc());
   return expr;
}

} // anonymous namespace

Expression::SharedPtr Parser::parse_expr_sequence(bool parenthesized,
                                                  bool ignoreColon) {
   if (StaticExprStack)
      return parse_static_expr();

   auto start = currentTok().getSourceLoc();
   bool foundQuestion = false;

   std::vector<ExprSequence::SequenceElement> frags;
   for (;;) {
      if (currentTok().oneOf(tok::ident, tok::kw_self, tok::kw_super)) {
         auto expr = parse_unary_expr_target();
         auto ident = dyn_cast<IdentifierRefExpr>(expr.get());

         if (ident && !ident->getMemberExpr()
                                          && ident->getTemplateArgs().empty()) {
            frags.emplace_back(string(ident->getIdent()),
                               ident->getSourceLoc());
         }
         else {
            frags.emplace_back(move(expr));
         }
      }
      else if (currentTok().oneOf(tok::as, tok::as_question, tok::as_exclaim)) {
         frags.emplace_back(stringToOperator(currentTok().toString()),
                            currentTok().getSourceLoc());

         advance();
         frags.emplace_back(parse_type());
      }
      else if (currentTok().is(tok::triple_period)) {
         if (frags.empty()) {
            diag::err(err_generic_error)
               << "unexpected pack expansion operator"
               << lexer << diag::term;
         }

         if (frags.size() == 1) {
            auto &back = frags.back();

            if (back.isExpression()) {
               back.getExpr()->setIsVariadicArgPackExpansion(true);
            }
            else {
               string ident = back.isOperator()
                              ? operatorToString(back.getOperatorKind())
                              : back.getOp();

               auto expr = makeExpr<IdentifierRefExpr>(back.getLoc(),
                                                       move(ident));

               expr->setIsVariadicArgPackExpansion(true);

               frags.pop_back();
               frags.emplace_back(move(expr));
            }
         }
         else {
            using Kind = ExprSequence::SequenceElement::Kind;

            auto it = frags.end() - 2;

            // ++args...
            if (match(frags, it, Kind::EF_Operator, Kind::EF_PossibleOperator)){
               auto &back = frags.back();
               string ident = back.isOperator()
                              ? operatorToString(back.getOperatorKind())
                              : back.getOp();

               auto expr = makeExpr<IdentifierRefExpr>(back.getLoc(),
                                                       move(ident));

               frags.pop_back();
               frags.emplace_back(move(expr));
            }
            // args && ...
            else if (match(frags, it, Kind::EF_PossibleOperator,
                           Kind::EF_Operator)) {
               auto op = std::move(frags.back());
               frags.pop_back();

               auto ident = std::move(frags.back());
               frags.pop_back();

               auto identExpr =
                  makeExpr<IdentifierRefExpr>(ident.getLoc(),
                                              move(ident.getOp()));

               auto triplePeriod =
                  makeExpr<IdentifierRefExpr>(currentTok().getSourceLoc(),
                                              "...");

               frags.emplace_back(makeExpr<BinaryOperator>(
                  op.getLoc(),
                  operatorToString(op.getOperatorKind()),
                  move(identExpr), move(triplePeriod)));
            }
            // args op ...
            else if (match(frags, it, Kind::EF_PossibleOperator,
                           Kind::EF_PossibleOperator)) {
               auto op = std::move(frags.back());
               frags.pop_back();

               auto ident = std::move(frags.back());
               frags.pop_back();

               auto identExpr =
                  makeExpr<IdentifierRefExpr>(ident.getLoc(),
                                              move(ident.getOp()));

               auto triplePeriod =
                  makeExpr<IdentifierRefExpr>(currentTok().getSourceLoc(),
                                              "...");

               frags.emplace_back(makeExpr<BinaryOperator>(
                  op.getLoc(),
                  move(op.getOp()),
                  move(identExpr), move(triplePeriod)));
            }
         }
      }
      else if (currentTok().is(tok::question)) {
         foundQuestion = true;
         frags.emplace_back("?", currentTok().getSourceLoc());
      }
      else if (currentTok().is(tok::colon) && foundQuestion) {
         foundQuestion = false;
         frags.emplace_back(":", currentTok().getSourceLoc());
      }
      else if (currentTok().is(tok::colon) && ignoreColon) {
         lexer->backtrack();
         break;
      }
      else if (currentTok().is_operator()) {
         frags.emplace_back(stringToOperator(currentTok().toString()),
                            currentTok().getSourceLoc());
      }
      else if (!currentTok().oneOf(tok::comma, tok::semicolon,
                                   tok::close_paren, tok::eof,
                                   tok::newline, tok::sentinel,
                                   tok::open_brace, tok::close_brace,
                                   tok::close_square, tok::colon,
                                   tok::question)) {
         frags.emplace_back(parse_unary_expr_target());
      }
      else {
         lexer->backtrack();
         break;
      }

      advance(frags.back().isOperator());
   }

   if (frags.size() == 1 && !parenthesized) {
      std::shared_ptr<Expression> singleExpr;
      switch (frags.front().getKind()) {
         case ExprSequence::SequenceElement::EF_Operator:
            singleExpr = makeExpr<IdentifierRefExpr>(
               start, operatorToString(frags.front().getOperatorKind()));
            break;
         case ExprSequence::SequenceElement::EF_Expression:
            singleExpr = frags.front().getExpr();
            break;
         case ExprSequence::SequenceElement::EF_PossibleOperator:
            singleExpr = makeExpr<IdentifierRefExpr>(
               start, string(frags.front().getOp()));
            break;
      }

      return singleExpr;
   }
   else if (frags.empty()) {
      diag::err(err_generic_error) << "unexpected token "
                                      + currentTok().toString()
                                   << lexer << diag::term;
   }

   return makeExpr<ExprSequence>(start, move(frags), parenthesized);
}

namespace {

int hasHigherPrecedence(Token& next, int& min_precedence) {
   if (next.isContextualKeyword(":>") || next.isContextualKeyword("<:"))
      return util::op_precedence["as"];

   if (next.is(tok::colon))
      return util::op_precedence["as"];

   if (next.is_operator()) {
      auto op = next.toString();
      auto it = util::op_precedence.find(op);

      return (it != util::op_precedence.end() && it->second > min_precedence)
             ? it->second : 0;
   }

   return 0;
}

int hasHigherOrEqualPrecedence(Token& next, int& min_precedence) {
   if (next.isContextualKeyword(":>") || next.isContextualKeyword("<:"))
      return util::op_precedence["as"];

   if (next.is(tok::colon))
      return util::op_precedence["as"];

   if (next.is_operator()) {
      auto op = next.toString();
      auto it = util::op_precedence.find(op);

      return (it != util::op_precedence.end() && it->second >= min_precedence)
             ? it->second : 0;
   }

   return 0;
}

} // anonymous namespace

std::shared_ptr<StaticExpr> Parser::parse_static_expr(
                                                bool ignoreGreater,
                                                bool ignoreColon,
                                                std::shared_ptr<Expression> lhs,
                                                int minPrecedence) {
   ++StaticExprStack;

   auto start = currentTok().getSourceLoc();
   if (lhs == nullptr) {
      lhs = parse_unary_expr();
   }

   Token next = lookahead(false);
   while (auto prec = hasHigherOrEqualPrecedence(next, minPrecedence)) {
      if (next.is(tok::greater) && ignoreGreater)
         break;
      if (next.is(tok::colon) && ignoreColon)
         break;

      Expression::SharedPtr rhs;
      string op = next.rawRepr();
      auto operatorLoc = next.getSourceLoc();

      if (next.oneOf(tok::as, tok::isa, tok::isa,
                     tok::as_exclaim, tok::as_question)
          || op == ":>" || op == "<:") {
         advance();
         advance();

         rhs = parse_type();
      }
      else if (next.is(tok::colon)) {
         advance();
         advance();

         rhs = parse_constraint_expr();
      }
      else {
         advance();
         advance();

         rhs = parse_unary_expr();
      }

      next = lookahead(false);
      while (auto nextPrec = hasHigherPrecedence(next, prec)) {
         rhs = parse_static_expr(ignoreGreater, ignoreColon, rhs, nextPrec);
         next = lookahead(false);
      }

      lhs = makeExpr<BinaryOperator>(operatorLoc, move(op),
                                     move(lhs), move(rhs));
   }

   // tertiary operator
   if (next.is(tok::question)) {
      advance(false);
      lhs = parse_tertiary_operator(lhs);
   }

   --StaticExprStack;

   return makeExpr<StaticExpr>(start, move(lhs));
}

std::shared_ptr<ConstraintExpr> Parser::parse_constraint_expr()
{
   auto start = currentTok().getSourceLoc();
   ConstraintExpr::Kind kind;

   switch (currentTok().getKind()) {
      case tok::kw_class:
         kind = ConstraintExpr::Class;
         break;
      case tok::kw_struct:
         kind = ConstraintExpr::Struct;
         break;
      case tok::kw_enum:
         kind = ConstraintExpr::Enum;
         break;
      case tok::kw_union:
         kind = ConstraintExpr::Union;
         break;
      case tok::kw_def:
         kind = ConstraintExpr::Function;
         break;
      case tok::times:
         kind = ConstraintExpr::Pointer;
         break;
      case tok::kw_ref:
         kind = ConstraintExpr::Reference;
         break;
      case tok::ident:
         if (currentTok().isContextualKeyword("default")) {
            kind = ConstraintExpr::DefaultConstructible;
            break;
         }
         LLVM_FALLTHROUGH;
      default:
         return makeExpr<ConstraintExpr>(start, parse_type());
   }

   return makeExpr<ConstraintExpr>(start, kind);
}

namespace {

struct TraitArguments {
   enum Kind : unsigned char {
      None,
      Expr,
      Stmt,
      Type,
      String,
      Identifier
   };

   TraitArguments(Kind arg1 = None, Kind arg2 = None,
                  Kind arg3 = None, Kind arg4 = None)
      : args{ arg1, arg2, arg3, arg4 }
   { }

   Kind args[4];
};

using Kind = TraitArguments::Kind;

TraitArguments traitArgs[] = {
   { Kind::Stmt },               // Compiles
   { Kind::Stmt },               // CompileErrors
   { Kind::Type, Kind::String }, // HasMember
   { Kind::Type, Kind::String }, // HasProperty
   { Kind::Type, Kind::String }, // HasStaticMember
   { Kind::Type, Kind::String }, // HasStaticProperty
   { Kind::Type, Kind::String }, // HasMethod
   { Kind::Type, Kind::String }, // HasStaticMethod,
   { Kind::Identifier },         // ValidIdentifier
   { Kind::Identifier },         // ValidFunction
   { Kind::Type },               // IsInteger
   { Kind::Type },               // IsFloat
   { Kind::Type },               // IntegerBitwidth
   { Kind::Type },               // IsUnsigned
   { Kind::Type },               // FPPrecision
   { Kind::Expr },               // Arity
};

} // anonymous namespace

std::shared_ptr<TraitsExpr> Parser::parse_traits_expr()
{
   auto prev = StaticExprStack;
   StaticExprStack = 0;

   auto start = currentTok().getSourceLoc();

   lexer->expect(tok::open_paren);
   lexer->expect(tok::ident);

   auto str = currentTok()._value.getString();
   auto kind = llvm::StringSwitch<TraitsExpr::Kind>(str)
      .Case("compiles", TraitsExpr::Compiles)
      .Case("compile_errors", TraitsExpr::CompileErrors)
      .Case("has_member", TraitsExpr::HasMember)
      .Case("has_static_member", TraitsExpr::HasStaticMember)
      .Case("has_property", TraitsExpr::HasProperty)
      .Case("has_static_property", TraitsExpr::HasStaticProperty)
      .Case("has_method", TraitsExpr::HasMethod)
      .Case("has_static_method", TraitsExpr::HasStaticMethod)
      .Case("valid_identifier", TraitsExpr::ValidIdentifier)
      .Case("valid_function", TraitsExpr::ValidFunction)
      .Case("is_integer", TraitsExpr::IsInteger)
      .Case("is_fp", TraitsExpr::IsFloat)
      .Case("bitwidth_of", TraitsExpr::IntegerBitwidth)
      .Case("is_unsigned", TraitsExpr::IsUnsigned)
      .Case("fp_precision", TraitsExpr::FPPrecision)
      .Case("arity", TraitsExpr::Arity)
      .Default(TraitsExpr::Invalid);

   if (kind == TraitsExpr::Invalid)
      diag::err(err_generic_error)
         << "invalid __traits directive" << lexer << diag::term;

   std::vector<TraitsExpr::TraitsArgument> args;
   auto &argKinds = traitArgs[kind];

   size_t i = 0;
   while (argKinds.args[i] != TraitArguments::None) {
      advance();
      if (currentTok().is(tok::comma))
         advance();

      switch (argKinds.args[i]) {
         case TraitArguments::Expr:
            args.emplace_back(parse_expr_sequence());
            break;
         case TraitArguments::Stmt:
            args.emplace_back(parse_next_stmt());
            break;
         case TraitArguments::Type:
            args.emplace_back(parse_type());
            break;
         case TraitArguments::Identifier:
            if (!currentTok().is(tok::ident))
               diag::err(err_generic_error)
                  << "expected identifier" << lexer << diag::term;

            args.emplace_back(string(currentTok()._value.getString()));
            break;
         case TraitArguments::String:
            if (!currentTok().is(tok::stringliteral))
               diag::err(err_generic_error)
                  << "expected string literal" << lexer << diag::term;

            args.emplace_back(string(currentTok()._value.getString()));
            break;
         default:
            llvm_unreachable("bad arg kind");
      }

      ++i;
   }

   lexer->expect(tok::close_paren);
   StaticExprStack = prev;

   return makeExpr<TraitsExpr>(start, kind, std::move(args));
}

Statement::SharedPtr Parser::parse_declaration(bool is_const,
                                               bool is_declaration) {
   std::vector<Statement::SharedPtr> declarations;
   auto access = maybeParseAccessModifier();

   auto start = currentTok().getSourceLoc();

   for (;;) {
      auto start = currentTok().getSourceLoc();
      advance();
      std::vector<string> identifiers;

      if (currentTok().is(tok::open_paren)) {
         advance();

         while (!currentTok().is(tok::close_paren)) {
            identifiers.emplace_back(move(lexer->strRef()));

            advance();
            if (currentTok().is(tok::comma))
               advance();
         }
      }
      else {
         identifiers.emplace_back(move(lexer->strRef()));
      }

      Expression::SharedPtr value = nullptr;
      TypeRef::SharedPtr typeref = makeExpr<TypeRef>(start);

      Token next = lookahead(false);

      if (next.is(tok::colon)) {
         advance();
         advance();

         typeref = parse_type();

         next = lookahead();
      }
      else if (is_declaration) {
         ParseError::raise("Declared variables must be type annotated", lexer);
      }

      typeref->getType(true).isConst(is_const);

      if (next.is(tok::equals)) {
         if (is_declaration) {
            ParseError::raise("Declared variables cannot be assigned", lexer);
         }

         advance(false);
         advance();

         value = parse_expr_sequence();
      }

      std::shared_ptr<Statement> decl;
      if (top_level) {
         decl = makeExpr<GlobalVarDecl>(start, access, move(identifiers),
                                        move(typeref), is_const, move(value));
      }
      else {
         decl = makeExpr<LocalVarDecl>(start, move(identifiers), move(typeref),
                                       is_const, move(value));
      }

      declarations.push_back(decl);

      next = lookahead();
      if (!next.is(tok::comma)) {
         break;
      }
      else if (is_declaration) {
         ParseError::raise("Compound declarations not allowed here", lexer);
      }
   }

   if (declarations.size() == 1) {
      return declarations.front();
   }

   auto compound = makeExpr<CompoundStmt>(start, true);
   for (auto & decl : declarations) {
      compound->addStatement(move(decl));
   }

   return compound;
}

std::vector<pair<string, std::shared_ptr<TypeRef>>> Parser::parse_tuple_type()
{
   std::vector<pair<string, std::shared_ptr<TypeRef>>> tupleTypes;

   while (!currentTok().is(tok::close_paren)) {
      advance();

      if (currentTok().is(tok::close_paren)) {
         break;
      }

      string label;
      if (currentTok().getKind() == tok::ident
          && lookahead().is(tok::colon)) {
         label = lexer->strVal();
         advance();
         advance();
      }

      bool vararg = false;
      if (currentTok().is(tok::triple_period)) {
         vararg = true;
         advance();

         // c style vararg
         if (currentTok().getKind() != tok::ident) {
            auto typeref = makeExpr<TypeRef>(currentTok().getSourceLoc());
            typeref->isCStyleVararg(true);

            tupleTypes.emplace_back(label, typeref);
            continue;
         }
      }

      auto argType = parse_type(true);
      argType->isVararg(vararg);

      tupleTypes.emplace_back(label, argType);
      advance();
      if (!currentTok().is(tok::comma)
          && !currentTok().is(tok::close_paren)) {
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
std::vector<FuncArgDecl::SharedPtr> Parser::parse_arg_list(bool optionalNames,
                                                           bool optionalTypes,
                                                       bool isTemplateArgList) {
   auto start = currentTok().getSourceLoc();
   std::vector<FuncArgDecl::SharedPtr> args;
   if (!currentTok().is(tok::open_paren)) {
      lexer->backtrack();
      return args;
   }

   bool def_arg = false;
   bool var_arg = false;

   advance();

   while (!currentTok().is(tok::close_paren)) {
      if (var_arg) {
         ParseError::raise("Vararg arguments can only be the last argument"
                              "of a function", lexer);
      }

      std::vector<Attribute> attributes = parse_attributes();

      start = currentTok().getSourceLoc();

      string argName;
      TypeRef::SharedPtr argType;
      Expression::SharedPtr defaultVal;
      bool templateArgExpansion = false;
      bool isConst = true;

      if (currentTok().oneOf(tok::kw_var, tok::kw_let)) {
         isConst = currentTok().is(tok::kw_let);
         advance();
      }

      if (!optionalNames || lookahead().is(tok::colon)) {
         if (currentTok().getKind() != tok::ident) {
            ParseError::raise("Expected identifier", lexer);
         }

         argName = std::move(lexer->strRef());
         advance();
      }
      else if (currentTok().is(tok::triple_period)) {
         var_arg = true;
         advance();
         if (lookahead().getKind() != tok::ident) {
            auto typeref = makeExpr<TypeRef>(start);
            typeref->isCStyleVararg(true);

            auto argDecl = makeExpr<FuncArgDecl>(start, "", move(typeref),
                                                 nullptr, false, true);
            args.push_back(move(argDecl));
            break;
         }
      }

      if (currentTok().is(tok::colon) || optionalNames) {
         if (currentTok().is(tok::colon)) {
            advance();
         }

         argType = parse_type(true);

         argType->isVararg(var_arg);

         advance();

         if (argType->isVariadicArgPackExpansion()) {
            if (var_arg) {
               diag::err(err_generic_error) << "template parameter pack "
                  "expansions cannot be vararg" << lexer << diag::term;
            }

            templateArgExpansion = true;
         }
      }
      else if (!optionalTypes) {
         ParseError::raise("Function arguments have to have a specified type",
                           lexer);
      }

      // optional default value
      if (currentTok().is(tok::equals)) {
         if (var_arg) {
            ParseError::raise("Vararg arguments cannot have a default value",
                              lexer);
         }

         advance();

         defaultVal = parse_expr_sequence();
         def_arg = true;

         advance();

      } else if (def_arg) {
         ParseError::raise("Default values are only allowed as last items of "
                              "an argument list", lexer);
      }

      auto argDecl = makeExpr<FuncArgDecl>(start, move(argName),
                                           move(argType),
                                           move(defaultVal),
                                           templateArgExpansion,
                                           isConst);

      args.push_back(move(argDecl));

      // end of argument list or next argument
      if (currentTok().is(tok::comma)) {
         advance();
      }
      else if (!currentTok().is(tok::close_paren)) {
         ParseError::raise("Expected closing parenthesis after "
                              "argument list", lexer);
      }

      start = currentTok().getSourceLoc();
   }

   if (!currentTok().is(tok::close_paren)) {
      ParseError::raise("Expected closing parenthesis after argument "
                           "list", lexer);
   }

   return args;
}

bool Parser::isValidOperatorChar(const Token& next) {
   return !next.is_punctuator() && !next.is(tok::eof)
          && !next.is(tok::arrow_single);
}

FunctionDecl::SharedPtr Parser::parse_function_decl(bool is_declaration)
{
   auto start = currentTok().getSourceLoc();

   AccessModifier am = maybeParseAccessModifier();
   OperatorInfo op;

   if (lookahead().oneOf(tok::kw_infix, tok::kw_prefix, tok::kw_postfix)) {
      advance();

      switch (currentTok().getKind()) {
         case tok::kw_infix: op.setFix(FixKind::Infix); break;
         case tok::kw_prefix: op.setFix(FixKind::Prefix); break;
         case tok::kw_postfix: op.setFix(FixKind::Postfix); break;
         default:
            llvm_unreachable("bad fix kind");
      }

      op.setPrecedenceGroup(PrecedenceGroup(12, Associativity::Left));
   }

   advance();

   string funcName;
   TypeRef::SharedPtr returnType;
   bool isCastOp;

   if (op.getPrecedenceGroup().isValid()) {
      funcName = parse_operator_name(isCastOp, returnType);
   }
   else {
      funcName = lexer->strVal();
   }

   std::vector<Statement::SharedPtr> innerDecls;
   CurrentFuncDecls = &innerDecls;

   auto templateParams = try_parse_template_parameters();
   templateParamStack.push_back(&templateParams);

   advance();

   auto args = parse_arg_list(true);

   // optional return type
   if (lookahead().is(tok::arrow_single)) {
      advance();
      advance();
      returnType = parse_type();
   }
   else if (is_declaration) {
      ParseError::raise("Declared functions have to have a defined return type",
                        lexer);
   }
   else {
      returnType = makeExpr<TypeRef>(start);
   }

   std::vector<std::shared_ptr<StaticExpr>> constraints;
   while (lookahead().is(tok::kw_where)) {
      advance();
      advance();

      constraints.push_back(parse_static_expr());
   }

   CompoundStmt::SharedPtr body = nullptr;

   if (lookahead().is(tok::open_brace)) {
      if (is_declaration) {
         ParseError::raise("declared functions cannot have a body", lexer);
      }

      body = parse_block();
   }

   CurrentFuncDecls = nullptr;

   auto funcDecl = makeExpr<FunctionDecl>(start, am, std::move(funcName),
                                          std::move(args),
                                          std::move(returnType),
                                          std::move(templateParams),
                                          move(constraints),
                                          move(body), op);

   funcDecl->isDeclaration(is_declaration);
   funcDecl->setInnerDecls(std::move(innerDecls));

   decl->DeclareFunction(funcDecl.get());
   templateParamStack.pop_back();

   return funcDecl;
}

LambdaExpr::SharedPtr Parser::parse_lambda_expr()
{
   auto start = currentTok().getSourceLoc();
   auto args = parse_arg_list(false, true);

   advance();
   auto retType = makeExpr<TypeRef>(start);

   if (currentTok().is(tok::arrow_single)) {
      advance();
      retType = parse_type();
      advance();
   }

   if (currentTok().is(tok::arrow_double)) {
      advance(); // =>
   }
   else {
      llvm_unreachable("function should never be called in this case");
   }

   Statement::SharedPtr body = parse_next_stmt();

   auto lambda_expr = makeExpr<LambdaExpr>(start, std::move(retType),
                                           std::move(args), std::move(body));

   lambda_expr->setMemberExpr(try_parse_member_expr());

   return lambda_expr;
}

std::vector<TemplateParameter> Parser::try_parse_template_parameters()
{
   std::vector<TemplateParameter> templateArgs;
   auto guard = lexer->makeModeGuard(Lexer<>::TemplateConstraints);

   if (!lookahead().is(tok::smaller)) {
      return templateArgs;
   }

   advance();

   bool defaultGiven = false;

   advance();
   while (!currentTok().is(tok::greater)) {
      if (templateArgs.size() > MAX_TEMPLATE_ARGS) {
         ParseError::raise("templates cannot have more than "
            + std::to_string(MAX_TEMPLATE_ARGS) + " arguments", lexer);
      }

      bool variadic = false;
      auto loc = currentTok().getSourceLoc();

      TemplateParameter::Kind kind = TemplateParameter::TypeName;

      if (currentTok().is(tok::ident)) {
         if (currentTok().isContextualKeyword("typename")) {
            // default
            advance();
         }
         else if (currentTok().isContextualKeyword("value")) {
            kind = TemplateParameter::Value;
            advance();
         }
      }

      if (currentTok().is(tok::triple_period)) {
         variadic = true;
         advance();
      }

      if (currentTok().getKind() != tok::ident) {
         ParseError::raise("expected template argument name", lexer);
      }

      auto genericTypeName = std::move(currentTok()._value.strVal);
      std::shared_ptr<TypeRef> unresolvedCovariance = nullptr;
      std::shared_ptr<TypeRef> unresolvedContravariance = nullptr;

      if (lookahead().is(tok::colon)) {
         advance();
         advance();

         if (kind == TemplateParameter::TypeName) {
            bool covarSet = false;
            bool contravarSet = false;

            for (;;) {
               auto isCovar = true;
               if (currentTok().is(tok::plus)) {
                  isCovar = false;
                  advance();
               }
               else if (currentTok().is(tok::minus)) {
                  // default
                  advance();
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

               if (lookahead().is(tok::comma)
                   || lookahead().is(tok::greater)) {
                  break;
               }

               advance();
            }
         }
         else if (kind == TemplateParameter::Value) {
            unresolvedCovariance = parse_type();
         }
         else {
            ParseError::raise("cannot specify template argument", lexer);
         }
      }

      advance();

      std::shared_ptr<TemplateArg> defaultValue = nullptr;
      if (currentTok().is(tok::equals)) {
         defaultGiven = true;
         advance();

         while (!currentTok().is(tok::greater)
                && !currentTok().is(tok::comma)) {
            switch (kind) {
               case TemplateParameter::TypeName: {
                  defaultValue = makeExpr<TemplateArg>(
                     currentTok().getSourceLoc(), parse_type());
                  break;
               }
               case TemplateParameter::Value: {
                  defaultValue = makeExpr<TemplateArg>(
                     currentTok().getSourceLoc(),
                     parse_static_expr(true));
                  break;
               }
            }

            advance();
         }
      }
      else if (defaultGiven) {
         lexer->backtrack();
         diag::err(err_generic_error) << "default template paremeter values "
            "can only appear at the end of a parameter list"
                                      << lexer << diag::term;
      }

      templateArgs.emplace_back(kind, std::move(genericTypeName),
                                std::move(unresolvedCovariance),
                                std::move(unresolvedContravariance),
                                std::move(defaultValue), variadic, loc);

      if (currentTok().is(tok::comma)) {
         advance();
      }
   }

   return templateArgs;
}

std::vector<TemplateArg> Parser::parse_unresolved_template_args()
{
   if (!currentTok().is(tok::smaller)) {
      return {};
   }

   Lexer<>::ModeScopeGuard guard((Lexer<>::TemplateArgMode), lexer);
   std::vector<TemplateArg> args;

   advance();

   unsigned openCount = 1;
   unsigned closeCount = 0;

   for (;;) {
      auto loc = currentTok().getSourceLoc();
      switch (currentTok().getKind()) {
#        define CDOT_LITERAL_TOKEN(Name, Spelling) \
         case tok::Name:
#        include "../lex/Tokens.def"
            args.emplace_back(parse_static_expr(true));
            break;
#        define CDOT_OPERATOR_TOKEN(Name, Spelling)               \
         case tok::Name:                                          \
            if (currentTok().is(tok::smaller)) ++openCount;       \
            else if (currentTok().is(tok::greater)) ++closeCount; \
            else args.emplace_back(parse_static_expr(true));     \
            break;
#        include "../lex/Tokens.def"
         case tok::open_paren:
            args.emplace_back(parse_type(true));
            break;
         case tok::kw_true:
         case tok::kw_false:
            args.emplace_back(parse_static_expr(true));
            break;
         case tok::ident:
            if (lexer->strRef() == "value") {
               advance();
               args.emplace_back(parse_static_expr(true));
            }
            else {
               auto next = lookahead();
               if (next.is_operator() && !next.oneOf(tok::greater,
                                                     tok::smaller,
                                                     tok::times,
                                                     tok::times_times,
                                                     tok::triple_period)) {
                  args.emplace_back(parse_static_expr(true));
               }
               else {
                  args.emplace_back(parse_type(true));
               }
            }

            break;
         default:
            args.emplace_back(parse_type(true));
            break;
      }

      if (openCount == closeCount) {
         break;
      }

      args.back().setSourceLoc(loc);
      advance();

      if (currentTok().is(tok::comma))
         advance();
   }

   return args;
}

namespace {

void copyRecordDecls(RecordInner &Inner, std::shared_ptr<CompoundStmt> &cmpnd)
{
   for (auto &C : Inner.constructors)
      cmpnd->addStatement(move(C));

   for (auto &F : Inner.fields)
      cmpnd->addStatement(move(F));

   for (auto &M : Inner.methods)
      cmpnd->addStatement(move(M));

   for (auto &T : Inner.typedefs)
      cmpnd->addStatement(move(T));

   for (auto &P : Inner.properties)
      cmpnd->addStatement(move(P));

   for (auto &I : Inner.innerDeclarations)
      cmpnd->addStatement(move(I));

   for (auto &C : Inner.cases)
      cmpnd->addStatement(move(C));

   for (auto &S : Inner.staticStatements)
      cmpnd->addStatement(move(S));

   if (Inner.destructor)
      cmpnd->addStatement(move(Inner.destructor));
}

} // anonymous namespace

IfStmt::SharedPtr Parser::parse_if_stmt()
{
   auto start = currentTok().getSourceLoc();
   advance();

   auto cond = parse_expr_sequence();
   advance();

   auto ifBranch = parse_next_stmt();
   std::shared_ptr<Statement> elseBranch;

   if (lookahead().is(tok::kw_else)) {
      advance();
      advance();

      elseBranch = parse_next_stmt();
   }
   else {
      elseBranch = makeExpr<NullStmt>(start);
   }

   return makeExpr<IfStmt>(start, move(cond), move(ifBranch),
                           move(elseBranch));
}

std::shared_ptr<StaticIfStmt> Parser::parse_static_if(ClassHead *head)
{
   auto start = currentTok().getSourceLoc();
   lexer->advanceIf(tok::kw_static_if);

   auto cond = parse_static_expr();
   advance();

   std::shared_ptr<Statement> ifBranch;
   std::shared_ptr<Statement> elseBranch;

   if (head) {
      RecordInner Inner;
      parse_class_inner(Inner, *head, false, false, false, false, false,
                        nullptr);

      auto body = makeExpr<CompoundStmt>(start);
      copyRecordDecls(Inner, body);

      ifBranch = body;
   }
   else {
      ifBranch = parse_next_stmt();
   }

   if (lookahead().is(tok::kw_else)) {
      advance();
      advance();

      if (head) {
         RecordInner Inner;
         parse_class_inner(Inner, *head, false, false, false, false, false,
                           nullptr);

         auto body = makeExpr<CompoundStmt>(start);
         copyRecordDecls(Inner, body);

         elseBranch = body;
      }
      else {
         elseBranch = parse_next_stmt();
      }
   }
   else {
      elseBranch = makeExpr<NullStmt>(start);
   }

   return makeExpr<StaticIfStmt>(start, move(cond), move(ifBranch),
                                 move(elseBranch));
}

std::shared_ptr<PatternExpr> Parser::parse_pattern()
{
   auto start = currentTok().getSourceLoc();

   if (currentTok().is(tok::isa)) {
      advance();

      return makeExpr<IsPattern>(start, parse_type());
   }

   if (currentTok().is(tok::period)) {
      advance();

      string caseName = move(lexer->strRef());
      std::vector<CasePattern::Argument> args;

      if (!lookahead().is(tok::open_paren))
         return makeExpr<CasePattern>(start, move(caseName), move(args));

      advance();
      while (!currentTok().is(tok::close_paren)) {
         advance();

         auto loc = currentTok().getSourceLoc();
         if (currentTok().oneOf(tok::kw_let, tok::kw_var)) {
            bool isConst = currentTok().is(tok::kw_let);
            lexer->expect(tok::ident);

            auto ident = move(lexer->strRef());
            args.emplace_back(move(ident), isConst, loc);
         }
         else {
            args.emplace_back(parse_expr_sequence(), loc);
         }

         switch (lookahead().getKind()) {
            case tok::comma:
            case tok::close_paren:
               advance();
               break;
            default:
               diag::err(err_generic_error)
                  << "unexpected token " + lookahead().toString()
                  << lexer << diag::term;
         }
      }

      return makeExpr<CasePattern>(start, move(caseName), move(args));
   }

   return makeExpr<ExpressionPattern>(start, parse_expr_sequence());
}

CaseStmt::SharedPtr Parser::parse_case_stmt(bool default_)
{
   auto start = currentTok().getSourceLoc();

   std::shared_ptr<CompoundStmt> body;
   std::shared_ptr<PatternExpr> pattern;

   if (!default_) {
      advance();
      pattern = parse_pattern();
   }

   lexer->expect(tok::colon);

   bool isDefault = lookahead().isContextualKeyword("default");
   if (!lookahead().oneOf(tok::kw_case, tok::close_brace) && !isDefault) {
      body = std::make_shared<CompoundStmt>();

      while (!lookahead().oneOf(tok::kw_case, tok::close_brace) && !isDefault) {
         advance();
         body->addStatement(parse_next_stmt());

         isDefault = lookahead().isContextualKeyword("default");
      }
   }

   std::shared_ptr<CaseStmt> caseStmt;
   if (default_)
      caseStmt = makeExpr<CaseStmt>(start, move(body));
   else
      caseStmt = makeExpr<CaseStmt>(start, move(pattern), move(body));

   advance();

   return caseStmt;
}

MatchStmt::SharedPtr Parser::parse_match_stmt()
{
   auto start = currentTok().getSourceLoc();
   advance();

   Expression::SharedPtr switch_val = parse_expr_sequence();
   advance();
   if (!currentTok().is(tok::open_brace)) {
      ParseError::raise("Expected '{' after 'match'", lexer);
   }

   auto switch_stmt = makeExpr<MatchStmt>(start, move(switch_val));
   advance();

   bool isDefault = false;
   while (!currentTok().is(tok::close_brace)) {
      if (isDefault) {
         ParseError::raise("Expected no more cases after 'default'", lexer);
      }

      isDefault = currentTok().isContextualKeyword("default");
      if (!currentTok().is(tok::kw_case) && !isDefault) {
         ParseError::raise("Expected 'case' or 'default'", lexer);
      }

      auto caseStmt = parse_case_stmt(isDefault);
      switch_stmt->addCase(move(caseStmt));
   }

   return switch_stmt;
}

WhileStmt::SharedPtr Parser::parse_while_stmt(bool conditionBefore)
{
   auto start = currentTok().getSourceLoc();
   advance();

   Expression::SharedPtr cond = makeExpr<BoolLiteral>(start, true);
   if (conditionBefore) {
      cond = parse_expr_sequence();
      advance();
   }

   auto body = std::static_pointer_cast<CompoundStmt>(parse_next_stmt());
   if (!conditionBefore && lookahead().is(tok::kw_while)) {
      advance();
      advance();

      cond = parse_expr_sequence();
   }

   auto whileStmt = makeExpr<WhileStmt>(start, move(cond), move(body),
                                        !conditionBefore);

   return whileStmt;
}

Statement::SharedPtr Parser::parse_for_stmt()
{
   auto start = currentTok().getSourceLoc();
   advance();

   Statement::SharedPtr init;
   if (currentTok().is(tok::semicolon)) {
      init = nullptr;
   }
   else {
      init = parse_next_stmt();

      // parse in as keyword
      auto guard = lexer->makeModeGuard(Lexer<>::ForInStmt);
      advance();
   }

   // range based for loop
   if (currentTok().isContextualKeyword("in")) {
      if (!isa<LocalVarDecl>(init)) {
         ParseError::raise("Expected declaration before 'in' in range based"
                              "for loop", lexer);
      }

      advance();

      Expression::SharedPtr range = parse_expr_sequence();
      advance();

      if (currentTok().is(tok::close_paren)) {
         advance();
      }

      auto body = std::static_pointer_cast<CompoundStmt>(parse_next_stmt());
      auto decl = std::static_pointer_cast<LocalVarDecl>(init);

      auto forInStmt = makeExpr<ForInStmt>(start, std::move(decl),
                                           std::move(range), std::move(body));

      return forInStmt;
   }

   if (!currentTok().is(tok::semicolon)) {
      ParseError::raise("Expected ';' to seperate for loop arguments", lexer);
   }

   Expression::SharedPtr term;
   advance();
   if (currentTok().is(tok::semicolon)) {
      term = makeExpr<BoolLiteral>(start, true);
   }
   else {
      term = parse_expr_sequence();
      advance();
   }

   if (!currentTok().is(tok::semicolon)) {
      ParseError::raise("Expected ';' to seperate for loop arguments", lexer);
   }

   Statement::SharedPtr inc;
   advance();
   if (currentTok().is(tok::open_brace)) {
      inc = nullptr;
   }
   else {
      inc = parse_next_stmt();
      advance();
   }

   auto block = std::static_pointer_cast<CompoundStmt>(parse_next_stmt());

   ForStmt::SharedPtr for_stmt = makeExpr<ForStmt>(start, move(init),
                                                   move(term), move(inc));
   for_stmt->setBody(move(block));

   return for_stmt;
}

std::shared_ptr<StaticForStmt> Parser::parse_static_for(ClassHead *head)
{
   auto start = currentTok().getSourceLoc();

   lexer->expect(tok::kw_let);
   lexer->expect(tok::ident);

   auto ident = std::move(lexer->strRef());
   lexer->expect(tok::ident);
   if (!currentTok().isContextualKeyword("in"))
      diag::err(err_generic_error) << "expected 'in'" << lexer << diag::term;

   advance();

   auto range = parse_static_expr();
   advance();

   Statement::SharedPtr body;
   if (head) {
      RecordInner Inner;
      parse_class_inner(Inner, *head, false, false, false, false, false,
                        nullptr);

      auto compound = makeExpr<CompoundStmt>(start);
      copyRecordDecls(Inner, compound);

      body = compound;
   }
   else {
      body = parse_next_stmt();
   }

   return makeExpr<StaticForStmt>(start, move(ident), move(range),
                                  move(body));
}

std::shared_ptr<StaticStmt> Parser::parse_static_stmt(ClassHead *head)
{
   switch (currentTok().getKind()) {
      case tok::kw_static_assert:
         return parse_static_assert();
      case tok::kw_static_for:
         return parse_static_for(head);
      case tok::kw_static_if:
         return parse_static_if(head);
      default:
         llvm_unreachable("bad static stmt kind");
   }
}

std::shared_ptr<StaticAssertStmt> Parser::parse_static_assert()
{
   auto start = currentTok().getSourceLoc();
   lexer->expect(tok::open_paren);
   advance();

   auto expr = parse_static_expr();

   string msg;
   if (lookahead().is(tok::comma)) {
      advance();
      advance();

      auto StringLit = parse_unary_expr_target();
      if (!isa<StringLiteral>(StringLit))
         diag::err(err_generic_error)
            << "expected string literal as second argument to static_assert"
            << lexer << diag::term;

      msg = move(cast<StringLiteral>(StringLit.get())->getValue());
   }

   lexer->expect(tok::close_paren);

   return makeExpr<StaticAssertStmt>(start, move(expr), move(msg));
}

std::shared_ptr<StaticPrintStmt> Parser::parse_static_print()
{
   auto start = currentTok().getSourceLoc();
   lexer->expect(tok::open_paren);
   advance();

   auto expr = parse_static_expr();
   lexer->expect(tok::close_paren);

   return makeExpr<StaticPrintStmt>(start, move(expr));
}

DeclareStmt::SharedPtr Parser::parse_declare_stmt(ExternKind externKind,
                                            std::shared_ptr<DeclareStmt> decl) {
   auto start = currentTok().getSourceLoc();
   if (lookahead().getKind() == tok::ident) {
      advance();
      auto str = lexer->strVal();

      if (str == "C") {
         externKind = ExternKind::C;
      }
      else if (str == "C++" || str == "CPP") {
         externKind = ExternKind::CPP;
      }
      else if (str == "__native") {
         externKind = ExternKind::Native;
      }
      else {
         ParseError::raise("Unknown extern kind " + str, lexer);
      }
   }

   if (lookahead().is(tok::open_brace)) {
      advance();
      return parse_multiple_declare_stmt(externKind);
   }

   advance();

   if (!decl) {
      decl = makeExpr<DeclareStmt>(start, externKind);
   }

   decl->addDeclaration(parse_next_stmt());
   return decl;
}

DeclareStmt::SharedPtr Parser::parse_multiple_declare_stmt(
                                                        ExternKind externKind) {
   auto start = currentTok().getSourceLoc();
   auto decl = makeExpr<DeclareStmt>(start, externKind);

   while (!lookahead().is(tok::close_brace)
          && currentTok().getKind() != tok::eof) {
      parse_declare_stmt(externKind, decl);
   }

   advance();
   return decl;
}

TryStmt::SharedPtr Parser::parse_try_stmt()
{
   auto start = currentTok().getSourceLoc();
   advance();

   auto tryBody = parse_next_stmt();
   TryStmt::SharedPtr tryStmt = makeExpr<TryStmt>(start, std::move(tryBody));

   while (lookahead().oneOf(tok::kw_catch, tok::kw_finally)) {
      advance();

      auto kind = currentTok().getKind();
      bool finallySet = false;

      if (kind == tok::kw_catch) {
         advance();
         CatchBlock catchBlock;

         if (currentTok().getKind() != tok::ident) {
            ParseError::raise("expected identifier", lexer);
         }

         catchBlock.identifier = lexer->strVal();
         advance();

         if (!currentTok().is(tok::colon)) {
            ParseError::raise("expected ':'", lexer);
         }

         advance();
         catchBlock.caughtType = parse_type();

         advance();
         catchBlock.body = parse_next_stmt();

         tryStmt->addCatch(std::move(catchBlock));
      }
      else {
         if (finallySet) {
            ParseError::raise("finally block already defined", lexer);
         }

         advance();

         auto finallyBody = parse_next_stmt();
         tryStmt->setFinally(std::move(finallyBody));

         finallySet = true;
      }
   }

   return tryStmt;
}

ThrowStmt::SharedPtr Parser::parse_throw_stmt()
{
   auto start = currentTok().getSourceLoc();
   advance();

   return makeExpr<ThrowStmt>(start, parse_expr_sequence());
}

std::shared_ptr<ReturnStmt> Parser::parse_return_stmt()
{
   auto start = currentTok().getSourceLoc();
   Token next = lookahead(false);
   ReturnStmt::SharedPtr ret;

   if (!next.is_separator()) {
      advance();
      ret = makeExpr<ReturnStmt>(start, parse_expr_sequence());
   }
   else {
      ret = makeExpr<ReturnStmt>(start);
   }

   return ret;
}

/**
 * Interprets a keyword statement
 */
Statement::SharedPtr Parser::parse_keyword()
{
   auto start = currentTok().getSourceLoc();

   auto kind = currentTok().getKind();
   tok::TokenType relevantToken = tok::sentinel;

   switch (kind) {
      case tok::kw_self:
      case tok::kw_true:
      case tok::kw_false:
      case tok::kw_none:
         return parse_expr_sequence();
      case tok::kw_var:
      case tok::kw_let:
         return parse_declaration(kind == tok::kw_let);
      case tok::kw_def:
         return parse_function_decl();
      case tok::kw_if:
         return parse_if_stmt();
      case tok::kw_while:
      case tok::kw_loop:
         return parse_while_stmt(kind == tok::kw_while);
      case tok::kw_match:
         return parse_match_stmt();
      case tok::kw_for:
         return parse_for_stmt();
      case tok::kw_continue:
      case tok::kw_break: {
         Statement::SharedPtr stmt;
         if (kind == tok::kw_continue) {
            stmt = makeExpr<ContinueStmt>(start);
         }
         else {
            stmt = makeExpr<BreakStmt>(start);
         }

         return stmt;
      }
      case tok::kw_typedef:
         return parse_typedef();
      case tok::kw_alias:
         return parse_alias();
      case tok::kw_declare:
         return parse_declare_stmt(ExternKind::NONE);
      case tok::kw_return:
         return parse_return_stmt();
      case tok::kw_try:
         return parse_try_stmt();
      case tok::kw_throw:
         return parse_throw_stmt();
      case tok::kw_goto: {
         advance();
         auto gotoStmt = makeExpr<GotoStmt>(start, move(lexer->strRef()));
         return gotoStmt;
      }
      case tok::kw_namespace:
         return parse_namespace_decl();
      case tok::kw_using:
         return parse_using_stmt();
      case tok::kw_module:
         CDOT_DIAGNOSE("module declaration is only allowed at the beginning "
                          "of a file");
         break;
      case tok::kw_import:
         CDOT_DIAGNOSE("import declaration is only allowed at the beginning "
                          "of a file");
         break;
      case tok::kw___debug:
         return makeExpr<DebugStmt>(start, false);
      case tok::kw___unreachable:
         return makeExpr<DebugStmt>(start, true);
      case tok::kw_static_assert:
         return parse_static_assert();
      case tok::kw_static_print:
         return parse_static_print();
      case tok::kw_static_if:
         return parse_static_if();
      case tok::kw_static_for:
         return parse_static_for();
      case tok::kw_struct:
      case tok::kw_enum:
      case tok::kw_class:
      case tok::kw_protocol:
      case tok::kw_union:
      case tok::kw_extend:
         relevantToken = kind;
         LLVM_FALLTHROUGH;
      case tok::kw_public:
      case tok::kw_abstract:
      case tok::kw_private:
         {
            Lexer<>::StateSaveGuard guard(lexer);
            while (relevantToken == tok::sentinel) {
               if (currentTok().oneOf(tok::kw_struct, tok::kw_enum,
                                             tok::kw_class, tok::kw_union,
                                             tok::kw_protocol, tok::kw_let,
                                             tok::kw_var, tok::kw_def,
                                             tok::kw_typedef, tok::kw_extend)) {
                  relevantToken = currentTok().getKind();
                  break;
               }

               advance();
            }
         }
         switch (relevantToken) {
            case tok::kw_struct:
               return parse_struct_decl();
            case tok::kw_class:
               return parse_class_decl();
            case tok::kw_enum:
               return parse_enum_decl();
            case tok::kw_union:
               return parse_union_decl();
            case tok::kw_protocol:
               return parse_class_decl(false, true);
            case tok::kw_extend:
               return parse_extend_stmt();
            case tok::kw_typedef:
               return parse_typedef();
            case tok::kw_var:
            case tok::kw_let:
               return parse_declaration(relevantToken == tok::kw_let);
            case tok::kw_def:
               return parse_function_decl();
            default:
               break;
         }
         LLVM_FALLTHROUGH;
      default:
         ParseError::raise("unexpected keyword "
                            + currentTok().toString(), lexer);
   }

   llvm_unreachable("bad keyword");
}

CallExpr::SharedPtr Parser::parse_function_call(bool allowLet)
{
   auto start = currentTok().getSourceLoc();

   BuiltinFn Builtin = BuiltinFn::None;

   string funcName;
   if (currentTok().is(tok::kw_init)) {
      funcName = "init";
   }
   else if (currentTok().is(tok::kw_deinit)) {
      funcName = "deinit";
   }
   else if (Builtin == BuiltinFn::None) {
      funcName = move(lexer->strRef());
      Builtin = llvm::StringSwitch<BuiltinFn>(funcName)
         .Case("sizeof", BuiltinFn::SIZEOF)
         .Case("alignof", BuiltinFn::ALIGNOF)
         .Case("typeof", BuiltinFn::TYPEOF)
         .Case("default", BuiltinFn::DefaultVal)
         .Case("__builtin_sizeof", BuiltinFn::BuiltinSizeof)
         .Case("__builtin_isnull", BuiltinFn::ISNULL)
         .Case("__builtin_memcpy", BuiltinFn::MEMCPY)
         .Case("__builtin_memset", BuiltinFn::MEMSET)
         .Case("__builtin_memcmp", BuiltinFn::MemCmp)
         .Case("__nullptr", BuiltinFn::NULLPTR)
         .Case("__builtin_bitcast", BuiltinFn::BITCAST)
         .Case("stackalloc", BuiltinFn::STACK_ALLOC)
         .Case("__ctfe_stacktrace", BuiltinFn::CtfePrintStackTrace)
         .Default(BuiltinFn::None);
   }

   advance();

   bool isVariadicSizeof = false;
   if (Builtin == BuiltinFn::SIZEOF && currentTok().is(tok::triple_period)) {
      isVariadicSizeof = true;
      advance();
   }

   auto generics = parse_unresolved_template_args();
   if (!generics.empty()) {
      advance();
   }

   auto args = parse_arguments(allowLet);

   auto call = makeExpr<CallExpr>(start, move(args.args), move(funcName));

   call->setBuiltinFnKind(Builtin);
   call->setMemberExpr(try_parse_member_expr());
   call->setTemplateArgs(std::move(generics));

   if (isVariadicSizeof)
      call->setKind(CallKind::VariadicSizeof);

   return call;
}

std::shared_ptr<EnumCaseExpr> Parser::parse_enum_case_expr()
{
   auto start = currentTok().getSourceLoc();
   auto ident = move(lexer->strRef());

   std::shared_ptr<EnumCaseExpr> expr;
   if (lookahead().is(tok::open_paren)) {
      advance();

      expr = makeExpr<EnumCaseExpr>(start, move(ident), parse_arguments().args);
   }
   else {
      expr = makeExpr<EnumCaseExpr>(start, move(ident));
   }

   return expr;
}

Parser::ArgumentList Parser::parse_arguments(bool allowLet)
{
   ArgumentList args;
   bool isLabeled = false;

   while (!currentTok().is(tok::close_paren)) {
      string label;

      if (currentTok().is(tok::open_paren)
          || currentTok().is(tok::comma)) {
         advance();
         if (currentTok().getKind() == tok::ident
             && lookahead().is(tok::colon)) {
            label = lexer->strVal();
            advance();
            advance();

            isLabeled = true;
         }
         else if (currentTok().is(tok::close_paren)) {
            break;
         }
         else if (isLabeled) {
            ParseError::raise("Labeled arguments have to come last in a call",
                              lexer);
         }
      }

      bool isVar = false;
      bool isLet = false;
      if (currentTok().is(tok::kw_let) && allowLet) {
         isLet = true;
         advance();
      }
      else if (currentTok().is(tok::kw_var) && allowLet) {
         isVar = true;
         advance();
      }

      auto argVal = parse_expr_sequence();
      if ((isLet || isVar) && !isa<IdentifierRefExpr>(argVal)) {
         ParseError::raise("Expected identifier after 'let' / 'var'", lexer);
      }
      else if (isLet) {
         std::static_pointer_cast<IdentifierRefExpr>(argVal)->isLetExpr(true);
      }
      else if (isVar) {
         std::static_pointer_cast<IdentifierRefExpr>(argVal)->isVarExpr(true);
      }

      args.labels.emplace_back(move(label));
      args.args.emplace_back(move(argVal));

      lexer->expect(tok::comma, tok::close_paren);
   }

   return args;
}

std::vector<Attribute> Parser::parse_attributes()
{
   std::vector<Attribute> attributes;
   std::vector<string> foundAttrs;

   while (currentTok().is(tok::at)) {
      advance();
      if (currentTok().getKind() != tok::ident) {
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

      if (lookahead().is(tok::open_paren)) {
         advance();
         advance();

         while (!currentTok().is(tok::close_paren)
                && currentTok().getKind() != tok::eof) {
            attr.args.push_back(lexer->parseExpression({}, 0, true));
            if (lookahead().is(tok::comma)) {
               advance();
            }

            advance();
         }
      }

      auto check = isValidAttribute(attr);
      if (!check.empty()) {
         ParseError::raise(check, lexer);
      }

      attributes.push_back(attr);
      foundAttrs.push_back(attr.name);
      advance();
   }

   return attributes;
}

CompoundStmt::SharedPtr Parser::parse_block(bool preserveTopLevel)
{
   auto start = currentTok().getSourceLoc();
   bool last_top_level = top_level;

   if (!preserveTopLevel) {
      top_level = false;
   }

   advance();

   if (!(currentTok().is(tok::open_brace))) {
      ParseError::raise("Expected '{' to start a block statement.", lexer);
   }

   advance();

   CompoundStmt::SharedPtr block = makeExpr<CompoundStmt>(start);
   while (!currentTok().is(tok::close_brace)) {
      while (currentTok().oneOf(tok::semicolon, tok::newline)) {
         advance();
      }

      if (currentTok().getKind() == tok::eof
          || currentTok().is(tok::close_brace)) {
         break;
      }

      Statement::SharedPtr stmt = parse_next_stmt();

      if (CurrentFuncDecls != nullptr
          && (isa<ClassDecl>(stmt) || isa<UnionDecl>(stmt)
              || isa<EnumDecl>(stmt))) {
         CurrentFuncDecls->push_back(stmt);
      }
      else {
         block->addStatement(move(stmt));
      }

      advance();
   }

   if (!currentTok().is(tok::close_brace)) {
      ParseError::raise("Expected '}' to end a block statement", lexer);
   }

   top_level = last_top_level;

   return block;
}

void Parser::skip_block()
{
   auto openedBraces = 1;
   auto closedBraces = 0;

   if (!currentTok().is(tok::open_brace)) {
      ParseError::raise("expected '{'", lexer);
   }

   Lexer<>::IgnoreScope guard(lexer);

   while (openedBraces > closedBraces) {
      advance();

      if (currentTok().is(tok::open_brace)) {
         ++openedBraces;
      }
      else if (currentTok().is(tok::close_brace)) {
         ++closedBraces;
      }
   }
}

Statement::SharedPtr Parser::parse_next_stmt()
{
   auto attrs = parse_attributes();
   std::shared_ptr<Statement> stmt;

   if (currentTok().is(tok::open_brace)) {
      lexer->backtrack();
      stmt = parse_block();
   }
   else if (currentTok().is_keyword()) {
      stmt = parse_keyword();
   }
   else if (currentTok().getKind() == tok::ident
            && lookahead().is(tok::colon)) {
      string label = std::move(lexer->strRef());
      advance();

      auto label_stmt = makeExpr<LabelStmt>(currentTok().getSourceLoc(),
                                            move(label));

      stmt = label_stmt;
   }
   else {
      stmt = parse_expr_sequence();
   }

   stmt->setAttributes(move(attrs));
   return stmt;
}

/**
 * Parses the module declaration for the current file
 * @return
 */
NamespaceDecl::SharedPtr Parser::parse_namespace_decl()
{
   auto start = currentTok().getSourceLoc();
   bool anonymous = false;
   string nsName;

   if (lookahead().getKind() != tok::ident) {
      anonymous = true;
      nsName = util::nextAnonymousNamespace();
   }
   else {
      advance();
      nsName = lexer->strVal();
   }

   while (lookahead().is(tok::period)) {
      advance();
      advance();

      if (currentTok().getKind() != tok::ident) {
         ParseError::raise("Expected identifier after 'namespace'", lexer);
      }

      nsName += "." + lexer->strVal();
   }

   decl->pushNamespace(nsName, anonymous);

   auto mod = makeExpr<NamespaceDecl>(start, move(nsName),
                                      parse_block(true), anonymous);

   popNamespace();
   return mod;
}

/**
 * Parses a module import statement
 * @return
 */
UsingStmt::SharedPtr Parser::parse_using_stmt()
{
   auto start = currentTok().getSourceLoc();
   advance();

   string importNs;
   std::vector<string> items;

   while (currentTok().oneOf(tok::ident, tok::times)) {
      if (lookahead().is(tok::period)) {
         if (!importNs.empty()) {
            importNs += ".";
         }

         importNs += lexer->strVal();

         advance();
         if (lookahead().is(tok::open_brace)) {
            advance();
            advance();

            while (!currentTok().is(tok::close_brace)) {
               items.push_back(lexer->strVal());
               advance();
               if (currentTok().is(tok::comma)) {
                  advance();
               }
            }

            break;
         }

         advance();
      }
      else {
         items.push_back(currentTok().is(tok::ident) ? lexer->strVal()
                                                     : "*");
         break;
      }
   }

   if (importNs.empty()) {
      importNs = items.back();
      items.pop_back();
   }

   decl->importNamespace(importNs);

   auto stmt = makeExpr<UsingStmt>(start, std::move(importNs),
                                   std::move(items));

   decl->addGlobalStatement(stmt.get());

   return stmt;
}

std::shared_ptr<ModuleStmt> Parser::parse_module_stmt()
{
   auto start = currentTok().getSourceLoc();

   assert(currentTok().is(tok::kw_module));
   lexer->expect(tok::ident);

   std::vector<string> moduleName;

   while (1) {
      moduleName.emplace_back(std::move(lexer->strRef()));

      if (lookahead().is(tok::period)) {
         advance();
         lexer->expect(tok::ident);
      }
      else {
         break;
      }
   }

   return makeExpr<ModuleStmt>(start, move(moduleName));
}

std::shared_ptr<ImportStmt> Parser::parse_import_stmt()
{
   auto start = currentTok().getSourceLoc();

   assert(currentTok().is(tok::kw_import));
   lexer->expect(tok::ident);

   std::vector<string> moduleName;

   while (1) {
      moduleName.emplace_back(std::move(lexer->strRef()));

      if (lookahead().is(tok::period)) {
         advance();
         lexer->expect(tok::ident);
      }
      else {
         break;
      }
   }

   return makeExpr<ImportStmt>(start, move(moduleName));
}

CompoundStmt::SharedPtr Parser::parse()
{
   auto root = makeExpr<CompoundStmt>(currentTok().getSourceLoc());
   while (currentTok().oneOf(tok::newline, tok::semicolon)) {
      advance();
   }

   if (currentTok().is(tok::kw_module)) {
      root->addStatement(parse_module_stmt());
      advance();
   }

   while (currentTok().is(tok::kw_import)) {
      root->addStatement(parse_import_stmt());
      advance();
   }

   while(currentTok().getKind() != tok::eof) {
      while (currentTok().oneOf(tok::newline, tok::semicolon)) {
         advance();
      }

      if (currentTok().getKind() == tok::eof) {
         break;
      }

      Statement::SharedPtr stmt = parse_next_stmt();
      if (stmt != nullptr) {
         root->addStatement(move(stmt));
      }

      advance();
   }

   return root;
}

} // namespace parse
} // namespace cdot