//
// Created by Jonas Zell on 2018-12-12.
//

#include "OverloadResolver.h"

#include "AST/Decl.h"
#include "AST/TypeVisitor.h"
#include "ConstraintBuilder.h"
#include "Query/QueryContext.h"
#include "SemaPass.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::sema;
using namespace cdot::support;

using PathElement = ConstraintLocator::PathElement;
using DeclArgMapType = llvm::DenseMap<FuncArgDecl*, std::vector<Expression*>>;
using TemplateParamSet = llvm::SmallPtrSet<TemplateParamDecl*, 2>;

static bool checkImplicitLabel(IdentifierInfo *NeededLabel,
                               IdentifierInfo *GivenLabel,
                               Expression *E) {
   if (GivenLabel && GivenLabel->isStr("_")) {
      return true;
   }

   // Trailing closures don't need labels.
   // FIXME actually check for trailing closures
   if (isa<LambdaExpr>(E)) {
      return true;
   }

   if (auto *Ident = dyn_cast<IdentifierRefExpr>(E->ignoreParensAndImplicitCasts())) {
      if ((!Ident->getParentExpr()
           || isa<SelfExpr>(Ident->getParentExpr()->ignoreParensAndImplicitCasts()))
          && Ident->getIdentInfo() == NeededLabel) {
         return true;
      }
   }

   return false;
}

static bool matchingLabels(SemaPass &Sema,
                           CandidateSet &CandSet,
                           CandidateSet::Candidate &Cand,
                           ArrayRef<Expression*> UnorderedArgs,
                           ArrayRef<IdentifierInfo*> Labels,
                           DeclArgMapType &DeclArgMap) {
   CallableDecl *CD = Cand.getFunc();
   ArrayRef<FuncArgDecl*> ArgDecls = CD->getArgs();
   FuncArgDecl *VariadicArgDecl = nullptr;

   bool CStyleVararg = CD->isCstyleVararg();
   unsigned NumGivenArgs = UnorderedArgs.size();

   unsigned i = 0;
   unsigned LabelNo = 0;

   for (auto *ArgVal : UnorderedArgs) {
      // Check if this argument is labeled.
      IdentifierInfo *Label;
      if (isa<MethodDecl>(CD) && i == 0 && !CD->isCompleteInitializer()) {
         Label = nullptr;
      }
      else if (LabelNo < Labels.size()) {
         Label = Labels[LabelNo++];
      }
      else {
         Label = nullptr;
      }

      // If so, look for an argument with that label.
      if (Label && !Label->isStr("_")) {
         bool FoundLabel = false;
         for (auto *ArgDecl : ArgDecls) {
            if (ArgDecl->getLabel() == Label
                || ArgDecl->isVariadicArgPackExpansion()) {
               FoundLabel = true;
               DeclArgMap[ArgDecl].push_back(ArgVal);
               break;
            }
         }

         if (!FoundLabel) {
            Cand.setHasIncompatibleLabel(i, Label);
            return false;
         }

         ++i;
         continue;
      }

      // Otherwise, if the argument is labeled, bail out.
      FuncArgDecl *ArgDecl;
      if (!VariadicArgDecl && i < ArgDecls.size()) {
         ArgDecl = ArgDecls[i];
      }
      else {
         ArgDecl = VariadicArgDecl;
      }

      if (!ArgDecl) {
         if (CStyleVararg) {
            DeclArgMap[nullptr].push_back(ArgVal);

            ++i;
            continue;
         }

         Cand.setHasTooManyArguments(NumGivenArgs, ArgDecls.size());
         return false;
      }

      // Allow missing labels for operator calls.
      if (ArgDecl->getLabel() != nullptr) {
         if (!checkImplicitLabel(ArgDecl->getLabel(), Label, ArgVal)) {
            Cand.setHasIncompatibleLabel(i, Label);
            return false;
         }
      }

      if (ArgDecl->isVariadicArgPackExpansion()) {
         VariadicArgDecl = ArgDecl;
      }

      DeclArgMap[ArgDecl].push_back(ArgVal);
      ++i;
   }

   return true;
}

static bool matchingAnonymousLabels(SemaPass &Sema,
                                    CandidateSet &CandSet,
                                    CandidateSet::Candidate &Cand,
                                    ArrayRef<Expression*> UnorderedArgs,
                                    ArrayRef<IdentifierInfo*> Labels,
                                    std::vector<StmtOrDecl> &ArgExprs) {
   ArgExprs.resize(UnorderedArgs.size());

   auto *FnTy = Cand.getFunctionType();
   auto ParamInfo = FnTy->getParamInfo();
   unsigned NumGivenArgs = UnorderedArgs.size();

   unsigned i = 0;
   unsigned LabelNo = 0;

   for (auto *ArgVal : UnorderedArgs) {
      // Check if this argument is labeled.
      IdentifierInfo *Label;
      if (LabelNo < Labels.size()) {
         Label = Labels[LabelNo++];
      }
      else {
         Label = nullptr;
      }

      // If it is, look for a parameter with that label.
      if (Label && !Label->isStr("_")) {
         bool FoundLabel = false;
         unsigned j = 0;
         for (auto &PI : ParamInfo) {
            if (PI.getLabel() == Label) {
               FoundLabel = true;
               ArgExprs[j] = ArgVal;
               break;
            }

            ++i;
         }

         if (!FoundLabel) {
            Cand.setHasIncompatibleLabel(i, Label);
            return false;
         }

         ++i;
         continue;
      }

      // Otherwise, if the argument is labeled, bail out.
      const FunctionType::ParamInfo *PI = nullptr;
      if (i < ParamInfo.size()) {
         PI = &ParamInfo[i];
      }

      if (!PI) {
         Cand.setHasTooManyArguments(NumGivenArgs, ParamInfo.size());
         return false;
      }

      // Allow missing labels in some situations.
      if (PI->getLabel() != nullptr) {
         if (!checkImplicitLabel(PI->getLabel(), Label, ArgVal)) {
            Cand.setHasIncompatibleLabel(i, Label);
            return false;
         }
      }

      ArgExprs[i++] = ArgVal;
   }

   return true;
}

namespace {

class TemplateParamVisitor: public RecursiveTypeVisitor<TemplateParamVisitor> {
   /// Set of encountered variadic template parameters.
   TemplateParamSet &VariadicParams;

public:
   explicit TemplateParamVisitor(TemplateParamSet &VariadicParams)
      : VariadicParams(VariadicParams)
   {}

   bool visitTemplateParamType(TemplateParamType *T)
   {
      if (T->getParam()->isVariadic()) {
         VariadicParams.insert(T->getParam());
      }

      return false;
   }
};

class TemplateParamFinder: public RecursiveTypeVisitor<TemplateParamFinder> {
   /// Reference to the Sema instance.
   SemaPass &Sema;

   /// Reference to the constraint builder.
   ConstraintBuilder &Builder;

   /// The template argument list.
   MultiLevelTemplateArgList &TemplateArgs;

   /// Set of encountered variadic template parameters.
   TemplateParamSet &VariadicParams;

   /// Set to true if we were able to infer the variadic parameter.
   bool CouldInfer = false;

   /// Set to true if the type layout is incompatible.
   bool IncompatibleType = false;

public:
   /// Set to true if we encountered an error.
   bool EncounteredError = false;

private:
   ExprResult visitDefault(QualType T, Expression *&E)
   {
      auto Result = Sema.visitExpr(E);
      if (!Result) {
         EncounteredError = true;
         return Result;
      }

      E = Result.get();

      TemplateParamSet InferredParams;
      TemplateParamVisitor{InferredParams}.visit(T);

      if (!TemplateArgs.inferFromType(E->getExprType(), T)) {
         IncompatibleType = true;
      }

      return E;
   }

   void visit(QualType T, Expression *E = nullptr)
   {
      switch (T->getTypeID()) {
      case Type::TupleTypeID: {
         visitTupleType(T->asTupleType(), E);
         break;
      }
      case Type::FunctionTypeID:
      case Type::LambdaTypeID: {
         visitFunctionType(T->asFunctionType(), E);
         break;
      }
      default:
         break;
      }

      switch (T->getTypeID()) {
#     define CDOT_TYPE(NAME, Parent)                              \
      case Type::NAME##ID: visit##NAME(T->as##NAME()); break;
#     include "AST/Types.def"
      }
   }

public:
   explicit TemplateParamFinder(SemaPass &Sema,
                                ConstraintBuilder &Builder,
                                MultiLevelTemplateArgList &TemplateArgs,
                                TemplateParamSet &VariadicParams)
      : Sema(Sema), Builder(Builder), TemplateArgs(TemplateArgs),
        VariadicParams(VariadicParams)
   {}

   bool findTemplateParams(QualType ParamTy, Expression *&Expr)
   {
      assert(ParamTy->containsTemplateParamType());
      visit(ParamTy, Expr);

      if (!CouldInfer) {
         visitDefault(ParamTy, Expr);
      }

      return IncompatibleType;
   }

   bool visitTupleType(TupleType *T, Expression *Expr = nullptr)
   {
      auto *Tup = dyn_cast_or_null<TupleLiteral>(Expr);
      TemplateParamDecl *VariadicParam = nullptr;

      unsigned i = 0;
      unsigned VariadicIdx = -1;

      for (auto &El : T->getContainedTypes()) {
         auto *Param = El->asTemplateParamType();
         if (!Param) {
            if (El->properties().containsUnexpandedParameterPack()) {
               if (Tup) {
                  visit(El, Tup->getElements()[i]);
               }
            }
         }
         else if (Param->isVariadic()) {
            VariadicParam = Param->getParam();
            VariadicIdx = i;
            break;
         }

         ++i;
      }

      if (!VariadicParam) {
         return false;
      }

      if (!Tup) {
         return false;
      }

      CouldInfer = true;
      Builder.registerTemplateParam(VariadicParam);

      // Try to infer based on the literal expressions.
      MutableArrayRef<Expression*> Exprs = Tup->getElements();

      if (Exprs.size() < VariadicIdx) {
         // Check for empty variadic parameter.
         if (Exprs.size() == VariadicIdx - 1) {
            TemplateArgument Arg(VariadicParam, true,
                                 std::vector<TemplateArgument>(),
                                 Expr->getSourceLoc());

            TemplateArgs.setParamValue(VariadicParam, move(Arg));
            return false;
         }

         // The type layout is not compatible.
         IncompatibleType = true;
         return false;
      }

      bool Inferrable = VariadicParams.insert(VariadicParam).second;
      QualType NeededTy = Sema.Context.getTemplateArgType(VariadicParam);

      unsigned LeftoverElements = T->getArity() - VariadicIdx - 1;
      unsigned j = 0;

      for (i = VariadicIdx; i < Exprs.size() - LeftoverElements; ++i, ++j) {
         auto *&Element = Exprs[i];
         auto *Loc = Builder.makeLocator(
            Element, PathElement::contextualType(Expr->getSourceRange()));

         QualType Ty = Builder.visitExpr(Element, NeededTy, Loc);
         if (auto *TV = Ty->asTypeVariableType()) {
            Builder.Sys.setVariadicParamIdx(TV, j);

            if (Inferrable) {
               auto *ParamVar = Builder.Bindings.ParamBindings[VariadicParam];
               Builder.Sys.newConstraint<InferenceConstraint>(ParamVar, TV,
                                                              nullptr);
            }
         }
      }

      return false;
   }

   bool visitFunctionType(FunctionType *T, Expression *Expr = nullptr)
   {
      return false;
   }
};

static unsigned getPreviousNumVariadicArgs(TemplateParamDecl *Param,
                                           DeclArgMapType &DeclArgMap) {
   for (auto &ArgPair : DeclArgMap) {
      auto *ArgDecl = ArgPair.getFirst();
      if (!ArgDecl->isVariadicArgPackExpansion()) {
         continue;
      }

      if (ArgDecl->getType()->asTemplateParamType()->getParam() == Param) {
         return ArgPair.getSecond().size();
      }
   }

   return 1;
}

} // anonymous namespace

static bool createParamConstraints(ConstraintSystem &Sys,
                                   DeclArgMapType &DeclArgMap,
                                   CandidateSet::Candidate &Cand,
                                   FuncArgDecl *ArgDecl,
                                   unsigned NumGivenArgs,
                                   unsigned i,
                                   std::vector<StmtOrDecl> &ArgExprs,
                                   TemplateParamSet &VariadicParams,
                                   MultiLevelTemplateArgList &templateArgs,
                                   ConstraintBuilder &Builder,
                                   SemaPass &Sema,
                                   Statement *Caller) {
   auto *CD = Cand.getFunc();

   // If no argument is given, check if there is a default one.
   auto It = DeclArgMap.find(ArgDecl);
   if (It == DeclArgMap.end()) {
      if (ArgDecl->isVariadicArgPackExpansion()) {
         auto *Param = ArgDecl->getType()->asTemplateParamType()->getParam();
         bool FirstOccurence = VariadicParams.count(Param) == 0;

         if (FirstOccurence) {
            return false;
         }

         unsigned NeededCount = getPreviousNumVariadicArgs(Param, DeclArgMap);
         if (NeededCount == 0) {
            return false;
         }

         Cand.setHasTooFewArguments(NumGivenArgs, NumGivenArgs + NeededCount);
         return true;
      }

      if (!ArgDecl->getValue()) {
         Cand.setHasTooFewArguments(NumGivenArgs, CD->getArgs().size());
         return true;
      }

      if (Sema.QC.TypecheckDecl(ArgDecl)) {
         Cand.setIsInvalid();
         return true;
      }

      auto DefaultVal = ArgDecl->getDefaultVal();
      if (DefaultVal->isMagicArgumentValue()) {
         auto Alias = cast<IdentifierRefExpr>(DefaultVal)->getAlias();
         auto Result = Sema.HandleReflectionAlias(Alias,
                                                  cast<Expression>(Caller));

         if (Result) {
            ArgExprs.emplace_back(Result.getValue());
         }
         else {
            ArgExprs.emplace_back(DefaultVal);
         }
      }
      else if (CD->isTemplate()
      || CD->isInitializerOfTemplate()
      || CD->isCaseOfTemplatedEnum()) {
         ArgExprs.emplace_back(ArgDecl);
      }
      else {
         ArgExprs.emplace_back(DefaultVal);
      }

      return false;
   }

   MutableArrayRef<Expression*> ArgValues = DeclArgMap[ArgDecl];

   // If the parameter type is a pack expansion, we can use the supplied
   // values directly.
   if (ArgDecl->isVariadicArgPackExpansion()) {
      auto *Param = ArgDecl->getType()->asTemplateParamType()->getParam();
      auto *Arg = templateArgs.getArgForParam(Param);

      if (!Arg->isFrozen() && Arg->getVariadicArgs().empty()) {
         bool Inferrable = VariadicParams.insert(Param).second;
         Builder.registerTemplateParam(Param);

         unsigned i = 0;
         for (auto *ArgVal : ArgValues) {
            auto *Loc = Builder.makeLocator(
               ArgVal, PathElement::parameterType(ArgDecl));

            auto Result = Builder.generateConstraints(ArgVal,
                                                      ArgDecl->getType(),
                                                      Loc);

            switch (Result.Kind) {
            case ConstraintBuilder::Success:
               break;
            case ConstraintBuilder::InvalidArgument:
               Cand.setHasIncompatibleArgument(i, Result.Type, ArgDecl->getType());
               return true;
            case ConstraintBuilder::Failure:
               Cand.setIsInvalid();
               return true;
            case ConstraintBuilder::Dependent:
               Cand.setIsDependent();
               return false;
            }

            QualType Ty = Result.Type;
            ArgExprs.emplace_back(ArgVal);

            TypeVariableType *TV = Ty->asTypeVariableType();
            if (!TV) {
               TV = Sys.newTypeVariable();
               Sys.newConstraint<TypeBindingConstraint>(TV, Ty, nullptr);
            }

            Sys.setVariadicParamIdx(TV, i);

            if (Inferrable) {
               auto *ParamVar = Builder.Bindings.ParamBindings[Param];
               Sys.newConstraint<InferenceConstraint>(ParamVar, TV, nullptr);
            }

            ++i;
         }
      }
      else {
         auto &VAs = Arg->getVariadicArgs();
         if (VAs.size() > ArgValues.size()) {
            Cand.setHasTooFewArguments(
               NumGivenArgs, NumGivenArgs + VAs.size() - ArgValues.size());

            return true;
         }
         else if (VAs.size() < ArgValues.size()) {
            Cand.setHasTooManyArguments(
               NumGivenArgs, NumGivenArgs - VAs.size() + ArgValues.size());

            return true;
         }

         for (auto *ArgVal : ArgValues) {
            auto *Loc = Builder.makeLocator(
               ArgVal, PathElement::parameterType(ArgDecl));

            auto Result =  Builder.generateConstraints(ArgVal,
                                                       VAs[i].getType(),
                                                       Loc);

            switch (Result.Kind) {
            case ConstraintBuilder::Success:
               break;
            case ConstraintBuilder::InvalidArgument:
               Cand.setHasIncompatibleArgument(i, Result.Type,
                                               VAs[i].getType());
               return true;
            case ConstraintBuilder::Failure:
               Cand.setIsInvalid();
               return true;
            case ConstraintBuilder::Dependent:
               Cand.setIsDependent();
               return false;
            }

            ArgExprs.emplace_back(ArgVal);
            ++i;
         }
      }
   }
   else if (ArgDecl->getType()->properties().containsUnexpandedParameterPack()){
      TemplateParamFinder Finder(Sema, Builder, templateArgs, VariadicParams);

      for (auto *&ArgVal : ArgValues) {
         if (Finder.findTemplateParams(ArgDecl->getType(), ArgVal)) {
            Cand.setHasIncompatibleArgument(0, ArgVal->getExprType(),
                                            ArgDecl->getType());

            return true;
         }
         if (Finder.EncounteredError) {
            Cand.setIsInvalid();
            return true;
         }

         auto *Loc = Builder.makeLocator(
            ArgVal, PathElement::parameterType(ArgDecl));

         QualType NeededTy = ArgDecl->getType();
         Sema.QC.SubstTemplateParamTypesNonFinal(NeededTy, NeededTy,
                                           templateArgs,
                                           ArgVal->getSourceRange());

         auto Result = Builder.generateConstraints(ArgVal, NeededTy, Loc);
         switch (Result.Kind) {
         case ConstraintBuilder::Success:
            break;
         case ConstraintBuilder::InvalidArgument:
            Cand.setHasIncompatibleArgument(i, Result.Type, NeededTy);
            return true;
         case ConstraintBuilder::Failure:
            Cand.setIsInvalid();
            return true;
         case ConstraintBuilder::Dependent:
            Cand.setIsDependent();
            return false;
         }

         ArgExprs.emplace_back(ArgVal);
      }
   }
   else {
      assert(ArgValues.size() == 1);

      auto *ArgVal = ArgValues.front();
      auto *Loc = Builder.makeLocator(
         ArgVal, PathElement::parameterType(ArgDecl));

      QualType NeededTy = ArgDecl->getType();
      if (Cand.getFunc()->isTemplate()) {
         Sema.QC.SubstTemplateParamTypesNonFinal(NeededTy, NeededTy,
                                           templateArgs,
                                           ArgVal->getSourceRange());
      }

      auto Result = Builder.generateArgumentConstraints(ArgVal, NeededTy, Loc);
      switch (Result.Kind) {
      case ConstraintBuilder::Success:
         break;
      case ConstraintBuilder::InvalidArgument:
         Cand.setHasIncompatibleArgument(i, Result.Type, NeededTy);
         return true;
      case ConstraintBuilder::Failure:
         Cand.setIsInvalid();
         return true;
      case ConstraintBuilder::Dependent:
         Cand.setIsDependent();
         return false;
      }

      if (!Result.Type->containsTypeVariable()) {
         bool Convertible;
         if (Sema.QC.IsValidParameterValue(Convertible, Result.Type, NeededTy, ArgDecl->isSelf())) {
            Cand.setIsInvalid();
            return true;
         }

         if (!Convertible) {
            Cand.setHasIncompatibleArgument(i, Result.Type, NeededTy);
            return true;
         }
      }

      ArgExprs.emplace_back(ArgVal);
   }

   return false;
}

static bool createAnonymousParamConstraints(ConstraintSystem &Sys,
                                            CandidateSet::Candidate &Cand,
                                            unsigned i,
                                            CanType NeededTy,
                                            ArgumentConvention Conv,
                                            Expression *ArgVal,
                                            ConstraintBuilder &Builder,
                                            SemaPass &Sema,
                                            Statement *Caller) {
   auto *Loc = Builder.makeLocator(ArgVal, {});
   auto Result = Builder.generateArgumentConstraints(ArgVal, NeededTy, Loc);
   switch (Result.Kind) {
   case ConstraintBuilder::Success:
      break;
   case ConstraintBuilder::InvalidArgument:
      Cand.setHasIncompatibleArgument(i, Result.Type, NeededTy);
      return true;
   case ConstraintBuilder::Failure:
      Cand.setIsInvalid();
      return true;
   case ConstraintBuilder::Dependent:
      Cand.setIsDependent();
      return false;
   }

   if (!Result.Type->containsTypeVariable()) {
      bool Convertible;
      if (Sema.QC.IsImplicitlyConvertible(Convertible, Result.Type,
                                          NeededTy)) {
         Cand.setIsInvalid();
         return true;
      }

      if (!Convertible) {
         Cand.setHasIncompatibleArgument(i, Result.Type, NeededTy);
         return true;
      }
   }

   return false;
}

namespace {

class CaptureMarker: public RecursiveASTVisitor<CaptureMarker> {
   ASTContext &C;
   LambdaExpr *LE;

public:
   CaptureMarker(ASTContext &C, LambdaExpr *LE) : C(C), LE(LE)
   {}

   bool visitIdentifierRefExpr(IdentifierRefExpr *Expr)
   {
      switch (Expr->getKind()) {
      case IdentifierKind::LocalVar:
      case IdentifierKind::FunctionArg: {
         Expr->getVarDecl()->setCaptured(true);
         Expr->setCaptureIndex(LE->addCapture(C, Expr->getVarDecl()));
         Expr->setIsCapture(true);

         break;
      }
      default:
         break;
      }

      return true;
   }

   bool visitSelfExpr(SelfExpr *E)
   {
      if (!E->isUppercase()) {
         E->setCaptureIndex(LE->addCapture(C, E->getSelfArg()));
      }

      return true;
   }

   bool visitSuperExpr(SuperExpr *E)
   {
      E->setCaptureIndex(LE->addCapture(C, E->getSelfArg()));
      return true;
   }
};

} // anonymous namespace

static Expression *convertCStyleVarargParam(SemaPass &Sema, Expression *Expr)
{
   Expr = Sema.castToRValue(Expr);

   QualType type = Expr->getExprType();

   // (unsigned) char / short are promoted to int for c-style vararg functions.
   if (type->isIntegerType() && type->getBitwidth() < 32) {
      QualType promotedType = type->isUnsigned()
                              ? Sema.Context.getUInt32Ty()
                              : Sema.Context.getInt32Ty();

      return Sema.forceCast(Expr, promotedType);
   }

   // float is promoted to double for c-style vararg functions.
   if (type->isFloatTy()) {
      return Sema.forceCast(Expr, Sema.Context.getDoubleTy());
   }

   return Expr;
}

static bool applyConversions(SemaPass &SP,
                             CandidateSet &CandSet,
                             CandidateSet::Candidate &Cand,
                             std::vector<StmtOrDecl> &ArgExprs,
                             Statement *Caller) {
   ArrayRef<FuncArgDecl*> ArgDecls;
   if (!Cand.isAnonymousCandidate()) {
      if (Cand.getFunc()->isInvalid()) {
         CandSet.InvalidCand = true;
         return true;
      }

      ArgDecls = Cand.getFunc()->getArgs();
   }

   unsigned i = 0;
   auto ParamTys = Cand.getFunctionType()->getParamTypes();
   bool cstyleVararg = Cand.getFunctionType()->isCStyleVararg();

   for (Expression *&E : CandSet.ResolvedArgs) {
      FuncArgDecl *ArgDecl = nullptr;
      if (i < ArgDecls.size()) {
         ArgDecl = ArgDecls[i];
      }

      // Make sure the expression type is resolved.
      auto Result = SP.visitExpr(Caller, E);
      if (!Result) {
         ++i;
         continue;
      }

      E = Result.get();

      // Convert to the parameter type and apply automatic promotion.
      if (i < ParamTys.size()) {
         E = SP.implicitCastIfNecessary(E, ParamTys[i]);
      }
      else if (cstyleVararg) {
         E = convertCStyleVarargParam(SP, E);
      }

      // Check argument convention and apply implicit conversion.
      if (E->getExprType()->isVoidType()) {
         SP.diagnose(E, diag::err_vararg_cannot_pass_void, E->getSourceRange());
      }

      if (ArgDecl) {
         // Check if the value needs to be moved.
         if (ArgDecl->getConvention() == ArgumentConvention::Owned) {
            if (auto Ident = dyn_cast<IdentifierRefExpr>(E)) {
               auto ND = Ident->getNamedDecl();
               if (auto VD = dyn_cast_or_null<VarDecl>(ND)) {
                  VD->setMovedFrom(true);
               }
            }
         }

         // Handle @autoclosure
         if (ArgDecl->hasAttribute<AutoClosureAttr>()) {
            auto LE = LambdaExpr::Create(
               SP.getContext(), E->getSourceRange(),
               E->getSourceLoc(), SourceType(SP.getContext().getAutoType()),
               {}, ReturnStmt::Create(SP.getContext(), E->getSourceLoc(), E));

            (void) SP.visitExpr(E, LE);
            CaptureMarker(SP.getContext(), LE).visit(E);

            E = LE;
         }
      }
      else {
         E = SP.castToRValue(E);
      }

      ++i;
   }

   return false;
}

static bool replaceDefaultValues(SemaPass &SP,
                                 CandidateSet &CandSet,
                                 CandidateSet::Candidate &Cand,
                                 std::vector<StmtOrDecl> &ArgExprs) {
   if (!Cand.isAnonymousCandidate() && Cand.getFunc()->isInvalid()) {
      CandSet.InvalidCand = true;
      return true;
   }

   for (auto &SOD : ArgExprs) {
      // Check if this is an argument with a default value.
      if (auto *Decl = SOD.asDecl()) {
         // Get the instantiated default value.
         auto *Inst = Cand.getFunc()->lookupSingle<FuncArgDecl>(
            cast<FuncArgDecl>(Decl)->getDeclName());

         if (Inst != Decl) {
            if (SP.QC.TypecheckDecl(Inst)) {
               CandSet.InvalidCand = true;
               CandSet.ResolvedArgs.push_back(
                  cast<FuncArgDecl>(Decl)->getDefaultVal());

               continue;
            }
         }

         SOD = Inst->getDefaultVal();
      }

      CandSet.ResolvedArgs.push_back(cast<Expression>(SOD.getStatement()));
   }

   return false;
}

static bool compatibleReturnType(SemaPass &Sema,
                                 CandidateSet::Candidate &Cand,
                                 CanType RequiredType) {
   QualType ReturnType;
   if (Cand.isAnonymousCandidate()) {
      ReturnType = Cand.getFunctionType()->getReturnType();
   }
   else if (Cand.getFunc()->isCompleteInitializer()) {
      ReturnType = Sema.Context.getRecordType(Cand.getFunc()->getRecord());
   }
   else {
      ReturnType = Cand.getFunctionType()->getReturnType();
   }

   bool Convertible;
   if (Sema.QC.IsImplicitlyConvertible(Convertible, ReturnType, RequiredType)) {
      return true;
   }

   if (!Convertible) {
      Cand.setHasIncompatibleReturnType(RequiredType, ReturnType);
      return false;
   }

   return true;
}

static void bindTemplateParams(SemaPass &Sema,
                               CandidateSet::Candidate &Cand,
                               SmallVectorImpl<ConstraintSystem::Solution> &Solutions,
                               MultiLevelTemplateArgList &templateArgList) {
   for (auto &B : Cand.Builder->Bindings.ParamBindings) {
      QualType ParamTy = Sema.Context.getTemplateArgType(B.getFirst());
      QualType AssignedTy = Solutions.front().AssignmentMap[B.getSecond()];

      if (!AssignedTy) {
         continue;
      }

      if (B.getFirst()->isVariadic()) {
         auto *Tup = AssignedTy->asTupleType();
         assert(Tup && "bad variadic argument");

         for (QualType Cont : Tup->getContainedTypes()) {
            templateArgList.inferFromType(Cont, ParamTy);
         }
      }
      else {
         templateArgList.inferFromType(AssignedTy, ParamTy);
      }
   }
}

static bool resolveCandidate(SemaPass &Sema,
                        std::vector<StmtOrDecl> &ArgExprs,
                        SmallVectorImpl<ConstraintSystem::Solution> &Solutions,
                        CandidateSet &CandSet,
                        CandidateSet::Candidate &Cand,
                        const SourceType &RequiredType,
                        ArrayRef<Expression*> UnorderedArgs,
                        ArrayRef<IdentifierInfo*> Labels,
                        unsigned NumGivenArgs,
                        ConstraintSystem::Solution &BestSolution,
                        CandidateSet::Candidate *&BestCandidate,
                        std::vector<StmtOrDecl> &BestMatchArgExprs,
                        bool &FoundAmbiguity,
                        Statement *Caller) {
   if (Sema.QC.PrepareDeclInterface(Cand.getFunc()) || Cand.getFunc()->isInvalid()) {
      Cand.setIsInvalid();
      CandSet.InvalidCand = true;
      return false;
   }

   bool isTemplateInitializer = Cand.getFunc()->isInitializerOfTemplate()
      || Cand.getFunc()->isCaseOfTemplatedEnum();

   MultiLevelTemplateArgList templateArgList;
   TemplateArgList outerTemplateArgs;

   if (Cand.getFunc()->isTemplate()) {
      Cand.InnerTemplateArgs = TemplateArgList(Sema, Cand.getFunc());
      templateArgList.addOuterList(Cand.InnerTemplateArgs);
   }
   if (isTemplateInitializer) {
      outerTemplateArgs = TemplateArgList(Sema, Cand.getFunc()->getRecord());
      templateArgList.addOuterList(outerTemplateArgs);
   }

   DeclArgMapType DeclArgMap;
   ArgExprs.clear();
   Solutions.clear();

   // Check if the returned type is viable.
   QualType ReturnType = Cand.getFunctionType()->getReturnType();
   bool DependentReturnType = ReturnType->isDependentType();

   if (RequiredType) {
      if (DependentReturnType) {
         templateArgList.inferFromType(RequiredType, ReturnType);
      }
      else if (!compatibleReturnType(Sema, Cand, RequiredType)) {
         return false;
      }
   }

   if (!matchingLabels(Sema, CandSet, Cand, UnorderedArgs, Labels, DeclArgMap)) {
      return false;
   }

   // Create a new constraint system for this overload.
   Cand.Builder = std::make_unique<ConstraintBuilder>(Sema.QC, Caller->getSourceRange());
   ConstraintSystem &Sys = Cand.Builder->Sys;

   // Generate the constraints for each parameter.
   bool Valid = true;
   TemplateParamSet VariadicParams;

   // Register the template parameters.
   if (Cand.getFunc()->isTemplate()) {
      for (auto *Param : Cand.getFunc()->getTemplateParams()) {
         if (!Param->isVariadic()
             && Cand.InnerTemplateArgs.getArgForParam(Param)->isNull()) {
            Cand.Builder->registerTemplateParam(Param);
         }
      }
   }
   if (isTemplateInitializer) {
      for (auto *Param : Cand.getFunc()->getRecord()->getTemplateParams()) {
         if (!Param->isVariadic()
             && outerTemplateArgs.getArgForParam(Param)->isNull()) {
            Cand.Builder->registerTemplateParam(Param);
         }
      }
   }

   unsigned i = 0;
   for (auto *ArgDecl : Cand.getFunc()->getArgs()) {
      if (createParamConstraints(Sys, DeclArgMap, Cand, ArgDecl,
                                 NumGivenArgs, i++, ArgExprs, VariadicParams,
                                 templateArgList,
                                 *Cand.Builder, Sema, Caller)) {
         Valid = false;
         break;
      }
   }

   if (!Valid) {
      CandSet.InvalidCand |= Cand.FR == CandidateSet::IsInvalid;
      return false;
   }

   // Add C-Style varargs.
   if (Cand.getFunc()->isCstyleVararg()) {
      auto It = DeclArgMap.find(nullptr);
      if (It != DeclArgMap.end()) {
         for (Expression *argValue : It->getSecond()) {
            ArgExprs.emplace_back(argValue);
         }
      }
   }

   // Solve the constraint system.
   auto SolveResult = Sys.solve(Solutions);
   switch (SolveResult) {
   case ConstraintSystem::Success:
      break;
   case ConstraintSystem::Failure:
      Sys.solve(Solutions, true);
      if (!Sys.diagnoseCandidateFailure(Cand)) {
         // FIXME we need to still put some error here to prevent Sema
         //  from just moving on...
         Cand.setIsInvalid();
         CandSet.InvalidCand = true;
      }

      return false;
   case ConstraintSystem::Dependent:
      Cand.setIsDependent();
      return false;
   case ConstraintSystem::Error:
      Cand.setIsInvalid();
      CandSet.InvalidCand = true;
      return false;
   }

   if (Solutions.size() != 1) {
      if (!Sys.diagnoseAmbiguity(Solutions[0], Solutions[1])) {
         Sema.diagnose(err_generic_error, "ambiguous solution",
                       Caller->getSourceRange());
      }
   }

   // Apply template parameter bindings.
   if (Cand.getFunc()->isTemplate() || isTemplateInitializer) {
      bindTemplateParams(Sema, Cand, Solutions, templateArgList);
   }

   // Instantiate the template record first.
   if (isTemplateInitializer) {
      if (!Sema.maybeInstantiateRecord(Cand, templateArgList.outermost(), Caller)) {
         Cand.setIsInvalid();
         CandSet.InvalidCand = true;

         return false;
      }
   }

   // Now instantiate template functions or methods.
   Sema.maybeInstantiate(Cand, Caller);

   // Check the return type again in case it was dependent.
   if (RequiredType && DependentReturnType) {
      if (!compatibleReturnType(Sema, Cand, RequiredType)) {
         return false;
      }
   }

   // Check for ambiguity.
   auto &S = Solutions.front();
   Cand.ConversionPenalty = S.Score;

   if (!BestCandidate || BestSolution.Score > S.Score) {
      BestCandidate = &Cand;
      BestSolution = move(S);
      BestMatchArgExprs = move(ArgExprs);
      CandSet.BestConversionPenalty = S.Score;

      return true;
   }

   if (BestSolution.Score == S.Score) {
      auto CompResult = Sys.compareSolutions(BestSolution, S);
      switch (CompResult) {
      case ConstraintSystem::EquivalentSolution:
      case ConstraintSystem::BetterSolution:
         return false;
      case ConstraintSystem::EqualSolution:
         FoundAmbiguity = true;
         return false;
      case ConstraintSystem::WorseSolution:
         BestCandidate = &Cand;
         BestSolution = move(S);
         BestMatchArgExprs = move(ArgExprs);
         CandSet.BestConversionPenalty = S.Score;

         return true;
      }
   }

   return false;
}

static bool resolveAnonymousCandidate(SemaPass &Sema,
                       SmallVectorImpl<ConstraintSystem::Solution> &Solutions,
                       CandidateSet &CandSet,
                       CandidateSet::Candidate &Cand,
                       const SourceType &RequiredType,
                       ArrayRef<Expression*> UnorderedArgs,
                       ArrayRef<IdentifierInfo*> Labels,
                       unsigned NumGivenArgs,
                       ConstraintSystem::Solution &BestSolution,
                       CandidateSet::Candidate *&BestCandidate,
                       std::vector<StmtOrDecl> &BestMatchArgExprs,
                       bool &FoundAmbiguity,
                       Statement *Caller) {
   auto *FnTy = Cand.getFunctionType();
   std::vector<StmtOrDecl> ArgExprs;

   // Check if the returned type is viable.
   if (RequiredType) {
      if (!compatibleReturnType(Sema, Cand, RequiredType)) {
         return false;
      }
   }

   auto Params = FnTy->getParamTypes();
   auto ParamInfo = FnTy->getParamInfo();

   if (NumGivenArgs > Params.size()) {
      Cand.setHasTooManyArguments(NumGivenArgs, Params.size());
      return false;
   }
   else if (NumGivenArgs < Params.size()) {
      Cand.setHasTooFewArguments(NumGivenArgs, Params.size());
      return false;
   }

   DeclArgMapType DeclArgMap;
   if (!matchingAnonymousLabels(Sema, CandSet, Cand, UnorderedArgs, Labels,
                                ArgExprs)) {
      return false;
   }

   // Create a new constraint system for this overload.
   Cand.Builder = std::make_unique<ConstraintBuilder>(Sema.QC, Caller->getSourceRange());
   ConstraintSystem &Sys = Cand.Builder->Sys;

   // Generate the constraints for each parameter.
   bool Valid = true;
   TemplateParamSet VariadicParams;

   unsigned i = 0;
   for (auto &ParamTy : Params) {
      auto *ArgVal = cast<Expression>(ArgExprs[i].getStatement());
      if (createAnonymousParamConstraints(Sys, Cand, i, ParamTy,
                                          ParamInfo[i].getConvention(), ArgVal,
                                          *Cand.Builder, Sema, Caller)) {
         Valid = false;
         break;
      }

      ++i;
   }

   if (!Valid) {
      CandSet.InvalidCand |= Cand.FR == CandidateSet::IsInvalid;
      return false;
   }

   // Solve the constraint system.
   auto SolveResult = Sys.solve(Solutions);
   switch (SolveResult) {
   case ConstraintSystem::Success:
      break;
   case ConstraintSystem::Failure:
      Sys.solve(Solutions, true);
      if (!Sys.diagnoseCandidateFailure(Cand)) {
         // FIXME we need to still put some error here to prevent Sema
         //  from just moving on...
         Cand.setIsInvalid();
         CandSet.InvalidCand = true;
      }

      return false;
   case ConstraintSystem::Dependent:
      Cand.setIsDependent();
      return false;
   case ConstraintSystem::Error:
      Cand.setIsInvalid();
      CandSet.InvalidCand = true;
      return false;
   }

   if (Solutions.size() != 1) {
      if (!Sys.diagnoseAmbiguity(Solutions[0], Solutions[1])) {
         Sema.diagnose(err_generic_error, "ambiguous solution",
                       Caller->getSourceRange());
      }
   }

   // Check for ambiguity.
   auto &S = Solutions.front();
   Cand.ConversionPenalty = S.Score;

   if (!BestCandidate || BestSolution.Score > S.Score) {
      BestCandidate = &Cand;
      CandSet.BestConversionPenalty = S.Score;
      BestSolution = move(S);
      BestMatchArgExprs = move(ArgExprs);

      return true;
   }

   if (BestSolution.Score == S.Score) {
      auto CompResult = Sys.compareSolutions(BestSolution, S);
      switch (CompResult) {
      case ConstraintSystem::EquivalentSolution:
      case ConstraintSystem::BetterSolution:
         return false;
      case ConstraintSystem::EqualSolution:
         FoundAmbiguity = true;
         return false;
      case ConstraintSystem::WorseSolution:
         BestCandidate = &Cand;
         CandSet.BestConversionPenalty = S.Score;
         BestSolution = move(S);
         BestMatchArgExprs = move(ArgExprs);

         return true;
      }
   }

   return false;
}

static bool shouldUseSelfArgument(CallableDecl *Fn, Expression *SelfArg,
                                  ArrayRef<Expression*> UnorderedArgs) {
   auto *M = dyn_cast<MethodDecl>(Fn);
   if (!M || M->isCompleteInitializer()) {
      return Fn->isOperator();
   }

   // Because of the way operators are handled, sometimes there are two self
   // arguments, so we need to drop one.
   if (!M->isOperator() || M->isStatic()) {
      return true;
   }

   return UnorderedArgs.size() < M->getArgs().size();
}

CandidateSet::Candidate*
sema::resolveCandidateSet(SemaPass &Sema,
                          CandidateSet &CandSet,
                          Expression *SelfArg,
                          ArrayRef<Expression*> UnorderedArgs,
                          ArrayRef<IdentifierInfo*> Labels,
                          ArrayRef<Expression*> TemplateArgExprs,
                          SourceType RequiredType,
                          Statement *Caller,
                          bool DiagnoseFailure) {
   unsigned NumGivenArgs = UnorderedArgs.size();

   std::vector<StmtOrDecl> ArgExprs;
   std::vector<StmtOrDecl> BestMatchArgExprs;

   bool FoundAmbiguity = false;
   SmallVector<ConstraintSystem::Solution, 2> Solutions;
   ConstraintSystem::Solution BestSolution;
   CandidateSet::Candidate *BestCandidate = nullptr;

   SmallVector<Expression*, 4> ArgsWithSelf;
   ArgsWithSelf.reserve(NumGivenArgs + 1);
   ArgsWithSelf.push_back(SelfArg);
   ArgsWithSelf.append(UnorderedArgs.begin(), UnorderedArgs.end());

   for (auto *&argValue : ArgsWithSelf) {
      if (argValue == nullptr) {
         continue;
      }

      auto rebuiltExpr = ConstraintBuilder::rebuildExpression(Sema, argValue);
      if (!rebuiltExpr) {
         argValue->setIsInvalid(true);
         argValue->setExprType(Sema.ErrorTy);
      }
      else {
         argValue = rebuiltExpr.get();
      }
   }

   unsigned bestMatchIdx = 0;
   unsigned i = 0;

   for (auto &Cand : CandSet.Candidates) {
      if (Cand.isAnonymousCandidate()) {
         if (resolveAnonymousCandidate(Sema, Solutions, CandSet, Cand,
                                   RequiredType, UnorderedArgs, Labels,
                                   NumGivenArgs, BestSolution, BestCandidate,
                                   BestMatchArgExprs, FoundAmbiguity, Caller)) {
            bestMatchIdx = i;
         }
      }
      else {
         auto *Fn = Cand.getFunc();
         ArrayRef<Expression*> ArgValues = ArgsWithSelf;
         if (!shouldUseSelfArgument(Fn, SelfArg, UnorderedArgs)) {
            ArgValues = ArgValues.drop_front(1);
         }

         if (resolveCandidate(Sema, ArgExprs, Solutions, CandSet, Cand,
                             RequiredType, ArgValues, Labels, NumGivenArgs + 1,
                             BestSolution, BestCandidate, BestMatchArgExprs,
                             FoundAmbiguity, Caller)) {
            bestMatchIdx = i;
         }
      }

      ++i;
   }

   if (!BestSolution) {
      if (DiagnoseFailure) {
         CandSet.diagnose(Sema, SelfArg, UnorderedArgs, TemplateArgExprs,
                          Caller);
      }

      return nullptr;
   }

   CandSet.foundMatch(bestMatchIdx);

   if (FoundAmbiguity) {
      if (DiagnoseFailure) {
         CandSet.diagnoseAmbiguousCandidates(Sema, Caller);
      }

      CandSet.Status = CandidateSet::Ambiguous;
   }

   // Replace default argument values.
   replaceDefaultValues(Sema, CandSet, *BestCandidate, BestMatchArgExprs);

   // Apply the solution.
   BestCandidate->Builder->applySolution(BestSolution, CandSet.ResolvedArgs);

   // Apply implicit argument conversions.
   // Note: there can still be errors here if there were any template parameters
   // to resolve.
   if (applyConversions(Sema, CandSet, *BestCandidate, BestMatchArgExprs, Caller)) {
      return nullptr;
   }

   return BestCandidate;
}