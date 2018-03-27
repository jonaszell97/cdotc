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
                          llvm::SmallPtrSetImpl<CallableDecl*> &Overloads) {
   auto lookupRes = Ctx->lookup(name);
   for (auto &ND : lookupRes) {
      if (auto C = dyn_cast<T>(ND)) {
         if (Overloads.insert(C).second)
            CandSet.addCandidate(C);
      }
   }

   if (auto Parent = Ctx->getParentCtx())
      addCandidates(Parent, name, CandSet, Overloads);
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

   if (!args.empty() && args.front()->getExprType()) {
      auto SelfExpr = args.front();
      RecordType *MaybeRecordTy = SelfExpr->getExprType()->asRecordType();
      if (!MaybeRecordTy && SelfExpr->isLValue())
         MaybeRecordTy = SelfExpr->getExprType()->asReferenceType()
                                 ->getReferencedType()->asRecordType();

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

   RecordType *MaybeRecordTy = SelfExpr->getExprType()->asRecordType();
   if (!MaybeRecordTy && SelfExpr->isLValue())
      MaybeRecordTy = SelfExpr->getExprType()->asReferenceType()
                              ->getReferencedType()->asRecordType();

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
      CandSet.diagnoseAnonymous(*this, DeclarationName(), args, Caller);

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
      if (suppressDiags)
         return;

      return CandSet.diagnose(*this, name, args, templateArgs, Caller);
   }

   maybeInstantiate(CandSet, Caller);
}

static MethodDecl *getEquivalentMethod(MethodDecl *Orig,
                                       RecordDecl *Inst) {
   for (auto &decl : Inst->getDecls())
      if (auto M = dyn_cast<MethodDecl>(decl))
         if (Orig->getBody() == M->getBodyTemplate())
            return M;

   llvm_unreachable("no equivalent method!");
}

static bool maybeInstantiateRecord(SemaPass &SP,
                                   CandidateSet::Candidate &Cand,
                                   Statement *Caller) {
   auto F = Cand.Func;

   if (auto Case = dyn_cast<EnumCaseDecl>(F)) {
      if (!Case->getRecord()->isTemplate())
         return false;

      auto Inst =
         SP.getInstantiator().InstantiateRecord(Caller, Case->getRecord(),
                                                move(Cand.TemplateArgs));

      Cand.Func = cast<EnumDecl>(Inst)
         ->lookupSingle<EnumCaseDecl>(Case->getIdentifierInfo());

      assert(Cand.Func && "did not instantiate enum correctly");
      return true;
   }

   auto Init = dyn_cast<InitDecl>(F);
   if (!Init || !Init->getRecord()->isTemplate())
      return false;

   auto Inst = SP.getInstantiator().InstantiateRecord(Caller,
                                                      Init->getRecord(),
                                                      move(Cand.TemplateArgs));

   Cand.Func = getEquivalentMethod(Init, Inst);
   SP.maybeInstantiateMemberFunction(cast<InitDecl>(Cand.Func), Caller);

   return true;
}

void SemaPass::maybeInstantiate(CandidateSet &CandSet,
                                Statement *Caller) {
   if (!CandSet)
      return;

   auto &Cand = CandSet.getBestMatch();
   if (!Cand.Func || Cand.TemplateArgs.isStillDependent())
      return;

   if (maybeInstantiateRecord(*this, Cand, Caller))
      return;

   auto F = Cand.Func;
   if (!F->isTemplate()) {
      if (auto M = dyn_cast<MethodDecl>(F)) {
         return maybeInstantiateMemberFunction(M, Caller);
      }

      return;
   }

   if (auto Fn = dyn_cast<FunctionDecl>(Cand.Func)) {
      Cand.Func = Instantiator.InstantiateFunction(Caller, Fn,
                                                   move(Cand.TemplateArgs));
   }
   else {
      Cand.Func = Instantiator.InstantiateMethod(Caller,
                                                 cast<MethodDecl>(Cand.Func),
                                                    move(Cand.TemplateArgs));
   }
}

void SemaPass::maybeInstantiateMemberFunction(MethodDecl *M,
                                              Statement *Caller) {
   if (M->getBody() || !M->getBodyTemplate())
      return;

   Instantiator.InstantiateMethodBody(Caller, M);
}

void SemaPass::ApplyCasts(std::vector<Expression* > &args,
                          CandidateSet &CandSet) {
   size_t i = 0;
   for (auto& arg : args) {
      if (i >= CandSet.Conversions.size())
         break;

      auto &ConvSeq = CandSet.Conversions[i];
      if (!ConvSeq.isNoOp()) {
         arg = ImplicitCastExpr::Create(Context, arg, move(ConvSeq));
      }

      ++i;
   }
}

void SemaPass::PrepareCallArgs(std::vector<Expression*>& args,
                               FunctionType *FuncTy) {
   size_t i = 0;
   auto declaredArgs = FuncTy->getParamTypes();

   for (auto& arg : args) {
      auto given = arg->getExprType();

      if (given->isDependentType()) {
         ++i;
         continue;
      }

      if (i >= declaredArgs.size()) {
         // cstyle varargs need an rvalue
         arg = castToRValue(arg);

         ++i;
         continue;
      }

      auto &declared = i >= declaredArgs.size() ? declaredArgs.back()
                                                : declaredArgs[i];

      if (!declared->isReferenceType())
         arg = castToRValue(arg);

      ++i;
   }
}

void SemaPass::PrepareCallArgs(std::vector<Expression*>& args,
                               CallableDecl *C) {
   auto &declaredArgs = C->getArgs();
   auto FuncTy = C->getFunctionType();

   PrepareCallArgs(args, FuncTy);

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
   if (auto Ident = dyn_cast<IdentifierRefExpr>(ParentExpr)) {
      if (Ident->getTemplateArgs().empty())
         return false;

      Call->setTemplateArgs(move(Ident->getTemplateArgRef()));
      Call->setIdent(Ident->getIdentInfo());
      Call->setParentExpr(nullptr);

      return true;
   }

   return false;
}

ExprResult SemaPass::visitCallExpr(CallExpr *Call)
{
   if (getTemplateParam(Call->getDeclName())) {
      Call->setIsTypeDependent(true);
      return ExprError();
   }

   auto &args = Call->getArgs();
   for (auto &arg : args) {
      if (!arg->isContextDependent()) {
         auto result = visitExpr(Call, arg);
         if (!result)
            return ExprError();

         arg = result.get();
      }
   }

   for (auto &TA : Call->getTemplateArgs()) {
      auto res = visitExpr(Call, TA);
      if (!res)
         return ExprError();

      TA = res.get();
   }

   // expression was previously resolved (for example by a binary operator node)
   if (Call->getKind() != CallKind::Unknown) {
      return Call;
   }

   if (Call->isNamedCall()) {
      auto Builtin = llvm::StringSwitch<BuiltinFn>(Call->getDeclName()
                                                       .getIdentifierInfo()
                                                       ->getIdentifier())
         .Case("sizeof", BuiltinFn::SIZEOF)
         .Case("alignof", BuiltinFn::ALIGNOF)
         .Case("typeof", BuiltinFn::TYPEOF)
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

   bool isBuiltin = Call->getBuiltinFnKind() != BuiltinFn::None;
   if (isBuiltin) {
      HandleBuiltinCall(Call);
   }
   else if (auto ParentExpr = Call->getParentExpr()) {
      if (checkTemplatedCall(*this, Call))
         return visitCallExpr(Call);

      auto ParentResult = visitExpr(Call, ParentExpr);
      if (!ParentResult)
         return ExprError();

      ParentExpr = ParentResult.get();
      Call->setParentExpr(ParentExpr);

      auto ty = ParentExpr->getExprType();
      if (ty->isUnknownAnyType())
         return ExprError();

      if (ParentExpr->getExprType()->isFunctionType()) {
         HandleAnonCall(Call, ParentExpr);
      }
      else if (!Call->getDeclName()) {
         if (ty->isMetaType()) {
            if (ty->isDependentType()) {
               Call->setIsTypeDependent(true);
               Call->setExprType(UnknownAnyTy);

               return Call;
            }
            else {
               HandleStaticTypeCall(Call, ty->uncheckedAsMetaType()
                                            ->getUnderlyingType());
            }
         }
         else {
            HandleCallOperator(Call, ParentExpr);
         }
      }
      else {
         auto Expr = HandleMethodCall(Call, ParentExpr);
         Call->setExprType(Call->getReturnType());

         return Expr;
      }
   }
   else {
      auto lookupResult = getDeclContext().lookup(Call->getDeclName());
      if (!lookupResult) {
         if (auto builtinTy = getBuiltinType(Call->getDeclName())) {
            HandleStaticTypeCall(Call, builtinTy);
         }
         else if (auto I = wouldBeValidIdentifier(Call->getIdentLoc(),
                                                  Call->getDeclName()
                                                      .getIdentifierInfo())) {
            Call->setIdentExpr(I);

            auto Ty = I->getExprType();
            if (Ty->isFunctionType()) {
               HandleAnonCall(Call, I);
            }
            else {
               HandleCallOperator(Call, I);
            }
         }
         else {
            diagnose(Call, err_func_not_found, 0, Call->getDeclName());
         }
      }
      else {
         auto ND = lookupResult.front();
         if (auto R = dyn_cast<RecordDecl>(ND)) {
            HandleConstructorCall(Call, R);
         }
         else if (auto M = dyn_cast<MethodDecl>(ND)) {
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

            HandleMethodCall(Call, Self);
         }
         else {
            HandleFunctionCall(Call, &getDeclContext(),
                               Call->getDeclName());
         }
      }
   }

   if (!Call->getExprType()) {
      Call->setHadError(true);
      return ExprError();
   }

   return Call;
}

void SemaPass::HandleFunctionCall(ast::CallExpr *node,
                                  DeclContext *Ctx,
                                  DeclarationName funcName) {
   auto CandSet = lookupFunction(Ctx, funcName, node->getArgs(),
                                 node->getTemplateArgs(), node);

   if (!CandSet)
      return;

   auto func = CandSet.getBestMatch().Func;

   PrepareCallArgs(node->getArgs(), func);
   ApplyCasts(node->getArgs(), CandSet);

   node->setKind(CallKind::NamedFunctionCall);
   node->setFunc(func);
   node->setReturnType(func->getReturnType());
}

void SemaPass::HandleBuiltinCall(CallExpr *node)
{
   if (node->getKind() == CallKind::VariadicSizeof) {
      node->setReturnType(getObjectTy("UInt"));

      auto &TA = node->getTemplateArgs().front();

      auto Ident = dyn_cast<IdentifierRefExpr>(TA);
      if (!Ident)
         return diagnose(node, err_variadic_sizeof_expects);

      auto Param = getTemplateParam(Ident->getIdentInfo());
      if (!Param)
         return diagnose(node, err_does_not_name_template_parm,
                         Ident->getIdent(), false);

      if (!Param->isVariadic())
         return diagnose(node, err_does_not_name_template_parm,
                         Ident->getIdent(), true);

      return;
   }

   HandleFunctionCall(node, &getDeclContext(), node->getDeclName());
   node->setKind(CallKind::Builtin);

   if (node->hadError())
      return;

   if (node->getBuiltinFnKind() == BuiltinFn::SIZEOF) {
      auto Ty = node->getTemplateArgs().front()->getExprType();
      if (Ty->isMetaType())
         Ty = Ty->uncheckedAsMetaType()->getUnderlyingType();

      if (Ty->isRecordType()) {
         ensureSizeKnown(Ty->getRecord(), node->getSourceLoc());
      }
   }
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
   DeclContext *Ctx = nullptr;
   QualType ty = ParentExpr->getExprType();

   if (ty->isPointerType()) {
      if (!Call->isPointerAccess()) {
         diagnose(Call, err_access_member_on_pointer);
      }

      ty = ty->getPointeeType();
   }
   else if (Call->isPointerAccess()) {
      diagnose(Call, err_member_access_non_pointer, ty);
   }

   if (NamespaceType *NS = ty->asNamespaceType()) {
      Ctx = NS->getNamespace();
   }
   else if (cdot::MetaType *Meta = ty->asMetaType()) {
      auto underlying = Meta->getUnderlyingType();

      // static type call, e.g.
      //  let i = i64(291)
      //          ^
      if (!underlying->isRecordType()) {
         HandleStaticTypeCall(Call, *underlying);
         return Call;
      }

      // enum case, e.g.
      //  let opt = Option[i64].Some(29)
      //                        ^
      if (underlying->isEnum()) {
         auto E = cast<EnumDecl>(underlying->getRecord());
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

      ty = underlying;
   }
   else if (ty->isRecordType()) {
      Ctx = ty->getRecord();
   }

   CandidateSet CandSet = lookupMethod(Call->getDeclName(),
                                       ParentExpr, Call->getArgs(),
                                       Call->getTemplateArgs(), Call);

   if (CandSet.Candidates.empty() && Call->getDeclName().isSimpleIdentifier()) {
      auto Ident = new(Context) IdentifierRefExpr(Call->getSourceLoc(),
                                                  Call->getDeclName()
                                                      .getIdentifierInfo(),
                                                  move(Call->getTemplateArgs()),
                                                  Ctx);

      Call->setIdent(nullptr);
      Ident->setPointerAccess(Call->isPointerAccess());

      DiagnosticScopeRAII DiagScope(*this);
      visitIdentifierRefExpr(Ident);

      if (!Ident->hadError())
         return Ident;

      return ExprError();
   }
   else if (!CandSet) {
      return ExprError();
   }

   auto &BestMatch = CandSet.getBestMatch();
   auto func = CandSet.getBestMatch().Func;

   // check UFCS, transform
   //  `3.method()` into
   //  `method(3)`
   // if necessary
   if (!func || !isa<MethodDecl>(func)) {
      Call->getArgs().insert(Call->getArgs().begin(), ParentExpr);
      Call->setParentExpr(Call->getParentExpr()->maybeGetParentExpr());
   }

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
   ApplyCasts(Call->getArgs(), CandSet);

   if (!func->hasMutableSelf() && Call->getParentExpr()) {
      Call->setParentExpr(castToRValue(Call->getParentExpr()));
   }

   return Call;
}

void SemaPass::HandleStaticTypeCall(CallExpr *node, Type *Ty)
{
   bool isValid = Ty->isVoidType() || Ty->isIntegerType() || Ty->isFPType();
   if (isValid) {
      unsigned neededArgs = Ty->isVoidType() ? 0 : 1;
      auto& args = node->getArgs();
      if (args.size() != neededArgs) {
         auto diag = args.size() < neededArgs
                     ? err_too_few_args_for_call
                     : err_too_many_args_for_call;

         diagnose(node, diag, neededArgs, args.size());

         node->setHadError(true);
         return;
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
            diagnose(node, err_must_be_integral_constant, 1 /*st argument*/,
                     1 /*primitive initializer*/, args.front()->getSourceLoc());

            return;
         }
      }
      else if (!Ty->isVoidType()) {
         assert(Ty->isFPType());
         auto lit = dyn_cast<FPLiteral>(args.front());
         if (!lit) {
            diagnose(node, err_must_be_floating_constant, 1 /*st argument*/,
                     1 /*primitive initializer*/, args.front()->getSourceLoc());

            return;
         }

         lit->setContextualType(Ty);
         lit->setType(Ty);

         visitFPLiteral(lit);
      }

      node->setKind(CallKind::PrimitiveInitializer);
      node->setReturnType(Ty);

      return;
   }
   else if (Ty->isVoidType()) {
      auto& args = node->getArgs();
      if (!args.empty())
         return diagnose(node, err_too_many_args_for_call, 0, args.size());

      node->setKind(CallKind::PrimitiveInitializer);
      node->setReturnType(Ty);

      return;
   }
   else if (Ty->isRecordType()) {
      return HandleConstructorCall(node, Ty->getRecord());
   }

   return diagnose(node, err_cannot_call_type, Ty, true);
}

void SemaPass::HandleConstructorCall(CallExpr *node, RecordDecl *record)
{
   assert(record && "should not be called otherwise");

   if (!record->isTemplate() && !node->getTemplateArgs().empty()) {
      diagnose(node, err_generic_type_count, 0, node->getTemplateArgs().size(),
               node->getDeclName());

      node->getTemplateArgs().clear();
   }

   if (record->isUnion()) {
      auto &args = node->getArgs();
      if (args.size() != 1)
         return diagnose(node, args.empty() ? err_too_few_args_for_call
                                            : err_too_many_args_for_call,
                         1, args.size());

      auto ty = node->getArgs().front()->getExprType();
      auto U = cast<UnionDecl>(record);

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
         diagnose(node, err_union_initializer_type);
         return;
      }

      castToRValue(node->getArgs().front());
      implicitCastIfNecessary(node->getArgs().front(), neededTy);

      node->setReturnType(Context.getRecordType(U));
      node->setKind(CallKind::UnionInitializer);
      node->setUnion(U);

      return;
   }

   if (isa<EnumDecl>(record))
      return diagnose(node, err_enum_initializer);

   if (isa<ProtocolDecl>(record))
      return diagnose(node, err_protocol_initializer);

   auto rec = cast<StructDecl>(record);
   checkClassAccessibility(rec, node);

   auto& givenArgs = node->getArgs();
   auto DeclName = Context.getDeclNameTable()
                          .getConstructorName(Context.getRecordType(rec));

   auto CandSet = lookupFunction(rec, DeclName, givenArgs,
                                 node->getTemplateArgs(), node);

   if (!CandSet) {
      node->setHadError(true);

      if (!rec->isTemplate())
         node->setReturnType(Context.getRecordType(rec));

      return;
   }

   auto method = cast<MethodDecl>(CandSet.getBestMatch().Func);
   rec = cast<StructDecl>(method->getRecord());

   // check accessibility
//   checkMemberAccessibility(rec, method->getName(), method->getAccess(),
//                            node);

   if (auto Cl = dyn_cast<ClassDecl>(rec)) {
      if (Cl->isAbstract())
         diagnose(node, err_instantiate_abstract_class, Cl->getName());
   }

   node->setKind(CallKind::InitializerCall);

   QualType RecordTy = Context.getRecordType(rec);

   node->setReturnType(RecordTy);
   node->setFunc(method);

   PrepareCallArgs(node->getArgs(), method);
   ApplyCasts(node->getArgs(), CandSet);
}

void SemaPass::HandleCallOperator(CallExpr *Call, Expression *ParentExpr)
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

   if (!CandSet)
      return;

   auto method = CandSet.getBestMatch().Func;

   Call->setKind(CallKind::CallOperator);
   Call->setFunc(method);

   PrepareCallArgs(Call->getArgs(), method);
   ApplyCasts(Call->getArgs(), CandSet);

   Call->setReturnType(method->getReturnType());

   if (!method->hasMutableSelf()) {
      Call->setParentExpr(castToRValue(ParentExpr));
   }
}

void SemaPass::HandleAnonCall(CallExpr *Call, Expression *ParentExpr)
{
   ParentExpr = castToRValue(ParentExpr);
   Call->setParentExpr(ParentExpr);

   auto ty = ParentExpr->getExprType();
   if (ty->isRecordType())
      return HandleCallOperator(Call, ParentExpr);

   if (!ty->isFunctionType())
      return diagnose(Call, err_cannot_call_type, ty, false);

   FunctionType *func = ty->asFunctionType();
   auto CandSet = checkAnonymousCall(func, Call->getArgs(), Call);

   if (!CandSet)
      return;

   PrepareCallArgs(Call->getArgs(), func);
   ApplyCasts(Call->getArgs(), CandSet);

   Call->setKind(CallKind::AnonymousCall);
   Call->setReturnType(func->getReturnType());
   Call->setFunctionType(func);
}

} // namespace ast
} // namespace cdot