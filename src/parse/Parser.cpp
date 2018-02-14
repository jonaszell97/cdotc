//
// Created by Jonas Zell on 13.06.17.
//

#include "Parser.h"

#include "module/Module.h"
#include "lex/Lexer.h"

#include "Basic/IdentifierInfo.h"
#include "Variant/Variant.h"

#include "AST/Passes/Declaration/DeclPass.h"
#include "AST/Passes/ASTIncludes.h"

#include "AST/Passes/SemanticAnalysis/SemaPass.h"
#include "AST/Passes/SemanticAnalysis/Builtin.h"
#include "AST/Traverse.h"

#include "Files/FileManager.h"
#include "Message/Diagnostics.h"

#include "Support/Casting.h"

#include <cassert>
#include <vector>
#include <string>
#include <llvm/ADT/StringSwitch.h>
#include <llvm/ADT/SmallString.h>
#include <Support/Format.h>

using namespace cdot::diag;
using namespace cdot::support;
using namespace cdot::lex;

namespace cdot {
namespace parse {

Parser::Parser(ASTContext const& Context,
               lex::Lexer *lexer,
               SemaPass &SP,
               bool isModuleParser)
   : Context(Context),
     source_id(lexer->getSourceId()),
     isModuleParser(isModuleParser),
     lexer(lexer),
     SP(SP)
{
   lexer->lex();
}

Parser::~Parser() = default;

Parser::DeclContextRAII::DeclContextRAII(Parser &P,
                                         DeclContext *Ctx) : P(P)
{
   Ctx->setParentCtx(&P.SP.getDeclPass()->getDeclContext());
   P.SP.getDeclPass()->pushDeclContext(Ctx);
}

Parser::DeclContextRAII::~DeclContextRAII()
{
   P.SP.getDeclPass()->popDeclContext();
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

AccessModifier Parser::tokenToAccessSpec(tok::TokenType kind)
{
   switch (kind) {
      case tok::kw_public:
         return AccessModifier ::PUBLIC;
      case tok::kw_protected:
         return AccessModifier ::PROTECTED;
      case tok::kw_private:
         return AccessModifier ::PRIVATE;
      default:
         llvm_unreachable("not an access specifier");
   }
}

void Parser::skipUntilProbableEndOfStmt()
{
   while (!currentTok().oneOf(tok::newline, tok::semicolon, tok::eof))
      advance(false);
}

void Parser::skipUntilProbableEndOfStmt(cdot::lex::tok::TokenType kind)
{
   while (!currentTok().is(kind))
      advance(false);
}

namespace {

void markAllowUnexpandedParams(TypeRef *ty)
{
   ty->setAllowUnexpandedTemplateArgs(true);
}

} // anonymous namespace

TypeRef* Parser::parse_type(bool allowVariadic)
{
   auto start = currentTok().getSourceLoc();

   bool isReference = false;
   if (currentTok().is(tok::kw_ref)) {
      advance();
      isReference = true;
   }

   bool globalLookup = false;
   if (currentTok().is(tok::period)) {
      advance();
      globalLookup = true;
   }

   auto attrs = parse_attributes();
   auto typeref = parse_type_impl();

   // pointer type
   auto next = lookahead();
   while (1) {
      if (next.oneOf(tok::times, tok::times_times) || next.is_identifier()) {
         if (next.is(tok::times)) {
            advance();
            typeref = makeExpr<TypeRef>(start, typeref, TypeRef::Pointer);
            break;
         }
         if (next.is(tok::times_times)) {
            advance();
            typeref = makeExpr<TypeRef>(start, typeref, TypeRef::Pointer);
            typeref = makeExpr<TypeRef>(start, typeref, TypeRef::Pointer);
            break;
         }

         auto op = next.getIdentifierInfo()->getIdentifier();
         bool onlyStars = std::count(op.begin(), op.end(), '*') == op.size();

         if (onlyStars) {
            advance();
            for (size_t i = 0; i < op.size(); ++i)
               typeref = makeExpr<TypeRef>(start, typeref, TypeRef::Pointer);
         }
      }
      // optional type
      else if (next.is(tok::question)) {
         advance();
         typeref = makeExpr<TypeRef>(start, typeref, TypeRef::Option);
      }
      // array type
      else if (next.is(tok::open_square)) {
         advance();
         advance();

         auto arraySize = new (Context) StaticExpr(parse_static_expr());
         lexer->expect(tok::close_square);

         typeref = makeExpr<TypeRef>(start, typeref, arraySize);
      }
      else {
         break;
      }

      next = lookahead();
   }

   if (typeref) {
      typeref->isReference(isReference);
      typeref->setGlobalLookup(globalLookup);
      typeref->setAttributes(std::move(attrs));
   }

   if (typeref->isVariadicArgPackExpansion()) {
      if (!allowVariadic) {
         err(err_generic_error)
            << "variadic expansion is not allowed here"
            << lexer << diag::term;
      }

      typeref->forEachContainedType(markAllowUnexpandedParams);
   }

   return typeref;
}

TypeRef* Parser::parse_type_impl()
{
   auto start = currentTok().getSourceLoc();

   if (currentTok().isIdentifier("decltype")) {
      lexer->expect(tok::open_paren);
      advance();

      auto expr = parse_expr_sequence();
      lexer->expect(tok::close_paren);

      return makeExpr<TypeRef>(start, expr);
   }

   std::vector<pair<string, std::vector<TemplateArgExpr*>>> ns;

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
            err(err_generic_error)
               << "Expected ']' after dictionary type" << diag::term;
         }

         std::vector<TemplateArgExpr*> templateArgs;
         templateArgs.emplace_back(makeExpr<TemplateArgExpr>(
            elType->getSourceLoc(), elType));

         templateArgs.emplace_back(makeExpr<TemplateArgExpr>(
            valType->getSourceLoc(), valType));

         ns.emplace_back("Dictionary", move(templateArgs));

         return makeExpr<TypeRef>(start, std::move(ns));
      }

      if (!currentTok().is(tok::close_square)) {
         err(err_generic_error)
            << "Expected ']' after array type" << diag::term;
      }

      std::vector<TemplateArgExpr*> templateArgs;
      templateArgs.emplace_back(makeExpr<TemplateArgExpr>(
         elType->getSourceLoc(), elType));

      ns.emplace_back("Array", std::move(templateArgs));
      return makeExpr<TypeRef>(start, std::move(ns));
   }

   // function or tuple type
   if (currentTok().is(tok::open_paren)) {
      auto argTypes = parse_tuple_type();

      // tuple
      if (!lookahead().is(tok::arrow_single)) {
         return makeExpr<TypeRef>(start, std::move(argTypes));
      }

      advance();
      advance();

      auto returnType = parse_type();
      return makeExpr<TypeRef>(start, returnType, std::move(argTypes));
   }


   std::vector<pair<string, TypeRef*>> containedTypes;

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
         err(err_generic_error)
            << "Unexpected character in type reference"
            << currentTok().getSourceLoc()
            << diag::term;

         return nullptr;
      }

      string subName = lexer->getCurrentIdentifier();
      std::vector<TemplateArgExpr*> subTemplateArgs;

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
         err(err_generic_error)
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

NamedDecl* Parser::parse_typedef(AccessModifier am, bool inRecord)
{
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

   auto alias = lexer->getCurrentIdentifier();

   auto params = try_parse_template_parameters();

   auto td = makeExpr<TypedefDecl>(start, am, alias, originTy);
   td->setTemplateParams(move(params));

   if (!inRecord)
      SP.getDeclPass()->DeclareTypedef(td);

   return td;
}

NamedDecl* Parser::parse_alias(bool inRecord)
{
   auto start = currentTok().getSourceLoc();
   lexer->advanceIf(tok::kw_alias);

   string name;
   if (!currentTok().is(tok::ident)) {
      err(err_generic_error)
         << "expected identifier"
         << diag::term;
   }
   else {
      name = lexer->getCurrentIdentifier();
   }

   auto params = try_parse_template_parameters();

   std::vector<StaticExpr*> constraints;
   while (lookahead().is(tok::kw_where)) {
      advance();
      advance();

      constraints.push_back(new (Context) StaticExpr(parse_static_expr()));
   }

   lexer->expect(tok::equals);
   advance();

   auto aliasExpr = new (Context) StaticExpr(parse_static_expr());
   auto aliasDecl = makeExpr<AliasDecl>(start, move(name), move(constraints),
                                        aliasExpr);

   aliasDecl->setTemplateParams(move(params));

   if (!inRecord)
      SP.getDeclPass()->DeclareAlias(aliasDecl);

   return aliasDecl;
}

namespace {

bool identifierTemplateDecider(const Token &t)
{
   return t.oneOf(tok::newline, tok::close_paren, tok::comma, tok::period,
                  tok::close_square, tok::semicolon, tok::open_brace,
                  tok::colon)
          || t.is_operator();
}

} // anonymous namespace

Expression* Parser::parse_identifier()
{
   auto start = currentTok().getSourceLoc();
   string ident;

   switch (currentTok().getKind()) {
      case tok::ident:
         ident = lexer->getCurrentIdentifier();
         break;
      case tok::kw_self:
         ident = "self";
         break;
      case tok::kw_super:
         ident = "super";
         break;
      default:
         err(err_generic_error) << "expected identifier"
                                      << lexer << diag::term;
   }

   auto expr = makeExpr<IdentifierRefExpr>(start, std::move(ident));

   if (_is_generic_any(identifierTemplateDecider)) {
      advance();
      expr->setTemplateArgs(parse_unresolved_template_args());
   }

   expr->setSubExpr(try_parse_member_expr());

   return expr;
}

Expression* Parser::try_parse_member_expr()
{
   auto start = currentTok().getSourceLoc();
   advance(false);

   // member access
   bool pointerAccess = currentTok().is(tok::arrow_single);
   if (currentTok().is(tok::period) || pointerAccess) {
      advance(false);

      // tuple access
      if (currentTok().is(tok::integerliteral)) {
         size_t index = std::stoull(currentTok().getText());

         auto memberExpr = makeExpr<MemberRefExpr>(start, index, pointerAccess);
         memberExpr->setSubExpr(try_parse_member_expr());

         return memberExpr;
      }

      // method call
      Token next = lookahead();
      if (next.is(tok::open_paren) || is_generic_call()) {
         auto call = parse_function_call();
         call->isPointerAccess(pointerAccess);

         return call;
      }

      string ident = lexer->getCurrentIdentifier();

      auto mem_ref = makeExpr<MemberRefExpr>(start, std::move(ident),
                                             pointerAccess);

      if (_is_generic_any(identifierTemplateDecider)) {
         advance();
         mem_ref->setTemplateArgs(parse_unresolved_template_args());
      }

      mem_ref->setSubExpr(try_parse_member_expr());

      return mem_ref;
   }

   // call
   if (currentTok().is(tok::open_paren)) {
      auto args = parse_arguments();
      auto call = makeExpr<CallExpr>(start, std::move(args.args));

      call->setSubExpr(try_parse_member_expr());

      return call;
   }

   // subscript
   if (currentTok().is(tok::open_square)) {
      advance();

      std::vector<Expression*> indices;
      while (!currentTok().is(tok::close_square)) {
         indices.push_back(parse_expr_sequence());
         advance();

         if (currentTok().is(tok::comma))
            advance();
      }

      auto* subscriptExpr = makeExpr<SubscriptExpr>(start, move(indices));
      subscriptExpr->setSubExpr(try_parse_member_expr());

      return subscriptExpr;
   }

   lexer->backtrack();
   return nullptr;
}

Expression* Parser::parse_collection_literal()
{
   auto start = currentTok().getSourceLoc();

   bool isDictionary = false;
   bool first = true;
   std::vector<Expression*> keys;
   std::vector<Expression*> values;

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
            err(err_generic_error)
               << "Unexpected token ':'" << diag::term;
         }

         advance();
         auto value = parse_expr_sequence();
         keys.push_back(key);
         values.push_back(value);

         isDictionary = true;
      }
      else {
         if (isDictionary) {
            err(err_generic_error) << "Expected ':'" << diag::term;
         }

         values.push_back(key);
      }

      first = false;
   }

   if (!currentTok().is(tok::close_square)) {
      err(err_generic_error)
         << "Expected ']' after array literal" << diag::term;
   }

   if (isDictionary)
      return makeExpr<DictionaryLiteral>(start, std::move(keys),
                                         std::move(values));

   return makeExpr<ArrayLiteral>(start, std::move(values));
}

static string prepareStringLiteral(Token const& tok)
{
   llvm::SmallString<128> str;
   bool escaped = false;

   for (auto &c : tok.getText()) {
      if (escaped) {
         str += support::escape_char(c);
         escaped = false;
      }
      else if (c == '\\') {
         escaped = true;
      }
      else {
         str += c;
      }
   }

   return str.str();
}

Expression* Parser::parse_unary_expr_target()
{
   std::vector<Attribute> attributes;

   if (currentTok().is(tok::at)) {
      attributes = parse_attributes();
   }

   if (currentTok().isIdentifier("__traits")) {
      auto expr = parse_traits_expr();
      expr->setAttributes(move(attributes));

      return expr;
   }

   if (currentTok().is(tok::kw_static)) {
      lexer->expect(tok::open_paren);
      advance();

      auto expr = new (Context) StaticExpr(parse_static_expr());

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
      bool isVariadicSizeof = currentTok().isIdentifier("sizeof")
                              && lookahead().is(tok::triple_period);
      if (isVariadicSizeof || next.is(tok::open_paren) || is_generic_call()) {
         return parse_function_call();
      }

      // single argument lambda
      if (next.is(tok::arrow_double)) {
         auto argName = lexer->getCurrentIdentifier();
         auto arg = makeExpr<FuncArgDecl>(start, argName,
                                          makeExpr<TypeRef>(start), nullptr,
                                          false, true, false);

         start = currentTok().getSourceLoc();

         advance();
         advance();

         auto body = parse_next_stmt();
         auto lambda = makeExpr<LambdaExpr>(start, makeExpr<TypeRef>(start),
            std::vector<FuncArgDecl*>{ arg }, body);

         lambda->setSubExpr(try_parse_member_expr());

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
      auto expr = makeExpr<BoolLiteral>(start, Context.getBoolTy(),
                                        currentTok().is(tok::kw_true));
      expr->setAttributes(move(attributes));

      return expr;
   }

   if (currentTok().oneOf(tok::integerliteral, tok::fpliteral,
                          tok::charliteral)) {
      auto expr = parse_numeric_literal();

      expr->setSubExpr(try_parse_member_expr());
      expr->setAttributes(std::move(attributes));

      return expr;
   }

   if (currentTok().is(tok::stringliteral)) {
      auto strLiteral = makeExpr<StringLiteral>(
         start, prepareStringLiteral(currentTok()));

      strLiteral->setAttributes(std::move(attributes));

      if (!lookahead().is(tok::sentinel)) {
         // concatenate adjacent string literals
         if (lookahead().is(tok::stringliteral)) {
            string s = strLiteral->getValue();
            while (lookahead().is(tok::stringliteral)) {
               advance();
               s += prepareStringLiteral(currentTok());
            }

            strLiteral = makeExpr<StringLiteral>(start, move(s));
         }

         strLiteral->setSubExpr(try_parse_member_expr());
         return strLiteral;
      }

      std::vector<Expression*> strings{ strLiteral };
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
            auto lit = makeExpr<StringLiteral>(
               start, prepareStringLiteral(currentTok()));

            strings.push_back(lit);

            if (!lookahead().is(tok::sentinel)) {
               break;
            }
            else {
               advance();
            }
         }
      }

      auto interp = makeExpr<StringInterpolation>(start, move(strings));
      interp->setSubExpr(try_parse_member_expr());

      return interp;
   }

   if (currentTok().is(tok::preprocessor_value)) {
      auto &V = currentTok().getPreprocessorValue();
      if (V.isInt()) {
         auto I = makeExpr<IntegerLiteral>(start, Context.getIntTy(),
                                           llvm::APSInt(V.getAPSInt()));

         I->setSubExpr(try_parse_member_expr());
         return I;
      }

      if (V.isFloat()) {
         auto F = makeExpr<FPLiteral>(start, Context.getDoubleTy(),
                                      llvm::APFloat(V.getAPFloat()));

         F->setSubExpr(try_parse_member_expr());
         return F;
      }

      if (V.isStr()) {
         auto S = makeExpr<StringLiteral>(start, string(V.getString()));
         S->setSubExpr(try_parse_member_expr());

         return S;
      }

      llvm_unreachable("bad preprocessor value kind");
   }

   err(err_generic_error)
      << "expected expression but found " +
         currentTok().rawRepr()
      << currentTok().getSourceLoc()
      << diag::term;

   return nullptr;
}

bool Parser::modifierFollows(char c)
{
   auto next = lookahead(false, true);
   if (!next.is(tok::ident))
      return false;

   auto I = next.getIdentifierInfo()->getIdentifier();
   if (I.size() == 1 && (I.front() == c || I.front() == ::toupper(c))) {
      advance(false, true);
      return true;
   }

   return false;
}

Expression* Parser::parse_numeric_literal()
{
   auto start = currentTok().getSourceLoc();
   auto str = currentTok().getText();
   auto kind = currentTok().getKind();

   if (kind == tok::charliteral) {
      assert(str.size() == 1);
      return makeExpr<CharLiteral>(start,
                                   Context.getCharTy(),
                                   str.front());
   }

   if (kind == tok::fpliteral) {
      llvm::APFloat APF(0.0);
      auto res = APF.convertFromString(str, llvm::APFloat::rmNearestTiesToEven);
      switch (res) {
         default:
            break;
         case llvm::APFloat::opInexact:
            warn(warn_generic_warn)
               << "floating point value cannot be represented exactly"
               << currentTok().getSourceLoc()
               << diag::cont;

            break;
      }

      cdot::Type *Ty;
      if (modifierFollows('f')) {
         Ty = Context.getFloatTy();
      }
      else {
         // advance past 'd' modifier
         (void)modifierFollows('d');
         Ty = Context.getDoubleTy();
      }

      return makeExpr<FPLiteral>(start, Ty, std::move(APF));
   }

   auto Int = currentTok().getIntegerValue();
   cdot::Type *Ty;

   if (modifierFollows('u')) {
      Ty = Context.getUIntTy();
   }
   else {
      Ty = Context.getIntTy();
   }

   return makeExpr<IntegerLiteral>(start, Ty, std::move(Int));
}

Expression* Parser::parse_unary_expr(UnaryOperator* literal,
                                     bool postfix) {
   auto start = currentTok().getSourceLoc();

   // prefix unary op
   if (currentTok().is_operator()) {
      auto op = currentTok().toString();
      advance();

      auto expr = parse_unary_expr();
      auto unaryOp = makeExpr<UnaryOperator>(start, move(op),
                                             expr, /*prefix=*/true);

      return unaryOp;
   }

   Expression* expr;

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

      auto op = currentTok().toString();
      auto unaryOp = makeExpr<UnaryOperator>(start, move(op),
                                             expr, /*prefix=*/false);

      unaryOp->setSubExpr(try_parse_member_expr());

      return unaryOp;
   }

   // call
   if (next.is(tok::open_paren) || is_generic_call()) {
      advance();

      auto generics = parse_unresolved_template_args();

      auto call = makeExpr<CallExpr>(start, parse_arguments().args);

      call->setTemplateArgs(std::move(generics));
      expr->setSubExpr(call);
   }

   return expr;
}

TertiaryOperator*
Parser::parse_tertiary_operator(Expression* cond)
{
   auto start = currentTok().getSourceLoc();

   advance();
   auto lhs = parse_static_expr(false, true);

   advance();
   if (!currentTok().is(tok::colon)) {
      err(err_generic_error)
         << "expected ':' in tertiary expression" << diag::term;
   }

   advance();
   auto rhs = parse_expr_sequence();

   return makeExpr<TertiaryOperator>(start, cond, lhs, rhs);
}

ParenExprType Parser::get_paren_expr_type()
{
   int open_parens = 1;
   int closed_parens = 0;

   auto isLambda = false;
   bool isTuple = false;
   bool maybeFuncTy = false;

   Lexer::StateSaveGuard guard(lexer);
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
            err(err_generic_error)
               << "unexpected end of file, expecting ')'"
               << lexer->getTokens()[lexer->getTokens().size() - 2]
                  .getSourceLoc()
               << diag::term;

            note(note_generic_note)
               << "to match this" << begin << diag::end;
            
         default:
            break;
      }
   }

   if (open_parens != closed_parens) {
      err(err_generic_error)
         << "Expression contains unmatched parentheses" << diag::term;
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
   Lexer::StateSaveGuard guard(lexer);
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

Expression* Parser::parse_paren_expr()
{
   ParenExprType type = get_paren_expr_type();
   Expression* expr;
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
            err(err_generic_error) << "Expected ')'" << diag::term;
         }

         expr->setSubExpr(try_parse_member_expr());

         break;
      case ParenExprType::TUPLE:
         expr = parse_tuple_literal();
         break;
   }

   return expr;
}

TupleLiteral* Parser::parse_tuple_literal()
{
   auto start = currentTok().getSourceLoc();
   std::vector<pair<string, Expression*>> elements;

   advance();

   while (!currentTok().is(tok::close_paren)) {
      std::vector<Attribute> attributes = parse_attributes();
      string label;

      if (currentTok().getKind() == tok::ident
          && lookahead().is(tok::colon)) {
         label = lexer->getCurrentIdentifier();
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
         err(err_generic_error) << "Expected ',' or ')'" << diag::term;
      }
   }

   if (!currentTok().is(tok::close_paren)) {
      err(err_generic_error)
         << "expected ')" << diag::term;
   }

   auto tup = makeExpr<TupleLiteral>(start, move(elements));
   tup->setSubExpr(try_parse_member_expr());

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

} // anonymous namespace

Expression* Parser::parse_expr_sequence(bool parenthesized,
                                        bool ignoreColon) {
   if (StaticExprStack)
      return parse_static_expr();

   auto start = currentTok().getSourceLoc();
   bool foundQuestion = false;

   std::vector<ExprSequence::SequenceElement> frags;
   for (;;) {
      if (currentTok().oneOf(tok::ident, tok::kw_self, tok::kw_super)) {
         auto expr = parse_unary_expr_target();
         auto ident = dyn_cast<IdentifierRefExpr>(expr);

         if (ident && !ident->getSubExpr()
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
            err(err_generic_error)
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
      else if (currentTok().is(tok::colon)) {
         frags.emplace_back(":", currentTok().getSourceLoc());
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
      Expression* singleExpr;
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
      err(err_generic_error)
         << "unexpected token " + currentTok().toString()
         << lexer << diag::term;
   }

   return makeExpr<ExprSequence>(start, move(frags), parenthesized);
}

Expression* Parser::parse_static_expr(bool ignoreGreater,
                                      bool ignoreColon,
                                      Expression* lhs,
                                      int minPrecedence) {
   return parse_expr_sequence();
   ++StaticExprStack;

   if (lhs == nullptr) {
      lhs = parse_unary_expr();
   }

   Token next = lookahead(false);
   auto prec = getOperatorPrecedence(next.getKind());
   while (prec != prec::Unknown && prec >= minPrecedence) {
      if (next.is(tok::greater) && ignoreGreater)
         break;
      if (next.is(tok::colon) && ignoreColon)
         break;

      Expression* rhs;
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

      auto nextPrec = getOperatorPrecedence(next.getKind());
      while (nextPrec != prec::Unknown && nextPrec > prec) {
         rhs = parse_static_expr(ignoreGreater, ignoreColon, rhs, nextPrec);

         next = lookahead(false);
         prec = nextPrec;
         nextPrec = getOperatorPrecedence(next.getKind());
      }

      lhs = makeExpr<BinaryOperator>(operatorLoc, move(op), lhs, rhs);
      prec = getOperatorPrecedence(next.getKind());
   }

   // tertiary operator
   if (next.is(tok::question)) {
      advance(false);
      lhs = parse_tertiary_operator(lhs);
   }

   --StaticExprStack;

   return lhs;
}

ConstraintExpr* Parser::parse_constraint_expr()
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
         if (currentTok().isIdentifier("default")) {
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

TraitsExpr* Parser::parse_traits_expr()
{
   auto prev = StaticExprStack;
   StaticExprStack = 0;

   auto start = currentTok().getSourceLoc();

   lexer->expect(tok::open_paren);
   lexer->expect(tok::ident);

   auto str = currentTok().getIdentifierInfo()->getIdentifier();
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
      err(err_generic_error)
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
               err(err_generic_error)
                  << "expected identifier" << lexer << diag::term;

            args.emplace_back(currentTok().getIdentifierInfo()
                                          ->getIdentifier());
            break;
         case TraitArguments::String:
            if (!currentTok().is(tok::stringliteral))
               err(err_generic_error)
                  << "expected string literal" << lexer << diag::term;

            args.emplace_back(currentTok().getText());
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

Statement* Parser::parse_var_decl()
{
   AccessModifier access = AccessModifier::DEFAULT;
   bool isLet = false;
   auto loc = currentTok().getSourceLoc();

   while (currentTok().is_keyword()) {
      switch (currentTok().getKind()) {
         case tok::kw_public:
         case tok::kw_private:
            if (access != AccessModifier::DEFAULT)
               err(err_generic_error)
                  << "duplicate access specifier"
                  << currentTok().getSourceLoc() << diag::term;

            access = tokenToAccessSpec(currentTok().getKind());
            break;
         case tok::kw_let:
            isLet = true;
            LLVM_FALLTHROUGH;
         case tok::kw_var:
            break;
         default:
            err(err_generic_error)
               << "unexpected keyword in variable declaration: "
                  + currentTok().toString()
               << currentTok().getSourceLoc() << diag::term;
            break;
      }

      advance();
   }

   if (currentTok().is(tok::open_paren))
      return parse_destructuring_decl(access, isLet);

   if (!currentTok().is(tok::ident)) {
      err(err_generic_error)
         << "expected identifier"
         << currentTok().getSourceLoc() << diag::term;

      skipUntilProbableEndOfStmt();
      return nullptr;
   }

   string ident = lexer->getCurrentIdentifier();

   TypeRef *type = nullptr;
   Expression *value = nullptr;

   if (lookahead().is(tok::colon)) {
      advance();
      advance();

      type = parse_type();
   }
   else {
      type = new (Context) TypeRef;
   }

   if (lookahead().is(tok::equals)) {
      advance();
      advance();

      value = parse_expr_sequence();
   }

   if (SP.getDeclPass()->getDeclContext().isGlobalDeclContext()) {
      auto G = makeExpr<GlobalVarDecl>(loc, access, isLet, move(ident),
                                       type, value);

      SP.addDeclToContext(SP.getDeclPass()->getDeclContext(), G);
      return G;
   }
   else {
      return makeExpr<LocalVarDecl>(loc, access, isLet, move(ident), type,
                                    value);
   }
}

DestructuringDecl * Parser::parse_destructuring_decl(AccessModifier access,
                                                     bool isLet) {
   assert(currentTok().is(tok::open_paren) && "should not be called otherwise");

   llvm::SmallVector<VarDecl *, 8> decls;
   bool global = SP.getDeclPass()->getDeclContext().isGlobalDeclContext();
   auto parenLoc = currentTok().getSourceLoc();

   while (!currentTok().is(tok::close_paren)) {
      lexer->expect(tok::ident);

      auto loc = currentTok().getSourceLoc();
      auto ident = lexer->getCurrentIdentifier();
      if (global) {
         auto G = makeExpr<GlobalVarDecl>(loc, access, isLet, move(ident),
                                          nullptr, nullptr);

         SP.addDeclToContext(SP.getDeclPass()->getDeclContext(), G);
         decls.push_back(G);
      }
      else {
         decls.push_back(makeExpr<LocalVarDecl>(loc, access, isLet, move(ident),
                                                nullptr, nullptr));
      }

      if (lookahead().is(tok::comma))
         advance();
   }

   VarDecl **declAlloc = new (Context) VarDecl*[decls.size()];
   declAlloc[decls.size()] = nullptr;

   size_t i = 0;
   for (auto decl : decls)
      declAlloc[i++] = decl;

   TypeRef *type = nullptr;
   Expression *value = nullptr;

   if (lookahead().is(tok::colon)) {
      advance();
      advance();

      type = parse_type();
   }
   else {
      type = new (Context) TypeRef;
   }

   if (lookahead().is(tok::equals)) {
      advance();
      advance();

      value = parse_expr_sequence();
   }

   if (global) {
      return makeExpr<GlobalDestructuringDecl>(parenLoc, access, isLet,
                                               declAlloc, decls.size(), type,
                                               value);
   }
   else {
      return makeExpr<LocalDestructuringDecl>(parenLoc, access, isLet,
                                              declAlloc, decls.size(), type,
                                              value);
   }
}

std::vector<pair<string, TypeRef*>> Parser::parse_tuple_type()
{
   std::vector<pair<string, TypeRef*>> tupleTypes;

   while (!currentTok().is(tok::close_paren)) {
      advance();

      if (currentTok().is(tok::close_paren)) {
         break;
      }

      string label;
      if (currentTok().getKind() == tok::ident
          && lookahead().is(tok::colon)) {
         label = lexer->getCurrentIdentifier();
         advance();
         advance();
      }

      auto argType = parse_type(true);

      tupleTypes.emplace_back(label, argType);
      advance();
      if (!currentTok().is(tok::comma)
          && !currentTok().is(tok::close_paren)) {
         err(err_generic_error)
            << "expected ')'" << diag::term;
      }
   }

   return tupleTypes;
}

std::vector<FuncArgDecl*> Parser::parse_arg_list(SourceLocation &varargLoc)
{
   auto start = currentTok().getSourceLoc();
   std::vector<FuncArgDecl*> args;
   if (!currentTok().is(tok::open_paren)) {
      lexer->backtrack();
      return args;
   }

   bool def_arg = false;
   varargLoc = SourceLocation();

   advance();

   while (!currentTok().is(tok::close_paren)) {
      if (varargLoc) {
         err(err_generic_error)
            << "'...' must appear last in a function signature"
            << varargLoc << diag::term;
      }

      std::vector<Attribute> attributes = parse_attributes();
      start = currentTok().getSourceLoc();

      string argName;
      TypeRef* argType = nullptr;
      Expression* defaultVal = nullptr;
      bool templateArgExpansion = false;
      bool isConst = true;

      if (currentTok().oneOf(tok::kw_var, tok::kw_let)) {
         isConst = currentTok().is(tok::kw_let);
         advance();
      }

      if (lookahead().is(tok::colon)) {
         if (currentTok().getKind() != tok::ident) {
            err(err_generic_error) << "Expected identifier" << diag::term;
         }

         argName = lexer->getCurrentIdentifier();
         advance();
      }
      else if (currentTok().is(tok::triple_period)) {
         varargLoc = currentTok().getSourceLoc();
         advance();

         if (currentTok().getKind() != tok::ident) {
            continue;
         }
      }

      if (currentTok().is(tok::colon)) {
         advance();
      }

      argType = parse_type(true);
      advance();

      if (argType->isVariadicArgPackExpansion()) {
         if (varargLoc) {
            err(err_generic_error) << "template parameter pack "
               "expansions cannot be vararg" << lexer << diag::term;
         }

         templateArgExpansion = true;
      }

      // optional default value
      if (currentTok().is(tok::equals)) {
         if (varargLoc) {
            err(err_generic_error)
               << "vararg arguments cannot have a default value"
               << diag::term;
         }

         advance();

         defaultVal = parse_expr_sequence();
         def_arg = true;

         advance();

      } else if (def_arg) {
         err(err_generic_error)
            << "default values are only allowed as last items of"
               "an argument list" << diag::term;
      }

      auto argDecl = makeExpr<FuncArgDecl>(start, move(argName),
                                           move(argType),
                                           move(defaultVal),
                                           templateArgExpansion,
                                           isConst, /*vararg=*/ false);

      args.push_back(move(argDecl));

      // end of argument list or next argument
      if (currentTok().is(tok::comma)) {
         advance();
      }
      else if (!currentTok().is(tok::close_paren)) {
         err(err_generic_error)
            << "expected ')'"
            << diag::term;
      }

      start = currentTok().getSourceLoc();
   }

   if (!currentTok().is(tok::close_paren)) {
      err(err_generic_error)
         << "expected ')'"
         << diag::term;
   }

   return args;
}

bool Parser::isValidOperatorChar(const Token& next) {
   return !next.is_punctuator() && !next.is(tok::eof)
          && !next.is(tok::arrow_single);
}

NamedDecl* Parser::parse_function_decl()
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
   TypeRef* returnType;
   bool isCastOp;

   if (op.getPrecedenceGroup().isValid()) {
      funcName = parse_operator_name(isCastOp, returnType);
   }
   else {
      funcName = lexer->getCurrentIdentifier();
   }

   auto templateParams = try_parse_template_parameters();
   advance();

   SourceLocation varargLoc;
   auto args = parse_arg_list(varargLoc);

   // optional return type
   if (lookahead().is(tok::arrow_single)) {
      advance();
      advance();
      returnType = parse_type();
   }
   else {
      returnType = makeExpr<TypeRef>(start);
   }

   std::vector<StaticExpr*> constraints;
   while (lookahead().is(tok::kw_where)) {
      advance();
      advance();

      constraints.push_back(
         new (Context) StaticExpr(parse_static_expr()));
   }

   auto funcDecl = makeExpr<FunctionDecl>(start, am, std::move(funcName),
                                          std::move(args),
                                          returnType,
                                          move(constraints),
                                          nullptr, op);

   CompoundStmt* body = nullptr;
   if (lookahead().is(tok::open_brace)) {
      DeclContextRAII declContextRAII(*this, funcDecl);
      body = parse_block();
   }

   funcDecl->setCstyleVararg(varargLoc.isValid());
   funcDecl->setBody(body);
   funcDecl->setTemplateParams(move(templateParams));

   SP.getDeclPass()->DeclareFunction(funcDecl);
   return funcDecl;
}

LambdaExpr* Parser::parse_lambda_expr()
{
   auto start = currentTok().getSourceLoc();

   SourceLocation varargLoc;
   auto args = parse_arg_list(varargLoc);

   if (varargLoc.isValid())
      err(err_generic_error)
         << "lambdas cannot be vararg"
         << varargLoc << diag::term;

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

   Statement* body = parse_next_stmt();

   auto lambdaExpr = makeExpr<LambdaExpr>(start, retType, std::move(args),
                                          body);

   lambdaExpr->setSubExpr(try_parse_member_expr());
   return lambdaExpr;
}

void Parser::joinAdjacentOperators()
{
   // <...T, U, V>
   // ^~~~
   if (currentTok().is(tok::smaller)) {
      auto loc = currentTok().getSourceLoc();
      auto beginPtr = currentTok().getIdentifierInfo()->getIdentifier().data();

      while (lookahead(false, true).is(tok::op_ident))
         advance(false, true);

      auto end = currentTok().getIdentifierInfo()->getIdentifier();
      llvm::StringRef fullOp(beginPtr, (end.data() + end.size()) - beginPtr);

      auto &II = lexer->Idents.get(fullOp);
      lexer->tokens[lexer->tokenIndex - 1] = Token(&II, loc, tok::op_ident);
   }
   // Array<List<Int>>
   //               ^~
   else if (currentTok().is(tok::greater)) {
      auto loc = currentTok().getSourceLoc();
      auto beginPtr = currentTok().getIdentifierInfo()->getIdentifier().data();

      while (lookahead(false, true).is(tok::greater))
         advance(false, true);

      auto end = currentTok().getIdentifierInfo()->getIdentifier();
      llvm::StringRef fullOp(beginPtr, (end.data() + end.size()) - beginPtr);

      auto &II = lexer->Idents.get(fullOp);
      lexer->tokens[lexer->tokenIndex - 1] = Token(&II, loc, tok::op_ident);
   }
}

std::vector<TemplateParamDecl*> Parser::try_parse_template_parameters()
{
   std::vector<TemplateParamDecl*> templateArgs;
   if (!lookahead().is(tok::open_square))
      return templateArgs;

   advance();

   bool defaultGiven = false;

   advance();
   while (!currentTok().is(tok::close_square)) {
      bool variadic = false;
      auto loc = currentTok().getSourceLoc();
      bool isTypeName = true;

      if (currentTok().is(tok::ident)) {
         if (currentTok().isIdentifier("typename")) {
            // default
            advance();
         }
         else if (currentTok().isIdentifier("value")) {
            isTypeName = false;
            advance();
         }
      }

      if (currentTok().is(tok::triple_period)) {
         variadic = true;
         advance();
      }

      if (currentTok().getKind() != tok::ident) {
         err(err_generic_error)
            << "expected template argument name" << diag::term;
      }

      string genericTypeName = lexer->getCurrentIdentifier();
      TypeRef* covariance = nullptr;
      TypeRef* contravariance = nullptr;

      if (lookahead().is(tok::colon)) {
         advance();
         advance();

         if (isTypeName) {
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
                     err(err_generic_error) << "covariance already "
                        "specified" << lexer << diag::term;
                  }

                  covarSet = true;
                  covariance = parse_type();
               }
               else {
                  if (contravarSet) {
                     err(err_generic_error) << "contravariance already "
                        "specified" << lexer << diag::term;
                  }

                  contravarSet = true;
                  contravariance = parse_type();
               }

               if (lookahead().is(tok::comma)
                   || lookahead().is(tok::greater)) {
                  break;
               }

               advance();
            }
         }
         else {
            covariance = parse_type();
         }
      }

      advance();

      TemplateArgExpr *defaultValue = nullptr;
      if (currentTok().is(tok::equals)) {
         defaultGiven = true;
         advance();

         SourceLocation defaultLoc = currentTok().getSourceLoc();

         if (isTypeName) {
            defaultValue = makeExpr<TemplateArgExpr>(defaultLoc, parse_type());
         }
         else {
            defaultValue = makeExpr<TemplateArgExpr>(
               defaultLoc, new (Context) StaticExpr(parse_static_expr(true)));
         }

         advance();
      }
      else if (defaultGiven) {
         lexer->backtrack();
         err(err_generic_error) << "default template paremeter values "
            "can only appear at the end of a parameter list"
                                      << lexer << diag::term;
      }

      if (!covariance)
         covariance = new (Context) TypeRef;

      if (!contravariance)
         contravariance = new (Context) TypeRef;

      if (isTypeName) {
         templateArgs.push_back(
            makeExpr<TemplateParamDecl>(loc, move(genericTypeName),
                                        covariance,
                                        contravariance,
                                        variadic, defaultValue));
      }
      else {
         templateArgs.push_back(
            makeExpr<TemplateParamDecl>(loc, move(genericTypeName),
                                        covariance,
                                        variadic, defaultValue));
      }

      if (currentTok().is(tok::comma)) {
         advance();
      }
   }

   return templateArgs;
}

std::vector<TemplateArgExpr*> Parser::parse_unresolved_template_args()
{
   if (!currentTok().is(tok::smaller)) {
      return {};
   }

   std::vector<TemplateArgExpr*> args;
   advance();

   unsigned openCount = 1;
   unsigned closeCount = 0;

   for (;;) {
      Expression *Expr = nullptr;
      TypeRef *Type = nullptr;

      auto loc = currentTok().getSourceLoc();
      switch (currentTok().getKind()) {
#        define CDOT_LITERAL_TOKEN(Name, Spelling) \
         case tok::Name:
#        include "../lex/Tokens.def"
            Expr = parse_static_expr(true);
            break;
#        define CDOT_OPERATOR_TOKEN(Name, Spelling)               \
         case tok::Name:                                          \
            if (currentTok().is(tok::smaller)) ++openCount;       \
            else if (currentTok().is(tok::greater)) ++closeCount; \
            else Expr = parse_static_expr(true);                  \
            break;
#        include "../lex/Tokens.def"
         case tok::open_paren:
            Type = parse_type(true);
            break;
         case tok::kw_true:
         case tok::kw_false:
            Expr = parse_static_expr(true);
            break;
         case tok::ident:
            if (lexer->getCurrentIdentifier() == "value") {
               advance();
               Expr = parse_static_expr(true);
            }
            else {
               auto next = lookahead();
               if (next.is_operator() && !next.oneOf(tok::greater,
                                                     tok::smaller,
                                                     tok::times,
                                                     tok::times_times,
                                                     tok::triple_period)) {
                  Expr = parse_static_expr(true);
               }
               else {
                  Type = parse_type(true);
               }
            }

            break;
         default:
            Type = parse_type(true);
            break;
      }

      if (openCount == closeCount) {
         break;
      }

      if (Expr)
         args.emplace_back(makeExpr<TemplateArgExpr>(
            loc, new (Context) StaticExpr(Expr)));
      else
         args.emplace_back(makeExpr<TemplateArgExpr>(loc, Type));

      advance();

      if (currentTok().is(tok::comma))
         advance();
   }

   return args;
}

IfStmt* Parser::parse_if_stmt()
{
   auto start = currentTok().getSourceLoc();
   advance();

   auto cond = parse_expr_sequence();
   advance();

   auto ifBranch = parse_next_stmt();
   Statement* elseBranch;

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

StaticIfStmt* Parser::parse_static_if(bool inRecordDecl)
{
   auto start = currentTok().getSourceLoc();
   lexer->advanceIf(tok::kw_static_if);

   auto cond = new (Context) StaticExpr(parse_static_expr());
   advance();

   Statement* ifBranch;
   Statement* elseBranch;

   if (inRecordDecl) {
//      ExtensionDecl *ext = makeExpr<ExtensionDecl>(start);
//      parse_class_inner(ext);
//
      ifBranch = nullptr;
   }
   else {
      ifBranch = parse_next_stmt();
   }

   if (lookahead().is(tok::kw_else)) {
      advance();
      advance();

      if (inRecordDecl) {
//         ExtensionDecl *ext = makeExpr<ExtensionDecl>(start);
//         parse_class_inner(ext);
//
         elseBranch = nullptr;
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

PatternExpr* Parser::parse_pattern()
{
   auto start = currentTok().getSourceLoc();

   if (currentTok().is(tok::isa)) {
      advance();

      return makeExpr<IsPattern>(start, parse_type());
   }

   if (currentTok().is(tok::period)) {
      advance();

      string caseName = lexer->getCurrentIdentifier();
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

            auto ident = lexer->getCurrentIdentifier();
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
               err(err_generic_error)
                  << "unexpected token " + lookahead().toString()
                  << lexer << diag::term;
         }
      }

      return makeExpr<CasePattern>(start, move(caseName), move(args));
   }

   return makeExpr<ExpressionPattern>(start, parse_expr_sequence());
}

CaseStmt* Parser::parse_case_stmt(bool default_)
{
   auto start = currentTok().getSourceLoc();

   CompoundStmt* body = nullptr;
   PatternExpr* pattern = nullptr;

   if (!default_) {
      advance();
      pattern = parse_pattern();
   }

   lexer->expect(tok::colon);

   bool isDefault = lookahead().isIdentifier("default");
   if (!lookahead().oneOf(tok::kw_case, tok::close_brace) && !isDefault) {
      body = new (Context) CompoundStmt;

      while (!lookahead().oneOf(tok::kw_case, tok::close_brace) && !isDefault) {
         advance();
         body->addStatement(parse_next_stmt());

         isDefault = lookahead().isIdentifier("default");
      }
   }

   CaseStmt* caseStmt;
   if (default_)
      caseStmt = makeExpr<CaseStmt>(start, move(body));
   else
      caseStmt = makeExpr<CaseStmt>(start, move(pattern), move(body));

   advance();

   return caseStmt;
}

MatchStmt* Parser::parse_match_stmt()
{
   auto start = currentTok().getSourceLoc();
   advance();

   Expression* switch_val = parse_expr_sequence();
   advance();
   if (!currentTok().is(tok::open_brace)) {
      err(err_generic_error)
         << "expected '{' after 'match'" << diag::term;
   }

   auto switch_stmt = makeExpr<MatchStmt>(start, move(switch_val));
   advance();

   bool isDefault = false;
   while (!currentTok().is(tok::close_brace)) {
      if (isDefault) {
         err(err_generic_error)
            << "expected no more cases after 'default'" << diag::term;
      }

      isDefault = currentTok().isIdentifier("default");
      if (!currentTok().is(tok::kw_case) && !isDefault) {
         err(err_generic_error)
            << "expected 'case' or 'default'" << diag::term;
      }

      auto caseStmt = parse_case_stmt(isDefault);
      switch_stmt->addCase(move(caseStmt));
   }

   return switch_stmt;
}

WhileStmt* Parser::parse_while_stmt(bool conditionBefore)
{
   auto start = currentTok().getSourceLoc();
   advance();

   Expression* cond = nullptr;
   if (conditionBefore) {
      cond = parse_expr_sequence();
      advance();
   }

   auto body = parse_next_stmt();
   if (!conditionBefore && lookahead().is(tok::kw_while)) {
      advance();
      advance();

      cond = parse_expr_sequence();
   }

   if (!cond)
      cond = makeExpr<BoolLiteral>(start, Context.getBoolTy(), true);

   auto whileStmt = makeExpr<WhileStmt>(start, move(cond), move(body),
                                        !conditionBefore);

   return whileStmt;
}

Statement* Parser::parse_for_stmt()
{
   auto start = currentTok().getSourceLoc();
   advance();

   Statement* init;
   if (currentTok().is(tok::semicolon)) {
      init = nullptr;
   }
   else {
      init = parse_next_stmt();
      advance();
   }

   // range based for loop
   if (currentTok().isIdentifier("in")) {
      if (!isa<LocalVarDecl>(init)) {
         err(err_generic_error)
            << "expected declaration before 'in' in range based for loop"
            << diag::term;
      }

      advance();

      Expression* range = parse_expr_sequence();
      advance();

      if (currentTok().is(tok::close_paren)) {
         advance();
      }

      auto body = cast<CompoundStmt>(parse_next_stmt());
      auto decl = cast<LocalVarDecl>(init);

      auto forInStmt = makeExpr<ForInStmt>(start, std::move(decl),
                                           std::move(range), std::move(body));

      return forInStmt;
   }

   if (!currentTok().is(tok::semicolon)) {
      err(err_generic_error)
         << "Expected ';' to seperate for loop arguments" << diag::term;
   }

   Expression* term;
   advance();
   if (currentTok().is(tok::semicolon)) {
      term = makeExpr<BoolLiteral>(start, Context.getBoolTy(), true);
   }
   else {
      term = parse_expr_sequence();
      advance();
   }

   if (!currentTok().is(tok::semicolon)) {
      err(err_generic_error)
      << "Expected ';' to seperate for loop arguments" << diag::term;
   }

   Statement* inc;
   advance();
   if (currentTok().is(tok::open_brace)) {
      inc = nullptr;
   }
   else {
      inc = parse_next_stmt();
      advance();
   }

   return makeExpr<ForStmt>(start, init, term, inc, parse_next_stmt());
}

StaticForStmt* Parser::parse_static_for(bool inRecordDecl)
{
   auto start = currentTok().getSourceLoc();

   lexer->expect(tok::kw_let);
   lexer->expect(tok::ident);

   auto ident = lexer->getCurrentIdentifier();
   lexer->expect(tok::ident);
   if (!currentTok().isIdentifier("in"))
      err(err_generic_error) << "expected 'in'" << lexer << diag::term;

   advance();

   auto range = new (Context) StaticExpr(parse_static_expr());
   advance();

   Statement* body;
   if (inRecordDecl) {
//      ExtensionDecl *ext = makeExpr<ExtensionDecl>(start);
//      parse_class_inner(ext);
//
      body = nullptr;
   }
   else {
      body = parse_next_stmt();
   }

   return makeExpr<StaticForStmt>(start, move(ident), move(range),
                                  move(body));
}

StaticStmt* Parser::parse_static_stmt(bool inRecordDecl)
{
   switch (currentTok().getKind()) {
      case tok::kw_static_assert:
         return parse_static_assert();
      case tok::kw_static_for:
         return parse_static_for(inRecordDecl);
      case tok::kw_static_if:
         return parse_static_if(inRecordDecl);
      default:
         llvm_unreachable("bad static stmt kind");
   }
}

StaticAssertStmt* Parser::parse_static_assert()
{
   auto start = currentTok().getSourceLoc();
   lexer->expect(tok::open_paren);
   advance();

   auto expr = new (Context) StaticExpr(parse_static_expr());

   string msg;
   if (lookahead().is(tok::comma)) {
      advance();
      advance();

      auto StringLit = parse_unary_expr_target();
      if (!isa<StringLiteral>(StringLit))
         err(err_generic_error)
            << "expected string literal as second argument to static_assert"
            << lexer << diag::term;

      msg = move(cast<StringLiteral>(StringLit)->getValue());
   }

   lexer->expect(tok::close_paren);

   return makeExpr<StaticAssertStmt>(start, move(expr), move(msg));
}

StaticPrintStmt* Parser::parse_static_print()
{
   auto start = currentTok().getSourceLoc();
   lexer->expect(tok::open_paren);
   advance();

   auto expr = new (Context) StaticExpr(parse_static_expr());
   lexer->expect(tok::close_paren);

   return makeExpr<StaticPrintStmt>(start, move(expr));
}

static void validateDeclareStmt(Parser &P, Statement *Stmt,
                                ExternKind externKind) {
   auto decl = dyn_cast<NamedDecl>(Stmt);

   if (!decl) {
      err(err_generic_error)
         << "expected declaration after 'declare'"
         << Stmt->getSourceLoc() << diag::term;

      return;
   }
   else if (decl->hasDefinition()) {
      err(err_generic_error)
         << "declared statement may not have a definition"
         << Stmt->getSourceLoc() << diag::term;
   }

   decl->setExternKind(externKind);
}

Statement* Parser::parse_declare_stmt()
{
   advance();

   ExternKind externKind = ExternKind::None;
   auto start = currentTok().getSourceLoc();

   if (currentTok().getKind() == tok::ident) {
      auto str = lexer->getCurrentIdentifier();

      if (str == "C") {
         externKind = ExternKind::C;
      }
      else if (str == "C++" || str == "CPP") {
         externKind = ExternKind::CXX;
      }
      else if (str == "__native") {
         externKind = ExternKind::Native;
      }
      else {
         err(err_generic_error)
            << "unsupported extern kind: " + str << diag::term;
      }

      advance();
   }

   if (currentTok().is(tok::open_brace)) {
      auto compound = makeExpr<CompoundStmt>(start);
      while (!lookahead().is(tok::close_brace)) {
         advance();

         auto stmt = parse_next_stmt();
         validateDeclareStmt(*this, stmt, externKind);

         compound->addStatement(stmt);
      }

      lexer->expect(tok::close_brace);
      return compound;
   }
   else {
      auto stmt = parse_next_stmt();
      validateDeclareStmt(*this, stmt, externKind);

      return stmt;
   }
}

TryStmt* Parser::parse_try_stmt()
{
   auto start = currentTok().getSourceLoc();
   advance();

   auto tryBody = parse_next_stmt();
   TryStmt* tryStmt = makeExpr<TryStmt>(start, tryBody);

   while (lookahead().oneOf(tok::kw_catch, tok::kw_finally)) {
      advance();

      auto kind = currentTok().getKind();
      bool finallySet = false;

      if (kind == tok::kw_catch) {
         advance();
         CatchBlock catchBlock;

         auto decl = parse_var_decl();
         catchBlock.varDecl = dyn_cast<LocalVarDecl>(decl);

         if (!catchBlock.varDecl) {
            err(err_generic_error)
               << "destructuring declaration cannot appear in catch"
               << decl->getSourceLoc() << diag::term;
         }

         if (!catchBlock.varDecl->getTypeRef())
            err(err_generic_error)
               << "catch must have a defined type"
               << decl->getSourceLoc() << diag::term;

         advance();
         catchBlock.body = parse_next_stmt();

         tryStmt->addCatch(catchBlock);
      }
      else {
         if (finallySet) {
            err(err_generic_error)
               << "finally block already defined" << diag::term;
         }

         advance();

         auto finallyBody = parse_next_stmt();
         tryStmt->setFinally(finallyBody);

         finallySet = true;
      }
   }

   return tryStmt;
}

ThrowStmt* Parser::parse_throw_stmt()
{
   auto start = currentTok().getSourceLoc();
   advance();

   return makeExpr<ThrowStmt>(start, parse_expr_sequence());
}

ReturnStmt* Parser::parse_return_stmt()
{
   auto start = currentTok().getSourceLoc();
   Token next = lookahead(false);
   ReturnStmt* ret;

   if (!next.is_separator()) {
      advance();
      ret = makeExpr<ReturnStmt>(start, parse_expr_sequence());
   }
   else {
      ret = makeExpr<ReturnStmt>(start);
   }

   return ret;
}

Statement* Parser::parse_keyword()
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
         return parse_var_decl();
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
         Statement* stmt;
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
         return parse_declare_stmt();
      case tok::kw_return:
         return parse_return_stmt();
      case tok::kw_try:
         return parse_try_stmt();
      case tok::kw_throw:
         return parse_throw_stmt();
      case tok::kw_goto:
         advance();
         return makeExpr<GotoStmt>(start, lexer->getCurrentIdentifier());
      case tok::kw_namespace:
         return parse_namespace_decl();
      case tok::kw_using:
         return parse_using_stmt();
      case tok::kw_import:
         err(err_generic_error)
            << "import statements can only appear at the beginning of a file"
            << currentTok().getSourceLoc()
            << diag::term;

         skipUntilProbableEndOfStmt();
         return nullptr;
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
            Lexer::StateSaveGuard guard(lexer);
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
            case tok::kw_class:
            case tok::kw_enum:
            case tok::kw_union:
            case tok::kw_protocol:
            case tok::kw_extend:
               return parse_any_record(relevantToken);
            case tok::kw_typedef:
               return parse_typedef();
            case tok::kw_var:
            case tok::kw_let:
               return parse_var_decl();
            case tok::kw_def:
               return parse_function_decl();
            default:
               break;
         }
         LLVM_FALLTHROUGH;
      default:
         err(err_generic_error)
            << "unexpected keyword " + currentTok().toString()
            << diag::term;
   }

   llvm_unreachable("bad keyword");
}

CallExpr* Parser::parse_function_call(bool allowLet)
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
      funcName = lexer->getCurrentIdentifier();
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
   call->setSubExpr(try_parse_member_expr());
   call->setTemplateArgs(std::move(generics));

   if (isVariadicSizeof)
      call->setKind(CallKind::VariadicSizeof);

   return call;
}

EnumCaseExpr* Parser::parse_enum_case_expr()
{
   auto start = currentTok().getSourceLoc();
   auto ident = lexer->getCurrentIdentifier();

   EnumCaseExpr* expr;
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
            label = lexer->getCurrentIdentifier();
            advance();
            advance();

            isLabeled = true;
         }
         else if (currentTok().is(tok::close_paren)) {
            break;
         }
         else if (isLabeled) {
            err(err_generic_error)
               << "labeled arguments have to come last in a call" << diag::term;
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
         err(err_generic_error)
            << "Expected identifier after 'let' / 'var'" << diag::term;
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
         err(err_generic_error)
            << "expected attribute name" << diag::term;
      }

      Attribute attr;
      attr.name = lexer->getCurrentIdentifier();
      attr.kind = AttributeMap[attr.name];

      if (lookahead().is(tok::open_paren)) {
         advance();
         advance();

         while (!currentTok().is(tok::close_paren)
                && currentTok().getKind() != tok::eof) {
//            attr.args.push_back(lexer->parseExpression({}, 0, true));
            if (lookahead().is(tok::comma)) {
               advance();
            }

            advance();
         }
      }

      auto check = isValidAttribute(attr);
      if (!check.empty()) {
         err(err_generic_error)
            << check << diag::term;
      }

      attributes.push_back(attr);
      foundAttrs.push_back(attr.name);
      advance();
   }

   return attributes;
}

CompoundStmt* Parser::parse_block(bool preserveTopLevel)
{
   auto start = currentTok().getSourceLoc();
   bool last_top_level = top_level;

   if (!preserveTopLevel) {
      top_level = false;
   }

   advance();

   if (!(currentTok().is(tok::open_brace))) {
      err(err_generic_error)
         << "expected '{' to start a block statement." << diag::term;
   }

   advance();

   CompoundStmt* block = makeExpr<CompoundStmt>(start);
   while (!currentTok().is(tok::close_brace)) {
      while (currentTok().oneOf(tok::semicolon, tok::newline)) {
         advance();
      }

      if (currentTok().getKind() == tok::eof
          || currentTok().is(tok::close_brace)) {
         break;
      }

      block->addStatement(parse_next_stmt());
      advance();
   }

   if (!currentTok().is(tok::close_brace)) {
      err(err_generic_error)
         << "Expected '}' to end a block statement" << diag::term;
   }

   top_level = last_top_level;

   return block;
}

void Parser::skip_block()
{
   auto openedBraces = 1;
   auto closedBraces = 0;

   if (!currentTok().is(tok::open_brace)) {
      err(err_generic_error) << "expected '{'" << diag::term;
   }

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

Statement* Parser::parse_next_stmt()
{
   auto attrs = parse_attributes();
   Statement* stmt;

   if (currentTok().is(tok::open_brace)) {
      lexer->backtrack();
      stmt = parse_block();
   }
   else if (currentTok().is_keyword()) {
      stmt = parse_keyword();
   }
   else if (currentTok().getKind() == tok::ident
            && lookahead().is(tok::colon)) {
      string label = lexer->getCurrentIdentifier();
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

NamespaceDecl* Parser::parse_namespace_decl()
{
   auto start = currentTok().getSourceLoc();
   bool anonymous = false;
   string nsName;

   if (lookahead().getKind() != tok::ident) {
      anonymous = true;
      nsName = "(anonymous namespace)";
   }
   else {
      advance();
      nsName = lexer->getCurrentIdentifier();
   }

   while (lookahead().is(tok::period)) {
      advance();
      advance();

      if (currentTok().getKind() != tok::ident) {
         err(err_generic_error)
            << "expected identifier after 'namespace'" << diag::term;
      }

      nsName += ".";
      nsName += lexer->getCurrentIdentifier();
   }

   auto NS = makeExpr<NamespaceDecl>(start, move(nsName),
                                     nullptr, anonymous);

   {
      DeclContextRAII declContextRAII(*this, NS);
      NS->setBody(parse_block(true));
   }

   SP.addDeclToContext(SP.getDeclPass()->getDeclContext(), NS);

   return NS;
}

UsingStmt* Parser::parse_using_stmt()
{
   auto start = currentTok().getSourceLoc();
   lexer->advanceIf(tok::kw_using);

   std::vector<string> declContext;
   std::vector<string> importedItems;

   SourceLocation wildCardLoc;
   while (lookahead().is(tok::period)) {
      if (currentTok().is(tok::times)) {
         wildCardLoc = currentTok().getSourceLoc();
         break;
      }
      if (!currentTok().is(tok::ident)) {
         err(err_generic_error)
            << "expected identifier"
            << currentTok().getSourceLoc() << diag::term;

         break;
      }

      declContext.emplace_back(lexer->getCurrentIdentifier());
      advance();
      advance();
   }

   if (importedItems.empty()) {
      if (currentTok().is(tok::open_brace)) {
         lexer->expect(tok::ident, tok::times);
         while (!currentTok().is(tok::close_brace)) {
            if (currentTok().is(tok::times)) {
               wildCardLoc = currentTok().getSourceLoc();
            }
            else {
               importedItems.emplace_back(lexer->getCurrentIdentifier());
            }

            advance();

            if (currentTok().is(tok::comma))
               advance();
         }
      }
      else if (currentTok().is(tok::times)) {
         wildCardLoc = currentTok().getSourceLoc();
      }
      else if (!currentTok().is(tok::ident)) {
         err(err_generic_error)
            << "expected identifier"
            << currentTok().getSourceLoc() << diag::term;
      }
      else {
         importedItems.emplace_back(lexer->getCurrentIdentifier());
      }
   }

   if (wildCardLoc.isValid() && !importedItems.empty()) {
      warn(warn_generic_warn)
         << "other imports have no effect when wildcard import ('*') is present"
         << wildCardLoc << diag::cont;

      importedItems.clear();
   }

   return makeExpr<UsingStmt>(start,
                              std::move(declContext),
                              std::move(importedItems),
                              wildCardLoc.isValid());
}

ModuleStmt* Parser::parse_module_stmt()
{
   auto start = currentTok().getSourceLoc();

   assert(currentTok().is(tok::kw_module));
   lexer->expect(tok::ident);

   std::vector<string> moduleName;

   while (1) {
      moduleName.emplace_back(lexer->getCurrentIdentifier());

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

ImportStmt* Parser::parse_import_stmt()
{
   auto start = currentTok().getSourceLoc();

   assert(currentTok().is(tok::kw_import));
   lexer->expect(tok::ident);

   std::vector<string> moduleName;

   while (1) {
      moduleName.emplace_back(lexer->getCurrentIdentifier());

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

void Parser::parseImports(llvm::SmallVectorImpl<ImportStmt*> &stmts)
{
   while (currentTok().oneOf(tok::newline, tok::space, tok::semicolon))
      advance();

   while (currentTok().is(tok::kw_import)) {
      stmts.push_back(parse_import_stmt());
      advance();
   }
}

void Parser::parse(llvm::SmallVectorImpl<Statement*> &stmts)
{
//   if (!Compiler::getOptions().noBasicLib) {
//      auto basicImport = std::make_shared<ImportStmt>(
//         std::vector<string>{ "std", "basic" }
//      );
//
//      decl->addGlobalStatement(basicImport);
//      root->addStatement(move(basicImport));
//   }

   while(currentTok().getKind() != tok::eof) {
      while (currentTok().oneOf(tok::newline, tok::semicolon)) {
         advance();
      }

      if (currentTok().getKind() == tok::eof) {
         break;
      }

      auto stmt = parse_next_stmt();
      if (!stmt)
         return;

      stmts.push_back(stmt);
      advance();
   }
}

} // namespace parse
} // namespace cdot