//
// Created by Jonas Zell on 27.08.18.
//

#include "AST/Decl.h"
#include "Module/Module.h"
#include "Sema/SemaPass.h"
#include "Serialization/ModuleFile.h"
#include "QueryContext.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::support;

static void instantiateArgumentList(QueryContext &QC, ASTContext &Context,
                                    ArrayRef<FuncArgDecl*> Args,
                                    QualType Self,
                                    SmallVectorImpl<FuncArgDecl*> &Out) {
   for (auto *Arg : Args) {
      if (Arg->isSelf()) {
         FuncArgDecl *SelfArg;
         if (QC.CreateSelfArgument(SelfArg, Self, Arg->getSourceLoc())) {
            return;
         }

         Out.push_back(SelfArg);
         continue;
      }

      QualType SubstTy;
      if (QC.SubstAssociatedTypes(SubstTy, Arg->getType(), Self,
                                  Arg->getSourceLoc())) {
         SubstTy = Context.getErrorTy();
      }

      auto *ArgInst = FuncArgDecl::Create(Context, Arg->getOwnershipLoc(),
                                          Arg->getColonLoc(), Arg->getDeclName(),
                                          Arg->getLabel(), Arg->getConvention(),
                                          SourceType(SubstTy), Arg->getDefaultVal(),
                                          Arg->isVariadicArgPackExpansion(),
                                          Arg->isCstyleVararg(), Arg->isSelf());

      Out.push_back(ArgInst);
   }
}

static MethodDecl *InstantiateMethodDefaultImpl(QueryContext &QC,
                                                MethodDecl *Impl,
                                                QualType Self) {
   SmallVector<FuncArgDecl*, 4> ArgInsts;
   instantiateArgumentList(QC, QC.Context, Impl->getArgs(), Self, ArgInsts);

   QualType ReturnType;
   if (QC.SubstAssociatedTypes(ReturnType, Impl->getReturnType(), Self,
                               Self->getRecord()->getSourceLoc())) {
      ReturnType = QC.Context.getErrorTy();
   }

   ASTVector<TemplateParamDecl*> TemplateParams(
      QC.Context, (unsigned)Impl->getTemplateParams().size());
   TemplateParams.append(QC.Context, Impl->getTemplateParams().begin(),
                         Impl->getTemplateParams().end());

   MethodDecl *Inst;
   if (auto *I = dyn_cast<InitDecl>(Impl)) {
      auto &Tbl = QC.Context.getDeclNameTable();
      Inst = InitDecl::Create(QC.Context, Impl->getAccess(),Impl->getSourceLoc(),
                              ArgInsts, move(TemplateParams), nullptr,
                              Tbl.getConstructorName(Self), I->isFallible());
   }
   else {
      Inst = MethodDecl::Create(QC.Context, Impl->getAccess(),Impl->getDefLoc(),
                                Impl->getDeclName(), SourceType(ReturnType),
                                ArgInsts, move(TemplateParams),
                                nullptr, Impl->isStatic());
   }

   Inst->setBodyTemplate(Impl);
   Inst->setFunctionFlags(Impl->getFunctionFlags());
   Inst->setInstantiatedFromProtocolDefaultImpl(true);

   return Inst;
}

QueryResult InstantiateProtocolDefaultImplQuery::run()
{
   if (QC.PrepareDeclInterface(Impl)) {
      return fail();
   }

   NamedDecl *Inst;
   if (auto *M = dyn_cast<MethodDecl>(Impl)) {
      Inst = InstantiateMethodDefaultImpl(QC, M, Self);
   }
   else if (auto *P = dyn_cast<PropDecl>(Impl)) {
      MethodDecl *Getter = nullptr;
      MethodDecl *Setter = nullptr;

      SourceType T;
      if (auto *GetterImpl = P->getGetterMethod()) {
         Getter = InstantiateMethodDefaultImpl(QC, GetterImpl, Self);

         if (Getter) {
            T = Getter->getReturnType();
         }
      }
      if (auto *SetterImpl = P->getSetterMethod()) {
         Setter = InstantiateMethodDefaultImpl(QC, SetterImpl, Self);

         if (!T && Setter) {
            T = Setter->getArgs().back()->getType();
         }
      }

      Inst = PropDecl::Create(sema().Context, P->getAccess(),
                              P->getSourceRange(), P->getDeclName(),
                              T, P->isStatic(), Getter, Setter);
   }
   else if (auto *S = dyn_cast<SubscriptDecl>(Impl)) {
      MethodDecl *Getter = nullptr;
      MethodDecl *Setter = nullptr;

      SourceType T;
      if (auto *GetterImpl = S->getGetterMethod()) {
         Getter = InstantiateMethodDefaultImpl(QC, GetterImpl, Self);

         if (Getter) {
            T = Getter->getReturnType();
         }
      }
      if (auto *SetterImpl = S->getSetterMethod()) {
         Setter = InstantiateMethodDefaultImpl(QC, SetterImpl, Self);

         if (!T && Setter) {
            T = Setter->getArgs().back()->getType();
         }
      }

      Inst = SubscriptDecl::Create(sema().Context, P->getAccess(),
                                   P->getSourceRange(), T, Getter, Setter);
   }
   else {
      llvm_unreachable("bad protocol default implementation kind!");
   }

   if (!Inst) {
      return fail();
   }

   QC.Sema->ActOnDecl(Self->getRecord(), Inst);
   return finish(Inst);
}

QueryResult CheckTemplateExtensionApplicabilityQuery::run()
{
   auto &Context = QC.CI.getContext();

   auto Constraints = Context.getExtConstraints(Ext);
   if (Constraints.empty())
      return finish(true);

   QualType Ty = Context.getRecordType(Inst);

   bool AllSatisfied;
   for (auto *C : Constraints) {
      if (QC.IsConstraintSatisfied(AllSatisfied, C, Ty, Ext)) {
         return fail();
      }

      if (!AllSatisfied) {
         return finish(false);
      }
   }

   return finish(true);
}