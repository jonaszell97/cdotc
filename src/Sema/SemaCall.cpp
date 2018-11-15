//
// Created by Jonas Zell on 24.10.17.
//

#include "SemaPass.h"

#include "Basic/NestedNameSpecifier.h"
#include "ILGen/ILGenPass.h"
#include "Message/Diagnostics.h"
#include "Query/QueryContext.h"
#include "Sema/OverloadResolver.h"
#include "Sema/Builtin.h"
#include "Serialization/ModuleFile.h"
#include "Support/StringSwitch.h"
#include "TemplateInstantiator.h"

#include <llvm/ADT/SmallString.h>

using namespace cdot::diag;
using namespace cdot::sema;
using namespace cdot::support;

namespace cdot {
namespace ast {

static bool addCandidates(const MultiLevelLookupResult &lookupRes,
                          CandidateSet &CandSet) {
   // add candidates found via normal lookup
   unsigned Distance = 0;
   for (auto &Lookup : lookupRes) {
      for (auto &D : Lookup) {
         if (auto CD = dyn_cast<CallableDecl>(D)) {
            if (auto M = dyn_cast<MethodDecl>(D)) {
               // only consider the 'deepest' override
               if (M->isOverride())
                  CandSet.CandidateFns.insert(M->getOverridenMethod());

               if (M->isProtocolDefaultImpl() && !M->isProtocolRequirement())
                  continue;
            }

            CandSet.addCandidate(CD, Distance);
         }
      }

      ++Distance;
   }

   return false;
}

static bool addCandidates(SemaPass &Sema,
                          DeclContext *Ctx,
                          DeclarationName name,
                          CandidateSet &CandSet,
                          bool IsLocalLookup) {
   LookupOpts Opts = DefaultLookupOpts;
   if (IsLocalLookup) {
      Opts |= LookupOpts::LocalLookup;
   }

   const MultiLevelLookupResult *LookupRes;
   if (Sema.QC.MultiLevelLookup(LookupRes, Ctx, name, Opts)) {
      return true;
   }

   return addCandidates(*LookupRes, CandSet);
}

std::pair<DeclContext*, bool> SemaPass::getAsContext(QualType Ty)
{
   if (auto RecTy = Ty->asRecordType()) {
      return { RecTy->getRecord(), false };
   }

   if (auto Meta = Ty->asMetaType()) {
      if (auto MetaRecTy = Meta->getUnderlyingType()->asRecordType()) {
         return { MetaRecTy->getRecord(), true };
      }
   }

   return { nullptr, false };
}

NamedDecl *SemaPass::getTypeDecl(cdot::QualType Ty)
{
   if (auto Meta = Ty->asMetaType()) {
      Ty = Meta->getUnderlyingType();
   }

   if (auto RecTy = Ty->asRecordType()) {
      return RecTy->getRecord();
   }

   return nullptr;
}

static bool addUFCSCandidates(SemaPass &Sema,
                              DeclContext *Ctx,
                              DeclarationName name,
                              CandidateSet &CandSet) {
   const MultiLevelLookupResult *LookupRes;
   if (Sema.QC.MultiLevelLookup(LookupRes, Ctx, name)) {
      return true;
   }

   // add candidates found via normal lookup
   unsigned Distance = 0;
   for (auto &Lookup : *LookupRes) {
      for (auto &D : Lookup) {
         if (auto CD = dyn_cast<CallableDecl>(D)) {
            if (auto M = dyn_cast<MethodDecl>(D)) {
               // only consider the 'deepest' override
               if (M->isOverride())
                  CandSet.CandidateFns.insert(M->getOverridenMethod());

               if (M->isProtocolDefaultImpl() && !M->isProtocolRequirement())
                  continue;
            }

            auto *Cand = CandSet.addCandidate(CD, Distance);
            if (Cand && isa<MethodDecl>(D))
               Cand->UFCS = CandidateSet::MethodCalledAsFunction;
         }
      }

      ++Distance;
   }

   return false;
}

static bool addUFCSCandidates(SemaPass &SP,
                              Expression *FirstArg,
                              DeclarationName name,
                              CandidateSet &CandSet) {
   QualType FirstArgTy;
   if (FirstArg->isContextDependent()) {
      FirstArgTy = SP.GetDefaultExprType(FirstArg);
   }
   else {
      FirstArgTy = FirstArg->getExprType()->stripReference()
         ->getDesugaredType();
   }

   if (!FirstArgTy)
      return false;

   auto ContextAndIsStatic = SP.getAsContext(FirstArgTy);
   if (ContextAndIsStatic.first) {
      return addUFCSCandidates(SP, ContextAndIsStatic.first, name, CandSet);
   }

   return false;
}

CandidateSet
SemaPass::lookupFunction(DeclarationName name,
                         ArrayRef<Expression*> args,
                         ArrayRef<Expression*> templateArgs,
                         ArrayRef<IdentifierInfo*> labels,
                         Statement *Caller,
                         bool suppressDiags) {
   return lookupFunction(&getDeclContext(), name, args,
                         templateArgs, labels, Caller, suppressDiags);
}

CandidateSet
SemaPass::lookupFunction(DeclarationName name,
                         Expression *SelfArg,
                         ArrayRef<Expression *> args,
                         ArrayRef<Expression *> templateArgs,
                         ArrayRef<IdentifierInfo*> labels,
                         Statement *Caller,
                         bool suppressDiags) {
   return lookupFunction(&getDeclContext(), name, SelfArg, args,
                         templateArgs, labels, Caller, suppressDiags);
}

CandidateSet
SemaPass::lookupFunction(DeclContext *Ctx,
                         DeclarationName name,
                         ArrayRef<Expression *> args,
                         ArrayRef<Expression *> templateArgs,
                         ArrayRef<IdentifierInfo*> labels,
                         Statement *Caller,
                         bool suppressDiags) {
   return lookupFunction(Ctx, name, nullptr, args, templateArgs, labels, Caller,
                         suppressDiags);
}

CandidateSet
SemaPass::lookupFunction(DeclContext *Ctx,
                         DeclarationName name,
                         Expression *SelfArg,
                         ArrayRef<Expression *> args,
                         ArrayRef<Expression *> templateArgs,
                         ArrayRef<IdentifierInfo*> labels,
                         Statement *Caller,
                         bool suppressDiags) {
   CandidateSet CandSet;
   bool IsLocalLookup = Ctx == &getDeclContext();
   bool DoUFCSLookup = IsLocalLookup;

   addCandidates(*this, Ctx, name, CandSet, IsLocalLookup);

   // check if we can find additional candidates through UFCS
   if (DoUFCSLookup && !args.empty()) {
      auto First = args.front();
      addUFCSCandidates(*this, First, name, CandSet);
   }

   CandBuilder.addBuiltinCandidates(CandSet, name, args);

   lookupFunction(CandSet, name, SelfArg, args, templateArgs, labels, Caller,
                  suppressDiags);

   return CandSet;
}

CandidateSet SemaPass::getCandidates(DeclarationName name,
                                     Expression *SelfExpr) {
   CandidateSet CandSet;
   if (addCandidates(*this, &getDeclContext(), name, CandSet, true)) {
      return CandSet;
   }
   if (addUFCSCandidates(*this, SelfExpr, name, CandSet)) {
      return CandSet;
   }

   CandBuilder.addBuiltinCandidates(CandSet, name, SelfExpr->getExprType());
   return CandSet;
}

CandidateSet
SemaPass::lookupCase(DeclarationName name,
                     EnumDecl *E,
                     ArrayRef<Expression *> args,
                     ArrayRef<Expression *> templateArgs,
                     ArrayRef<IdentifierInfo*> labels,
                     Statement *Caller,
                     bool suppressDiags) {
   CandidateSet CandSet;

   if (auto Case = E->hasCase(name))
      CandSet.addCandidate(Case);

   lookupFunction(CandSet, name, args, templateArgs, labels, Caller,
                  suppressDiags);

   return CandSet;
}

CandidateSet SemaPass::checkAnonymousCall(FunctionType *FTy,
                                          ArrayRef<Expression *> args,
                                          ArrayRef<IdentifierInfo*> labels,
                                          Statement *Caller) {
   CandidateSet CandSet;
   CandSet.addCandidate(FTy);

   lookupFunction(CandSet, DeclarationName(), args, {}, labels, Caller, true);
   if (!CandSet && !CandSet.isDependent()) {
      CandSet.diagnoseAnonymous(*this, args, Caller);
   }

   return CandSet;
}

static void checkIfCalledFromTemplate(SemaPass &SP, CallableDecl *C)
{
   if (!isa<NamedDecl>(SP.getDeclContext()))
      return;

   // Check if the function was called from a template.
   if (cast<NamedDecl>(SP.getDeclContext()).isInUnboundedTemplate()) {
      C->setCalledFromTemplate(true);
      SP.getILGen().notifyFunctionCalledInTemplate(C);
   }
}

void SemaPass::lookupFunction(CandidateSet &CandSet,
                              DeclarationName name,
                              ArrayRef<Expression*> args,
                              ArrayRef<Expression*> templateArgs,
                              ArrayRef<IdentifierInfo*> labels,
                              Statement *Expr,
                              bool suppressDiags) {
   lookupFunction(CandSet, name, nullptr, args, templateArgs, labels, Expr,
                  suppressDiags);
}

void SemaPass::lookupFunction(CandidateSet &CandSet,
                              DeclarationName name,
                              Expression *SelfArg,
                              ArrayRef<Expression*> args,
                              ArrayRef<Expression*> templateArgs,
                              ArrayRef<IdentifierInfo*> labels,
                              Statement *Expr,
                              bool suppressDiags) {
   OverloadResolver Resolver(*this, name, SelfArg, args, templateArgs,
                             labels, Expr);

   Resolver.resolve(CandSet);

   if (!CandSet) {
      if (CandSet.isDependent()) {
         for (auto &Cand : CandSet.Candidates) {
            if (!Cand.isBuiltinCandidate())
               checkIfCalledFromTemplate(*this, Cand.Func);
         }

         return;
      }

      if (suppressDiags)
         return;

      return CandSet.diagnose(*this, name, args, templateArgs, Expr);
   }
}

MethodDecl *SemaPass::getEquivalentMethod(MethodDecl *Orig,
                                          RecordDecl *Inst) {
   if (Orig->isMemberwiseInitializer())
      return cast<StructDecl>(Inst)->getMemberwiseInitializer();

   if (isa<DeinitDecl>(Orig))
      return Inst->getDeinitializer();

   DeclarationName Name = Orig->getDeclName();
   switch (Name.getKind()) {
   case DeclarationName::ConstructorName:
   case DeclarationName::BaseConstructorName:
      Name = Context.getDeclNameTable().getConstructorName(
         Context.getRecordType(Inst),
         Name.getKind() == DeclarationName::ConstructorName);

      break;
   case DeclarationName::DestructorName:
      Name = Context.getDeclNameTable().getDestructorName(
         Context.getRecordType(Inst));
      break;
   default:
      break;
   }

   const MultiLevelLookupResult *LookupRes;
   if (QC.MultiLevelLookup(LookupRes, Inst, Name)) {
      return Orig;
   }

   for (auto *D : LookupRes->allDecls()) {
      auto *M = dyn_cast<MethodDecl>(D);
      if (!M)
         continue;

      if (M->getMethodID() == Orig->getMethodID())
         return M;
   }

   llvm_unreachable("no equivalent method!");
}

bool SemaPass::maybeInstantiateRecord(CandidateSet::Candidate &Cand,
                                      const TemplateArgList &templateArgs,
                                      Statement *Caller) {
   auto F = Cand.Func;
   Cand.OuterTemplateArgs = FinalTemplateArgumentList::Create(Context,
                                                              templateArgs);

   if (auto Case = dyn_cast<EnumCaseDecl>(F)) {
      if (!Case->getRecord()->isTemplate())
         return true;

      RecordDecl *Inst;
      if (QC.InstantiateRecord(Inst, Case->getRecord(), Cand.OuterTemplateArgs,
                               Caller->getSourceLoc())) {
         Inst = nullptr;
      }

      if (!Inst)
         return false;

      Cand.Func = maybeInstantiateTemplateMember(Inst, Case);
      return true;
   }

   auto Init = dyn_cast<InitDecl>(F);
   if (!Init || !Init->getRecord()->isTemplate())
      return true;

   RecordDecl *Inst;
   if (QC.InstantiateRecord(Inst, Init->getRecord(), Cand.OuterTemplateArgs,
                            Caller->getSourceLoc())) {
      Inst = nullptr;
   }

   if (Inst) {
      if (Init->isSynthesized()) {
         if (QC.DeclareImplicitInitializers(Inst)) {
            return false;
         }

         Cand.Func = cast<StructDecl>(Inst)->getMemberwiseInitializer();
      }
      else {
         Cand.Func = maybeInstantiateTemplateMember(Inst, Init);
      }

      if (QC.PrepareDeclInterface(Cand.Func)) {
         return false;
      }

      return true;
   }

   return false;
}

void SemaPass::maybeInstantiate(CandidateSet &CandSet,
                                Statement *Caller) {
   if (!CandSet)
      return;

   auto &Cand = CandSet.getBestMatch();
   if (!Cand.Func || Cand.InnerTemplateArgs.isStillDependent()
         || Cand.isBuiltinCandidate()) {
      return;
   }

   auto F = Cand.Func;
   checkIfCalledFromTemplate(*this, F);

   // this function was deserialized from a module, so we need at least it's
   // IL definition
   if (auto Info = F->getLazyFnInfo()) {
      // if the function is a template, we also need it's body
      if (F->isUnboundedTemplate())
         Info->loadBody(F);
   }

   // FIXME runtime-generics
   if (!F->isTemplate()) {
      if (auto M = dyn_cast<MethodDecl>(F)) {
         Cand.Func = maybeInstantiateMemberFunction(M, Caller);
      }

      return;
   }

   auto &TAs = Cand.InnerTemplateArgs;
   auto *FinalList = FinalTemplateArgumentList::Create(Context, TAs);

   if (auto Fn = dyn_cast<FunctionDecl>(Cand.Func)) {
      FunctionDecl *Inst;
      if (QC.InstantiateFunction(Inst, Fn, FinalList,
                                 Caller->getSourceLoc())) {
         return;
      }
      if (QC.PrepareDeclInterface(Inst)) {
         return;
      }

      Cand.Func = Inst;
   }
   else {
      MethodDecl *Inst;
      if (QC.InstantiateMethod(Inst, cast<MethodDecl>(Cand.Func), FinalList,
                               Caller->getSourceLoc())) {
         return;
      }
      if (QC.PrepareDeclInterface(Inst)) {
         return;
      }

      Cand.Func = Inst;
   }

   if (!F->isUnboundedTemplate() && RuntimeGenerics) {
      Cand.Func->setShouldBeSpecialized(true);
   }
   else {
      Cand.Func = maybeInstantiateMemberFunction(Cand.Func, Caller);
   }
}

CallableDecl *SemaPass::maybeInstantiateMemberFunction(CallableDecl *M,
                                                       StmtOrDecl Caller) {
   if (M->getBody() || !M->getBodyTemplate()) {
      return M;
   }

   CallableDecl *Template = M->getBodyTemplate();
   if (Template->isInvalid()) {
      return M;
   }

   if (auto FnInfo = Template->getLazyFnInfo()) {
      FnInfo->loadBody(Template);
   }

   if (!Template->isInUnboundedTemplate()
         && !Template->isExternal()
         && RuntimeGenerics) {
      M->setShouldBeSpecialized(true);
      return M;
   }

   QC.InstantiateMethodBody(M);
   return M;
}

MethodDecl *SemaPass::InstantiateMethod(RecordDecl *R, StringRef Name,
                                        StmtOrDecl SOD) {
   auto *II = &Context.getIdentifiers().get(Name);
   auto *M = LookupSingle<MethodDecl>(*R, II);

   assert(M && "method does not exist!");
   maybeInstantiateMemberFunction(M, SOD);

   if (inCTFE())
      ILGen->prepareFunctionForCtfe(M, SOD);

   return M;
}

MethodDecl *SemaPass::InstantiateProperty(RecordDecl *R,
                                          StringRef Name,
                                          bool FindGetter,
                                          StmtOrDecl SOD) {
   auto *II = &Context.getIdentifiers().get(Name);

   if (FindGetter) {
      auto GetterName = Context.getDeclNameTable()
                               .getAccessorName(*II,
                                                DeclarationName::AccessorKind::Getter);

      auto *Getter = LookupSingle<MethodDecl>(*R, GetterName);
      assert(Getter && "method does not exist!");

      maybeInstantiateMemberFunction(Getter, SOD);

      if (inCTFE())
         ILGen->prepareFunctionForCtfe(Getter, SOD);

      return Getter;
   }

   auto SetterName = Context.getDeclNameTable()
                            .getAccessorName(*II,
                                         DeclarationName::AccessorKind::Setter);

   auto *Setter = LookupSingle<MethodDecl>(*R, SetterName);
   assert(Setter && "method does not exist!");

   maybeInstantiateMemberFunction(Setter, SOD);

   if (inCTFE())
      ILGen->prepareFunctionForCtfe(Setter, SOD);

   return Setter;
}

RecordDecl *SemaPass::InstantiateRecord(SourceLocation POI,
                                        RecordDecl *R,
                                        FinalTemplateArgumentList *TemplateArgs){
   RecordDecl *Inst;
   if (QC.InstantiateRecord(Inst, R, TemplateArgs, POI)) {
      return nullptr;
   }

   return Inst;
}

RecordDecl *SemaPass::InstantiateRecord(SourceLocation POI,
                                        RecordDecl *R,
                                        const TemplateArgList &TemplateArgs) {
   return InstantiateRecord(
      POI, R, FinalTemplateArgumentList::Create(Context, TemplateArgs));
}

static bool wouldSelfBeValid(SemaPass &SP, bool &Static)
{
   Static = false;

   auto *Ctx = &SP.getDeclContext();
   switch (Ctx->getDeclKind()) {
   case Decl::MethodDeclID:
      Static = cast<MethodDecl>(Ctx)->isStatic();
      LLVM_FALLTHROUGH;
   case Decl::InitDeclID:
   case Decl::DeinitDeclID:
      return true;
   default:
      return false;
   }
}

template<class Expr>
static void visitContextDependentArgs(SemaPass &Sema, Expr *C)
{
   for (auto *Arg : C->getArgs()) {
      Sema.visitTypeDependentContextualExpr(Arg);
      C->copyStatusFlags(Arg);
   }
}

ExprResult SemaPass::visitCallExpr(CallExpr *Call,
                                   TemplateArgListExpr *ArgExpr) {
   MutableArrayRef<Expression*> TemplateArgs;
   if (ArgExpr)
      TemplateArgs = ArgExpr->getExprs();

   bool Dependent = false;
   auto &args = Call->getArgs();
   for (auto &arg : args) {
      if (!arg->isContextDependent()) {
         auto result = visitExpr(Call, arg);
         if (!result)
            return ExprError();

         arg = result.get();
         Dependent |= arg->isDependent();
      }
   }

   // expression was previously resolved (for example by a binary operator node)
   if (Call->getKind() != CallKind::Unknown) {
      return Call;
   }

   // check if '.init' is called without a parent expression
   if (!Call->getParentExpr() && Call->isDotInit() && !Call->includesSelf()) {
      diagnose(Call, err_dot_init_must_be_on_self, Call->getSourceRange());
      Call->setExprType(Context.getVoidType());

      return Call;
   }

   bool IsLocalLookup = true;
   DeclContext *Ctx = Call->getContext();

   // We only perform UFCS lookup if no explicit context is provided.
   bool DoUFCSLookup = true;

   // Check if the call's parent expr refers to a namespace or module.
   auto *NameSpec = checkNamespaceRef(Call);
   if (Call->isInvalid()) {
      return ExprError();
   }

   if (NameSpec) {
      if (NameSpec->isAnyNameDependent()) {
         visitContextDependentArgs(*this, Call);
         Call->setIsTypeDependent(true);
         Call->setExprType(UnknownAnyTy);

         return Call;
      }

      Ctx = Call->getContext();
      IsLocalLookup = false;
      DoUFCSLookup = false;
   }

   auto ParentExpr = Call->getParentExpr();
   QualType ParentType;
   QualType ParentTypeNoSugar;

   if (ParentExpr && !refersToNamespace(ParentExpr)) {
      ParentExpr->setAllowNamespaceRef(true);

      auto ParentResult = visitExpr(Call, ParentExpr);
      if (!ParentResult)
         return ExprError();

      ParentExpr = ParentResult.get();
      Call->setParentExpr(ParentExpr);
      ParentType = checkCurrentTypeCapabilities(
         ParentExpr->getExprType()
         ->getCanonicalType()->stripReference());

      ParentTypeNoSugar = ParentType->getDesugaredType();
      if (ParentTypeNoSugar->isDependentType()) {
         visitContextDependentArgs(*this, Call);
         Call->setIsTypeDependent(true);
         Call->setExprType(UnknownAnyTy);

         return Call;
      }

      // Handle '->'
      if (ParentTypeNoSugar->isPointerType() && Call->isPointerAccess()) {
         QualType RefTy;
         if (ParentType->isMutablePointerType()) {
            RefTy = Context.getMutableReferenceType(ParentTypeNoSugar->getPointeeType());
         }
         else {
            RefTy = Context.getReferenceType(ParentTypeNoSugar->getPointeeType());
         }

         FunctionType *DerefFnTy = Context.getFunctionType(RefTy, { ParentTypeNoSugar });
         auto Deref = UnaryOperator::Create(Context, Call->getSourceLoc(),
                                            op::Deref, DerefFnTy,
                                            castToRValue(ParentExpr),
                                            true);

         auto Res = visitExpr(Call, Deref);
         assert(Res && "invalid deref operator?");

         Call->setParentExpr(Res.get());
         Call->setIsPointerAccess(false);

         ParentExpr = Call->getParentExpr();
         ParentType = ParentExpr->getExprType()->stripReference();
         ParentTypeNoSugar = ParentType->getDesugaredType();
      }
      else if (Call->isPointerAccess()) {
         diagnose(Call, err_member_access_non_pointer, ParentType,
                  Call->getSourceLoc());
      }

      if (Call->isDotInit()) {
         // '.init' must either be called on self, in which case it has to be
         // in an initializer, or statically on a type
         if (auto Meta = ParentTypeNoSugar->asMetaType()) {
            auto UnderlyingTy = Meta->getUnderlyingType();
            if (!UnderlyingTy->isRecordType()) {
               diagnose(Call, err_dot_init_bad_type, UnderlyingTy,
                        Call->getSourceRange());

               Call->setExprType(UnderlyingTy);
               return Call;
            }

            // treat this as an initializer call
            Call->setIdent(Context.getDeclNameTable()
                                  .getConstructorName(UnderlyingTy));

            Call->setIsDotInit(false);
         }
         else if (!isa<SelfExpr>(ParentExpr->ignoreParens())
               && !isa<SuperExpr>(ParentExpr->ignoreParens())) {
            diagnose(Call, err_dot_init_must_be_on_self, Call->getSourceRange());
            Call->setExprType(Context.getVoidType());

            return Call;
         }
         else {
            // delegating initializer call
            Call->setIdent(Context.getDeclNameTable()
                                  .getConstructorName(ParentTypeNoSugar));
         }
      }

      IsLocalLookup = false;

      auto ContextAndIsStatic = getAsContext(ParentTypeNoSugar);
      Call->setDirectCall(isa<SuperExpr>(ParentExpr)
                            || ContextAndIsStatic.second);

      ParentType = ParentType->stripMetaType();
      ParentTypeNoSugar = ParentType->getDesugaredType();
   }
   else if (Call->isPointerAccess()) {
      diagnose(Call, err_member_access_non_pointer, "<namespace>",
               Call->getSourceLoc());
   }
   else if (Call->hasLeadingDot()) {
      auto CtxTy = Call->getContextualType();
      if (!CtxTy || !CtxTy->isRecordType()) {
         diagnose(Call, err_requires_contextual_type, 2,
                  Call->getSourceRange());

         return ExprError();
      }

      auto *R = CtxTy->getRecord();
      if (QC.PrepareDeclInterface(R)) {
         visitContextDependentArgs(*this, Call);
         Call->setExprType(Context.getRecordType(R));
         return Call;
      }

      Ctx = R;
   }

   if (!Ctx) {
      Ctx = &getDeclContext();
   }

   // The name to look for.
   DeclarationName Name = Call->getDeclName();

   // The set of function candidates.
   CandidateSet CandSet;

   // We need to track if the called function is a subscript setter to adjust
   // the return type later.
   bool IsSubscriptSetter = false;

   LookupOpts Opts = DefaultLookupOpts;
   if (IsLocalLookup) {
      Opts |= LookupOpts::LocalLookup;
   }

   // FIXME primitive-extensions
   bool DoTypeLookup = ParentTypeNoSugar
      && (ParentTypeNoSugar->isRecordType()
          || ParentTypeNoSugar->isExistentialType());

   const MultiLevelLookupResult *Result;
   if (DoTypeLookup) {
      if (QC.MultiLevelTypeLookup(Result, ParentTypeNoSugar, Name, Opts)) {
         return ExprError();
      }
   }
   else {
      if (QC.MultiLevelLookup(Result, Ctx, Name, Opts)) {
         return ExprError();
      }
   }

   if (!Result->empty()) {
      // check if we found something that is not a function first, no need to do
      // overload resolution or check UFCS here
      auto ND = Result->front().front();
      switch (ND->getKind()) {
      case Decl::EnumCaseDeclID: {
         auto enumCaseExpr =
            new(getContext()) EnumCaseExpr(Call->getSourceLoc(),
                                           cast<EnumCaseDecl>(ND),
                                           move(Call->getArgs()));

         enumCaseExpr->setContextualType(Call->getContextualType());
         return visitExpr(Call, enumCaseExpr);
      }
      case Decl::SubscriptDeclID: {
         bool FoundGetter = false;
         bool FoundSetter = false;

         for (auto &Decls : *Result) {
            for (auto *Decl : Decls) {
               if (auto *SD = dyn_cast<SubscriptDecl>(Decl)) {
                  FoundGetter |= SD->hasGetter();
                  FoundSetter |= SD->hasSetter();
               }
            }

            if (FoundGetter && FoundSetter)
               break;
         }

         if (Call->isLHSOfAssignment()) {
            if (!FoundSetter) {
               diagnose(Call, err_subscript_does_not_have, 1,
                        Call->getSourceLoc());
            }

            Call->setIdent(Context.getDeclNameTable().getSubscriptName(
               DeclarationName::SubscriptKind::Setter));

            IsSubscriptSetter = true;
         }
         else {
            if (!FoundGetter) {
               diagnose(Call, err_subscript_does_not_have, 0,
                        Call->getSourceLoc());
            }

            Call->setIdent(Context.getDeclNameTable().getSubscriptName(
               DeclarationName::SubscriptKind::Getter));
         }

         Name = Call->getDeclName();

         if (ParentType) {
            if (QC.MultiLevelTypeLookup(Result, ParentTypeNoSugar, Name, Opts)) {
               return ExprError();
            }
         }
         else {
            if (QC.MultiLevelLookup(Result, Ctx, Name, Opts)) {
               return ExprError();
            }
         }

         break;
      }
      case Decl::AliasDeclID: {
         auto *Alias = cast<AliasDecl>(ND);
         if (Alias->getType()->isMetaType()) {
            auto UnderlyingTy = Alias->getType()->asMetaType()
               ->getUnderlyingType();

            return HandleStaticTypeCall(Call, ArgExpr, UnderlyingTy);
         }

         goto case_anonymous_call;
      }
      case Decl::AssociatedTypeDeclID: {
         auto *AT = dyn_cast<AssociatedTypeDecl>(ND);
         if (AT->isImplementation()) {
            return HandleStaticTypeCall(Call, ArgExpr, AT->getActualType());
         }

         Call->setContainsAssociatedType(true);

         QualType Ty = Context.getAssociatedType(AT);
         QualType CapableTy = checkCurrentTypeCapabilities(Ty);

         QualType Covar;
         if (Ty != CapableTy) {
            Covar = CapableTy;
         }
         else {
            Covar = AT->getCovariance();
         }

         if (Covar->isRecordType()) {
            return HandleConstructorCall(Call, ArgExpr, Covar->getRecord(),
                                         nullptr, AT);
         }
         if (Covar->isUnknownAnyType()) {
            visitContextDependentArgs(*this, Call);
            Call->setExprType(UnknownAnyTy);

            return Call;
         }

         goto case_anonymous_call;
      }
      case Decl::TemplateParamDeclID: {
         Call->setContainsGenericParam(true);

         auto *P = cast<TemplateParamDecl>(ND);
         if (!P->isVariadic() && P->isTypeName()) {
            QualType Covar = P->getCovariance();
            if (Covar->isRecordType()) {
               return HandleConstructorCall(Call, ArgExpr,
                                            Covar->getRecord(), P);
            }
         }

         goto case_anonymous_call;
      }
      case Decl::FuncArgDeclID:
      case Decl::LocalVarDeclID:
      case Decl::GlobalVarDeclID:
      case Decl::FieldDeclID:
      case Decl::PropDeclID:
      case_anonymous_call: {
         // this is semantically an anonymous call that happens to be spelled
         // like a normal function call, i.e.
         //  `let fn = () => {}; fn()` is semantically equivalent to
         //  `(fn)()`
         auto ID = new(Context) IdentifierRefExpr(Call->getSourceRange(),
                                                  Call->getDeclName(), { },
                                                  Call->getContext());

         ID->setIsPointerAccess(Call->isPointerAccess());
         ID->setParentExpr(Call->getParentExpr());

         auto Anon = AnonymousCallExpr::Create(Context, Call->getParenRange(),
                                               ID, Call->getArgs(),
                                               Call->getLabels());

         return visitExpr(Call, Anon);
      }
      case Decl::StructDeclID:
      case Decl::ClassDeclID:
      case Decl::EnumDeclID:
      case Decl::UnionDeclID:
      case Decl::ProtocolDeclID:
         return HandleConstructorCall(Call, ArgExpr, cast<RecordDecl>(ND));
      case Decl::InitDeclID:
         return HandleConstructorCall(Call, ArgExpr,
                                      Call->getDeclName().getConstructorType()
                                                         ->getRecord());
      case Decl::MethodDeclID: {
         // if we found an instance method through local lookup, build an
         // implicit self expression. In this case we do not perform UFCS
         // lookup.
         if (IsLocalLookup && !Call->includesSelf()) {
            bool Static;
            if (wouldSelfBeValid(*this, Static)) {
               Expression *Self = SelfExpr::Create(Context,
                                                   Call->getSourceLoc(),
                                                   Static);

               auto Res = visitExpr(Call, Self);
               if (!Res)
                  return ExprError();

               Call->setParentExpr(Self);
            }

            DoUFCSLookup = false;
         }

         auto *M = cast<MethodDecl>(ND);
         if (M->getDeclName().getKind() == DeclarationName::SubscriptName
             && M->getDeclName().getSubscriptKind()
                == DeclarationName::SubscriptKind::Setter) {
            IsSubscriptSetter = true;
         }

         break;
      }
      default:
         break;
      }

      // Add candidates found via normal lookup.
      if (addCandidates(*Result, CandSet)) {
         Call->setIsInvalid(true);
         return ExprError();
      }
   }

   // check if we can find additional candidates through UFCS
   if (DoUFCSLookup && !args.empty()) {
      auto First = args.front();
      if (addUFCSCandidates(*this, First, Name, CandSet)) {
         Call->setIsInvalid(true);
         return ExprError();
      }
   }

   if (CandSet.Candidates.empty()) {
      // If this expression is type dependent, new overloads might be visible
      // at instantiation time via UFCS, so don't report an error for now.
      if (CandSet.isDependent() || inUnboundedTemplate()) {
         visitContextDependentArgs(*this, Call);
         Call->setIsTypeDependent(true);
         Call->setExprType(UnknownAnyTy);

         return Call;
      }

      if (ParentType) {
         if (ParentType->isMetaType()) {
            diagnose(Call, err_method_not_found, Call->getDeclName(), false,
                     ParentType->stripMetaType(), Call->getSourceLoc());
         }
         else {
            diagnose(Call, err_method_not_found, Call->getDeclName(), true,
                     ParentType, Call->getSourceLoc());
         }
      }
      else {
         diagnose(Call, err_func_not_found, 0, Call->getDeclName(),
                  Call->getSourceLoc());
      }

      return ExprError();
   }

   Expression *SelfExpr = Call->getParentExpr();
   if (SelfExpr && refersToNamespace(SelfExpr))
      SelfExpr = nullptr;

   lookupFunction(CandSet, Call->getDeclName(), SelfExpr, args,
                  TemplateArgs, Call->getLabels(), Call);

   if (CandSet.isDependent()) {
      visitContextDependentArgs(*this, Call);
      Call->setIsTypeDependent(true);
      Call->setExprType(UnknownAnyTy);

      return Call;
   }

   if (!CandSet)
      return ExprError();

   // Get the called function.
   auto &Cand = CandSet.getBestMatch();
   auto func = Cand.Func;
   checkAccessibility(func, Call);

   // Resolve call.
   Call->setFunc(func);
   Call->setExprType(func->getReturnType());
   Call->setKind(CallKind::NamedFunctionCall);

   if (func->isInUnboundedTemplate()) {
      visitContextDependentArgs(*this, Call);

      Call->setIsTypeDependent(true);
      Call->setExprType(func->getReturnType());

      return Call;
   }

   if (func->throws()) {
      if (TryScopeStack.empty()) {
         diagnose(Call, err_throwing_call_not_in_try, func->getDeclName(),
                  Call->getSourceRange());
      }
      else {
         TryScopeStack.back() = true;
      }
   }

   Expression *ResultExpr = Call;
   if (!Call->needsInstantiation()) {
      Call->setIncludesSelf(true);
      Call->setParentExpr(nullptr);
      Call->getArgs().clear();
      Call->getArgs().append(Context, CandSet.ResolvedArgs.begin(),
                             CandSet.ResolvedArgs.end());

      if (!Cand.InnerTemplateArgs.empty() && func->isTemplate()) {
         auto *FinalList  = sema::FinalTemplateArgumentList::Create(
            Context, Cand.InnerTemplateArgs);

         if (ArgExpr) {
            ArgExpr->setTemplateArgs(FinalList);
         }
         else {
            ArgExpr = TemplateArgListExpr::Create(Context,
                                                  Call->getSourceRange(),
                                                  Call, FinalList);

            ResultExpr = ArgExpr;
         }
      }
   }

   // Check if this function call should be specialized.
//   if (!Call->isDependent() && isa<MethodDecl>(func)) {
//      auto *SelfVal = args.front();
//      if (func->getRecord()->isTemplate() || SelfVal->containsGenericParam()) {
//         Call->setSpecializationCandidate(true);
//      }
//      else if (isa<ast::SelfExpr>(SelfVal->ignoreParens())
//            && SelfVal->getExprType()->isProtocol()) {
//         Call->setSpecializationCandidate(true);
//      }
//   }

   if (IsSubscriptSetter) {
      // the actual return type will be void, this is just to allow
      // subscripts to appear on the left hand side of an assignment
      Call->setExprType(Context.getMutableReferenceType(
         func->getArgs()[2]->getType()));
   }

   if (isInBuiltinModule(func)) {
      HandleBuiltinCall(Call);
   }

   if (func->getDeclName().isStr("decltype")) {
      if (BuiltinDecls.find(func->getDeclName()) != BuiltinDecls.end()) {
         auto DT = DeclTypeExpr::Create(Context, Call->getSourceRange(),
                                        Call->getArgs().front());

         return visitExpr(Call, DT);
      }
   }

//   if (ParentType) {
//      ResultExpr = UnwrapExistential(ParentType, ResultExpr);
//   }

   return ResultExpr;
}

static void addTemplateArgs(SmallVectorImpl<Expression*> &Exprs,
                            SmallVectorImpl<std::unique_ptr<BuiltinExpr>>
                               &PlaceholderExprs,
                            const sema::TemplateArgument &Arg) {
   if (Arg.isVariadic()) {
      for (auto &VA : Arg.getVariadicArgs()) {
         addTemplateArgs(Exprs, PlaceholderExprs, VA);
      }

      return;
   }

   if (Arg.isValue()) {
      Exprs.push_back(Arg.getValueExpr());
      return;
   }

   PlaceholderExprs.emplace_back(
      std::make_unique<BuiltinExpr>(BuiltinExpr::CreateTemp(Arg.getType())));
   Exprs.push_back(PlaceholderExprs.back().get());
}

ExprResult SemaPass::HandleStaticTypeCall(CallExpr *Call,
                                          TemplateArgListExpr *ArgExpr,
                                          CanType Ty) {
   if (Ty->isVoidType()) {
      auto& args = Call->getArgs();
      if (!args.empty()) {
         diagnose(Call, err_too_many_args_for_call, 0, args.size(),
                  Call->getSourceLoc());

         return Call;
      }

      Call->setKind(CallKind::PrimitiveInitializer);
      Call->setExprType(Ty);

      return Call;
   }

   if (Ty->isRecordType()) {
      MutableArrayRef<Expression*> TemplateArgs;
      if (ArgExpr)
         TemplateArgs = ArgExpr->getExprs();

      SmallVector<Expression*, 0> RealTemplateArgs;
      SmallVector<std::unique_ptr<BuiltinExpr>, 0> PlaceholderExprs;
      if (Ty->isDependentRecordType()) {
         if (!TemplateArgs.empty()) {
            diagnose(Call, err_generic_error,
                     "template arguments already specified",
                     Call->getSourceRange());
         }

         for (auto &Arg : Ty->getTemplateArgs()) {
            addTemplateArgs(RealTemplateArgs, PlaceholderExprs, Arg);
         }

         TemplateArgs = RealTemplateArgs;
      }

      return HandleConstructorCall(Call, ArgExpr, Ty->getRecord());
   }

   if (Call->getArgs().empty() && hasDefaultValue(Ty)) {
      Call->setKind(CallKind::PrimitiveInitializer);
      Call->setExprType(Ty);

      return Call;
   }

   bool isValid = Ty->isVoidType() || Ty->isIntegerType() || Ty->isFPType();
   if (isValid) {
      Call->setExprType(Ty);

      unsigned neededArgs = Ty->isVoidType() ? 0 : 1;
      auto& args = Call->getArgs();
      if (args.size() != neededArgs) {
         auto diag = args.size() < neededArgs
                     ? err_too_few_args_for_call
                     : err_too_many_args_for_call;

         diagnose(Call, diag, neededArgs, args.size(), Call->getSourceLoc());
         return Call;
      }

      if (Ty->isIntegerType()) {
         if (auto lit = dyn_cast<IntegerLiteral>(args.front())) {
            lit->setContextualType(Ty);
            lit->setType(Ty);

            visitIntegerLiteral(lit);
         }
         else if (auto B = dyn_cast<BoolLiteral>(args.front())) {
            B->setContextualType(Ty);
            B->setType(Ty);

            visitBoolLiteral(B);
         }
         else {
            diagnose(Call, err_must_be_integral_constant, 1 /*st argument*/,
                     1 /*primitive initializer*/, args.front()->getSourceLoc());

            return Call;
         }
      }
      else if (!Ty->isVoidType()) {
         assert(Ty->isFPType());
         auto lit = dyn_cast<FPLiteral>(args.front());
         if (!lit) {
            diagnose(Call, err_must_be_floating_constant,1 /*st argument*/,
                     1 /*primitive initializer*/, args.front()->getSourceLoc());

            return Call;
         }

         lit->setContextualType(Ty);
         lit->setType(Ty);

         visitFPLiteral(lit);
      }

      Call->setKind(CallKind::PrimitiveInitializer);
      return Call;
   }

   diagnose(Call, err_cannot_call_type, Ty, true, Call->getSourceLoc());
   return ExprError();
}

ExprResult SemaPass::HandleConstructorCall(CallExpr *Call,
                                           TemplateArgListExpr *ArgExpr,
                                           RecordDecl *R,
                                           TemplateParamDecl *Param,
                                           AssociatedTypeDecl *AT) {
   MutableArrayRef<Expression*> TemplateArgs;
   if (ArgExpr)
      TemplateArgs = ArgExpr->getExprs();

   if (QC.PrepareDeclInterface(R)) {
      visitContextDependentArgs(*this, Call);
      Call->setExprType(Context.getRecordType(R));
      return Call;
   }

   if (!R->isTemplate() && !TemplateArgs.empty()) {
      if (Call->getDeclName()) {
         diagnose(Call, err_generic_type_count, 0,
                  TemplateArgs.size(),
                  Call->getDeclName(), Call->getSourceLoc());
      }
      else {
         diagnose(Call, err_generic_type_count, 0,
                  TemplateArgs.size(),
                  "(anonymous)", Call->getSourceLoc());
      }

      TemplateArgs = MutableArrayRef<Expression*>();
   }

   if (isa<ProtocolDecl>(R) && !Call->isDotInit() && !Param && !AT) {
      diagnose(Call, err_protocol_initializer, Call->getSourceRange());

      Call->setExprType(Context.getRecordType(R));
      return Call;
   }

   checkAccessibility(R, Call);

   auto& givenArgs = Call->getArgs();
   auto DeclName = Context.getDeclNameTable()
                          .getConstructorName(Context.getRecordType(R),
                                              !Call->isDotInit());

   Expression *SelfExpr = Call->getParentExpr();
   if (SelfExpr && refersToNamespace(SelfExpr)) {
      SelfExpr = nullptr;
   }

   auto CandSet = lookupFunction(R, DeclName, SelfExpr, givenArgs,
                                 TemplateArgs, Call->getLabels(), Call);

   if (CandSet.isDependent()) {
      visitContextDependentArgs(*this, Call);
      Call->setIsTypeDependent(true);
      Call->setExprType(UnknownAnyTy);

      return Call;
   }

   if (!CandSet) {
      Call->setIsInvalid(true);

      if (!R->isTemplate()) {
         Call->setReturnType(Context.getRecordType(R));
         return Call;
      }

      return ExprError();
   }

   auto &Cand = CandSet.getBestMatch();
   auto Init = cast<InitDecl>(Cand.Func);

   // Update record since it might have been instantiated.
   R = Init->getRecord();

   // Check if this initializer is accessible.
   QC.CheckAccessibility(DeclCtx, Init, Call->getSourceLoc());

   // Make sure the record we're constructing is valid.
   if (QC.PrepareDeclInterface(R)) {
      return ExprError();
   }

   if (Init->isInUnboundedTemplate()) {
      visitContextDependentArgs(*this, Call);
      Call->setIsTypeDependent(true);
      Call->setExprType(UnknownAnyTy);

      if (!R->isTemplate()) {
         Call->setReturnType(Context.getRecordType(R));
         return Call;
      }

      return Call;
   }

   if (R->isAbstract() && !Call->isDotInit()) {
      diagnose(Call, err_instantiate_abstract_class, R->getDeclName(),
               Call->getSourceRange());
   }

   if (Call->isDotInit()) {
      if (Init->isFallible()) {
         auto fn = getCurrentFun();
         if (!fn->isFallibleInit()) {
            diagnose(Call, err_fallible_init_in_non_fallible_init,
                     Call->getSourceRange());
         }
      }

      Call->setExprType(Context.getVoidType());
   }
   else if (Init->isFallibleInit()) {
      Call->setExprType(cast<InitDecl>(Init)->getOptionTy());
   }
   else if (Param) {
      Call->setExprType(Context.getTemplateArgType(Param));
   }
   else if (AT) {
      Call->setExprType(Context.getAssociatedType(AT));
   }
   else if (R->isTemplate()) {
      Call->setExprType(Context.getDependentRecordType(
         R, Cand.OuterTemplateArgs, getParentType(Call->getParentExpr())));
   }
   else {
      Call->setExprType(Context.getRecordType(R));
   }

   if (!Call->needsInstantiation()) {
      Call->setIncludesSelf(true);
      Call->setParentExpr(nullptr);
      Call->getArgs().clear();
      Call->getArgs().append(Context, CandSet.ResolvedArgs.begin(),
                             CandSet.ResolvedArgs.end());
   }

   Call->setFunc(Init);

   if (Param && !Call->needsInstantiation()) {
      Call->setParentExpr(
         new(Context) IdentifierRefExpr(Call->getIdentLoc(),
                                        IdentifierKind::TemplateParam,
                                        Param, Param->getCovariance()));

      Call->setKind(CallKind::GenericInitializerCall);
   }
   else if (AT && !Call->needsInstantiation()) {
      Call->setParentExpr(
         new(Context) IdentifierRefExpr(Call->getIdentLoc(),
                                        IdentifierKind::AssociatedType, AT,
                                        Param->getCovariance()));

      Call->setKind(CallKind::GenericInitializerCall);
   }
   else {
      Call->setKind(CallKind::InitializerCall);
   }

   if (!Cand.InnerTemplateArgs.empty() && Init->isTemplate()) {
      auto *FinalList  = sema::FinalTemplateArgumentList::Create(
         Context, Cand.InnerTemplateArgs);

      if (ArgExpr) {
         ArgExpr->setTemplateArgs(FinalList);
      }
      else {
         ArgExpr = TemplateArgListExpr::Create(Context,
                                               Call->getSourceRange(),
                                               Call, FinalList);

         return ArgExpr;
      }
   }

   return Call;
}

static ExprResult checkPrimitiveInitializer(SemaPass &SP,
                                            AnonymousCallExpr *Call) {
   if (!Call->getArgs().empty()) {
      SP.diagnose(Call, err_generic_error,
                  "primitive intializer expects no arguments",
                  Call->getSourceRange());
   }

   auto Ty = Call->getParentExpr()->getExprType()->asMetaType()
      ->getUnderlyingType();

   if (SP.hasDefaultValue(Ty)) {
      Call->setIsPrimitiveInit(true);
      Call->setExprType(Ty);
      return Call;
   }

   SP.diagnose(Call, err_cannot_call_type, Call->getSourceLoc(), Ty, false);
   return ExprError();
}

ExprResult SemaPass::visitAnonymousCallExpr(AnonymousCallExpr *Call)
{
   auto args = Call->getArgs();
   for (auto &arg : args) {
      if (!arg->isContextDependent()) {
         auto result = visitExpr(Call, arg);
         if (!result)
            return ExprError();

         arg = result.get();
      }
   }

   Expression *ParentExpr = Call->getParentExpr();

   auto ParentResult = getRValue(Call, ParentExpr);
   if (!ParentResult)
      return ExprError();

   ParentExpr = ParentResult.get();
   Call->setParentExpr(ParentExpr);

   auto ty = checkCurrentTypeCapabilities(ParentExpr->getExprType());
   if (ty->isDependentType()) {
      visitContextDependentArgs(*this, Call);
      Call->setIsTypeDependent(true);
      Call->setExprType(UnknownAnyTy);

      return Call;
   }

   FunctionType *func = ty->asFunctionType();
   if (!func) {
      if (ty->isRecordType()) {
         // call operator
         auto &II = Context.getIdentifiers().get("()");
         auto DN = Context.getDeclNameTable().getPostfixOperatorName(II);

         ASTVector<Expression*> Args(Context, Call->getArgs());
         auto NamedCall = CallExpr::Create(Context, Call->getSourceLoc(),
                                           Call->getParenRange(),
                                           move(Args), {}, DN);

         NamedCall->setParentExpr(Call->getParentExpr());
         return visitExpr(Call, NamedCall);
      }

      if (auto Meta = ty->asMetaType()) {
         if (auto MetaRecTy = Meta->getUnderlyingType()->asRecordType()) {
            // constructor call
            auto DN = Context.getDeclNameTable().getConstructorName(MetaRecTy);
            ASTVector<Expression*> Args(Context, Call->getArgs());
            auto NamedCall = CallExpr::Create(Context, Call->getSourceLoc(),
                                              Call->getParenRange(),
                                              move(Args), Call->getLabels(),
                                              DN);

            NamedCall->setParentExpr(Call->getParentExpr());
            return visitExpr(Call, NamedCall);
         }

         return checkPrimitiveInitializer(*this, Call);
      }

      diagnose(Call, err_cannot_call_type, Call->getSourceLoc(), ty, false);
      return ExprError();
   }

   auto CandSet = checkAnonymousCall(func, Call->getArgs(), Call->getLabels(),
                                     Call);

   if (CandSet.isDependent()) {
      visitContextDependentArgs(*this, Call);
      Call->setIsTypeDependent(true);
      Call->setExprType(UnknownAnyTy);

      return Call;
   }

   if (!CandSet)
      return ExprError();

   std::copy(CandSet.ResolvedArgs.begin(),
             CandSet.ResolvedArgs.end(), Call->getArgs().begin());

   Call->setFunctionType(func);
   Call->setExprType(func->getReturnType());

   return Call;
}

namespace {

enum : int {
   No = -1, Yes = 0, YesWithPenalty = 1,
};
   
} // anonymous namespace

int SemaPass::ExprCanReturnImpl(Expression *E, CanType Ty)
{
   switch (E->getTypeID()) {
   case Expression::IntegerLiteralID: {
      auto IL = cast<IntegerLiteral>(E);

      // an expression with an explicit suffix can only return that type
      switch (IL->getSuffix()) {
      case IntegerLiteral::Suffix::i1: case IntegerLiteral::Suffix::u1:
      case IntegerLiteral::Suffix::i8: case IntegerLiteral::Suffix::u8:
      case IntegerLiteral::Suffix::i16: case IntegerLiteral::Suffix::u16:
      case IntegerLiteral::Suffix::i32: case IntegerLiteral::Suffix::u32:
      case IntegerLiteral::Suffix::i64: case IntegerLiteral::Suffix::u64:
      case IntegerLiteral::Suffix::i128: case IntegerLiteral::Suffix::u128:
         return Ty == IL->getType() ? Yes : No;
      case IntegerLiteral::Suffix::u:
         if (*Ty == Context.getUIntTy())
            return Yes;

         return (Ty->isIntegerType() && Ty->isUnsigned()) ? YesWithPenalty : No;
      case IntegerLiteral::Suffix::i:
         if (*Ty == Context.getIntTy())
            return Yes;

         return (Ty->isIntegerType() && !Ty->isUnsigned()) ? YesWithPenalty
                                                           : No;
      case IntegerLiteral::Suffix::None:
         break;
      }

      if (Ty->isRecordType()) {
         auto R = Ty->getRecord();
         auto *ExpressibleByInt = getInitializableByDecl(
            InitializableByKind::Integer);

         return Context.getConformanceTable().conformsTo(R, ExpressibleByInt)
                ? YesWithPenalty : No;
      }

      if (*Ty == Context.getIntTy())
         return Yes;

      return Ty->isIntegerType() ? YesWithPenalty : No;
   }
   case Expression::CharLiteralID: {
      if (Ty->isRecordType()) {
         auto R = Ty->getRecord();
         auto *ExpressibleBy = getInitializableByDecl(InitializableByKind::Char);
         return Context.getConformanceTable().conformsTo(R, ExpressibleBy)
                ? YesWithPenalty
                : No;
      }

      return (Ty->isIntegerType() && Ty->getBitwidth() == 8) ? Yes : No;
   }
   case Expression::BoolLiteralID: {
      if (Ty->isRecordType()) {
         auto R = Ty->getRecord();
         auto *ExpressibleBy = getInitializableByDecl(InitializableByKind::Bool);
         return Context.getConformanceTable().conformsTo(R, ExpressibleBy)
                ? YesWithPenalty
                : No;
      }

      return (Ty->isIntegerType() && Ty->getBitwidth() == 1) ? Yes : No;
   }
   case Expression::FPLiteralID: {
      auto FP = cast<FPLiteral>(E);

      if (Ty->isRecordType()) {
         auto R = Ty->getRecord();
         auto *ExpressibleBy = getInitializableByDecl(InitializableByKind::Float);
         return Context.getConformanceTable().conformsTo(R, ExpressibleBy)
                ? YesWithPenalty
                : No;
      }

      switch (FP->getSuffix()) {
      case FPLiteral::Suffix::f:
      case FPLiteral::Suffix::d:
         return (Ty == FP->getType()) ? Yes : No;
      case FPLiteral::Suffix::None:
         break;
      }

      if (Ty->isDoubleTy())
         return Yes;

      return Ty->isFPType() ? YesWithPenalty : No;
   }
   case Expression::StringLiteralID: {
      if (Ty->isPointerType() && Ty->getPointeeType()->isInt8Ty()) {
         return YesWithPenalty;
      }

      if (Ty->isArrayType() && Ty->asArrayType()->getElementType()->isInt8Ty()){
         return YesWithPenalty;
      }

      if (Ty->isRecordType() && (Ty->getRecord() == getStringDecl()))
         return Yes;

      if (Ty->isRecordType()) {
         auto R = Ty->getRecord();
         auto *ExpressibleBy = getInitializableByDecl(
            InitializableByKind::String);

         return Context.getConformanceTable().conformsTo(R, ExpressibleBy)
                ? YesWithPenalty : No;
      }

      return No;
   }
   case Expression::IdentifierRefExprID:
   case Expression::CallExprID: {
      if (!Ty->isRecordType())
         return No;

      DeclarationName Name;
      if (auto *Call = dyn_cast<CallExpr>(E)) {
         Name = Call->getDeclName();
      }
      else {
         Name = cast<IdentifierRefExpr>(E)->getDeclName();
      }

      auto *R = Ty->getRecord();
      auto Result = Lookup(*R, Name);

      return Result ? Yes : No;
   }
   case Expression::EnumCaseExprID: {
      if (!Ty->isRecordType())
         return No;

      auto Enum = dyn_cast<EnumDecl>(Ty->getRecord());
      if (!Enum)
         return No;

      return Enum->hasCase(cast<EnumCaseExpr>(E)->getDeclName()) != nullptr
             ? Yes : No;
   }
   case Expression::LambdaExprID: {
      if (!Ty->isLambdaType())
         return No;

      auto LE = cast<LambdaExpr>(E);
      auto Func = Ty->asLambdaType();
      auto NeededParams = Func->getParamTypes();

      if (NeededParams.size() < LE->getNumArgs())
         return No;

      // check only the types of explicitly specified arguments
      unsigned i = 0;
      for (auto &Arg : LE->getArgs()) {
         auto NeededTy = NeededParams[i];
         auto GivenTypeRes = visitSourceType(Arg, Arg->getType());

         if (!GivenTypeRes) {
            LE->setIsInvalid(true);
            return No;
         }

         if (GivenTypeRes.getValue()->isAutoType())
            continue;

         if (GivenTypeRes.getValue() != NeededTy)
            return No;
      }

      auto NeededRetRes = visitSourceType(LE, LE->getReturnType());
      if (!NeededRetRes)
         return No;

      if (NeededRetRes.getValue()->isAutoType())
         return No;

      return NeededRetRes.getValue() == Func->getReturnType() ? Yes : No;
   }
   case Expression::BuiltinIdentExprID: {
      auto *BE = cast<BuiltinIdentExpr>(E);
      switch (BE->getIdentifierKind()) {
      case BuiltinIdentifier ::NULLPTR:
         return Ty->isPointerType();
      default:
         return false;
      }
   }
   default:
      llvm_unreachable("bad context dependent expression kind!");
   }
}

int SemaPass::ExprCanReturn(Expression *E, CanType Ty)
{
   E = E->ignoreParens();
   Ty = Ty->getDesugaredType();

   if (Ty->isUnknownAnyType()) {
      return Yes;
   }

   // Look for an implicit initializer.
   if (Ty->isRecordType()) {
      auto *R = Ty->getRecord();

      // Load all external declarations.
      auto DN = Context.getDeclNameTable().getConstructorName(Ty);

      const MultiLevelLookupResult *Initializers;
      if (QC.MultiLevelLookup(Initializers, R, DN)) {
         return Yes;
      }

      for (auto *D : Initializers->allDecls()) {
         auto *I = cast<InitDecl>(D);
         if (!I->hasAttribute<ImplicitAttr>())
            continue;

         assert(I->getArgs().size() == 1 && "invalid implicit attribute");

         auto Res = ExprCanReturnImpl(E, I->getArgs().front()->getType());
         if (Res != No) {
            return Res;
         }
      }
   }

   assert(E->isContextDependent() && "not a context dependent expression!");
   return ExprCanReturnImpl(E, Ty);
}

static bool ArgTypesAreCompatible(QualType Given, QualType Needed)
{
   if (Given == Needed || Needed->isUnknownAnyType())
      return true;

   if (!isa<GenericType>(Needed))
      return false;

   return ArgTypesAreCompatible(Given,
                                cast<GenericType>(Needed)->getCovariance());
}

QualType SemaPass::ResolveContextualLambdaExpr(LambdaExpr *LE, QualType Ty)
{
   if (!Ty->isLambdaType())
      return QualType();

   auto Func = Ty->asLambdaType();
   auto NeededParams = Func->getParamTypes();

   if (NeededParams.size() < LE->getNumArgs())
      return QualType();

   bool AllArgsResolved = true;
   SmallVector<QualType, 4> ArgTys;

   // check only the types of explicitly specified arguments
   unsigned i = 0;
   for (auto &Arg : LE->getArgs()) {
      auto NeededTy = NeededParams[i++];
      auto &ArgTy = Arg->getType();

      auto GivenTypeRes = visitSourceType(Arg, ArgTy);
      LE->copyStatusFlags(Arg);

      if (!GivenTypeRes) {
         LE->setIsInvalid(true);
         return QualType();
      }

      if (ArgTy->isAutoType()) {
         if (NeededTy->containsGenericType()) {
            AllArgsResolved = false;
         }
         else {
            ArgTy = NeededTy;
         }

         ArgTys.push_back(NeededTy);
         continue;
      }

      if (!ArgTypesAreCompatible(ArgTy, NeededTy))
         return QualType();

      ArgTys.push_back(ArgTy);
   }

   auto &RetTy = LE->getReturnType();

   auto NeededRetRes = visitSourceType(LE, RetTy);
   if (!NeededRetRes)
      return QualType();

   if (!RetTy->isAutoType()) {
      if (RetTy != Func->getReturnType())
         return QualType();
   }

   if (!AllArgsResolved) {
      if (!getCurrentDecl()->inDependentContext())
         return QualType();

      // return a new function type with all types that we could resolve to
      // aid inference
      return Context.getLambdaType(RetTy->isAutoType()
                                      ? Func->getReturnType()
                                      : RetTy.getResolvedType(),
                                   ArgTys);
   }

   // evaluate the lambda expression to help inference via the return type
   auto Res = visitLambdaExpr(LE);
   if (!Res)
      return QualType();

   return LE->getExprType();
}

QualType SemaPass::GetDefaultExprType(Expression *E)
{
   E = E->ignoreParens();

   assert(E->isContextDependent() && "not a context dependent expression!");
   switch (E->getTypeID()) {
   case Expression::IntegerLiteralID:
      return Context.getIntTy();
   case Expression::CharLiteralID:
      return Context.getUInt8Ty();
   case Expression::BoolLiteralID:
      return Context.getBoolTy();
   case Expression::FPLiteralID:
      return Context.getDoubleTy();
   case Expression::StringLiteralID: {
      // FIXME
//      auto S = getStringDecl();
//      if (S)
//         return Context.getRecordType(S);

      return Context.getUInt8PtrTy();
   }
   case Expression::LambdaExprID: {
      auto LE = cast<LambdaExpr>(E);

      std::vector<QualType> ParamTys;
      ParamTys.reserve(LE->getArgs().size());

      // check only the types of explicitly specified arguments
      for (auto &Arg : LE->getArgs()) {
         auto GivenTypeRes = visitSourceType(Arg, Arg->getType());
         if (!GivenTypeRes) {
            LE->setIsInvalid(true);
            return QualType();
         }

         // can't determine the type of this expression without context
         if (GivenTypeRes.getValue()->isAutoType())
            return QualType();

         ParamTys.push_back(GivenTypeRes.getValue());
      }

      auto RetResult = visitSourceType(LE, LE->getReturnType());
      if (!RetResult)
         return QualType();

      QualType RetTy;
      if (RetResult.getValue()->isAutoType())
         RetTy = Context.getVoidType();
      else
         RetTy = RetResult.getValue();

      return Context.getLambdaType(RetTy, ParamTys);
   }
   case Expression::IdentifierRefExprID:
   case Expression::CallExprID:
   case Expression::BuiltinIdentExprID: {
      return QualType();
   }
   default:
      llvm_unreachable("bad context dependent expression kind!");
   }
}

ExprResult SemaPass::visitTypeDependentContextualExpr(Expression *E)
{
   E = E->ignoreParens();

   if (!E->isContextDependent()) {
      return visitExpr(E);
   }

   E->setIsTypeDependent(true);
   switch (E->getTypeID()) {
   case Expression::IntegerLiteralID:
   case Expression::CharLiteralID:
   case Expression::BoolLiteralID:
   case Expression::FPLiteralID:
   case Expression::StringLiteralID:
   case Expression::BuiltinIdentExprID:
   case Expression::IdentifierRefExprID:
      // These expressions do not have any substatements that need to be
      // checked.
      return ExprError();
   case Expression::LambdaExprID: {
      auto *LE = cast<LambdaExpr>(E);

      // Check return type.
      (void) visitSourceType(LE, LE->getReturnType());

      // Check argument types.
      for (auto *Arg : LE->getArgs()) {
         (void) declareStmt(LE, Arg);
      }

      // Check closure body.
      DeclScopeRAII DSR(*this, LE->getFunc());
      auto Result = visitStmt(LE, LE->getBody());
      if (Result) {
         LE->setBody(Result.get());
      }

      break;
   }
   case Expression::CallExprID: {
      auto *Call = cast<CallExpr>(E);

      // Check arguments.
      for (auto &Arg : Call->getArgs()) {
         auto Result = visitTypeDependentContextualExpr(Arg);
         if (Result) {
            Arg = Result.get();
         }
      }

      break;
   }
   default:
      llvm_unreachable("bad context dependent expression kind!");
   }

   return E;
}

} // namespace ast
} // namespace cdot