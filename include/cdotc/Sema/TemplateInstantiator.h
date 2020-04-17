#ifndef CDOT_TEMPLATEINSTANTIATOR_H
#define CDOT_TEMPLATEINSTANTIATOR_H

#include "cdotc/AST/StmtOrDecl.h"
#include "cdotc/AST/Type.h"
#include "cdotc/Lex/SourceLocation.h"
#include "cdotc/Sema/ActionResult.h"
#include "cdotc/Support/Optional.h"

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/DenseMap.h>

#include <queue>

namespace cdot {

class QueryContext;
struct QueryResult;

namespace il {
class Constant;
} // namespace il

namespace sema {
class TemplateArgList;
class MultiLevelTemplateArgList;
class MultiLevelFinalTemplateArgList;
class FinalTemplateArgumentList;
} // namespace sema

namespace ast {
class AliasDecl;
class CallableDecl;
class EnumDecl;
class Expression;
class FunctionDecl;
class MethodDecl;
class NamedDecl;
class RecordDecl;
class SemaPass;
class Statement;
class StaticExpr;
class StructDecl;

class TemplateInstantiator {
public:
   explicit TemplateInstantiator(SemaPass& SP);

   RecordDecl *InstantiateRecord(RecordDecl *Template,
                                 sema::FinalTemplateArgumentList *TemplateArgs,
                                 SourceLocation POI,
                                 RecordDecl *OuterInst = nullptr);

   AliasDecl *InstantiateAlias(AliasDecl *Template,
                               sema::FinalTemplateArgumentList *TemplateArgs,
                               SourceLocation POI);

   CallableDecl *InstantiateCallable(CallableDecl *Template,
                                     sema::FinalTemplateArgumentList *TemplateArgs,
                                     SourceLocation POI);

   FunctionDecl *InstantiateFunction(FunctionDecl *Template,
                                     sema::FinalTemplateArgumentList *TemplateArgs,
                                     SourceLocation POI);

   MethodDecl *InstantiateMethod(MethodDecl *Template,
                                 sema::FinalTemplateArgumentList *TemplateArgs,
                                 SourceLocation POI);

   bool InstantiateMethodBody(MethodDecl *MethodInst);

   Decl *InstantiateDecl(Decl *Template,
                         sema::MultiLevelFinalTemplateArgList *TemplateArgs,
                         SourceLocation POI);

   NamedDecl *InstantiateTemplateMember(NamedDecl *TemplateMember,
                                        RecordDecl *Inst,
                                        sema::FinalTemplateArgumentList *TemplateArgs = nullptr,
                                        SourceLocation POI = SourceLocation());

   NamedDecl *InstantiateProtocolDefaultImpl(NamedDecl *Impl,
                                             QualType Self,
                                             bool ActOnDecl = true);

   StmtResult InstantiateStatement(SourceLocation instantiatedFrom,
                                   ast::Statement* stmt,
                                   const sema::TemplateArgList& templateArgs);

   StmtResult
   InstantiateStatement(SourceLocation instantiatedFrom, ast::Statement* stmt,
                        sema::MultiLevelFinalTemplateArgList&& templateArgs);

   DeclResult
   InstantiateDecl(SourceLocation instantiatedFrom, ast::Decl* D,
                   sema::MultiLevelFinalTemplateArgList&& templateArgs);

   StmtResult InstantiateStatement(StmtOrDecl POI, ast::Statement* stmt,
                                   IdentifierInfo* SubstName,
                                   il::Constant* SubstVal);

   bool InstantiateFields(StructDecl *S);
   bool InstantiateCases(EnumDecl *E);

   unsigned getInstantiationDepth(ast::NamedDecl* Decl);
   void setInstantiationDepth(ast::NamedDecl* Decl, unsigned Depth);

   template<class T>
   T *getInstantiation(T *Template,
                       sema::FinalTemplateArgumentList* TemplateArgs) {
      return support::dyn_cast_or_null<T>(getInstantiationImpl((NamedDecl*)Template, TemplateArgs));
   }


   void registerInstantiation(NamedDecl *Template,
                              sema::FinalTemplateArgumentList* TemplateArgs,
                              NamedDecl *Inst);

private:
   SemaPass& SP;
   QueryContext &QC;
   unsigned InstantiationDepth = 0;

   llvm::DenseMap<NamedDecl*, unsigned> InstantiationDepthMap;
   llvm::DenseMap<std::pair<NamedDecl*, uintptr_t>, NamedDecl*> InstMap;

   NamedDecl *getInstantiationImpl(NamedDecl *Template,
                                   sema::FinalTemplateArgumentList* TemplateArgs);

   bool PrepareForInstantiation(NamedDecl *D);
};

} // namespace ast
} // namespace cdot

#endif // CDOT_TEMPLATEINSTANTIATOR_H
