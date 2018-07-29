//
// Created by Jonas Zell on 24.10.17.
//

#include "SemaPass.h"

#include "AST/Transform.h"
#include "ILGen/ILGenPass.h"
#include "Message/Diagnostics.h"
#include "Sema/OverloadResolver.h"
#include "Sema/Builtin.h"
#include "Serialization/ModuleFile.h"
#include "Support/StringSwitch.h"
#include "TemplateInstantiator.h"

#include <llvm/ADT/SmallString.h>

using namespace cdot::diag;
using namespace cdot::support;

namespace cdot {
namespace ast {

static void addCandidates(MultiLevelLookupResult &lookupRes,
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
}

static void addCandidates(SemaPass &Sema,
                          DeclContext *Ctx,
                          DeclarationName name,
                          CandidateSet &CandSet,
                          bool IsLocalLookup) {
   auto lookupRes = Sema.MultiLevelLookup(*Ctx, name, true, IsLocalLookup);
   addCandidates(lookupRes, CandSet);
}

static std::pair<DeclContext*, bool> getAsContext(SemaPass &SP, QualType Ty)
{
   if (auto RecTy = Ty->asRecordType()) {
      return { RecTy->getRecord(), false };
   }

   if (auto Meta = Ty->asMetaType()) {
      if (auto MetaRecTy = Meta->getUnderlyingType()->asRecordType()) {
         return { MetaRecTy->getRecord(), true };
      }
   }

   return { SP.getDeclContext().getDeclModule(), false };
}

static void addUFCSCandidates(SemaPass &Sema,
                              DeclContext *Ctx,
                              DeclarationName name,
                              CandidateSet &CandSet) {
   auto lookupRes = Sema.MultiLevelLookup(*Ctx, name);
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

            auto *Cand = CandSet.addCandidate(CD, Distance);
            if (Cand && isa<MethodDecl>(D))
               Cand->UFCS = CandidateSet::MethodCalledAsFunction;
         }
      }

      ++Distance;
   }
}

static void addUFCSCandidates(SemaPass &SP,
                              Expression *FirstArg,
                              DeclarationName name,
                              CandidateSet &CandSet) {
   QualType FirstArgTy;
   if (FirstArg->isContextDependent()) {
      FirstArgTy = SP.GetDefaultExprType(FirstArg);
   }
   else {
      FirstArgTy = FirstArg->getExprType()->stripReference();
   }

   if (!FirstArgTy)
      return;

   auto ContextAndIsStatic = getAsContext(SP, FirstArgTy);
   if (ContextAndIsStatic.first) {
      addUFCSCandidates(SP, ContextAndIsStatic.first, name, CandSet);
   }
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
   addCandidates(*this, &getDeclContext(), name, CandSet, true);
   addUFCSCandidates(*this, SelfExpr, name, CandSet);

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
   if (cast<NamedDecl>(SP.getDeclContext()).isTemplateOrInTemplate()) {
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
   OverloadResolver Resolver(*this, SelfArg, args, templateArgs, labels, Expr);
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

static MethodDecl *getEquivalentMethod(SemaPass &SP,
                                       ASTContext &Ctx,
                                       MethodDecl *Orig,
                                       RecordDecl *Inst) {
   if (Orig->isMemberwiseInitializer())
      return cast<StructDecl>(Inst)->getMemberwiseInitializer();

   if (isa<DeinitDecl>(Orig))
      return Inst->getDeinitializer();

   DeclarationName Name = Orig->getDeclName();
   switch (Name.getKind()) {
   case DeclarationName::ConstructorName:
   case DeclarationName::BaseConstructorName:
      Name = Ctx.getDeclNameTable().getConstructorName(
         Ctx.getRecordType(Inst),
         Name.getKind() == DeclarationName::ConstructorName);

      break;
   case DeclarationName::DestructorName:
      Name = Ctx.getDeclNameTable().getDestructorName(Ctx.getRecordType(Inst));
      break;
   default:
      break;
   }

   auto Result = SP.MultiLevelLookup(*Inst, Name);
   for (auto *D : Result.allDecls()) {
      auto *M = dyn_cast<MethodDecl>(D);
      if (!M)
         continue;

      if (M->getBodyTemplate() == Orig)
         return M;
   }

   llvm_unreachable("no equivalent method!");
}

bool SemaPass::maybeInstantiateRecord(CandidateSet::Candidate &Cand,
                                      const TemplateArgList &templateArgs,
                                      Statement *Caller) {
   auto F = Cand.Func;

   if (auto Case = dyn_cast<EnumCaseDecl>(F)) {
      if (!Case->getRecord()->isTemplate())
         return true;

      auto Inst =
         Instantiator.InstantiateRecord(Caller, Case->getRecord(),
                                        templateArgs);

      if (!Inst)
         return false;

      Cand.Func = LookupSingle<EnumCaseDecl>(*cast<EnumDecl>(Inst.getValue()),
                                             Case->getIdentifierInfo());

      assert(Cand.Func && "did not instantiate enum correctly");
      return true;
   }

   auto Init = dyn_cast<InitDecl>(F);
   if (!Init || !Init->getRecord()->isTemplate())
      return true;

   auto Inst = Instantiator.InstantiateRecord(Caller, Init->getRecord(),
                                              templateArgs);

   if (Inst.hasValue()) {
      Cand.Func = getEquivalentMethod(*this, Context, Init, Inst.getValue());
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
         || Cand.isBuiltinCandidate())
      return;

   auto F = Cand.Func;
   checkIfCalledFromTemplate(*this, F);

   // this function was deserialized from a module, so we need at least it's
   // IL definition
   if (auto Info = F->getLazyFnInfo()) {
      // if the function is a template, we also need it's body
      if (F->isTemplate())
         Info->loadBody(F);
   }

   if (!F->isTemplate()) {
      if (auto M = dyn_cast<MethodDecl>(F)) {
         Cand.Func = maybeInstantiateMemberFunction(M, Caller);
      }

      return;
   }

   auto &TAs = Cand.InnerTemplateArgs;
   if (auto Fn = dyn_cast<FunctionDecl>(Cand.Func)) {
      auto Inst = Instantiator.InstantiateFunction(Caller, Fn,
                                                   move(TAs));

      if (Inst)
         Cand.Func = Inst.getValue();
   }
   else {
      auto Inst = Instantiator.InstantiateMethod(Caller,
                                                 cast<MethodDecl>(Cand.Func),
                                                 move(TAs));

      if (Inst)
         Cand.Func = Inst.getValue();
   }
}

MethodDecl *SemaPass::maybeInstantiateMemberFunction(MethodDecl *M,
                                                     StmtOrDecl Caller) {
   if (M->getBody() || !M->getBodyTemplate())
      return M;

   if (M->getBodyTemplate()->isInvalid())
      return M;

   if (auto FnInfo = M->getBodyTemplate()->getLazyFnInfo())
      FnInfo->loadBody(M->getBodyTemplate());

   Instantiator.InstantiateMethodBody(Caller, M);

   if (auto Init = dyn_cast<InitDecl>(M)) {
      if (Init->isCompleteInitializer()) {
         Init->getBaseInit()->setBody(Init->getBody());
      }
   }

   return M;
}

MethodDecl *SemaPass::InstantiateMethod(RecordDecl *R, StringRef Name,
                                         StmtOrDecl SOD) {
   auto *II = &Context.getIdentifiers().get(Name);
   auto *M = LookupSingle<MethodDecl>(*R, II);

   assert(M && "method does not exist!");
   maybeInstantiateMemberFunction(M, SOD);

   if (inCTFE())
      ILGen->prepareFunctionForCtfe(M);

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
         ILGen->prepareFunctionForCtfe(Getter);

      return Getter;
   }

   auto SetterName = Context.getDeclNameTable()
                            .getAccessorName(*II,
                                         DeclarationName::AccessorKind::Setter);

   auto *Setter = LookupSingle<MethodDecl>(*R, SetterName);
   assert(Setter && "method does not exist!");

   maybeInstantiateMemberFunction(Setter, SOD);

   if (inCTFE())
      ILGen->prepareFunctionForCtfe(Setter);

   return Setter;
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

   // check if the call's parent expr refers to a namespace
   if (!checkNamespaceRef(Call))
      return ExprError();

   // expression was previously resolved (for example by a binary operator node)
   if (Call->getKind() != CallKind::Unknown)
      return Call;

   // check if '.init' is called without a parent expression
   if (!Call->getParentExpr() && Call->isDotInit()) {
      diagnose(Call, err_dot_init_must_be_on_self, Call->getSourceRange());
      Call->setExprType(Context.getVoidType());

      return Call;
   }

   bool IsLocalLookup = true;

   DeclContext *Ctx = Call->getContext();
   if (Ctx)
      IsLocalLookup = false;

   if (auto ParentExpr = Call->getParentExpr()) {
      auto ParentResult = visitExpr(Call, ParentExpr);
      if (!ParentResult)
         return ExprError();

      ParentExpr = ParentResult.get();
      Call->setParentExpr(ParentExpr);

      auto ty = ParentExpr->getExprType()->stripReference();
      if (ty->isDependentType()) {
         visitContextDependentArgs(*this, Call);
         Call->setIsTypeDependent(true);
         Call->setExprType(UnknownAnyTy);

         return Call;
      }

      // Handle '->'
      if (ty->isPointerType() && Call->isPointerAccess()) {
         QualType RefTy;
         if (ty->isMutablePointerType()) {
            RefTy = Context.getMutableReferenceType(ty->getPointeeType());
         }
         else {
            RefTy = Context.getReferenceType(ty->getPointeeType());
         }

         FunctionType *DerefFnTy = Context.getFunctionType(RefTy, { ty });
         auto Deref = UnaryOperator::Create(Context, Call->getSourceLoc(),
                                            op::Deref, DerefFnTy,
                                            castToRValue(ParentExpr),
                                            true);

         auto Res = visitExpr(Call, Deref);
         assert(Res && "invalid deref operator?");

         Call->setParentExpr(Res.get());
         Call->setIsPointerAccess(false);

         ParentExpr = Call->getParentExpr();
         ty = ParentExpr->getExprType()->stripReference();
      }
      else if (Call->isPointerAccess()) {
         diagnose(Call, err_member_access_non_pointer, ty,
                  Call->getSourceLoc());
      }

      if (Call->isDotInit()) {
         // '.init' must either be called on self, in which case it has to be
         // in an initializer, or statically on a type
         if (auto Meta = ty->asMetaType()) {
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
                                  .getConstructorName(ty));
         }
      }

      auto ContextAndIsStatic = getAsContext(*this, ty);
      IsLocalLookup = false;
      Ctx = ContextAndIsStatic.first;

      Call->setDirectCall(isa<SuperExpr>(ParentExpr)
                            || ContextAndIsStatic.second);
   }

   if (!Ctx)
      Ctx = &getDeclContext();

   Call->setContext(Ctx);

   // The set of function candidates.
   CandidateSet CandSet;

   // We need to track if the called function is a subscript setter to adjust
   // the return type later.
   bool IsSubscriptSetter = false;

   // We only do UFCS lookup if no explicit context is provided.
   bool DoUFCSLookup = IsLocalLookup;

   // Do a normal lookup first.
   auto Result = MultiLevelLookup(*Ctx, Call->getDeclName(),
                                  true, IsLocalLookup);

   if (!Result.empty()) {
      // check if we found something that is not a function first, no need to do
      // overload resolution or check UFCS here
      auto ND = Result.front().front();
      switch (ND->getKind()) {
      case Decl::EnumCaseDeclID: {
         auto *Case = LookupSingle<EnumCaseDecl>(*cast<EnumDecl>(Ctx),
                                                 Call->getDeclName());

         auto enumCaseExpr =
            new(getContext()) EnumCaseExpr(Call->getSourceLoc(),
                                           Case, move(Call->getArgs()));

         enumCaseExpr->setContextualType(Call->getContextualType());
         return visitExpr(Call, enumCaseExpr);
      }
      case Decl::SubscriptDeclID: {
         bool FoundGetter = false;
         bool FoundSetter = false;

         for (auto &Decls : Result) {
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

         Result = MultiLevelLookup(*Ctx, Call->getDeclName(),
                                   true, IsLocalLookup);

         break;
      }
      case Decl::FuncArgDeclID:
      case Decl::LocalVarDeclID:
      case Decl::GlobalVarDeclID:
      case Decl::FieldDeclID:
      case Decl::PropDeclID:
      case Decl::AliasDeclID:
      case Decl::AssociatedTypeDeclID:
      case Decl::TemplateParamDeclID: {
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
         return HandleConstructorCall(Call, TemplateArgs, cast<RecordDecl>(ND));
      case Decl::InitDeclID:
         return HandleConstructorCall(Call, TemplateArgs, ND->getRecord());
      case Decl::MethodDeclID:
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

         break;
      default:
         break;
      }

      // Add candidates found via normal lookup.
      addCandidates(Result, CandSet);
   }

   // check if we can find additional candidates through UFCS
   if (DoUFCSLookup && !args.empty()) {
      auto First = args.front();
      addUFCSCandidates(*this, First, Call->getDeclName(), CandSet);
   }

   if (CandSet.Candidates.empty()) {
      diagnose(Call, err_func_not_found, 0, Call->getDeclName(),
               Call->getSourceLoc());

      return ExprError();
   }

   lookupFunction(CandSet, Call->getDeclName(), Call->getParentExpr(), args,
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

   if (func->isTemplateOrInTemplate()) {
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

   Call->setIncludesSelf(true);
   Call->setParentExpr(nullptr);
   Call->getArgs().clear();
   Call->getArgs().append(Context, CandSet.ResolvedArgs.begin(),
                          CandSet.ResolvedArgs.end());

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

   return Call;
}

ExprResult SemaPass::HandleStaticTypeCall(CallExpr *Call,
                                          MutableArrayRef<Expression*> TemplateArgs,
                                          Type *Ty) {
   if (Ty->isVoidType()) {
      auto& args = Call->getArgs();
      if (!args.empty()) {
         diagnose(Call, err_too_many_args_for_call, 0, args.size(),
                  Call->getSourceLoc());

         return Call;
      }

      Call->setKind(CallKind::PrimitiveInitializer);
      return Call;
   }

   if (Ty->isRecordType()) {
      return HandleConstructorCall(Call, TemplateArgs, Ty->getRecord());
   }

   if (Call->getArgs().empty() && hasDefaultValue(Ty)) {
      Call->setKind(CallKind::PrimitiveInitializer);
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
                                           MutableArrayRef<Expression*> TemplateArgs,
                                           RecordDecl *R) {
   ensureDeclared(R);

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

   if (isa<ProtocolDecl>(R)) {
      diagnose(Call, err_protocol_initializer);

      Call->setExprType(Context.getRecordType(R));
      return Call;
   }

   checkAccessibility(R, Call);

   auto& givenArgs = Call->getArgs();
   auto DeclName = Context.getDeclNameTable()
                          .getConstructorName(Context.getRecordType(R),
                                              !Call->isDotInit());

   auto CandSet = lookupFunction(R, DeclName, Call->getParentExpr(), givenArgs,
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

   auto method = cast<InitDecl>(CandSet.getBestMatch().Func);
   checkAccessibility(method, Call);

   if (method->isTemplateOrInTemplate()) {
      visitContextDependentArgs(*this, Call);
      Call->setIsTypeDependent(true);
      Call->setExprType(UnknownAnyTy);

      if (!R->isTemplate()) {
         Call->setReturnType(Context.getRecordType(R));
         return Call;
      }

      return Call;
   }

   R = method->getRecord();
   if (R->isAbstract() && !Call->isDotInit())
         diagnose(Call, err_instantiate_abstract_class, R->getDeclName(),
                  Call->getSourceRange());

   if (Call->isDotInit()) {
      if (method->isFallible()) {
         auto fn = getCurrentFun();
         if (!fn->isFallibleInit()) {
            diagnose(Call, err_fallible_init_in_non_fallible_init,
                     Call->getSourceRange());
         }
      }

      Call->setExprType(Context.getVoidType());
   }
   else if (method->isFallibleInit()) {
      Call->setExprType(cast<InitDecl>(method)->getOptionTy());
   }
   else {
      Call->setExprType(Context.getRecordType(R));
   }

   Call->setIncludesSelf(true);
   Call->setParentExpr(nullptr);
   Call->getArgs().clear();
   Call->getArgs().append(Context, CandSet.ResolvedArgs.begin(),
                          CandSet.ResolvedArgs.end());

   Call->setKind(CallKind::InitializerCall);
   Call->setFunc(method);

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

   auto ty = ParentExpr->getExprType();
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
         auto &II = Context.getIdentifiers().get("[]");
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

int SemaPass::ExprCanReturn(Expression *E, QualType Ty)
{
   E = E->ignoreParens();
   Ty = Ty->getCanonicalType();
   
   enum : int {
      No = -1, Yes = 0, YesWithPenalty = 1,
   };

   if (Ty->isUnknownAnyType())
      return Yes;

   assert(E->isContextDependent() && "not a context dependent expression!");
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
      auto NeededTy = NeededParams[i];
      auto &ArgTy = Arg->getType();

      auto GivenTypeRes = visitSourceType(Arg, ArgTy);
      LE->copyStatusFlags(Arg);

      if (!GivenTypeRes) {
         LE->setIsInvalid(true);
         return QualType();
      }

      if (ArgTy->isAutoType()) {
         if (NeededTy->isDependentType()) {
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
      auto S = getStringDecl();
      if (S)
         return Context.getRecordType(S);

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
   case Expression::EnumCaseExprID:
   case Expression::BuiltinIdentExprID: {
      return QualType();
   }
   default:
      llvm_unreachable("bad context dependent expression kind!");
   }
}

void SemaPass::visitTypeDependentContextualExpr(Expression *E)
{
   E = E->ignoreParens();

   if (!E->isContextDependent())
      return;

   switch (E->getTypeID()) {
   case Expression::IntegerLiteralID:
   case Expression::CharLiteralID:
   case Expression::BoolLiteralID:
   case Expression::FPLiteralID:
   case Expression::StringLiteralID:
   case Expression::BuiltinIdentExprID:
      // These expressions do not have any substatements that need to be
      // checked.
      return;
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
      (void) visitStmt(LE, LE->getBody());

      break;
   }
   case Expression::EnumCaseExprID: {
      auto *EC = cast<EnumCaseExpr>(E);

      // Check arguments.
      for (auto *Arg : EC->getArgs()) {
         visitTypeDependentContextualExpr(Arg);
      }

      break;
   }
   default:
      llvm_unreachable("bad context dependent expression kind!");
   }
}

} // namespace ast
} // namespace cdot