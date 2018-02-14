//
// Created by Jonas Zell on 24.10.17.
//

#include "SemaPass.h"

#include "AST/ASTContext.h"
#include "AST/Transform.h"

#include "AST/Expression/RefExpr/CallExpr.h"
#include "AST/Expression/RefExpr/MemberRefExpr.h"
#include "AST/Expression/RefExpr/IdentifierRefExpr.h"

#include "AST/Expression/Literal/IntegerLiteral.h"
#include "AST/Expression/Literal/StringLiteral.h"

#include "AST/Expression/TypeRef.h"
#include "AST/Expression/StaticExpr.h"

#include "AST/Operator/BinaryOperator.h"
#include "AST/Operator/ExprSequence.h"

#include "AST/Statement/Declaration/LocalVarDecl.h"
#include "AST/Statement/Declaration/CallableDecl.h"
#include "AST/Statement/Declaration/LocalVarDecl.h"
#include "AST/Statement/Declaration/TypedefDecl.h"
#include "AST/Statement/Declaration/NamespaceDecl.h"
#include "AST/Statement/Declaration/Class/EnumCaseDecl.h"
#include "AST/Statement/Declaration/Class/FieldDecl.h"
#include "AST/Statement/Declaration/Class/RecordDecl.h"
#include "AST/Statement/Declaration/Class/MethodDecl.h"
#include "AST/Statement/Block/CompoundStmt.h"

#include "AST/Passes/Declaration/DeclPass.h"

#include "AST/Passes/SemanticAnalysis/OverloadResolver.h"
#include "AST/Passes/SemanticAnalysis/Builtin.h"
#include "AST/Passes/SemanticAnalysis/TemplateInstantiator.h"

#include "Util.h"

#include "Variant/Type/Type.h"
#include "Message/Diagnostics.h"

#include <llvm/ADT/SmallString.h>

using namespace cdot::diag;
using namespace cdot::support;

namespace cdot {
namespace ast {

SemaPass::CallResult::~CallResult()
{
//   if (isCompatible())
//      static_cast<CompatibleCallResult*>(this)->destroyValue();
//   else
//      static_cast<IncompatibleCallResult*>(this)->destroyValue();
}

FunctionDecl* SemaPass::CallResult::getFunction() const
{
   assert(isCompatible());
   return static_cast<CompatibleCallResult const*>(this)->F;
}

MethodDecl* SemaPass::CallResult::getMethod() const
{
   assert(isCompatible());
   return static_cast<CompatibleCallResult const*>(this)->M;
}

CallableDecl* SemaPass::CallResult::getCallable() const
{
   assert(isCompatible());
   return static_cast<CompatibleCallResult const*>(this)->C;
}

EnumCaseDecl* SemaPass::CallResult::getCase() const
{
   assert(isCompatible());
   return static_cast<CompatibleCallResult const*>(this)->Case;
}

sema::TemplateArgList& SemaPass::CallResult::getTemplateArgs()
{
   assert(isCompatible());
   return static_cast<CompatibleCallResult*>(this)->templateArgs;
}

FunctionType* SemaPass::CallResult::getFunctionType() const
{
   assert(isCompatible());
   return static_cast<CompatibleCallResult const*>(this)->functionType;
}

void SemaPass::CompatibleCallResult::destroyValue()
{
   templateArgs.~TemplateArgList();
}

std::vector<SemaPass::CallCandidate>&
SemaPass::CallResult::getFailedCandidates()
{
   assert(!isCompatible());
   return static_cast<IncompatibleCallResult*>(this)->failedCandidates;
}

void SemaPass::IncompatibleCallResult::destroyValue()
{
   failedCandidates.~vector();
}

static bool tryStringifyConstraint(llvm::SmallString<128> &Str,
                                   Expression *Expr) {
   if (auto BinOp = dyn_cast<BinaryOperator>(Expr)) {
      auto lhsValid = tryStringifyConstraint(Str, BinOp->getLhs());
      if (!lhsValid)
         return false;

      auto &op = BinOp->getOp();
      if (op == "==" || op == "===") {
         Str += " must equal ";
      }
      else if (op == "!=" || op == "!==") {
         Str += " must not equal ";
      }
      else if (op == "<") {
         Str += " must be lower than ";
      }
      else if (op == ">") {
         Str += " must be greater than ";
      }
      else if (op == "<=") {
         Str += " must be lower than or equal to ";
      }
      else if (op == ">=") {
         Str += " must be greater than or equal to ";
      }
      else if (op == ":") {
         switch (cast<ConstraintExpr>(BinOp->getRhs())->getKind()) {
            case ConstraintExpr::Struct: Str += " be a struct"; break;
            case ConstraintExpr::Class: Str += " be a class"; break;
            case ConstraintExpr::Enum: Str += " be an enum"; break;
            case ConstraintExpr::Union: Str += " be a union"; break;
            case ConstraintExpr::Function: Str += " be a function"; break;
            case ConstraintExpr::DefaultConstructible:
               Str += " be default constructible";
               break;
            case ConstraintExpr::Pointer: Str += " be a pointer"; break;
            case ConstraintExpr::Reference: Str += " be a reference"; break;
            case ConstraintExpr::Type: {
               auto ty = cast<ConstraintExpr>(BinOp->getRhs())
                  ->getTypeConstraint()->getType();

               if (ty->isProtocol()) {
                  Str += " conform to ";
                  Str += ty->getRecord()->getName();
               }
               else if (ty->isClass()) {
                  Str += " be a subclass of or equal to ";
                  Str += ty->getRecord()->getName();
               }
               else {
                  Str += " be equal to ";
                  Str += ty->getRecord()->getName();
               }

               break;
            }

            return true;
         }
      }

      auto rhsValid = tryStringifyConstraint(Str, BinOp->getRhs());
      return rhsValid;
   }
   else if (auto Seq = dyn_cast<ExprSequence>(Expr)) {
      return tryStringifyConstraint(Str, Seq->getResolvedExpression());
   }
   else if (auto ID = dyn_cast<IdentifierRefExpr>(Expr)) {
      Str += ID->getIdent();
      return true;
   }
   else if (auto M = dyn_cast<MemberRefExpr>(Expr)) {
      Str += M->getIdent();
      return true;
   }
   else if (auto I = dyn_cast<IntegerLiteral>(Expr)) {
      if (I->getType()->isInt1OrBool()) {
         Str += (I->getValue().getBoolValue() ? "true" : "false");
      }
      else if (I->getType()->isInt8OrChar()) {
         Str += "'";
         Str += (char)I->getValue().getZExtValue();
         Str += "'";
      }
      else {
         I->getValue().toString(Str, 10, !I->getType()->isUnsigned());
      }

      return true;
   }
   else if (auto FP = dyn_cast<FPLiteral>(Expr)) {
      FP->getValue().toString(Str);
      return true;
   }
   else if (auto S = dyn_cast<StringLiteral>(Expr)) {
      Str += '"';
      Str += S->getValue();
      Str += '"';

      return true;
   }

   return false;
}

SemaPass::CallCandidate::CallCandidate(CallCompatability &Comp,
                                       llvm::ArrayRef<Expression*> resolvedArgs,
                                       CallableDecl *C)
   : loc(C->getSourceLoc())
{
   auto reason = Comp.getFailureReason();
   if (reason == FailureReason::IncompatibleArgCount) {
      diagnostic = diag::note(note_cand_mismatched_arg_count)
         << Comp.FuncTy->getArgTypes().size()
         << resolvedArgs.size()
         << C->getSourceLoc();
   }
   else if (reason == FailureReason::IncompatibleArgument) {
      auto &neededArg = Comp.FuncTy->getArgTypes()[Comp.incompatibleArg];
      diagnostic = diag::note(note_cand_no_implicit_conv)
         << resolvedArgs[Comp.incompatibleArg]->getExprType()
         << neededArg
         << Comp.incompatibleArg + 1 << C->getSourceLoc();
   }
   else if (reason == FailureReason::CouldNotInferTemplateArg) {
      diagnostic = Comp.templateArgList.getDiagnostics().empty()
                ? Comp.initializerTemplateArgList.getDiagnostics().front()
                : Comp.templateArgList.getDiagnostics().front();
   }
   else if (reason == FailureReason::IncompatibleTemplateArg) {
      diagnostic = Comp.templateArgList.getDiagnostics().empty()
                   ? Comp.initializerTemplateArgList.getDiagnostics().front()
                   : Comp.templateArgList.getDiagnostics().front();
   }
   else if (reason == FailureReason::FailedConstraint) {
      diagnostic = diag::note(note_generic_note);

      llvm::SmallString<128> str("candidate function not viable: ");
      if (tryStringifyConstraint(str, Comp.failedConstraint->getExpr())) {
         diagnostic << str;
      }
      else {
         diagnostic << "candidate function not viable: failed constraint";
      }

      diagnostic << Comp.failedConstraint->getSourceLoc();
   }
   else {
      llvm_unreachable("bad reason");
   }

   if (auto M = dyn_cast<MethodDecl>(C))
      if (M->isMemberwiseInitializer())
         diagnostic << diag::memberwise_init;
}

SemaPass::CallCandidate::CallCandidate(CallCompatability &Comp,
                                       llvm::ArrayRef<Expression*> resolvedArgs,
                                       EnumCaseDecl *Case)
   : loc(Case->getSourceLoc())
{
   auto reason = Comp.getFailureReason();
   if (reason == FailureReason::IncompatibleArgCount) {
      diagnostic = diag::note(note_cand_mismatched_arg_count)
         << Comp.FuncTy->getArgTypes().size()
         << resolvedArgs.size()
         << Case->getSourceLoc();
   }
   else if (reason == FailureReason::IncompatibleArgument) {
      auto &neededArg = Case->getArgs()[Comp.incompatibleArg];
      diagnostic = diag::note(note_cand_no_implicit_conv)
         << resolvedArgs[Comp.incompatibleArg]->getExprType()
         << neededArg->getArgType()->getType()
         << Comp.incompatibleArg + 1 << Case->getSourceLoc();
   }
   else {
      llvm_unreachable("bad reason");
   }
}

SemaPass::CallCandidate::CallCandidate(CallCompatability &Comp,
                                       llvm::ArrayRef<Expression*> resolvedArgs,
                                       FunctionType *FTy,
                                       SourceLocation declLoc)
   : loc(declLoc)
{
   auto reason = Comp.getFailureReason();
   if (reason == FailureReason::IncompatibleArgCount) {
      diagnostic = diag::note(note_cand_mismatched_arg_count)
         << Comp.FuncTy->getArgTypes().size()
         << resolvedArgs.size()
         << declLoc;
   }
   else if (reason == FailureReason::IncompatibleArgument) {
      auto &neededArg = FTy->getArgTypes()[Comp.incompatibleArg];
      diagnostic = diag::note(note_cand_no_implicit_conv)
         << resolvedArgs[Comp.incompatibleArg]->getExprType()
         << neededArg
         << Comp.incompatibleArg + 1 << declLoc;
   }
   else {
      llvm_unreachable("bad reason");
   }
}

void
SemaPass::diagnoseAmbiguousCall(llvm::ArrayRef<CallableDecl*> perfectMatches,
                                Statement *Caller) {
   if (perfectMatches.size() <= 1)
      return;

   auto &first = perfectMatches.front();
   diagnose(Caller, err_ambiguous_call, isa<MethodDecl>(first),
            first->getName());

   for (size_t i = 0; i < perfectMatches.size(); ++i) {
      note(note_ambiguous_call)
         << perfectMatches[i]->getSourceLoc();
   }
}

SemaPass::ConstraintResult SemaPass::checkConstraint(
                                 StaticExpr* const &constraint,
                                 TemplateArgList &TAs) {
   llvm_unreachable("fuck it");
}

SemaPass::FunctionResult
SemaPass::getUFCS(llvm::StringRef funcName,
                  llvm::ArrayRef<Expression*> args,
                  const std::vector<TemplateArgExpr*> &templateArgs,
                  Statement *caller) {
   if (!args.empty() && args.front()->getExprType()->isObjectType()) {
      auto result = getMethod(args.front()->getExprType()->getRecord(),
                              funcName, { args.data() + 1, args.size() - 1 },
                              templateArgs, caller);

      if (result->isCompatible())
         return result;
   }

   return getFunction(funcName, args, templateArgs, caller);
}

SemaPass::FunctionResult
SemaPass::getFunction(llvm::StringRef funcName,
                      llvm::ArrayRef<Expression*> args,
                      std::vector<TemplateArgExpr*> const& templateArgs,
                      Statement *caller) {
   llvm::SmallVector<FunctionDecl*, 8> overloads;
   auto lookupResult = declPass->getDeclContext().lookup(funcName);

   for (auto decl : lookupResult)
      if (auto C = dyn_cast<FunctionDecl>(decl))
         overloads.push_back(C);

   return getFunction(overloads, args, templateArgs, caller);
}

bool SemaPass::checkFunctionCompatibility(
                           NamedDecl *Decl,
                           OverloadResolver &Resolver,
                           std::unique_ptr<CompatibleCallResult> &Res,
                           llvm::SmallVectorImpl<CallableDecl *> &perfectCalls,
                           std::vector<CallCandidate> &failedCandidates,
                           size_t &bestMatch,
                           size_t &maxSatisfiedConstraints) {
   CallableDecl *C = dyn_cast<CallableDecl>(Decl);
   auto res = Resolver.checkIfViable(C);
   if (res.isCompatible()) {
      size_t numConstraints = C->getConstraints().size();

      if (res.perfectMatch && numConstraints <= maxSatisfiedConstraints) {
         perfectCalls.push_back(C);
      }

      bool satisifiesMoreConstraints =
         numConstraints > maxSatisfiedConstraints;

      if (res.perfectMatch || satisifiesMoreConstraints
          || res.castPenalty < bestMatch) {
         Res = std::make_unique<CompatibleCallResult>(
            C, std::move(res.templateArgList));

         bestMatch = res.getCastPenalty();
         maxSatisfiedConstraints = numConstraints;

         return true;
      }
   }
   else if (!Res) {
      failedCandidates.emplace_back(res, Resolver.getGivenArgs(), C);
   }

   return false;
}

SemaPass::FunctionResult
SemaPass::getFunction(llvm::ArrayRef<FunctionDecl*> overloads,
                      llvm::ArrayRef<Expression*> args,
                      const std::vector<ast::TemplateArgExpr *> &templateArgs,
                      Statement *caller) {
   std::vector<CallCandidate> failedCandidates;
   if (overloads.empty())
      return std::make_unique<IncompatibleCallResult>(move(failedCandidates));

   OverloadResolver Resolver(*this, args, templateArgs, caller);
   llvm::SmallVector<CallableDecl*, 4> perfectCalls;
   std::unique_ptr<CompatibleCallResult> Res = nullptr;

   size_t bestMatch = size_t(-1);
   size_t maxSatisfiedConstraints = 0;

   for (const auto &overload : overloads) {
      checkFunctionCompatibility(overload, Resolver, Res, perfectCalls,
                                 failedCandidates, bestMatch,
                                 maxSatisfiedConstraints);
   }

   if (!Res)
      return std::make_unique<IncompatibleCallResult>(move(failedCandidates));

   diagnoseAmbiguousCall(perfectCalls, caller);

   if (!Res->getTemplateArgs().empty()) {
      bool isNew = false;
      auto newFunc = TemplateInstantiator::InstantiateFunction(
         *this, caller->getSourceLoc(), Res->getFunction(),
         std::move(Res->getTemplateArgs()), &isNew);

      if (isNew) {
         checkDeclTypeReturnType(newFunc);
      }

      Res->setFunction(newFunc);
   }

   return std::move(Res);
}

static MethodDecl *getEquivalentMethod(MethodDecl *Orig,
                                       RecordDecl *Inst) {
   for (auto &decl : Inst->getDecls())
      if (auto M = dyn_cast<MethodDecl>(decl))
         if (Orig == M->getSpecializedTemplate())
            return M;

   llvm_unreachable("no equivalent method!");
}

SemaPass::FunctionResult
SemaPass::getMethod(RecordDecl *R,
                    llvm::StringRef methodName,
                    llvm::ArrayRef<Expression*> args,
                    std::vector<TemplateArgExpr*> const& templateArgs,
                    Statement *caller,
                    bool isStatic) {
   llvm::SmallVector<MethodDecl*, 8> overloads;

   while (R) {
      auto lookupResult = R->lookupOwn(methodName);
      for (auto decl : lookupResult) {
         if (auto M = dyn_cast<MethodDecl>(decl))
            overloads.push_back(M);
      }

      if (auto C = dyn_cast<ClassDecl>(R)) {
         R = C->getParentClass();
      }
      else
         break;
   }

   return getMethod(R, overloads, args, templateArgs, caller, isStatic);
}

SemaPass::FunctionResult
SemaPass::getMethod(RecordDecl *R,
                    LookupResult &lookupResult,
                    llvm::ArrayRef<Expression *> args,
                    const std::vector<TemplateArgExpr *> &templateArgs,
                    Statement *caller,
                    bool isStatic) {
   llvm::SmallVector<MethodDecl*, 8> overloads;
   for (auto decl : lookupResult) {
      if (auto M = dyn_cast<MethodDecl>(decl))
         overloads.push_back(M);
   }

   return getMethod(R, overloads, args, templateArgs, caller, isStatic);
}

SemaPass::FunctionResult
SemaPass::getMethod(RecordDecl *rec,
                    llvm::ArrayRef<MethodDecl*> overloads,
                    llvm::ArrayRef<Expression*> args,
                    const std::vector<TemplateArgExpr *> &templateArgs,
                    Statement *caller,
                    bool isStatic) {
   std::vector<CallCandidate> failedCandidates;
   if (overloads.empty())
      return std::make_unique<IncompatibleCallResult>(move(failedCandidates));

   OverloadResolver Resolver(*this, args, templateArgs, caller);

   llvm::SmallVector<CallableDecl*, 4> perfectCalls;
   std::unique_ptr<CompatibleCallResult> Res = nullptr;

   size_t bestMatch(-1);
   size_t maxSatisfiedConstraints = 0;

   for (auto &overload : overloads) {
      if (isStatic && !overload->isStatic())
         continue;

      checkFunctionCompatibility(overload, Resolver, Res, perfectCalls,
                                 failedCandidates, bestMatch,
                                 maxSatisfiedConstraints);
   }

   diagnoseAmbiguousCall(perfectCalls, caller);

   if (!Res)
      return std::make_unique<IncompatibleCallResult>(move(failedCandidates));

   auto method = Res->getMethod();
   auto R = method->getRecord();
   auto &argList = Res->getTemplateArgs();

   if (isa<InitDecl>(method) && R->isTemplate()) {
      auto initializerArgs = argList.moveInitializerArgs();
      auto offset = method->getTemplateParams().size()
                    - R->getTemplateParams().size();

      initializerArgs.setParameterOffsetBegin(offset);
      argList.setParameterOffsetEnd(offset);

      auto Inst = InstantiateRecord(rec, std::move(argList),
                                    caller->getSourceLoc());

      method = getEquivalentMethod(Res->getMethod(), Inst);
      Res->setMethod(method);

      argList = std::move(initializerArgs);
   }

   maybeInstantiateMemberFunction(Res->getMethod(), caller->getSourceLoc());

   bool needsInstantiation =
      (!isa<InitDecl>(method) || method->isTemplatedInitializer())
      && !argList.empty()
      && method->isTemplate();

   if (needsInstantiation) {
      bool isNew = false;
      auto newMethod = TemplateInstantiator::InstantiateMethod(
         *this, caller->getSourceLoc(), method,
         std::move(argList), &isNew);

      if (isNew)
         checkDeclTypeReturnType(newMethod);

      Res->setMethod(newMethod);
   }

   return std::move(Res);
}

void SemaPass::maybeInstantiateMemberFunction(MethodDecl *M,
                                              const SourceLocation &callerLoc) {
   if (M->getBody() || !M->isInstantiation())
      return;

   if (!M->getSpecializedTemplate()->getBody())
      return;

   auto instantiatedBody =
      TemplateInstantiator::InstantiateMethodBody(
         *this, callerLoc, cast<MethodDecl>(M->getSpecializedTemplate()), M);

   M->setBody(cast<CompoundStmt>(instantiatedBody));

   visitFunctionInstantiation(M);
}

SemaPass::FunctionResult SemaPass::getCase(EnumDecl *E,
                                           const string &caseName,
                                           llvm::ArrayRef<Expression*> args,
                                           Statement *Caller) {
   auto Case = E->hasCase(caseName);
   if (!Case)
      return std::make_unique<IncompatibleCallResult>();

   auto &types = Case->getArgs();
   if (types.size() != args.size())
      return std::make_unique<IncompatibleCallResult>();

   OverloadResolver Resolver(*this, args, {}, Caller);
   auto res = Resolver.checkIfViable(Case);

   if (!res.isCompatible()) {
      std::vector<CallCandidate> candidates;
      candidates.emplace_back(res, args, Case);

      return std::make_unique<IncompatibleCallResult>(move(candidates));
   }

   return std::make_unique<CompatibleCallResult>(Case);
}

SemaPass::FunctionResult
SemaPass::checkAnonymousCall(FunctionType *FTy,
                             llvm::ArrayRef<Expression*> args,
                             Statement *Caller) {
   OverloadResolver Resolver(*this, args, {}, Caller);
   auto res = Resolver.checkIfViable(FTy);

   if (!res.isCompatible()) {
      std::vector<CallCandidate> candidates;
      candidates.emplace_back(res, args, FTy, Caller->getSourceLoc());

      return std::make_unique<IncompatibleCallResult>(move(candidates));
   }

   return std::make_unique<CompatibleCallResult>(FTy);
}

void SemaPass::ApplyCasts(std::vector<Expression* > &args,
                          FunctionType *FuncTy) {
   size_t i = 0;
   auto declaredArgs = FuncTy->getArgTypes();

   for (auto& arg : args) {
      if (i >= declaredArgs.size()) {
         break;
      }

      assert(args.size() > i && "No arg to cast");
      implicitCastIfNecessary(arg, arg->getExprType(), declaredArgs[i]);

      ++i;
   }
}

void SemaPass::PrepareCallArgs(std::vector<Expression*>& args,
                               FunctionType *FuncTy) {
   size_t i = 0;
   bool vararg = FuncTy->isCStyleVararg();
   auto declaredArgs = FuncTy->getArgTypes();

   for (auto& arg : args) {
      auto given = arg->getExprType();
      if (i >= declaredArgs.size() || (vararg && i >= declaredArgs.size() - 1)){
         // assume that c style varargs need a primitive...
         if (given->isBoxedPrimitive()) {
            QualType unboxed = getUnboxedType(given);
            forceCast(arg, given, unboxed);

            given = unboxed;
         }

         // and an rvalue
         toRvalueIfNecessary(given, arg);

         ++i;
         continue;
      }

      auto &declared = i >= declaredArgs.size() ? declaredArgs.back()
                                                : declaredArgs[i];

      toRvalueIfNecessary(given, arg, !declared.isLvalue());
      ++i;
   }

   if (!vararg && args.size() < declaredArgs.size()) {
      llvm_unreachable("no default args on anonymous calls!");
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

QualType SemaPass::visitCallExpr(CallExpr *node, QualType ty)
{
   if (auto Param = hasTemplateParam(node->getIdent())) {
      if (!Param->isTypeName())
         diagnose(node, warn_generic_warn,
                  "calling non-type template parameter is never valid");

      node->setIsTypeDependent(true);
      return {};
   }

   auto &args = node->getArgs();
   for (auto &arg : args) {
      if (!arg->isContextDependent()) {
         auto result = visitExpr(node, arg);
         if (result.hadError()) {
            return {};
         }
      }
   }

   for (auto &TA : node->getTemplateArgs()) {
      if (TA->isTypeName()) {
         auto res = visitExpr(node, TA->getType());
         if (res.hadError()) {
            return { };
         }
      }
   }

   // expression was previously resolved (for example by a binary operator node)
   if (node->getKind() != CallKind::Unknown) {
      return node->getReturnType();
   }

   bool isBuiltin = node->getBuiltinFnKind() != BuiltinFn::None;
   if (isBuiltin) {
      HandleBuiltinCall(node);
   }
   else if (ty) {
      if (ty->isFunctionType()) {
         HandleAnonCall(node, ty);
      }
      else if (node->getIdent().empty()) {
         if (ty->isMetaType()) {
            HandleStaticTypeCall(node, ty->asMetaType()->getUnderlyingType());
         }
         else {
            HandleCallOperator(node, ty);
         }
      }
      else {
         HandleMethodCall(node, ty);
      }
   }
   else {
      auto lookupResult = declPass->getDeclContext().lookup(node->getIdent());
      if (!lookupResult) {
         if (auto Rec = getCurrentRecord()) {
            if (Rec->hasMethodWithName(node->getIdent())) {
               if (Rec->isTemplate()) {
                  node->setIsTypeDependent(true);
                  return {};
               }

               node->setImplicitSelf(true);
               HandleMethodCall(node, Context.getRecordType(Rec));
            }
         }
         else if (auto ty = getBuiltinType(node->getIdent())) {
            HandleStaticTypeCall(node, ty);
         }
         else if (auto I = wouldBeValidIdentifier(node->getIdent())) {
            node->setIdentExpr(I);

            auto expr = node->getIdentExpr();
            auto Ty = visitIdentifierRefExpr(expr);

            if (Ty->isFunctionType()) {
               HandleAnonCall(node, Ty);
            }
            else {
               HandleCallOperator(node, Ty);
            }
         }
         else {
            diagnose(node, err_func_not_found, 0, node->getIdent());
         }
      }
      else if (lookupResult.size() == 1) {
         if (auto R = dyn_cast<RecordDecl>(lookupResult.front())) {
            HandleConstructorCall(node, R);
         }
         else {
            llvm::SmallVector<FunctionDecl *, 8> Fns;
            for (auto decl : lookupResult) {
               if (auto Fn = dyn_cast<FunctionDecl>(decl))
                  Fns.push_back(Fn);
            }

            HandleFunctionCall(node, Fns);
         }
      }
   }

   if (node->hadError())
      return {};

   return node->getReturnType();
}

void SemaPass::HandleFunctionCall(CallExpr *node,
                                  llvm::ArrayRef<FunctionDecl*> overloads) {
   auto result = getFunction(overloads, node->getArgs(),
                             node->getTemplateArgs(), node);

   if (!result->isCompatible()) {
      return diagnoseNoMatchingCall(result.get(), NoMatchingCallKind::Function,
                                    node->getIdent(), node);
   }

   auto func = result->getFunction();

   PrepareCallArgs(node->getArgs(), func);
   ApplyCasts(node->getArgs(), func->getFunctionType());

   node->setKind(CallKind::NamedFunctionCall);
   node->setFunc(func);
   node->setReturnType(func->getReturnType()->getType());
}

bool SemaPass::TryTypedef(CallExpr *node, TypedefDecl *td)
{
   if (!td)
      td = getTypedef(node->getIdent());

   if (td) {
      if (td->getOriginTy()->getType()->isPrimitiveType()) {
         auto& args = node->getArgs();
         if (args.size() != 1) {
            diagnose(node, err_generic_error,
                     "primitive initializer expects one argument");

            node->setHadError(true);
            return true;
         }

         auto given = args.front()->getExprType();
         implicitCastIfNecessary(node->getArgs().front(),
                                 given, td->getOriginTy()->getType());

         node->setKind(CallKind::PrimitiveInitializer);
         node->setReturnType(td->getOriginTy()->getType());

         return true;
      }
      else if (td->getOriginTy()->getType()->isObjectType()) {
         HandleConstructorCall(node, td->getOriginTy()->getType()
                                       ->getRecord());
         return true;
      }
   }

   return false;
}

void SemaPass::HandleBuiltinCall(CallExpr *node)
{
   if (node->getKind() == CallKind::VariadicSizeof) {
      node->setReturnType(getObjectTy("UInt"));

      auto &TA = node->getTemplateArgs().front();
      if (!TA->isTypeName()) {
         return diagnose(node, err_generic_error, "expected typename");
      }

      TA->getType()->setAllowUnexpandedTemplateArgs(true);

      auto GenericTy = *visitTypeRef(TA->getType());

      auto G = GenericTy->asGenericType();
      if (!G)
         return diagnose(node, err_generic_error,
                         "sizeof... expects variadic type parameter");

      auto TP = hasTemplateParam(G->getGenericTypeName());
      assert(TP && "invalid template parameter");

      if (!TP->isVariadic())
         return diagnose(node, err_generic_error,
                         "sizeof... expects variadic type parameter");

      return;
   }

   HandleFunctionCall(node, {});
   node->setKind(CallKind::Builtin);
}

void SemaPass::HandleMethodCall(CallExpr *node, QualType ty)
{
   bool staticCall = false;

   if (ty->isPointerType()) {
      if (!node->isPointerAccess()) {
         diagnose(node, err_generic_error,
                  "cannot access value on pointer, did you mean to use '->'?");
      }

      ty = ty->getPointeeType();
   }
   else if (node->isPointerAccess()) {
      diagnose(node, err_generic_error, "value is not a pointer");
   }

   if (auto NS = ty->asNamespaceType()) {
      return HandleDeclContextMember(node, NS->getNamespace());
   }
   else if (auto Meta = ty->asMetaType()) {
      auto underlying = Meta->getUnderlyingType();

      if (!underlying->isObjectType())
         return HandleStaticTypeCall(node, *underlying);

      staticCall = true;
      ty = underlying;
   }
   else if (!ty->isObjectType()) {
      return HandleOtherTypeCall(node, *ty);
   }

   auto R = ty->getRecord();

   if (auto lookupResult = R->lookup(node->getIdent())) {
      auto decl = lookupResult.front();

      if (auto F = dyn_cast<FieldDecl>(decl)) {
         QualType fieldTy(F->getType()->getType());

         if (fieldTy->isFunctionType()) {
            auto subExpr = new (getContext())
               MemberRefExpr(string(node->getIdent()));

            subExpr->setIsPointerAccess(node->isPointerAccess());

            node->setIsPointerAccess(false);
            node->setKind(CallKind::AnonymousCall);

            visitMemberRefExpr(subExpr,
                               subExpr->isPointerAccess()
                                  ? QualType(Context.getPointerType(ty))
                                  : ty);

            auto sub = node->getSubExpr();
            replaceExpressionWith(*this, node, subExpr);

            subExpr->setSubExpr(sub);
            visitMemberRefExpr(subExpr, ty);

            return;
         }
      }
      else if (auto M = dyn_cast<MethodDecl>(decl)) {
         if (staticCall && !M->isStatic())
            diagnose(node, err_generic_error,
                     "cannot call non-static method statically");

         auto &args = node->getArgs();
         auto result = getMethod(R, lookupResult, args,
                                 node->getTemplateArgs(), node, staticCall);

         if (!result->isCompatible()) {
            return diagnoseNoMatchingCall(result.get(),
                                          NoMatchingCallKind::Method,
                                          node->getIdent(), node);
         }

         auto method = result->getMethod();

         checkMemberAccessibility(R, method->getName(),
                                  method->getAccess(), node);

         auto returnTy = method->getReturnType()->getType();
         node->setReturnType(returnTy);

         node->setKind(method->isStatic() ? CallKind::StaticMethodCall
                                          : CallKind::MethodCall);

         node->setMethod(method);

         PrepareCallArgs(node->getArgs(), method);
         ApplyCasts(node->getArgs(), method->getFunctionType());

         return;
      }
      else if (auto C = dyn_cast<EnumCaseDecl>(decl)) {
         auto enumCaseExpr = new (getContext())
            EnumCaseExpr(string(node->getIdent()),
                         move(node->getArgs()));

         enumCaseExpr->setEnum(cast<EnumDecl>(R));

         replaceExpressionWith(*this, node, enumCaseExpr);
         node->setReturnType(visitEnumCaseExpr(enumCaseExpr));

         return;
      }
      else if (auto Inner = dyn_cast<RecordDecl>(decl)) {
         return HandleConstructorCall(node, Inner);
      }
      else if (auto TD = dyn_cast<TypedefDecl>(decl)) {
         return HandleStaticTypeCall(node, *TD->getOriginTy()->getType());
      }
      else {
         llvm_unreachable("unhandled decl kind");
      }
   }

   diagnose(node, err_generic_error,
            "method " + node->getIdent() + " does not exist on type "
            + R->getName());
}

void SemaPass::HandleDeclContextMember(CallExpr *node, DeclContext *Ctx)
{
   auto lookupResult = Ctx->lookup(node->getIdent());
   if (!lookupResult) {
      return diagnoseMemberNotFound(Ctx, node, node->getIdent(), "function");
   }

   auto lookup = lookupResult.front();
   if (auto R = dyn_cast<RecordDecl>(lookup)) {
      return HandleConstructorCall(node, R);
   }
   else if (auto F = dyn_cast<FunctionDecl>(lookup)) {
      return HandleFunctionCall(node, { F });
   }
   else if (auto GV = dyn_cast<GlobalVarDecl>(lookup)) {
      node->setGlobalVar(GV);

      auto res = GV->getTypeRef()->getType();
      res.isLvalue(true);

      return HandleAnonCall(node, res);
   }
   else if (auto TD = dyn_cast<TypedefDecl>(lookup)) {
      if (TryTypedef(node, TD)) {
         return;
      }

      return diagnose(node, err_generic_error, "cannot call typedef");
   }
   else if (isa<AliasDecl>(lookup)) {
      return diagnose(node, err_generic_error, "cannot call alias");
   }
   else if (isa<NamespaceDecl>(lookup)) {
      return diagnose(node, err_generic_error, "cannot call namespace");
   }
   else {
      llvm_unreachable("bad lookup result");
   }
}

void SemaPass::HandleStaticTypeCall(CallExpr *node, Type *Ty)
{
   if (Ty->isPrimitiveType()) {
      auto& args = node->getArgs();
      if (args.size() != 1) {
         diagnose(node, err_generic_error,
                  "primitive initializer expects one argument");

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
            diagnose(node, err_generic_error,
                     "primitive initializer expects an integral constant");

            return;
         }
      }
      else {
         assert(Ty->isFPType());
         auto lit = dyn_cast<FPLiteral>(args.front());
         if (!lit) {
            diagnose(node, err_generic_error,
                     "primitive initializer expects a floating point constant");

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
      if (!args.empty()) {
         diagnose(node, err_generic_error,
                  "'void' initializer takes no arguments");

         node->setHadError(true);
         return;
      }

      node->setKind(CallKind::PrimitiveInitializer);
      node->setReturnType(Ty);

      return;
   }
   else if (Ty->isObjectType()) {
      return HandleConstructorCall(node, Ty->getRecord());
   }

   diagnose(node, err_generic_error, "type " + Ty->toUniqueString() + " "
      "cannot be called statically");
}

void SemaPass::HandleOtherTypeCall(CallExpr *node, Type *ty)
{
   if (auto tup = ty->asTupleType()) {
      if (node->getIdent() == "__get_unsafe") {
         auto &args = node->getArgs();
         if (args.empty())
            return diagnose(node, err_generic_error,
                            "expected one integral argument");

         implicitCastIfNecessary(node->getArgs().front(),
                                 args.front()->getExprType(),
                                 Context.getUIntTy());

         QualType elementTypes[2];
         elementTypes[0] = Context.getPointerType(
            Context.getRecordType(getRecord("cdot.TypeInfo"))
         );

         elementTypes[1] = Context.getPointerType(Context.getCharTy());

         node->setKind(CallKind::UnsafeTupleGet);
         node->setReturnType(Context.getTupleType(elementTypes));

         return;
      }
   }

   diagnose(node, err_generic_error,
            "cannot call method on value of type " + ty->toString());
}

void SemaPass::HandleConstructorCall(CallExpr *node, RecordDecl *record)
{
   assert(record && "should not be called otherwise");

   if (!record->isTemplate() && !node->getTemplateArgs().empty()) {
      diagnose(node, err_generic_type_count, 0, node->getTemplateArgs().size(),
               node->getIdent());

      node->getTemplateArgs().clear();
   }

   if (record->isUnion()) {
      if (node->getArgs().size() != 1)
         return diagnose(node, err_union_initializer_arg_count);
      if (record->isTemplate() && node->getTemplateArgs().empty()) {
         return diagnose(node, err_generic_error,
                         "union template parameters cannot be inferred");
      }

      auto ty = node->getArgs().front()->getExprType();
      auto U = cast<UnionDecl>(record);

      cdot::Type *neededTy = nullptr;
      for (auto &decl : U->getDecls()) {
         if (auto F = dyn_cast<FieldDecl>(decl)) {
            auto fieldType = F->getType()->getType();
            if (implicitlyCastableTo(ty, fieldType)) {
               neededTy = *fieldType;
               break;
            }
         }
      }

      if (!neededTy) {
         diag::err(err_union_initializer_type) << node
                                               << diag::term;
      }

      toRvalueIfNecessary(ty, node->getArgs().front());
      implicitCastIfNecessary(node->getArgs().front(), ty,
                              QualType(neededTy));

      node->setReturnType(Context.getRecordType(U));
      node->setKind(CallKind::UnionInitializer);
      node->setUnion(U);

      return;
   }

   if (isa<EnumDecl>(record)) {
      return diagnose(node, err_generic_error,
                      "enums cannot be constructed directly");
   }

   if (isa<ProtocolDecl>(record))
      return diagnose(node, err_generic_error,
                      "protocols cannot be constructed directly");

   auto rec = cast<StructDecl>(record);
   checkClassAccessibility(rec, node);

   auto& givenArgs = node->getArgs();
   auto result = getMethod(rec, "init", givenArgs, node->getTemplateArgs(),
                           node);

   if (!result->isCompatible()) {
      return diagnoseNoMatchingCall(result.get(), NoMatchingCallKind::Method,
                                    node->getIdent(), node);
   }

   auto method = result->getMethod();
   rec = cast<StructDecl>(method->getRecord());

   // check accessibility
   checkMemberAccessibility(rec, method->getName(), method->getAccess(),
                            node);

   if (auto Cl = dyn_cast<ClassDecl>(rec)) {
      if (Cl->isAbstract())
         diagnose(node, err_instantiate_abstract_class, Cl->getName());
   }

   node->setKind(CallKind::InitializerCall);

   node->setReturnType(Context.getRecordType(rec));
   node->setMethod(method);

   PrepareCallArgs(node->getArgs(), method);
   ApplyCasts(node->getArgs(), method->getFunctionType());
}

void SemaPass::diagnoseNoMatchingCall(CallResult *Res,
                                      NoMatchingCallKind kind,
                                      llvm::StringRef name,
                                      CallExpr *expr) {
   assert(!Res->isCompatible());
   diagnose(expr, err_no_matching_call,
            kind == NoMatchingCallKind::Method ? 1 : 0,
            name);

   for (auto &Cand : Res->getFailedCandidates()) {
      diagnostics.emplace_back(std::move(Cand.getDiagnostic()));
   }
}

void SemaPass::HandleCallOperator(CallExpr *node, QualType latest)
{
   if (!latest->isObjectType() || !getRecord(latest->getClassName())) {
      return HandleAnonCall(node, latest);
   }

   string methodName = "postfix ()";
   auto rec = latest->getRecord();

   auto& givenArgs = node->getArgs();
   auto result = getMethod(rec, methodName, givenArgs);

   if (!result->isCompatible()) {
      return diagnoseNoMatchingCall(result.get(),
                                    NoMatchingCallKind::AnonymousCall,
                                    node->getIdent(), node);
   }

   auto method = result->getMethod();

   node->setKind(CallKind::CallOperator);
   node->setMethod(method);

   PrepareCallArgs(node->getArgs(), method);
   ApplyCasts(node->getArgs(), method->getFunctionType());

   node->setReturnType(method->getReturnType()->getType());
}

void SemaPass::HandleAnonCall(CallExpr *node, QualType latest)
{
   auto& givenArgs = node->getArgs();
   if (latest->isPointerType()) {
      latest = latest->asPointerType()->getPointeeType();
   }

   if (latest->isObjectType()) {
      auto R = latest->getRecord();
      auto CallOp = getMethod(R, "postfix ()", givenArgs);

      if (CallOp->isCompatible()) {
         node->setKind(CallKind::CallOperator);
         node->setMethod(CallOp->getMethod());
         node->setReturnType(node->getMethod()->getReturnType()->getType());

         return;
      }
   }

   if (!latest->isFunctionType()) {
      return diagnose(node, err_generic_error,
                      latest.toString() + " is not a callable type");
   }

   auto func = latest->asFunctionType();
   auto result = checkAnonymousCall(func, givenArgs, node);

   if (!result->isCompatible()) {
      return diagnoseNoMatchingCall(result.get(),
                                    NoMatchingCallKind::AnonymousCall,
                                    node->getIdent(), node);
   }

   PrepareCallArgs(node->getArgs(), func);
   ApplyCasts(node->getArgs(), func);

   node->setKind(CallKind::AnonymousCall);
   node->setReturnType(func->getReturnType());
   node->setFunctionType(func);
}

} // namespace ast
} // namespace cdot