//
// Created by Jonas Zell on 05.12.17.
//

#ifndef CDOT_TEMPLATEINSTANTIATOR_H
#define CDOT_TEMPLATEINSTANTIATOR_H

#include "AST/Type.h"
#include "AST/StmtOrDecl.h"
#include "Lex/SourceLocation.h"
#include "Sema/ActionResult.h"
#include "Support/Optional.h"

#include <llvm/ADT/ArrayRef.h>

namespace cdot {

struct Variant;
struct SourceLocation;
class IdentifierInfo;

namespace ast {
   class Expression;
   class NamedDecl;
   class FunctionDecl;
   class RecordDecl;
   class MethodDecl;
   class Statement;
   class StaticExpr;
   class SemaPass;
   class AliasDecl;
} // namespace ast

namespace sema {
   class TemplateArgList;
   class MultiLevelTemplateArgList;
} // namespace sema

template<> struct IsLowBitAvailable<ast::RecordDecl*> {
   static constexpr bool value = true;
};

template<> struct IsLowBitAvailable<ast::FunctionDecl*> {
   static constexpr bool value = true;
};

template<> struct IsLowBitAvailable<ast::MethodDecl*> {
   static constexpr bool value = true;
};

template<> struct IsLowBitAvailable<ast::AliasDecl*> {
   static constexpr bool value = true;
};

using RecordInstResult   = ActionResult<ast::RecordDecl*>;
using FunctionInstResult = ActionResult<ast::FunctionDecl*>;
using MethodInstResult   = ActionResult<ast::MethodDecl*>;
using AliasInstResult    = ActionResult<ast::AliasDecl*>;

class TemplateInstantiator {
public:
   using TemplateArgs = sema::TemplateArgList;

   explicit TemplateInstantiator(ast::SemaPass &SP)
      : SP(SP)
   {

   }

   RecordInstResult InstantiateRecord(StmtOrDecl POI,
                                      ast::RecordDecl *rec,
                                      TemplateArgs&& templateArgs,
                                      bool *isNew = nullptr);

   FunctionInstResult InstantiateFunction(StmtOrDecl POI,
                                          ast::FunctionDecl *F,
                                          TemplateArgs&& templateArgs,
                                          bool *isNew = nullptr);

   MethodInstResult InstantiateMethod(StmtOrDecl POI,
                                      ast::MethodDecl *M,
                                      TemplateArgs&& templateArgs,
                                      bool *isNew = nullptr);

   MethodInstResult
   InstantiateProtocolDefaultImpl(SourceLocation instantiatedFrom,
                                  ast::RecordDecl *Rec,
                                  ast::MethodDecl *M);

   StmtResult
   InstantiateStatement(SourceLocation instantiatedFrom,
                        ast::Statement* stmt,
                        sema::MultiLevelTemplateArgList &&templateArgs);

   StmtResult InstantiateStatement(StmtOrDecl POI,
                                   ast::Statement* stmt,
                                   IdentifierInfo *SubstName,
                                   QualType SubstTy,
                                   const Variant &SubstVal);

   StmtResult InstantiateMethodBody(StmtOrDecl POI,
                                    ast::MethodDecl *Method);

   ExprResult
   InstantiateStaticExpr(SourceLocation instantiatedFrom,
                         ast::Expression* stmt,
                         sema::MultiLevelTemplateArgList &&templateArgs);

   AliasInstResult InstantiateAlias(ast::AliasDecl *alias,
                                    SourceLocation instantiatedFrom,
                                    TemplateArgs&& templateArgs);

private:
   ast::SemaPass &SP;
   bool InstantiatingRecord = false;
};

} // namespace cdot

#endif //CDOT_TEMPLATEINSTANTIATOR_H
