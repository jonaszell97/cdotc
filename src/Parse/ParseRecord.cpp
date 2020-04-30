#include "cdotc/Parse/Parser.h"

#include "cdotc/AST/Decl.h"
#include "cdotc/Basic/IdentifierInfo.h"
#include "cdotc/Lex/Lexer.h"
#include "cdotc/Diagnostics/Diagnostics.h"
#include "cdotc/Module/Module.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Support/Casting.h"

#include <llvm/ADT/SmallString.h>

using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::lex;

namespace cdot {
namespace parse {

ParseResult Parser::parseAnyRecord(lex::tok::TokenType kind)
{
   auto Loc = currentTok().getSourceLoc();

   RecordHead head;
   parseClassHead(head);

   RecordDecl* decl;
   switch (kind) {
   case tok::kw_struct:
      decl = StructDecl::Create(Context, CurDeclAttrs.Access, Loc,
                                head.recordName, move(head.conformances),
                                move(head.templateParams));
      break;
   case tok::kw_class:
      decl = ClassDecl::Create(Context, CurDeclAttrs.Access, Loc,
                               head.recordName, move(head.conformances),
                               move(head.templateParams), head.parentClass,
                               CurDeclAttrs.Abstract);
      break;
   case tok::kw_enum:
      decl = EnumDecl::Create(Context, CurDeclAttrs.Access, Loc,
                              head.recordName, move(head.conformances),
                              move(head.templateParams), head.enumRawType);
      break;
   case tok::kw_union:
      decl = UnionDecl::Create(Context, CurDeclAttrs.Access, Loc,
                               head.recordName, move(head.conformances),
                               move(head.templateParams));
      break;
   case tok::kw_protocol:
      decl = ProtocolDecl::Create(Context, CurDeclAttrs.Access, Loc,
                                  head.recordName, move(head.conformances),
                                  move(head.templateParams));
      break;
   default:
      llvm_unreachable("not a record decl!");
   }

   if (head.ColonLoc && !isa<ClassDecl>(decl)) {
      SP.diagnose(
          err_generic_error, "only classes can have an inheritance clause",
          head.ColonLoc);
   }

   if (head.OpenParenLoc && !isa<EnumDecl>(decl)) {
      SP.diagnose(
          err_generic_error, "only enums can define a raw type",
          head.OpenParenLoc);
   }

   Context.setParsedConstraints(decl, move(head.constraints));

   decl->setAccessLoc(CurDeclAttrs.AccessLoc);
   decl->setHasDefinition(head.hasDefinition);

   if (head.hasDefinition) {
      ParsingProtocolRAII PRP(*this, isa<ProtocolDecl>(decl));
      DeclContextRAII declContextRAII(*this, decl);

      parseClassInner();
   }

   return ActOnDecl(decl);
}

ParseResult Parser::parseExtension()
{
   auto Loc = consumeToken(tok::kw_extend);
   auto ExtendedType = parseType(false, true, true).tryGet();

   if (!ExtendedType) {
      if (!findTokOnLine(tok::open_brace)) {
         skipUntilNextDecl();
         return ParseError();
      }
   }

   SmallVector<SourceType, 4> Conformances;
   if (lookahead().is(Ident_with)) {
      advance();
      advance();

      while (true) {
         auto proto = parseType().tryGet();
         Conformances.push_back(proto);

         if (lookahead().is(tok::comma)) {
            advance();
            advance();
         }
         else {
            break;
         }
      }
   }

   std::vector<ParsedConstraint> Constraints;
   if (lookahead().is(Ident_where)) {
      advance();
      parseDeclConstraints(Constraints);
   }

   ExtensionDecl* decl = ExtensionDecl::Create(Context, CurDeclAttrs.Access,
                                               Loc, ExtendedType, Conformances);

   Context.setParsedConstraints(decl, move(Constraints));
   decl->setAccessLoc(CurDeclAttrs.AccessLoc);

   if (!expect(tok::open_brace)) {
      skipUntilNextDecl();
      return decl;
   }

   {
      DeclContextRAII declContextRAII(*this, decl);
      parseClassInner();
   }

   return ActOnDecl(decl);
}

void Parser::parseClassInner()
{
   RecordScopeRAII RSR(*this, true);
   advance();

   while (!currentTok().is(tok::close_brace)) {
      (void)parseNextDecl();

      advance();
      while (currentTok().is(tok::semicolon)) {
         advance();
      }
   }
}

ParseResult Parser::parseRecordLevelDecl()
{
   if (currentTok().is(tok::at))
      return parseAttributedDecl();

   bool StaticValid = false;
   bool DefaultValid = false;
   bool AccessValid = true;

   tok::TokenType DeclKind = currentTok().getKind();
   ParseResult NextDecl;
   if (currentTok().is(Ident_deinit)) {
      NextDecl = parseDestrDecl();
   }
   else if (currentTok().is(Ident_subscript)) {
      NextDecl = parseSubscriptDecl();
      DefaultValid = true;
   }
   else if (currentTok().is(Ident_memberwise)) {
      NextDecl = parseConstrDecl();
   }
   else
      switch (DeclKind) {
      case tok::kw_public:
      case tok::kw_private:
      case tok::kw_fileprivate:
      case tok::kw_protected:
      case tok::kw_internal:
         NextDecl = parseCompoundDecl(false);
         break;
      case tok::ident: {
         if (currentTok().is(Ident_macro)) {
            SP.diagnose(err_generic_error,
                        "macros cannot be members of a type",
                        currentTok().getSourceLoc());

            return parseMacro();
         }

         goto case_bad_token;
      }
      case tok::open_brace:
         return parseCompoundDecl(false, false);
      case tok::kw_for: {
         if (!lookahead().is(tok::triple_period)) {
            SP.diagnose(err_generic_error,
                        "only 'for...' declarations can appear in records",
                        currentTok().getSourceLoc());
         }

         return parseStaticForDecl(true);
      }
      case tok::kw_static:
         AccessValid = false;
         switch (lookahead().getKind()) {
         case tok::kw_if:
            NextDecl = parseStaticIfDecl();
            break;
         case tok::kw_for:
            NextDecl = parseStaticForDecl();
            break;
         default:
            llvm_unreachable("should have been caught before!");
         }

         break;
      case tok::macro_name:
         NextDecl = parseMacroExpansionDecl();
         break;
      case tok::macro_statement:
      case tok::macro_expression: {
         enum DiagKind {
            Expression,
            Statement,
            Type,
            Decl,
         };

         SP.diagnose(err_bad_macro_variable_kind, currentTok().getSourceLoc(),
                     currentTok().is(tok::macro_statement) ? Statement
                                                           : Expression,
                     Decl);

         return ParseError();
      }
      case tok::macro_declaration:
         NextDecl = currentTok().getDecl();
         break;
      case tok::kw_static_assert:
         AccessValid = false;
         NextDecl = parseStaticAssert();
         break;
      case tok::kw_static_print:
         AccessValid = false;
         NextDecl = parseStaticPrint();
         break;
      case tok::kw_typedef:
         NextDecl = parseTypedef();
         break;
      case tok::kw_alias:
         NextDecl = parseAlias();
         DefaultValid = true;
         break;
      case tok::kw_let:
      case tok::kw_var:
         StaticValid = true;
         DefaultValid = true;
         NextDecl = parseFieldDecl();
         break;
      case tok::kw_prop:
         StaticValid = true;
         DefaultValid = true;
         NextDecl = parsePropDecl();
         break;
      case tok::kw_def:
         StaticValid = true;
         DefaultValid = true;
         NextDecl = parseMethodDecl();
         break;
      case tok::kw_init:
         DefaultValid = true;
         NextDecl = parseConstrDecl();
         break;
      case tok::kw_case: {
         AccessValid = false;

         ParseResult PR;
         while (1) {
            PR = parseEnumCase();

            if (lookahead().is(tok::comma)) {
               advance();

               if (!lookahead().is(tok::ident))
                  break;

               advance();
            }
            else
               break;
         }

         NextDecl = PR;
         break;
      }
      case tok::kw_class:
      case tok::kw_enum:
      case tok::kw_struct:
      case tok::kw_union:
      case tok::kw_protocol:
         NextDecl = parseAnyRecord(DeclKind);
         break;
      case tok::kw_associatedType:
         AccessValid = false;
         DefaultValid = true;
         NextDecl = parseAssociatedType();
         break;
      case tok::kw___debug:
         NextDecl = DebugDecl::Create(Context, currentTok().getSourceLoc());
         break;
      case tok::kw_module:
         SP.diagnose(err_module_must_be_first, currentTok().getSourceLoc());
         (void)parseModuleDecl();

         return ParseError();
      default:
      case_bad_token:
         SP.diagnose(err_expecting_decl, currentTok().getSourceLoc(),
                     currentTok().toString(), /*record level*/ false);

         return skipUntilProbableEndOfStmt();
      }

   int selector;
   if (currentTok().is(Ident_deinit)) {
      selector = 8;
   }
   else
      switch (DeclKind) {
      case tok::kw_associatedType:
         selector = 0;
         break;
      case tok::kw_static: {
         if (lookahead().getKind() == tok::kw_if)
            selector = 1;
         else
            selector = 2;

         break;
      }
      case tok::kw_static_assert:
         selector = 3;
         break;
      case tok::kw_static_print:
         selector = 4;
         break;
      case tok::kw_typedef:
         selector = 5;
         break;
      case tok::kw_alias:
         selector = 6;
         break;
      case tok::kw_init:
         selector = 7;
         break;
      case tok::kw_case:
         selector = 9;
         break;
      case tok::kw_class:
      case tok::kw_enum:
      case tok::kw_struct:
      case tok::kw_union:
      case tok::kw_protocol:
         selector = 10;
         break;
      default:
         selector = -1;
         break;
      }

   if (!NextDecl)
      return ParseError();

   if (!StaticValid && CurDeclAttrs.StaticLoc) {
      SP.diagnose(err_cannot_be_static, CurDeclAttrs.StaticLoc, selector);
   }

   if (!DefaultValid && CurDeclAttrs.Default) {
      SP.diagnose(err_generic_error, "declaration cannot be marked 'default'",
                  CurDeclAttrs.DefaultLoc, selector);
   }

   if (!AccessValid && CurDeclAttrs.AccessLoc) {
      SP.diagnose(err_cannot_have_access_spec, CurDeclAttrs.AccessLoc,
                  selector);
   }

   auto* D = NextDecl.getDecl();
   if (auto ND = dyn_cast<NamedDecl>(D)) {
      ND->setAccessLoc(CurDeclAttrs.AccessLoc);
      ND->setDefault(CurDeclAttrs.Default);
   }

   return D;
}

void Parser::parseClassHead(RecordHead& Head)
{
   assert(currentTok().oneOf(tok::kw_struct, tok::kw_enum, tok::kw_class,
                             tok::kw_protocol)
          && "not a record decl!");

   advance();

   if (currentTok().getKind() != tok::ident) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                  currentTok().toString(), true, "identifier");

      skipUntilProbableEndOfStmt();
      return;
   }

   Head.recordName = currentTok().getIdentifierInfo();

   auto params = tryParseTemplateParameters();
   Head.templateParams.insert(Context, Head.templateParams.end(),
                              params.begin(), params.end());

   if (lexer->lookahead().is(tok::open_paren)) {
      advance();

      Head.OpenParenLoc = currentTok().getSourceLoc();
      advance();

      Head.enumRawType = parseType().get();

      advance();
      if (!currentTok().is(tok::close_paren)) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                     currentTok().toString(), true, "')'");
      }
   }

   auto next = lookahead();
   while (!next.is(tok::open_brace)) {
      if (next.is(tok::colon)) {
         Head.ColonLoc = next.getSourceLoc();

         if (Head.parentClass) {
            SP.diagnose(err_multiple_inheritance, currentTok().getSourceLoc());
         }

         advance();
         advance();

         Head.parentClass = parseType().tryGet();
      }
      else if (next.is(Ident_with)) {
         Head.WithLoc = next.getSourceLoc();

         advance();
         advance();

         while (true) {
            auto proto = parseType().get();
            Head.conformances.push_back(proto, Context);

            if (lookahead().is(tok::comma)) {
               advance();
               advance();
            }
            else {
               break;
            }
         }
      }
      else if (next.is(Ident_where)) {
         advance();
         parseDeclConstraints(Head.constraints);
      }
      else {
         break;
      }

      next = lookahead();
   }

   if (lookahead().is(tok::open_brace)) {
      Head.hasDefinition = true;
      advance();
   }
}

ParseResult Parser::parseConstrDecl()
{
   auto Loc = currentTok().getSourceLoc();

   bool IsMemberwise = currentTok().is(Ident_memberwise);
   if (IsMemberwise) {
      expect(tok::kw_init);

      if (auto* R = dyn_cast<StructDecl>(&SP.getDeclContext())) {
         R->setExplicitMemberwiseInit(true);
      }
      else {
         SP.diagnose(err_generic_error,
                     "'memberwise init' declaration cannot"
                     " appear here",
                     currentTok().getSourceLoc());
      }

      return ParseError();
   }

   bool IsFallible = lookahead().is(tok::question);
   if (IsFallible) {
      advance();
   }

   auto params = tryParseTemplateParameters();
   advance();

   SourceLocation varargLoc;
   auto args = parseFuncArgs(varargLoc);

   auto Init
       = InitDecl::Create(Context, CurDeclAttrs.Access, Loc, args, move(params),
                          nullptr, DeclarationName(), IsFallible);

   std::vector<ParsedConstraint> Constraints;
   if (lookahead().is(Ident_where)) {
      advance();
      parseDeclConstraints(Constraints);
   }

   Context.setParsedConstraints(Init, move(Constraints));

   Statement* body = nullptr;
   if (lexer->lookahead().is(tok::open_brace)) {
      if (ParsingProtocol) {
         SP.diagnose(Init, err_definition_in_protocol, 3 /*initializer*/,
                     currentTok().getSourceLoc());
      }

      DeclContextRAII declContextRAII(*this, Init);
      EnterFunctionScope EF(*this);

      body = parseCompoundStmt().tryGetStatement();
   }
   else if (!ParsingProtocol) {
      SP.diagnose(Init, err_generic_error, "initializer must have a definition",
                  Init->getSourceLoc());
   }

   Init->setBody(body);
   Init->setVararg(varargLoc.isValid());

   return ActOnDecl(Init);
}

ParseResult Parser::parseDestrDecl()
{
   auto Loc = currentTok().getSourceLoc();
   advance();

   SourceLocation varargLoc;
   auto args = parseFuncArgs(varargLoc);

   if (!args.empty()) {
      SP.diagnose(err_deinit_args, currentTok().getSourceLoc());
      args.clear();
   }

   auto Deinit = DeinitDecl::Create(Context, Loc, nullptr, args);
   if (ParsingProtocol) {
      SP.diagnose(Deinit, err_may_not_appear_in_protocol, Deinit,
                  currentTok().getSourceLoc());
   }

   Statement* body = nullptr;
   if (lexer->lookahead().is(tok::open_brace)) {
      DeclContextRAII declContextRAII(*this, Deinit);
      EnterFunctionScope EF(*this);

      body = parseCompoundStmt().tryGetStatement();
   }

   Deinit->setReturnType(SourceType(Context.getVoidType()));
   Deinit->setBody(body);

   return ActOnDecl(Deinit);
}

ParseResult Parser::parseFieldDecl()
{
   bool isConst = currentTok().is(tok::kw_let);
   advance();

   bool Variadic = false;
   if (currentTok().is(tok::triple_period)) {
      Variadic = true;
      advance();
   }

   if (currentTok().getKind() != tok::ident) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                  currentTok().toString(), true, "identifier");

      return ParseError();
   }

   auto Loc = currentTok().getSourceLoc();
   IdentifierInfo* fieldName = currentTok().getIdentifierInfo();

   SourceType typeref;
   Token next = lookahead();

   SourceLocation ColonLoc;
   if (next.is(tok::colon)) {
      advance();
      ColonLoc = consumeToken();

      auto TypeRes = parseType(true);
      if (TypeRes)
         typeref = TypeRes.get();
   }
   else {
      typeref = SourceType(Context.getAutoType());
   }

   // Rename the field to _fieldName and synthesize a property that modifies it.
   FieldDecl *field;
   if (lookahead().is(tok::open_brace)) {
      AccessorInfo Info;
      SmallVector<FuncArgDecl*, 2> Args{nullptr};
      parseAccessor(Loc, fieldName, typeref, CurDeclAttrs.StaticLoc.isValid(),
                    Args, Info, true);

      if ((Info.GetterMethod && Info.GetterMethod->getBody())
      || (Info.SetterMethod && Info.SetterMethod->getBody())) {
         SourceLocation loc = (Info.GetterMethod && Info.GetterMethod->getBody())
            ? Info.GetterMethod->getSourceLoc()
            : Info.SetterMethod->getSourceLoc();

         SP.diagnose(err_generic_error, "field accessor cannot have a body", loc);
      }

      std::string newFieldName = "_";
      newFieldName += fieldName->getIdentifier();

      auto *newFieldNameIdent = &Idents.get(newFieldName);
      field = FieldDecl::Create(Context, CurDeclAttrs.Access, Loc, ColonLoc,
                                newFieldNameIdent, typeref,
                                CurDeclAttrs.StaticLoc, isConst, nullptr);

      // Synthesize the return statement for the 'get' accessor.
      if (Info.GetterMethod) {
         auto loc = Info.GetterMethod->getSourceLoc();
         auto *self = DeclRefExpr::Create(Context, Info.GetterMethod->getArgs()[0], loc);
         auto *retExpr = MemberRefExpr::Create(Context, self, field, loc);
         auto *ret = ReturnStmt::Create(Context, loc, retExpr);
         auto *cs = CompoundStmt::Create(Context, ret, false, loc, loc);
         Info.GetterMethod->setBody(cs);
      }

      // Synthesize the assign statement for the 'set' accessor.
      if (Info.SetterMethod) {
         auto loc = Info.SetterMethod->getSourceLoc();
         if (isConst) {
            SP.diagnose(err_generic_error, "'let' field cannot have a 'set' accessor", loc);
         }

         auto *self = DeclRefExpr::Create(Context, Info.SetterMethod->getArgs()[0], loc);
         auto *lhs = MemberRefExpr::Create(Context, self, field, loc);
         auto *rhs = DeclRefExpr::Create(Context, Info.SetterMethod->getArgs()[1], loc);
         auto *assign = AssignExpr::Create(Context, loc, lhs, rhs);
         auto *cs = CompoundStmt::Create(Context, assign, false, loc, loc);
         Info.SetterMethod->setBody(cs);
      }

      SourceRange SR(Loc, currentTok().getSourceLoc());
      auto prop = PropDecl::Create(Context, CurDeclAttrs.Access, SR, fieldName,
                                   typeref, CurDeclAttrs.StaticLoc, false,
                                   Info.GetterMethod, Info.SetterMethod);

      prop->setSynthesized(true);
      ActOnDecl(prop);
   }
   else {
      field = FieldDecl::Create(Context, CurDeclAttrs.Access, Loc, ColonLoc,
                                fieldName, typeref, CurDeclAttrs.StaticLoc,
                                isConst, nullptr);
   }

   field->setVariadic(Variadic);

   // optional default value
   if (lookahead().is(tok::equals)) {
      advance();
      advance();
      field->setValue(parseExprSequence().tryGetExpr());
   }

   return ActOnDecl(field);
}

void Parser::parseAccessor(SourceLocation Loc, IdentifierInfo* Name,
                           SourceType Type, bool IsStatic,
                           SmallVectorImpl<FuncArgDecl*>& Args,
                           AccessorInfo& Info, bool IsProperty)
{
   Token next = lookahead();
   if (next.is(tok::arrow_double)) {
      if (ParsingProtocol) {
         SP.diagnose(err_definition_in_protocol, IsProperty ? 1 : 2,
                     currentTok().getSourceLoc());
      }

      advance();
      auto ArrowLoc = consumeToken();
      if (Info.GetterMethod) {
         SP.diagnose(err_duplicate_getter_setter,
                     currentTok().getSourceLoc(), 0);
      }

      DeclarationName DN;
      if (IsProperty) {
         DN = Context.getDeclNameTable().getAccessorName(
            *Name, DeclarationName::Getter);
      }
      else {
         DN = Context.getDeclNameTable().getSubscriptName(
            DeclarationName::SubscriptKind::Getter);
      }

      Args[0] = SP.MakeSelfArg(ArrowLoc);
      Info.GetterMethod = MethodDecl::Create(
         Context, AccessSpecifier::Default, ArrowLoc, DN, Type, Args, {},
         nullptr, IsStatic);

      Info.GetterMethod->setSynthesized(true);

      {
         DeclContextRAII DCR(*this, Info.GetterMethod);
         EnterFunctionScope EF(*this);

         auto retExpr = parseExprSequence().tryGetExpr();
         if (retExpr != nullptr) {
            auto *ret = ReturnStmt::Create(Context, ArrowLoc, retExpr);
            auto *cs = CompoundStmt::Create(Context, ret, false, ArrowLoc, ArrowLoc);
            Info.GetterMethod->setBody(cs);
         }
      }
   }
   else if (next.is(tok::open_brace)) {
      advance();

      AccessSpecifier AS = AccessSpecifier::Default;
      bool SawGetOrSet = false;

      next = lookahead();
      while (!next.is(tok::close_brace)) {
         switch (next.getKind()) {
         case tok::kw_public:
         case tok::kw_private:
         case tok::kw_protected:
         case tok::kw_fileprivate:
         case tok::kw_internal:
            if (AS != AccessSpecifier::Default) {
               SP.diagnose(err_duplicate_access_spec, next.getSourceLoc());
            }

            AS = tokenToAccessSpec(next.getKind());
            break;
         case tok::semicolon:
            advance();
            next = lookahead();
            continue;
         default:
            break;
         }

         bool NonMutating = false;
         if (next.is(Ident_nonmutating)) {
            NonMutating = true;
            advance();

            next = lookahead();

            if (!next.oneOf(Ident_get, Ident_read, Ident_set, Ident_write)) {
               SP.diagnose(
                   err_generic_error,
                   "expected 'get', 'set' or 'read' after 'nonmutating'",
                   next.getSourceLoc());
            }
         }

         if (next.oneOf(Ident_set, Ident_write)) {
            if (next.is(Ident_write)) {
               Info.IsReadWrite = true;
            }
            else if (Info.IsReadWrite) {
               SP.diagnose(err_generic_error,
                           "expected 'write' to go along with 'read'",
                           next.getSourceLoc());
            }

            auto SetLoc = consumeToken();
            if (Info.SetterMethod) {
               SP.diagnose(err_duplicate_getter_setter,
                           currentTok().getSourceLoc(), 1);
            }

            SawGetOrSet = true;

            IdentifierInfo* NewValName = nullptr;
            if (lexer->lookahead().is(tok::open_paren)) {
               advance();
               advance();
               if (currentTok().getKind() != tok::ident) {
                  SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                              currentTok().toString(), true, "identifier");

                  skipUntilProbableEndOfStmt();
                  return;
               }

               NewValName = currentTok().getIdentifierInfo();
               advance();

               if (!currentTok().is(tok::close_paren)) {
                  SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                              currentTok().toString(), true, "')'");
               }
            }

            if (!NewValName) {
               NewValName = &Context.getIdentifiers().get("newVal");
            }

            DeclarationName DN;
            if (IsProperty) {
               DN = Context.getDeclNameTable().getAccessorName(
                   *Name, DeclarationName::Setter);
            }
            else {
               DN = Context.getDeclNameTable().getSubscriptName(
                   DeclarationName::SubscriptKind::Setter);
            }

            Expression* DefaultVal = nullptr;
            if (!IsProperty) {
               // The argument needs a dummy default value for overload
               // resolution.
               DefaultVal = BuiltinExpr::Create(Context, Type);
            }

            auto* NewValArg = FuncArgDecl::Create(
                Context, SetLoc, SetLoc, NewValName, nullptr,
                ArgumentConvention::Owned, Type, DefaultVal, false);

            NewValArg->setSynthesized(true);

            Args[0] = SP.MakeSelfArg(SetLoc);
            Args.push_back(NewValArg);

            Info.SetterMethod = MethodDecl::Create(
                Context, AS, SetLoc, DN, SourceType(Context.getEmptyTupleType()),
                Args, {}, nullptr, IsStatic);

            Info.SetterMethod->setSynthesized(true);
            Info.SetterMethod->setMutating(!NonMutating);

            if (lexer->lookahead().is(tok::open_brace)) {
               if (ParsingProtocol) {
                  SP.diagnose(err_definition_in_protocol, IsProperty ? 1 : 2,
                              currentTok().getSourceLoc());
               }

               DeclContextRAII DCR(*this, Info.SetterMethod);
               EnterFunctionScope EF(*this);

               Info.SetterMethod->setBody(
                   parseCompoundStmt().tryGetStatement());
            }
         }
         else if (next.oneOf(Ident_get, Ident_read)) {
            if (next.is(Ident_read)) {
               Info.IsReadWrite = true;
            }
            else if (Info.IsReadWrite) {
               SP.diagnose(err_generic_error,
                           "expected 'read' to go along with 'write'",
                           next.getSourceLoc());
            }

            auto GetLoc = consumeToken();
            if (Info.GetterMethod) {
               SP.diagnose(err_duplicate_getter_setter,
                           currentTok().getSourceLoc(), 0);
            }

            SawGetOrSet = true;

            DeclarationName DN;
            if (IsProperty) {
               DN = Context.getDeclNameTable().getAccessorName(
                   *Name, DeclarationName::Getter);
            }
            else {
               DN = Context.getDeclNameTable().getSubscriptName(
                   DeclarationName::SubscriptKind::Getter);
            }

            Args[0] = SP.MakeSelfArg(GetLoc);
            Info.GetterMethod = MethodDecl::Create(
                Context, AS, GetLoc, DN, Type, Args, {}, nullptr, IsStatic);

            Info.GetterMethod->setSynthesized(true);

            if (lexer->lookahead().is(tok::open_brace)) {
               if (ParsingProtocol) {
                  SP.diagnose(err_definition_in_protocol, IsProperty ? 1 : 2,
                              currentTok().getSourceLoc());
               }

               DeclContextRAII DCR(*this, Info.GetterMethod);
               EnterFunctionScope EF(*this);

               Info.GetterMethod->setBody(
                   parseCompoundStmt().tryGetStatement());
            }
         }
         else if (!SawGetOrSet) {
            assert(!Info.GetterMethod
                   && "found getter but didn't "
                      "update SawGetOrSet!");
            assert(currentTok().is(tok::open_brace)
                   && "shouldn't get here "
                      "otherwise");

            lexer->backtrack();

            DeclarationName DN;
            if (IsProperty) {
               DN = Context.getDeclNameTable().getAccessorName(
                   *Name, DeclarationName::Getter);
            }
            else {
               DN = Context.getDeclNameTable().getSubscriptName(
                   DeclarationName::SubscriptKind::Getter);
            }

            Args[0] = SP.MakeSelfArg(Loc);
            Info.GetterMethod = MethodDecl::Create(Context, AS, Loc, DN, Type,
                                                   Args, {}, nullptr, IsStatic);

            Info.GetterMethod->setSynthesized(true);

            DeclContextRAII DCR(*this, Info.GetterMethod);
            EnterFunctionScope EF(*this);

            Info.GetterMethod->setBody(parseCompoundStmt().tryGetStatement());

            break;
         }
         else {
            SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                        currentTok().toString(), true, "'get' or 'set'");
            break;
         }

         next = lookahead();
         AS = AccessSpecifier::Default;
      }

      if (SawGetOrSet) {
         advance();
      }
   }
}

ParseResult Parser::parsePropDecl()
{
   auto BeginLoc = currentTok().getSourceLoc();
   advance();

   if (currentTok().getKind() != tok::ident) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                  currentTok().toString(), true, "identifier");

      return ParseError();
   }

   IdentifierInfo* fieldName = currentTok().getIdentifierInfo();
   SourceType typeref;
   advance();

   if (!currentTok().is(tok::colon)) {
      SP.diagnose(err_prop_must_have_type, currentTok().getSourceLoc(),
                  0 /*property*/);
   }
   else {
      advance();
      typeref = parseType().get();
   }

   AccessorInfo Info;
   SmallVector<FuncArgDecl*, 2> Args{nullptr};
   parseAccessor(BeginLoc, fieldName, typeref, CurDeclAttrs.StaticLoc.isValid(),
                 Args, Info, true);

   SourceRange SR(BeginLoc, currentTok().getSourceLoc());
   auto prop
       = PropDecl::Create(Context, CurDeclAttrs.Access, SR, fieldName, typeref,
                          CurDeclAttrs.StaticLoc, Info.IsReadWrite,
                          Info.GetterMethod, Info.SetterMethod);

   if (!Info.GetterMethod && !Info.SetterMethod) {
      SP.diagnose(prop, err_prop_must_have_get_or_set, BeginLoc,
                  0 /*property*/);
   }

   return ActOnDecl(prop);
}

ParseResult Parser::parseSubscriptDecl()
{
   auto BeginLoc = currentTok().getSourceLoc();
   SourceType typeref;
   advance();

   SourceLocation varargLoc;
   auto Args = parseFuncArgs(varargLoc, true);

   if (varargLoc.isValid()) {
      SP.diagnose(err_vararg_not_valid, varargLoc, 0 /*subscript*/);
   }

   advance();
   if (!currentTok().is(tok::arrow_single)) {
      SP.diagnose(err_prop_must_have_type, currentTok().getSourceLoc(),
                  1 /*subscript*/);
   }
   else {
      advance();
      typeref = parseType().get();
   }

   AccessorInfo Info;
   Args.insert(Args.begin(), nullptr);
   parseAccessor(BeginLoc, nullptr, typeref, CurDeclAttrs.StaticLoc.isValid(),
                 Args, Info, false);

   if (!Info.GetterMethod && !Info.SetterMethod) {
      SP.diagnose(err_prop_must_have_get_or_set, BeginLoc, 1 /*subscript*/);
   }

   SourceRange SR(BeginLoc, currentTok().getSourceLoc());
   auto SD = SubscriptDecl::Create(Context, CurDeclAttrs.Access, SR, typeref,
                                   Info.GetterMethod, Info.SetterMethod);

   return ActOnDecl(SD);
}

ParseResult Parser::parseAssociatedType()
{
   if (!ParsingProtocol) {
      SP.diagnose(err_generic_error,
                  "'associatedType' declarations may only appear in protocols, "
                  "implement them with an alias instead",
                  currentTok().getSourceLoc());
   }

   auto Loc = currentTok().getSourceLoc();
   consumeToken(tok::kw_associatedType);

   auto name = currentTok().getIdentifierInfo();

   SourceType covariance;
   if (lookahead().is(tok::colon)) {
      advance();
      advance();

      if (currentTok().is(tok::question)) {
         covariance = Context.getUnknownAnyTy();
      }
      else {
         covariance = parseType().tryGet();
      }
   }

   SourceType actualType;
   if (lookahead().is(tok::equals)) {
      advance();
      advance();

      actualType = parseType().tryGet();
   }

   std::vector<ParsedConstraint> Constraints;
   if (lookahead().is(Ident_where)) {
      advance();
      parseDeclConstraints(Constraints);
   }

   NamedDecl* decl;
   if (!ParsingProtocol) {
      // Pretend the programmer used an alias.
      SourceType metaTy;
      if (actualType) {
         metaTy = actualType;
      }
      else {
         metaTy = covariance;
      }

      decl = AliasDecl::Create(Context, Loc, AccessSpecifier::Public, name,
                               metaTy, nullptr, {});

      decl->setIsInvalid(true);
   }
   else {
      decl = AssociatedTypeDecl::Create(Context, Loc, name, actualType,
                                        covariance);
   }

   Context.setParsedConstraints(decl, move(Constraints));
   return ActOnDecl(decl);
}

DeclarationName Parser::parseOperatorName(FixKind Fix, bool& isCastOp)
{
   auto MakeDeclName = [&](llvm::StringRef Ident) {
      auto& II = Context.getIdentifiers().get(Ident);

      if (Fix == FixKind::Infix) {
         Context.registerInfixOperator(&II);
         return Context.getDeclNameTable().getInfixOperatorName(II);
      }

      if (Fix == FixKind::Prefix) {
         Context.registerPrefixOperator(&II);
         return Context.getDeclNameTable().getPrefixOperatorName(II);
      }

      Context.registerPostfixOperator(&II);
      return Context.getDeclNameTable().getPostfixOperatorName(II);
   };

   if (Fix == FixKind ::Postfix && currentTok().is(tok::open_paren)
       && lexer->lookahead().is(tok::close_paren)) {
      advance();

      return MakeDeclName("()");
   }

   if (currentTok().is(Ident_as)) {
      isCastOp = true;

      if (lookahead(false, true).is(tok::question)) {
         advance(false, true);
         return MakeDeclName("as?");
      }
      if (lookahead(false, true).is(tok::exclaim)) {
         advance(false, true);
         return MakeDeclName("as!");
      }

      return MakeDeclName("as");
   }

   if (currentTok().oneOf(tok::ident, tok::op_ident)) {
      return MakeDeclName(currentTok().getIdentifier());
   }

   if (currentTok().is_operator()) {
      return MakeDeclName(currentTok().toString());
   }

   SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
               currentTok().toString(), true, "operator");

   return DeclarationName();
}

ParseResult Parser::parseMethodDecl()
{
   auto DefLoc = currentTok().getSourceLoc();
   advance();

   bool IsAbstract = false;
   bool IsMutating = false;
   bool IsOperator = false;
   bool IsVirtual = false;
   bool IsOverride = false;

   FixKind Fix;
   bool done = false;
   while (!done) {
      if (currentTok().is(Ident_virtual)) {
         IsVirtual = true;
         advance();
      }
      else if (currentTok().is(Ident_override)) {
         IsOverride = true;
         advance();
      }
      else {
         switch (currentTok().getKind()) {
         case tok::kw_mutating:
            IsMutating = true;
            advance();
            break;
         case tok::kw_abstract:
            IsAbstract = true;
            advance();
            break;
         case tok::ident:
            if (currentTok().oneOf(Ident_prefix, Ident_postfix, Ident_infix)
                && validOperatorFollows()) {
               Fix = tokenToFix(currentTok());
               IsOperator = true;
               advance();
            }

            LLVM_FALLTHROUGH;
         default:
            done = true;
            break;
         }
      }
   }

   bool isConversionOp = false;
   SourceType returnType;
   DeclarationName methodName;

   if (IsOperator) {
      methodName = parseOperatorName(Fix, isConversionOp);
   }
   else {
      methodName = currentTok().getIdentifierInfo();
   }

   auto templateParams = tryParseTemplateParameters();
   advance();

   SmallVector<FuncArgDecl*, 4> args;
   args.push_back(FuncArgDecl::Create(Context, SourceLocation(), DefLoc,
                                      DeclarationName(Ident_self), nullptr,
                                      ArgumentConvention::Default, SourceType(),
                                      nullptr, false, false, /*isSelf=*/true));

   SourceLocation varargLoc;
   parseFuncArgs(varargLoc, args, IsOperator);

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

   MethodDecl* methodDecl;
   if (isConversionOp) {
      methodDecl = MethodDecl::CreateConversionOp(
          Context, CurDeclAttrs.Access, DefLoc, returnType, args,
          move(templateParams), nullptr);
   }
   else {
      methodDecl = MethodDecl::Create(
          Context, CurDeclAttrs.Access, DefLoc, methodName, returnType, args,
          move(templateParams), nullptr, CurDeclAttrs.StaticLoc);
   }

   CompoundStmt* body = nullptr;
   if (lookahead().is(tok::open_brace)) {
      if (IsAbstract) {
         SP.diagnose(methodDecl, err_abstract_definition,
                     methodDecl->getDeclName(), currentTok().getSourceLoc());
      }
      else if (ParsingProtocol) {
         SP.diagnose(methodDecl, err_definition_in_protocol, 0 /*method*/,
                     currentTok().getSourceLoc());
      }

      DeclContextRAII declContextRAII(*this, methodDecl);
      EnterFunctionScope EF(*this);

      body = parseCompoundStmt().tryGetStatement<CompoundStmt>();

      if (IsAbstract)
         body = nullptr;
   }
   else if (!ParsingProtocol && !IsAbstract) {
      SP.diagnose(methodDecl, err_generic_error,
                  "method must have a definition", methodDecl->getSourceLoc());
   }

   methodDecl->setVararg(varargLoc.isValid());
   methodDecl->setBody(body);
   methodDecl->setMutating(IsMutating);
   methodDecl->setIsVirtual(IsVirtual | IsAbstract);
   methodDecl->setIsOverride(IsOverride);
   methodDecl->setThrows(Throws);
   methodDecl->setAsync(Async);
   methodDecl->setUnsafe(Unsafe);
   methodDecl->setAbstract(IsAbstract);

   Context.setParsedConstraints(methodDecl, move(Constraints));
   return ActOnDecl(methodDecl);
}

ParseResult Parser::parseEnumCase()
{
   auto CaseLoc = currentTok().getSourceLoc();
   consumeToken(tok::kw_case);

   if (currentTok().getKind() != tok::ident) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                  currentTok().toString(), true, "identifier");

      return ParseError();
   }

   auto caseName = currentTok().getIdentifierInfo();
   SmallVector<FuncArgDecl*, 2> associatedTypes;

   if (lexer->lookahead().is(tok::open_paren)) {
      advance();

      SourceLocation varargLoc;
      associatedTypes = parseFuncArgs(varargLoc, /*FIXME*/ true);
   }

   EnumCaseDecl* caseDecl;
   StaticExpr* Expr = nullptr;

   if (lexer->lookahead().is(tok::equals)) {
      advance();
      advance();

      Expr = StaticExpr::Create(Context, parseExprSequence().tryGetExpr());
   }

   caseDecl = EnumCaseDecl::Create(Context, CurDeclAttrs.Access, CaseLoc,
                                   CaseLoc, caseName, Expr, associatedTypes);

   return ActOnDecl(caseDecl);
}

} // namespace parse
} // namespace cdot