//
// Created by Jonas Zell on 10.10.17.
//

#include <llvm/ADT/SmallString.h>
#include "Parser.h"

#include "../Basic/IdentifierInfo.h"
#include "../module/Module.h"
#include "../lex/Lexer.h"

#include "../AST/SymbolTable.h"
#include "../AST/Passes/Declaration/DeclPass.h"
#include "../AST/Passes/SemanticAnalysis/SemaPass.h"

#include "../AST/Expression/TypeRef.h"
#include "../AST/Expression/StaticExpr.h"

#include "../AST/Statement/Declaration/Class/RecordDecl.h"
#include "../AST/Statement/Declaration/Class/MethodDecl.h"
#include "../AST/Statement/Declaration/Class/FieldDecl.h"
#include "../AST/Statement/Declaration/Class/PropDecl.h"
#include "../AST/Statement/Declaration/Class/EnumCaseDecl.h"
#include "../AST/Statement/Declaration/TypedefDecl.h"
#include "../AST/Statement/Declaration/LocalVarDecl.h"
#include "../AST/Statement/Block/CompoundStmt.h"

#include "../AST/Statement/Static/StaticStmt.h"

#include "../Message/Diagnostics.h"
#include "../Support/Casting.h"

using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::lex;

namespace cdot {
namespace parse {

NamedDecl* Parser::parse_any_record(lex::tok::TokenType kind,
                                    RecordDecl *outer) {
   auto start = currentTok().getSourceLoc();

   RecordHead head{};
   if (outer)
      head.templateParams.insert(head.templateParams.end(),
                                 outer->getTemplateParams().begin(),
                                 outer->getTemplateParams().end());

   parse_class_head(head);

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
      parse_class_inner(decl);
   }

   SP.getDeclPass()->DeclareRecord(decl);
   return decl;
}

void Parser::parse_class_inner(RecordDecl *decl)
{
   bool declaration_finished = false;
   enum class DeclType {
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

      STATIC_STMT
   };

   while (!declaration_finished) {
      lexer->advance();

      auto start = currentTok().getSourceLoc();
      AccessModifier access = AccessModifier::DEFAULT;
      bool am_set = false;
      bool isStatic = false;
      auto type = DeclType::NONE;
      bool memberwiseInit = false;
      bool isConstField = false;
      bool isMutating = false;
      bool isOperator = false;
      bool isInnerDeclaration = false;
      tok::TokenType innerRecordKind = tok::sentinel;

      auto attributes = parse_attributes();

      bool done = false;
      while (currentTok().is_keyword() && !done) {
         auto kind = currentTok().getKind();
         switch (kind) {
            case tok::kw_public:
            case tok::kw_protected:
            case tok::kw_private:
               if (am_set) {
                  err(err_generic_error)
                     << "duplicate access specifier"
                     << diag::term;
               }
               break;
            case tok::kw_static:
               if (isStatic) {
                  err(err_generic_error)
                     << "static declared twice" << diag::term;
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
               if (type != DeclType::NONE) {
                  err(err_generic_error)
                     << "Declaration type already defined" << diag::term;
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
               isStatic = true;
               break;
            case tok::kw_typedef:
               type = DeclType::TYPEDEF;
               break;
            case tok::kw_alias:
               type = DeclType::Alias;
               break;
            case tok::kw_let:
               isConstField = true;
               LLVM_FALLTHROUGH;
            case tok::kw_var:
               type = DeclType::FIELD;
               break;
            case tok::kw_prop:
               type = DeclType::PROP;
               break;
            case tok::kw_def:
               type = DeclType::METHOD;
               break;
            case tok::kw_mutating:
               isMutating = true;
               break;
            case tok::kw_memberwise:
               lexer->advance();
               if (!currentTok().is(tok::kw_init)) {
                  err(err_generic_error)
                     << "Expected 'init' after 'memberwise'"
                     << lexer << diag::term;
               }

               memberwiseInit = true;
               LLVM_FALLTHROUGH;
            case tok::kw_init:
               type = DeclType::CONSTR;
               break;
            case tok::kw_deinit:
               type = DeclType::DESTR;
               break;
            case tok::kw_case:
               type = DeclType::CASE;
               break;
            case tok::kw_class:
            case tok::kw_enum:
            case tok::kw_struct:
            case tok::kw_union:
            case tok::kw_protocol:
               type = DeclType::InnerRecord;
               innerRecordKind = kind;
               break;
            case tok::kw_declare:
               isInnerDeclaration = true;
               break;
            case tok::kw_associatedType:
               type = DeclType::AssociatedType;
               break;
            case tok::kw_infix:
            case tok::kw_prefix:
            case tok::kw_postfix:
               isOperator = true;
               break;
            case tok::kw_static_if:
            case tok::kw_static_for:
            case tok::kw_static_assert:
               done = true;
               type = DeclType::STATIC_STMT;
               break;
            default:
            err(err_generic_error)
               << "unexpected keyword '" + currentTok().toString() +
                  + "' in class declaration"
               << lexer << diag::term;
         }

         if (type == DeclType::TYPEDEF && isStatic) {
            err(err_generic_error)
               << "Typedefs are static by default" << lexer << diag::term;
         }

         start = currentTok().getSourceLoc();
         lexer->advance();
      }

      if (isMutating && type != DeclType::METHOD) {
         err(err_generic_error)
            << "'mutating' can only appear before method definitions"
            << lexer << diag::term;
      }

      if (type == DeclType::TYPEDEF) {
         auto td = parse_typedef(access, true);
         td->setAttributes(move(attributes));

         SP.addDeclToContext(*decl, td);
      }
      else if (type == DeclType::Alias) {
         auto alias = parse_alias(true);
         alias->setAttributes(move(attributes));

         SP.addDeclToContext(*decl, alias);
      }
      else if (type == DeclType::CONSTR) {
         if (isStatic) {
            err(err_generic_error)
               << "initializer cannot be declared 'static'"
               << lexer << diag::term;
         }

         lexer->backtrack();

         if (memberwiseInit) {
            advance();
            SP.addDeclToContext(*decl, makeExpr<InitDecl>(start));
         }
         else {
            auto constr = parse_constr_decl(access);
            SP.addDeclToContext(*decl, constr);
         }
      }
      else if (type == DeclType::DESTR) {
         lexer->backtrack();
         auto destructor = parse_destr_decl();
         destructor->setAttributes(move(attributes));

         SP.addDeclToContext(*decl, destructor);
      }
      else if (type == DeclType::FIELD) {
         auto field = parse_field_decl(access, isStatic, isConstField);

         field->setAttributes(move(attributes));
         SP.addDeclToContext(*decl, field);
      }
      else if (type == DeclType::PROP) {
         auto prop = parse_prop_decl(access, isStatic, isConstField);

         prop->setAttributes(move(attributes));
         SP.addDeclToContext(*decl, prop);
      }
      else if (type == DeclType::METHOD) {
         if (isOperator) {
            lexer->backtrack();
         }

         auto method = parse_method_decl(access, isStatic,
                                         isMutating, isOperator);

         method->setAttributes(move(attributes));
         SP.addDeclToContext(*decl, method);
      }
      else if (type == DeclType::CASE) {
         while (1) {
            auto Case = parse_enum_case();

            if (!attributes.empty())
               Case->setAttributes(move(attributes));

            SP.addDeclToContext(*decl, Case);

            if (lookahead().is(tok::comma)) {
               advance();
               advance();
            }
            else
               break;
         }
      }
      else if (type == DeclType::AssociatedType) {
         auto AT = parse_associated_type();
         AT->setAttributes(move(attributes));

         SP.addDeclToContext(*decl, AT);
      }
      else if (type == DeclType::InnerRecord) {
         auto innerDecl = parse_any_record(innerRecordKind, decl);
         innerDecl->setAttributes(move(attributes));

         SP.addDeclToContext(*decl, innerDecl);
      }
      else if (type == DeclType::STATIC_STMT) {
         lexer->backtrack();
         auto stmt = parse_static_stmt(true);

         stmt->setAttributes(move(attributes));
         decl->addStaticStatement(stmt);
      }
      else if (currentTok().is(tok::close_brace)) {
         declaration_finished = true;
      }
      else {
         err(err_generic_error)
            << "unexpected token in record definition: "
               + currentTok().toString() << lexer << lexer << diag::term;
      }

      Token next = lexer->lookahead();
      if (next.is(tok::semicolon)) {
         lexer->advance();
         next = lexer->lookahead();
      }
      if (!declaration_finished && next.is(tok::close_brace)) {
         lexer->advance();
         declaration_finished = true;
      }
   }
}

void Parser::parse_class_head(RecordHead &Head)
{
   bool am_set = false;

   while (currentTok().is_keyword()) {
      auto kind = currentTok().getKind();
      switch (kind) {
         case tok::kw_public:
         case tok::kw_private:
            if (am_set) {
               err(err_generic_error)
                  << "duplicate access specifier"
                  << diag::term;
            }

            Head.access = kind == tok::kw_public ? AccessModifier::PUBLIC
                                                 : AccessModifier::PRIVATE;
            am_set = true;
            break;
         case tok::kw_protected:
            err(err_generic_error)
               << "records cannot be declared protected"
               << diag::term;

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
            err(err_generic_error)
               << "unexpected keyword " + currentTok().toString()
               << lexer << diag::term;
      }

      advance();
   }

   if (currentTok().getKind() != tok::ident) {
      err(err_generic_error)
         << "expected record name"
         << lexer << diag::term;
   }

   Head.recordName = lexer->getCurrentIdentifier();

   if (lexer->lookahead().is(tok::open_paren)) {
      lexer->advance();
      lexer->advance();

      Head.enumRawType = parse_type();

      lexer->advance();
      if (!currentTok().is(tok::close_paren)) {
         err(err_generic_error)
            << "expected ')'"
            << lexer << diag::term;
      }
   }

   auto params = try_parse_template_parameters();
   Head.templateParams.insert(Head.templateParams.end(),
                              params.begin(),
                              params.end());

   auto next = lookahead();
   while (!next.is(tok::open_brace)) {
      if (next.is(tok::colon)) {
         if (Head.parentClass) {
            err(err_generic_error)
               << "only one inheritance specification is allowed"
               << lexer << diag::term;
         }

         advance();
         advance();

         Head.parentClass = parse_type();
      }
      else if (next.is(tok::kw_with)) {
         advance();
         advance();

         while (true) {
            auto proto = parse_type(true);
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
            new (Context) StaticExpr(parse_static_expr()));
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

NamedDecl* Parser::parse_constr_decl(AccessModifier am)
{
   auto start = currentTok().getSourceLoc();
   auto params = try_parse_template_parameters();

   advance();

   SourceLocation varargLoc;
   auto args = parse_arg_list(varargLoc);

   auto Init =  makeExpr<InitDecl>(start, std::move(args), am, nullptr);

   CompoundStmt* body = nullptr;
   if (lexer->lookahead().is(tok::open_brace)) {
      DeclContextRAII declContextRAII(*this, Init);
      body = parse_block();
   }

   Init->setBody(body);
   Init->setVararg(varargLoc.isValid());
   Init->setTemplateParams(move(params));

   return Init;
}

DeinitDecl* Parser::parse_destr_decl()
{
   auto start = currentTok().getSourceLoc();
   lexer->advance();

   SourceLocation varargLoc;
   auto args = parse_arg_list(varargLoc);

   if (!args.empty()) {
      err(err_generic_error)
         << "deinitializers cannot have arguments" << diag::term;

      args.clear();
   }

   auto Deinit = makeExpr<DeinitDecl>(start);

   CompoundStmt* body = nullptr;
   if (lexer->lookahead().is(tok::open_brace)) {
      DeclContextRAII declContextRAII(*this, Deinit);
      body = parse_block();
   }

   Deinit->setReturnType(new (Context) TypeRef(Context.getVoidType()));
   Deinit->setBody(body);

   return Deinit;
}

FieldDecl* Parser::parse_field_decl(AccessModifier am,
                                    bool is_static,
                                    bool isConst) {
   if (currentTok().getKind() != tok::ident) {
      err(err_generic_error)
         << "Field name must be a valid identifier" << diag::term;
   }

   auto start = currentTok().getSourceLoc();
   string fieldName = lexer->getCurrentIdentifier();

   TypeRef* typeref;
   bool typeDeclared = false;
   Token next = lexer->lookahead();

   if (next.is(tok::colon)) {
      lexer->advance();
      lexer->advance();
      typeDeclared = true;

      typeref = parse_type();
      next = lexer->lookahead();
   }
   else {
      typeref = makeExpr<TypeRef>(start);
   }

   auto field = makeExpr<FieldDecl>(start, move(fieldName), move(typeref),
                                    am, is_static, isConst);

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
               err(err_generic_error)
                  << "Getter already declared" << diag::term;
            }

            getter = true;

            if (lexer->lookahead().is(tok::open_brace)) {
               auto body = parse_block();
               field->addGetter(move(body));

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
               err(err_generic_error)
                  << "Setter already declared" << diag::term;
            }

            setter = true;

            if (lexer->lookahead().is(tok::open_brace)) {
               auto body = parse_block();
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
            err(err_generic_error)
               << "Expected 'get' or 'set'" << diag::term;
         }
      }

      next = lexer->lookahead();
   }

   // optional default value
   if (next.is(tok::equals)) {
      lexer->advance();
      lexer->advance();
      field->setDefault(parse_expr_sequence());
   }
   else if (!typeDeclared) {
      err(err_generic_error)
         << "fields have to have an annotated type or a default"
            "value" << diag::term;
   }

   field->setHasDefinition(hasDefinition);

   return field;
}

PropDecl* Parser::parse_prop_decl(AccessModifier am,
                                  bool isStatic,
                                  bool isConst) {
   if (currentTok().getKind() != tok::ident) {
      err(err_generic_error)
         << "property name must be a valid identifier" << diag::term;
   }

   auto start = currentTok().getSourceLoc();
   string fieldName = lexer->getCurrentIdentifier();
   bool hasDefinition = true;

   TypeRef* typeref;
   lexer->advance();

   if (!currentTok().is(tok::colon)) {
      err(err_generic_error) << "property must have a defined type"
                                   << lexer << lexer << diag::term;
   }

   lexer->advance();
   typeref = parse_type();

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
               err(err_generic_error)
                  << "Getter already declared" << diag::term;
            }

            hasGetter = true;

            if (lexer->lookahead().is(tok::open_brace)) {
               getter = parse_block();
            }
            else {
               hasDefinition = false;
            }

            lexer->advance();
         }
         else if (currentTok().getKind() == tok::ident
                  && currentTok().getIdentifierInfo()->isStr("set")) {
            if (hasSetter) {
               err(err_generic_error)
                  << "Setter already declared" << diag::term;
            }

            hasSetter = true;

            if (lexer->lookahead().is(tok::open_paren)) {
               lexer->advance();
               lexer->advance();
               if (currentTok().getKind() != tok::ident) {
                  err(err_generic_error)
                     << "expected identifier"
                     << currentTok().getSourceLoc()
                     << diag::term;

                  return nullptr;
               }

               newValName = lexer->getCurrentIdentifier();
               lexer->advance();

               if (!currentTok().is(tok::close_paren)) {
                  err(err_generic_error)
                     << "expected ')'"
                     << currentTok().getSourceLoc()
                     << diag::term;
               }
            }

            if (lexer->lookahead().is(tok::open_brace)) {
               setter = parse_block();
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
            err(err_generic_error)
            << "expected 'get' or 'set'" << diag::term;
         }
      }

      next = lexer->lookahead();
   }

   if (!hasGetter && !hasSetter) {
      err(err_generic_error)
         << "property must define a getter or a setter"
         << lexer << lexer << diag::term;
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
   return prop;
}

AssociatedTypeDecl* Parser::parse_associated_type()
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

   TypeRef* actualType;
   if (lookahead().is(tok::equals)) {
      advance();
      advance();

      actualType = parse_type();
   }
   else {
      actualType = new (Context) TypeRef();
   }

   std::vector<StaticExpr*> constraints;
   while (lookahead().is(tok::kw_where)) {
      advance();
      advance();

      constraints.push_back(new (Context) StaticExpr(parse_static_expr()));
   }

   return makeExpr<AssociatedTypeDecl>(start, protoSpecifier.str().str(),
                                       move(name), move(constraints),
                                       actualType);
}

string Parser::parse_operator_name(bool &isCastOp,
                                   TypeRef* &castTarget) {
   string name;
   if (currentTok().is(tok::open_paren)
       && lexer->lookahead().is(tok::close_paren)) {
      lexer->advance();
      name = "()";
   }
   else if (currentTok().is(tok::open_square)
            && lexer->lookahead().is(tok::close_square)) {
      lexer->advance();
      name = "[]";
   }
   else if (currentTok().is(tok::as)) {
      lexer->advance();
      castTarget = parse_type();
      isCastOp = true;
   }
   else {
      if (currentTok().is_operator()) {
         name += currentTok().toString();
      }
      else {
         name += lexer->getCurrentIdentifier();
      }

      while (isValidOperatorChar(lookahead(false, true))) {
         advance(false, true);

         if (currentTok().is_operator()) {
            name += currentTok().toString();
         }
         else {
            name += currentTok().getIdentifierInfo()->getIdentifier().str();
         }
      }

      if (!util::matches("(..+|[^.])*", name)) {
         err(err_generic_error)
            << "custom operators can only contain periods"
               "in sequences of two or more"
            << lexer << diag::term;
      }
   }

   return name;
}

NamedDecl* Parser::parse_method_decl(AccessModifier am,
                                     bool isStatic,
                                     bool isMutating,
                                     bool isOperator) {
   auto start = currentTok().getSourceLoc();

   OperatorInfo op;
   FixKind fix;

   if (isOperator) {
      switch (currentTok().getKind()) {
         case tok::kw_prefix: fix = FixKind::Prefix; break;
         case tok::kw_infix: fix = FixKind::Infix; break;
         case tok::kw_postfix: fix = FixKind::Postfix; break;
         default:
            llvm_unreachable("bad fix kind");
      }

      op.setFix(fix);
      op.setPrecedenceGroup(PrecedenceGroup(12, Associativity::Left));

      advance();
   }

   bool isCastOp = false;
   TypeRef* returnType = nullptr;
   string methodName;

   if (isOperator) {
      methodName = parse_operator_name(isCastOp, returnType);
      if (isCastOp) {
         if (op.getFix() != FixKind::Infix) {
            err(err_generic_error)
               << "cast operator must be infix"
               << lexer << diag::term;

            op.setFix(FixKind::Infix);
         }

         // all cast operators are grouped together during parsing,
         // this will be fixed in Sema
         methodName = "infix as";
      }
   }
   else {
      methodName = lexer->getCurrentIdentifier();
   }

   // method alias
   if (currentTok().is(tok::equals)) {
      advance();

      if (currentTok().getKind() != tok::ident
          && currentTok().is_operator()) {
         err(err_generic_error)
            << "method aliasee must be a valid identifier"
            << lexer << diag::term;
      }

      string aliasee = lexer->getCurrentIdentifier();
      advance();

      SourceLocation varargLoc;
      auto args = parse_arg_list(varargLoc);
      auto op_decl = makeExpr<MethodDecl>(start, std::move(methodName),
                                          std::move(aliasee), std::move(args));

      op_decl->setVararg(varargLoc.isValid());

      return op_decl;
   }

   auto templateParams = try_parse_template_parameters();
   advance();

   SourceLocation varargLoc;
   auto args = parse_arg_list(varargLoc);

   // optional return type
   if (lookahead().is(tok::arrow_single)) {
      if (isCastOp) {
         err(err_generic_error) << "conversion operators cannot specify"
            " a return type" << lexer << lexer << diag::term;
      }

      lexer->advance();
      lexer->advance();
      returnType = parse_type();
   }
   else if (!returnType) {
      returnType = makeExpr<TypeRef>(start);
   }

   std::vector<StaticExpr*> constraints;
   while (lookahead().is(tok::kw_where)) {
      advance();
      advance();

      constraints.push_back(new (Context) StaticExpr(parse_static_expr()));
   }

   MethodDecl* methodDecl = makeExpr<MethodDecl>(start, std::move(methodName),
                                                 returnType, std::move(args),
                                                 move(constraints),
                                                 nullptr, op, isCastOp, am,
                                                 isStatic);

   CompoundStmt* body = nullptr;
   if (lookahead().is(tok::open_brace)) {
      DeclContextRAII declContextRAII(*this, methodDecl);
      body = parse_block();
   }

   methodDecl->setVararg(varargLoc.isValid());
   methodDecl->setBody(body);
   methodDecl->setMutating(isMutating);
   methodDecl->setTemplateParams(move(templateParams));

   return methodDecl;
}

EnumCaseDecl* Parser::parse_enum_case()
{
   auto start = currentTok().getSourceLoc();

   if (currentTok().getKind() != tok::ident) {
      err(err_generic_error)
         << "expected valid identifier as case name"
         << lexer << diag::term;
   }

   string caseName = lexer->getCurrentIdentifier();
   EnumCaseDecl::AssociatedTypeVec associatedTypes;

   if (lexer->lookahead().is(tok::open_paren)) {
      lexer->advance();

      SourceLocation varargLoc;
      associatedTypes = parse_arg_list(varargLoc);
   }

   EnumCaseDecl* caseDecl;

   if (lexer->lookahead().is(tok::equals)) {
      lexer->advance();
      lexer->advance();

      auto expr = new (Context) StaticExpr(parse_static_expr());
      caseDecl = makeExpr<EnumCaseDecl>(start, std::move(caseName),
                                        std::move(expr),
                                        std::move(associatedTypes));
   }
   else {
      caseDecl = makeExpr<EnumCaseDecl>(start, std::move(caseName),
                                        std::move(associatedTypes));
   }

   return caseDecl;
}

} // namespace parse
} // namespace cdot