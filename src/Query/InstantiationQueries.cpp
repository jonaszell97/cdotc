#include "cdotc/AST/Decl.h"
#include "cdotc/Module/Module.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Sema/TemplateInstantiator.h"
#include "cdotc/Serialization/ModuleFile.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::support;

static void instantiateArgumentList(QueryContext& QC, ASTContext& Context,
                                    ArrayRef<FuncArgDecl*> Args, QualType Self,
                                    SmallVectorImpl<FuncArgDecl*>& Out)
{
   for (auto* Arg : Args) {
      if (Arg->isSelf()) {
         FuncArgDecl* SelfArg;
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

      auto* ArgInst = FuncArgDecl::Create(
          Context, Arg->getOwnershipLoc(), Arg->getColonLoc(),
          Arg->getDeclName(), Arg->getLabel(), Arg->getConvention(),
          SourceType(SubstTy), Arg->getDefaultVal(),
          Arg->isVariadicArgPackExpansion(), Arg->isCstyleVararg(),
          Arg->isSelf());

      Out.push_back(ArgInst);
   }
}

static MethodDecl* InstantiateMethodDefaultImpl(QueryContext& QC,
                                                MethodDecl* Impl, QualType Self)
{
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

   MethodDecl* Inst;
   if (auto* I = dyn_cast<InitDecl>(Impl)) {
      auto& Tbl = QC.Context.getDeclNameTable();
      Inst = InitDecl::Create(QC.Context, Impl->getAccess(),
                              Impl->getSourceLoc(), ArgInsts,
                              move(TemplateParams), nullptr,
                              Tbl.getConstructorName(Self), I->isFallible());
   }
   else {
      Inst = MethodDecl::Create(
          QC.Context, Impl->getAccess(), Impl->getDefLoc(), Impl->getDeclName(),
          SourceType(ReturnType), ArgInsts, move(TemplateParams), nullptr,
          Impl->isStatic());
   }

   Inst->setBodyTemplate(Impl);
   Inst->setFunctionFlags(Impl->getFunctionFlags());
   Inst->setInstantiatedFromProtocolDefaultImpl(true);

   QC.Sema->LookupContextMap[Inst] = Impl;

   if (Impl->isProtocolDefaultImpl()
   && !Inst->isTemplate() && !Self->getRecord()->isTemplate()
   && !cast<ProtocolDecl>(Impl->getRecord())->hasAssociatedTypeConstraint()) {
      QC.Sema->QueuedInstantiations.insert(Inst);
   }

   return Inst;
}

NamedDecl*
TemplateInstantiator::InstantiateProtocolDefaultImpl(NamedDecl *Impl,
                                                     QualType Self,
                                                     bool ActOnDecl)
{
   auto key = std::make_pair(Impl, (uintptr_t)Self.getAsOpaquePtr());

   auto it = InstMap.find(key);
   if (it != InstMap.end()) {
      return it->getSecond();
   }

   assert(isa<ExtensionDecl>(Impl->getNonTransparentDeclContext())
          && cast<ExtensionDecl>(Impl->getNonTransparentDeclContext())
             ->getExtendedRecord()
             ->isProtocol()
          && "not a protocol default impl!");

   if (QC.PrepareDeclInterface(Impl)) {
      return nullptr;
   }

   NamedDecl* Inst;
   if (auto* M = dyn_cast<MethodDecl>(Impl)) {
      Inst = InstantiateMethodDefaultImpl(QC, M, Self);
   }
   else if (auto* P = dyn_cast<PropDecl>(Impl)) {
      MethodDecl* Getter = nullptr;
      MethodDecl* Setter = nullptr;

      QualType SubstTy;
      if (QC.SubstAssociatedTypes(SubstTy, P->getType(), Self,
                                  P->getSourceLoc())) {
         SubstTy = QC.Context.getErrorTy();
      }

      if (auto* GetterImpl = P->getGetterMethod()) {
         Getter = InstantiateMethodDefaultImpl(QC, GetterImpl, Self);
      }
      if (auto* SetterImpl = P->getSetterMethod()) {
         Setter = InstantiateMethodDefaultImpl(QC, SetterImpl, Self);
      }

      Inst = PropDecl::Create(
         SP.Context, P->getAccess(), P->getSourceRange(), P->getDeclName(),
         SourceType(SubstTy), P->isStatic(), P->isReadWrite(), Getter, Setter);
   }
   else if (auto* S = dyn_cast<SubscriptDecl>(Impl)) {
      MethodDecl* Getter = nullptr;
      MethodDecl* Setter = nullptr;

      SourceType T;
      if (auto* GetterImpl = S->getGetterMethod()) {
         Getter = InstantiateMethodDefaultImpl(QC, GetterImpl, Self);

         if (Getter) {
            T = Getter->getReturnType();
         }
      }
      if (auto* SetterImpl = S->getSetterMethod()) {
         Setter = InstantiateMethodDefaultImpl(QC, SetterImpl, Self);

         if (!T && Setter) {
            T = Setter->getArgs().back()->getType();
         }
      }

      Inst = SubscriptDecl::Create(SP.Context, S->getAccess(),
                                   S->getSourceRange(), T, Getter, Setter);
   }
   else if (auto* alias = dyn_cast<AliasDecl>(Impl)) {
      QualType InstTy;
      if (QC.SubstAssociatedTypes(InstTy, alias->getAliasedType(), Self,
                                  alias->getSourceLoc())) {
         return nullptr;
      }

      assert(!InstTy->containsAssociatedType());

      if (!InstTy->isMetaType()) {
         InstTy = QC.Context.getMetaType(InstTy);
      }

      auto* typeExpr = new (QC.Context) IdentifierRefExpr(
         alias->getSourceLoc(), IdentifierKind::MetaType, InstTy);

      auto* rawTypeExpr = StaticExpr::Create(QC.Context, typeExpr);
      if (InstTy->containsTemplateParamType()) {
         typeExpr->setNeedsInstantiation(true);
         rawTypeExpr->setNeedsInstantiation(true);
      }

      Inst = AliasDecl::Create(QC.Context, alias->getSourceLoc(),
                               AccessSpecifier::Public, alias->getDeclName(),
                               InstTy, rawTypeExpr, {});
   }
   else {
      llvm_unreachable("bad protocol default implementation kind!");
   }

   if (!Inst) {
      return nullptr;
   }

   if (Impl->isImplOfProtocolRequirement()) {
      Inst->setImplOfProtocolRequirement(true);
      QC.Context.updateProtocolImpl(Self->getRecord(), Impl, Inst);
   }

   InstMap[key] = Inst;
   QC.Context.setAttributes(Inst, Impl->getAttributes());

   if (ActOnDecl) {
      QC.Sema->ActOnDecl(Self->getRecord(), Inst);
   }

   return Inst;
}

QueryResult CheckTemplateExtensionApplicabilityQuery::run()
{
   auto& Context = QC.CI.getContext();

   auto Constraints = QC.Sema->getDeclConstraints(Ext);
   if (Constraints->empty())
      return finish(true);

   QualType Ty = Context.getRecordType(Inst);

   bool AllSatisfied;
   for (auto* C : *Constraints) {
      if (QC.IsConstraintSatisfied(AllSatisfied, C, Ty, Ext)) {
         return fail();
      }

      if (!AllSatisfied) {
         return finish(false);
      }
   }

   return finish(true);
}

bool TemplateInstantiator::InstantiateFields(StructDecl *S)
{
   assert(S->isInstantiation() && "not an instantiation!");

   bool error = false;
   for (auto* F : S->getSpecializedTemplate()->getDecls<FieldDecl>()) {
      if (F->isStatic()) {
         continue;
      }

      NamedDecl* Inst = InstantiateTemplateMember(F, S);
      if (!Inst) {
         error = true;
      }
   }

   return error;
}

bool TemplateInstantiator::InstantiateCases(EnumDecl *E)
{
   assert(E->isInstantiation() && "not an instantiation!");

   bool error = false;
   for (auto* Case : E->getSpecializedTemplate()->getDecls<EnumCaseDecl>()) {
      NamedDecl* Inst = InstantiateTemplateMember(Case, E);
      if (!Inst) {
         error = true;
      }
   }

   return error;
}