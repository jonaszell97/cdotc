//
// Created by Jonas Zell on 01.02.18.
//

#include "Parser.h"

#include "Record.h"
#include "TableGen.h"
#include "Value.h"

#include "Basic/Precedence.h"

#include "Support/Casting.h"
#include "Support/LiteralParser.h"

#include <llvm/ADT/StringSwitch.h>
#include <llvm/ADT/SmallString.h>

using namespace cdot::lex;
using namespace cdot::diag;
using namespace cdot::support;

LLVM_ATTRIBUTE_NORETURN
static void abortBP()
{
   abort();
}

namespace cdot {
namespace tblgen {

Parser::Parser(TableGen &TG,
               llvm::MemoryBuffer &Buf,
               unsigned sourceId)
   : TG(TG), lex(TG.getIdents(), TG.Diags, &Buf, sourceId, 1, '\0'),
     GlobalRK(std::make_unique<RecordKeeper>(TG)),
     RK(GlobalRK.get())
{
   TG.getIdents().addTblGenKeywords();
   lex.lex();
}

Parser::~Parser()
{

}

bool Parser::parse()
{
   if (currentTok().oneOf(tok::newline, tok::space))
      advance();

   while (!currentTok().is(tok::eof)) {
      parseNextDecl();
      advance();
   }

   return TG.Diags.getNumErrors() == 0;
}

void Parser::parseNextDecl()
{
   if (currentTok().is(tok::kw_class)) {
      parseClass();
   }
   else if (currentTok().is(tok::kw_def)) {
      parseRecord();
   }
   else if (currentTok().is(tok::kw_let)) {
      parseValue();
   }
   else if (currentTok().is(tok::kw_namespace)) {
      parseNamespace();
   }
   else if (currentTok().is(tok::tblgen_foreach)) {
      parseForEach();
   }
   else if (currentTok().is(tok::tblgen_print)) {
      parsePrint();
   }
   else {
      TG.Diags.Diag(err_generic_error)
         << "unexpected token " + currentTok().toString()
         << lex.getSourceLoc();

      return;
   }
}

void Parser::parseClass()
{
   assert(currentTok().is(tok::kw_class));
   advance();

   auto name = tryParseIdentifier();
   if (auto prevLoc = RK->lookupAnyDecl(name)) {
      TG.Diags.Diag(err_generic_error)
         << "redeclaration of " + name
         << currentTok().getSourceLoc();

      TG.Diags.Diag(note_generic_note)
         << "previous declaration here"
         << prevLoc;

      abortBP();
   }

   auto C = RK->CreateClass(name, currentTok().getSourceLoc());
   currentClass = C;

   llvm::SmallVector<size_t, 8> fieldParameters;

   if (peek().is(tok::smaller)) {
      advance();
      parseTemplateParams(C, fieldParameters);
      assert(currentTok().is(tok::greater));
   }

   if (peek().is(tok::colon)) {
      advance();
      parseBases(C);
   }

   if (peek().is(tok::open_brace)) {
      advance();
      advance();

      while (!currentTok().is(tok::close_brace)) {
         if (currentTok().is(tok::kw_let)) {
            parseFieldDecl(C);
         }
         else {
            TG.Diags.Diag(err_generic_error)
               << "expected in-class declaration"
               << lex.getSourceLoc();

            abortBP();
         }

         advance();
      }
   }

   for (auto &idx : fieldParameters) {
      auto &P = C->getParameters()[idx];
      auto newDecl = C->addField(P.getName(), P.getType(), nullptr,
                                 P.getDeclLoc(), idx);

      if (!newDecl) {
         TG.Diags.Diag(err_generic_error)
            << "duplicate declaration of field " + P.getName() + " for class "
               + C->getName()
            << P.getDeclLoc();

         TG.Diags.Diag(note_generic_note)
            << "previous declaration here"
            << C->getField(P.getName())->getDeclLoc();

         abortBP();
      }
   }

   currentClass = nullptr;
}

void Parser::parseTemplateParams(Class *C,
                                llvm::SmallVectorImpl<size_t> &fieldParameters){
   assert(currentTok().is(tok::smaller));
   advance();

   while (!currentTok().is(tok::greater)) {
      if (currentTok().is(tok::kw_let)) {
         advance();
         fieldParameters.push_back(C->getParameters().size());
      }

      auto loc = currentTok().getSourceLoc();
      auto name = tryParseIdentifier();
      advance();

      if (!currentTok().is(tok::colon)) {
         TG.Diags.Diag(err_generic_error)
            << "expected parameter type"
            << lex.getSourceLoc();

         abortBP();
      }

      advance();
      auto Ty = parseType();

      Value *defaultVal = nullptr;
      if (peek().is(tok::equals)) {
         advance();
         advance();

         defaultVal = parseExpr();
      }

      auto newDecl = C->addTemplateParam(name, Ty, defaultVal, loc);
      if (!newDecl) {
         TG.Diags.Diag(err_generic_error)
            << "duplicate declaration of parameter " + name + " for class "
               + C->getName()
            << loc;

         TG.Diags.Diag(err_generic_error)
            << "previous declaration here"
            << C->getTemplateParameter(name)->getDeclLoc();
      }

      advance();
      if (currentTok().is(tok::comma))
         advance();
   }
}

void Parser::parseBases(Class *C)
{
   assert(currentTok().is(tok::colon));
   advance();

   while (1) {
      auto name = tryParseIdentifier();
      auto Base = RK->lookupClass(name);

      if (!Base) {
         TG.Diags.Diag(err_generic_error)
            << "class " + name + " does not exist"
            << lex.getSourceLoc();

         abortBP();
      }

      llvm::SmallVector<SourceLocation, 8> locs;
      std::vector<Value *> templateArgs;
      if (peek().is(tok::smaller)) {
         advance();
         parseTemplateArgs(templateArgs, locs, Base);
      }

      C->addBase(Base, move(templateArgs));

      if (peek().is(tok::comma)) {
         advance();
         advance();
      }
      else
         break;
   }
}

void Parser::parseFieldDecl(Class *C)
{
   assert(currentTok().is(tok::kw_let));
   advance();

   auto loc = currentTok().getSourceLoc();
   auto name = tryParseIdentifier();
   advance();

   if (!currentTok().is(tok::colon)) {
      TG.Diags.Diag(err_generic_error)
         << "expected field type"
         << lex.getSourceLoc();

      abortBP();
   }

   advance();
   auto Ty = parseType();

   Value *defaultValue = nullptr;
   if (peek().is(tok::equals)) {
      advance();
      advance();

      defaultValue = parseExpr(Ty);
   }

   auto newDecl = C->addField(name, Ty, defaultValue, loc);
   if (!newDecl) {
      TG.Diags.Diag(err_generic_error)
         << "duplicate declaration of field " + name + " for class "
            + C->getName()
         << loc;

      TG.Diags.Diag(note_generic_note)
         << "previous declaration here"
         << C->getField(name)->getDeclLoc();
   }
}

namespace {
   enum TemplateParamResultKind {
      TP_Success,
      TP_TooFewParamsGiven,
      TP_TooManyParamsGiven,
      TP_IncompatibleType,
   };

   struct TemplateParamResult {
      TemplateParamResultKind kind;
      size_t incompatibleIndex = size_t(-1);
   };
} // anonymous namespace

static bool typesCompatible(Type *given, Type *needed)
{
   if (given == needed)
      return true;

   if (auto C = dyn_cast<ClassType>(needed)) {
      if (auto R = dyn_cast<RecordType>(given))
         return R->getRecord()->inheritsFrom(C->getClass());
      if (auto C2 = dyn_cast<ClassType>(given))
         return C2->getClass()->inheritsFrom(C->getClass());

      return false;
   }
   if (auto L = dyn_cast<ListType>(needed)) {
      auto L2 = dyn_cast<ListType>(given);
      if (!L2)
         return false;

      return typesCompatible(L2->getElementType(), L->getElementType());
   }
   if (auto D = dyn_cast<DictType>(needed)) {
      auto D2 = dyn_cast<DictType>(given);
      if (!D2)
         return false;

      return typesCompatible(D2->getElementType(), D->getElementType());
   }

   return false;
}

static TemplateParamResult checkTemplateParams(Class &C,
                                               llvm::SmallVectorImpl<SourceLocation> &locs,
                                               std::vector<Value*> &givenParams) {
   auto &neededParams = C.getParameters();
   if (givenParams.size() > neededParams.size())
      return { TP_TooManyParamsGiven };

   size_t i = 0;
   for (auto &P : neededParams) {
      if (givenParams.size() <= i) {
         if (!P.getDefaultValue())
            return { TP_TooFewParamsGiven };
         else {
            givenParams.push_back(P.getDefaultValue());
            locs.push_back(P.getDeclLoc());
         }

         ++i;
         continue;
      }

      auto givenTy = givenParams[i]->getType();
      if (!typesCompatible(givenTy, P.getType()))
         return { TP_IncompatibleType, i };

      ++i;
   }

   return { TP_Success };
}

void Parser::validateTemplateArgs(Class &Base,
                                  llvm::SmallVectorImpl<SourceLocation> &locs,
                                  std::vector<Value *> &templateArgs) {
   auto checkResult = checkTemplateParams(Base, locs, templateArgs);
   switch (checkResult.kind) {
   case TP_Success:
      break;
   case TP_TooFewParamsGiven:
      TG.Diags.Diag(err_generic_error)
         << "expected at least "
            + std::to_string(Base.getParameters().size())
            + " parameters, " + std::to_string(templateArgs.size())
            + " given"
         << currentTok().getSourceLoc();

      break;
   case TP_TooManyParamsGiven:
      TG.Diags.Diag(err_generic_error)
         << "expected at most "
            + std::to_string(Base.getParameters().size())
            + " parameters, " + std::to_string(templateArgs.size())
            + " given"
         << currentTok().getSourceLoc();

      break;
   case TP_IncompatibleType: {
      size_t idx = checkResult.incompatibleIndex;
      llvm::SmallString<128> str;

      str += Base.getParameters()[idx].getType()->toString();
      str += " and ";
      str += templateArgs[idx]->getType()->toString();

      TG.Diags.Diag(err_generic_error)
         << "incompatible types " + str.str()
         << locs[checkResult.incompatibleIndex];

      break;
   }
   }
}

void Parser::parseRecord()
{
   assert(currentTok().is(tok::kw_def));
   advance();

   auto name = tryParseIdentifier();
   if (auto prevLoc = RK->lookupAnyDecl(name)) {
      TG.Diags.Diag(err_generic_error)
         << "redeclaration of " + name
         << currentTok().getSourceLoc();

      TG.Diags.Diag(err_generic_error)
         << "previous declaration here"
         << prevLoc;
   }

   auto R = RK->CreateRecord(name, currentTok().getSourceLoc());

   if (peek().is(tok::colon)) {
      advance();
      parseBases(R);
   }

   if (peek().is(tok::open_brace)) {
      advance();
      advance();

      while (!currentTok().is(tok::close_brace)) {
         if (currentTok().is(tok::ident)) {
            parseFieldDef(R);
         }
         else {
            TG.Diags.Diag(err_generic_error)
               << "expected field definition"
               << lex.getSourceLoc();

            abortBP();
         }

         advance();
      }
   }

   finalizeRecord(*R);
}

void Parser::finalizeRecord(Record &R)
{
   auto result = TG.finalizeRecord(R);
   switch (result.status) {
   case TableGen::RFS_Success:
      break;
   case TableGen::RFS_MissingFieldValue:
      TG.Diags.Diag(err_generic_error)
         << "record " + R.getName() + " is missing a definition for field "
            + result.missingOrDuplicateFieldName
         << R.getDeclLoc();

      TG.Diags.Diag(err_generic_error)
         << "field declared here"
         << result.declLoc;

      abortBP();
   case TableGen::RFS_DuplicateField:
      break;
   }
}

void Parser::parseBases(Record *R)
{
   assert(currentTok().is(tok::colon));
   advance();

   while (1) {
      auto name = tryParseIdentifier();
      auto Base = RK->lookupClass(name);

      if (!Base) {
         TG.Diags.Diag(err_generic_error)
            << "class " + name + " does not exist"
            << lex.getSourceLoc();

         abortBP();
      }

      llvm::SmallVector<SourceLocation, 8> locs;
      std::vector<Value *> templateArgs;
      if (peek().is(tok::smaller)) {
         advance();
         parseTemplateArgs(templateArgs, locs, Base);
      }

      validateTemplateArgs(*Base, locs, templateArgs);
      R->addBase(Base, move(templateArgs));

      if (peek().is(tok::comma)) {
         advance();
         advance();
      }
      else
         break;
   }
}

void Parser::parseFieldDef(Record *R)
{
   assert(currentTok().is(tok::ident));
   auto loc = currentTok().getSourceLoc();

   auto name = currentTok().getIdentifierInfo()->getIdentifier();
   auto FTy = R->getFieldType(name);

   if (!FTy) {
      TG.Diags.Diag(err_generic_error)
         << "record " + R->getName() + " does not inherit a field named "
            + name + " from any of its bases"
         << lex.getSourceLoc();

      abortBP();
   }

   advance();
   if (!currentTok().is(tok::equals)) {
      TG.Diags.Diag(err_generic_error)
         << "record fields must have a definition"
         << lex.getSourceLoc();

      abortBP();
   }

   advance();

   auto value = parseExpr(FTy);
   if (!typesCompatible(value->getType(), FTy)) {
      TG.Diags.Diag(err_generic_error)
         << "incompatible types " + value->getType()->toString() + " and "
            + FTy->toString()
         << loc;
   }

   R->addOwnField(loc, name, FTy, value);
}

llvm::StringRef Parser::tryParseIdentifier()
{
   if (currentTok().is(tok::ident))
      return currentTok().getIdentifier();

   if (currentTok().is(tok::dollar)) {
      if (peek().is(tok::open_paren)) {
         advance();
         advance();

         auto ident = tryParseIdentifier();
         auto V = getForEachVal(ident);

         if (!V) {
            TG.Diags.Diag(err_generic_error)
               << "value " + ident + " not found"
               << currentTok().getSourceLoc();

            abortBP();
         }

         auto S = dyn_cast<StringLiteral>(V);
         if (!S) {
            TG.Diags.Diag(err_generic_error)
               << "cannot use non-string value as identifier"
               << currentTok().getSourceLoc();

            abortBP();
         }

         expect(tok::close_paren);
         return S->getVal();
      }
   }

   TG.Diags.Diag(err_generic_error)
      << "expected identifier"
      << currentTok().getSourceLoc();

   abortBP();
}

void Parser::parseValue()
{
   assert(currentTok().is(tok::kw_let));
   advance();

   auto name = tryParseIdentifier();
   auto loc = currentTok().getSourceLoc();

   if (auto prevLoc = RK->lookupAnyDecl(name)) {
      TG.Diags.Diag(err_generic_error)
         << "redeclaration of " + name
         << loc;

      TG.Diags.Diag(note_generic_note)
         << "previous declaration here"
         << prevLoc;
   }

   Type *Ty = nullptr;
   if (peek().is(tok::colon)) {
      advance();
      advance();

      Ty = parseType();
   }

   expect(tok::equals);
   advance();

   auto Val = parseExpr(Ty);
   if (Ty && !typesCompatible(Val->getType(), Ty)) {
      TG.Diags.Diag(err_generic_error)
         << "incompatible types " + Val->getType()->toString() + " and "
            + Ty->toString()
         << loc;
   }

   RK->addValue(name, Val, loc);
}

void Parser::parsePrint()
{
   assert(currentTok().is(tok::tblgen_print));
   advance();

   auto loc = currentTok().getSourceLoc();
   auto E = parseExpr();

   llvm::SmallString<128> s;
   {
      llvm::raw_svector_ostream ss(s);
      ss << E;
   }

   TG.Diags.Diag(err_generic_error)
      << s.str()
      << loc;
}

void Parser::parseForEach()
{
   assert(currentTok().is(tok::tblgen_foreach));

   advance();
   auto name = tryParseIdentifier();

   expect(tok::kw_in);
   advance();

   auto loc = currentTok().getSourceLoc();
   auto Range = parseExpr();

   if (!isa<ListType>(Range->getType()) && !isa<DictType>(Range->getType())) {
      TG.Diags.Diag(err_generic_error)
         << "expected list or dict value"
         << loc;
   }

   expect(tok::open_brace);
   advance();

   auto SafePoint = lex.makeSavePoint();

   if (auto L = dyn_cast<ListLiteral>(Range)) {
      for (auto &V : L->getValues()) {
         ForEachScope scope(*this, name, V);
         SafePoint.reset();

         while (!currentTok().is(tok::close_brace)) {
            parseNextDecl();
            advance();
         }
      }
   }
   else if (auto D = dyn_cast<DictLiteral>(Range)) {
      for (auto &V : D->getValues()) {
         ForEachScope scope(*this, name, V.getValue());
         SafePoint.reset();

         while (!currentTok().is(tok::close_brace)) {
            parseNextDecl();
            advance();
         }
      }
   }
   else {
      llvm_unreachable("hmmm...");
   }
}

void Parser::parseNamespace()
{
   assert(currentTok().is(tok::kw_namespace));
   advance();

   auto name = tryParseIdentifier();
   auto loc = currentTok().getSourceLoc();

   auto NS = RK->addNamespace(name, loc);
   NamespaceScope Scope(*this, NS);

   expect(tok::open_brace);
   advance();

   while (!currentTok().is(tok::close_brace)) {
      parseNextDecl();
      advance();
   }
}

Type* Parser::parseType()
{
   auto ident = tryParseIdentifier();
   if ((ident.size() == 2 || ident.size() == 3)
       && (ident[0] == 'i' || ident[0] == 'u')) {
      auto isUnsigned = ident[0] == 'u';
      auto bw = llvm::StringSwitch<unsigned>(ident.substr(1))
         .Case("1", 1).Case("8", 8).Case("16", 16).Case("32", 32)
         .Case("64", 64).Default(0);

      if (bw)
         return TG.getIntegerTy(bw, isUnsigned);
   }

   if (ident == "string")
      return TG.getStringTy();

   if (ident == "f64")
      return TG.getDoubleTy();

   if (ident == "f32")
      return TG.getFloatTy();

   if (ident == "list") {
      expect(tok::smaller);
      advance();

      auto elTy = parseType();
      expect(tok::greater);

      return TG.getListType(elTy);
   }

   if (ident == "dict") {
      expect(tok::smaller);
      advance();

      auto elTy = parseType();
      expect(tok::greater);

      return TG.getDictType(elTy);
   }

   if (ident == "code") {
      return TG.getCodeTy();
   }

   if (auto C = RK->lookupClass(ident))
      return TG.getClassType(C);

   TG.Diags.Diag(err_generic_error)
      << "unknown type " + ident
      << currentTok().getSourceLoc();

   abortBP();
}

static Class *findCommonBase(Class *C1, Class *C2)
{
   if (C1 == C2)
      return C1;

   for (auto &B : C1->getBases()) {
      if (C2->inheritsFrom(B.getBase()))
         return B.getBase();

      if (auto Common = findCommonBase(B.getBase(), C2))
         return Common;
   }

   for (auto &B : C2->getBases()) {
      if (C1->inheritsFrom(B.getBase()))
         return B.getBase();

      if (auto Common = findCommonBase(C1, B.getBase()))
         return Common;
   }

   return nullptr;
}

Value* Parser::parseExpr(Type *contextualTy)
{
   if (currentTok().is(tok::integerliteral)) {
      unsigned bitwidth = 64;
      bool isSigned     = true;

      if (auto IntTy = dyn_cast_or_null<IntType>(contextualTy)) {
         bitwidth = IntTy->getBitWidth();
         isSigned = !IntTy->isUnsigned();
      }

      LiteralParser LParser(currentTok().getText());

      auto Res = LParser.parseInteger(bitwidth, isSigned);
      assert(!Res.wasTruncated && "value too large for type");

      auto APSInt = std::move(Res.APS);

      return new(TG) IntegerLiteral(contextualTy, std::move(APSInt));
   }

   if (currentTok().is(tok::fpliteral)) {
      LiteralParser LParser(currentTok().getText());
      auto APFloat = std::move(LParser.parseFloating().APF);

      if (!contextualTy || !isa<DoubleType>(contextualTy))
         contextualTy = TG.getDoubleTy();

      return new(TG) FPLiteral(contextualTy, std::move(APFloat));
   }

   if (currentTok().is(tok::stringliteral)) {
      return new(TG) StringLiteral(TG.getStringTy(), currentTok().getText());
   }

   if (currentTok().oneOf(tok::kw_true, tok::kw_false)) {
      llvm::APInt APInt(1, (uint64_t)currentTok().is(tok::kw_true));
      return new(TG) IntegerLiteral(TG.getInt1Ty(), std::move(APInt));
   }

   if (currentTok().is(tok::charliteral)) {
      llvm::APSInt APSInt(8, currentTok().getText().front());
      return new(TG) IntegerLiteral(TG.getInt8Ty(), std::move(APSInt));
   }

   if (currentTok().is(tok::exclaim)) {
      advance();
      return parseFunction(contextualTy);
   }

   if (currentTok().is(tok::dollar)) {
      if (peek().is(tok::open_paren)) {
         advance();
         advance();

         auto ident = tryParseIdentifier();
         auto V = getForEachVal(ident);

         if (!V) {
            TG.Diags.Diag(err_generic_error)
               << "undeclared value " + ident
               << currentTok().getSourceLoc();

            abortBP();
         }

         expect(tok::close_paren);
         return V;
      }
   }

   if (currentTok().is(tok::open_brace)) {
      advance(false, false);

      unsigned openedBraces = 1;
      unsigned closedBraces = 0;
      llvm::SmallString<512> str;

      while (openedBraces != closedBraces) {
         switch (currentTok().getKind()) {
            case tok::open_brace:
               ++openedBraces;
               break;
            case tok::close_brace:
               ++closedBraces;
               if (openedBraces == closedBraces)
                  continue;

               break;
            case tok::eof:
               TG.Diags.Diag(err_generic_error)
                  << "unexpected end of file, expecting '}'"
                  << currentTok().getSourceLoc();

               abortBP();
            default:
               break;
         }

         currentTok().rawRepr(str);
         advance(false, false);
      }

      return new(TG) CodeBlock(TG.getCodeTy(), str.str());
   }

   if (currentTok().is(tok::ident)) {
      auto ident =  currentTok().getIdentifierInfo()->getIdentifier();

      if (auto R = RK->lookupRecord(ident)) {
         if (peek().is(tok::period)) {
            advance();
            expect(tok::ident);

            auto field = currentTok().getIdentifierInfo()->getIdentifier();
            auto F = R->getFieldValue(field);

            if (!F) {
               TG.Diags.Diag(err_generic_error)
                  << "record " + R->getName() + " does not have a field named "
                     + field
                  << currentTok().getSourceLoc();

               abortBP();
            }

            return F;
         }

         return new(TG) RecordVal(TG.getRecordType(R), R);
      }

      // anonymous record
      if (auto Base = RK->lookupClass(ident)) {
         auto BeginLoc = currentTok().getSourceLoc();

         llvm::SmallVector<SourceLocation, 8> locs;
         std::vector<Value *> templateArgs;
         if (peek().is(tok::smaller)) {
            advance();
            parseTemplateArgs(templateArgs, locs, Base);
         }

         validateTemplateArgs(*Base, locs, templateArgs);

         auto R = RK->CreateAnonymousRecord(BeginLoc);
         R->addBase(Base, move(templateArgs));

         finalizeRecord(*R);

         return new(TG) RecordVal(TG.getRecordType(R), R);
      }

      if (auto V = RK->lookupValueDecl(ident))
         return V->getVal();

      Type *Ty = nullptr;
      if (currentClass) {
         auto F = currentClass->getTemplateParameter(ident);
         if (F)
            Ty = F->getType();
      }

      if (!Ty) {
         TG.Diags.Diag(err_generic_error)
            << "reference to undeclared identifier " + ident
            << currentTok().getSourceLoc();

         abortBP();
      }

      auto Id = new(TG) IdentifierVal(Ty, ident);

      if (peek().is(tok::open_square)) {
         advance();
         expect(tok::stringliteral);

         auto key = currentTok().getText();
         expect(tok::close_square);

         return new(TG) DictAccessExpr(Id, key);
      }

      return Id;
   }

   if (currentTok().is(tok::open_square)) {
      advance();

      bool isDict = false;
      std::vector<Value*> keys;
      std::vector<Value*> values;

      Type *ElementTy = nullptr;
      if (contextualTy) {
         if (auto L = dyn_cast<ListType>(contextualTy))
            ElementTy = L->getElementType();
         else if (auto D = dyn_cast<DictType>(contextualTy))
            ElementTy = D->getElementType();
      }

      while (!currentTok().is(tok::close_square)) {
         auto expr = parseExpr(ElementTy);
         if (peek().is(tok::colon)) {
            auto S = dyn_cast<StringLiteral>(expr);
            if (!S) {
               TG.Diags.Diag(err_generic_error)
                  << "dictionary key must be a string"
                  << currentTok().getSourceLoc();

               abortBP();
            }

            advance();
            advance();

            auto val = parseExpr();
            keys.push_back(expr);
            values.push_back(val);

            isDict = true;
         }
         else {
            values.push_back(expr);
         }

         if (!ElementTy) {
            ElementTy = values.back()->getType();
         }
         else if (!typesCompatible(values.back()->getType(), ElementTy)) {
            TG.Diags.Diag(err_generic_error)
               << "all values in a list literal must have the same type"
               << currentTok().getSourceLoc();

            abortBP();
         }

         advance();
         if (currentTok().is(tok::comma))
            advance();
      }

      if (!values.empty()) {
         if (isDict)
            contextualTy = TG.getDictType(ElementTy);
         else
            contextualTy = TG.getListType(ElementTy);
      }
      else if (!contextualTy) {
         if (values.empty()) {
            TG.Diags.Diag(err_generic_error)
               << "could not infer type of list literal"
               << currentTok().getSourceLoc();

            abortBP();
         }
      }

      if (isDict || isa<DictType>(contextualTy))
         return new(TG) DictLiteral(contextualTy, move(keys), move(values));

      return new(TG) ListLiteral(contextualTy, move(values));
   }

   TG.Diags.Diag(err_generic_error)
      << "expected expression, found " + currentTok().toString()
      << lex.getSourceLoc();

   abortBP();
}

#define EXPECT_NUM_ARGS(ArgCnt)                                           \
   if (args.size() != ArgCnt) { TG.Diags.Diag(err_generic_error)          \
      << "function " + func + " expects " + std::to_string(ArgCnt)        \
         + " arguments" << parenLoc; abortBP(); }

#define EXPECT_AT_LEAST_ARGS(ArgCnt)                                      \
   if (args.size() < ArgCnt) { TG.Diags.Diag(err_generic_error)           \
      << "function " + func + " expects at least "                        \
         + std::to_string(ArgCnt)                                         \
         + " arguments" << parenLoc; abortBP(); }

#define EXPECT_ARG_VALUE(ArgNo, ValKind)                                \
   if (!isa<ValKind>(args[ArgNo])) { TG.Diags.Diag(err_generic_error)   \
      << "function " + func + " expects arg #" + std::to_string(ArgNo)  \
         + " to be a " #ValKind; abortBP(); }

Value* Parser::parseFunction(Type *contextualTy)
{
   enum FuncKind {
      Unknown,
      AllOf,
      Concat,
      Push,
      Pop,
      First,
      Last,
      StrConcat,
   };

   auto func = tryParseIdentifier();
   auto kind = llvm::StringSwitch<FuncKind>(func)
      .Case("allof", AllOf)
      .Case("push", Push)
      .Case("pop", Pop)
      .Case("line", First)
      .Case("last", Last)
      .Case("concat", Concat)
      .Case("str_concat", StrConcat)
      .Default(Unknown);

   expect(tok::open_paren);
   auto parenLoc = currentTok().getSourceLoc();

   advance();

   llvm::SmallVector<SourceLocation, 8> argLocs;
   llvm::SmallVector<Value*, 8> args;

   while (!currentTok().is(tok::close_paren)) {
      argLocs.push_back(currentTok().getSourceLoc());
      args.push_back(parseExpr());

      advance();
      if (currentTok().is(tok::comma))
         advance();
   }

   switch (kind) {
      case Unknown:
         TG.Diags.Diag(err_generic_error)
            << "unknown function '" + func + "'"
            << currentTok().getSourceLoc();

         abortBP();

      case AllOf: {
         EXPECT_AT_LEAST_ARGS(1);

         Class *CommonBase = nullptr;
         llvm::SmallVector<Record *, 8> Records;

         size_t i = 0;
         for (auto &arg : args) {
            EXPECT_ARG_VALUE(i, StringLiteral);

            auto className = cast<StringLiteral>(arg)->getVal();
            auto C = RK->lookupClass(className);
            if (!C) {
               TG.Diags.Diag(err_generic_error)
                  << "class " + className + " does not exist"
                  << argLocs[i];

               abortBP();
            }

            if (!CommonBase) {
               CommonBase = C;
            }
            else {
               CommonBase = findCommonBase(C, CommonBase);
               if (!CommonBase) {
                  TG.Diags.Diag(err_generic_error)
                     << "incompatible classes " + C->getName() + " and "
                        + CommonBase->getName()
                     << argLocs[i];

                  abortBP();
               }
            }

            RK->getAllDefinitionsOf(C, Records);
            ++i;
         }


         assert(CommonBase && "no common base class");

         std::vector<Value *> vals;
         for (auto &r : Records)
            vals.push_back(new(TG) RecordVal(TG.getRecordType(r), r));

         Type *listTy = TG.getListType(TG.getClassType(CommonBase));
         if (contextualTy && typesCompatible(listTy, contextualTy))
            listTy = contextualTy;

         return new(TG) ListLiteral(listTy, move(vals));
      }
      case Push: {
         EXPECT_NUM_ARGS(2);
         EXPECT_ARG_VALUE(0, ListLiteral);

         auto list = cast<ListLiteral>(args[0]);
         if (!typesCompatible(args[1]->getType(),
                              cast<ListType>(list->getType())
                                 ->getElementType())) {
            TG.Diags.Diag(err_generic_error)
               << "incompatible types " + args[1]->getType()->toString()
                + " and " + cast<ListType>(list->getType())
                  ->getElementType()->toString()
               << currentTok().getSourceLoc();

            abortBP();
         }

         std::vector<Value*> copy = list->getValues();
         copy.push_back(args[1]);

         return new(TG) ListLiteral(list->getType(), move(copy));
      }
      case Pop: {
         EXPECT_NUM_ARGS(1);
         EXPECT_ARG_VALUE(0, ListLiteral);

         auto list = cast<ListLiteral>(args[0]);
         std::vector<Value*> copy = list->getValues();

         if (copy.empty()) {
            TG.Diags.Diag(err_generic_error)
               << "popping from empty list"
               << parenLoc;

            abortBP();
         }

         copy.pop_back();
         return new(TG) ListLiteral(list->getType(), move(copy));
      }
      case First: {
         EXPECT_NUM_ARGS(1);
         EXPECT_ARG_VALUE(0, ListLiteral);

         auto list = cast<ListLiteral>(args[0]);
         if (list->getValues().empty()) {
            TG.Diags.Diag(err_generic_error)
               << "list is empty"
               << parenLoc;

            abortBP();
         }

         return list->getValues().front();
      }
      case Last: {
         EXPECT_NUM_ARGS(1);
         EXPECT_ARG_VALUE(0, ListLiteral);

         auto list = cast<ListLiteral>(args[0]);
         if (list->getValues().empty()) {
            TG.Diags.Diag(err_generic_error)
               << "list is empty"
               << parenLoc;

            abortBP();
         }

         return list->getValues().back();
      }
      case Concat: {
         EXPECT_NUM_ARGS(2);
         EXPECT_ARG_VALUE(0, ListLiteral);
         EXPECT_ARG_VALUE(1, ListLiteral);

         auto l1 = cast<ListLiteral>(args[0]);
         auto l2 = cast<ListLiteral>(args[1]);

         if (!typesCompatible(l2->getType(), l1->getType())) {
            TG.Diags.Diag(err_generic_error)
               << "incompatible types " + l1->getType()->toString()
                  + " and " + l2->getType()->toString()
               << currentTok().getSourceLoc();

            abortBP();
         }

         auto copy = l1->getValues();
         copy.insert(copy.end(),
                     l2->getValues().begin(),
                     l2->getValues().end());

         return new(TG) ListLiteral(l1->getType(), move(copy));
      }
      case StrConcat: {
         EXPECT_NUM_ARGS(2);
         EXPECT_ARG_VALUE(0, StringLiteral);
         EXPECT_ARG_VALUE(1, StringLiteral);

         auto l1 = cast<StringLiteral>(args[0]);
         auto l2 = cast<StringLiteral>(args[1]);

         return new(TG) StringLiteral(l1->getType(),
                                      (l1->getVal() + l2->getVal()).str());
      }
   }

   llvm_unreachable("unhandled function kind");
}

#undef EXPECT_NUM_ARGS
#undef EXPECT_AT_LEAST_ARGS
#undef EXPECT_ARG_VALUE

void Parser::parseTemplateArgs(std::vector<Value *> &args,
                               llvm::SmallVectorImpl<SourceLocation> &locs,
                               Class *forClass) {
   assert(currentTok().is(tok::smaller));
   advance();

   size_t idx = 0;
   while (!currentTok().is(tok::greater)) {
      locs.push_back(currentTok().getSourceLoc());

      if (idx < forClass->getParameters().size()) {
         args.push_back(parseExpr(forClass->getParameters()[idx].getType()));
      }
      else {
         args.push_back(parseExpr());
      }

      advance();
      if (currentTok().is(tok::comma))
         advance();

      ++idx;
   }
}

} // namespace tblgen
} // namespace cdot