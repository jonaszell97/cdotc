//
// Created by Jonas Zell on 05.12.17.
//

#ifndef CDOT_TEMPLATEINSTANTIATOR_H
#define CDOT_TEMPLATEINSTANTIATOR_H

#include "lex/SourceLocation.h"
#include "Variant/Type/Type.h"

#include <llvm/ADT/ArrayRef.h>

namespace cdot {

struct Variant;
struct SourceLocation;

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

class TemplateInstantiator {
public:
   using TemplateArgs = sema::TemplateArgList;

   explicit TemplateInstantiator(ast::SemaPass &SP)
      : SP(SP)
   {

   }

   ast::RecordDecl* InstantiateRecord(ast::Statement *POI,
                                      ast::RecordDecl *rec,
                                      TemplateArgs&& templateArgs,
                                      bool *isNew = nullptr);

   ast::FunctionDecl* InstantiateFunction(ast::Statement *POI,
                                          ast::FunctionDecl *F,
                                          TemplateArgs&& templateArgs,
                                          bool *isNew = nullptr);

   ast::MethodDecl *InstantiateMethod(ast::Statement *POI,
                                      ast::MethodDecl *M,
                                      TemplateArgs&& templateArgs,
                                      bool *isNew = nullptr);

   ast::MethodDecl*
   InstantiateProtocolDefaultImpl(SourceLocation instantiatedFrom,
                                  ast::RecordDecl *Rec,
                                  ast::MethodDecl *M);

   ast::Statement* InstantiateStatement(SourceLocation instantiatedFrom,
                                        ast::Statement* stmt,
                                        TemplateArgs const& templateArgs);

   ast::Statement* InstantiateStatement(ast::Statement *POI,
                                        ast::Statement* stmt,
                                        llvm::StringRef SubstName,
                                        QualType SubstTy,
                                        const Variant &SubstVal);

   ast::Statement *InstantiateMethodBody(ast::Statement *POI,
                                         ast::MethodDecl *Method);

   ast::Expression*
   InstantiateStaticExpr(SourceLocation instantiatedFrom,
                         ast::Expression* stmt,
                         TemplateArgs const& templateArgs);

   ast::AliasDecl* InstantiateAlias(ast::AliasDecl *alias,
                                    SourceLocation instantiatedFrom,
                                    TemplateArgs&& templateArgs);

private:
   ast::SemaPass &SP;
};

} // namespace cdot

#endif //CDOT_TEMPLATEINSTANTIATOR_H
