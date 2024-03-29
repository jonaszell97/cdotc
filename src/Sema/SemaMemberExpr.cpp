#include "cdotc/AST/Decl.h"
#include "cdotc/AST/Type.h"
#include "cdotc/AST/TypeVisitor.h"
#include "cdotc/Basic/NestedNameSpecifier.h"
#include "cdotc/IL/Constants.h"
#include "cdotc/ILGen/ILGenPass.h"
#include "cdotc/Module/Module.h"
#include "cdotc/Parse/Parser.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/Builtin.h"
#include "cdotc/Sema/OverloadResolver.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Sema/TemplateInstantiator.h"
#include "cdotc/Serialization/IncrementalCompilation.h"
#include "cdotc/Serialization/ModuleFile.h"

#include <llvm/ADT/Twine.h>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::sema;
using namespace cdot::support;

using std::string;

void SemaPass::diagnoseCircularlyDependentGlobalVariables(Expression* Expr,
                                                          NamedDecl* globalVar)
{
   Expr->setIsInvalid(true);

   diagnose(globalVar, err_circular_global_value, globalVar->getSourceLoc(),
            globalVar->getDeclName(), EvaluatingGlobalVar->getDeclName());
   diagnose(note_dependent_global_here, EvaluatingGlobalVar->getSourceLoc());
}

/// Add an implicit 'self' expression for member accesses that don't use one.
static bool checkImplicitSelf(SemaPass& SP, NamedDecl* Decl,
                              IdentifierRefExpr* Ident)
{
   if (Ident->getParentExpr()) {
      return true;
   }

   bool Uppercase = SP.inStaticContext();

   auto Self = SelfExpr::Create(SP.getContext(), Ident->getSourceLoc(), Uppercase);
   Ident->setParentExpr(Self);

   return SP.visitExpr(Ident, Self);
}

/// Verify that the necessary standard library features for closure captures are
/// available.
static void checkCapture(SemaPass& SP, Expression* Expr)
{
   if (!SP.getBoxDecl()) {
      SP.diagnose(Expr, err_no_builtin_decl, Expr->getSourceLoc(),
                  /*Box*/ 13);
   }
}

/// Convert a pointer deref expression (e.g. myValue->referencedProperty).
template<class ExprTy> void checkPointerDeref(SemaPass& SP, QualType ty, ExprTy* Expr)
{
   QualType RefTy;
   if (ty->isMutablePointerType()) {
      RefTy = SP.getContext().getMutableReferenceType(ty->getPointeeType());
   }
   else if (ty->isPointerType()) {
      RefTy = SP.getContext().getReferenceType(ty->getPointeeType());
   }
   else {
      Expr->setIsInvalid(true);
      return;
   }

   auto ParentExpr = SP.castToRValue(Expr->getParentExpr());

   FunctionType* DerefFnTy = SP.getContext().getFunctionType(RefTy, {ty});
   auto Deref = UnaryOperator::Create(SP.getContext(), Expr->getSourceLoc(),
                                      op::Deref, DerefFnTy, ParentExpr, true);

   auto Res = SP.visitExpr(Expr, Deref);
   assert(Res && "invalid deref operator?");

   Expr->setParentExpr(Res.get());
   Expr->setIsPointerAccess(false);
}

/// Convert a deref expression (e.g. myValue->referencedProperty).
Expression* SemaPass::checkCustomDeref(Expression* E, QualType T)
{
   auto diagnoseInvalidDeref = [&]() {
      diagnose(E, err_generic_error,
               T.toDiagString() + " does not conform to 'Dereferenceable'",
               E->getSourceRange());

      return E;
   };

   ProtocolDecl* Dereferenceable;
   if (QC.GetBuiltinProtocol(Dereferenceable,
                             GetBuiltinProtocolQuery::Dereferenceable)) {
      return E;
   }

   if (!Dereferenceable) {
      return diagnoseInvalidDeref();
   }

   bool Conforms = ConformsTo(T, Dereferenceable);
   if (!Conforms || T->getRecord()->isInvalid()) {
      return diagnoseInvalidDeref();
   }

   auto* PropAccess = new (Context)
       IdentifierRefExpr(E->getSourceRange(), E, getIdentifier("deref"));

   auto Res = visitExpr(E, PropAccess);
   if (Res) {
      return Res.get();
   }

   return E;
}

bool SemaPass::refersToNamespace(Expression* E)
{
   auto* DeclRef = dyn_cast<DeclRefExpr>(E);
   if (!DeclRef)
      return false;

   switch (DeclRef->getDecl()->getKind()) {
   case Decl::NamespaceDeclID:
   case Decl::ImportDeclID:
   case Decl::ModuleDeclID:
      return true;
   default:
      return false;
   }
}

QualType SemaPass::getParentType(Expression* ParentExpr)
{
   auto* Ident = dyn_cast_or_null<IdentifierRefExpr>(ParentExpr);
   if (!Ident)
      return QualType();

   if (Ident->getKind() != IdentifierKind::MetaType)
      return QualType();

   auto ParentType = Ident->getExprType()->asMetaType()->getUnderlyingType();
   if (auto* Dep = ParentType->asDependentRecordType()) {
      return Context.getDependentRecordType(
          Dep->getRecord(), &Dep->getTemplateArgs(),
          getParentType(Ident->getParentExpr()));
   }

   return ParentType;
}

/// Check whether an identifier refers to a builtin type member, like
/// the static '.Type' member or an array size.
static ExprResult CheckBuiltinIdentifier(SemaPass &Sema,
                                         IdentifierRefExpr *Ident,
                                         QualType ParentType,
                                         DeclarationName DeclName)
{
   /// T.Type -> MetaType<T>
   if (ParentType && DeclName.isStr("Type")) {
      if (auto *Tup = dyn_cast<TupleLiteral>(Ident->getParentExpr())) {
         if (Tup->getElements().empty()) {
            // Since '()' is the spelling of both the value and the type,
            // ().Type actually introduces two levels of meta type.
            ParentType = Sema.Context.getMetaType(ParentType);
         }
      }

      Ident->setExprType(Sema.Context.getMetaType(ParentType));
      Ident->setKind(IdentifierKind::TypeOf);

      return Ident;
   }

   /// Fixed-size array size
   if (ParentType && ParentType->isArrayType() && DeclName.isStr("size")) {
      Ident->setExprType(Sema.Context.getRecordType(Sema.getInt64Decl()));
      Ident->setKind(IdentifierKind::BuiltinArraySize);

      return Ident;
   }

   return ExprError();
}

static ExprResult CreatePartiallyAppliedMethod(SemaPass &Sema,
                                               IdentifierRefExpr *Ident,
                                               MethodDecl *M)
{
   if (Sema.QC.PrepareDeclInterface(M)) {
      return ExprError();
   }

   SmallVector<FuncArgDecl*, 2> Args;
   for (auto *Arg : M->getArgs().drop_front(1)) {
      auto *NewArg = FuncArgDecl::Create(Sema.Context, SourceLocation(),
         SourceLocation(), Arg->getDeclName(),
         Arg->getLabel(), Arg->getConvention(),
         Arg->getType(), nullptr, false);

      Args.push_back(NewArg);
   }

   auto *LE = LambdaExpr::Create(Sema.Context, Ident->getSourceRange(),
      Ident->getSourceLoc(), M->getReturnType(), Args, nullptr);

   SmallVector<Expression*, 2> CallArgs{ Ident->getParentExpr() };
   for (auto *Arg : Args) {
      CallArgs.push_back(DeclRefExpr::Create(
         Sema.Context, Arg, Ident->getSourceRange()));
   }

   auto *Call = Sema.CreateCall(M, CallArgs, Ident->getSourceLoc());
   auto *Ret = ReturnStmt::Create(Sema.Context, Ident->getSourceLoc(), Call);

   LE->setBody(Ret);
   sema::markCaptures(Sema, LE);

   return Sema.visitExpr(LE);
}

/// Find declarations whose name is close to the given one for fix-its.
static DeclContextLookupResult
FindSimilarlyNamedDecl(SemaPass &Sema, DeclarationName Name,
                       DeclContext* LookupCtx, QualType LookupType) {
   StringRef NameRef;
   if (Name.isSimpleIdentifier()) {
      NameRef = Name.getIdentifierInfo()->getIdentifier();
   }
   else if (Name.getKind() == DeclarationName::MacroName) {
      NameRef = Name.getMacroName()->getIdentifier();
   }
   else if (Name.getKind() == DeclarationName::LocalVarName) {
      NameRef = Name.getLocalVarName().getIdentifierInfo()->getIdentifier();
   }
   else {
      return DeclContextLookupResult();
   }

   bool directContextOnly = true;
   if (!LookupCtx) {
      if (LookupType) {
         if (!LookupType->isRecordType()) {
            return DeclContextLookupResult();
         }

         LookupCtx = LookupType->getRecord();
      }
      else {
         LookupCtx = &Sema.getDeclContext();
         directContextOnly = false;
      }
   }
   else {
      directContextOnly = !isa<ModuleDecl>(LookupCtx);
   }

   static constexpr unsigned MaxEditDistance = 3;

   DeclContextLookupResult PotentialMatch;
   unsigned MinDistance = -1;

   llvm::SmallSetVector<DeclContext*, 4> DeclCtxQueue;
   DeclCtxQueue.insert(LookupCtx);

   if (!directContextOnly) {
      LookupCtx = LookupCtx->getParentCtx();

      while (LookupCtx && isa<NamedDecl>(LookupCtx)) {
         DeclCtxQueue.insert(LookupCtx);
         LookupCtx = LookupCtx->getParentCtx();
      }
   }

   for (size_t i = 0; i < DeclCtxQueue.size(); ++i) {
      auto *DC = DeclCtxQueue[i];
      if (auto *ModFile = DC->getModFile()) {
         ModFile->LoadAllDecls(*DC);
      }

      for (auto &Entry : DC->getAllNamedDecls()) {
         auto OtherName = Entry.getFirst();

         StringRef OtherNameRef;
         if (OtherName.isSimpleIdentifier()) {
            OtherNameRef = OtherName.getIdentifierInfo()->getIdentifier();
         }
         else if (OtherName.getKind() == DeclarationName::MacroName) {
            OtherNameRef = OtherName.getMacroName()->getIdentifier();
         }
         else if (OtherName.getKind() == DeclarationName::LocalVarName) {
            OtherNameRef = OtherName.getLocalVarName().getIdentifierInfo()->getIdentifier();
         }
         else {
            continue;
         }

         unsigned distance = NameRef.edit_distance(OtherNameRef, true, MaxEditDistance);
         if (distance < MaxEditDistance && distance < MinDistance) {
            MinDistance = distance;
            PotentialMatch = Entry.getSecond().getAsLookupResult();
         }
      }

      if (auto *Mod = dyn_cast<ModuleDecl>(DC)) {
         for (auto *SubMod : Mod->getModule()->getSubModules()) {
            DeclCtxQueue.insert(SubMod->getDecl());
         }
      }
   }

   return PotentialMatch;
}

/// Diagnose that an identifier could not be resolved.
static void diagnoseIdentifierNotFound(SemaPass &Sema, IdentifierRefExpr *Ident,
                                       DeclarationName DeclName,
                                       DeclContext *LookupCtx,
                                       CanType LookupType)
{
   if (!Ident->shouldIssueDiag()) {
      Ident->setIsInvalid(true);
      return;
   }

   if (Ident->isSynthesized()) {
      unsigned DiagIdx;
      if (DeclName.isStr("Array")) {
         DiagIdx = 5;
      }
      else if (DeclName.isStr("Dictionary")) {
         DiagIdx = 6;
      }
      else if (DeclName.isStr("Option")) {
         DiagIdx = 7;
      }
      else {
         llvm_unreachable("bad synthesized identifier!");
      }

      Sema.diagnose(Ident, err_no_builtin_decl, Ident->getSourceRange(), DiagIdx);
      return;
   }

   MessageKind diagId;
   DeclContext *DC = nullptr;

   // Provide appropriate diagnostic for type lookup, member lookup and
   // normal identifier lookup.
   if (Ident->isInTypePosition()) {
      diagId = diag::err_type_not_found;
   }
   else if (LookupCtx) {
      diagId = diag::err_member_not_found;
      DC = LookupCtx;
   }
   else if (LookupType && LookupType->isRecordType()) {
      diagId = diag::err_member_not_found;
      DC = LookupType->getRecord();
   }
   else {
      diagId = diag::err_undeclared_identifer;
   }

   Sema.diagnoseMemberNotFound(DC, Ident, DeclName, diagId);
}

static DeclContext *checkNamespaceRef(SemaPass& SP, IdentifierRefExpr* Expr)
{
   Expression* ParentExpr = Expr->getParentExpr();
   if (!ParentExpr) {
      return nullptr;
   }

   bool IsBaseExpr = !ParentExpr->getParentExpr();

   if (auto* Ident = dyn_cast<IdentifierRefExpr>(ParentExpr->ignoreParens())) {
      Ident->setAllowNamespaceRef(true);
   }

   auto SemaRes = SP.typecheckExpr(ParentExpr);
   Expr->copyStatusFlags(ParentExpr);

   if (!SemaRes) {
      return nullptr;
   }

   ParentExpr = SemaRes.get();
   Expr->setParentExpr(ParentExpr);

   DeclContext* Ctx = nullptr;
   if (auto* TAExpr = dyn_cast<TemplateArgListExpr>(ParentExpr)) {
      ParentExpr = TAExpr->getParentExpr();
   }

   if (auto* DeclRef = dyn_cast<DeclRefExpr>(ParentExpr)) {
      auto* ND = DeclRef->getDecl();
      switch (ND->getKind()) {
      case Decl::NamespaceDeclID:
      case Decl::ImportDeclID:
         Ctx = cast<DeclContext>(ND);
         break;
      case Decl::ModuleDeclID: {
         auto *Mod = cast<ModuleDecl>(ND);
         if (IsBaseExpr && !Mod->getModule()->isBaseModule()) {
            SP.diagnose(err_module_must_be_imported, ND->getDeclName(),
                        DeclRef->getSourceLoc());
         }

         Ctx = Mod;
         break;
      }
      default:
         break;
      }
   }

   Expr->copyStatusFlags(ParentExpr);
   return Ctx;
}

ExprResult SemaPass::visitIdentifierRefExpr(IdentifierRefExpr* Ident,
                                            TemplateArgListExpr* ArgExpr)
{
   MutableArrayRef<Expression*> TemplateArgs;
   if (ArgExpr) {
      TemplateArgs = ArgExpr->getExprs();
   }

   // Check if this expressions parent expr refers to a namespace, and if it does,
   // use that as the lookup context.
   auto* LookupCtx = ::checkNamespaceRef(*this, Ident);
   if (Ident->isInvalid()) {
      return ExprError();
   }

   // Remove the parent expression because it's only used for providing a lookup
   // context.
   if (LookupCtx) {
      Ident->setDeclCtx(LookupCtx);
      Ident->setParentExpr(nullptr);
   }
   // Check that there is a contextual type present if this identifier has a
   // leading period.
   else if (Ident->hasLeadingDot()) {
      auto CtxTy = Ident->getContextualType();
      if (!CtxTy) {
         diagnose(Ident, err_requires_contextual_type, 2,
                  Ident->getSourceRange());

         return ExprError();
      }

      auto* MetaExpr = new (Context)
          IdentifierRefExpr(Ident->getSourceLoc(), IdentifierKind::MetaType,
                            Context.getMetaType(CtxTy));

      Ident->setParentExpr(MetaExpr);
   }

   // We know that the parent expression does not refer to a namespace, so check
   // the context we should be looking in.
   QualType ParentType;
   QualType ParentTypeNoRef;

   // Set to true if we should also perform a normal lookup in the current
   // context if type lookup yields no results.
   bool doTypeAndNormalLookup = Ident->getDeclName().isAnyOperatorName();
   bool parentTypeWasMetaType = false;

   if (auto* PE = Ident->getParentExpr()) {
      auto ParentRes = visitExpr(Ident, PE);
      if (!ParentRes) {
         return ExprError();
      }

      PE = ParentRes.get();
      Ident->setParentExpr(PE);
      ParentType = PE->getExprType();

      QualType NoSugar = ParentType->removeReference()->getDesugaredType();

      // Handle pointer and custom dereference operators.
      if (Ident->isPointerAccess()) {
         // Pointer access
         if (NoSugar->isPointerType()) {
            if (!Ident->isTypeDependent()) {
               checkPointerDeref(*this, ParentType, Ident);
               Ident->setIsPointerAccess(false);

               PE = Ident->getParentExpr();
               ParentType = PE->getExprType();
            }
            else {
               ParentType = ParentType->getPointeeType();
            }
         }
         // Custom 'Dereferenceable' conformance
         else {
            PE = checkCustomDeref(PE, NoSugar);
            Ident->setParentExpr(PE);
            ParentType = PE->getExprType();
         }
      }

      // If the parent type is a meta type, it can either:
      //   - refer to a DeclContext if it's a record type
      //   - refer to a static type member otherwise
      if (auto* Meta = NoSugar->asMetaType()) {
         parentTypeWasMetaType = true;

         auto Underlying = Meta->getUnderlyingType();
         ParentType = Underlying;
         Ident->setStaticLookup(true);
      }

      ParentTypeNoRef = ParentType->removeReference();
      NoSugar = ParentTypeNoRef->getDesugaredType();

      if (auto* R = NoSugar->asRecordType()) {
         Ident->setDeclCtx(R->getRecord());
      }
      else if (auto* TP = NoSugar->asTemplateParamType()) {
         Ident->setDeclCtx(TP->getCovariance()->getRecord());
      }
      else if (Ident->getDeclName().isAnyOperatorName()) {
         doTypeAndNormalLookup = true;
      }
   }

   auto DeclName = Ident->getDeclName();

   // Check that '.init' is only called on 'self'.
   if (DeclName.getKind() == DeclarationName::BaseConstructorName) {
      if (!dyn_cast_or_null<SelfExpr>(Ident->getParentExpr())
          && !dyn_cast_or_null<SuperExpr>(Ident->getParentExpr())) {
         diagnose(Ident, err_dot_init_must_be_on_self, Ident->getSourceRange());
         Ident->setExprType(Context.getVoidType());

         return Ident;
      }

      DeclName = Context.getDeclNameTable().getConstructorName(
          ParentTypeNoRef->getDesugaredType(), false);
   }
   // Convert explicit deinitializer calls.
   else if (DeclName.getKind() == DeclarationName::DestructorName) {
      if (!ParentTypeNoRef || !ParentTypeNoRef->isRecordType()) {
         DeclName = getIdentifier("deinit");
      }
      else {
         DeclName = Context.getDeclNameTable().getDestructorName(
             ParentTypeNoRef->getDesugaredType());
      }
   }

   // Only look for local names if no context is specified.
   bool LocalLookup = !LookupCtx && ParentType.isNull();
   if (LookupCtx) {
      Ident->setDeclCtx(LookupCtx);
   }
   else {
      Ident->setDeclCtx(DeclCtx);
   }

   LookupOpts Opts = DefaultLookupOpts;
   if (LocalLookup) {
      Opts |= LookupOpts::LocalLookup;
   }
   if (Ident->isInTypePosition()) {
      Opts |= LookupOpts::TypeLookup;
   }

   const MultiLevelLookupResult* LookupResult = nullptr;
   if (ParentType) {
      if (QC.MultiLevelTypeLookup(LookupResult,
                                  ParentTypeNoRef->getDesugaredType(),
                                  DeclName, Opts)) {
         return ExprError();
      }
   }

   if (!LookupResult || (LookupResult->empty() && doTypeAndNormalLookup)) {
      if (QC.MultiLevelLookup(LookupResult, Ident->getDeclCtx(), DeclName,Opts)) {
         return ExprError();
      }
   }

   // Nothing left to check, this identifier does not reference a declaration
   // in scope.
   if (LookupResult->empty()) {
      if (auto Result = CheckBuiltinIdentifier(*this, Ident,
            parentTypeWasMetaType
                ? Context.getMetaType(ParentTypeNoRef) : ParentTypeNoRef,
            DeclName)) {
         return Result;
      }

      diagnoseIdentifierNotFound(*this, Ident, DeclName,
                                 LookupCtx, ParentTypeNoRef);

      // Look for declarations with similar names that the user might have meant.
      auto PotentialMatches = FindSimilarlyNamedDecl(*this, DeclName, LookupCtx,
                                                     ParentTypeNoRef);

      if (PotentialMatches.empty()) {
         return ExprError();
      }

      auto *FirstMatch = PotentialMatches.front();

      std::string MatchName;
      if (FirstMatch->getModule() != Ident->getDeclCtx()->getDeclModule()) {
         MatchName = FirstMatch->getJoinedName('.', false, false, true);
      }
      else {
         MatchName = FirstMatch->getDeclName().toString();
      }

      SourceLocation Loc = FirstMatch->getSourceLoc();
      if (auto *Exp = compilerInstance->getFileMgr().getMacroExpansionLoc(Loc)) {
         Loc = Exp->ExpandedFrom;
      }

      diagnose(note_generic_note, "did you mean '" + MatchName + "'?", Loc);

      auto *FakeLookupRes = new(Context) MultiLevelLookupResult;
      FakeLookupRes->addResult(PotentialMatches);

      LookupResult = FakeLookupRes;
   }

   // If we found several declarations, check if they should be considered an
   // overload set.
   bool IsOverloadSet = false;
   llvm::SetVector<NamedDecl*> Overloads;
   llvm::SmallPtrSet<NamedDecl*, 4> CheckedDecls;

   for (auto* ND : LookupResult->allDecls()) {
      if (!CheckedDecls.insert(ND).second) {
         continue;
      }

      // Never include protocol default implementations in the lookup result.
      if (ND->isProtocolDefaultImpl()) {
         continue;
      }

      bool done = false;
      switch (ND->getKind()) {
      case Decl::LocalVarDeclID:
      case Decl::FuncArgDeclID:
      case Decl::GlobalVarDeclID: {
         IsOverloadSet = false;
         done = true;
         break;
      }
      case Decl::FunctionDeclID:
      case Decl::MethodDeclID:
      case Decl::InitDeclID: {
         if (QC.PrepareDeclInterface(ND)) {
            continue;
         }

         auto* Inst = checkFunctionReference(Ident, cast<CallableDecl>(ND),
                                             TemplateArgs);

         if (!Inst) {
            continue;
         }

         IsOverloadSet = true;
         ND = Inst;

         break;
      }
      case Decl::SubscriptDeclID: {
         if (QC.PrepareDeclInterface(ND)) {
            continue;
         }

         auto* subscriptDecl = cast<SubscriptDecl>(ND);
         if (auto* getter = subscriptDecl->getGetterMethod()) {
            auto* Inst = checkFunctionReference(Ident, getter, TemplateArgs);
            if (Inst) {
               Overloads.insert(Inst);
            }
         }
         if (auto* setter = subscriptDecl->getSetterMethod()) {
            auto* Inst = checkFunctionReference(Ident, setter, TemplateArgs);
            if (Inst) {
               Overloads.insert(Inst);
            }
         }

         IsOverloadSet = true;
         continue;
      }
      case Decl::AliasDeclID: {
         if (QC.PrepareDeclInterface(ND)) {
            continue;
         }

         auto* Inst = checkAliasReference(
            Ident, cast<AliasDecl>(ND), TemplateArgs);

         if (!Inst) {
            if (Ident->needsInstantiation() && Ident->getExprType()) {
               return Ident;
            }

            continue;
         }

         IsOverloadSet = true;
         ND = Inst;

         break;
      }
      case Decl::StructDeclID:
      case Decl::EnumDeclID:
      case Decl::ClassDeclID:
      case Decl::ProtocolDeclID: {
         auto* Inst = checkRecordReference(
            Ident, cast<RecordDecl>(ND), TemplateArgs);

         if (!Inst) {
            if (Ident->needsInstantiation() && Ident->getExprType()) {
               return Ident;
            }

            continue;
         }

         ND = Inst;
         break;
      }
      default:
         break;
      }

      if (done) {
         Overloads.insert(ND);
         break;
      }

      // Check if this is an invalid template reference.
      if (ND->isTemplate() && !Ident->allowIncompleteTemplateArgs()
          && !Ident->needsInstantiation()) {
         TemplateArgList Args(*this, ND, TemplateArgs, Ident->getSourceLoc());

         auto CompRes = Args.checkCompatibility();
         assert(!CompRes && "should have been instantiated!");

         diagnoseTemplateArgErrors(ND, Ident, Args, TemplateArgs, CompRes);
         continue;
      }

      Overloads.insert(ND);
   }

   if (Overloads.empty()) {
      if (Ident->shouldIssueDiag()) {
         auto diagnosed = false;
         for (auto *ND : CheckedDecls) {
            if (ND->isTemplate()) {
               TemplateArgList ArgList(*this, ND, TemplateArgs,
                                       Ident->getSourceLoc());

               auto CompRes = ArgList.checkCompatibility();
               if (CompRes) {
                  continue;
               }

               diagnosed = true;
               diagnoseTemplateArgErrors(ND, Ident, ArgList, TemplateArgs,
                                         CompRes);
            }
            else if (!TemplateArgs.empty()) {
               diagnosed = true;
               diagnose(err_not_a_template, ND, ND->getDeclName(),
                        Ident->getSourceRange());
            }
         }

         if (!diagnosed) {
            diagnoseIdentifierNotFound(*this, Ident, DeclName,
                                       LookupCtx, ParentTypeNoRef);
         }
      }

      return ExprError();
   }

   if (IsOverloadSet && !Ident->allowOverloadRef()) {
      IsOverloadSet = false;

      if (Overloads.size() > 1) {
         auto* firstOverload = Overloads.front();
         switch (firstOverload->getKind()) {
         case Decl::StructDeclID:
         case Decl::EnumDeclID:
         case Decl::ClassDeclID:
         case Decl::ProtocolDeclID:
         case Decl::LocalVarDeclID:
         case Decl::FuncArgDeclID:
         case Decl::GlobalVarDeclID: {
            // Okay, just use the closest one to the current context.
            break;
         }
         default:
            diagnose(Ident, err_ambiguous_reference, DeclName,
                     Ident->getSourceRange());

            for (auto* ND : LookupResult->allDecls()) {
               diagnose(note_candidate_here, ND->getSourceLoc());
            }

            break;
         }
      }
   }

   bool IsMemberRef = false;
   if (IsOverloadSet && Overloads.size() > 1) {
      bool foundTemplate = false;
      for (auto* ND : Overloads) {
         foundTemplate |= ND->isTemplate();

         switch (ND->getKind()) {
         case Decl::AssociatedTypeDeclID: {
            IsMemberRef |= Ident->getParentExpr() != nullptr;
            break;
         }
         case Decl::FunctionDeclID:
            IsMemberRef |= cast<FunctionDecl>(ND)->isOperator();
            break;
         case Decl::FieldDeclID:
            if (ND->isStatic()) {
               break;
            }

            LLVM_FALLTHROUGH;
         case Decl::PropDeclID:
         case Decl::MethodDeclID:
         case Decl::InitDeclID:
         case Decl::DeinitDeclID:
            if (!checkImplicitSelf(*this, ND, Ident)) {
               return ExprError();
            }

            IsMemberRef = true;
            break;
         default:
            break;
         }
      }

      Expression* parentExpr = IsMemberRef ? Ident->getParentExpr() : nullptr;
      auto* ResultExpr
          = OverloadedDeclRefExpr::Create(Context, Overloads.getArrayRef(),
                                          Ident->getSourceRange(), parentExpr);

      if (foundTemplate) {
         ResultExpr->setTemplateArgs(ArgExpr);
      }

      ResultExpr->setEllipsisLoc(Ident->getEllipsisLoc());
      return ResultExpr;
   }

   // We found a single declaration.
   NamedDecl* FoundDecl = Overloads.front();

   // If we had to leave a lambda body to get to this declaration, this will
   // be that lambdas scope.
   LambdaScope* lambdaScope = LookupResult->front().LS;

   // Opt out if we already encountered an error.
   if (FoundDecl->isInvalid()) {
      Ident->setIsInvalid(true);
      return ExprError();
   }

   if (auto* Var = dyn_cast<VarDecl>(FoundDecl)) {
      if (lambdaScope) {
         Var->setCaptured(true);

         Ident->setIsCapture(true);
         Ident->setCaptureIndex(
             lambdaScope->getLambdaExpr()->addCapture(Context, Var));

         checkCapture(*this, Ident);
      }
   }

   switch (FoundDecl->getKind()) {
   case Decl::StructDeclID:
   case Decl::EnumDeclID:
   case Decl::ClassDeclID:
   case Decl::ProtocolDeclID: {
      auto* R = cast<RecordDecl>(FoundDecl);
      if (R->isTemplate()) {
         if (TemplateArgs.empty()) {
            Ident->setExprType(Context.getMetaType(Context.getRecordType(R)));
         }
         else {
            TemplateArgList ArgList(*this, R, TemplateArgs, Ident->getSourceLoc());
            assert(ArgList.checkCompatibility() && "can this happen?");

            Ident->setNeedsInstantiation(true);
            Ident->setExprType(
                Context.getMetaType(Context.getDependentRecordType(
                    R, FinalTemplateArgumentList::Create(Context, ArgList))));
         }

         return Ident;
      }

      break;
   }
   case Decl::AssociatedTypeDeclID: {
      IsMemberRef = Ident->getParentExpr() != nullptr;
      break;
   }
   case Decl::FuncArgDeclID: {
      auto* arg = cast<FuncArgDecl>(FoundDecl);
      if (arg->isVariadic() && !Ident->allowVariadicRef()) {
         diagnose(Ident, err_generic_error,
                  "variadic function argument can only be referenced in a "
                  "for... statement",
                  Ident->getSourceRange());
      }

      break;
   }
   case Decl::TemplateParamDeclID: {
      auto* param = cast<TemplateParamDecl>(FoundDecl);
      if (param->isVariadic() && !Ident->allowVariadicRef()) {
         diagnose(Ident, err_generic_error,
                  "variadic template parameter can only be referenced in a "
                  "for... statement",
                  Ident->getSourceRange());
      }

      break;
   }
   case Decl::FunctionDeclID:
      IsMemberRef = cast<FunctionDecl>(FoundDecl)->isOperator() && ParentType;
      break;
   case Decl::FieldDeclID: {
      auto* field = cast<FieldDecl>(FoundDecl);
      if (field->isVariadic() && !Ident->allowVariadicRef()) {
         diagnose(Ident, err_generic_error,
                  "variadic field can only be referenced in a for... statement",
                  Ident->getSourceRange());
      }

      LLVM_FALLTHROUGH;
   }
   case Decl::PropDeclID:
   case Decl::InitDeclID:
   case Decl::DeinitDeclID:
      if (!checkImplicitSelf(*this, FoundDecl, Ident)) {
         return ExprError();
      }

      IsMemberRef = true;
      break;
   case Decl::MethodDeclID: {
      if (!checkImplicitSelf(*this, FoundDecl, Ident)) {
         return ExprError();
      }

      if (!Ident->isCalled() && !FoundDecl->isStatic()
      && !Ident->getParentExpr()->getExprType()->isMetaType()) {
         if (!ParentType->isReferenceType()) {
            diagnose(
               Ident, err_generic_error,
               "cannot apply 'self' function reference to temporary value",
               Ident->getSourceRange());
         }

         // Transform the method reference into a lambda.
         return CreatePartiallyAppliedMethod(*this, Ident,
                                             cast<MethodDecl>(FoundDecl));
      }

      IsMemberRef = true;
      break;
   }
   case Decl::EnumCaseDeclID:
      IsMemberRef = false;
      break;
   case Decl::AliasDeclID: {
      auto* Alias = cast<AliasDecl>(FoundDecl);
      if (isInReflectModule(Alias) && !Ident->allowIncompleteTemplateArgs()) {
         auto result = HandleReflectionAlias(Alias, Ident);
         if (Ident->isMagicArgumentValue()) {
            Ident->setAlias(Alias);
         }

         return result;
      }
      if (isInBuiltinModule(Alias) && !Ident->allowIncompleteTemplateArgs()) {
         return HandleBuiltinAlias(Alias, Ident, TemplateArgs);
      }

      break;
   }
   default:
      break;
   }

   Expression* NewExpr;
   if (IsMemberRef) {
      auto* MemRef = MemberRefExpr::Create(Context, Ident->getParentExpr(),
                                           FoundDecl, Ident->getSourceRange());

      MemRef->setCalled(Ident->isCalled());
      NewExpr = MemRef;
   }
   else {
      auto* DeclRef
          = DeclRefExpr::Create(Context, FoundDecl, Ident->getSourceRange());

      DeclRef->setAllowModuleRef(Ident->allowNamespaceRef());

      if (Ident->isCapture()) {
         DeclRef->setCaptureIndex(Ident->getCaptureIndex());
      }
      if (FoundDecl->isTemplate()) {
         DeclRef->setTemplateArgs(ArgExpr);
      }

      NewExpr = DeclRef;
   }

   NewExpr->setEllipsisLoc(Ident->getEllipsisLoc());
   NewExpr->setExprType(Ident->getExprType());
   NewExpr->setIsLHSOfAssignment(Ident->isLHSOfAssignment());

   return visitExpr(NewExpr);
}

ExprResult SemaPass::visitDeclRefExpr(DeclRefExpr* Expr)
{
   if (Expr->getExprType()) {
      return Expr;
   }

   auto* ND = Expr->getDecl();
   checkAccessibility(ND, Expr);

   QualType ResultType;
   switch (ND->getKind()) {
   case Decl::AliasDeclID: {
      if (QC.PrepareDeclInterface(ND)) {
         return ExprError();
      }

      if (cast<AliasDecl>(ND)->isVariadicForDecl()) {
         Expr->setNeedsInstantiation(true);
      }

      ResultType = cast<AliasDecl>(ND)->getType();
      if (ResultType->isMetaType()) {
         ResultType
             = Context.getMetaType(Context.getTypedefType(cast<AliasDecl>(ND)));
      }

      break;
   }
   case Decl::AssociatedTypeDeclID: {
      if (QC.PrepareDeclInterface(ND)) {
         return ExprError();
      }

      ResultType = Context.getMetaType(
          Context.getAssociatedType(cast<AssociatedTypeDecl>(ND)));

      break;
   }
   case Decl::TemplateParamDeclID: {
      auto* Param = cast<TemplateParamDecl>(ND);
      if (QC.PrepareDeclInterface(Param)) {
         return ExprError();
      }

      if (Param->isTypeName()) {
         ResultType = Context.getMetaType(Context.getTemplateParamType(Param));
      }
      else {
         ResultType = Param->getValueType();
      }

      if (Param->isVariadic()) {
         Expr->setContainsUnexpandedParameterPack(true);
      }

      Expr->setContainsGenericParam(true);
      Expr->setNeedsInstantiation(true);

      break;
   }
   case Decl::ClassDeclID:
   case Decl::StructDeclID:
   case Decl::EnumDeclID:
   case Decl::ProtocolDeclID:
      ResultType
          = Context.getMetaType(Context.getRecordType(cast<RecordDecl>(ND)));
      break;
   case Decl::LocalVarDeclID:
   case Decl::GlobalVarDeclID:
   case Decl::FieldDeclID: {
      if (QC.PrepareDeclInterface(ND)) {
         return ExprError();
      }
      if (ND->isTemplateOrInTemplate()) {
         Expr->setNeedsInstantiation(true);
      }

      auto* Var = cast<VarDecl>(ND);
      if (isa<GlobalVarDecl>(ND)) {
         if (QC.PrepareDeclInterface(Var)) {
            return ExprError();
         }
      }
      else if (auto* localVar = dyn_cast<LocalVarDecl>(ND)) {
         if (localVar->isVariadicForDecl()) {
            Expr->setNeedsInstantiation(true);
         }
      }

      ResultType = Var->getType();
      if (!ResultType->isReferenceType()) {
         if (Var->isConst() && (Var->getValue() || Var->isSynthesized())) {
            ResultType = Context.getReferenceType(ResultType);
         }
         else {
            ResultType = Context.getMutableReferenceType(ResultType);
         }
      }

      break;
   }
   case Decl::FuncArgDeclID: {
      if (QC.PrepareDeclInterface(ND)) {
         return ExprError();
      }

      auto* Arg = cast<FuncArgDecl>(ND);
      if (Arg->isVariadicArgPackExpansion()) {
         Expr->setContainsUnexpandedParameterPack(true);
      }

      ResultType = Arg->getType();
      if (!ResultType->isReferenceType()
      && !Arg->hasAttribute<AutoClosureAttr>()) {
         switch (Arg->getConvention()) {
         case ArgumentConvention::MutableRef:
         case ArgumentConvention::Owned:
            ResultType = Context.getMutableReferenceType(ResultType);
            break;
         default:
            ResultType = Context.getReferenceType(ResultType);
            break;
         }
      }

      break;
   }
   case Decl::EnumCaseDeclID: {
      ResultType = Context.getRecordType(ND->getRecord());
      break;
   }
   case Decl::FunctionDeclID:
   case Decl::MethodDeclID:
   case Decl::InitDeclID:
   case Decl::DeinitDeclID: {
      if (QC.PrepareDeclInterface(ND)) {
         return ExprError();
      }

      ResultType = cast<CallableDecl>(ND)->getFunctionType();
      break;
   }
   case Decl::NamespaceDeclID:
   case Decl::ImportDeclID:
   case Decl::ModuleDeclID:
      if (!Expr->allowModuleRef()) {
         diagnose(Expr, err_generic_error, "unexpected namespace reference",
                  Expr->getSourceRange());
      }

      ResultType = ErrorTy;
      break;
   default:
      llvm_unreachable("should be a member decl!");
   }

   Expr->setExprType(ResultType);
   return Expr;
}

ExprResult SemaPass::visitMemberRefExpr(MemberRefExpr* Expr)
{
   if (Expr->getExprType()) {
      return Expr;
   }

   auto* PE = Expr->getParentExpr();
   auto ParentRes = visitExpr(Expr, PE);
   if (!ParentRes) {
      return ExprError();
   }

   PE = ParentRes.get();
   Expr->setParentExpr(PE);

   auto* ND = Expr->getMemberDecl();
   if (QC.PrepareDeclInterface(ND)) {
      return ExprError();
   }

   if (!isa<FieldDecl>(ND) && !isa<PropDecl>(ND)) {
      checkAccessibility(ND, Expr);
   }

   QualType ResultType;
   switch (ND->getKind()) {
   case Decl::AssociatedTypeDeclID: {
      QualType parentTy
          = PE->getExprType()->removeReference()->removeMetaType();
      if (!parentTy->isTemplateParamType() && !parentTy->isAssociatedType()) {
         parentTy = QualType();
      }

      ResultType = Context.getMetaType(
          Context.getAssociatedType(cast<AssociatedTypeDecl>(ND), parentTy));

      break;
   }
   case Decl::PropDeclID: {
      auto* P = cast<PropDecl>(ND);
      ResultType = HandlePropAccess(Expr, P);

      break;
   }
   case Decl::FieldDeclID: {
      auto* F = cast<FieldDecl>(ND);
      ResultType = HandleFieldAccess(Expr, F);

      break;
   }
   case Decl::MethodDeclID:
   case Decl::InitDeclID:
   case Decl::DeinitDeclID:
   case Decl::FunctionDeclID: {
      auto* FTy = cast<CallableDecl>(ND)->getFunctionType();
      if (!Expr->isCalled() && !PE->getExprType()->isMetaType()) {
         if (!PE->isLValue()) {
            diagnose(
                Expr, err_generic_error,
                "cannot apply 'self' function reference to temporary value",
                Expr->getSourceRange());
         }
         else if (auto* IE = dyn_cast<DeclRefExpr>(PE)) {
            if (auto *VD = dyn_cast<VarDecl>(IE->getDecl())) {
               VD->setCaptured(true);
            }
         }
         else if (auto* SE = dyn_cast<SelfExpr>(PE)) {
            SE->setCaptureIndex(0);
            getCurrentFun()->getArgs().front()->setCaptured(true);
         }

         // Function does not take 'self' argument.
         if ((ND->isStatic() && PE->getExprType()->isMetaType())
         || (!ND->isStatic() && !PE->getExprType()->isMetaType())) {
            FTy = Context.getLambdaType(
               FTy->getReturnType(), FTy->getParamTypes().drop_front(1),
               FTy->getParamInfo().drop_front(1), FTy->getRawFlags());
         }
      }

      ResultType = FTy;
      break;
   }
   default:
      llvm_unreachable("not a member decl!");
   }

   if (!ResultType) {
      Expr->setIsInvalid(true);
      return ExprError();
   }

   if (auto* AT = ResultType->asAssociatedType()) {
      QualType Outer = PE->getExprType()->removeReference();
      if (Outer->containsAssociatedType() || Outer->containsTemplateParamType()) {
         ResultType = Context.getAssociatedType(AT->getDecl(), Outer);
      }
      else {
         ResultType = CreateConcreteTypeFromAssociatedType(AT, Outer, ResultType);
      }
   }

   Expr->setExprType(ResultType);
   return Expr;
}

ExprResult SemaPass::visitOverloadedDeclRefExpr(OverloadedDeclRefExpr* Expr)
{
   return Expr;
}

ExprResult SemaPass::visitBuiltinIdentExpr(BuiltinIdentExpr* Ident)
{
   cdot::Type* builtinType;

   switch (Ident->getIdentifierKind()) {
   default:
      llvm_unreachable("bad builtin ident!");
   case BuiltinIdentifier::defaultValue: {
      if (Ident->getExprType()) {
         return Ident;
      }

      QualType ctx = Ident->getContextualType();
      if (!ctx) {
         diagnose(Ident, err_generic_error,
                  "cannot use 'default' expresssion without a contextual type",
                  Ident->getSourceRange());

         return Ident;
      }
      if (!hasDefaultValue(ctx)) {
         diagnose(Ident, err_generic_error,
                  "type '" + ctx.toDiagString() + "' does not have a default value",
                  Ident->getSourceRange());
      }
      else {
         // Make sure the default initializers are instantiated.
         visitSpecificType<RecordType>(
             [&](RecordType* RT) {
                if (auto* S = dyn_cast<StructDecl>(RT->getRecord())) {
                   auto* Init = S->getParameterlessConstructor();
                   if (!Init)
                      return true;

                   auto* Inst = maybeInstantiateTemplateMember(S, Init);
                   if (Inst) {
                      maybeInstantiateMemberFunction(Inst, Ident);
                   }
                }

                return true;
             },
             ctx);
      }

      Ident->setExprType(ctx);
      Ident->setIsTypeDependent(ctx->isDependentType());

      return Ident;
   }
   case BuiltinIdentifier::NULLPTR: {
      if (Ident->getContextualType().isNull()) {
         diagnose(Ident, err_requires_contextual_type, Ident->getSourceLoc(),
                  1 /*__nullptr*/);

         return ExprError();
      }

      if (Ident->getContextualType()->isRefcounted()
          || Ident->getContextualType()->isThinFunctionTy()
          || Ident->getContextualType()->isPointerType())
         builtinType = *Ident->getContextualType();
      else
         builtinType = Ident->getContextualType()->getPointerTo(Context);

      break;
   }
   case BuiltinIdentifier::FUNC:
   case BuiltinIdentifier::MANGLED_FUNC: {
      auto C = getFuncScope()->getCallableDecl();
      llvm::StringRef str
          = Ident->getIdentifierKind() == BuiltinIdentifier ::FUNC
                ? C->getName()
                : ILGen->getFunc(C)->getName();

      return StringLiteral::Create(Context, Ident->getSourceRange(), str.str());
   }
   case BuiltinIdentifier::FLOAT_QNAN:
   case BuiltinIdentifier::FLOAT_SNAN:
      builtinType = Context.getFloatTy();
      break;
   case BuiltinIdentifier::DOUBLE_QNAN:
   case BuiltinIdentifier::DOUBLE_SNAN:
      builtinType = Context.getDoubleTy();
      break;
   case BuiltinIdentifier::__ctfe:
      builtinType = Context.getBoolTy();
      break;
   case BuiltinIdentifier::__builtin_void:
      builtinType = Context.getMetaType(Context.getVoidType());
      break;
   }

   Ident->setExprType(builtinType);
   return Ident;
}

template<class ExprTy>
static ExprResult checkIfSelfCapture(SemaPass& SP, ExprTy* E)
{
   LambdaExpr* LE = nullptr;
   for (auto S = SP.getCurrentScope(); S; S = S->getEnclosingScope()) {
      switch (S->getTypeID()) {
      case Scope::LambdaScopeID:
         LE = cast<LambdaScope>(S)->getLambdaExpr();
         break;
      case Scope::FunctionScopeID:
      case Scope::MethodScopeID: {
         auto Fn = cast<FunctionScope>(S)->getCallableDecl();
         if (!isa<MethodDecl>(Fn)
             || (cast<MethodDecl>(Fn)->isStatic() && !isa<InitDecl>(Fn))) {
            SP.diagnose(E, err_self_outside_method, E->getSourceLoc(),
                        isa<SuperExpr>(E));

            return ExprError();
         }

         auto R = Fn->getRecord();
         if (R->isInUnboundedTemplate()) {
            E->setIsTypeDependent(true);
         }

         if (SP.QC.DeclareSelfAlias(R)) {
            E->setIsInvalid(true);
            return ExprError();
         }

         QualType SelfTy;
         if (R->isProtocol()) {
            auto* SelfDecl
                = R->lookupSingle<AssociatedTypeDecl>(SP.getIdentifier("Self"));

            assert(SelfDecl && "no Self associated type!");
            SelfTy = SP.Context.getAssociatedType(SelfDecl);

            E->setContainsAssociatedType(true);
         }
         else {
            AliasDecl* SelfDecl = SP.getAssociatedTypeImpl(R, "Self");
            assert(SelfDecl && "no Self associated type!");
            SelfTy = SP.Context.getTypedefType(SelfDecl);
         }

         if (Fn->hasMutableSelf() || R->isClass()) {
            SelfTy = SP.getContext().getMutableReferenceType(SelfTy);
         }
         else {
            SelfTy = SP.getContext().getReferenceType(SelfTy);
         }

         E->setExprType(SelfTy);

         if (!isa<InitDecl>(Fn) && !isa<DeinitDecl>(Fn)) {
            auto Arg = Fn->getArgs().front();
            E->setSelfArg(Arg);

            if (LE) {
               Arg->setCaptured(true);
               E->setCaptureIndex(LE->addCapture(SP.getContext(), Arg));
            }
         }

         return E;
      }
      default:
         break;
      }
   }

   SP.diagnose(E, err_self_outside_method, E->getSourceLoc(),
               isa<SuperExpr>(E));

   return ExprError();
}

ExprResult SemaPass::visitSuperExpr(SuperExpr* Expr)
{
   if (!checkIfSelfCapture(*this, Expr))
      return ExprError();

   auto R = getCurrentRecordCtx();
   auto currentCl = dyn_cast<ClassDecl>(R);
   if (!currentCl || currentCl->getParentClass() == nullptr) {
      diagnose(Expr, err_super_without_base, Expr->getSourceLoc(),
               getCurrentRecordCtx()->getName());

      return ExprError();
   }

   QualType SelfTy = Context.getRecordType(currentCl->getParentClass());
   if (Expr->getExprType()->isMutableReferenceType())
      SelfTy = Context.getMutableReferenceType(SelfTy);
   else
      SelfTy = Context.getReferenceType(SelfTy);

   Expr->setExprType(SelfTy);
   return Expr;
}

ExprResult SemaPass::visitSelfExpr(SelfExpr* Expr)
{
   if (Expr->isUppercase()) {
      auto* R = getCurrentRecordCtx();
      if (!R) {
         diagnose(Expr, err_self_outside_method, Expr->getSourceLoc(), 2);
         return ExprError();
      }

      if (QC.DeclareSelfAlias(R)) {
         return ExprError();
      }

      if (R->isProtocol()) {
         auto* SelfDecl
             = R->lookupSingle<AssociatedTypeDecl>(getIdentifier("Self"));

         assert(SelfDecl && "no Self associated type!");
         Expr->setExprType(
             Context.getMetaType(Context.getAssociatedType(SelfDecl)));

         Expr->setContainsAssociatedType(true);
      }
      else {
         AliasDecl* SelfDecl = getAssociatedTypeImpl(R, "Self");
         assert(SelfDecl && "no Self associated type!");
         Expr->setExprType(
             Context.getMetaType(Context.getTypedefType(SelfDecl)));
      }

      return Expr;
   }

   if (!checkIfSelfCapture(*this, Expr)) {
      return ExprError();
   }

   return Expr;
}

namespace {

class PlaceholderExprBuilder {
   std::vector<std::unique_ptr<BuiltinExpr>> Exprs;

public:
   PlaceholderExprBuilder() = default;

   Expression* CreateExpr(QualType Ty)
   {
      auto NewExpr = std::make_unique<BuiltinExpr>(BuiltinExpr::CreateTemp(Ty));
      auto Ptr = NewExpr.get();

      Exprs.emplace_back(move(NewExpr));
      return Ptr;
   }
};

} // anonymous namespace

CallableDecl*
SemaPass::checkFunctionReference(IdentifierRefExpr* E, CallableDecl* CD,
                                 ArrayRef<Expression*> TemplateArgs)
{
   if (!CD->isTemplate()) {
      if (!TemplateArgs.empty()) {
         return nullptr;
      }

      return CD;
   }

   if (E->allowIncompleteTemplateArgs() && TemplateArgs.empty()) {
      return CD;
   }

   TemplateArgList ArgList(*this, CD, TemplateArgs, E->getSourceLoc());
   auto CompRes = ArgList.checkCompatibility();
   if (!CompRes) {
      return nullptr;
   }

   if (ArgList.isStillDependent()) {
      E->setNeedsInstantiation(true);
      return CD;
   }

   auto* FinalList = FinalTemplateArgumentList::Create(Context, ArgList);
   return Instantiator->InstantiateCallable(CD, FinalList, E->getSourceLoc());
}

RecordDecl* SemaPass::checkRecordReference(IdentifierRefExpr* E, RecordDecl* R,
                                           ArrayRef<Expression*> TemplateArgs)
{
   if (!R->isTemplate()) {
      if (!TemplateArgs.empty()) {
         return nullptr;
      }

      return R;
   }

   if (E->allowIncompleteTemplateArgs() && TemplateArgs.empty()) {
      return R;
   }

   if (QC.PrepareTemplateParameters(R)) {
      return nullptr;
   }

   for (auto* ArgExpr : TemplateArgs) {
      if (ArgExpr->getExprType()->containsAssociatedType()
      || ArgExpr->getExprType()->containsTemplateParamType()) {
         E->setNeedsInstantiation(true);
      }
   }

   TemplateArgList ArgList(*this, R, TemplateArgs, E->getSourceLoc());

   auto CompRes = ArgList.checkCompatibility();
   if (!CompRes) {
      return nullptr;
   }

   if (ArgList.isStillDependent()) {
      E->setNeedsInstantiation(true);
      return R;
   }

   auto* FinalList = FinalTemplateArgumentList::Create(Context, ArgList);
   return Instantiator->InstantiateRecord(R, FinalList, E->getSourceLoc());
}

AliasDecl* SemaPass::checkAliasReference(IdentifierRefExpr* E, AliasDecl* Alias,
                                         ArrayRef<Expression*> TemplateArgs)
{
   if (!Alias->isTemplate()) {
      if (!TemplateArgs.empty()) {
         return nullptr;
      }

      return Alias;
   }

   TemplateArgList ArgList(*this, Alias, TemplateArgs, E->getSourceLoc());
   if (ArgList.isStillDependent()) {
      E->setNeedsInstantiation(true);

      if (QC.PrepareDeclInterface(Alias)) {
         E->setIsInvalid(true);
         return nullptr;
      }

      if (isInBuiltinModule(Alias)) {
         return Alias;
      }

      if (Alias->getType()->isMetaType()) {
         E->setExprType(Context.getMetaType(Context.getDependentTypedefType(
             Alias, FinalTemplateArgumentList::Create(Context, ArgList))));
      }
      else {
         E->setExprType(Alias->getType());
      }

      return nullptr;
   }

   auto CompRes = ArgList.checkCompatibility();
   if (!CompRes) {
      if (E->allowIncompleteTemplateArgs() && TemplateArgs.empty()) {
         return Alias;
      }

      return nullptr;
   }

   auto* FinalList = FinalTemplateArgumentList::Create(Context, ArgList);
   return Instantiator->InstantiateAlias(Alias, FinalList, E->getSourceLoc());
}

ExprResult SemaPass::checkNamespaceRef(MacroExpansionExpr* Expr)
{
   Expression* ParentExpr = Expr->getParentExpr();
   if (!ParentExpr)
      return Expr;

   if (auto* Ident = dyn_cast<IdentifierRefExpr>(ParentExpr)) {
      Ident->setAllowNamespaceRef(true);
   }

   auto SemaRes = visitExpr(ParentExpr);
   if (!SemaRes) {
      Expr->copyStatusFlags(ParentExpr);
      return ExprError();
   }

   ParentExpr = SemaRes.get();
   Expr->setParentExpr(ParentExpr);
   Expr->copyStatusFlags(ParentExpr);

   return Expr;
}

StmtResult SemaPass::checkNamespaceRef(MacroExpansionStmt* Stmt)
{
   Expression* ParentExpr = Stmt->getParentExpr();
   if (!ParentExpr)
      return Stmt;

   if (auto* Ident = dyn_cast<IdentifierRefExpr>(ParentExpr)) {
      Ident->setAllowNamespaceRef(true);
   }

   auto SemaRes = visitExpr(ParentExpr);
   if (!SemaRes) {
      Stmt->copyStatusFlags(ParentExpr);
      return StmtError();
   }

   ParentExpr = SemaRes.get();
   Stmt->setParentExpr(ParentExpr);
   Stmt->copyStatusFlags(ParentExpr);

   return Stmt;
}

DeclResult SemaPass::checkNamespaceRef(MacroExpansionDecl* D)
{
   Expression* ParentExpr = D->getParentExpr();
   if (!ParentExpr)
      return D;

   if (auto* Ident = dyn_cast<IdentifierRefExpr>(ParentExpr)) {
      Ident->setAllowNamespaceRef(true);
   }

   auto SemaRes = visitExpr(ParentExpr);
   if (!SemaRes) {
      D->copyStatusFlags(ParentExpr);
      return DeclError();
   }

   ParentExpr = SemaRes.get();
   D->setParentExpr(ParentExpr);
   D->copyStatusFlags(ParentExpr);

   return D;
}

StmtOrDecl SemaPass::checkMacroCommon(StmtOrDecl SOD, DeclarationName MacroName,
                                      DeclContext& Ctx,
                                      MacroDecl::Delimiter Delim,
                                      llvm::ArrayRef<lex::Token> Tokens,
                                      unsigned Kind)
{
   auto Macro = QC.LookupSingleAs<MacroDecl>(&Ctx, MacroName);
   parse::ParseResult Result;

   if (!Macro) {
      diagnoseMemberNotFound(&Ctx, SOD, MacroName, err_macro_does_not_exist);

      // Look for declarations with similar names that the user might have meant.
      auto PotentialMatches = FindSimilarlyNamedDecl(*this, MacroName, &Ctx,
                                                     QualType());

      if (PotentialMatches.empty()) {
         return StmtOrDecl();
      }

      auto *FirstMatch = PotentialMatches.front();

      std::string MatchName;
      if (FirstMatch->getModule() != Ctx.getDeclModule()) {
         MatchName = FirstMatch->getJoinedName('.', false, false, true);
      }
      else {
         MatchName = FirstMatch->getDeclName().toString();
      }

      SourceLocation Loc = FirstMatch->getSourceLoc();
      if (auto *Exp = compilerInstance->getFileMgr().getMacroExpansionLoc(Loc)) {
         Loc = Exp->ExpandedFrom;
      }

      diagnose(note_generic_note, "did you mean '" + MatchName + "'?", Loc);
      return StmtOrDecl();
   }
   else if (Delim != Macro->getDelim()) {
      llvm::StringRef ExpectedDelim;
      switch (Macro->getDelim()) {
      case MacroDecl::Paren:
         ExpectedDelim = "()";
         break;
      case MacroDecl::Brace:
         ExpectedDelim = "{}";
         break;
      case MacroDecl::Square:
         ExpectedDelim = "[]";
         break;
      }

      diagnose(SOD, err_macro_expects_delim, SOD.getSourceRange(), MacroName,
               ExpectedDelim);
   }

   if (Macro) {
      Result = parse::Parser::expandMacro(*this, Macro, SOD, Tokens,
                                          (parse::Parser::ExpansionKind)Kind);
   }

   if (Result.holdsDecl()) {
      return Result.getDecl();
   }
   if (Result.holdsExpr()) {
      return Result.getExpr();
   }
   if (Result.holdsStatement()) {
      return Result.getStatement();
   }

   return nullptr;
}

static QualType ReplaceAssociatedTypes(SemaPass &Sema, QualType T,
                                       NamedDecl *ND, QualType SelfType)
{
   if (ND->isProtocolRequirement() || ND->isProtocolDefaultImpl()) {
      if (auto *AT = T->asAssociatedType()) {
         SelfType = SelfType->removeReference()->removeMetaType();

         if (AT->getDecl()->isSelf()) {
            return SelfType;
         }
         else if (AT->getOuterAT()->isAssociatedType()
         && AT->getOuterAT()->uncheckedAsAssociatedType()->getDecl()->isSelf()) {
            QualType LookupTy;
            if (auto *P = SelfType->asTemplateParamType()) {
               LookupTy = P->getCovariance();
            }
            else if (auto *ATType = SelfType->asAssociatedType()) {
               LookupTy = ATType->getDecl()->getCovariance();
            }
            else {
               LookupTy = SelfType;
            }

            const MultiLevelLookupResult *Result;
            if (Sema.QC.MultiLevelTypeLookup(Result, LookupTy,
                                             AT->getDecl()->getDeclName())) {
               return T;
            }

            if (Result->empty()) {
               return T;
            }

            auto *OwnImpl = dyn_cast<AssociatedTypeDecl>(Result->front().front());
            if (OwnImpl) {
               return Sema.Context.getAssociatedType(OwnImpl);
            }

            return Sema.Context.getAssociatedType(AT->getDecl(), SelfType);;
         }
         else if (SelfType->isTemplateParamType() || SelfType->isAssociatedType()) {
            SelfType = Sema.Context.getAssociatedType(AT->getDecl(), SelfType);
         }
      }
   }

   return T;
}

QualType SemaPass::HandleFieldAccess(MemberRefExpr* Expr, FieldDecl* F)
{
   if (QC.PrepareDeclInterface(F)) {
      return QualType();
   }

   QualType ty = F->getType();
   if (ty->isDependentType()) {
      Expr->setIsTypeDependent(true);
   }

   checkAccessibility(F, Expr);

   bool ParentIsConst = false;
   bool BeingInitialized = false;

   auto *PE = Expr->getParentExpr();
   QualType ParentType = PE->getExprType();

   if (!F->isStatic()) {
      // Field access on class instances returns a mutable reference.
      if (ParentType->isNonMutableReferenceType()) {
         ParentIsConst = !ParentType->removeReference()->isClass();
      }
      else if (!ParentType->isMutableReferenceType()) {
         ParentIsConst = true;
      }

      // If we're in this records initializer, return a mutable reference for
      // now. Later IL passes will ensure that each field is initialized
      // exactly once
      BeingInitialized = false;

      auto* Init = dyn_cast_or_null<InitDecl>(getCurrentFun());
      if (Init) {
         BeingInitialized = Init->getRecord() == F->getRecord();
      }
   }

   ty = ReplaceAssociatedTypes(*this, ty, F, Expr->getParentExpr()->getExprType());

   // If the expression that we're accessing the field on or the field itself
   // is constant, return a constant reference
   if ((F->isConst() && !BeingInitialized) || ParentIsConst) {
      return Context.getReferenceType(ty);
   }
   else {
      return Context.getMutableReferenceType(ty);
   }
}

static bool UseBackingField(RecordDecl *CurDecl, RecordDecl *SelfDecl)
{
   if (CurDecl == SelfDecl)
      return true;

   auto *CurClass = dyn_cast<ClassDecl>(CurDecl);
   auto *SelfClass = dyn_cast<ClassDecl>(SelfDecl);

   if (!CurClass || !SelfClass)
      return false;

   return SelfClass->isBaseClassOf(CurClass);
}

QualType SemaPass::HandlePropAccess(MemberRefExpr* Expr, PropDecl* P)
{
   if (QC.PrepareDeclInterface(P)) {
      return QualType();
   }

   // In an initializer, the name of a synthesized property actually refers
   // to the backing field.
   if (P->isSynthesized()) {
      if (auto *M = dyn_cast_or_null<MethodDecl>(getCurrentFun())) {
         if (UseBackingField(M->getRecord(), P->getRecord())) {
            string backingFieldName = "_";
            backingFieldName += P->getName();

            DeclarationName DN
                = &Context.getIdentifiers().get(backingFieldName);

            auto* F = P->getRecord()->lookupSingle<FieldDecl>(DN);
            if (F) {
               Expr->setMemberDecl(F);
               return HandleFieldAccess(Expr, F);
            }
         }
      }
   }

   QualType ResultTy;
   QualType SelfType;
   bool MutableSelf;

   QualType PropTy = ReplaceAssociatedTypes(*this, P->getType(), P,
                                            Expr->getParentExpr()->getExprType());

   if (Expr->isLHSOfAssignment()) {
      if (!P->hasSetter()) {
         diagnose(Expr, err_prop_does_not_have, Expr->getSourceLoc(),
                  P->getName(), /*setter*/ 1);

         return Context.getVoidType();
      }

      checkAccessibility(P->getSetterMethod(), Expr);

      MutableSelf = P->getSetterMethod()->hasMutableSelf();
      SelfType = P->getSetterMethod()->getSelfType();

      if (!P->isSynthesized()) {
         maybeInstantiateMemberFunction(P->getSetterMethod(), Expr);
      }

      // This path should only be taken when resolving an expression sequence
      // so this type is only used to get the correct `=` overloads; the
      // actual type of the resolved expression will be `Void`
      ResultTy = Context.getMutableReferenceType(PropTy);
   }
   else {
      if (!P->hasGetter()) {
         diagnose(Expr, err_prop_does_not_have, Expr->getSourceLoc(),
                  P->getName(), /*getter*/ 0);

         return PropTy;
      }

      checkAccessibility(P->getGetterMethod(), Expr);

      MutableSelf = P->getGetterMethod()->hasMutableSelf();
      SelfType = P->getGetterMethod()->getSelfType();

      if (!P->isSynthesized()) {
         maybeInstantiateMemberFunction(P->getGetterMethod(), Expr);
      }

      if (P->isReadWrite()) {
         ResultTy = Context.getMutableReferenceType(PropTy);
      }
      else {
         ResultTy = PropTy;
      }
   }

   // If the expression that we're accessing the property on is constant,
   // issue an error if we're trying to use a setter.
   bool ParentIsConst
       = !Expr->getParentExpr()->getExprType()->isMutableReferenceType();

   // Field access in non mutating methods of a class returns a mutable
   // reference.
   if (auto Self = dyn_cast<SelfExpr>(Expr->getParentExpr())) {
      ParentIsConst &= !Self->getExprType()->removeReference()->isClass();
   }

   if (!SelfType->isDependentType() && !ResultTy->isDependentType()) {
      if (MutableSelf && ParentIsConst) {
         diagnose(
             Expr, err_mutating_accessor_on_const, Expr->isLHSOfAssignment(),
             Expr->getParentExpr()->getSourceRange(), Expr->getSourceLoc());
      }
      else {
         Expr->setParentExpr(
             implicitCastIfNecessary(Expr->getParentExpr(), SelfType));
      }
   }
   else {
      Expr->setIsTypeDependent(true);
   }

   return ResultTy;
}

SubscriptDecl* SemaPass::getSubscriptDecl(MethodDecl* AccessorFn)
{
   assert(AccessorFn->isSubscript());

   auto *R = AccessorFn->getRecord();
   SubscriptDecl *Sub = nullptr;
   auto Name = Context.getDeclNameTable().getSubscriptName(
       DeclarationName::SubscriptKind::General);

   for (auto *PossibleImpl : R->lookup(Name)) {
      auto *Impl = cast<SubscriptDecl>(PossibleImpl);
      if (Impl->getGetterMethod() == AccessorFn
      || Impl->getSetterMethod() == AccessorFn) {
         Sub = Impl;
         break;
      }
   }

   if (!Sub) {
      for (auto *Ext : R->getExtensions()) {
         for (auto *PossibleImpl : Ext->lookup(Name)) {
            auto *Impl = cast<SubscriptDecl>(PossibleImpl);
            if (Impl->getGetterMethod() == AccessorFn
            || Impl->getSetterMethod() == AccessorFn) {
               Sub = Impl;
               break;
            }
         }

         if (Sub)
            break;
      }
   }

   assert(Sub && "stray subscript function!");
   return Sub;
}

ExprResult SemaPass::visitTupleMemberExpr(TupleMemberExpr* Expr)
{
   auto ParentExpr = Expr->getParentExpr();
   auto ParentResult = visitExpr(Expr, ParentExpr);
   if (!ParentResult)
      return ExprError();

   ParentExpr = ParentResult.get();
   Expr->setParentExpr(ParentExpr);

   if (ParentExpr->getExprType()->removeReference()->isUnknownAnyType()) {
      Expr->setIsTypeDependent(true);
      Expr->setExprType(UnknownAnyTy);

      return Expr;
   }

   TupleType* tup = ParentExpr->getExprType()->removeReference()->asTupleType();
   if (!tup) {
      diagnose(Expr, err_not_tuple, Expr->getSourceLoc(),
               ParentExpr->getExprType()->removeReference());

      return ExprError();
   }

   if (tup->getArity() <= Expr->getIndex()) {
      diagnose(Expr, diag::err_tuple_arity, Expr->getSourceLoc(),
               Expr->getIndex(), tup->getArity());

      return ExprError();
   }

   // if the expression that we're accessing the property on is constant,
   // return an immutable reference
   bool ParentIsConst
       = !Expr->getParentExpr()->getExprType()->isMutableReferenceType();

   // field access in non mutating methods of a class returns a mutable
   // reference
   if (auto Self = dyn_cast<SelfExpr>(Expr->getParentExpr())) {
      ParentIsConst
          &= !Self->getExprType()->removeReference()->getRecord()->isClass();
   }

   QualType contained = tup->getContainedType(Expr->getIndex());

   if (ParentIsConst) {
      Expr->setExprType(Context.getReferenceType(contained));
   }
   else {
      Expr->setExprType(Context.getMutableReferenceType(contained));
   }

   Expr->setParentExpr(castToRValue(Expr->getParentExpr()));
   return Expr;
}

ExprResult SemaPass::visitSubscriptExpr(SubscriptExpr* Expr)
{
   assert(!Expr->getCallExpr());

   auto ParentExpr = Expr->getParentExpr();
   auto ParentResult = visitExpr(Expr, ParentExpr);
   if (!ParentResult) {
      return ExprError();
   }

   ParentExpr = ParentResult.get();
   Expr->setParentExpr(ParentExpr);

   QualType ParentTy = ParentExpr->getExprType();
   QualType SubscriptedTy = ParentTy->removeReference()->getDesugaredType();

   assert(!SubscriptedTy->isRecordType() && !SubscriptedTy->isExistentialType()
      && "should have been transformed into a call!");

   auto *IndexTy = Context.getRecordType(getInt64Decl());
   for (auto& Idx : Expr->getIndices()) {
      if (!SubscriptedTy->isRecordType()) {
         Idx->setContextualType(IndexTy);
      }

      auto indexResult = visitExpr(Expr, Idx);
      if (!indexResult)
         return ExprError();

      Idx = indexResult.get();
   }

   if (!Expr->hasSingleIndex()) {
      diagnose(Expr, err_subscript_too_many_indices, Expr->getSourceLoc());
   }

   QualType resultType;
   if (PointerType* Ptr = SubscriptedTy->asPointerType()) {
      resultType = Ptr->getPointeeType();
   }
   else if (ArrayType* Arr = SubscriptedTy->asArrayType()) {
      resultType = Arr->getElementType();
   }
   else {
      diagnose(Expr, err_illegal_subscript, Expr->getSourceLoc(),
               SubscriptedTy);

      return ExprError();
   }

   if (!Expr->getIndices().empty()) {
      auto& idx = Expr->getIndices().front();
      auto IdxTy = idx->getExprType()->removeReference();

      // Allow signed and unsigned subscript indices
      if (IdxTy->isIntegerType()) {
         auto IntBits = Context.getTargetInfo().getPointerSizeInBytes() * 8;
         idx = implicitCastIfNecessary(
             idx, Context.getIntegerTy(IntBits, IdxTy->isUnsigned()));
      }
      else {
         idx = implicitCastIfNecessary(idx, IndexTy);
      }
   }

   bool IsMutablePtr
       = ParentExpr->getExprType()->removeReference()->isMutablePointerType();

   bool ParentIsConst
       = !Expr->getParentExpr()->getExprType()->isMutableReferenceType();

   Expr->setParentExpr(castToRValue(Expr->getParentExpr()));

   // if the expression that we're subscripting is constant, return a
   // constant reference
   if (ParentIsConst && !IsMutablePtr) {
      Expr->setExprType(Context.getReferenceType(resultType));
   }
   else {
      Expr->setExprType(Context.getMutableReferenceType(resultType));
   }

   return Expr;
}

ExprResult SemaPass::visitTemplateArgListExpr(TemplateArgListExpr* Expr)
{
   bool Dependent = false;
   for (auto& TA : Expr->getExprs()) {
      auto res = visitExpr(Expr, TA);
      if (!res)
         return ExprError();

      TA = res.get();
      Dependent |= TA->isUnknownAny();
   }

   Expression* ParentExpr = Expr->getParentExpr();
   if (ParentExpr->isSemanticallyChecked()) {
      Expr->setExprType(ParentExpr->getExprType());
      return Expr;
   }

   ExprResult Res;
   switch (ParentExpr->getTypeID()) {
   case Expression::IdentifierRefExprID:
      Res = visitIdentifierRefExpr(cast<IdentifierRefExpr>(ParentExpr), Expr);
      break;
   case Expression::CallExprID:
      Res = visitCallExpr(cast<CallExpr>(ParentExpr), Expr);
      break;
   default:
      diagnose(Expr, err_generic_error,
               "template arguments not allowed "
               "here",
               Expr->getSourceRange());
      break;
   }

   if (!Res) {
      Expr->copyStatusFlags(ParentExpr);
      ParentExpr->setSemanticallyChecked(true);

      return ExprError();
   }

   ParentExpr = Res.get();

   Expr->copyStatusFlags(ParentExpr);
   Expr->setParentExpr(ParentExpr);
   Expr->setExprType(ParentExpr->getExprType());

   ParentExpr->copyStatusFlags(Expr);
   ParentExpr->setSemanticallyChecked(true);

   return Expr;
}

void SemaPass::diagnoseMemberNotFound(DeclContext* Ctx, StmtOrDecl Subject,
                                      DeclarationName memberName,
                                      diag::MessageKind msg, SourceRange SR)
{
   if (!SR.getStart() && !SR.getEnd()) {
      SR = Subject.getSourceRange();
   }

   if (!Ctx) {
      diagnose(Subject, msg, SR, memberName);
   }
   else {
      Ctx = Ctx->lookThroughExtension();
      while (Ctx) {
         switch (Ctx->getDeclKind()) {
         case Decl::StructDeclID:
         case Decl::ClassDeclID:
         case Decl::EnumDeclID:
         case Decl::UnionDeclID:
         case Decl::ProtocolDeclID: {
            auto ND = cast<NamedDecl>(Ctx);
            diagnose(Subject, err_member_not_found, SR, ND, ND->getDeclName(),
                     memberName, false);

            return;
         }
         case Decl::ImportDeclID: {
            auto ND = cast<ImportDecl>(Ctx)->getImportedModule()->getDecl();
            diagnose(Subject, err_member_not_found, SR, ND, ND->getDeclName(),
                     memberName, false);

            return;
         }
         case Decl::ModuleDeclID:
         case Decl::NamespaceDeclID: {
            auto ND = cast<NamedDecl>(Ctx);
            diagnose(Subject, err_member_not_found, SR, ND, ND->getFullName(),
                     memberName, false);

            return;
         }
         default:
            break;
         }

         Ctx = Ctx->getParentCtx()->lookThroughExtension();
      }
   }
}

void SemaPass::diagnoseTemplateArgErrors(
    NamedDecl* Template, Statement* ErrorStmt, TemplateArgList& list,
    llvm::ArrayRef<Expression*> OriginalArgs, TemplateArgListResult& Cand)
{
   diagnose(ErrorStmt, err_incompatible_template_args,
            ErrorStmt->getSourceRange(), Template, Template->getDeclName());

   switch (Cand.ResultKind) {
   case sema::TemplateArgListResultKind::TLR_CouldNotInfer: {
      auto missingParam = reinterpret_cast<TemplateParamDecl*>(Cand.Data1);

      diagnose(note_could_not_infer_template_arg, missingParam->getDeclName(),
               missingParam->getSourceLoc());

      break;
   }
   case sema::TemplateArgListResultKind::TLR_TooManyTemplateArgs: {
      auto neededSize = Template->getTemplateParams().size();
      auto givenSize = OriginalArgs.size();

      diagnose(ErrorStmt, note_too_many_template_args, neededSize, givenSize,
               Template->getSourceLoc());

      break;
   }
   case sema::TemplateArgListResultKind::TLR_IncompatibleArgKind: {
      unsigned diagSelect = unsigned(Cand.Data1);
      unsigned select1 = diagSelect & 0x3u;
      unsigned select2 = (diagSelect >> 2u) & 0x3u;

      auto Param = reinterpret_cast<TemplateParamDecl*>(Cand.Data2);
      diagnose(ErrorStmt, note_template_arg_kind_mismatch, select2, select1,
               Param->getIndex() + 1,
               list.getArgForParam(Param)->getLoc());

      diagnose(ErrorStmt, note_template_parameter_here, Param->getSourceLoc());

      break;
   }
   case sema::TemplateArgListResultKind::TLR_IncompatibleArgVal: {
      auto givenTy = reinterpret_cast<Type*>(Cand.Data1);
      auto Param = reinterpret_cast<TemplateParamDecl*>(Cand.Data2);

      diagnose(ErrorStmt, note_template_arg_type_mismatch,
               Param->getCovariance(), Param->getIndex() + 1, givenTy,
               list.getArgForParam(Param)->getLoc());

      diagnose(ErrorStmt, note_template_parameter_here, Param->getSourceLoc());
      break;
   }
   case sema::TemplateArgListResultKind::TLR_ConflictingInferredArg: {
      auto Param = reinterpret_cast<TemplateParamDecl*>(Cand.Data2);

      if (Param->isTypeName()) {
         auto conflictingTy
             = QualType::getFromOpaquePtr(reinterpret_cast<void*>(Cand.Data1));

         auto templateArg = list.getArgForParam(Param);
         assert(templateArg && "bad diagnostic data");

         string name = Param->getName().str();
         if (templateArg->isVariadic()) {
            name += "[";
            name += std::to_string(templateArg->getVariadicArgs().size() - 1);
            name += "]";

            templateArg = &templateArg->getVariadicArgs().back();
         }

         diagnose(ErrorStmt, note_inferred_template_arg_conflict, 0 /*types*/,
                  templateArg->getType(), conflictingTy, name,
                  templateArg->getLoc());

         diagnose(ErrorStmt, note_template_parameter_here,
                  Param->getSourceLoc());
      }
      else {
         llvm_unreachable("TODO");
      }

      break;
   }
   case sema::TemplateArgListResultKind::TLR_CovarianceError: {
      QualType Given = QualType::getFromOpaquePtr((void*)Cand.Data1);
      auto Param = reinterpret_cast<TemplateParamDecl*>(Cand.Data2);

      auto templateArg = list.getArgForParam(Param);
      assert(templateArg && "bad diagnostic data");

      auto* CovarRec = Param->getCovariance()->getRecord();
      diagnose(ErrorStmt, note_template_arg_covariance,
               isa<ClassDecl>(CovarRec), CovarRec->getDeclName(),
               Param->getDeclName(), Given, templateArg->getLoc());

      diagnose(ErrorStmt, note_template_parameter_here, Param->getSourceLoc());

      break;
   }
   default:
      break;
   }
}

static bool IsTestable(NamedDecl* ND)
{
   if (ND->hasAttribute<TestableAttr>())
      return true;

   auto* DC = ND->getNonTransparentDeclContext()->lookThroughExtension();
   while (DC) {
      DC = DC->lookThroughExtension();
      if (isa<NamedDecl>(DC) && cast<NamedDecl>(DC)->hasAttribute<TestableAttr>()) {
         return true;
      }

      DC = DC->getParentCtx();
   }

   return false;
}

void SemaPass::checkAccessibility(NamedDecl* ND, StmtOrDecl SOD)
{
   addDependency(ND);
   auto AccessSpec = ND->getAccess();

   switch (AccessSpec) {
   case AccessSpecifier::Default:
      llvm_unreachable("didn't remove default access specifier from decl!");
   case AccessSpecifier::Public:
      return;
   case AccessSpecifier::Private: {
      if (compilerInstance->getOptions().isTest() && IsTestable(ND))
         return;

      // only visible within the immediate context the symbol was defined in
      auto* Ctx = ND->getDeclContext();

      // All extensions within the same file can access private declarations.
      bool SameFile = ND->getModule() == getDeclContext().getDeclModule();
      if (SameFile) {
         Ctx = Ctx->lookThroughExtension();
      }

      for (auto* Curr = &getDeclContext(); Curr; Curr = Curr->getParentCtx()) {
         if (SameFile && isa<ExtensionDecl>(Curr)) {
            auto *R = cast<ExtensionDecl>(Curr)->getExtendedRecord();
            if (R == Ctx) {
               return;
            }
            if (auto *Other = dyn_cast<RecordDecl>(Ctx)) {
               if (Other->isInstantiation() && Other->getSpecializedTemplate() == R) {
                  return;
               }
            }
         }

         if (Curr == Ctx) {
            return;
         }
      }

      // declaration is not accessible here
      diagnose(SOD, err_private_access, ND, ND->getDeclName(),
               SOD.getSourceRange());

      break;
   }
   case AccessSpecifier::Protected: {
      if (compilerInstance->getOptions().isTest() && IsTestable(ND))
         return;

      // Only visible within declaration context or subclasses (should have
      // been rejected outside of classes)
      auto C = cast<ClassDecl>(ND->getNonTransparentDeclContext());
      auto* Ctx = ND->getDeclContext()->lookThroughExtension();
      for (auto* Curr = &getDeclContext(); Curr; Curr = Curr->getParentCtx()) {
         if (auto *Ext = dyn_cast<ExtensionDecl>(Curr)) {
            auto *R = Ext->getExtendedRecord();
            if (R == Ctx) {
               return;
            }
            if (auto *Other = dyn_cast<RecordDecl>(Ctx)) {
               if (Other->isInstantiation() && Other->getSpecializedTemplate() == R) {
                  return;
               }
            }
         }

         auto SubClass = dyn_cast<ClassDecl>(Curr);
         if (SubClass && (C == SubClass || C->isBaseClassOf(SubClass))) {
            return;
         }
      }

      // declaration is not accessible here
      diagnose(SOD, err_protected_access, ND, ND->getDeclName(),
               C->getDeclName(), SOD.getSourceRange());

      break;
   }
   case AccessSpecifier::FilePrivate: {
      if (compilerInstance->getOptions().isTest() && IsTestable(ND))
         return;

      // visible within the file it was declared
      auto& FileMgr = compilerInstance->getFileMgr();
      auto DeclID = FileMgr.getLexicalSourceId(ND->getSourceLoc());
      auto CurrID = FileMgr.getLexicalSourceId(SOD.getSourceLoc());

      if (DeclID == CurrID || FileMgr.wasIncludedFrom(CurrID, DeclID))
         return;

      // declaration is not accessible here
      diagnose(SOD, err_fileprivate_access, ND, ND->getDeclName(),
               FileMgr.getFileName(DeclID), SOD.getSourceRange());

      break;
   }
   case AccessSpecifier::Internal: {
      if (compilerInstance->getOptions().isTest() && IsTestable(ND))
         return;

      if (getDeclContext().getDeclModule()->getBaseModule()
          == ND->getModule()->getBaseModule()) {
         return;
      }

      diagnose(SOD, err_internal_access, ND, ND->getDeclName(),
               ND->getModule()->getBaseModule()->getDeclName(),
               SOD.getSourceRange());

      break;
   }
   }

   diagnose(note_access_spec_here, /*implicitly*/ !ND->getAccessRange(),
            (int)AccessSpec, ND->getAccessRange(), ND->getSourceLoc());
}

unsigned SemaPass::getSerializationFile(Decl* D)
{
   if (D->instantiatedFromProtocolDefaultImpl()) {
      D = D->getModule();
   }

   if (auto* ND = dyn_cast<NamedDecl>(D)) {
      while (auto* InstScope = getInstantiationScope(ND)) {
         ND = InstScope;
      }

      D = ND;
   }

   return getCompilerInstance().getFileMgr().getLexicalSourceId(
       D->getSourceLoc());
}

void SemaPass::addDependency(Decl* ReferencedDecl)
{
   addDependency(getCurrentDecl(), ReferencedDecl);
}

void SemaPass::addDependency(NamedDecl* D, Decl* ReferencedDecl)
{
   auto& CI = getCompilerInstance();
   auto* IncMgr = CI.getIncMgr();
   if (!IncMgr)
      return;

   IncMgr->addDependency(getSerializationFile(D),
                         getSerializationFile(ReferencedDecl));
}