//
// Created by Jonas Zell on 10.10.17.
//

#include "Parser.h"

#include "AST/Decl.h"
#include "Basic/IdentifierInfo.h"
#include "Lex/Lexer.h"
#include "Module/Module.h"
#include "Message/Diagnostics.h"
#include "Sema/SemaPass.h"
#include "Support/Casting.h"

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

   RecordDecl *decl;
   switch (kind) {
   case tok::kw_struct:
      decl = StructDecl::Create(Context, CurDeclAttrs.Access, Loc,
                                head.recordName,
                                move(head.conformances),
                                move(head.templateParams));
      break;
   case tok::kw_class:
      decl = ClassDecl::Create(Context, CurDeclAttrs.Access, Loc,
                               head.recordName,
                               move(head.conformances),
                               move(head.templateParams),
                               head.parentClass, CurDeclAttrs.Abstract);
      break;
   case tok::kw_enum:
      decl = EnumDecl::Create(Context, CurDeclAttrs.Access, Loc,
                              head.recordName,
                              move(head.conformances),
                              move(head.templateParams),
                              head.enumRawType);
      break;
   case tok::kw_union:
      decl = UnionDecl::Create(Context, CurDeclAttrs.Access, Loc,
                               head.recordName,
                               move(head.conformances),
                               move(head.templateParams));
      break;
   case tok::kw_protocol:
      decl = ProtocolDecl::Create(Context, CurDeclAttrs.Access, Loc,
                                  head.recordName,
                                  move(head.conformances),
                                  move(head.templateParams));
      break;
   default:
      llvm_unreachable("not a record decl!");
   }

   Context.setConstraints(decl, head.constraints);

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

   SmallVector<DeclConstraint*, 4> Constraints;
   if (lookahead().is(tok::kw_where)) {
      advance();
      parseDeclConstraints(Constraints);
   }

   ExtensionDecl *decl = ExtensionDecl::Create(Context, CurDeclAttrs.Access,Loc,
                                               ExtendedType, Conformances);

   Context.setConstraints(decl, Constraints);
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
      if (!parseNextDecl()) {
         if (!skipUntilNextDeclOrClosingBrace())
            break;
      }

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
   bool AccessValid = true;

   tok::TokenType DeclKind = currentTok().getKind();
   ParseResult NextDecl;
   if (currentTok().is(Ident_deinit)) {
      NextDecl = parseDestrDecl();
   }
   else if (currentTok().is(Ident_subscript)) {
      NextDecl = parseSubscriptDecl();
   }
   else if (currentTok().is(Ident_memberwise)) {
      NextDecl = parseConstrDecl();
   }
   else switch (DeclKind) {
   case tok::kw_public:
   case tok::kw_private:
   case tok::kw_fileprivate:
   case tok::kw_protected:
   case tok::kw_internal:
      NextDecl = parseCompoundDecl(false);
      break;
   case tok::ident: {
      if (currentTok().is(Ident_macro))
         return parseMacro();

      goto case_bad_token;
   }
   case tok::open_brace:
      return parseCompoundDecl(false, false);
   case tok::kw_static:
      AccessValid = false;
      switch (lookahead().getKind()) {
      case tok::kw_if:
         NextDecl = parseStaticIfDecl(); break;
      case tok::kw_for:
         NextDecl = parseStaticForDecl(); break;
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
         Expression, Statement, Type, Decl,
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
      break;
   case tok::kw_let:
   case tok::kw_var:
      StaticValid = true;
      NextDecl = parseFieldDecl();
      break;
   case tok::kw_prop:
      StaticValid = true;
      NextDecl = parsePropDecl();
      break;
   case tok::kw_def:
      StaticValid = true;
      NextDecl = parseMethodDecl();
      break;
   case tok::kw_init:
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
   else switch (DeclKind) {
   case tok::kw_associatedType: selector = 0; break;
   case tok::kw_static: {
      if (lookahead().getKind() == tok::kw_if)
         selector = 1;
      else
         selector = 2;

      break;
   }
   case tok::kw_static_assert: selector = 3; break;
   case tok::kw_static_print: selector = 4; break;
   case tok::kw_typedef: selector = 5; break;
   case tok::kw_alias: selector = 6; break;
   case tok::kw_init: selector = 7; break;
   case tok::kw_case: selector = 9; break;
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

   if (!AccessValid && CurDeclAttrs.AccessLoc) {
      SP.diagnose(err_cannot_have_access_spec, CurDeclAttrs.AccessLoc,selector);
   }

   auto *D = NextDecl.getDecl();
   if (auto ND = dyn_cast<NamedDecl>(D))
      ND->setAccessLoc(CurDeclAttrs.AccessLoc);

   return D;
}

void Parser::parseClassHead(RecordHead &Head)
{
   assert(currentTok().oneOf(tok::kw_struct, tok::kw_enum, tok::kw_class,
                             tok::kw_protocol) && "not a record decl!");

   advance();

   if (currentTok().getKind() != tok::ident) {
      SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                  currentTok().toString(), true, "identifier");

      skipUntilProbableEndOfStmt();
      return;
   }

   Head.recordName = currentTok().getIdentifierInfo();

   if (lexer->lookahead().is(tok::open_paren)) {
      advance();
      advance();

      Head.enumRawType = parseType().get();

      advance();
      if (!currentTok().is(tok::close_paren)) {
         SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(), 
                     currentTok().toString(), true, "')'");
      }
   }

   auto params = tryParseTemplateParameters();
   Head.templateParams.insert(Context, Head.templateParams.end(),
                              params.begin(),
                              params.end());

   auto next = lookahead();
   while (!next.is(tok::open_brace)) {
      if (next.is(tok::colon)) {
         if (Head.parentClass) {
            SP.diagnose(err_multiple_inheritance, currentTok().getSourceLoc());
         }

         advance();
         advance();

         Head.parentClass = parseType().tryGet();
      }
      else if (next.is(Ident_with)) {
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
      else if (next.is(tok::kw_where)) {
         advance();
         advance();

         Head.constraints.push_back(StaticExpr::Create(Context,
            parseExprSequence(DefaultFlags & ~F_AllowBraceClosure).tryGetExpr()));
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

      auto Decl = InitDecl::CreateMemberwise(Context, CurDeclAttrs.Access, Loc);
      return ActOnDecl(Decl);
   }

   auto params = tryParseTemplateParameters();
   advance();

   bool IsFallible = currentTok().is(tok::question);
   if (IsFallible)
      advance();

   SourceLocation varargLoc;
   std::vector<FuncArgDecl*> args;
   parseFuncArgs(varargLoc, args);

   auto Init =  InitDecl::Create(Context, CurDeclAttrs.Access, Loc, args,
                                 move(params), nullptr, DeclarationName(),
                                 IsFallible);

   llvm::TinyPtrVector<StaticExpr*> constraints;
   while (lookahead().is(tok::kw_where)) {
      advance();
      advance();

      constraints.push_back(StaticExpr::Create(Context,
         parseExprSequence(DefaultFlags & ~F_AllowBraceClosure).tryGetExpr()));
   }

   Context.setConstraints(Init, constraints);

   Statement* body = nullptr;
   if (lexer->lookahead().is(tok::open_brace)) {
      if (ParsingProtocol) {
         SP.diagnose(Init, err_definition_in_protocol, 3 /*initializer*/,
                     currentTok().getSourceLoc());
      }

      DeclContextRAII declContextRAII(*this, Init);
      body = parseCompoundStmt().tryGetStatement();
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

   std::vector<FuncArgDecl*> args;
   parseFuncArgs(varargLoc, args);

   if (!args.empty()) {
      SP.diagnose(err_deinit_args, currentTok().getSourceLoc());
      args.clear();
   }

   auto Deinit = DeinitDecl::Create(Context, Loc, nullptr, args);
   if (ParsingProtocol) {
      SP.diagnose(Deinit, err_may_not_appear_in_protocol,
                  Deinit->getSpecifierForDiagnostic(),
                  currentTok().getSourceLoc());
   }

   Statement* body = nullptr;
   if (lexer->lookahead().is(tok::open_brace)) {
      DeclContextRAII declContextRAII(*this, Deinit);
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
   IdentifierInfo *fieldName = currentTok().getIdentifierInfo();

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

   auto field = FieldDecl::Create(Context, CurDeclAttrs.Access, Loc, ColonLoc,
                                  fieldName, typeref, CurDeclAttrs.StaticLoc,
                                  isConst, nullptr);

   field->setVariadic(Variadic);

   if (lookahead().is(tok::open_brace)) {
      AccessorInfo Info;
      parseAccessor(Info, true);

      if (Info.GetterAccess == AccessSpecifier::Default)
         Info.GetterAccess = AccessSpecifier::Public;

      if (Info.SetterAccess == AccessSpecifier::Default)
         Info.SetterAccess = AccessSpecifier::Public;

      SourceRange SR(Loc, currentTok().getSourceLoc());

      auto prop =
         PropDecl::Create(Context, CurDeclAttrs.Access, SR, fieldName,
                          typeref, CurDeclAttrs.StaticLoc,
                          Info.HasGetter, Info.HasSetter,
                          Info.GetterAccess, Info.SetterAccess,
                          Info.GetterBody.tryGetStatement<CompoundStmt>(),
                          Info.SetterBody.tryGetStatement<CompoundStmt>(),
                          Info.NewValName);

      prop->setSynthesized(true);
      field->setAccessor(prop);
   }

   // optional default value
   if (lookahead().is(tok::equals)) {
      advance();
      advance();
      field->setValue(parseExprSequence().tryGetExpr());
   }

   return ActOnDecl(field);
}

void Parser::parseAccessor(AccessorInfo &Info, bool IsProperty)
{
   Token next = lookahead();
   if (next.is(tok::open_brace)) {
      advance();

      AccessSpecifier AS = AccessSpecifier::Default;
      bool SawGetOrSet = false;

      next = lookahead();
      while (!next.is(tok::close_brace)) {
         switch (next.getKind()) {
         case tok::kw_public: case tok::kw_private: case tok::kw_protected:
         case tok::kw_fileprivate: case tok::kw_internal:
            if (AS != AccessSpecifier::Default) {
               SP.diagnose(err_duplicate_access_spec, next.getSourceLoc());
            }

            AS = tokenToAccessSpec(next.getKind());
            break;
         default:
            break;
         }

         if (next.is(Ident_set)) {
            advance();

            if (Info.HasSetter) {
               SP.diagnose(err_duplicate_getter_setter,
                           currentTok().getSourceLoc(), 1);
            }

            SawGetOrSet = true;
            Info.HasSetter = true;
            Info.GetterAccess = AS;

            if (lexer->lookahead().is(tok::open_paren)) {
               advance();
               advance();
               if (currentTok().getKind() != tok::ident) {
                  SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                              currentTok().toString(), true, "identifier");

                  skipUntilProbableEndOfStmt();
                  return;
               }

               Info.NewValName = currentTok().getIdentifierInfo();
               advance();

               if (!currentTok().is(tok::close_paren)) {
                  SP.diagnose(err_unexpected_token, currentTok().getSourceLoc(),
                              currentTok().toString(), true, "')'");
               }
            }

            if (lexer->lookahead().is(tok::open_brace)) {
               if (ParsingProtocol) {
                  SP.diagnose(err_definition_in_protocol, IsProperty ? 1 : 2,
                              currentTok().getSourceLoc());
               }

               Info.SetterBody = parseCompoundStmt();
            }
         }
         else if (next.is(Ident_get)) {
            advance();

            if (Info.HasGetter) {
               SP.diagnose(err_duplicate_getter_setter,
                           currentTok().getSourceLoc(), 0);
            }

            SawGetOrSet = true;
            Info.HasGetter = true;
            Info.SetterAccess = AS;

            if (lexer->lookahead().is(tok::open_brace)) {
               if (ParsingProtocol) {
                  SP.diagnose(err_definition_in_protocol, IsProperty ? 1 : 2,
                              currentTok().getSourceLoc());
               }

               Info.GetterBody = parseCompoundStmt();
            }
         }
         else if (!SawGetOrSet) {
            assert(!Info.HasGetter && "found getter but didn't "
                                      "update SawGetOrSet!");
            assert(currentTok().is(tok::open_brace) && "shouldn't get here "
                                                       "otherwise");

            lexer->backtrack();

            Info.HasGetter = true;
            Info.GetterAccess = AS;
            Info.GetterBody = parseCompoundStmt();

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

      if (SawGetOrSet)
         advance();
   }

   if (!Info.NewValName)
      Info.NewValName = &Context.getIdentifiers().get("newVal");
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

   IdentifierInfo *fieldName = currentTok().getIdentifierInfo();
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
   parseAccessor(Info, true);

   if (!Info.HasGetter && !Info.HasSetter) {
      SP.diagnose(err_prop_must_have_get_or_set, BeginLoc, 0 /*property*/);
   }

   if (Info.GetterAccess == AccessSpecifier::Default)
      Info.GetterAccess = CurDeclAttrs.Access;

   if (Info.SetterAccess == AccessSpecifier::Default)
      Info.SetterAccess = CurDeclAttrs.Access;

   SourceRange SR(BeginLoc, currentTok().getSourceLoc());
   auto prop = PropDecl::Create(Context, CurDeclAttrs.Access, SR, fieldName,
                                typeref, CurDeclAttrs.StaticLoc,
                                Info.HasGetter, Info.HasSetter,
                                Info.GetterAccess, Info.SetterAccess,
                                Info.GetterBody.tryGetStatement<CompoundStmt>(),
                                Info.SetterBody.tryGetStatement<CompoundStmt>(),
                                Info.NewValName);

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
   parseAccessor(Info, false);

   if (!Info.HasGetter && !Info.HasSetter) {
      SP.diagnose(err_prop_must_have_get_or_set, BeginLoc, 1 /*subscript*/);
   }

   if (Info.GetterAccess == AccessSpecifier::Default)
      Info.GetterAccess = CurDeclAttrs.Access;

   if (Info.SetterAccess == AccessSpecifier::Default)
      Info.SetterAccess = CurDeclAttrs.Access;

   SourceRange SR(BeginLoc, currentTok().getSourceLoc());
   auto SD = SubscriptDecl::Create(Context, CurDeclAttrs.Access, SR, Args,
                                typeref, Info.HasGetter, Info.HasSetter,
                                Info.GetterAccess, Info.SetterAccess,
                                Info.GetterBody.tryGetStatement<CompoundStmt>(),
                                Info.SetterBody.tryGetStatement<CompoundStmt>(),
                                Info.NewValName);

   return ActOnDecl(SD);
}

ParseResult Parser::parseAssociatedType()
{
   auto Loc = currentTok().getSourceLoc();
   consumeToken(tok::kw_associatedType);

   IdentifierInfo *protoSpecifier = nullptr;
   while (lookahead().is(tok::period)) {
//      if (!protoSpecifier.empty())
//         protoSpecifier += '.';

//      protoSpecifier += lexer->getCurrentIdentifier();

      advance();
      advance();
   }

   auto name = currentTok().getIdentifierInfo();

   SourceType actualType;
   if (lookahead().is(tok::equals)) {
      advance();
      advance();

      actualType = parseType().tryGet();
   }

   SmallVector<DeclConstraint*, 4> Constraints;
   if (lookahead().is(tok::kw_where)) {
      advance();
      parseDeclConstraints(Constraints);
   }

   auto AT = AssociatedTypeDecl::Create(Context, Loc, protoSpecifier,
                                        name, actualType, !ParsingProtocol);

   Context.setConstraints(AT, Constraints);
   return ActOnDecl(AT);
}

DeclarationName Parser::parseOperatorName(FixKind Fix,
                                          bool &isCastOp) {
   auto MakeDeclName = [&](llvm::StringRef Ident) {
      auto &II = Context.getIdentifiers().get(Ident);

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

   if (currentTok().is_operator()) {
      return MakeDeclName(currentTok().toString());
   }

   if (currentTok().oneOf(tok::ident, tok::op_ident)) {
      return MakeDeclName(currentTok().getIdentifier());
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
         case tok::kw_infix:
         case tok::kw_prefix:
         case tok::kw_postfix:
            switch (currentTok().getKind()) {
            case tok::kw_infix: Fix = FixKind::Infix; break;
            case tok::kw_prefix: Fix = FixKind::Prefix; break;
            case tok::kw_postfix: Fix = FixKind::Postfix; break;
            default:
               llvm_unreachable("bad fix kind");
            }

            IsOperator = true;
            advance();

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

   std::vector<FuncArgDecl*> args;
   args.push_back(
      FuncArgDecl::Create(Context, SourceLocation(), DefLoc,
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

   std::vector<StaticExpr*> constraints;
   while (lookahead().is(tok::kw_where)) {
      advance();
      advance();

      constraints.push_back(StaticExpr::Create(Context,
         parseExprSequence(DefaultFlags & ~F_AllowBraceClosure).tryGetExpr()));
   }

   MethodDecl *methodDecl;
   if (isConversionOp) {
      methodDecl = MethodDecl::CreateConversionOp(Context, CurDeclAttrs.Access,
                                                  DefLoc, returnType, args,
                                                  move(templateParams),
                                                  nullptr);
   }
   else {
      methodDecl = MethodDecl::Create(Context, CurDeclAttrs.Access, DefLoc,
                                      methodName, returnType, args,
                                      move(templateParams),
                                      nullptr, CurDeclAttrs.StaticLoc);
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
      body = parseCompoundStmt().tryGetStatement<CompoundStmt>();

      if (IsAbstract)
         body = nullptr;
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

   Context.setConstraints(methodDecl, constraints);

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
   std::vector<FuncArgDecl*> associatedTypes;

   if (lexer->lookahead().is(tok::open_paren)) {
      advance();

      SourceLocation varargLoc;
      associatedTypes = parseFuncArgs(varargLoc, /*FIXME*/ true);
   }

   EnumCaseDecl* caseDecl;
   StaticExpr *Expr = nullptr;

   if (lexer->lookahead().is(tok::equals)) {
      advance();
      advance();

      Expr = StaticExpr::Create(Context, parseExprSequence().tryGetExpr());
   }

   caseDecl = EnumCaseDecl::Create(Context, CurDeclAttrs.Access,
                                   CaseLoc, CaseLoc,
                                   caseName, Expr,
                                   associatedTypes);

   return ActOnDecl(caseDecl);
}

} // namespace Parse
} // namespace cdot