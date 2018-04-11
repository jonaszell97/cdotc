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
                          unsigned Penalty = 0) {
   auto lookupRes = Ctx->lookup(name);
   for (auto &ND : lookupRes) {
      if (auto C = dyn_cast<T>(ND)) {
         if (Overloads.insert(C).second) {
            auto &Cand = CandSet.addCandidate(C);

            // penalize candidates that are further removed from the callee
            // context
            Cand.ConversionPenalty += Penalty;
         }
      }
   }

   if (auto Parent = Ctx->getParentCtx())
      addCandidates(Parent, name, CandSet, Overloads, Penalty);
}

template<class T = CallableDecl>
static void addCandidates(DeclContext *Ctx,
                          DeclarationName name,
                          CandidateSet &CandSet) {
   auto lookupRes = Ctx->lookup(name);
   for (auto &ND : lookupRes) {
      if (auto C = dyn_cast<T>(ND)) {
         CandSet.addCandidate(C);
      }
   }

   if (auto Parent = Ctx->getParentCtx())
      addCandidates(Parent, name, CandSet);
}

CandidateSet
SemaPass::lookupFunction(DeclarationName name,
                         llvm::ArrayRef<Expression *> args,
                         llvm::ArrayRef<Expression *> templateArgs,
                         Statement *Caller,
                         bool suppressDiags,
                         bool includesSelf) {
   return lookupFunction(&getDeclContext(), name, args,
                         templateArgs, Caller, suppressDiags,
                         includesSelf);
}

CandidateSet
SemaPass::lookupFunction(DeclContext *Ctx,
                         DeclarationName name,
                         llvm::ArrayRef<Expression *> args,
                         llvm::ArrayRef<Expression *> templateArgs,
                         Statement *Caller,
                         bool suppressDiags,
                         bool includesSelf) {
   CandidateSet CandSet;
   CandSet.IncludesSelfArgument = includesSelf;

   llvm::SmallPtrSet<CallableDecl*, 8> Overloads;
   addCandidates(Ctx, name, CandSet, Overloads);

   if (!args.empty() && args.front()->getExprType()
         && Ctx == &getDeclContext()) {
      auto SelfExpr = args.front();
      RecordType *MaybeRecordTy = SelfExpr->getExprType()->stripReference()
                                          ->asRecordType();

      if (MaybeRecordTy)
         addCandidates(MaybeRecordTy->getRecord(), name, CandSet, Overloads);
   }

   CandBuilder.addBuiltinCandidates(CandSet, name, args);

   lookupFunction(CandSet, name, args, templateArgs, Caller, suppressDiags);
   return CandSet;
}

CandidateSet
SemaPass::lookupMethod(DeclarationName name,
                       Expression *SelfExpr,
                       llvm::ArrayRef<Expression *> args,
                       llvm::ArrayRef<Expression *> templateArgs,
                       Statement *Caller,
                       bool suppressDiags) {
   llvm::SmallVector<Expression*, 8> argsWithSelf{ SelfExpr };
   argsWithSelf.append(args.begin(), args.end());

   return lookupFunction(name, argsWithSelf, templateArgs, Caller,
                         suppressDiags, /*includesSelf=*/ true);
}

CandidateSet SemaPass::getCandidates(DeclarationName name,
                                     Expression *SelfExpr) {
   CandidateSet CandSet;
   CandSet.IncludesSelfArgument = true;

   RecordType *MaybeRecordTy = SelfExpr->getExprType()->stripReference()
                                       ->asRecordType();

   llvm::SmallPtrSet<CallableDecl*, 8> Overloads;
   if (MaybeRecordTy) {
      auto lookupRes = MaybeRecordTy->getRecord()->lookup(name);
      for (auto &ND : lookupRes) {
         if (auto M = dyn_cast<MethodDecl>(ND)) {
            if (Overloads.insert(M).second)
               CandSet.addCandidate(M);
         }
      }
   }

   addCandidates(&getDeclContext(), name, CandSet, Overloads);
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
   addCandidates<EnumCaseDecl>(E, name, CandSet);

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
                              Statement *Caller,
                              bool suppressDiags) {
   OverloadResolver Resolver(*this, args, templateArgs, Caller);
   Resolver.resolve(CandSet);

   if (!CandSet) {
      if (suppressDiags || CandSet.isDependent())
         return;

      return CandSet.diagnose(*this, name, args, templateArgs, Caller);
   }

   maybeInstantiate(CandSet, Caller);
}

static MethodDecl *getEquivalentMethod(MethodDecl *Orig,
                                       RecordDecl *Inst) {
   for (auto M : Inst->getDecls<MethodDecl>())
      if (M->getMethodID() == Orig->getMethodID())
         return M;

   llvm_unreachable("no equivalent method!");
}

bool SemaPass::maybeInstantiateRecord(CandidateSet::Candidate &Cand,
                                      TemplateArgList &&templateArgs,
                                      Statement *Caller) {
   auto F = Cand.Func;

   if (auto Case = dyn_cast<EnumCaseDecl>(F)) {
      assert(Case->getRecord()->isTemplate());

      auto Inst =
         Instantiator.InstantiateRecord(Caller, Case->getRecord(),
                                        move(templateArgs));

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
                                              move(templateArgs));

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
}

void SemaPass::ApplyCasts(llvm::MutableArrayRef<Expression*> args,
                          Expression *DependentExpr,
                          CandidateSet &CandSet) {
   auto &Cand = CandSet.getBestMatch();

   FunctionType *FuncTy = Cand.getFunctionType();
   auto ParamTys = FuncTy->getParamTypes();

   unsigned i = 0;
   bool FirstIsSelf = false;

   if (CandSet.IncludesSelfArgument) {
      if (Cand.isBuiltinCandidate()) {
         if (args.size() < ParamTys.size()) {
            // skip first (self) parameter
            ++i;
         }
      }
      else {
         // skip the first argument because it's not represented in the
         // function type
         FirstIsSelf = isa<MethodDecl>(Cand.Func);
      }
   }

   for (auto& arg : args) {
      if (FirstIsSelf) {
         if (!Cand.Func->hasMutableSelf())
            arg = castToRValue(arg);

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
      if (!ConvSeq.isNoOp()) {
         arg = ImplicitCastExpr::Create(Context, arg, move(ConvSeq));
      }

      ++i;
   }
}

void SemaPass::PrepareCallArgs(std::vector<Expression*>& args,
                               CallableDecl *C) {
   auto declaredArgs = C->getArgs();
   auto FuncTy = C->getFunctionType();

   if (!FuncTy->isCStyleVararg() && args.size() < declaredArgs.size()) {
      assert(C && "no default arguments!");

      while (args.size() < declaredArgs.size()) {
         args.push_back(C->getArgs()[args.size()]->getDefaultVal());
      }
   }
}

static bool checkTemplatedCall(SemaPass &SP, CallExpr *Call)
{
   if (Call->isNamedCall())
      return false;

   auto ParentExpr = Call->getParentExpr();
   if (!ParentExpr)
      return false;

   if (auto Ident = dyn_cast<IdentifierRefExpr>(ParentExpr)) {
      if (Ident->getTemplateArgs().empty())
         return false;

      Call->setTemplateArgs(move(Ident->getTemplateArgRef()));
      Call->setIdent(Ident->getIdentInfo());
      Call->setParentExpr(Ident->getParentExpr());

      return true;
   }
   if (auto MemExpr = dyn_cast<MemberRefExpr>(ParentExpr)) {
      if (MemExpr->getTemplateArgs().empty())
         return false;

      Call->setTemplateArgs(move(MemExpr->getTemplateArgRef()));
      Call->setIdent(MemExpr->getIdentInfo());
      Call->setParentExpr(MemExpr->getParentExpr());

      return true;
   }

   return false;
}

ExprResult SemaPass::visitCallExpr(CallExpr *Call)
{
   if (auto Param = getTemplateParam(Call->getDeclName())) {
      Call->setIsTypeDependent(true);
      Call->setExprType(Param->getCovariance());

      return Call;
   }

   auto &args = Call->getArgs();
   for (auto &arg : args) {
      if (!arg->isContextDependent()) {
         auto result = visitExpr(Call, arg);
         if (!result)
            return ExprError();

         arg = result.get();
         if (arg->isUnknownAny()) {
            Call->setIsTypeDependent(true);
            Call->setExprType(UnknownAnyTy);

            return Call;
         }
      }
   }

   for (auto &TA : Call->getTemplateArgs()) {
      auto res = visitExpr(Call, TA);
      if (!res)
         return ExprError();

      TA = res.get();
      if (TA->isUnknownAny()) {
         Call->setIsTypeDependent(true);
         Call->setExprType(UnknownAnyTy);

         return Call;
      }
   }

   // expression was previously resolved (for example by a binary operator node)
   if (Call->getKind() != CallKind::Unknown) {
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
            .Case("sizeof", BuiltinFn::SIZEOF)
            .Case("alignof", BuiltinFn::ALIGNOF)
            .Case("decltype", BuiltinFn::DeclType)
            .Case("default", BuiltinFn::DefaultVal)
            .Case("__builtin_sizeof", BuiltinFn::BuiltinSizeof)
            .Case("__builtin_isnull", BuiltinFn::ISNULL)
            .Case("__builtin_memcpy", BuiltinFn::MEMCPY)
            .Case("__builtin_memset", BuiltinFn::MEMSET)
            .Case("__builtin_memcmp", BuiltinFn::MemCmp)
            .Case("__nullptr", BuiltinFn::NULLPTR)
            .Case("__builtin_bitcast", BuiltinFn::BITCAST)
            .Case("stackalloc", BuiltinFn::STACK_ALLOC)
            .Case("__ctfe_stacktrace", BuiltinFn::CtfePrintStackTrace)
            .Default(BuiltinFn::None);

         Call->setBuiltinFnKind(Builtin);
      }
   }

   // check for a templated function call, e.g.
   //    func[u8]()
   // this will be parsed as
   //    IdentifierRefExpr ['func'] [TemplateArgs = [u8]]
   //    |- CallExpr [<anonymous>]
   // and should be turned into
   //    CallExpr ['func'] [TemplateArgs = [u8]]
   if (checkTemplatedCall(*this, Call))
      return visitCallExpr(Call);

   // check if the call's parent expr refers to a namespace
   if (!checkNamespaceRef(Call))
      return ExprError();

   bool isBuiltin = Call->getBuiltinFnKind() != BuiltinFn::None;
   if (isBuiltin) {
      return HandleBuiltinCall(Call);
   }

   if (auto ParentExpr = Call->getParentExpr()) {
      auto ParentResult = visitExpr(Call, ParentExpr);
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

      // called expression or call operator, e.g.
      //  let fn = () -> {}
      //  (() => {})()
      //            ^
      if (!Call->getDeclName()) {
         return HandleAnonCall(Call, ParentExpr);
      }

      // static or non-static method call
      //  x.method(); X.staticMethod()
      //    ^           ^
      return HandleMethodCall(Call, ParentExpr);
   }

   auto *Ctx = Call->getContext();
   if (!Ctx)
      Ctx = &getDeclContext();

   auto lookupResult = Ctx->lookup(Call->getDeclName());
   if (!lookupResult) {
      // static type call, e.g.
      //  let i = i64(291)
      //          ^
      if (auto builtinTy = getBuiltinType(Call->getDeclName())) {
         return HandleStaticTypeCall(Call, builtinTy);
      }

      // variable call, e.g.
      //  let fn = () => {}
      //  fn()
      //  ^
      if (auto I = wouldBeValidIdentifier(Call->getIdentLoc(),
                                          Call->getDeclName()
                                              .getIdentifierInfo())) {
         Call->setIdent(DeclarationName());
         Call->setParentExpr(I);

         return visitCallExpr(Call);
      }

      diagnose(Call, err_func_not_found, 0, Call->getDeclName(),
               Call->getSourceLoc());

      return ExprError();
   }

   auto ND = lookupResult.front();

   // initializer call, e.g.
   //  let s = String("hello")
   //          ^
   if (auto R = dyn_cast<RecordDecl>(ND)) {
      return HandleConstructorCall(Call, R);
   }

   // implicit method call without 'self'
   //  method("hello")
   //  ^
   if (auto M = dyn_cast<MethodDecl>(ND)) {
      Expression *Self;
      if (!M->isStatic()) {
         Self = SelfExpr::Create(Context, Call->getSourceLoc());
      }
      else {
         auto *II = &Context.getIdentifiers().get("Self");
         Self = new(Context) IdentifierRefExpr(Call->getSourceLoc(), II);
      }

      updateParent(Call, Self);
      Call->setParentExpr(Self);

      auto Res = visitExpr(Call, Self);
      if (!Res)
         return ExprError();

      return HandleMethodCall(Call, Self);
   }

   // ordinary named function call
   //  fn("hello")
   //  ^
   return HandleFunctionCall(Call, Ctx, Call->getDeclName());
}

ExprResult SemaPass::HandleFunctionCall(CallExpr *Call,
                                        DeclContext *Ctx,
                                        DeclarationName funcName) {
   // check if this is actually a variable of function type
   //
   // let fn: () -> Void
   // fn()
   // ^
   // FIXME
#if 0
   if (funcName.isSimpleIdentifier()) {
      if (auto I = wouldBeValidIdentifier(Call->getSourceLoc(),
                                          funcName.getIdentifierInfo())) {
         Call->setParentExpr(I);
         Call->setIdent(DeclarationName());

         return visitCallExpr(Call);
      }
   }
#endif

   auto CandSet = lookupFunction(Ctx, funcName, Call->getArgs(),
                                 Call->getTemplateArgs(), Call);

   if (CandSet.isDependent()) {
      Call->setIsTypeDependent(true);
      Call->setExprType(UnknownAnyTy);

      return Call;
   }

   if (!CandSet)
      return ExprError();

   auto func = CandSet.getBestMatch().Func;
   checkAccessibility(func, Call);

   PrepareCallArgs(Call->getArgs(), func);
   ApplyCasts(Call->getArgs(), Call, CandSet);

   Call->setKind(CallKind::NamedFunctionCall);
   Call->setFunc(func);
   Call->setReturnType(func->getReturnType());

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

   if (!HandleFunctionCall(Call, &getDeclContext(), Call->getDeclName()))
      return ExprError();

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

static ExprResult buildOperator(SemaPass &SP,
                                CallExpr *Call,
                                CandidateSet::Candidate &Cand) {
   //FIXME
   llvm::StringRef opName = Call->getDeclName().getIdentifierInfo()
                                ->getIdentifier();

   FixKind fix;
   if (opName.startswith("infix ")) {
      fix = FixKind::Infix;
      opName = opName.drop_front(6);
   }
   else if (opName.startswith("prefix ")) {
      fix = FixKind::Prefix;
      opName = opName.drop_front(7);
   }
   else if (opName.startswith("postfix ")) {
      fix = FixKind::Postfix;
      opName = opName.drop_front(8);
   }
   else {
      llvm_unreachable("not an operator name!");
   }

   auto &args = Call->getArgs();

   auto opKind = op::fromString(fix, opName);
   Expression *Expr;

   if (fix == FixKind::Infix) {
      Expr = BinaryOperator::Create(SP.getContext(), Call->getSourceLoc(),
                                    opKind, Cand.getFunctionType(),
                                    args[0], args[1]);
   }
   else {
      Expr = UnaryOperator::Create(SP.getContext(), Call->getSourceLoc(),
                                   opKind, Cand.getFunctionType(),
                                   args[0], fix == FixKind::Prefix);
   }

   SP.updateParent(Expr, SP.getParent(Call));
   return SP.visitExpr(Expr);
}

ExprResult SemaPass::HandleMethodCall(CallExpr *Call, Expression *ParentExpr)
{
   QualType Ty = ParentExpr->getExprType()->stripReference();
   if (Ty->isPointerType()) {
      if (!Call->isPointerAccess()) {
         diagnose(Call, err_access_member_on_pointer, Call->getSourceLoc());
      }

      Ty = Ty->getPointeeType();
   }
   else if (Call->isPointerAccess()) {
      diagnose(Call, err_member_access_non_pointer, Ty, Call->getSourceLoc());
   }

   bool IsMeta = false;
   bool StaticCall = false;
   CandidateSet CandSet;

   if (cdot::MetaType *Meta = Ty->asMetaType()) {
      Ty = Meta->getUnderlyingType();
      IsMeta = true;
   }

   // check if this is actually a field of function type (or another
   // callable type)
   //
   // struct X {
   //    let fn: () -> Void
   // }
   //
   // X().fn()
   //     ^
   if (Ty->isRecordType()) {
      auto R = Ty->getRecord();
      auto lookup = R->lookup(Call->getDeclName());

      for (auto D : lookup) {
         if (!isa<FieldDecl>(D))
            break;

         auto MemExpr = new(Context) MemberRefExpr(Call->getSourceLoc(),
                                                   ParentExpr,
                                                   Call->getDeclName()
                                                       .getIdentifierInfo(),
                                                   Call->isPointerAccess());

         Call->setIdent(DeclarationName());
         Call->setParentExpr(MemExpr);

         return visitCallExpr(Call);
      }
   }

   if (IsMeta) {
      // enum case, e.g.
      //  let opt = Option[i64].Some(29)
      //                        ^
      if (Ty->isEnum()) {
         auto E = cast<EnumDecl>(Ty->getRecord());
         if (E->hasCase(Call->getDeclName())) {
            auto enumCaseExpr =
               new(getContext()) EnumCaseExpr(Call->getSourceLoc(),
                                              Call->getDeclName()
                                                  .getIdentifierInfo(),
                                              move(Call->getArgs()));

            enumCaseExpr->setEnum(E);
            enumCaseExpr->setContextualType(Call->getContextualType());

            return visitEnumCaseExpr(enumCaseExpr);
         }
      }
      else if (auto RecTy = Ty->asRecordType()) {
         // must be a static method call
         StaticCall = true;
         CandSet = lookupFunction(RecTy->getRecord(),
                                  Call->getDeclName(), Call->getArgs(),
                                  Call->getTemplateArgs(), Call);
      }
      else {
         diagnose(Call, err_cannot_call_type, Ty, true /*statically*/,
                  Call->getSourceLoc());
      }
   }
   else {
      CandSet = lookupMethod(Call->getDeclName(),
                             ParentExpr, Call->getArgs(),
                             Call->getTemplateArgs(), Call);
   }

   if (CandSet.isDependent()) {
      Call->setIsTypeDependent(true);
      Call->setExprType(UnknownAnyTy);

      return Call;
   }

   if (!CandSet) {
      return ExprError();
   }

   auto &BestMatch = CandSet.getBestMatch();
   auto func = CandSet.getBestMatch().Func;

   checkAccessibility(func, Call);

   // check UFCS, transform
   //  `3.method()` into
   //  `method(3)`
   // if necessary
   if (!StaticCall) {
      Call->getArgs().insert(Call->getArgs().begin(), ParentExpr);
      Call->setParentExpr(Call->getParentExpr()->maybeGetParentExpr());
   }

   // this happens if a builtin operator is used as a method, e.g.
   // 3.`infix +`(5)
   //    ^
   if (!func) {
      assert(BestMatch.isBuiltinCandidate() && "no function for call!");
      return buildOperator(*this, Call, BestMatch);
   }

   auto &returnTy = func->getReturnType();

   Call->setReturnType(returnTy);
   Call->setFunc(func);
   Call->setKind(func->isStatic() ? CallKind::StaticMethodCall
                                  : CallKind::MethodCall);

   PrepareCallArgs(Call->getArgs(), func);
   ApplyCasts(Call->getArgs(), Call, CandSet);

   if (!func->hasMutableSelf() && Call->getParentExpr()) {
      Call->setParentExpr(castToRValue(Call->getParentExpr()));
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

   if (auto U = dyn_cast<UnionDecl>(R)) {
      Call->setReturnType(Context.getRecordType(U));

      auto &args = Call->getArgs();
      if (args.size() != 1) {
         diagnose(Call, args.empty() ? err_too_few_args_for_call
                                     : err_too_many_args_for_call,
                  1, args.size(), Call->getSourceLoc());

         return Call;
      }

      auto ty = Call->getArgs().front()->getExprType();

      QualType neededTy;
      for (auto &decl : U->getDecls()) {
         if (auto F = dyn_cast<FieldDecl>(decl)) {
            auto &fieldType = F->getType();
            if (implicitlyCastableTo(ty, fieldType)) {
               neededTy = fieldType.getResolvedType();
               break;
            }
         }
      }

      if (!neededTy) {
         diagnose(Call, err_union_initializer_type, Call->getSourceLoc());
         return Call;
      }

      implicitCastIfNecessary(Call->getArgs().front(), neededTy);

      Call->setKind(CallKind::UnionInitializer);
      Call->setUnion(U);

      return Call;
   }

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

   S = cast<StructDecl>(method->getRecord());

   if (auto Cl = dyn_cast<ClassDecl>(S))
      if (Cl->isAbstract())
         diagnose(Call, err_instantiate_abstract_class, Cl->getName());


   QualType RecordTy = Context.getRecordType(S);
   Call->setReturnType(RecordTy);
   Call->setKind(CallKind::InitializerCall);
   Call->setFunc(method);

   PrepareCallArgs(Call->getArgs(), method);
   ApplyCasts(Call->getArgs(), Call, CandSet);

   return Call;
}

ExprResult SemaPass::HandleCallOperator(CallExpr *Call, Expression *ParentExpr)
{
   auto ty = ParentExpr->getExprType();
   if (!ty->isRecordType())
      return HandleAnonCall(Call, ParentExpr);

   auto& givenArgs = Call->getArgs();
   auto DeclName = Context.getDeclNameTable()
                          .getPostfixOperatorName(Context.getIdentifiers()
                                                         .get("()"));

   auto CandSet = lookupMethod(DeclName, ParentExpr, givenArgs,
                               Call->getTemplateArgs(), Call);

   if (CandSet.isDependent()) {
      Call->setIsTypeDependent(true);
      Call->setExprType(UnknownAnyTy);

      return Call;
   }

   if (!CandSet)
      return ExprError();

   auto method = CandSet.getBestMatch().Func;
   checkAccessibility(method, Call);

   Call->setReturnType(method->getReturnType());
   Call->setKind(CallKind::CallOperator);
   Call->setFunc(method);

   PrepareCallArgs(Call->getArgs(), method);
   ApplyCasts(Call->getArgs(), Call, CandSet);

   if (!method->hasMutableSelf()) {
      Call->setParentExpr(castToRValue(ParentExpr));
   }

   return Call;
}

ExprResult SemaPass::HandleAnonCall(CallExpr *Call, Expression *ParentExpr)
{
   ParentExpr = castToRValue(ParentExpr);
   Call->setParentExpr(ParentExpr);

   auto ty = ParentExpr->getExprType();
   if (ty->isRecordType())
      return HandleCallOperator(Call, ParentExpr);

   if (ty->isMetaType()) {
      return HandleStaticTypeCall(
         Call, cast<cdot::MetaType>(ty)->getUnderlyingType());
   }

   if (!ty->isFunctionType()) {
      diagnose(Call, err_cannot_call_type, Call->getSourceLoc(), ty, false);
      return ExprError();
   }

   FunctionType *func = ty->asFunctionType();
   auto CandSet = checkAnonymousCall(func, Call->getArgs(), Call);

   if (CandSet.isDependent()) {
      Call->setIsTypeDependent(true);
      Call->setExprType(UnknownAnyTy);

      return Call;
   }

   if (!CandSet)
      return ExprError();

   ApplyCasts(Call->getArgs(), Call, CandSet);

   Call->setKind(CallKind::AnonymousCall);
   Call->setReturnType(func->getReturnType());
   Call->setFunctionType(func);

   return Call;
}

bool SemaPass::ExprCanReturn(Expression *E, QualType Ty)
{
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

} // namespace ast
} // namespace cdot