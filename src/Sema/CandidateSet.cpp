//
// Created by Jonas Zell on 15.02.18.
//

#include "CandidateSet.h"

#include "AST/Expression.h"
#include "AST/Decl.h"
#include "ConstraintBuilder.h"
#include "Query/QueryContext.h"
#include "SemaPass.h"
#include "Support/Casting.h"

#include <llvm/ADT/SmallString.h>
#include <AST/PrettyPrinter.h>

using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::support;

namespace cdot {

CandidateSet::Candidate::Candidate() = default;

CandidateSet::Candidate::Candidate(CallableDecl *Func, unsigned Distance)
   : CandDecl(Func), FR(None), IsAnonymousCand(false), Distance(Distance)
{}

CandidateSet::Candidate::Candidate(AliasDecl *Alias, unsigned Distance)
   : CandDecl(Alias), FR(None), IsAnonymousCand(false), Distance(Distance)
{}

CandidateSet::Candidate::Candidate(Expression *FnExpr)
   : CandDecl(FnExpr), FR(None), IsAnonymousCand(true), Distance(0)
{}

CandidateSet::Candidate::Candidate(NamedDecl *FnDecl)
   : CandDecl(FnDecl), FR(None), IsAnonymousCand(true), Distance(0)
{}

CandidateSet::Candidate::~Candidate() = default;

CandidateSet::Candidate::Candidate(CandidateSet::Candidate &&)
   noexcept = default;

CandidateSet::Candidate&
CandidateSet::Candidate::operator=(CandidateSet::Candidate &&)
   noexcept = default;

CallableDecl* CandidateSet::Candidate::getFunc() const
{
   return cast<CallableDecl>(CandDecl.getDecl());
}

AliasDecl* CandidateSet::Candidate::getAlias() const
{
   return cast<AliasDecl>(CandDecl.getDecl());
}

Expression* CandidateSet::Candidate::getFuncExpr() const
{
   return dyn_cast_or_null<Expression>(CandDecl.asStatement());
}

NamedDecl* CandidateSet::Candidate::getFuncDecl() const
{
   return dyn_cast_or_null<NamedDecl>(CandDecl.asDecl());
}

FunctionType* CandidateSet::Candidate::getFunctionType() const
{
   if (isAnonymousCandidate()) {
      if (auto *E = getFuncExpr()) {
         return E->getExprType()->removeReference()->asFunctionType();
      }

      return cast<VarDecl>(getFuncDecl())->getType()->removeReference()
                                         ->asFunctionType();
   }

   return getFunc()->getFunctionType();
}

SourceLocation CandidateSet::Candidate::getSourceLoc() const
{
   if (isAnonymousCandidate()) {
      if (auto *E = getFuncExpr()) {
         return E->getSourceLoc();
      }

      return getFuncDecl()->getSourceLoc();
   }

   return getFunc()->getSourceLoc();
}

ast::PrecedenceGroupDecl *CandidateSet::Candidate::getPrecedenceGroup() const
{
   assert(!isAnonymousCandidate());
   return getFunc()->getPrecedenceGroup();
}

bool CandidateSet::Candidate::isAssignmentOperator()
{
   auto Prec = getPrecedenceGroup();
   if (!Prec)
      return false;

   return Prec->isAssignment();
}

unsigned CandidateSet::Candidate::getNumConstraints() const
{
   if (isAnonymousCandidate())
      return 0;

   return (unsigned)getFunc()->getConstraints()->size();
}

CandidateSet::CandidateSet(CandidateSet &&Other) noexcept
   : Status(Other.Status), IncludesSelfArgument(Other.IncludesSelfArgument),
     InvalidCand(Other.InvalidCand), Dependent(Other.Dependent),
     MatchIdx(Other.MatchIdx),
     BestConversionPenalty(Other.BestConversionPenalty),
     ResolvedArgs(move(Other.ResolvedArgs)), Candidates(move(Other.Candidates)),
     CandidateFns(move(Other.CandidateFns))
{

}

CandidateSet& CandidateSet::operator=(CandidateSet &&Other) noexcept
{
   this->~CandidateSet();
   new(this) CandidateSet(move(Other));

   return *this;
}

CandidateSet::Candidate*
CandidateSet::addCandidate(ast::CallableDecl *CD, unsigned Distance)
{
   if (!CandidateFns.insert(CD).second)
      return nullptr;

   Candidates.emplace_back(CD, Distance);
   return &Candidates.back();
}

CandidateSet::Candidate*
CandidateSet::addCandidate(ast::AliasDecl *Alias, unsigned Distance)
{
   if (!CandidateFns.insert(Alias).second)
      return nullptr;

   Candidates.emplace_back(Alias, Distance);
   return &Candidates.back();
}

CandidateSet::Candidate&
CandidateSet::addCandidate(Expression *FnExpr)
{
   Candidates.emplace_back(FnExpr);
   return Candidates.back();
}

CandidateSet::Candidate&
CandidateSet::addCandidate(NamedDecl *FnDecl)
{
   Candidates.emplace_back(FnDecl);
   return Candidates.back();
}

static bool tryStringifyConstraint(llvm::SmallString<128> &Str,
                                   Expression *Expr) {
   llvm::raw_svector_ostream sstream(Str);
   sstream << "'";

   ast::PrettyPrinter PP(sstream);
   PP.print(Expr);

   sstream << "'";

   return true;
}

static FakeSourceLocation makeFakeSourceLoc(CandidateSet &CandSet,
                                            DeclarationName funcName,
                                            CandidateSet::Candidate &Cand) {
   assert(Cand.isAnonymousCandidate() && "not a builtin candidate!");

   auto FuncTy = Cand.getFunctionType();
   llvm::SmallString<128> str;
   llvm::raw_svector_ostream OS(str);

   OS << "def " << funcName << "(";

   size_t i = 0;
   for (auto &arg : FuncTy->getParamTypes()) {
      if (i++ != 0) OS << ", ";
      OS << arg;
   }

   OS << ") -> " << FuncTy->getReturnType();

   return FakeSourceLocation{ str.str() };
}

static SourceLocation getArgumentLoc(CandidateSet::Candidate &Cand,
                                     llvm::ArrayRef<ast::Expression*> args) {
   unsigned idx = Cand.Data1;
   if (!Cand.isAnonymousCandidate() && Cand.getFunc()->isNonStaticMethod())
      ++idx;

   if (!Cand.isAnonymousCandidate()) {
      auto ArgDecls = Cand.getFunc()->getArgs();

      // variadic template arguments might create more arguments
      if (ArgDecls.size() > idx) {
         auto *D = ArgDecls[idx];
         if (D->getType().getTypeExpr())
            return D->getType().getTypeExpr()->getSourceLoc();

         return D->getSourceLoc();
      }

      if (!ArgDecls.empty()) {
         auto *D = ArgDecls.back();
         if (D->getType().getTypeExpr())
            return D->getType().getTypeExpr()->getSourceLoc();

         return D->getSourceLoc();
      }

      return Cand.getSourceLoc();
   }

   if (args.size() > idx)
      return args[idx]->getSourceLoc();

   return Cand.getSourceLoc();
}

static QualType getArgumentType(CandidateSet::Candidate &Cand)
{
   unsigned idx = Cand.Data1;
   if (!Cand.isAnonymousCandidate() && Cand.getFunc()->isNonStaticMethod())
      ++idx;

   if (!Cand.isAnonymousCandidate()) {
      auto ArgDecls = Cand.getFunc()->getArgs();

      // variadic template arguments might create more arguments
      if (ArgDecls.size() > idx) {
         return ArgDecls[idx]->getType();
      }

      return ArgDecls.back()->getType();
   }

   return Cand.getFunctionType()->getParamTypes()[idx];
}

static QualType getArgumentType(CandidateSet::Candidate &Cand,
                                ArrayRef<Expression*> args) {
   unsigned idx = Cand.Data1;
   if (!Cand.isAnonymousCandidate() && Cand.getFunc()->isNonStaticMethod())
      ++idx;

   // variadic template arguments might create more arguments
   if (args.size() > idx) {
      return args[idx]->getExprType();
   }

   return args.back()->getExprType();
}

static void diagnoseCandidate(SemaPass &SP,
                              CandidateSet &CandSet,
                              CandidateSet::Candidate &Cand,
                              ArrayRef<Expression*> args,
                              ArrayRef<Expression*> templateArgs,
                              Statement *Caller) {
   bool IncludesSelf = false;
   if (!Cand.isAnonymousCandidate()) {
      if (auto *I = dyn_cast<InitDecl>(Cand.getFunc())) {
         IncludesSelf = I->isBaseInitializer();
      }
      else if (isa<MethodDecl>(Cand.getFunc())) {
         IncludesSelf = true;
      }
   }

   switch (Cand.FR) {
   case CandidateSet::None: llvm_unreachable("found a matching call!");
   case CandidateSet::IsInvalid:
   case CandidateSet::IsDependent:
      // diagnostics were already emitted for the invalid decl; we don't
      // know whether this candidate would have been valid had the
      // declaration not contained errors
      break;
   case CandidateSet::TooFewArguments: {
      auto &TemplateArgs = Cand.InnerTemplateArgs;

      auto GivenArgAmt = Cand.Data1;
      auto NeededArgAmt = Cand.Data2;

      if (TemplateArgs.isInferred()) {
         SP.diagnose(Caller, note_too_few_arguments_inferred,
                     NeededArgAmt, GivenArgAmt,
                     TemplateArgs.toString('\0', '\0', true),
                     Cand.getSourceLoc());
      }
      else {
         SP.diagnose(Caller, note_too_few_arguments, NeededArgAmt,
                     GivenArgAmt, Cand.getSourceLoc(), false);
      }

      break;
   }
   case CandidateSet::TooManyArguments: {
      auto &TemplateArgs = Cand.InnerTemplateArgs;

      auto GivenArgAmt = Cand.Data1;
      auto NeededArgAmt = Cand.Data2;

      if (TemplateArgs.isInferred()) {
         SP.diagnose(Caller, note_too_many_arguments_inferred,
                     NeededArgAmt, GivenArgAmt,
                     TemplateArgs.toString('\0', '\0', true),
                     Cand.getSourceLoc());
      }
      else {
         SP.diagnose(Caller, note_too_many_arguments, NeededArgAmt,
                     GivenArgAmt, Cand.getSourceLoc(), false);
      }

      break;
   }
   case CandidateSet::IncompatibleArgument: {
      QualType neededTy = QualType::getFromOpaquePtr(
         reinterpret_cast<void*>(Cand.Data3));

      QualType givenTy = QualType::getFromOpaquePtr(
         reinterpret_cast<void*>(Cand.Data2));

      SourceLocation loc = getArgumentLoc(Cand, args);
      auto &TemplateArgs = Cand.InnerTemplateArgs;
      SP.QC.SubstTemplateParamTypesNonFinal(neededTy, neededTy, TemplateArgs,
                                      Caller->getSourceRange());

      auto ArgNo = Cand.Data1 + 1;
      if (isa<MethodDecl>(Cand.getFunc()) && !Cand.getFunc()->isCompleteInitializer())
         --ArgNo;

      if (TemplateArgs.isInferred()) {
         SP.diagnose(Caller, note_cand_no_implicit_conv_inferred,
                     diag::opt::show_constness,
                     givenTy, neededTy, ArgNo,
                     TemplateArgs.toString('\0', '\0', true), loc);
      }
      else {
         SP.diagnose(Caller, note_cand_no_implicit_conv,
                     diag::opt::show_constness, givenTy, neededTy,
                     ArgNo, false, loc);
      }

      break;
   }
   case CandidateSet::IncompatibleLabel: {
      auto ArgIdx = Cand.Data1;
      auto *GivenLabel = (IdentifierInfo*)Cand.Data2;

      SourceLocation loc = getArgumentLoc(Cand, args);
      auto ArgNo = ArgIdx + 1;
      if (IncludesSelf)
         --ArgNo;

      IdentifierInfo *NeededLabel = nullptr;
      if (!Cand.isAnonymousCandidate() && ArgIdx < Cand.getFunc()->getArgs().size()){
         NeededLabel = Cand.getFunc()->getArgs()[ArgIdx]->getLabel();
      }

      SP.diagnose(Caller, note_cand_label, NeededLabel, ArgNo,
                  GivenLabel, loc);

      break;
   }
   case CandidateSet::ArgumentRequiresRef: {
      SourceLocation loc = getArgumentLoc(Cand, args);
      bool IsMutable = false;
      bool IsPointer = false;

      QualType Ty = getArgumentType(Cand);
      if (Ty->isMutablePointerType()) {
         IsMutable = true;
         IsPointer = true;
      }
      else if (!Ty->isNonMutableReferenceType()) {
         IsMutable = true;
      }

      auto ArgNo = Cand.Data1 + 1;
      if (IncludesSelf)
         --ArgNo;

      SP.diagnose(Caller, note_candidate_requires_ref, IsMutable, IsPointer,
                  ArgNo, loc);

      break;
   }
   case CandidateSet::IncompatibleReturnType: {
      auto needed = QualType::getFromOpaquePtr(
         reinterpret_cast<void*>(Cand.Data1));
      auto given = QualType::getFromOpaquePtr(
         reinterpret_cast<void*>(Cand.Data2));

      SP.diagnose(Caller, note_cand_invalid_return, given, needed,
                  Cand.getSourceLoc());

      break;
   }
   case CandidateSet::CouldNotInferArgumentType: {
      SourceLocation loc = getArgumentLoc(Cand, args);
      auto ArgNo = Cand.Data1 + 1;
      if (IncludesSelf)
         --ArgNo;

      SP.diagnose(Caller, note_candidate_requires_context,
                  ArgNo, loc);

      break;
   }
   case CandidateSet::IncompatibleSelfArgument: {
      auto needed = QualType::getFromOpaquePtr(
         reinterpret_cast<void*>(Cand.Data1));
      auto given = QualType::getFromOpaquePtr(
         reinterpret_cast<void*>(Cand.Data2));

      SP.diagnose(Caller, note_cand_invalid_self, needed, given,
                  Cand.getSourceLoc());

      break;
   }
   case CandidateSet::FailedConstraint: {
      llvm::SmallString<128> Str;
      auto Constraint = reinterpret_cast<ast::Expression*>(Cand.Data1);

      if (!tryStringifyConstraint(Str, Constraint)) {
         Str = "failed constraint";
      }

      auto &TemplateArgs = Cand.InnerTemplateArgs;
      if (TemplateArgs.isInferred()) {
         SP.diagnose(Caller, note_cand_failed_constraint_inferred,
                     Str.str(),
                     TemplateArgs.toString('\0', '\0', true),
                     Cand.getSourceLoc());
      }
      else {
         SP.diagnose(Caller, note_cand_failed_constraint, Str.str(),
                     Cand.getSourceLoc());
      }

      SP.diagnose(note_constraint_here, Constraint->getSourceRange());

      break;
   }
   case CandidateSet::MustBeStatic: {
      SP.diagnose(Caller, note_method_must_be_static,
                  Cand.getSourceLoc());

      break;
   }
   case CandidateSet::MutatingOnConstSelf: {
      SP.diagnose(Caller, note_candidate_is_mutating, 0,
                  Cand.getSourceLoc());

      break;
   }
   case CandidateSet::MutatingOnRValueSelf: {
      SP.diagnose(Caller, note_candidate_is_mutating, 1,
                  Cand.getSourceLoc());

      break;
   }
   case CandidateSet::CouldNotInferTemplateArg: {
      auto TP = reinterpret_cast<TemplateParamDecl*>(Cand.Data1);
      SP.diagnose(Caller, note_could_not_infer_template_arg,
                  TP->getName(), Cand.getFunc()->getSourceLoc());
      SP.diagnose(note_declared_here, TP->getSourceLoc());

      break;
   }
   case CandidateSet::ConflictingInferredArg: {
      auto Param = reinterpret_cast<TemplateParamDecl*>(Cand.Data2);

      if (Param->isTypeName()) {
         auto conflictingTy = QualType::getFromOpaquePtr(
            reinterpret_cast<void*>(Cand.Data1));

         auto templateArg = Cand.InnerTemplateArgs.getArgForParam(Param);
         assert(templateArg && "bad diagnostic data");

         string name = Param->getName();
         if (templateArg->isVariadic()) {
            name += "[";
            name += std::to_string(templateArg->getVariadicArgs().size()
                                   - 1);
            name += "]";

            templateArg = &templateArg->getVariadicArgs().back();
         }

         SP.diagnose(Caller, note_inferred_template_arg_conflict,
                     0 /*types*/, templateArg->getType(), conflictingTy,
                     name, templateArg->getLoc());

         SP.diagnose(Caller, note_template_parameter_here,
                     Param->getSourceLoc());
      }
      else {
         llvm_unreachable("TODO");
      }

      break;
   }
   case CandidateSet::TooManyTemplateArgs: {
      auto neededSize = Cand.getFunc()->getTemplateParams().size();
      auto givenSize  = templateArgs.size();

      SP.diagnose(Caller, note_too_many_template_args, neededSize,
                  givenSize, Cand.getSourceLoc());

      break;
   }
   case CandidateSet::IncompatibleTemplateArgKind: {
      unsigned diagSelect = unsigned(Cand.Data1);
      unsigned select1    = diagSelect & 0x3u;
      unsigned select2    = (diagSelect >> 2u) & 0x3u;

      auto Param = reinterpret_cast<TemplateParamDecl*>(Cand.Data2);
      SP.diagnose(Caller, note_template_arg_kind_mismatch, select2,
                  select1, Param->getIndex() + 1,
                  Cand.InnerTemplateArgs.getArgForParam(Param)->getLoc());

      SP.diagnose(Caller, note_template_parameter_here,
                  Param->getSourceLoc());

      break;
   }
   case CandidateSet::IncompatibleTemplateArgVal: {
      auto givenTy = QualType::getFromOpaquePtr(
         reinterpret_cast<void*>(Cand.Data1));
      auto P = reinterpret_cast<const TemplateParamDecl*>(Cand.Data2);

      auto neededTy = P->getValueType();
      SP.diagnose(Caller, note_template_arg_type_mismatch,
                  neededTy, P->getIndex() + 1, givenTy,
                  templateArgs[0]->getSourceRange());

      SP.diagnose(Caller, note_template_parameter_here,
                  P->getSourceLoc());

      break;
   }
   case CandidateSet::TemplateArgCovarianceError: {
      QualType Given = QualType::getFromOpaquePtr((void*)Cand.Data1);
      auto Param = reinterpret_cast<TemplateParamDecl*>(Cand.Data2);

      auto templateArg = Cand.InnerTemplateArgs.getArgForParam(Param);
      assert(templateArg && "bad diagnostic data");

      QualType Covar = Param->getCovariance();
      SP.diagnose(Caller, note_template_arg_covariance,
                  Covar->isClass(), Covar,
                  Param->getDeclName(), Given, templateArg->getLoc());

      SP.diagnose(Caller, note_template_parameter_here,
                  Param->getSourceLoc());

      break;
   }
   case CandidateSet::RecursivelyDependent: {
      SP.diagnose(note_generic_note,
                  "candidate cannot be referred to here",
                  Cand.getFunc()->getSourceLoc());
      break;
   }
   }
}

static void diagnoseAnonymousCandidate(SemaPass &SP,
                                       CandidateSet &CandSet,
                                       CandidateSet::Candidate &Cand,
                                       ArrayRef<Expression*> args,
                                       ArrayRef<Expression*> templateArgs,
                                       Statement *Caller) {
   auto CandLoc = Cand.getSourceLoc();
   switch (Cand.FR) {
   default: llvm_unreachable("should not happen on anonymous call");
   case CandidateSet::TooFewArguments:
      SP.diagnose(Caller, err_too_few_args_for_call,
                  Cand.getFunctionType()->getParamTypes().size(),
                  args.size(), CandLoc);

      break;
   case CandidateSet::TooManyArguments:
      SP.diagnose(Caller, err_too_many_args_for_call,
                  Cand.getFunctionType()->getParamTypes().size(),
                  args.size(), CandLoc);

      break;
   case CandidateSet::ArgumentRequiresRef: {
      SourceLocation loc = args[Cand.Data1]->getSourceLoc();
      bool IsMutable = false;
      bool IsPointer = false;

      QualType Ty = getArgumentType(Cand, args);
      if (Ty->isMutablePointerType()) {
         IsMutable = true;
         IsPointer = true;
      }
      else if (Ty->isMutableReferenceType()) {
         IsMutable = true;
      }

      SP.diagnose(Caller, note_candidate_requires_ref, IsMutable, IsPointer,
                  Cand.Data1 + 1, loc);

      break;
   }
   case CandidateSet::IncompatibleLabel: {
      auto ArgIdx = Cand.Data1;
      auto *GivenLabel = (IdentifierInfo*)Cand.Data2;

      IdentifierInfo *NeededLabel = nullptr;
      SP.diagnose(Caller, note_cand_label, NeededLabel, ArgIdx + 1,
                  GivenLabel, args[ArgIdx]->getSourceRange());

      break;
   }
   case CandidateSet::IncompatibleArgument: {
      auto idx = Cand.Data1;
      auto givenTy = args[idx]->getExprType();
      auto neededTy = Cand.getFunctionType()->getParamTypes()[idx];

      SP.diagnose(Caller, err_no_implicit_conv, givenTy, neededTy,
                  args[idx]->getSourceRange());

      break;
   }
   case CandidateSet::IncompatibleReturnType: {
      auto needed = QualType::getFromOpaquePtr(
         reinterpret_cast<void*>(Cand.Data1));
      auto given = QualType::getFromOpaquePtr(
         reinterpret_cast<void*>(Cand.Data2));

      SP.diagnose(Caller, note_cand_invalid_return, given, needed,
                  CandLoc);

      break;
   }
   case CandidateSet::CouldNotInferArgumentType: {
      SP.diagnose(Caller, note_candidate_requires_context, Cand.Data1 + 1,
                  CandLoc);

      break;
   }
   }
}

void CandidateSet::diagnoseFailedCandidates(SemaPass &SP,
                                            Expression *SelfVal,
                                            ArrayRef<Expression*> constArgs,
                                            ArrayRef<Expression*> constTemplateArgs,
                                            Statement *Caller,
                                            SourceLocation OpLoc) {
   if (InvalidCand) {
      Caller->setIsInvalid(true);
      return;
   }

   std::vector<Expression*> args = constArgs.vec();
   std::vector<Expression*> templateArgs = constTemplateArgs.vec();

   // Resolve all arguments ambiguous types.
   bool issuedDiag = false;
   for (auto *&arg : args) {
      if (!arg->getExprType()) {
         auto result = SP.typecheckExpr(arg);
         if (!result) {
            issuedDiag = true;
            continue;
         }

         arg = result.get();
      }
   }
   for (auto *&arg : templateArgs) {
      if (!arg->getExprType()) {
         auto result = SP.typecheckExpr(arg);
         if (!result) {
            issuedDiag = true;
            continue;
         }

         arg = result.get();
      }
   }

   if (issuedDiag) {
      return;
   }

   DeclarationName FuncName;
   for (auto &Cand : Candidates) {
      if (!Cand.isAnonymousCandidate()) {
         FuncName = Cand.getFunc()->getDeclName();
         break;
      }
   }

   bool Diagnosed = false;
   if (FuncName) {
      auto Kind = FuncName.getKind();
      if (Kind == DeclarationName::InfixOperatorName) {
         Diagnosed = true;
         SP.diagnose(Caller, err_binop_not_applicable,
                     FuncName.getInfixOperatorName()->getIdentifier(),
                     SelfVal ? SelfVal->getExprType() : args[0]->getExprType(),
                     SelfVal ? args[0]->getExprType() : args[1]->getExprType(),
                     OpLoc ? OpLoc : Caller->getSourceLoc());
      }
      else if (Kind == DeclarationName::PrefixOperatorName
      || Kind == DeclarationName::PostfixOperatorName) {
         Diagnosed = true;
         bool IsPostfix = Kind == DeclarationName::PostfixOperatorName;
         SP.diagnose(Caller, err_unary_op_not_applicable, IsPostfix,
                     FuncName, 0,
                     SelfVal ? SelfVal->getExprType() : args[0]->getExprType(),
                     OpLoc ? OpLoc : Caller->getSourceLoc());
      }
      else if (Kind == DeclarationName::ConstructorName) {
         Diagnosed = true;
         enum : int { Matching = 0, Accessible = 1 };
         auto R = FuncName.getConstructorType()->getRecord();
         SP.diagnose(Caller, err_no_matching_initializer,
                     Candidates.empty() ? Accessible : Matching,
                     R, R->getFullName(),
                     Caller->getSourceRange());
      }
   }

   if (!Diagnosed) {
      if (Candidates.empty()) {
         SP.diagnose(Caller, err_func_not_found, Caller->getSourceRange(),
                     0, FuncName);

         return;
      }

      SP.diagnose(Caller, err_no_matching_call, Caller->getSourceRange(),
                  (bool)FuncName, 0, FuncName);
   }

   SmallVector<Expression*, 4> ArgsWithSelf;
   ArgsWithSelf.reserve(args.size() + 1);
   ArgsWithSelf.push_back(SelfVal);
   ArgsWithSelf.append(args.begin(), args.end());

   for (auto &Cand : Candidates) {
      if (Cand.isAnonymousCandidate()) {
         diagnoseAnonymousCandidate(SP, *this, Cand, ArgsWithSelf, templateArgs,
                                    Caller);
      }
      else {
         auto *Fn = Cand.getFunc();
         ArrayRef<Expression*> ArgValues;

         if (isa<MethodDecl>(Fn) && !Fn->isCompleteInitializer()) {
            ArgValues = ArgsWithSelf;
         }
         else {
            ArgValues = args;
         }

         diagnoseCandidate(SP, *this, Cand, ArgValues, templateArgs, Caller);
      }
   }
}

void
CandidateSet::diagnoseAmbiguousCandidates(SemaPass &SP, Statement *Caller)
{
   DeclarationName FuncName;
   for (auto &Cand : Candidates) {
      if (!Cand.isAnonymousCandidate()) {
         FuncName = Cand.getFunc()->getDeclName();
         break;
      }
   }

   assert(FuncName && "unnamed call can't be ambiguous!");
   SP.diagnose(Caller, err_ambiguous_call, 0, FuncName,
               Caller->getSourceRange());

   for (auto &Cand : Candidates) {
      if (!Cand || Cand.ConversionPenalty != BestConversionPenalty)
         continue;

      if (Cand.isAnonymousCandidate()) {
         SP.diagnose(note_builtin_candidate, /*operator*/ true,
                     makeFakeSourceLoc(*this, FuncName, Cand));
      }
      else {
         SP.diagnose(Caller, note_candidate_here, Cand.getSourceLoc());
      }
   }
}

void CandidateSet::diagnoseAlias(SemaPass &SP,
                                 DeclarationName AliasName,
                                 llvm::ArrayRef<Expression *> templateArgs,
                                 Statement *Caller) {
   if (InvalidCand)
      return;

   if (Status == Ambiguous) {
      SP.diagnose(Caller, err_ambiguous_call, 2 /*alias*/, AliasName,
                  Caller->getSourceRange());

      for (auto &Cand : Candidates) {
         if (!Cand.isValid())
            continue;

         SP.diagnose(Caller, note_candidate_here, Cand.getSourceLoc());
      }

      return;
   }

   assert(Status != Success && "diagnosing successful candidate set!");

   SP.diagnose(Caller, err_no_matching_call, Caller->getSourceRange(),
               (bool)AliasName, 2, AliasName);

   for (auto &Cand : Candidates) {
      switch (Cand.FR) {
      case None:
         llvm_unreachable("found a matching alias!");
      case IsInvalid:
      case IsDependent:
         // diagnostics were already emitted for the invalid decl. we don't
         // know whether this candidate would have been valid, had the
         // declaration not contained errors
         break;
      case TooFewArguments:
      case TooManyArguments:
      case IncompatibleArgument:
      case IncompatibleSelfArgument:
      case MustBeStatic:
      case MutatingOnConstSelf:
      case MutatingOnRValueSelf:
      case CouldNotInferArgumentType:
      case ArgumentRequiresRef:
      case IncompatibleLabel:
      case IncompatibleReturnType:
         llvm_unreachable("should be impossible on alias candidate set!");
      case FailedConstraint: {
         llvm::SmallString<128> Str;
         auto Constraint = reinterpret_cast<ast::Expression*>(Cand.Data1);

         if (!tryStringifyConstraint(Str, Constraint)) {
            Str = "failed constraint";
         }

         auto &TemplateArgs = Cand.InnerTemplateArgs;
         if (TemplateArgs.isInferred()) {
            SP.diagnose(Caller, note_cand_failed_constraint_inferred,
                        Str.str(),
                        TemplateArgs.toString('\0', '\0', true),
                        Cand.getAlias()->getSourceLoc());
         }
         else {
            SP.diagnose(Caller, note_cand_failed_constraint, Str.str(),
                        Cand.getAlias()->getSourceLoc());
         }

         SP.diagnose(note_constraint_here, Constraint->getSourceRange());

         break;
      }
      case CouldNotInferTemplateArg: {
         auto TP = reinterpret_cast<TemplateParamDecl*>(Cand.Data1);
         SP.diagnose(Caller, note_could_not_infer_template_arg,
                     TP->getName(), TP->getSourceLoc());

         break;
      }
      case ConflictingInferredArg: {
         auto idx = Cand.Data2;
         auto Param = Cand.getAlias()->getTemplateParams()[idx];

         if (Param->isTypeName()) {
            auto conflictingTy = QualType::getFromOpaquePtr(
               reinterpret_cast<void*>(Cand.Data1));
            auto templateArg =
               Cand.InnerTemplateArgs.getArgForParam(Param);

            string name = Param->getName();
            if (templateArg->isVariadic()) {
               name += "[";
               name += std::to_string(templateArg->getVariadicArgs().size()
                                      - 1);
               name += "]";

               templateArg = &templateArg->getVariadicArgs().back();
            }

            SP.diagnose(Caller, note_inferred_template_arg_conflict,
                        0 /*types*/, templateArg->getType(), conflictingTy,
                        name, templateArg->getLoc());

            SP.diagnose(Caller, note_template_parameter_here,
                        Param->getSourceLoc());
         }
         else {
            llvm_unreachable("TODO");
         }

         break;
      }
      case TooManyTemplateArgs: {
         auto neededSize = Cand.getAlias()->getTemplateParams().size();
         auto givenSize  = templateArgs.size();

         SP.diagnose(Caller, note_too_many_template_args, neededSize,
                     givenSize, Cand.getSourceLoc());

         break;
      }
      case IncompatibleTemplateArgKind: {
         unsigned diagSelect = unsigned(Cand.Data1);
         unsigned select1    = diagSelect & 0x3u;
         unsigned select2    = (diagSelect >> 2u) & 0x3u;

         auto Param = reinterpret_cast<TemplateParamDecl*>(Cand.Data2);
         SP.diagnose(Caller, note_template_arg_kind_mismatch, select1,
                     select2, 0,
                     templateArgs[0]->getSourceRange());

         SP.diagnose(Caller, note_template_parameter_here,
                     Param->getSourceLoc());

         break;
      }
      case IncompatibleTemplateArgVal: {
         auto givenTy = QualType::getFromOpaquePtr(
            reinterpret_cast<void*>(Cand.Data1));

         auto Param = reinterpret_cast<TemplateParamDecl*>(Cand.Data2);
         SP.diagnose(Caller, note_template_arg_type_mismatch,
                     Param->getValueType(), 0, givenTy,
                     templateArgs[0]->getSourceRange());

         SP.diagnose(Caller, note_template_parameter_here,
                     Param->getSourceRange());

         break;
      }
      case TemplateArgCovarianceError: {
         QualType Given = QualType::getFromOpaquePtr((void*)Cand.Data1);
         auto Param = reinterpret_cast<TemplateParamDecl*>(Cand.Data2);

         auto templateArg = Cand.InnerTemplateArgs.getArgForParam(Param);
         assert(templateArg && "bad diagnostic data");

         QualType Covar = Param->getCovariance();
         SP.diagnose(Caller, note_template_arg_covariance,
                     Covar->isClass(), Covar,
                     Param->getDeclName(), Given, templateArg->getLoc());

         SP.diagnose(Caller, note_template_parameter_here,
                     Param->getSourceLoc());

         break;
      }
      case RecursivelyDependent: {
         SP.diagnose(note_generic_note,
                     "candidate cannot be referred to here",
                     Cand.getAlias()->getSourceLoc());

         break;
      }
      }
   }
}

} // namespace cdot