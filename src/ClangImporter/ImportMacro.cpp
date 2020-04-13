#include "ImporterImpl.h"

#include "AST/ASTContext.h"
#include "AST/Decl.h"
#include "AST/Expression.h"
#include "Driver/Compiler.h"
#include "Sema/SemaPass.h"
#include "Support/LiteralParser.h"

#include <clang/Frontend/CompilerInstance.h>
#include <clang/Lex/Preprocessor.h>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::support;

using ImporterImpl = ClangImporter::ImporterImpl;

lex::Token ImporterImpl::getToken(const clang::Token &Tok)
{
   using namespace lex;
   namespace cl = clang::tok;

   auto &Ctx = CI.getContext();
   auto &Idents = Ctx.getIdentifiers();
   SourceLocation Loc = getSourceLoc(Tok.getLocation());

#  define SAME_TOK(KIND) case cl::KIND: return Token(tok::KIND, Loc)
#  define REPLACE(CLANG, CDOT) case cl::CLANG: return Token(tok::CDOT, Loc)
#  define KEYWORD(KW) case cl::kw_##KW: return Token(tok::kw_##KW, Loc)
#  define KEYWORD_ID(KW) case cl::kw_##KW:                              \
      return Token(&Idents.get(Tok.getIdentifierInfo()->getName()),     \
                   Loc, tok::ident)

   switch (Tok.getKind()) {
   SAME_TOK(eof);
   
   case cl::identifier:
   case cl::raw_identifier:
      return Token(&Idents.get(Tok.getIdentifierInfo()->getName()), Loc, 
                   tok::ident);
   case cl::numeric_constant: {
      // FIXME detect this better.
      StringRef Txt(Tok.getLiteralData(), Tok.getLength());

      char *Data = Ctx.Allocate<char>(Tok.getLength());
      std::copy(Txt.begin(), Txt.end(), Data);

      if (Txt.find('.') != string::npos) {
         return Token(Data, Txt.size(), tok::fpliteral, Loc);
      }
      
      return Token(Data, Txt.size(), tok::integerliteral, Loc);
   }
   case cl::char_constant:
   case cl::utf8_char_constant:
   case cl::utf16_char_constant:
   case cl::utf32_char_constant: {
      StringRef Txt(Tok.getLiteralData(), Tok.getLength());

      char *Data = Ctx.Allocate<char>(Tok.getLength());
      std::copy(Txt.begin(), Txt.end(), Data);

      return Token(Data, Tok.getLength(), tok::charliteral, Loc);
   }
   case cl::string_literal:
   case cl::angle_string_literal:
   case cl::utf8_string_literal:
   case cl::utf16_string_literal:
   case cl::utf32_string_literal: {
      StringRef Txt(Tok.getLiteralData(), Tok.getLength());

      char *Data = Ctx.Allocate<char>(Tok.getLength());
      std::copy(Txt.begin(), Txt.end(), Data);

      return Token(Data, Tok.getLength(), tok::stringliteral, Loc);
   }

   REPLACE(l_square, open_square);
   REPLACE(r_square, close_square);

   REPLACE(l_paren, open_paren);
   REPLACE(r_paren, close_paren);

   REPLACE(l_brace, open_brace);
   REPLACE(r_brace, close_brace);

   SAME_TOK(period);
   REPLACE(ellipsis, triple_period);
   REPLACE(amp, op_and);
   REPLACE(ampamp, logical_and);
   REPLACE(ampequal, and_equals);
   REPLACE(star, times);
   REPLACE(starequal, times_equals);
   SAME_TOK(plus);
   REPLACE(plusplus, plus_plus);
   REPLACE(plusequal, plus_equals);
   SAME_TOK(minus);
   REPLACE(minusminus, minus_minus);
   REPLACE(minusequal, minus_equals);
   SAME_TOK(tilde);
   SAME_TOK(exclaim);
   REPLACE(exclaimequal, exclaim_equals);
   REPLACE(slash, div);
   REPLACE(slashequal, div_equals);
   SAME_TOK(percent);
   REPLACE(percentequal, mod_equals);
   REPLACE(less, smaller);
   REPLACE(lessless, shl);
   REPLACE(lessequal, smaller_equals);
   REPLACE(lesslessequal, shl_equals);
   REPLACE(greater, greater);
   REPLACE(greatergreater, ashr);
   REPLACE(greaterequal, greater_equals);
   REPLACE(greatergreaterequal, ashr_equals);
   SAME_TOK(caret);
   REPLACE(caretequal, xor_equals);
   REPLACE(pipe, op_or);
   REPLACE(pipepipe, logical_or);
   REPLACE(pipeequal, or_equals);
   SAME_TOK(question);
   SAME_TOK(colon);
   REPLACE(semi, semicolon);
   REPLACE(equal, equals);
   REPLACE(equalequal, double_equals);
   SAME_TOK(comma);
   REPLACE(hash, pound);
   REPLACE(arrow, arrow_single);

   case cl::spaceship:
   case cl::periodstar:
   case cl::arrowstar:
   case cl::coloncolon:
   case cl::lesslessless:
   case cl::greatergreatergreater:
   case cl::caretcaret:
      return Token(&Idents.get(Tok.getIdentifierInfo()->getName()), Loc,
                   tok::op_ident);

   case cl::hashhash: return Token(&Idents.get("##"), Loc, tok::op_ident);
   case cl::hashat: return Token(&Idents.get("#@"), Loc, tok::op_ident);
   
   SAME_TOK(at);

   KEYWORD_ID(auto);
   KEYWORD(break);
   KEYWORD(case);
   KEYWORD_ID(char);
   KEYWORD_ID(const);
   KEYWORD(continue);
   KEYWORD_ID(default);
   KEYWORD_ID(do);
   KEYWORD_ID(double);
   KEYWORD(else);
   KEYWORD(enum);
   KEYWORD_ID(extern);
   KEYWORD_ID(float);
   KEYWORD(for);
   KEYWORD_ID(goto);
   KEYWORD(if);
   KEYWORD_ID(inline);
   KEYWORD_ID(int);
   KEYWORD_ID(long);
   KEYWORD_ID(register);
   KEYWORD_ID(restrict);
   KEYWORD(return);
   KEYWORD_ID(short);
   KEYWORD_ID(signed);
   KEYWORD_ID(sizeof);
   KEYWORD(static);
   KEYWORD(struct);
   KEYWORD_ID(switch);
   KEYWORD(typedef);
   KEYWORD(union);
   KEYWORD_ID(unsigned);
   KEYWORD_ID(void);
   KEYWORD_ID(volatile);
   KEYWORD(while);
   KEYWORD_ID(_Alignas);
   KEYWORD_ID(_Alignof);
   KEYWORD_ID(_Atomic);
   KEYWORD_ID(_Bool);
   KEYWORD_ID(_Complex);
   KEYWORD_ID(_Generic);
   KEYWORD_ID(_Imaginary);
   KEYWORD_ID(_Noreturn);
   KEYWORD_ID(_Thread_local);
   KEYWORD_ID(__func__);
   KEYWORD_ID(__objc_yes);
   KEYWORD_ID(__objc_no);
   KEYWORD_ID(asm);
   KEYWORD_ID(bool);
   KEYWORD(catch);
   KEYWORD(class);
   KEYWORD_ID(const_cast);
   KEYWORD_ID(delete);
   KEYWORD_ID(dynamic_cast);
   KEYWORD_ID(explicit);
   KEYWORD_ID(export);
   KEYWORD(false);
   KEYWORD_ID(friend);
   KEYWORD_ID(mutable);
   KEYWORD(namespace);
   KEYWORD_ID(new);
   KEYWORD_ID(operator);
   KEYWORD(private);
   KEYWORD(protected);
   KEYWORD(public);
   KEYWORD_ID(reinterpret_cast);
   KEYWORD_ID(static_cast);
   KEYWORD_ID(template);
   KEYWORD(throw);
   KEYWORD(true);
   KEYWORD(try);
   KEYWORD_ID(typename);
   KEYWORD_ID(typeid);
   KEYWORD_ID(using);
   KEYWORD_ID(virtual);
   KEYWORD_ID(wchar_t);

   case cl::kw_this:
      return Token(tok::kw_self, Loc);
   case cl::kw__Static_assert:
   case cl::kw_static_assert:
      return Token(tok::kw_static_assert, Loc);
   default:
      return Token(&Idents.get(Tok.getIdentifierInfo()->getName()), Loc,
                   tok::ident);
   }

#undef SAME_TOK
#undef REPLACE
#undef KEYWORD
#undef KEYWORD_ID
}

static bool isSimpleConstant(clang::MacroInfo *MI)
{
   if (MI->getNumTokens() != 1)
      return false;

   return MI->getReplacementToken(0).isLiteral();
}

static bool isSimpleExpression(clang::MacroInfo *MI)
{
   namespace cl = clang::tok;

   for (auto &Tok : MI->tokens()) {
      switch (Tok.getKind()) {
      case cl::numeric_constant:
      case cl::char_constant:
      case cl::utf8_char_constant:
      case cl::utf16_char_constant:
      case cl::utf32_char_constant:
      case cl::string_literal:
      case cl::angle_string_literal:
      case cl::utf8_string_literal:
      case cl::utf16_string_literal:
      case cl::utf32_string_literal:
      case cl::l_paren:
      case cl::r_paren:
      case cl::l_square:
      case cl::r_square:
      case cl::period:
      case cl::ellipsis:
      case cl::amp:
      case cl::ampamp:
      case cl::ampequal:
      case cl::star:
      case cl::starequal:
      case cl::plus:
      case cl::plusplus:
      case cl::plusequal:
      case cl::minus:
      case cl::minusminus:
      case cl::minusequal:
      case cl::tilde:
      case cl::exclaim:
      case cl::exclaimequal:
      case cl::slash:
      case cl::slashequal:
      case cl::percent:
      case cl::percentequal:
      case cl::less:
      case cl::lessless:
      case cl::lessequal:
      case cl::greater:
      case cl::greatergreater:
      case cl::greaterequal:
      case cl::caret:
      case cl::caretequal:
      case cl::pipe:
      case cl::pipepipe:
      case cl::pipeequal:
      case cl::question:
      case cl::colon:
      case cl::semi:
      case cl::equal:
      case cl::equalequal:
      case cl::comma:
      case cl::hash:
      case cl::spaceship:
         break;
      default:
         return false;
      }
   }
   
   return true;
}

static void importMacro(ImporterImpl &Importer,
                        ASTContext &Ctx,
                        SemaPass &Sema,
                        DeclContext *DC,
                        clang::IdentifierInfo *Name,
                        clang::MacroInfo *MI) {
   bool IsSimpleConstant = isSimpleConstant(MI);
   bool HasNoTokens = MI->getNumTokens() == 0;
   bool IsSimpleExpression = false;

   if (!IsSimpleConstant && !HasNoTokens) {
      IsSimpleExpression = isSimpleExpression(MI);
   }

   if (IsSimpleConstant || HasNoTokens || IsSimpleExpression) {
      SourceLocation Loc = Importer.getSourceLoc(MI->getDefinitionLoc());
      Expression *Expr;

      // Handle the simple case of a constant first, these get imported as aliases.
      if (IsSimpleConstant) {
         Expr = Importer.expressionFromLiteralToken(MI->getReplacementToken(0));
      }
      // Empty expansions get imported as `true`.
      else if (HasNoTokens) {
         Expr = BoolLiteral::Create(Ctx, Loc, Ctx.getBoolTy(), true);
      }
      // For other Parameterless macros, we need to parse the expression.
      else {
         Expr = Importer.parseExpression(MI->tokens());
      }

      if (!Expr)
         return;

      auto *SE = StaticExpr::Create(Ctx, Expr);
      auto *Alias = AliasDecl::Create(Ctx, Loc,
                                  AccessSpecifier::Public,
                                  Ctx.getIdentifiers().get(Name->getName()),
                                  SourceType(Ctx.getAutoType()), SE, {});

      Alias->setImportedFromClang(true);
      Sema.ActOnDecl(DC, Alias);
      return;
   }

   // Otherwise, translate the full pattern.
   SmallVector<PatternFragment*, 4> PatternFragments;

   auto BeginPat = PatternFragment::Create(Ctx);
   PatternFragments.push_back(BeginPat);

   // Remember the parameter names so they can be properly expanded.
   SmallPtrSet<const clang::IdentifierInfo*, 4> ParamNames;
   SourceLocation Loc = Importer.getSourceLoc(MI->getDefinitionLoc());

   unsigned i = 0;
   for (auto *ParamName : MI->params()) {
      auto *Pat = PatternFragment::Create(Ctx, Loc,
                                          PatternFragment::Any,
                                          &Ctx.getIdentifiers().get(
                                             ParamName->getName()));

      PatternFragments.back()->addTransition(
         lex::Token(i++ != 0 ? lex::tok::comma : lex::tok::sentinel), Pat);

      ParamNames.insert(ParamName);
      PatternFragments.push_back(Pat);
   }

   // Handle variadic macro
   if (MI->isVariadic()) {
      // The final fragment that will merge all paths.
      auto *Merge = PatternFragment::Create(Ctx);

      // Add an unconditional transition to the first expansion.
      auto *Previous = PatternFragments.back();

      /// Add a state to skip a leading comma.
      auto *SkippedComma = PatternFragment::Create(Ctx);
      Previous->addTransition(lex::Token(lex::tok::comma), SkippedComma);
      Previous->addTransition(lex::Token(), SkippedComma);

      // Create a repetition fragment.
      auto *Repetition = PatternFragment::Create(Ctx, Loc,
         PatternFragment::Any, &Ctx.getIdentifiers().get("__VA_ARGS__"));

      // Has to happen at least once, transition to error if not
      SkippedComma->addTransition(lex::Token(), Repetition);
      SkippedComma->addTransition(lex::Token(lex::tok::eof),
                                  PatternFragment::GetErrorState());

      // create a new state with the same transitions as LastInRep,
      // except that now it works like the '*' qualfiier
      auto ZeroOrMoreState = PatternFragment::Create(Ctx);
      Repetition->addTransition(lex::Token(lex::tok::eof), Merge);
      Repetition->addTransition(lex::Token(lex::tok::comma), ZeroOrMoreState);

      // after one successful iteration, go either to merge or back to
      // the beginning
      ZeroOrMoreState->addTransition(lex::Token(lex::tok::eof), Merge);
      ZeroOrMoreState->addTransition(lex::Token(), Repetition);

      PatternFragments.push_back(Merge);
   }

   auto EndPat = PatternFragment::Create(Ctx);
   PatternFragments.back()->addTransition(lex::Token(lex::tok::eof), EndPat);

   // Create the expansion.
   SmallVector<ExpansionFragment*, 4> ExpansionFragments;
   SmallVector<lex::Token, 8> CurrToks;

   auto FlushToks = [&] {
      if (CurrToks.empty())
         return;

      auto *Exp = ExpansionFragment::Create(Ctx,
                                            CurrToks.front().getSourceLoc(),
                                            CurrToks);

      CurrToks.clear();
      ExpansionFragments.push_back(Exp);
   };

   unsigned Len = 0;
   if (MI->getNumTokens() != 0) {
      auto First = MI->getReplacementToken(0);
      auto Last = MI->getReplacementToken(MI->getNumTokens() - 1);

      auto &SourceMgr = Importer.Instance->getPreprocessor().getSourceManager();
      Len = SourceMgr.getFileOffset(Last.getLocation())
         - SourceMgr.getFileOffset(First.getLocation())
         + Last.getLength();
   }

   for (auto &Tok : MI->tokens()) {
      auto CopyTok = Importer.getToken(Tok);
      Len += CopyTok.getLength();

      if (CopyTok.is(lex::tok::ident)) {
         if (Tok.getIdentifierInfo()->isStr("__VA_ARGS__")) {
            // Expand variadic arguments.
            auto *Var = ExpansionFragment::Create(Ctx, CopyTok.getSourceLoc(),
                                                  CopyTok.getIdentifierInfo());

            auto *Comma = ExpansionFragment::Create(Ctx, CopyTok.getSourceLoc(),
                                                    lex::Token(lex::tok::comma));

            auto *Exp = ExpansionFragment::Create(Ctx, CopyTok.getSourceLoc(),
                                                  {Var,Comma},
                                                  CopyTok.getIdentifierInfo());

            ExpansionFragments.push_back(Exp);
            continue;
         }
         if (ParamNames.find(Tok.getIdentifierInfo()) != ParamNames.end()) {
            FlushToks();

            auto *Ident = CopyTok.getIdentifierInfo();
            auto *Exp = ExpansionFragment::Create(Ctx, CopyTok.getSourceLoc(),
                                                  Ident);

            ExpansionFragments.push_back(Exp);
            continue;
         }
      }

      CurrToks.push_back(CopyTok);
   }

   FlushToks();

   SourceLocation ExpansionLoc;
   if (!ExpansionFragments.empty()) {
      ExpansionLoc = ExpansionFragments.front()->getLoc();
   }
   else {
      ExpansionLoc = Loc;
   }

   auto *Pat = MacroPattern::Create(Ctx, Loc, BeginPat,
                                    ExpansionLoc, ExpansionFragments,
                                    Len);

   auto MacroName = Ctx.getDeclNameTable().getMacroName(
      Ctx.getIdentifiers().get(Name->getName()));

   auto *MD = MacroDecl::Create(Ctx, Loc, AccessSpecifier::Public,
                                MacroName, MacroDecl::Paren, Pat);

   MD->setImportedFromClang(true);
   Sema.ActOnDecl(DC, MD);
}

void ImporterImpl::importMacros(DeclContext *DC)
{
   auto &Ctx = CI.getContext();
   auto &Sema = CI.getSema();

   for (auto *Name : MacroNames) {
      auto MD = Instance->getPreprocessor().getMacroDefinition(Name);
      auto *MI = MD.getMacroInfo();

      if (!MI)
         continue;

      importMacro(*this, Ctx, Sema, DC, Name, MI);
   }
}