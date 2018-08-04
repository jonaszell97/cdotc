//
// Created by Jonas Zell on 08.11.17.
//

#include "OverloadResolver.h"

#include "Sema/SemaPass.h"

using namespace cdot::support;
using namespace cdot::sema;

namespace cdot {
namespace ast {

using ConvSeqVec = SmallVectorImpl<ConversionSequenceBuilder>;

OverloadResolver::OverloadResolver(SemaPass &SP,
                                   Expression *SelfArg,
                                   ArrayRef<Expression*> givenArgs,
                                   ArrayRef<Expression*> givenTemplateArgs,
                                   ArrayRef<IdentifierInfo*> givenLabels,
                                   Statement *Caller)
   : SP(SP), SelfArg(SelfArg), givenArgs(givenArgs),
     givenTemplateArgs(givenTemplateArgs), givenLabels(givenLabels),
     Caller(Caller)
{

}

static unsigned castPenalty(const ConversionSequenceBuilder &neededCast)
{
   unsigned penalty = 0;

   // only implicit casts should occur here
   for (const auto &C : neededCast.getSteps()) {
      switch (C.getKind()) {
      case CastKind::NoOp:
      case CastKind::Move:
      case CastKind::Forward:
      case CastKind::LValueToRValue:
      case CastKind::MutRefToRef:
      case CastKind::RValueToConstRef:
      case CastKind::ToMetaType:
         break;
      case CastKind::Ext:
      case CastKind::FPExt:
      case CastKind::SignFlip:
      case CastKind::EnumToInt:
      case CastKind::IntToEnum:
      case CastKind::Copy:
      case CastKind::MutPtrToPtr:
      case CastKind::IsNull:
      case CastKind::BitCast:
         penalty += 1;
         break;
      case CastKind::ProtoWrap:
         penalty += 2;
         break;
      case CastKind::UpCast:
         penalty += 2;
         break;
      case CastKind::ConversionOp:
         penalty += 3;
         break;
      default:
         llvm_unreachable("bad implicit cast kind!");
      }
   }

   return penalty;
}

static bool getConversionPenalty(SemaPass &SP,
                                 QualType NeededTy, QualType GivenTy,
                                 FunctionType::ParamInfo NeededParamInfo,
                                 CandidateSet::Candidate &Cand,
                                 CandidateSet &CandSet,
                                 ConvSeqVec &Conversions,
                                 bool IsSelf, unsigned ArgNo) {
   if (NeededTy->isUnknownAnyType()) {
      assert((SP.isInDependentContext() || Cand.isBuiltinCandidate())
             && "argument of UnknownAny type in non-dependent context!");

      if (!Cand.isBuiltinCandidate()) {
         Cand.setIsDependent();
         CandSet.Dependent = true;
      }

      return false;
   }

   if (GivenTy->isDependentType() || NeededTy->isDependentType()) {
      Cand.setIsDependent();
      CandSet.Dependent = true;

      return false;
   }

   ConversionSequenceBuilder ConvSeq;
   bool AddCopy = false;

   // Allow implicit conversion from mutable reference to mutable
   // borrow for self argument.
   if (IsSelf
       && Cand.Func->isNonStaticMethod()
       && NeededTy->isMutableBorrowType()
       && GivenTy->isMutableReferenceType()) {
      GivenTy = SP.getContext().getMutableBorrowType(
         GivenTy->stripReference());

      ConvSeq.addStep(CastKind::BitCast, GivenTy);
   }

   // check parameter passing convention
   switch (NeededParamInfo.getConvention()) {
   case ArgumentConvention::Default:
      llvm_unreachable("didn't remove default convention!");
   case ArgumentConvention::Borrowed:
      if (GivenTy->isMutableBorrowType()) {
         ++Cand.ConversionPenalty;
      }

      // if we're given a mutable reference, we need to pass an
      // immutable one
      if (GivenTy->isMutableReferenceType()) {
         ConvSeq.addStep(CastKind::MutRefToRef,
                         SP.getContext().getReferenceType(
                            GivenTy->getReferencedType()));
      }

      break;
   case ArgumentConvention::ImmutableRef: {
      if (GivenTy->isMutableBorrowType()) {
         Cand.setRequiresRef(ArgNo);
         return true;
      }

      if (GivenTy->isMutableReferenceType()) {
         ++Cand.ConversionPenalty;
         ConvSeq.addStep(CastKind::MutRefToRef,
                         SP.getContext().getReferenceType(
                            GivenTy->getReferencedType()));
      }
      else if (!GivenTy->isReferenceType()) {
         Cand.setRequiresRef(ArgNo);
         return true;
      }

      if (!NeededTy->isReferenceType())
         NeededTy = SP.getContext().getReferenceType(NeededTy);

      break;
   }
   case ArgumentConvention::MutableRef: {
      // the left hand side of an assignment does not need to be
      // explicitly borrowed
      if (Cand.isAssignmentOperator() && ArgNo == 0) {
         if (!NeededTy->isMutableReferenceType())
            NeededTy = SP.getContext()
                         .getMutableReferenceType(
                            NeededTy->stripReference());
      }
      else if (!NeededTy->isMutableBorrowType()) {
         NeededTy = SP.getContext()
                      .getMutableBorrowType(NeededTy->stripReference());
      }
      else if (!GivenTy->isMutableBorrowType()) {
         if (IsSelf && GivenTy->isReferenceType()) {
            Cand.setMutatingOnConstSelf();
            return true;
         }
         if (IsSelf) {
            Cand.setMutatingOnRValueSelf();
            return true;
         }

         Cand.setRequiresRef(ArgNo);
         return true;
      }

      break;
   }
   case ArgumentConvention::Owned: {
      // Requires passing ownership: for a temporary, this is a noop
      // since we can forward, but for an lvalue it's a move.

      // if the type is implicitly copyable, pass a copy instead
      if (GivenTy->isReferenceType()) {
         QualType Deref = GivenTy->getReferencedType();
         if (Deref->isRefcounted()) {
            AddCopy = true;
         }
         else if (SP.IsImplicitlyCopyableType(Deref)) {
            AddCopy = true;
         }
         else {
            // can't pass a non-mutable reference to an owned parameter
            if (GivenTy->isNonMutableReferenceType()) {
               Cand.setRequiresRef(ArgNo);
               return true;
            }

            ConvSeq.addStep(CastKind::Forward, GivenTy);
         }
      }
         // if it's an rvalue, forward it
      else {
         ConvSeq.addStep(CastKind::Forward, GivenTy);
      }

      break;
   }
   }

   SP.getConversionSequence(ConvSeq, GivenTy, NeededTy);

   if (!ConvSeq.isImplicit()) {
      if (IsSelf) {
         Cand.setHasIncompatibleSelfArgument(NeededTy, GivenTy);
      }
      else {
         Cand.setHasIncompatibleArgument(ArgNo, GivenTy);
      }

      return true;
   }

   if (AddCopy)
      ConvSeq.addStep(CastKind::Copy, ConvSeq.getSteps().back()
                                             .getResultType());

   Cand.ConversionPenalty += castPenalty(ConvSeq);
   Conversions.emplace_back(move(ConvSeq));

   return false;
}

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

   if (auto *Ident = dyn_cast<IdentifierRefExpr>(E)) {
      if ((!Ident->getParentExpr()
      || isa<SelfExpr>(Ident->getParentExpr()->ignoreParensAndImplicitCasts()))
      && Ident->getIdentInfo() == NeededLabel) {
         return true;
      }
   }

   return false;
}

static bool isVariadic(QualType Ty)
{
   return Ty->isGenericType() && Ty->asGenericType()->isVariadic();
}

static bool resolveContextDependentArgs(SemaPass &SP,
                                        Statement *Caller,
                                        CallableDecl *CD,
                                        Expression *SelfVal,
                                        ArrayRef<IdentifierInfo*> labels,
                                        ArrayRef<Expression*> UnorderedArgs,
                                        SmallVectorImpl<StmtOrDecl> &ArgExprs,
                                        CandidateSet::Candidate &Cand,
                                        CandidateSet &CandSet,
                                        ConvSeqVec &Conversions,
                                        MultiLevelTemplateArgList
                                            *TemplateArgs = nullptr) {
   // Assign arguments based on labels.
   llvm::DenseMap<FuncArgDecl*, std::vector<Expression*>> DeclArgMap;

   ArrayRef<FuncArgDecl*> ArgDecls = CD->getArgs();
   FuncArgDecl *VariadicArgDecl = nullptr;

   bool CStyleVararg = CD->isCstyleVararg();
   unsigned NumGivenArgs = (unsigned)UnorderedArgs.size();

   unsigned i = 0;
   unsigned LabelNo = 0;

   for (auto *ArgVal : UnorderedArgs) {
      // Check if this argument is labeled.
      IdentifierInfo *Label;
      if (SelfVal && i == 0 && !CD->isCompleteInitializer()) {
         Label = nullptr;
      }
      else if (LabelNo < labels.size()) {
         Label = labels[LabelNo++];
      }
      else {
         Label = nullptr;
      }

      // If so, look for an argument with that label.
      if (Label && !Label->isStr("_")) {
         bool FoundLabel = false;
         for (auto *ArgDecl : ArgDecls) {
            if (ArgDecl->getLabel() == Label) {
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
   
   // Now resolve types of context dependent arguments.
   i = 0;
   for (FuncArgDecl *ArgDecl : ArgDecls) {
      assert(ArgDecl->wasDeclared());

      // If no argument is given, check if there is a default one.
      auto It = DeclArgMap.find(ArgDecl);
      if (It == DeclArgMap.end()) {
         if (ArgDecl->isVariadicArgPackExpansion()) {
            ++i;
            continue;
         }
         if (!ArgDecl->getValue()) {
            Cand.setHasTooFewArguments(NumGivenArgs, CD->getArgs().size());
            return false;
         }

         auto DefaultVal = ArgDecl->getDefaultVal();
         if (DefaultVal->isMagicArgumentValue()) {
            auto Alias = cast<IdentifierRefExpr>(DefaultVal)->getAlias();
            auto Result = SP.HandleReflectionAlias(Alias,
                                                   cast<Expression>(Caller));

            if (Result) {
               ArgExprs.emplace_back(Result.getValue());
            }
            else {
               ArgExprs.emplace_back(DefaultVal);
            }
         }
         else if (CD->isTemplate() || CD->isInitializerOfTemplate()
               || CD->isCaseOfTemplatedEnum()) {
            ArgExprs.emplace_back(ArgDecl);
         }
         else {
            ArgExprs.emplace_back(DefaultVal);
         }

         Conversions.emplace_back(ConversionSequenceBuilder::MakeNoop());
         ++i;

         continue;
      }

      for (Expression *ArgVal : DeclArgMap[ArgDecl]) {
         QualType NeededTy = ArgDecl->getType();
         if (TemplateArgs) {
            NeededTy = SP.resolveDependencies(NeededTy, *TemplateArgs,
                                              Caller);
         }

         // If the arguments type is context depentent, we need to some extra
         // checking.
         if (ArgVal->isContextDependent()) {
            // Handle closures specially.
            if (auto LE = dyn_cast<LambdaExpr>(ArgVal)) {
               auto LambdaTy = SP.ResolveContextualLambdaExpr(LE, NeededTy);
               if (!LambdaTy) {
                  if (LE->isInvalid()) {
                     Cand.setIsInvalid();
                  }
                  else if (auto Def = SP.GetDefaultExprType(LE)) {
                     Cand.setHasIncompatibleArgument(i, Def);
                  }
                  else {
                     Cand.setCouldNotInferArgumentType(i);
                  }

                  return false;
               }

               if (TemplateArgs)
                  TemplateArgs->inferFromType(
                     LambdaTy, NeededTy,
                     false);

               Conversions.emplace_back(ConversionSequenceBuilder::MakeNoop());
               ArgExprs.emplace_back(ArgVal);

               continue;
            }

            // Check if the expression would be able to return a value of
            // that type.
            int CanReturn = SP.ExprCanReturn(ArgVal, NeededTy);
            if (CanReturn == -1) {
               if (ArgVal->isInvalid()) {
                  Cand.setIsInvalid();
               }
               else {
                  if (auto Def = SP.GetDefaultExprType(ArgVal)) {
                     Cand.setHasIncompatibleArgument(i, Def);
                  }
                  else {
                     Cand.setCouldNotInferArgumentType(i);
                  }
               }

               return false;
            }

            QualType ArgValType;
            if (NeededTy->isDependentType()) {
               // we can't infer a context dependent expression from a
               // dependent type
               auto DefaultType = SP.GetDefaultExprType(ArgVal);
               if (DefaultType) {
                  if (TemplateArgs && NeededTy->isDependentType()) {
                     if (!TemplateArgs->inferFromType(DefaultType,
                                                      NeededTy, false)) {
                        Cand.setHasIncompatibleArgument(i, DefaultType);
                        return false;
                     }

                     if (isVariadic(NeededTy)) {
                        NeededTy = DefaultType;
                     }
                     else {
                        NeededTy = SP.resolveDependencies(NeededTy,
                                                          *TemplateArgs,
                                                          Caller);
                     }
                  }

                  ArgValType = DefaultType;
               }
               else if (NeededTy->isDependentType()) {
                  ArgValType = NeededTy;
               }
               else {
                  if (ArgVal->isInvalid()) {
                     Cand.setIsInvalid();
                     return false;
                  }

                  Cand.setCouldNotInferArgumentType(i);
                  return false;
               }

               if (NeededTy->isDependentType()) {
                  Cand.setIsDependent();
                  return true;
               }

               if (getConversionPenalty(SP, NeededTy, ArgValType,
                                        {ArgDecl->getConvention()},
                                        Cand, CandSet, Conversions,
                                        ArgDecl->isSelf(), i)) {
                  return false;
               }
            }
            else {
               Cand.ConversionPenalty += CanReturn;
               Conversions.emplace_back(ConversionSequenceBuilder::MakeNoop());
            }
         }
         else {
            auto ArgValType = ArgVal->getExprType();
            if (TemplateArgs && NeededTy->isDependentType()) {
               if (!TemplateArgs->inferFromType(ArgValType, NeededTy,
                                                false)) {

                  Cand.setHasIncompatibleArgument(i, ArgValType);
                  return false;
               }

               if (isVariadic(NeededTy)) {
                  // Never infer reference types for template arguments.
                  NeededTy = ArgValType->stripReference();
               }
               else {
                  NeededTy = SP.resolveDependencies(NeededTy,
                                                    *TemplateArgs,
                                                    Caller);
               }
            }

            if (NeededTy->isDependentType()) {
               Cand.setIsDependent();
               return true;
            }

            if (getConversionPenalty(SP, NeededTy, ArgValType,
                                     {ArgDecl->getConvention()},
                                     Cand, CandSet, Conversions,
                                     ArgDecl->isSelf(), i)) {
               return false;
            }
         }

         ArgExprs.emplace_back(ArgVal);
      }

      ++i;
   }

   auto VariadicArgIt = DeclArgMap.find(nullptr);
   if (VariadicArgIt != DeclArgMap.end()) {
      for (auto *E : VariadicArgIt->getSecond()) {
         // Apply standard c-style vararg conversion.
         ConversionSequenceBuilder ConvSeq;
         if (E->isLValue()) {
            ConvSeq.addStep(CastKind::LValueToRValue,
                            E->getExprType()->stripReference());
         }
         else {
            ConvSeq.addStep(CastKind::NoOp, QualType());
         }

         Conversions.emplace_back(move(ConvSeq));
         ArgExprs.emplace_back(E);
      }
   }

   return true;
}

static bool resolveContextDependentArgs(SemaPass &SP,
                                        Statement *Caller,
                                        Expression *SelfVal,
                                        ArrayRef<Expression*> UnorderedArgs,
                                        ArrayRef<IdentifierInfo*> labels,
                                        SmallVectorImpl<StmtOrDecl> &ArgExprs,
                                        CandidateSet::Candidate &Cand,
                                        CandidateSet &CandSet,
                                        ConvSeqVec &Conversions,
                                        MultiLevelTemplateArgList*TemplateArgs){
   if (!Cand.isBuiltinCandidate()) {
      return resolveContextDependentArgs(SP, Caller, Cand.Func, SelfVal, labels,
                                         UnorderedArgs, ArgExprs, Cand, CandSet,
                                         Conversions, TemplateArgs);
   }

   auto *FuncTy = Cand.getFunctionType();
   auto neededArgs = FuncTy->getParamTypes();
   auto NeededParamInfo = FuncTy->getParamInfo();

   unsigned i = 0;
   for (auto *ArgVal : UnorderedArgs) {
      if (i >= neededArgs.size()) {
         Cand.setHasTooManyArguments(UnorderedArgs.size(), neededArgs.size());
         return false;
      }

      QualType NeededTy = neededArgs[i];
      if (TemplateArgs)
         NeededTy = SP.resolveDependencies(NeededTy, *TemplateArgs, Caller);

      if (ArgVal->isContextDependent()) {
         if (auto LE = dyn_cast<LambdaExpr>(ArgVal)) {
            auto LambdaTy = SP.ResolveContextualLambdaExpr(LE, NeededTy);
            if (!LambdaTy) {
               if (LE->isInvalid()) {
                  Cand.setIsInvalid();
               }
               else if (auto Def = SP.GetDefaultExprType(LE)) {
                  Cand.setHasIncompatibleArgument(i, Def);
               }
               else {
                  Cand.setCouldNotInferArgumentType(i);
               }

               return false;
            }

            ++i;
            continue;
         }

         int CanReturn = SP.ExprCanReturn(ArgVal, NeededTy);
         if (CanReturn == -1) {
            if (ArgVal->isInvalid()) {
               Cand.setIsInvalid();
            }
            else {
               if (auto Def = SP.GetDefaultExprType(ArgVal)) {
                  Cand.setHasIncompatibleArgument(i, Def);
               }
               else {
                  Cand.setCouldNotInferArgumentType(i);
               }
            }

            return false;
         }

         QualType ArgValType;
         if (NeededTy->isDependentType()) {
            // we can't infer a context dependent expression from a
            // dependent type
            auto DefaultType = SP.GetDefaultExprType(ArgVal);
            if (DefaultType) {
               ArgValType = DefaultType;
            }
            else if (NeededTy->isDependentType()) {
               ArgValType = NeededTy;
            }
            else {
               if (ArgVal->isInvalid()) {
                  Cand.setIsInvalid();
                  return false;
               }

               Cand.setCouldNotInferArgumentType(i);
               return false;
            }

            if (getConversionPenalty(SP, NeededTy, ArgValType,
                                     NeededParamInfo[i],
                                     Cand, CandSet, Conversions,
                                     false, i)) {
               return false;
            }
         }
         else {
            Cand.ConversionPenalty += CanReturn;
         }
      }
      else {
         if (getConversionPenalty(SP, NeededTy, ArgVal->getExprType(),
                                  NeededParamInfo[i],
                                  Cand, CandSet, Conversions,
                                  false, i)) {
            return false;
         }
      }

      ++i;
   }

   for (auto *Arg : UnorderedArgs)
      ArgExprs.emplace_back(Arg);

   return true;
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
      E->setCaptureIndex(LE->addCapture(C, E->getSelfArg()));
      return true;
   }

   bool visitSuperExpr(SuperExpr *E)
   {
      E->setCaptureIndex(LE->addCapture(C, E->getSelfArg()));
      return true;
   }
};

} // anonymous namespace

static void applyConversions(SemaPass &SP,
                             CandidateSet &CandSet,
                             SmallVectorImpl<StmtOrDecl> &ArgExprs,
                             SmallVectorImpl<ConversionSequenceBuilder>
                                &Conversions,
                             Statement *Caller) {
   auto &Cand = CandSet.getBestMatch();
   auto ParamTys = Cand.getFunctionType()->getParamTypes();

   ArrayRef<FuncArgDecl*> ArgDecls;
   if (!Cand.isBuiltinCandidate()) {
      ArgDecls = Cand.Func->getArgs();
   }

   unsigned i = 0;
   for (auto &SOD : ArgExprs) {
      if (auto *Decl = SOD.asDecl()) {
         // Get the instantiated default value.
         SOD = Cand.Func->lookupSingle<FuncArgDecl>(
            cast<FuncArgDecl>(Decl)->getDeclName())->getDefaultVal();
      }

      auto *E = cast<Expression>(SOD.getStatement());

      FuncArgDecl *ArgDecl = nullptr;
      if (i < ArgDecls.size()) {
         ArgDecl = ArgDecls[i];
      }

      if (E->isContextDependent()) {
         if (ParamTys.size() > i)
            E->setContextualType(ParamTys[i]);

         auto Result = SP.visitExpr(Caller, E);
         if (!Result) {
            ++i;
            continue;
         }

         E = Result.get();
      }
      else if (E->getExprType()->isVoidType()) {
         SP.diagnose(E, diag::err_vararg_cannot_pass_void, E->getSourceRange());
      }
      else if (ArgDecl &&ArgDecl->getConvention() == ArgumentConvention::Owned){
         // mark this declaration as moved from
         if (auto Ident = dyn_cast<IdentifierRefExpr>(E)) {
            auto ND = Ident->getNamedDecl();
            if (auto VD = dyn_cast_or_null<VarDecl>(ND)) {
               VD->setMovedFrom(true);
            }
         }
      }

      // Handle @autoclosure
      if (ArgDecl && ArgDecl->hasAttribute<AutoClosureAttr>()) {
         auto LE = LambdaExpr::Create(
            SP.getContext(), E->getSourceRange(),
            E->getSourceLoc(), SourceType(SP.getContext().getAutoType()),
            {}, ReturnStmt::Create(SP.getContext(), E->getSourceLoc(), E));

         (void) SP.visitExpr(E, LE);
         CaptureMarker(SP.getContext(), LE).visit(E);

         E = LE;
      }

      // Apply conversion.
      if (i < Conversions.size()) {
         auto &ConvSeq = Conversions[i];
         if (!ConvSeq.isNoOp()) {
            auto *Seq = ConversionSequence::Create(SP.getContext(), ConvSeq);
            E = ImplicitCastExpr::Create(SP.getContext(), E, Seq);

            auto Res = SP.visitExpr(E); (void) Res;
            assert(Res && "bad implicit cast sequence!");
         }
      }

      SOD = E;
      ++i;
   }
}

void OverloadResolver::resolve(CandidateSet &CandSet)
{
   SmallVector<ConversionSequenceBuilder, 4> BestMatchConversions;
   SmallVector<StmtOrDecl, 4> BestMatchArgExprs;

   bool foundMatch       = false;
   bool MatchIsDependent = false;
   bool Dependent        = false;
   bool ambiguous        = false;

   unsigned BestMatch      = 0;
   unsigned BestMatchDistance = 0;
   unsigned MaxConstraints = 0;
   unsigned NumCandidates  = (unsigned)CandSet.Candidates.size();

   SmallVector<Expression*, 4> ArgVec;
   ArgVec.push_back(SelfArg);
   ArgVec.append(givenArgs.begin(), givenArgs.end());

   ArrayRef<Expression*> ArgRef = ArgVec;
   for (unsigned i = 0; i < NumCandidates; ++i) {
      CandidateSet::Candidate &Cand = CandSet.Candidates[i];
      if (Cand.Func) {
         if (!SP.ensureDeclared(Cand.Func) || Cand.Func->isInvalid()) {
            Cand.setIsInvalid();
            CandSet.InvalidCand = true;
            continue;
         }
      }

      assert(Cand.isBuiltinCandidate() || Cand.Func->isTemplate()
             || Cand.Func->getFunctionType()
                && "function without function type");

      // Drop self argument for self.init() calls.
      SmallVector<ConversionSequenceBuilder, 4> Conversions;
      SmallVector<StmtOrDecl, 4> ArgExprs;

      if (Cand.isBuiltinCandidate()
            || Cand.Func->isCompleteInitializer()
            || !SelfArg) {
         resolve(CandSet, Cand, ArgRef.drop_front(1), Conversions, ArgExprs);
      }
      else {
         resolve(CandSet, Cand, ArgRef, Conversions, ArgExprs);
      }

#     ifndef NDEBUG
      if (Cand.FR == CandidateSet::IsInvalid)
         assert(CandSet.InvalidCand
                && "didn't update InvalidCand flag on CandidateSet!");
#     endif

      bool IsDependent = Cand.FR == CandidateSet::IsDependent;
      Dependent |= IsDependent;

      if (Cand || IsDependent) {
         bool IsBetterMatch =
            (!foundMatch || BestMatchDistance >= Cand.Distance)
            && CandSet.BestConversionPenalty >= Cand.ConversionPenalty
            && Cand.getNumConstraints() >= MaxConstraints;

         if (foundMatch
                && CandSet.BestConversionPenalty == Cand.ConversionPenalty
                && Cand.getNumConstraints() == MaxConstraints) {
            // dependent candidates might not actually be valid at
            // instantiation time, so don't report an error
            if (Cand.FR != CandidateSet::IsDependent && !MatchIsDependent) {
               ambiguous = true;
            }
         }

         if (IsDependent) {
            MatchIsDependent = true;
            Cand.FR = CandidateSet::None;
         }
         else if (IsBetterMatch) {
            BestMatch = i;
            BestMatchConversions = move(Conversions);
            BestMatchArgExprs = move(ArgExprs);
            MaxConstraints = Cand.getNumConstraints();
         }

         foundMatch = true;
         BestMatchDistance = Cand.Distance;
         CandSet.maybeUpdateBestConversionPenalty(Cand.ConversionPenalty);
      }
   }

   if (Dependent) {
      CandSet.Dependent = true;
      return;
   }

   if (ambiguous) {
      CandSet.Status = CandidateSet::Ambiguous;
   }
   else if (foundMatch) {
      CandSet.Status = CandidateSet::Success;
      CandSet.MatchIdx = BestMatch;

      SP.maybeInstantiate(CandSet, Caller);

      applyConversions(SP, CandSet, BestMatchArgExprs, BestMatchConversions,
                       Caller);

      for (auto &Arg : BestMatchArgExprs)
         CandSet.ResolvedArgs.push_back(cast<Expression>(Arg.getStatement()));
   }
}

LLVM_ATTRIBUTE_UNUSED
static bool hasDependentSignature(CallableDecl *C)
{
   for (auto &Arg : C->getArgs())
      if (Arg->isDependent())
         return true;

   if (auto E = C->getReturnType().getTypeExpr())
      return E->isDependent();

   return false;
}

void OverloadResolver::resolve(CandidateSet &CandSet,
                               CandidateSet::Candidate &Cand,
                               llvm::ArrayRef<Expression*> givenArgs,
                               ConvSeqVec &Conversions,
                               ArgVec &ArgExprs) {
   FunctionType *FuncTy = Cand.getFunctionType();
   llvm::SmallVector<QualType, 8> resolvedGivenArgs;

   bool IsTemplate = !Cand.isBuiltinCandidate()
                     && (Cand.Func->isTemplate()
                         || Cand.Func->isInitializerOfTemplate()
                         || Cand.Func->isCaseOfTemplatedEnum());

   if (IsTemplate) {
      SourceLocation listLoc = givenTemplateArgs.empty()
                               ? Caller->getSourceLoc()
                               : givenTemplateArgs.front()->getSourceLoc();

      bool NeedOuterTemplateParams
         = (isa<InitDecl>(Cand.Func) || isa<EnumCaseDecl>(Cand.Func))
            && Cand.Func->getRecord()->isTemplate();

      Cand.InnerTemplateArgs = TemplateArgList(
         SP, Cand.Func,
         // if we have an initializer or enum case, the given template
         // arguments will be passed to the record parameter list
         NeedOuterTemplateParams ? llvm::ArrayRef<Expression*>()
                                 : givenTemplateArgs,
         listLoc);

      TemplateArgList OuterTemplateArgs;
      MultiLevelTemplateArgList TemplateArgs(Cand.InnerTemplateArgs);

      // initializers and enum cases also need their containing records
      // template arguments specified (or inferred)
      if (NeedOuterTemplateParams) {
         OuterTemplateArgs = TemplateArgList(SP, Cand.Func->getRecord(),
                                             givenTemplateArgs, listLoc);

         TemplateArgs.addOuterList(OuterTemplateArgs);
      }

      // try to infer unspecified template arguments from the function signature
      if (Caller && Caller->getContextualType()) {
         TemplateArgs.inferFromType(Caller->getContextualType(),
                                    FuncTy->getReturnType());
      }

      if (!resolveContextDependentArgs(SP, Caller, SelfArg, givenArgs,
                                       givenLabels, ArgExprs, Cand, CandSet,
                                       Conversions, &TemplateArgs)) {
         if (Cand.FR != CandidateSet::None)
            return;

         if (NeedOuterTemplateParams) {
            auto OuterComp = OuterTemplateArgs.checkCompatibility();
            if (!OuterComp) {
               // This is kind of a hack, but the template argument list will
               // only be used for diagnostics after this.
               Cand.InnerTemplateArgs = move(OuterTemplateArgs);
               Cand.setTemplateArgListFailure(OuterComp);

               return;
            }
         }

         auto comp = Cand.InnerTemplateArgs.checkCompatibility();
         if (!comp)
            return Cand.setTemplateArgListFailure(comp);

         return;
      }

      // if the template argument list is itself dependent, we have to delay
      // the overload resolution until instantiation time
      if (TemplateArgs.isStillDependent()) {
         CandSet.Dependent = true;
         return Cand.setIsDependent();
      }

      auto comp = TemplateArgs.checkCompatibility();
      if (!comp)
         return Cand.setTemplateArgListFailure(comp);

      // Instantiate the record if this is a template initializer.
      if (NeedOuterTemplateParams) {
         if (!SP.maybeInstantiateRecord(Cand, OuterTemplateArgs, Caller))
            return Cand.setIsInvalid();
      }

      // check the constraints here to take the resolved
      // template arguments into account
      if (!Cand.Func->getConstraints().empty()) {
         auto Res = SP.checkConstraints(Caller, Cand.Func,
                                        TemplateArgs.outermost());

         if (auto C = Res.getFailedConstraint()) {
            return Cand.setHasFailedConstraint(C);
         }

         if (Res.isDependent()) {
            Cand.setIsDependent();
         }
      }

      return;
   }

   if (!givenTemplateArgs.empty()) {
      return Cand.setHasTooManyTemplateArgs(givenTemplateArgs.size(), 0);
   }

   resolveContextDependentArgs(SP, Caller, SelfArg, givenArgs,
                               givenLabels, ArgExprs, Cand,
                               CandSet, Conversions, nullptr);
}

} // namespace ast
} // namespace cdot