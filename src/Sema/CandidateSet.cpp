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

ast::PrecedenceGroupDecl *CandidateSet::Candidate::getPrecedenceGroup() const
{
   if (isBuiltinCandidate())
      return precedenceGroup;

   return Func->getPrecedenceGroup();
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

static SourceLocation getArgumentLoc(CandidateSet::Candidate &Cand,
                                     llvm::ArrayRef<ast::Expression*> args) {
   unsigned idx = Cand.Data1;
   if (!Cand.isBuiltinCandidate() && Cand.Func->isNonStaticMethod())
      ++idx;

   if (!Cand.isBuiltinCandidate()) {
      auto ArgDecls = Cand.Func->getArgs();

      // variadic template arguments might create more arguments
      if (ArgDecls.size() > idx) {
         return ArgDecls[idx]->getSourceLoc();
      }

      if (!ArgDecls.empty())
         return ArgDecls.back()->getSourceLoc();

      return Cand.getSourceLoc();
   }

   return args[idx]->getSourceLoc();
}

static QualType getArgumentType(CandidateSet::Candidate &Cand)
{
   unsigned idx = Cand.Data1;
   if (!Cand.isBuiltinCandidate() && Cand.Func->isNonStaticMethod())
      ++idx;

   if (!Cand.isBuiltinCandidate()) {
      auto ArgDecls = Cand.Func->getArgs();

      // variadic template arguments might create more arguments
      if (ArgDecls.size() > idx) {
         return ArgDecls[idx]->getType();
      }

      return ArgDecls.back()->getType();
   }

   return Cand.getFunctionType()->getParamTypes()[idx];
}

static QualType getArgumentType(CandidateSet::Candidate &Cand,
                                llvm::ArrayRef<ast::Expression*> args) {
   unsigned idx = Cand.Data1;
   if (!Cand.isBuiltinCandidate() && Cand.Func->isNonStaticMethod())
      ++idx;

   // variadic template arguments might create more arguments
   if (args.size() > idx) {
      return args[idx]->getExprType();
   }

   return args.back()->getExprType();
}

static void diagnoseConstAssignment(SemaPass &SP,
                                    Statement *DependentExpr,
                                    SourceLocation EqualsLoc,
                                    Expression *LHS) {
   VarDecl *ConstDecl = nullptr;
   SourceRange ConstExprLoc;

   auto DeclRef = LHS;
   while (DeclRef) {
      auto Expr = DeclRef->ignoreParensAndImplicitCasts();
      if (auto Ident = dyn_cast<IdentifierRefExpr>(Expr)) {
         switch (Ident->getKind()) {
         case IdentifierKind::LocalVar:
         case IdentifierKind::GlobalVar:
         case IdentifierKind::Field:
         case IdentifierKind::StaticField:
         case IdentifierKind::FunctionArg:
            ConstDecl = Ident->getVarDecl();
            ConstExprLoc = Expr->getSourceRange();

            break;
         default:
            break;
         }

         if (ConstDecl && ConstDecl->isConst())
            break;

         DeclRef = Ident->getParentExpr();
         continue;
      }

      if (auto Subscript = dyn_cast<SubscriptExpr>(Expr)) {
         DeclRef = Subscript->getParentExpr();
         continue;
      }

      if (auto Tup = dyn_cast<TupleMemberExpr>(Expr)) {
         DeclRef = Tup->getParentExpr();
         continue;
      }

      if (auto Self = dyn_cast<SelfExpr>(Expr)) {
         if (ConstExprLoc) {
            SP.diagnose(DependentExpr, err_cannot_assign_to_property,
                        ConstExprLoc, EqualsLoc);
         }
         else {
            SP.diagnose(DependentExpr, err_cannot_assign_to_self,
                        Self->getSourceRange(),
                        EqualsLoc);
         }

         SP.diagnose(note_mark_mutating, SP.getCurrentFun()->getSourceLoc());
         return;
      }

      if (auto UnOp = dyn_cast<UnaryOperator>(Expr)) {
         if (UnOp->getKind() == op::Deref) {
            DeclRef = UnOp->getTarget();
            continue;
         }
      }

      break;
   }

   if (!ConstDecl) {
      SP.diagnose(DependentExpr, err_reassign_constant, LHS->getSourceRange(),
                  EqualsLoc);

      return;
   }

   SP.diagnose(DependentExpr, err_reassign_constant, ConstExprLoc,
               EqualsLoc);
   SP.diagnose(note_declared_const_here, ConstDecl->getDeclName(),
               ConstDecl->getSourceLoc());
}

void CandidateSet::diagnoseFailedCandidates(ast::SemaPass &SP,
                                            DeclarationName funcName,
                                            llvm::ArrayRef<ast::Expression*> args,
                                            llvm::ArrayRef<ast::Expression*> templateArgs,
                                            ast::Statement *Caller,
                                            bool OperatorLookup,
                                            SourceLocation OpLoc) {
   if (InvalidCand) {
      Caller->setIsInvalid(true);
      return;
   }

   bool IgnoreBuiltinCandidates = false;

   if (OperatorLookup) {
      auto Kind = funcName.getDeclarationKind();
      if (Kind == DeclarationName::InfixOperatorName) {
         // diagnose '=' specially
         if (funcName.getInfixOperatorName()->isStr("=")) {
            auto LHS = args[0];
            if (LHS->getExprType()->isReferenceType()
                && !LHS->getExprType()->isMutableReferenceType()) {
               diagnoseConstAssignment(SP, Caller, OpLoc, LHS);
               IgnoreBuiltinCandidates = true;
            }
            else {
               SP.diagnose(Caller, err_assign_type_mismatch,
                           args[1]->getExprType(), args[0]->getExprType(),
                           OpLoc ? OpLoc : Caller->getSourceLoc(),
                           args[0]->getSourceRange(),
                           args[1]->getSourceRange());
            }
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
   else if (funcName.getDeclarationKind() == DeclarationName::ConstructorName) {
      enum : int { Matching = 0, Accessible = 1 };
      auto R = funcName.getConstructorType()->getRecord();
      SP.diagnose(Caller, err_no_matching_initializer,
                  Candidates.empty() ? Accessible : Matching,
                  R->getSpecifierForDiagnostic(), R->getFullName(),
                  Caller->getSourceRange());
   }
   else {
      if (Candidates.empty()) {
         SP.diagnose(Caller, err_func_not_found, Caller->getSourceRange(),
                     0, funcName);

         return;
      }

      SP.diagnose(Caller, err_no_matching_call, Caller->getSourceRange(),
                  true, 0, funcName);
   }

   for (auto &Cand : Candidates) {
      if (IgnoreBuiltinCandidates && Cand.isBuiltinCandidate())
         continue;

      switch (Cand.FR) {
      case None: llvm_unreachable("found a matching call!");
      case IsInvalid:
      case IsDependent:
         // diagnostics were already emitted for the invalid decl; we don't
         // know whether this candidate would have been valid had the
         // declaration not contained errors
         break;
      case TooFewArguments: {
         auto &TemplateArgs = Cand.InnerTemplateArgs;
         if (TemplateArgs.isInferred()) {
            SP.diagnose(Caller, note_too_few_arguments_inferred,
                        TemplateArgs.toString('\0', '\0', true),
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
      }
      case TooManyArguments: {
         auto &TemplateArgs = Cand.InnerTemplateArgs;
         if (TemplateArgs.isInferred()) {
            SP.diagnose(Caller, note_too_many_arguments_inferred,
                        Cand.Data2, Cand.Data1,
                        TemplateArgs.toString('\0', '\0', true),
                        Cand.getSourceLoc());
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
      }
      case IncompatibleArgument: {
         auto FTy = Cand.getFunctionType();
         auto neededTy = FTy->getParamTypes()[Cand.Data1];

         QualType givenTy = getArgumentType(Cand, args);
         SourceLocation loc = getArgumentLoc(Cand, args);

         auto &TemplateArgs = Cand.InnerTemplateArgs;
         if (TemplateArgs.isInferred()) {
            SP.diagnose(Caller, note_cand_no_implicit_conv_inferred,
                        diag::opt::show_constness,
                        givenTy, neededTy, Cand.Data1 + 1,
                        TemplateArgs.toString('\0', '\0', true), loc);
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
                        neededTy, Cand.Data1 + 1, false, loc);
         }

         break;
      }
      case ArgumentRequiresRef: {
         SourceLocation loc = getArgumentLoc(Cand, args);
         bool IsMutable = false;
         bool IsPointer = false;

         QualType Ty = getArgumentType(Cand);
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
      case CouldNotInferArgumentType: {
         SourceLocation loc = getArgumentLoc(Cand, args);
         SP.diagnose(Caller, note_candidate_requires_context,
                     Cand.Data1 + 1, loc);

         break;
      }
      case IncompatibleSelfArgument: {
         auto needed = QualType::getFromOpaquePtr(
            reinterpret_cast<void*>(Cand.Data1));
         auto given = QualType::getFromOpaquePtr(
            reinterpret_cast<void*>(Cand.Data2));

         SP.diagnose(Caller, note_cand_invalid_self, needed, given,
                     Cand.getSourceLoc());

         break;
      }
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
                        Cand.getSourceLoc());
         }
         else {
            SP.diagnose(Caller, note_cand_failed_constraint, Str.str(),
                        Cand.getSourceLoc());
         }

         SP.diagnose(note_constraint_here, Constraint->getSourceRange());

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
                     TP->getName(), TP->getSourceLoc());

         break;
      }
      case ConflictingInferredArg: {
         auto idx = Cand.Data2;
         auto Param = Cand.Func->getTemplateParams()[idx];

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
                     Cand.InnerTemplateArgs.getArgForParam(Param)->getLoc());

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

         SP.diagnose(Caller, note_template_arg_type_mismatch,
                     neededTy, idx + 1, givenTy,
                     templateArgs[idx]->getSourceRange());

         SP.diagnose(Caller, note_template_parameter_here,
                     Param->getSourceLoc());

         break;
      }
      }
   }
}

void CandidateSet::diagnoseAnonymous(SemaPass &SP,
                                     llvm::ArrayRef<ast::Expression *> args,
                                     Statement *Caller) {
   assert(Candidates.size() == 1 && "not an anonymous call!");

   SP.diagnose(Caller, err_no_matching_call, Caller->getSourceRange(),
               false);

   auto &Cand = Candidates.front();
   switch (Cand.FR) {
   default: llvm_unreachable("should not happen on anonymous call");
   case TooFewArguments:
   case TooManyArguments: {
      auto diag = Cand.FR == TooFewArguments ? err_too_few_args_for_call
                                             : err_too_many_args_for_call;

      SP.diagnose(Caller, diag,
                  Cand.BuiltinCandidate.FuncTy->getParamTypes().size(),
                  args.size(), Caller->getSourceRange());

      break;
   }
   case ArgumentRequiresRef: {
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
   case IncompatibleArgument: {
      auto idx = Cand.Data1;
      auto givenTy = args[idx]->getExprType();
      auto neededTy = Cand.BuiltinCandidate.FuncTy->getParamTypes()[idx];

      SP.diagnose(Caller, err_no_implicit_conv, givenTy, neededTy,
                  args[idx]->getSourceRange());

      break;
   }
   case CouldNotInferArgumentType: {
      SP.diagnose(Caller, note_candidate_requires_context, Cand.Data1 + 1,
                  Cand.getSourceLoc());

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
   SP.diagnose(Caller, err_ambiguous_call, 0, funcName,
               Caller->getSourceRange());

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
               true, 2, AliasName);

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
                        Cand.Alias->getSourceLoc());
         }
         else {
            SP.diagnose(Caller, note_cand_failed_constraint, Str.str(),
                        Cand.Alias->getSourceLoc());
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
         auto Param = Cand.Alias->getTemplateParams()[idx];

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
         auto neededSize = Cand.Alias->getTemplateParams().size();
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
         auto Param = Cand.Alias->getTemplateParams()[idx];

         SP.diagnose(Caller, note_template_arg_kind_mismatch, select1,
                     select2, idx + 1,
                     Cand.InnerTemplateArgs.getArgForParam(Param)
                         ->getLoc());

         SP.diagnose(Caller, note_template_parameter_here,
                     Param->getSourceLoc());

         break;
      }
      case IncompatibleTemplateArgVal: {
         auto givenTy = QualType::getFromOpaquePtr(
            reinterpret_cast<void*>(Cand.Data1));
         auto idx = Cand.Data2;

         auto Param = Cand.Alias->getTemplateParams()[idx];
         auto neededTy = Param->getValueType();

         SP.diagnose(Caller, note_template_arg_type_mismatch,
                     neededTy, idx + 1, givenTy,
                     templateArgs[idx]->getSourceRange());

         SP.diagnose(Caller, note_template_parameter_here,
                     Param->getSourceRange());

         break;
      }
      }
   }
}

} // namespace cdot