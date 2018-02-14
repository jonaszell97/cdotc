//
// Created by Jonas Zell on 05.12.17.
//

#ifndef CDOT_TEMPLATEINSTANTIATOR_H
#define CDOT_TEMPLATEINSTANTIATOR_H

#include <vector>
#include <llvm/ADT/ArrayRef.h>

#include "lex/SourceLocation.h"

namespace cdot {

struct SourceLocation;

namespace ast {
   class FunctionDecl;
   class RecordDecl;
   class MethodDecl;
   class DeclPass;
   class Statement;
   class StaticExpr;
   class SemaPass;
   class AliasDecl;
} // namespace ast

class TemplateArg;

namespace sema {
   class TemplateArgList;
} // namespace sema

class TemplateInstantiator {
public:
   static ast::RecordDecl *
   InstantiateRecord(ast::SemaPass &SP,
                     SourceLocation instantiatedFrom,
                     ast::RecordDecl *rec,
                     sema::TemplateArgList&& templateArgs,
                     bool *isNew = nullptr);

   static ast::FunctionDecl *
   InstantiateFunction(ast::SemaPass &SP,
                       SourceLocation instantiatedFrom,
                       ast::FunctionDecl *F,
                       sema::TemplateArgList&& templateArgs,
                       bool *isNew = nullptr);

   static ast::MethodDecl *
   InstantiateMethod(ast::SemaPass &SP,
                     SourceLocation instantiatedFrom,
                     ast::MethodDecl *M,
                     sema::TemplateArgList &&templateArgs,
                     bool *isNew = nullptr,
                     ast::RecordDecl *R = nullptr);

   static ast::MethodDecl *
   InstantiateProtocolDefaultImpl(ast::SemaPass &SP,
                                  SourceLocation instantiatedFrom,
                                  ast::RecordDecl *Rec,
                                  ast::MethodDecl *M);

   static ast::Statement*
   InstantiateStatement(ast::SemaPass &SP,
                        SourceLocation instantiatedFrom,
                        ast::Statement* stmt,
                        sema::TemplateArgList const& templateArgs);

   static ast::Statement*
   InstantiateMethodBody(ast::SemaPass &SP,
                         SourceLocation instantiatedFrom,
                         ast::MethodDecl const* baseMethod,
                         ast::MethodDecl *newMethod);

   static ast::StaticExpr*
   InstantiateStaticExpr(ast::SemaPass &SP,
                         SourceLocation instantiatedFrom,
                         ast::StaticExpr* stmt,
                         sema::TemplateArgList const& templateArgs);

   static ast::AliasDecl*
   InstantiateAlias(ast::SemaPass &SP,
                    ast::AliasDecl *alias,
                    SourceLocation instantiatedFrom,
                    sema::TemplateArgList&& templateArgs);
};

} // namespace cdot

#endif //CDOT_TEMPLATEINSTANTIATOR_H
