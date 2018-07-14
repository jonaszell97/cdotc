//
// Created by Jonas Zell on 19.04.18.
//

#include "Parser.h"

#include "Basic/FileUtils.h"
#include "Lex/Lexer.h"
#include "Sema/SemaPass.h"
#include "Support/StringSwitch.h"

#include <llvm/ADT/SmallString.h>

using namespace cdot::lex;
using namespace cdot::diag;
using namespace cdot::support;

namespace cdot {
namespace parse {

class MacroParser {
   SemaPass &SP;
   Parser &P;

   // macro parsing
   struct PatternExpansionRAII {
      PatternExpansionRAII(MacroParser &P)
         : P(P)
      {
         P.PatternExpansionStack.emplace_back();
         ++P.LastExpansionScopeID;
      }

      ~PatternExpansionRAII()
      {
         P.PatternExpansionStack.pop_back();
      }

   private:
      MacroParser &P;
   };

   struct VariableExpansionRAII {
      VariableExpansionRAII(MacroParser &P)
         : P(P)
      {
         P.VariableExpansionStack.emplace_back();
      }

      ~VariableExpansionRAII()
      {
         P.VariableExpansionStack.pop_back();
      }

   private:
      MacroParser &P;
   };

   unsigned LastExpansionScopeID = 1;

   using ExpansionStackTy = std::vector<
      llvm::SmallPtrSet<const IdentifierInfo*, 9>>;

   ExpansionStackTy PatternExpansionStack;
   ExpansionStackTy VariableExpansionStack;

   struct PatternVariableDeclaration {
      unsigned ExpansionScopeID = 0;
   };

   llvm::DenseMap<const IdentifierInfo*, PatternVariableDeclaration>
      MacroVarDecls;

   const lex::Token &currentTok() const
   {
      return P.currentTok();
   }

   Token lookahead() const
   {
      return P.lookahead();
   }

   void advance(bool ignoreNewline = true, bool whitespace = false) const
   {
      return P.advance(ignoreNewline, whitespace);
   }

   void backtrack() const
   {
      return P.lexer->backtrack();
   }

public:
   MacroParser(SemaPass &SP, Parser &P)
      : SP(SP), P(P)
   { }

   PatternFragment *parsePattern();
   PatternFragment *parseInnerPattern(PatternFragment *Previous,
                                      bool SetErrorState = true);

   void parseExpansionFragment(llvm::SmallVectorImpl<ExpansionFragment*> &Vec);
   void parseExpansionFragments(llvm::SmallVectorImpl<ExpansionFragment*> &Vec);

   MacroPattern *parseMacroPattern();
};

PatternFragment *MacroParser::parsePattern()
{
   auto Begin = PatternFragment::Create(SP.getContext());
   auto Penultimate = parseInnerPattern(Begin);

   auto End = PatternFragment::Create(SP.getContext());
   Penultimate->addTransition(Token(tok::eof), End);

   return Begin;
}

static void diagnoseInvalidSeperator(SemaPass &SP,
                                     PatternFragment *Previous,
                                     const lex::Token &Tok) {
   if (!Previous)
      return;

   bool DoCheck = false;
   if (Previous->isVariable()) {
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
      return;

   bool Valid = false;
   switch (Tok.getKind()) {
   case tok::comma: case tok::semicolon: case tok::close_paren:
   case tok::newline: case tok::eof: case tok::sentinel:
   case tok::close_brace: case tok::close_square:
   case tok::arrow_double: case tok::arrow_single:
   case tok::kw_def: case tok::kw_let:
   case tok::kw_var: case tok::kw_return: case tok::kw_else:
   case tok::kw_while: case tok::kw_loop: case tok::kw_match:
   case tok::kw_for: case tok::kw_case: case tok::kw_throw:
   case tok::kw_try: case tok::kw_catch: case tok::kw_finally:
   case tok::kw_alias: case tok::kw_typedef: case tok::kw_namespace:
   case tok::kw_struct: case tok::kw_class: case tok::kw_union:
   case tok::kw_enum: case tok::kw_protocol: case tok::kw_extend:
   case tok::kw_public: case tok::kw_private:
   case tok::kw_protected: case tok::kw_static: case tok::kw_abstract:
   case tok::kw_prop: case tok::kw_where:
   case tok::kw_continue: case tok::kw_init:
   case tok::kw_associatedType: case tok::kw_break: case tok::kw_goto:
   case tok::kw_infix: case tok::kw_prefix: case tok::kw_postfix:
   case tok::kw_mutating: case tok::kw_declare: case tok::kw_module:
   case tok::kw_import: case tok::kw_using:
   case tok::kw_static_if: case tok::kw_static_for:
   case tok::kw_static_assert: case tok::kw_static_print:
   case tok::kw___debug: case tok::kw___unreachable:
   case tok::interpolation_begin: case tok::interpolation_end:
      Valid = true;
      break;
   default:
      break;
   }

   if (!Valid) {
      SP.diagnose(err_invalid_seperator_token, Tok.getSourceLoc(),
                  (int)Previous->getVarKind());
   }
}

PatternFragment *MacroParser::parseInnerPattern(PatternFragment *Previous,
                                                bool SetErrorState) {
   bool done = false;
   unsigned OpenParens = 1;
   PatternFragment *FragmentForSeperatorCheck = nullptr;

   while (true) {
      SourceLocation Loc = currentTok().getSourceLoc();

      switch (currentTok().getKind()) {
      default: {
         llvm::SmallVector<Token, 2> Tokens;

         unsigned InnerOpenBraces = 0;
         unsigned InnerOpenSquare = 0;
         bool innerDone = false;

         diagnoseInvalidSeperator(SP, FragmentForSeperatorCheck, currentTok());

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
            case tok::dollar_ident: case tok::dollar:
               innerDone = true;
               backtrack();
               break;
            case tok::backslash:
               advance();
               break;
            default:
               break;
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
               }
            }
         }

         auto It = MacroVarDecls.find(VarName);
         if (It != MacroVarDecls.end()) {
            SP.diagnose(err_macro_var_redeclaration, VarName->getIdentifier(),
                        currentTok().getSourceLoc());
         }

         if (PatternExpansionStack.empty()) {
            MacroVarDecls[VarName] = PatternVariableDeclaration{ 0 };
         }
         else {
            MacroVarDecls[VarName] =
               PatternVariableDeclaration{ LastExpansionScopeID };
            PatternExpansionStack.back().insert(VarName);
         }

         auto Next = PatternFragment::Create(SP.getContext(), Loc, VarKind,
                                             VarName);

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
         }

         PatternExpansionRAII patternExpansionRAII(*this);

         advance();
         auto LastInRepetition = parseInnerPattern(Previous, false);

         PatternFragment::FragmentKind Kind = PatternFragment::Star;
         bool GotKind = false;
         lex::Token Delim;

         advance();

         while (true) {
            switch (currentTok().getKind()) {
            case tok::times:
               GotKind = true;
               Kind = PatternFragment::Star;
               break;
            case tok::plus:
               GotKind = true;
               Kind = PatternFragment::Plus;
               break;
            case tok::question:
               GotKind = true;
               Kind = PatternFragment::Question;
               break;
            default:
               if (Delim.getKind() != tok::sentinel
                     || currentTok().oneOf(tok::close_paren, tok::close_brace,
                                           tok::close_square)) {
                  SP.diagnose(err_expected_after_repetition,
                              currentTok().getSourceLoc());

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

         auto Merge = PatternFragment::Create(SP.getContext());
         switch (Kind) {
         case PatternFragment::Star: {
            // if the repetition can happen zero times (as with '*' and '?')
            // add a default transition to the merge state
            Previous->addTransition(Token(), Merge);

            // if there's a delimiter, add a transition for it
            if (Delim.getKind() != tok::sentinel) {
               LastInRepetition->addTransition(Delim, Previous);
               LastInRepetition->addTransition(Token(), Merge);
            }
            // otherwise, only eof gets you to the end
            else {
               LastInRepetition->addTransition(Token(), Previous);
               LastInRepetition->addTransition(Token(tok::eof), Merge);
            }

            break;
         }
         case PatternFragment::Plus: {
            // has to happen at least once, transition to error if not
            Previous->addTransition(Token(), PatternFragment::GetErrorState());

            // create a new state with the same transitions as LastInRep,
            // except that now it works like the '*' qualfiier
            auto ZeroOrMoreState = PatternFragment::Create(SP.getContext());
            LastInRepetition->addTransition(Token(), ZeroOrMoreState);

            // after one successful iteration, go either to merge or back to
            // the beginning
            if (Delim.getKind() != tok::sentinel) {
               ZeroOrMoreState->addTransition(Delim, Previous);
               ZeroOrMoreState->addTransition(Token(), Merge);
            }
               // otherwise, only eof gets you to the end
            else {
               LastInRepetition->addTransition(Token(), Previous);
               LastInRepetition->addTransition(Token(tok::eof), Merge);
            }

            break;
         }
         case PatternFragment::Question: {
            // if the repetition can happen zero times (as with '*' and '?')
            // add a default transition to the merge state
            Previous->addTransition(Token(), Merge);

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
                              llvm::SmallVectorImpl<ExpansionFragment*> &Vec) {
   SourceLocation Loc = currentTok().getSourceLoc();

   switch (currentTok().getKind()) {
   case tok::dollar_ident: {
      // variable expansion
      auto VarName = currentTok().getIdentifierInfo();
      auto It = MacroVarDecls.find(VarName);
      if (It == MacroVarDecls.end()) {
         SP.diagnose(err_macro_undeclared_variable, VarName->getIdentifier(),
                     currentTok().getSourceLoc());
      }
      else if (It->getSecond().ExpansionScopeID != 0
               && VariableExpansionStack.empty()) {
         SP.diagnose(err_macro_variable_unexpanded, VarName->getIdentifier(),
                     currentTok().getSourceLoc());
      }
      else if (!VariableExpansionStack.empty()) {
         auto &back = VariableExpansionStack.back();
         back.insert(VarName);
      }

      Vec.push_back(ExpansionFragment::Create(SP.getContext(), Loc, VarName));

      break;
   }
   case tok::dollar: {
      VariableExpansionRAII variableExpansionRAII(*this);

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
      }

      advance();
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
      }

      const IdentifierInfo *Fst = nullptr;

      auto &Var = VariableExpansionStack.back();
      if (Var.empty()) {
         SP.diagnose(err_expansion_without_variable,
                     currentTok().getSourceLoc());
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
         case tok::dollar_ident: case tok::dollar:
            if (!Tokens.empty())
               Vec.push_back(ExpansionFragment::Create(SP.getContext(), Loc,
                                                       Tokens));

            Tokens.clear();
            parseExpansionFragment(Vec);

            advance();
            continue;
         case tok::backslash:
            advance();
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
         Vec.push_back(ExpansionFragment::Create(SP.getContext(), Loc,
                                                 Tokens));

      break;
   }
   }
}

void MacroParser::parseExpansionFragments(
                              llvm::SmallVectorImpl<ExpansionFragment*> &Vec) {
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

      if (!P.findTokOnLine(tok::open_brace))
         return nullptr;
   }

   advance(false, true);

   unsigned Begin = currentTok().getOffset();
   parseExpansionFragments(ExpansionFragments);

   assert(PatternExpansionStack.empty());
   assert(VariableExpansionStack.empty());
   LastExpansionScopeID = 1;
   MacroVarDecls.clear();

   unsigned End = P.currentTok().getOffset() + P.currentTok().getLength();
   return MacroPattern::Create(SP.getContext(), Loc,
                               Pattern, ExpansionFragments,
                               End - Begin);
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

   DeclarationName MacroName =
      Context.getDeclNameTable()
             .getMacroName(*currentTok().getIdentifierInfo());

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

   auto Decl = MacroDecl::Create(Context, SR, CurDeclAttrs.Access,
                                 MacroName, Delim, Patterns);

   Decl->setAccessLoc(CurDeclAttrs.AccessLoc);
   SP.addDeclToContext(SP.getDeclContext(), Decl);

   return Decl;
}

ParseResult Parser::parseMacroExpansionExpr(Expression *ParentExpr)
{
   auto BeginLoc = currentTok().getSourceLoc();
   DeclarationName MacroName =
      Context.getDeclNameTable()
             .getMacroName(*currentTok().getIdentifierInfo());

   advance();

   tok::TokenType EndTok = tok::sentinel;
   MacroExpansionExpr::Delimiter Delim = MacroExpansionExpr::Paren;

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

   llvm::SmallVector<lex::Token, 4> Toks;
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

   Toks.emplace_back(tok::eof);

   SourceRange SR(BeginLoc, currentTok().getEndLoc());
   return MacroExpansionExpr::Create(Context, SR, MacroName, Delim, Toks,
                                     ParentExpr);
}

ParseResult Parser::parseMacroExpansionStmt()
{
   auto BeginLoc = currentTok().getSourceLoc();
   DeclarationName MacroName =
      Context.getDeclNameTable()
             .getMacroName(*currentTok().getIdentifierInfo());

   advance();

   tok::TokenType EndTok = tok::sentinel;
   MacroExpansionStmt::Delimiter Delim = MacroExpansionStmt::Paren;

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

   llvm::SmallVector<lex::Token, 4> Toks;
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

   Toks.emplace_back(tok::eof);

   SourceRange SR(BeginLoc, currentTok().getEndLoc());
   return MacroExpansionStmt::Create(Context, SR, MacroName, Delim, Toks);
}

ParseResult Parser::parseMacroExpansionDecl()
{
   auto BeginLoc = currentTok().getSourceLoc();
   DeclarationName MacroName =
      Context.getDeclNameTable()
             .getMacroName(*currentTok().getIdentifierInfo());

   advance();

   tok::TokenType EndTok = tok::sentinel;
   MacroExpansionDecl::Delimiter Delim = MacroExpansionDecl::Paren;

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

   llvm::SmallVector<lex::Token, 4> Toks;
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

   Toks.emplace_back(tok::eof);
   SourceRange SR(BeginLoc, currentTok().getEndLoc());

   auto Decl = MacroExpansionDecl::Create(Context, SR, MacroName, Delim, Toks);
   return ActOnDecl(Decl);
}

namespace {

struct MacroVariable {
   enum Kind {
      AstNode, TokenTree, Identifier,
   };

   explicit MacroVariable(Kind kind)
      : kind(kind)
   {
      if (kind == AstNode) {
         new(&Values) std::vector<StmtOrDecl>;
      }
      else if (kind == TokenTree) {
         new(&Tokens) std::vector<std::vector<Token>>;
      }
      else if (kind == Identifier) {
         new(&Idents) std::vector<Token>;
      }
   }

   MacroVariable(MacroVariable &&other) noexcept : kind(other.kind)
   {
      if (kind == AstNode) {
         new(&Values) std::vector<StmtOrDecl>(move(other.Values));
      }
      else if (kind == TokenTree) {
         new(&Tokens) std::vector<std::vector<Token>>(move(other.Tokens));
      }
      else if (kind == Identifier) {
         new(&Idents) std::vector<Token>(move(other.Idents));
      }
   }

   MacroVariable &operator=(MacroVariable &&other)
   {
      destroyValue();
      new(this) MacroVariable(move(other));

      return *this;
   }

   ~MacroVariable()
   {
      destroyValue();
   }

   Kind getKind() const { return kind; }
   std::vector<StmtOrDecl> &getValues()
   {
      assert(kind == AstNode);
      return Values;
   }

   std::vector<std::vector<Token>> &getTokens()
   {
      assert(kind == TokenTree);
      return Tokens;
   }

   std::vector<Token> &getIdents()
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
   Parser &parser;
   VariableMap &VarMap;
   PatternFragment *State;

   bool compatibleTokens(const Token &Given, const Token &Needed);
   bool moveNext();

public:
   PatternMatcher(Parser &parser, MacroPattern *Pat, VariableMap &VarMap)
      : parser(parser), VarMap(VarMap),
        State(Pat->getPattern())
   {
      parser.skipWhitespace();
   }

   bool match();
};

bool PatternMatcher::match()
{
   while (!State->isEndState()) {
      if (!moveNext())
         return false;
   }

   return !State->isErrorState();
}

bool PatternMatcher::compatibleTokens(const Token &Given, const Token &Needed)
{
   if (Given.getKind() != Needed.getKind())
      return false;

   switch (Needed.getKind()) {
   case tok::ident: case tok::op_ident:
      return Given.getIdentifierInfo() == Needed.getIdentifierInfo();
   case tok::stringliteral: case tok::integerliteral: case tok::fpliteral:
   case tok::charliteral:
      return Given.getText() == Needed.getText();
   default:
      return true;
   }
}

bool PatternMatcher::moveNext()
{
   PatternFragment *NextState               = nullptr;
   PatternFragment *UnconditionalTransition = nullptr;
   PatternFragment *VariableTransition      = nullptr;

   for (auto &Trans : State->getTransitions()) {
      if (!Trans)
         continue;

      if (Trans.isUnconditional()) {
         if (Trans.Next->getKind() == PatternFragment::Variable) {
            VariableTransition = Trans.Next;
         }
         else {
            UnconditionalTransition = Trans.Next;
         }
      }
      else if (Trans.isEOF() && parser.currentTok().is(tok::eof)) {
         NextState = Trans.Next;
         break;
      }
      else if (compatibleTokens(parser.currentTok(), Trans.Tok)) {
         NextState = Trans.Next;
         parser.advance();

         break;
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
            parser.SP.diagnose(err_unexpected_token,
                               parser.currentTok().getSourceLoc(),
                               parser.currentTok().toString(), true,
                               "identifier");

            return false;
         }

         auto It = VarMap.find(NextState->getVariableName());
         if (It == VarMap.end()) {
            It = VarMap.try_emplace(NextState->getVariableName(),
                                    MacroVariable::Identifier).first;
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
         auto It = VarMap.find(NextState->getVariableName());
         if (It == VarMap.end()) {
            It = VarMap.try_emplace(NextState->getVariableName(),
                                    MacroVariable::TokenTree).first;
         }

         auto &Toks = It->getSecond().getTokens().emplace_back();

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

         for (auto &Trans : NextState->getTransitions()) {
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
            It = VarMap.try_emplace(NextState->getVariableName(),
                                    MacroVariable::TokenTree).first;
         }

         auto &Toks = It->getSecond().getTokens().emplace_back();
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

      if (!Result)
         return false;

      parser.advance();

      StmtOrDecl SOD;
      if (Result.holdsDecl())
         SOD = Result.getDecl();
      else if (Result.holdsExpr())
         SOD = Result.getExpr();
      else
         SOD = Result.getStatement();

      auto It = VarMap.find(NextState->getVariableName());
      if (It == VarMap.end()) {
         It = VarMap.try_emplace(NextState->getVariableName(),
                                 MacroVariable::AstNode).first;
      }

      It->getSecond().getValues().push_back(SOD);
   }
   else if (NextState->getKind() == PatternFragment::Tokens) {
      for (auto &Tok : NextState->getTokens().drop_front(1)) {
         if (!compatibleTokens(parser.currentTok(), Tok)) {
            return false;
         }

         parser.advance();
      }
   }

   State = NextState;
   return true;
}

static bool matchPattern(Parser &parser, MacroPattern *Pat, VariableMap &VarMap)
{
   return PatternMatcher(parser, Pat, VarMap).match();
}

class MacroExpander {
   SemaPass &SP;
   MacroPattern *Pat;
   VariableMap &VarMap;
   Parser::ExpansionKind Kind;

   SourceLocation ExpandedFrom;
   unsigned BaseOffset;
   unsigned ExpansionOffset;

   bool expandInto(ExpansionFragment *Frag,
                   llvm::SmallVectorImpl<Token> &Vec,
                   unsigned idx = 0,
                   unsigned NumRepetitions = 0,
                   const IdentifierInfo *ExpandedVarName = nullptr,
                   SourceLocation ExpandedVarLoc = SourceLocation());

   SourceLocation makeSourceLoc(SourceLocation Loc)
   {
      return SourceLocation(ExpansionOffset + (Loc.getOffset() - BaseOffset));
   }

public:
   MacroExpander(SemaPass &SP, MacroPattern *Pat, VariableMap &VarMap,
                 Parser::ExpansionKind Kind,
                 SourceLocation ExpandedFrom, MacroDecl *M)
      : SP(SP), Pat(Pat), VarMap(VarMap), Kind(Kind),
        ExpandedFrom(ExpandedFrom)
   {
      BaseOffset = Pat->getSourceLoc().getOffset();
      ExpansionOffset = SP.getCompilationUnit().getFileMgr()
                     .createMacroExpansion(ExpandedFrom, Pat->getSourceLoc(),
                                           Pat->getSourceLength() + 1,
                                           M->getDeclName());
   }

   ParseResult expand();
};

bool MacroExpander::expandInto(ExpansionFragment *Frag,
                               llvm::SmallVectorImpl<Token> &Vec,
                               unsigned idx,
                               unsigned NumRepetitions,
                               const IdentifierInfo *ExpandedVarName,
                               SourceLocation ExpandedVarLoc) {
   switch (Frag->getKind()) {
   case ExpansionFragment::Tokens: {
      for (auto &Tok : Frag->getTokens())
         Vec.emplace_back(Tok, makeSourceLoc(Tok.getSourceLoc()));

      break;
   }
   case ExpansionFragment::Variable: {
      auto It = VarMap.find(Frag->getVariableName());
      assert(It != VarMap.end() && "variable not captured!");

      auto &Var = It->getSecond();
      if (NumRepetitions && Var.size() != NumRepetitions) {
         SP.diagnose(err_multiple_expanded_macro_variables,
                     Frag->getVariableName()->getIdentifier(),
                     ExpandedVarName->getIdentifier(),
                     NumRepetitions, Var.size(),
                     Frag->getLoc(), ExpandedVarLoc);

         return false;
      }

      assert(Var.size() > idx && "index out of bounds");

      switch (Var.getKind()) {
      case MacroVariable::AstNode: {
         auto &SOD = Var.getValues()[idx];
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
         auto &Toks = Var.getTokens()[idx];
         for (auto &Tok : Toks)
            Vec.emplace_back(Tok, makeSourceLoc(Tok.getSourceLoc()));

         break;
      }
      case MacroVariable::Identifier: {
         // concatenate immediately adjacent identifiers
         if (!Vec.empty() && Vec.back().oneOf(tok::ident, tok::op_ident)) {
            auto LastTok = Vec.back();

            auto concat = LastTok.getIdentifier().str();
            concat += Var.getIdents()[idx].getIdentifier().str();

            auto *II = &SP.getContext().getIdentifiers().get(concat);
            Vec.pop_back();

            Vec.emplace_back(II, makeSourceLoc(LastTok.getSourceLoc()),
                             LastTok.getKind());
         }
         else {
            auto &Tok = Var.getIdents()[idx];
            Vec.emplace_back(Tok, makeSourceLoc(Tok.getSourceLoc()));
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

      auto &Var = It->getSecond();
      unsigned NumValues = (unsigned)Var.size();

      for (unsigned i = 0; i < NumValues; ++i) {
         for (auto &ExpFrag : Frag->getRepetitionFragments()) {
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
      for (auto &ExpFrag : Frag->getRepetitionFragments()) {
         if (!expandInto(ExpFrag, Toks, idx, NumRepetitions, ExpandedVarName,
                         ExpandedVarLoc)) {
            return false;
         }
      }

      // stringify tokens
      std::string str;
      llvm::raw_string_ostream OS(str);

      for (auto &Tok : Toks) {
         if (Tok.oneOf(tok::space, tok::newline))
            continue;

         OS << Tok;
      }

      // lex back into tokens
      auto Buf = llvm::MemoryBuffer::getMemBuffer(OS.str());
      Lexer Lex(SP.getContext().getIdentifiers(), SP.getDiags(), Buf.get(), 0);

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

ParseResult MacroExpander::expand()
{
   SmallVector<Token, 0> Tokens;
   for (auto &Frag : Pat->getExpansion()) {
      if (!expandInto(Frag, Tokens))
         return ParseError();
   }

   unsigned SourceID = SP.getCompilationUnit().getFileMgr()
                         .getSourceId(ExpandedFrom);

   lex::Lexer Lexer(SP.getContext().getIdentifiers(), SP.getDiags(), Tokens,
                    SourceID);

   parse::Parser parser(SP.getContext(), &Lexer, SP);
   return parser.parseWithKind(ExpandedFrom, Kind);
}

ParseResult Parser::expandMacro(SemaPass &SP,
                                MacroDecl *Macro,
                                StmtOrDecl SOD,
                                ArrayRef<Token> Tokens,
                                ExpansionKind Kind) {
   auto &Context = SP.getContext();

   VariableMap VarMap;
   MacroPattern *Match = nullptr;

   lex::Lexer Lexer(Context.getIdentifiers(), SP.getDiags(), Tokens, 0);
   parse::Parser parser(Context, &Lexer, SP);

   for (auto &Pat : Macro->getPatterns()) {
      if (matchPattern(parser, Pat, VarMap)) {
         Match = Pat;
         break;
      }

      VarMap.clear();
   }

   if (!Match) {
      SP.diagnose(SOD, err_could_not_match_pattern, Macro->getDeclName(),
                  SOD.getSourceRange());

      return ParseError();
   }

   return MacroExpander(SP, Match, VarMap, Kind, SOD.getSourceLoc(), Macro)
      .expand();
}

enum class BuiltinMacro {
   none, stringify, include_str, include,
};

std::pair<ParseResult, bool>
Parser::checkBuiltinMacro(SemaPass &SP,
                          DeclarationName DN,
                          StmtOrDecl SOD,
                          llvm::ArrayRef<Token> Tokens,
                          ExpansionKind Kind) {
   auto MacroKind =
      StringSwitch<BuiltinMacro>(DN.getMacroName()->getIdentifier())
         .Case("stringify", BuiltinMacro::stringify)
         .Case("include_str", BuiltinMacro::include_str)
         .Case("include", BuiltinMacro::include)
         .Default(BuiltinMacro::none);

   switch (MacroKind) {
   case BuiltinMacro::none:
      return { ParseError(), false };
   case BuiltinMacro::stringify: {
      std::string str;
      {
         llvm::raw_string_ostream OS(str);
         for (auto &Tok : Tokens.drop_back(1)) {
            OS << Tok;
         }
      }

      auto Alloc = (char*)SP.getContext().Allocate(str.size());
      std::copy(str.begin(), str.end(), Alloc);

      auto S = StringLiteral::Create(SP.getContext(),
                                     SOD.getSourceRange(),
                                     StringRef(Alloc, str.size()));

      return { ParseResult(S), true };
   }
   case BuiltinMacro::include:
   case BuiltinMacro::include_str: {
      if (Tokens.size() != 2 || !Tokens.front().is(tok::stringliteral)) {
         SP.diagnose(err_generic_error, "expected string literal");
         return { ParseError(), true };
      }

      auto &FileMgr = *SP.getDiags().getFileMgr();

      auto FileName = Tokens.front().getText();
      auto includeDirs = SP.getCompilationUnit().getOptions().getIncludeDirs()
                           .vec();

      includeDirs.push_back(
         fs::getPath(FileMgr.getFileName(SOD.getSourceLoc()).str()));

      auto Path = fs::getPath(FileName);
      if (!Path.empty()) {
         for (auto &Dir : includeDirs) {
            fs::appendToPath(Dir, Path);
         }

         FileName = fs::getFileNameAndExtension(FileName);
      }

      auto realFile = fs::findFileInDirectories(FileName, includeDirs);
      if (realFile.empty()) {
         SP.diagnose(err_generic_error, "file " + FileName + " not found",
                     Tokens.front().getSourceLoc());

         return { ParseError(), true };
      }

      auto File = FileMgr.openFile(realFile);

      if (MacroKind == BuiltinMacro::include_str) {
         auto S = StringLiteral::Create(SP.getContext(), SOD.getSourceRange(),
                                        llvm::StringRef(
                                           File.Buf->getBufferStart(),
                                           File.Buf->getBufferSize()));

         return { ParseResult(S), true };
      }

      Lexer lexer(SP.getContext().getIdentifiers(), SP.getDiags(),
                  File.Buf, File.SourceId, File.BaseOffset);
      Parser parser(SP.getContext(), &lexer, SP);

      return { parser.parseWithKind(SOD.getSourceLoc(), Kind, true), true };
   }
   }
}

ParseResult Parser::parseWithKind(SourceLocation Loc, ExpansionKind Kind,
                                  bool IsIncludeMacro) {
   ParseResult Result;
   switch (Kind) {
   case Parser::ExpansionKind::Expr:
      Result = parseExprSequence();

      advance();
      if (!currentTok().is(tok::eof))
         SP.diagnose(err_leftover_tokens_after_parsing,
                     currentTok().getSourceLoc(), (int)Kind);

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

} // namespace parse
} // namespace cdot