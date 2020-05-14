#include "cdotc/AST/Decl.h"
#include "cdotc/Basic/NestedNameSpecifier.h"
#include "cdotc/IL/Constants.h"
#include "cdotc/Module/ModuleManager.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Sema/TemplateInstantiator.h"
#include "cdotc/Serialization/ModuleFile.h"
#include "cdotc/Support/SaveAndRestore.h"
#include "cdotc/Support/StringSwitch.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::sema;
using namespace cdot::support;

QueryResult ResolveMacrosQuery::run()
{
   llvm_unreachable("remove this!");
}

QueryResult ResolveStaticDeclarationsQuery::run()
{
   llvm_unreachable("remove this!");
}

QueryResult ResolveWildcardDeclarationsQuery::run()
{
   llvm_unreachable("remove this!");
}

QueryResult ResolveMetaDeclarationsQuery::run()
{
   llvm_unreachable("remove this!");
}

static void updateSpecialNames(SemaPass& Sema, QualType T, ExtensionDecl* Ext)
{
   if (!T->isRecordType())
      return;

   auto& Tbl = Sema.Context.getDeclNameTable();
   auto InitName = Tbl.getConstructorName(T);
   auto DeinitName = Tbl.getDestructorName(T);

   for (auto* Decl : Ext->getDecls()) {
      if (auto* Init = dyn_cast<InitDecl>(Decl)) {
         assert(Init->isCompleteInitializer()
                && "already added base initializers?");

         Init->setName(InitName);
         Sema.makeDeclAvailable(*Init->getDeclContext(), InitName, Init);
      }
      else if (auto* Deinit = dyn_cast<DeinitDecl>(Decl)) {
         Deinit->setName(DeinitName);
         Sema.makeDeclAvailable(*Init->getDeclContext(), DeinitName, Deinit);
      }
   }
}

static void CheckTemplateExtension(QueryContext &QC, ExtensionDecl *Ext,
                                   RecordDecl *Template)
{
   for (auto *D : Ext->getDecls()) {
      if (isa<RecordDecl>(D)) {
         QC.Sema->diagnose(err_template_nested_type, D->getSourceLoc());
         QC.Sema->diagnose(note_template_declared_here, Template->getDeclName(),
             Template->getSourceLoc());
      }
   }
}

QueryResult FindExtensionsQuery::run()
{
   using ResultKind = MatchExtensionTypeQuery::ResultKind;

   auto& Context = QC.Context;
   SmallPtrSet<ExtensionDecl*, 2> ExtensionsToRemove;

   Status S = Done;
   for (auto* Ext : Context.UnresolvedExtensions) {
      SemaPass::DeclScopeRAII DSR(*QC.Sema, Ext->getDeclContext());

      ResultKind Applicability;
      if (QC.ExtensionApplies(Applicability, Ext, T)) {
         return fail();
      }

      switch (Applicability) {
      case ResultKind::DoesNotApply:
      case ResultKind::MightApply:
         break;
      case ResultKind::AppliesDirectly:
         // FIXME generalized extensions
         Ext->setExtendedRecord(T->getRecord());
         Ext->getExtendedType().setResolvedType(T);

         ExtensionsToRemove.insert(Ext);

         // Update the names of contained initializers and deinitializers.
         updateSpecialNames(*QC.Sema, T, Ext);

         LLVM_FALLTHROUGH;
      case ResultKind::Applies: {
         if (T->isRecordType() && T->getRecord()->isTemplate()) {
            CheckTemplateExtension(QC, Ext, T->getRecord());
         }

         Context.addExtension(T, Ext);
         Ext->setName(QC.Context.getDeclNameTable().getExtensionName(T));

         break;
      }
      }
   }

   bool isProtocol = T->isRecordType() && T->getRecord()->isProtocol();
   for (auto* Ext : ExtensionsToRemove) {
      // Diagnose invalid 'default' attributes.
      if (!isProtocol) {
         for (auto *Decl : Ext->getDecls()) {
            if (Decl->isDefault()) {
               QC.Sema->diagnose(err_default_only_in_protocol_extension,
                  Decl->getSourceLoc());

               Decl->setDefault(false);
            }
         }
      }

      Context.UnresolvedExtensions.remove(Ext);
   }

   QC.Sema->updateLookupLevel(T->getRecord(), LookupLevel::Extensions);
   return finish(S);
}

static bool MatchNominalType(MatchExtensionTypeQuery::ResultKind& Result,
                             QueryContext& QC, SemaPass& SP, QualType GivenTy,
                             Expression* PatternTypeExpr)
{
   NamedDecl* FoundDecl = nullptr;
   if (QC.GetExtendedDecl(FoundDecl, SourceType(PatternTypeExpr))) {
      return true;
   }

   if (auto* Alias = dyn_cast<AliasDecl>(FoundDecl)) {
      if (QC.TypecheckDecl(Alias)) {
         return true;
      }

      if (!Alias->getType()->isMetaType()) {
         SP.diagnose(err_generic_error, "expected type alias",
                     PatternTypeExpr->getSourceRange());

         return true;
      }

      if (GivenTy == Alias->getType()->removeMetaType()) {
         Result = MatchExtensionTypeQuery::AppliesDirectly;
      }
      else if (Alias->getType()->isDependentType()) {
         Result = MatchExtensionTypeQuery::MightApply;
      }
      else {
         Result = MatchExtensionTypeQuery::DoesNotApply;
      }

      return false;
   }

   auto* R = dyn_cast<RecordDecl>(FoundDecl);
   if (!R) {
      SP.diagnose(err_generic_error,
                  "unexpected declaration referenced in extension",
                  PatternTypeExpr->getSourceRange());

      return true;
   }

   if (!GivenTy->isRecordType()) {
      if (GivenTy->isDependentType()) {
         Result = MatchExtensionTypeQuery::MightApply;
      }
      else {
         Result = MatchExtensionTypeQuery::DoesNotApply;
      }

      return false;
   }

   auto* GivenRec = GivenTy->getRecord();
   if (R->isTemplate()) {
      if (GivenRec->isInstantiation()
          && R == GivenRec->getSpecializedTemplate()) {
         Result = MatchExtensionTypeQuery::Applies;
      }
      else if (R == GivenRec) {
         Result = MatchExtensionTypeQuery::AppliesDirectly;
      }
      else {
         Result = MatchExtensionTypeQuery::DoesNotApply;
      }

      return false;
   }

   if (R == GivenRec) {
      Result = MatchExtensionTypeQuery::AppliesDirectly;
   }
   else {
      Result = MatchExtensionTypeQuery::DoesNotApply;
   }

   return false;
}

QueryResult GetExtendedDeclQuery::run()
{
   if (ExtendedTy.isResolved()) {
      if (ExtendedTy->isRecordType())
         return finish(ExtendedTy->getRecord());

      return finish(nullptr);
   }

   auto& SP = *QC.Sema;
   auto* PatternTypeExpr = ExtendedTy.getTypeExpr();

   if (!isa<IdentifierRefExpr>(PatternTypeExpr)
   && !isa<TemplateArgListExpr>(PatternTypeExpr)) {
      return finish(nullptr);
   }

   SmallVector<Expression*, 4> NestedNameSpec;
   while (PatternTypeExpr) {
      NestedNameSpec.push_back(PatternTypeExpr);
      PatternTypeExpr = PatternTypeExpr->getParentExpr();
   }

   std::reverse(NestedNameSpec.begin(), NestedNameSpec.end());

   NamedDecl* FoundDecl = nullptr;
   DeclContext* DC = &SP.getDeclContext();

   for (unsigned i = 0, Depth = NestedNameSpec.size(); i < Depth; ++i) {
      auto* Ident = cast<IdentifierRefExpr>(NestedNameSpec[i]);

      MultiLevelLookupResult LookupRes;
      if (i == 0) {
         const SingleLevelLookupResult* Result;
         if (QC.LookupFirst(Result, DC, Ident->getDeclName(),
                            LookupOpts::Restricted)) {
            return fail();
         }

         LookupRes.addResult(*Result);
      }
      else {
         const MultiLevelLookupResult* Result;
         if (QC.DirectLookup(Result, DC, Ident->getDeclName(), true,
                             LookupOpts::Restricted)) {
            return fail();
         }

         LookupRes.addResult(*Result);
      }

      if (LookupRes.empty()) {
         SP.diagnoseMemberNotFound(DC, Ident, Ident->getDeclName());
         return fail();
      }

      if (!LookupRes.unique()) {
         SP.diagnose(err_ambiguous_reference, Ident->getDeclName(),
                     Ident->getSourceLoc());

         for (auto* D : LookupRes.allDecls()) {
            SP.diagnose(note_candidate_here, D->getSourceLoc());
         }

         return fail();
      }

      auto* ND = LookupRes.front().front();
      if (i == Depth - 1) {
         FoundDecl = ND;
         break;
      }

      DC = dyn_cast<DeclContext>(ND);
      if (!DC || isa<CallableDecl>(ND)) {
         SP.diagnose(err_cannot_lookup_member_in, ND, ND->getDeclName(),
                     ND->getSourceLoc());

         return fail();
      }

      // Check template arguments.
      auto* TemplateArgs = dyn_cast<TemplateArgListExpr>(NestedNameSpec[i + 1]);
      if (!TemplateArgs)
         continue;

      i += 1;

      if (!ND->isTemplate()) {
         SP.diagnose(err_not_a_template, ND, ND->getDeclName(),
                     ND->getSourceLoc());

         return fail();
      }

      if (auto* R = dyn_cast<RecordDecl>(ND)) {
         //         RecordDecl *Inst;
         llvm_unreachable("FIXME get template argument list");
      }
      else {
         assert(isa<AliasDecl>(ND));
         llvm_unreachable("FIXME instantiate alias");
      }
   }

   assert(FoundDecl && "no declaration found?");

   return finish(FoundDecl);
}

QueryResult MatchExtensionTypeQuery::run()
{
   using TypeKind = GetExtensionTypeKindQuery::ResultKind;

   auto& SP = *QC.Sema;

   // Check if the type is already resolved.
   const SourceType& ExtendedTy = PatternTy;
   if (ExtendedTy.isResolved()) {
      if (ExtendedTy.getResolvedType() == T) {
         return finish(AppliesDirectly);
      }
      if (ExtendedTy->isDependentType()) {
         return finish(MightApply);
      }

      return finish(DoesNotApply);
   }

   // Get the kind of extension to quickly rule out simple cases.
   TypeKind Kind;
   if (QC.GetExtensionTypeKind(Kind, PatternTy)) {
      return fail();
   }

   Expression* Expr = ExtendedTy.getTypeExpr();

   switch (Kind) {
   case TypeKind::Nominal: {
      // Can't do normal lookup here because this query is called within
      // PrepareNameLookup. Also we do not complain about missing template
      // parameters here.
      ResultKind Result;
      if (MatchNominalType(Result, QC, SP, T, Expr))
         return fail();

      return finish(Result);
   }
   case TypeKind::Builtin: {
      if (!T->isBuiltinType()) {
         return finish(DoesNotApply);
      }

      auto TypeRes = QC.Sema->visitSourceType(ExtendedTy);
      if (!TypeRes)
         return fail();

      if (TypeRes.get() != T) {
         if (ExtendedTy->isDependentType())
            return finish(MightApply);

         return finish(DoesNotApply);
      }

      return finish(AppliesDirectly);
   }
   case TypeKind::Pointer: {
      if (!T->isPointerType()) {
         return finish(DoesNotApply);
      }

      llvm_unreachable("remove when pointers become nominal types");
   }
   case TypeKind::Tuple: {
      if (!T->isTupleType()) {
         return finish(DoesNotApply);
      }

      auto* GivenTy = T->uncheckedAsTupleType();
      auto* TupleExpr = cast<TupleTypeExpr>(Expr);

      auto GivenTys = GivenTy->getContainedTypes();
      auto NeededTys = TupleExpr->getContainedTypes();

      if (GivenTys.size() != NeededTys.size()) {
         return finish(DoesNotApply);
      }

      unsigned NumParams = NeededTys.size();
      for (unsigned i = 0; i < NumParams; ++i) {
         ResultKind RK;
         if (QC.MatchExtensionType(RK, GivenTys[i], NeededTys[i])) {
            return fail();
         }

         if (RK != Applies && RK != AppliesDirectly) {
            return finish(RK);
         }
      }

      return finish(AppliesDirectly);
   }
   case TypeKind::Function: {
      if (!T->isFunctionType()) {
         return finish(DoesNotApply);
      }

      auto* GivenTy = T->uncheckedAsFunctionType();
      auto* FnExpr = cast<FunctionTypeExpr>(Expr);

      // Check thin vs. thick.
      if (isa<LambdaType>(GivenTy) && FnExpr->isThin()) {
         return finish(DoesNotApply);
      }

      // Check function type attributes.
      if (GivenTy->throws() && !FnExpr->throws()) {
         return finish(DoesNotApply);
      }
      if (GivenTy->isUnsafe() && !FnExpr->isUnsafe()) {
         return finish(DoesNotApply);
      }
      if (GivenTy->isAsync() != FnExpr->isAsync()) {
         return finish(DoesNotApply);
      }

      // Check return type.
      ResultKind RK;
      if (QC.MatchExtensionType(RK, GivenTy->getReturnType(),
                                FnExpr->getReturnType())) {
         return fail();
      }

      if (RK != Applies && RK != AppliesDirectly) {
         return finish(RK);
      }

      // Check parameters.
      auto NeededParams = FnExpr->getArgTypes();
      auto NeededInfo = FnExpr->getParamInfo();

      auto GivenParams = GivenTy->getParamTypes();
      auto GivenInfo = GivenTy->getParamInfo();

      if (NeededParams.size() != GivenParams.size()) {
         return finish(DoesNotApply);
      }

      unsigned NumParams = NeededParams.size();
      for (unsigned i = 0; i < NumParams; ++i) {
         ArgumentConvention GivenConvention = GivenInfo[i].getConvention();
         if (GivenConvention == ArgumentConvention::Default) {
            GivenConvention = ArgumentConvention::Borrowed;
         }

         if (NeededInfo[i].getConvention() != GivenConvention) {
            return finish(DoesNotApply);
         }

         if (QC.MatchExtensionType(RK, GivenParams[i], NeededParams[i])) {
            return fail();
         }

         if (RK != Applies && RK != AppliesDirectly) {
            return finish(RK);
         }
      }

      return finish(AppliesDirectly);
   }
   case TypeKind::Array: {
      if (!T->isArrayType()) {
         return finish(DoesNotApply);
      }

      auto* GivenTy = T->uncheckedAsArrayType();
      auto* ArrExpr = cast<ArrayTypeExpr>(Expr);

      // Check if the element type matches.
      ResultKind RK;
      if (QC.MatchExtensionType(RK, GivenTy->getElementType(),
                                ArrExpr->getElementTy())) {
         return fail();
      }

      if (RK != Applies && RK != AppliesDirectly) {
         return finish(RK);
      }

      // Check if the size matches.
      auto SemaRes = SP.visitExpr(ArrExpr->getSizeExpr());
      if (!SemaRes)
         return fail();

      // If the needed type is a value template parameter, we have a match.
      if (auto* Ident = dyn_cast<IdentifierRefExpr>(ArrExpr->getSizeExpr())) {
         if (Ident->getKind() == IdentifierKind::TemplateParam) {
            auto* Param = Ident->getTemplateParam();
            if (!Param->isTypeName() && !Param->isVariadic()) {
               return finish(Applies);
            }

            return finish(MightApply);
         }
      }

      // Otherwise, compare the sizes.
      il::Constant* SizeVal;
      if (QC.ResolveStaticExpr(SizeVal, ArrExpr->getSizeExpr())) {
         return fail();
      }

      if (!isa<il::ConstantInt>(SizeVal)) {
         SP.diagnose(err_generic_error,
                     "array size expression must be integral",
                     ArrExpr->getSizeExpr()->getSourceRange());

         return fail();
      }

      if (cast<il::ConstantInt>(SizeVal)->getZExtValue()
          != GivenTy->getNumElements()) {
         return finish(DoesNotApply);
      }

      return finish(AppliesDirectly);
   }
   }
}

QueryResult ExtensionAppliesQuery::run()
{
   auto& SP = *QC.Sema;
   SemaPass::DeclScopeRAII declContextRAII(SP, Ext->getDeclContext());

   MatchExtensionTypeQuery::ResultKind Result;
   if (QC.MatchExtensionType(Result, T, Ext->getExtendedType())) {
      return fail();
   }

   return finish(Result);
}

QueryResult GetExtensionTypeKindQuery::run()
{
   Expression* Expr = T.getTypeExpr();

   if (isa<TupleTypeExpr>(Expr)) {
      return finish(Tuple);
   }

   if (isa<FunctionTypeExpr>(Expr)) {
      return finish(Function);
   }

   if (isa<ArrayTypeExpr>(Expr)) {
      return finish(Array);
   }

   bool HasTemplateArgs = false;
   if (auto* TemplateArgs = dyn_cast<TemplateArgListExpr>(Expr)) {
      Expr = TemplateArgs->getParentExpr();
      HasTemplateArgs = true;
   }

   assert(isa<IdentifierRefExpr>(Expr) && "invalid type expression");
   return finish(Nominal);
}

QueryResult ResolveExtensionQuery::run()
{
   QC.Sema->makeExtensionVisible(Ext);
   return finish();
}

QueryResult GetAssociatedTypeImplQuery::run()
{
   auto* Impl = R->lookupSingle<AliasDecl>(Name);
   if (Impl) {
      return finish(Impl);
   }

   for (auto* Ext : Extensions) {
      Impl = Ext->lookupSingle<AliasDecl>(Name);
      if (Impl) {
         return finish(Impl);
      }
   }

   return finish(nullptr);
}

QueryResult GetAssociatedTypeDeclQuery::run()
{
   auto* Impl = P->lookupSingle<AssociatedTypeDecl>(Name);
   if (Impl) {
      return finish(Impl);
   }

   auto conformances = QC.Context.getConformanceTable().getAllConformances(P);
   for (auto* conf : conformances) {
      if (conf->isConditional() && Constraints) {
         if (!QC.IsSupersetOf(conf->getConstraints(), Constraints)) {
            continue;
         }
      }

      Impl = conf->getProto()->lookupSingle<AssociatedTypeDecl>(Name);
      if (Impl) {
         return finish(Impl);
      }
   }

   return finish(nullptr);
}

static DeclarationName adaptName(ASTContext& Ctx, DeclContext& OldCtx,
                                 DeclContext& NewCtx, DeclarationName Name)
{
   switch (Name.getKind()) {
   default:
      return Name;
   case DeclarationName::ConstructorName:
   case DeclarationName::BaseConstructorName:
   case DeclarationName::DestructorName: {
      QualType Ty = Name.getConstructorType();
      auto* OldRec = dyn_cast<RecordDecl>(&OldCtx);
      if (!OldRec || OldRec != Ty->getRecord())
         return Name;

      auto* NewRec = dyn_cast<RecordDecl>(&NewCtx);
      if (!NewRec)
         return Name;

      switch (Name.getKind()) {
      case DeclarationName::ConstructorName:
         return Ctx.getDeclNameTable().getConstructorName(
             Ctx.getRecordType(NewRec));
      case DeclarationName::BaseConstructorName:
         return Ctx.getDeclNameTable().getConstructorName(
             Ctx.getRecordType(NewRec), false);
      case DeclarationName::DestructorName:
         return Ctx.getDeclNameTable().getDestructorName(
             Ctx.getRecordType(NewRec));
      default:
         llvm_unreachable("bad name kind");
      }
   }
   }
}

namespace {

struct LookupOptions {
   /// If true, look in imported modules.
   bool ExternalLookup = true;

   /// If true, look for local names.
   bool LocalLookup = false;

   /// If true, stop after the first result that was found.
   bool FindFirst = false;

   /// If true, only look for types.
   bool TypeLookup = false;

   /// If true, only look in this context, not any enclosing ones.
   bool ImmediateContextOnly = false;

   /// Whether or not to look in protocol conformances.
   bool LookInConformances = true;
};

struct LookupData {
   /// The Sema instance.
   SemaPass& SP;

   /// The vector to store lookup results in.
   MultiLevelLookupResult& Result;

   /// The original context of this lookup.
   DeclContext* OriginalCtx;

   /// True if we already checked the extended record of an extension. Only
   /// applies if OriginalCtx is an ExtensionDecl.
   mutable bool CheckedExtendedRecord;

   /// The options for this lookup.
   LookupOptions& Opts;

   /// Reference to the std.prelude module for easy access.
   Module* PreludeMod;

   /// Keeps track of whether we looked up in prelude.
   bool& LookedInPrelude;

   LookupData(SemaPass& SP, MultiLevelLookupResult& Result,
              DeclContext* OriginalCtx, LookupOptions& Opts, Module* PreludeMod,
              bool& LookedInPrelude)
       : SP(SP), Result(Result), OriginalCtx(OriginalCtx),
         CheckedExtendedRecord(false), Opts(Opts), PreludeMod(PreludeMod),
         LookedInPrelude(LookedInPrelude)
   {
      LookedInPrelude = false;
   }
};

} // anonymous namespace

static bool MultiLevelLookupImpl(DeclContext& CtxRef, DeclarationName Name,
                                 LookupData& Data);

static void DoLocalLookup(DeclContext* LocalCtx, DeclarationName Name,
                          LookupData& Data)
{
   auto& NameTable = Data.SP.getContext().getDeclNameTable();
   LambdaScope* LS = nullptr;

   for (auto S = Data.SP.getCurrentScope(); S; S = S->getEnclosingScope()) {
      switch (S->getTypeID()) {
      case Scope::LambdaScopeID: {
         if (!LS)
            LS = cast<LambdaScope>(S);

         LocalCtx = LocalCtx->getParentCtx();
         LLVM_FALLTHROUGH;
      }
      case Scope::FunctionScopeID:
      case Scope::MethodScopeID: {
         auto ScopedResult
             = cast<FunctionScope>(S)->getCallableDecl()->lookup(Name);

         if (ScopedResult) {
            if (LS == S) {
               // this argument isn't captured.
               LS = nullptr;
            }

            Data.Result.addResult(DeclContextLookupResult(ScopedResult), LS);
            if (Data.Opts.FindFirst) {
               return;
            }

            LS = nullptr;
         }

         break;
      }
      case Scope::BlockScopeID: {
         auto DN = NameTable.getLocalVarName(Name,
                                             cast<BlockScope>(S)->getScopeID());

         auto ScopedResult = LocalCtx->lookup(DN);
         if (ScopedResult) {
            Data.Result.addResult(ScopedResult, LS);
            if (Data.Opts.FindFirst) {
               return;
            }

            LS = nullptr;
         }

         break;
      }
      default:
         break;
      }
   }
}

static bool LookupInExtension(ExtensionDecl* Ext, DeclarationName Name,
                              LookupData& Data);

static bool LookupInRecord(RecordDecl* R, DeclarationName Name,
                           LookupData& Data)
{
   bool LookupInConformances
       = isa<ProtocolDecl>(R) && Data.Opts.LookInConformances
          && Data.SP.hasLookupLevel(R, LookupLevel::Conformances);

   ASTContext& Context = Data.SP.Context;

   // Lookup in protocol conformances.
   if (LookupInConformances) {
      auto Conformances = Context.getConformanceTable().getAllConformances(R);
      for (auto* Conf : Conformances) {
         auto NewName = adaptName(Context, *R, *Conf->getProto(), Name);
         if (MultiLevelLookupImpl(*Conf->getProto(), NewName, Data)) {
            return true;
         }

         if (!Data.Result.empty() && Data.Opts.FindFirst) {
            return false;
         }
      }
   }

   if (!Data.SP.hasLookupLevel(R, LookupLevel::Extensions)) {
      return false;
   }

   // Lookup in all extensions. Accessibility will be checked later.
   for (auto* D : R->getExtensions()) {
      auto* Ext = dyn_cast<ExtensionDecl>(D);
      if (MultiLevelLookupImpl(*Ext, Name, Data)) {
         return true;
      }

      if (!Data.Result.empty() && Data.Opts.FindFirst) {
         return false;
      }
   }

   return false;
}

static bool LookupInBaseClass(ClassDecl* C, DeclarationName Name,
                              LookupData& Data)
{
   if (auto* Base = C->getParentClass()) {
      if (MultiLevelLookupImpl(*Base, Name, Data)) {
         return true;
      }

      if (!Data.Result.empty() && Data.Opts.FindFirst) {
         return false;
      }
   }

   return LookupInRecord(C, Name, Data);
}

static bool LookupInExtension(ExtensionDecl* Ext, DeclarationName Name,
                              LookupData& Data)
{
   // Lookup in the extension conformances.
   auto Rec = Ext->getExtendedRecord();
   if (!Rec) {
      return false;
   }

   // Only look for members of the extended record if this extension is the
   // original lookup context.
   if (!Data.CheckedExtendedRecord) {
      Data.CheckedExtendedRecord = true;

      if (MultiLevelLookupImpl(*Ext->getExtendedRecord(), Name, Data)) {
         return true;
      }

      if (!Data.Result.empty() && Data.Opts.FindFirst) {
         return false;
      }
   }

   // Only look in conformances of protocols, since all types that implement
   // a protocol are required to provide visible declarations.
   if (!isa<ProtocolDecl>(Rec) || !Data.SP.hasLookupLevel(Rec, LookupLevel::Conformances)) {
      return false;
   }

   for (auto& Conf : Ext->getConformanceTypes()) {
      if (!Conf.isResolved() || Conf->isErrorType()) {
         break;
      }

      if (MultiLevelLookupImpl(*Conf->getRecord(), Name, Data)) {
         return true;
      }

      if (!Data.Result.empty() && Data.Opts.FindFirst) {
         return false;
      }
   }

   return false;
}

static bool LookupInImports(DeclContext* Ctx, DeclarationName Name,
                            LookupData& Data)
{
   for (auto* I : Ctx->getImportedModules()) {
      if (MultiLevelLookupImpl(*I->getDecl(), Name, Data)) {
         return true;
      }

      if (!Data.Result.empty() && Data.Opts.FindFirst) {
         return false;
      }
   }

   return false;
}

static bool instantiateTemplateMembers(QueryContext& QC, DeclContext* Ctx,
                                       DeclarationName Name,
                                       DeclContextLookupResult& Result)
{
   if (auto *Ext = dyn_cast<ExtensionDecl>(Ctx)) {
      Ctx = Ext->getExtendedRecord();
   }

   auto* R = dyn_cast<RecordDecl>(Ctx);
   if (!R) {
      return false;
   }

   if (Result.size() == 1) {
      auto* D = Result.front();
      if (D->getRecord() != R) {
         NamedDecl* Res = QC.Sema->getInstantiator().InstantiateTemplateMember(D, R);
         if (!Res) {
            return true;
         }

         Result = Ctx->lookup(Name);
      }

      return false;
   }

   // Since InstantiateTemplateMember might modify the underlying storage of
   // the lookup result, we need to copy it.
   SmallVector<NamedDecl*, 2> Decls;
   Decls.reserve(Result.size());
   Decls.append(Result.begin(), Result.end());

   bool MadeChanges = false;
   for (auto* D : Decls) {
      if (D->getRecord() != R) {
         NamedDecl* Res = QC.Sema->getInstantiator().InstantiateTemplateMember(D, R);
         if (!Res) {
            return true;
         }

         MadeChanges = true;
      }
   }

   if (MadeChanges) {
      Result = Ctx->lookup(Name);
   }

   return false;
}

static bool HandleFoundDecl(DeclContext* Ctx, DeclarationName Name,
                            LookupData& Data)
{
   auto& QC = Data.SP.QC;

   auto Result = Ctx->lookup(Name);
   assert(!Result.empty());

   // Check for template members we need to instantiate.
   if (instantiateTemplateMembers(QC, Ctx, Name, Result)) {
      return true;
   }

   // If we found a using declaration, resolve it.
   if (auto* U = dyn_cast<UsingDecl>(Result.front())) {
      SmallVector<NamedDecl*, 2> FoundDecls;
      if (QC.ResolveUsing(U, &FoundDecls)) {
         return true;
      }

      for (auto* ND : FoundDecls) {
         // FIXME
         Data.Result.addResult(DeclContextLookupResult(ND));
      }

      return false;
   }

   Data.Result.addResult(Result);
   return false;
}

static bool MultiStageLookup(DeclContext* DC, DeclarationName Name,
                             LookupData& Data)
{
   // If there's already a declaration with that name, we are already done.
   if (DC->hasAnyDeclNamed(Name)) {
      return HandleFoundDecl(DC, Name, Data);
   }

   return false;
}

static bool MultiLevelLookupImpl(DeclContext& CtxRef, DeclarationName Name,
                                 LookupData& Data)
{
   auto* Ctx = &CtxRef;

   // First do a local lookup considering scoped names. This can only be
   // valid in the current context.
   if (Data.Opts.LocalLookup) {
      DoLocalLookup(Ctx, Name, Data);

      if (!Data.Result.empty() && Data.Opts.FindFirst) {
         return false;
      }
   }

   DeclContext *FoundAltDC = nullptr;

   while (Ctx) {
      if (serial::ModuleFile* ModFile = Ctx->getModFile()) {
         ModFile->PerformExternalLookup(*Ctx, Name);
      }

      if (auto *AltDC = Data.SP.LookupContextMap[Ctx]) {
         FoundAltDC = AltDC;
      }

      if (auto* Mod = dyn_cast<ModuleDecl>(Ctx)) {
         if (Mod->getModule() == Data.PreludeMod) {
            if (Data.LookedInPrelude) {
               return false;
            }

            Data.LookedInPrelude = true;
         }

         Ctx = Mod->getPrimaryCtx();
      }

      if (MultiStageLookup(Ctx, Name, Data)) {
         return true;
      }

      if (!Data.Result.empty() && Data.Opts.FindFirst) {
         return false;
      }

      // Depending on the type of context, do additional lookups.
      {
         LookupOptions Opts = Data.Opts;
         Opts.ImmediateContextOnly = true;

         auto OptsRAII = support::saveAndRestore(Data.Opts, Opts);
         switch (Ctx->getDeclKind()) {
         case Decl::ClassDeclID: {
            // Lookup in base classes.
            auto* C = cast<ClassDecl>(Ctx);
            if (LookupInBaseClass(C, Name, Data)) {
               return true;
            }

            break;
         }
         case Decl::StructDeclID:
         case Decl::EnumDeclID:
         case Decl::UnionDeclID:
         case Decl::ProtocolDeclID: {
            auto* R = cast<RecordDecl>(Ctx);
            if (LookupInRecord(R, Name, Data)) {
               return true;
            }

            break;
         }
         case Decl::ExtensionDeclID: {
            // Lookup in the extended record.
            auto* Ext = cast<ExtensionDecl>(Ctx);
            if (LookupInExtension(Ext, Name, Data)) {
               return true;
            }

            break;
         }
         default:
            break;
         }
      }

      // Return if result found.
      if (!Data.Result.empty() && Data.Opts.FindFirst) {
         return false;
      }

      // Lookup in module imports.
      if (Data.Opts.ExternalLookup && LookupInImports(Ctx, Name, Data)) {
         return true;
      }

      // Return if result found.
      if (!Data.Result.empty() && Data.Opts.FindFirst) {
         return false;
      }

      // Return if we should not look in enclosing contexts.
      if (Data.Opts.ImmediateContextOnly) {
         return false;
      }

      Ctx = Ctx->getParentCtx();
   }

   if (Data.Result.empty() && FoundAltDC) {
      return MultiLevelLookupImpl(*FoundAltDC, Name, Data);
   }

   return false;
}

QueryResult LookupSingleQuery::run()
{
   const SingleLevelLookupResult* SingleResult;
   if (QC.LookupFirst(SingleResult, DC, Name, Opts)) {
      return fail();
   }

   if (SingleResult->size() != 1)
      return finish(nullptr);

   return finish(SingleResult->front());
}

LookupOptions prepareLookupOptions(LookupOpts Opts)
{
   LookupOptions Options;
   Options.TypeLookup = (Opts & LookupOpts::TypeLookup) != LookupOpts::None;
   Options.LocalLookup = (Opts & LookupOpts::LocalLookup) != LookupOpts::None;
   Options.LookInConformances
       = (Opts & LookupOpts::LookInConformances) != LookupOpts::None;

   return Options;
}

QueryResult LookupFirstQuery::run()
{
   Module* Prelude = nullptr;
   bool LookInPrelude = !QC.CI.getOptions().noPrelude();
   if (LookInPrelude) {
      if (QC.GetBuiltinModule(Prelude, GetBuiltinModuleQuery::Prelude)) {
         return fail();
      }
   }

   LookupOptions Opts = prepareLookupOptions(this->Opts);
   Opts.FindFirst = true;

   bool LookedInPrelude = false;
   MultiLevelLookupResult Result;
   LookupData Data(*QC.Sema, Result, DC, Opts, Prelude, LookedInPrelude);

   if (MultiLevelLookupImpl(*DC, Name, Data)) {
      return fail();
   }

   if (!Result.empty()) {
      return finish(move(Result.front()));
   }

   if (LookInPrelude && !LookedInPrelude) {
      Opts.ImmediateContextOnly = true;
      MultiLevelLookupImpl(*Prelude->getDecl(), Name, Data);
   }

   if (Result.empty()) {
      return finish(SingleLevelLookupResult());
   }

   return finish(move(Result.front()));
}

QueryResult MultiLevelLookupQuery::run()
{
   Module* Prelude = nullptr;
   bool LookInPrelude = !QC.CI.getOptions().noPrelude();
   if (LookInPrelude) {
      if (QC.GetBuiltinModule(Prelude, GetBuiltinModuleQuery::Prelude)) {
         return fail();
      }
   }

   LookupOptions Opts = prepareLookupOptions(this->Opts);
   Opts.FindFirst = false;

   bool LookedInPrelude = false;
   MultiLevelLookupResult Result;
   LookupData Data(*QC.Sema, Result, DC, Opts, Prelude, LookedInPrelude);

   if (MultiLevelLookupImpl(*DC, Name, Data)) {
      return fail();
   }

   if (LookInPrelude && !LookedInPrelude) {
      Opts.ImmediateContextOnly = true;
      MultiLevelLookupImpl(*Prelude->getDecl(), Name, Data);
   }

   return finish(move(Result));
}

static DeclarationName AdaptName(DeclarationNameTable &Tbl,
                                 DeclarationName Name,
                                 QualType CurrentExt,
                                 ExistentialType *Ext) {
   switch (Name.getKind()) {
   case DeclarationName::ConstructorName:
   case DeclarationName::BaseConstructorName: {
      QualType T = Name.getConstructorType();
      if (T == Ext) {
         return Tbl.getConstructorName(
             CurrentExt, Name.getKind() == DeclarationName::ConstructorName);
      }

      return Name;
   }
   case DeclarationName::DestructorName: {
      QualType T = Name.getDestructorType();
      if (T == Ext) {
         return Tbl.getDestructorName(CurrentExt);
      }

      return Name;
   }
   default:
      return Name;
   }
}

QueryResult MultiLevelTypeLookupQuery::run()
{
   MultiLevelLookupResult Result;
   T = T->removeMetaType()->getDesugaredType();

   if (auto* R = T->asRecordType()) {
      const MultiLevelLookupResult* Lookup;
      if (auto Err = QC.DirectLookup(Lookup, R->getRecord(), Name, true, Opts)) {
         return Query::finish(Err);
      }

      if (!Lookup->empty()) {
         Result.addResult(*Lookup);
      }
   }
   else if (auto* Ext = T->asExistentialType()) {
      for (QualType P : Ext->getExistentials()) {
         auto AdaptedName = AdaptName(QC.Context.getDeclNameTable(), Name, P, Ext);
         const MultiLevelLookupResult* Lookup;
         if (auto Err = QC.DirectLookup(Lookup, P->getRecord(), AdaptedName, true, Opts))
            return Query::finish(Err);

         if (!Lookup->empty()) {
            Result.addResult(*Lookup);
         }
      }
   }

   return finish(move(Result));
}

QueryResult DirectLookupQuery::run()
{
   MultiLevelLookupResult Result;

   auto DirectLookup = DC->lookup(Name);
   if (!DirectLookup.empty()) {
      // Check for template members we need to instantiate.
      if (instantiateTemplateMembers(QC, DC, Name, DirectLookup)) {
         return fail();
      }

      Result.addResult(DirectLookup);
   }

   // Try looking in record extensions.
   if (auto* R = dyn_cast<RecordDecl>(DC)) {
      auto& Context = QC.Sema->Context;
      if (LookInExtensions && QC.Sema->hasLookupLevel(DC, LookupLevel::Extensions)) {
         auto Extensions = R->getExtensions();
         for (auto* Ext : Extensions) {
            if (Constraints) {
               auto* CS = QC.Sema->getDeclConstraints(Ext);
               if (!QC.IsSupersetOf(Constraints, CS)) {
                  continue;
               }
            }

            DirectLookup = Ext->lookup(Name);
            if (!DirectLookup.empty()) {
               Result.addResult(DirectLookup);
            }
         }
      }

      if (auto *C = dyn_cast<ClassDecl>(R)) {
         if (auto *Base = C->getParentClass()) {
            const MultiLevelLookupResult* BaseResult;
            if (QC.DirectLookup(BaseResult, Base, Name, LookInExtensions, Opts)) {
               return fail();
            }

            if (!BaseResult->empty()) {
               Result.addResult(*BaseResult);
               return finish(move(Result));
            }
         }
      }

      if (!Result.empty() || !isa<ProtocolDecl>(R)) {
         return finish(move(Result));
      }

      // Make sure unconditional conformances are declared.
      if (((int)Opts & (int)LookupOpts::LookInConformances) == 0
      || !QC.Sema->hasLookupLevel(DC, LookupLevel::Conformances)) {
         return finish(move(Result));
      }

      // If this is a protocol, look in conformances.
      auto Conformances = Context.getConformanceTable().getAllConformances(R);
      for (auto* Conf : Conformances) {
         auto NewName = adaptName(Context, *R, *Conf->getProto(), Name);
         const MultiLevelLookupResult* ConfResult;

         if (QC.DirectLookup(ConfResult, Conf->getProto(), NewName,
                             LookInExtensions, Opts)) {
            return fail();
         }

         if (!ConfResult->empty()) {
            Result.addResult(*ConfResult);
            break;
         }
      }
   }
   // If this context is a ModuleDecl, prepare all other declarations of that
   // module.
   else if (auto* ModDecl = dyn_cast<ModuleDecl>(DC)) {
      if (ModDecl->isPrimaryCtx()) {
         for (auto* OtherDecl : ModDecl->getModule()->getDecls()) {
            if (OtherDecl == ModDecl)
               continue;

            DirectLookup = OtherDecl->lookup(Name);
            if (!DirectLookup.empty()) {
               Result.addResult(DirectLookup);
            }
         }
      }
      else {
         const MultiLevelLookupResult* PrimaryRes;
         if (QC.DirectLookup(PrimaryRes, ModDecl->getPrimaryCtx(), Name,
                             LookInExtensions, Opts)) {
            return fail();
         }

         for (auto& Decls : *PrimaryRes) {
            Result.addResult(Decls);
         }
      }
   }

   // FIXME namespace redeclarations

   return finish(move(Result));
}

QueryResult RestrictedLookupQuery::run()
{
   llvm_unreachable("yeet this");
}

QueryResult NestedNameLookupQuery::run()
{
   DeclContext* CurDC = DC;

   unsigned i = 0;
   unsigned Depth = (unsigned)Names.size();

   bool IssueDiag = (this->Opts & LookupOpts::IssueDiag) != LookupOpts::None;

   for (DeclarationName Name : Names) {
      const SingleLevelLookupResult* LookupRes;
      if (QC.LookupFirst(LookupRes, CurDC, Name, Opts)) {
         return fail();
      }

      if (LookupRes->empty()) {
         if (IssueDiag) {
            QC.Sema->diagnoseMemberNotFound(CurDC, StmtOrDecl(), Name,
                                            err_member_not_found, Loc);
         }

         return fail();
      }

      if (i++ == Depth - 1) {
         MultiLevelLookupResult Result;
         Result.addResult(*LookupRes);

         return finish(move(Result));
      }

      if (LookupRes->size() != 1) {
         if (IssueDiag) {
            QC.Sema->diagnose(err_ambiguous_reference, Name, Loc);

            for (auto* Decl : *LookupRes) {
               QC.Sema->diagnose(note_candidate_here, Decl->getSourceLoc());
            }
         }

         return fail();
      }

      auto* SingleDecl = LookupRes->front();
      if (!isa<DeclContext>(SingleDecl)) {
         if (IssueDiag) {
            QC.Sema->diagnose(err_cannot_lookup_member_in, SingleDecl,
                              SingleDecl->getDeclName(), Loc);
         }

         return fail();
      }

      CurDC = cast<DeclContext>(SingleDecl);
   }

   llvm_unreachable("must return inside the loop");
}

QueryResult FindEquivalentDeclQuery::run()
{
   if (QC.PrepareDeclInterface(Decl)) {
      return fail();
   }

   DeclarationName LookupName
       = adaptName(QC.Context, *Decl->getDeclContext()->lookThroughExtension(),
                   *DC, Decl->getDeclName());

   const MultiLevelLookupResult* LookupRes;
   if (QC.DirectLookup(LookupRes, DC, LookupName, LookInExtensions)) {
      return fail();
   }

   if (LookupRes->empty()) {
      return finish(nullptr);
   }

   if (isa<AssociatedTypeDecl>(Decl)) {
      for (auto* Impl : LookupRes->allDecls()) {
         auto* ATImpl = dyn_cast<AliasDecl>(Impl);
         if (!ATImpl) {
            continue;
         }

         return finish(ATImpl);
      }

      return finish(nullptr);
   }

   if (isa<AliasDecl>(Decl)) {
      for (auto* Impl : LookupRes->allDecls()) {
         NamedDecl* ATImpl;
         if (isa<ProtocolDecl>(DC)) {
            ATImpl = dyn_cast<AssociatedTypeDecl>(Impl);
         }
         else {
            ATImpl = dyn_cast<AliasDecl>(Impl);
         }

         if (!ATImpl) {
            continue;
         }

         return finish(ATImpl);
      }

      return finish(nullptr);
   }

   auto AllDecls = LookupRes->allDeclsStable();
   if (auto* P = dyn_cast<PropDecl>(Decl)) {
      for (auto* Impl : AllDecls) {
         auto* PropImpl = dyn_cast<PropDecl>(Impl);
         if (!PropImpl)
            continue;

         if (QC.PrepareDeclInterface(PropImpl)) {
            return fail();
         }

         bool Equivalent;
         if (QC.CheckTypeEquivalence(
                 Equivalent, P->getType(), PropImpl->getType(), Self,
                 P->getDeclContext(), P->getDeclContext())) {
            return fail();
         }

         if (!Equivalent)
            continue;

         if (P->hasGetter() && !PropImpl->hasGetter())
            continue;

         if (P->hasSetter() && !PropImpl->hasSetter())
            continue;

         return finish(PropImpl);
      }

      return finish(nullptr);
   }

   if (auto* F = dyn_cast<FieldDecl>(Decl)) {
      for (auto* Impl : AllDecls) {
         auto* FieldImpl = dyn_cast<FieldDecl>(Impl);
         if (!FieldImpl)
            continue;

         if (QC.PrepareDeclInterface(FieldImpl)) {
            return fail();
         }

         if (F->isStatic() != FieldImpl->isStatic())
            continue;

         bool Equivalent;
         if (QC.CheckTypeEquivalence(
                 Equivalent, F->getType(), FieldImpl->getType(), Self,
                 F->getDeclContext(), F->getDeclContext())) {
            return fail();
         }

         if (!Equivalent)
            continue;

         return finish(FieldImpl);
      }

      return finish(nullptr);
   }

   if (auto* Sub = dyn_cast<SubscriptDecl>(Decl)) {
      for (auto* Impl : AllDecls) {
         auto* SubImpl = dyn_cast<SubscriptDecl>(Impl);
         if (!SubImpl)
            continue;

         if (QC.PrepareDeclInterface(SubImpl)) {
            return fail();
         }

         bool Equivalent;
         if (QC.CheckTypeEquivalence(
                 Equivalent, Sub->getType(), SubImpl->getType(), Self,
                 Sub->getDeclContext(), Sub->getDeclContext())) {
            return fail();
         }

         if (!Equivalent)
            continue;

         if (Sub->hasGetter()) {
            if (!SubImpl->hasGetter())
               continue;

            EquivalentSignaturesQuery::ResultKind Result;
            if (QC.EquivalentSignatures(Result, Sub->getGetterMethod(),
                                        SubImpl->getGetterMethod(), Self)) {
               return fail();
            }

            if (Result != EquivalentSignaturesQuery::Equivalent)
               continue;
         }

         if (Sub->hasSetter()) {
            if (!SubImpl->hasSetter())
               continue;

            EquivalentSignaturesQuery::ResultKind Result;
            if (QC.EquivalentSignatures(Result, Sub->getSetterMethod(),
                                        SubImpl->getSetterMethod(), Self)) {
               return fail();
            }

            if (Result != EquivalentSignaturesQuery::Equivalent)
               continue;
         }

         return finish(SubImpl);
      }

      return finish(nullptr);
   }

   if (auto* C = dyn_cast<CallableDecl>(Decl)) {
      for (auto* Impl : AllDecls) {
         if (C->getKind() != Impl->getKind())
            continue;

         auto* FnImpl = cast<CallableDecl>(Impl);
         if (QC.PrepareDeclInterface(FnImpl)) {
            return fail();
         }

         EquivalentSignaturesQuery::ResultKind Result;
         if (QC.EquivalentSignatures(Result, C, FnImpl, Self)) {
            return fail();
         }

         if (Result != EquivalentSignaturesQuery::Equivalent)
            continue;

         return finish(FnImpl);
      }

      return finish(nullptr);
   }

   return finish(nullptr);
}

static DeclContext* getContextForDecl(SemaPass& SP, NamedDecl* ND)
{
   if (!ND)
      return nullptr;

   if (auto* R = dyn_cast<RecordDecl>(ND)) {
      return R;
   }

   if (auto* AT = dyn_cast<AssociatedTypeDecl>(ND)) {
      return getContextForDecl(SP, SP.getTypeDecl(AT->getDefaultType()));
   }

   if (auto* Alias = dyn_cast<AliasDecl>(ND)) {
      if (auto Meta = Alias->getType()->asMetaType()) {
         return getContextForDecl(SP,
                                  SP.getTypeDecl(Meta->getUnderlyingType()));
      }

      return getContextForDecl(SP, SP.getTypeDecl(Alias->getType()));
   }

   return nullptr;
}

static QualType getTypeForDecl(QueryContext& QC, NamedDecl* ND, bool IssueDiag,
                               NestedNameSpecifierWithLoc* Name)
{
   if (ND) {
      if (auto* R = dyn_cast<RecordDecl>(ND)) {
         return R->getType();
      }

      if (auto* AT = dyn_cast<AssociatedTypeDecl>(ND)) {
         if (QC.PrepareDeclInterface(AT)) {
            return QualType();
         }

         return QC.Context.getAssociatedType(AT);
      }

      if (auto* Alias = dyn_cast<AliasDecl>(ND)) {
         if (QC.PrepareDeclInterface(Alias)) {
            return QualType();
         }
         if (Alias->getType()->isMetaType()) {
            return QC.Context.getTypedefType(Alias);
         }

         return Alias->getType();
      }
   }

   if (IssueDiag) {
      QC.Sema->diagnose(err_generic_error, "name does not refer to a type",
                        Name->getFullRange());
   }

   return QualType();
}

QueryResult ResolveNestedNameSpecToTypeQuery::run()
{
   if (!DC) {
      DC = &QC.Sema->getDeclContext();
   }

   NestedNameSpecifier* Name = this->Name->getNameSpec();

   SmallVector<const NestedNameSpecifier*, 4> Names{Name};
   auto* Prev = Name->getPrevious();
   while (Prev) {
      Names.push_back(Prev);
      Prev = Prev->getPrevious();
   }

   unsigned i = 0;
   unsigned NameDepth = Names.size();

   for (auto it = Names.rbegin(), end = Names.rend(); it != end; ++i) {
      const NestedNameSpecifier* CurName = *it++;

      switch (CurName->getKind()) {
      case NestedNameSpecifier::Identifier: {
         const MultiLevelLookupResult* Result;
         if (QC.MultiLevelLookup(Result, DC, CurName->getIdentifier())) {
            return fail();
         }

         if (Result->empty()) {
            if (IssueDiag) {
               QC.Sema->diagnoseMemberNotFound(
                   DC, StmtOrDecl(), CurName->getIdentifier(),
                   diag::err_member_not_found, this->Name->getSourceRange(i));
            }

            return fail();
         }

         auto* ND = Result->front().front();
         if (i == NameDepth - 1) {
            QualType T = getTypeForDecl(QC, ND, IssueDiag, this->Name);
            if (!T) {
               return fail();
            }

            return finish(T);
         }

         DC = getContextForDecl(*QC.Sema, ND);
         if (!DC) {
            if (IssueDiag) {
               QC.Sema->diagnose(err_cannot_lookup_member_in, ND,
                                 ND->getDeclName(),
                                 this->Name->getSourceRange(i));
            }

            return fail();
         }

         break;
      }
      case NestedNameSpecifier::Namespace:
         DC = CurName->getNamespace();
         break;
      case NestedNameSpecifier::Type: {
         QualType Ty = CurName->getType();
         if (i == NameDepth - 1) {
            return finish(Ty);
         }

         auto* TypeDecl = QC.Sema->getTypeDecl(Ty);
         if (!TypeDecl) {
            if (IssueDiag) {
               QC.Sema->diagnose(err_access_member_on_type, Ty,
                                 this->Name->getSourceRange(i));
            }

            return fail();
         }
         if (!isa<DeclContext>(TypeDecl)) {
            if (IssueDiag) {
               QC.Sema->diagnose(err_cannot_lookup_member_in, TypeDecl,
                                 TypeDecl->getDeclName(),
                                 this->Name->getSourceRange(i));
            }

            return fail();
         }

         DC = cast<DeclContext>(TypeDecl);
         break;
      }
      case NestedNameSpecifier::Module:
         DC = CurName->getModule()->getDecl();
         break;
      case NestedNameSpecifier::AssociatedType: {
         auto* AT = CurName->getAssociatedType();
         auto* ATImpl = QC.LookupSingleAs<AliasDecl>(DC, AT->getDeclName());

         if (!ATImpl || !ATImpl->isTypedef()) {
            return finish(nullptr, Dependent);
         }

         if (i == NameDepth - 1) {
            return finish(AT->getDefaultType());
         }

         QualType Ty = ATImpl->getAliasedType();
         auto* TypeDecl = QC.Sema->getTypeDecl(Ty);

         if (!TypeDecl) {
            if (IssueDiag) {
               QC.Sema->diagnose(err_access_member_on_type, Ty,
                                 this->Name->getSourceRange(i));
            }

            return fail();
         }
         if (!isa<DeclContext>(TypeDecl)) {
            if (IssueDiag) {
               QC.Sema->diagnose(err_cannot_lookup_member_in, TypeDecl,
                                 TypeDecl->getDeclName(),
                                 this->Name->getSourceRange(i));
            }

            return fail();
         }

         DC = cast<DeclContext>(TypeDecl);
         break;
      }
      case NestedNameSpecifier::Alias: {
         auto* Alias = CurName->getAlias();
         if (i == NameDepth - 1) {
            QualType T = getTypeForDecl(QC, Alias, IssueDiag, this->Name);
            if (!T) {
               return fail();
            }

            return finish(T);
         }

         if (auto Err = QC.PrepareDeclInterface(Alias)) {
            return Query::finish(Err);
         }

         QualType Ty = Alias->getType();
         if (!Ty->isMetaType()) {
            if (IssueDiag) {
               QC.Sema->diagnose(err_access_member_on_type, Ty,
                                 this->Name->getSourceRange(i));
            }

            return fail();
         }

         auto* TypeDecl = QC.Sema->getTypeDecl(Ty);
         if (!TypeDecl) {
            if (IssueDiag) {
               QC.Sema->diagnose(err_access_member_on_type, Ty,
                                 this->Name->getSourceRange(i));
            }

            return fail();
         }

         if (!isa<DeclContext>(TypeDecl)) {
            if (IssueDiag) {
               QC.Sema->diagnose(err_cannot_lookup_member_in, TypeDecl,
                                 TypeDecl->getDeclName(),
                                 this->Name->getSourceRange(i));
            }

            return fail();
         }

         DC = cast<DeclContext>(TypeDecl);
         break;
      }
      case NestedNameSpecifier::TemplateParam:
      case NestedNameSpecifier::TemplateArgList: {
         return finish(nullptr, Dependent);
      }
      }
   }

   QualType T
       = getTypeForDecl(QC, dyn_cast<NamedDecl>(DC), IssueDiag, this->Name);

   if (!T) {
      return fail();
   }

   return finish(T);
}

QueryResult ResolveNestedNameSpecToDeclQuery::run()
{
   if (!DC) {
      DC = &QC.Sema->getDeclContext();
   }

   NestedNameSpecifier* Name = this->Name->getNameSpec();

   SmallVector<const NestedNameSpecifier*, 4> Names{Name};
   auto* Prev = Name->getPrevious();
   while (Prev) {
      Names.push_back(Prev);
      Prev = Prev->getPrevious();
   }

   unsigned i = 0;
   unsigned NameDepth = Names.size();

   for (auto it = Names.rbegin(), end = Names.rend(); it != end; ++i) {
      const NestedNameSpecifier* CurName = *it++;

      switch (CurName->getKind()) {
      case NestedNameSpecifier::Identifier: {
         const MultiLevelLookupResult* Result;
         if (QC.MultiLevelLookup(Result, DC, CurName->getIdentifier())) {
            return fail();
         }

         if (Result->empty()) {
            if (IssueDiag) {
               QC.Sema->diagnoseMemberNotFound(
                   DC, StmtOrDecl(), CurName->getIdentifier(),
                   diag::err_member_not_found, this->Name->getSourceRange(i));
            }

            return fail();
         }

         auto* ND = Result->front().front();
         if (i == NameDepth - 1) {
            return finish(ND);
         }

         DC = getContextForDecl(*QC.Sema, ND);
         if (!DC) {
            if (IssueDiag) {
               QC.Sema->diagnose(err_cannot_lookup_member_in, ND,
                                 ND->getDeclName(),
                                 this->Name->getSourceRange(i));
            }

            return fail();
         }

         break;
      }
      case NestedNameSpecifier::Namespace:
         DC = CurName->getNamespace();
         break;
      case NestedNameSpecifier::Type: {
         QualType Ty = CurName->getType();
         auto* TypeDecl = QC.Sema->getTypeDecl(Ty);

         if (!TypeDecl) {
            if (IssueDiag) {
               QC.Sema->diagnose(err_access_member_on_type, Ty,
                                 this->Name->getSourceRange(i));
            }

            return fail();
         }
         if (!isa<DeclContext>(TypeDecl)) {
            if (i == NameDepth - 1)
               return finish(TypeDecl);

            if (IssueDiag) {
               QC.Sema->diagnose(err_cannot_lookup_member_in, TypeDecl,
                                 TypeDecl->getDeclName(),
                                 this->Name->getSourceRange(i));
            }

            return fail();
         }

         DC = cast<DeclContext>(TypeDecl);
         break;
      }
      case NestedNameSpecifier::Module:
         DC = CurName->getModule()->getDecl();
         break;
      case NestedNameSpecifier::AssociatedType: {
         auto* AT = CurName->getAssociatedType();
         auto* ATImpl = QC.LookupSingleAs<AliasDecl>(DC, AT->getDeclName());

         if (!ATImpl || !ATImpl->isTypedef()) {
            return finish(nullptr, Dependent);
         }

         QualType Ty = ATImpl->getAliasedType();
         auto* TypeDecl = QC.Sema->getTypeDecl(Ty);

         if (!TypeDecl) {
            if (IssueDiag) {
               QC.Sema->diagnose(err_access_member_on_type, Ty,
                                 this->Name->getSourceRange(i));
            }

            return fail();
         }
         if (!isa<DeclContext>(TypeDecl)) {
            if (i == NameDepth - 1)
               return finish(TypeDecl);

            if (IssueDiag) {
               QC.Sema->diagnose(err_cannot_lookup_member_in, TypeDecl,
                                 TypeDecl->getDeclName(),
                                 this->Name->getSourceRange(i));
            }

            return fail();
         }

         DC = cast<DeclContext>(TypeDecl);
         break;
      }
      case NestedNameSpecifier::Alias: {
         auto* Alias = CurName->getAlias();
         if (auto Err = QC.PrepareDeclInterface(Alias)) {
            return Query::finish(Err);
         }

         QualType Ty = Alias->getType();
         if (!Ty->isMetaType()) {
            if (IssueDiag) {
               QC.Sema->diagnose(err_access_member_on_type, Ty,
                                 this->Name->getSourceRange(i));
            }

            return fail();
         }

         auto* TypeDecl = QC.Sema->getTypeDecl(Ty);
         if (!TypeDecl) {
            if (IssueDiag) {
               QC.Sema->diagnose(err_access_member_on_type, Ty,
                                 this->Name->getSourceRange(i));
            }

            return fail();
         }

         if (!isa<DeclContext>(TypeDecl)) {
            if (i == NameDepth - 1)
               return finish(TypeDecl);

            if (IssueDiag) {
               QC.Sema->diagnose(err_cannot_lookup_member_in, TypeDecl,
                                 TypeDecl->getDeclName(),
                                 this->Name->getSourceRange(i));
            }

            return fail();
         }

         DC = cast<DeclContext>(TypeDecl);
         break;
      }
      case NestedNameSpecifier::TemplateParam:
      case NestedNameSpecifier::TemplateArgList: {
         return finish(nullptr, Dependent);
      }
      }
   }

   return finish(cast<NamedDecl>(DC));
}