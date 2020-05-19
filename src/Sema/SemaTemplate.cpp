#include "cdotc/ILGen/ILGenPass.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Sema/TemplateInstantiator.h"
#include "cdotc/Serialization/ModuleFile.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::sema;
using namespace cdot::support;

/// Instantiate a potential template member.
NamedDecl* SemaPass::maybeInstantiateTemplateMemberImpl(DeclContext* LookupCtx,
                                                        NamedDecl* Member)
{
   auto* TemplateCtx = Member->getRecord();
   if (auto* P = dyn_cast_or_null<ProtocolDecl>(TemplateCtx)) {
      RecordDecl* SelfRec = nullptr;
      while (LookupCtx) {
         SelfRec = dyn_cast<RecordDecl>(LookupCtx);
         if (SelfRec) {
            break;
         }

         LookupCtx = LookupCtx->getParentCtx();
      }

      if (!SelfRec || SelfRec == P || isa<ProtocolDecl>(SelfRec)
      || SelfRec->isTemplate()) {
         return Member;
      }

      QualType Self = Context.getRecordType(SelfRec);
      NamedDecl* Inst = Instantiator->InstantiateProtocolDefaultImpl(Member, Self);

      if (!Inst) {
         return Member;
      }

      return Inst;
   }

   if (!TemplateCtx || !TemplateCtx->isTemplateOrInTemplate()) {
      return Member;
   }

   LookupCtx = LookupCtx->lookThroughExtension();

   // Find the instantiation this member was found in.
   RecordDecl* Inst = nullptr;
   while (LookupCtx) {
      auto* CurInst = dyn_cast<RecordDecl>(LookupCtx);
      if (CurInst && CurInst->isInstantiation()
          && CurInst->getSpecializedTemplate() == TemplateCtx) {
         Inst = CurInst;
         break;
      }

      LookupCtx = LookupCtx->getParentCtx();
   }

   if (!Inst) {
      return Member;
   }

   NamedDecl* MemberInst = Instantiator->InstantiateTemplateMember(Member, Inst);
   if (MemberInst == nullptr) {
      MemberInst = Member;
   }

   if (QC.PrepareDeclInterface(MemberInst)) {
      MemberInst = Member;
   }

   return MemberInst;
}

bool SemaPass::maybeInstantiateRecord(CandidateSet::Candidate& Cand,
                                      const TemplateArgList& templateArgs,
                                      Statement* Caller)
{
   auto F = Cand.getFunc();
   Cand.OuterTemplateArgs
       = FinalTemplateArgumentList::Create(Context, templateArgs);

   if (templateArgs.isStillDependent()) {
      return true;
   }

   if (auto Case = dyn_cast<EnumCaseDecl>(F)) {
      if (!Case->getRecord()->isTemplate())
         return true;

      RecordDecl* Inst = Instantiator->InstantiateRecord(
          Case->getRecord(), Cand.OuterTemplateArgs, Caller->getSourceLoc());

      if (!Inst)
         return false;

      Cand.setCandDecl(maybeInstantiateTemplateMember(Inst, Case));
      return true;
   }

   auto Init = dyn_cast<InitDecl>(F);
   if (!Init || !Init->getRecord()->isTemplate())
      return true;

   RecordDecl* Inst = Instantiator->InstantiateRecord(
       Init->getRecord(), Cand.OuterTemplateArgs, Caller->getSourceLoc());

   if (Inst) {
      if (Init->isSynthesized()) {
         if (QC.DeclareMemberwiseInit(cast<StructDecl>(Inst))) {
            return false;
         }

         Cand.setCandDecl(cast<StructDecl>(Inst)->getMemberwiseInitializer());
      }
      else {
         Cand.setCandDecl(maybeInstantiateTemplateMember(Inst, Init));
      }

      if (QC.PrepareDeclInterface(Cand.getFunc())) {
         return false;
      }

      return true;
   }

   return false;
}

void SemaPass::maybeInstantiate(CandidateSet& CandSet, Statement* Caller)
{
   if (!CandSet) {
      return;
   }

   maybeInstantiate(CandSet.getBestMatch(), Caller);
}

static void checkIfCalledFromTemplate(SemaPass& SP, CallableDecl* C)
{
   if (!isa<NamedDecl>(SP.getDeclContext()))
      return;

   // Check if the function was called from a template.
   if (cast<NamedDecl>(SP.getDeclContext()).isInUnboundedTemplate()) {
      C->setCalledFromTemplate(true);
      SP.getILGen().notifyFunctionCalledInTemplate(C);
   }
}

void SemaPass::maybeInstantiate(CandidateSet::Candidate& Cand,
                                Statement* Caller)
{
   if (Cand.isAnonymousCandidate()
       || Cand.InnerTemplateArgs.isStillDependent()) {
      return;
   }

   auto F = Cand.getFunc();
   checkIfCalledFromTemplate(*this, F);

   // this function was deserialized from a module, so we need at least it's
   // IL definition
   if (auto Info = F->getLazyFnInfo()) {
      // if the function is a template, we also need it's body
      Info->loadBody(F);
   }

   if (!F->isTemplate()) {
      if (auto M = dyn_cast<MethodDecl>(F)) {
         Cand.setCandDecl(maybeInstantiateMemberFunction(M, Caller));
      }

      return;
   }

   auto& TAs = Cand.InnerTemplateArgs;
   auto* FinalList = FinalTemplateArgumentList::Create(Context, TAs);

   if (auto *CS = Context.getExtConstraints(F)) {
      if (!CS->empty()) {
         DeclConstraint *FailedConstraint = nullptr;
         for (auto *C : *CS) {
            bool satisfied = true;
            if (QC.IsConstraintSatisfied(satisfied, C, F, F, FinalList)) {
               continue;
            }

            if (!satisfied) {
               FailedConstraint = C;
               break;
            }
         }

         if (FailedConstraint) {
            Cand.setHasFailedConstraint(FailedConstraint);
            return;
         }
      }
   }

   if (auto Fn = dyn_cast<FunctionDecl>(Cand.getFunc())) {
      FunctionDecl* Inst = Instantiator->InstantiateFunction(
          Fn, FinalList, Caller->getSourceLoc());

      if (!Inst || QC.PrepareDeclInterface(Inst)) {
         return;
      }

      Cand.setCandDecl(Inst);
   }
   else {
      if (F->getRecord()->isProtocol()) {
         return;
      }

      MethodDecl* Inst = Instantiator->InstantiateMethod(
          cast<MethodDecl>(Cand.getFunc()), FinalList, Caller->getSourceLoc());

      if (!Inst || QC.PrepareDeclInterface(Inst)) {
         return;
      }

      Cand.setCandDecl(Inst);
   }

   if (!F->isUnboundedTemplate() && RuntimeGenerics) {
      Cand.getFunc()->setShouldBeSpecialized(true);
   }
   else {
      Cand.setCandDecl(maybeInstantiateMemberFunction(Cand.getFunc(), Caller));
   }
}

CallableDecl*
SemaPass::maybeInstantiateMemberFunction(CallableDecl* Fn, StmtOrDecl Caller,
                                         bool NeedImmediateInstantiation)
{
   if (Fn->getBody() || !Fn->getBodyTemplate()) {
      return Fn;
   }

   CallableDecl* Template = Fn->getBodyTemplate();
   if (Template->isInvalid()) {
      return Fn;
   }

   if (auto FnInfo = Template->getLazyFnInfo()) {
      FnInfo->loadBody(Template);
   }

   if (NeedImmediateInstantiation) {
      size_t prevSize = QueuedInstantiations.size();
      Instantiator->InstantiateFunctionBody(Fn);

      for (size_t i = prevSize; i < QueuedInstantiations.size(); ++i) {
         Instantiator->InstantiateFunctionBody(QueuedInstantiations[i]);
      }
   }
   else {
      QueuedInstantiations.insert(Fn);
   }

   return Fn;
}

RecordDecl* SemaPass::InstantiateRecord(SourceLocation POI, RecordDecl* R,
                                        FinalTemplateArgumentList* TemplateArgs)
{
   return Instantiator->InstantiateRecord(R, TemplateArgs, POI);
}

AliasDecl* SemaPass::InstantiateAlias(SourceLocation POI, AliasDecl* td,
                                      FinalTemplateArgumentList* TemplateArgs)
{
   return Instantiator->InstantiateAlias(td, TemplateArgs, POI);
}

RecordDecl* SemaPass::InstantiateRecord(SourceLocation POI, RecordDecl* R,
                                        const TemplateArgList& TemplateArgs)
{
   return InstantiateRecord(
       POI, R, FinalTemplateArgumentList::Create(Context, TemplateArgs));
}
