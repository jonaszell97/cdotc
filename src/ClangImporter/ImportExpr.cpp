//
// Created by Jonas Zell on 29.07.18.
//

#include "ImporterImpl.h"

#include "AST/ASTContext.h"
#include "AST/Expression.h"
#include "Driver/Compiler.h"
#include "Lex/Lexer.h"
#include "Parse/Parser.h"
#include "Sema/SemaPass.h"
#include "Support/LiteralParser.h"

#include <clang/Basic/TargetInfo.h>
#include <clang/Frontend/CompilerInstance.h>
#include <clang/Lex/LiteralSupport.h>
#include <clang/Lex/Token.h>
#include <clang/Lex/Preprocessor.h>

using namespace cdot;
using namespace cdot::ast;

using ImporterImpl = ClangImporter::ImporterImpl;

static Expression *expressionFromNumericConstantToken(ImporterImpl &I,
                                                      const clang::Token &Tok) {
   SmallString<128> SpellingBuffer;
   SpellingBuffer.resize(Tok.getLength() + 1);

   auto &PP = I.Instance->getPreprocessor();
   auto &Ctx = I.CI.getContext();

   // Get the spelling of the token, which eliminates trigraphs, etc.
   bool Invalid = false;
   StringRef TokSpelling = PP.getSpelling(Tok, SpellingBuffer, &Invalid);
   if (Invalid)
      return nullptr;

   clang::NumericLiteralParser Literal(TokSpelling, Tok.getLocation(), PP);
   if (Literal.hadError)
      return nullptr;

   auto SR = SourceRange(I.getSourceLoc(Tok.getLocation()),
                         I.getSourceLoc(Tok.getEndLoc()));

   // User defined literals are not supported.
   if (Literal.hasUDSuffix())
      return nullptr;

   if (Literal.isFloatingLiteral()) {
      QualType Ty;
      if (Literal.isHalf || Literal.isFloat16) {
         Ty = Ctx.getFloatTy();
      }
      else if (Literal.isFloat) {
         Ty = Ctx.getFloatTy();
      }
      else {
         Ty = Ctx.getDoubleTy();
      }

      llvm::APFloat APF(0.0);
      auto Status = Literal.GetFloatValue(APF);

      // FIXME handle errors
      (void) Status;

      return FPLiteral::Create(Ctx, SR, Ty, APF);
   }

   if (!Literal.isIntegerLiteral()) {
      return nullptr;
   }

   QualType Ty;

   // Get the value in the widest-possible width.
   unsigned MaxWidth = 128;
   llvm::APInt ResultVal(MaxWidth, 0);

   if (Literal.GetIntegerValue(ResultVal)) {
      // If this value didn't fit into uintmax_t, error and force to ull.
      I.CI.getSema().diagnose(diag::err_generic_error, "integer literal too "
                                                       "wide", SR);

      Ty = Ctx.getUInt128Ty();
   }
   else {
      // If this value fits into a ULL, try to figure out what else it fits into
      // according to the rules of C99 6.4.4.1p5.

      // Octal, Hexadecimal, and integers with a U suffix are allowed to
      // be an unsigned int.
      bool AllowUnsigned = Literal.isUnsigned || Literal.getRadix() != 10;

      // Check from smallest to largest, picking the smallest type we can.
      unsigned Width = 0;

      // Microsoft specific integer suffixes are explicitly sized.
      if (Literal.MicrosoftInteger) {
         if (Literal.MicrosoftInteger == 8 && !Literal.isUnsigned) {
            Width = 8;
            Ty = Ctx.getInt8Ty();
         } else {
            Width = Literal.MicrosoftInteger;
            Ty = Ctx.getIntegerTy(Width, Literal.isUnsigned);
         }
      }

      if (Ty.isNull()) {
         // Does it fit in a unsigned int?
         if (ResultVal.isIntN(64)) {
            // Does it fit in a signed int?
            if (!Literal.isUnsigned && ResultVal[63] == 0) {
               Ty = Ctx.getIntTy();
            }
            else if (AllowUnsigned) {
               Ty = Ctx.getUIntTy();
            }

            Width = 64;
         }
      }

      if (Ty.isNull()) {
         Ty = Ctx.getUInt128Ty();
         Width = 128;
      }

      if (ResultVal.getBitWidth() != Width)
         ResultVal = ResultVal.trunc(Width);
   }

   return IntegerLiteral::Create(Ctx, SR, Ty,
                                 llvm::APSInt(ResultVal, Ty->isUnsigned()));
}

static Expression *expressionFromCharToken(ImporterImpl &I,
                                           const clang::Token &Tok) {
   LiteralParser LP(StringRef(Tok.getLiteralData(), Tok.getLength()));
   auto Result = LP.parseCharacter();

   auto SR = SourceRange(I.getSourceLoc(Tok.getLocation()),
                         I.getSourceLoc(Tok.getEndLoc()));

   return CharLiteral::Create(I.CI.getContext(), SR,
                              I.CI.getContext().getUInt8Ty(),
                              (char)Result.Char);
}

static Expression *expressionFromStringToken(ImporterImpl &I,
                                             const clang::Token &Tok) {
   LiteralParser LP(StringRef(Tok.getLiteralData(), Tok.getLength()));
   auto Result = LP.parseString();

   auto SR = SourceRange(I.getSourceLoc(Tok.getLocation()),
                         I.getSourceLoc(Tok.getEndLoc()));

   return StringLiteral::Create(I.CI.getContext(), SR, move(Result.Str));
}

Expression *ImporterImpl::expressionFromLiteralToken(const clang::Token &Tok)
{
   switch (Tok.getKind()) {
   case clang::tok::numeric_constant: {
      return expressionFromNumericConstantToken(*this, Tok);
   case clang::tok::char_constant:
   case clang::tok::utf8_char_constant:
   case clang::tok::utf16_char_constant:
   case clang::tok::utf32_char_constant:
      return expressionFromCharToken(*this, Tok);
   case clang::tok::string_literal:
   case clang::tok::angle_string_literal:
   case clang::tok::utf8_string_literal:
   case clang::tok::utf16_string_literal:
   case clang::tok::utf32_string_literal:
      return expressionFromStringToken(*this, Tok);
   default:
      return nullptr;
   }
   }
}

Expression *ImporterImpl::parseExpression(ArrayRef<clang::Token> Toks)
{
   // FIXME this is pretty inefficient.
   SmallVector<lex::Token, 64> TranslatedToks;
   TranslatedToks.reserve(Toks.size());

   for (auto &Tok : Toks) {
      TranslatedToks.push_back(getToken(Tok));
   }

   // Make sure there is an EOF token at the end.
   TranslatedToks.emplace_back(lex::tok::eof);

   // Create a token lexer.
   lex::Lexer Lexer(CI.getContext().getIdentifiers(),
                    CI.getSema().getDiags(),
                    TranslatedToks, SourceID, BaseOffset);

   // Create a parser.
   parse::Parser Parser(CI.getContext(), &Lexer, CI.getSema());

   // Parse the expression.
   return Parser.parseExprSequence().tryGetExpr();
}