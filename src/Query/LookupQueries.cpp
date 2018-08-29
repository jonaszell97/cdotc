//
// Created by Jonas Zell on 24.08.18.
//

#include "AST/Decl.h"
#include "IL/Constants.h"
#include "Module/ModuleManager.h"
#include "QueryContext.h"
#include "Sema/SemaPass.h"
#include "Serialization/ModuleFile.h"
#include "Support/StringSwitch.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::sema;
using namespace cdot::support;

static bool InstantiateFields(QueryContext &QC, StructDecl *S)
{
   for (auto &Decls : S->getAllNamedDecls()) {
      for (auto *D : Decls.getSecond().getAsLookupResult()) {
         if (auto *F = dyn_cast<FieldDecl>(D)) {
            if (!F->isStatic()) {
               NamedDecl *Inst;
               if (QC.InstantiateTemplateMember(Inst, F, S)) {
                  return true;
               }
            }
         }
      }
   }

   return false;
}

QueryResult PrepareNameLookupQuery::run()
{
   // Lookups into this context are allowed during the execution of this
   // query (but might fail), so set the status to Done already.
   finish(Done);

   // Make sure all wildcard import- & using-declarations are resolved,
   // all macros are expanded and all static if/for/print/assert declarations
   // have been resolved.
   for (auto *Decl : DC->getDecls()) {
      switch (Decl->getKind()) {
      case Decl::ImportDeclID: {
         auto *I = cast<ImportDecl>(Decl);
         if (!I->isWildcardImport())
            break;

         if (QC.ResolveImport(I, Loc)) {
            return fail();
         }

         break;
      }
      case Decl::UsingDeclID: {
         auto *U = cast<UsingDecl>(Decl);
         if (!U->isWildcardImport())
            break;

         if (QC.ResolveUsing(U, Loc)) {
            return fail();
         }

         break;
      }
      case Decl::MacroExpansionDeclID: {
         auto *MD = cast<MacroExpansionDecl>(Decl);
         ast::Decl *Result;
         if (QC.ExpandMacroDecl(Result, MD, Loc)) {
            return fail();
         }

         break;
      }
      case Decl::StaticIfDeclID: {
         auto *If = cast<StaticIfDecl>(Decl);
         ast::Decl *Result;

         if (QC.ResolveStaticIf(Result, If, Loc)) {
            return fail();
         }

         break;
      }
      case Decl::StaticForDeclID: {
         auto *For = cast<StaticForDecl>(Decl);
         ast::Decl *Result;

         if (QC.ResolveStaticFor(Result, For, Loc)) {
            return fail();
         }

         break;
      }
      case Decl::StaticAssertDeclID: {
         auto *Assert = cast<StaticAssertDecl>(Decl);
         if (QC.ResolveStaticAssert(Assert, Loc)) {
            return fail();
         }

         break;
      }
      case Decl::StaticPrintDeclID: {
         auto *Print = cast<StaticPrintDecl>(Decl);
         if (QC.ResolveStaticPrint(Print, Loc)) {
            return fail();
         }

         break;
      }
      case Decl::InitDeclID: {
         if (QC.AssignInitName(cast<InitDecl>(Decl))) {
            return fail();
         }

         break;
      }
      default:
         break;
      }
   }

   // If this context is a ModuleDecl, prepare all other declarations of that
   // module.
   if (auto *ModDecl = dyn_cast<ModuleDecl>(DC)) {
      if (ModDecl->isPrimaryCtx()) {
         for (auto *OtherDecl : ModDecl->getModule()->getDecls()) {
            if (OtherDecl == ModDecl)
               continue;

            if (QC.PrepareNameLookup(OtherDecl, Loc)) {
               return fail();
            }
         }
      }
      else {
         if (QC.PrepareNameLookup(ModDecl->getPrimaryCtx(), Loc)) {
            return fail();
         }
      }
   }

   // FIXME namespace redeclarations

   // If this is an extension, make sure we know what type it extends.
   if (auto *Ext = dyn_cast<ExtensionDecl>(DC)) {
      if (!Ext->getExtendedRecord()) {
         NamedDecl *ExtendedDecl;
         if (QC.GetExtendedDecl(ExtendedDecl, Ext->getExtendedType())) {
            return fail();
         }

         if (auto *R = dyn_cast<RecordDecl>(ExtendedDecl)) {
            Ext->setExtendedRecord(R);
            R->addExtension(Ext);
         }
      }
   }

   // If this context is not a record, we're done.
   auto *R = dyn_cast<RecordDecl>(DC);
   if (!R)
      return finish();

   // Make template parameters visible.
   for (auto &TP : R->getTemplateParams()) {
      if (QC.PrepareTemplateParamInterface(TP, Loc)) {
         return fail();
      }
   }

   auto &Context = sema().Context;
   if (R->isTemplate()) {
      SmallVector<TemplateArgument, 2> TemplateArgs;
      for (auto *P : R->getTemplateParams()) {
         const TemplateArgument *DefaultArg = nullptr;
         if (QC.GetDefaultTemplateArg(DefaultArg, P, Loc)) {
            return fail();
         }

         TemplateArgs.emplace_back(DefaultArg->clone());
      }

      auto *List = FinalTemplateArgumentList::Create(Context, TemplateArgs);
      R->setType(Context.getDependentRecordType(R, List));
   }
   else {
      R->setType(Context.getRecordType(R));
   }

   // Declare explicit memberwise init.
   if (auto S = dyn_cast<StructDecl>(DC)) {
      // Instantiate all fields of an instantiation.
      if (S->isInstantiation()) {
         if (InstantiateFields(QC, S)) {
            return fail();
         }
      }

      if (!S->getMemberwiseInitializer()) {
         sema().declareMemberwiseInitializer(S);
      }
   }

   // Assign extensions.
   if (QC.FindExtensions(Context.getRecordType(R))) {
      return fail();
   }

   // Make sure all associated types of this record have been resolved.
   if (QC.ResolveAssociatedTypes(QC.CI.getContext().getRecordType(R))) {
      return fail();
   }

   if (!isa<ProtocolDecl>(R)) {
      // Resolve extensions.
      for (auto *Ext : R->getExtensions()) {
         if (QC.ResolveExtension(Ext, Loc)) {
            return fail();
         }
      }
   }

   return finish();
}

QueryResult FindExtensionsQuery::run()
{
   using ResultKind = MatchExtensionTypeQuery::ResultKind;

   auto &Context = QC.CI.getContext();
   SmallPtrSet<ExtensionDecl*, 2> ExtensionsToRemove;

   for (auto *Ext : Context.UnresolvedExtensions) {
      ResultKind Applicability;
      if (QC.ExtensionApplies(Applicability, Ext, T)) {
         return fail();
      }

      switch (Applicability) {
      case ResultKind::DoesNotApply:
      case ResultKind::MightApply:
         break;
      case ResultKind::AppliesDirectly:
         Context.addExtension(T, Ext);
         ExtensionsToRemove.insert(Ext);

         // FIXME
         Ext->setExtendedRecord(T->getRecord());

         break;
      case ResultKind::Applies:
         Context.addExtension(T, Ext);
         break;
      }
   }

   for (auto *Ext : ExtensionsToRemove) {
      Context.UnresolvedExtensions.erase(Ext);
   }

   return finish();
}

static bool
MatchNominalType(MatchExtensionTypeQuery::ResultKind &Result,
                 QueryContext &QC, SemaPass &SP,
                 QualType GivenTy,
                 Expression *PatternTypeExpr) {
   NamedDecl *FoundDecl = nullptr;
   if (QC.GetExtendedDecl(FoundDecl, SourceType(PatternTypeExpr))) {
      return true;
   }

   if (auto *Alias = dyn_cast<AliasDecl>(FoundDecl)) {
      if (QC.TypecheckDecl(Alias)) {
         return true;
      }

      if (!Alias->getType()->isMetaType()) {
         SP.diagnose(err_generic_error, "expected type alias",
                     PatternTypeExpr->getSourceRange());

         return true;
      }

      if (GivenTy == Alias->getType()->stripMetaType()) {
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

   auto *R = dyn_cast<RecordDecl>(FoundDecl);
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

   auto *GivenRec = GivenTy->getRecord();
   if (R->isTemplate()) {
      if (GivenRec->isInstantiation()
            && R == GivenRec->getSpecializedTemplate()) {
         Result = MatchExtensionTypeQuery::Applies;
      }
      else {
         Result = MatchExtensionTypeQuery::DoesNotApply;
      }

      return false;
   }

   if (R == GivenRec) {
      Result = MatchExtensionTypeQuery::Applies;
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

   auto &SP = sema();
   auto *PatternTypeExpr = ExtendedTy.getTypeExpr();

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

   NamedDecl *FoundDecl = nullptr;
   DeclContext *DC = &SP.getDeclContext();

   for (unsigned i = 0, Depth = NestedNameSpec.size(); i < Depth; ++i) {
      auto *Ident = cast<IdentifierRefExpr>(NestedNameSpec[i]);

      const MultiLevelLookupResult *LookupRes;
      if (QC.DirectLookup(LookupRes, DC, Ident->getDeclName(), true, false)) {
         return fail();
      }

      if (LookupRes->empty()) {
         SP.diagnoseMemberNotFound(DC, Ident, Ident->getDeclName());
         return fail();
      }

      if (LookupRes->size() != 1 || LookupRes->front().size() != 1) {
         SP.diagnose(err_ambiguous_reference, Ident->getDeclName());

         for (auto *D : LookupRes->allDecls()) {
            SP.diagnose(note_candidate_here, D->getSourceLoc());
         }

         return fail();
      }

      auto *ND = LookupRes->front().front();
      if (i == Depth - 1) {
         FoundDecl = ND;
         break;
      }

      DC = dyn_cast<DeclContext>(ND);
      if (!DC || isa<CallableDecl>(ND)) {
         SP.diagnose(err_cannot_lookup_member_in,
                     ND->getSpecifierForDiagnostic(),
                     ND->getDeclName(), ND->getSourceLoc());

         return fail();
      }

      // Check template arguments.
      auto *TemplateArgs = dyn_cast<TemplateArgListExpr>(NestedNameSpec[i + 1]);
      if (!TemplateArgs)
         continue;

      i += 1;

      if (!ND->isTemplate()) {
         SP.diagnose(err_not_a_template,
                     ND->getSpecifierForDiagnostic(),
                     ND->getDeclName(), ND->getSourceLoc());

         return fail();
      }

      if (auto *R = dyn_cast<RecordDecl>(ND)) {
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

   auto &SP = sema();

   // Check if the type is already resolved.
   const SourceType &ExtendedTy = PatternTy;
   if (ExtendedTy.isResolved()) {
      if (ExtendedTy.getResolvedType() == T)
         return finish(AppliesDirectly);

      if (ExtendedTy->isDependentType())
         return finish(MightApply);

      return finish(DoesNotApply);
   }

   // Get the kind of extension to quickly rule out simple cases.
   TypeKind Kind;
   if (QC.GetExtensionTypeKind(Kind, PatternTy)) {
      return fail();
   }

   Expression *Expr = ExtendedTy.getTypeExpr();

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

      auto TypeRes = sema().visitSourceType(ExtendedTy);
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

      auto *GivenTy = T->uncheckedAsTupleType();
      auto *TupleExpr = cast<TupleTypeExpr>(Expr);

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

      auto *GivenTy = T->uncheckedAsFunctionType();
      auto *FnExpr = cast<FunctionTypeExpr>(Expr);

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

      auto *GivenTy = T->uncheckedAsArrayType();
      auto *ArrExpr = cast<ArrayTypeExpr>(Expr);

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
      if (auto *Ident = dyn_cast<IdentifierRefExpr>(ArrExpr->getSizeExpr())) {
         if (Ident->getKind() == IdentifierKind::TemplateParam) {
            auto *Param = Ident->getTemplateParam();
            if (!Param->isTypeName() && !Param->isVariadic()) {
               return finish(Applies);
            }

            return finish(MightApply);
         }
      }

      // Otherwise, compare the sizes.
      il::Constant *SizeVal;
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
   auto &SP = sema();
   SemaPass::DeclScopeRAII declContextRAII(SP, Ext->getDeclContext());

   MatchExtensionTypeQuery::ResultKind Result;
   if (QC.MatchExtensionType(Result, T, Ext->getExtendedType())) {
      return fail();
   }

   return finish(Result);
}

QueryResult GetExtensionTypeKindQuery::run()
{
   Expression *Expr = T.getTypeExpr();

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
   if (auto *TemplateArgs = dyn_cast<TemplateArgListExpr>(Expr)) {
      Expr = TemplateArgs->getParentExpr();
      HasTemplateArgs = true;
   }

   assert(isa<IdentifierRefExpr>(Expr) && "invalid type expression");

   auto *Ident = cast<IdentifierRefExpr>(Expr);
   if (Ident->getParentExpr())
      return finish(Nominal);

   if (HasTemplateArgs) {
      if (Ident->getDeclName().isStr("UnsafePtr"))
         return finish(Pointer);

      if (Ident->getDeclName().isStr("UnsafeMutablePtr"))
         return finish(Pointer);

      return finish(Nominal);
   }

   if (!Ident->getDeclName().isSimpleIdentifier())
      return finish(Nominal);

   return finish(StringSwitch<ResultKind>(Ident->getIdent())
      .Case("UnsafeRawPtr", Pointer)
      .Case("UnsafeMutableRawPtr", Pointer)
#     define CDOT_BUILTIN_INT(Name, BW, Unsigned)           \
      .Case(#Name, Builtin)
#     define CDOT_BUILTIN_FP(Name, Precision)               \
      .Case(#Name, Builtin)
#     include "Basic/BuiltinTypes.def"
      .Default(Nominal)
   );
}

QueryResult ResolveExtensionQuery::run()
{
   sema().makeExtensionVisible(Ext);
   return finish();
}

QueryResult GetAssociatedTypeQuery::run()
{
   if (auto *Impl = R->lookupSingle<AssociatedTypeDecl>(Name)) {
      return finish(Impl);
   }

   for (auto *Ext : Extensions) {
      if (auto *Impl = Ext->lookupSingle<AssociatedTypeDecl>(Name)) {
         return finish(Impl);
      }
   }

   return finish(nullptr);
}

static DeclarationName adaptName(ASTContext &Ctx,
                                 DeclContext &OldCtx,
                                 DeclContext &NewCtx,
                                 DeclarationName Name) {
   switch (Name.getKind()) {
   default:
      return Name;
   case DeclarationName::ConstructorName:
   case DeclarationName::BaseConstructorName:
   case DeclarationName::DestructorName: {
      QualType Ty = Name.getConstructorType();
      auto *OldRec = dyn_cast<RecordDecl>(&OldCtx);
      if (!OldRec || OldRec != Ty->getRecord())
         return Name;

      auto *NewRec = dyn_cast<RecordDecl>(&NewCtx);
      if (!NewRec)
         return Name;

      switch (Name.getKind()) {
      case DeclarationName::ConstructorName:
         return Ctx.getDeclNameTable().getConstructorName(NewRec->getType());
      case DeclarationName::BaseConstructorName:
         return Ctx.getDeclNameTable().getConstructorName(NewRec->getType(),
                                                          false);
      case DeclarationName::DestructorName:
         return Ctx.getDeclNameTable().getDestructorName(NewRec->getType());
      default:
         llvm_unreachable("bad name kind");
      }
   }
   }
}

static bool MultiLevelLookupImpl(MultiLevelLookupResult &Result,
                                 SemaPass &SP,
                                 DeclContext &CtxRef,
                                 DeclarationName Name,
                                 bool ExternalLookup,
                                 bool LocalLookup,
                                 bool FindFirst,
                                 bool ImmediateContextOnly = false);

static void DoLocalLookup(MultiLevelLookupResult &Result,
                          SemaPass &SP,
                          DeclContext *LocalCtx,
                          DeclarationName Name,
                          bool FindFirst) {
   auto &NameTable = SP.getContext().getDeclNameTable();
   LambdaScope *LS = nullptr;

   for (auto S = SP.getCurrentScope(); S; S = S->getEnclosingScope()) {
      switch (S->getTypeID()) {
      case Scope::LambdaScopeID: {
         if (!LS)
            LS = cast<LambdaScope>(S);

         LocalCtx = LocalCtx->getParentCtx();
         LLVM_FALLTHROUGH;
      }
      case Scope::FunctionScopeID:
      case Scope::MethodScopeID: {
         auto ScopedResult = cast<FunctionScope>(S)->getCallableDecl()
                                                   ->lookup(Name);

         if (ScopedResult) {
            if (LS == S) {
               // this argument isn't captured.
               LS = nullptr;
            }

            Result.addResult(DeclContextLookupResult(ScopedResult), LS);
            if (FindFirst)
               return;

            LS = nullptr;
         }

         break;
      }
      case Scope::BlockScopeID: {
         auto DN = NameTable.getLocalVarName(Name,
                                             cast<BlockScope>(S)->getScopeID());

         auto ScopedResult = LocalCtx->lookup(DN);
         if (ScopedResult) {
            Result.addResult(ScopedResult, LS);
            if (FindFirst) {
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

static bool LookupInExtension(MultiLevelLookupResult &Result,
                              SemaPass &SP,
                              ExtensionDecl *Ext,
                              DeclarationName Name,
                              bool FindFirst);

static bool LookupInRecord(MultiLevelLookupResult &Result,
                           SemaPass &SP,
                           RecordDecl *R,
                           DeclarationName Name,
                           bool FindFirst) {
   bool LookupInConformances = false;
   if (isa<ProtocolDecl>(R)) {
      LookupInConformances = true;
   }

   // Lookup in protocol conformances.
   if (LookupInConformances) {
      auto Conformances = SP.getContext().getConformanceTable()
                            .getAllConformances(R);

      for (auto *Conf : Conformances) {
         auto NewName = adaptName(SP.getContext(), *R, *Conf->getProto(), Name);
         if (MultiLevelLookupImpl(Result, SP, *Conf->getProto(), NewName,
                                  true, false, FindFirst, true)) {
            return true;
         }

         if (!Result.empty() && FindFirst) {
            return false;
         }
      }
   }

   // Lookup in extensions.
   auto DN = SP.getContext().getDeclNameTable().getExtensionName(
      SP.getContext().getRecordType(R));

   auto Extensions = SP.getCompilationUnit().getCompilationModule()
                       ->getDecl()->lookup(DN);

   for (auto *D : Extensions) {
      auto *Ext = dyn_cast<ExtensionDecl>(D);
      if (MultiLevelLookupImpl(Result, SP, *Ext, Name,
                               true, false, FindFirst, true)) {
         return true;
      }

      if (!Result.empty() && FindFirst) {
         return false;
      }
   }

   return false;
}

static bool LookupInBaseClass(MultiLevelLookupResult &Result,
                              SemaPass &SP,
                              ClassDecl *C,
                              DeclarationName Name,
                              bool FindFirst) {
   if (auto *Base = C->getParentClass()) {
      if (MultiLevelLookupImpl(Result, SP, *Base, Name,
                               true, false, FindFirst, true)) {
         return true;
      }

      if (FindFirst && !Result.empty())
         return false;
   }

   return LookupInRecord(Result, SP, C, Name, FindFirst);
}

static bool LookupInExtension(MultiLevelLookupResult &Result,
                              SemaPass &SP,
                              ExtensionDecl *Ext,
                              DeclarationName Name,
                              bool FindFirst) {
   // Lookup in the extension conformances.
   auto Rec = Ext->getExtendedRecord();
   if (!Rec)
      return false;

   if (!isa<ProtocolDecl>(Rec)) {
      return false;
   }

   for (auto &Conf : Ext->getConformanceTypes()) {
      if (!Conf.isResolved())
         break;

      if (MultiLevelLookupImpl(Result, SP, *Conf->getRecord(), Name,
                               true, false, FindFirst, true)) {
         return true;
      }

      if (Result && FindFirst)
         return false;
   }

   return false;
}

static bool LookupInImports(MultiLevelLookupResult &Result,
                            SemaPass &SP,
                            DeclContext *Ctx,
                            DeclarationName Name,
                            bool FindFirst) {
   for (auto *I : Ctx->getImportedModules()) {
      if (MultiLevelLookupImpl(Result, SP, *I->getDecl(), Name,
                               true, false, FindFirst)) {
         return true;
      }

      if (Result && FindFirst)
         return false;
   }

   return false;
}

static bool MultiLevelLookupImpl(MultiLevelLookupResult &Result,
                                 SemaPass &SP,
                                 DeclContext &CtxRef,
                                 DeclarationName Name,
                                 bool ExternalLookup,
                                 bool LocalLookup,
                                 bool FindFirst,
                                 bool ImmediateContextOnly) {
   auto *Ctx = &CtxRef;
   if (SP.QC.PrepareNameLookup(Ctx)) {
      return true;
   }

   // First do a local lookup considering scoped names. This can only be
   // valid in the current context.
   if (LocalLookup) {
      DoLocalLookup(Result, SP, Ctx, Name, FindFirst);
      if (!Result.empty() && FindFirst)
         return false;
   }

   while (Ctx) {
      if (auto *ModFile = Ctx->getModFile())
         ModFile->PerformExternalLookup(*Ctx, Name);

      auto SingleResult = Ctx->lookup(Name);
      if (SingleResult) {
         Result.addResult(SingleResult);
         if (FindFirst)
            return false;
      }

      switch (Ctx->getDeclKind()) {
      case Decl::ClassDeclID: {
         /// Lookup in base classes.
         auto *C = cast<ClassDecl>(Ctx);
         if (LookupInBaseClass(Result, SP, C, Name, FindFirst))
            return true;

         break;
      }
      case Decl::StructDeclID:
      case Decl::EnumDeclID:
      case Decl::UnionDeclID:
      case Decl::ProtocolDeclID: {
         auto *R = cast<RecordDecl>(Ctx);
         if (LookupInRecord(Result, SP, R, Name, FindFirst))
            return true;

         break;
      }
      case Decl::ExtensionDeclID: {
         /// Lookup in the extended record.
         auto *Ext = cast<ExtensionDecl>(Ctx);
         if (LookupInExtension(Result, SP, Ext, Name, FindFirst))
            return true;

         break;
      }
      default:
         break;
      }

      if (FindFirst && Result)
         return false;

      /// Lookup in module imports.
      if (ExternalLookup) {
         if (LookupInImports(Result, SP, Ctx, Name, FindFirst))
            return true;
      }

      if (FindFirst && Result)
         return false;

      if (ImmediateContextOnly)
         return false;

      if (auto *ND = dyn_cast<NamedDecl>(Ctx)) {
         Ctx = ND->getNonTransparentDeclContext();
      }
      else {
         Ctx = Ctx->getParentCtx();
      }

      if (Ctx) {
         Ctx = Ctx->lookThroughExtension();
         if (SP.QC.PrepareNameLookup(Ctx)) {
            return true;
         }
      }
   }

   return false;
}

QueryResult LookupSingleQuery::run()
{
   const SingleLevelLookupResult *SingleResult;
   if (QC.LookupFirst(SingleResult, DC, Name, LocalLookup)) {
      return fail();
   }

   if (SingleResult->size() != 1)
      return finish(nullptr);

   return finish(SingleResult->front());
}

QueryResult LookupFirstQuery::run()
{
   MultiLevelLookupResult Result;
   if (MultiLevelLookupImpl(Result, sema(), *DC, Name, true, LocalLookup, true))
      return fail();

   if (!Result.empty())
      return finish(move(Result.front()));

   Module *Std;
   if (QC.GetBuiltinModule(Std, GetBuiltinModuleQuery::Std)) {
      return fail();
   }

   if (DC->getDeclModule()->getBaseModule()->getModule() == Std) {
      Module *Prelude;
      if (QC.GetBuiltinModule(Prelude, GetBuiltinModuleQuery::Prelude)) {
         return fail();
      }

      if (DC != Prelude->getDecl()) {
         MultiLevelLookupImpl(Result, sema(), *Prelude->getDecl(),
                              Name, true, false, true);
      }
   }

   if (Result.empty())
      return finish(SingleLevelLookupResult());

   return finish(move(Result.front()));
}

QueryResult MultiLevelLookupQuery::run()
{
   MultiLevelLookupResult Result;
   if (MultiLevelLookupImpl(Result, sema(), *DC, Name, true, LocalLookup,false))
      return fail();

   Module *Std;
   if (QC.GetBuiltinModule(Std, GetBuiltinModuleQuery::Std)) {
      return fail();
   }

   if (DC->getDeclModule()->getBaseModule()->getModule() == Std) {
      Module *Prelude;
      if (QC.GetBuiltinModule(Prelude, GetBuiltinModuleQuery::Prelude)) {
         return fail();
      }

      if (DC != Prelude->getDecl()) {
         MultiLevelLookupImpl(Result, sema(), *Prelude->getDecl(),
                              Name, true, false, false);
      }
   }

   return finish(move(Result));
}

QueryResult DirectLookupQuery::run()
{
   if (PrepareNameLookup) {
      if (QC.PrepareNameLookup(DC)) {
         return fail();
      }
   }

   MultiLevelLookupResult Result;

   auto DirectLookup = DC->lookup(Name);
   if (!DirectLookup.empty()) {
      Result.addResult(DirectLookup);
   }

   // Try looking in record extensions.
   if (LookInExtensions) {
      if (auto *R = dyn_cast<RecordDecl>(DC)) {
         auto Extensions = R->getExtensions();
         for (auto *Ext : Extensions) {
            DirectLookup = Ext->lookup(Name);
            if (!DirectLookup.empty()) {
               Result.addResult(DirectLookup);
            }
         }
      }
   }

   // If this context is a ModuleDecl, prepare all other declarations of that
   // module.
   else if (auto *ModDecl = dyn_cast<ModuleDecl>(DC)) {
      if (ModDecl->isPrimaryCtx()) {
         for (auto *OtherDecl : ModDecl->getModule()->getDecls()) {
            if (OtherDecl == ModDecl)
               continue;

            DirectLookup = OtherDecl->lookup(Name);
            if (!DirectLookup.empty()) {
               Result.addResult(DirectLookup);
            }
         }
      }
      else {
         const MultiLevelLookupResult *PrimaryRes;
         if (QC.DirectLookup(PrimaryRes, ModDecl->getPrimaryCtx(), Name,
                             LookInExtensions, PrepareNameLookup)) {
            return fail();
         }

         for (auto &Decls : *PrimaryRes) {
            Result.addResult(Decls);
         }
      }
   }

   // FIXME namespace redeclarations

   return finish(move(Result));
}

QueryResult NestedNameLookupQuery::run()
{
   DeclContext *CurDC = DC;

   unsigned i = 0;
   unsigned Depth = (unsigned)Names.size();

   for (DeclarationName Name : Names) {
      const MultiLevelLookupResult *LookupRes;
      if (QC.MultiLevelLookup(LookupRes, CurDC, Name, false, Loc)) {
         return fail();
      }

      if (LookupRes->empty()) {
         sema().diagnoseMemberNotFound(CurDC, StmtOrDecl(), Name,
                                       err_member_not_found, Loc);

         return fail();
      }

      if (i++ == Depth - 1) {
         return finish(MultiLevelLookupResult(*LookupRes));
      }

      if (LookupRes->size() != 1 || LookupRes->front().size() != 1) {
         sema().diagnose(err_ambiguous_reference, Name, Loc);
         return fail();
      }

      auto *SingleDecl = LookupRes->front().front();
      if (!isa<DeclContext>(SingleDecl)) {
         sema().diagnose(err_cannot_lookup_member_in,
                         SingleDecl->getSpecifierForDiagnostic(),
                         SingleDecl->getDeclName(), Loc);

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

   const MultiLevelLookupResult *LookupRes;
   if (QC.DirectLookup(LookupRes, DC, Decl->getDeclName(), LookInExtensions)) {
      return fail();
   }

   if (LookupRes->empty()) {
      return finish(nullptr);
   }

   if (auto *AT = dyn_cast<AssociatedTypeDecl>(Decl)) {
      for (auto *Impl : LookupRes->allDecls()) {
         auto *ATImpl = dyn_cast<AssociatedTypeDecl>(Impl);
         if (!ATImpl)
            continue;

         return finish(ATImpl);
      }

      return finish(nullptr);
   }

   if (auto *P = dyn_cast<PropDecl>(Decl)) {
      for (auto *Impl : LookupRes->allDecls()) {
         auto *PropImpl = dyn_cast<PropDecl>(Impl);
         if (!PropImpl)
            continue;

         if (QC.PrepareDeclInterface(PropImpl)) {
            return fail();
         }

         bool Equivalent;
         if (QC.CheckTypeEquivalence(Equivalent, P->getType(),
                                     PropImpl->getType(), Self,
                                     P->getDeclContext())) {
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

   if (auto *F = dyn_cast<FieldDecl>(Decl)) {
      for (auto *Impl : LookupRes->allDecls()) {
         auto *FieldImpl = dyn_cast<FieldDecl>(Impl);
         if (!FieldImpl)
            continue;

         if (QC.PrepareDeclInterface(FieldImpl)) {
            return fail();
         }

         if (F->isStatic() != FieldImpl->isStatic())
            continue;

         bool Equivalent;
         if (QC.CheckTypeEquivalence(Equivalent, F->getType(),
                                     FieldImpl->getType(), Self,
                                     F->getDeclContext())) {
            return fail();
         }

         if (!Equivalent)
            continue;

         return finish(FieldImpl);
      }

      return finish(nullptr);
   }

   if (auto *Sub = dyn_cast<SubscriptDecl>(Decl)) {
      for (auto *Impl : LookupRes->allDecls()) {
         auto *SubImpl = dyn_cast<SubscriptDecl>(Impl);
         if (!SubImpl)
            continue;

         if (QC.PrepareDeclInterface(SubImpl)) {
            return fail();
         }

         bool Equivalent;
         if (QC.CheckTypeEquivalence(Equivalent, Sub->getType(),
                                     SubImpl->getType(), Self,
                                     Sub->getDeclContext())) {
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

   if (auto *C = dyn_cast<CallableDecl>(Decl)) {
      for (auto *Impl : LookupRes->allDecls()) {
         if (C->getKind() != Impl->getKind())
            continue;

         auto *FnImpl = cast<CallableDecl>(Impl);
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