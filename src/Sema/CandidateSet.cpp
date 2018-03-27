//
// Created by Jonas Zell on 15.02.18.
//

#include "CandidateSet.h"

#include "AST/Expression.h"
#include "AST/Decl.h"
#include "SemaPass.h"
#include "Support/Casting.h"

#include <llvm/ADT/SmallString.h>
#include <AST/PrettyPrinter.h>

using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::support;

namespace cdot {

FunctionType* CandidateSet::Candidate::getFunctionType() const
{
   if (auto fn = BuiltinCandidate.FuncTy)
      return fn;

   return Func->getFunctionType();
}

SourceLocation CandidateSet::Candidate::getSourceLoc() const
{
   if (!Func)
      return SourceLocation();

   return Func->getSourceLoc();
}

PrecedenceGroup CandidateSet::Candidate::getPrecedenceGroup() const
{
   if (!Func)
      return BuiltinCandidate.precedenceGroup;

   return Func->getOperator().getPrecedenceGroup();
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
   assert(Cand.isBuiltinCandidate() && "not a builtin candidate!");

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

void CandidateSet::diagnoseFailedCandidates(ast::SemaPass &SP,
                                            DeclarationName funcName,
                                            llvm::ArrayRef<ast::Expression*> args,
                                            llvm::ArrayRef<ast::Expression*> templateArgs,
                                            ast::Statement *Caller,
                                            bool OperatorLookup,
                                            SourceLocation OpLoc) {
   if (Candidates.size() == 1 && Candidates.front().FR == IsInvalid) {
      return;
   }

   if (OperatorLookup) {
      auto Kind = funcName.getDeclarationKind();
      if (Kind == DeclarationName::InfixOperatorName) {
         // diagnose '=' specially
         if (funcName.getInfixOperatorName()->isStr("=")) {
            SP.diagnose(Caller, err_assign_type_mismatch,
                        args[1]->getExprType(),
                        args[0]->getExprType()->stripReference(),
                        OpLoc ? OpLoc : Caller->getSourceLoc(),
                        args[0]->getSourceRange(), args[1]->getSourceRange());
         }
         else {
            SP.diagnose(Caller, err_binop_not_applicable,
                        funcName.getInfixOperatorName()->getIdentifier(),
                        args[0]->getExprType(), args[1]->getExprType(),
                        OpLoc ? OpLoc : Caller->getSourceLoc(),
                        args[0]->getSourceRange(), args[1]->getSourceRange());
         }
      }
      else {
         bool IsPostfix = Kind == DeclarationName::PostfixOperatorName;
         SP.diagnose(Caller, err_unary_op_not_applicable, IsPostfix,
                     (IsPostfix
                      ? funcName.getPostfixOperatorName()
                      : funcName.getPrefixOperatorName())->getIdentifier(),
                     0, args[0]->getExprType(),
                     OpLoc ? OpLoc : Caller->getSourceLoc(),
                     args[0]->getSourceRange());
      }
   }
   else {
      SP.diagnose(Caller, err_no_matching_call, Caller->getSourceRange(),
                  0, funcName);
   }

   for (auto &Cand : Candidates) {
      switch (Cand.FR) {
      case None: llvm_unreachable("found a matching call!");
      case IsInvalid:
         // diagnostics were already emitted for the invalid decl. we don't
         // know whether this candidate would have been valid, had the
         // declaration not contained errors
         break;
      case TooFewArguments:
         if (Cand.TemplateArgs.isInferred()) {
            SP.diagnose(Caller, note_too_few_arguments_inferred,
                        Cand.TemplateArgs.toString('\0', '\0', true),
                        Cand.Data2, Cand.Data1, Cand.getSourceLoc());
         }
         else if (Cand.isBuiltinCandidate()) {
            SP.diagnose(Caller, note_too_few_arguments,
                        Cand.Data2, Cand.Data1, Cand.getSourceLoc(),
                        true, makeFakeSourceLoc(*this, funcName, Cand));
         }
         else {
            SP.diagnose(Caller, note_too_few_arguments, Cand.Data2,
                        Cand.Data1, Cand.getSourceLoc(), false);
         }

         break;
      case TooManyArguments:
         if (Cand.TemplateArgs.isInferred()) {
            SP.diagnose(Caller, note_too_many_arguments_inferred,
                        Cand.TemplateArgs.toString('\0', '\0', true),
                        Cand.Data2, Cand.Data1, Cand.getSourceLoc());
         }
         else if (Cand.isBuiltinCandidate()) {
            SP.diagnose(Caller, note_too_many_arguments, Cand.Data2,
                        Cand.Data1, Cand.getSourceLoc(),
                        true, makeFakeSourceLoc(*this, funcName, Cand));
         }
         else {
            SP.diagnose(Caller, note_too_many_arguments, Cand.Data2,
                        Cand.Data1, Cand.getSourceLoc(), false);
         }

         break;
      case IncompatibleArgument: {
         auto FTy = Cand.getFunctionType();
         auto neededTy = FTy->getParamTypes()[Cand.Data1];
         auto givenTy = IncludesSelfArgument
                           && dyn_cast_or_null<MethodDecl>(Cand.Func)
                        ? args[Cand.Data1 + 1]->getExprType()
                        : args[Cand.Data1]->getExprType();

         SourceLocation loc;
         if (Cand.Func) {
            auto &ArgDecls = Cand.Func->getArgs();

            // variadic template arguments might create more arguments
            if (ArgDecls.size() > Cand.Data1) {
               loc = ArgDecls[Cand.Data1]->getSourceLoc();
            }
            else {
               loc = ArgDecls.back()->getSourceLoc();
            }
         }
         else {
            loc = Cand.getSourceLoc();
         }

         if (Cand.TemplateArgs.isInferred()) {
            SP.diagnose(Caller, note_cand_no_implicit_conv_inferred,
                        diag::opt::show_constness,
                        givenTy, neededTy, Cand.Data1 + 1,
                        Cand.TemplateArgs.toString('\0', '\0', true), loc);
         }
         else if (Cand.isBuiltinCandidate()) {
            SP.diagnose(Caller, note_cand_no_implicit_conv,
                        diag::opt::show_constness, givenTy,
                        neededTy, Cand.Data1 + 1, true,
                        makeFakeSourceLoc(*this, funcName, Cand));
         }
         else {
            SP.diagnose(Caller, note_cand_no_implicit_conv,
                        diag::opt::show_constness, givenTy,
                        neededTy, Cand.Data1 + 1, loc, false);
         }

         break;
      }
      case IncompatibleSelfArgument: {
         auto needed = QualType::getFromOpaquePtr(
            reinterpret_cast<void*>(Cand.Data1));
         auto given = QualType::getFromOpaquePtr(
            reinterpret_cast<void*>(Cand.Data2));

         SP.diagnose(Caller, note_cand_invalid_self, needed, given);

         break;
      }
      case FailedConstraint: {
         llvm::SmallString<128> Str;
         auto Constraint = reinterpret_cast<ast::Expression*>(Cand.Data1);

         if (!tryStringifyConstraint(Str, Constraint)) {
            Str = "failed constraint";
         }

         if (Cand.TemplateArgs.isInferred()) {
            SP.diagnose(Caller, note_cand_failed_constraint_inferred,
                        Str.str(),
                        Cand.TemplateArgs.toString('\0', '\0', true));
         }
         else {
            SP.diagnose(Caller, note_cand_failed_constraint, Str.str());
         }

         SP.diagnose(Constraint, note_constraint_here);

         break;
      }
      case MustBeStatic: {
         SP.diagnose(Caller, note_method_must_be_static,
                     Cand.getSourceLoc());

         break;
      }
      case MutatingOnConstSelf: {
         SP.diagnose(Caller, note_candidate_is_mutating, 0,
                     Cand.getSourceLoc());

         SP.noteConstantDecl(args.front());

         break;
      }
      case MutatingOnRValueSelf: {
         SP.diagnose(Caller, note_candidate_is_mutating, 1,
                     Cand.getSourceLoc());

         break;
      }
      case CouldNotInferTemplateArg: {
         auto TP = reinterpret_cast<TemplateParamDecl*>(Cand.Data1);
         SP.diagnose(Caller, note_could_not_infer_template_arg,
                     TP->getName(), Cand.getSourceLoc());

         break;
      }
      case ConflictingInferredArg: {
         auto idx = Cand.Data2;
         auto Param = Cand.Func->getTemplateParams()[idx];

         if (Param->isTypeName()) {
            auto conflictingTy = QualType::getFromOpaquePtr(
               reinterpret_cast<void*>(Cand.Data1));
            auto templateArg =
               Cand.TemplateArgs.getNamedArg(Param->getName());

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
         auto neededSize = Cand.Func->getTemplateParams().size();
         auto givenSize  = templateArgs.size();

         SP.diagnose(Caller, note_too_many_template_args, neededSize,
                     givenSize, Cand.getSourceLoc());

         break;
      }
      case IncompatibleTemplateArgKind: {
         unsigned diagSelect = unsigned(Cand.Data1);
         unsigned select1    = diagSelect & 0x3u;
         unsigned select2    = (diagSelect >> 2u) & 0x3u;

         auto idx = Cand.Data2;
         auto Param = Cand.Func->getTemplateParams()[idx];

         SP.diagnose(Caller, note_template_arg_kind_mismatch, select1,
                     select2, idx + 1,
                     Cand.TemplateArgs.getNamedArg(Param->getName())
                         ->getLoc());

         SP.diagnose(Caller, note_template_parameter_here,
                     Param->getSourceLoc());

         break;
      }
      case IncompatibleTemplateArgVal: {
         auto givenTy = QualType::getFromOpaquePtr(
            reinterpret_cast<void*>(Cand.Data1));
         auto idx = Cand.Data2;

         auto Param = Cand.Func->getTemplateParams()[idx];
         auto neededTy = Param->getValueType();

         SP.diagnose(Caller, note_template_arg_type_mismatch, givenTy,
                     idx + 1, neededTy);

         SP.diagnose(Caller, note_template_parameter_here,
                     Param->getSourceLoc());

         break;
      }
      }
   }
}

void CandidateSet::diagnoseAnonymous(SemaPass &SP,
                                     DeclarationName funcName,
                                     llvm::ArrayRef<ast::Expression *> args,
                                     Statement *Caller) {
   assert(Candidates.size() == 1 && "not an anonymous call!");

   auto &Cand = Candidates.front();
   switch (Cand.FR) {
   default: llvm_unreachable("should not happen on anonymous call");
   case TooFewArguments:
   case TooManyArguments: {
      auto diag = Cand.FR == TooFewArguments ? err_too_few_args_for_call
                                             : err_too_many_args_for_call;

      SP.diagnose(Caller, diag,
                  Cand.BuiltinCandidate.FuncTy->getParamTypes().size(),
                  args.size());

      break;
   }
   case IncompatibleArgument: {
      auto idx = Cand.Data1;
      auto givenTy = args[idx]->getExprType();
      auto neededTy = Cand.BuiltinCandidate.FuncTy->getParamTypes()[idx];

      SP.diagnose(Caller, err_no_implicit_conv, givenTy, neededTy);
      break;
   }
   }
}

void
CandidateSet::diagnoseAmbiguousCandidates(SemaPass &SP,
                                          DeclarationName funcName,
                                          llvm::ArrayRef<Expression*> args,
                                          llvm::ArrayRef<Expression*> templateArgs,
                                          Statement *Caller,
                                          bool OperatorLookup,
                                          SourceLocation OpLoc) {
   SP.diagnose(Caller, err_ambiguous_call, 0, funcName);
   for (auto &Cand : Candidates) {
      if (!Cand || Cand.ConversionPenalty != BestConversionPenalty)
         continue;

      if (Cand.isBuiltinCandidate()) {
         SP.diagnose(note_builtin_candidate, /*operator*/ true,
                     makeFakeSourceLoc(*this, funcName, Cand));
      }
      else {
         SP.diagnose(Caller, note_candidate_here, Cand.getSourceLoc());
      }
   }
}

} // namespace cdot