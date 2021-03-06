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

   /// The base expression we're rebuilding.
   Expression *BaseExpr;

   /// Whether or not we're generating constraints for a function argument.
   bool GeneratingArgConstraints;

public:
   /// Set to true iff this expression is type dependent.
   bool TypeDependent = false;

   /// Set to true iff an error was encountered during rebuilding.
   bool EncounteredError = false;

   /// Set to true if the expression needs full constraint based typechecking.
   bool NeedsFullTypechecking = false;

   explicit ExprRebuilder(SemaPass& Sema, Expression *BaseExpr,
                          bool GeneratingArgConstraints)
      : Sema(Sema), BaseExpr(BaseExpr),
        GeneratingArgConstraints(GeneratingArgConstraints)
   {}

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

   ExprResult visitSubscriptExpr(SubscriptExpr *Sub)
   {
      if (Sub->getCallExpr()) {
         NeedsFullTypechecking = true;
         return Sub;
      }

      auto ParentExpr = Sub->getParentExpr();
      auto ParentResult = Sema.typecheckExpr(ParentExpr, SourceType(), Sub);
      if (!ParentResult) {
         Sub->setIsInvalid(true);
         Sub->setExprType(Sema.ErrorTy);
         EncounteredError = true;

         return ExprError();
      }

      ParentExpr = ParentResult.get();
      Sub->setParentExpr(ParentExpr);

      QualType ParentTy = ParentExpr->getExprType();
      QualType SubscriptedTy = ParentTy->removeReference()->getDesugaredType();

      if (SubscriptedTy->isRecordType() || SubscriptedTy->isExistentialType()) {
         auto &Context = Sema.Context;
         auto DeclName = Context.getDeclNameTable().getSubscriptName(
             DeclarationName::SubscriptKind::Getter);

         auto* ident = new (Context) IdentifierRefExpr(Sub->getSourceRange(),
                                                       ParentExpr, DeclName);

         auto *Call = AnonymousCallExpr::Create(
             Context, Sub->getSourceRange(), ident, Sub->getIndices(), {});

         auto Result = visitExpr(Call);
         if (!Result) {
            return ExprError();
         }

         Sub->setCallExpr(Result.get());
         return Sub;
      }

      return Sema.visitExpr(Sub);
   }

   ExprResult visitAssignExpr(AssignExpr *Expr)
   {
      auto *Sub = dyn_cast<SubscriptExpr>(Expr->getLhs());
      if (!Sub) {
         return StmtBuilder::visitAssignExpr(Expr);
      }

      auto ParentExpr = Sub->getParentExpr();
      auto ParentResult = Sema.visitExpr(Sub, ParentExpr);
      if (!ParentResult) {
         Sub->setIsInvalid(true);
         Sub->setExprType(Sema.ErrorTy);
         EncounteredError = true;

         return ExprError();
      }

      ParentExpr = ParentResult.get();
      Sub->setParentExpr(ParentExpr);

      QualType ParentTy = ParentExpr->getExprType();
      QualType SubscriptedTy = ParentTy->removeReference()->getDesugaredType();

      if (SubscriptedTy->isRecordType() || SubscriptedTy->isExistentialType()) {
         auto &Context = Sema.Context;
         auto DeclName = Context.getDeclNameTable().getSubscriptName(
             DeclarationName::SubscriptKind::Setter);

         auto* ident = new (Context) IdentifierRefExpr(Sub->getSourceRange(),
                                                       ParentExpr, DeclName);

         auto Result = visitExpr(AnonymousCallExpr::Create(
             Context, Sub->getSourceRange(), ident, Sub->getIndices(), {}));

         if (Result) {
            Sub->setCallExpr(Result.get());
         }
      }

      return StmtBuilder::visitAssignExpr(Expr);
   }

   ExprResult visitLambdaExpr(LambdaExpr* Expr)
   {
      for (auto *arg : Expr->getArgs()) {
         if (!arg->getType().getTypeExpr()) {
            NeedsFullTypechecking = true;
            break;
         }
      }

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
      NeedsFullTypechecking = true;

      if (auto Val = Expr->getParentExpr()) {
         ExprResult Result;
         if (auto *Ident = dyn_cast<IdentifierRefExpr>(Val)) {
            Ident->setAllowIncompleteTemplateArgs(true);
            Ident->setCalled(true);

            Result = visitIdentifierRefExpr(Ident);
         }
         else if (auto *ArgExpr = dyn_cast<TemplateArgListExpr>(Val)) {
            if (auto *Ident = dyn_cast<IdentifierRefExpr>(ArgExpr->getParentExpr())) {
               Ident->setCalled(true);
            }

            Result = visitExpr(Val);
         }
         else {
            Result = visitExpr(Val);
         }

         if (Result) {
            Expr->setParentExpr(Result.get());
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
      if (Expr->isSemanticallyChecked()) {
         return Expr;
      }
      if (Expr->hasLeadingDot()) {
         NeedsFullTypechecking = true;
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

      // Update the original expression's type. Sometimes SourceTypes are reused
      // (for example for synthesized properties), so the original expression
      // may still be used.
      if (QualType Ty = ExprRes->getExprType()) {
         Expr->setExprType(Ty);
      }
      else {
         Expr->setSemanticallyChecked(false);
      }

      return visitExpr(ExprRes);
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
      auto* Call = dyn_cast<AnonymousCallExpr>(PE);

      if (Call) {
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
         if (Call) {
            Call->setParentExpr(Res.get());
            return visitExpr(Call);
         }

         return Res;
      }
      default:
         llvm_unreachable("bad template arg list expression");
      }
   }

   ExprResult visitAttributedExpr(AttributedExpr* Expr)
   {
      auto E = Expr->getExpr();
      for (auto& A : Expr->getAttributes()) {
         switch (A->getKind()) {
#define CDOT_EXPR_ATTR(Name, Spelling)                                         \
   case AttrKind::Name:                                                        \
      Sema.check##Name##Attr(E, cast<Name##Attr>(A));                          \
      break;
#define CDOT_TYPE_ATTR(Name, Spelling)                                         \
   case AttrKind::Name:                                                        \
      Sema.check##Name##Attr(E, cast<Name##Attr>(A));                          \
      break;
#include "cdotc/AST/Attributes.def"

         default:
            llvm_unreachable("bad expr attr");
         }
      }

      return visitExpr(Expr->getExpr());
   }

   ExprResult visitTryExpr(TryExpr* Expr)
   {
      {
         bool needsFullTypeChecking = false;
         auto SAR = support::saveAndRestore(this->NeedsFullTypechecking,
                                            needsFullTypeChecking);

         auto Result = visitExpr(Expr->getExpr());
         if (!Result) {
            return ExprError();
         }

         Expr->setExpr(Result.get());

         if (!NeedsFullTypechecking) {
            return Sema.visitExpr(Expr);
         }
      }

      NeedsFullTypechecking = true;
      return Expr;
   }

   ExprResult visitAwaitExpr(AwaitExpr* Expr)
   {
      return Sema.visitExpr(Expr);
   }

   ExprResult visitMacroExpansionExpr(MacroExpansionExpr* Expr)
   {
      return Sema.visitExpr(Expr);
   }

   ExprResult visitMixinExpr(MixinExpr* Expr)
   {
      return Sema.visitExpr(Expr);
   }

   ExprResult visitSelfExpr(SelfExpr* Expr)
   {
      return Sema.visitExpr(Expr);
   }

   ExprResult visitSuperExpr(SuperExpr* Expr)
   {
      return Sema.visitExpr(Expr);
   }

   ExprResult visitCallExpr(CallExpr* Expr)
   {
      return Sema.visitExpr(Expr);
   }

   ExprResult visitBuiltinExpr(BuiltinExpr* Expr)
   {
      return Sema.visitExpr(Expr);
   }

   ExprResult visitExpressionPattern(ExpressionPattern* Expr)
   {
      return Sema.visitExpr(Expr);
   }

   ExprResult visitCasePattern(CasePattern* Expr)
   {
      return Sema.visitExpr(Expr);
   }

   ExprResult visitIsPattern(IsPattern* Expr)
   {
      return Sema.visitExpr(Expr);
   }

   ExprResult visitUnaryOperator(UnaryOperator* Expr)
   {
      return Sema.visitExpr(Expr);
   }

   ExprResult visitBinaryOperator(BinaryOperator* Expr)
   {
      return Sema.visitExpr(Expr);
   }

   ExprResult visitDeclRefExpr(DeclRefExpr* Expr)
   {
      return Sema.visitExpr(Expr);
   }

   ExprResult visitMemberRefExpr(MemberRefExpr* Expr)
   {
      return Sema.visitExpr(Expr);
   }

   ExprResult visitFunctionTypeExpr(FunctionTypeExpr* Expr)
   {
      return Sema.visitExpr(Expr);
   }

   ExprResult visitTupleTypeExpr(TupleTypeExpr* Expr)
   {
      return Sema.visitExpr(Expr);
   }

   ExprResult visitArrayTypeExpr(ArrayTypeExpr* Expr)
   {
      return Sema.visitExpr(Expr);
   }

   ExprResult visitDeclTypeExpr(DeclTypeExpr* Expr)
   {
      return Sema.visitExpr(Expr);
   }

   ExprResult visitReferenceTypeExpr(ReferenceTypeExpr* Expr)
   {
      return Sema.visitExpr(Expr);
   }

   ExprResult visitPointerTypeExpr(PointerTypeExpr* Expr)
   {
      return Sema.visitExpr(Expr);
   }

   ExprResult visitOptionTypeExpr(OptionTypeExpr* Expr)
   {
      return Sema.visitExpr(Expr);
   }

   ExprResult visitExistentialTypeExpr(ExistentialTypeExpr* Expr)
   {
      return Sema.visitExpr(Expr);
   }

   ExprResult visitStaticExpr(StaticExpr* Expr)
   {
      if (!Expr->getExpr()) {
         return Sema.visitExpr(Expr);
      }

      {
         bool needsFullTypeChecking = false;
         auto SAR = support::saveAndRestore(this->NeedsFullTypechecking,
                                            needsFullTypeChecking);

         auto Result = visitExpr(Expr->getExpr());
         if (!Result) {
            return ExprError();
         }

         Expr->setExpr(Result.get());

         if (!NeedsFullTypechecking) {
            return Sema.visitExpr(Expr);
         }
      }

      NeedsFullTypechecking = true;
      return Expr;
   }

   ExprResult visitTraitsExpr(TraitsExpr* Expr)
   {
      return Sema.visitExpr(Expr);
   }

   ExprResult visitParenExpr(ParenExpr* Expr)
   {
      {
         bool needsFullTypeChecking = false;
         auto SAR = support::saveAndRestore(this->NeedsFullTypechecking,
                                            needsFullTypeChecking);

         auto Result = visitExpr(Expr->getParenthesizedExpr());
         if (!Result) {
            return ExprError();
         }

         Expr->setParenthesizedExpr(Result.get());

         if (!NeedsFullTypechecking) {
            return Sema.visitExpr(Expr);
         }
      }

      NeedsFullTypechecking = true;
      return Expr;
   }

   ExprResult visitOverloadedDeclRefExpr(OverloadedDeclRefExpr *Expr)
   {
      NeedsFullTypechecking = true;
      return Expr;
   }

   ExprResult visitBuiltinIdentExpr(BuiltinIdentExpr *Expr)
   {
      NeedsFullTypechecking |= GeneratingArgConstraints;
      return Expr;
   }

   ExprResult visitIntegerLiteral(IntegerLiteral *Expr)
   {
      NeedsFullTypechecking |= GeneratingArgConstraints;
      return Expr;
   }

   ExprResult visitFPLiteral(FPLiteral *Expr)
   {
      NeedsFullTypechecking |= GeneratingArgConstraints;
      return Expr;
   }

   ExprResult visitStringLiteral(StringLiteral *Expr)
   {
      NeedsFullTypechecking |= GeneratingArgConstraints;
      return Expr;
   }

   ExprResult visitCharLiteral(CharLiteral *Expr)
   {
      NeedsFullTypechecking |= GeneratingArgConstraints;
      return Expr;
   }

   ExprResult visitBoolLiteral(BoolLiteral *Expr)
   {
      NeedsFullTypechecking |= GeneratingArgConstraints;
      return Expr;
   }

   ExprResult visitNoneLiteral(NoneLiteral *Expr)
   {
      NeedsFullTypechecking |= GeneratingArgConstraints;
      return Expr;
   }

   ExprResult visitArrayLiteral(ArrayLiteral *Expr)
   {
      NeedsFullTypechecking |= Expr != BaseExpr;
      return StmtBuilder::visitArrayLiteral(Expr);
   }

   ExprResult visitDictionaryLiteral(DictionaryLiteral *Expr)
   {
      NeedsFullTypechecking |= Expr != BaseExpr;
      return StmtBuilder::visitDictionaryLiteral(Expr);
   }

   ExprResult visitCastExpr(CastExpr *Expr)
   {
      NeedsFullTypechecking = true;
      return StmtBuilder::visitCastExpr(Expr);
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

   /// Whether or not we should only create a defaultable constraint if no
   /// other one exists.
   bool onlyNewDefaultableConstraints;

public:
   TypeParamBinder(ConstraintSystem& Sys,
                   ConstraintSystem::SolutionBindings& Bindings,
                   ConstraintLocator* Loc, ConstraintBuilder* outerBuilder,
                   bool onlyNewDefaultableConstraints = false)
       : Sys(Sys), Bindings(Bindings), Loc(Loc), outerBuilder(outerBuilder),
         onlyNewDefaultableConstraints(onlyNewDefaultableConstraints)
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
         if (!onlyNewDefaultableConstraints
         || !Sys.getFirstConstraint<DefaultableConstraint>(it->getSecond())) {
            Sys.newConstraint<DefaultableConstraint>(it->getSecond(), binding,
                                                     Loc);
         }
      }

      return true;
   }
};

} // anonymous namespace

std::pair<ExprResult, bool>
ConstraintBuilder::rebuildExpression(ast::SemaPass& Sema, Expression* E,
                                     ast::Expression *baseExpr,
                                     bool GeneratingArgConstraints)
{
   if (!baseExpr)
      baseExpr = E;

   ExprRebuilder ExprBuilder(Sema, baseExpr, GeneratingArgConstraints);

   auto Result = ExprBuilder.visitExpr(E);
   if (!Result || ExprBuilder.EncounteredError) {
      E->setIsInvalid(true);
      return std::make_pair(ExprError(), false);
   }

   return std::make_pair(Result, ExprBuilder.NeedsFullTypechecking);
}

std::pair<ExprResult, bool>
 ConstraintBuilder::rebuildExpression(Expression* E,
                                      ast::Expression *baseExpr)
{
   return rebuildExpression(Sema, E, baseExpr, GeneratingArgConstraints);
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
                                               bool importedFromClang,
                                               ConstraintBuilder* outerBuilder)
{
   auto SAR = support::saveAndRestore(GeneratingArgConstraints, true);
   auto SAR2 = support::saveAndRestore(this->outerBuilder, outerBuilder);

   auto Result = generateConstraints(E, RequiredType, Locator);
   if (InvalidArg) {
      Result.Kind = InvalidArgument;
   }

   if (importedFromClang && Result.Type) {
      if (auto *TV = Result.Type->asTypeVariableType()) {
         Sys.addFlag(TV, ConstraintSystem::ParameterOfClangImportedFunc);
      }
   }

   return Result;
}

void ConstraintBuilder::registerTemplateParam(TemplateParamDecl* Param,
                                              ast::Expression *Anchor)
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
      auto* Loc = makeLocator(Anchor, PathElement::templateParam(Param));
      Sys.newConstraint<CovarianceConstraint>(ParamType, Param->getCovariance(),
                                              Param->isVariadic(), Loc);
   }

   // Add a defaultable constraint if there is a default value.
   if (auto *DefaultVal = Param->getDefaultValue()) {
      Sys.newConstraint<DefaultableConstraint>(
          ParamType, DefaultVal->getExprType()->removeMetaType(), nullptr);
   }

   // Remember this binding.
   Bindings.ParamBindings[Param] = ParamType;
}

void ConstraintBuilder::addTemplateParamBinding(QualType Param,
                                                QualType Binding)
{
   TypeParamBinder Binder(Sys, Bindings, nullptr, outerBuilder, true);
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

QualType ConstraintBuilder::getClosureParam(DeclarationName Name)
{
   if (!ClosureParams) {
      return QualType();
   }

   auto It = ClosureParams->find(Name.getClosureArgumentIdx());
   if (It == ClosureParams->end()) {
      return QualType();
   }

   return It->getSecond();
}

QualType ConstraintBuilder::getRValue(Expression *Expr, SourceType RequiredType,
                                      ConstraintLocator *Locator,
                                      bool isHardRequirement) {
   if (QualType T = Expr->getExprType()) {
      return T->removeReference();
   }

   auto Result = visitExpr(Expr, RequiredType, Locator, false);
   if (!Result) {
      return Result;
   }

   if (Result->isReferenceType()) {
      auto *Deref = Sys.newTypeVariable();

      QualType Ref;
      if (Result->isMutableReferenceType()) {
         Ref = Sema.Context.getMutableReferenceType(Deref);
      }
      else {
         Ref = Sema.Context.getReferenceType(Deref);
      }

      newConstraint<TypeEqualityConstraint>(Result, Ref, nullptr);
      Result = Deref;
   }

   return Result;
}

QualType ConstraintBuilder::visitExpr(Expression* Expr, SourceType RequiredType,
                                      ConstraintLocator* Locator,
                                      bool isHardRequirement,
                                      CastStrength allowedStrength)
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

      if (RequiredType && isHardRequirement) {
         QualType NeededTy = RequiredType;
         if (RequiredType->containsTemplateParamType()) {
            TypeParamSubstVisitor Builder(
               Sema, Expr->getSourceRange(), *this, Bindings);

            NeededTy = Builder.visit(RequiredType)->getDesugaredType();
         }

         auto *Var = Sys.newTypeVariable(ConstraintSystem::HasConcreteBinding);
         Sys.newConstraint<TypeBindingConstraint>(Var, T, nullptr);
         Sys.newConstraint<ImplicitConversionConstraint>(
            Var, NeededTy, Locator ? Locator : makeLocator(
               Expr, PathElement::contextualType(RequiredType.getSourceRange())),
               allowedStrength);

         T = Var;
      }

      return T;
   }

   QualType contextualType;
   bool shouldGenerateConversionConstraint = isHardRequirement;

   if (RequiredType) {
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

   if (!T || T->isErrorType()) {
      T = Sys.newTypeVariable();
   }

   Bindings.ExprBindings[Expr] = T;

   if (auto* typeVar = T->asTypeVariableType()) {
      typeVarMap[Expr] = typeVar;
   }

   if (!RequiredType || RequiredType->isAutoType()) {
      return T;
   }

   if (!RequiredType->containsTemplateParamType()
   && !shouldGenerateConversionConstraint) {
      return T;
   }

   auto* Var = T->asTypeVariableType();
   if (!Var) {
      if (T->containsTypeVariable()) {
         Var = Sys.newTypeVariable();
         Sys.newConstraint<TypeEqualityConstraint>(Var, T, nullptr);
      }
      else {
         Var = Sys.newTypeVariable(ConstraintSystem::HasConcreteBinding);
         Sys.newConstraint<TypeBindingConstraint>(Var, T, nullptr);
      }
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
         Binder.visit(RequiredType->getCanonicalType(),
                      DirectBinding->getCanonicalType());
      }
      else {
         TypeParamBinder Binder(Sys, Bindings, Locator, outerBuilder);
         Binder.visit(RequiredType->getCanonicalType(), T->getCanonicalType());
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
                                                      Locator, allowedStrength);
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
      Bindings.ExprBindings.try_emplace(E, typeVar);
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
   auto E = Expr->getExpr();
   for (auto& A : Expr->getAttributes()) {
      switch (A->getKind()) {
#define CDOT_EXPR_ATTR(Name, Spelling)                                         \
   case AttrKind::Name:                                                        \
      Sema.check##Name##Attr(E, cast<Name##Attr>(A));                          \
      break;
#define CDOT_TYPE_ATTR(Name, Spelling)                                         \
   case AttrKind::Name:                                                        \
      Sema.check##Name##Attr(E, cast<Name##Attr>(A));                          \
      break;
#include "cdotc/AST/Attributes.def"

      default:
         llvm_unreachable("bad expr attr");
      }
   }

   return visitExpr(Expr->getExpr());
}

QualType ConstraintBuilder::visitTryExpr(TryExpr* Expr, SourceType T)
{
   SemaPass::TryScopeRAII TSR(Sema);
   return visitExpr(Expr->getExpr());
}

QualType ConstraintBuilder::visitAwaitExpr(AwaitExpr* Expr, SourceType T)
{
   return visitExpr(Expr->getExpr());
}

QualType ConstraintBuilder::visitParenExpr(ParenExpr* Expr, SourceType T)
{
   return visitExpr(Expr->getParenthesizedExpr());
}

QualType ConstraintBuilder::visitIntegerLiteral(IntegerLiteral* Expr,
                                                SourceType T)
{
   // Allow 0 to represent a nullptr.
   if (T && T->isPointerType() && Expr->getValue().isNullValue()) {
      return T;
   }

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
      QualType Ty = getRValue(Val);
      newConstraint<ConformanceConstraint>(Ty, StrRep, makeLocator(Val));
   }

   auto* Var = Sys.newTypeVariable(ConstraintSystem::HasConcreteBinding);
   Sys.newConstraint<TypeBindingConstraint>(
       Var, Sema.Context.getRecordType(Str), nullptr);

   return Var;
}

QualType ConstraintBuilder::visitTupleLiteral(TupleLiteral* Expr, SourceType T)
{
   auto Elements = Expr->getElements();
   unsigned Arity = Elements.size();

   ArrayRef<SourceType> ContextualElementTypes;
   if (auto* Tup = dyn_cast_or_null<TupleTypeExpr>(T.getTypeExpr())) {
      ContextualElementTypes = Tup->getContainedTypes();
   }

   ArrayRef<QualType> ContextualTypes;
   if (T.isResolved() && T->isTupleType()) {
      ContextualTypes = T->uncheckedAsTupleType()->getContainedTypes();
   }

   SmallVector<QualType, 2> ElementTypes;
   ElementTypes.reserve(Arity);

   unsigned i = 0;
   for (auto* Val : Elements) {
      SourceType ContextualElementType;
      if (i < ContextualElementTypes.size()) {
         ContextualElementType = ContextualElementTypes[i];
      }
      else if (i < ContextualTypes.size()) {
         ContextualElementType = ContextualTypes[i];
      }

      QualType ValueTypeVar = getRValue(Val, ContextualElementType);
      ElementTypes.push_back(ValueTypeVar);

      ++i;
   }

   // Equate the type of the whole expression with a tuple type containing
   // the element types.
   QualType ExprType = Sema.Context.getTupleType(ElementTypes);
   TypeVariable TupleTy;

   if (ExprType->containsTypeVariable()) {
      TupleTy = Sys.newTypeVariable();
      Sys.newConstraint<TypeEqualityConstraint>(TupleTy, ExprType, nullptr);
   }
   else {
      TupleTy = Sys.newTypeVariable(ConstraintSystem::HasConcreteBinding);
      Sys.newConstraint<TypeBindingConstraint>(TupleTy, ExprType, nullptr);
   }

   return TupleTy;
}

QualType ConstraintBuilder::visitArrayLiteral(ArrayLiteral* Expr, SourceType T)
{
   TypeVariable Var = Sys.newTypeVariable();
   auto* Loc
       = makeLocator(Expr, PathElement::contextualType(T.getSourceRange()));

   // Constrain each element to be convertible to a common type.
   TypeVariable ElementTy = Sys.newTypeVariable();
   for (auto* E : Expr->getValues()) {
      auto ElVar = visitExpr(E);
      newConstraint<ImplicitConversionConstraint>(ElVar, ElementTy, nullptr);
   }

   Sys.newConstraint<LiteralConstraint>(Var, LiteralConstraint::ArrayLiteral,
                                        Loc, ElementTy);

   return Var;
}

QualType ConstraintBuilder::visitDictionaryLiteral(DictionaryLiteral* Expr,
                                                   SourceType T)
{
   TypeVariable Var = Sys.newTypeVariable();
   auto* Loc
       = makeLocator(Expr, PathElement::contextualType(T.getSourceRange()));

   // Constrain each key type to be convertible to a common type.
   TypeVariable KeyTy = Sys.newTypeVariable();
   for (auto* E : Expr->getKeys()) {
      auto ElVar = visitExpr(E);
      newConstraint<ImplicitConversionConstraint>(ElVar, KeyTy, nullptr);
   }

   // Constrain the key type to be hashable.
   Sys.newConstraint<ConformanceConstraint>(KeyTy, Sema.getHashableDecl(),
                                            nullptr);

   // Constrain each element type to be convertible to a common type.
   TypeVariable ElementTy = Sys.newTypeVariable();
   for (auto* E : Expr->getValues()) {
      auto ElVar = visitExpr(E);
      newConstraint<ImplicitConversionConstraint>(ElVar, ElementTy, nullptr);
   }

   Sys.newConstraint<LiteralConstraint>(
       Var, LiteralConstraint::DictionaryLiteral, Loc, KeyTy, ElementTy);

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
      auto TypeVar = getClosureParam(Expr->getDeclName());
      if (TypeVar) {
         return TypeVar;
      }
   }

   return nullptr;
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

      QualType RHSType;
      if (GeneratingArgConstraints && T) {
         Sys.newConstraint<ImplicitConversionConstraint>(
             PointerType, T, makeLocator(Expr));

         if (T->isMutablePointerType()) {
            RHSType = Sema.Context.getMutablePointerType(Pointee);
         }
      }

      if (!RHSType) {
         RHSType = Sema.Context.getPointerType(Pointee);
      }

      // The pointer type must be a raw pointer pointing to the pointee type.
      Sys.newConstraint<TypeEqualityConstraint>(PointerType, RHSType, nullptr);

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

QualType ConstraintBuilder::visitTupleMemberExpr(TupleMemberExpr* Expr,
                                                 SourceType T)
{
   auto Result = Sema.typecheckExpr(Expr->getParentExpr());
   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   Expr->setParentExpr(Result.get());
   Result = Sema.visitExpr(Expr);

   if (!Result) {
      EncounteredError = true;
      return Sys.newTypeVariable();
   }

   return Result.get()->getExprType();
}

static bool addCandidateDecl(CandidateSet& CandSet, SemaPass& Sema,
                             NamedDecl* ND, Expression *Expr,
                             TemplateArgListExpr *TemplateArgs)
{
   if (auto* CD = dyn_cast<CallableDecl>(ND)) {
      auto *Cand = CandSet.addCandidate(CD, 0);
      if (Cand && CD->isTemplate() && TemplateArgs) {
         TemplateArgList ArgList(Sema, CD, TemplateArgs->getExprs(), Expr->getSourceLoc());
         assert(ArgList.isStillDependent());

         Cand->InnerTemplateArgs = move(ArgList);
      }

      return false;
   }

   QualType T = Expr->getExprType();
   if (!T) {
      return false;
   }

   // Anonymous call
   if (auto* FnTy = T->asFunctionType()) {
      CandSet.addCandidate(ND);
      return false;
   }

   // Initializer call
   if (auto* Meta = T->asMetaType()) {
      QualType initializedType = Meta->getUnderlyingType();

      if (auto *Param = initializedType->asTemplateParamType()) {
         initializedType = Param->getCovariance();
      }
      if (auto *AT = initializedType->asAssociatedType()) {
         initializedType = AT->getDecl()->getCovariance();
      }

      SmallVector<QualType, 1> Types;
      if (initializedType->isRecordType()) {
         Types.push_back(initializedType);
      }
      else if (auto *Ext = initializedType->asExistentialType()) {
         for (QualType ET : Ext->getExistentials())
            Types.push_back(ET);
      }

      for (QualType Ty : Types) {
         QualType RT = Sema.Context.getRecordType(Ty->getRecord());
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

      if (!initializedType->isRecordType()) {
         return false;
      }

      FinalTemplateArgumentList *TemplateArgs = nullptr;
      if (auto *DepRec = initializedType->asDependentRecordType()) {
         QualType realTy = Sema.Context.getRecordType(
             initializedType->getRecord());

         // Don't use the standard template parameters that are given to
         // unspecified template types.
         if (realTy != initializedType) {
            TemplateArgs = &DepRec->getTemplateArgs();
            initializedType = realTy;
         }
      }

      auto* R = initializedType->getRecord();
      auto initName = Sema.Context.getDeclNameTable().getConstructorName(
          initializedType, true);

      const MultiLevelLookupResult* LookupRes;
      if (Sema.QC.MultiLevelLookup(LookupRes, R, initName)) {
         return true;
      }

      for (auto* initDecl : LookupRes->allDecls()) {
         auto *Cand = CandSet.addCandidate(cast<InitDecl>(initDecl));
         if (Cand) {
            Cand->OuterTemplateArgs = TemplateArgs;
         }
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
   auto* TemplateArgs = dyn_cast<TemplateArgListExpr>(ParentExpr);

   IdentifierRefExpr* Ident;
   if (TemplateArgs) {
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

   if (TemplateArgs) {
      ParentExpr = TemplateArgs->getParentExpr();
   }

   if (auto* DeclRef = dyn_cast<DeclRefExpr>(ParentExpr)) {
      if (auto *TAExpr = DeclRef->getTemplateArgs()) {
         assert(!TemplateArgs && "duplicate template args!");
         TemplateArgs = TAExpr;
      }

      auto* ND = DeclRef->getDecl();
      if (addCandidateDecl(CandSet, Sema, ND, DeclRef, TemplateArgs)) {
         Call->setIsInvalid(true);
      }

      Decls.push_back(ND);
      Name = ND->getDeclName();
   }
   else if (auto* MemRef = dyn_cast<MemberRefExpr>(ParentExpr)) {
      auto* ND = MemRef->getMemberDecl();
      if (addCandidateDecl(CandSet, Sema, ND, MemRef, TemplateArgs)) {
         Call->setIsInvalid(true);
      }

      Decls.push_back(ND);
      Name = ND->getDeclName();
      SelfVal = MemRef->getParentExpr();
   }
   else if (auto* Ovl = dyn_cast<OverloadedDeclRefExpr>(ParentExpr)) {
      if (auto *TAExpr = Ovl->getTemplateArgs()) {
         assert(!TemplateArgs && "duplicate template args!");
         TemplateArgs = TAExpr;
      }

      for (auto* ND : Ovl->getOverloads()) {
         if (addCandidateDecl(CandSet, Sema, ND, Ovl, TemplateArgs)) {
            Call->setIsInvalid(true);
         }

         Decls.push_back(ND);
      }

      assert(!Decls.empty() && "should have been diagnosed before!");
      Name = Decls.front()->getDeclName();
      overloadExpr = Ovl;
      SelfVal = Ovl->getParentExpr();
   }
   else {
      TypeDependent |= ParentExpr->isDependent();
      SelfVal = ParentExpr->getParentExpr();

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
   if (T && T->containsTypeVariable()) {
      T = SourceType();
   }

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
      if (GeneratingArgConstraints && !CandSet.InvalidCand) {
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
   if (Cand->isAnonymousCandidate()) {
      ReturnType = Cand->getFunctionType()->getReturnType();
   }
   else if (!Cand->getFunc()->isCompleteInitializer()) {
      ReturnType = Cand->getFunctionType()->getReturnType();

      if (Cand->getFunc()->isImportedFromClang()) {
         ReturnType = Sema.TransformImportedCType(ReturnType);
      }
      else if (auto *M = dyn_cast<MethodDecl>(Cand->getFunc())) {
         if (M->isProtocolRequirement() || M->isProtocolDefaultImpl()) {
            if (auto *AT = ReturnType->asAssociatedType()) {
               CanType ParentType = CandSet.ResolvedArgs.front()->getExprType();
               ParentType = ParentType->removeMetaType();

               if (AT->getDecl()->isSelf()) {
                  ReturnType = ParentType;
               }
               else if (ParentType->isTemplateParamType() || ParentType->isAssociatedType()) {
                  ReturnType = Sema.Context.getAssociatedType(AT->getDecl(), ParentType);
               }
            }
         }
         else if (M->isTemplateOrInTemplate()
         && SelfVal->getExprType()->removeReference()->hasTemplateArgs()) {
            if (Sema.QC.SubstTemplateParamTypes(
                   ReturnType, ReturnType,
                   SelfVal->getExprType()->removeReference()->getTemplateArgs(),
                   Call->getSourceRange())) {
               EncounteredError = true;
            }
         }
      }
   }
   else {
      auto *fn = cast<InitDecl>(Cand->getFunc());

      // e.g.`<T: FixedWidthInteger> T(3)`
      // This calls an initializer on BinaryInteger, but should return a value
      // of type T.
      if (isa<ProtocolDecl>(fn->getRecord())) {
         if (auto *PE = Call->getParentExpr()) {
            QualType ParentType = PE->ignoreParens()->getExprType()->removeMetaType();
            if (ParentType->isAssociatedType() || ParentType->isTemplateParamType()) {
               ReturnType = ParentType;
            }
         }
      }

      if (!ReturnType) {
         if (fn->getRecord()->isTemplate() && Cand->OuterTemplateArgs) {
            ReturnType = Sema.Context.getDependentRecordType(
                fn->getRecord(), Cand->OuterTemplateArgs);
         }
         else {
            ReturnType = Sema.Context.getRecordType(fn->getRecord());
         }
      }

      if (fn->isFallible()) {
         ReturnType = Sema.getOptionOf(ReturnType, Call);
      }
   }

   if (!Cand->isAnonymousCandidate() && Cand->getFunc()->isTemplate()) {
      if (Sema.QC.SubstTemplateParamTypesNonFinal(ReturnType, ReturnType,
                                                  Cand->InnerTemplateArgs,
                                                  Call->getSourceRange())) {
         EncounteredError = true;
      }
   }

   Data.BestCand = Cand;
   Data.ReturnType = ReturnType;

   return ReturnType;
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
         Sys.QC.Sema->diagnose(err_lambda_complex_return,
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

   CANT_INFER(AnonymousCall)

#undef CANT_INFER
};

} // anonymous namespace

QualType ConstraintBuilder::visitLambdaExpr(LambdaExpr* Expr, SourceType T)
{
   SmallVector<QualType, 2> ParamTypes;
   ParamTypes.reserve(Expr->getArgs().size());

   SmallVector<FunctionType::ParamInfo, 2> ParamInfo;
   ParamTypes.reserve(Expr->getArgs().size());

   llvm::DenseMap<unsigned, QualType> ClosureParams;
   auto SAR = support::saveAndRestore(this->ClosureParams, &ClosureParams);

   unsigned i = 0;
   bool FoundParamWithoutType = false;

   FunctionType *ReqType = nullptr;
   if (T && T->isFunctionType()) {
      ReqType = T->asFunctionType();
   }

   for (auto* Arg : Expr->getArgs()) {
      auto TypeRes = Sema.visitSourceType(Expr, Arg->getType());
      if (!TypeRes || TypeRes.get()->isAutoType()) {
         FoundParamWithoutType = true;

         QualType ParamTypeVar = Sys.newTypeVariable();
         if (ReqType && ReqType->getNumParams() > i) {
            ParamInfo.emplace_back(ReqType->getParamInfo()[i]);
         }
         else {
            ParamInfo.emplace_back(ArgumentConvention::Default);
         }

         switch (ParamInfo.back().getConvention()) {
         case ArgumentConvention::ImmutableRef:
            ParamTypeVar = Sema.Context.getReferenceType(ParamTypeVar);
            break;
         case ArgumentConvention::MutableRef:
            ParamTypeVar = Sema.Context.getMutableReferenceType(ParamTypeVar);
            break;
         default:
            break;
         }

         ParamTypes.push_back(ParamTypeVar);
         ClosureParams[i] = ParamTypeVar;
      }
      else {
         ParamTypes.push_back(TypeRes.get());
         ParamInfo.emplace_back(Arg->getConvention(), Arg->getLabel());
      }

      ++i;
   }

   // If there is no parameter whose type is not specified, we can typecheck
   // the closure body to get the return type.
   QualType ReturnType;
   if (!FoundParamWithoutType) {
      if (!Sema.visitExpr(Expr)) {
         EncounteredError = true;
         return Sys.newTypeVariable();
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

      Sys.newConstraint<DefaultableConstraint>(
          RetTyVar, Sema.Context.getEmptyTupleType(), nullptr);
   }
   else {
      Sys.setPreferredBinding(RetTyVar, ReturnType);
   }

   // Equate the type of the whole expression with a tuple type containing
   // the element types.
   QualType ExprType = Sema.Context.getLambdaType(RetTyVar, ParamTypes, ParamInfo);

   TypeVariable FunctionTy = Sys.newTypeVariable(ConstraintSystem::HasConcreteBinding);
   Sys.newConstraint<TypeBindingConstraint>(FunctionTy, ExprType, nullptr);

   return FunctionTy;
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

QualType ConstraintBuilder::visitCastExpr(CastExpr* Cast, SourceType T)
{
   // Right hand side might not have been parsed as a type, check if we were
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

   auto &to = Cast->getTargetType();
   if (!to->isMetaType()) {
      Sema.diagnose(
          Cast, err_expression_in_type_position,
          Cast->getTargetType().getSourceRange(Cast->getSourceRange()));
   }

   (void) visitExpr(Cast->getTarget(), to->removeMetaType(),
                    nullptr, true, Cast->getStrength());

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
      Sema.diagnose(Expr, err_cannot_mutably_borrow, ReferenceTy,
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

   QualType TrueType = getRValue(TrueVal, T);
   TypeVariableType *TrueTypeVar = TrueType->asTypeVariableType();

   if (!TrueTypeVar) {
      TrueTypeVar = Sys.newTypeVariable();
      Sys.newConstraint<TypeBindingConstraint>(
          TrueTypeVar, TrueType->removeReference(), nullptr);
   }

   QualType FalseType = getRValue(FalseVal, T);
   TypeVariableType *FalseTypeVar = FalseType->asTypeVariableType();

   if (!FalseTypeVar) {
      FalseTypeVar = Sys.newTypeVariable();
      Sys.newConstraint<TypeBindingConstraint>(
          FalseTypeVar, FalseType->removeReference(), nullptr);
   }

   // Create a type variable for a common type between TrueVal and FalseVal.
   auto* commonType = Sys.newTypeVariable();

   Sys.newConstraint<ImplicitConversionConstraint>(TrueTypeVar, commonType,
                                                   makeLocator(TrueVal));

   Sys.newConstraint<ImplicitConversionConstraint>(FalseTypeVar, commonType,
                                                   makeLocator(TrueVal));

   return commonType;
}

QualType ConstraintBuilder::visitStaticExpr(StaticExpr* Expr, SourceType T)
{
   return visitExpr(Expr->getExpr(), T);
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

QualType ConstraintBuilder::visitTraitsExpr(TraitsExpr*, SourceType)
{
   llvm_unreachable("should have been replaced!");
}

QualType ConstraintBuilder::visitMixinExpr(MixinExpr*, SourceType)
{
   llvm_unreachable("should have been replaced!");
}

QualType ConstraintBuilder::visitMacroVariableExpr(MacroVariableExpr*,
                                                   SourceType)
{
   llvm_unreachable("should have been replaced!");
}

QualType ConstraintBuilder::visitMacroExpansionExpr(MacroExpansionExpr*,
                                                    SourceType)
{
   llvm_unreachable("should have been replaced!");
}

QualType ConstraintBuilder::visitFunctionTypeExpr(FunctionTypeExpr*,
                                                  SourceType)
{
   llvm_unreachable("should have been replaced!");
}

QualType ConstraintBuilder::visitTupleTypeExpr(TupleTypeExpr*,
                                               SourceType)
{
   llvm_unreachable("should have been replaced!");
}

QualType ConstraintBuilder::visitArrayTypeExpr(ArrayTypeExpr*,
                                               SourceType)
{
   llvm_unreachable("should have been replaced!");
}

QualType ConstraintBuilder::visitDeclTypeExpr(DeclTypeExpr*, SourceType)
{
   llvm_unreachable("should have been replaced!");
}

QualType ConstraintBuilder::visitReferenceTypeExpr(ReferenceTypeExpr*,
                                                   SourceType)
{
   llvm_unreachable("should have been replaced!");
}

QualType ConstraintBuilder::visitPointerTypeExpr(PointerTypeExpr*,
                                                 SourceType)
{
   llvm_unreachable("should have been replaced!");
}

QualType ConstraintBuilder::visitOptionTypeExpr(OptionTypeExpr*,
                                                SourceType)
{
   llvm_unreachable("should have been replaced!");
}

QualType ConstraintBuilder::visitExistentialTypeExpr(ExistentialTypeExpr*,
                                                     SourceType)
{
   llvm_unreachable("should have been replaced!");
}

QualType ConstraintBuilder::visitConstraintExpr(ConstraintExpr*,
                                                SourceType)
{
   llvm_unreachable("should have been replaced!");
}

QualType ConstraintBuilder::visitTemplateArgListExpr(TemplateArgListExpr*,
                                                     SourceType)
{
   llvm_unreachable("should have been replaced!");
}

QualType ConstraintBuilder::visitExprSequence(ExprSequence*, SourceType)
{
   llvm_unreachable("should have been replaced!");
}

QualType ConstraintBuilder::visitDeclRefExpr(DeclRefExpr*, SourceType)
{
   llvm_unreachable("should have been replaced!");
}

QualType ConstraintBuilder::visitMemberRefExpr(MemberRefExpr*, SourceType)
{
   llvm_unreachable("should have been replaced!");
}

QualType ConstraintBuilder::visitSelfExpr(SelfExpr*, SourceType)
{
   llvm_unreachable("should have been replaced!");
}

QualType ConstraintBuilder::visitSuperExpr(SuperExpr*, SourceType)
{
   llvm_unreachable("should have been replaced!");
}

QualType ConstraintBuilder::visitCallExpr(CallExpr*, SourceType)
{
   llvm_unreachable("should have been replaced!");
}

QualType ConstraintBuilder::visitSubscriptExpr(SubscriptExpr *Expr,
                                               SourceType T)
{
   assert(Expr->getCallExpr() && "should have been replaced!");

   if (auto *ResolvedCall = dyn_cast<CallExpr>(Expr->getCallExpr())) {
      return visitExpr(ResolvedCall, T);
   }

   auto ResultType = visitExpr(Expr->getCallExpr());
   if (!ResultType || ResultType->isErrorType()) {
      return ResultType;
   }

   auto &Ovl = UnresolvedCalls[cast<AnonymousCallExpr>(Expr->getCallExpr())];
   if (!Ovl.BestCand) {
      return QualType();
   }

   auto *Fn = cast<MethodDecl>(Ovl.BestCand->getFunc());
   SubscriptDecl *Sub = Sema.getSubscriptDecl(Fn);
   Expr->setSubscriptDecl(Sub);

   bool IsGet = Fn->getDeclName().getSubscriptKind()
       == DeclarationName::SubscriptKind::Getter;

   // Check read subscript getter.
   if (Sub->isReadWrite() && IsGet) {
      if (!Sub->hasSetter()) {
         ResultType = ResultType->getTemplateArgs().front().getType();
      }

      ResultType = Sema.Context.getMutableReferenceType(ResultType);
   }
   else if (!IsGet) {
      ResultType = Sema.Context.getMutableReferenceType(Sub->getType());
   }

   return ResultType;
}

QualType ConstraintBuilder::visitBuiltinExpr(BuiltinExpr*, SourceType)
{
   llvm_unreachable("should have been replaced!");
}

QualType ConstraintBuilder::visitExpressionPattern(ExpressionPattern*,
                                                   SourceType)
{
   llvm_unreachable("should have been replaced!");
}

QualType ConstraintBuilder::visitCasePattern(CasePattern*, SourceType)
{
   llvm_unreachable("should have been replaced!");
}

QualType ConstraintBuilder::visitIsPattern(IsPattern*, SourceType)
{
   llvm_unreachable("should have been replaced!");
}

QualType ConstraintBuilder::visitUnaryOperator(UnaryOperator*,
                                               SourceType)
{
   llvm_unreachable("should have been replaced!");
}

QualType ConstraintBuilder::visitBinaryOperator(BinaryOperator*,
                                                SourceType)
{
   llvm_unreachable("should have been replaced!");
}