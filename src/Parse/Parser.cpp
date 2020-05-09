#include "cdotc/Parse/Parser.h"

#include "cdotc/AST/ASTContext.h"
#include "cdotc/AST/Decl.h"
#include "cdotc/AST/Expression.h"
#include "cdotc/AST/Statement.h"
#include "cdotc/Basic/IdentifierInfo.h"
#include "cdotc/Basic/Variant.h"
#include "cdotc/Lex/Lexer.h"
#include "cdotc/Diagnostics/Diagnostics.h"
#include "cdotc/Module/ModuleManager.h"
#include "cdotc/Sema/Builtin.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Support/Casting.h"
#include "cdotc/Support/Format.h"
#include "cdotc/Support/LiteralParser.h"
#include "cdotc/Support/SaveAndRestore.h"
#include "cdotc/Support/StringSwitch.h"

#include <cassert>
#include <string>
#include <vector>

#include <llvm/ADT/SmallString.h>
#include <llvm/Support/PrettyStackTrace.h>

using namespace cdot::diag;
using namespace cdot::support;
using namespace cdot::lex;

namespace cdot {
namespace parse {

Parser::Parser(ASTContext& Context, lex::Lexer* lexer, SemaPass& SP,
               bool isModuleParser)
    : Context(Context), source_id(lexer->getSourceId()),
      UnboundedByDefault(!Context.CI.getOptions().runtimeGenerics()),
      lexer(lexer), SP(SP), Idents(lexer->getIdents()),
      Ident_self(&Idents.get("self")), Ident_Self(&Idents.get("Self")),
      Ident_super(&Idents.get("super")), Ident_in(&Idents.get("in")),
      Ident_as(&Idents.get("as")), Ident_is(&Idents.get("is")),
      Ident_do(&Idents.get("do")), Ident_then(&Idents.get("then")),
      Ident_where(&Idents.get("where")), Ident_prefix(&Idents.get("prefix")),
      Ident_postfix(&Idents.get("postfix")), Ident_infix(&Idents.get("infix")),
      Ident_default(&Idents.get("default")),
      Ident_deinit(&Idents.get("deinit")),
      Ident_typename(&Idents.get("typename")),
      Ident_sizeof(&Idents.get("sizeof")),
      Ident_decltype(&Idents.get("decltype")),
      Ident_subscript(&Idents.get("subscript")),
      Ident_memberwise(&Idents.get("memberwise")),
      Ident_get(&Idents.get("get")), Ident_set(&Idents.get("set")),
      Ident_virtual(&Idents.get("virtual")),
      Ident_override(&Idents.get("override")), Ident_with(&Idents.get("with")),
      Ident_throws(&Idents.get("throws")), Ident_async(&Idents.get("async")),
      Ident_unsafe(&Idents.get("unsafe")),
      Ident_precedenceGroup(&Idents.get("precedenceGroup")),
      Ident_higherThan(&Idents.get("higherThan")),
      Ident_lowerThan(&Idents.get("lowerThan")),
      Ident_associativity(&Idents.get("associativity")),
      Ident_assignment(&Idents.get("assignment")),
      Ident_macro(&Idents.get("macro")), Ident_owned(&Idents.get("owned")),
      Ident_borrow(&Idents.get("borrow")), Ident_ref(&Idents.get("ref")),
      Ident_mut(&Idents.get("mut")), Ident_from(&Idents.get("from")),
      Ident_unittest(&Idents.get("unittest")),
      Ident___traits(&Idents.get("__traits")),
      Ident___nullptr(&Idents.get("__nullptr")),
      Ident___builtin_void(&Idents.get("__builtin_void")),
      Ident___mangled_func(&Idents.get("__mangled_func")),
      Ident___ctfe(&Idents.get("__ctfe")),
#define CDOT_SOFT_KEYWORD(NAME) Ident_##NAME(&Idents.get(#NAME)),
#include "cdotc/Lex/Tokens.def"
      CurDeclAttrs()
{
   AllowTrailingClosureStack.push(true);
}

Parser::~Parser() = default;

namespace {

class ParseDeclPrettyStackTraceEntry : public llvm::PrettyStackTraceEntry {
   DeclContext* D;

public:
   ParseDeclPrettyStackTraceEntry(DeclContext* D) : D(D) {}

   void print(llvm::raw_ostream& OS) const override
   {
      if (auto ND = dyn_cast<NamedDecl>(D))
         OS << "while parsing '" << ND->getDeclName() << "'\n";
   }
};

} // anonymous namespace

Parser::DeclContextRAII::DeclContextRAII(Parser& P, DeclContext* Ctx)
    : P(P), PrevInRecordDecl(P.InRecordScope)
{
   static_assert(sizeof(StackTraceEntry)
                     == sizeof(ParseDeclPrettyStackTraceEntry),
                 "insufficient storage!");

   Ctx->setParentCtx(&P.SP.getDeclContext());
   P.SP.pushDeclContext(Ctx);

   new (StackTraceEntry) ParseDeclPrettyStackTraceEntry(Ctx);

   switch (Ctx->getDeclKind()) {
   case Decl::StructDeclID:
   case Decl::ClassDeclID:
   case Decl::EnumDeclID:
   case Decl::UnionDeclID:
   case Decl::ProtocolDeclID:
   case Decl::ExtensionDeclID:
      P.InRecordScope = true;
      break;
   case Decl::CompoundDeclID:
   case Decl::StaticIfDeclID:
   case Decl::StaticForDeclID:
      break;
   default:
      P.InRecordScope = false;
      break;
   }
}

Parser::DeclContextRAII::DeclContextRAII(DeclContextRAII&& Other) noexcept
    : P(Other.P), PrevInRecordDecl(P.InRecordScope)
{
}

Parser::DeclContextRAII::~DeclContextRAII()
{
   P.InRecordScope = PrevInRecordDecl;
   P.SP.popDeclContext();
   reinterpret_cast<ParseDeclPrettyStackTraceEntry*>(StackTraceEntry)
       ->~ParseDeclPrettyStackTraceEntry();
}

Parser::StateSaveRAII::StateSaveRAII(Parser& P)
    : P(P), enabled(true), SavedCurTok(P.currentTok()),
      SavedLastTok(P.lexer->getLastTok())
{
}

void Parser::StateSaveRAII::advance(bool ignoreNewline,
                                    bool significantWhitespace)
{
   P.advance(ignoreNewline, significantWhitespace);
   Tokens.push_back(P.currentTok());
}

Parser::StateSaveRAII::~StateSaveRAII()
{
   if (enabled) {
      P.lexer->insertLookaheadTokens(Tokens);
      P.lexer->setCurTok(SavedCurTok);
      P.lexer->setLastTok(SavedLastTok);
   }
}

ParseResult Parser::ActOnDecl(Decl* D)
{
   if (DiscardDecls)
      return ParseError();

   SP.ActOnDecl(&SP.getDeclContext(), D);
   return D;
}

void Parser::skipWhitespace()
{
   while (currentTok().oneOf(tok::semicolon, tok::newline, tok::space)
          && !lookahead().is(tok::eof)) {
      advance();
   }
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
   //   llvm::outs()<<lexer->currentTok().toString();
}

const lex::Token& Parser::currentTok() const { return lexer->currentTok(); }

bool Parser::expectToken(cdot::lex::tok::TokenType expected)
{
   if (currentTok().is(expected))
      return true;

   errorUnexpectedToken(currentTok(), expected);
   return false;
}

void Parser::errorUnexpectedToken(const lex::Token& given,
                                  tok::TokenType expected)
{
   SP.diagnose(err_unexpected_token, given.getSourceLoc(), given.toString(),
               true, tok::tokenTypeToString(expected));
}

void Parser::maybeParseConvention(ArgumentConvention& Conv, SourceLocation& Loc)
{
   Conv = ArgumentConvention::Default;
   if (currentTok().is(Ident_owned)) {
      Conv = ArgumentConvention::Owned;
      Loc = consumeToken();
   }
   else if (currentTok().is(Ident_borrow)) {
      Conv = ArgumentConvention::Borrowed;
      Loc = consumeToken();
   }
   else if (currentTok().is(Ident_ref)) {
      Conv = ArgumentConvention::ImmutableRef;
      Loc = consumeToken();
   }
   else if (currentTok().is(Ident_mut) && lookahead().is(Ident_ref)) {
      Conv = ArgumentConvention::MutableRef;
      Loc = consumeToken();
      advance();
   }
}

AccessSpecifier Parser::tokenToAccessSpec(tok::TokenType kind)
{
   switch (kind) {
   case tok::kw_public:
      return AccessSpecifier::Public;
   case tok::kw_protected:
      return AccessSpecifier::Protected;
   case tok::kw_private:
      return AccessSpecifier::Private;
   case tok::kw_fileprivate:
      return AccessSpecifier::FilePrivate;
   case tok::kw_internal:
      return AccessSpecifier::Internal;
   default:
      llvm_unreachable("not an access specifier");
   }
}

FixKind Parser::tokenToFix(const lex::Token& Tok)
{
   assert(Tok.getKind() == tok::ident && "not a fix token");
   auto* II = Tok.getIdentifierInfo();
   if (II == Ident_prefix) {
      return FixKind::Prefix;
   }
   else if (II == Ident_postfix) {
      return FixKind::Postfix;
   }
   else {
      return FixKind::Infix;
   }
}

bool Parser::validOperatorFollows()
{
   assert(currentTok().oneOf(Ident_infix, Ident_prefix, Ident_postfix));
   switch (lookahead().getKind()) {
   case tok::ident:
   case tok::op_ident:
#define CDOT_OPERATOR_TOKEN(Name, Spelling) case tok::Name:
#include "cdotc/Lex/Tokens.def"
      return true;
   case tok::open_paren: {
      if (!currentTok().is(Ident_postfix)) {
         return false;
      }

      advance();

      bool Result = lookahead().is(tok::close_paren);
      lexer->backtrack();

      return Result;
   }
   default:
      return false;
   }
}

bool Parser::skipUntilNextDecl()
{
   if (isAtRecordLevel())
      return skipUntilNextDeclOrClosingBrace();

   while (1) {
      switch (lookahead().getKind()) {
      case tok::at:
      case tok::kw_var:
      case tok::kw_let:
      case tok::kw_def:
      case tok::kw_prop:
      case tok::kw_typedef:
      case tok::kw_alias:
      case tok::kw_declare:
      case tok::kw_namespace:
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
      case tok::kw_abstract:
      case tok::kw_public:
      case tok::kw_private:
      case tok::kw_protected:
      case tok::kw_internal:
      case tok::kw_fileprivate:
         return true;
      case tok::ident:
         if (currentTok().oneOf(Ident_using, Ident_infix, Ident_prefix,
                                Ident_postfix)) {
            return true;
         }

         return false;
      case tok::eof:
         return false;
      default:
         break;
      }

      advance();
   }

   llvm_unreachable("no EOF token!");
}

bool Parser::skipUntilNextDeclOrClosingBrace()
{
   unsigned openBraces = 1;
   unsigned closedBraces = 0;

   while (1) {
      switch (lookahead().getKind()) {
      case tok::at:
      case tok::kw_var:
      case tok::kw_let:
      case tok::kw_def:
      case tok::kw_prop:
      case tok::kw_typedef:
      case tok::kw_alias:
      case tok::kw_declare:
      case tok::kw_namespace:
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
      case tok::kw_abstract:
      case tok::kw_public:
      case tok::kw_private:
      case tok::kw_protected:
      case tok::kw_internal:
      case tok::kw_fileprivate:
         return true;
      case tok::ident:
         if (currentTok().oneOf(Ident_using, Ident_infix, Ident_prefix,
                                Ident_postfix)) {
            return true;
         }

         break;
      case tok::eof:
         return false;
      case tok::open_brace:
         ++openBraces;
         break;
      case tok::close_brace:
         ++closedBraces;
         break;
      default:
         break;
      }

      if (closedBraces == openBraces)
         return true;

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
   case tok::open_paren:
      closeTok = tok::close_paren;
      break;
   case tok::open_brace:
      closeTok = tok::close_brace;
      break;
   case tok::open_square:
      closeTok = tok::close_square;
      break;
   case tok::smaller:
      closeTok = tok::greater;
      break;
   default:
      llvm_unreachable("not a paren token!");
   }

   unsigned closed = 0;
   while (true) {
      auto next = lookahead();
      if (next.is(openTok))
         ++open;
      else if (next.is(closeTok))
         ++closed;
      else if (next.is(tok::eof)) {
         errorUnexpectedToken(next, closeTok);
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

      raii.advance(false);
   }

   return false;
}

bool Parser::findTokOnLine(IdentifierInfo* Id)
{
   StateSaveRAII raii(*this);

   while (!currentTok().oneOf(tok::newline, tok::eof)) {
      if (currentTok().is(Id)) {
         raii.disable();
         return true;
      }

      raii.advance(false);
   }

   return false;
}

ParseTypeResult Parser::parseType(bool allowInferredArraySize,
                                  bool InTypePosition,
                                  bool AllowMissingTemplateArguments)
{
   if (currentTok().is(tok::at)) {
      return parseAttributedType();
   }

   auto BeginLoc = currentTok().getSourceLoc();
   auto typeResult = parseTypeImpl(allowInferredArraySize, InTypePosition,
                                   AllowMissingTemplateArguments);

   if (!typeResult)
      return ParseTypeResult();

   auto typeref = typeResult.get();

   auto next = lookahead();
   while (1) {
      if (next.oneOf(tok::times, tok::times_times, tok::op_ident)) {
         if (next.is(tok::times)) {
            advance();

            auto PtrTy = PointerTypeExpr::Create(
                Context, SourceRange(BeginLoc, currentTok().getSourceLoc()),
                typeref, /*meta=*/!InTypePosition);

            typeref = SourceType(PtrTy);
            break;
         }
         if (next.is(tok::times_times)) {
            advance();

            auto PtrTy = PointerTypeExpr::Create(
                Context, SourceRange(BeginLoc, currentTok().getSourceLoc()),
                typeref, /*meta=*/!InTypePosition);

            typeref = SourceType(PtrTy);

            PtrTy = PointerTypeExpr::Create(
                Context, SourceRange(BeginLoc, currentTok().getSourceLoc()),
                typeref, /*meta=*/!InTypePosition);

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
                   typeref, /*meta=*/!InTypePosition);

               typeref = SourceType(PtrTy);
            }
         }
      }
      else if (next.is(tok::question)) {
         advance();

         SourceRange SR(SourceLocation(BeginLoc.getOffset()),
                        currentTok().getSourceLoc());

         auto* II = &Context.getIdentifiers().get("Option");
         auto OptTy = new (Context) IdentifierRefExpr(SR, II);
         auto TAs = TemplateArgListExpr::Create(Context, SR, OptTy,
                                                typeref.getTypeExpr());

         OptTy->setIsSynthesized(true);
         typeref = SourceType(TAs);
      }
      else if (next.is(tok::period)) {
         auto subExpr = maybeParseSubExpr(typeref.getTypeExpr(), true);
         typeref = SourceType(subExpr.tryGetExpr());
      }
      else {
         break;
      }

      next = lookahead();
   }

   // Existential type union
   if (next.is(tok::op_and)) {
      SmallVector<SourceType, 2> Types{typeref};

      while (next.is(tok::op_and)) {
         advance();
         advance();

         auto Next = parseTypeImpl(allowInferredArraySize, InTypePosition,
                                   AllowMissingTemplateArguments);

         if (Next) {
            Types.push_back(Next.get());
         }

         next = lookahead();
      }

      SourceRange SR(BeginLoc, currentTok().getEndLoc());
      auto* Expr
          = ExistentialTypeExpr::Create(Context, SR, Types, !InTypePosition);

      typeref = SourceType(Expr);
   }

   // As a special case, allow a '...' with a space after a function type to
   // apply to the function type itself, without a space it would always
   // apply to the return type
   if (isa<FunctionTypeExpr>(typeref.getTypeExpr())
       && lookahead().is(tok::triple_period)) {
      advance();

      typeref.setTypeExpr(VariadicExpansionExpr::Create(
          Context, currentTok().getSourceLoc(), typeref.getTypeExpr()));
   }
   else if (lookahead(false, true).is(tok::triple_period)) {
      advance(false, true);

      typeref.setTypeExpr(VariadicExpansionExpr::Create(
          Context, currentTok().getSourceLoc(), typeref.getTypeExpr()));
   }

   typeref.getTypeExpr()->setIsInTypePosition(InTypePosition);
   return typeref;
}

ParseTypeResult Parser::parseTypeImpl(bool allowInferredArraySize,
                                      bool InTypePosition,
                                      bool AllowMissingTemplateArguments)
{
   auto BeginLoc = currentTok().getSourceLoc();

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
      return SourceType(DeclTypeExpr::Create(
          Context, SourceRange(BeginLoc, currentTok().getSourceLoc()),
          expr.getExpr(), /*meta=*/!InTypePosition));
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

         SourceRange SR(SourceLocation(BeginLoc.getOffset()),
                        currentTok().getSourceLoc());

         auto* II = &Context.getIdentifiers().get("Dictionary");
         auto Expr = new (Context) IdentifierRefExpr(SR, II);
         auto TAs = TemplateArgListExpr::Create(
             Context, SR, Expr,
             {elType.get().getTypeExpr(), valType.get().getTypeExpr()});

         Expr->setIsSynthesized(true);
         return SourceType(TAs);
      }

      // fixed (or inferred) size array
      if (currentTok().is(tok::semicolon)) {
         advance();

         StaticExpr* SizeExpr;
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

         return SourceType(ArrayTypeExpr::Create(
             Context, {BeginLoc, currentTok().getSourceLoc()}, elType.get(),
             SizeExpr,
             /*meta=*/!InTypePosition));
      }

      if (!currentTok().is(tok::close_square)) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                     currentTok().toString(), true, "']'");
      }

      SourceRange SR(SourceLocation(BeginLoc.getOffset()),
                     currentTok().getSourceLoc());

      auto* II = &Context.getIdentifiers().get("Array");
      auto Expr = new (Context) IdentifierRefExpr(SR, II);
      auto TAs = TemplateArgListExpr::Create(Context, SR, Expr,
                                             elType.get().getTypeExpr());

      Expr->setIsSynthesized(true);
      return SourceType(TAs);
   }

   // function or tuple type
   if (currentTok().is(tok::open_paren)) {
      advance();

      SmallVector<SourceType, 4> TupleTypes;
      SmallVector<IdentifierInfo*, 4> Labels;
      SmallVector<FunctionType::ParamInfo, 4> ParamInfo;

      SourceLocation AnnotLoc;

      unsigned i = 0;
      while (!currentTok().is(tok::close_paren)) {
         IdentifierInfo* label = nullptr;

         if (currentTok().getKind() == tok::ident
             && lookahead().is(tok::colon)) {
            label = currentTok().getIdentifierInfo();
            advance();
            advance();
         }
         else if (currentTok().getKind() == tok::underscore
                  && lookahead().is(tok::colon)) {
            label = SP.getIdentifier("_");
            advance();
            advance();
         }

         if (i++ != 0 && !Labels.empty() && !label) {
            label = SP.getIdentifier("_");
         }
         if (label) {
            Labels.push_back(label);
         }

         ArgumentConvention Conv;
         maybeParseConvention(Conv, AnnotLoc);

         ParamInfo.emplace_back(Conv);

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

      assert((Labels.empty() || Labels.size() == TupleTypes.size())
             && "wrong number of labels!");

      // tuple type
      if (!lookahead().oneOf(tok::arrow_single, Ident_throws, Ident_async,
                             Ident_unsafe)) {
         if (AnnotLoc) {
            SP.diagnose(err_ownership_annot_not_allowed,
                        currentTok().getSourceLoc(), AnnotLoc);
         }

         return SourceType(TupleTypeExpr::Create(
             Context, {BeginLoc, currentTok().getSourceLoc()}, TupleTypes,
             /*meta=*/!InTypePosition));
      }

      advance();
      return parseFunctionType(BeginLoc, TupleTypes, ParamInfo, InTypePosition);
   }

   auto IdentTy = parseIdentifierExpr(true);
   if (!IdentTy)
      return ParseTypeResult();

   return SourceType(IdentTy.getExpr());
}

ParseTypeResult Parser::parseFunctionType()
{
   SmallVector<SourceType, 4> TupleTypes;
   SmallVector<FunctionType::ParamInfo, 4> ParamInfo;

   SourceLocation AnnotLoc;
   SourceLocation BeginLoc = currentTok().getSourceLoc();
   advance();

   while (!currentTok().is(tok::close_paren)) {
      ArgumentConvention Conv;
      maybeParseConvention(Conv, AnnotLoc);

      ParamInfo.emplace_back(Conv);

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

   advance();
   return parseFunctionType(BeginLoc, TupleTypes, ParamInfo, false);
}

ParseTypeResult Parser::parseFunctionType(
    SourceLocation BeginLoc, ArrayRef<SourceType> ParamTys,
    ArrayRef<FunctionType::ParamInfo> ParamInfo, bool InTypePosition)
{
   bool Thin = false;
   bool Throws = false;
   bool Async = false;
   bool Unsafe = false;

   while (!currentTok().is(tok::arrow_single)) {
      if (currentTok().is(Ident_throws)) {
         Throws = true;
         advance();
      }
      else if (currentTok().is(Ident_async)) {
         Async = true;
         advance();
      }
      else if (currentTok().is(Ident_unsafe)) {
         Unsafe = true;
         advance();
      }
      else {
         llvm_unreachable("unexpected token");
      }
   }

   advance();

   auto returnType = parseType();
   if (!returnType) {
      return ParseTypeResult();
   }

   SourceRange SR(BeginLoc,
                  returnType.get().getTypeExpr()->getSourceRange().getEnd());

   return SourceType(FunctionTypeExpr::Create(
       Context, SR, returnType.get(), ParamTys, ParamInfo,
       /*IsMeta=*/!InTypePosition, Thin, Throws, Async, Unsafe));
}

void Parser::parseDeclConstraints(std::vector<ParsedConstraint>& Constraints)
{
   std::vector<IdentifierInfo*> NameQualifier;
   while (currentTok().is(Ident_where)) {
      SourceLocation BeginLoc = consumeToken();

      if (!currentTok().is(tok::ident)) {
         errorUnexpectedToken(currentTok(), tok::ident);
      }

      while (currentTok().is(tok::ident)) {
         NameQualifier.push_back(currentTok().getIdentifierInfo());
         advance();

         if (currentTok().is(tok::period)) {
            advance();

            if (!currentTok().is(tok::ident)) {
               errorUnexpectedToken(currentTok(), tok::ident);
            }
         }
         else {
            break;
         }
      }

      bool addedConstraint = false;
      if (currentTok().is(tok::smaller)) {
         IdentifierRefExpr* ParentExpr = nullptr;
         for (auto& Name : NameQualifier) {
            ParentExpr
                = new (Context) IdentifierRefExpr(BeginLoc, ParentExpr, Name);
         }

         // Parse the associated type constrained by this concept.
         advance();
         NameQualifier.clear();

         if (!currentTok().is(tok::ident)) {
            errorUnexpectedToken(currentTok(), tok::ident);
         }

         while (currentTok().is(tok::ident)) {
            NameQualifier.push_back(currentTok().getIdentifierInfo());
            advance();

            if (currentTok().is(tok::period)) {
               advance();

               if (!currentTok().is(tok::ident)) {
                  errorUnexpectedToken(currentTok(), tok::ident);
               }
            }
            else {
               break;
            }
         }

         if (!currentTok().is(tok::greater)) {
            errorUnexpectedToken(currentTok(), tok::greater);
         }

         SourceLocation EndLoc = currentTok().getEndLoc();
         Constraints.emplace_back(DeclConstraint::Concept,
                                  SourceRange(BeginLoc, EndLoc),
                                  move(NameQualifier), ParentExpr);

         addedConstraint = true;
      }
      else {
         DeclConstraint::Kind K;
         if (currentTok().is(tok::double_equals)) {
            K = DeclConstraint::TypeEquality;
         }
         else if (currentTok().is(tok::exclaim_equals)) {
            K = DeclConstraint::TypeInequality;
         }
         else if (currentTok().is(tok::exclaim_is)) {
            K = DeclConstraint::TypePredicateNegated;
         }
         else if (currentTok().is(Ident_is)) {
            if (lookahead().is(tok::kw_class)) {
               advance();

               SourceLocation EndLoc = currentTok().getEndLoc();
               Constraints.emplace_back(DeclConstraint::Class,
                                        SourceRange(BeginLoc, EndLoc),
                                        move(NameQualifier), nullptr);

               addedConstraint = true;
            }
            else if (lookahead().is(tok::kw_enum)) {
               advance();

               SourceLocation EndLoc = currentTok().getEndLoc();
               Constraints.emplace_back(DeclConstraint::Enum,
                                        SourceRange(BeginLoc, EndLoc),
                                        move(NameQualifier), nullptr);

               addedConstraint = true;
            }
            else if (lookahead().is(tok::kw_struct)) {
               advance();

               SourceLocation EndLoc = currentTok().getEndLoc();
               Constraints.emplace_back(DeclConstraint::Struct,
                                        SourceRange(BeginLoc, EndLoc),
                                        move(NameQualifier), nullptr);

               addedConstraint = true;
            }

            K = DeclConstraint::TypePredicate;
         }
         else {
            errorUnexpectedToken(currentTok(), tok::colon);
            break;
         }

         if (!addedConstraint) {
            advance();
            auto Type = parseType();

            SourceLocation EndLoc = currentTok().getEndLoc();
            Constraints.emplace_back(K, SourceRange(BeginLoc, EndLoc),
                                     move(NameQualifier),
                                     Type.tryGet().getTypeExpr());
         }
      }

      if (!lookahead().is(Ident_where))
         break;

      NameQualifier.clear();
      advance();
   }
}

ParseResult Parser::parseTypedef()
{
   auto TypedefLoc = consumeToken(tok::kw_typedef);

   auto originTy = parseType();
   if (!originTy) {
      return skipUntilProbableEndOfStmt();
   }

   advance();
   if (!currentTok().is(Ident_as)) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                  currentTok().toString(), true, "'as'");

      return skipUntilProbableEndOfStmt();
   }

   if (!expect(tok::ident)) {
      return skipUntilProbableEndOfStmt();
   }

   auto Name = currentTok().getIdentifierInfo();
   auto params = tryParseTemplateParameters();

   auto td = TypedefDecl::Create(Context, CurDeclAttrs.Access, TypedefLoc, Name,
                                 originTy.get(), move(params));

   td->setAccessLoc(CurDeclAttrs.AccessLoc);
   return ActOnDecl(td);
}

ParseResult Parser::parseAlias()
{
   auto AliasLoc = consumeToken(tok::kw_alias);
   if (!currentTok().is(tok::ident)) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                  currentTok().toString(), true, "identifier");

      return ParseError();
   }

   auto Name = currentTok().getIdentifierInfo();
   auto params = tryParseTemplateParameters();

   // Alias template parameters are unbounded by default.
   for (auto* P : params) {
      if (P->getCovariance().isResolved()) {
         assert(P->getCovariance()->isAutoType());
         P->setUnbounded(true);
      }
   }

   SourceType Type;
   if (lookahead().is(tok::colon)) {
      advance();
      advance();

      auto TypeRes = parseType(true);
      if (!TypeRes) {
         if (!findTokOnLine(tok::equals)) {
            return ParseError();
         }
      }
      else {
         Type = TypeRes.get();
      }
   }
   else {
      Type.setResolvedType(Context.getAutoType());
   }

   StaticExpr* aliasExpr = nullptr;
   if (lookahead().is(tok::equals)) {
      advance();
      advance();

      auto ExprRes = parseExprSequence();
      if (!ExprRes) {
         return ParseError();
      }

      aliasExpr = StaticExpr::Create(Context, ExprRes.getExpr());
   }

   std::vector<ParsedConstraint> Constraints;
   if (lookahead().is(Ident_where)) {
      advance();
      parseDeclConstraints(Constraints);
   }

   auto aliasDecl = AliasDecl::Create(Context, AliasLoc, CurDeclAttrs.Access,
                                      Name, Type, aliasExpr, params);

   if (ParsingProtocol) {
      SP.diagnose(aliasDecl, err_may_not_appear_in_protocol, aliasDecl,
                  currentTok().getSourceLoc());
   }

   aliasDecl->setAccessLoc(CurDeclAttrs.AccessLoc);
   Context.setParsedConstraints(aliasDecl, move(Constraints));

   return ActOnDecl(aliasDecl);
}

ParseResult Parser::parsePrecedenceGroup()
{
   auto KeywordLoc = consumeToken();
   if (!currentTok().is(tok::ident)) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                  currentTok().toString(), true, "identifier");

      return ParseError();
   }

   auto Name = currentTok().getIdentifierInfo();

   bool AssociativityDefined = false;
   bool SawAssignment = false;

   Associativity Assoc = Associativity::Left;
   bool IsAssignment = false;
   const IdentifierInfo* HigherThan = nullptr;
   const IdentifierInfo* LowerThan = nullptr;

   if (lookahead().is(tok::open_brace)) {
      advance();
      advance();

      while (!currentTok().is(tok::close_brace)) {
         if (currentTok().is(Ident_higherThan)) {
            if (HigherThan) {
               SP.diagnose(err_prec_prop_already_defined,
                           currentTok().getSourceLoc(), /*higherThan*/ 0);
            }

            expect(tok::colon);
            advance();

            if (!currentTok().is(tok::ident)) {
               SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                           currentTok().toString(), true, "identifier");

               return ParseError();
            }

            HigherThan = currentTok().getIdentifierInfo();
         }
         else if (currentTok().is(Ident_lowerThan)) {
            if (LowerThan) {
               SP.diagnose(err_prec_prop_already_defined,
                           currentTok().getSourceLoc(), /*lowerThan*/ 1);
            }

            expect(tok::colon);
            advance();

            if (!currentTok().is(tok::ident)) {
               SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                           currentTok().toString(), true, "identifier");

               return ParseError();
            }

            LowerThan = currentTok().getIdentifierInfo();
         }
         else if (currentTok().is(Ident_associativity)) {
            if (AssociativityDefined) {
               SP.diagnose(err_prec_prop_already_defined,
                           currentTok().getSourceLoc(), /*associativity*/ 2);
            }

            expect(tok::colon);
            advance();

            if (!currentTok().is(tok::ident)) {
               SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                           currentTok().toString(), true, "identifier");

               return ParseError();
            }

            if (currentTok().getIdentifierInfo()->isStr("left")) {
               Assoc = Associativity::Left;
            }
            else if (currentTok().getIdentifierInfo()->isStr("right")) {
               Assoc = Associativity::Right;
            }
            else {
               SP.diagnose(err_bad_associativity, currentTok().getSourceLoc());
            }

            AssociativityDefined = true;
         }
         else if (currentTok().is(Ident_assignment)) {
            if (SawAssignment) {
               SP.diagnose(err_prec_prop_already_defined,
                           currentTok().getSourceLoc(), /*assignment*/ 3);
            }

            SawAssignment = true;

            expect(tok::colon);
            advance();

            if (!currentTok().oneOf(tok::kw_true, tok::kw_false)) {
               SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                           currentTok().toString(), true, "'true' or 'false'");

               return ParseError();
            }

            IsAssignment = currentTok().is(tok::kw_true);
         }
         else if (currentTok().is(tok::semicolon)) {
            // ignore
         }
         else {
            SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                        currentTok().toString(), true,
                        "'higherThan', 'lowerThan' or 'associativity'");
         }

         advance();
      }
   }

   SourceRange SR(KeywordLoc, currentTok().getEndLoc());
   auto PG = PrecedenceGroupDecl::Create(Context, SR, CurDeclAttrs.Access, Name,
                                         Assoc, HigherThan, LowerThan,
                                         IsAssignment);

   PG->setAccessLoc(CurDeclAttrs.AccessLoc);
   return ActOnDecl(PG);
}

ParseResult Parser::parseOperatorDecl()
{
   auto KeywordLoc = currentTok().getSourceLoc();

   FixKind Fix = tokenToFix(currentTok());
   advance();

   bool isCastOp;
   auto Name = parseOperatorName(Fix, isCastOp);
   if (!Name)
      Name = Context.getDeclNameTable().getErrorName();

   const IdentifierInfo* PrecedenceGroup = nullptr;
   if (lookahead().is(tok::colon)) {
      advance();
      advance();

      if (!currentTok().is(tok::ident)) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                     currentTok().toString(), true, "identifier");

         return ParseError();
      }

      PrecedenceGroup = currentTok().getIdentifierInfo();
   }

   SourceRange SR(KeywordLoc, currentTok().getEndLoc());

   auto Decl = OperatorDecl::Create(Context, SR, CurDeclAttrs.Access, Name,
                                    PrecedenceGroup);

   Decl->setAccessLoc(CurDeclAttrs.AccessLoc);
   return ActOnDecl(Decl);
}

ParseResult Parser::parseIdentifierExpr(bool parsingType, bool parseSubExpr,
                                        DeclarationName Ident,
                                        SourceLocation BeginLoc)
{
   if (!Ident) {
      BeginLoc = currentTok().getSourceLoc();
      if (!currentTok().is(tok::ident)) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                     currentTok().toString(), true,
                     parsingType ? "type" : "identifier");

         return ParseError();
      }

      Ident = currentTok().getIdentifierInfo();
   }

   if (Ident == Ident___nullptr) {
      return BuiltinIdentExpr::Create(Context, BeginLoc,
                                      BuiltinIdentifier::NULLPTR);
   }

   if (Ident == Ident___builtin_void) {
      return BuiltinIdentExpr::Create(Context, BeginLoc,
                                      BuiltinIdentifier::__builtin_void);
   }

   Expression* IdentExpr;
   if (Ident == Ident_Self) {
      IdentExpr = SelfExpr::Create(Context, currentTok().getSourceLoc(), true);
   }
   else {
      IdentExpr = new (Context)
          IdentifierRefExpr(BeginLoc, Ident, nullptr, parsingType);
   }

   if (!parseSubExpr)
      return IdentExpr;

   return maybeParseSubExpr(IdentExpr, parsingType);
}

ParseResult Parser::maybeParseSubExpr(Expression* ParentExpr, bool parsingType,
                                      bool parsingStmt)
{
   // pack expansions must immediately follow their operand without any
   // whitespace
   if (lookahead(false, true).is(tok::triple_period)) {
      advance(false, true);
      return maybeParseSubExpr(VariadicExpansionExpr::Create(
          Context, currentTok().getSourceLoc(), ParentExpr));
   }

   /// The '<' in a template argument list must immediately follow it's
   // parent expression, otherwise it will be parsed as an operator.
   if (lookahead(false, true).is(tok::smaller)) {
      advance(false, true);
      auto TAs = parseTemplateArgListExpr(ParentExpr, parsingType);
      if (!TAs)
         return ParseError();

      return maybeParseSubExpr(TAs.getExpr(), parsingType);
   }

   auto Next = lookahead(false);
   auto start = Next.getSourceLoc();

   // member access
   bool pointerAccess = Next.is(tok::arrow_single);
   if (Next.is(tok::period) || pointerAccess) {
      advance(false);
      advance(false);

      // Operator function reference
      if (currentTok().oneOf(Ident_infix, Ident_prefix, Ident_postfix)
          && validOperatorFollows()) {
         if (ParentExpr) {
            ParentExpr->setAllowNamespaceRef(true);
         }

         auto Loc = currentTok().getSourceLoc();
         auto Fix = tokenToFix(currentTok());

         advance();

         bool IsConvOp;
         auto OpName = parseOperatorName(Fix, IsConvOp);

         auto Expr = new (Context)
             IdentifierRefExpr(Loc, ParentExpr, OpName, pointerAccess);

         return maybeParseSubExpr(Expr, parsingType);
      }

      // tuple access
      if (currentTok().is(tok::integerliteral) && !parsingType) {
         unsigned index = unsigned(std::stoul(currentTok().getText()));
         auto tupleExpr = new (Context) TupleMemberExpr(
             currentTok().getSourceLoc(), ParentExpr, index, pointerAccess);

         return maybeParseSubExpr(tupleExpr, parsingType);
      }

      // .init or .deinit call
      if (currentTok().oneOf(tok::kw_init, Ident_deinit)
          && lookahead().is(tok::open_paren) && !parsingType) {
         auto Call = parseFunctionCall(false, ParentExpr, pointerAccess);

         Expression* Expr;
         if (Call) {
            Expr = Call.getExpr();
         }
         else {
            skipUntilProbableEndOfExpr();
            Expr = ParentExpr;
         }

         return maybeParseSubExpr(Expr, parsingType);
      }

      // Macro expansion
      if (currentTok().is(tok::macro_name)) {
         if (ParentExpr) {
            ParentExpr->setAllowNamespaceRef(true);
         }

         if (parsingStmt) {
            return parseMacroExpansionStmt(ParentExpr);
         }

         return parseMacroExpansionExpr(ParentExpr);
      }

      if (!currentTok().is(tok::ident)) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                     currentTok().toString(), true, "identifier");

         skipUntilProbableEndOfExpr();
         return ParentExpr;
      }

      if (ParentExpr) {
         ParentExpr->setAllowNamespaceRef(true);
      }

      Expression* Expr;

      // Trailing closure
      if (AllowBraceClosure() && !parsingType
          && lookahead(false).is(tok::open_brace)) {
         auto IdentLoc = currentTok().getSourceLoc();
         auto Ident = currentTok().getIdentifierInfo();

         advance();

         auto Result = parseTrailingClosure(false);
         if (!Result)
            return ParentExpr;

         auto Closure = Result.getExpr();
         auto* IdentExpr
             = new (Context) IdentifierRefExpr(IdentLoc, ParentExpr, Ident);

         Expr = AnonymousCallExpr::Create(Context, IdentLoc, IdentExpr, Closure,
                                          {});

         return maybeParseSubExpr(Expr, parsingType);
      }

      SourceLocation IdentLoc = currentTok().getSourceLoc();
      DeclarationName Name = currentTok().getIdentifierInfo();

      // If a template argument list follows, parse that first.
      TemplateArgListExpr* TAs = nullptr;
      if (lookahead(false, true).is(tok::smaller)) {
         advance();

         auto Res = parseTemplateArgListExpr(nullptr, false);
         if (!Res)
            return Res;

         TAs = Res.getExpr<TemplateArgListExpr>();
      }

      // Method call
      if (lookahead().is(tok::open_paren) && !parsingType) {
         ParseResult Call;
         if (AllowPattern) {
            Call = parseCallPattern(false, ParentExpr, pointerAccess, Name);
         }
         else {
            Call = parseFunctionCall(false, ParentExpr, pointerAccess, Name);
         }

         if (Call) {
            Expr = Call.getExpr();
         }
         else {
            skipUntilProbableEndOfExpr();
            Expr = ParentExpr;
         }

         if (TAs) {
            TAs->setParentExpr(Expr);
            Expr = TAs;
         }
      }
      // Member ref expression
      else {
         Expr = new (Context)
             IdentifierRefExpr(IdentLoc, ParentExpr, Name, pointerAccess);

         if (TAs) {
            TAs->setParentExpr(Expr);
            Expr = TAs;
         }
      }

      return maybeParseSubExpr(Expr, parsingType);
   }

   // Anonymous call
   if (Next.is(tok::open_paren) && !parsingType) {
      advance();

      auto args = parseCallArguments();
      SourceRange Parens(start, currentTok().getSourceLoc());

      // trailing closure
      if (AllowBraceClosure() && lookahead(false).is(tok::open_brace)
          && !parsingType) {
         advance();

         auto Result = parseTrailingClosure(false);
         if (!Result)
            return ParentExpr;

         auto Closure = Result.getExpr();
         args.args.push_back(Closure, Context);
      }

      auto call = AnonymousCallExpr::Create(Context, Parens, ParentExpr,
                                            args.args, args.labels);

      return maybeParseSubExpr(call, parsingType);
   }

   // Subscript expression
   if (Next.is(tok::open_square)) {
      advance();
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

      SourceRange SquareRange(start, currentTok().getSourceLoc());
      ParentExpr
          = SubscriptExpr::Create(Context, SquareRange, ParentExpr, indices);

      return maybeParseSubExpr(ParentExpr, parsingType);
   }

   // macro expansion
   if (Next.is(tok::macro_name)) {
      advance();

      if (parsingStmt) {
         return parseMacroExpansionStmt(ParentExpr);
      }

      return parseMacroExpansionExpr(ParentExpr);
   }

   // 'is' expression
   if (Next.oneOf(Ident_is, tok::exclaim_is) && !parsingType) {
      advance();

      auto Negate = currentTok().is(tok::exclaim_is);
      auto BeginLoc = ParentExpr->getSourceLoc();
      auto IsLoc = currentTok().getSourceLoc();
      advance();

      auto rhs = parseConstraintExpr();
      if (!rhs)
         return skipUntilProbableEndOfExpr();

      SourceRange SR;
      auto CE = rhs.getExpr<ConstraintExpr>();
      if (CE->getKind() == ConstraintExpr::Type) {
         SR = SourceRange(
             BeginLoc,
             CE->getTypeConstraint().getTypeExpr()->getSourceRange().getEnd());
      }
      else {
         SR = SourceRange(BeginLoc, currentTok().getSourceLoc());
      }

      return TypePredicateExpr::Create(Context, IsLoc, SR, ParentExpr, CE,
                                       Negate);
   }

   return ParentExpr;
}

ParseResult Parser::parseTemplateArgListExpr(Expression* ParentExpr,
                                             bool parsingType)
{
   Lexer::ModeRAII MR(*lexer, Lexer::Mode::ParsingTemplateArgs);
   assert(currentTok().is(tok::smaller) && "not a template argument list!");

   SourceLocation LAngleLoc = consumeToken();
   SmallVector<Expression*, 4> Args;

   while (!currentTok().is(tok::greater)) {
      auto NextExpr = parseExprSequence(DefaultFlags | F_StopAtGreater);

      if (NextExpr) {
         auto* E = NextExpr.getExpr();
         if (auto* Ty = dyn_cast<TypeExpr>(E)) {
            Ty->setIsMeta(false);
         }
         else if (auto* Ident = dyn_cast<IdentifierRefExpr>(E)) {
            Ident->setInTypePos(true);
         }

         Args.push_back(E);
      }

      advance();
      if (currentTok().is(tok::comma)) {
         advance();
      }
      else if (currentTok().is(tok::eof)) {
         lexer->backtrack();
         break;
      }
   }

   SourceRange AngleRange(LAngleLoc, currentTok().getSourceLoc());
   return TemplateArgListExpr::Create(Context, AngleRange, ParentExpr, Args);
}

ParseResult Parser::parseCollectionLiteral()
{
   assert(currentTok().is(tok::open_square));

   SourceLocation LSquareLoc = currentTok().getSourceLoc();

   bool isDictionary = false;
   bool first = true;

   SmallVector<Expression*, 8> keys;
   SmallVector<Expression*, 8> values;

   advance();

   while (!currentTok().is(tok::close_square)) {
      if (currentTok().is(tok::eof)) {
         SP.diagnose(err_unexpected_eof, currentTok().getSourceLoc(), true,
                     "']'");
         return ParseExprError();
      }
      if (first && currentTok().is(tok::colon)) {
         advance();
         isDictionary = true;
         break;
      }

      if (AllowPattern && currentTok().oneOf(Ident_is, tok::kw_var, tok::kw_let)
          && !isDictionary) {
         return parseArrayPattern(LSquareLoc, values);
      }

      auto key = parseExprSequence(DefaultFlags | F_StopAtColon);
      if (!key) {
         skipUntilEven(tok::open_square);
         return ParseError();
      }

      advance();

      // fixed-size array type expression
      if (first && currentTok().is(tok::semicolon)) {
         advance();

         Expression* Size = nullptr;
         auto SizeRes = parseExprSequence();
         if (SizeRes)
            Size = SizeRes.getExpr();

         expect(tok::close_square);

         SourceRange SR(LSquareLoc, currentTok().getSourceLoc());
         return ArrayTypeExpr::Create(Context, SR, SourceType(key.getExpr()),
                                      StaticExpr::Create(Context, Size));
      }

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
      if (currentTok().is(tok::comma)) {
         advance();
      }
      else if (currentTok().is(tok::close_square)) {
         break;
      }
      else {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                     currentTok().toString(), true, "',' or ']'");

         skipUntilProbableEndOfExpr();
         break;
      }
   }

   SourceRange SquareRange(LSquareLoc, currentTok().getSourceLoc());
   if (isDictionary)
      return DictionaryLiteral::Create(Context, SquareRange, keys, values);

   return ArrayLiteral::Create(Context, SquareRange, values);
}

string Parser::prepareStringLiteral(Token const& tok)
{
   llvm::SmallString<128> str;
   bool escaped = false;

   const char* ptr = tok.getText().data();
   unsigned Len = (unsigned)tok.getText().size();

   for (unsigned i = 0; i < Len; ++ptr, ++i) {
      char c = *ptr;

      if (escaped) {
         str += support::escape_char(c);
         escaped = false;
      }
      else if (c == '\\') {
         escaped = true;
      }
      else if (c == '$' && ptr[1] == '$') {
         // ignore this '$', append the next one
      }
      else {
         str += c;
      }
   }

   return str.str();
}

ParseResult Parser::parseUnaryExpr(bool parsingStmt)
{
   ParseResult Expr;

   if (currentTok().is(tok::at)) {
      return parseAttributedExpr();
   }

   if (currentTok().is(Ident_unsafe)) {
      advance();

      Expr = parseUnaryExpr();
      if (Expr)
         Expr.getExpr()->setUnsafe(true);
   }
   else if (currentTok().is(tok::macro_expression)) {
      Expr = currentTok().getExpr();
   }
   else if (currentTok().oneOf(tok::macro_statement, tok::macro_declaration)) {
      enum DiagKind {
         Expression,
         Statement,
         Type,
         Decl,
      };

      SP.diagnose(err_bad_macro_variable_kind, currentTok().getSourceLoc(),
                  currentTok().is(tok::macro_statement) ? Statement : Decl,
                  Expression);

      return ParseError();
   }
   else if (currentTok().is(tok::open_brace)) {
      Expr = parseTrailingClosure(true);
   }
   else if (currentTok().is(tok::kw_try)) {
      SourceLocation TryLoc = currentTok().getSourceLoc();
      TryExpr::TryKind Kind = TryExpr::Normal;

      if (lookahead(false, true).is(tok::question)) {
         Kind = TryExpr::Fallible;
         advance(false, true);
      }
      else if (lookahead(false, true).is(tok::exclaim)) {
         Kind = TryExpr::Force;
         advance(false, true);
      }

      advance();

      Expression* E = parseExprSequence().tryGetExpr();
      Expr = TryExpr::Create(Context, TryLoc, Kind, E);
   }
   else if (currentTok().is(tok::kw_await)) {
      SourceLocation AwaitLoc = consumeToken();

      Expression* E = parseExprSequence().tryGetExpr();
      Expr = AwaitExpr::Create(Context, AwaitLoc, E);
   }
   else if (currentTok().getIdentifierInfo() == Ident___traits) {
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
   else if (currentTok().is(tok::period) && lookahead().is(tok::ident)) {
      advance();

      if (lookahead().is(tok::open_paren)) {
         ParseResult Call;
         if (AllowPattern) {
            Call = parseCallPattern(false);
         }
         else {
            Call = parseFunctionCall(false);
         }

         if (Call) {
            auto* E = Call.getExpr();
            if (auto* CE = dyn_cast<AnonymousCallExpr>(E)) {
               cast<IdentifierRefExpr>(CE->getParentExpr())
                   ->setLeadingDot(true);
            }
            else {
               cast<CasePattern>(E)->setLeadingDot(true);
            }
         }

         Expr = Call;
      }
      else {
         SourceRange SR(currentTok().getSourceLoc());
         auto* Ident = currentTok().getIdentifierInfo();

         auto* IE = new (Context) IdentifierRefExpr(SR, Ident);
         IE->setLeadingDot(true);

         Expr = IE;
      }
   }
   else if (currentTok().is(tok::kw_self)) {
      Expr = maybeParseSubExpr(
          SelfExpr::Create(Context, currentTok().getSourceLoc(), false));
   }
   else if (currentTok().is(Ident_Self)) {
      Expr = maybeParseSubExpr(
          SelfExpr::Create(Context, currentTok().getSourceLoc(), true));
   }
   else if (currentTok().is(tok::kw_super)) {
      Expr = maybeParseSubExpr(
          SuperExpr::Create(Context, currentTok().getSourceLoc()));
   }
   else if (currentTok().is(tok::closure_arg)) {
      if (UnnamedClosureArgumentStack.empty()) {
         SP.diagnose(err_anon_closure_arg, currentTok().getSourceLoc());
         return ParseError();
      }

      llvm::APInt API(64, currentTok().getText(), 10);
      if (API.ugt(256)) {
         SP.diagnose(err_anon_closure_idx_too_high, 256, API,
                     currentTok().getSourceLoc());

         API = 0;
      }

      auto Idx = (unsigned)API.getZExtValue();
      if (Idx + 1 > UnnamedClosureArgumentStack.top().NumArgs)
         UnnamedClosureArgumentStack.top().NumArgs = Idx + 1;

      UnnamedClosureArgumentStack.top().ArgLocs[Idx]
          = currentTok().getSourceLoc();

      auto DeclName = Context.getDeclNameTable().getClosureArgumentName(Idx);
      Expr = new (Context)
          IdentifierRefExpr(currentTok().getSourceLoc(), DeclName);
   }
   else if (currentTok().oneOf(tok::kw_init, Ident_deinit)
            && lookahead().is(tok::open_paren)) {
      Expr = parseFunctionCall();
   }
   else if (currentTok().oneOf(Ident_infix, Ident_prefix, Ident_postfix)
            && validOperatorFollows()) {
      // Operator function reference
      auto Loc = currentTok().getSourceLoc();
      auto Fix = tokenToFix(currentTok().getKind());

      advance();

      bool IsConvOp;
      auto OpName = parseOperatorName(Fix, IsConvOp);

      Expr = parseIdentifierExpr(false, false, OpName, Loc);
   }
   else if (currentTok().is(Ident_default)) {
      Expr = BuiltinIdentExpr::Create(Context, currentTok().getSourceLoc(),
                                      BuiltinIdentifier::defaultValue);
   }
   else if (currentTok().is(tok::ident)) {
      // single argument lambda
      if (lookahead().is(tok::arrow_double)) {
         auto ArgLoc = currentTok().getSourceLoc();
         auto ArrowLoc = lookahead().getSourceLoc();
         auto argName = currentTok().getIdentifierInfo();
         SourceType AutoTy(Context.getAutoType());

         auto arg = FuncArgDecl::Create(Context, ArgLoc, ArgLoc, argName,
                                        nullptr, ArgumentConvention::Default,
                                        AutoTy, nullptr, false);

         advance();
         advance();

         Statement* body = parseNextStmt().tryGetStatement();
         if (auto E = dyn_cast_or_null<Expression>(body))
            body = ReturnStmt::Create(Context, body->getSourceLoc(), E);

         Expr = LambdaExpr::Create(Context, SourceRange(ArgLoc, ArgLoc),
                                   ArrowLoc, AutoTy, {arg}, body);
      }

      SourceLocation IdentLoc = currentTok().getSourceLoc();
      DeclarationName Ident = currentTok().getIdentifierInfo();

      Expression *IdentExpr;
      if (Ident == Ident___nullptr) {
         IdentExpr = BuiltinIdentExpr::Create(
             Context, IdentLoc, BuiltinIdentifier::NULLPTR);
      }
      else if (Ident == Ident___builtin_void) {
         IdentExpr =  BuiltinIdentExpr::Create(
             Context, IdentLoc, BuiltinIdentifier::__builtin_void);
      }
      else if (Ident == Ident_Self) {
         IdentExpr = SelfExpr::Create(
             Context, currentTok().getSourceLoc(), true);
      }
      else {
         IdentExpr = new (Context)
             IdentifierRefExpr(IdentLoc, nullptr, Ident);
      }

      // If a template argument list follows, parse that first.
      if (lookahead(false, true).is(tok::smaller)) {
         advance(false, true);

         auto Res = parseTemplateArgListExpr(IdentExpr, false);
         if (!Res)
            return Res;

         IdentExpr = Res.getExpr();
      }

      Token next = lookahead(false);

      // function call
      if (next.is(tok::open_paren)) {
         if (AllowPattern) {
            Expr = parseCallPattern(true, IdentExpr, false, Ident);
         }
         else {
            Expr = parseFunctionCall(true, IdentExpr, false, Ident);
         }

         if (!Expr) {
            return Expr;
         }
      }
      // Trailing closure
      else if (AllowBraceClosure() && lookahead(false).is(tok::open_brace)) {
         advance();

         auto Result = parseTrailingClosure(false);
         if (!Result)
            return Expr;

         auto Closure = Result.getExpr();
         Expr = AnonymousCallExpr::Create(Context, IdentLoc, IdentExpr, Closure,
                                          {});
      }
      else {
         Expr = IdentExpr;
      }
   }
   else if (currentTok().is(tok::macro_name)) {
      Expr = parseMacroExpansionExpr();
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
                     SourceLocation(Offset + currentTok().getText().size()));

      string str = prepareStringLiteral(currentTok());

      // concatenate adjacent string literals
      if (lookahead().is(tok::stringliteral)) {
         while (lookahead().is(tok::stringliteral)) {
            advance();
            str += prepareStringLiteral(currentTok());
         }
      }

      auto Alloc = (char*)Context.Allocate(str.size());
      std::copy(str.begin(), str.end(), Alloc);

      Expr = StringLiteral::Create(Context, SR, StringRef(Alloc, str.size()));
   }
   else if (currentTok().is(tok::interpolation_begin)) {
      advance();

      unsigned Offset = currentTok().getSourceLoc().getOffset();
      unsigned BeginOffset = Offset - 1;
      SourceRange SR(SourceLocation(Offset - 1),
                     SourceLocation(Offset + currentTok().getText().size()));

      string str = prepareStringLiteral(currentTok());
      auto Alloc = (char*)Context.Allocate(str.size());
      std::copy(str.begin(), str.end(), Alloc);

      auto strLiteral
          = StringLiteral::Create(Context, SR, StringRef(Alloc, str.size()));

      advance();
      assert(currentTok().is(tok::expr_begin) && "not an interpolation");

      SmallVector<Expression*, 4> strings{strLiteral};
      while (1) {
         advance();

         auto nextString = parseExprSequence();
         if (!nextString) {
            skipUntilProbableEndOfExpr();
            advance();

            return ParseError();
         }

         strings.push_back(nextString.getExpr());
         advance();

         assert(currentTok().is(tok::interpolation_end));
         if (!lookahead().is(tok::stringliteral))
            break;

         advance();

         Offset = currentTok().getSourceLoc().getOffset();
         SR = SourceRange(
             SourceLocation(Offset - 1),
             SourceLocation(Offset + currentTok().getText().size()));

         str = prepareStringLiteral(currentTok());
         Alloc = (char*)Context.Allocate(str.size());
         std::copy(str.begin(), str.end(), Alloc);

         auto lit
             = StringLiteral::Create(Context, SR, StringRef(Alloc, str.size()));

         strings.push_back(lit);

         if (!lookahead().is(tok::expr_begin)) {
            break;
         }

         advance();
      }

      SourceRange TotalRange(SourceLocation(BeginOffset),
                             strings.back()->getSourceRange().getEnd());

      Expr = StringInterpolation::Create(Context, TotalRange, strings);
   }
   else if (currentTok().is(tok::kw_if)) {
      auto IfLoc = currentTok().getSourceLoc();
      advance();

      auto Condition = parseExprSequence(DefaultFlags | F_StopAtThen);
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

   return maybeParseSubExpr(Expr.getExpr(), false, parsingStmt);
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

   cdot::Type* Ty = Context.getIntTy();

   using Suffix = IntegerLiteral::Suffix;
   Suffix suffix = Suffix::None;

   auto next = lookahead(false, true);
   if (next.is(tok::ident)) {
      auto modifier = next.getIdentifier();
      suffix = StringSwitch<Suffix>(modifier)
                   .Case("u", Suffix::u)
                   .Case("i", Suffix::i)
#define CDOT_BUILTIN_INT(Name, BW, Unsigned) .Case(#Name, Suffix::Name)
#include "cdotc/Basic/BuiltinTypes.def"
                   .Default(Suffix::None);

      if (suffix != Suffix::None) {
         advance(false, true);

         switch (suffix) {
#define CDOT_BUILTIN_INT(Name, BW, Unsigned)                                   \
   case Suffix::Name:                                                          \
      Ty = Context.getBuiltinType(BuiltinType::Name);                          \
      break;
#include "cdotc/Basic/BuiltinTypes.def"
         case Suffix::u:
            Ty = Context.getUIntTy();
            break;
         case Suffix::i:
            Ty = Context.getIntTy();
            break;
         default:
            llvm_unreachable("bad suffix!");
         }
      }
   }

   LiteralParser Parser(text);
   auto APS = Parser.parseInteger(Ty->getBitwidth(), !Ty->isUnsigned());

   return IntegerLiteral::Create(Context, Loc, Ty, APS.APS, suffix);
}

Expression* Parser::parseFloatingPointLiteral()
{
   auto text = currentTok().getText();

   unsigned Offset = currentTok().getSourceLoc().getOffset();
   SourceRange Loc(SourceLocation(Offset),
                   SourceLocation(Offset + text.size()));

   llvm::APFloat APF(0.0);
   (void)APF.convertFromString(text, llvm::APFloat::rmNearestTiesToEven);

   cdot::Type* Ty;
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

   return FPLiteral::Create(Context, Loc, Ty, APF, suffix);
}

Expression* Parser::parseCharacterLiteral()
{
   auto text = currentTok().getText();

   unsigned Offset = currentTok().getSourceLoc().getOffset();
   SourceRange Loc(SourceLocation(Offset),
                   SourceLocation(Offset + (unsigned)text.size()));

   LiteralParser Parser(text);
   auto Result = Parser.parseCharacter();

   if (Result.Char > 255) {
      return CharLiteral::Create(Context, Loc, Context.getCharTy(),
                                 Result.Char);
   }

   return CharLiteral::Create(Context, Loc, Context.getCharTy(),
                              static_cast<char>(Result.Char));
}

Parser::ParenExprKind Parser::getParenExprKind()
{
   auto IsLambda = false;
   bool IsTuple = false;
   bool MaybeFuncTy = false;

   Lexer::LookaheadRAII L(*lexer);
   assert(currentTok().is(tok::open_paren));

   L.advance();
   if (currentTok().is(tok::close_paren)) {
      IsTuple = true;
   }

   auto begin = currentTok().getSourceLoc();

   unsigned OpenParens = 1;
   unsigned OpenSquare = 0;
   unsigned OpenBrace = 0;

   while (OpenParens) {
      switch (currentTok().getKind()) {
      case tok::open_paren:
         ++OpenParens;
         break;
      case tok::close_paren:
         --OpenParens;
         break;
      case tok::open_square:
         ++OpenSquare;
         break;
      case tok::close_square:
         --OpenSquare;
         break;
      case tok::open_brace:
         ++OpenBrace;
         break;
      case tok::close_brace:
         --OpenBrace;
         break;
      case tok::comma:
         IsTuple |= (!OpenBrace && !OpenSquare && OpenParens == 1);
         break;
      case tok::eof:
         SP.diagnose(err_unexpected_eof, currentTok().getSourceLoc(), true,
                     "')'");
         SP.diagnose(note_to_match_this, begin);

         return ParenExprKind::Error;
      default:
         break;
      }

      L.advance();
   }

   if (currentTok().is(tok::arrow_single)) {
      MaybeFuncTy = true;
      L.advance();
   }

   if (currentTok().is(tok::arrow_double)) {
      IsLambda = true;
   }

   if (IsLambda) {
      return ParenExprKind::Lambda;
   }

   if (MaybeFuncTy) {
      return ParenExprKind::FunctionType;
   }

   if (IsTuple) {
      return ParenExprKind::Tuple;
   }

   return ParenExprKind::Expr;
}

ParseResult Parser::parseParenExpr()
{
   Lexer::ModeRAII MR(*lexer, Lexer::Mode::Normal);
   auto Kind = getParenExprKind();

   switch (Kind) {
   case ParenExprKind::Error:
      return ParseError();
   case ParenExprKind::Expr: {
      SourceLocation BeginLoc = currentTok().getSourceLoc();

      advance();
      auto ExprRes = parseExprSequence(DefaultFlags & ~F_StopAtNewline);

      advance();

      if (ExprRes) {
         SourceRange ParenRange(BeginLoc, currentTok().getSourceLoc());
         if (!currentTok().is(tok::close_paren)) {
            errorUnexpectedToken(currentTok(), tok::close_paren);
         }

         auto* E = ExprRes.getExpr();

         // An expression of the form (x...) is also a tuple.
         if (E->isVariadicArgPackExpansion()) {
            return TupleLiteral::Create(Context, ParenRange, E);
         }

         // Otherwise, it's just a parenthesized expression.
         E = ParenExpr::Create(Context, ParenRange, E);
         return maybeParseSubExpr(E);
      }

      return ParseError();
   }
   case ParenExprKind::Tuple:
      if (AllowPattern)
         return parseTuplePattern();

      return parseTupleLiteral();
   case ParenExprKind::Lambda:
      return parseLambdaExpr();
   case ParenExprKind::FunctionType:
      return parseFunctionType().tryGet().getTypeExpr();
   }
}

Parser::ParseArtefactKind Parser::getNextArtefactKind()
{
   // Skip attributes.
   Lexer::LookaheadRAII L(*lexer);
   if (currentTok().is(tok::at)) {
      while (currentTok().is(tok::at)) {
         L.advance();
         L.advance();

         if (currentTok().is(tok::open_paren)) {
            L.advance();

            unsigned open = 1;
            unsigned closed = 0;
            while (true) {
               if (currentTok().is(tok::open_paren))
                  ++open;
               else if (currentTok().is(tok::close_paren))
                  ++closed;
               else if (currentTok().is(tok::eof)) {
                  errorUnexpectedToken(currentTok(), tok::close_paren);
                  break;
               }

               if (open == closed)
                  break;

               L.advance();
            }

            L.advance();
         }
      }
   }

   switch (currentTok().getKind()) {
   case tok::kw_def:
   case tok::kw_alias:
   case tok::kw_typedef:
   case tok::kw_namespace:
   case tok::kw_struct:
   case tok::kw_class:
   case tok::kw_union:
   case tok::kw_enum:
   case tok::kw_protocol:
   case tok::kw_extend:
   case tok::kw_public:
   case tok::kw_private:
   case tok::kw_fileprivate:
   case tok::kw_internal:
   case tok::kw_protected:
   case tok::kw_static:
   case tok::kw_abstract:
   case tok::kw_prop:
   case tok::kw_init:
   case tok::kw_associatedType:
   case tok::kw_mutating:
   case tok::kw_declare:
   case tok::kw_module:
   case tok::kw_import:
   case tok::kw_static_assert:
   case tok::kw_static_print:
      return K_Decl;
   case tok::ident:
      if (currentTok().is(Ident_using)) {
         return K_Decl;
      }
      if ((currentTok().oneOf(Ident_infix, Ident_prefix, Ident_postfix)
           && validOperatorFollows())) {
         return K_Decl;
      }

      LLVM_FALLTHROUGH;
   default:
      return K_Statement;
   }
}

bool Parser::moduleDeclFollows()
{
   // Skip attributes.
   Lexer::LookaheadRAII L(*lexer);
   if (currentTok().is(tok::at)) {
      while (currentTok().is(tok::at)) {
         L.advance();
         L.advance();

         if (currentTok().is(tok::open_paren)) {
            L.advance();

            unsigned open = 1;
            unsigned closed = 0;
            while (true) {
               if (currentTok().is(tok::open_paren))
                  ++open;
               else if (currentTok().is(tok::close_paren))
                  ++closed;
               else if (currentTok().is(tok::eof)) {
                  errorUnexpectedToken(currentTok(), tok::close_paren);
                  break;
               }

               if (open == closed)
                  break;

               L.advance();
            }

            L.advance();
         }
      }
   }

   return currentTok().is(tok::kw_module);
}

ParseResult Parser::parseTupleLiteral()
{
   auto LParenLoc = currentTok().getSourceLoc();
   llvm::SmallVector<Expression*, 4> elements;

   advance();

   while (!currentTok().is(tok::close_paren)) {
      string label;

      if (currentTok().getKind() == tok::ident && lookahead().is(tok::colon)) {
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

Expression* getExpression(SemaPass& SP, SourceLocation loc, SequenceElement& El)
{
   switch (El.getKind()) {
   case SequenceElement::EF_Operator: {
      auto* II = &SP.getContext().getIdentifiers().get(
          op::toString(El.getOperatorKind()));

      return new (SP.getContext()) IdentifierRefExpr(loc, II);
   }
   case SequenceElement::EF_Expression:
      return El.getExpr();
   case SequenceElement::EF_PossibleOperator:
      return new (SP.getContext()) IdentifierRefExpr(loc, move(El.getOp()));
   }
}

} // anonymous namespace

static IdentifierRefExpr* getSimpleIdentifier(Expression* Expr)
{
   auto Ident = dyn_cast<IdentifierRefExpr>(Expr);
   if (!Ident || Ident->isVariadicArgPackExpansion() || Ident->getParentExpr())
      return nullptr;

   return Ident;
}

ParseResult Parser::parseExprSequence(int Flags)
{
   auto start = currentTok().getSourceLoc();
   AllowTrailingClosureRAII closureRAII(*this,
                                        (Flags & F_AllowBraceClosure) != 0);

   std::vector<SequenceElement> frags;
   bool done = false;
   while (!done) {
      uint8_t whitespace = SequenceElement::None;
      if (currentTok().is(tok::space)) {
         whitespace |= SequenceElement::Left;
         advance();
      }

      switch (currentTok().getKind()) {
      case tok::ident: {
         if (currentTok().is(Ident_is)) {
            lexer->backtrack();

            auto Expr = ExprSequence::Create(Context, frags);
            frags.clear();

            auto SubExpr = maybeParseSubExpr(Expr);
            if (!SubExpr)
               return ParseError();

            frags.emplace_back(SubExpr.getExpr());
            break;
         }
         if (currentTok().is(Ident_as)) {
            op::OperatorKind Kind = op::As;
            SourceLocation Loc = currentTok().getSourceLoc();

            if (lookahead(false, true).is(tok::exclaim)) {
               advance(false, true);
               Kind = op::AsExclaim;
            }
            else if (lookahead(false, true).is(tok::question)) {
               advance(false, true);
               Kind = op::AsQuestion;
            }

            if (lookahead(false, true).isWhitespace()) {
               whitespace |= SequenceElement::Right;
            }

            frags.emplace_back(Kind, whitespace, Loc);
            break;
         }
         if (currentTok().is(Ident_then) && (Flags & F_StopAtThen) != 0) {
            lexer->backtrack();
            done = true;
            break;
         }
         if (currentTok().is(Ident_with) && (Flags & F_ParsingType) != 0) {
            lexer->backtrack();
            done = true;
            break;
         }
         if (currentTok().is(Ident_where)) {
            lexer->backtrack();
            done = true;
            break;
         }

         auto expr = parseUnaryExpr((Flags & F_ParsingStatement) != 0 && frags.empty());
         if (!expr) {
            break;
         }

         if (lookahead(false, true).isWhitespace()) {
            whitespace |= SequenceElement::Right;
         }

         auto *Stmt = expr.getStatement();
         auto *E = dyn_cast<Expression>(Stmt);

         if (!E) {
            assert(frags.empty());
            return Stmt;
         }

         if (auto Ident = getSimpleIdentifier(E)) {
            frags.emplace_back(Ident->getIdentInfo(), whitespace,
                               Ident->getSourceLoc());
         }
         else {
            frags.emplace_back(E);
         }

         break;
      }
      case tok::op_ident:
         if (lookahead(false, true).isWhitespace()) {
            whitespace |= SequenceElement::Right;
         }

         frags.emplace_back(currentTok().getIdentifierInfo(), whitespace,
                            currentTok().getSourceLoc());

         break;
#define CDOT_OPERATOR_TOKEN(Name, Spelling) case tok::Name:
#include "cdotc/Lex/Tokens.def"
         if (currentTok().is(tok::colon) && (Flags & F_StopAtColon) != 0) {
            lexer->backtrack();
            done = true;
            break;
         }
         if (currentTok().is(tok::equals) && (Flags & F_StopAtEquals) != 0) {
            lexer->backtrack();
            done = true;
            break;
         }
         if (currentTok().is(tok::greater) && (Flags & F_StopAtGreater) != 0) {
            lexer->backtrack();
            done = true;
            break;
         }

         if (lookahead(false, true).isWhitespace()) {
            whitespace |= SequenceElement::Right;
         }

         frags.emplace_back(op::fromString(currentTok().toString()), whitespace,
                            currentTok().getSourceLoc());

         break;
      case tok::underscore:
         if (frags.empty()) {
            if (lookahead(false, true).isWhitespace()) {
               whitespace |= SequenceElement::Right;
            }

            auto* II = &Context.getIdentifiers().get("_");
            frags.emplace_back(II, whitespace, currentTok().getSourceLoc());
            break;
         }

         LLVM_FALLTHROUGH;
      case tok::comma:
      case tok::semicolon:
      case tok::close_paren:
      case tok::newline:
      case tok::eof:
      case tok::sentinel:
      case tok::close_brace:
      case tok::close_square:
      case tok::arrow_double:
      case tok::arrow_single:
      case tok::kw_def:
      case tok::kw_let:
      case tok::kw_var:
      case tok::kw_return:
      case tok::kw_else:
      case tok::kw_while:
      case tok::kw_loop:
      case tok::kw_match:
      case tok::kw_for:
      case tok::kw_case:
      case tok::kw_throw:
      case tok::kw_catch:
      case tok::kw_finally:
      case tok::kw_alias:
      case tok::kw_typedef:
      case tok::kw_namespace:
      case tok::kw_struct:
      case tok::kw_class:
      case tok::kw_union:
      case tok::kw_enum:
      case tok::kw_protocol:
      case tok::kw_extend:
      case tok::kw_public:
      case tok::kw_private:
      case tok::kw_protected:
      case tok::kw_static:
      case tok::kw_abstract:
      case tok::kw_prop:
      case tok::kw_continue:
      case tok::kw_init:
      case tok::kw_associatedType:
      case tok::kw_break:
      case tok::kw_mutating:
      case tok::kw_declare:
      case tok::kw_module:
      case tok::kw_import:
      case tok::kw_static_if:
      case tok::kw_static_for:
      case tok::kw_static_assert:
      case tok::kw_static_print:
      case tok::kw___debug:
      case tok::kw___unreachable:
      case tok::interpolation_end:
      case tok::expr_begin:
         if (!frags.empty())
            lexer->backtrack();

         done = true;
         break;
      case tok::kw_try:
         if ((Flags & F_AllowTry) == 0) {
            lexer->backtrack();
            done = true;
            break;
         }

         goto case_unary_expr;
      case tok::open_brace:
         if ((Flags & F_AllowBraceClosure) == 0) {
            lexer->backtrack();
            done = true;
            break;
         }

         goto case_unary_expr;
      default:
      case_unary_expr : {
         auto seqResult = parseUnaryExpr();
         if (!seqResult) {
            return ParseError();
         }

         frags.emplace_back(seqResult.getExpr());
         break;
      }
      }

      if (!done) {
         advance((Flags & F_StopAtNewline) == 0, true);
      }
   }

   if (frags.size() == 1) {
      return getExpression(SP, start, frags.front());
   }

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
   enum Kind : unsigned char { None, Expr, Stmt, Type, String, Identifier };

   TraitArguments(Kind arg1 = None, Kind arg2 = None, Kind arg3 = None,
                  Kind arg4 = None)
       : args{arg1, arg2, arg3, arg4}
   {
   }

   Kind args[4];
};

using Kind = TraitArguments::Kind;

TraitArguments traitArgs[] = {
    {Kind::Stmt},               // Compiles
    {Kind::Stmt},               // CompileErrors
    {Kind::Type, Kind::String}, // HasMember
    {Kind::Type, Kind::String}, // HasProperty
    {Kind::Type, Kind::String}, // HasStaticMember
    {Kind::Type, Kind::String}, // HasStaticProperty
    {Kind::Type, Kind::String}, // HasMethod
    {Kind::Type, Kind::String}, // HasStaticMethod,
    {Kind::Identifier},         // ValidIdentifier
    {Kind::Identifier},         // ValidFunction
    {Kind::Type},               // IsInteger
    {Kind::Type},               // IsFloat
    {Kind::Type},               // IntegerBitwidth
    {Kind::Type},               // IsUnsigned
    {Kind::Type},               // FPPrecision
    {Kind::Expr},               // Arity
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
   auto kind = StringSwitch<TraitsExpr::Kind>(str)
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
                   .Case("is_integral", TraitsExpr::IsInteger)
                   .Case("is_floating_point", TraitsExpr::IsFloat)
                   .Case("bitwidth_of", TraitsExpr::IntegerBitwidth)
                   .Case("is_unsigned", TraitsExpr::IsUnsigned)
                   .Case("fp_precision", TraitsExpr::FPPrecision)
                   .Case("arity", TraitsExpr::Arity)
                   .Default(TraitsExpr::Invalid);

   if (kind == TraitsExpr::Invalid) {
      SP.diagnose(err_invalid_traits, currentTok().getSourceLoc(), str);
      return skipUntilEven(tok::open_paren);
   }

   std::vector<TraitsArgument> args;
   auto& argKinds = traitArgs[kind];

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

         args.emplace_back(currentTok().getIdentifierInfo()->getIdentifier());
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

ParseResult Parser::parseVarDecl(bool allowTrailingClosure, bool skipKeywords,
                                 bool ignoreDeclAttrs)
{
   bool isLet = false;
   SourceLocation VarOrLetLoc;

   if (!skipKeywords) {
      assert(currentTok().oneOf(tok::kw_var, tok::kw_let));

      isLet = currentTok().is(tok::kw_let);
      VarOrLetLoc = consumeToken(tok::kw_var, tok::kw_let);

      if (currentTok().is(tok::open_paren))
         return parseDestructuringDecl(isLet);
   }

   if (!currentTok().is(tok::ident)) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                  currentTok().toString(), true, "identifier");

      return skipUntilProbableEndOfStmt();
   }

   auto Name = currentTok().getIdentifierInfo();

   SourceType type;
   Expression* value = nullptr;
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

   SourceLocation EqualsLoc;
   if (lookahead().is(tok::equals)) {
      advance();
      EqualsLoc = consumeToken();

      int Flags = DefaultFlags;
      if (!allowTrailingClosure) {
         Flags &= ~F_AllowBraceClosure;
      }

      auto valueResult = parseExprSequence(Flags);
      if (valueResult)
         value = valueResult.getExpr();
      else
         return skipUntilProbableEndOfStmt();
   }

   if (!InRecordScope && !InFunctionScope) {
      auto G = GlobalVarDecl::Create(Context, CurDeclAttrs.Access, VarOrLetLoc,
                                     ColonLoc, isLet, Name, type, value);

      G->setAccessLoc(CurDeclAttrs.AccessLoc);
      G->setEqualsLoc(EqualsLoc);

      return ActOnDecl(G);
   }

   if (!ignoreDeclAttrs && CurDeclAttrs.AccessLoc) {
      SP.diagnose(err_generic_error,
                  "local variables cannot have an access specifier",
                  CurDeclAttrs.AccessLoc);
   }

   auto L = LocalVarDecl::Create(Context, AccessSpecifier::Public, VarOrLetLoc,
                                 ColonLoc, isLet, Name, type, value);

   L->setAccessLoc(CurDeclAttrs.AccessLoc);
   L->setEqualsLoc(EqualsLoc);
   L->setLexicalContext(&SP.getDeclContext());

   return L;
}

ParseResult Parser::parseDestructuringDecl(bool isLet, bool isForIn)
{
   assert(currentTok().is(tok::open_paren) && "should not be called otherwise");

   SmallVector<VarDecl*, 8> decls;
   bool global = SP.getDeclContext().isGlobalDeclContext();
   SourceLocation VarOrLetLoc = currentTok().getSourceLoc();

   advance();

   while (!currentTok().is(tok::close_paren)) {
      if (!currentTok().is(tok::ident)) {
         errorUnexpectedToken(currentTok(), tok::ident);
         skipUntilNextDecl();

         return ParseError();
      }

      auto loc = currentTok().getSourceLoc();
      auto Name = currentTok().getIdentifierInfo();
      if (global) {
         auto G
             = GlobalVarDecl::Create(Context, CurDeclAttrs.Access, VarOrLetLoc,
                                     loc, isLet, Name, SourceType(), nullptr);

         ActOnDecl(G);
         decls.push_back(G);
      }
      else {
         auto L
             = LocalVarDecl::Create(Context, CurDeclAttrs.Access, VarOrLetLoc,
                                    loc, isLet, Name, SourceType(), nullptr);

         L->setLexicalContext(&SP.getDeclContext());
         decls.push_back(L);
      }

      advance();
      if (currentTok().is(tok::comma))
         advance();
   }

   SourceType type;
   Expression* value = nullptr;

   if (lookahead().is(tok::colon) && !isForIn) {
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

   if (lookahead().is(tok::equals) && !isForIn) {
      advance();
      advance();

      value = parseExprSequence().tryGetExpr();
   }

   SourceRange SR(VarOrLetLoc, currentTok().getSourceLoc());
   auto* D = DestructuringDecl::Create(Context, SR, decls, type, value);

   if (isForIn) {
      D->setLexicalContext(&SP.getDeclContext());
      return D;
   }

   return ActOnDecl(D);
}

SmallVector<FuncArgDecl*, 2> Parser::parseFuncArgs(SourceLocation& varargLoc,
                                                   bool ImplicitUnderscores)
{
   SmallVector<FuncArgDecl*, 2> args;
   parseFuncArgs(varargLoc, args, ImplicitUnderscores);

   return args;
}

void Parser::parseFuncArgs(SourceLocation& varargLoc,
                           SmallVectorImpl<FuncArgDecl*>& args,
                           bool ImplicitUnderscores)
{
   if (!currentTok().is(tok::open_paren)) {
      lexer->backtrack();
      return;
   }

   bool foundDefault = false;
   varargLoc = SourceLocation();

   SmallVector<Attr*, 1> Attrs;
   advance();

   unsigned i = 0;
   while (!currentTok().is(tok::close_paren)) {
      if (varargLoc) {
         SP.diagnose(err_vararg_must_be_last, currentTok().getSourceLoc());
         SP.diagnose(note_previous_vararg_here, varargLoc);
      }

      SourceType argType;
      Expression* defaultVal = nullptr;
      bool templateArgExpansion = false;

      if (currentTok().is(tok::at)) {
         parseAttributes(Attrs, AttrClass::Decl);
      }

      SourceLocation ColonLoc;
      SourceLocation NameLoc;

      DeclarationName ArgName;
      IdentifierInfo* ArgLabel = nullptr;

      // C-Style vararg
      if (currentTok().is(tok::triple_period)) {
         varargLoc = currentTok().getSourceLoc();
         advance();

         if (currentTok().getKind() != tok::ident) {
            continue;
         }
      }
      else if (currentTok().is(tok::ident)) {
         ArgName = currentTok().getIdentifierInfo();
         advance();

         // Labeled argument
         if (currentTok().is(tok::ident) && lookahead().is(tok::colon)) {
            ArgLabel = ArgName.getIdentifierInfo();
            ArgName = currentTok().getIdentifierInfo();
            NameLoc = currentTok().getSourceLoc();

            advance();
            ColonLoc = consumeToken();
         }
         else if (currentTok().is(tok::colon)) {
            ColonLoc = consumeToken();

            if (!ImplicitUnderscores) {
               ArgLabel = ArgName.getIdentifierInfo();
            }
         }
         // Type
         else {
            ArgName = DeclarationName();
            lexer->backtrack();
         }
      }
      else if (currentTok().is(tok::underscore)) {
         consumeToken(tok::underscore);

         if (!currentTok().is(tok::ident)) {
            errorUnexpectedToken(currentTok(), tok::ident);
         }
         else if (lookahead().is(tok::colon)) {
            ArgName = currentTok().getIdentifierInfo();
            NameLoc = currentTok().getSourceLoc();
            advance();

            ColonLoc = consumeToken(tok::colon);
         }
         // Type
         else {
            ArgName = DeclarationName();
         }
      }

      ArgumentConvention Conv;
      SourceLocation OwnershipLoc;
      maybeParseConvention(Conv, OwnershipLoc);

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
         argType.getTypeExpr()->setEllipsisLoc({});
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

      // Fake up a name corresponding to the argument #
      if (!ArgName) {
         IdentifierInfo* II;
         if (i < 9) {
            char c = '0' + (char)i;
            II = &Context.getIdentifiers().get(llvm::StringRef(&c, 1));
         }
         else {
            II = &Context.getIdentifiers().get(std::to_string(i));
         }

         ArgName = Context.getDeclNameTable().getNormalIdentifier(*II);
      }

      auto argDecl = FuncArgDecl::Create(Context, OwnershipLoc, ColonLoc,
                                         ArgName, ArgLabel, Conv, argType,
                                         defaultVal, templateArgExpansion,
                                         /*cstyleVararg=*/false);

      Context.setAttributes(argDecl, Attrs);
      Attrs.clear();

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

      ++i;
   }
}

ParseResult Parser::parseFunctionDecl()
{
   auto DefLoc = currentTok().getSourceLoc();
   advance();

   bool IsOperator = false;

   FixKind Fix;
   if (currentTok().oneOf(Ident_infix, Ident_prefix, Ident_postfix)
       && validOperatorFollows()) {
      Fix = tokenToFix(currentTok());
      IsOperator = true;
      advance();
   }

   DeclarationName funcName;
   SourceType returnType;
   bool isCastOp;

   if (IsOperator) {
      funcName = parseOperatorName(Fix, isCastOp);
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
   auto args = parseFuncArgs(varargLoc, IsOperator);

   bool Throws = false;
   bool Async = false;
   bool Unsafe = false;

   while (1) {
      if (lookahead().is(Ident_throws)) {
         Throws = true;
         advance();
      }
      else if (lookahead().is(Ident_async)) {
         Async = true;
         advance();
      }
      else if (lookahead().is(Ident_unsafe)) {
         Unsafe = true;
         advance();
      }
      else {
         break;
      }
   }

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

   std::vector<ParsedConstraint> Constraints;
   if (lookahead().is(Ident_where)) {
      advance();
      parseDeclConstraints(Constraints);
   }

   auto funcDecl
       = FunctionDecl::Create(Context, CurDeclAttrs.Access, DefLoc, funcName,
                              args, returnType, nullptr, move(templateParams));

   CompoundStmt* body = nullptr;
   if (lookahead().is(tok::open_brace)) {
      DeclContextRAII declContextRAII(*this, funcDecl);
      EnterFunctionScope EF(*this);

      body = parseCompoundStmt().tryGetStatement<CompoundStmt>();
   }

   funcDecl->setAccessLoc(CurDeclAttrs.AccessLoc);
   funcDecl->setCstyleVararg(varargLoc.isValid());
   funcDecl->setThrows(Throws);
   funcDecl->setAsync(Async);
   funcDecl->setUnsafe(Unsafe);
   funcDecl->setBody(body);

   Context.setParsedConstraints(funcDecl, move(Constraints));
   return ActOnDecl(funcDecl);
}

ParseResult Parser::parseGlobalCtor()
{
   auto InitLoc = currentTok().getSourceLoc();

   SourceType RetTy(Context.getVoidType());
   DeclarationName Name(&Context.getIdentifiers().get("__global_init_fn"));

   auto Fn = FunctionDecl::Create(Context, AccessSpecifier::Private, InitLoc,
                                  Name, {}, RetTy, nullptr, {});

   CompoundStmt* body = nullptr;
   if (lookahead().is(tok::open_brace)) {
      DeclContextRAII declContextRAII(*this, Fn);
      EnterFunctionScope EF(*this);

      body = parseCompoundStmt().tryGetStatement<CompoundStmt>();
   }

   Fn->setGlobalCtor(true);
   Fn->setBody(body);

   return ActOnDecl(Fn);
}

ParseResult Parser::parseGlobalDtor()
{
   auto DeinitLoc = currentTok().getSourceLoc();

   SourceType RetTy(Context.getVoidType());
   DeclarationName Name(&Context.getIdentifiers().get("__global_deinit_fn"));

   auto Fn = FunctionDecl::Create(Context, AccessSpecifier::Private, DeinitLoc,
                                  Name, {}, RetTy, nullptr, {});

   CompoundStmt* body = nullptr;
   if (lookahead().is(tok::open_brace)) {
      DeclContextRAII declContextRAII(*this, Fn);
      EnterFunctionScope EF(*this);

      body = parseCompoundStmt().tryGetStatement<CompoundStmt>();
   }

   Fn->setGlobalDtor(true);
   Fn->setBody(body);

   return ActOnDecl(Fn);
}

ParseResult Parser::parseLambdaExpr()
{
   SmallVector<FuncArgDecl*, 4> args;
   SourceLocation LParenLoc = currentTok().getSourceLoc();

   return parseLambdaExpr(LParenLoc, args);
}

ParseResult Parser::parseLambdaExpr(SourceLocation LParenLoc,
                                    SmallVectorImpl<FuncArgDecl*>& args)
{
   if (currentTok().is(tok::open_paren)) {
      consumeToken(tok::open_paren);

      while (!currentTok().is(tok::close_paren)) {
         if (!currentTok().is(tok::ident)) {
            SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                        currentTok().toString(), true, "identifier");

            advance();
            continue;
         }

         IdentifierInfo* ArgName = nullptr;
         IdentifierInfo* ArgLabel = nullptr;
         SourceLocation ColonLoc = currentTok().getSourceLoc();

         ArgName = currentTok().getIdentifierInfo();
         advance();

         SourceLocation OwnershipLoc;
         ArgumentConvention Conv = ArgumentConvention::Default;

         SourceType argType;
         if (currentTok().is(tok::colon)) {
            ColonLoc = consumeToken();

            maybeParseConvention(Conv, OwnershipLoc);
            argType = parseType().tryGet();

            advance();
         }

         if (!argType)
            argType = SourceType(Context.getAutoType());

         args.emplace_back(FuncArgDecl::Create(Context, OwnershipLoc, ColonLoc,
                                               ArgName, ArgLabel, Conv, argType,
                                               nullptr, false, true));

         if (currentTok().is(tok::comma))
            advance();
      }
   }
   else if (args.empty()) {
      assert(currentTok().is(tok::ident) && "not begin of lambda expr!");

      SourceLocation Loc = currentTok().getSourceLoc();
      IdentifierInfo* Name = currentTok().getIdentifierInfo();

      args.emplace_back(FuncArgDecl::Create(
          Context, {}, Loc, Name, nullptr, ArgumentConvention::Default,
          SourceType(Context.getAutoType()), nullptr, false, true));
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

   if (!currentTok().is(tok::arrow_double)) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                  currentTok().toString(), true, "=>");
   }

   SourceLocation ArrowLoc = currentTok().getSourceLoc();

   advance();

   EnterFunctionScope EF(*this);

   auto BodyRes = parseNextStmt();
   if (!BodyRes || !BodyRes.holdsStatement())
      return ParseError();

   Statement* body = BodyRes.getStatement();
   if (auto E = dyn_cast_or_null<Expression>(body))
      body = ReturnStmt::Create(Context, body->getSourceLoc(), E);

   auto Expr
       = LambdaExpr::Create(Context, Parens, ArrowLoc, retType, args, body);

   auto* II = &Context.getIdentifiers().get("__anonymous_lambda");
   auto Fun = FunctionDecl::Create(Context, AccessSpecifier::Private,
                                   Expr->getSourceLoc(), II, Expr->getArgs(),
                                   Expr->getReturnType(), Expr->getBody(), {});

   Fun->setIsLambda(true);
   Fun->setExternC(true);
   Fun->setDeclared(true);
   Fun->setSynthesized(true);
   Fun->setSemanticallyChecked(true);

   Expr->setFunc(Fun);
   ActOnDecl(Fun);

   return maybeParseSubExpr(Expr);
}

ParseResult Parser::parseTrailingClosure(bool ParseSubExpr)
{
   ClosureRAII closureRAII(*this);
   assert(currentTok().is(tok::open_brace) && "shouldn't be called otherwise");

   auto& DNT = Context.getDeclNameTable();
   SourceLocation Loc = currentTok().getSourceLoc();

   EnterFunctionScope EF(*this);
   auto Block = parseCompoundStmt(false, true).tryGetStatement<CompoundStmt>();

   llvm::SmallVector<FuncArgDecl*, 4> Args;
   auto& Scope = UnnamedClosureArgumentStack.top();

   for (unsigned i = 0; i < Scope.NumArgs; ++i) {
      auto ArgName = DNT.getClosureArgumentName(i);
      auto ArgLoc = Scope.ArgLocs[i];
      if (!ArgLoc) {
         ArgLoc = Loc;
      }

      auto FuncArg = FuncArgDecl::Create(Context, ArgLoc, ArgLoc, ArgName,
                                         nullptr, ArgumentConvention::Default,
                                         SourceType(Context.getAutoType()),
                                         nullptr, false, true);

      Args.push_back(FuncArg);
   }

   auto Expr
       = LambdaExpr::Create(Context, SourceRange(Loc), {},
                            SourceType(Context.getAutoType()), Args, Block);

   auto* II = &Context.getIdentifiers().get("__anonymous_lambda");
   auto Fun = FunctionDecl::Create(Context, AccessSpecifier::Private,
                                   Expr->getSourceLoc(), II, Expr->getArgs(),
                                   Expr->getReturnType(), Expr->getBody(), {});

   Fun->setIsLambda(true);
   Fun->setExternC(true);
   Fun->setDeclared(true);
   Fun->setSynthesized(true);
   Fun->setSemanticallyChecked(true);

   Expr->setFunc(Fun);
   ActOnDecl(Fun);

   if (!ParseSubExpr)
      return Expr;

   return maybeParseSubExpr(Expr);
}

ASTVector<TemplateParamDecl*> Parser::tryParseTemplateParameters()
{
   Lexer::ModeRAII MR(*lexer, Lexer::Mode::ParsingTemplateParams);

   ASTVector<TemplateParamDecl*> params;
   if (!lookahead().is(tok::smaller))
      return params;

   advance();
   advance();

   while (!currentTok().is(tok::greater)) {
      if (currentTok().is(tok::eof)) {
         lexer->backtrack();
         lexer->backtrack();
         return params;
      }

      SourceLocation TypeNameOrValueLoc;
      SourceLocation EllipsisLoc;

      bool variadic = false;
      bool isTypeName = true;

      if (currentTok().getIdentifierInfo() == Ident_typename) {
         // default
         TypeNameOrValueLoc = consumeToken();
      }
      else if (currentTok().is(tok::kw_let)) {
         isTypeName = false;
         TypeNameOrValueLoc = consumeToken();
      }

      if (currentTok().is(tok::triple_period)) {
         EllipsisLoc = currentTok().getSourceLoc();
         variadic = true;
         advance();
      }

      if (currentTok().getKind() != tok::ident) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                     currentTok().toString(), true, "identifier");

         if (findTokOnLine(tok::comma, tok::close_square))
            continue;

         return params;
      }

      SourceLocation NameLoc = currentTok().getSourceLoc();
      auto Name = currentTok().getIdentifierInfo();

      bool Unbounded = !isTypeName || variadic;
      SourceType covariance;
      SourceType contravariance;

      if (lookahead().is(tok::colon)) {
         advance();
         advance();

         if (currentTok().is(tok::question)) {
            Unbounded = true;
         }
         else if (isTypeName) {
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

                  auto CovResult = parseType();
                  if (!CovResult) {
                     if (!findTokOnLine(tok::comma, tok::close_square)) {
                        skipUntilProbableEndOfStmt();
                        return params;
                     }
                  }
                  else {
                     covariance = CovResult.get();
                  }
               }
               else {
                  if (contravarSet) {
                     SP.diagnose(err_covar_convar_already_specified,
                                 currentTok().getSourceLoc(), 1);
                  }

                  contravarSet = true;

                  auto ConResult = parseType();
                  if (!ConResult) {
                     if (!findTokOnLine(tok::comma, tok::close_square)) {
                        skipUntilProbableEndOfStmt();
                        return params;
                     }
                  }
                  else {
                     contravariance = ConResult.get();
                  }
               }

               if (lookahead().is(tok::comma) || lookahead().is(tok::greater)) {
                  break;
               }

               if (lookahead().is(tok::eof)) {
                  lexer->backtrack();
                  return params;
               }

               advance();
            }
         }
         else {
            auto TypeRes = parseType();
            if (!TypeRes) {
               if (!findTokOnLine(tok::comma, tok::close_square)) {
                  skipUntilProbableEndOfStmt();
                  return params;
               }
            }

            covariance = TypeRes.tryGet();
         }
      }

      advance();

      Expression* defaultValue = nullptr;
      if (currentTok().is(tok::equals)) {
         advance();

         if (isTypeName) {
            defaultValue = parseType().tryGet().getTypeExpr();
         }
         else {
            auto E = parseExprSequence(DefaultFlags | F_StopAtGreater);
            defaultValue = StaticExpr::Create(Context, E.tryGetExpr());
         }

         advance();
      }

      if (!covariance)
         covariance = SourceType(Context.getAutoType());

      if (!contravariance)
         contravariance = SourceType(Context.getAutoType());

      unsigned idx = (unsigned)params.size();
      if (isTypeName) {
         params.push_back(TemplateParamDecl::Create(
                              Context, Name, covariance, contravariance,
                              defaultValue, idx, TypeNameOrValueLoc, NameLoc,
                              EllipsisLoc, Unbounded),
                          Context);
      }
      else {
         params.push_back(TemplateParamDecl::Create(Context, Name, covariance,
                                                    defaultValue, idx,
                                                    TypeNameOrValueLoc, NameLoc,
                                                    EllipsisLoc, Unbounded),
                          Context);
      }

      if (currentTok().is(tok::comma)) {
         advance();
      }
   }

   return params;
}

void Parser::parseIfConditions(SmallVectorImpl<IfCondition>& Conditions,
                               tok::TokenType StopAt)
{
   bool expectNewline = StopAt == tok::newline;
   while (!currentTok().is(StopAt)) {
      if (currentTok().oneOf(tok::kw_var, tok::kw_let)) {
         auto* VD = parseVarDecl(false, false, true).tryGetDecl<LocalVarDecl>();

         if (VD && !VD->getValue()) {
            SP.diagnose(err_if_let_must_have_value, VD->getSourceLoc());
         }

         Conditions.emplace_back(VD, nullptr);
      }
      else if (currentTok().is(tok::kw_case)) {
         advance();

         auto* Pat = parsePattern(DefaultFlags | F_StopAtEquals)
                         .tryGetExpr<PatternExpr>();

         expect(tok::equals);
         advance();

         auto* CaseVal = parseExprSequence(DefaultFlags & ~F_AllowBraceClosure)
                             .tryGetExpr();

         Conditions.emplace_back(Pat, CaseVal);
      }
      else {
         Conditions.emplace_back(
             parseExprSequence(DefaultFlags & ~F_AllowBraceClosure)
                 .tryGetExpr());
      }

      advance(!expectNewline);
      if (currentTok().is(tok::comma)) {
         advance(!expectNewline);
         continue;
      }

      break;
   }
}

ParseResult Parser::parseIfStmt(IdentifierInfo* Label)
{
   auto IfLoc = currentTok().getSourceLoc();
   advance();

   SmallVector<IfCondition, 2> Conditions;
   parseIfConditions(Conditions, tok::open_brace);

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

   return IfStmt::Create(Context, IfLoc, Conditions, ifBranch, elseBranch,
                         Label);
}

ParseResult Parser::parseStaticIf()
{
   if (inGlobalDeclContext())
      return parseStaticIfDecl();

   auto StaticLoc = currentTok().getSourceLoc();
   advance(); // static

   auto IfLoc = currentTok().getSourceLoc();
   advance(); // if

   auto cond = StaticExpr::Create(
       Context,
       parseExprSequence(DefaultFlags & ~F_AllowBraceClosure).tryGetExpr());

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

   if (!ifBranch) {
      return ParseError();
   }

   return StaticIfStmt::Create(Context, StaticLoc, IfLoc, cond, ifBranch,
                               elseBranch);
}

ParseResult Parser::parseStaticIfDecl()
{
   SourceLocation StaticLoc = currentTok().getSourceLoc();
   SourceLocation RBRaceLoc;

   advance(); // static
   advance(); // if

   auto cond = StaticExpr::Create(
       Context,
       parseExprSequence(DefaultFlags & ~F_AllowBraceClosure).tryGetExpr());

   advance();

   CompoundDecl* IfDecl
       = CompoundDecl::Create(Context, currentTok().getSourceLoc(), false);

   if (!currentTok().is(tok::open_brace)) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                  currentTok().toString(), true, "'{'");
   }
   else {
      DeclContextRAII declContextRAII(*this, IfDecl);
      advance();

      while (!currentTok().is(tok::close_brace)) {
         if (currentTok().is(tok::eof)) {
            SP.diagnose(err_unexpected_eof, currentTok().getSourceLoc(), false);
            lexer->backtrack();

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

   CompoundDecl* ElseDecl = nullptr;
   if (lookahead().is(tok::kw_else)) {
      advance();
      advance();

      if (!currentTok().is(tok::open_brace)) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                     currentTok().toString(), true, "'{'");
      }
      else {
         ElseDecl = CompoundDecl::Create(Context, currentTok().getSourceLoc(),
                                         false);

         DeclContextRAII declContextRAII(*this, ElseDecl);
         advance();

         while (!currentTok().is(tok::close_brace)) {
            if (currentTok().is(tok::eof)) {
               SP.diagnose(err_unexpected_eof, currentTok().getSourceLoc(),
                           false);

               lexer->backtrack();
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

   return ActOnDecl(Decl);
}

ParseResult Parser::parseStaticFor(bool variadic)
{
   if (inGlobalDeclContext()) {
      return parseStaticForDecl(variadic);
   }

   SourceLocation StaticLoc = currentTok().getSourceLoc();
   SourceLocation ForLoc = lookahead().getSourceLoc();

   if (variadic) {
      advance(); // for

      // Will already be diagnosed if not present.
      if (currentTok().is(tok::triple_period)) {
         advance(); // ...
      }
   }
   else {
      advance(); // static
      advance(); // for
   }

   IdentifierInfo* ident = nullptr;
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
      ident = currentTok().getIdentifierInfo();
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

   auto* rangeExpr
       = parseExprSequence(DefaultFlags & ~F_AllowBraceClosure).tryGetExpr();
   auto range = StaticExpr::Create(Context, rangeExpr);

   advance();

   Statement* body = parseNextStmt().tryGetStatement();
   if (auto Compound = dyn_cast_or_null<CompoundStmt>(body)) {
      Compound->setPreserveScope(true);
   }

   auto* Stmt
       = StaticForStmt::Create(Context, StaticLoc, ForLoc, ident, range, body);
   Stmt->setVariadic(variadic);

   return Stmt;
}

ParseResult Parser::parseStaticForDecl(bool variadic)
{
   SourceLocation StaticLoc = currentTok().getSourceLoc();
   SourceLocation RBRaceLoc;

   if (variadic) {
      advance(); // for

      // Will already be diagnosed if not present.
      if (currentTok().is(tok::triple_period)) {
         advance(); // ...
      }
   }
   else {
      advance(); // static
      advance(); // for
   }

   IdentifierInfo* ident = nullptr;
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

   auto range = StaticExpr::Create(
       Context,
       parseExprSequence(DefaultFlags & ~F_AllowBraceClosure).tryGetExpr());
   advance();

   CompoundDecl* BodyDecl
       = CompoundDecl::Create(Context, currentTok().getSourceLoc(), true);

   if (!currentTok().is(tok::open_brace)) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                  currentTok().toString(), true, "'{'");
   }
   else {
      DeclContextRAII declContextRAII(*this, BodyDecl);
      advance();

      while (!currentTok().is(tok::close_brace)) {
         if (currentTok().is(tok::eof)) {
            SP.diagnose(err_unexpected_eof, currentTok().getSourceLoc(), false);

            lexer->backtrack();
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

   Decl->setVariadic(variadic);
   return ActOnDecl(Decl);
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

   char* Alloc = (char*)Context.Allocate(msg.size());
   std::copy(msg.begin(), msg.end(), Alloc);

   auto Assert = StaticAssertDecl::Create(Context, Loc, Parens, expr,
                                          StringRef(Alloc, msg.size()));

   return ActOnDecl(Assert);
}

ParseResult Parser::parseStaticPrint()
{
   auto Loc = currentTok().getSourceLoc();
   if (!expect(tok::open_paren)) {
      return skipUntilProbableEndOfExpr();
   }

   SourceLocation LParenLoc = consumeToken();

   auto expr = parseExprSequence().tryGetExpr();
   expect(tok::close_paren);

   SourceRange Parens(LParenLoc, currentTok().getSourceLoc());
   auto PrintStmt = StaticPrintDecl::Create(Context, Loc, Parens, expr);

   return ActOnDecl(PrintStmt);
}

ParseResult Parser::parsePattern(int ExprFlags)
{
   if (currentTok().is(Ident_is)) {
      auto IsLoc = currentTok().getSourceLoc();
      advance();

      auto TypeRes = parseType();
      if (!TypeRes)
         return ParseError();

      SourceRange SR(IsLoc, lookahead().getSourceLoc());
      return IsPattern::Create(Context, SR, TypeRes.get());
   }

   auto SAR = support::saveAndRestore(this->AllowPattern, true);
   auto ExprRes = parseExprSequence(ExprFlags);
   if (!ExprRes) {
      return ParseError();
   }

   if (isa<PatternExpr>(ExprRes.getExpr())) {
      return ExprRes;
   }

   return ExpressionPattern::Create(Context, lookahead().getSourceLoc(),
                                    ExprRes.getExpr());
}

void Parser::parseCaseStmts(llvm::SmallVectorImpl<CaseStmt*>& Cases)
{
   ContinueStmt* Fallthrough = nullptr;

   while (!currentTok().is(tok::close_brace)) {
      auto CaseLoc = currentTok().getSourceLoc();
      PatternExpr* patternExpr = nullptr;

      if (currentTok().is(tok::kw_case)) {
         advance();
         patternExpr = parsePattern(DefaultFlags | F_StopAtColon)
                           .tryGetExpr<PatternExpr>();
      }
      else if (!currentTok().is(Ident_default)) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                     currentTok().toString(), true, "'case' or 'default'");

         skipUntilEven(tok::open_brace);
         return;
      }

      expect(tok::colon);

      if (lookahead().oneOf(tok::kw_case, Ident_default)) {
         if (!Fallthrough) {
            Fallthrough = ContinueStmt::Create(
                Context, currentTok().getSourceLoc(), nullptr);
         }

         Cases.push_back(
             CaseStmt::Create(Context, CaseLoc, patternExpr, Fallthrough));

         advance();
         continue;
      }

      SmallVector<Statement*, 2> Stmts;
      while (
          !lookahead().oneOf(tok::kw_case, tok::close_brace, Ident_default)) {
         advance();

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
            skipUntilEven(tok::open_brace);
            return;
         }
      }

      if (Stmts.empty()) {
         SP.diagnose(err_last_match_case_empty, currentTok().getSourceLoc());
      }

      auto Compound = CompoundStmt::Create(Context, Stmts, false,
                                           Stmts.front()->getSourceLoc(),
                                           currentTok().getSourceLoc());

      Cases.push_back(
          CaseStmt::Create(Context, CaseLoc, patternExpr, Compound));

      advance();
   }
}

ParseResult Parser::parseMatchStmt(IdentifierInfo* Label)
{
   auto MatchLoc = currentTok().getSourceLoc();
   advance();

   Expression* matchVal
       = parseExprSequence(DefaultFlags & ~F_AllowBraceClosure).tryGetExpr();

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
   return MatchStmt::Create(Context, MatchLoc, Braces, matchVal, Cases, Label);
}

ParseResult Parser::parseWhileStmt(IdentifierInfo* Label, bool conditionBefore)
{
   auto WhileLoc = currentTok().getSourceLoc();
   advance();

   SmallVector<IfCondition, 2> Conditions;
   if (conditionBefore) {
      parseIfConditions(Conditions, tok::open_brace);
   }

   auto body = parseNextStmt().tryGetStatement();
   if (!conditionBefore && lookahead().is(tok::kw_while)) {
      advance();
      advance();

      parseIfConditions(Conditions, tok::newline);
   }

   return WhileStmt::Create(Context, WhileLoc, Conditions, body, Label,
                            !conditionBefore);
}

ParseResult Parser::parseForStmt(IdentifierInfo* Label)
{
   if (lookahead().is(tok::triple_period)) {
      if (Label != nullptr) {
         SP.diagnose(err_generic_error, "for... statement cannot be labeled",
                     currentTok().getSourceLoc());
      }

      return parseStaticFor(true);
   }

   auto ForLoc = currentTok().getSourceLoc();
   advance();

   SourceLocation UnderscoreLoc;
   Statement* init = nullptr;
   class Decl* initDecl = nullptr;

   if (!currentTok().is(tok::semicolon)) {
      // for i in 0..5 {}
      if (currentTok().is(tok::ident) && lookahead().is(Ident_in)) {
         auto Loc = currentTok().getSourceLoc();
         auto* II = currentTok().getIdentifierInfo();
         initDecl = LocalVarDecl::Create(Context, AccessSpecifier::Public, Loc,
                                         Loc, true, II, SourceType(), nullptr);

         initDecl->setLexicalContext(&SP.getDeclContext());
      }
      // for _ in 0..5 {}
      else if (currentTok().is(tok::underscore) && lookahead().is(Ident_in)) {
         initDecl = nullptr;
         UnderscoreLoc = currentTok().getSourceLoc();
      }
      // for (key, value) in [:] {}
      else if (currentTok().is(tok::open_paren)) {
         initDecl = parseDestructuringDecl(true, true).tryGetDecl();
      }
      else {
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
      }

      if (initDecl) {
         initDecl->setSynthesized(true);
      }

      advance();
   }

   // range based for loop
   if (currentTok().getIdentifierInfo() == Ident_in) {
      advance();

      Expression* range
          = parseExprSequence(DefaultFlags & ~F_AllowBraceClosure).tryGetExpr();

      advance();
      if (currentTok().is(tok::close_paren)) {
         advance();
      }

      auto body = parseNextStmt().tryGetStatement();
      return ForInStmt::Create(Context, ForLoc, initDecl, range, body, Label);
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
      auto incResult = parseExprSequence(DefaultFlags & ~F_AllowBraceClosure);
      if (incResult.holdsExpr()) {
         inc = incResult.getExpr();
      }

      advance();
   }

   return ForStmt::Create(Context, ForLoc, init, term, inc,
                          parseNextStmt().tryGetStatement(), Label);
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
      else if (str == "CXX" || str == "CPP" || str == "C++") {
         Lang = ExternAttr::CXX;
         HasLang = true;
      }
      else {
         SP.diagnose(err_bad_extern_kind, currentTok().getSourceLoc(), str);
      }

      advance();
   }

   if (currentTok().is(tok::open_brace)) {
      advance();

      auto LBraceLoc = currentTok().getSourceLoc();
      auto compoundDecl = CompoundDecl::Create(Context, LBraceLoc, true);

      {
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
                        CD->addAttribute(new (Context) ExternAttr(Lang));

                  if (nextDecl->hasDefinition())
                     SP.diagnose(nextDecl, err_declared_with_definition,
                                 nextDecl->getSourceLoc());
               }
            }

            advance();
         }
      }

      compoundDecl->setRBraceLoc(currentTok().getSourceLoc());

      return ActOnDecl(compoundDecl);
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
                  CD->addAttribute(new (Context) ExternAttr(Lang));

            if (nextDecl->hasDefinition())
               SP.diagnose(nextDecl, err_declared_with_definition,
                           nextDecl->getSourceLoc());

            return nextDecl;
         }
      }

      return ParseStmtError();
   }
}

ParseResult Parser::parseCompoundDecl(bool TopLevel, bool Transparent)
{
   CompoundDecl* CD
       = CompoundDecl::Create(Context, consumeToken(), Transparent);

   {
      DeclContextRAII declContextRAII(*this, CD);
      while (!currentTok().is(tok::close_brace)) {
         ParseResult Next = parseNextDecl();
         if (!Next) {
            if (!skipUntilNextDecl())
               break;
         }

         if (lookahead().is(tok::eof))
            break;

         advance();
      }
   }

   CD->setRBraceLoc(currentTok().getSourceLoc());
   ActOnDecl(CD);

   if (CurDeclAttrs.AccessLoc) {
      for (auto& D : CD->getDecls()) {
         if (auto ND = dyn_cast<NamedDecl>(D)) {
            if (ND->getAccess() == AccessSpecifier::Default) {
               ND->setAccessLoc(CurDeclAttrs.AccessLoc);
               ND->setAccess(CurDeclAttrs.Access);
            }
         }
      }
   }

   return CD;
}

ParseResult Parser::parseThrowStmt()
{
   auto ThrowLoc = consumeToken(tok::kw_throw);
   return new (Context) ThrowStmt(ThrowLoc, parseExprSequence().tryGetExpr());
}

ParseResult Parser::parseReturnStmt()
{
   auto RetLoc = currentTok().getSourceLoc();
   Token next = lookahead(false);
   ReturnStmt* ret = nullptr;

   switch (next.getKind()) {
   default:
      advance();
      ret = ReturnStmt::Create(Context, RetLoc,
                               parseExprSequence().tryGetExpr());
      break;
   case tok::comma:
   case tok::semicolon:
   case tok::close_paren:
   case tok::newline:
   case tok::eof:
   case tok::sentinel:
   case tok::close_brace:
   case tok::close_square:
      ret = ReturnStmt::Create(Context, RetLoc);
      break;
   }

   return ret;
}

ParseResult Parser::parseKeyword()
{
   auto BeginLoc = currentTok().getSourceLoc();
   auto kind = currentTok().getKind();

   switch (kind) {
   case tok::kw_self:
   case tok::kw_super:
   case tok::kw_true:
   case tok::kw_false:
   case tok::kw_none:
   case tok::kw_try:
      return parseExprSequence();
   case tok::kw_if:
      return parseIfStmt();
   case tok::kw_while:
   case tok::kw_loop:
      return parseWhileStmt(nullptr, kind == tok::kw_while);
   case tok::kw_match:
      return parseMatchStmt();
   case tok::kw_for:
      return parseForStmt();
   case tok::kw_continue:
   case tok::kw_break: {
      IdentifierInfo* Label = nullptr;
      if (lookahead(false).is(tok::ident)) {
         advance();
         Label = currentTok().getIdentifierInfo();
      }

      Statement* stmt;
      if (kind == tok::kw_continue) {
         stmt = ContinueStmt::Create(Context, BeginLoc, Label);
      }
      else {
         stmt = BreakStmt::Create(Context, BeginLoc, Label);
      }

      return stmt;
   }
   case tok::kw_return:
      return parseReturnStmt();
   case tok::kw_throw:
      return parseThrowStmt();
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
      return new (Context) DebugStmt(BeginLoc, false);
   case tok::kw___unreachable:
      return new (Context) DebugStmt(BeginLoc, true);
   case tok::kw_static:
      if (lookahead().is(tok::kw_if)) {
         return parseStaticIf();
      }
      if (lookahead().is(tok::kw_for)) {
         return parseStaticFor();
      }

      break;
   case tok::kw_var:
   case tok::kw_let:
   case tok::kw_def:
   case tok::kw_typedef:
   case tok::kw_alias:
   case tok::kw_declare:
   case tok::kw_namespace:
   case tok::kw_import:
   case tok::kw_static_assert:
   case tok::kw_static_print:
   case tok::kw_struct:
   case tok::kw_enum:
   case tok::kw_class:
   case tok::kw_protocol:
   case tok::kw_union:
   case tok::kw_extend:
   case tok::kw_public:
   case tok::kw_fileprivate:
   case tok::kw_private:
   case tok::kw_internal:
   case tok::kw_abstract:
      return parseNextDecl();
   default:
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                  currentTok().toString(), false);
      break;
   }

   return ParseError();
}

ParseResult Parser::parseTopLevelDecl()
{
   if (CurDeclAttrs.Default) {
      SP.diagnose(err_generic_error,
                  "top-level declarations cannot be marked 'default'",
                  CurDeclAttrs.DefaultLoc);

      CurDeclAttrs.Default = false;
   }

   if (currentTok().is(tok::at))
      return parseAttributedDecl();

   if (currentTok().is(Ident_deinit))
      return parseGlobalDtor();

   if (currentTok().is(Ident_unittest))
      return parseUnittestDecl();

   auto Tok = currentTok();
   auto kind = Tok.getKind();

   switch (kind) {
   case tok::close_paren:
   case tok::close_square:
   case tok::close_brace: {
      unsigned idx
          = kind == tok::close_paren ? 0 : kind == tok::close_brace ? 1 : 2;

      SP.diagnose(err_extraneous_paren, currentTok().getSourceLoc(), idx);
      if (skipUntilNextDecl()) {
         advance();
         return parseNextDecl();
      }

      return ParseError();
   }
   case tok::open_brace:
      return parseCompoundDecl(true, CurDeclAttrs.AccessLoc.isValid());
   case tok::ident: {
      if (Tok.is(Ident_precedenceGroup)) {
         return parsePrecedenceGroup();
      }

      if (Tok.is(Ident_macro)) {
         return parseMacro();
      }

      if (Tok.is(Ident_using)) {
         return parseUsingDecl();
      }

      if (currentTok().oneOf(Ident_infix, Ident_prefix, Ident_postfix)
          && validOperatorFollows()) {
         return parseOperatorDecl();
      }

      // This might be a macro expansion with a nested name.
      if (lookahead().is(tok::period)) {
         SourceRange SR(Tok.getSourceLoc(), Tok.getEndLoc());

         // Create the first expression
         auto* Ident
             = new (Context) IdentifierRefExpr(SR, Tok.getIdentifierInfo());

         advance();
         advance();

         while (true) {
            Tok = currentTok();
            SR = SourceRange(Tok.getSourceLoc(), Tok.getEndLoc());

            if (Tok.is(tok::ident)) {
               Ident = new (Context)
                   IdentifierRefExpr(SR, Ident, Tok.getIdentifierInfo());

               if (lookahead().is(tok::period)) {
                  advance();
                  advance();
               }
               else {
                  break;
               }
            }
            else if (currentTok().is(tok::macro_name)) {
               return parseMacroExpansionDecl(Ident);
            }
            else {
               errorUnexpectedToken(currentTok(), tok::period);
               return ParseError();
            }
         }
      }

      goto case_bad_token;
   }
   case tok::macro_statement:
   case tok::macro_expression: {
      enum DiagKind {
         Expression,
         Statement,
         Type,
         Decl,
      };

      SP.diagnose(
          err_bad_macro_variable_kind, currentTok().getSourceLoc(),
          currentTok().is(tok::macro_statement) ? Statement : Expression, Decl);

      return ParseError();
   }
   case tok::macro_declaration:
      return currentTok().getDecl();
   case tok::macro_name:
      return parseMacroExpansionDecl();
   case tok::kw_var:
   case tok::kw_let:
      return parseVarDecl();
   case tok::kw_def:
      return parseFunctionDecl();
   case tok::kw_init:
      return parseGlobalCtor();
   case tok::kw_typedef:
      return parseTypedef();
   case tok::kw_alias:
      return parseAlias();
   case tok::kw_declare:
      return parseDeclareStmt();
   case tok::kw_namespace:
      return parseNamespaceDecl();
   case tok::kw_import:
      return parseImportDecl();
   case tok::kw_mixin: {
      auto loc = currentTok().getSourceLoc();
      if (!expect(tok::open_paren)) {
         return skipUntilProbableEndOfExpr();
      }

      advance();

      auto E = parseExprSequence().tryGetExpr();
      expect(tok::close_paren);

      SourceRange Parens(loc, currentTok().getSourceLoc());

      auto MD = MixinDecl::Create(Context, loc, Parens, E);
      return ActOnDecl(MD);
   }
   case tok::kw_for: {
      if (!lookahead().is(tok::triple_period)) {
         SP.diagnose(err_generic_error,
                     "only 'for...' declarations can appear at the top level",
                     currentTok().getSourceLoc());
      }

      return parseStaticForDecl(true);
   }
   case tok::kw_static:
      if (lookahead().is(tok::kw_if)) {
         return parseStaticIf();
      }
      if (lookahead().is(tok::kw_for)) {
         return parseStaticFor();
      }

      goto case_bad_token;
   case tok::kw_static_assert:
      return parseStaticAssert();
   case tok::kw_static_print:
      return parseStaticPrint();
   case tok::kw___debug:
      return DebugDecl::Create(Context, currentTok().getSourceLoc());
   case tok::kw_struct:
   case tok::kw_enum:
   case tok::kw_class:
   case tok::kw_protocol:
   case tok::kw_union:
      return parseAnyRecord(kind);
   case tok::kw_extend:
      return parseExtension();
   case tok::kw_public:
   case tok::kw_fileprivate:
   case tok::kw_private:
   case tok::kw_internal:
   case tok::kw_abstract:
      llvm_unreachable("should not happen here!");
   case tok::kw_module:
      SP.diagnose(err_module_must_be_first, currentTok().getSourceLoc());
      (void)parseModuleDecl();

      return ParseError();
   default:
   case_bad_token:
      SP.diagnose(err_expecting_decl, currentTok().getSourceLoc(),
                  currentTok().toString(), /*top level*/ true);

      if (skipUntilNextDecl()) {
         advance();
         return parseNextDecl();
      }

      return ParseError();
   }
}

CompoundDecl* Parser::parseDecls(SourceLocation Loc, bool RecordLevel)
{
   CompoundDecl* CD = CompoundDecl::Create(Context, Loc, true);
   skipWhitespace();

   {
      DeclContextRAII declContextRAII(*this, CD);
      while (!lookahead().is(tok::eof)) {
         (void)parseNextDecl();
         advance();
      }
   }

   ActOnDecl(CD);
   return CD;
}

void Parser::parseDecls(bool RecordLevel)
{
   RecordScopeRAII RSR(*this, RecordLevel);

   skipWhitespace();
   while (!currentTok().is(tok::eof)) {
      (void)parseNextDecl();
      advance();
   }
}

Statement* Parser::parseStmts()
{
   skipWhitespace();

   llvm::SmallVector<Statement*, 4> Stmts;
   while (!currentTok().is(tok::eof)) {
      auto NextStmt = parseNextStmt();
      if (NextStmt) {
         if (NextStmt.holdsStatement()) {
            Stmts.push_back(NextStmt.getStatement());
         }
         else {
            auto DS = DeclStmt::Create(Context, NextStmt.getDecl());
            Stmts.push_back(DS);
         }
      }

      advance();
   }

   if (Stmts.empty())
      return nullptr;

   if (Stmts.size() == 1)
      return Stmts.front();

   return CompoundStmt::Create(Context, Stmts, true, SourceLocation(),
                               SourceLocation());
}

void Parser::parsePatternCommon(SmallVectorImpl<IfCondition>& Args,
                                SmallVectorImpl<IdentifierInfo*>& Labels,
                                bool& OnlyExprs, tok::TokenType EndTok)
{
   while (!currentTok().is(EndTok)) {
      IdentifierInfo* label = nullptr;

      if (currentTok().getKind() == tok::ident && lookahead().is(tok::colon)) {
         label = currentTok().getIdentifierInfo();
         advance();
         advance();
      }
      else if (currentTok().getKind() == tok::underscore
               && lookahead().is(tok::colon)) {
         label = SP.getIdentifier("_");
         advance();
         advance();
      }

      if (currentTok().oneOf(tok::kw_var, tok::kw_let)) {
         auto* VD = parseVarDecl(false, false, true).tryGetDecl<LocalVarDecl>();

         OnlyExprs = false;
         Args.emplace_back(VD, nullptr);
      }
      else if (currentTok().is(tok::underscore)) {
         OnlyExprs = false;
         Args.emplace_back((LocalVarDecl*)nullptr);
      }
      else if (currentTok().is(Ident_is)) {
         auto Loc = currentTok().getSourceLoc();
         advance();

         auto TypeRes = parseType();
         if (!TypeRes)
            break;

         SourceRange SR(Loc, lookahead().getSourceLoc());
         Args.emplace_back(IsPattern::Create(Context, SR, TypeRes.get()));
      }
      else {
         auto* E = parseExprSequence(DefaultFlags & ~F_AllowBraceClosure)
                       .tryGetExpr();
         if (E && isa<PatternExpr>(E)) {
            Args.emplace_back(cast<PatternExpr>(E));
            OnlyExprs = false;
         }
         else {
            Args.emplace_back(E);
         }
      }

      Labels.emplace_back(label);
      advance();

      if (currentTok().is(tok::comma)) {
         advance();
      }
      else if (currentTok().is(EndTok)) {
         break;
      }
      else {
         errorUnexpectedToken(currentTok(), tok::close_paren);
         skipUntilEven(tok::open_paren);

         break;
      }
   }

   while (!Labels.empty() && !Labels.back()) {
      Labels.pop_back();
   }
}

ParseResult Parser::parseCallPattern(bool skipName, Expression* ParentExpr,
                                     bool pointerAccess, DeclarationName Name)
{
   auto IdentLoc = currentTok().getSourceLoc();
   bool IsInit = currentTok().is(tok::kw_init);
   bool IsDeinit = currentTok().is(Ident_deinit);

   if (!IsInit && !Name) {
      Name = currentTok().getIdentifierInfo();
   }

   advance();

   auto LParenLoc = currentTok().getSourceLoc();

   bool OnlyExprs = true;
   SmallVector<IfCondition, 4> Args;
   SmallVector<IdentifierInfo*, 4> Labels;

   consumeToken(tok::open_paren);
   parsePatternCommon(Args, Labels, OnlyExprs, tok::close_paren);

   SourceRange Parens(LParenLoc, currentTok().getSourceLoc());

   // trailing closure
   if (AllowBraceClosure() && lookahead(false).is(tok::open_brace)) {
      advance();

      auto Result = parseTrailingClosure(false);
      if (Result) {
         auto Closure = Result.getExpr();
         Args.emplace_back(Closure);
      }
   }

   if (OnlyExprs) {
      if (IsInit) {
         Name = Context.getDeclNameTable().getConstructorName(CanType(), false);
      }
      else if (IsDeinit) {
         Name = Context.getDeclNameTable().getDestructorName(CanType());
      }

      if (!skipName) {
         ParentExpr = new (Context)
             IdentifierRefExpr(IdentLoc, ParentExpr, Name, pointerAccess);
      }

      SmallVector<Expression*, 2> ArgVec;
      ArgVec.reserve(Args.size());

      for (auto& Arg : Args) {
         ArgVec.push_back(Arg.ExprData.Expr);
      }

      return AnonymousCallExpr::Create(Context, IdentLoc, ParentExpr, ArgVec,
                                       Labels);
   }

   if (!skipName) {
      //      ParentExpr = new(Context) IdentifierRefExpr(IdentLoc, ParentExpr,
      //      Name,
      //                                                  pointerAccess);
   }

   return CasePattern::Create(Context, SourceRange(IdentLoc, Parens.getEnd()),
                              CasePattern::K_EnumOrStruct, ParentExpr,
                              Name.getIdentifierInfo(), Args);
}

ParseResult Parser::parseTuplePattern()
{
   auto LParenLoc = currentTok().getSourceLoc();

   bool OnlyExprs = true;
   SmallVector<IfCondition, 4> Args;
   SmallVector<IdentifierInfo*, 4> Labels;

   consumeToken(tok::open_paren);
   parsePatternCommon(Args, Labels, OnlyExprs, tok::close_paren);

   // trailing closure
   if (AllowBraceClosure() && lookahead(false).is(tok::open_brace)) {
      advance();

      auto Result = parseTrailingClosure(false);
      if (Result) {
         auto Closure = Result.getExpr();
         Args.emplace_back(Closure);
      }
   }

   SourceRange ParenRange(LParenLoc, currentTok().getSourceLoc());
   if (OnlyExprs) {
      SmallVector<Expression*, 4> ArgVec;
      ArgVec.reserve(Args.size());

      for (auto& Arg : Args) {
         ArgVec.push_back(Arg.ExprData.Expr);
      }

      return TupleLiteral::Create(Context, ParenRange, ArgVec);
   }

   return CasePattern::Create(Context, ParenRange, CasePattern::K_Tuple,
                              nullptr, nullptr, Args);
}

ParseResult Parser::parseArrayPattern(SourceLocation LSquareLoc,
                                      ArrayRef<Expression*> ExprsSoFar)
{
   bool OnlyExprs = true;
   SmallVector<IfCondition, 4> Args;
   SmallVector<IdentifierInfo*, 4> Labels;

   for (auto* E : ExprsSoFar) {
      Args.emplace_back(E);
      Labels.push_back(nullptr);
   }

   parsePatternCommon(Args, Labels, OnlyExprs, tok::close_square);

   SourceRange ParenRange(LSquareLoc, currentTok().getSourceLoc());
   if (OnlyExprs) {
      SmallVector<Expression*, 4> ArgVec;
      ArgVec.reserve(Args.size());

      for (auto& Arg : Args) {
         ArgVec.push_back(Arg.ExprData.Expr);
      }

      return ArrayLiteral::Create(Context, ParenRange, ArgVec);
   }

   return CasePattern::Create(Context, ParenRange, CasePattern::K_Array,
                              nullptr, nullptr, Args);
}

ParseResult Parser::parseFunctionCall(bool skipName, Expression* ParentExpr,
                                      bool pointerAccess, DeclarationName Name)
{
   auto IdentLoc = currentTok().getSourceLoc();
   bool IsInit = currentTok().is(tok::kw_init);
   bool IsDeinit = currentTok().is(Ident_deinit);

   if (!IsInit && !Name) {
      Name = currentTok().getIdentifierInfo();
   }

   advance();

   auto LParenLoc = currentTok().getSourceLoc();
   auto args = parseCallArguments();

   SourceRange Parens(LParenLoc, currentTok().getSourceLoc());

   // trailing closure
   if (AllowBraceClosure() && lookahead(false).is(tok::open_brace)) {
      advance();

      auto Result = parseTrailingClosure(false);
      if (!Result)
         return ParentExpr;

      auto Closure = Result.getExpr();
      args.args.push_back(Closure, Context);
   }

   if (IsInit) {
      Name = Context.getDeclNameTable().getConstructorName(CanType(), false);
   }
   else if (IsDeinit) {
      Name = Context.getDeclNameTable().getDestructorName(CanType());
   }

   if (!skipName) {
      ParentExpr = new (Context)
          IdentifierRefExpr(IdentLoc, ParentExpr, Name, pointerAccess);
   }

   return AnonymousCallExpr::Create(Context, Parens, ParentExpr, args.args,
                                    args.labels);
}

Parser::ArgumentList Parser::parseCallArguments()
{
   ArgumentList args{};
   consumeToken(tok::open_paren);

   while (!currentTok().is(tok::close_paren)) {
      IdentifierInfo* label = nullptr;

      if (currentTok().getKind() == tok::ident && lookahead().is(tok::colon)) {
         label = currentTok().getIdentifierInfo();
         advance();
         advance();
      }
      else if (currentTok().getKind() == tok::underscore
               && lookahead().is(tok::colon)) {
         label = SP.getIdentifier("_");
         advance();
         advance();
      }

      auto argVal = parseExprSequence();
      if (!argVal) {
         expect(tok::comma, tok::close_paren);
         continue;
      }

      args.labels.emplace_back(label);
      args.args.push_back(argVal.getExpr(), Context);

      advance();

      if (currentTok().is(tok::comma)) {
         advance();
      }
      else if (currentTok().is(tok::close_paren)) {
         break;
      }
      else {
         errorUnexpectedToken(currentTok(), tok::close_paren);
         skipUntilEven(tok::open_paren);

         break;
      }
   }

   while (!args.labels.empty() && !args.labels.back()) {
      args.labels.pop_back();
   }

   return args;
}

ParseResult Parser::parseCompoundStmt(bool preserveTopLevel, bool noOpenBrace)
{
   if (!noOpenBrace) {
      advance();

      if (!(currentTok().is(tok::open_brace))) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                     currentTok().toString(), true, "'{'");

         if (!findTokOnLine(tok::open_brace))
            return skipUntilProbableEndOfStmt();
      }
   }

   RecordScopeRAII RSR(*this, false);

   auto LBraceLoc = currentTok().getSourceLoc();
   advance();

   SmallVector<Statement*, 8> Stmts;
   while (!currentTok().is(tok::close_brace)) {
      while (currentTok().oneOf(tok::semicolon, tok::newline)) {
         advance();
      }

      if (currentTok().getKind() == tok::eof
          || currentTok().is(tok::close_brace)) {
         break;
      }

      auto nextStmt = parseNextStmt(false);
      if (nextStmt.holdsStatement()) {
         Stmts.push_back(nextStmt.getStatement());
      }
      else if (nextStmt.holdsExpr()) {
         Stmts.push_back(nextStmt.getExpr());
      }
      else if (nextStmt.holdsDecl()) {
         Stmts.push_back(DeclStmt::Create(Context, nextStmt.getDecl()));
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

ParseResult Parser::parseDoStmt(IdentifierInfo* Label)
{
   assert(currentTok().is(Ident_do) && "not a do stmt!");
   auto DoLoc = currentTok().getSourceLoc();

   advance();
   if (!(currentTok().is(tok::open_brace))) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                  currentTok().toString(), true, "'{'");

      if (!findTokOnLine(tok::open_brace))
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

      auto nextStmt = parseNextStmt(false);
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

   auto CS = CompoundStmt::Create(Context, Stmts, false, LBraceLoc,
                                  currentTok().getSourceLoc());

   if (!lookahead().is(tok::kw_catch)) {
      return CS;
   }

   SourceLocation CatchAllLoc;
   SmallVector<CatchBlock, 2> CatchBlocks;

   while (lookahead().is(tok::kw_catch)) {
      advance();
      advance();

      if (currentTok().is(tok::underscore)) {
         CatchAllLoc = currentTok().getSourceLoc();
         advance();

         auto body = parseNextStmt().tryGetStatement();
         CatchBlocks.emplace_back(nullptr, body, nullptr);

         continue;
      }

      SourceLocation VarOrLetLoc = currentTok().getSourceLoc();
      AccessSpecifier access = AccessSpecifier::Default;
      bool isLet = true;

      if (!currentTok().is(tok::ident)) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                     currentTok().toString(), true, "identifier");

         return skipUntilProbableEndOfStmt();
      }

      auto Name = currentTok().getIdentifierInfo();

      SourceType type;
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
      else {
         SP.diagnose(err_generic_error, currentTok().getSourceLoc(),
                     "catch must have a defined type");
      }

      auto L = LocalVarDecl::Create(Context, access, VarOrLetLoc, ColonLoc,
                                    isLet, Name, type, nullptr);

      L->setLexicalContext(&SP.getDeclContext());

      advance();

      Expression* Cond = nullptr;
      if (currentTok().is(Ident_where)) {
         advance();
         Cond = parseExprSequence(DefaultFlags & ~F_AllowBraceClosure)
                    .tryGetExpr();

         advance();
      }

      auto body = parseNextStmt().tryGetStatement();
      CatchBlocks.emplace_back(CatchBlock(L, body, Cond));
   }

   if (CatchAllLoc.isValid() && CatchBlocks.size() != 1) {
      SP.diagnose(err_catch_all_must_be_alone, CatchAllLoc);
   }

   return new (Context) DoStmt(SourceRange(DoLoc), CS, CatchBlocks, Label);
}

Parser::DeclAttrs Parser::pushDeclAttrs()
{
   DeclAttrs Prev = CurDeclAttrs;
   CurDeclAttrs = DeclAttrs();

   bool done = false;
   while (true) {
      switch (currentTok().getKind()) {
      case tok::kw_public:
      case tok::kw_fileprivate:
      case tok::kw_private:
      case tok::kw_internal:
      case tok::kw_protected:
         CurDeclAttrs.Access = tokenToAccessSpec(currentTok().getKind());
         CurDeclAttrs.AccessLoc = currentTok().getSourceLoc();
         break;
      case tok::kw_abstract:
         CurDeclAttrs.Abstract = true;
         break;
      case tok::kw_static:
         if (lookahead().oneOf(tok::kw_if, tok::kw_for)) {
            done = true;
            break;
         }

         CurDeclAttrs.StaticLoc = currentTok().getSourceLoc();
         break;
      case tok::ident: {
         auto* II = currentTok().getIdentifierInfo();
         if (II == Ident_default) {
            CurDeclAttrs.Default = true;
            CurDeclAttrs.DefaultLoc = currentTok().getSourceLoc();
         }
         else {
            done = true;
         }

         break;
      }
      default:
         done = true;
         break;
      }

      if (done)
         break;

      advance();
   }

   assert((int)CurDeclAttrs.Access <= 5 && "bad access specifier");
   return Prev;
}

ParseResult Parser::parseNextDecl()
{
   DeclAttrs Prev = pushDeclAttrs();

   ParseResult Result;
   if (isAtRecordLevel()) {
      Result = parseRecordLevelDecl();
   }
   else {
      Result = parseTopLevelDecl();
   }

   assert((int)Prev.Access <= 5 && "bad access specifier");
   popDeclAttrs(Prev);

   return Result;
}

ParseResult Parser::parseNextStmt(bool AllowBracedBlock)
{
   if (currentTok().is(tok::at)) {
      auto K = getNextArtefactKind();
      if (K == K_Decl) {
         return parseAttributedDecl();
      }

      return parseAttributedStmt();
   }

   if (currentTok().is(Ident_do) && lookahead(false).is(tok::open_brace))
      return parseDoStmt();

   if (currentTok().is(Ident_unsafe)) {
      advance();

      auto Stmt = parseNextStmt(true);
      if (Stmt)
         Stmt.getStatement()->setUnsafe(true);

      return Stmt;
   }

   if (currentTok().is(Ident_macro))
      return parseMacro();

   ParseResult stmt;

   auto kind = currentTok().getKind();
   switch (kind) {
#define CDOT_KEYWORD_TOKEN(Name, Str) case tok::Name:
#include "cdotc/Lex/Tokens.def"
      stmt = parseKeyword();
      break;
   case tok::close_paren:
   case tok::close_square:
   case tok::close_brace: {
      unsigned idx
          = kind == tok::close_paren ? 0 : kind == tok::close_brace ? 1 : 2;

      SP.diagnose(err_extraneous_paren, currentTok().getSourceLoc(), idx);
      return skipUntilProbableEndOfStmt();
   }
   case tok::open_brace:
      if (AllowBracedBlock) {
         lexer->backtrack();
         stmt = parseCompoundStmt();
      }
      else {
         stmt = parseExprSequence();
      }

      break;
   case tok::macro_name:
      stmt = parseMacroExpansionStmt();
      break;
   case tok::macro_expression:
      stmt = currentTok().getExpr();
      break;
   case tok::macro_statement:
      stmt = currentTok().getStmt();
      break;
   case tok::macro_declaration:
      stmt = DeclStmt::Create(Context, currentTok().getDecl());
      break;
   case tok::semicolon:
      stmt = NullStmt::Create(Context, currentTok().getSourceLoc());
      break;
   case tok::underscore: {
      if (lookahead().is(tok::equals)) {
         auto UnderscoreLoc = currentTok().getSourceLoc();
         advance();

         auto EqualsLoc = currentTok().getSourceLoc();
         advance();

         auto* RHS = parseExprSequence().tryGetExpr();

         return DiscardAssignStmt::Create(Context, UnderscoreLoc, EqualsLoc,
                                          RHS);
      }

      stmt = parseExprSequence();
      break;
   }
   case tok::ident:
      if (lookahead(false, true).is(tok::colon)) {
         auto* Label = currentTok().getIdentifierInfo();
         advance();
         advance();

         if (currentTok().is(Ident_do))
            return parseDoStmt(Label);

         switch (currentTok().getKind()) {
         case tok::kw_if:
            return parseIfStmt(Label);
         case tok::kw_for:
            return parseForStmt(Label);
         case tok::kw_match:
            return parseMatchStmt(Label);
         default:
            SP.diagnose(err_generic_error, "expected loop after label",
                        currentTok().getSourceLoc());

            return parseNextStmt();
         }
      }

      LLVM_FALLTHROUGH;
   default:
      stmt = parseExprSequence(DefaultFlags | F_ParsingStatement);
      break;
   }

   return stmt;
}

void Parser::parseStmts(llvm::SmallVectorImpl<Statement*>& Stmts)
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

ParseResult Parser::parseUnittestDecl()
{
   if (isAtRecordLevel()) {
      SP.diagnose(err_generic_error,
                  "'unittest' can only appear at the top level",
                  currentTok().getSourceLoc());
   }

   assert(currentTok().is(Ident_unittest));

   auto KeywordLoc = consumeToken();
   IdentifierInfo* Name = nullptr;

   if (currentTok().is(tok::stringliteral)) {
      Name = &Context.getIdentifiers().get(currentTok().getText());
      advance();
   }

   if (!currentTok().is(tok::open_brace)) {
      errorUnexpectedToken(currentTok(), tok::open_brace);
      skipUntilNextDecl();

      return ParseError();
   }

   if (!SP.getCompilerInstance().getOptions().runUnitTests()) {
      advance();
      skipUntilEven(tok::open_brace);
      advance();

      return ParseError();
   }

   SourceLocation LBraceLoc = currentTok().getSourceLoc();

   auto* D = UnittestDecl::Create(Context, KeywordLoc, {}, Name, nullptr);
   {
      DeclContextRAII DCR(*this, D);
      EnterFunctionScope EF(*this);

      auto* Body = parseCompoundStmt(false, true).tryGetStatement();
      D->setBody(Body);

      SourceRange BraceRange(LBraceLoc, currentTok().getSourceLoc());
      D->setBraceRange(BraceRange);
   }

   return ActOnDecl(D);
}

ParseResult Parser::parseNamespaceDecl()
{
   auto Loc = currentTok().getSourceLoc();

   NamespaceDecl* NS = nullptr;
   SmallVector<DeclContextRAII*, 2> DCRs;

   advance();
   while (currentTok().is(tok::ident)) {
      // return previous namespace if a namespace with this name is already
      // in scope
      auto* Name = currentTok().getIdentifierInfo();

      NamespaceDecl* NextNS = NamespaceDecl::Create(Context, Loc, {}, Name);
      if (auto Prev = SP.getDeclContext().lookupSingle<NamespaceDecl>(Name)) {
         NextNS->setPrimaryCtx(Prev);
      }

      ActOnDecl(NextNS);
      DCRs.emplace_back(new DeclContextRAII(*this, NextNS));

      advance();
      if (currentTok().is(tok::period)) {
         advance();
      }

      NS = NextNS;
   }

   if (DCRs.empty()) {
      NamespaceDecl* AnonNS
          = NamespaceDecl::Create(Context, Loc, {}, DeclarationName());

      ActOnDecl(AnonNS);
      DCRs.emplace_back(new DeclContextRAII(*this, AnonNS));

      NS = AnonNS;
   }

   if (!currentTok().is(tok::open_brace)) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                  currentTok().toString(), true, "'{'");

      return ParseError();
   }

   SourceLocation LBrace = currentTok().getSourceLoc();
   advance();

   while (!currentTok().is(tok::close_brace)) {
      auto declResult = parseNextDecl();
      if (!declResult.holdsDecl()) {
         if (!skipUntilNextDeclOrClosingBrace())
            break;
      }

      if (lookahead().is(tok::eof))
         break;

      advance();
   }

   SourceRange SR(LBrace, currentTok().getSourceLoc());
   while (!DCRs.empty()) {
      auto* NextNS = cast<NamespaceDecl>(&SP.getDeclContext());
      NextNS->setBraces(SR);

      delete DCRs.pop_back_val();
   }

   if (ParsingProtocol) {
      SP.diagnose(NS, err_may_not_appear_in_protocol, NS,
                  currentTok().getSourceLoc());
   }

   return NS;
}

ParseResult Parser::parseUsingDecl()
{
   auto UsingLoc = currentTok().getSourceLoc();

   llvm::SmallVector<DeclarationName, 4> declContext;
   llvm::SmallVector<DeclarationName, 4> importedItems;

   bool First = true;
   SourceLocation wildCardLoc;

   while (First || lookahead().is(tok::period)) {
      advance();
      if (currentTok().is(tok::period))
         advance();

      if (currentTok().is(tok::open_brace))
         break;

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
      First = false;
   }

   SourceLocation LBraceLoc;
   SourceLocation RBraceLoc;

   if (currentTok().is(tok::open_brace)) {
      LBraceLoc = currentTok().getSourceLoc();

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

      RBraceLoc = currentTok().getSourceLoc();
   }

   DeclarationName Name;
   if (lookahead().is(Ident_as)) {
      if (!importedItems.empty() || wildCardLoc.isValid()) {
         SP.diagnose(err_invalid_using_alias, currentTok().getSourceLoc());
      }

      advance();
      advance();

      if (!currentTok().is(tok::ident)) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                     currentTok().toString(), true, "identifier");
      }
      else {
         Name = currentTok().getIdentifierInfo();
      }
   }
   else if (!wildCardLoc) {
      Name = declContext.back();
   }

   SourceRange SR(UsingLoc, currentTok().getSourceLoc());
   if (!importedItems.empty()) {
      if (wildCardLoc.isValid()) {
         SP.diagnose(err_import_multiple_with_wildcard, wildCardLoc);
         importedItems.clear();
      }
      else {
         for (auto& Item : importedItems) {
            declContext.push_back(Item);

            auto D = UsingDecl::Create(Context, SR, CurDeclAttrs.Access, Item,
                                       declContext, wildCardLoc.isValid());

            D->setAccessLoc(CurDeclAttrs.AccessLoc);

            ActOnDecl(D);
            declContext.pop_back();
         }

         return ParseError();
      }
   }

   auto D = UsingDecl::Create(Context, SR, CurDeclAttrs.Access, Name,
                              declContext, wildCardLoc.isValid());

   D->setAccessLoc(CurDeclAttrs.AccessLoc);
   ActOnDecl(D);

   return D;
}

ParseResult Parser::parseModuleDecl()
{
   assert(currentTok().is(tok::kw_module) && "don't call this otherwise!");

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
   return SP.getCompilerInstance().getModuleMgr().GetOrCreateModule(SR,
                                                                   moduleName);
}

ParseResult Parser::parseImportDecl()
{
   bool Wildcard = false;
   auto ImportLoc = consumeToken();
   SmallVector<DeclarationName, 2> ImportedNames;
   SmallVector<DeclarationName, 4> moduleName;

   bool ExpectFrom = false;
   if (currentTok().is(tok::times)) {
      Wildcard = true;
      ExpectFrom = true;
      advance();
   }
   else if (currentTok().is(tok::open_brace)) {
      advance();
      while (!currentTok().is(tok::close_brace)) {
         if (!currentTok().is(tok::ident)) {
            SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                        currentTok().toString(), true, "identifier");

            advance();
            continue;
         }

         ImportedNames.push_back(currentTok().getIdentifierInfo());
         advance();

         if (currentTok().is(tok::comma))
            advance();
      }

      ExpectFrom = true;
      advance();
   }
   else if (lookahead().is(Ident_from)) {
      if (!currentTok().is(tok::ident)) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                     currentTok().toString(), true, "identifier");
      }
      else {
         ImportedNames.push_back(currentTok().getIdentifierInfo());
      }

      advance();
      advance();
   }

   if (ExpectFrom) {
      if (!currentTok().is(Ident_from)) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                     currentTok().toString(), true, "'from'");
      }
      else {
         advance();
      }
   }

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

   unsigned EndOffset = currentTok().getSourceLoc().getOffset()
                        + moduleName.back().getIdentifierInfo()->getLength();

   SourceRange SR(ImportLoc, SourceLocation(EndOffset));
   auto D = ImportDecl::Create(Context, SR, CurDeclAttrs.Access, moduleName,
                               ImportedNames, Wildcard);

   D->setAccessLoc(CurDeclAttrs.AccessLoc);
   return ActOnDecl(D);
}

ModuleDecl* Parser::parseModule(bool& IgnoreSourceFile)
{
   IgnoreSourceFile = false;

   while (currentTok().oneOf(tok::newline, tok::space, tok::semicolon))
      advance();

   if (!moduleDeclFollows())
      return nullptr;

   SmallVector<Attr*, 4> Attrs;
   if (currentTok().is(tok::at)) {
      bool FoundVersionAttr;
      parseAttributes(Attrs, AttrClass::Decl, &FoundVersionAttr);

      VersionDeclAttr::VersionKind V = VersionDeclAttr::Windows;
      for (auto* A : Attrs) {
         if (auto* VA = dyn_cast<VersionDeclAttr>(A)) {
            V = VA->getVersion();
            break;
         }
      }

      if (FoundVersionAttr) {
         IgnoreSourceFile = !versionSatisfied(V);
      }
   }

   if (currentTok().is(tok::kw_module)) {
      auto Mod = parseModuleDecl().tryGetDecl<ModuleDecl>();
      advance();

      if (Mod) {
         Mod->getModule()->setContainsNewDecls(true);
         Context.setAttributes(Mod, Attrs);
      }

      return Mod;
   }

   return nullptr;
}

void Parser::parse()
{
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

void Parser::parseMainFile()
{
   EnterFunctionScope EF(*this);

   SourceLocation Loc = SP.getCompilerInstance().getMainFileLoc();
   auto* II = &Context.getIdentifiers().get("_start");
   auto* Fn = FunctionDecl::Create(
       Context, AccessSpecifier::Private, Loc, DeclarationName(II), {},
       SourceType(Context.getVoidType()), nullptr, {});

   SmallVector<Statement*, 8> Stmts;
   {
      DeclContextRAII DCR(*this, Fn);
      while (true) {
         while (currentTok().oneOf(tok::semicolon, tok::newline, tok::space)) {
            advance();
         }
         if (currentTok().getKind() == tok::eof) {
            break;
         }

         auto nextStmt = parseNextStmt(false);
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
            skipUntilProbableEndOfStmt();
         }

         advance();
      }
   }

   auto* CS = CompoundStmt::Create(Context, Stmts, false, Loc,
                                   currentTok().getSourceLoc());

   Fn->setBody(CS);
   Fn->setIsMain(true);

   SP.getCompilerInstance().setMainFn(Fn);
   ActOnDecl(Fn);
}

} // namespace parse
} // namespace cdot