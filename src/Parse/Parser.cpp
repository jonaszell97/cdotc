//
// Created by Jonas Zell on 13.06.17.
//

#include "Parser.h"

#include "Module/Module.h"
#include "Lex/Lexer.h"

#include "Basic/IdentifierInfo.h"
#include "Basic/Variant.h"

#include "AST/ASTContext.h"
#include "AST/Decl.h"
#include "AST/Expression.h"
#include "AST/Statement.h"

#include "Sema/SemaPass.h"
#include "Sema/Builtin.h"
#include "AST/Traverse.h"

#include "Basic/FileManager.h"
#include "Message/Diagnostics.h"

#include "Support/Casting.h"

#include <cassert>
#include <vector>
#include <string>
#include <llvm/ADT/StringSwitch.h>
#include <llvm/ADT/SmallString.h>
#include <Support/Format.h>
#include <llvm/Support/PrettyStackTrace.h>

using namespace cdot::diag;
using namespace cdot::support;
using namespace cdot::lex;

namespace cdot {
namespace parse {

Parser::Parser(ASTContext& Context,
               lex::Lexer *lexer,
               SemaPass &SP,
               bool isModuleParser)
   : Context(Context),
     source_id(lexer->getSourceId()),
     isModuleParser(isModuleParser),
     lexer(lexer),
     SP(SP),
     Idents(lexer->getIdents()),
     Ident_self(&Idents.get("self")),
     Ident_super(&Idents.get("super")),
     Ident_in(&Idents.get("in")),
     Ident_then(&Idents.get("then")),
     Ident_default(&Idents.get("default")),
     Ident_typename(&Idents.get("typename")),
     Ident_value(&Idents.get("value")),
     Ident_sizeof(&Idents.get("sizeof")),
     Ident_decltype(&Idents.get("decltype")),
     Ident___traits(&Idents.get("__traits")),
     Ident___nullptr(&Idents.get("__nullptr")),
     Ident___func__(&Idents.get("__func__")),
     Ident___mangled_func(&Idents.get("__mangled_func")),
     Ident___ctfe(&Idents.get("__ctfe"))
{
   lexer->lex();
}

Parser::~Parser() = default;

namespace {

class ParseDeclPrettyStackTraceEntry: public llvm::PrettyStackTraceEntry {
   DeclContext *D;

public:
   ParseDeclPrettyStackTraceEntry(DeclContext *D) : D(D)
   {}

   void print(llvm::raw_ostream &OS) const override
   {
      if (auto ND = dyn_cast<NamedDecl>(D))
         OS << "while parsing '" << ND->getDeclName() << "'\n";
   }
};

}

Parser::DeclContextRAII::DeclContextRAII(Parser &P, DeclContext *Ctx)
   : P(P)
{
   static_assert(sizeof(StackTraceEntry)
                    == sizeof(ParseDeclPrettyStackTraceEntry),
                 "insufficient storage!");

   Ctx->setParentCtx(&P.SP.getDeclContext());
   P.SP.pushDeclContext(Ctx);

   new(StackTraceEntry) ParseDeclPrettyStackTraceEntry(Ctx);
}

Parser::DeclContextRAII::~DeclContextRAII()
{
   P.SP.popDeclContext();
   reinterpret_cast<ParseDeclPrettyStackTraceEntry*>(StackTraceEntry)
      ->~ParseDeclPrettyStackTraceEntry();
}

Parser::StateSaveRAII::StateSaveRAII(Parser &P)
   : P(P), idx(unsigned(P.lexer->tokenIndex)), enabled(true)
{

}

Parser::StateSaveRAII::~StateSaveRAII()
{
   if (enabled)
      P.lexer->tokenIndex = idx;
}

bool Parser::inGlobalDeclContext() const
{
   return !isa<CallableDecl>(&SP.getDeclContext());
}

Token Parser::lookahead(bool ignoreNewline, bool sw)
{
   return lexer->lookahead(ignoreNewline, sw);
}

void Parser::advance(bool ignoreNewline, bool sw)
{
   lexer->advance(ignoreNewline, sw);
}

SourceLocation Parser::consumeToken(tok::TokenType kind)
{
   if (currentTok().is(kind)) {
      auto loc = currentTok().getSourceLoc();
      advance();
      return loc;
   }

   return SourceLocation();
}

const lex::Token& Parser::currentTok() const
{
   return lexer->currentTok();
}

bool Parser::expectToken(cdot::lex::tok::TokenType expected)
{
   if (currentTok().is(expected))
      return true;

   errorUnexpectedToken(expected);
   return false;
}

void Parser::errorUnexpectedToken()
{
   SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
               currentTok().toString(), false);
}

void Parser::errorUnexpectedToken(tok::TokenType expected)
{
   SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
               currentTok().toString(), true,
               tok::tokenTypeToString(expected));
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

ParseResult Parser::skipUntilNextDecl()
{
   while (1) {
      switch (lookahead().getKind()) {
      case tok::at:
      case tok::kw_var:
      case tok::kw_let:
      case tok::kw_def:
      case tok::kw_typedef:
      case tok::kw_alias:
      case tok::kw_declare:
      case tok::kw_namespace:
      case tok::kw_using:
      case tok::kw_import:
      case tok::kw_mixin:
      case tok::kw_static:
      case tok::kw_static_assert:
      case tok::kw_static_print:
      case tok::kw_struct:
      case tok::kw_enum:
      case tok::kw_class:
      case tok::kw_protocol:
      case tok::kw_union:
      case tok::kw_extend:
      case tok::kw_public:
      case tok::kw_abstract:
      case tok::kw_private:
      case tok::eof:
         return ParseError();
      default:
         break;
      }

      advance();
   }

   llvm_unreachable("no EOF token!");
}

ParseResult Parser::skipUntilProbableEndOfStmt()
{
   while (!lookahead().oneOf(tok::newline, tok::semicolon, tok::eof,
                              tok::open_brace, tok::close_paren,
                              tok::close_square, tok::close_brace)
          && !lookahead().is_keyword())
      advance(false);

   return ParseError();
}

ParseResult Parser::skipUntilProbableEndOfStmt(cdot::lex::tok::TokenType kind)
{
   while (!lookahead().oneOf(kind, tok::newline, tok::semicolon, tok::eof,
                              tok::open_brace, tok::close_paren,
                              tok::close_square, tok::close_brace)
          && !lookahead().is_keyword())
      advance(false);

   return ParseError();
}

ParseResult Parser::skipUntilProbableEndOfExpr()
{
   while (!lookahead().oneOf(tok::newline, tok::semicolon, tok::eof,
                              tok::open_brace, tok::close_paren,
                              tok::close_square, tok::close_brace)
          && !lookahead().is_keyword())
      advance(false);

   return ParseError();
}

ParseResult Parser::skipUntilEven(tok::TokenType openTok, unsigned int open)
{
   tok::TokenType closeTok;
   switch (openTok) {
   case tok::open_paren: closeTok = tok::close_paren; break;
   case tok::open_brace: closeTok = tok::close_brace; break;
   case tok::open_square: closeTok = tok::close_square; break;
   case tok::smaller: closeTok = tok::greater; break;
   default: llvm_unreachable("not a paren token!");
   }

   unsigned closed = 0;
   while (true) {
      auto next = lookahead();
      if (next.is(openTok))
         ++open;
      else if (next.is(closeTok))
         ++closed;
      else if (next.is(tok::eof)) {
         SP.diagnose(err_unexpected_eof, next.getSourceLoc(), false);
         return ParseError();
      }

      if (open == closed)
         break;

      advance();
   }

   return ParseError();
}

bool Parser::findTokOnLine(tok::TokenType kind)
{
   StateSaveRAII raii(*this);

   while (!currentTok().oneOf(tok::newline, tok::eof)) {
      if (currentTok().is(kind)) {
         raii.disable();
         return true;
      }

      advance(false);
   }

   return false;
}

bool Parser::findTokOnLine(IdentifierInfo *Id)
{
   StateSaveRAII raii(*this);

   while (!currentTok().oneOf(tok::newline, tok::eof)) {
      if (currentTok().is(Id)) {
         raii.disable();
         return true;
      }

      advance(false);
   }

   return false;
}

ParseTypeResult Parser::parseType(bool allowInferredArraySize)
{
   auto BeginLoc = currentTok().getSourceLoc();

   bool isReference = false;
   if (currentTok().is(tok::kw_ref)) {
      advance();
      isReference = true;
   }

   auto typeResult = parseTypeImpl(allowInferredArraySize);
   if (!typeResult)
      return ParseTypeResult();

   auto typeref = typeResult.get();

   // pointer type
   auto next = lookahead();
   while (1) {
      if (next.oneOf(tok::times, tok::times_times, tok::op_ident)) {
         if (next.is(tok::times)) {
            advance();

            auto PtrTy = PointerTypeExpr::Create(
               Context, SourceRange(BeginLoc, currentTok().getSourceLoc()),
               typeref);

            typeref = SourceType(PtrTy);
            break;
         }
         if (next.is(tok::times_times)) {
            advance();

            auto PtrTy = PointerTypeExpr::Create(
               Context, SourceRange(BeginLoc, currentTok().getSourceLoc()),
               typeref);

            typeref = SourceType(PtrTy);

            PtrTy = PointerTypeExpr::Create(
               Context, SourceRange(BeginLoc, currentTok().getSourceLoc()),
               typeref);

            typeref = SourceType(PtrTy);
            break;
         }

         auto op = next.getIdentifierInfo()->getIdentifier();
         bool onlyStars = std::count(op.begin(), op.end(), '*') == op.size();

         if (onlyStars) {
            advance();
            for (size_t i = 0; i < op.size(); ++i) {
               auto PtrTy = PointerTypeExpr::Create(
                  Context, SourceRange(BeginLoc, currentTok().getSourceLoc()),
                  typeref);

               typeref = SourceType(PtrTy);
            }
         }

         break;
      }
      // optional type
      else if (next.is(tok::question)) {
         advance();
         auto OptTy = OptionTypeExpr::Create(
            Context, SourceRange(BeginLoc, currentTok().getSourceLoc()),
            typeref);

         typeref = SourceType(OptTy);
      }
      else {
         break;
      }

      next = lookahead();
   }

   if (typeref.isValid() && isReference) {
      auto RefTy = ReferenceTypeExpr::Create(
         Context, SourceRange(BeginLoc, currentTok().getSourceLoc()),
         typeref);

      typeref = SourceType(RefTy);
   }

   return typeref;
}

ParseTypeResult Parser::parseTypeImpl(bool allowInferredArraySize)
{
   auto start = currentTok().getSourceLoc();

   if (currentTok().getIdentifierInfo() == Ident_decltype) {
      if (!expect(tok::open_paren)) {
         skipUntilProbableEndOfExpr();
         return ParseTypeResult();
      }

      advance();

      auto expr = parseExprSequence();
      if (!expr) {
         skipUntilEven(tok::open_paren);
         return ParseTypeResult();
      }

      expect(tok::close_paren);
      return SourceType(
         DeclTypeExpr::Create(Context,
                              SourceRange(start, currentTok().getSourceLoc()),
                              expr.getExpr()));
   }

   // collection type
   if (currentTok().is(tok::open_square)) {
      advance();

      auto elType = parseType();
      if (!elType) {
         skipUntilEven(tok::open_square);
         return ParseTypeResult();
      }

      advance();

      if (currentTok().is(tok::colon)) {
         advance();

         auto valType = parseType();
         if (!valType) {
            skipUntilEven(tok::open_square);
            return ParseTypeResult();
         }

         advance();

         if (!currentTok().is(tok::close_square)) {
            SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                        currentTok().toString(), true, "']'");
         }

         std::vector<Expression*> templateArgs{
            elType.get().getTypeExpr(),
            valType.get().getTypeExpr()
         };

         static auto *II = &Context.getIdentifiers().get("Dictionary");
         return SourceType(new(Context) IdentifierRefExpr(start, II,
                                                          move(templateArgs)));
      }

      // fixed (or inferred) size array
      if (currentTok().is(tok::semicolon)) {
         advance();

         StaticExpr *SizeExpr;
         if (currentTok().is(tok::question)) {
            if (!allowInferredArraySize)
               SP.diagnose(err_inferred_arr_size_not_allowed,
                           currentTok().getSourceLoc());

            SizeExpr = nullptr;
         }
         else {
            auto sizeResult = parseExprSequence();
            if (!sizeResult) {
               skipUntilEven(tok::open_square);
               return ParseTypeResult();
            }

            SizeExpr = StaticExpr::Create(Context, sizeResult.getExpr());
         }

         expect(tok::close_square);

         return SourceType(
            ArrayTypeExpr::Create(Context,
                                  { start, currentTok().getSourceLoc() },
                                  elType.get(), SizeExpr));
      }

      if (!currentTok().is(tok::close_square)) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                     currentTok().toString(), true, "']'");
      }

      std::vector<Expression*> templateArgs{ elType.get().getTypeExpr() };

      static auto *II = &Context.getIdentifiers().get("Array");
      return SourceType(new(Context) IdentifierRefExpr(start, II,
                                                       move(templateArgs)));
   }

   // function or tuple type
   if (currentTok().is(tok::open_paren)) {
      advance();

      llvm::SmallVector<SourceType, 4> TupleTypes;
      while (!currentTok().is(tok::close_paren)) {
         auto NextTy = parseType();
         if (!NextTy) {
            skipUntilEven(tok::open_paren);
            break;
         }

         TupleTypes.push_back(NextTy.get());
         advance();

         if (currentTok().is(tok::comma)) {
            advance();
         }
         else if (currentTok().is(tok::close_paren)) {
            break;
         }
         else {
            SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                        currentTok().toString(), true, "')'");
         }
      }

      // tuple type
      if (!lookahead().is(tok::arrow_single)) {
         return SourceType(TupleTypeExpr::Create(
            Context, { start, currentTok().getSourceLoc() }, TupleTypes));
      }

      advance();
      advance();

      auto returnType = parseType();
      if (!returnType) {
         return ParseTypeResult();
      }

      return SourceType(FunctionTypeExpr::Create(
         Context, { start, currentTok().getSourceLoc() }, returnType.get(),
         TupleTypes));
   }

   auto IdentTy = parseIdentifierExpr(true);
   if (!IdentTy)
      return ParseTypeResult();

   return SourceType(IdentTy.getExpr());
}

ParseResult Parser::parseTypedef(AccessModifier am)
{
   auto TypedefLoc = currentTok().getSourceLoc();

   if (am == AccessModifier::DEFAULT) {
      am = maybeParseAccessModifier();
   }

   if (currentTok().is(tok::kw_typedef))
      advance();

   auto originTy = parseType();
   if (!originTy) {
      return skipUntilProbableEndOfStmt();
   }
   if (!expect(tok::as)) {
      return skipUntilProbableEndOfStmt();
   }
   if (!expect(tok::ident)) {
      return skipUntilProbableEndOfStmt();
   }

   auto Name = currentTok().getIdentifierInfo();
   auto params = tryParseTemplateParameters();

   auto td = TypedefDecl::Create(Context, am, TypedefLoc, Name, originTy.get(),
                                 move(params));

   SP.ActOnTypedefDecl(td);
   return td;
}

ParseResult Parser::parseAlias()
{
   auto AliasLoc = currentTok().getSourceLoc();
   lexer->advanceIf(tok::kw_alias);

   if (!currentTok().is(tok::ident)) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), true, "identifier");

      return ParseStmtError();
   }

   auto Name = currentTok().getIdentifierInfo();
   auto params = tryParseTemplateParameters();

   std::vector<StaticExpr*> constraints;
   while (lookahead().is(tok::kw_where)) {
      advance();
      advance();

      auto expr = parseExprSequence();
      if (expr)
         constraints.push_back(StaticExpr::Create(Context, expr.getExpr()));
      else
         skipUntilProbableEndOfExpr();
   }

   if (!expect(tok::equals)) {
      return skipUntilProbableEndOfStmt();
   }

   advance();

   auto ExprRes = parseExprSequence();
   if (!ExprRes)
      return skipUntilNextDecl();

   auto aliasExpr = StaticExpr::Create(Context, ExprRes.getExpr());
   auto aliasDecl = AliasDecl::Create(Context, AliasLoc, Name, aliasExpr,
                                      move(params));

   Context.setConstraints(aliasDecl, constraints);

   SP.ActOnAliasDecl(aliasDecl);
   return aliasDecl;
}

ParseResult Parser::parseIdentifierExpr(bool parsingType)
{
   auto start = currentTok().getSourceLoc();
   auto ident = currentTok().getIdentifierInfo();

   if (ident == Ident___nullptr)
      return BuiltinIdentExpr::Create(Context, start,
                                      BuiltinIdentifier::NULLPTR);
   if (ident == Ident___func__)
      return BuiltinIdentExpr::Create(Context, start,
                                      BuiltinIdentifier::FUNC);
   if (ident == Ident___mangled_func)
      return BuiltinIdentExpr::Create(Context, start,
                                      BuiltinIdentifier::MANGLED_FUNC);
   if (ident == Ident___ctfe)
      return BuiltinIdentExpr::Create(Context, start,
                                      BuiltinIdentifier::__ctfe);

   auto IdentExpr = new(Context) IdentifierRefExpr(start, ident, {},
                                                   nullptr, parsingType);

   return maybeParseSubExpr(IdentExpr, parsingType);
}

ParseResult Parser::maybeParseSubExpr(Expression *ParentExpr, bool parsingType)
{
   advance(false);
   auto start = currentTok().getSourceLoc();

   // member access
   bool pointerAccess = currentTok().is(tok::arrow_single);
   if (currentTok().is(tok::period) || pointerAccess) {
      advance(false);

      // tuple access
      if (currentTok().is(tok::integerliteral) && !parsingType) {
         unsigned index = unsigned(std::stoul(currentTok().getText()));
         auto tupleExpr = new(Context) TupleMemberExpr(
            currentTok().getSourceLoc(), ParentExpr, index, pointerAccess);

         return maybeParseSubExpr(tupleExpr);
      }

      Expression *Expr;
      if (lookahead().is(tok::open_paren) && !parsingType) {
         auto Call = parseFunctionCall(false, ParentExpr, pointerAccess);
         if (Call)
            Expr = Call.getExpr();
         else {
            skipUntilProbableEndOfExpr();
            Expr = ParentExpr;
         }
      }
      else {
         auto ident = currentTok().getIdentifierInfo();
         Expr = new (Context) MemberRefExpr(currentTok().getSourceLoc(),
                                            ParentExpr, ident, pointerAccess);
      }

      return maybeParseSubExpr(Expr);
   }

   // call
   if (currentTok().is(tok::open_paren) && !parsingType) {
      auto args = parseCallArguments();

      SourceRange Parens(start, currentTok().getSourceLoc());
      auto call = new(Context) CallExpr(ParentExpr->getSourceLoc(),
                                        Parens, ParentExpr,
                                        move(args.args));

      return maybeParseSubExpr(call);
   }

   // subscript or template argument list - always assumes template arguments
   // if applicable, will be fixed by Sema if it's meant to be a subscript
   if (currentTok().is(tok::open_square)) {
      advance();

      std::vector<Expression*> indices;
      while (!currentTok().is(tok::close_square)) {
         auto expr = parseExprSequence();
         if (!expr) {
            skipUntilEven(tok::open_square);
            break;
         }

         indices.push_back(expr.getExpr());
         advance();

         if (currentTok().is(tok::comma))
            advance();
      }

      bool IsSubscript = true;
      if (auto Ident = dyn_cast<IdentifierRefExpr>(ParentExpr)) {
         if (Ident->getTemplateArgs().empty()) {
            IsSubscript = false;
            Ident->setTemplateArgs(move(indices));
         }
      }
      else if (auto MemExpr = dyn_cast<MemberRefExpr>(ParentExpr)) {
         if (MemExpr->getTemplateArgs().empty()) {
            IsSubscript = false;
            MemExpr->setTemplateArgs(move(indices));
         }
      }
      else if (auto Call = dyn_cast<CallExpr>(ParentExpr)) {
         if (Call->getTemplateArgs().empty()) {
            IsSubscript = false;
            Call->setTemplateArgs(move(indices));
         }
      }

      if (IsSubscript) {
         SourceRange SquareRange(start, currentTok().getSourceLoc());
         ParentExpr = SubscriptExpr::Create(Context, SquareRange, ParentExpr,
                                            move(indices));
      }

      return maybeParseSubExpr(ParentExpr);
   }

   if (currentTok().is(tok::triple_period)) {
      ParentExpr->setIsVariadicArgPackExpansion(true);
      return maybeParseSubExpr(ParentExpr);
   }

   // cast
   if (currentTok().is(tok::as) && !parsingType) {
      auto AsLoc = currentTok().getSourceLoc();
      CastStrength kind = CastStrength ::Normal;

      if (lookahead(false, true).is(tok::question)) {
         advance();
         kind = CastStrength::Fallible;
      }
      else if (lookahead(false, true).is(tok::exclaim)) {
         advance();
         kind = CastStrength::Force;
      }

      advance();

      auto type = parseType();
      if (!type)
         return ParseError();

      auto Cast = CastExpr::Create(Context, AsLoc, kind, ParentExpr,
                                   type.get());

      return maybeParseSubExpr(Cast);
   }

   // 'is' expression
   if (currentTok().is(tok::is) && !parsingType) {
      auto IsLoc = currentTok().getSourceLoc();
      advance();

      auto rhs = parseConstraintExpr();
      if (!rhs)
         return skipUntilProbableEndOfExpr();

      return TypePredicateExpr::Create(Context, IsLoc,
                                       ParentExpr, rhs.getExpr());
   }

   lexer->backtrack();
   return ParentExpr;
}

ParseResult Parser::parseCollectionLiteral()
{
   assert(currentTok().is(tok::open_square));

   SourceLocation LSquareLoc = currentTok().getSourceLoc();

   bool isDictionary = false;
   bool first = true;

   llvm::SmallVector<Expression*, 8> keys;
   llvm::SmallVector<Expression*, 8> values;

   advance();

   while (!currentTok().is(tok::close_square)) {
      if (currentTok().is(tok::eof)) {
         SP.diagnose(err_unexpected_eof, currentTok().getSourceLoc(),  true,
                     "']'");
         return ParseExprError();
      }
      if (first && currentTok().is(tok::colon)) {
         advance();
         isDictionary = true;
         break;
      }

      auto key = parseExprSequence(false, /*stopAtColon*/ true);
      if (!key) {
         skipUntilEven(tok::open_square);
         return ParseError();
      }

      advance();

      if (auto colonLoc = consumeToken(tok::colon)) {
         if (!first && !isDictionary) {
            SP.diagnose(err_unexpected_token, colonLoc, "':'", false);
         }

         auto value = parseExprSequence();
         if (!value) {
            skipUntilProbableEndOfExpr();
            continue;
         }

         advance();

         keys.push_back(key.getExpr());
         values.push_back(value.getExpr());

         isDictionary = true;
      }
      else {
         if (isDictionary) {
            SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                        currentTok().toString(), true, "':'");
         }

         values.push_back(key.getExpr());
      }

      first = false;
      consumeToken(tok::comma);
   }

   if (!currentTok().is(tok::close_square)) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), true, "']'");

      skipUntilProbableEndOfExpr();
   }

   SourceRange SquareRange(LSquareLoc, currentTok().getSourceLoc());
   if (isDictionary)
      return DictionaryLiteral::Create(Context, SquareRange, keys,
                                       values);

   return ArrayLiteral::Create(Context, SquareRange, values);
}

string Parser::prepareStringLiteral(Token const& tok)
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

ParseResult Parser::parseUnaryExpr()
{
   ParseResult Expr;

   if (currentTok().is(tok::at)) {
      return parseAttributedExpr();
   }

   if (currentTok().getIdentifierInfo() == Ident___traits) {
      Expr = parseTraitsExpr();
   }
   else if (currentTok().is(tok::kw_static)) {
      if (!expect(tok::open_paren)) {
         return skipUntilProbableEndOfExpr();
      }

      advance();

      Expr = StaticExpr::Create(Context, parseExprSequence().tryGetExpr());
      expect(tok::close_paren);
   }
   else if (currentTok().is(tok::kw_mixin)) {
      auto loc = currentTok().getSourceLoc();
      if (!expect(tok::open_paren)) {
         return skipUntilProbableEndOfExpr();
      }

      advance();

      auto E = parseExprSequence().tryGetExpr();
      expect(tok::close_paren);

      SourceRange Parens(loc, currentTok().getSourceLoc());
      Expr = MixinExpr::Create(Context, Parens, E);
   }
   else if (currentTok().is(tok::open_square)) {
      Expr = parseCollectionLiteral();
   }
   else if (currentTok().is(tok::open_paren)) {
      Expr = parseParenExpr();
   }
   // enum case with inferred type
   else if (currentTok().is(tok::period)) {
      advance(tok::ident);
      Expr = parseEnumCaseExpr();
   }
   else if (currentTok().is(tok::kw_self)) {
      Expr = maybeParseSubExpr(SelfExpr::Create(Context,
                                                currentTok().getSourceLoc()));
   }
   else if (currentTok().is(tok::kw_super)) {
      Expr = maybeParseSubExpr(SuperExpr::Create(Context,
                                                 currentTok().getSourceLoc()));
   }
   else if (currentTok().is(tok::ident)) {
      auto start = currentTok().getSourceLoc();
      Token next = lookahead(false);

      // function call
      bool isVariadicSizeof = currentTok().getIdentifierInfo() == Ident_sizeof
                              && lookahead().is(tok::triple_period);
      if (isVariadicSizeof || next.is(tok::open_paren)) {
         Expr = parseFunctionCall();
      }
      // single argument lambda
      else if (next.is(tok::arrow_double)) {
         auto ArrowLoc = next.getSourceLoc();
         auto argName = currentTok().getIdentifierInfo();
         auto arg = FuncArgDecl::Create(Context, currentTok().getSourceLoc(),
                                        currentTok().getSourceLoc(), argName,
                                        SourceType(), nullptr, false, true,
                                        false);

         start = currentTok().getSourceLoc();

         advance();
         advance();

         auto body = parseNextStmt().tryGetStatement();
         Expr = LambdaExpr::Create(Context, SourceRange(), ArrowLoc,
                                   SourceType(), { arg }, body);
      }
      else {
         Expr = parseIdentifierExpr();
      }
   }
   else if (currentTok().is(tok::kw_none)) {
      Expr = NoneLiteral::Create(Context, currentTok().getSourceLoc());
   }
   else if (currentTok().oneOf(tok::kw_true, tok::kw_false)) {
      Expr = BoolLiteral::Create(Context, currentTok().getSourceLoc(),
                                 Context.getBoolTy(),
                                 currentTok().is(tok::kw_true));
   }
   else if (currentTok().oneOf(tok::integerliteral, tok::fpliteral,
                               tok::charliteral)) {
      Expr = parseNumericLiteral();
   }
   else if (currentTok().is(tok::stringliteral)) {
      unsigned Offset = currentTok().getSourceLoc().getOffset();
      SourceRange SR(SourceLocation(Offset - 1),
                     SourceLocation(Offset+ currentTok().getText().size()));

      auto strLiteral = StringLiteral::Create(
         Context, SR, prepareStringLiteral(currentTok()));

      if (!lookahead().is(tok::sentinel)) {
         // concatenate adjacent string literals
         if (lookahead().is(tok::stringliteral)) {
            string s = strLiteral->getValue();
            while (lookahead().is(tok::stringliteral)) {
               advance();
               s += prepareStringLiteral(currentTok());
            }

            strLiteral = StringLiteral::Create(Context, SR, move(s));
         }

         Expr = strLiteral;
      }
      else {
         llvm::SmallVector<Expression*, 4> strings{ strLiteral };
         advance();

         while (1) {
            advance();

            auto nextString = parseExprSequence();
            if (!nextString) {
               skipUntilProbableEndOfExpr();
               advance();

               continue;
            }

            strings.push_back(nextString.getExpr());
            advance();

            assert(currentTok().is(tok::sentinel) && "bad interpolation tok");

            if (!lookahead().is(tok::stringliteral)) {
               break;
            }
            else {
               advance();
               auto lit = StringLiteral::Create(
                  Context, SR, prepareStringLiteral(currentTok()));

               strings.push_back(lit);

               if (!lookahead().is(tok::sentinel)) {
                  break;
               }
               else {
                  advance();
               }
            }
         }

         Expr = StringInterpolation::Create(Context, SR, strings);
      }
   }
   else if (currentTok().is(tok::kw_if)) {
      auto IfLoc = currentTok().getSourceLoc();
      advance();

      auto Condition = parseExprSequence(true);
      if (!Condition) {
         return ParseError();
      }

      advance();

      if (currentTok().getIdentifierInfo() != Ident_then)
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                     currentTok().toString(), true, "'then'");

      advance();
      auto Lhs = parseExprSequence();
      if (!Lhs)
         return ParseError();

      expect(tok::kw_else);
      advance();

      auto Rhs = parseExprSequence();
      if (!Rhs)
         return ParseError();

      Expr = IfExpr::Create(Context, IfLoc, Condition.getExpr(), Lhs.getExpr(),
                            Rhs.getExpr());
   }
   else {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), true, "expression");

      return ParseExprError();
   }

   if (!Expr)
      return ParseError();

   return maybeParseSubExpr(Expr.getExpr());
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

Expression* Parser::parseNumericLiteral()
{
   auto kind = currentTok().getKind();

   if (kind == tok::charliteral)
      return parseCharacterLiteral();

   if (kind == tok::fpliteral)
      return parseFloatingPointLiteral();

   return parseIntegerLiteral();
}

Expression* Parser::parseIntegerLiteral()
{
   auto text = currentTok().getText();

   unsigned Offset = currentTok().getSourceLoc().getOffset();
   SourceRange Loc(SourceLocation(Offset),
                   SourceLocation(Offset + text.size() - 1));

   cdot::Type *Ty = Context.getIntTy();

   using Suffix = IntegerLiteral::Suffix;
   Suffix suffix = Suffix::None;

   auto next = lookahead(false, true);
   if (next.is(tok::ident)) {
      auto modifier = next.getIdentifier();
      suffix = llvm::StringSwitch<Suffix>(modifier)
         .Case("u", Suffix::u).Case("i", Suffix::i)
#        define CDOT_BUILTIN_INT(Name, BW, Unsigned)           \
         .Case(#Name, Suffix::Name)
#        include "Basic/BuiltinTypes.def"
         .Default(Suffix::None);

      if (suffix != Suffix::None) {
         advance(false, true);

         switch (suffix) {
#        define CDOT_BUILTIN_INT(Name, BW, Unsigned)           \
         case Suffix::Name:                                    \
            Ty = Context.getBuiltinType(BuiltinType::Name);    \
            break;
#        include "Basic/BuiltinTypes.def"
         case Suffix::u: Ty = Context.getUIntTy(); break;
         case Suffix::i: Ty = Context.getIntTy(); break;
         default:
            llvm_unreachable("bad suffix!");
         }
      }
   }

   uint8_t radix = 10;
   unsigned offset = 0;

   if (text[0] == '0') {
      if (text.size() > 1) {
         if (text[1] == 'x' || text[1] == 'X') {
            offset = 2;
            radix = 16;
         }
         else if (text[1] == 'b' || text[1] == 'B') {
            offset = 2;
            radix = 2;
         }
         else {
            offset = 1;
            radix = 8;
         }
      }
   }

   auto API = llvm::APSInt(llvm::APInt(Ty->getBitwidth(), text.substr(offset),
                                       radix),
                           Ty->isUnsigned());

   return IntegerLiteral::Create(Context, Loc, Ty, std::move(API), suffix);
}

Expression* Parser::parseFloatingPointLiteral()
{
   auto text = currentTok().getText();

   unsigned Offset = currentTok().getSourceLoc().getOffset();
   SourceRange Loc(SourceLocation(Offset),
                   SourceLocation(Offset + text.size()));

   llvm::APFloat APF(0.0);
   (void)APF.convertFromString(text, llvm::APFloat::rmNearestTiesToEven);

   cdot::Type *Ty;
   FPLiteral::Suffix suffix = FPLiteral::Suffix::None;

   if (modifierFollows('f')) {
      suffix = FPLiteral::Suffix::f;
      Ty = Context.getFloatTy();
   }
   else if (modifierFollows('d')) {
      suffix = FPLiteral::Suffix::d;
      Ty = Context.getDoubleTy();
   }
   else {
      Ty = Context.getDoubleTy();
   }

   return FPLiteral::Create(Context, Loc, Ty, std::move(APF), suffix);
}

static uint32_t parseCharacterLiteralImpl(llvm::StringRef text)
{
   if (text.size() == 1)
      return (uint32_t)text.front();

   assert(text.front() == '\\');

   if (text.size() == 2)
      return (uint32_t)support::escape_char(text[1]);

   llvm_unreachable("TODO!");
}

Expression* Parser::parseCharacterLiteral()
{
   auto text = currentTok().getText();

   unsigned Offset = currentTok().getSourceLoc().getOffset();
   SourceRange Loc(SourceLocation(Offset),
                   SourceLocation(Offset + text.size()));

   return CharLiteral::Create(Context, Loc,
                              Context.getCharTy(),
                              parseCharacterLiteralImpl(text));
}

Parser::ParenExprKind Parser::getParenExprKind()
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
      switch (lookahead().getKind()) {
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
            SP.diagnose(err_unexpected_eof, currentTok().getSourceLoc(), 
                        true, "')'");
            SP.diagnose(note_to_match_this, begin);

            return ParenExprKind::Error;
         default:
            break;
      }

      advance();
   }

   advance();

   if (currentTok().is(tok::arrow_single)) {
      maybeFuncTy = true;
      advance();
   }

   if (currentTok().is(tok::arrow_double)) {
      isLambda = true;
   }

   if (isLambda) {
      return ParenExprKind::Lambda;
   }
   else if (maybeFuncTy) {
      return ParenExprKind::FunctionType;
   }
   else if (isTuple) {
      return ParenExprKind::Tuple;
   }

   return ParenExprKind::Expr;
}

ParseResult Parser::parseParenExpr()
{
   ParenExprKind type = getParenExprKind();
   ParseResult expr;

   switch (type) {
   case ParenExprKind::Error:
      return ParseError();
   case ParenExprKind::Lambda:
      expr = parseLambdaExpr();
      break;
   case ParenExprKind::FunctionType: {
      auto FnTy = parseType();
      if (FnTy) {
         return FnTy.get().getTypeExpr();
      }

      return ParseError();
   }
   case ParenExprKind::Expr: {
      SourceLocation LParenLoc = currentTok().getSourceLoc();
      advance();

      expr = parseExprSequence(false, false, /*stopAtNewline=*/ false);
      if (!expr)
         return skipUntilEven(tok::open_paren);

      advance();
      if (!currentTok().is(tok::close_paren)) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                     currentTok().toString(), true, "')'");
      }

      SourceLocation RParenLoc = currentTok().getSourceLoc();

      if (expr.getExpr()->isVariadicArgPackExpansion()) {
         return TupleLiteral::Create(Context, SourceRange(LParenLoc, RParenLoc),
                                     { expr.getExpr()});
      }

      expr = ParenExpr::Create(Context, SourceRange(LParenLoc, RParenLoc),
                               expr.getExpr());

      expr = maybeParseSubExpr(expr.getExpr());

      break;
   }
   case ParenExprKind::Tuple:
      expr = parseTupleLiteral();
      break;
   }

   return expr;
}

ParseResult Parser::parseTupleLiteral()
{
   auto LParenLoc = currentTok().getSourceLoc();
   llvm::SmallVector<Expression*, 4> elements;

   advance();

   while (!currentTok().is(tok::close_paren)) {
      string label;

      if (currentTok().getKind() == tok::ident
          && lookahead().is(tok::colon)) {
         label = lexer->getCurrentIdentifier();
         advance();
      }

      auto exprResult = parseExprSequence();
      if (!exprResult)
         return skipUntilEven(tok::open_paren);

      auto expr = exprResult.getExpr();
      elements.emplace_back(expr);

      advance();

      if (currentTok().is(tok::comma)) {
         advance();
      }
      else if (!currentTok().is(tok::close_paren)) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                     currentTok().toString(), true, "')' or ','");

         break;
      }
   }

   if (!currentTok().is(tok::close_paren)) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), true, "')'");
   }

   SourceRange Parens(LParenLoc, currentTok().getSourceLoc());
   auto tup = TupleLiteral::Create(Context, Parens, elements);

   return maybeParseSubExpr(tup);
}

namespace {

Expression *getExpression(SemaPass &SP,
                          SourceLocation loc,
                          SequenceElement &El) {
   switch (El.getKind()) {
      case SequenceElement::EF_Operator: {
         auto *II = &SP.getContext().getIdentifiers().get(
            op::toString(El.getOperatorKind()));

         return new(SP.getContext()) IdentifierRefExpr(loc, II);
      }
      case SequenceElement::EF_Expression:
         return El.getExpr();
      case SequenceElement::EF_PossibleOperator:
         return new(SP.getContext()) IdentifierRefExpr(loc, move(El.getOp()));
   }
}

} // anonymous namespace

static IdentifierRefExpr *getSimpleIdentifier(Expression *Expr)
{
   auto Ident = dyn_cast<IdentifierRefExpr>(Expr);
   if (!Ident || Ident->isVariadicArgPackExpansion()
       || !Ident->getTemplateArgs().empty())
      return nullptr;

   return Ident;
}

ParseResult Parser::parseExprSequence(bool stopAtThen,
                                      bool stopAtColon,
                                      bool stopAtNewline) {
   auto start = currentTok().getSourceLoc();

   std::vector<SequenceElement> frags;

   bool done = false;
   while (!done) {
      if (stopAtThen && currentTok().getIdentifierInfo() == Ident_then) {
         lexer->backtrack();
         break;
      }
      if (stopAtColon && currentTok().is(tok::colon)) {
         lexer->backtrack();
         break;
      }

      switch (currentTok().getKind()) {
      case tok::ident: {
         auto expr = parseUnaryExpr();
         if (!expr)
            break;

         if (auto Ident = getSimpleIdentifier(expr.getExpr())) {
            frags.emplace_back(Ident->getIdentInfo(),
                               Ident->getSourceLoc());
         }
         else {
            frags.emplace_back(expr.getExpr());
         }

         break;
      }
      case tok::op_ident:
         frags.emplace_back(currentTok().getIdentifierInfo(),
                            currentTok().getSourceLoc());

         break;
#  define CDOT_OPERATOR_TOKEN(Name, Spelling)                              \
      case tok::Name:
#  include "Lex/Tokens.def"

         frags.emplace_back(op::fromString(currentTok().toString()),
                            currentTok().getSourceLoc());

         break;
      case tok::underscore:
         if (frags.empty()) {
            auto *II = &Context.getIdentifiers().get("_");
            frags.emplace_back(II, currentTok().getSourceLoc());
            break;
         }

         LLVM_FALLTHROUGH;
      case tok::comma: case tok::semicolon: case tok::close_paren:
      case tok::newline: case tok::eof: case tok::sentinel:
      case tok::open_brace: case tok::close_brace: case tok::close_square:
      case tok::arrow_double: case tok::arrow_single:
         lexer->backtrack();
         done = true;
         break;
      default: {
         auto seqResult = parseUnaryExpr();
         if (!seqResult) {
            return ParseError();
         }

         frags.emplace_back(seqResult.getExpr());
         break;
      }
      }

      if (!done)
         advance(!stopAtNewline);
   }

   if (frags.size() == 1)
      return getExpression(SP, start, frags.front());

   if (frags.empty()) {
      if (!lookahead().is(tok::eof))
         advance();

      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), true, "expression");

      return ParseError();
   }

   return ExprSequence::Create(Context, frags);
}

ParseResult Parser::parseConstraintExpr()
{
   auto Loc = currentTok().getSourceLoc();
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
         if (currentTok().getIdentifierInfo() == Ident_default) {
            kind = ConstraintExpr::DefaultConstructible;
            break;
         }
         LLVM_FALLTHROUGH;
      default: {
         auto Ty = parseType();
         if (!Ty)
            return ParseError();

         return ConstraintExpr::Create(Context, Loc, Ty.get());
      }
   }

   return ConstraintExpr::Create(Context, Loc, kind);
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

ParseResult Parser::parseTraitsExpr()
{
   auto TraitsLoc = currentTok().getSourceLoc();
   if (!expect(tok::open_paren)) {
      return skipUntilProbableEndOfExpr();
   }

   auto LParenLoc = currentTok().getSourceLoc();
   if (!expect(tok::ident)) {
      return skipUntilEven(tok::open_paren);
   }

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

   if (kind == TraitsExpr::Invalid) {
      SP.diagnose(err_invalid_traits, currentTok().getSourceLoc(),  str);
      return skipUntilEven(tok::open_paren);
   }

   std::vector<TraitsArgument> args;
   auto &argKinds = traitArgs[kind];

   size_t i = 0;
   while (argKinds.args[i] != TraitArguments::None) {
      advance();
      if (currentTok().is(tok::comma))
         advance();

      switch (argKinds.args[i]) {
         case TraitArguments::Expr: {
            auto expr = parseExprSequence();
            if (!expr) {
               skipUntilProbableEndOfExpr();
               break;
            }

            args.emplace_back(expr.getExpr());
            break;
         }
         case TraitArguments::Stmt: {
            auto stmt = parseNextStmt();
            if (!stmt) {
               skipUntilProbableEndOfStmt();
               break;
            }

            args.emplace_back(stmt.getStatement());
            break;
         }
         case TraitArguments::Type: {
            auto typeResult = parseType();
            if (!typeResult) {
               skipUntilProbableEndOfExpr();
               break;
            }

            args.emplace_back(typeResult.get());
            break;
         }
         case TraitArguments::Identifier:
            if (!currentTok().is(tok::ident)) {
               SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                           currentTok().toString(), true, "identifier");

               skipUntilProbableEndOfExpr();
               break;
            }

            args.emplace_back(currentTok().getIdentifierInfo()
                                          ->getIdentifier());
            break;
         case TraitArguments::String:
            if (!currentTok().is(tok::stringliteral)) {
               SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                           currentTok().toString(), true, "string literal");

               skipUntilProbableEndOfExpr();
               break;
            }

            args.emplace_back(currentTok().getText());
            break;
         default:
            llvm_unreachable("bad arg kind");
      }

      ++i;
   }

   expect(tok::close_paren);

   SourceRange Parens(LParenLoc, currentTok().getSourceLoc());
   return TraitsExpr::Create(Context, TraitsLoc, Parens, kind, args);
}

ParseResult Parser::parseVarDecl()
{
   AccessModifier access = AccessModifier::DEFAULT;
   bool isLet = false;
   auto VarOrLetLoc = currentTok().getSourceLoc();

   while (currentTok().is_keyword()) {
      switch (currentTok().getKind()) {
         case tok::kw_public:
         case tok::kw_private:
            if (access != AccessModifier::DEFAULT)
               SP.diagnose(err_duplicate_access_spec,
                           currentTok().getSourceLoc());

            access = tokenToAccessSpec(currentTok().getKind());
            break;
         case tok::kw_let:
            isLet = true;
            LLVM_FALLTHROUGH;
         case tok::kw_var:
            break;
         default:
            SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                        currentTok().toString(), false);

            break;
      }

      advance();
   }

   if (currentTok().is(tok::open_paren))
      return parseDestructuringDecl(access, isLet);

   if (!currentTok().is(tok::ident)) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), true, "identifier");

      return skipUntilProbableEndOfStmt();
   }

   auto Name = currentTok().getIdentifierInfo();

   SourceType type;
   Expression *value = nullptr;
   SourceLocation ColonLoc;

   if (lookahead().is(tok::colon)) {
      ColonLoc = consumeToken();
      advance();

      auto typeResult = parseType(true);
      if (typeResult)
         type = typeResult.get();
      else
         skipUntilProbableEndOfExpr();
   }

   if (!type.isValid()) {
      type = SourceType(Context.getAutoType());
   }

   if (lookahead().is(tok::equals)) {
      advance();
      advance();

      auto valueResult = parseExprSequence();
      if (valueResult)
         value = valueResult.getExpr();
      else
         return skipUntilProbableEndOfStmt();
   }

   if (SP.getDeclContext().isGlobalDeclContext()) {
      auto G = GlobalVarDecl::Create(Context, access, VarOrLetLoc,
                                     ColonLoc, isLet, Name, type, value);

      SP.addDeclToContext(SP.getDeclContext(), G);
      return G;
   }
   else {
      auto L = LocalVarDecl::Create(Context, access, VarOrLetLoc,
                                    ColonLoc, isLet, Name, type, value);

      L->setDeclContext(&SP.getDeclContext());
      return L;
   }
}

ParseResult Parser::parseDestructuringDecl(AccessModifier access,
                                           bool isLet) {
   assert(currentTok().is(tok::open_paren) && "should not be called otherwise");

   llvm::SmallVector<VarDecl *, 8> decls;
   bool global = SP.getDeclContext().isGlobalDeclContext();
   SourceLocation VarOrLetLoc = currentTok().getSourceLoc();

   while (!currentTok().is(tok::close_paren)) {
      if (!expect(tok::ident)) {
         skipUntilEven(tok::open_paren);
         break;
      }

      auto loc = currentTok().getSourceLoc();
      auto Name = currentTok().getIdentifierInfo();
      if (global) {
         auto G = GlobalVarDecl::Create(Context, access, VarOrLetLoc,
                                        loc, isLet, Name, SourceType(),
                                        nullptr);

         SP.addDeclToContext(SP.getDeclContext(), G);
         decls.push_back(G);
      }
      else {
         decls.push_back(LocalVarDecl::Create(Context, access, VarOrLetLoc,
                                              loc, isLet, Name, SourceType(),
                                              nullptr));
      }

      if (lookahead().is(tok::comma))
         advance();
   }

   SourceType type;
   Expression *value = nullptr;

   if (lookahead().is(tok::colon)) {
      advance();
      advance();

      auto typeResult = parseType();
      if (typeResult)
         type = typeResult.get();
      else
         skipUntilProbableEndOfExpr();
   }

   if (!type)
      type = SourceType(Context.getAutoType());

   if (lookahead().is(tok::equals)) {
      advance();
      advance();

      auto valueResult = parseExprSequence();
      if (valueResult)
         value = valueResult.getExpr();
      else
         skipUntilProbableEndOfExpr();
   }

   SourceRange SR(VarOrLetLoc, currentTok().getSourceLoc());
   if (global) {
      return GlobalDestructuringDecl::Create(Context, SR, access, isLet, decls,
                                             type, value);
   }
   else {
      return LocalDestructuringDecl::Create(Context, SR, access, isLet, decls,
                                            type, value);
   }
}

std::vector<FuncArgDecl*> Parser::parseFuncArgs(SourceLocation &varargLoc)
{
   SourceLocation start;
   std::vector<FuncArgDecl*> args;

   if (!currentTok().is(tok::open_paren)) {
      lexer->backtrack();
      return args;
   }

   bool foundDefault = false;
   varargLoc = SourceLocation();

   advance();

   while (!currentTok().is(tok::close_paren)) {
      if (varargLoc) {
         SP.diagnose(err_vararg_must_be_last, currentTok().getSourceLoc());
         SP.diagnose(note_previous_vararg_here, varargLoc);
      }

      start = currentTok().getSourceLoc();

      IdentifierInfo *argName = nullptr;
      SourceType argType;
      Expression* defaultVal = nullptr;
      bool templateArgExpansion = false;
      bool isConst = true;

      SourceLocation VarOrLetLoc;
      if (currentTok().oneOf(tok::kw_var, tok::kw_let)) {
         isConst = currentTok().is(tok::kw_let);
         VarOrLetLoc = consumeToken();
      }
      else {
         VarOrLetLoc = currentTok().getSourceLoc();
      }

      SourceLocation ColonLoc;
      if (lookahead().is(tok::colon)) {
         if (currentTok().getKind() != tok::ident) {
            SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                        currentTok().toString(), true, "identifier");
         }

         argName = currentTok().getIdentifierInfo();
         ColonLoc = consumeToken();
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

      auto typeResult = parseType();
      if (typeResult)
         argType = typeResult.get();
      else {
         argType = SourceType(Context.getAutoType());
         skipUntilProbableEndOfExpr();
      }

      advance();

      if (argType.getTypeExpr()
          && argType.getTypeExpr()->isVariadicArgPackExpansion()) {
         templateArgExpansion = true;
      }

      // optional default value
      if (currentTok().is(tok::equals)) {
         if (varargLoc) {
            SP.diagnose(err_vararg_default_value, currentTok().getSourceLoc());
         }

         advance();

         auto defaultValResult = parseExprSequence();
         if (defaultValResult) {
            defaultVal = defaultValResult.getExpr();
         }
         else {
            skipUntilProbableEndOfExpr();
         }

         foundDefault = true;
         advance();

      }
      else if (foundDefault) {
         SP.diagnose(err_expected_default_value, currentTok().getSourceLoc());
      }

      auto argDecl = FuncArgDecl::Create(Context, VarOrLetLoc, ColonLoc,
                                         argName, argType, defaultVal,
                                         templateArgExpansion, isConst,
                                         /*vararg=*/ false);

      args.push_back(argDecl);

      // end of argument list or next argument
      if (currentTok().is(tok::comma)) {
         advance();
      }
      else if (!currentTok().is(tok::close_paren)) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                     currentTok().toString(), true, "')'");

         break;
      }
   }

   return args;
}

ParseResult Parser::parseFunctionDecl()
{
   auto DefLoc = currentTok().getSourceLoc();
   AccessModifier am = maybeParseAccessModifier();
   OperatorInfo OpInfo;
   bool IsOperator = false;

   if (lookahead().oneOf(tok::kw_infix, tok::kw_prefix, tok::kw_postfix)) {
      advance();

      switch (currentTok().getKind()) {
         case tok::kw_infix: OpInfo.setFix(FixKind::Infix); break;
         case tok::kw_prefix: OpInfo.setFix(FixKind::Prefix); break;
         case tok::kw_postfix: OpInfo.setFix(FixKind::Postfix); break;
         default:
            llvm_unreachable("bad fix kind");
      }

      IsOperator = true;
   }

   advance();

   DeclarationName funcName;
   SourceType returnType;
   bool isCastOp;

   if (IsOperator) {
      funcName = parseOperatorName(OpInfo, isCastOp, returnType);
   }
   else {
      if (!expectToken(tok::ident)) {
         if (!findTokOnLine(tok::ident))
            return skipUntilProbableEndOfStmt();
      }

      funcName = currentTok().getIdentifierInfo();
   }

   auto templateParams = tryParseTemplateParameters();
   advance();

   SourceLocation varargLoc;
   auto args = parseFuncArgs(varargLoc);

   // optional return type
   if (lookahead().is(tok::arrow_single)) {
      advance();
      advance();

      auto TypeRes = parseType();
      if (TypeRes)
         returnType = TypeRes.get();
   }

   if (!returnType) {
      returnType = SourceType(Context.getAutoType());
   }

   std::vector<StaticExpr*> constraints;
   while (lookahead().is(tok::kw_where)) {
      advance();
      advance();

      auto constraintResult = parseExprSequence();
      if (constraintResult) {
         constraints.push_back(StaticExpr::Create(Context,
                                                  constraintResult.getExpr()));
      }
   }

   auto funcDecl = FunctionDecl::Create(Context, am, DefLoc,
                                        funcName, std::move(args), returnType,
                                        nullptr, OpInfo, move(templateParams));

   CompoundStmt* body = nullptr;
   if (lookahead().is(tok::open_brace)) {
      DeclContextRAII declContextRAII(*this, funcDecl);
      body = parseBlock().tryGetStatement<CompoundStmt>();
   }

   funcDecl->setCstyleVararg(varargLoc.isValid());
   funcDecl->setBody(body);

   Context.setConstraints(funcDecl, constraints);

   SP.ActOnFunctionDecl(funcDecl);
   return funcDecl;
}

ParseResult Parser::parseLambdaExpr()
{
   llvm::SmallVector<FuncArgDecl*, 4> args;

   SourceLocation LParenLoc = currentTok().getSourceLoc();
   if (currentTok().is(tok::open_paren)) {
      advance();

      while (!currentTok().is(tok::close_paren)) {
         if (!currentTok().is(tok::ident)) {
            SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                        currentTok().toString(), true, "identifier");

            advance();
            continue;
         }

         auto loc = currentTok().getSourceLoc();
         auto name = currentTok().getIdentifierInfo();
         SourceType argType;

         SourceLocation ColonLoc;
         if (lookahead().is(tok::colon)) {
            ColonLoc = consumeToken();
            advance();

            argType = parseType().get();
         }

         if (!argType)
            argType = SourceType(Context.getAutoType());


         args.emplace_back(FuncArgDecl::Create(Context, loc, ColonLoc, name,
                                               argType, nullptr, false, true));

         advance();
         if (currentTok().is(tok::comma))
            advance();
      }
   }
   else {
      assert(currentTok().is(tok::ident) && "not begin of lambda expr!");

      SourceLocation Loc = currentTok().getSourceLoc();
      IdentifierInfo *Name = currentTok().getIdentifierInfo();

      args.emplace_back(FuncArgDecl::Create(Context, Loc, Loc, Name,
                                            SourceType(Context.getAutoType()),
                                            nullptr, false, true));
   }

   SourceRange Parens(LParenLoc, currentTok().getSourceLoc());

   advance();
   SourceType retType;

   if (currentTok().is(tok::arrow_single)) {
      advance();
      retType = parseType().get();

      advance();
   }

   if (!retType)
      retType = SourceType(Context.getAutoType());

   assert(currentTok().is(tok::arrow_double));
   SourceLocation ArrowLoc = currentTok().getSourceLoc();

   advance();

   Statement* body = parseNextStmt().tryGetStatement();
   auto lambdaExpr = LambdaExpr::Create(Context, Parens, ArrowLoc, retType,
                                        args, body);

   return maybeParseSubExpr(lambdaExpr);
}

std::vector<TemplateParamDecl*> Parser::tryParseTemplateParameters()
{
   std::vector<TemplateParamDecl*> templateArgs;
   if (!lookahead().is(tok::open_square))
      return templateArgs;

   advance();

   bool defaultGiven = false;

   advance();
   while (!currentTok().is(tok::close_square)) {
      SourceLocation TypeNameOrValueLoc;
      SourceLocation EllipsisLoc;

      bool variadic = false;
      bool isTypeName = true;

      if (!currentTok().is(tok::ident)) {
         if (currentTok().getIdentifierInfo() == Ident_typename) {
            // default
            TypeNameOrValueLoc = consumeToken();
         }
         else if (currentTok().getIdentifierInfo() == Ident_value) {
            isTypeName = false;
            TypeNameOrValueLoc = consumeToken();
         }
      }

      if (currentTok().is(tok::triple_period)) {
         EllipsisLoc = currentTok().getSourceLoc();
         variadic = true;
         advance();
      }

      if (currentTok().getKind() != tok::ident) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                     currentTok().toString(), true, "identifier");

         skipUntilProbableEndOfStmt();
         continue;
      }

      SourceLocation NameLoc = currentTok().getSourceLoc();
      auto Name = currentTok().getIdentifierInfo();

      SourceType covariance;
      SourceType contravariance;

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
                     SP.diagnose(err_covar_convar_already_specified,
                                 currentTok().getSourceLoc(), 0);
                  }

                  covarSet = true;
                  covariance = parseType().get();
               }
               else {
                  if (contravarSet) {
                     SP.diagnose(err_covar_convar_already_specified,
                                 currentTok().getSourceLoc(), 1);
                  }

                  contravarSet = true;
                  contravariance = parseType().get();
               }

               if (lookahead().is(tok::comma)
                   || lookahead().is(tok::greater)) {
                  break;
               }

               advance();
            }
         }
         else {
            covariance = parseType().get();
         }
      }

      advance();

      Expression *defaultValue = nullptr;
      if (currentTok().is(tok::equals)) {
         defaultGiven = true;
         advance();

         if (isTypeName) {
            defaultValue = parseType().get().getTypeExpr();
         }
         else {
            defaultValue = StaticExpr::Create(Context,
                                              parseExprSequence().tryGetExpr());
         }

         advance();
      }
      else if (defaultGiven) {
         lexer->backtrack();
         SP.diagnose(err_expected_default_value, currentTok().getSourceLoc());
      }

      if (!covariance)
         covariance = SourceType(Context.getAutoType());

      if (!contravariance)
         contravariance = SourceType(Context.getAutoType());

      if (isTypeName) {
         templateArgs.push_back(
            TemplateParamDecl::Create(Context, Name, covariance,
                                      contravariance, defaultValue,
                                      TypeNameOrValueLoc, NameLoc,
                                      EllipsisLoc));
      }
      else {
         templateArgs.push_back(
            TemplateParamDecl::Create(Context, Name, covariance,
                                      defaultValue, TypeNameOrValueLoc,
                                      NameLoc, EllipsisLoc));
      }

      if (currentTok().is(tok::comma)) {
         advance();
      }
   }

   return templateArgs;
}

ParseResult Parser::parseIfStmt()
{
   auto IfLoc = currentTok().getSourceLoc();
   advance();

   auto cond = parseExprSequence().tryGetExpr();
   advance();

   auto ifBranch = parseNextStmt().tryGetStatement();
   Statement* elseBranch = nullptr;

   if (lookahead().is(tok::kw_else)) {
      advance();
      advance();

      elseBranch = parseNextStmt().tryGetStatement();
   }

   if (!elseBranch) {
      elseBranch = NullStmt::Create(Context, currentTok().getSourceLoc());
   }

   return IfStmt::Create(Context, IfLoc, cond, ifBranch, elseBranch);
}

ParseResult Parser::parseStaticIf()
{
   if (inGlobalDeclContext())
      return parseStaticIfDecl();

   auto StaticLoc = currentTok().getSourceLoc();
   advance(); // static

   auto IfLoc = currentTok().getSourceLoc();
   advance(); // if

   auto cond = StaticExpr::Create(Context, parseExprSequence().tryGetExpr());
   advance();

   Statement* ifBranch;
   Statement* elseBranch = nullptr;

   ifBranch = parseNextStmt().tryGetStatement();

   if (lookahead().is(tok::kw_else)) {
      advance();
      advance();

      elseBranch = parseNextStmt().tryGetStatement();
   }

   if (!elseBranch) {
      elseBranch = NullStmt::Create(Context, currentTok().getSourceLoc());
   }

   if (auto Compound = dyn_cast<CompoundStmt>(ifBranch)) {
      Compound->setPreserveScope(true);
   }
   if (auto Compound = dyn_cast_or_null<CompoundStmt>(elseBranch)) {
      Compound->setPreserveScope(true);
   }


   return new(Context) StaticIfStmt(StaticLoc, IfLoc, cond, ifBranch,
                                    elseBranch);
}

ParseResult Parser::parseStaticIfDecl()
{
   SourceLocation StaticLoc = currentTok().getSourceLoc();
   SourceLocation RBRaceLoc;

   advance(); // static
   advance(); // if

   auto cond = StaticExpr::Create(Context, parseExprSequence().tryGetExpr());
   advance();

   CompoundDecl *IfDecl = CompoundDecl::Create(Context,
                                               currentTok().getSourceLoc(),
                                               false);

   if (!currentTok().is(tok::open_brace)) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), true, "'{'");
   }
   else {
      DeclContextRAII declContextRAII(*this, IfDecl);
      advance();

      while (!currentTok().is(tok::close_brace)) {
         if (currentTok().is(tok::eof)) {
            SP.diagnose(err_unexpected_eof, currentTok().getSourceLoc());
            return ParseStmtError();
         }

         auto nextDecl = parseNextDecl();
         if (nextDecl && !nextDecl.holdsDecl()) {
            SP.diagnose(err_expected_declaration,
                        nextDecl.getDecl()->getSourceLoc(), "static if");
         }

         advance();
      }

      RBRaceLoc = currentTok().getSourceLoc();
   }

   CompoundDecl *ElseDecl = nullptr;
   if (lookahead().is(tok::kw_else)) {
      advance();
      advance();

      if (!currentTok().is(tok::open_brace)) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                     currentTok().toString(), true, "'{'");
      }
      else {
         ElseDecl = CompoundDecl::Create(Context,
                                         currentTok().getSourceLoc(), false);

         DeclContextRAII declContextRAII(*this, ElseDecl);
         advance();

         while (!currentTok().is(tok::close_brace)) {
            if (currentTok().is(tok::eof)) {
               SP.diagnose(err_unexpected_eof, currentTok().getSourceLoc());
               return ParseStmtError();
            }

            auto nextDecl = parseNextDecl();
            if (nextDecl && !nextDecl.holdsDecl()) {
               SP.diagnose(err_expected_declaration,
                           nextDecl.getDecl()->getSourceLoc(), "static if");
            }

            advance();
         }

         RBRaceLoc = currentTok().getSourceLoc();
      }
   }

   auto Decl = StaticIfDecl::Create(Context, StaticLoc, RBRaceLoc, cond, IfDecl,
                                    ElseDecl);

   SP.addDeclToContext(SP.getDeclContext(), Decl);
   return Decl;
}

ParseResult Parser::parseStaticFor()
{
   if (inGlobalDeclContext())
      return parseStaticForDecl();

   auto StaticLoc = currentTok().getSourceLoc();
   advance(); // static

   auto ForLoc = currentTok().getSourceLoc();
   advance(); // for

   llvm::StringRef ident;
   if (!currentTok().is(tok::ident)) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), true, "identifier");

      if (!findTokOnLine(Ident_in)) {
         if (findTokOnLine(tok::open_brace)) {
            return skipUntilEven(tok::open_brace);
         }

         return skipUntilProbableEndOfStmt();
      }
   }
   else {
      ident = lexer->getCurrentIdentifier();
      advance();
   }

   if (currentTok().getIdentifierInfo() != Ident_in) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), true, "'in'");

      if (!findTokOnLine(Ident_in)) {
         if (findTokOnLine(tok::open_brace)) {
            return skipUntilEven(tok::open_brace);
         }

         return skipUntilProbableEndOfStmt();
      }
   }

   advance();

   auto range = StaticExpr::Create(Context, parseExprSequence().tryGetExpr());
   advance();

   Statement* body = parseNextStmt().tryGetStatement();
   if (auto Compound = dyn_cast_or_null<CompoundStmt>(body)) {
      Compound->setPreserveScope(true);
   }

   return new(Context) StaticForStmt(StaticLoc, ForLoc, ident, range, body);
}

ParseResult Parser::parseStaticForDecl()
{
   SourceLocation StaticLoc = currentTok().getSourceLoc();
   SourceLocation RBRaceLoc;

   advance(); // static
   advance(); // for

   IdentifierInfo *ident = nullptr;
   if (!currentTok().is(tok::ident)) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), true, "identifier");

      if (!findTokOnLine(Ident_in)) {
         if (findTokOnLine(tok::open_brace)) {
            advance();
            return skipUntilEven(tok::open_brace);
         }

         return skipUntilProbableEndOfStmt();
      }
   }
   else {
      ident = currentTok().getIdentifierInfo();
      advance();
   }

   if (currentTok().getIdentifierInfo() != Ident_in)
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), true, "'in'");

   advance();

   auto range = StaticExpr::Create(Context, parseExprSequence().tryGetExpr());
   advance();

   CompoundDecl *BodyDecl = CompoundDecl::Create(Context,
                                                 currentTok().getSourceLoc(),
                                                 false);

   if (!currentTok().is(tok::open_brace)) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), true, "'{'");
   }
   else {
      DeclContextRAII declContextRAII(*this, BodyDecl);
      advance();

      while (!currentTok().is(tok::close_brace)) {
         if (currentTok().is(tok::eof)) {
            SP.diagnose(err_unexpected_eof, currentTok().getSourceLoc());
            return ParseStmtError();
         }

         auto nextDecl = parseNextDecl();
         if (nextDecl && !nextDecl.holdsDecl()) {
            SP.diagnose(err_expected_declaration,
                        nextDecl.getDecl()->getSourceLoc(), "static if");
         }

         advance();
      }

      RBRaceLoc = currentTok().getSourceLoc();
   }

   auto Decl = StaticForDecl::Create(Context, StaticLoc, RBRaceLoc, ident,
                                     range, BodyDecl);

   SP.addDeclToContext(SP.getDeclContext(), Decl);
   return Decl;
}

ParseResult Parser::parseStaticAssert()
{
   auto Loc = currentTok().getSourceLoc();
   if (!expect(tok::open_paren)) {
      return skipUntilProbableEndOfExpr();
   }

   SourceLocation LParenLoc = consumeToken();

   auto expr = StaticExpr::Create(Context, parseExprSequence().tryGetExpr());

   string msg;
   if (lookahead().is(tok::comma)) {
      advance();
      advance();

      auto StringLit = parseUnaryExpr();
      if (StringLit) {
         if (!isa<StringLiteral>(StringLit.getExpr()))
            SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                        currentTok().toString(), true, "string literal");
         else
            msg = cast<StringLiteral>(StringLit.getExpr())->getValue();
      }
   }

   expect(tok::close_paren);

   SourceRange Parens(LParenLoc, currentTok().getSourceLoc());
   auto Assert = StaticAssertStmt::Create(Context, Loc, Parens,
                                          expr, move(msg));

   SP.addDeclToContext(SP.getDeclContext(), Assert);
   return Assert;
}

ParseResult Parser::parseStaticPrint()
{
   auto Loc = currentTok().getSourceLoc();
   if (!expect(tok::open_paren)) {
      return skipUntilProbableEndOfExpr();
   }

   SourceLocation LParenLoc = consumeToken();

   auto expr = StaticExpr::Create(Context, parseExprSequence().tryGetExpr());
   expect(tok::close_paren);

   SourceRange Parens(LParenLoc, currentTok().getSourceLoc());
   auto PrintStmt = StaticPrintStmt::Create(Context, Loc, Parens, expr);

   SP.addDeclToContext(SP.getDeclContext(), PrintStmt);
   return PrintStmt;
}

ParseResult Parser::parsePattern()
{
   if (currentTok().is(tok::is)) {
      auto IsLoc = currentTok().getSourceLoc();
      advance();

      auto TypeRes = parseType();
      if (!TypeRes)
         return ParseError();

      SourceRange SR(IsLoc, lookahead().getSourceLoc());
      return IsPattern::Create(Context, SR, TypeRes.get());
   }

   if (currentTok().is(tok::period)) {
      auto PeriodLoc = currentTok().getSourceLoc();
      advance();

      auto *caseName = currentTok().getIdentifierInfo();
      std::vector<CasePatternArgument> args;

      if (!lookahead().is(tok::open_paren)) {
         SourceRange SR(PeriodLoc, lookahead().getSourceLoc());
         return CasePattern::Create(Context, SR, caseName, args);
      }

      advance();
      while (!currentTok().is(tok::close_paren)) {
         advance();

         auto loc = currentTok().getSourceLoc();
         if (currentTok().oneOf(tok::kw_let, tok::kw_var)) {
            bool isConst = currentTok().is(tok::kw_let);
            if (!expect(tok::ident)) {
               return skipUntilProbableEndOfStmt(tok::colon);
            }

            auto ident = currentTok().getIdentifierInfo();
            args.emplace_back(ident, isConst, loc);
         }
         else {
            auto expr = parseExprSequence();
            if (expr)
               args.emplace_back(expr.getExpr(), loc);
         }

         switch (lookahead().getKind()) {
            case tok::comma:
            case tok::close_paren:
               advance();
               break;
            default:
               SP.diagnose(err_unexpected_token, lookahead().getSourceLoc(),
                           lookahead().toString(), false);
         }
      }

      SourceRange SR(PeriodLoc, lookahead().getSourceLoc());
      return CasePattern::Create(Context, SR, move(caseName), args);
   }

   auto ExprRes = parseExprSequence(false, true);
   if (!ExprRes)
      return ParseError();

   return ExpressionPattern::Create(Context, lookahead().getSourceLoc(),
                                    ExprRes.getExpr());
}

void Parser::parseCaseStmts(llvm::SmallVectorImpl<CaseStmt*> &Cases)
{
   while (!currentTok().is(tok::close_brace)) {
      auto start = currentTok().getSourceLoc();
      PatternExpr *patternExpr = nullptr;

      if (currentTok().is(tok::kw_case)) {
         advance();
         patternExpr = parsePattern().tryGetExpr<PatternExpr>();
      }
      else if (!currentTok().is(Ident_default)) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                     currentTok().toString(), true, "'case' or 'default'");

         skipUntilEven(tok::open_brace);
         return;
      }

      expect(tok::colon);

      if (lookahead().oneOf(tok::kw_case, Ident_default)) {
         Cases.push_back(CaseStmt::Create(Context, start, patternExpr));
         advance();
         continue;
      }

      std::vector<Statement*> Stmts;
      while (!lookahead().oneOf(tok::kw_case, tok::close_brace,
                                Ident_default)) {
         advance();
         Stmts.push_back(parseNextStmt().tryGetStatement());
      }

      if (Stmts.empty()) {
         SP.diagnose(err_last_match_case_empty, currentTok().getSourceLoc());
      }

      if (Stmts.size() == 1) {
         Cases.push_back(CaseStmt::Create(Context, start, patternExpr,
                                          Stmts.front()));
      }
      else {
         auto Compound = CompoundStmt::Create(Context, Stmts, false,
                                              Stmts.front()->getSourceLoc(),
                                              currentTok().getSourceLoc());

         Cases.push_back(CaseStmt::Create(Context, start, patternExpr,
                                          Compound));
      }

      advance();
   }
}

ParseResult Parser::parseMatchStmt()
{
   auto MatchLoc = currentTok().getSourceLoc();
   advance();

   Expression* matchVal = parseExprSequence().tryGetExpr();
   advance();

   if (!currentTok().is(tok::open_brace)) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), true, "'{'");

      return ParseStmtError();
   }

   auto LBraceLoc = currentTok().getSourceLoc();
   advance();

   llvm::SmallVector<CaseStmt*, 8> Cases;
   parseCaseStmts(Cases);

   SourceRange Braces(LBraceLoc, currentTok().getSourceLoc());
   return MatchStmt::Create(Context, MatchLoc, Braces, matchVal, Cases);
}

ParseResult Parser::parseWhileStmt(bool conditionBefore)
{
   auto WhileLoc = currentTok().getSourceLoc();
   advance();

   Expression* cond = nullptr;
   if (conditionBefore) {
      cond = parseExprSequence().tryGetExpr();
      advance();
   }

   auto body = parseNextStmt().tryGetStatement();
   if (!conditionBefore && lookahead().is(tok::kw_while)) {
      advance();
      advance();

      cond = parseExprSequence().tryGetExpr();
   }

   if (!cond)
      cond = BoolLiteral::Create(Context, WhileLoc, Context.getBoolTy(), true);

   return WhileStmt::Create(Context, WhileLoc, cond, body,
                            !conditionBefore);
}

ParseResult Parser::parseForStmt()
{
   auto ForLoc = currentTok().getSourceLoc();
   advance();

   Statement* init = nullptr;
   Decl *initDecl = nullptr;

   if (!currentTok().is(tok::semicolon)) {
      auto initResult = parseNextStmt();
      if (initResult.holdsDecl()) {
         initDecl = initResult.getDecl();
      }
      else if (initResult.holdsDecl()) {
         init = initResult.getStatement();
      }
      else if (initResult.holdsExpr()) {
         init = initResult.getExpr();
      }

      advance();
   }

   // range based for loop
   if (currentTok().getIdentifierInfo() == Ident_in) {
      advance();

      Expression* range = parseExprSequence().tryGetExpr();
      advance();

      if (currentTok().is(tok::close_paren)) {
         advance();
      }

      auto body = parseNextStmt().tryGetStatement();
      return ForInStmt::Create(Context, ForLoc, cast<LocalVarDecl>(initDecl),
                               range, body);
   }

   if (initDecl) {
      init = DeclStmt::Create(Context, initDecl);
   }

   if (!currentTok().is(tok::semicolon)) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), true, "';'");
   }

   Expression* term;
   advance();

   if (currentTok().is(tok::semicolon)) {
      term = BoolLiteral::Create(Context, ForLoc, Context.getBoolTy(), true);
      advance();
   }
   else {
      term = parseExprSequence().tryGetExpr();
      advance();
   }

   if (!currentTok().is(tok::semicolon)) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), true, "';'");
   }

   Statement* inc = nullptr;
   advance();

   if (!currentTok().is(tok::open_brace)) {
      auto incResult = parseNextStmt();
      if (incResult.holdsDecl()) {
         inc = DeclStmt::Create(Context, incResult.getDecl());
      }
      else if (incResult.holdsDecl()) {
         inc = incResult.getStatement();
      }
      else if (incResult.holdsExpr()) {
         inc = incResult.getExpr();
      }

      advance();
   }

   return ForStmt::Create(Context, ForLoc, init, term, inc,
                          parseNextStmt().tryGetStatement());
}

ParseResult Parser::parseDeclareStmt()
{
   advance();

   bool HasLang = false;
   ExternAttr::LangKind Lang;

   if (currentTok().getKind() == tok::ident) {
      auto str = lexer->getCurrentIdentifier();
      if (str == "C") {
         Lang = ExternAttr::C;
         HasLang = true;
      }
      else if (str == "CXX" || str == "CPP") {
         Lang = ExternAttr::CXX;
         HasLang = true;
      }
      else if (str == "__native") {
//         externKind = ExternKind::Native;
      }
      else {
         SP.diagnose(err_bad_extern_kind, currentTok().getSourceLoc(),  str);
      }

      advance();
   }

   if (currentTok().is(tok::open_brace)) {
      advance();

      auto LBraceLoc = currentTok().getSourceLoc();
      auto compoundDecl = CompoundDecl::Create(Context, LBraceLoc, true);

      DeclContextRAII declContextRAII(*this, compoundDecl);
      while (!currentTok().is(tok::close_brace)) {
         auto declResult = parseNextDecl();
         if (declResult) {
            if (!declResult.holdsDecl()) {
               SP.diagnose(err_expected_declaration,
                           currentTok().getSourceLoc(), "declare");
            }
            else {
               auto nextDecl = declResult.getDecl();

               if (HasLang)
                  if (auto CD = dyn_cast<CallableDecl>(nextDecl))
                     CD->addAttribute(new(Context) ExternAttr(Lang));

               if (nextDecl->hasDefinition())
                  SP.diagnose(nextDecl, err_declared_with_definition);
            }
         }

         advance();
      }

      compoundDecl->setRBraceLoc(currentTok().getSourceLoc());
      return compoundDecl;
   }
   else {
      auto declResult = parseNextDecl();
      if (declResult) {
         if (!declResult.holdsDecl()) {
            SP.diagnose(err_expected_declaration, currentTok().getSourceLoc(),
                        "declare");
         }
         else {
            auto nextDecl = declResult.getDecl();

            if (HasLang)
               if (auto CD = dyn_cast<CallableDecl>(nextDecl))
                  CD->addAttribute(new(Context) ExternAttr(Lang));

            if (nextDecl->hasDefinition())
               SP.diagnose(nextDecl, err_declared_with_definition);

            return nextDecl;
         }
      }

      return ParseStmtError();
   }
}

ParseResult Parser::parseTryStmt()
{
   auto start = currentTok().getSourceLoc();
   advance();

   auto tryBody = parseNextStmt().tryGetStatement();
   TryStmt* tryStmt = new(Context) TryStmt(SourceRange(start), tryBody);

   while (lookahead().oneOf(tok::kw_catch, tok::kw_finally)) {
      advance();

      auto kind = currentTok().getKind();
      bool finallySet = false;

      if (kind == tok::kw_catch) {
         advance();
         CatchBlock catchBlock;

         auto decl = parseVarDecl();
         catchBlock.varDecl = decl.tryGetDecl<LocalVarDecl>();

         if (!catchBlock.varDecl) {
            SP.diagnose(err_generic_error, currentTok().getSourceLoc(), 
                        "destructuring declaration cannot appear in catch");
         }
         else if (!catchBlock.varDecl->getType())
            SP.diagnose(err_generic_error, currentTok().getSourceLoc(), 
                        "catch"" must have a defined type");

         advance();
         catchBlock.body = parseNextStmt().tryGetStatement();

         tryStmt->addCatch(catchBlock);
      }
      else {
         if (finallySet) {
            SP.diagnose(err_generic_error, currentTok().getSourceLoc(), 
                        "finally"" block already defined");
         }

         advance();

         auto finallyBody = parseNextStmt().tryGetStatement();
         tryStmt->setFinally(finallyBody);

         finallySet = true;
      }
   }

   return tryStmt;
}

ParseResult Parser::parseThrowStmt()
{
   auto ThrowLoc = consumeToken(tok::kw_throw);
   return new(Context) ThrowStmt(ThrowLoc, parseExprSequence().tryGetExpr());
}

ParseResult Parser::parseReturnStmt()
{
   auto RetLoc = currentTok().getSourceLoc();
   Token next = lookahead(false);
   ReturnStmt* ret = nullptr;

   switch (next.getKind()) {
   default:
      advance();
      ret = ReturnStmt::Create(Context,
                               RetLoc, parseExprSequence().tryGetExpr());
      break;
   case tok::comma: case tok::semicolon: case tok::close_paren:
   case tok::newline: case tok::eof: case tok::sentinel:
   case tok::open_brace: case tok::close_brace: case tok::close_square:
      ret = ReturnStmt::Create(Context, RetLoc);
      break;
   }

   return ret;
}

ParseResult Parser::parseKeyword()
{
   auto BeginLoc = currentTok().getSourceLoc();

   auto kind = currentTok().getKind();
   tok::TokenType relevantToken = tok::sentinel;

   switch (kind) {
   case tok::kw_self:
   case tok::kw_true:
   case tok::kw_false:
   case tok::kw_none:
      return parseExprSequence();
   case tok::kw_var:
   case tok::kw_let:
      return parseVarDecl();
   case tok::kw_def:
      return parseFunctionDecl();
   case tok::kw_if:
      return parseIfStmt();
   case tok::kw_while:
   case tok::kw_loop:
      return parseWhileStmt(kind == tok::kw_while);
   case tok::kw_match:
      return parseMatchStmt();
   case tok::kw_for:
      return parseForStmt();
   case tok::kw_continue:
   case tok::kw_break: {
      Statement* stmt;
      if (kind == tok::kw_continue) {
         stmt = ContinueStmt::Create(Context, BeginLoc);
      }
      else {
         stmt = BreakStmt::Create(Context, BeginLoc);
      }

      return stmt;
   }
   case tok::kw_typedef:
      return parseTypedef();
   case tok::kw_alias:
      return parseAlias();
   case tok::kw_declare:
      return parseDeclareStmt();
   case tok::kw_return:
      return parseReturnStmt();
   case tok::kw_try:
      return parseTryStmt();
   case tok::kw_throw:
      return parseThrowStmt();
   case tok::kw_goto:
      advance();
      return GotoStmt::Create(Context, BeginLoc,
                              currentTok().getIdentifierInfo());
   case tok::kw_namespace:
      return parseNamespaceDecl();
   case tok::kw_using:
      return parseUsingStmt();
   case tok::kw_import:
      SP.diagnose(err_import_not_at_begin, currentTok().getSourceLoc());
      return parseImportStmt();
   case tok::kw_mixin: {
      auto loc = currentTok().getSourceLoc();
      if (!expect(tok::open_paren)) {
         return skipUntilProbableEndOfExpr();
      }

      advance();

      auto E = parseExprSequence().tryGetExpr();
      expect(tok::close_paren);

      SourceRange Parens(loc, currentTok().getSourceLoc());
      return MixinStmt::Create(Context, Parens, E);
   }
   case tok::kw___debug:
      return new(Context) DebugStmt(BeginLoc, false);
   case tok::kw___unreachable:
      return new(Context) DebugStmt(BeginLoc, true);
   case tok::kw_static:
      if (lookahead().is(tok::kw_if)) {
         return parseStaticIf();
      }
      if (lookahead().is(tok::kw_for)) {
         return parseStaticFor();
      }

      break;
   case tok::kw_static_assert:
      return parseStaticAssert();
   case tok::kw_static_print:
      return parseStaticPrint();
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
            return parseAnyRecord(relevantToken);
         case tok::kw_typedef:
            return parseTypedef();
         case tok::kw_var:
         case tok::kw_let:
            return parseVarDecl();
         case tok::kw_def:
            return parseFunctionDecl();
         default:
            break;
      }
      LLVM_FALLTHROUGH;
   default:
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), false);
      break;
   }

   return ParseError();
}

ParseResult Parser::parseTopLevelDecl()
{
   if (currentTok().is(tok::at))
      return parseAttributedDecl();

   auto kind = currentTok().getKind();
   tok::TokenType relevantToken = tok::sentinel;

   switch (kind) {
   case tok::close_paren:
   case tok::close_square:
   case tok::close_brace: {
      unsigned idx =
         kind == tok::close_paren ? 0 : kind == tok::close_brace ? 1 : 2;

      SP.diagnose(err_extraneous_paren, currentTok().getSourceLoc(),  idx);
      return skipUntilNextDecl();
   }
   case tok::kw_var:
   case tok::kw_let:
      return parseVarDecl();
   case tok::kw_def:
      return parseFunctionDecl();
   case tok::kw_typedef:
      return parseTypedef();
   case tok::kw_alias:
      return parseAlias();
   case tok::kw_declare:
      return parseDeclareStmt();
   case tok::kw_namespace:
      return parseNamespaceDecl();
   case tok::kw_using:
      return parseUsingStmt();
   case tok::kw_import:
      SP.diagnose(err_import_not_at_begin, currentTok().getSourceLoc());
      return parseImportStmt();
   case tok::kw_mixin: {
      auto loc = currentTok().getSourceLoc();
      if (!expect(tok::open_paren)) {
         return skipUntilProbableEndOfExpr();
      }

      advance();

      auto E = parseExprSequence().tryGetExpr();
      expect(tok::close_paren);

      SourceRange Parens(loc, currentTok().getSourceLoc());
      return MixinDecl::Create(Context, loc, Parens, E);
   }
   case tok::kw_static:
      if (lookahead().is(tok::kw_if)) {
         return parseStaticIf();
      }
      if (lookahead().is(tok::kw_for)) {
         return parseStaticFor();
      }

      break;
   case tok::kw_static_assert:
      return parseStaticAssert();
   case tok::kw_static_print:
      return parseStaticPrint();
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
         return parseAnyRecord(relevantToken);
      case tok::kw_typedef:
         return parseTypedef();
      case tok::kw_var:
      case tok::kw_let:
         return parseVarDecl();
      case tok::kw_def:
         return parseFunctionDecl();
      default:
         break;
      }

      LLVM_FALLTHROUGH;
   default:
      SP.diagnose(err_expecting_decl, currentTok().getSourceLoc(), 
                  currentTok().toString(), /*top level*/ true);

      return skipUntilNextDecl();
   }

   llvm_unreachable("bad token kind");
}

ParseResult Parser::parseFunctionCall(bool,
                                      Expression *ParentExpr,
                                      bool pointerAccess) {
   auto IdentLoc = currentTok().getSourceLoc();

   auto ident = currentTok().getIdentifierInfo();
   advance();

   auto LParenLoc = currentTok().getSourceLoc();
   auto args = parseCallArguments();

   SourceRange Parens(LParenLoc, currentTok().getSourceLoc());
   auto call = new(Context) CallExpr(IdentLoc, Parens, ParentExpr,
                                     move(args.args), ident);

   call->setIsPointerAccess(pointerAccess);
   return maybeParseSubExpr(call);
}

ParseResult Parser::parseEnumCaseExpr()
{
   auto PeriodLoc = currentTok().getSourceLoc();
   auto ident = currentTok().getIdentifierInfo();

   EnumCaseExpr* expr;
   if (lookahead().is(tok::open_paren)) {
      advance();
      expr = new(Context) EnumCaseExpr(PeriodLoc, ident,
                                       parseCallArguments().args);
   }
   else {
      expr = new(Context) EnumCaseExpr(PeriodLoc, ident);
   }

   return expr;
}

Parser::ArgumentList Parser::parseCallArguments()
{
   ArgumentList args;
   bool isLabeled = false;

   while (!currentTok().is(tok::close_paren)) {
      string label;

      if (currentTok().oneOf(tok::open_paren, tok::comma)) {
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
            SP.diagnose(err_labeled_args_last, currentTok().getSourceLoc());
         }
      }

      auto argVal = parseExprSequence();
      if (!argVal) {
         expect(tok::comma, tok::close_paren);
         continue;
      }

      args.labels.emplace_back(move(label));
      args.args.emplace_back(argVal.getExpr());

      if (!expect(tok::comma, tok::close_paren)) {
         skipUntilEven(tok::open_paren);
         break;
      }
   }

   return args;
}

ParseResult Parser::parseBlock(bool preserveTopLevel)
{
   advance();

   if (!(currentTok().is(tok::open_brace))) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), true, "'{'");

      return skipUntilProbableEndOfStmt();
   }

   auto LBraceLoc = currentTok().getSourceLoc();
   advance();

   llvm::SmallVector<Statement*, 8> Stmts;
   while (!currentTok().is(tok::close_brace)) {
      while (currentTok().oneOf(tok::semicolon, tok::newline)) {
         advance();
      }

      if (currentTok().getKind() == tok::eof
          || currentTok().is(tok::close_brace)) {
         break;
      }

      auto nextStmt = parseNextStmt();
      if (nextStmt.holdsStatement()) {
         Stmts.push_back(nextStmt.getStatement());
      }
      else if (nextStmt.holdsExpr()) {
         Stmts.push_back(nextStmt.getExpr());
      }
      else if (nextStmt.holdsDecl()) {
         Stmts.push_back(DeclStmt::Create(Context, nextStmt.getDecl()));
      }
      else {
         return skipUntilEven(tok::open_brace);
      }

      advance();
   }

   if (!currentTok().is(tok::close_brace)) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), true, "'}'");
   }

   return CompoundStmt::Create(Context, Stmts, preserveTopLevel, LBraceLoc,
                               currentTok().getSourceLoc());
}

ParseResult Parser::parseNextDecl()
{
   if (isAtRecordLevel())
      return parseRecordLevelDecl();

   return parseTopLevelDecl();
}

ParseResult Parser::parseNextStmt()
{
   if (currentTok().is(tok::at))
      return parseAttributedStmt();

   ParseResult stmt;

   auto kind = currentTok().getKind();
   switch (kind) {
#  define CDOT_KEYWORD_TOKEN(Name, Str) \
   case tok::Name:
#  include "Lex/Tokens.def"
      stmt = parseKeyword();
      break;
   case tok::close_paren:
   case tok::close_square:
   case tok::close_brace: {
      unsigned idx =
         kind == tok::close_paren ? 0 : kind == tok::close_brace ? 1 : 2;

      SP.diagnose(err_extraneous_paren, currentTok().getSourceLoc(),  idx);
      return skipUntilProbableEndOfStmt();
   }
   case tok::open_brace:
      lexer->backtrack();
      stmt = parseBlock();
      break;
   case tok::semicolon:
      stmt = NullStmt::Create(Context, currentTok().getSourceLoc());
      break;
   case tok::ident:
      if (lookahead(false, true).is(tok::colon)) {
         auto label = currentTok().getIdentifierInfo();
         advance();

         stmt = LabelStmt::Create(Context, currentTok().getSourceLoc(), label);
         break;
      }

      LLVM_FALLTHROUGH;
   default:
      stmt = parseExprSequence();
   }

   return stmt;
}

void Parser::parseStmts(llvm::SmallVectorImpl<Statement *> &Stmts)
{
   while (currentTok().getKind() != tok::eof) {
      while (currentTok().oneOf(tok::newline, tok::semicolon)) {
         advance();
      }

      if (currentTok().getKind() == tok::eof) {
         break;
      }

      auto nextStmt = parseNextStmt();
      if (nextStmt.holdsStatement()) {
         Stmts.push_back(nextStmt.getStatement());
      }
      else if (nextStmt.holdsExpr()) {
         Stmts.push_back(nextStmt.getExpr());
      }
      else if (nextStmt.holdsDecl()) {
         Stmts.push_back(DeclStmt::Create(Context, nextStmt.getDecl()));
      }

      if (currentTok().getKind() == tok::eof) {
         break;
      }

      advance();
   }
}

ParseResult Parser::parseNamespaceDecl()
{
   auto Loc = currentTok().getSourceLoc();
   bool anonymous = false;

   IdentifierInfo *nsName;
   if (lookahead().getKind() != tok::ident) {
      anonymous = true;
      nsName = &Context.getIdentifiers().get("(anonymous namespace)");
   }
   else {
      advance();
      nsName = currentTok().getIdentifierInfo();
   }

   while (lookahead().is(tok::period)) {
      advance();
      advance();

      if (currentTok().getKind() != tok::ident) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                     currentTok().toString(), true, "identifier");

         return skipUntilProbableEndOfStmt();
      }

      llvm_unreachable("Todo!");
   }

   advance();
   if (!currentTok().is(tok::open_brace)) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), true, "'{'");

      return skipUntilNextDecl();
   }

   SourceLocation LBrace = currentTok().getSourceLoc();
   auto NS = NamespaceDecl::Create(Context, Loc, LBrace, nsName, anonymous);

   advance();
   {
      DeclContextRAII declContextRAII(*this, NS);
      while (!currentTok().is(tok::close_brace)) {
         auto declResult = parseNextStmt();
         if (!declResult.holdsDecl()) {
            SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                        currentTok().toString(), true, "declaration");
         }

         advance();
      }
   }

   NS->setRBraceLoc(currentTok().getSourceLoc());
   SP.addDeclToContext(SP.getDeclContext(), NS);

   return NS;
}

ParseResult Parser::parseUsingStmt()
{
   auto UsingLoc = consumeToken();

   llvm::SmallVector<IdentifierInfo*, 4> declContext;
   llvm::SmallVector<IdentifierInfo*, 4> importedItems;

   SourceLocation wildCardLoc;
   while (lookahead().is(tok::period)) {
      if (currentTok().is(tok::times)) {
         wildCardLoc = currentTok().getSourceLoc();
         break;
      }
      if (!currentTok().is(tok::ident)) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                     currentTok().toString(), true, "identifier");

         return skipUntilProbableEndOfStmt();
      }

      declContext.emplace_back(currentTok().getIdentifierInfo());
      advance();
      advance();
   }

   if (importedItems.empty()) {
      if (currentTok().is(tok::open_brace)) {
         if (!expect(tok::ident, tok::times)) {
            return skipUntilProbableEndOfStmt();
         }

         while (!currentTok().is(tok::close_brace)) {
            if (currentTok().is(tok::times)) {
               wildCardLoc = currentTok().getSourceLoc();
            }
            else {
               importedItems.emplace_back(currentTok().getIdentifierInfo());
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
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                     currentTok().toString(), true, "identifier");
      }
      else {
         importedItems.emplace_back(currentTok().getIdentifierInfo());
      }
   }

   if (wildCardLoc.isValid() && !importedItems.empty()) {
      SP.diagnose(err_import_multiple_with_wildcard, wildCardLoc);
      importedItems.clear();
   }

   SourceRange SR(UsingLoc, currentTok().getSourceLoc());
   return UsingStmt::Create(Context, SR, declContext, importedItems,
                            wildCardLoc.isValid());
}

ParseResult Parser::parseModuleStmt()
{
   auto ModuleLoc = consumeToken();
   if (!currentTok().is(tok::ident)) {
      return skipUntilProbableEndOfStmt();
   }

   llvm::SmallVector<IdentifierInfo*, 4> moduleName;

   while (1) {
      moduleName.emplace_back(currentTok().getIdentifierInfo());

      if (lookahead().is(tok::period)) {
         advance();
         if (!expect(tok::ident)) {
            skipUntilProbableEndOfStmt();
            break;
         }
      }
      else {
         break;
      }
   }

   SourceRange SR(ModuleLoc, currentTok().getSourceLoc());
   return ModuleStmt::Create(Context, SR, moduleName);
}

ParseResult Parser::parseImportStmt()
{
   auto ImportLoc = consumeToken();
   if (!currentTok().is(tok::ident)) {
      return skipUntilProbableEndOfStmt();
   }

   llvm::SmallVector<IdentifierInfo*, 4> moduleName;

   while (1) {
      moduleName.emplace_back(currentTok().getIdentifierInfo());

      if (lookahead().is(tok::period)) {
         advance();
         if (!expect(tok::ident)) {
            skipUntilProbableEndOfStmt();
            break;
         }
      }
      else {
         break;
      }
   }

   SourceRange SR(ImportLoc, currentTok().getSourceLoc());
   return ImportStmt::Create(Context, SR, moduleName);
}

void Parser::parseImports(llvm::SmallVectorImpl<ImportStmt*> &stmts)
{
   while (currentTok().oneOf(tok::newline, tok::space, tok::semicolon))
      advance();

   while (currentTok().is(tok::kw_import)) {
      auto importResult = parseImportStmt();
      if (importResult)
         stmts.push_back(importResult.getStatement<ImportStmt>());

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

   while (currentTok().getKind() != tok::eof) {
      while (currentTok().oneOf(tok::newline, tok::semicolon)) {
         advance();
      }

      if (currentTok().getKind() == tok::eof) {
         break;
      }

      parseNextDecl();
      if (currentTok().getKind() == tok::eof) {
         break;
      }

      advance();
   }
}

} // namespace Parse
} // namespace cdot