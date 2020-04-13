////
//// Created by Jonas Zell on 08.11.17.
////
//
//#include "OverloadResolver.h"
//
//#include "ConstraintBuilder.h"
//#include "cdotc/Query/QueryContext.h"
//#include "cdotc/Sema/SemaPass.h"
//
// using namespace cdot::support;
// using namespace cdot::sema;
//
// namespace cdot {
// namespace ast {
//
// using ConvSeqVec = SmallVectorImpl<ConversionSequenceBuilder>;
//
// OverloadResolver::OverloadResolver(SemaPass &SP,
//                                   DeclarationName FuncName,
//                                   Expression *SelfArg,
//                                   ArrayRef<Expression*> givenArgs,
//                                   ArrayRef<Expression*> givenTemplateArgs,
//                                   ArrayRef<IdentifierInfo*> givenLabels,
//                                   Statement *Caller)
//   : SP(SP), FuncName(FuncName), SelfArg(SelfArg), givenArgs(givenArgs),
//     givenTemplateArgs(givenTemplateArgs), givenLabels(givenLabels),
//     Caller(Caller)
//{
//
//}
//
// static unsigned castPenalty(const ConversionSequenceBuilder &neededCast)
//{
//   unsigned penalty = 0;
//
//   // only implicit casts should occur here
//   for (const auto &C : neededCast.getSteps()) {
//      switch (C.getKind()) {
//      case CastKind::NoOp:
//      case CastKind::Move:
//      case CastKind::Forward:
//      case CastKind::LValueToRValue:
//      case CastKind::MutRefToRef:
//      case CastKind::RValueToConstRef:
//      case CastKind::ToMetaType:
//      case CastKind::NoThrowToThrows:
//      case CastKind::ThinToThick:
//      case CastKind::MetaTypeCast:
//         break;
//      case CastKind::Ext:
//      case CastKind::FPExt:
//      case CastKind::SignFlip:
//      case CastKind::EnumToInt:
//      case CastKind::IntToEnum:
//      case CastKind::Copy:
//      case CastKind::MutPtrToPtr:
//      case CastKind::IsNull:
//      case CastKind::BitCast:
//         penalty += 1;
//         break;
//      case CastKind::ExistentialInit:
//      case CastKind::ExistentialCast:
//         penalty += 2;
//         break;
//      case CastKind::UpCast:
//         penalty += 2;
//         break;
//      case CastKind::ConversionOp:
//         penalty += 3;
//         break;
//      default:
//         llvm_unreachable("bad implicit cast kind!");
//      }
//   }
//
//   return penalty;
//}
//
// static bool isMovable(Expression *Expr)
//{
//   auto *Ident = dyn_cast<IdentifierRefExpr>(Expr);
//   if (!Ident)
//      return true;
//
//   switch (Ident->getKind()) {
//   case IdentifierKind::Field: {
//      // Fields of classes or global variables are not movable.
//      auto *Parent = Ident->getParentExpr();
//      if (Parent->getExprType()->isClass())
//         return false;
//
//      auto *IE = dyn_cast<IdentifierRefExpr>(Parent);
//      return IE && IE->getKind() == IdentifierKind::GlobalVar;
//   }
//   case IdentifierKind::GlobalVar:
//      return false;
//   default:
//      return true;
//   }
//}
//
// static bool createsNewValue(ConversionSequenceBuilder &Seq)
//{
//   for (auto &S : Seq.getSteps()) {
//      switch (S.getKind()) {
//      case CastKind::ConversionOp:
//      case CastKind::ExistentialInit:
//      case CastKind::ExistentialCast:
//      case CastKind::ExistentialCastFallible:
//      case CastKind::ExistentialUnwrapFallible:
//      case CastKind::ToEmptyTuple:
//         return true;
//      default:
//         break;
//      }
//   }
//
//   return false;
//}
//
// static bool getConversionPenalty(SemaPass &SP, Expression *Expr,
//                                 CanType NeededTy, CanType GivenTy,
//                                 FunctionType::ParamInfo NeededParamInfo,
//                                 CandidateSet::Candidate &Cand,
//                                 CandidateSet &CandSet,
//                                 ConvSeqVec &Conversions,
//                                 bool IsSelf, unsigned ArgNo) {
//   QualType NeededNoSugar = NeededTy->removeReference()->getDesugaredType();
//   QualType GivenNoSugar = GivenTy->removeReference()->getDesugaredType();
//
//   if (GivenNoSugar->isErrorType() || NeededNoSugar->isErrorType()) {
//      return false;
//   }
//
//   if (NeededNoSugar->isUnknownAnyType()) {
//      // Conversion operators have 'UnknownAny' as their right-hand side type.
//      assert((SP.isInDependentContext() || Cand.isAnonymousCandidate())
//             && "argument of UnknownAny type in non-dependent context!");
//
//      if (!Cand.isAnonymousCandidate()) {
//         Cand.setIsDependent();
//         CandSet.Dependent = true;
//      }
//
//      return false;
//   }
//
//   if (GivenNoSugar->isDependentType() || NeededNoSugar->isDependentType()) {
//      Cand.setIsDependent();
//      CandSet.Dependent = true;
//
//      return false;
//   }
//
//   ConversionSequenceBuilder ConvSeq;
//   bool AddCopy = false;
//   bool AddForward = false;
//
//   // Allow implicit conversion from mutable reference to mutable
//   // borrow for self argument.
//   if (IsSelf
//       && Cand.getFunc()->isNonStaticMethod()
//       && NeededTy->isMutableBorrowType()
//       && GivenTy->isMutableReferenceType()) {
//      GivenTy = SP.getContext().getMutableReferenceType(
//         GivenTy->removeReference())->getCanonicalType();
//
//      ConvSeq.addStep(CastKind::BitCast, GivenTy);
//   }
//
//   // check parameter passing convention
//   switch (NeededParamInfo.getConvention()) {
//   case ArgumentConvention::Default:
//      llvm_unreachable("didn't remove default convention!");
//   case ArgumentConvention::Borrowed:
//      if (GivenTy->isMutableBorrowType()) {
//         ++Cand.ConversionPenalty;
//      }
//
//      // if we're given a mutable reference, we need to pass an
//      // immutable one
//      if (GivenTy->isMutableReferenceType()) {
//         ConvSeq.addStep(CastKind::MutRefToRef,
//                         SP.getContext().getReferenceType(
//                            GivenTy->getReferencedType()));
//      }
//
//      break;
//   case ArgumentConvention::ImmutableRef: {
//      if (GivenTy->isMutableBorrowType()) {
//         Cand.setRequiresRef(ArgNo);
//         return true;
//      }
//
//      if (GivenTy->isMutableReferenceType()) {
//         ++Cand.ConversionPenalty;
//         ConvSeq.addStep(CastKind::MutRefToRef,
//                         SP.getContext().getReferenceType(
//                            GivenTy->getReferencedType()));
//      }
//      else if (!GivenTy->isReferenceType()) {
//         Cand.setRequiresRef(ArgNo);
//         return true;
//      }
//
//      if (!NeededTy->isReferenceType())
//         NeededTy = SP.getContext().getReferenceType(NeededTy)
//            ->getCanonicalType();
//
//      break;
//   }
//   case ArgumentConvention::MutableRef: {
//      // the left hand side of an assignment does not need to be
//      // explicitly borrowed
//      if (Cand.isAssignmentOperator() && ArgNo == 0) {
//         if (!NeededTy->isMutableReferenceType()) {
//            NeededTy = SP.getContext()
//                         .getMutableReferenceType(NeededTy->removeReference())
//                         ->getCanonicalType();
//         }
//      }
//      else if (!NeededTy->isMutableBorrowType()) {
//         NeededTy = SP.getContext()
//                      .getMutableReferenceType(NeededTy->removeReference())
//                      ->getCanonicalType();
//      }
//      else if (!GivenTy->isMutableBorrowType()) {
//         if (IsSelf && GivenTy->isReferenceType()) {
//            Cand.setMutatingOnConstSelf();
//            return true;
//         }
//         if (IsSelf) {
//            Cand.setMutatingOnRValueSelf();
//            return true;
//         }
//
//         Cand.setRequiresRef(ArgNo);
//         return true;
//      }
//
//      break;
//   }
//   case ArgumentConvention::Owned: {
//      // Requires passing ownership: for a temporary, this is a noop
//      // since we can forward, but for an lvalue it's a move.
//
//      // if the type is implicitly copyable, pass a copy instead
//      if (GivenTy->isReferenceType()) {
//         QualType Deref = GivenTy->getReferencedType();
//         if (Deref->isRefcounted()) {
//            AddCopy = true;
//            break;
//         }
//
//         if (SP.getContext().getTargetInfo().isTriviallyCopyable(Deref)) {
//            AddCopy = true;
//            break;
//         }
//
//         if (GivenTy->isMutableReferenceType() && isMovable(Expr)) {
//            // Prefer moving a mutable reference where possible.
//            AddForward = true;
//            break;
//         }
//
//         bool ImplicitlyCopyable;
//         if (SP.QC.IsImplicitlyCopyable(ImplicitlyCopyable, Deref)) {
//            break;
//         }
//
//         if (ImplicitlyCopyable) {
//            AddCopy = true;
//            break;
//         }
//
//         // can't pass a non-mutable reference to an owned parameter
//         Cand.setRequiresRef(ArgNo);
//         return true;
//      }
//      // if it's an rvalue, forward it
//      else {
//         AddForward = true;
//      }
//
//      break;
//   }
//   }
//
//   SP.getConversionSequence(ConvSeq, GivenTy, NeededTy);
//
//   if (ConvSeq.isDependent()) {
//      Cand.setIsDependent();
//      CandSet.Dependent = true;
//
//      return false;
//   }
//
//   if (!ConvSeq.isImplicit()) {
//      if (IsSelf) {
//         Cand.setHasIncompatibleSelfArgument(NeededTy, GivenTy);
//      }
//      else {
//         Cand.setHasIncompatibleArgument(ArgNo, GivenTy, NeededTy);
//      }
//
//      return true;
//   }
//
//   if (AddForward) {
//      ConvSeq.addStep(CastKind::Forward, ConvSeq.getSteps().back()
//                                                .getResultType());
//   }
//   else if (AddCopy && !createsNewValue(ConvSeq)) {
//      ConvSeq.addStep(CastKind::Copy, ConvSeq.getSteps().back()
//                                             .getResultType());
//   }
//
//   Cand.ConversionPenalty += castPenalty(ConvSeq);
//   Conversions.emplace_back(move(ConvSeq));
//
//   return false;
//}
//
// static bool checkImplicitLabel(IdentifierInfo *NeededLabel,
//                               IdentifierInfo *GivenLabel,
//                               Expression *E) {
//   if (GivenLabel && GivenLabel->isStr("_")) {
//      return true;
//   }
//
//   // Trailing closures don't need labels.
//   // FIXME actually check for trailing closures
//   if (isa<LambdaExpr>(E)) {
//      return true;
//   }
//
//   if (auto *Ident =
//   dyn_cast<IdentifierRefExpr>(E->ignoreParensAndImplicitCasts())) {
//      if ((!Ident->getParentExpr()
//      ||
//      isa<SelfExpr>(Ident->getParentExpr()->ignoreParensAndImplicitCasts()))
//      && Ident->getIdentInfo() == NeededLabel) {
//         return true;
//      }
//   }
//
//   return false;
//}
//
// static bool isVariadic(QualType Ty)
//{
//   return Ty->isTemplateParamType() &&
//   Ty->asTemplateParamType()->isVariadic();
//}
//
// static bool resolveContextualArgument(SemaPass &SP,
//                                      unsigned i,
//                                      FuncArgDecl *ArgDecl,
//                                      Expression *ArgVal,
//                                      MultiLevelTemplateArgList *TemplateArgs,
//                                      CandidateSet &CandSet,
//                                      CandidateSet::Candidate &Cand,
//                                      ConvSeqVec &Conversions,
//                                      SmallVectorImpl<StmtOrDecl> &ArgExprs) {
//   QualType NeededTy = ArgDecl->getType();
//   SourceLocation ArgLoc = ArgVal->getSourceLoc();
//
//   if (TemplateArgs && NeededTy->isDependentType()) {
//      if (SP.QC.SubstTemplateParamTypesNonFinal(NeededTy, NeededTy,
//                                          *TemplateArgs, ArgLoc)) {
//         return true;
//      }
//   }
//
//   // Handle closures specially.
//   if (auto LE = dyn_cast<LambdaExpr>(ArgVal)) {
//      auto LambdaTy = SP.ResolveContextualLambdaExpr(LE, NeededTy);
//      if (!LambdaTy) {
//         if (LE->isInvalid()) {
//            Cand.setIsInvalid();
//            CandSet.InvalidCand = true;
//         }
//         else if (auto Def = SP.GetDefaultExprType(LE)) {
//            Cand.setHasIncompatibleArgument(i, Def, NeededTy);
//         }
//         else {
//            Cand.setCouldNotInferArgumentType(i);
//         }
//
//         return true;
//      }
//
//      if (TemplateArgs) {
//         TemplateArgs->inferFromType(LambdaTy, NeededTy, false);
//      }
//
//      Conversions.emplace_back(
//         ConversionSequenceBuilder::MakeNoop(NeededTy));
//
//      ArgExprs.emplace_back(ArgVal);
//      return false;
//   }
//
//   // Check if the expression would be able to return a value of
//   // that type.
//   int CanReturn = SP.ExprCanReturn(ArgVal, NeededTy);
//   if (CanReturn == -1) {
//      if (ArgVal->isInvalid()) {
//         Cand.setIsInvalid();
//         CandSet.InvalidCand = true;
//      }
//      else {
//         if (auto Def = SP.GetDefaultExprType(ArgVal)) {
//            Cand.setHasIncompatibleArgument(i, Def, NeededTy);
//         }
//         else {
//            Cand.setCouldNotInferArgumentType(i);
//         }
//      }
//
//      return false;
//   }
//
//   QualType ArgValType;
//   if (NeededTy->containsTemplateParamType()) {
//      // We can't infer a context dependent expression from a
//      // dependent type.
//      auto DefaultType = SP.GetDefaultExprType(ArgVal);
//      if (DefaultType) {
//         if (TemplateArgs && NeededTy->containsTemplateParamType()) {
//            if (!TemplateArgs->inferFromType(DefaultType,
//                                             NeededTy, false)) {
//               Cand.setHasIncompatibleArgument(i, DefaultType,
//                                               NeededTy);
//
//               return false;
//            }
//
//            if (isVariadic(NeededTy)) {
//               NeededTy = DefaultType;
//            }
//            else if (SP.QC.SubstTemplateParamTypesNonFinal(NeededTy,
//                                                     NeededTy,
//                                                     *TemplateArgs,
//                                                     ArgLoc)) {
//               return true;
//            }
//         }
//
//         ArgValType = DefaultType;
//      }
//      else if (NeededTy->containsTemplateParamType()) {
//         ArgValType = NeededTy;
//      }
//      else {
//         if (ArgVal->isInvalid()) {
//            Cand.setIsInvalid();
//            CandSet.InvalidCand = true;
//            return true;
//         }
//
//         Cand.setCouldNotInferArgumentType(i);
//         return true;
//      }
//
//      if (NeededTy->isDependentType()) {
//         Cand.setIsDependent();
//         return true;
//      }
//
//      if (getConversionPenalty(SP, ArgVal, NeededTy, ArgValType,
//                               {ArgDecl->getConvention()},
//                               Cand, CandSet, Conversions,
//                               ArgDecl->isSelf(), i)) {
//         return true;
//      }
//   }
//   else {
//      Cand.ConversionPenalty += CanReturn;
//      Conversions.emplace_back(
//         ConversionSequenceBuilder::MakeNoop(NeededTy));
//   }
//
//   ArgExprs.push_back(ArgVal);
//   return false;
//}
//
// LLVM_ATTRIBUTE_UNUSED
// static bool resolveSingleArgument(SemaPass &SP,
//                                  unsigned i,
//                                  FuncArgDecl *ArgDecl,
//                                  Expression *ArgVal,
//                                  MultiLevelTemplateArgList *TemplateArgs,
//                                  CandidateSet &CandSet,
//                                  CandidateSet::Candidate &Cand,
//                                  ConvSeqVec &Conversions,
//                                  SmallVectorImpl<StmtOrDecl> &ArgExprs) {
//   // If the argument's type is context depentent, we need to some extra
//   // checking.
//   if (ArgVal->isContextDependent()) {
//      return resolveContextualArgument(SP, i, ArgDecl, ArgVal, TemplateArgs,
//                                       CandSet, Cand, Conversions, ArgExprs);
//   }
//
//   QualType NeededTy = ArgDecl->getType();
//   SourceLocation ArgLoc = ArgVal->getSourceLoc();
//
//   if (TemplateArgs && NeededTy->isDependentType()) {
//      if (SP.QC.SubstTemplateParamTypesNonFinal(NeededTy, NeededTy,
//                                          *TemplateArgs, ArgLoc)) {
//         return true;
//      }
//   }
//
//   auto ArgValType = ArgVal->getExprType();
//   if (TemplateArgs && NeededTy->containsTemplateParamType()) {
//      if (!TemplateArgs->inferFromType(ArgValType, NeededTy,
//                                       false)) {
//
//         Cand.setHasIncompatibleArgument(i, ArgValType, NeededTy);
//         return true;
//      }
//
//      if (isVariadic(NeededTy)) {
//         // Never infer reference types for template arguments.
//         NeededTy = ArgValType->removeReference();
//      }
//      else if (SP.QC.SubstTemplateParamTypesNonFinal(NeededTy, NeededTy,
//                                               *TemplateArgs,
//                                               ArgLoc)) {
//         return true;
//      }
//   }
//
//   if (NeededTy->isDependentType()) {
//      if (ArgValType->isDependentType() || NeededTy->containsAssociatedType())
//      {
//         Cand.setIsDependent();
//         return true;
//      }
//
//      Cand.setHasIncompatibleArgument(i, ArgValType, NeededTy);
//      return true;
//   }
//
//   if (getConversionPenalty(SP, ArgVal, NeededTy, ArgValType,
//                            {ArgDecl->getConvention()},
//                            Cand, CandSet, Conversions,
//                            ArgDecl->isSelf(), i)) {
//      return true;
//   }
//
//   ArgExprs.emplace_back(ArgVal);
//   return false;
//}
//
// static bool resolveContextDependentArgs(SemaPass &SP,
//                                        Statement *Caller,
//                                        CallableDecl *CD,
//                                        Expression *SelfVal,
//                                        ArrayRef<IdentifierInfo*> labels,
//                                        ArrayRef<Expression*> UnorderedArgs,
//                                        SmallVectorImpl<StmtOrDecl> &ArgExprs,
//                                        CandidateSet::Candidate &Cand,
//                                        CandidateSet &CandSet,
//                                        ConvSeqVec &Conversions,
//                                        MultiLevelTemplateArgList
//                                            *TemplateArgs = nullptr) {
//   // Assign arguments based on labels.
//   llvm::DenseMap<FuncArgDecl*, std::vector<Expression*>> DeclArgMap;
//
//   ArrayRef<FuncArgDecl*> ArgDecls = CD->getArgs();
//   FuncArgDecl *VariadicArgDecl = nullptr;
//
//   bool CStyleVararg = CD->isCstyleVararg();
//   unsigned NumGivenArgs = (unsigned)UnorderedArgs.size();
//
//   unsigned i = 0;
//   unsigned LabelNo = 0;
//
//   for (auto *ArgVal : UnorderedArgs) {
//      // Check if this argument is labeled.
//      IdentifierInfo *Label;
//      if (SelfVal && i == 0 && !CD->isCompleteInitializer()) {
//         Label = nullptr;
//      }
//      else if (LabelNo < labels.size()) {
//         Label = labels[LabelNo++];
//      }
//      else {
//         Label = nullptr;
//      }
//
//      // If so, look for an argument with that label.
//      if (Label && !Label->isStr("_")) {
//         bool FoundLabel = false;
//         for (auto *ArgDecl : ArgDecls) {
//            if (ArgDecl->getLabel() == Label
//            || ArgDecl->isVariadicArgPackExpansion()) {
//               FoundLabel = true;
//               DeclArgMap[ArgDecl].push_back(ArgVal);
//               break;
//            }
//         }
//
//         if (!FoundLabel) {
//            Cand.setHasIncompatibleLabel(i, Label);
//            return false;
//         }
//
//         ++i;
//         continue;
//      }
//
//      // Otherwise, if the argument is labeled, bail out.
//      FuncArgDecl *ArgDecl;
//      if (!VariadicArgDecl && i < ArgDecls.size()) {
//         ArgDecl = ArgDecls[i];
//      }
//      else {
//         ArgDecl = VariadicArgDecl;
//      }
//
//      if (!ArgDecl) {
//         if (CStyleVararg) {
//            DeclArgMap[nullptr].push_back(ArgVal);
//            ++i;
//            continue;
//         }
//
//         Cand.setHasTooManyArguments(NumGivenArgs, ArgDecls.size());
//         return false;
//      }
//
//      // Allow missing labels for operator calls.
//      if (ArgDecl->getLabel() != nullptr) {
//         if (!checkImplicitLabel(ArgDecl->getLabel(), Label, ArgVal)) {
//            Cand.setHasIncompatibleLabel(i, Label);
//            return false;
//         }
//      }
//
//      if (ArgDecl->isVariadicArgPackExpansion()) {
//         VariadicArgDecl = ArgDecl;
//      }
//
//      DeclArgMap[ArgDecl].push_back(ArgVal);
//      ++i;
//   }
//
////   SmallVector<ConstraintSystem::Solution, 4> Solutions;
////   ConstraintSystem::SolutionBindings Bindings;
////
////   ConstraintSystem Sys(SP.QC);
////   ConstraintBuilder Builder(Sys, Bindings);
////
////   for (auto *Param : CD->getTemplateParams()) {
////      Builder.registerTemplateParam(Param);
////   }
////
////   // Now resolve types of context dependent arguments.
////   i = 0;
////   for (FuncArgDecl *ArgDecl : ArgDecls) {
////      // If no argument is given, check if there is a default one.
////      auto It = DeclArgMap.find(ArgDecl);
////      if (It == DeclArgMap.end()) {
////         if (ArgDecl->isVariadicArgPackExpansion()) {
////            ++i;
////            continue;
////         }
////         if (!ArgDecl->getValue()) {
////            Cand.setHasTooFewArguments(NumGivenArgs, CD->getArgs().size());
////            return false;
////         }
////
////         if (SP.QC.TypecheckDecl(ArgDecl)) {
////            Cand.setIsInvalid();
////            return false;
////         }
////
////         auto DefaultVal = ArgDecl->getDefaultVal();
////         if (DefaultVal->isMagicArgumentValue()) {
////            auto Alias = cast<IdentifierRefExpr>(DefaultVal)->getAlias();
////            auto Result = SP.HandleReflectionAlias(Alias,
//// cast<Expression>(Caller));
////
////            if (Result) {
////               ArgExprs.emplace_back(Result.getValue());
////            }
////            else {
////               ArgExprs.emplace_back(DefaultVal);
////            }
////         }
////         else if (CD->isTemplate() || CD->isInitializerOfTemplate()
////                  || CD->isCaseOfTemplatedEnum()) {
////            ArgExprs.emplace_back(ArgDecl);
////         }
////         else {
////            ArgExprs.emplace_back(DefaultVal);
////         }
////
////         Conversions.emplace_back(
////            ConversionSequenceBuilder::MakeNoop(DefaultVal->getExprType()));
////
////         ++i;
////         continue;
////      }
////
////      for (Expression *ArgVal : DeclArgMap[ArgDecl]) {
////         QualType NeededTy = ArgDecl->getType();
////
////         auto GenRes = Builder.generateConstraints(ArgVal,
///SourceType(NeededTy)); /         switch (GenRes) { /         case
///ConstraintBuilder::Dependent: /            Cand.setIsDependent(); /
///return true; /         case ConstraintBuilder::Failure: /
///Cand.setIsInvalid(); /            return true; /         case
///ConstraintBuilder::Success: /            break; /         }
////
////         ArgExprs.push_back(ArgVal);
////      }
////
////      ++i;
////   }
////
////   auto Res = Sys.solve(Solutions);
////   switch (Res) {
////   case ConstraintSystem::Dependent:
////      Cand.setIsDependent();
////      return true;
////   case ConstraintSystem::Error:
////      Cand.setIsInvalid();
////      CandSet.InvalidCand = true;
////      return true;
////   case ConstraintSystem::Failure:
////      Cand.setIsInvalid();
////      CandSet.InvalidCand = true;
////      SP.diagnose(diag::err_generic_error, "call does not typecheck",
////                  Caller->getSourceRange());
////
////      return true;
////   case ConstraintSystem::Success:
////      break;
////   }
////
////   unsigned BestScore = -1;
////   ConstraintSystem::Solution *BestSolution = nullptr;
////
////   for (auto &S : Solutions) {
////      if (S.Score < BestScore) {
////         BestScore = S.Score;
////         BestSolution = &S;
////      }
////      else if (S.Score == BestScore) {
////         // FIXME
////         SP.diagnose(diag::note_generic_note, "ambiguous assignment",
////                     Caller->getSourceRange());
////      }
////   }
////
////   assert(BestSolution && "no solution!");
////
////   // Check the template parameter bindings.
////   if (TemplateArgs) {
////      for (auto &B : Bindings.ParamBindings) {
////         QualType ParamTy = SP.Context.getTemplateArgType(B.getFirst());
////         QualType AssignedTy = BestSolution->AssignmentMap[B.getSecond()];
////
////         TemplateArgs->inferFromType(AssignedTy, ParamTy);
////      }
////   }
////
//////   for (auto &ExprAssignment : Bindings.ExprBindings) {
//////      Cand.TypeAssignmentMap[ExprAssignment.getFirst()]
//////         = BestSolution->AssignmentMap[ExprAssignment.getSecond()];
//////   }
////
////   auto VariadicArgIt = DeclArgMap.find(nullptr);
////   if (VariadicArgIt != DeclArgMap.end()) {
////      for (auto *E : VariadicArgIt->getSecond()) {
////         // Apply standard c-style vararg conversion.
////         ConversionSequenceBuilder ConvSeq;
////         if (E->isLValue()) {
////            ConvSeq.addStep(CastKind::LValueToRValue,
////                            E->getExprType()->removeReference());
////         }
////         else {
////            ConvSeq.addStep(CastKind::NoOp, QualType());
////         }
////
////         Conversions.emplace_back(move(ConvSeq));
////         ArgExprs.emplace_back(E);
////      }
////   }
//
//   return true;
//}
//
// static bool resolveContextDependentArgs(SemaPass &SP,
//                                        Statement *Caller,
//                                        Expression *SelfVal,
//                                        ArrayRef<Expression*> UnorderedArgs,
//                                        ArrayRef<IdentifierInfo*> labels,
//                                        SmallVectorImpl<StmtOrDecl> &ArgExprs,
//                                        CandidateSet::Candidate &Cand,
//                                        CandidateSet &CandSet,
//                                        ConvSeqVec &Conversions,
//                                        MultiLevelTemplateArgList*TemplateArgs){
//   if (!Cand.isAnonymousCandidate()) {
//      return resolveContextDependentArgs(SP, Caller, Cand.getFunc(), SelfVal,
//      labels,
//                                         UnorderedArgs, ArgExprs, Cand,
//                                         CandSet, Conversions, TemplateArgs);
//   }
//
//   auto *FuncTy = Cand.getFunctionType();
//   auto neededArgs = FuncTy->getParamTypes();
//   auto NeededParamInfo = FuncTy->getParamInfo();
//
//   unsigned i = 0;
//   for (auto *ArgVal : UnorderedArgs) {
//      if (i >= neededArgs.size()) {
//         Cand.setHasTooManyArguments(UnorderedArgs.size(), neededArgs.size());
//         return false;
//      }
//
//      QualType NeededTy = neededArgs[i];
//      if (TemplateArgs && NeededTy->isDependentType()) {
//         if (SP.QC.SubstTemplateParamTypesNonFinal(NeededTy, NeededTy,
//                                             *TemplateArgs,
//                                             ArgVal->getSourceLoc())) {
//            continue;
//         }
//      }
//
//      if (ArgVal->isContextDependent()) {
//         if (auto LE = dyn_cast<LambdaExpr>(ArgVal)) {
//            auto LambdaTy = SP.ResolveContextualLambdaExpr(LE, NeededTy);
//            if (!LambdaTy) {
//               if (LE->isInvalid()) {
//                  Cand.setIsInvalid();
//                  CandSet.InvalidCand = true;
//               }
//               else if (auto Def = SP.GetDefaultExprType(LE)) {
//                  Cand.setHasIncompatibleArgument(i, Def, NeededTy);
//               }
//               else {
//                  Cand.setCouldNotInferArgumentType(i);
//               }
//
//               return false;
//            }
//
//            ++i;
//            continue;
//         }
//
//         int CanReturn = SP.ExprCanReturn(ArgVal, NeededTy);
//         if (CanReturn == -1) {
//            if (ArgVal->isInvalid()) {
//               Cand.setIsInvalid();
//               CandSet.InvalidCand = true;
//            }
//            else {
//               if (auto Def = SP.GetDefaultExprType(ArgVal)) {
//                  Cand.setHasIncompatibleArgument(i, Def, NeededTy);
//               }
//               else {
//                  Cand.setCouldNotInferArgumentType(i);
//               }
//            }
//
//            return false;
//         }
//
//         QualType ArgValType;
//         if (NeededTy->isDependentType()) {
//            // we can't infer a context dependent expression from a
//            // dependent type
//            auto DefaultType = SP.GetDefaultExprType(ArgVal);
//            if (DefaultType) {
//               ArgValType = DefaultType;
//            }
//            else if (NeededTy->isDependentType()) {
//               ArgValType = NeededTy;
//            }
//            else {
//               if (ArgVal->isInvalid()) {
//                  Cand.setIsInvalid();
//                  CandSet.InvalidCand = true;
//                  return false;
//               }
//
//               Cand.setCouldNotInferArgumentType(i);
//               return false;
//            }
//
//            if (getConversionPenalty(SP, ArgVal, NeededTy, ArgValType,
//                                     NeededParamInfo[i],
//                                     Cand, CandSet, Conversions,
//                                     false, i)) {
//               return false;
//            }
//         }
//         else {
//            Cand.ConversionPenalty += CanReturn;
//         }
//      }
//      else {
//         if (getConversionPenalty(SP, ArgVal, NeededTy, ArgVal->getExprType(),
//                                  NeededParamInfo[i],
//                                  Cand, CandSet, Conversions,
//                                  false, i)) {
//            return false;
//         }
//      }
//
//      ++i;
//   }
//
//   for (auto *Arg : UnorderedArgs)
//      ArgExprs.emplace_back(Arg);
//
//   return true;
//}
//
// namespace {
//
// class CaptureMarker: public RecursiveASTVisitor<CaptureMarker> {
//   ASTContext &C;
//   LambdaExpr *LE;
//
// public:
//   CaptureMarker(ASTContext &C, LambdaExpr *LE) : C(C), LE(LE)
//   {}
//
//   bool visitIdentifierRefExpr(IdentifierRefExpr *Expr)
//   {
//      switch (Expr->getKind()) {
//      case IdentifierKind::LocalVar:
//      case IdentifierKind::FunctionArg: {
//         Expr->getVarDecl()->setCaptured(true);
//         Expr->setCaptureIndex(LE->addCapture(C, Expr->getVarDecl()));
//         Expr->setIsCapture(true);
//
//         break;
//      }
//      default:
//         break;
//      }
//
//      return true;
//   }
//
//   bool visitSelfExpr(SelfExpr *E)
//   {
//      if (!E->isUppercase()) {
//         E->setCaptureIndex(LE->addCapture(C, E->getSelfArg()));
//      }
//
//      return true;
//   }
//
//   bool visitSuperExpr(SuperExpr *E)
//   {
//      E->setCaptureIndex(LE->addCapture(C, E->getSelfArg()));
//      return true;
//   }
//};
//
//} // anonymous namespace
//
// static bool applyConversions(SemaPass &SP,
//                             CandidateSet &CandSet,
//                             SmallVectorImpl<StmtOrDecl> &ArgExprs,
//                             SmallVectorImpl<ConversionSequenceBuilder>
//                                &Conversions,
//                             Statement *Caller) {
//   auto &Cand = CandSet.getBestMatch();
//   ArrayRef<FuncArgDecl*> ArgDecls;
//   if (!Cand.isAnonymousCandidate()) {
//      auto *Func = Cand.getFunc();
//      if (Func->isInvalid()) {
//         Cand.setIsInvalid();
//         CandSet.Status = CandidateSet::NoMatch;
//         CandSet.InvalidCand = true;
//
//         return true;
//      }
//
//      ArgDecls = Func->getArgs();
//   }
//
////   for (auto &Assignment : Cand.TypeAssignmentMap) {
////      Assignment.getFirst()->setContextualType(Assignment.getSecond());
////   }
//
//   auto ParamTys = Cand.getFunctionType()->getParamTypes();
//   unsigned i = 0;
//   for (auto &SOD : ArgExprs) {
//      if (auto *Decl = SOD.asDecl()) {
//         // Get the instantiated default value.
//         auto *Inst = Cand.getFunc()->lookupSingle<FuncArgDecl>(
//            cast<FuncArgDecl>(Decl)->getDeclName());
//
//         if (Inst != Decl) {
//            if (SP.QC.TypecheckDecl(Inst)) {
//               ++i;
//               CandSet.InvalidCand = true;
//               CandSet.ResolvedArgs.push_back(
//                  cast<FuncArgDecl>(Decl)->getDefaultVal());
//
//               continue;
//            }
//         }
//
//         SOD = Inst->getDefaultVal();
//      }
//
//      auto *E = cast<Expression>(SOD.getStatement());
//
//      FuncArgDecl *ArgDecl = nullptr;
//      if (i < ArgDecls.size()) {
//         ArgDecl = ArgDecls[i];
//      }
//
//      auto Result = SP.visitExpr(Caller, E);
//      if (!Result) {
//         ++i;
//         CandSet.ResolvedArgs.push_back(E);
//         continue;
//      }
//
//      E = Result.get();
//
//      if (E->getExprType()->isVoidType()) {
//         SP.diagnose(E, diag::err_vararg_cannot_pass_void,
//         E->getSourceRange());
//      }
//      else if (ArgDecl &&ArgDecl->getConvention() ==
//      ArgumentConvention::Owned){
//         // mark this declaration as moved from
//         if (auto Ident = dyn_cast<IdentifierRefExpr>(E)) {
//            auto ND = Ident->getNamedDecl();
//            if (auto VD = dyn_cast_or_null<VarDecl>(ND)) {
//               VD->setMovedFrom(true);
//            }
//         }
//      }
//
//      if (i < ParamTys.size()) {
//         E = SP.implicitCastIfNecessary(E, ParamTys[i]);
//      }
//
//      // Handle @autoclosure
//      if (ArgDecl && ArgDecl->hasAttribute<AutoClosureAttr>()) {
//         auto LE = LambdaExpr::Create(
//            SP.getContext(), E->getSourceRange(),
//            E->getSourceLoc(), SourceType(SP.getContext().getAutoType()),
//            {}, ReturnStmt::Create(SP.getContext(), E->getSourceLoc(), E));
//
//         (void) SP.visitExpr(E, LE);
//         CaptureMarker(SP.getContext(), LE).visit(E);
//
//         E = LE;
//      }
//
//      SOD = E;
//      CandSet.ResolvedArgs.push_back(E);
//
//      ++i;
//   }
//
//   return false;
//}
//
// void OverloadResolver::resolve(CandidateSet &CandSet)
//{
//   SmallVector<ConversionSequenceBuilder, 4> BestMatchConversions;
//   SmallVector<StmtOrDecl, 4> BestMatchArgExprs;
//
//   bool foundMatch       = false;
//   bool MatchIsDependent = false;
//   bool Dependent        = false;
//   bool ambiguous        = false;
//
//   unsigned BestMatch      = 0;
//   unsigned BestMatchDistance = 0;
//   unsigned MaxConstraints = 0;
//   unsigned NumCandidates  = (unsigned)CandSet.Candidates.size();
//
//   SmallVector<Expression*, 4> ArgVec;
//   ArgVec.push_back(SelfArg);
//   ArgVec.append(givenArgs.begin(), givenArgs.end());
//
//   ArrayRef<Expression*> ArgRef = ArgVec;
//   for (unsigned i = 0; i < NumCandidates; ++i) {
//      CandidateSet::Candidate &Cand = CandSet.Candidates[i];
//
//      if (!Cand.isAnonymousCandidate()
//      && SP.QC.PrepareDeclInterface(Cand.getFunc())) {
//         Cand.setIsInvalid();
//         CandSet.InvalidCand = true;
//         continue;
//      }
//
//      assert(Cand.isAnonymousCandidate() || Cand.getFunc()->isTemplate()
//             || Cand.getFunc()->getFunctionType()
//                && "function without function type");
//
//      // Drop self argument for self.init() calls.
//      SmallVector<ConversionSequenceBuilder, 4> Conversions;
//      SmallVector<StmtOrDecl, 4> ArgExprs;
//
//      if (Cand.isAnonymousCandidate()
//      || Cand.getFunc()->isCompleteInitializer()
//      || !SelfArg) {
//         resolve(CandSet, Cand, ArgRef.drop_front(1), Conversions, ArgExprs);
//      }
//      else {
//         resolve(CandSet, Cand, ArgRef, Conversions, ArgExprs);
//      }
//
//#     ifndef NDEBUG
//      if (Cand.FR == CandidateSet::IsInvalid)
//         assert(CandSet.InvalidCand
//                && "didn't update InvalidCand flag on CandidateSet!");
//#     endif
//
//      bool IsDependent = Cand.FR == CandidateSet::IsDependent;
//      Dependent |= IsDependent;
//
//      if (Cand || IsDependent) {
//         bool IsBetterMatch =
//            (!foundMatch || BestMatchDistance >= Cand.Distance)
//            && CandSet.BestConversionPenalty >= Cand.ConversionPenalty
//            && Cand.getNumConstraints() >= MaxConstraints;
//
//         if (foundMatch
//                && BestMatchDistance == Cand.Distance
//                && CandSet.BestConversionPenalty == Cand.ConversionPenalty
//                && Cand.getNumConstraints() == MaxConstraints) {
//            // dependent candidates might not actually be valid at
//            // instantiation time, so don't report an error
//            if (Cand.FR != CandidateSet::IsDependent && !MatchIsDependent) {
//               ambiguous = true;
//            }
//         }
//
//         if (IsDependent) {
//            MatchIsDependent = true;
//            Cand.FR = CandidateSet::None;
//         }
//         else if (IsBetterMatch) {
//            BestMatch = i;
//            BestMatchDistance = Cand.Distance;
//            BestMatchConversions = move(Conversions);
//            BestMatchArgExprs = move(ArgExprs);
//            MaxConstraints = Cand.getNumConstraints();
//
//            CandSet.maybeUpdateBestConversionPenalty(Cand.ConversionPenalty);
//         }
//
//         foundMatch = true;
//      }
//   }
//
//   if (Dependent) {
//      CandSet.Dependent = true;
//      return;
//   }
//
//   if (ambiguous) {
//      CandSet.Status = CandidateSet::Ambiguous;
//   }
//   else if (foundMatch) {
//      CandSet.Status = CandidateSet::Success;
//      CandSet.MatchIdx = BestMatch;
//
//      SP.maybeInstantiate(CandSet, Caller);
//
//      if (applyConversions(SP, CandSet, BestMatchArgExprs,
//      BestMatchConversions,
//                           Caller)) {
//         return;
//      }
//   }
//}
//
// LLVM_ATTRIBUTE_UNUSED
// static bool hasDependentSignature(CallableDecl *C)
//{
//   for (auto &Arg : C->getArgs())
//      if (Arg->isDependent())
//         return true;
//
//   if (auto E = C->getReturnType().getTypeExpr())
//      return E->isDependent();
//
//   return false;
//}
//
// void OverloadResolver::resolve(CandidateSet &CandSet,
//                               CandidateSet::Candidate &Cand,
//                               llvm::ArrayRef<Expression*> givenArgs,
//                               ConvSeqVec &Conversions,
//                               ArgVec &ArgExprs) {
//   FunctionType *FuncTy = Cand.getFunctionType();
//   SmallVector<QualType, 8> resolvedGivenArgs;
//
//   // FIXME runtime-generics
//   bool IsTemplate = !Cand.isAnonymousCandidate()
//                     && (Cand.getFunc()->isTemplate()
//                         || Cand.getFunc()->isInitializerOfTemplate()
//                         || Cand.getFunc()->isCaseOfTemplatedEnum());
//
//   if (!IsTemplate) {
//      if (!givenTemplateArgs.empty()) {
//         return Cand.setHasTooManyTemplateArgs(givenTemplateArgs.size(), 0);
//      }
//
//      resolveContextDependentArgs(SP, Caller, SelfArg, givenArgs,
//                                  givenLabels, ArgExprs, Cand,
//                                  CandSet, Conversions, nullptr);
//
//      return;
//   }
//
//   // Penalize templates over non-templates.
//   ++Cand.ConversionPenalty;
//
//   SourceLocation listLoc = givenTemplateArgs.empty()
//                            ? Caller->getSourceLoc()
//                            : givenTemplateArgs.front()->getSourceLoc();
//
//   bool NeedOuterTemplateParams
//      = (isa<InitDecl>(Cand.getFunc()) || isa<EnumCaseDecl>(Cand.getFunc()))
//        && Cand.getFunc()->getRecord()->isTemplate();
//
//   Cand.InnerTemplateArgs = TemplateArgList(
//      SP, Cand.getFunc(),
//      // if we have an initializer or enum case, the given template
//      // arguments will be passed to the record parameter list
//      NeedOuterTemplateParams ? llvm::ArrayRef<Expression*>()
//                              : givenTemplateArgs,
//      listLoc);
//
//   TemplateArgList OuterTemplateArgs;
//   MultiLevelTemplateArgList TemplateArgs(Cand.InnerTemplateArgs);
//
//   // initializers and enum cases also need their containing records
//   // template arguments specified (or inferred)
//   if (NeedOuterTemplateParams) {
//      OuterTemplateArgs = TemplateArgList(SP, Cand.getFunc()->getRecord(),
//                                          givenTemplateArgs, listLoc);
//
//      TemplateArgs.addOuterList(OuterTemplateArgs);
//   }
//
//   // Try to infer unspecified template arguments from the function
//   // signature.
//   if (Caller && Caller->getContextualType()) {
//      TemplateArgs.inferFromType(Caller->getContextualType(),
//                                 FuncTy->getReturnType());
//   }
//
//   if (!resolveContextDependentArgs(SP, Caller, SelfArg, givenArgs,
//                                    givenLabels, ArgExprs, Cand, CandSet,
//                                    Conversions, &TemplateArgs)) {
//      // Already diagnosed an error.
//      if (Cand.FR != CandidateSet::None) {
//         return;
//      }
//
//      if (NeedOuterTemplateParams) {
//         auto OuterComp = OuterTemplateArgs.checkCompatibility();
//         if (!OuterComp) {
//            // This is kind of a hack, but the template argument list will
//            // only be used for diagnostics after this.
//            Cand.InnerTemplateArgs = move(OuterTemplateArgs);
//            Cand.setTemplateArgListFailure(OuterComp);
//
//            return;
//         }
//      }
//
//      auto comp = Cand.InnerTemplateArgs.checkCompatibility();
//      if (!comp) {
//         return Cand.setTemplateArgListFailure(comp);
//      }
//
//      return;
//   }
//
//   if (Cand.FR == CandidateSet::IsDependent) {
//      return;
//   }
//
//   // If the template argument list is itself dependent, we have to delay
//   // the overload resolution until instantiation time.
//   if (TemplateArgs.isStillDependent()) {
//      CandSet.Dependent = true;
//      return Cand.setIsDependent();
//   }
//
//   if (NeedOuterTemplateParams) {
//      auto comp = OuterTemplateArgs.checkCompatibility();
//      if (!comp) {
//         Cand.InnerTemplateArgs = move(OuterTemplateArgs);
//         return Cand.setTemplateArgListFailure(comp);
//      }
//   }
//
//   auto comp = Cand.InnerTemplateArgs.checkCompatibility();
//   if (!comp) {
//      return Cand.setTemplateArgListFailure(comp);
//   }
//
//   // Instantiate the record if this is a template initializer.
//   if (NeedOuterTemplateParams) {
//      if (!SP.maybeInstantiateRecord(Cand, OuterTemplateArgs, Caller)) {
//         Cand.setIsInvalid();
//         CandSet.InvalidCand = true;
//
//         return;
//      }
//   }
//
//   // Check the constraints here to take the resolved
//   // template arguments into account
//   if (!Cand.getFunc()->getConstraints().empty()) {
//      auto Res = SP.checkConstraints(Caller, Cand.getFunc(),
//                                     TemplateArgs.outermost());
//
//      if (auto C = Res.getFailedConstraint()) {
//         return Cand.setHasFailedConstraint(C);
//      }
//
//      if (Res.isDependent()) {
//         Cand.setIsDependent();
//      }
//   }
//}
//
//} // namespace ast
//} // namespace cdot