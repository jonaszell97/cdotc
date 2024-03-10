#include "cdotc/Parse/Parser.h"

#include "cdotc/Basic/FileUtils.h"
#include "cdotc/ClangImporter/ClangImporter.h"
#include "cdotc/Lex/Lexer.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Support/StringSwitch.h"

#include <llvm/ADT/SmallString.h>

using namespace cdot::lex;
using namespace cdot::diag;
using namespace cdot::support;

using std::string;

namespace cdot {
namespace parse {

class MacroParser {
   SemaPass& SP;
   Parser& P;

   // macro parsing
   struct PatternExpansionRAII {
      PatternExpansionRAII(MacroParser& P) : P(P)
      {
         P.PatternExpansionStack.emplace_back();
         ++P.LastExpansionScopeID;
      }

      ~PatternExpansionRAII() { P.PatternExpansionStack.pop_back(); }

   private:
      MacroParser& P;
   };

   struct VariableExpansionRAII {
      VariableExpansionRAII(MacroParser& P, bool Enabled)
          : P(P), Enabled(Enabled)
      {
         if (Enabled) {
            P.VariableExpansionStack.emplace_back();
         }
      }

      ~VariableExpansionRAII()
      {
         if (Enabled) {
            P.VariableExpansionStack.pop_back();
         }
      }

   private:
      MacroParser& P;
      bool Enabled;
   };

   unsigned LastExpansionScopeID = 1;

   using ExpansionStackTy = std::vector<SmallPtrSet<const IdentifierInfo*, 8>>;

   ExpansionStackTy PatternExpansionStack;
   ExpansionStackTy VariableExpansionStack;

   struct PatternVariableDeclaration {
      unsigned ExpansionScopeID = 0;
   };

   llvm::DenseMap<const IdentifierInfo*, PatternVariableDeclaration>
       MacroVarDecls;

   const lex::Token& currentTok() const { return P.currentTok(); }

   Token lookahead() const { return P.lookahead(); }

   void advance(bool ignoreNewline = true, bool whitespace = false) const
   {
      return P.advance(ignoreNewline, whitespace);
   }

   void backtrack() const { return P.lexer->backtrack(); }

public:
   MacroParser(SemaPass& SP, Parser& P) : SP(SP), P(P) {}

   bool Invalid = false;

   PatternFragment* parsePattern();
   PatternFragment* parseInnerPattern(PatternFragment* Previous,
                                      bool SetErrorState = true);

   void parseExpansionFragment(llvm::SmallVectorImpl<ExpansionFragment*>& Vec);
   void parseExpansionFragments(llvm::SmallVectorImpl<ExpansionFragment*>& Vec);

   MacroPattern* parseMacroPattern();
};

PatternFragment* MacroParser::parsePattern()
{
   auto Begin = PatternFragment::Create(SP.getContext());
   auto Penultimate = parseInnerPattern(Begin);

   auto End = PatternFragment::Create(SP.getContext());
   Penultimate->addTransition(P.lexer->makeEOF(), End);

   return Begin;
}

static bool diagnoseInvalidSeparator(SemaPass& SP, PatternFragment* Previous,
                                     const lex::Token& Tok)
{
   if (!Previous)
      return false;

   bool DoCheck = false;
   if (Previous->isVariable()) {
      // If the previous segment has unconditional transitions,
      // backpatch them.
      for (auto& Trans : Previous->getTransitions()) {
         if (Trans.Next && !Trans.Tok && Trans.NeedsBackpatching) {
            Trans.Tok = Tok;
            Trans.NeedsBackpatching = false;
         }
      }

      switch (Previous->getVarKind()) {
      case PatternFragment::Expr:
      case PatternFragment::Stmt:
      case PatternFragment::Type:
         DoCheck = true;
         break;
      default:
         break;
      }
   }

   if (!DoCheck)
      return false;

   bool Valid = false;
   switch (Tok.getKind()) {
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
   case tok::kw_try:
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
   case tok::interpolation_begin:
   case tok::interpolation_end:
      Valid = true;
      break;
   case tok::ident: {
      if (Tok.getIdentifierInfo()->isStr("where"))
         Valid = true;

      if (Tok.getIdentifierInfo()->isStr("using"))
         Valid = true;

      break;
   }
   default:
      break;
   }

   if (!Valid) {
      SP.diagnose(err_invalid_seperator_token, Tok.getSourceLoc(),
                  (int)Previous->getVarKind());
   }

   return !Valid;
}

PatternFragment* MacroParser::parseInnerPattern(PatternFragment* Previous,
                                                bool SetErrorState)
{
   bool done = false;
   unsigned OpenParens = 1;
   PatternFragment* FragmentForSeperatorCheck = nullptr;

   while (true) {
      SourceLocation Loc = currentTok().getSourceLoc();

      switch (currentTok().getKind()) {
      default: {
         llvm::SmallVector<Token, 2> Tokens;

         unsigned InnerOpenBraces = 0;
         unsigned InnerOpenSquare = 0;
         bool innerDone = false;

         bool DiagnosedSeparator = false;
         while (true) {
            switch (currentTok().getKind()) {
            case tok::close_brace:
               if (!InnerOpenBraces) {
                  innerDone = true;
               }
               else {
                  --InnerOpenBraces;
               }

               break;
            case tok::close_paren:
               if (!--OpenParens) {
                  innerDone = true;
                  done = true;
               }

               break;
            case tok::close_square:
               if (!InnerOpenSquare) {
                  innerDone = true;
               }
               else {
                  --InnerOpenSquare;
               }

               break;
            case tok::open_brace:
               ++InnerOpenBraces;
               break;
            case tok::open_paren:
               ++OpenParens;
               break;
            case tok::open_square:
               ++InnerOpenSquare;
               break;
            case tok::dollar_ident:
            case tok::dollar:
               innerDone = true;
               backtrack();
               break;
            case tok::backslash:
               advance();
               break;
            default:
               break;
            }

            if (!done && !DiagnosedSeparator) {
               DiagnosedSeparator = true;
               if (diagnoseInvalidSeparator(SP, FragmentForSeperatorCheck,
                                            currentTok())) {
                  Invalid = true;
               }
            }

            if (innerDone)
               break;

            if (lookahead().is(tok::eof))
               break;

            Tokens.push_back(currentTok());
            advance();
         }

         if (!Tokens.empty()) {
            auto Next = PatternFragment::Create(SP.getContext(), Loc, Tokens);
            Previous->addTransition(Tokens.front(), Next);

            if (SetErrorState)
               Previous->addTransition(Token(),
                                       PatternFragment::GetErrorState());

            Previous = Next;
         }

         FragmentForSeperatorCheck = Previous;

         break;
      }
      case tok::dollar_ident: {
         // variable introduction
         auto VarName = currentTok().getIdentifierInfo();
         PatternFragment::VariableKind VarKind = PatternFragment::Any;

         if (lookahead().is(tok::colon)) {
            advance();
            advance();

            if (!currentTok().is(tok::ident)) {
               SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                           currentTok().toString(), true, "identifier");

               Invalid = true;
            }
            else {
               auto Ident = currentTok().getIdentifierInfo();
               if (Ident->isStr("expr")) {
                  VarKind = PatternFragment::Expr;
               }
               else if (Ident->isStr("stmt")) {
                  VarKind = PatternFragment::Stmt;
               }
               else if (Ident->isStr("type")) {
                  VarKind = PatternFragment::Type;
               }
               else if (Ident->isStr("decl")) {
                  VarKind = PatternFragment::Decl;
               }
               else if (Ident->isStr("ident")) {
                  VarKind = PatternFragment::Ident;
               }
               else if (Ident->isStr("any")) {
                  VarKind = PatternFragment::Any;
               }
               else if (Ident->isStr("tok")) {
                  VarKind = PatternFragment::Tok;
               }
               else {
                  SP.diagnose(err_variable_kind_must_be,
                              currentTok().getSourceLoc());

                  Invalid = true;
               }
            }
         }

         auto It = MacroVarDecls.find(VarName);
         if (It != MacroVarDecls.end()) {
            SP.diagnose(err_macro_var_redeclaration, VarName->getIdentifier(),
                        currentTok().getSourceLoc());

            Invalid = true;
         }

         if (PatternExpansionStack.empty()) {
            MacroVarDecls[VarName] = PatternVariableDeclaration{0};
         }
         else {
            MacroVarDecls[VarName]
                = PatternVariableDeclaration{LastExpansionScopeID};
            PatternExpansionStack.back().insert(VarName);
         }

         auto Next
             = PatternFragment::Create(SP.getContext(), Loc, VarKind, VarName);

         Previous->addTransition(Token(), Next);
         Previous = Next;

         FragmentForSeperatorCheck = Previous;

         break;
      }
      case tok::dollar: {
         // repetition pattern
         advance();

         if (!currentTok().is(tok::open_paren)) {
            SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                        currentTok().toString(), true, "'('");

            Invalid = true;
         }

         PatternExpansionRAII patternExpansionRAII(*this);

         advance();

         auto* PatternStart = PatternFragment::Create(SP.Context);
         Previous->addTransition(Token(), PatternStart);

         auto LastInRepetition = parseInnerPattern(PatternStart, false);

         PatternFragment::FragmentKind Kind;
         lex::Token Delim;

         advance();

         while (true) {
            switch (currentTok().getKind()) {
            case tok::times:
               Kind = PatternFragment::Star;
               break;
            case tok::plus:
               Kind = PatternFragment::Plus;
               break;
            case tok::question:
               Kind = PatternFragment::Question;
               break;
            default:
               if (Delim.getKind() != tok::sentinel
                   || currentTok().oneOf(tok::close_paren, tok::close_brace,
                                         tok::close_square)) {
                  SP.diagnose(err_expected_after_repetition,
                              currentTok().getSourceLoc());

                  Invalid = true;
                  Kind = PatternFragment::Star;
                  backtrack();

                  break;
               }
               else {
                  Delim = currentTok();
                  advance();

                  continue;
               }
            }

            break;
         }

         auto& Ctx = SP.getContext();

         auto Merge = PatternFragment::Create(Ctx);
         switch (Kind) {
         case PatternFragment::Star: {
            // if the repetition can happen zero times (as with '*' and '?')
            // add a default transition to the merge state
            PatternStart->addTransition(Token(), Merge);

            // if there's a delimiter, add a transition for it
            if (Delim.getKind() != tok::sentinel) {
               LastInRepetition->addTransition(Delim, PatternStart);
               LastInRepetition->addTransition(Token(), Merge, true);
            }
            // Check if a distinct token gets you back in the repetition,
            // otherwise only eof gets you to the end.
            else {
               lex::Token FirstTokInRepetition
                   = Previous->getTransitions().front().Tok;

               lex::Token MergeTok
                   = FirstTokInRepetition ? Token() : Token(tok::eof);

               LastInRepetition->addTransition(MergeTok, Merge);
               LastInRepetition->addTransition(FirstTokInRepetition,
                                               PatternStart,
                                               /*IsConsuming=*/false);
            }

            break;
         }
         case PatternFragment::Plus: {
            // has to happen at least once, transition to error if not
            PatternStart->addTransition(Token(),
                                        PatternFragment::GetErrorState());

            // create a new state with the same transitions as LastInRep,
            // except that now it works like the '*' qualfiier
            auto ZeroOrMoreState = PatternFragment::Create(Ctx);

            // after one successful iteration, go either to merge or back to
            // the beginning
            if (Delim.getKind() != tok::sentinel) {
               LastInRepetition->addTransition(Delim, ZeroOrMoreState,
                                               /*IsConsuming=*/false);
               LastInRepetition->addUnconditionalTransition(
                   Merge,
                   /*IsConsuming=*/true,
                   /*NeedsBackpatching=*/true);

               ZeroOrMoreState->addTransition(Delim, PatternStart);
               ZeroOrMoreState->addTransition(Token(), Merge);
            }
            // Check if a distinct token gets you back in the repetition,
            // otherwise only eof gets you to the end.
            else {
               lex::Token FirstTokInRepetition
                   = Previous->getTransitions().front().Tok;

               lex::Token MergeTok
                   = FirstTokInRepetition ? Token() : Token(tok::eof);

               LastInRepetition->addTransition(FirstTokInRepetition,
                                               ZeroOrMoreState,
                                               /*IsConsuming=*/false);
               LastInRepetition->addTransition(MergeTok, Merge,
                                               /*IsConsuming=*/true,
                                               /*NeedsBackpatching=*/true);

               ZeroOrMoreState->addTransition(MergeTok, Merge);
               ZeroOrMoreState->addTransition(FirstTokInRepetition,
                                              PatternStart,
                                              /*IsConsuming=*/false);
            }

            break;
         }
         case PatternFragment::Question: {
            // if the repetition can happen zero times (as with '*' and '?')
            // add a default transition to the merge state
            PatternStart->addTransition(Token(), Merge);

            // if the transition is taken, go to merge as well
            LastInRepetition->addTransition(Token(), Merge);
            break;
         }
         default:
            llvm_unreachable("can't happen here!");
         }

         Previous = Merge;
         FragmentForSeperatorCheck = LastInRepetition;

         break;
      }
      }

      if (done)
         break;

      advance();
   }

   return Previous;
}

void MacroParser::parseExpansionFragment(
    llvm::SmallVectorImpl<ExpansionFragment*>& Vec)
{
   SourceLocation Loc = currentTok().getSourceLoc();

   switch (currentTok().getKind()) {
   case tok::dollar_ident: {
      // variable expansion
      auto VarName = currentTok().getIdentifierInfo();
      auto It = MacroVarDecls.find(VarName);
      if (It == MacroVarDecls.end()) {
         SP.diagnose(err_macro_undeclared_variable, VarName->getIdentifier(),
                     currentTok().getSourceLoc());

         Invalid = true;
      }
      else if (It->getSecond().ExpansionScopeID != 0
               && VariableExpansionStack.empty()) {
         SP.diagnose(err_macro_variable_unexpanded, VarName->getIdentifier(),
                     currentTok().getSourceLoc());

         Invalid = true;
      }
      else if (It->getSecond().ExpansionScopeID != 0
               && !VariableExpansionStack.empty()) {
         auto& back = VariableExpansionStack.back();
         back.insert(VarName);
      }

      Vec.push_back(ExpansionFragment::Create(SP.getContext(), Loc, VarName));
      break;
   }
   case tok::dollar: {
      // repetition expansion
      llvm::SmallVector<ExpansionFragment*, 4> RepetitionFragments;
      advance();

      bool IsConcatExpr = false;
      if (currentTok().is(tok::open_brace)) {
         IsConcatExpr = true;
      }
      else if (!currentTok().is(tok::open_paren)) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                     currentTok().toString(), true, "'('");

         Invalid = true;
      }

      advance();

      VariableExpansionRAII variableExpansionRAII(*this, !IsConcatExpr);
      parseExpansionFragments(RepetitionFragments);

      // concat expression, e.g. ${xyz = +} -> tok::ident(xyz=+)
      if (IsConcatExpr) {
         P.lexer->backtrack();
         P.expect(tok::close_brace);
         Vec.push_back(ExpansionFragment::Create(SP.getContext(), Loc,
                                                 RepetitionFragments));

         break;
      }

      advance();
      if (!currentTok().is(tok::triple_period)) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                     currentTok().toString(), true, "'...'");

         Invalid = true;
      }

      const IdentifierInfo* Fst = nullptr;

      auto& Var = VariableExpansionStack.back();
      if (Var.empty()) {
         SP.diagnose(err_expansion_without_variable,
                     currentTok().getSourceLoc());

         Invalid = true;
      }
      else {
         Fst = *Var.begin();
      }

      Vec.push_back(ExpansionFragment::Create(SP.getContext(), Loc,
                                              RepetitionFragments, Fst));

      break;
   }
   default: {
      llvm::SmallVector<Token, 2> Tokens;
      unsigned OpenBraces = 0;
      unsigned OpenParens = 0;
      unsigned OpenSquare = 0;

      bool done = false;
      while (true) {
         switch (currentTok().getKind()) {
         case tok::close_brace:
            if (!OpenBraces) {
               done = true;
            }
            else {
               --OpenBraces;
            }

            break;
         case tok::close_paren:
            if (!OpenParens) {
               done = true;
            }
            else {
               --OpenParens;
            }

            break;
         case tok::close_square:
            if (!OpenSquare) {
               done = true;
            }
            else {
               --OpenSquare;
            }

            break;
         case tok::open_brace:
            ++OpenBraces;
            break;
         case tok::open_paren:
            ++OpenParens;
            break;
         case tok::open_square:
            ++OpenSquare;
            break;
         case tok::dollar_ident:
         case tok::dollar:
            if (!Tokens.empty())
               Vec.push_back(
                   ExpansionFragment::Create(SP.getContext(), Loc, Tokens));

            Tokens.clear();
            parseExpansionFragment(Vec);

            advance(false, true);
            continue;
         case tok::backslash:
            advance(false, true);
            break;
         default:
            break;
         }

         if (done) {
            backtrack();
            break;
         }

         if (lookahead().is(tok::eof))
            break;

         Tokens.push_back(currentTok());
         advance(false, true);
      }

      if (!Tokens.empty())
         Vec.push_back(ExpansionFragment::Create(SP.getContext(), Loc, Tokens));

      break;
   }
   }
}

void MacroParser::parseExpansionFragments(
    llvm::SmallVectorImpl<ExpansionFragment*>& Vec)
{
   unsigned OpenBraces = 0;
   unsigned OpenParens = 0;
   unsigned OpenSquare = 0;

   bool done = false;
   while (true) {
      switch (currentTok().getKind()) {
      case tok::close_brace:
         if (!OpenBraces) {
            done = true;
         }
         else {
            --OpenBraces;
         }

         break;
      case tok::close_paren:
         if (!OpenParens) {
            done = true;
         }
         else {
            --OpenParens;
         }

         break;
      case tok::close_square:
         if (!OpenSquare) {
            done = true;
         }
         else {
            --OpenSquare;
         }

         break;
      case tok::open_brace:
         ++OpenBraces;
         break;
      case tok::open_paren:
         ++OpenParens;
         break;
      case tok::open_square:
         ++OpenSquare;
         break;
      default:
         break;
      }

      if (done)
         break;

      parseExpansionFragment(Vec);

      if (lookahead().is(tok::eof))
         break;

      advance();
   }
}

MacroPattern* MacroParser::parseMacroPattern()
{
   SourceLocation Loc = currentTok().getSourceLoc();
   llvm::SmallVector<ExpansionFragment*, 8> ExpansionFragments;

   if (currentTok().getKind() != tok::open_paren) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                  currentTok().toString(), true, "'('");

      Invalid = true;

      if (!P.findTokOnLine(tok::open_paren))
         return nullptr;
   }

   advance();
   auto Pattern = parsePattern();

   P.expect(tok::arrow_double);
   advance();

   if (currentTok().getKind() != tok::open_brace) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                  currentTok().toString(), true, "'{'");

      Invalid = true;

      if (!P.findTokOnLine(tok::open_brace))
         return nullptr;
   }

   SourceLocation ExpansionLoc = currentTok().getSourceLoc();
   advance(false, true);

   unsigned Begin = currentTok().getOffset();
   parseExpansionFragments(ExpansionFragments);

   assert(PatternExpansionStack.empty());
   assert(VariableExpansionStack.empty());
   LastExpansionScopeID = 1;
   MacroVarDecls.clear();

   unsigned End = P.currentTok().getOffset() + P.currentTok().getLength();
   return MacroPattern::Create(SP.getContext(), Loc, Pattern, ExpansionLoc,
                               ExpansionFragments, End - Begin);
}

ParseResult Parser::parseMacro()
{
   auto MacroLoc = consumeToken(Ident_macro);
   if (currentTok().getKind() != tok::ident) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                  currentTok().toString(), true, "identifier");

      if (!findTokOnLine(tok::ident))
         return ParseError();
   }

   DeclarationName MacroName = Context.getDeclNameTable().getMacroName(
       *currentTok().getIdentifierInfo());

   advance();

   tok::TokenType EndTok;
   MacroDecl::Delimiter Delim;
   while (true) {
      switch (currentTok().getKind()) {
      case tok::open_paren:
         Delim = MacroDecl::Paren;
         EndTok = tok::close_paren;
         break;
      case tok::open_brace:
         Delim = MacroDecl::Brace;
         EndTok = tok::close_brace;
         break;
      case tok::open_square:
         Delim = MacroDecl::Square;
         EndTok = tok::close_square;
         break;
      default:
         SP.diagnose(err_macro_delim_must_be, currentTok().getSourceLoc());

         if (!findTokOnLine(tok::open_square, tok::open_paren, tok::open_brace))
            return ParseError();

         continue;
      }

      break;
   }

   advance();

   MacroParser MP(SP, *this);

   llvm::SmallVector<MacroPattern*, 4> Patterns;
   while (!currentTok().is(EndTok)) {
      auto NextPat = MP.parseMacroPattern();
      if (NextPat)
         Patterns.push_back(NextPat);

      if (lookahead().is(tok::eof))
         break;

      advance();

      if (currentTok().oneOf(tok::comma, tok::semicolon))
         advance();
   }

   SourceRange SR(MacroLoc, currentTok().getEndLoc());

   auto Decl = MacroDecl::Create(Context, SR, CurDeclAttrs.Access, MacroName,
                                 Delim, Patterns);

   if (MP.Invalid) {
      Decl->setIsInvalid(true);
   }

   Decl->setAccessLoc(CurDeclAttrs.AccessLoc);
   return ActOnDecl(Decl);
}

ParseResult Parser::parseMacroExpansionExpr(Expression* ParentExpr)
{
   auto BeginLoc = currentTok().getSourceLoc();
   DeclarationName MacroName = Context.getDeclNameTable().getMacroName(
       *currentTok().getIdentifierInfo());

   advance();

   SmallVector<lex::Token, 4> Toks;
   tok::TokenType EndTok = tok::sentinel;
   MacroExpansionExpr::Delimiter Delim = MacroExpansionExpr::Paren;

   // Identifiers can appear before the delimiter.
   while (currentTok().is(tok::ident)) {
      Toks.emplace_back(currentTok());
      advance();
   }

   switch (currentTok().getKind()) {
   case tok::open_paren:
      Delim = MacroExpansionExpr::Paren;
      EndTok = tok::close_paren;
      break;
   case tok::open_square:
      Delim = MacroExpansionExpr::Square;
      EndTok = tok::close_square;
      break;
   case tok::open_brace:
      Delim = MacroExpansionExpr::Brace;
      EndTok = tok::close_brace;
      break;
   default:
      break;
   }

   if (EndTok != tok::sentinel) {
      auto BeginTok = currentTok().getKind();
      advance(false, true);

      unsigned Begin = 1;
      unsigned End = 0;

      while (true) {
         if (currentTok().is(BeginTok)) {
            ++Begin;
         }
         else if (currentTok().is(EndTok)) {
            ++End;
         }

         if (Begin == End)
            break;

         if (lookahead().is(tok::eof)) {
            SP.diagnose(err_unexpected_eof, lookahead().getSourceLoc(), true,
                        tok::tokenTypeToString(EndTok));

            break;
         }

         Toks.push_back(currentTok());
         advance(false, true);
      }
   }

   Toks.emplace_back(tok::eof, currentTok().getSourceLoc());

   SourceRange SR(BeginLoc, currentTok().getEndLoc());
   return MacroExpansionExpr::Create(Context, SR, MacroName, Delim, Toks,
                                     ParentExpr);
}

ParseResult Parser::parseMacroExpansionStmt(Expression* ParentExpr)
{
   auto BeginLoc = currentTok().getSourceLoc();
   DeclarationName MacroName = Context.getDeclNameTable().getMacroName(
       *currentTok().getIdentifierInfo());

   advance();

   SmallVector<lex::Token, 4> Toks;
   tok::TokenType EndTok = tok::sentinel;
   MacroExpansionStmt::Delimiter Delim = MacroExpansionStmt::Paren;

   // Identifiers can appear before the delimiter.
   while (currentTok().is(tok::ident)) {
      Toks.emplace_back(currentTok());
      advance();
   }

   switch (currentTok().getKind()) {
   case tok::open_paren:
      Delim = MacroExpansionStmt::Paren;
      EndTok = tok::close_paren;
      break;
   case tok::open_square:
      Delim = MacroExpansionStmt::Square;
      EndTok = tok::close_square;
      break;
   case tok::open_brace:
      Delim = MacroExpansionStmt::Brace;
      EndTok = tok::close_brace;
      break;
   default:
      break;
   }

   if (EndTok != tok::sentinel) {
      auto BeginTok = currentTok().getKind();
      advance(false, true);

      unsigned Begin = 1;
      unsigned End = 0;

      while (true) {
         if (currentTok().is(BeginTok)) {
            ++Begin;
         }
         else if (currentTok().is(EndTok)) {
            ++End;
         }

         if (Begin == End)
            break;

         if (lookahead().is(tok::eof)) {
            SP.diagnose(err_unexpected_eof, lookahead().getSourceLoc(), true,
                        tok::tokenTypeToString(EndTok));

            break;
         }

         Toks.push_back(currentTok());
         advance(false, true);
      }
   }

   Toks.emplace_back(tok::eof, currentTok().getSourceLoc());

   SourceRange SR(BeginLoc, currentTok().getEndLoc());
   return MacroExpansionStmt::Create(Context, SR, MacroName, ParentExpr, Delim,
                                     Toks);
}

ParseResult Parser::parseMacroExpansionDecl(Expression* ParentExpr)
{
   auto BeginLoc = currentTok().getSourceLoc();
   DeclarationName MacroName = Context.getDeclNameTable().getMacroName(
       *currentTok().getIdentifierInfo());

   advance();

   SmallVector<lex::Token, 4> Toks;
   tok::TokenType EndTok = tok::sentinel;
   MacroExpansionDecl::Delimiter Delim = MacroExpansionDecl::Paren;

   // Identifiers can appear before the delimiter.
   while (currentTok().is(tok::ident)) {
      Toks.emplace_back(currentTok());
      advance();
   }

   switch (currentTok().getKind()) {
   case tok::open_paren:
      Delim = MacroExpansionDecl::Paren;
      EndTok = tok::close_paren;
      break;
   case tok::open_square:
      Delim = MacroExpansionDecl::Square;
      EndTok = tok::close_square;
      break;
   case tok::open_brace:
      Delim = MacroExpansionDecl::Brace;
      EndTok = tok::close_brace;
      break;
   default:
      break;
   }

   if (EndTok != tok::sentinel) {
      auto BeginTok = currentTok().getKind();
      advance(false, true);

      unsigned Begin = 1;
      unsigned End = 0;

      while (true) {
         if (currentTok().is(BeginTok)) {
            ++Begin;
         }
         else if (currentTok().is(EndTok)) {
            ++End;
         }

         if (Begin == End)
            break;

         if (lookahead().is(tok::eof)) {
            SP.diagnose(err_unexpected_eof, lookahead().getSourceLoc(), true,
                        tok::tokenTypeToString(EndTok));

            break;
         }

         Toks.push_back(currentTok());
         advance(false, true);
      }
   }

   Toks.emplace_back(tok::eof, currentTok().getSourceLoc());
   SourceRange SR(BeginLoc, currentTok().getEndLoc());

   auto Decl = MacroExpansionDecl::Create(Context, SR, MacroName, ParentExpr,
                                          Delim, Toks);

   return ActOnDecl(Decl);
}

namespace {

struct MacroVariable {
   enum Kind {
      AstNode,
      TokenTree,
      Identifier,
   };

   explicit MacroVariable(Kind kind) : kind(kind)
   {
      if (kind == AstNode) {
         new (&Values) std::vector<StmtOrDecl>;
      }
      else if (kind == TokenTree) {
         new (&Tokens) std::vector<std::vector<Token>>;
      }
      else if (kind == Identifier) {
         new (&Idents) std::vector<Token>;
      }
   }

   MacroVariable(MacroVariable&& other) noexcept : kind(other.kind)
   {
      if (kind == AstNode) {
         new (&Values) std::vector<StmtOrDecl>(move(other.Values));
      }
      else if (kind == TokenTree) {
         new (&Tokens) std::vector<std::vector<Token>>(move(other.Tokens));
      }
      else if (kind == Identifier) {
         new (&Idents) std::vector<Token>(move(other.Idents));
      }
   }

   MacroVariable& operator=(MacroVariable&& other)
   {
      destroyValue();
      new (this) MacroVariable(move(other));

      return *this;
   }

   ~MacroVariable() { destroyValue(); }

   Kind getKind() const { return kind; }
   std::vector<StmtOrDecl>& getValues()
   {
      assert(kind == AstNode);
      return Values;
   }

   std::vector<std::vector<Token>>& getTokens()
   {
      assert(kind == TokenTree);
      return Tokens;
   }

   std::vector<Token>& getIdents()
   {
      assert(kind == Identifier);
      return Idents;
   }

   size_t size() const
   {
      if (kind == AstNode) {
         return Values.size();
      }
      if (kind == TokenTree) {
         return Tokens.size();
      }
      if (kind == Identifier) {
         return Idents.size();
      }

      llvm_unreachable("bad kind");
   }

private:
   void destroyValue()
   {
      if (kind == AstNode) {
         Values.~vector();
      }
      else if (kind == TokenTree) {
         Tokens.~vector();
      }
      else if (kind == Identifier) {
         Idents.~vector();
      }
   }

   Kind kind;
   union {
      std::vector<StmtOrDecl> Values;
      std::vector<std::vector<Token>> Tokens;
      std::vector<Token> Idents;
   };
};

} // anonymous namespace

using VariableMap = llvm::DenseMap<const IdentifierInfo*, MacroVariable>;

class PatternMatcher {
   Parser& parser;
   VariableMap& VarMap;
   PatternFragment* State;

   struct MatchFailureReason {
      MatchFailureReason(MessageKind msg, SourceLocation PatternLoc,
                         const Token& ExpectedTok1, const Token& ExpectedTok2,
                         const Token& GivenTok)
          : msg(msg), PatternLoc(PatternLoc), ExpectedTok1(ExpectedTok1),
            ExpectedTok2(ExpectedTok2), GivenTok(GivenTok)
      {
      }

      MatchFailureReason() : msg(diag::_first_err) {}

      diag::MessageKind msg;
      SourceLocation PatternLoc;

      Token ExpectedTok1;
      Token ExpectedTok2;
      Token GivenTok;
   };

   /// Reasons for failed pattern matches.
   std::vector<MatchFailureReason> FailureReasons;

   /// Transitions to check.
   SmallVector<StateTransition, 2> TransitionsToCheck;

   bool compatibleTokens(const Token& Given, const Token& Needed);
   bool moveNext();

public:
   PatternMatcher(Parser& parser, VariableMap& VarMap)
       : parser(parser), VarMap(VarMap), State(nullptr)
   {
   }

   bool match(MacroPattern* Pat);
   void diagnose(MacroDecl* D);
};

bool PatternMatcher::match(MacroPattern* Pat)
{
   State = Pat->getPattern();

   while (!State->isEndState()) {
      if (!moveNext()) {
         return false;
      }
   }

   if (State->isErrorState()) {
      FailureReasons.emplace_back();
   }

   return !State->isErrorState();
}

static void printTok(const Token& Tok, llvm::raw_ostream& OS)
{
   if (!Tok)
      return;

   if (Tok.is(tok::ident)) {
      if (Tok.getIdentifierInfo()) {
         OS << "identifier '" << Tok.getIdentifier() << "'";
      }
      else {
         OS << "identifier";
      }
   }
   else if (Tok.is(tok::eof)) {
      OS << "end of file";
   }
   else {
      OS << "'";
      Tok.print(OS);
      OS << "'";
   }
}

void PatternMatcher::diagnose(MacroDecl* D)
{
   assert(FailureReasons.size() == D->getNumPatterns());

   std::string Tok1;
   std::string Tok2;
   std::string GivenTok;

   llvm::raw_string_ostream GivenOS(GivenTok);
   llvm::raw_string_ostream Tok1OS(Tok1);
   llvm::raw_string_ostream Tok2OS(Tok2);

   for (auto& Reason : FailureReasons) {
      if (!Reason.PatternLoc)
         continue;

      switch (Reason.msg) {
      case diag::note_pattern_not_viable_expected_tok: {
         printTok(Reason.GivenTok, GivenOS);
         printTok(Reason.ExpectedTok1, Tok1OS);
         printTok(Reason.ExpectedTok2, Tok2OS);

         if (Reason.ExpectedTok2) {
            parser.SP.diagnose(Reason.msg, Reason.GivenTok.getSourceLoc(),
                               Tok1OS.str(), Tok2OS.str(), GivenOS.str());
         }
         else {
            parser.SP.diagnose(Reason.msg, Reason.GivenTok.getSourceLoc(),
                               Tok1OS.str(), false, GivenOS.str());
         }

         parser.SP.diagnose(diag::note_pattern_here, Reason.PatternLoc);
         break;
      }
      default:
         llvm_unreachable("unexpected message kind!");
      }

      GivenTok.clear();
      Tok1.clear();
      Tok2.clear();
   }
}

bool PatternMatcher::compatibleTokens(const Token& Given, const Token& Needed)
{
   if (Given.getKind() != Needed.getKind())
      return false;

   switch (Needed.getKind()) {
   case tok::ident:
   case tok::op_ident:
      return Given.getIdentifierInfo() == Needed.getIdentifierInfo();
   case tok::stringliteral:
   case tok::integerliteral:
   case tok::fpliteral:
   case tok::charliteral:
      return Given.getText() == Needed.getText();
   default:
      return true;
   }
}

bool PatternMatcher::moveNext()
{
   PatternFragment* NextState = nullptr;
   PatternFragment* UnconditionalTransition = nullptr;
   PatternFragment* VariableTransition = nullptr;

   auto Transitions = State->getTransitions();
   TransitionsToCheck.clear();
   TransitionsToCheck.append(Transitions.begin(), Transitions.end());

   SourceLocation neededLoc;
   lex::Token neededTok1;
   lex::Token neededTok2;

   for (size_t i = 0; i < TransitionsToCheck.size(); ++i) {
      auto &Trans = TransitionsToCheck[i];
      if (!Trans)
         continue;

      if (Trans.isUnconditional()) {
         if (Trans.Next->getKind() == PatternFragment::Variable) {
            VariableTransition = Trans.Next;
         }
         else {
            auto NextTransitions = Trans.Next->getTransitions();
            TransitionsToCheck.append(NextTransitions.begin(),
                                      NextTransitions.end());
         }
      }
      else if (Trans.isEOF() && parser.currentTok().is(tok::eof)) {
         NextState = Trans.Next;
         break;
      }
      else {
         if (compatibleTokens(parser.currentTok(), Trans.Tok)) {
            NextState = Trans.Next;

            if (Trans.IsConsuming) {
               parser.advance();
            }

            break;
         }
         else if (!neededTok1) {
            neededLoc = Trans.Next->getLoc();
            neededTok1 = Trans.Tok;
         }
         else if (!neededTok2) {
            neededTok2 = Trans.Tok;
         }
      }
   }

   if (!NextState) {
      if (VariableTransition) {
         NextState = VariableTransition;
      }
      else if (UnconditionalTransition) {
         NextState = UnconditionalTransition;
      }
      else {
         FailureReasons.emplace_back(
             diag::note_pattern_not_viable_expected_tok, neededLoc,
             neededTok1, neededTok2, parser.currentTok());

         return false;
      }
   }

   // if the next state is a variable, capture it
   if (NextState->getKind() == PatternFragment::Variable) {
      ParseResult Result;
      switch (NextState->getVarKind()) {
      case PatternFragment::Expr:
         Result = parser.parseExprSequence();
         break;
      case PatternFragment::Stmt:
         Result = parser.parseNextStmt();
         break;
      case PatternFragment::Decl:
         Result = parser.parseNextDecl();
         break;
      case PatternFragment::Ident: {
         if (!parser.currentTok().oneOf(tok::ident, tok::op_ident)) {
            FailureReasons.emplace_back(
                diag::note_pattern_not_viable_expected_tok, NextState->getLoc(),
                Token(tok::ident), Token(), parser.currentTok());

            return false;
         }

         auto It = VarMap.find(NextState->getVariableName());
         if (It == VarMap.end()) {
            It = VarMap
                     .try_emplace(NextState->getVariableName(),
                                  MacroVariable::Identifier)
                     .first;
         }

         It->getSecond().getIdents().push_back(parser.currentTok());
         parser.advance();

         State = NextState;
         return true;
      }
      case PatternFragment::Type: {
         auto TypeResult = parser.parseType();
         if (TypeResult)
            Result = TypeResult.get().getTypeExpr();

         break;
      }
      case PatternFragment::Tok: {
         if (parser.currentTok().is(tok::eof)) {
            FailureReasons.emplace_back();
            return false;
         }

         auto It = VarMap.find(NextState->getVariableName());
         if (It == VarMap.end()) {
            It = VarMap
                     .try_emplace(NextState->getVariableName(),
                                  MacroVariable::TokenTree)
                     .first;
         }

         auto& Toks = It->getSecond().getTokens().emplace_back();

         Toks.push_back(parser.currentTok());
         parser.advance();

         State = NextState;
         return true;
      }
      case PatternFragment::Any: {
         // get the first token that follows this captured token sequence,
         // pattern parsing should have verified that this is not another
         // variable
         lex::Token NextTok1;
         lex::Token NextTok2;

         for (auto& Trans : NextState->getTransitions()) {
            if (!Trans.isUnconditional()) {
               if (NextTok1.getKind() == tok::sentinel) {
                  NextTok1 = Trans.Tok;
               }
               else {
                  NextTok2 = Trans.Tok;
               }
            }
         }

         assert((NextTok1.getKind() != tok::sentinel
                 || NextTok2.getKind() != tok::sentinel)
                && "no seperator token!");

         auto It = VarMap.find(NextState->getVariableName());
         if (It == VarMap.end()) {
            It = VarMap
                     .try_emplace(NextState->getVariableName(),
                                  MacroVariable::TokenTree)
                     .first;
         }

         auto& Toks = It->getSecond().getTokens().emplace_back();
         while (!compatibleTokens(parser.currentTok(), NextTok1)
                && !compatibleTokens(parser.currentTok(), NextTok2)
                && !parser.currentTok().is(tok::eof)) {
            Toks.push_back(parser.currentTok());
            parser.advance(false, true);
         }

         State = NextState;
         return true;
      }
      default:
         llvm_unreachable("unimplemented!");
      }

      if (!Result) {
         FailureReasons.emplace_back();
         return false;
      }

      parser.advance();

      StmtOrDecl SOD;
      if (Result.holdsDecl()) {
         SOD = Result.getDecl();
      }
      else if (Result.holdsExpr()) {
         SOD = Result.getExpr();
      }
      else {
         SOD = Result.getStatement();
      }

      auto It = VarMap.find(NextState->getVariableName());
      if (It == VarMap.end()) {
         It = VarMap
                  .try_emplace(NextState->getVariableName(),
                               MacroVariable::AstNode)
                  .first;
      }

      It->getSecond().getValues().push_back(SOD);
   }
   else if (NextState->getKind() == PatternFragment::Tokens) {
      for (auto& Tok : NextState->getTokens().drop_front(1)) {
         if (!compatibleTokens(parser.currentTok(), Tok)) {
            FailureReasons.emplace_back(
                diag::note_pattern_not_viable_expected_tok, NextState->getLoc(),
                Tok, Token(), parser.currentTok());

            return false;
         }

         parser.advance();
      }
   }

   State = NextState;
   return true;
}

ParseResult Parser::parseWithKind(SourceLocation Loc, ExpansionKind Kind, bool)
{
   ParseResult Result;
   switch (Kind) {
   case Parser::ExpansionKind::Expr:
      Result = parseExprSequence();

      advance();
      if (!currentTok().is(tok::eof))
         SP.diagnose(err_leftover_tokens_after_parsing, Loc, (int)Kind);

      break;
   case Parser::ExpansionKind::Stmt: {
      Result = parseStmts();

      if (Result.holdsDecl()) {
         Result = DeclStmt::Create(Context, Result.getDecl());
      }

      break;
   }
   case Parser::ExpansionKind::Decl:
      parseDecls(isa<RecordDecl>(SP.getDeclContext())
                 || isa<ExtensionDecl>(SP.getDeclContext()));
      break;
   case Parser::ExpansionKind::Type: {
      auto TypeResult = parseType();
      if (TypeResult)
         Result = TypeResult.get().getTypeExpr();

      break;
   }
   }

   return Result;
}

class MacroExpander {
   SemaPass& SP;
   fs::FileManager &FileMgr;
   MacroPattern* Pat;
   VariableMap& VarMap;
   Parser::ExpansionKind Kind;
   MacroDecl* M;

   /// The source location where this macro was expanded.
   SourceLocation ExpandedFrom;

   /// The base offset of the macro pattern.
   unsigned BaseOffset;

   /// The assigned artificial offset for this macro expansion.
   unsigned ExpansionOffset;

   /// Set to true if we encounter a builtin macro that needs expansion.
   bool FoundBuiltinMacro = false;

   /// The counter values mapped by index.
   llvm::DenseMap<uint64_t, uint64_t> CounterValues;

   /// The unique names mapped by index.
   llvm::DenseMap<uint64_t, DeclarationName> UniqueNames;

   /// The last assigned unique name ID.
   static uint64_t LastUniqueNameID;

   enum class BuiltinMacro {
      none,

      // Utility
      stringify,
      counter,
      unique_name,

      // Text inclusion
      include_str,
      include,

      // Clang inclusion
      include_c,
      include_cxx,
      include_system_header,
   };

   bool expandInto(ExpansionFragment* Frag, llvm::SmallVectorImpl<Token>& Vec,
                   unsigned idx = 0, unsigned NumRepetitions = 0,
                   const IdentifierInfo* ExpandedVarName = nullptr,
                   SourceLocation ExpandedVarLoc = SourceLocation());

   BuiltinMacro getBuiltinMacroKind(const Token& Tok);
   void checkBuiltinMacros(SmallVectorImpl<Token>& Vec);
   bool checkBuiltinMacro(StringRef MacroName,
                          ArrayRef<lex::Token> ExpansionToks, unsigned& i,
                          SmallVectorImpl<Token>& Vec, SourceLocation Loc);

   SourceLocation makeSourceLoc(SourceLocation Loc)
   {
      return SourceLocation(ExpansionOffset + (Loc.getOffset() - BaseOffset));
   }

public:
   MacroExpander(SemaPass& SP, MacroPattern* Pat, VariableMap& VarMap,
                 Parser::ExpansionKind Kind, SourceLocation ExpandedFrom,
                 MacroDecl* M)
       : SP(SP), FileMgr(SP.getCompilerInstance().getFileMgr()),
         Pat(Pat), VarMap(VarMap), Kind(Kind), M(M),
         ExpandedFrom(ExpandedFrom)
   {
      BaseOffset = Pat->getSourceLoc().getOffset();
      ExpansionOffset
          = FileMgr
                .createMacroExpansion(ExpandedFrom, Pat->getSourceLoc(),
                                      Pat->getSourceLength() + 1,
                                      this->M->getDeclName().getMacroName())
                .BaseOffset;
   }

   ParseResult expand();
};

uint64_t MacroExpander::LastUniqueNameID = 0;

bool MacroExpander::expandInto(ExpansionFragment* Frag,
                               SmallVectorImpl<Token>& Vec, unsigned idx,
                               unsigned NumRepetitions,
                               const IdentifierInfo* ExpandedVarName,
                               SourceLocation ExpandedVarLoc)
{
   switch (Frag->getKind()) {
   case ExpansionFragment::Tokens: {
      for (auto& Tok : Frag->getTokens()) {
         FoundBuiltinMacro |= getBuiltinMacroKind(Tok) != BuiltinMacro::none;
         Vec.emplace_back(Tok, makeSourceLoc(Tok.getSourceLoc()));
      }

      break;
   }
   case ExpansionFragment::Variable: {
      auto It = VarMap.find(Frag->getVariableName());
      assert(It != VarMap.end() && "variable not captured!");

      auto& Var = It->getSecond();
      if (NumRepetitions && Var.size() != 1 && NumRepetitions != 1
          && Var.size() != NumRepetitions) {
         SP.diagnose(err_multiple_expanded_macro_variables,
                     Frag->getVariableName()->getIdentifier(),
                     ExpandedVarName->getIdentifier(), NumRepetitions,
                     Var.size(), Frag->getLoc(), ExpandedVarLoc);

         return false;
      }

      if (Var.size() == 1) {
         idx = 0;
      }

      assert(Var.size() > idx && "index out of bounds");
      switch (Var.getKind()) {
      case MacroVariable::AstNode: {
         auto& SOD = Var.getValues()[idx];
         if (auto Stmt = SOD.asStatement()) {
            if (auto Expr = dyn_cast<Expression>(Stmt)) {
               Vec.emplace_back(Expr, makeSourceLoc(Frag->getLoc()));
            }
            else {
               Vec.emplace_back(Stmt, makeSourceLoc(Frag->getLoc()));
            }
         }
         else {
            Vec.emplace_back(SOD.getDecl(), makeSourceLoc(Frag->getLoc()));
         }

         break;
      }
      case MacroVariable::TokenTree: {
         auto& Toks = Var.getTokens()[idx];
         auto VarLoc = makeSourceLoc(Frag->getLoc());

         for (auto& Tok : Toks)
            Vec.emplace_back(Tok, VarLoc);

         break;
      }
      case MacroVariable::Identifier: {
         auto VarLoc = makeSourceLoc(Frag->getLoc());

         // concatenate immediately adjacent identifiers
         if (!Vec.empty() && Vec.back().oneOf(tok::ident, tok::op_ident)) {
            auto LastTok = Vec.back();

            auto concat = LastTok.getIdentifier().str();
            concat += Var.getIdents()[idx].getIdentifier().str();

            auto* II = &SP.getContext().getIdentifiers().get(concat);
            Vec.pop_back();

            Vec.emplace_back(II, VarLoc, LastTok.getKind());
         }
         else {
            auto& Tok = Var.getIdents()[idx];
            Vec.emplace_back(Tok, VarLoc);
         }

         break;
      }
      }

      break;
   }
   case ExpansionFragment::Expansion: {
      auto ExpandedVarName = Frag->getExpandedVariable();
      auto It = VarMap.find(ExpandedVarName);
      assert(It != VarMap.end() && "variable not captured!");

      auto& Var = It->getSecond();
      unsigned NumValues = (unsigned)Var.size();

      for (unsigned i = 0; i < NumValues; ++i) {
         for (auto& ExpFrag : Frag->getRepetitionFragments()) {
            if (!expandInto(ExpFrag, Vec, i, NumValues,
                            Frag->getExpandedVariable(), Frag->getLoc())) {
               return false;
            }
         }
      }

      break;
   }
   case ExpansionFragment::ConcatExpr: {
      llvm::SmallVector<Token, 8> Toks;
      for (auto& ExpFrag : Frag->getRepetitionFragments()) {
         if (!expandInto(ExpFrag, Toks, idx, NumRepetitions, ExpandedVarName,
                         ExpandedVarLoc)) {
            return false;
         }
      }

      // stringify tokens
      std::string str;
      llvm::raw_string_ostream OS(str);

      for (auto& Tok : Toks) {
         if (Tok.oneOf(tok::space, tok::newline))
            continue;

         OS << Tok;
      }

      // lex back into tokens
      auto Buf = llvm::MemoryBuffer::getMemBuffer(OS.str());
      Lexer Lex(SP.getContext().getIdentifiers(), SP.getDiags(), Buf.get(),
                FileMgr.getSourceId(Frag->getLoc()),
                Frag->getLoc().getOffset());

      while (!Lex.currentTok().is(tok::eof)) {
         auto Tok = Lex.currentTok();
         Vec.emplace_back(Tok, makeSourceLoc(Tok.getSourceLoc()));
         Lex.advance();
      }

      break;
   }
   }

   return true;
}

void MacroExpander::checkBuiltinMacros(SmallVectorImpl<Token>& Toks)
{
   SmallVector<Token, 16> NewVec;

   unsigned NumTokens = Toks.size();
   for (unsigned i = 0; i < NumTokens; ++i) {
      auto& Tok = Toks[i];

      // Expand builtin macros.
      if (Tok.is(tok::macro_name)) {
         if (checkBuiltinMacro(Tok.getIdentifier(), Toks, i, NewVec,
                               Tok.getSourceLoc())) {
            continue;
         }
      }

      NewVec.emplace_back(Tok);
   }

   Toks.clear();
   Toks.append(NewVec.begin(), NewVec.end());
}

MacroExpander::BuiltinMacro MacroExpander::getBuiltinMacroKind(const Token& Tok)
{
   if (!Tok.is(tok::macro_name)) {
      return BuiltinMacro::none;
   }

   return StringSwitch<BuiltinMacro>(Tok.getIdentifier())
       .Case("stringify", BuiltinMacro::stringify)
       .Case("counter", BuiltinMacro::counter)
       .Case("include_str", BuiltinMacro::include_str)
       .Case("include", BuiltinMacro::include)
       .Case("include_c", BuiltinMacro::include_c)
       .Case("include_cxx", BuiltinMacro::include_cxx)
       .Case("include_system_header", BuiltinMacro::include_system_header)
       .Default(BuiltinMacro::none);
}

bool MacroExpander::checkBuiltinMacro(StringRef MacroName,
                                      ArrayRef<Token> ExpansionToks,
                                      unsigned& i, SmallVectorImpl<Token>& Vec,
                                      SourceLocation Loc)
{
   // Only allow parentheses as delimiters for builtin macros.
   if (!ExpansionToks[i + 1].is(tok::open_paren)) {
      return false;
   }

   auto MacroKind = getBuiltinMacroKind(ExpansionToks[i]);
   if (MacroKind == BuiltinMacro::none) {
      return false;
   }

   unsigned TokensStart = i++;
   unsigned OpenParens = 0;
   unsigned CloseParens = 0;

   while (true) {
      switch (ExpansionToks[i].getKind()) {
      case tok::open_paren:
         ++OpenParens;
         break;
      case tok::close_paren:
         ++CloseParens;
         break;
      case tok::eof:
         SP.diagnose(err_unexpected_eof, true, ExpansionToks[i].getSourceLoc());
         return true;
      default:
         break;
      }

      if (OpenParens == CloseParens) {
         break;
      }

      ++i;
   }

   unsigned TokensEnd = i;
   ArrayRef<Token> Tokens = ExpansionToks.drop_front(TokensStart + 2)
                                .drop_back(ExpansionToks.size() - TokensEnd);

   switch (MacroKind) {
   case BuiltinMacro::none:
      return false;
   case BuiltinMacro::stringify: {
      std::string str;
      {
         llvm::raw_string_ostream OS(str);
         for (auto& Tok : Tokens) {
            switch (Tok.getKind()) {
            case tok::charliteral:
               OS << "'" << Tok.getText() << "'";
               break;
            case tok::stringliteral:
               OS << '"' << Tok.getText() << '"';
               break;
            case tok::macro_expression:
               SP.diagnose(err_macro_argument_cannot_be_stringified,
                           "expression",
                           Tok.getSourceLoc());

               break;
            case tok::macro_statement:
               SP.diagnose(err_macro_argument_cannot_be_stringified,
                           "statement",
                           Tok.getSourceLoc());

               break;
            case tok::macro_declaration:
               SP.diagnose(err_macro_argument_cannot_be_stringified,
                           "declaration",
                           Tok.getSourceLoc());

               break;
            default:
               OS << Tok;
               break;
            }
         }
      }

      auto Alloc = (char*)SP.getContext().Allocate(str.size());
      std::copy(str.begin(), str.end(), Alloc);

      SourceLocation EndLoc = Tokens.empty() ? Loc : Tokens.back().getEndLoc();
      SourceRange SR(Loc, EndLoc);

      auto* S = StringLiteral::Create(SP.Context, SR, string(Alloc, str.size()));
      Vec.emplace_back(S, Loc);

      return true;
   }
   case BuiltinMacro::counter: {
      uint64_t counterIndex;
      if (Tokens.size() == 1 && Tokens.front().is(tok::integerliteral)) {
         auto Txt = Tokens.front().getText();
         auto Val = llvm::APSInt(Txt);
         counterIndex = Val.getZExtValue();
      }
      else if (Tokens.empty()) {
         counterIndex = 0;
      }
      else {
         SP.diagnose(
             err_counter_expects,
             Tokens.front().getSourceLoc());

         counterIndex = 0;
      }

      llvm::APSInt Val(llvm::APInt(64, CounterValues[counterIndex]++, true),
                       false);

      auto* I = IntegerLiteral::Create(SP.Context, Loc, SP.Context.getIntTy(),
                                       move(Val));

      Vec.emplace_back(I, Loc);
      return true;
   }
   case BuiltinMacro::unique_name: {
      uint64_t nameIndex;
      if (Tokens.size() == 1 && Tokens.front().is(tok::integerliteral)) {
         auto Txt = Tokens.front().getText();
         auto Val = llvm::APSInt(Txt);
         nameIndex = Val.getZExtValue();
      }
      else if (Tokens.empty()) {
         nameIndex = 0;
      }
      else {
         SP.diagnose(err_unique_name_expects, Tokens.front().getSourceLoc());
         nameIndex = 0;
      }

      llvm_unreachable("figure out how to do this!");
   }
   case BuiltinMacro::include:
   case BuiltinMacro::include_str: {
      if (Tokens.size() != 1 || !Tokens.front().is(tok::stringliteral)) {
         SP.diagnose(err_unexpected_token, Tokens.front(), true, "string literal");
         return true;
      }

      auto& FileMgr = *SP.getDiags().getFileMgr();

      auto FileName = Tokens.front().getText();
      std::string realFile;

      if (!FileName.starts_with(StringRef(&fs::PathSeperator, 1))) {
         auto includeDirs
             = SP.getCompilerInstance().getOptions().getIncludeDirs().vec();

         includeDirs.push_back(fs::getPath(FileMgr.getFileName(Loc).str()).str());

         auto Path = fs::getPath(FileName);
         if (!Path.empty()) {
            for (auto& Dir : includeDirs) {
               fs::appendToPath(Dir, Path);
            }

            FileName = fs::getFileNameAndExtension(FileName);
         }

         realFile = fs::findFileInDirectories(FileName, includeDirs);
      }
      else if (fs::fileExists(FileName)) {
         realFile = FileName;
      }

      if (realFile.empty()) {
         SP.diagnose(err_cannot_open_file, FileName, true, "not found",
                     Tokens.front().getSourceLoc());

         return true;
      }

      auto File = FileMgr.openFile(realFile);
      if (MacroKind == BuiltinMacro::include_str) {
         auto S = StringLiteral::Create(
             SP.getContext(), Loc,
             std::string(File.Buf->getBufferStart(), File.Buf->getBufferSize()));

         Vec.emplace_back(S, Loc);
         return true;
      }

      Lexer lexer(SP.getContext().getIdentifiers(), SP.getDiags(), File.Buf,
                  File.SourceId, File.BaseOffset);

      while (!lexer.eof()) {
         Vec.emplace_back(lexer.currentTok());
         lexer.advance();
      }

      return true;
   }
   case BuiltinMacro::include_c:
   case BuiltinMacro::include_cxx:
   case BuiltinMacro::include_system_header: {
      ClangImporter& Importer = SP.getCompilerInstance().getClangImporter();

      if (Tokens.size() != 1 || !Tokens.front().is(tok::stringliteral)) {
         SP.diagnose(err_unexpected_token, Tokens.front(), true, "string literal");
         return true;
      }

      auto FileName = Tokens.front().getText();
      if (MacroKind == BuiltinMacro::include_c) {
         Importer.importCModule(FileName, &SP.getDeclContext(), Loc);
      }
      else if (MacroKind == BuiltinMacro::include_system_header) {
         Importer.importSystemHeader(FileName, &SP.getDeclContext(), Loc);
      }
      else {
         Importer.importCXXModule(FileName, &SP.getDeclContext(), Loc);
      }

      return true;
   }
   }
}

ParseResult MacroExpander::expand()
{
   SmallVector<Token, 0> Tokens;
   for (auto& Frag : Pat->getExpansion()) {
      if (!expandInto(Frag, Tokens)) {
         return ParseError();
      }
   }

   if (FoundBuiltinMacro) {
      checkBuiltinMacros(Tokens);
   }

   unsigned SourceID
       = SP.getCompilerInstance().getFileMgr().getSourceId(ExpandedFrom);

   lex::Lexer Lexer(SP.getContext().getIdentifiers(), SP.getDiags(), Tokens,
                    SourceID);

   parse::Parser parser(SP.getContext(), &Lexer, SP);
   return parser.parseWithKind(ExpandedFrom, Kind);
}

ParseResult Parser::expandMacro(SemaPass& SP, MacroDecl* Macro, StmtOrDecl SOD,
                                ArrayRef<Token> Tokens, ExpansionKind Kind)
{
   if (Macro->isInvalid()) {
      return ParseError();
   }

   auto& Context = SP.getContext();

   VariableMap VarMap;
   MacroPattern* Match = nullptr;

   lex::Lexer Lexer(Context.getIdentifiers(), SP.getDiags(), Tokens, 0);
   parse::Parser Parser(Context, &Lexer, SP);

   PatternMatcher Matcher(Parser, VarMap);
   for (auto& Pat : Macro->getPatterns()) {
      Parser.skipWhitespace();

      if (Matcher.match(Pat)) {
         Match = Pat;
         break;
      }

      VarMap.clear();
      Lexer.reset(Tokens);
   }

   if (!Match) {
      SP.diagnose(SOD, err_could_not_match_pattern, Macro->getDeclName(),
                  SOD.getSourceRange());

      Matcher.diagnose(Macro);
      return ParseError();
   }

   return MacroExpander(SP, Match, VarMap, Kind, SOD.getSourceLoc(), Macro)
       .expand();
}

} // namespace parse
} // namespace cdot