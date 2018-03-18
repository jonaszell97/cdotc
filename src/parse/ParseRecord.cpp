//
// Created by Jonas Zell on 10.10.17.
//

#include "Parser.h"

#include "AST/NamedDecl.h"
#include "AST/Passes/SemanticAnalysis/SemaPass.h"
#include "Basic/IdentifierInfo.h"
#include "lex/Lexer.h"
#include "module/Module.h"
#include "Message/Diagnostics.h"
#include "Support/Casting.h"

#include <llvm/ADT/SmallString.h>

using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::lex;

namespace cdot {
namespace parse {

bool Parser::isAtRecordLevel() const
{
   auto *Ctx = &SP.getDeclContext();
   while (Ctx) {
      switch (Ctx->getDeclKind()) {
      case Decl::StructDeclID: case Decl::ClassDeclID: case Decl::EnumDeclID:
      case Decl::UnionDeclID: case Decl::ProtocolDeclID:
      case Decl::ExtensionDeclID:
         return true;
      case Decl::CompoundDeclID:
      case Decl::StaticIfDeclID:
      case Decl::StaticForDeclID:
         break;
      default:
         return false;
      }

      Ctx = Ctx->getParentCtx();
   }

   llvm_unreachable("bad decl context");
}

ParseResult Parser::parseAnyRecord(lex::tok::TokenType kind,
                                   RecordDecl *outer) {
   auto start = currentTok().getSourceLoc();

   RecordHead head;
   if (outer)
      head.templateParams.insert(head.templateParams.end(),
                                 outer->getTemplateParams().begin(),
                                 outer->getTemplateParams().end());

   parseClassHead(head);

   RecordDecl *decl;
   switch (kind) {
      case tok::kw_struct:
         decl = makeExpr<StructDecl>(start, head.access,
                                     move(head.recordName),
                                     move(head.conformances),
                                     move(head.constraints));
         break;
      case tok::kw_class:
         decl = makeExpr<ClassDecl>(start, head.access,
                                    move(head.recordName),
                                    move(head.conformances),
                                    move(head.constraints),
                                    head.parentClass, head.isAbstract);
         break;
      case tok::kw_enum:
         decl = makeExpr<EnumDecl>(start, head.access,
                                   move(head.recordName),
                                   move(head.conformances),
                                   move(head.constraints),
                                   head.enumRawType);
         break;
      case tok::kw_union:
         decl = makeExpr<UnionDecl>(start, head.access,
                                    move(head.recordName),
                                    move(head.conformances),
                                    move(head.constraints));
         break;
      case tok::kw_protocol:
         decl = makeExpr<ProtocolDecl>(start, head.access,
                                       move(head.recordName),
                                       move(head.conformances),
                                       move(head.constraints));
         break;
      case tok::kw_extend:
         decl = makeExpr<ExtensionDecl>(start, head.access,
                                        move(head.recordName),
                                        move(head.conformances),
                                        move(head.constraints));
         break;
      default:
         llvm_unreachable("not a record decl!");
   }

   decl->setHasDefinition(head.hasDefinition);
   decl->setTemplateParams(move(head.templateParams));

   if (head.hasDefinition) {
      DeclContextRAII declContextRAII(*this, decl);
      parseClassInner(decl);
   }

   SP.ActOnRecordDecl(decl);
   return decl;
}

void Parser::parseClassInner(RecordDecl *)
{
   while (!currentTok().is(tok::close_brace)) {
      parseRecordLevelDecl();

      advance();
      while (currentTok().is(tok::semicolon)) {
         advance();
      }
   }
}

ParseResult Parser::parseRecordLevelDecl()
{
   enum class DeclKind {
      NONE,
      FIELD,
      PROP,
      METHOD,
      CONSTR,
      DESTR,
      OPERATOR,
      TYPEDEF,
      Alias,
      CASE,
      AssociatedType,
      InnerRecord,

      StaticIf, StaticFor, StaticAssert, StaticPrint,
   };

   auto start = currentTok().getSourceLoc();
   AccessModifier access = AccessModifier::DEFAULT;
   bool am_set = false;
   bool isStatic = false;
   auto declKind = DeclKind::NONE;
   bool memberwiseInit = false;
   bool isConstField = false;
   bool isMutating = false;
   bool isOperator = false;
   tok::TokenType innerRecordKind = tok::sentinel;

   bool done = false;
   while (currentTok().is_keyword() && !done) {
      auto kind = currentTok().getKind();
      switch (kind) {
      case tok::kw_public:
      case tok::kw_protected:
      case tok::kw_private:
         if (am_set) {
            SP.diagnose(currentTok().getSourceLoc(),
                        err_duplicate_access_spec);
         }

         break;
      case tok::kw_static:
         if (isStatic) {
            SP.diagnose(currentTok().getSourceLoc(),
                        err_expected_declaration, "static");
         }

         break;
      case tok::kw_var:
      case tok::kw_let:
      case tok::kw_prop:
      case tok::kw_def:
      case tok::kw_init:
      case tok::kw_deinit:
      case tok::kw_typedef:
      case tok::kw_alias:
         if (declKind != DeclKind::NONE) {
            SP.diagnose(currentTok().getSourceLoc(),
                        err_duplicate_decl_kind);
         }

         break;
      default:
         break;
      }

      switch (kind) {
      case tok::kw_public:
         access = AccessModifier ::PUBLIC;
         break;
      case tok::kw_protected:
         access = AccessModifier ::PROTECTED;
         break;
      case tok::kw_private:
         access = AccessModifier ::PRIVATE;
         break;
      case tok::kw_static:
         switch (lookahead().getKind()) {
         case tok::kw_if:
            lexer->backtrack();
            declKind = DeclKind::StaticIf;
            done = true;
            break;
         case tok::kw_for:
            lexer->backtrack();
            declKind = DeclKind::StaticFor;
            done = true;
            break;
         default:
            isStatic = true;
            break;
         }

         break;
      case tok::kw_static_assert:
         lexer->backtrack();
         done = true;
         declKind = DeclKind::StaticAssert;
         break;
      case tok::kw_static_print:
         lexer->backtrack();
         done = true;
         declKind = DeclKind::StaticPrint;
         break;
      case tok::kw_typedef:
         declKind = DeclKind::TYPEDEF;
         break;
      case tok::kw_alias:
         declKind = DeclKind::Alias;
         break;
      case tok::kw_let:
         isConstField = true;
         LLVM_FALLTHROUGH;
      case tok::kw_var:
         declKind = DeclKind::FIELD;
         break;
      case tok::kw_prop:
         declKind = DeclKind::PROP;
         break;
      case tok::kw_def:
         declKind = DeclKind::METHOD;
         break;
      case tok::kw_mutating:
         isMutating = true;
         break;
      case tok::kw_memberwise:
         lexer->advance();
         if (!currentTok().is(tok::kw_init)) {
            SP.diagnose(currentTok().getSourceLoc(), err_unexpected_token,
                        currentTok().toString(), true, "'init'");
         }

         memberwiseInit = true;
         LLVM_FALLTHROUGH;
      case tok::kw_init:
         declKind = DeclKind::CONSTR;
         break;
      case tok::kw_deinit:
         declKind = DeclKind::DESTR;
         break;
      case tok::kw_case:
         declKind = DeclKind::CASE;
         break;
      case tok::kw_class:
      case tok::kw_enum:
      case tok::kw_struct:
      case tok::kw_union:
      case tok::kw_protocol:
         declKind = DeclKind::InnerRecord;
         innerRecordKind = kind;
         break;
      case tok::kw_associatedType:
         declKind = DeclKind::AssociatedType;
         break;
      case tok::kw_infix:
      case tok::kw_prefix:
      case tok::kw_postfix:
         isOperator = true;
         break;
      default:
         SP.diagnose(currentTok().getSourceLoc(), err_expecting_decl,
                     currentTok().toString(), /*not top level*/ false);

         return skipUntilProbableEndOfStmt();
      }

      if (declKind == DeclKind::TYPEDEF && isStatic) {
         SP.diagnose(currentTok().getSourceLoc(), err_cannot_be_static,
                     "typedef");
      }

      start = currentTok().getSourceLoc();
      lexer->advance();
   }

   if (isMutating && declKind != DeclKind::METHOD) {
      SP.diagnose(currentTok().getSourceLoc(), err_mutating_non_method);
   }

   switch (declKind) {
   case DeclKind::TYPEDEF:
      return parseTypedef(access);
   case DeclKind::Alias:
      return parseAlias();
   case DeclKind::CONSTR:
      if (isStatic) {
         SP.diagnose(currentTok().getSourceLoc(), err_cannot_be_static,
                     "initializer");
      }

      lexer->backtrack();

      if (memberwiseInit) {
         advance();
         auto Init = makeExpr<InitDecl>(start);
         SP.addDeclToContext(SP.getDeclContext(), Init);

         return Init;
      }
      else {
         return parseConstrDecl(access);
      }

      break;
   case DeclKind::DESTR:
      lexer->backtrack();
      return parseDestrDecl();
   case DeclKind::FIELD:
      return parseFieldDecl(access, isStatic, isConstField);
   case DeclKind::PROP:
      return parsePropDecl(access, isStatic, isConstField);
   case DeclKind::METHOD:
      if (isOperator) {
         lexer->backtrack();
      }

      return parseMethodDecl(access, isStatic, isMutating, isOperator);
   case DeclKind::CASE: {
      ParseResult PR;
      while (1) {
         PR = parseEnumCase();

         if (lookahead().is(tok::comma)) {
            advance();
            advance();
         }
         else
            break;
      }

      return PR;
   }
   case DeclKind::AssociatedType:
      return parseAssociatedType();
   case DeclKind::InnerRecord:
      return parseAnyRecord(innerRecordKind);
   case DeclKind::StaticIf:
      return parseStaticIfDecl();
   case DeclKind::StaticFor:
      return parseStaticForDecl();
      break;
   case DeclKind::StaticAssert:
      return parseStaticAssert();
   case DeclKind::StaticPrint:
      return parseStaticPrint();
   default:
      return ParseError();
   }
}

void Parser::parseClassHead(RecordHead &Head)
{
   bool am_set = false;

   while (currentTok().is_keyword()) {
      auto kind = currentTok().getKind();
      switch (kind) {
         case tok::kw_public:
         case tok::kw_private:
         case tok::kw_protected:
            if (am_set) {
               SP.diagnose(currentTok().getSourceLoc(),
                           err_duplicate_access_spec);
            }

            Head.access = kind == tok::kw_public ? AccessModifier::PUBLIC
                                                 : AccessModifier::PRIVATE;
            am_set = true;
            break;
         case tok::kw_abstract:
            Head.isAbstract = true;
            break;
         case tok::kw_struct:
         case tok::kw_enum:
         case tok::kw_class:
         case tok::kw_union:
         case tok::kw_protocol:
         case tok::kw_extend:
            break;
         default:
            SP.diagnose(currentTok().getSourceLoc(), err_unexpected_token,
                        currentTok().toString(), false);
      }

      advance();
   }

   if (currentTok().getKind() != tok::ident) {
      SP.diagnose(currentTok().getSourceLoc(), err_unexpected_token,
                  currentTok().toString(), true, "identifier");

      skipUntilProbableEndOfStmt();
      return;
   }

   Head.recordName = lexer->getCurrentIdentifier();

   if (lexer->lookahead().is(tok::open_paren)) {
      lexer->advance();
      lexer->advance();

      Head.enumRawType = parseType().get();

      lexer->advance();
      if (!currentTok().is(tok::close_paren)) {
         SP.diagnose(currentTok().getSourceLoc(), err_unexpected_token,
                     currentTok().toString(), true, "')'");
      }
   }

   auto params = tryParseTemplateParameters();
   Head.templateParams.insert(Head.templateParams.end(),
                              params.begin(),
                              params.end());

   auto next = lookahead();
   while (!next.is(tok::open_brace)) {
      if (next.is(tok::colon)) {
         if (Head.parentClass) {
            SP.diagnose(currentTok().getSourceLoc(), err_multiple_inheritance);
         }

         advance();
         advance();

         Head.parentClass = parseType().get();
      }
      else if (next.is(tok::kw_with)) {
         advance();
         advance();

         while (true) {
            auto proto = parseType(true).get();
            Head.conformances.push_back(proto);

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

         Head.constraints.push_back(
            StaticExpr::Create(Context, parseExprSequence().tryGetExpr()));
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

ParseResult Parser::parseConstrDecl(AccessModifier am)
{
   auto start = currentTok().getSourceLoc();
   auto params = tryParseTemplateParameters();

   advance();

   SourceLocation varargLoc;
   auto args = parseFuncArgs(varargLoc);

   auto Init =  makeExpr<InitDecl>(start, std::move(args), am, nullptr);

   Statement* body = nullptr;
   if (lexer->lookahead().is(tok::open_brace)) {
      DeclContextRAII declContextRAII(*this, Init);
      body = parseBlock().tryGetStatement();
   }

   Init->setBody(body);
   Init->setVararg(varargLoc.isValid());
   Init->setTemplateParams(move(params));

   SP.addDeclToContext(SP.getDeclContext(), Init);

   return Init;
}

ParseResult Parser::parseDestrDecl()
{
   auto start = currentTok().getSourceLoc();
   lexer->advance();

   SourceLocation varargLoc;
   auto args = parseFuncArgs(varargLoc);

   if (!args.empty()) {
      SP.diagnose(currentTok().getSourceLoc(), err_deinit_args);
      args.clear();
   }

   auto Deinit = makeExpr<DeinitDecl>(start);

   Statement* body = nullptr;
   if (lexer->lookahead().is(tok::open_brace)) {
      DeclContextRAII declContextRAII(*this, Deinit);
      body = parseBlock().tryGetStatement();
   }

   Deinit->setReturnType(SourceType(Context.getVoidType()));
   Deinit->setBody(body);

   SP.addDeclToContext(SP.getDeclContext(), Deinit);

   return Deinit;
}

ParseResult Parser::parseFieldDecl(AccessModifier am,
                                   bool isStatic,
                                   bool isConst) {
   if (currentTok().getKind() != tok::ident) {
      SP.diagnose(currentTok().getSourceLoc(), err_unexpected_token,
                  currentTok().toString(), true, "identifier");

      return skipUntilProbableEndOfStmt();
   }

   auto start = currentTok().getSourceLoc();
   string fieldName = lexer->getCurrentIdentifier();

   SourceType typeref;
   bool typeDeclared = false;
   Token next = lexer->lookahead();

   if (next.is(tok::colon)) {
      lexer->advance();
      lexer->advance();
      typeDeclared = true;

      typeref = parseType().get();
      next = lexer->lookahead();
   }
   else {
      typeref = SourceType(Context.getAutoType());
   }

   auto field = makeExpr<FieldDecl>(start, move(fieldName), typeref,
                                    am, isStatic, isConst);

   // getter and setter
   bool getter = false;
   bool setter = false;

   bool hasDefinition = true;

   if (next.is(tok::open_brace)) {
      lexer->advance();
      lexer->advance();

      while (!currentTok().is(tok::close_brace)) {
         if (currentTok().getKind() == tok::ident
             && currentTok().getIdentifierInfo()->isStr("get")) {
            if (getter) {
               SP.diagnose(currentTok().getSourceLoc(),
                           err_duplicate_getter_setter, 0);
            }

            getter = true;

            if (lexer->lookahead().is(tok::open_brace)) {
               auto body = parseBlock().tryGetStatement<CompoundStmt>();
               field->addGetter(body);

               lexer->advance();
            }
            else {
               hasDefinition = false;
               field->addGetter();
               lexer->advance();
            }
         }
         else if (currentTok().getKind() == tok::ident
                  && currentTok().getIdentifierInfo()->isStr("set")) {
            if (setter) {
               SP.diagnose(currentTok().getSourceLoc(),
                           err_duplicate_getter_setter, 1);
            }

            setter = true;

            if (lexer->lookahead().is(tok::open_brace)) {
               auto body = parseBlock().tryGetStatement<CompoundStmt>();
               field->addSetter(move(body));

               lexer->advance();
            }
            else {
               hasDefinition = false;
               field->addSetter();
               lexer->advance();
            }
         }
         else if (currentTok().is(tok::comma)) {
            lexer->advance();
         }
         else {
            SP.diagnose(currentTok().getSourceLoc(), err_unexpected_token,
                        currentTok().toString(), true, "'get' or 'set'");
         }
      }

      next = lexer->lookahead();
   }

   // optional default value
   if (next.is(tok::equals)) {
      lexer->advance();
      lexer->advance();
      field->setDefault(parseExprSequence().tryGetExpr());
   }

   field->setHasDefinition(hasDefinition);

   SP.ActOnFieldDecl(field);

   return field;
}

ParseResult Parser::parsePropDecl(AccessModifier am,
                                  bool isStatic,
                                  bool isConst) {
   if (currentTok().getKind() != tok::ident) {
      SP.diagnose(currentTok().getSourceLoc(), err_unexpected_token,
                  currentTok().toString(), true, "identifier");

      return skipUntilProbableEndOfStmt();
   }

   auto start = currentTok().getSourceLoc();
   string fieldName = lexer->getCurrentIdentifier();
   bool hasDefinition = true;

   SourceType typeref;
   lexer->advance();

   if (!currentTok().is(tok::colon)) {
      SP.diagnose(currentTok().getSourceLoc(), err_prop_must_have_type);
   }
   else {
      lexer->advance();
      typeref = parseType().get();
   }

   Token next = lexer->lookahead();

   bool hasGetter = false;
   bool hasSetter = false;

   CompoundStmt* getter = nullptr;
   CompoundStmt* setter = nullptr;

   string newValName = "newVal";

   if (next.is(tok::open_brace)) {
      hasDefinition = true;

      lexer->advance();
      lexer->advance();

      while (!currentTok().is(tok::close_brace)) {
         if (currentTok().getKind() == tok::ident
             && currentTok().getIdentifierInfo()->isStr("get")) {
            if (hasGetter) {
               SP.diagnose(currentTok().getSourceLoc(),
                           err_duplicate_getter_setter, 0);
            }

            hasGetter = true;

            if (lexer->lookahead().is(tok::open_brace)) {
               getter = parseBlock().tryGetStatement<CompoundStmt>();
            }
            else {
               hasDefinition = false;
            }

            lexer->advance();
         }
         else if (currentTok().getKind() == tok::ident
                  && currentTok().getIdentifierInfo()->isStr("set")) {
            if (hasSetter) {
               SP.diagnose(currentTok().getSourceLoc(),
                           err_duplicate_getter_setter, 1);
            }

            hasSetter = true;

            if (lexer->lookahead().is(tok::open_paren)) {
               lexer->advance();
               lexer->advance();
               if (currentTok().getKind() != tok::ident) {
                  SP.diagnose(currentTok().getSourceLoc(), err_unexpected_token,
                              currentTok().toString(), true, "identifier");

                  return skipUntilProbableEndOfStmt();
               }

               newValName = lexer->getCurrentIdentifier();
               lexer->advance();

               if (!currentTok().is(tok::close_paren)) {
                  SP.diagnose(currentTok().getSourceLoc(), err_unexpected_token,
                              currentTok().toString(), true, "')'");
               }
            }

            if (lexer->lookahead().is(tok::open_brace)) {
               setter = parseBlock().tryGetStatement<CompoundStmt>();
            }
            else {
               hasDefinition = false;
            }

            lexer->advance();
         }
         else if (currentTok().is(tok::comma)) {
            lexer->advance();
         }
         else {
            SP.diagnose(currentTok().getSourceLoc(), err_unexpected_token,
                        currentTok().toString(), true, "'get' or 'set'");
         }
      }

      next = lexer->lookahead();
   }

   if (!hasGetter && !hasSetter) {
      SP.diagnose(currentTok().getSourceLoc(), err_prop_must_have_get_or_set);
   }

   auto prop = makeExpr<PropDecl>(start, std::move(fieldName),
                                  typeref, am,
                                  isStatic,
                                  hasDefinition,
                                  hasGetter, hasSetter,
                                  getter,
                                  setter,
                                  move(newValName));

   prop->setHasDefinition(hasDefinition);

   SP.addDeclToContext(SP.getDeclContext(), prop);

   return prop;
}

ParseResult Parser::parseAssociatedType()
{
   auto start = currentTok().getSourceLoc();
   llvm::SmallString<128> protoSpecifier;

   while (lookahead().is(tok::period)) {
      if (!protoSpecifier.empty())
         protoSpecifier += '.';

      protoSpecifier += lexer->getCurrentIdentifier();

      advance();
      advance();
   }

   auto name = lexer->getCurrentIdentifier();

   SourceType actualType;
   if (lookahead().is(tok::equals)) {
      advance();
      advance();

      actualType = parseType().tryGet();
   }
   else {
      actualType = SourceType(Context.getAutoType());
   }

   std::vector<StaticExpr*> constraints;
   while (lookahead().is(tok::kw_where)) {
      advance();
      advance();

      constraints.push_back(
         StaticExpr::Create(Context, parseExprSequence().tryGetExpr()));
   }

   auto AT = makeExpr<AssociatedTypeDecl>(start, protoSpecifier.str().str(),
                                          move(name), move(constraints),
                                          actualType);

   SP.addDeclToContext(SP.getDeclContext(), AT);

   return AT;
}

string Parser::parseOperatorName(OperatorInfo &Info,
                                 bool &isCastOp,
                                 SourceType &castTarget) {
   string name;
   switch (Info.getFix()) {
      case FixKind::Infix: name += "infix "; break;
      case FixKind::Prefix: name += "prefix "; break;
      case FixKind::Postfix: name += "postfix "; break;
   }

   if (Info.isPostfix() && currentTok().is(tok::open_paren)
       && lexer->lookahead().is(tok::close_paren)) {
      lexer->advance();
      name += "()";
   }
   else if (Info.isPostfix() && currentTok().is(tok::open_square)
            && lexer->lookahead().is(tok::close_square)) {
      lexer->advance();
      name += "[]";
   }
   else if (Info.isPostfix() && currentTok().is(tok::as)) {
      lexer->advance();

      auto Ty = parseType();
      if (Ty) {
         castTarget = Ty.get();
      }

      isCastOp = true;
   }
   else if (currentTok().is_operator()) {
      name += currentTok().toString();

      auto opKind = op::fromToken(currentTok().getKind());
      Info.setPrecedenceGroup(
         PrecedenceGroup(getOperatorPrecedence(opKind),
                         getAssociativity(opKind)));
   }
   else if (currentTok().oneOf(tok::ident, tok::op_ident)) {
      name += currentTok().getIdentifier();

      auto builtinOp = op::fromString(Info.getFix(),
                                      currentTok().getIdentifier());

      if (builtinOp != op::UnknownOp) {
         Info.setPrecedenceGroup(
            PrecedenceGroup(getOperatorPrecedence(builtinOp),
                            getAssociativity(builtinOp)));
      }
      else {
         Info.setPrecedenceGroup(PrecedenceGroup(12, Associativity::Left));
      }
   }
   else {
      SP.diagnose(currentTok().getSourceLoc(), err_unexpected_token,
                  currentTok().toString(), true, "operator");
   }

   return name;
}

ParseResult Parser::parseMethodDecl(AccessModifier am,
                                    bool isStatic,
                                    bool isMutating,
                                    bool isOperator) {
   auto start = currentTok().getSourceLoc();

   OperatorInfo OpInfo;

   if (isOperator) {
      switch (currentTok().getKind()) {
         case tok::kw_infix: OpInfo.setFix(FixKind::Infix); break;
         case tok::kw_prefix: OpInfo.setFix(FixKind::Prefix); break;
         case tok::kw_postfix: OpInfo.setFix(FixKind::Postfix); break;
         default:
            llvm_unreachable("bad fix kind");
      }

      OpInfo.setPrecedenceGroup(PrecedenceGroup(12, Associativity::Left));
      advance();
   }

   bool isCastOp = false;
   SourceType returnType;
   string methodName;

   if (isOperator) {
      methodName = parseOperatorName(OpInfo, isCastOp, returnType);
      if (isCastOp) {
         if (OpInfo.getFix() != FixKind::Infix) {
            SP.diagnose(currentTok().getSourceLoc(), err_generic_error,
                        "cast operator must be infix");

            OpInfo.setFix(FixKind::Infix);
         }

         // all cast operators are grouped together during parsing,
         // this will be fixed in Sema
         methodName = "infix as";
      }
   }
   else {
      methodName = lexer->getCurrentIdentifier();
   }

   auto templateParams = tryParseTemplateParameters();
   advance();

   SourceLocation varargLoc;
   auto args = parseFuncArgs(varargLoc);

   // optional return type
   if (lookahead().is(tok::arrow_single)) {
      lexer->advance();
      lexer->advance();
      returnType = parseType().get();
   }

   if (!returnType) {
      returnType = SourceType(Context.getAutoType());
   }

   std::vector<StaticExpr*> constraints;
   while (lookahead().is(tok::kw_where)) {
      advance();
      advance();

      constraints.push_back(
         StaticExpr::Create(Context, parseExprSequence().tryGetExpr()));
   }

   auto methodDecl = makeExpr<MethodDecl>(start, std::move(methodName),
                                          returnType, std::move(args),
                                          move(constraints),
                                          nullptr, OpInfo, isCastOp, am,
                                          isStatic);

   CompoundStmt* body = nullptr;
   if (lookahead().is(tok::open_brace)) {
      DeclContextRAII declContextRAII(*this, methodDecl);
      body = parseBlock().tryGetStatement<CompoundStmt>();
   }

   methodDecl->setVararg(varargLoc.isValid());
   methodDecl->setBody(body);
   methodDecl->setMutating(isMutating);
   methodDecl->setTemplateParams(move(templateParams));

   SP.addDeclToContext(SP.getDeclContext(), methodDecl);
   return methodDecl;
}

ParseResult Parser::parseEnumCase()
{
   auto start = currentTok().getSourceLoc();

   if (currentTok().getKind() != tok::ident) {
      SP.diagnose(currentTok().getSourceLoc(), err_unexpected_token,
                  currentTok().toString(), true, "identifier");

      return skipUntilProbableEndOfStmt();
   }

   string caseName = lexer->getCurrentIdentifier();
   EnumCaseDecl::AssociatedTypeVec associatedTypes;

   if (lexer->lookahead().is(tok::open_paren)) {
      lexer->advance();

      SourceLocation varargLoc;
      associatedTypes = parseFuncArgs(varargLoc);
   }

   EnumCaseDecl* caseDecl;

   if (lexer->lookahead().is(tok::equals)) {
      lexer->advance();
      lexer->advance();

      auto expr = StaticExpr::Create(Context, parseExprSequence().tryGetExpr());
      caseDecl = makeExpr<EnumCaseDecl>(start, std::move(caseName),
                                        std::move(expr),
                                        std::move(associatedTypes));
   }
   else {
      caseDecl = makeExpr<EnumCaseDecl>(start, std::move(caseName),
                                        std::move(associatedTypes));
   }

   SP.addDeclToContext(SP.getDeclContext(), caseDecl);
   return caseDecl;
}

} // namespace parse
} // namespace cdot