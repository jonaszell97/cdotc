//
// Created by Jonas Zell on 24.10.17.
//

#include "SemaPass.h"

#include "../ASTIncludes.h"
#include "Builtin.h"

#include "../../../Util.h"
#include "../../SymbolTable.h"

#include "TemplateInstantiator.h"

#include "Record/Class.h"
#include "Record/Union.h"
#include "Record/Enum.h"
#include "Record/Protocol.h"

#include "../../../Variant/Type/Type.h"
#include "../../../Variant/Type/IntegerType.h"
#include "../../../Variant/Type/ObjectType.h"
#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/GenericType.h"
#include "../../../Variant/Type/VoidType.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/FPType.h"
#include "../../../Variant/Type/TupleType.h"
#include "../../../Variant/Type/AutoType.h"
#include "../../../Variant/Type/MetaType.h"

#include "../../../Message/Exceptions.h"
#include "../../../Message/Diagnostics.h"

#include "../Declaration/DeclPass.h"
#include "OverloadResolver.h"

using namespace cdot::diag;
using namespace cdot::support;
using ast::Function;

namespace cdot {
namespace ast {

SemaPass::CallResult::~CallResult()
{
//   if (isCompatible())
//      static_cast<CompatibleCallResult*>(this)->destroyValue();
//   else
//      static_cast<IncompatibleCallResult*>(this)->destroyValue();
}

Function* SemaPass::CallResult::getFunction() const
{
   assert(isCompatible());
   return static_cast<CompatibleCallResult const*>(this)->F;
}

cl::Method* SemaPass::CallResult::getMethod() const
{
   assert(isCompatible());
   return static_cast<CompatibleCallResult const*>(this)->M;
}

Callable* SemaPass::CallResult::getCallable() const
{
   assert(isCompatible());
   return static_cast<CompatibleCallResult const*>(this)->C;
}

cl::EnumCase* SemaPass::CallResult::getCase() const
{
   assert(isCompatible());
   return static_cast<CompatibleCallResult const*>(this)->Case;
}

sema::TemplateArgList& SemaPass::CallResult::getTemplateArgs()
{
   assert(isCompatible());
   return static_cast<CompatibleCallResult*>(this)->templateArgs;
}

std::vector<QualType> const& SemaPass::CallResult::getResolvedGivenArgs() const
{
   return static_cast<CompatibleCallResult const*>(this)->resolvedGivenArgs;
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

SemaPass::CallCandidate::CallCandidate(CallCompatability &Comp, Callable *C)
   : loc(C->getSourceLoc())
{
   auto reason = Comp.getFailureReason();
   if (reason == FailureReason::IncompatibleArgCount) {
      diagnostic = diag::note(note_cand_mismatched_arg_count)
         << Comp.resolvedNeededArgs.size() << Comp.resolvedArgs.size()
         << C->getSourceLoc();
   }
   else if (reason == FailureReason::IncompatibleArgument) {
      auto &neededArg = Comp.resolvedNeededArgs[Comp.incompatibleArg];
      diagnostic = diag::note(note_cand_no_implicit_conv)
         << Comp.resolvedArgs[Comp.incompatibleArg] << neededArg.type
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
      if (Comp.diagnostics.empty()) {
         diagnostic = diag::err(err_generic_error)
            << "failed constraint" << Comp.failedConstraint;
      }
      else
         diagnostic = Comp.diagnostics.front();
   }
   else {
      llvm_unreachable("bad reason");
   }

   if (auto M = dyn_cast<cl::Method>(C))
      if (M->isMemberwiseInitializer())
         diagnostic << diag::memberwise_init;
}

SemaPass::CallCandidate::CallCandidate(CallCompatability &Comp,
                                       cl::EnumCase *Case)
   : loc(Case->loc)
{
   auto reason = Comp.getFailureReason();
   if (reason == FailureReason::IncompatibleArgCount) {
      diagnostic = diag::note(note_cand_mismatched_arg_count)
         << Comp.resolvedNeededArgs.size() << Comp.resolvedArgs.size()
         << Case->loc;
   }
   else if (reason == FailureReason::IncompatibleArgument) {
      auto &neededArg = Case->associatedValues[Comp.incompatibleArg];
      diagnostic = diag::note(note_cand_no_implicit_conv)
         << Comp.resolvedArgs[Comp.incompatibleArg] << neededArg.type
         << Comp.incompatibleArg + 1 << Case->loc;
   }
   else {
      llvm_unreachable("bad reason");
   }
}

SemaPass::CallCandidate::CallCandidate(CallCompatability &Comp,
                                       FunctionType *FTy,
                                       SourceLocation declLoc)
   : loc(declLoc)
{
   auto reason = Comp.getFailureReason();
   if (reason == FailureReason::IncompatibleArgCount) {
      diagnostic = diag::note(note_cand_mismatched_arg_count)
         << Comp.resolvedNeededArgs.size() << Comp.resolvedArgs.size()
         << declLoc;
   }
   else if (reason == FailureReason::IncompatibleArgument) {
      auto &neededArg = FTy->getArgTypes()[Comp.incompatibleArg];
      diagnostic = diag::note(note_cand_no_implicit_conv)
         << Comp.resolvedArgs[Comp.incompatibleArg] << neededArg.type
         << Comp.incompatibleArg + 1 << declLoc;
   }
   else {
      llvm_unreachable("bad reason");
   }
}

void SemaPass::diagnoseAmbiguousCall(llvm::ArrayRef<Callable*> perfectMatches,
                                     Statement *Caller) {
   if (perfectMatches.size() <= 1)
      return;

   auto &first = perfectMatches.front();
   diagnose(Caller, err_ambiguous_call, isa<cl::Method>(first),
            first->getName());

   for (size_t i = 0; i < perfectMatches.size(); ++i) {
      note(note_ambiguous_call)
         << perfectMatches[i]->getSourceLoc();
   }
}

SemaPass::ConstraintResult SemaPass::checkConstraint(
                                 std::shared_ptr<StaticExpr> const &constraint,
                                 TemplateArgList &TAs) {
   llvm_unreachable("fuck it");
}

SemaPass::FunctionResult SemaPass::getUFCS(
                                 const string &funcName,
                                 std::vector<Argument> &args,
                                 const std::vector<TemplateArg> &templateArgs,
                                 Statement *caller) {
   if (!args.empty() && args.front().type->isObjectTy()) {
      Argument firstArg = std::move(args.front());
      args.erase(args.begin());

      auto result = getMethod(firstArg.type->getRecord(), funcName, args,
                              templateArgs, caller);

      if (result->isCompatible())
         return result;

      args.insert(args.begin(), std::move(firstArg));
   }

   return getFunction(funcName, args, templateArgs, caller);
}

SemaPass::FunctionResult SemaPass::getFunction(const string& funcName,
                                 std::vector<Argument>& args,
                                 std::vector<TemplateArg> const& templateArgs,
                                               Statement *caller) {
   std::vector<CallCandidate> failedCandidates;
   if (!getAnyFn(funcName))
      return std::make_unique<IncompatibleCallResult>(move(failedCandidates));

   string context;
   auto overloads = getFunctionOverloads(funcName);
   assert(!overloads.empty());

   OverloadResolver Resolver(*this, args, templateArgs,
                             caller ? caller->getSourceLoc()
                                    : SourceLocation());

   llvm::SmallVector<Callable*, 4> perfectCalls;
   std::unique_ptr<CompatibleCallResult> Res = nullptr;

   size_t bestMatch = -1;
   size_t maxSatisfiedConstraints = 0;

   for (const auto &overload : overloads) {
      auto res = Resolver.checkIfViable(overload);

      if (res.isCompatible()) {
         size_t numConstraints = 0;
         if (auto decl = overload->getDeclaration()) {
            numConstraints = decl->getConstraints().size();
         }

         if (res.perfectMatch && numConstraints <= maxSatisfiedConstraints) {
            perfectCalls.push_back(overload);
         }

         bool satisifiesMoreConstraints =
            numConstraints > maxSatisfiedConstraints;

         if (res.perfectMatch || satisifiesMoreConstraints
                                             || res.castPenalty < bestMatch) {
            Res = std::make_unique<CompatibleCallResult>(
               overload, std::move(res.resolvedArgs),
               std::move(res.templateArgList));

            bestMatch = res.getCastPenalty();
            maxSatisfiedConstraints = numConstraints;
         }
      }
      else if (!Res) {
         failedCandidates.emplace_back(res, overload);
      }
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

SemaPass::FunctionResult SemaPass::getMethod(Record *rec,
                                             const string &methodName,
                                             const std::vector<Argument> &args,
                                             std::vector<TemplateArg> const&
                                                                  templateArgs,
                                             Statement *caller,
                                             bool isStatic) {
   std::vector<CallCandidate> failedCandidates;
   auto overloads = rec->getMethods().equal_range(methodName);

   if (overloads.first == overloads.second)
      return std::make_unique<IncompatibleCallResult>(move(failedCandidates));

   OverloadResolver Resolver(*this, args, templateArgs,
                             caller ? caller->getSourceLoc()
                                    : SourceLocation());

   llvm::SmallVector<Callable*, 4> perfectCalls;
   std::unique_ptr<CompatibleCallResult> Res = nullptr;

   size_t bestMatch = -1;
   size_t maxSatisfiedConstraints = 0;

   while (overloads.first != overloads.second) {
      auto& overload = overloads.first->second;
      if (isStatic && !overload.isStatic())
         continue;

      auto res = Resolver.checkIfViable(&overload);
      if (res.isCompatible()) {
         size_t numConstraints = 0;
         if (auto decl = overload.getDeclaration()) {
            numConstraints = decl->getConstraints().size();
         }

         if (res.perfectMatch && numConstraints <= maxSatisfiedConstraints) {
            perfectCalls.push_back(&overload);
         }

         bool satisifiesMoreConstraints =
            numConstraints > maxSatisfiedConstraints;

         if (res.perfectMatch || satisifiesMoreConstraints
                                          || res.getCastPenalty() < bestMatch) {
            Res = std::make_unique<CompatibleCallResult>(
               &overload, std::move(res.resolvedArgs),
               std::move(res.templateArgList),
               std::move(res.initializerTemplateArgList));

            bestMatch = res.getCastPenalty();
            maxSatisfiedConstraints = numConstraints;
         }
      }
      else if (!Res) {
         failedCandidates.emplace_back(res, &overload);
      }
      ++overloads.first;
   }

   if (!Res && rec->isClass()) {
      auto cl = rec->getAs<Class>();
      if (cl->getParent()) {
         return getMethod(cl->getParent(), methodName, args, templateArgs);
      }
   }

   diagnoseAmbiguousCall(perfectCalls, caller);

   if (!Res)
      return std::make_unique<IncompatibleCallResult>(move(failedCandidates));

   auto method = Res->getMethod();
   auto R = method->getOwningRecord();
   auto &argList = Res->getTemplateArgs();

   if (method->isInitializer() && R->isTemplated()) {
      auto initializerArgs = argList.moveInitializerArgs();
      auto offset = method->getTemplateParams().size()
                    - R->getTemplateParams().size();

      initializerArgs.setParameterOffsetBegin(offset);
      argList.setParameterOffsetEnd(offset);

      auto Inst = InstantiateRecord(rec,
                                    std::move(argList),
                                    caller->getSourceLoc());

      method = Inst->getMethod(Res->getMethod()->getMethodID());
      Res->setMethod(method);

      argList = std::move(initializerArgs);
   }

   maybeInstantiateMemberFunction(Res->getMethod(), caller->getSourceLoc());

   bool needsInstantiation =
      (!method->isInitializer()
         || method->isTemplatedInitializer())
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

void SemaPass::maybeInstantiateMemberFunction(cl::Method *M,
                                              const SourceLocation &callerLoc) {
   if (!M->getDeclaration() || M->getDeclaration()->getBody()
         || !M->getSpecializedTemplate())
      return;

   auto instantiatedBody =
      TemplateInstantiator::InstantiateMethodBody(
         *this, callerLoc, cast<cl::Method>(M->getSpecializedTemplate()), M);

   M->setInstantiatedFrom(callerLoc);
   M->getDeclaration()
    ->setBody(std::static_pointer_cast<CompoundStmt>(instantiatedBody));

   DiagnosticBuilder::pushInstantiationCtx(M->getOwningRecord());
   DiagnosticBuilder::pushInstantiationCtx(M);

   visitFunctionInstantiation(M);

   DiagnosticBuilder::popInstantiationCtx();
   DiagnosticBuilder::popInstantiationCtx();
}

SemaPass::FunctionResult SemaPass::getCase(cl::Enum *en,
                                           const string &caseName,
                                           std::vector<Argument> const& args) {
   if (!en->hasCase(caseName))
      return std::make_unique<IncompatibleCallResult>();

   auto types = en->getCases()[caseName].associatedValues;
   if (types.size() != args.size())
      return std::make_unique<IncompatibleCallResult>();

   auto &Case = en->getCase(caseName);

   OverloadResolver Resolver(*this, args, {});
   auto res = Resolver.checkIfViable(Case);

   if (!res.isCompatible()) {
      std::vector<CallCandidate> candidates;
      candidates.emplace_back(res, &Case);

      return std::make_unique<IncompatibleCallResult>(move(candidates));
   }

   return std::make_unique<CompatibleCallResult>(&Case,
                                                 std::move(res.resolvedArgs));
}

SemaPass::FunctionResult SemaPass::checkAnonymousCall(
                                          FunctionType *FTy,
                                          const std::vector<Argument>& args,
                                          SourceLocation const& loc) {
   OverloadResolver Resolver(*this, args, {}, loc);
   auto res = Resolver.checkIfViable(FTy);

   if (!res.isCompatible()) {
      std::vector<CallCandidate> candidates;
      candidates.emplace_back(res, FTy, loc);

      return std::make_unique<IncompatibleCallResult>(move(candidates));
   }

   return std::make_unique<CompatibleCallResult>(FTy,
                                                 std::move(res.resolvedArgs));
}

void SemaPass::ApplyCasts(std::vector<std::shared_ptr<Expression>> &args,
                          std::vector<QualType> const& givenArgs,
                          const std::vector<Argument> &declaredArgs) {
   size_t i = 0;
   for (auto& arg : givenArgs) {
      if (i >= declaredArgs.size()) {
         break;
      }

      assert(args.size() > i && "No arg to cast");
      implicitCastIfNecessary(args[i], arg,
                              declaredArgs[i].type,
                              !arg->isTypeGroup()
                              && !declaredArgs[i].type->isAutoTy());

      ++i;
   }
}

void SemaPass::PrepareCallArgs(std::vector<std::shared_ptr<Expression>>& args,
                               std::vector<Argument> &givenArgs,
                               std::vector<Argument> const& declaredArgs) {
   size_t i = 0;
   bool vararg = !declaredArgs.empty() && declaredArgs.back().cstyleVararg;
   for (auto& arg : args) {
      if (i >= givenArgs.size()) {
         break;
      }

      auto &given = givenArgs[i].type;

      if (i >= declaredArgs.size() || (vararg && i >= declaredArgs.size() - 1)){
         if (given->isTypeGroup()) {
            given = visit(arg);
         }

         // assume that c style varargs need a primitive...
         if (given->isBoxedPrimitive()) {
            QualType unboxed(given->unbox());
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

      if (given->isTypeGroup()) {
         arg->setContextualType(declared.type);
         given = visit(arg);
      }

      toRvalueIfNecessary(given, arg, !declared.type.isLvalue());
      ++i;
   }

   if (!vararg) {
      while (args.size() < declaredArgs.size()) {
         assert(declaredArgs[args.size()].getDefaultVal());
         args.push_back(declaredArgs[args.size()].getDefaultVal());
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
   for (size_t i = node->getResolvedArgs().size(); i < args.size(); ++i) {
      auto& arg = args.at(i);
      QualType ty;

      if (auto Pl = getPlaceholderType(arg.get())) {
         *ty = Pl;
      }
      else {
         auto result = visitExpr(node, arg);
         if (result.hadError()) {
            return {};
         }

         ty = result.getType();
      }

      node->getResolvedArgs().emplace_back("", ty, arg);
   }

   for (auto &TA : node->getTemplateArgs()) {
      if (TA.isTypeName()) {
         auto res = visitExpr(node, TA.getType().get());
         if (res.hadError()) {
            return { };
         }
      }
   }

   bool isBuiltin = node->getBuiltinFnKind() != BuiltinFn::None;
   if (isBuiltin) {
      HandleBuiltinCall(node);
   }
   else if (ty) {
      if (ty->isFunctionTy())
         HandleAnonCall(node, ty);
      else if (node->getIdent().empty())
         HandleCallOperator(node, ty);
      else
         HandleMethodCall(node, ty);
   }
   else {
      HandleFunctionCall(node, node->getIdent());
   }

   return VisitSubExpr(node, node->getReturnType());
}

void SemaPass::HandleFunctionCall(CallExpr *node, llvm::StringRef ident)
{
   // resolve given template args
   auto Fun = getAnyFn(ident);
   if (!Fun) {
      if (auto Rec = currentClass()) {
         if (Rec->hasMethodWithName(ident)) {
            if (Rec->isTemplated()) {
               node->setIsTypeDependent(true);
               return;
            }

            node->setImplicitSelf(true);

            return HandleMethodCall(node, ObjectType::get(Rec));
         }
      }

      if (auto R = getRecord(ident)) {
         return HandleConstructorCall(node, R->getName());
      }

      if (wouldBeValidIdentifier(ident)) {
         node->setIdentExpr(new IdentifierRefExpr(string(ident)));

         auto expr = node->getIdentExpr();
         auto Ty = visitIdentifierRefExpr(expr);

         if (isa<FunctionType>(*Ty)) {
            node->setBinding(expr->getBinding());
            return HandleAnonCall(node, Ty);
         }

         return HandleCallOperator(node, Ty);
      }

      if (auto td = getTypedef(node->getIdent())) {
         if (isa<PrimitiveType>(td->aliasedType)) {
            auto& resolvedArgs = node->getResolvedArgs();
            if (resolvedArgs.size() != 1) {
               diagnose(node, err_generic_error,
                        "primitive initializer expects one argument");

               return node->setHadError(true);
            }

            auto &given = resolvedArgs.front();
            if (given.getType()->isTypeGroup()) {
               node->getArgs().front()->setContextualType(td->aliasedType);
               visitExpr(node, node->getArgs().front());
            }
            else {
               implicitCastIfNecessary(node->getArgs().front(),
                                       given.getType(), td->aliasedType);
            }

            node->setKind(CallKind::PrimitiveInitializer);
            node->setReturnType(td->aliasedType);

            return;
         }
      }

      return diagnose(node, err_func_not_found, 0, ident);
   }

   if (!Fun->isTemplate() && !node->getTemplateArgs().empty()) {
      diagnose(node, err_generic_type_count, 0,
               node->getTemplateArgs().size(), ident);

      node->getTemplateArgs().clear();
   }

   auto& resolvedArgs = node->getResolvedArgs();
   auto result = getFunction(ident, resolvedArgs, node->getTemplateArgs(),
                             node);

   if (!result->isCompatible()) {
      return diagnoseNoMatchingCall(result.get(), NoMatchingCallKind::Function,
                                    node->getIdent(), node);
   }

   auto func = result->getFunction();

   PrepareCallArgs(node->getArgs(), resolvedArgs, func->getArguments());
   ApplyCasts(node->getArgs(), result->getResolvedGivenArgs(),
              func->getArguments());

   node->setKind(CallKind::NamedFunctionCall);
   node->setFunc(func);
   node->setReturnType(func->getReturnType());
}

void SemaPass::HandleBuiltinCall(CallExpr *node)
{
   if (node->getKind() == CallKind::VariadicSizeof) {
      node->setReturnType(ObjectType::get("UInt"));

      auto &TA = node->getTemplateArgs().front();
      if (!TA.isTypeName()) {
         return diagnose(node, err_generic_error, "expected typename");
      }

      TA.getType()->setAllowUnexpandedTemplateArgs(true);

      auto GenericTy = *visitTypeRef(TA.getType().get());

      auto G = dyn_cast<GenericType>(GenericTy);
      if (!G)
         return diagnose(node, err_generic_error,
                         "sizeof... expects variadic type parameter");

      auto TP = hasTemplateParam(G->getGenericTypeName());
      assert(TP && "invalid template parameter");

      if (!TP->isVariadic)
         return diagnose(node, err_generic_error,
                         "sizeof... expects variadic type parameter");

      return;
   }

   HandleFunctionCall(node, node->getIdent());
   node->setKind(CallKind::Builtin);
}

void SemaPass::HandleMethodCall(CallExpr *node, QualType ty)
{
   bool staticCall = false;

   if (ty->isPointerTy()) {
      if (!node->isPointerAccess_()) {
         diagnose(node, err_generic_error,
                  "cannot access value on pointer, did you mean to use '->'?");
      }

      ty = ty->getPointeeType();
   }
   else if (node->isPointerAccess_()) {
      diagnose(node, err_generic_error, "value is not a pointer");
   }

   if (auto NS = dyn_cast<NamespaceType>(*ty)) {
      return HandleNamespaceMember(node, NS->getNamespace());
   }
   else if (auto Meta = dyn_cast<MetaType>(*ty)) {
      auto underlying = Meta->getUnderlyingType();

      if (!underlying->isObjectTy())
         return HandleStaticTypeCall(node, underlying);

      staticCall = true;
      *ty = underlying;
   }
   else if (!ty->isObjectTy()) {
      return HandleOtherTypeCall(node, *ty);
   }

   Record *R = ty->getRecord();

   if (staticCall) {
      if (auto E = dyn_cast<Enum>(R)) {
         auto enumCaseExpr = std::make_shared<EnumCaseExpr>(
            string(node->getIdent()), move(node->getArgs()));

         enumCaseExpr->setEnum(E);
         enumCaseExpr->setContextualType(node->getContextualType());
         enumCaseExpr->setSourceLoc(node->getSourceLoc());
         enumCaseExpr->setMemberExpr(node->getMemberExpr());

         node->getParentExpr()->setMemberExpr(enumCaseExpr);
         node->setReturnType(visitEnumCaseExpr(enumCaseExpr.get()));

         return;
      }

      if (!R->hasMethodWithName(node->getIdent())) {
         auto NS = SymbolTable::getNamespace(R->getName());
         assert(NS && "no namespace created for record");

         return HandleNamespaceMember(node, NS);
      }
   }

   // resolve given template args
   if (!R->hasMethodTemplate(node->getIdent())
                                          && !node->getTemplateArgs().empty()) {
      diagnose(node, err_no_template_method, R->getNameSelector(),
               R->getName(), node->getIdent());

      node->getTemplateArgs().clear();
   }

   checkClassAccessibility(R, node);

   auto &givenArgs = node->getResolvedArgs();
   auto result = getMethod(R, node->getIdent(), givenArgs,
                           node->getTemplateArgs(), node, staticCall);

   if (!result->isCompatible()) {
      if (auto TheStruct = dyn_cast<Struct>(R)) {
         if (TheStruct->hasField(node->getIdent())) {
            auto field = TheStruct->getField(node->getIdent());
            QualType fieldTy(field->fieldType);

            if (fieldTy->isFunctionTy()) {
               auto subExpr = std::make_shared<MemberRefExpr>(
                  string(node->getIdent()));

               auto parent = node->getParentExpr();
               auto nodePtr = parent->getMemberExpr();

               subExpr->setSourceLoc(node->getSourceLoc());
               subExpr->setMemberExpr(nodePtr);
               subExpr->setIsPointerAccess(node->isPointerAccess_());

               parent->setMemberExpr(subExpr);

               node->setIsPointerAccess_(false);
               node->setKind(CallKind::AnonymousCall);

               visitMemberRefExpr(subExpr.get(),
                                  subExpr->isPointerAccess()
                                     ? QualType(ty->getPointerTo())
                                     : ty);

               return;
            }
         }
      }

      return diagnoseNoMatchingCall(result.get(), NoMatchingCallKind::Method,
                                    node->getIdent(), node);
   }

   auto method = result->getMethod();

   checkMemberAccessibility(R, method->getName(),
                            method->getAccessModifier(), node);

   auto returnTy = method->getReturnType();
   node->setReturnType(returnTy);

   node->setKind(method->isStatic() ? CallKind::StaticMethodCall
                                    : CallKind::MethodCall);

   node->setMethod(method);

   PrepareCallArgs(node->getArgs(), givenArgs, method->getArguments());
   ApplyCasts(node->getArgs(), result->getResolvedGivenArgs(),
              method->getArguments());
}

void SemaPass::HandleNamespaceMember(CallExpr *node, Namespace *NS)
{
   llvm::SmallString<128> qualifiedName;
   qualifiedName += NS->getName();
   qualifiedName += ".";
   qualifiedName += node->getIdent();

   auto lookup = SymbolTable::lookup(qualifiedName.str(),
                                     declPass->importedNamespaces(),
                                     declPass->currentNamespace);

   using Kind = SymbolTable::LookupResult::LookupResultKind;

   switch (lookup.kind) {
      case Kind::LRK_Nothing:
         break;
      case Kind::LRK_Record:
      case Kind::LRK_Class:
      case Kind::LRK_Enum:
      case Kind::LRK_Struct:
      case Kind::LRK_Union:
      case Kind::LRK_Protocol: {
         auto R = lookup.Entry->getRecord();
         if (R->isTemplated()) {
            TemplateArgList list(*this, R, node->getTemplateArgs());
            if (!list.checkCompatibility()) {
               issueDiagnostics(list);
               node->setHadError(true);

               return;
            }

            R = TemplateInstantiator::InstantiateRecord(*this,
                                                        node->getSourceLoc(),
                                                        R, std::move(list));
         }

         return HandleConstructorCall(node, R->getName());
      }
      case Kind::LRK_Function:
         return HandleFunctionCall(node, qualifiedName.str());
      case Kind::LRK_GlobalVariable: {
         auto &G = *lookup.Entry->getVariable();
         node->setBinding(G.getName());

         auto res = G.getType();
         res.isLvalue(true);

         return HandleAnonCall(node, res);
      }
      case Kind::LRK_Typedef:
         return diagnose(node, err_generic_error, "cannot call typedef");
      case Kind::LRK_Alias:
         return diagnose(node, err_generic_error, "cannot call alias");
      case Kind::LRK_Namespace:
         return diagnose(node, err_generic_error, "cannot call namespace");
   }
}

void SemaPass::HandleStaticTypeCall(CallExpr *node, Type *Ty)
{
   llvm_unreachable("TODO");
}

void SemaPass::HandleOtherTypeCall(CallExpr *node, Type *ty)
{
   if (auto tup = dyn_cast<TupleType>(ty)) {
      if (node->getIdent() == "__get_unsafe") {
         auto &args = node->getResolvedArgs();
         if (args.empty())
            return diagnose(node, err_generic_error,
                            "expected one integral argument");

         implicitCastIfNecessary(node->getArgs().front(), args.front().type,
                                 IntegerType::getUnsigned());

         std::vector<std::pair<std::string, QualType>> elementTys{
            { "", QualType(ObjectType::get("cdot.TypeInfo")->getPointerTo()) },
            { "", QualType(IntegerType::getCharTy()->getPointerTo()) },
         };

         node->setKind(CallKind::UnsafeTupleGet);
         node->setReturnType(TupleType::get(elementTys));

         return;
      }
   }

   diagnose(node, err_generic_error,
            "cannot call method on value of type " + ty->toString());
}

void SemaPass::HandleConstructorCall(CallExpr *node, llvm::StringRef ident)
{
   auto record = getRecord(ident);
   if (!record->isTemplated() && !node->getTemplateArgs().empty()) {
      diagnose(node, err_generic_type_count, 0, node->getTemplateArgs().size(),
               ident);

      node->getTemplateArgs().clear();
   }

   if (record->isUnion()) {
      if (node->getResolvedArgs().size() != 1)
         return diagnose(node, err_union_initializer_arg_count);
      if (record->isTemplated() && node->getTemplateArgs().empty()) {
         return diagnose(node, err_generic_error,
                         "union template parameters cannot be inferred");
      }

      auto& ty = node->getResolvedArgs().front().type;
      auto U = record->getAs<Union>();

      auto neededTy = U->initializableWith(*ty);
      if (!neededTy) {
         diag::err(err_union_initializer_type) << node
                                               << diag::term;
      }

      if (ty->isTypeGroup()) {
         node->getArgs().front()->setContextualType(QualType(neededTy));
         ty = visit(node->getArgs().front());
      }

      toRvalueIfNecessary(ty, node->getArgs().front());
      implicitCastIfNecessary(node->getArgs().front(), ty,
                              QualType(neededTy));

      node->setReturnType({ObjectType::get(U->getName())});
      node->setKind(CallKind::UnionInitializer);
      node->setUnion(U);

      return;
   }

   if (isa<Enum>(record)) {
      return diagnose(node, err_generic_error,
                      "enums cannot be constructed directly");
   }

   if (isa<Protocol>(record))
      return diagnose(node, err_generic_error,
                      "protocols cannot be constructed directly");

   auto rec = cast<Struct>(record);
   checkClassAccessibility(rec, node);

   auto& givenArgs = node->getResolvedArgs();
   auto result = getMethod(rec, "init", givenArgs, node->getTemplateArgs(),
                           node);

   if (!result->isCompatible()) {
      return diagnoseNoMatchingCall(result.get(), NoMatchingCallKind::Method,
                                    node->getIdent(), node);
   }

   auto method = result->getMethod();
   rec = cast<Struct>(method->getOwningRecord());

   // check accessibility
   checkMemberAccessibility(rec, method->getName(), method->getAccessModifier(),
                            node);

   if (auto Cl = dyn_cast<Class>(rec)) {
      if (Cl->isAbstract())
         diagnose(node, err_instantiate_abstract_class, Cl->getName());
   }

   node->setKind(CallKind::InitializerCall);

   node->setReturnType(QualType(ObjectType::get(rec)));
   node->setMethod(method);

   PrepareCallArgs(node->getArgs(), givenArgs, method->getArguments());
   ApplyCasts(node->getArgs(), result->getResolvedGivenArgs(),
              method->getArguments());
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
   if (!latest->isObjectTy() || !getRecord(latest->getClassName())) {
      return HandleAnonCall(node, latest);
   }

   string methodName = "postfix ()";
   auto rec = latest->getRecord();

   auto& givenArgs = node->getResolvedArgs();
   auto result = getMethod(rec, methodName, givenArgs);

   if (!result->isCompatible()) {
      return diagnoseNoMatchingCall(result.get(),
                                    NoMatchingCallKind::AnonymousCall,
                                    node->getIdent(), node);
   }

   auto method = result->getMethod();

   node->setKind(CallKind::CallOperator);
   node->setMethod(method);

   PrepareCallArgs(node->getArgs(), givenArgs, method->getArguments());
   ApplyCasts(node->getArgs(), result->getResolvedGivenArgs(),
              method->getArguments());

   node->setReturnType(method->getReturnType());
}

void SemaPass::HandleAnonCall(CallExpr *node, QualType latest)
{
   auto& givenArgs = node->getResolvedArgs();

   if (latest->isPointerTy()) {
      latest = latest->asPointerTy()->getPointeeType();
   }

   if (latest->isObjectTy()) {
      auto R = latest->getRecord();
      auto CallOp = getMethod(R, "postfix ()", givenArgs);

      if (CallOp->isCompatible()) {
         node->setKind(CallKind::CallOperator);
         node->setMethod(CallOp->getMethod());
         node->setReturnType(node->getMethod()->getReturnType());

         return;
      }
   }

   if (!latest->isFunctionTy()) {
      return diagnose(node, err_generic_error,
                      latest.toString() + " is not a callable type");
   }

   auto func = cast<FunctionType>(*latest);
   auto result = checkAnonymousCall(func, givenArgs, node->getSourceLoc());

   if (!result->isCompatible()) {
      return diagnoseNoMatchingCall(result.get(),
                                    NoMatchingCallKind::AnonymousCall,
                                    node->getIdent(), node);
   }

   PrepareCallArgs(node->getArgs(), givenArgs, func->getArgTypes());
   ApplyCasts(node->getArgs(), result->getResolvedGivenArgs(),
              func->getArgTypes());

   node->setKind(CallKind::AnonymousCall);
   node->setReturnType(func->getReturnType());
   node->setFunctionType(func);
}

} // namespace ast
} // namespace cdot