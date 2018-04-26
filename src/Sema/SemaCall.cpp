//
// Created by Jonas Zell on 24.10.17.
//

#include "SemaPass.h"

#include "AST/Transform.h"
#include "AST/Expression.h"
#include "ILGen/ILGenPass.h"
#include "Sema/OverloadResolver.h"
#include "Sema/Builtin.h"
#include "TemplateInstantiator.h"

#include "Util.h"

#include "AST/Type.h"
#include "Message/Diagnostics.h"

#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/StringSwitch.h>

using namespace cdot::diag;
using namespace cdot::support;

namespace cdot {
namespace ast {

template<class T = CallableDecl>
static void addCandidates(DeclContext *Ctx,
                          DeclarationName name,
                          CandidateSet &CandSet,
                          llvm::SmallPtrSetImpl<CallableDecl*> &Overloads,
                          bool IsLocalLookup) {
   auto lookupRes = Ctx->lookup(name);

   // add candidates found via normal lookup
   for (auto &D : lookupRes) {
      if (auto CD = dyn_cast<CallableDecl>(D)) {
         if (auto M = dyn_cast<MethodDecl>(D)) {
            // ignore methods found through normal lookup
            if (IsLocalLookup)
               continue;

            // only consider the 'deepest' override
            if (M->isOverride())
               Overloads.insert(M->getOverridenMethod());
         }

         if (!Overloads.insert(CD).second)
            continue;

         auto &Cand = CandSet.addCandidate(CD);

         // check if this is a function found through member lookup
         if (!IsLocalLookup && isa<FunctionDecl>(CD)) {
            Cand.UFCS = CandidateSet::FunctionCalledAsMethod;
         }
      }
   }

   if (auto Parent = Ctx->getParentCtx())
      addCandidates(Parent, name, CandSet, Overloads, IsLocalLookup);
}

std::pair<DeclContext*, bool> getAsContext(QualType Ty)
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

template<class T = CallableDecl>
static void addUFCSCandidates(DeclContext *Ctx,
                              DeclarationName name,
                              CandidateSet &CandSet,
                              llvm::SmallPtrSetImpl<CallableDecl*> &Overloads) {
   auto lookupRes = Ctx->lookup(name);

   for (auto &D : lookupRes) {
      if (auto CD = dyn_cast<CallableDecl>(D)) {
         auto M = dyn_cast<MethodDecl>(D);
         if (M) {
            // only consider the 'deepest' override
            if (M->isOverride())
               Overloads.insert(M->getOverridenMethod());
         }

         if (Overloads.insert(CD).second) {
            auto &Cand = CandSet.addCandidate(CD);

            // check if this is a method called as a function, it doesn't
            // matter if the method is static or not as both
            //  `instance.instanceMethod()` and
            //  `instance.staticMethod()`
            // are valid
            if (M) {
               Cand.UFCS = CandidateSet::MethodCalledAsFunction;
            }
         }
      }
   }

   if (auto Parent = Ctx->getParentCtx())
      addUFCSCandidates(Parent, name, CandSet, Overloads);
}

template<class T = CallableDecl>
static void addUFCSCandidates(SemaPass &SP,
                              Expression *FirstArg,
                              DeclarationName name,
                              CandidateSet &CandSet,
                              llvm::SmallPtrSetImpl<CallableDecl*> &Overloads) {
   QualType FirstArgTy;
   if (FirstArg->isContextDependent()) {
      FirstArgTy = SP.GetDefaultExprType(FirstArg);
   }
   else {
      FirstArgTy = FirstArg->getExprType()->stripReference();
   }

   if (!FirstArgTy)
      return;

   auto ContextAndIsStatic = getAsContext(FirstArgTy);
   if (ContextAndIsStatic.first) {
      addUFCSCandidates(ContextAndIsStatic.first, name, CandSet, Overloads);
   }
}

CandidateSet
SemaPass::lookupFunction(DeclarationName name,
                         llvm::ArrayRef<Expression *> args,
                         llvm::ArrayRef<Expression *> templateArgs,
                         Statement *Caller,
                         bool suppressDiags) {
   return lookupFunction(&getDeclContext(), name, args,
                         templateArgs, Caller, suppressDiags);
}

CandidateSet
SemaPass::lookupFunction(DeclContext *Ctx,
                         DeclarationName name,
                         llvm::ArrayRef<Expression *> args,
                         llvm::ArrayRef<Expression *> templateArgs,
                         Statement *Caller,
                         bool suppressDiags) {
   CandidateSet CandSet;
   bool IsLocalLookup = Ctx == &getDeclContext();
   bool DoUFCSLookup = IsLocalLookup;

   llvm::SmallPtrSet<CallableDecl*, 8> CandidateFns;
   addCandidates(Ctx, name, CandSet, CandidateFns, IsLocalLookup);

   // check if we can find additional candidates through UFCS
   if (DoUFCSLookup && !args.empty()) {
      auto First = args.front();
      addUFCSCandidates(*this, First, name, CandSet, CandidateFns);
   }

   CandBuilder.addBuiltinCandidates(CandSet, name, args);

   lookupFunction(CandSet, name, args, templateArgs, Caller, suppressDiags);
   return CandSet;
}

CandidateSet SemaPass::getCandidates(DeclarationName name,
                                     Expression *SelfExpr) {
   llvm::SmallPtrSet<CallableDecl*, 8> Overloads;

   CandidateSet CandSet;
   addCandidates(&getDeclContext(), name, CandSet, Overloads, true);
   addUFCSCandidates(*this, SelfExpr, name, CandSet, Overloads);

   CandBuilder.addBuiltinCandidates(CandSet, name, SelfExpr->getExprType());

   return CandSet;
}

CandidateSet
SemaPass::lookupCase(DeclarationName name,
                     EnumDecl *E,
                     llvm::ArrayRef<Expression *> args,
                     llvm::ArrayRef<Expression *> templateArgs,
                     Statement *Caller,
                     bool suppressDiags) {
   CandidateSet CandSet;

   if (auto Case = E->hasCase(name))
      CandSet.addCandidate(Case);

   lookupFunction(CandSet, name, args, templateArgs, Caller, suppressDiags);
   return CandSet;
}

CandidateSet SemaPass::checkAnonymousCall(FunctionType *FTy,
                                          llvm::ArrayRef<Expression *> args,
                                          Statement *Caller) {
   CandidateSet CandSet;
   CandSet.addCandidate(FTy);

   lookupFunction(CandSet, DeclarationName(), args, {}, Caller, true);
   if (!CandSet)
      CandSet.diagnoseAnonymous(*this, args, Caller);

   return CandSet;
}

void SemaPass::lookupFunction(CandidateSet &CandSet,
                              DeclarationName name,
                              llvm::ArrayRef<Expression *> args,
                              llvm::ArrayRef<Expression *> templateArgs,
                              Statement *Expr,
                              bool suppressDiags) {
   OverloadResolver Resolver(*this, args, templateArgs, Expr);
   Resolver.resolve(CandSet);

   if (!CandSet) {
      if (suppressDiags || CandSet.isDependent())
         return;

      return CandSet.diagnose(*this, name, args, templateArgs, Expr);
   }

   maybeInstantiate(CandSet, Expr);
}

static MethodDecl *getEquivalentMethod(MethodDecl *Orig,
                                       RecordDecl *Inst) {
   for (auto M : Inst->getDecls<MethodDecl>())
      if (M->getMethodID() == Orig->getMethodID())
         return M;

   llvm_unreachable("no equivalent method!");
}

bool SemaPass::maybeInstantiateRecord(CandidateSet::Candidate &Cand,
                                      const TemplateArgList &templateArgs,
                                      Statement *Caller) {
   auto F = Cand.Func;

   if (auto Case = dyn_cast<EnumCaseDecl>(F)) {
      assert(Case->getRecord()->isTemplate());

      auto Inst =
         Instantiator.InstantiateRecord(Caller, Case->getRecord(),
                                        templateArgs);

      if (!Inst)
         return false;

      Cand.Func = cast<EnumDecl>(Inst.getValue())
         ->lookupSingle<EnumCaseDecl>(Case->getIdentifierInfo());

      assert(Cand.Func && "did not instantiate enum correctly");
      return true;
   }

   auto Init = dyn_cast<InitDecl>(F);
   assert(Init && Init->getRecord()->isTemplate());

   auto Inst = Instantiator.InstantiateRecord(Caller, Init->getRecord(),
                                              templateArgs);

   if (Inst.hasValue()) {
      Cand.Func = getEquivalentMethod(Init, Inst.getValue());
      return true;
   }

   return false;
}

void SemaPass::maybeInstantiate(CandidateSet &CandSet,
                                Statement *Caller) {
   if (!CandSet)
      return;

   auto &Cand = CandSet.getBestMatch();
   if (!Cand.Func || Cand.InnerTemplateArgs.isStillDependent())
      return;

   auto F = Cand.Func;
   if (!F->isTemplate()) {
      if (auto M = dyn_cast<MethodDecl>(F)) {
         return maybeInstantiateMemberFunction(M, Caller);
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

void SemaPass::maybeInstantiateMemberFunction(MethodDecl *M,
                                              StmtOrDecl Caller) {
   if (M->getBody() || !M->getBodyTemplate())
      return;

   Instantiator.InstantiateMethodBody(Caller, M);

   if (auto Init = dyn_cast<InitDecl>(M)) {
      if (Init->isCompleteInitializer()) {
         Init->getBaseInit()->setBody(Init->getBody());
      }
   }
}

void SemaPass::ApplyCasts(llvm::MutableArrayRef<Expression*> args,
                          Expression *DependentExpr,
                          CandidateSet &CandSet) {
   auto &Cand = CandSet.getBestMatch();

   FunctionType *FuncTy = Cand.getFunctionType();
   auto ParamTys = FuncTy->getParamTypes();

   unsigned i = 0;
   bool FirstIsSelf = Cand.Func && Cand.Func->isNonStaticMethod();

   for (auto& arg : args) {
      if (FirstIsSelf) {
         if (arg->isContextDependent())
            (void)visitExpr(DependentExpr, arg);

         QualType SelfType =
            cast<MethodDecl>(Cand.Func)->getSelfType();

         arg = implicitCastIfNecessary(arg, SelfType);
         FirstIsSelf = false;

         continue;
      }

      if (arg->isContextDependent()) {
         if (ParamTys.size() > i)
            arg->setContextualType(ParamTys[i]);

         (void)visitExpr(DependentExpr, arg);

         ++i;
         continue;
      }

      if (arg->getExprType()->isVoidType()) {
         diagnose(arg, err_vararg_cannot_pass_void, arg->getSourceRange());
      }

      if (i >= CandSet.Conversions.size()) {
         ++i;
         continue;
      }

      auto &ConvSeq = CandSet.Conversions[i];
      if (!ConvSeq.isNoOp() && !arg->isTypeDependent()) {
         arg = ImplicitCastExpr::Create(Context, arg, move(ConvSeq));

         auto Res = visitExpr(arg);
         (void)Res; assert(Res && "bad implicit cast sequence!");
      }

      ++i;
   }
}

void SemaPass::PrepareCallArgs(CandidateSet::Candidate &Cand,
                               std::vector<Expression*>& args,
                               Expression *Expr,
                               CallableDecl *C) {
   switch (Cand.UFCS) {
   case CandidateSet::NoUFCS:
      if (C->isNonStaticMethod()) {
         // add the parent expression to the argument list, e.g.
         //  `x.method()` => `method(x)`
         if (auto Parent = Expr->maybeGetParentExpr()) {
            args.insert(args.begin(), Parent);
            Expr->setParentExpr(Parent->maybeGetParentExpr());
         }
      }

      break;
   case CandidateSet::MethodCalledAsFunction:
      // remove the first argument if this method is static, as it only
      // served to provide a lookup context, e.g.
      //  `staticMethod(TypeName) => TypeName.staticMethod()`
      // note: non static methods found this way can be kept the same, as
      // they conceptually are just Ctions with an extra self parameter
      // anyway
      if (C->isStatic()) {
         Expr->setParentExpr(args.front());
         args.erase(args.begin());
      }

      break;
   case CandidateSet::FunctionCalledAsMethod:
      // add the parent expression to the argument list, e.g.
      //  `x.freeStandingFunc()` => `freeStandingFunc(x)`
      if (auto Parent = Expr->maybeGetParentExpr()) {
         args.insert(args.begin(), Parent);
         Expr->setParentExpr(Parent->maybeGetParentExpr());
      }

      break;
   }
   
   auto declaredArgs = C->getArgs();
   auto FuncTy = C->getFunctionType();

   if (!FuncTy->isCStyleVararg() && args.size() < declaredArgs.size()) {
      assert(C && "no default arguments!");

      while (args.size() < declaredArgs.size()) {
         auto DefaultVal = C->getArgs()[args.size()]->getDefaultVal();

         if (DefaultVal->isMagicArgumentValue()) {
            auto Alias = cast<IdentifierRefExpr>(DefaultVal)->getAlias();
            auto Result = HandleReflectionAlias(Alias, Expr);

            if (Result)
               args.push_back(Result.getValue());
            else
               args.push_back(DefaultVal);
         }
         else {
            args.push_back(DefaultVal);
         }
      }
   }
}

static CallExpr *checkTemplatedCall(SemaPass &SP, AnonymousCallExpr *Call)
{
   auto ParentExpr = Call->getParentExpr();
   if (!ParentExpr)
      return nullptr;

   if (auto Ident = dyn_cast<IdentifierRefExpr>(ParentExpr)) {
      if (Ident->getTemplateArgs().empty())
         return nullptr;

      auto NewCall = new(SP.getContext()) CallExpr(Call->getSourceLoc(),
                                                   Call->getParenRange(),
                                                   Ident->getParentExpr(),
                                                   Call->getArgs().vec(),
                                                   Ident->getDeclName());

      NewCall->setTemplateArgs(move(Ident->getTemplateArgRef()));
      return NewCall;
   }
   if (auto MemExpr = dyn_cast<MemberRefExpr>(ParentExpr)) {
      if (MemExpr->getTemplateArgs().empty())
         return nullptr;

      auto NewCall = new(SP.getContext()) CallExpr(Call->getSourceLoc(),
                                                   Call->getParenRange(),
                                                   MemExpr->getParentExpr(),
                                                   Call->getArgs().vec(),
                                                   MemExpr->getDeclName());

      NewCall->setTemplateArgs(move(MemExpr->getTemplateArgRef()));
      return NewCall;
   }

   return nullptr;
}

ExprResult SemaPass::visitCallExpr(CallExpr *Call)
{
   bool Dependent = false;
   auto &args = Call->getArgs();
   for (auto &arg : args) {
      if (!arg->isContextDependent()) {
         auto result = visitExpr(Call, arg);
         if (!result)
            return ExprError();

         arg = result.get();
         Dependent |= arg->getExprType()->isUnknownAnyType();
      }
   }

   for (auto &TA : Call->getTemplateArgs()) {
      auto res = visitExpr(Call, TA);
      if (!res)
         return ExprError();

      TA = res.get();
      Dependent |= TA->getExprType()->isUnknownAnyType();
   }

   // if one of the arguments already introduced dependence, stop now
   if (Dependent) {
      Call->setExprType(UnknownAnyTy);
      return Call;
   }

   // expression was previously resolved (for example by a binary operator node)
   if (Call->getKind() != CallKind::Unknown) {
      return Call;
   }

   // check if '.init' is called without a parent expression
   if (!Call->getParentExpr() && Call->isDotInit()) {
      diagnose(Call, err_dot_init_must_be_on_self, Call->getSourceRange());
      Call->setExprType(Context.getVoidType());

      return Call;
   }

   if (Call->isNamedCall()) {
      if (getStaticForValue(Call->getDeclName().getIdentifierInfo())) {
         Call->setIsValueDependent(true);
         Call->setIsTypeDependent(true);
         Call->setExprType(UnknownAnyTy);

         return Call;
      }

      if (!Call->getParentExpr()) {
         auto Builtin = llvm::StringSwitch<BuiltinFn>(Call->getDeclName()
                                                          .getIdentifierInfo()
                                                          ->getIdentifier())
            .Case("decltype", BuiltinFn::DeclType)
            .Case("__builtin_bitcast", BuiltinFn::BITCAST)
            .Case("stackalloc", BuiltinFn::STACK_ALLOC)
            .Case("__ctfe_stacktrace", BuiltinFn::CtfePrintStackTrace)
            .Default(BuiltinFn::None);

         Call->setBuiltinFnKind(Builtin);
      }
   }

   // check if the call's parent expr refers to a namespace
   if (!checkNamespaceRef(Call))
      return ExprError();

   bool isBuiltin = Call->getBuiltinFnKind() != BuiltinFn::None;
   if (isBuiltin) {
      return HandleBuiltinCall(Call);
   }

   bool AddSelf = false;
   bool IsLocalLookup = true;
   DeclContext *Ctx = Call->getContext();

   if (auto ParentExpr = Call->getParentExpr()) {
      auto ParentResult = visitExpr(Call, ParentExpr);
      if (!ParentResult)
         return ExprError();

      ParentExpr = ParentResult.get();
      Call->setParentExpr(ParentExpr);

      auto ty = ParentExpr->getExprType()->stripReference();
      if (ty->isDependentType()) {
         Call->setIsTypeDependent(true);
         Call->setExprType(UnknownAnyTy);

         return Call;
      }

      if (ty->isPointerType()) {
         if (!Call->isPointerAccess()) {
            diagnose(Call, err_access_member_on_pointer, Call->getSourceLoc());
         }

         ty = ty->getPointeeType();
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
         else if (!isa<SelfExpr>(ParentExpr->ignoreParens())) {
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

      auto ContextAndIsStatic = getAsContext(ty);
      if (!ContextAndIsStatic.first) {
         // cannot lookup members in this type, interpret as UFCS and add
         // as first argument
         Call->getArgs().insert(Call->getArgs().begin(),
                                Call->getParentExpr());
         Call->setParentExpr(nullptr);
      }
      else if (!ContextAndIsStatic.second) {
         AddSelf = true;
      }

      IsLocalLookup = false;
      Ctx = ContextAndIsStatic.first;
   }

   if (!Ctx)
      Ctx = &getDeclContext();

   Call->setContext(Ctx);

   CandidateSet CandSet;
   llvm::SmallPtrSet<CallableDecl*, 8> CandidateFns;
   bool DoUFCSLookup = IsLocalLookup;

   auto Result = PerformLocalLookup(Call->getDeclName(), Ctx);
   if (!Result.lookupResult.empty()) {
      // check if we found something that is not a function first, no need to do
      // overload resolution or check UFCS here
      auto ND = Result.lookupResult.front();
      switch (ND->getKind()) {
      case Decl::EnumCaseDeclID: {
         auto enumCaseExpr =
            new(getContext()) EnumCaseExpr(Call->getSourceLoc(),
                                           cast<EnumDecl>(Ctx),
                                           Call->getDeclName()
                                               .getIdentifierInfo(),
                                           move(Call->getArgs()));

         enumCaseExpr->setContextualType(Call->getContextualType());
         return visitExpr(Call, enumCaseExpr);
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

         ID->setPointerAccess(Call->isPointerAccess());
         ID->setParentExpr(Call->getParentExpr());

         auto Anon = AnonymousCallExpr::Create(Context, Call->getParenRange(),
                                               ID, Call->getArgs());

         return visitExpr(Call, Anon);
      }
      case Decl::StructDeclID:
      case Decl::ClassDeclID:
      case Decl::EnumDeclID:
      case Decl::UnionDeclID:
      case Decl::ProtocolDeclID:
         return HandleConstructorCall(Call, cast<RecordDecl>(ND));
      case Decl::InitDeclID:
         return HandleConstructorCall(Call, ND->getRecord());
      case Decl::MethodDeclID:
         // if we found an instance method through local lookup, build an
         // implicit self expression. In this case we do not perform UFCS lookup
         if (IsLocalLookup) {
            if (!ND->isStatic()) {
               Expression *Self = SelfExpr::Create(Context,
                                                   Call->getSourceLoc());

               auto Res = visitExpr(Call, Self);
               if (!Res)
                  return ExprError();

               updateParent(Call, Res.get());
               Call->setParentExpr(Res.get());

               AddSelf = true;
            }

            DoUFCSLookup = false;
            IsLocalLookup = false; // allow methods to be found via normal
                                   // lookup
         }

         break;
      case Decl::FunctionDeclID:
         break;
      default:
         llvm_unreachable("unhandled decl kind!");
      }

      // add candidates found via normal lookup
      addCandidates(Ctx, Call->getDeclName(), CandSet, CandidateFns,
                    IsLocalLookup);
   }

   // check if we can find additional candidates through UFCS
   if (DoUFCSLookup && !args.empty()) {
      auto First = args.front();
      addUFCSCandidates(*this, First, Call->getDeclName(), CandSet,
                        CandidateFns);
   }

   if (!Result.lookupResult) {
      diagnose(Call, err_func_not_found, 0, Call->getDeclName(),
               Call->getSourceLoc());

      return ExprError();
   }

   if (AddSelf) {
      llvm::SmallVector<Expression*, 8> ArgsWithSelf{ Call->getParentExpr() };
      ArgsWithSelf.append(Call->getArgs().begin(), Call->getArgs().end());

      lookupFunction(CandSet, Call->getDeclName(), ArgsWithSelf,
                     Call->getTemplateArgs(), Call);
   }
   else {
      lookupFunction(CandSet, Call->getDeclName(), Call->getArgs(),
                     Call->getTemplateArgs(), Call);
   }

   if (CandSet.isDependent()) {
      Call->setIsTypeDependent(true);
      Call->setExprType(UnknownAnyTy);

      return Call;
   }

   if (!CandSet)
      return ExprError();

   auto &Cand = CandSet.getBestMatch();
   auto func = Cand.Func;

   checkAccessibility(func, Call);

   if (isa<ProtocolDecl>(func->getDeclContext()))
      llvm_unreachable("FIXME protocol methods");

   if (func->isTemplateOrInTemplate()) {
      Call->setIsTypeDependent(true);
      Call->setExprType(func->getReturnType());

      return Call;
   }

   PrepareCallArgs(Cand, Call->getArgs(), Call, func);
   ApplyCasts(Call->getArgs(), Call, CandSet);

   Call->setFunc(func);
   Call->setExprType(func->getReturnType());
   Call->setKind(CallKind::NamedFunctionCall);

   return Call;
}

ExprResult SemaPass::HandleBuiltinCall(CallExpr *Call)
{
   if (Call->getKind() == CallKind::VariadicSizeof) {
      Call->setReturnType(Context.getUIntTy());

      auto &TA = Call->getTemplateArgs().front();

      auto Ident = dyn_cast<IdentifierRefExpr>(TA);
      if (!Ident) {
         diagnose(Call, err_variadic_sizeof_expects, Call->getSourceLoc());
         return Call;
      }

      auto Param = getTemplateParam(Ident->getIdentInfo());
      if (!Param) {
         diagnose(Call, err_does_not_name_template_parm, Ident->getIdent(),
                  false, Call->getSourceLoc());
      }

      else if (!Param->isVariadic()) {
         diagnose(Call, err_does_not_name_template_parm, Ident->getIdent(),
                  true, Call->getSourceLoc());
      }

      return Call;
   }

   if (Call->getBuiltinFnKind() == BuiltinFn::DeclType) {
      if (Call->getArgs().size() != 1) {
         Call->setBuiltinFnKind(BuiltinFn::None);
      }
      else {
         auto DT = DeclTypeExpr::Create(Context, Call->getSourceRange(),
                                        Call->getArgs().front());

         return visitExpr(Call, DT);
      }
   }

//   if (!HandleFunctionCall(Call, &getDeclContext(), Call->getDeclName()))
//      return ExprError();

   Call->setKind(CallKind::Builtin);
   if (Call->getBuiltinFnKind() == BuiltinFn::SIZEOF) {
      auto Ty = Call->getTemplateArgs().front()->getExprType();
      if (Ty->isMetaType())
         Ty = Ty->uncheckedAsMetaType()->getUnderlyingType();

      if (Ty->isRecordType()) {
         if (!ensureSizeKnown(Ty->getRecord(), Call->getSourceLoc()))
            Call->setIsInvalid(true);
      }
   }

   return Call;
}

ExprResult SemaPass::HandleStaticTypeCall(CallExpr *Call, Type *Ty)
{
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
      return HandleConstructorCall(Call, Ty->getRecord());
   }

   diagnose(Call, err_cannot_call_type, Ty, true, Call->getSourceLoc());
   return ExprError();
}

ExprResult SemaPass::HandleConstructorCall(CallExpr *Call, RecordDecl *R)
{
   assert(R && "should not be called otherwise");

   if (!R->isTemplate() && !Call->getTemplateArgs().empty()) {
      if (Call->getDeclName()) {
         diagnose(Call, err_generic_type_count, 0,
                  Call->getTemplateArgs().size(),
                  Call->getDeclName(), Call->getSourceLoc());
      }
      else {
         diagnose(Call, err_generic_type_count, 0,
                  Call->getTemplateArgs().size(),
                  "(anonymous)", Call->getSourceLoc());
      }

      Call->getTemplateArgs().clear();
   }

//   if (auto U = dyn_cast<UnionDecl>(R)) {
//      Call->setReturnType(Context.getRecordType(U));
//
//      auto &args = Call->getArgs();
//      if (args.size() != 1) {
//         diagnose(Call, args.empty() ? err_too_few_args_for_call
//                                     : err_too_many_args_for_call,
//                  1, args.size(), Call->getSourceLoc());
//
//         return Call;
//      }
//
//      auto ty = Call->getArgs().front()->getExprType();
//
//      QualType neededTy;
//      for (auto &F : U->getFields()) {
//         auto &fieldType = F->getType();
//         if (implicitlyCastableTo(ty, fieldType)) {
//            neededTy = fieldType.getResolvedType();
//            break;
//         }
//      }
//
//      if (!neededTy) {
//         diagnose(Call, err_union_initializer_type, Call->getSourceLoc());
//         return Call;
//      }
//
//      implicitCastIfNecessary(Call->getArgs().front(), neededTy);
//
//      Call->setKind(CallKind::UnionInitializer);
//      Call->setUnion(U);
//
//      return Call;
//   }

   if (isa<EnumDecl>(R)) {
      diagnose(Call, err_enum_initializer);

      Call->setExprType(Context.getRecordType(R));
      return Call;
   }

   if (isa<ProtocolDecl>(R)) {
      diagnose(Call, err_protocol_initializer);

      Call->setExprType(Context.getRecordType(R));
      return Call;
   }

   auto S = cast<StructDecl>(R);

   auto& givenArgs = Call->getArgs();
   auto DeclName = Context.getDeclNameTable()
                          .getConstructorName(Context.getRecordType(S));

   auto CandSet = lookupFunction(S, DeclName, givenArgs,
                                 Call->getTemplateArgs(), Call);

   if (CandSet.isDependent()) {
      Call->setIsTypeDependent(true);
      Call->setExprType(UnknownAnyTy);

      return Call;
   }

   if (!CandSet) {
      Call->setIsInvalid(true);

      if (!S->isTemplate()) {
         Call->setReturnType(Context.getRecordType(S));
         return Call;
      }

      return ExprError();
   }

   auto method = cast<MethodDecl>(CandSet.getBestMatch().Func);
   checkAccessibility(method, Call);

   if (method->isTemplateOrInTemplate()) {
      Call->setIsTypeDependent(true);
      Call->setExprType(UnknownAnyTy);

      if (!S->isTemplate()) {
         Call->setReturnType(Context.getRecordType(S));
         return Call;
      }

      return Call;
   }

   S = cast<StructDecl>(method->getRecord());

   if (auto Cl = dyn_cast<ClassDecl>(S))
      if (Cl->isAbstract())
         diagnose(Call, err_instantiate_abstract_class, Cl->getName());

   PrepareCallArgs(CandSet.getBestMatch(), Call->getArgs(), Call, method);
   ApplyCasts(Call->getArgs(), Call, CandSet);

   if (Call->isDotInit()) {
      Call->setExprType(Context.getVoidType());
      method = cast<InitDecl>(method)->getBaseInit();

      Call->getArgs().insert(Call->getArgs().begin(), Call->getParentExpr());
      Call->setParentExpr(Call->getParentExpr()->maybeGetParentExpr());
   }
   else {
      Call->setExprType(Context.getRecordType(S));
   }

   Call->setKind(CallKind::InitializerCall);
   Call->setFunc(method);

   return Call;
}

static bool canZeroInitializeType(QualType Ty)
{
   switch (Ty->getTypeID()) {
   case Type::BuiltinTypeID:
      return Ty->isIntegerType() || Ty->isFPType();
   case Type::PointerTypeID:
      return true;
   case Type::ArrayTypeID:
      return canZeroInitializeType(Ty->asArrayType()->getElementType());
   case Type::TupleTypeID:
      for (auto &SubTy : Ty->asTupleType()->getContainedTypes())
         if (!canZeroInitializeType(SubTy))
            return false;

      return true;
   default:
      return false;
   }
}

static ExprResult checkPrimitiveInitializer(SemaPass &SP,
                                            AnonymousCallExpr *Call) {
   if (!Call->getArgs().empty()) {
      llvm_unreachable("make diagnostic here");
   }

   auto Ty = Call->getParentExpr()->getExprType()->asMetaType()
      ->getUnderlyingType();

   if (canZeroInitializeType(Ty)) {
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

   // check for a templated function call, e.g.
   //    func[u8]()
   // this will be parsed as
   //    IdentifierRefExpr ['func'] [TemplateArgs = [u8]]
   //    |- AnonymousCallExpr [<anonymous>]
   // and should be turned into
   //    CallExpr ['func'] [TemplateArgs = [u8]]
   if (auto TemplatedCall = checkTemplatedCall(*this, Call))
      return visitExpr(Call, TemplatedCall);

   Expression *ParentExpr = Call->getParentExpr();

   auto ParentResult = getRValue(Call, ParentExpr);
   if (!ParentResult)
      return ExprError();

   ParentExpr = ParentResult.get();
   Call->setParentExpr(ParentExpr);

   auto ty = ParentExpr->getExprType();
   if (ty->isDependentType()) {
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
         auto NamedCall = new(Context) CallExpr(Call->getSourceLoc(),
                                                Call->getParenRange(),
                                                Call->getArgs().vec(),
                                                DN);

         NamedCall->setParentExpr(Call->getParentExpr());
         return visitExpr(Call, NamedCall);
      }

      if (auto Meta = ty->asMetaType()) {
         if (auto MetaRecTy = Meta->getUnderlyingType()->asRecordType()) {
            // constructor call
            auto DN = Context.getDeclNameTable().getConstructorName(MetaRecTy);
            auto NamedCall = new(Context) CallExpr(Call->getSourceLoc(),
                                                   Call->getParenRange(),
                                                   Call->getArgs().vec(),
                                                   DN);

            NamedCall->setParentExpr(Call->getParentExpr());
            return visitExpr(Call, NamedCall);
         }

         return checkPrimitiveInitializer(*this, Call);
      }

      diagnose(Call, err_cannot_call_type, Call->getSourceLoc(), ty, false);
      return ExprError();
   }

   auto CandSet = checkAnonymousCall(func, Call->getArgs(), Call);
   if (CandSet.isDependent()) {
      Call->setIsTypeDependent(true);
      Call->setExprType(UnknownAnyTy);

      return Call;
   }

   if (!CandSet)
      return ExprError();

   ApplyCasts(Call->getArgs(), Call, CandSet);

   Call->setFunctionType(func);
   Call->setExprType(func->getReturnType());

   return Call;
}

bool SemaPass::ExprCanReturn(Expression *E, QualType Ty)
{
   E = E->ignoreParens();
   Ty = Ty->getCanonicalType();

   if (Ty->isUnknownAnyType())
      return true;

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
         return Ty == IL->getType();
      case IntegerLiteral::Suffix::u:
         return Ty->isIntegerType() && Ty->isUnsigned();
      case IntegerLiteral::Suffix::i:
         return Ty->isIntegerType() && !Ty->isUnsigned();
      case IntegerLiteral::Suffix::None:
         break;
      }

      return Ty->isIntegerType();
   }
   case Expression::CharLiteralID:
      return Ty->isIntegerType() && Ty->getBitwidth() == 8;
   case Expression::FPLiteralID: {
      auto FP = cast<FPLiteral>(E);

      switch (FP->getSuffix()) {
      case FPLiteral::Suffix::f:
      case FPLiteral::Suffix::d:
         return Ty == FP->getType();
      case FPLiteral::Suffix::None:
         break;
      }

      return Ty->isFPType();
   }
   case Expression::StringLiteralID:
      if (Ty->isPointerType() && Ty->getPointeeType()->isInt8Ty()) {
         return true;
      }
      else if (Ty->isArrayType()
               && Ty->asArrayType()->getElementType()->isInt8Ty()) {
         return true;
      }

      return Ty->isRecordType() && (Ty->getRecord() == getStringDecl()
                                    || Ty->getRecord() == getStringViewDecl());
   case Expression::EnumCaseExprID: {
      if (!Ty->isRecordType())
         return false;

      auto Enum = dyn_cast<EnumDecl>(Ty->getRecord());
      if (!Enum)
         return false;

      return Enum->hasCase(cast<EnumCaseExpr>(E)->getDeclName()) != nullptr;
   }
   case Expression::LambdaExprID: {
      if (!Ty->isLambdaType())
         return false;

      auto LE = cast<LambdaExpr>(E);
      auto Func = Ty->asLambdaType();
      auto NeededParams = Func->getParamTypes();

      if (NeededParams.size() < LE->getNumArgs())
         return false;

      // check only the types of explicitly specified arguments
      unsigned i = 0;
      for (auto &Arg : LE->getArgs()) {
         auto NeededTy = NeededParams[i];
         auto GivenTypeRes = visitSourceType(Arg, Arg->getType());

         if (!GivenTypeRes) {
            LE->setIsInvalid(true);
            return false;
         }

         if (GivenTypeRes.getValue()->isAutoType())
            continue;

         if (GivenTypeRes.getValue() != NeededTy)
            return false;
      }

      auto NeededRetRes = visitSourceType(LE, LE->getReturnType());
      if (!NeededRetRes)
         return false;

      if (NeededRetRes.getValue()->isAutoType())
         return true;

      return NeededRetRes.getValue() == Func->getReturnType();
   }
   default:
      llvm_unreachable("bad context dependent expression kind!");
   }
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
   llvm::SmallVector<QualType, 4> ArgTys;

   // check only the types of explicitly specified arguments
   unsigned i = 0;
   for (auto &Arg : LE->getArgs()) {
      auto NeededTy = NeededParams[i];
      auto &ArgTy = Arg->getType();
      auto GivenTypeRes = visitSourceType(Arg, ArgTy);

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

      if (ArgTy != NeededTy)
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
   case Expression::EnumCaseExprID: {
      return QualType();
   }
   default:
      llvm_unreachable("bad context dependent expression kind!");
   }
}

} // namespace ast
} // namespace cdot