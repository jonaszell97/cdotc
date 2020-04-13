#include "cdotc/Sema/ConstraintBuilder.h"

#include "cdotc/AST/TypeBuilder.h"
#include "cdotc/AST/TypeVisitor.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/Builtin.h"
#include "cdotc/Sema/ExpressionResolver.h"
#include "cdotc/Sema/OverloadResolver.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Support/SaveAndRestore.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::sema;
using namespace cdot::support;

using LocatorKind = ConstraintLocator::LocatorKind;
using PathElement = ConstraintLocator::PathElement;

ConstraintBuilder::ConstraintBuilder(QueryContext& QC, SourceRange Loc,
                                     llvm::raw_ostream* LogStream)
    : Sys(QC, Loc, LogStream), Sema(*QC.Sema)
{
}

namespace {

class ExprRebuilder : public StmtBuilder<ExprRebuilder> {
   /// Reference to the Sema instance.
   SemaPass& Sema;

public:
   /// Set to true iff this expression is type dependent.
   bool TypeDependent = false;
   bool EncounteredError = false;

   explicit ExprRebuilder(SemaPass& Sema) : Sema(Sema) {}

   ExprResult visitExprSequence(ExprSequence* Expr)
   {
      ExpressionResolver Resolver(Sema);
      auto Result = Resolver.resolve(Expr);

      if (!Result) {
         if (Expr->isTypeDependent()) {
            TypeDependent = true;
            return Expr;
         }

         EncounteredError = true;
         return ExprError();
      }

      return visitExpr(Result);
   }

   ExprResult visitMacroExpansionExpr(MacroExpansionExpr* Expr)
   {
      return Sema.visitMacroExpansionExpr(Expr);
   }

   ExprResult visitMixinExpr(MixinExpr* Expr)
   {
      return Sema.visitMixinExpr(Expr);
   }

   ExprResult visitLambdaExpr(LambdaExpr* Expr)
   {
      // Don't rebuild these for now.
      return Expr;
   }

   ExprResult visitVariadicExpansionExpr(VariadicExpansionExpr* Expr)
   {
      // Mark identifiers so that they can reference variadic parameters.
      visitSpecificStatement<IdentifierRefExpr>(
          [](IdentifierRefExpr* ident) { ident->setAllowVariadicRef(true); },
          Expr->getExpr());

      auto result = visitExpr(Expr->getExpr());
      if (result) {
         Expr->setExpr(result.get());
      }

      return Expr;
   }

   ExprResult visitAnonymousCallExpr(AnonymousCallExpr* Expr)
   {
      if (auto Val = Expr->getParentExpr()) {
         if (!isa<IdentifierRefExpr>(Val)) {
            auto Result = visitExpr(Val);
            if (Result) {
               Expr->setParentExpr(Result.get());
            }
         }
      }

      for (auto& Val : Expr->getArgs()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Val = Result.get();
         }
      }

      return Expr;
   }

   ExprResult visitIdentifierRefExpr(IdentifierRefExpr* Expr,
                                     TemplateArgListExpr* ArgExpr = nullptr)
   {
      if (Expr->hasLeadingDot()
          || Expr->getDeclName().getKind()
                 == DeclarationName::ClosureArgumentName) {
         return Expr;
      }
      if (Expr->isSemanticallyChecked()) {
         return Expr;
      }

      Expr->setAllowOverloadRef(true);

      auto Result = Sema.visitIdentifierRefExpr(Expr, ArgExpr);
      Expr->setSemanticallyChecked(true);

      if (!Result) {
         Expr->setIsInvalid(true);
         Expr->setExprType(Sema.ErrorTy);
         EncounteredError = true;

         return ExprError();
      }

      TypeDependent |= Result.get()->isTypeDependent();
      auto* ExprRes = Result.get();

      ExprRes->setExprType(Sema.ApplyCapabilities(ExprRes->getExprType()));
      return ExprRes;
   }

   ExprResult visitTemplateArgListExpr(TemplateArgListExpr* E)
   {
      for (auto& TA : E->getExprs()) {
         auto res = Sema.typecheckExpr(TA, SourceType(), E);
         if (!res) {
            EncounteredError = true;
            return ExprError();
         }

         TA = res.get();
         TypeDependent |= TA->isUnknownAny();
      }

      auto* PE = E->getParentExpr();
      if (auto* Call = dyn_cast<AnonymousCallExpr>(PE)) {
         PE = Call->getParentExpr();
      }

      switch (PE->getTypeID()) {
      case Expression::IdentifierRefExprID: {
         auto Res = visitIdentifierRefExpr(cast<IdentifierRefExpr>(PE), E);
         if (!Res) {
            EncounteredError = true;
            return ExprError();
         }

         TypeDependent |= Res.get()->isTypeDependent();
         return Res;
      }
      default:
         llvm_unreachable("bad template arg list expression");
      }
   }
};

class TypeParamSubstVisitor : public TypeBuilder<TypeParamSubstVisitor> {
   /// Reference to the constraint builder.
   ConstraintBuilder& Builder;

   /// The template parameter bindings.
   ConstraintSystem::SolutionBindings& Bindings;

public:
   TypeParamSubstVisitor(SemaPass& Sema, SourceRange SR,
                         ConstraintBuilder& Builder,
                         ConstraintSystem::SolutionBindings& Bindings)
       : TypeBuilder(Sema, SR), Builder(Builder), Bindings(Bindings)
   {
      (void)this->Builder;
   }

   void visitTemplateParamType(TemplateParamType* T,
                               SmallVectorImpl<QualType>& VariadicTys)
   {
      VariadicTys.push_back(visitTemplateParamType(T));
   }

   QualType visitTemplateParamType(TemplateParamType* T)
   {
      auto* Param = T->getParam();
      auto It = Bindings.ParamBindings.find(Param);
      if (It == Bindings.ParamBindings.end()) {
         return T;
      }

      assert(It->getSecond() && "bad type binding");
      return It->getSecond();
   }
};

class TemplateParamRemover : public TypeBuilder<TemplateParamRemover> {
   /// The template parameter bindings.
   ConstraintBuilder& builder;

public:
   TemplateParamRemover(SemaPass& Sema, SourceRange SR,
                        ConstraintBuilder& builder)
       : TypeBuilder(Sema, SR), builder(builder)
   {
   }

   bool shouldGenerateConversionConstraint = true;

   void visitTemplateParamType(TemplateParamType* T,
                               SmallVectorImpl<QualType>& VariadicTys)
   {
      VariadicTys.push_back(visitTemplateParamType(T));
   }

   QualType visitTemplateParamType(TemplateParamType* T)
   {
      auto* Param = T->getParam();
      auto* builder = &this->builder;

      bool first = true;
      while (builder) {
         auto It = builder->Bindings.ParamBindings.find(Param);
         if (It != builder->Bindings.ParamBindings.end()) {
            shouldGenerateConversionConstraint = false;
            assert(first && "can this happen?");
            break;
         }

         builder = builder->outerBuilder;
         first = false;
      }

      return T;
   }
};

class TypeParamBinder : public TypeComparer<TypeParamBinder> {
   /// Reference to the constraint system.
   ConstraintSystem& Sys;

   /// The template parameter bindings.
   ConstraintSystem::SolutionBindings& Bindings;

   /// The locator to use for new constraints.
   ConstraintLocator* Loc;

   /// The outer constraint builder.
   ConstraintBuilder* outerBuilder;

public:
   TypeParamBinder(ConstraintSystem& Sys,
                   ConstraintSystem::SolutionBindings& Bindings,
                   ConstraintLocator* Loc, ConstraintBuilder* outerBuilder)
       : Sys(Sys), Bindings(Bindings), Loc(Loc), outerBuilder(outerBuilder)
   {
   }

   bool visitTemplateParamType(TemplateParamType* GT, QualType RHS)
   {
      QualType binding = RHS->removeReference();
      if (auto* rhsParam = binding->asTemplateParamType()) {
         ConstraintBuilder* builder = outerBuilder;
         while (builder) {
            auto It
                = builder->Bindings.ParamBindings.find(rhsParam->getParam());
            if (It != builder->Bindings.ParamBindings.end()) {
               return true;
            }

            builder = builder->outerBuilder;
         }
      }

      // Give a hint to the solver to try the actual type as the value of
      // the generic type to prevent the covariance from always being chosen.
      auto it = Bindings.ParamBindings.find(GT->getParam());
      if (it != Bindings.ParamBindings.end()) {
         Sys.newConstraint<DefaultableConstraint>(it->getSecond(), binding,
                                                  Loc);
      }

      return true;
   }
};

} // anonymous namespace

ExprResult ConstraintBuilder::rebuildExpression(ast::SemaPass& Sema,
                                                Expression* E)
{
   ExprRebuilder ExprBuilder(Sema);

   auto Result = ExprBuilder.visitExpr(E);
   if (!Result || ExprBuilder.EncounteredError) {
      return ExprError();
   }

   return Result;
}

ExprResult ConstraintBuilder::rebuildExpression(Expression* E)
{
   return rebuildExpression(Sema, E);
}

ConstraintBuilder::GenerationResult
ConstraintBuilder::generateConstraints(Expression* E, SourceType RequiredType,
                                       ConstraintLocator* Locator,
                                       bool isHardRequirement)
{
   if (E->isInvalid()) {
      return Failure;
   }

   QualType T = visitExpr(E, RequiredType, Locator, isHardRequirement);
   if (EncounteredError) {
      return Failure;
   }

   return GenerationResult(TypeDependent ? Dependent : Success, T);
}

ConstraintBuilder::GenerationResult
ConstraintBuilder::generateArgumentConstraints(Expression*& E,
                                               SourceType RequiredType,
                                               ConstraintLocator* Locator,
                                               ConstraintBuilder* outerBuilder)
{
   auto SAR = support::saveAndRestore(GeneratingArgConstraints, true);
   auto SAR2 = support::saveAndRestore(this->outerBuilder, outerBuilder);

   auto Result = generateConstraints(E, RequiredType, Locator);
   if (InvalidArg) {
      Result.Kind = InvalidArgument;
   }

   return Result;
}

void ConstraintBuilder::registerTemplateParam(TemplateParamDecl* Param)
{
   if (!Param->isTypeName() || Bindings.ParamBindings.count(Param) != 0) {
      return;
   }

   uint8_t Flags = ConstraintSystem::IsTemplateParameter;
   if (Param->isVariadic()) {
      Flags |= ConstraintSystem::IsVariadicTemplateParameter;
   }

   // Create a new type variable for the parameter.
   TypeVariable ParamType = Sys.newTypeVariable(Flags);

   // Add an implicit conversion constraint for the covariance.
   if (!Param->getCovariance()->isErrorType()) {
      auto* Loc = makeLocator(nullptr, PathElement::templateParam(Param));
      Sys.newConstraint<CovarianceConstraint>(ParamType, Param->getCovariance(),
                                              Param->isVariadic(), Loc);
   }

   // Remember this binding.
   Bindings.ParamBindings[Param] = ParamType;
}

void ConstraintBuilder::addTemplateParamBinding(QualType Param,
                                                QualType Binding)
{
   TypeParamBinder Binder(Sys, Bindings, nullptr, outerBuilder);
   Binder.visit(Param->removeReference(), Binding->removeReference());
}

Locator ConstraintBuilder::makeLocator(Expression* E,
                                       ArrayRef<PathElement> Elements)
{
   llvm::FoldingSetNodeID ID;
   ConstraintLocator::Profile(ID, E, Elements);

   void* InsertPos;
   if (auto* L = Sys.Locators.FindNodeOrInsertPos(ID, InsertPos)) {
      return L;
   }

   auto* L = ConstraintLocator::Create(Sys.Allocator, E, Elements);
   Sys.Locators.InsertNode(L, InsertPos);

   return L;
}

TypeVariableType* ConstraintBuilder::getClosureParam(DeclarationName Name)
{
   if (!ClosureParams) {
      return nullptr;
   }

   auto It = ClosureParams->find(Name.getClosureArgumentIdx());
   if (It == ClosureParams->end()) {
      return nullptr;
   }

   return It->getSecond();
}

QualType ConstraintBuilder::visitExpr(Expression* Expr, SourceType RequiredType,
                                      ConstraintLocator* Locator,
                                      bool isHardRequirement)
{
   auto It = Bindings.ExprBindings.find(Expr);
   if (It != Bindings.ExprBindings.end()) {
      return It->getSecond();
   }

   if (QualType T = Expr->getExprType()) {
      Bindings.ExprBindings[Expr] = T;
      EncounteredError |= Expr->isInvalid();

      if (RequiredType && RequiredType->containsTemplateParamType()) {
         TypeParamBinder Binder(Sys, Bindings, Locator, outerBuilder);
         Binder.visit(RequiredType->removeReference(), T->removeReference());
      }

      return T;
   }

   QualType contextualType;
   bool shouldGenerateConversionConstraint = isHardRequirement;

   if (RequiredType) {
      if (GeneratingArgConstraints) {
         TemplateParamRemover remover(Sema, {}, *this);
         remover.visit(RequiredType);

         shouldGenerateConversionConstraint
             = remover.shouldGenerateConversionConstraint;
      }

      contextualType = RequiredType;
   }

   QualType T;
   switch (Expr->getTypeID()) {
#define CDOT_EXPR(NAME)                                                        \
   case Expression::NAME##ID:                                                  \
      T = visit##NAME(static_cast<NAME*>(Expr), contextualType);               \
      break;
#include "cdotc/AST/AstNode.def"
   default:
      llvm_unreachable("not an expression!");
   }

   if (!T || Expr->isInvalid() || T->isErrorType()) {
      T = Sys.newTypeVariable();
   }

   Bindings.ExprBindings[Expr] = T;

   if (auto* typeVar = T->asTypeVariableType()) {
      typeVarMap[Expr] = typeVar;
   }

   if (!RequiredType || RequiredType->isAutoType()) {
      return T;
   }

   auto* Var = T->asTypeVariableType();
   if (!Var) {
      Var = Sys.newTypeVariable(ConstraintSystem::HasConcreteBinding);
      Sys.newConstraint<TypeBindingConstraint>(Var, T, nullptr);
   }

   typeVarMap[Expr] = Var;

   // Constrain the type of the entire expression to be convertible to the
   // required type.
   CanType DesugaredTy;
   if (RequiredType->containsTemplateParamType()) {
      TypeParamSubstVisitor Builder(Sema, Expr->getSourceRange(), *this,
                                    Bindings);
      DesugaredTy = Builder.visit(RequiredType)->getDesugaredType();

      if (auto DirectBinding = Sys.getConstrainedBinding(Var)) {
         TypeParamBinder Binder(Sys, Bindings, Locator, outerBuilder);
         Binder.visit(RequiredType, DirectBinding);
      }
      else {
         TypeParamBinder Binder(Sys, Bindings, Locator, outerBuilder);
         Binder.visit(RequiredType, T);
      }
   }
   else {
      DesugaredTy = RequiredType;
   }

   if (!Locator) {
      Locator = makeLocator(
          Expr, PathElement::contextualType(RequiredType.getSourceRange()));
   }

   if (shouldGenerateConversionConstraint) {
      Sys.newConstraint<ImplicitConversionConstraint>(Var, DesugaredTy,
                                                      Locator);
   }

   if (!DesugaredTy->containsTypeVariable()) {
      Sys.setPreferredBinding(Var, DesugaredTy);
   }

   return Var;
}

TypeVariableType* ConstraintBuilder::getTypeVar(Expression* E, bool force,
                                                SourceType T)
{
   auto resultType = visitExpr(E, T);
   if (!resultType) {
      return nullptr;
   }
   if (auto* typeVar = resultType->asTypeVariableType()) {
      return typeVar;
   }

   auto it = typeVarMap.find(E);
   if (it == typeVarMap.end()) {
      if (force) {
         QualType ty;
         auto bindingIt = Bindings.ExprBindings.find(E);
         if (bindingIt != Bindings.ExprBindings.end()) {
            ty = bindingIt->getSecond();
         }

         auto* typeVar
             = Sys.newTypeVariable(ConstraintSystem::HasConcreteBinding);
         Sys.newConstraint<TypeBindingConstraint>(typeVar, ty, nullptr);

         return typeVar;
      }

      return nullptr;
   }

   return it->getSecond();
}

QualType ConstraintBuilder::visitAttributedExpr(AttributedExpr* Expr,
                                                SourceType T)
{
   return visitExpr(Expr->getExpr());
}

QualType ConstraintBuilder::visitTryExpr(TryExpr* Expr, SourceType T)
{
   auto Result = Sema.visitExpr(Expr);
   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   return Result.get()->getExprType();
}

QualType ConstraintBuilder::visitAwaitExpr(AwaitExpr* Expr, SourceType T)
{
   auto Result = Sema.visitExpr(Expr);
   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   return Result.get()->getExprType();
}

QualType ConstraintBuilder::visitFunctionTypeExpr(FunctionTypeExpr* Expr,
                                                  SourceType T)
{
   auto Result = Sema.visitExpr(Expr);
   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   return Result.get()->getExprType();
}

QualType ConstraintBuilder::visitTupleTypeExpr(TupleTypeExpr* Expr,
                                               SourceType T)
{
   auto Result = Sema.visitExpr(Expr);
   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   return Result.get()->getExprType();
}

QualType ConstraintBuilder::visitArrayTypeExpr(ArrayTypeExpr* Expr,
                                               SourceType T)
{
   auto Result = Sema.visitExpr(Expr);
   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   return Result.get()->getExprType();
}

QualType ConstraintBuilder::visitDeclTypeExpr(DeclTypeExpr* Expr, SourceType T)
{
   auto Result = Sema.visitExpr(Expr);
   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   return Result.get()->getExprType();
}

QualType ConstraintBuilder::visitReferenceTypeExpr(ReferenceTypeExpr* Expr,
                                                   SourceType T)
{
   auto Result = Sema.visitExpr(Expr);
   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   return Result.get()->getExprType();
}

QualType ConstraintBuilder::visitPointerTypeExpr(PointerTypeExpr* Expr,
                                                 SourceType T)
{
   auto Result = Sema.visitExpr(Expr);
   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   return Result.get()->getExprType();
}

QualType ConstraintBuilder::visitOptionTypeExpr(OptionTypeExpr* Expr,
                                                SourceType T)
{
   auto Result = Sema.visitExpr(Expr);
   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   return Result.get()->getExprType();
}

QualType ConstraintBuilder::visitExistentialTypeExpr(ExistentialTypeExpr* Expr,
                                                     SourceType T)
{
   auto Result = Sema.visitExpr(Expr);
   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   return Result.get()->getExprType();
}

QualType ConstraintBuilder::visitParenExpr(ParenExpr* Expr, SourceType T)
{
   return visitExpr(Expr->getParenthesizedExpr());
}

QualType ConstraintBuilder::visitIntegerLiteral(IntegerLiteral* Expr,
                                                SourceType T)
{
   TypeVariable Var
       = Sys.newTypeVariable(ConstraintSystem::HasLiteralConstraint);
   auto* Loc
       = makeLocator(Expr, PathElement::contextualType(T.getSourceRange()));

   Sys.newConstraint<LiteralConstraint>(Var, LiteralConstraint::IntegerLiteral,
                                        Loc);

   return Var;
}

QualType ConstraintBuilder::visitFPLiteral(FPLiteral* Expr, SourceType T)
{
   TypeVariable Var
       = Sys.newTypeVariable(ConstraintSystem::HasLiteralConstraint);
   auto* Loc
       = makeLocator(Expr, PathElement::contextualType(T.getSourceRange()));

   Sys.newConstraint<LiteralConstraint>(Var, LiteralConstraint::FPLiteral, Loc);

   return Var;
}

QualType ConstraintBuilder::visitBoolLiteral(BoolLiteral* Expr, SourceType T)
{
   TypeVariable Var
       = Sys.newTypeVariable(ConstraintSystem::HasLiteralConstraint);
   auto* Loc
       = makeLocator(Expr, PathElement::contextualType(T.getSourceRange()));

   Sys.newConstraint<LiteralConstraint>(Var, LiteralConstraint::BoolLiteral,
                                        Loc);

   return Var;
}

QualType ConstraintBuilder::visitCharLiteral(CharLiteral* Expr, SourceType T)
{
   LiteralConstraint::LiteralKind LK;
   if (Expr->getWide() >= 128) {
      LK = LiteralConstraint::CodePointLiteral;
   }
   else {
      LK = LiteralConstraint::ASCIILiteral;
   }

   auto* Loc
       = makeLocator(Expr, PathElement::contextualType(T.getSourceRange()));

   TypeVariable Var
       = Sys.newTypeVariable(ConstraintSystem::HasLiteralConstraint);
   Sys.newConstraint<LiteralConstraint>(Var, LK, Loc);

   return Var;
}

QualType ConstraintBuilder::visitNoneLiteral(NoneLiteral* Expr, SourceType T)
{
   TypeVariable Var
       = Sys.newTypeVariable(ConstraintSystem::HasLiteralConstraint);
   auto* Loc
       = makeLocator(Expr, PathElement::contextualType(T.getSourceRange()));

   Sys.newConstraint<LiteralConstraint>(Var, LiteralConstraint::NoneLiteral,
                                        Loc);

   return Var;
}

QualType ConstraintBuilder::visitStringLiteral(StringLiteral* Expr,
                                               SourceType T)
{
   TypeVariable Var
       = Sys.newTypeVariable(ConstraintSystem::HasLiteralConstraint);
   auto* Loc
       = makeLocator(Expr, PathElement::contextualType(T.getSourceRange()));

   Sys.newConstraint<LiteralConstraint>(Var, LiteralConstraint::StringLiteral,
                                        Loc);

   return Var;
}

QualType ConstraintBuilder::visitStringInterpolation(StringInterpolation* Expr,
                                                     SourceType T)
{
   auto* Str = Sema.getStringDecl();
   if (!Str) {
      Sema.diagnose(Expr, err_no_builtin_decl, Expr->getSourceLoc(),
                    /*String*/ 4);

      EncounteredError = true;
      return nullptr;
   }

   auto* StrRep = Sema.getStringRepresentableDecl();
   if (!StrRep) {
      Sema.diagnose(Expr, err_no_builtin_decl, Expr->getSourceLoc(),
                    /*String*/ 4);

      EncounteredError = true;
      return nullptr;
   }

   // Constrain all interpolation segments to be StringRepresentable.
   for (auto* Val : Expr->getSegments()) {
      QualType Ty = visitExpr(Val);

      // Constrain the key type to be hashable.
      newConstraint<ConformanceConstraint>(Ty, StrRep, nullptr);
   }

   auto* Var = Sys.newTypeVariable(ConstraintSystem::HasConcreteBinding);
   Sys.newConstraint<TypeBindingConstraint>(
       Var, Sema.Context.getRecordType(Str), nullptr);

   return Var;
}

namespace {

class ReturnStmtVisitor : public RecursiveASTVisitor<ReturnStmtVisitor> {
   /// Reference to the constraint builder.
   ConstraintBuilder& Builder;

   /// Reference to the constraint system.
   ConstraintSystem& Sys;

   /// The type variable for the return type.
   TypeVariableType* RetTypeVar;

   /// Set to true if we found an expression we can not infer.
   bool CanInfer = true;

   bool canInferReturnExpr(Expression* Expr)
   {
      CanInfer = true;
      RecursiveASTVisitor::visit(Expr);

      return CanInfer;
   }

public:
   ReturnStmtVisitor(ConstraintBuilder& Builder, ConstraintSystem& Sys,
                     TypeVariableType* RetTypeVar)
       : Builder(Builder), Sys(Sys), RetTypeVar(RetTypeVar)
   {
   }

   /// True if we encountered a bad return statement.
   bool FoundBadReturnStmt = false;

   bool visitReturnStmt(ReturnStmt* Ret)
   {
      if (!Ret->getReturnValue()) {
         Sys.newConstraint<TypeBindingConstraint>(
             RetTypeVar, Sys.QC.Context.getEmptyTupleType(), nullptr);

         return false;
      }

      Expression* RetExpr = Ret->getReturnValue();
      if (!canInferReturnExpr(RetExpr)) {
         Sys.QC.Sema->diagnose(
             err_generic_error,
             "cannot infer return type of complex expression, provide manual "
             "annotation to disambiguate",
             RetExpr->getSourceRange());

         FoundBadReturnStmt = true;
         return false;
      }

      QualType ValueType = Builder.visitExpr(Ret->getReturnValue());
      Builder.newConstraint<ImplicitConversionConstraint>(ValueType, RetTypeVar,
                                                          nullptr);

      return false;
   }

   bool visitIdentifierRefExpr(IdentifierRefExpr* E)
   {
      if (E->getDeclName().getKind() != DeclarationName::ClosureArgumentName) {
         CanInfer = false;
      }

      return false;
   }

#define CANT_INFER(NAME)                                                       \
   bool visit##NAME##Expr(NAME##Expr* E)                                       \
   {                                                                           \
      CanInfer = false;                                                        \
      return false;                                                            \
   }

   CANT_INFER(Call)
   CANT_INFER(AnonymousCall)
};

} // anonymous namespace

QualType ConstraintBuilder::visitLambdaExpr(LambdaExpr* Expr, SourceType T)
{
   TypeVariable FunctionTy = Sys.newTypeVariable();

   SmallVector<QualType, 2> ParamTypes;
   ParamTypes.reserve(Expr->getArgs().size());

   llvm::DenseMap<unsigned, TypeVariableType*> ClosureParams;
   auto SAR = support::saveAndRestore(this->ClosureParams, &ClosureParams);

   unsigned i = 0;
   bool FoundParamWithoutType = false;

   for (auto* Arg : Expr->getArgs()) {
      auto TypeRes = Sema.visitSourceType(Expr, Arg->getType());
      if (!TypeRes || TypeRes.get()->isAutoType()) {
         FoundParamWithoutType = true;

         TypeVariable ParamTypeVar = Sys.newTypeVariable();
         ParamTypes.push_back(ParamTypeVar);

         ClosureParams[i] = ParamTypeVar;
      }
      else {
         ParamTypes.push_back(TypeRes.get());
      }

      ++i;
   }

   // If there is no parameter whose type is not specified, we can typecheck
   // the closure body to get the return type.
   QualType ReturnType;
   if (!FoundParamWithoutType) {
      if (!Sema.visitExpr(Expr)) {
         EncounteredError = true;
         return FunctionTy;
      }

      ReturnType = Expr->getReturnType();
   }

   auto TypeRes = Sema.visitSourceType(Expr, Expr->getReturnType());
   if (TypeRes && !TypeRes.get()->isAutoType()) {
      ReturnType = TypeRes.get();
   }

   // Create a fresh type variable for the return type.
   TypeVariable RetTyVar = Sys.newTypeVariable();
   if (!ReturnType) {
      // Include the returned value in the constraint system.
      ReturnStmtVisitor Visitor(*this, Sys, RetTyVar);
      Visitor.visit(Expr->getBody());

      if (Visitor.FoundBadReturnStmt) {
         EncounteredError = true;
      }
   }
   else {
      Sys.setPreferredBinding(RetTyVar, ReturnType);
   }

   // Equate the type of the whole expression with a tuple type containing
   // the element types.
   QualType ExprType = Sema.Context.getLambdaType(RetTyVar, ParamTypes);
   Sys.newConstraint<TypeBindingConstraint>(FunctionTy, ExprType, nullptr);

   return FunctionTy;
}

QualType ConstraintBuilder::visitTupleLiteral(TupleLiteral* Expr, SourceType T)
{
   auto Elements = Expr->getElements();
   unsigned Arity = Elements.size();

   ArrayRef<SourceType> ContextualElementTypes;
   if (auto* Tup = dyn_cast_or_null<TupleTypeExpr>(T.getTypeExpr())) {
      ContextualElementTypes = Tup->getContainedTypes();
   }

   TypeVariable TupleTy
       = Sys.newTypeVariable(ConstraintSystem::HasConcreteBinding);

   SmallVector<QualType, 2> ElementTypes;
   ElementTypes.reserve(Arity);

   unsigned i = 0;
   for (auto* Val : Elements) {
      SourceType ContextualElementType;
      if (i < ContextualElementTypes.size()) {
         ContextualElementType = ContextualElementTypes[i];
      }

      QualType ValueTypeVar = visitExpr(Val, ContextualElementType);
      ElementTypes.push_back(ValueTypeVar);

      ++i;
   }

   // Equate the type of the whole expression with a tuple type containing
   // the element types.
   QualType ExprType = Sema.Context.getTupleType(ElementTypes);
   Sys.newConstraint<TypeBindingConstraint>(TupleTy, ExprType, nullptr);

   return TupleTy;
}

QualType ConstraintBuilder::visitArrayLiteral(ArrayLiteral* Expr, SourceType T)
{
   TypeVariable Var = Sys.newTypeVariable();
   auto* Loc
       = makeLocator(Expr, PathElement::contextualType(T.getSourceRange()));

   Sys.newConstraint<LiteralConstraint>(Var, LiteralConstraint::ArrayLiteral,
                                        Loc);

   // Constrain each element to be convertible to a common type.
   TypeVariable ElementTy = Sys.newTypeVariable();
   for (auto* E : Expr->getValues()) {
      auto ElVar = visitExpr(E);
      newConstraint<ImplicitConversionConstraint>(ElVar, ElementTy, nullptr);
   }

   return Var;
}

QualType ConstraintBuilder::visitDictionaryLiteral(DictionaryLiteral* Expr,
                                                   SourceType T)
{
   TypeVariable Var = Sys.newTypeVariable();
   auto* Loc
       = makeLocator(Expr, PathElement::contextualType(T.getSourceRange()));

   Sys.newConstraint<LiteralConstraint>(
       Var, LiteralConstraint::DictionaryLiteral, Loc);

   // Constrain each element type to be convertible to a common type.
   TypeVariable ElementTy = Sys.newTypeVariable();
   for (auto* E : Expr->getValues()) {
      auto ElVar = visitExpr(E);
      newConstraint<ImplicitConversionConstraint>(ElVar, ElementTy, nullptr);
   }

   // Constrain each key type to be convertible to a common type.
   TypeVariable KeyTy = Sys.newTypeVariable();
   for (auto* E : Expr->getKeys()) {
      auto ElVar = visitExpr(E);
      newConstraint<ImplicitConversionConstraint>(ElVar, KeyTy, nullptr);
   }

   // Constrain the key type to be hashable.
   Sys.newConstraint<ConformanceConstraint>(KeyTy, Sema.getHashableDecl(),
                                            nullptr);

   return Var;
}

QualType ConstraintBuilder::visitIdentifierRefExpr(IdentifierRefExpr* Expr,
                                                   SourceType T)
{
   if (Expr->hasLeadingDot()) {
      // The type of the resolved member reference.
      TypeVariable Var = Sys.newTypeVariable();

      // The type of the parent expression.
      TypeVariable ParentTy = Sys.newTypeVariable();

      // Create the locator.
      auto* L = makeLocator(Expr,
                            PathElement::memberReference(Expr->getDeclName()));

      // The parent expression must have a member of the needed type.
      Sys.newConstraint<MemberConstraint>(ParentTy, Expr->getDeclName(), Var,
                                          L);

      return Var;
   }

   if (Expr->getDeclName().getKind() == DeclarationName::ClosureArgumentName) {
      auto* TypeVar = getClosureParam(Expr->getDeclName());
      if (TypeVar) {
         return TypeVar;
      }
   }

   return nullptr;
}

QualType ConstraintBuilder::visitDeclRefExpr(DeclRefExpr* Expr, SourceType T)
{
   auto Result = Sema.visitExpr(Expr);
   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   return Result.get()->getExprType();
}

QualType ConstraintBuilder::visitMemberRefExpr(MemberRefExpr* Expr,
                                               SourceType T)
{
   auto Result = Sema.visitExpr(Expr);
   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   return Result.get()->getExprType();
}

QualType
ConstraintBuilder::visitOverloadedDeclRefExpr(OverloadedDeclRefExpr* Expr,
                                              SourceType T)
{
   auto* Var = Sys.newTypeVariable(ConstraintSystem::IsOverloadChoice);
   SmallVector<Constraint*, 4> Constraints;

   bool first = true;
   unsigned defaultOverload = -1;

   for (auto* Ovl : Expr->getOverloads()) {
      if (first) {
         switch (Ovl->getKind()) {
         case Decl::FunctionDeclID:
         case Decl::MethodDeclID:
         case Decl::InitDeclID:
            break;
         default:
            defaultOverload = 0;
            break;
         }

         first = false;
      }

      QualType Ty = Sema.getTypeForDecl(Ovl);
      auto* Loc = makeLocator(
          Expr, PathElement::overloadedDeclLoc(Ovl->getSourceRange()));

      Constraints.push_back(TypeBindingConstraint::Create(Sys, Var, Ty, Loc));
   }

   auto* Loc
       = makeLocator(Expr, PathElement::overloadedDecl(
                               Expr->getOverloads().front()->getDeclName()));

   Sys.newConstraint<DisjunctionConstraint>(Constraints, Loc, defaultOverload);
   return Var;
}

QualType ConstraintBuilder::visitBuiltinIdentExpr(BuiltinIdentExpr* Expr,
                                                  SourceType T)
{
   switch (Expr->getIdentifierKind()) {
   case BuiltinIdentifier::NULLPTR: {
      // The pointee type.
      TypeVariable Pointee = Sys.newTypeVariable();

      // The pointer type.
      TypeVariable PointerType = Sys.newTypeVariable();

      // The pointer type must be a raw pointer pointing to the pointee type.
      Sys.newConstraint<TypeEqualityConstraint>(
          PointerType, Sema.Context.getPointerType(Pointee), nullptr);

      return PointerType;
   }
   case BuiltinIdentifier::defaultValue: {
      // Return an unconstrained new type variable.
      return Sys.newTypeVariable();
   }
   case BuiltinIdentifier::__ctfe: {
      TypeVariable Var = Sys.newTypeVariable();
      Sys.newConstraint<LiteralConstraint>(Var, LiteralConstraint::BoolLiteral,
                                           nullptr);

      return Var;
   }
   case BuiltinIdentifier::FUNC:
   case BuiltinIdentifier::MANGLED_FUNC: {
      TypeVariable Var = Sys.newTypeVariable();
      Sys.newConstraint<LiteralConstraint>(
          Var, LiteralConstraint::StringLiteral, nullptr);

      return Var;
   }
   default:
      break;
   }

   auto Result = Sema.visitExpr(Expr);
   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   return Result.get()->getExprType();
}

QualType ConstraintBuilder::visitSelfExpr(SelfExpr* Expr, SourceType T)
{
   auto Result = Sema.visitExpr(Expr);
   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   return Result.get()->getExprType();
}

QualType ConstraintBuilder::visitSuperExpr(SuperExpr* Expr, SourceType T)
{
   auto Result = Sema.visitExpr(Expr);
   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   return Result.get()->getExprType();
}

QualType ConstraintBuilder::visitTupleMemberExpr(TupleMemberExpr* Expr,
                                                 SourceType T)
{
   auto Result = Sema.visitExpr(Expr);
   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   return Result.get()->getExprType();
}

QualType ConstraintBuilder::visitCallExpr(CallExpr* Expr, SourceType T)
{
   auto Result = Sema.visitExpr(Expr);
   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   return Result.get()->getExprType();
}

static bool addCandidateDecl(CandidateSet& CandSet, SemaPass& Sema,
                             NamedDecl* ND)
{
   if (auto* C = dyn_cast<CallableDecl>(ND)) {
      CandSet.addCandidate(C, 0);
      return false;
   }

   QualType T;
   if (auto* P = dyn_cast<TemplateParamDecl>(ND)) {
      T = Sema.Context.getMetaType(P->getCovariance());
   }
   else {
      T = Sema.getTypeForDecl(ND);
   }

   if (!T) {
      return false;
   }

   // Anonymous call.
   if (auto* FnTy = T->asFunctionType()) {
      CandSet.addCandidate(ND);
      return false;
   }

   // Constructor call.
   if (auto* Meta = T->asMetaType()) {
      if (auto* RT = Meta->getUnderlyingType()->asRecordType()) {
         DeclarationName Name
             = Sema.Context.getDeclNameTable().getConstructorName(RT);

         const MultiLevelLookupResult* Result;
         if (Sema.QC.MultiLevelLookup(Result, RT->getRecord(), Name)) {
            return true;
         }

         for (auto* Init : Result->allDecls()) {
            CandSet.addCandidate(cast<InitDecl>(Init), 0);
         }
      }
   }

   return false;
}

static bool addCandidateType(CandidateSet& CandSet, SemaPass& Sema,
                             QualType ParentType, Expression* ParentExpr)
{
   if (ParentType->isFunctionType()) {
      CandSet.addCandidate(ParentExpr);
      return false;
   }

   if (MetaType* metaType = ParentType->asMetaType()) {
      QualType initializedType
          = metaType->getUnderlyingType()->getCanonicalType();

      // Happens when initializing 'Self' in a protocol default implementation.
      if (auto* AT = initializedType->asAssociatedType()) {
         initializedType = AT->getDecl()->getCovariance();
      }

      // TODO make primitive types initializeable like this?
      if (!initializedType->isRecordType()) {
         return false;
      }

      auto* R = initializedType->getRecord();
      auto initName = Sema.Context.getDeclNameTable().getConstructorName(
          initializedType, true);

      const MultiLevelLookupResult* LookupRes;
      if (Sema.QC.MultiLevelLookup(LookupRes, R, initName)) {
         return true;
      }

      for (auto* initDecl : LookupRes->allDecls()) {
         CandSet.addCandidate(cast<InitDecl>(initDecl));
      }

      return false;
   }

   return true;
}

bool ConstraintBuilder::buildCandidateSet(AnonymousCallExpr* Call, SourceType T,
                                          DeclarationName& Name,
                                          Expression*& SelfVal,
                                          OverloadedDeclRefExpr*& overloadExpr,
                                          SmallVectorImpl<NamedDecl*>& Decls)
{
   Expression* ParentExpr = Call->getParentExpr()->ignoreParens();

   IdentifierRefExpr* Ident;
   if (auto* TemplateArgs = dyn_cast<TemplateArgListExpr>(ParentExpr)) {
      Ident = dyn_cast<IdentifierRefExpr>(TemplateArgs->getParentExpr());
   }
   else {
      Ident = dyn_cast<IdentifierRefExpr>(ParentExpr);
   }

   if (Ident) {
      Name = Ident->getDeclName();
   }

   ExprResult ParentResult;
   if (Ident) {
      Ident->setAllowOverloadRef(true);
      Ident->setAllowIncompleteTemplateArgs(true);
      Ident->setCalled(true);

      if (T && Ident->hasLeadingDot() && !Ident->getContextualType()) {
         Ident->setContextualType(T);
      }

      ParentResult = Sema.getRValue(Call, ParentExpr);
      if (!ParentResult) {
         EncounteredError = true;
         return true;
      }
   }
   else if (auto* Ovl = dyn_cast<OverloadedDeclRefExpr>(ParentExpr)) {
      ParentResult = Ovl;
      overloadExpr = Ovl;
   }
   else {
      if (auto* MemRef = dyn_cast<MemberRefExpr>(ParentExpr)) {
         MemRef->setCalled(true);
      }

      ParentResult = Sema.typecheckExpr(ParentExpr, SourceType(), Call);
      if (!ParentResult) {
         EncounteredError = true;
         return true;
      }

      ParentResult = Sema.getRValue(Call, ParentResult.get());
      if (!ParentResult) {
         EncounteredError = true;
         return true;
      }
   }

   SelfVal = ParentExpr->getParentExpr();

   ParentExpr = ParentResult.get();
   Call->setParentExpr(ParentExpr);

   if (ParentExpr->isInvalid()) {
      EncounteredError = true;
      return Sema.ErrorTy;
   }

   TypeDependent |= ParentExpr->isTypeDependent();

   QualType ParentType = ParentExpr->getExprType();
   UnresolvedCallExpr& Data
       = UnresolvedCalls.try_emplace(Call).first->getSecond();

   CandidateSet& CandSet = Data.CandSet;

   if (auto* TemplateArgs = dyn_cast<TemplateArgListExpr>(ParentExpr)) {
      ParentExpr = TemplateArgs->getParentExpr();
   }

   if (auto* DeclRef = dyn_cast<DeclRefExpr>(ParentExpr)) {
      auto* ND = DeclRef->getDecl();
      if (addCandidateDecl(CandSet, Sema, ND)) {
         Call->setIsInvalid(true);
      }

      Decls.push_back(ND);
      Name = ND->getDeclName();
   }
   else if (auto* MemRef = dyn_cast<MemberRefExpr>(ParentExpr)) {
      auto* ND = MemRef->getMemberDecl();
      if (addCandidateDecl(CandSet, Sema, ND)) {
         Call->setIsInvalid(true);
      }

      Decls.push_back(ND);
      Name = ND->getDeclName();
   }
   else if (auto* Ovl = dyn_cast<OverloadedDeclRefExpr>(ParentExpr)) {
      for (auto* ND : Ovl->getOverloads()) {
         if (addCandidateDecl(CandSet, Sema, ND)) {
            Call->setIsInvalid(true);
         }

         Decls.push_back(ND);
      }

      assert(!Decls.empty() && "should have been diagnosed before!");
      Name = Decls.front()->getDeclName();
      overloadExpr = Ovl;
   }
   else {
      TypeDependent |= ParentExpr->isDependent();

      if (addCandidateType(CandSet, Sema, ParentType, ParentExpr)) {
         Call->setIsInvalid(true);
         EncounteredError = true;

         Sema.diagnose(Call, err_cannot_call_type, Call->getSourceLoc(),
                       ParentType, false);

         return true;
      }
   }

   if (auto* prevSelfVal = Call->getSelfVal()) {
      SelfVal = prevSelfVal;
   }
   else {
      Call->setSelfVal(SelfVal);
   }

   return false;
}

QualType ConstraintBuilder::visitAnonymousCallExpr(AnonymousCallExpr* Call,
                                                   SourceType T)
{
   DeclarationName Name;
   Expression* SelfVal = nullptr;
   SmallVector<NamedDecl*, 2> Decls;
   OverloadedDeclRefExpr* overloadExpr = nullptr;

   if (buildCandidateSet(Call, T, Name, SelfVal, overloadExpr, Decls)) {
      return Sema.ErrorTy;
   }

   auto& Data = UnresolvedCalls[Call];
   CandidateSet& CandSet = Data.CandSet;

   if (CandSet.Candidates.empty()) {
      EncounteredError = true;
      Sema.diagnose(Call, err_no_matching_call, (bool)Name, 0, Name,
                    Call->getParentExpr()->getSourceRange());

      for (auto* ND : Decls) {
         Sema.diagnose(note_generic_note,
                       "cannot call value of type "
                           + Sema.getTypeForDecl(ND).toDiagString(),
                       ND->getSourceLoc());
      }

      return Sema.ErrorTy;
   }

   assert(!T || !T->containsTypeVariable());

   if (SelfVal && Sema.refersToNamespace(SelfVal)) {
      SelfVal = nullptr;
   }

   auto* Cand = sema::resolveCandidateSet(
       Sema, CandSet, SelfVal, Call->getArgs(), Call->getLabels(), {}, T, Call,
       !GeneratingArgConstraints, GeneratingArgConstraints, this);

   if (!Cand) {
      if (GeneratingArgConstraints) {
         InvalidArg = true;
      }
      else {
         EncounteredError = true;
      }

      return Sema.ErrorTy;
   }

   if (overloadExpr) {
      Bindings.OverloadChoices.try_emplace(overloadExpr,
                                           OverloadChoice(CandSet.MatchIdx));
   }

   QualType ReturnType;
   if (Cand->isAnonymousCandidate()
       || !Cand->getFunc()->isCompleteInitializer()) {
      ReturnType = Cand->getFunctionType()->getReturnType();
   }
   else {
      ReturnType = Sema.Context.getRecordType(Cand->getFunc()->getRecord());
   }

   if (!Cand->isAnonymousCandidate() && Cand->getFunc()->isTemplate()) {
      if (Sema.QC.SubstTemplateParamTypesNonFinal(ReturnType, ReturnType,
                                                  Cand->InnerTemplateArgs,
                                                  Call->getSourceRange())) {
         EncounteredError = true;
      }
   }

   Data.BestCand = Cand;
   return ReturnType;
}

QualType ConstraintBuilder::visitEnumCaseExpr(EnumCaseExpr* Expr, SourceType T)
{
   auto Result = Sema.visitExpr(Expr);
   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   return Result.get()->getExprType();
}

QualType ConstraintBuilder::visitSubscriptExpr(SubscriptExpr* Expr,
                                               SourceType T)
{
   auto Result = Sema.visitExpr(Expr);
   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   return Result.get()->getExprType();
}

QualType ConstraintBuilder::visitTemplateArgListExpr(TemplateArgListExpr* Expr,
                                                     SourceType T)
{
   llvm_unreachable("should be treated specially");
}

QualType ConstraintBuilder::visitBuiltinExpr(BuiltinExpr* Expr, SourceType T)
{
   auto Result = Sema.visitExpr(Expr);
   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   return Result.get()->getExprType();
}

QualType ConstraintBuilder::visitExpressionPattern(ExpressionPattern* Expr,
                                                   SourceType T)
{
   auto Result = Sema.visitExpr(Expr);
   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   return Result.get()->getExprType();
}

QualType ConstraintBuilder::visitCasePattern(CasePattern* Expr, SourceType T)
{
   auto Result = Sema.visitExpr(Expr);
   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   return Result.get()->getExprType();
}

QualType ConstraintBuilder::visitIsPattern(IsPattern* Expr, SourceType T)
{
   auto Result = Sema.visitExpr(Expr);
   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   return Result.get()->getExprType();
}

QualType ConstraintBuilder::visitUnaryOperator(UnaryOperator* Expr,
                                               SourceType T)
{
   auto Result = Sema.visitExpr(Expr);
   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   return Result.get()->getExprType();
}

QualType ConstraintBuilder::visitBinaryOperator(BinaryOperator* Expr,
                                                SourceType T)
{
   auto Result = Sema.visitExpr(Expr);
   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   return Result.get()->getExprType();
}

QualType ConstraintBuilder::visitAssignExpr(AssignExpr* Expr, SourceType T)
{
   QualType lhsType = visitExpr(Expr->getLhs());
   if (!lhsType) {
      Expr->setIsInvalid(true);
      EncounteredError = true;

      return QualType();
   }

   TypeVariable rhsTypeVar
       = getTypeVar(Expr->getRhs(), true, lhsType->removeReference());
   if (!rhsTypeVar) {
      Expr->setIsInvalid(true);
      EncounteredError = true;

      return QualType();
   }

   TypeVariable lhsTypeVar = getTypeVar(Expr->getLhs(), true);

   // The right hand side must be convertible to a type to which the left
   // hand side is a mutable reference to.
   TypeVariable commonType = Sys.newTypeVariable();

   auto* locator = makeLocator(
       Expr, PathElement::contextualType(Expr->getRhs()->getSourceRange()));

   Sys.newConstraint<ImplicitConversionConstraint>(rhsTypeVar, commonType,
                                                   locator);

   Sys.newConstraint<TypeEqualityConstraint>(
       lhsTypeVar, Sema.Context.getMutableReferenceType(commonType),
       makeLocator(Expr->getLhs(), {}));

   return Sema.Context.getEmptyTupleType();
}

QualType ConstraintBuilder::visitTypePredicateExpr(TypePredicateExpr* Expr,
                                                   SourceType T)
{
   auto result = Sema.visitTypePredicateExpr(Expr);
   if (!result) {
      Expr->setIsInvalid(true);
      EncounteredError = true;

      return QualType();
   }

   return Expr->getExprType();
}

QualType ConstraintBuilder::visitExprSequence(ExprSequence* Expr, SourceType T)
{
   llvm_unreachable("should never appear here");
}

QualType ConstraintBuilder::visitCastExpr(CastExpr* Cast, SourceType T)
{
   // right hand side might not have been parsed as a type, check if we were
   // actually given a meta type
   auto TypeResult = Sema.visitSourceType(Cast, Cast->getTargetType(), true);
   if (!TypeResult) {
      Cast->setIsInvalid(true);
      EncounteredError = true;

      return QualType();
   }

   if (TypeResult.get()->isDependentType()) {
      Cast->setIsTypeDependent(true);
   }

   auto to = Cast->getTargetType();
   if (!to->isMetaType()) {
      Sema.diagnose(
          Cast, err_expression_in_type_position,
          Cast->getTargetType().getSourceRange(Cast->getSourceRange()));
   }

   (void)visitExpr(Cast->getTarget());
   return to->removeMetaType();
}

QualType ConstraintBuilder::visitAddrOfExpr(AddrOfExpr* Expr, SourceType T)
{
   auto ReferenceTy = visitExpr(Expr->getTarget(), T);
   if (auto* typeVar = ReferenceTy->asTypeVariableType()) {
      // We need to create a new type variable for the dereferenced type first.
      auto* deref = Sys.newTypeVariable();
      Sys.newConstraint<TypeEqualityConstraint>(
          typeVar, Sema.Context.getMutableReferenceType(deref), nullptr);

      ReferenceTy = deref;
   }
   else if (!ReferenceTy->isTypeVariableType()
            && !ReferenceTy->isMutableReferenceType()) {
      Sema.diagnose(Expr, err_generic_error,
                    "cannot mutably borrow value of type "
                        + ReferenceTy.toDiagString(),
                    Expr->getSourceRange());

      EncounteredError = true;
   }

   return Sema.Context.getMutableReferenceType(ReferenceTy->removeReference());
}

QualType ConstraintBuilder::visitImplicitCastExpr(ImplicitCastExpr* Expr,
                                                  SourceType T)
{
   auto result = Sema.visitImplicitCastExpr(Expr);
   if (!result) {
      Expr->setIsInvalid(true);
      EncounteredError = true;

      return QualType();
   }

   return Expr->getExprType();
}

QualType ConstraintBuilder::visitIfExpr(IfExpr* Expr, SourceType T)
{
   auto& Cond = Expr->getCond();
   switch (Cond.K) {
   case IfCondition::Expression: {
      auto* condVar = getTypeVar(Cond.ExprData.Expr, true);
      if (condVar) {
         auto* truthValueProto = Sema.getTruthValueDecl();
         Sys.newConstraint<ImplicitConversionConstraint>(
             condVar, Sema.Context.getRecordType(truthValueProto),
             makeLocator(Cond.ExprData.Expr));
      }

      break;
   }
   case IfCondition::Pattern:
      (void)visitExpr(Cond.PatternData.Expr);
      break;
   default:
      break;
   }

   auto* TrueVal = Expr->getTrueVal();
   auto* FalseVal = Expr->getFalseVal();

   auto* trueTypeVar = getTypeVar(TrueVal, true, T);
   auto* falseTypeVar = getTypeVar(FalseVal, true, T);

   if (!trueTypeVar || !falseTypeVar) {
      return nullptr;
   }

   // Create a type variable for a common type between TrueVal and FalseVal.
   auto* commonType = Sys.newTypeVariable();

   Sys.newConstraint<ImplicitConversionConstraint>(trueTypeVar, commonType,
                                                   makeLocator(TrueVal));

   Sys.newConstraint<ImplicitConversionConstraint>(falseTypeVar, commonType,
                                                   makeLocator(TrueVal));

   return commonType;
}

QualType ConstraintBuilder::visitStaticExpr(StaticExpr* Expr, SourceType T)
{
   return visitExpr(Expr->getExpr(), T);
}

QualType ConstraintBuilder::visitConstraintExpr(ConstraintExpr* Expr,
                                                SourceType T)
{
   llvm_unreachable("should never be called");
}

QualType ConstraintBuilder::visitTraitsExpr(TraitsExpr* Expr, SourceType T)
{
   auto result = Sema.visitTraitsExpr(Expr);
   if (!result) {
      EncounteredError = true;
      return QualType();
   }

   return Expr->getExprType();
}

QualType ConstraintBuilder::visitMixinExpr(MixinExpr* Expr, SourceType T)
{
   (void)visitExpr(Expr->getMixinExpr(), T);
   return Sys.newTypeVariable();
}

QualType
ConstraintBuilder::visitVariadicExpansionExpr(VariadicExpansionExpr* Expr,
                                              SourceType RequiredType)
{
   auto result = Sema.visitExpr(Expr);
   if (!result) {
      EncounteredError = true;
      return QualType();
   }

   return Expr->getExprType();
}

QualType ConstraintBuilder::visitMacroVariableExpr(MacroVariableExpr* Expr,
                                                   SourceType T)
{
   return nullptr;
}

QualType ConstraintBuilder::visitMacroExpansionExpr(MacroExpansionExpr* Expr,
                                                    SourceType T)
{
   return nullptr;
}