#include "cdotc/Sema/OverloadResolver.h"

#include "cdotc/AST/Decl.h"
#include "cdotc/AST/TypeVisitor.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/ConstraintBuilder.h"
#include "cdotc/Sema/SemaPass.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::sema;
using namespace cdot::support;

using PathElement = ConstraintLocator::PathElement;
using DeclArgMapType = llvm::DenseMap<FuncArgDecl*, std::vector<Expression*>>;
using TemplateParamSet = llvm::SmallPtrSet<TemplateParamDecl*, 2>;

static bool checkImplicitLabel(IdentifierInfo* NeededLabel,
                               IdentifierInfo* GivenLabel, Expression* E)
{
   if (GivenLabel && GivenLabel->isStr("_")) {
      return true;
   }

   E = E->ignoreParensAndImplicitCasts();

   // Trailing closures don't need labels.
   if (auto *LE = dyn_cast<LambdaExpr>(E)) {
      return LE->isTrailingClosure();
   }

   if (auto* declRef = dyn_cast<DeclRefExpr>(E)) {
      auto Name = declRef->getDecl()->getDeclName();
      if (Name.isSimpleIdentifier() && Name.getIdentifierInfo() == NeededLabel) {
         return true;
      }
   }

   if (auto* memberRef = dyn_cast<MemberRefExpr>(E)) {
      if (!isa<SelfExpr>(memberRef->getParentExpr())) {
         return false;
      }

      auto Name = memberRef->getMemberDecl()->getDeclName();
      if (Name.isSimpleIdentifier() && Name.getIdentifierInfo() == NeededLabel) {
         return true;
      }
   }

   if (auto *Ident = dyn_cast<IdentifierRefExpr>(E)) {
      if ((!Ident->getParentExpr()
           || isa<SelfExpr>(Ident->getParentExpr()->ignoreParensAndImplicitCasts()))
          && Ident->getIdentInfo() == NeededLabel) {
         return true;
      }
   }

   return false;
}

static bool matchingLabels(SemaPass& Sema, CandidateSet& CandSet,
                           CandidateSet::Candidate& Cand,
                           ArrayRef<Expression*> UnorderedArgs,
                           ArrayRef<IdentifierInfo*> Labels,
                           DeclArgMapType& DeclArgMap)
{
   CallableDecl* CD = Cand.getFunc();
   ArrayRef<FuncArgDecl*> ArgDecls = CD->getArgs();
   FuncArgDecl* VariadicArgDecl = nullptr;

   bool isSubscript = isa<MethodDecl>(CD) && cast<MethodDecl>(CD)->isSubscript();
   bool isOperator = CD->isOperator();
   bool CStyleVararg = CD->isCstyleVararg();
   unsigned NumGivenArgs = UnorderedArgs.size();

   unsigned i = 0;
   unsigned LabelNo = 0;

   for (auto* ArgVal : UnorderedArgs) {
      // Check if this argument is labeled.
      IdentifierInfo* Label;
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
         for (auto* ArgDecl : ArgDecls) {
            if (ArgDecl->getLabel() == Label) {
               FoundLabel = true;
               DeclArgMap[ArgDecl].push_back(ArgVal);
               break;
            }

            // Allow specifying optional labels for clang-imported functions.
            if (ArgDecl->isImportedFromClang()
            && ArgDecl->getDeclName().getIdentifierInfo() == Label) {
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
      FuncArgDecl* ArgDecl;
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

      // Allow missing labels for operator & subscript calls.
      if (ArgDecl->getLabel() != nullptr) {
         if (!isSubscript && !isOperator
         && !checkImplicitLabel(ArgDecl->getLabel(), Label, ArgVal)) {
            Cand.setHasIncompatibleLabel(i, Label);
            return false;
         }
      }

      if (ArgDecl->isVariadic()) {
         VariadicArgDecl = ArgDecl;
      }

      DeclArgMap[ArgDecl].push_back(ArgVal);
      ++i;
   }

   return true;
}

static bool matchingAnonymousLabels(SemaPass& Sema, CandidateSet& CandSet,
                                    CandidateSet::Candidate& Cand,
                                    ArrayRef<Expression*> UnorderedArgs,
                                    ArrayRef<IdentifierInfo*> Labels,
                                    std::vector<StmtOrDecl>& ArgExprs)
{
   ArgExprs.resize(UnorderedArgs.size());

   auto* FnTy = Cand.getFunctionType();
   auto ParamInfo = FnTy->getParamInfo();
   unsigned NumGivenArgs = UnorderedArgs.size();

   unsigned i = 0;
   unsigned LabelNo = 0;

   for (auto* ArgVal : UnorderedArgs) {
      // Check if this argument is labeled.
      IdentifierInfo* Label;
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
         for (auto& PI : ParamInfo) {
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
      const FunctionType::ParamInfo* PI = nullptr;
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

class TemplateParamVisitor : public RecursiveTypeVisitor<TemplateParamVisitor> {
   /// Set of encountered variadic template parameters.
   TemplateParamSet& VariadicParams;

public:
   explicit TemplateParamVisitor(TemplateParamSet& VariadicParams)
       : VariadicParams(VariadicParams)
   {
   }

   bool visitTemplateParamType(TemplateParamType* T)
   {
      if (T->getParam()->isVariadic()) {
         VariadicParams.insert(T->getParam());
      }

      return false;
   }
};

class TemplateParamFinder : public RecursiveTypeVisitor<TemplateParamFinder> {
   /// Reference to the Sema instance.
   SemaPass& Sema;

   /// Reference to the constraint builder.
   ConstraintBuilder& Builder;

   /// The template argument list.
   MultiLevelTemplateArgList& TemplateArgs;

   /// Set of encountered variadic template parameters.
   TemplateParamSet& VariadicParams;

   /// Set to true if we were able to infer the variadic parameter.
   bool CouldInfer = false;

   /// Set to true if the type layout is incompatible.
   bool IncompatibleType = false;

public:
   /// Set to true if we encountered an error.
   bool EncounteredError = false;

private:
   ExprResult visitDefault(QualType T, Expression*& E)
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

   void visit(QualType T, Expression* E = nullptr)
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
#define CDOT_TYPE(NAME, Parent)                                                \
   case Type::NAME##ID:                                                        \
      visit##NAME(T->as##NAME());                                              \
      break;
#include "cdotc/AST/Types.def"
      }
   }

public:
   explicit TemplateParamFinder(SemaPass& Sema, ConstraintBuilder& Builder,
                                MultiLevelTemplateArgList& TemplateArgs,
                                TemplateParamSet& VariadicParams)
       : Sema(Sema), Builder(Builder), TemplateArgs(TemplateArgs),
         VariadicParams(VariadicParams)
   {
   }

   bool findTemplateParams(QualType ParamTy, Expression*& Expr)
   {
      assert(ParamTy->containsTemplateParamType());
      visit(ParamTy, Expr);

      if (!CouldInfer) {
         visitDefault(ParamTy, Expr);
      }

      return IncompatibleType;
   }

   bool visitTupleType(TupleType* T, Expression* Expr = nullptr)
   {
      auto* Tup = dyn_cast_or_null<TupleLiteral>(Expr);
      TemplateParamDecl* VariadicParam = nullptr;

      unsigned i = 0;
      unsigned VariadicIdx = -1;

      for (auto& El : T->getContainedTypes()) {
         auto* Param = El->asTemplateParamType();
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
      QualType NeededTy = Sema.Context.getTemplateParamType(VariadicParam);

      unsigned LeftoverElements = T->getArity() - VariadicIdx - 1;
      unsigned j = 0;

      for (i = VariadicIdx; i < Exprs.size() - LeftoverElements; ++i, ++j) {
         auto*& Element = Exprs[i];
         auto* Loc = Builder.makeLocator(
             Element, PathElement::contextualType(Expr->getSourceRange()));

         QualType Ty = Builder.visitExpr(Element, NeededTy, Loc);
         if (auto* TV = Ty->asTypeVariableType()) {
            Builder.Sys.setVariadicParamIdx(TV, j);

            if (Inferrable) {
               auto it = Builder.Bindings.ParamBindings.find(VariadicParam);
               if (it != Builder.Bindings.ParamBindings.end()) {
                  Builder.Sys.newConstraint<InferenceConstraint>(
                      it->getSecond(), TV, nullptr);
               }
            }
         }
      }

      return false;
   }

   bool visitFunctionType(FunctionType* T, Expression* Expr = nullptr)
   {
      return false;
   }
};

static unsigned getPreviousNumVariadicArgs(TemplateParamDecl* Param,
                                           DeclArgMapType& DeclArgMap)
{
   for (auto& ArgPair : DeclArgMap) {
      auto* ArgDecl = ArgPair.getFirst();
      if (!ArgDecl->isVariadic()) {
         continue;
      }

      if (ArgDecl->getType()->asTemplateParamType()->getParam() == Param) {
         return ArgPair.getSecond().size();
      }
   }

   return 1;
}

} // anonymous namespace

static CanType TransformImportedCType(SemaPass &Sema, CanType T)
{
   if (Sema.getCompilerInstance().getOptions().noPrelude()) {
      return T;
   }

   if (T->isIntegerType()) {
      int bits = T->getBitwidth();
      bool isUnsigned = T->isUnsigned();
      switch (bits) {
      case 1:
         return Sema.getBoolDecl()->getType();
      case 8:
         return (isUnsigned ? Sema.getUInt8Decl() : Sema.getInt8Decl())->getType();
      case 16:
         return (isUnsigned ? Sema.getUInt16Decl() : Sema.getInt16Decl())->getType();
      case 32:
         return (isUnsigned ? Sema.getUInt32Decl() : Sema.getInt32Decl())->getType();
      case 64:
         return (isUnsigned ? Sema.getUInt64Decl() : Sema.getInt64Decl())->getType();
      case 128:
         return (isUnsigned ? Sema.getUInt128Decl() : Sema.getInt128Decl())->getType();
      default:
         return T;
      }
   }

   if (T->isFloatTy()) {
      return Sema.getFloatDecl()->getType();
   }

   if (T->isDoubleTy()) {
      return Sema.getDoubleDecl()->getType();
   }

   if (T->isMutablePointerType()) {
      CanType Pointee = T->getPointeeType();
      if (Pointee->isVoidType()) {
         return Sema.getUnsafeMutableRawPtrDecl()->getType();
      }

      StructDecl *PtrDecl;
      if (Pointee->isInt8Ty()) {
         PtrDecl = Sema.getUnsafeMutableBufferPtrDecl();
      }
      else {
         PtrDecl = Sema.getUnsafeMutablePtrDecl();
      }

      TemplateArgument Arg(PtrDecl->getTemplateParams().front(),
                           TransformImportedCType(Sema, Pointee));

      auto *ArgList = FinalTemplateArgumentList::Create(Sema.Context, Arg);
      assert(!ArgList->isStillDependent() && "dependent imported C func?");

      auto *Inst = Sema.InstantiateRecord(SourceLocation(), PtrDecl, ArgList);
      assert(Inst && "failed UnsafePtr instantiation!");

      return Sema.Context.getRecordType(Inst);
   }

   if (T->isPointerType()) {
      CanType Pointee = T->getPointeeType();
      if (Pointee->isVoidType()) {
         return Sema.getUnsafeRawPtrDecl()->getType();
      }

      StructDecl *PtrDecl;
      if (Pointee->isInt8Ty()) {
         PtrDecl = Sema.getUnsafeBufferPtrDecl();
      }
      else {
         PtrDecl = Sema.getUnsafePtrDecl();
      }

      TemplateArgument Arg(PtrDecl->getTemplateParams().front(),
                           TransformImportedCType(Sema, Pointee));

      auto *ArgList = FinalTemplateArgumentList::Create(Sema.Context, Arg);
      assert(!ArgList->isStillDependent() && "dependent imported C func?");

      auto *Inst = Sema.InstantiateRecord(SourceLocation(), PtrDecl, ArgList);
      assert(Inst && "failed UnsafePtr instantiation!");

      return Sema.Context.getRecordType(Inst);
   }

   return T;
}

CanType SemaPass::TransformImportedCType(CanType T)
{
   if (compilerInstance->getOptions().noPrelude()) {
      return T;
   }

   auto It = ClangTypeMap.find(T);
   if (It != ClangTypeMap.end()) {
      return It->getSecond();
   }

   CanType Result = ::TransformImportedCType(*this, T);
   ClangTypeMap[T] = Result;

   return Result;
}

static bool createParamConstraints(
    ConstraintSystem& Sys, DeclArgMapType& DeclArgMap,
    CandidateSet::Candidate& Cand, FuncArgDecl* ArgDecl, unsigned NumGivenArgs,
    unsigned i, std::vector<StmtOrDecl>& ArgExprs,
    TemplateParamSet& VariadicParams, MultiLevelTemplateArgList& templateArgs,
    ConstraintBuilder& Builder, SemaPass& Sema, Statement* Caller,
    ConstraintBuilder* outerBuilder, unsigned &currentConversionPenalty,
    unsigned bestConversionPenalty)
{
   auto* CD = Cand.getFunc();

   // If no argument is given, check if there is a default one.
   auto It = DeclArgMap.find(ArgDecl);
   if (It == DeclArgMap.end()) {
      if (ArgDecl->isVariadic()) {
         auto* Param = ArgDecl->getType()->asTemplateParamType()->getParam();
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

      auto *DefaultVal = ArgDecl->getDefaultVal();
      if (DefaultVal->isMagicArgumentValue()
      || CD->isTemplate() || CD->isInitializerOfTemplate()
      || CD->isCaseOfTemplatedEnum()) {
         ArgExprs.emplace_back(ArgDecl);
      }
      else {
         ArgExprs.emplace_back(ArgDecl->getDefaultVal());
      }

      return false;
   }

   MutableArrayRef<Expression*> ArgValues = DeclArgMap[ArgDecl];

   // Cstyle varargs.
   if (ArgDecl == nullptr) {
      for (auto *Val : ArgValues) {
         auto Result = Builder.generateConstraints(Val, SourceType(), nullptr);
         switch (Result.Kind) {
         case ConstraintBuilder::Success:
            break;
         case ConstraintBuilder::InvalidArgument:
            Cand.setHasIncompatibleArgument(i, Result.Type,
                                            ArgDecl->getType());
            return true;
         case ConstraintBuilder::Failure:
            Cand.setIsInvalid();
            return true;
         case ConstraintBuilder::Dependent:
            Cand.setIsDependent();
            return false;
         }

         ArgExprs.emplace_back(Val);
      }
   }
   // If the parameter type is a pack expansion, we can use the supplied
   // values directly.
   else if (ArgDecl->isVariadic()) {
      auto* Param = ArgDecl->getType()->asTemplateParamType()->getParam();
      auto* Arg = templateArgs.getArgForParam(Param);

      if (!Arg->isFrozen() && Arg->getVariadicArgs().empty()) {
         bool Inferrable = VariadicParams.insert(Param).second;
         Builder.registerTemplateParam(Param);

         unsigned i = 0;
         for (auto* ArgVal : ArgValues) {
            auto* Loc = Builder.makeLocator(
                ArgVal, PathElement::parameterType(ArgDecl));

            auto Result
                = Builder.generateConstraints(ArgVal, ArgDecl->getType(), Loc);

            switch (Result.Kind) {
            case ConstraintBuilder::Success:
               break;
            case ConstraintBuilder::InvalidArgument:
               Cand.setHasIncompatibleArgument(i, Result.Type,
                                               ArgDecl->getType());
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

            TypeVariableType* TV = Ty->asTypeVariableType();
            if (!TV) {
               TV = Sys.newTypeVariable();
               Sys.newConstraint<TypeBindingConstraint>(TV, Ty, Loc);
            }

            Sys.setVariadicParamIdx(TV, i);

            if (Inferrable) {
               auto it = Builder.Bindings.ParamBindings.find(Param);
               if (it != Builder.Bindings.ParamBindings.end()) {
                  Sys.newConstraint<InferenceConstraint>(it->getSecond(), TV,
                                                         nullptr);
               }
            }

            ++i;
         }
      }
      else {
         auto& VAs = Arg->getVariadicArgs();
         if (VAs.size() > ArgValues.size()) {
            Cand.setHasTooFewArguments(NumGivenArgs, NumGivenArgs + VAs.size()
                                                         - ArgValues.size());

            return true;
         }
         else if (VAs.size() < ArgValues.size()) {
            Cand.setHasTooManyArguments(NumGivenArgs, NumGivenArgs - VAs.size()
                                                          + ArgValues.size());

            return true;
         }

         for (auto* ArgVal : ArgValues) {
            auto* Loc = Builder.makeLocator(
                ArgVal, PathElement::parameterType(ArgDecl));

            auto Result
                = Builder.generateConstraints(ArgVal, VAs[i].getType(), Loc);

            switch (Result.Kind) {
            case ConstraintBuilder::Success:
               break;
            case ConstraintBuilder::InvalidArgument:
               Cand.setHasIncompatibleArgument(i, Result.Type, VAs[i].getType());
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
   else if (ArgDecl->getType()->properties().containsUnexpandedParameterPack()) {
      TemplateParamFinder Finder(Sema, Builder, templateArgs, VariadicParams);

      for (auto*& ArgVal : ArgValues) {
         if (Finder.findTemplateParams(ArgDecl->getType(), ArgVal)) {
            Cand.setHasIncompatibleArgument(0, ArgVal->getExprType(),
                                            ArgDecl->getType());

            return true;
         }
         if (Finder.EncounteredError) {
            Cand.setIsInvalid();
            return true;
         }

         auto* Loc
             = Builder.makeLocator(ArgVal, PathElement::parameterType(ArgDecl));

         QualType NeededTy = ArgDecl->getType();
         Sema.QC.SubstTemplateParamTypesNonFinal(
             NeededTy, NeededTy, templateArgs, ArgVal->getSourceRange());

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
      auto* ArgVal = ArgValues.front();

      if (ArgDecl->getConvention() == ArgumentConvention::MutableRef
      && !ArgDecl->isSelf() && !Cand.getFunc()->isOperator()) {
         if (!isa<AddrOfExpr>(ArgVal->ignoreParensAndImplicitCasts())) {
            Cand.setHasIncompatibleArgument(i, QualType(), QualType());
            return true;
         }
      }

      QualType NeededTy = ArgDecl->getType();
      if (Cand.getFunc()->isTemplate()) {
         Sema.QC.SubstTemplateParamTypesNonFinal(
             NeededTy, NeededTy, templateArgs, ArgVal->getSourceRange());
      }

      if (ArgVal->isSemanticallyChecked() && !NeededTy->containsTemplateParamType()) {
         IsValidParameterValueQuery::result_type result;
         if (Sema.QC.IsValidParameterValue(result, ArgVal->getExprType(),
                                           NeededTy, ArgDecl->isSelf(),
                                           Cand.getFunc()->isImportedFromClang())) {
            return true;
         }

         if (!result.isValid) {
            Cand.setHasIncompatibleArgument(i, ArgVal->getExprType(), NeededTy);
            return true;
         }

         currentConversionPenalty += result.conversionPenalty;
         if (currentConversionPenalty > bestConversionPenalty) {
            Cand.ConversionPenalty = -1;
            return true;
         }

         ArgExprs.emplace_back(ArgVal);
         return false;
      }

      auto* Loc = Builder.makeLocator(ArgVal,
                                      PathElement::parameterType(ArgDecl));

      auto Result = Builder.generateArgumentConstraints(
          ArgVal, NeededTy, Loc, Cand.getFunc()->isImportedFromClang(),
          outerBuilder);

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

   return false;
}

static bool createAnonymousParamConstraints(
    ConstraintSystem& Sys, CandidateSet::Candidate& Cand, unsigned i,
    CanType NeededTy, ArgumentConvention Conv, Expression* ArgVal,
    ConstraintBuilder& Builder, SemaPass& Sema, Statement* Caller,
    unsigned &currentConversionPenalty, unsigned bestConversionPenalty)
{
   if (Cand.getFunctionType()->getParamInfo()[i].getConvention()
          == ArgumentConvention::MutableRef) {
      if (!isa<AddrOfExpr>(ArgVal->ignoreParensAndImplicitCasts())) {
         Cand.setHasIncompatibleArgument(i, QualType(), QualType());
         return true;
      }
   }

   if (ArgVal->isSemanticallyChecked()) {
      IsValidParameterValueQuery::result_type result;
      if (Sema.QC.IsValidParameterValue(result, ArgVal->getExprType(),
                                        NeededTy, false)) {
         return true;
      }

      if (!result.isValid) {
         Cand.setHasIncompatibleArgument(i, ArgVal->getExprType(), NeededTy);
         return true;
      }

      currentConversionPenalty += result.conversionPenalty;
      if (currentConversionPenalty > bestConversionPenalty) {
         Cand.ConversionPenalty = -1;
         return true;
      }

      return false;
   }

   auto* Loc = Builder.makeLocator(ArgVal, {});
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

   return false;
}

namespace {

class CaptureMarker : public RecursiveASTVisitor<CaptureMarker> {
   ASTContext& C;
   LambdaExpr* LE;

public:
   CaptureMarker(ASTContext& C, LambdaExpr* LE) : C(C), LE(LE) {}

   bool visitDeclRefExpr(DeclRefExpr *Expr)
   {
      auto *ND = Expr->getDecl();
      switch (ND->getKind()) {
      case Decl::LocalVarDeclID:
      case Decl::FuncArgDeclID: {
         auto *VD = cast<VarDecl>(ND);
         VD->setCaptured(true);
         Expr->setCaptureIndex(LE->addCapture(C, VD));

         break;
      }
      default:
         break;
      }

      return true;
   }

   bool visitSelfExpr(SelfExpr* E)
   {
      if (!E->isUppercase()) {
         E->setCaptureIndex(LE->addCapture(C, E->getSelfArg()));
      }

      return true;
   }

   bool visitSuperExpr(SuperExpr* E)
   {
      E->setCaptureIndex(LE->addCapture(C, E->getSelfArg()));
      return true;
   }
};

} // anonymous namespace

static bool applyConversions(SemaPass& SP, CandidateSet& CandSet,
                             CandidateSet::Candidate& Cand,
                             std::vector<StmtOrDecl>& ArgExprs,
                             Statement* Caller)
{
   bool isTemplate = false;
   ArrayRef<FuncArgDecl*> ArgDecls;
   SemaPass::ConversionOpts opts = SemaPass::CO_None;

   if (!Cand.isAnonymousCandidate()) {
      if (Cand.getFunc()->isInvalid()) {
         CandSet.InvalidCand = true;
         return true;
      }

      ArgDecls = Cand.getFunc()->getArgs();
      isTemplate = Cand.getFunc()->isTemplateOrInTemplate();

      if (Cand.getFunc()->isImportedFromClang()) {
         opts |= SemaPass::CO_IsClangParameterValue;
      }
   }

   unsigned i = 0;
   auto ParamTys = Cand.getFunctionType()->getParamTypes();
   bool cstyleVararg = Cand.getFunctionType()->isCStyleVararg();

   for (Expression*& E : CandSet.ResolvedArgs) {
      FuncArgDecl* ArgDecl = nullptr;
      QualType requiredType;

      if (i < ArgDecls.size()) {
         ArgDecl = ArgDecls[i];
      }
      if (i < ParamTys.size()) {
         requiredType = ParamTys[i];
      }

      ArgumentConvention Conv;
      if (ArgDecl) {
         Conv = ArgDecl->getConvention();
      }
      else if (Cand.isAnonymousCandidate()) {
         Conv = Cand.getFunctionType()->getParamInfo()[i].getConvention();
      }
      else {
         Conv = ArgumentConvention::Borrowed;
      }

      if (isTemplate && ArgDecl->isVariadic()) {
         requiredType = QualType();
      }
      else if (isTemplate) {
         if (SP.QC.SubstTemplateParamTypesNonFinal(requiredType, requiredType,
                                                   Cand.InnerTemplateArgs,
                                                   Caller->getSourceRange())) {
            ++i;
            continue;
         }

         if (Cand.OuterTemplateArgs) {
            if (SP.QC.SubstTemplateParamTypes(requiredType, requiredType,
                                              *Cand.OuterTemplateArgs,
                                              Caller->getSourceRange())) {
               ++i;
               continue;
            }
         }
      }

      // Make sure the expression type is resolved.
      auto Result = SP.typecheckExpr(E, requiredType, Caller, false);
      if (!Result) {
         ++i;
         continue;
      }

      E = Result.get();

      if (!ArgDecl || !ArgDecl->isSelf()) {
         switch (Conv) {
         case ArgumentConvention::Owned: {
            auto* noConv = E->ignoreParensAndImplicitCasts();
            QualType T = noConv->getExprType();

            // If the type is implicitly copyable, make a copy instead of moving.
            if (!SP.IsImplicitlyCopyableType(T->removeReference())) {
               // Mark this declaration as moved from
               if (auto Ident = dyn_cast<DeclRefExpr>(noConv)) {
                  auto ND = Ident->getDecl();
                  if (auto VD = dyn_cast<VarDecl>(ND)) {
                     VD->setMovedFrom(true);
                  }
               }
            }

            break;
         }
         default:
            break;
         }
      }

      // Convert to the parameter type and apply automatic promotion.
      if (i < ParamTys.size() && requiredType) {
         auto localOpts = opts;
         if (ArgDecl && ArgDecl->isSelf()) {
            localOpts |= SemaPass::CO_IsSelfValue;
         }

         E = SP.implicitCastIfNecessary(E, requiredType, false, localOpts);
      }
      else if (cstyleVararg) {
         E = SP.convertCStyleVarargParam(E);
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
            bool isBuiltinShortCircuitingOp = false;

            // Special case - builtin || and && operators
            if (auto *BA = Cand.getFunc()->getAttribute<_BuiltinAttr>()) {
               isBuiltinShortCircuitingOp = BA->getBuiltinName().startswith("Bool.infix");
            }

            if (!isBuiltinShortCircuitingOp) {
               auto LE = LambdaExpr::Create(
                   SP.getContext(), E->getSourceRange(), E->getSourceLoc(),
                   E->getExprType(), {},
                   ReturnStmt::Create(SP.getContext(), E->getSourceLoc(), E));

               (void)SP.visitExpr(E, LE);
               CaptureMarker(SP.getContext(), LE).visit(E);

               E = LE;
            }
            else {
               // We still need the correct type for typechecking, ILGen will
               // ignore this conversion expr.
               auto *LamdbaTy = SP.Context.getLambdaType(ArgDecl->getType(), {});
               auto Seq = ConversionSequence::Create(
                   SP.Context, CastStrength::Implicit,
                   ConversionStep(CastKind::NoOp, LamdbaTy));

               E = ImplicitCastExpr::Create(SP.Context, E, Seq);
            }
         }
      }
      else if (!requiredType || !requiredType->isReferenceType()) {
         E = SP.castToRValue(E);
      }

      ++i;
   }

   return false;
}

static bool replaceDefaultValues(SemaPass& SP, CandidateSet& CandSet,
                                 CandidateSet::Candidate& Cand,
                                 std::vector<StmtOrDecl>& ArgExprs,
                                 Statement *Caller)
{
   if (!Cand.isAnonymousCandidate() && Cand.getFunc()->isInvalid()) {
      CandSet.InvalidCand = true;
      return true;
   }

   for (auto& SOD : ArgExprs) {
      // Check if this is an argument with a default value.
      if (auto* Decl = SOD.asDecl()) {
         // Get the instantiated default value.
         auto* Inst = Cand.getFunc()->lookupSingle<FuncArgDecl>(
             cast<FuncArgDecl>(Decl)->getDeclName());

         if (SP.QC.TypecheckDecl(Decl)) {
            CandSet.InvalidCand = true;
            continue;
         }

         if (Inst != Decl) {
            if (SP.QC.TypecheckDecl(Inst)) {
               CandSet.InvalidCand = true;
               CandSet.ResolvedArgs.push_back(
                   cast<FuncArgDecl>(Decl)->getDefaultVal());

               continue;
            }
         }

         auto DefaultVal = Inst->getDefaultVal();
         if (DefaultVal->isMagicArgumentValue()) {
            auto Alias = cast<IdentifierRefExpr>(DefaultVal)->getAlias();
            auto Result
                = SP.HandleReflectionAlias(Alias, cast<Expression>(Caller));

            assert(!isa<IdentifierRefExpr>(Result.getValue()));
            SOD = Result.getValue();
         }
         else {
            SOD = Inst->getDefaultVal();
         }
      }

      CandSet.ResolvedArgs.push_back(cast<Expression>(SOD.getStatement()));
   }

   return false;
}

static bool checkReturnType(SemaPass& Sema, CandidateSet::Candidate& Cand,
                            CanType RequiredType)
{
   QualType ReturnType;
   if (Cand.isAnonymousCandidate()) {
      ReturnType = Cand.getFunctionType()->getReturnType();
   }
   else if (Cand.getFunc()->isCompleteInitializer()) {
      ReturnType = Sema.Context.getRecordType(Cand.getFunc()->getRecord());
   }
   else if (Cand.getFunc()->isImportedFromClang()) {
      ReturnType = Sema.TransformImportedCType(
          Cand.getFunctionType()->getReturnType());
   }
   else {
      ReturnType = Cand.getFunctionType()->getReturnType();
   }

   IsImplicitlyConvertibleQuery::result_type result;
   if (Sema.QC.IsImplicitlyConvertible(result, ReturnType, RequiredType)) {
      return false;
   }

   if (!result.implicitlyConvertible) {
      // An incompatible return type does not mean that this candidate can't be
      // a valid overload choice, it just means that it loses priority over
      // overloads with the correct return type.
      Cand.setHasIncompatibleReturnType(RequiredType, ReturnType);
      return true;
   }

   return false;
}

static void
bindTemplateParams(SemaPass& Sema, CandidateSet::Candidate& Cand,
                   SmallVectorImpl<ConstraintSystem::Solution>& Solutions,
                   MultiLevelTemplateArgList& templateArgList)
{
   for (auto& B : Cand.Builder->Bindings.ParamBindings) {
      QualType ParamTy = Sema.Context.getTemplateParamType(B.getFirst());
      QualType AssignedTy = Solutions.front().AssignmentMap[B.getSecond()];

      if (!AssignedTy) {
         continue;
      }

      if (B.getFirst()->isVariadic()) {
         auto* Tup = AssignedTy->asTupleType();
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

static bool isBetterCandidate(CandidateSet::Candidate* BestCand,
                              CandidateSet::Candidate& NewCand,
                              ConstraintSystem::Solution& BestSolution,
                              ConstraintSystem::Solution& NewSolution)
{
   if (!BestCand) {
      return true;
   }

   if (BestCand->FR == CandidateSet::IncompatibleReturnType
       && NewCand.FR == CandidateSet::None) {
      return true;
   }

   return NewSolution.Score < BestSolution.Score;
}

static bool resolveCandidate(
    SemaPass& Sema, std::vector<StmtOrDecl>& ArgExprs,
    SmallVectorImpl<ConstraintSystem::Solution>& Solutions,
    CandidateSet& CandSet, CandidateSet::Candidate& Cand,
    const SourceType& RequiredType, ArrayRef<Expression*> UnorderedArgs,
    ArrayRef<IdentifierInfo*> Labels, unsigned NumGivenArgs,
    ConstraintSystem::Solution& BestSolution,
    CandidateSet::Candidate*& BestCandidate,
    std::vector<StmtOrDecl>& BestMatchArgExprs, bool& FoundAmbiguity,
    Statement* Caller, bool isFunctionArgument, ConstraintBuilder* outerBuilder)
{
   if (Sema.QC.PrepareDeclInterface(Cand.getFunc())
       || Cand.getFunc()->isInvalid()) {
      Cand.setIsInvalid();
      CandSet.InvalidCand = true;
      return false;
   }

   bool isTemplateInitializer =
       (Cand.getFunc()->isInitializerOfTemplate()
        || Cand.getFunc()->isCaseOfTemplatedEnum());

   MultiLevelTemplateArgList templateArgList;
   TemplateArgList outerTemplateArgs;

   if (Cand.getFunc()->isTemplate()) {
      if (Cand.InnerTemplateArgs.empty()) {
         Cand.InnerTemplateArgs = TemplateArgList(Sema, Cand.getFunc());
      }

      templateArgList.addOuterList(Cand.InnerTemplateArgs);
   }

   if (isTemplateInitializer) {
      if (Cand.OuterTemplateArgs) {
         outerTemplateArgs = TemplateArgList(Sema, *Cand.OuterTemplateArgs);
      }
      else {
         outerTemplateArgs = TemplateArgList(Sema, Cand.getFunc()->getRecord());
      }

      templateArgList.addOuterList(outerTemplateArgs);
   }

   DeclArgMapType DeclArgMap;
   ArgExprs.clear();
   Solutions.clear();

   // Check if the returned type is viable.
   QualType ReturnType = Cand.getFunctionType()->getReturnType();
   bool DependentReturnType = ReturnType->containsTemplateParamType();

   if (RequiredType && !DependentReturnType) {
      Cand.ValidReturnType = !checkReturnType(Sema, Cand, RequiredType);
   }

   // Check if the parameter amounts and labels match up.
   if (!matchingLabels(Sema, CandSet, Cand, UnorderedArgs, Labels,
                       DeclArgMap)) {
      return false;
   }

   // Create a new constraint system for this overload.
   Cand.Builder
       = std::make_unique<ConstraintBuilder>(Sema.QC, Caller->getSourceRange());
   ConstraintSystem& Sys = Cand.Builder->Sys;

   // Generate the constraints for each parameter.
   bool Valid = true;
   TemplateParamSet VariadicParams;

   // Register template parameters for the function itself.
   if (Cand.getFunc()->isTemplate()) {
      for (auto* Param : Cand.getFunc()->getTemplateParams()) {
         if (!Param->isVariadic()
             && Cand.InnerTemplateArgs.getArgForParam(Param)->isNull()) {
            Cand.Builder->registerTemplateParam(
                Param, dyn_cast<Expression>(Caller));
         }
      }
   }

   // Register template parameters for the initialized record or enum.
   if (isTemplateInitializer) {
      for (auto* Param : Cand.getFunc()->getRecord()->getTemplateParams()) {
         if (!Param->isVariadic()
             && outerTemplateArgs.getArgForParam(Param)->isNull()) {
            Cand.Builder->registerTemplateParam(
                Param, dyn_cast<Expression>(Caller));
         }
      }
   }

   unsigned paramConversionPenalty = 0;
   unsigned bestConversionPenalty = BestCandidate
       ? BestCandidate->ConversionPenalty : -1;

   unsigned i = 0;
   for (auto* ArgDecl : Cand.getFunc()->getArgs()) {
      if (createParamConstraints(Sys, DeclArgMap, Cand, ArgDecl, NumGivenArgs,
                                 i++, ArgExprs, VariadicParams, templateArgList,
                                 *Cand.Builder, Sema, Caller, outerBuilder,
                                 paramConversionPenalty, bestConversionPenalty)) {
         Valid = false;
         break;
      }
   }

   // Add C-Style varargs.
   if (Cand.getFunc()->isCstyleVararg()) {
      if (DeclArgMap.find(nullptr) != DeclArgMap.end()) {
         if (createParamConstraints(Sys, DeclArgMap, Cand, nullptr, NumGivenArgs,
                                    i++, ArgExprs, VariadicParams, templateArgList,
                                    *Cand.Builder, Sema, Caller, outerBuilder,
                                    paramConversionPenalty, bestConversionPenalty)) {
            Valid = false;
         }
      }
   }

   if (!Valid) {
      CandSet.InvalidCand |= Cand.FR == CandidateSet::IsInvalid;
      return false;
   }

   // Check dependent return types.
   if (RequiredType && DependentReturnType) {
      if (!RequiredType->containsTemplateParamType() || !isFunctionArgument) {
         // Only use the return type for inference if it doesn't appear in the
         // parameter types.
         Cand.Builder->addTemplateParamBinding(ReturnType, RequiredType);
      }
   }

   // Solve the constraint system.
   auto SolveResult = Sys.solve(Solutions);
   switch (SolveResult) {
   case ConstraintSystem::Success:
      break;
   case ConstraintSystem::Failure:
      Sys.solve(Solutions, true);
      Sys.diagnoseCandidateFailure(Cand, Cand.Builder->Bindings);

      if (Cand.FR == CandidateSet::None) {
         Cand.setIsInvalid();
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
         Sema.diagnose(err_ambiguous_solution, Caller->getSourceRange());
      }
   }

   // Apply template parameter bindings.
   bool isTemplate = Cand.getFunc()->isTemplate();
   if (isTemplate || isTemplateInitializer) {
      bindTemplateParams(Sema, Cand, Solutions, templateArgList);
   }

   // Instantiate the template record first.
   if (isTemplateInitializer) {
      if (!Sema.maybeInstantiateRecord(Cand, templateArgList.outermost(),
                                       Caller)) {
         Cand.setIsInvalid();
         CandSet.InvalidCand = true;

         return false;
      }
   }

   // Now instantiate template functions or methods.
   Sema.maybeInstantiate(Cand, Caller);

   // Return now if there was a failed constraint.
   if (Cand.FR == CandidateSet::FailedConstraint) {
      return false;
   }

   // Check the return type again in case it was dependent.
   if (RequiredType && DependentReturnType && !isFunctionArgument) {
      Cand.ValidReturnType |= !checkReturnType(Sema, Cand, RequiredType);
   }

   if (!Cand.ValidReturnType && BestSolution) {
      return false;
   }

   // Reset the failure reason in case it was set because of an incompatible
   // return type.
   Cand.FR = CandidateSet::None;

   // Check for ambiguity.
   auto& S = Solutions.front();

   /// Calculate the conversion penalty.
   uint64_t convPenalty = Sys.calculateConversionPenalty(S);
   S.Score += paramConversionPenalty;
   S.Score += convPenalty;
   S.Score += (int)isTemplate;

   if (isBetterCandidate(BestCandidate, Cand, BestSolution, S)) {
      CandSet.BestConversionPenalty = S.Score;
      BestCandidate = &Cand;
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
         BestSolution = move(S);
         BestMatchArgExprs = move(ArgExprs);
         CandSet.BestConversionPenalty = S.Score;

         return true;
      }
   }

   return false;
}

static bool resolveAnonymousCandidate(
    SemaPass& Sema, SmallVectorImpl<ConstraintSystem::Solution>& Solutions,
    CandidateSet& CandSet, CandidateSet::Candidate& Cand,
    const SourceType& RequiredType, ArrayRef<Expression*> UnorderedArgs,
    ArrayRef<IdentifierInfo*> Labels, unsigned NumGivenArgs,
    ConstraintSystem::Solution& BestSolution,
    CandidateSet::Candidate*& BestCandidate,
    std::vector<StmtOrDecl>& BestMatchArgExprs, bool& FoundAmbiguity,
    Statement* Caller)
{
   auto* FnTy = Cand.getFunctionType();
   std::vector<StmtOrDecl> ArgExprs;

   // Check if the returned type is viable.
   if (RequiredType) {
      checkReturnType(Sema, Cand, RequiredType);
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
   Cand.Builder
       = std::make_unique<ConstraintBuilder>(Sema.QC, Caller->getSourceRange());
   ConstraintSystem& Sys = Cand.Builder->Sys;

   // Generate the constraints for each parameter.
   bool Valid = true;
   TemplateParamSet VariadicParams;

   unsigned paramConversionPenalty = 0;
   unsigned bestConversionPenalty = BestCandidate
       ? BestCandidate->ConversionPenalty : -1;

   unsigned i = 0;
   for (auto& ParamTy : Params) {
      auto* ArgVal = cast<Expression>(ArgExprs[i].getStatement());
      if (createAnonymousParamConstraints(Sys, Cand, i, ParamTy,
                                          ParamInfo[i].getConvention(), ArgVal,
                                          *Cand.Builder, Sema, Caller,
                                          paramConversionPenalty,
                                          bestConversionPenalty)) {
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
      Sys.diagnoseCandidateFailure(Cand, Cand.Builder->Bindings);

      if (Cand.FR == CandidateSet::None) {
         Cand.setIsInvalid();
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
         Sema.diagnose(err_ambiguous_solution, Caller->getSourceRange());
      }
   }

   // Check for ambiguity.
   auto& S = Solutions.front();
   S.Score += Cand.ConversionPenalty;

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

static bool shouldUseSelfArgument(CallableDecl* Fn, Expression* SelfArg,
                                  ArrayRef<Expression*> UnorderedArgs)
{
   if (SelfArg && !SelfArg->getExprType()->isMetaType()) {
      // Always include non-metatype self arguments.
      return true;
   }

   auto* M = dyn_cast<MethodDecl>(Fn);
   if (M) {
      if (M->isCompleteInitializer()) {
         return false;
      }
      if (UnorderedArgs.size() >= Fn->getArgs().size()) {
         assert(SelfArg->getExprType()->isMetaType());
         return false;
      }

      return true;
   }

   return Fn->isOperator() && SelfArg && UnorderedArgs.size() < Fn->getArgs().size();
}

CandidateSet::Candidate* sema::resolveCandidateSet(
    SemaPass& Sema, CandidateSet& CandSet, Expression* SelfArg,
    ArrayRef<Expression*> UnorderedArgs, ArrayRef<IdentifierInfo*> Labels,
    ArrayRef<Expression*> TemplateArgExprs, SourceType RequiredType,
    Statement* Caller, bool DiagnoseFailure, bool isFunctionArgument,
    ConstraintBuilder* outerBuilder)
{
   unsigned NumGivenArgs = UnorderedArgs.size();

   std::vector<StmtOrDecl> ArgExprs;
   std::vector<StmtOrDecl> BestMatchArgExprs;

   bool FoundAmbiguity = false;
   SmallVector<ConstraintSystem::Solution, 2> Solutions;
   ConstraintSystem::Solution BestSolution;
   CandidateSet::Candidate* BestCandidate = nullptr;

   SmallVector<Expression*, 4> ArgsWithSelf;
   ArgsWithSelf.reserve(NumGivenArgs + 1);
   ArgsWithSelf.push_back(SelfArg);
   ArgsWithSelf.append(UnorderedArgs.begin(), UnorderedArgs.end());

   auto *baseExpr = dyn_cast<Expression>(Caller);
   for (auto*& argValue : ArgsWithSelf) {
      if (argValue == nullptr) {
         continue;
      }

      auto rebuiltExpr = ConstraintBuilder::rebuildExpression(Sema, argValue,
                                                              baseExpr, true);

      if (!rebuiltExpr.first) {
         argValue->setIsInvalid(true);
         argValue->setExprType(Sema.ErrorTy);
      }
      else {
         argValue = rebuiltExpr.first.get();
      }

      if (!rebuiltExpr.second) {
         auto Result = Sema.visitExpr(argValue);
         if (!Result) {
            argValue->setIsInvalid(true);
            argValue->setExprType(Sema.ErrorTy);
         }
         else {
            argValue = Result.get();
         }
      }
   }

   bool foundTemplate = false;
   unsigned bestMatchIdx = 0;
   unsigned i = 0;

   for (auto& Cand : CandSet.Candidates) {
      if (Cand.isAnonymousCandidate()) {
         if (resolveAnonymousCandidate(
                 Sema, Solutions, CandSet, Cand, RequiredType, UnorderedArgs,
                 Labels, NumGivenArgs, BestSolution, BestCandidate,
                 BestMatchArgExprs, FoundAmbiguity, Caller)) {
            bestMatchIdx = i;
         }
      }
      else if (Cand.getFunc()->isTemplate()) {
         foundTemplate = true;
      }
      else {
         auto* Fn = Cand.getFunc();
         ArrayRef<Expression*> ArgValues = ArgsWithSelf;
         if (!shouldUseSelfArgument(Fn, SelfArg, UnorderedArgs)) {
            ArgValues = ArgValues.drop_front(1);
         }

         if (resolveCandidate(Sema, ArgExprs, Solutions, CandSet, Cand,
                              RequiredType, ArgValues, Labels, NumGivenArgs + 1,
                              BestSolution, BestCandidate, BestMatchArgExprs,
                              FoundAmbiguity, Caller, isFunctionArgument,
                              outerBuilder)) {
            bestMatchIdx = i;
         }
      }

      ++i;
   }

   // Non-template functions are always preferred over templates.
   if ((!BestSolution || !BestCandidate->ValidReturnType || BestSolution.Score > 0)
   && foundTemplate) {
      i = 0;
      for (auto& Cand : CandSet.Candidates) {
         if (Cand.isAnonymousCandidate() || !Cand.getFunc()->isTemplate()) {
            ++i;
            continue;
         }

         auto* Fn = Cand.getFunc();
         ArrayRef<Expression*> ArgValues = ArgsWithSelf;
         if (!shouldUseSelfArgument(Fn, SelfArg, UnorderedArgs)) {
            ArgValues = ArgValues.drop_front(1);
         }

         if (resolveCandidate(Sema, ArgExprs, Solutions, CandSet, Cand,
                              RequiredType, ArgValues, Labels, NumGivenArgs + 1,
                              BestSolution, BestCandidate, BestMatchArgExprs,
                              FoundAmbiguity, Caller, isFunctionArgument,
                              outerBuilder)) {
            bestMatchIdx = i;
         }

         ++i;
      }
   }

   if (!BestSolution) {
      if (DiagnoseFailure) {
         CandSet.diagnose(Sema, SelfArg, UnorderedArgs, TemplateArgExprs,
                          Caller);
      }

      return nullptr;
   }

   // Might still have an incompatible return type.
   BestCandidate->FR = CandidateSet::None;
   CandSet.foundMatch(bestMatchIdx);

   if (FoundAmbiguity) {
      if (DiagnoseFailure) {
         CandSet.diagnoseAmbiguousCandidates(Sema, Caller);
      }

      CandSet.Status = CandidateSet::Ambiguous;
   }

   // Replace default argument values.
   replaceDefaultValues(Sema, CandSet, *BestCandidate, BestMatchArgExprs,
                        Caller);

   // Apply the solution.
   BestCandidate->Builder->applySolution(BestSolution, CandSet.ResolvedArgs);

   // Apply implicit argument conversions.
   // Note: there can still be errors here if there were any template parameters
   // left to resolve.
   if (applyConversions(Sema, CandSet, *BestCandidate, BestMatchArgExprs,
                        Caller)) {
      return nullptr;
   }

   return BestCandidate;
}

void sema::markCaptures(SemaPass &Sema, LambdaExpr *Expr)
{
   CaptureMarker(Sema.Context, Expr).visit(Expr);
}