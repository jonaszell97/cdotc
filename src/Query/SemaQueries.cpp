//
// Created by Jonas Zell on 24.08.18.
//

#include "Query.h"

#include "AST/Decl.h"
#include "AST/TypeVisitor.h"
#include "Module/Module.h"
#include "Module/ModuleManager.h"
#include "Parse/Parser.h"
#include "QueryContext.h"
#include "Sema/ConformanceChecker.h"
#include "Sema/SemaPass.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::sema;
using namespace cdot::support;

QueryResult TypeCheckASTQuery::run()
{
   // Loop over the top level declarations in the source file and act on each
   // one accordingly.
   if (QC.TypeCheckDeclContext(Mod, Loc)) {
      return fail();
   }

   return finish();
}

QueryResult GetDefaultTemplateArgQuery::run()
{
   if (P->isVariadic()) {
      return finish(TemplateArgument(P, P->isTypeName(), {},
                                     P->getSourceLoc()));
   }
   if (P->isTypeName()) {
      return finish(TemplateArgument(P, sema().Context.getTemplateArgType(P),
                                     P->getSourceLoc()));
   }

   return finish(TemplateArgument(P, nullptr, P->getSourceLoc()));
}

QueryResult CreateSelfArgumentQuery::run()
{
   auto &Context = QC.CI.getContext();
   auto *SelfII = &Context.getIdentifiers().get("self");
   auto *Arg = FuncArgDecl::Create(Context, SelfLoc, SelfLoc,
                                   DeclarationName(SelfII),
                                   nullptr, ArgumentConvention::Default,
                                   SourceType(Self), nullptr,
                                   false, false, /*isSelf=*/true);

   return finish(Arg);
}

QueryResult FindPrecedenceGroupQuery::run()
{
   auto &DC = QC.CI.getGlobalDeclCtx();

   NamedDecl *LookupRes;
   if (QC.LookupSingle(LookupRes, &DC, Name, false, false, Loc)) {
      return fail();
   }

   if (!LookupRes) {
      if (DiagOnError) {
         sema().diagnose(err_precedence_group_does_not_exist, Name, Loc);
         return fail();
      }

      return finish(nullptr);
   }

   auto *PG = dyn_cast<PrecedenceGroupDecl>(LookupRes);
   if (!PG) {
      return finish(nullptr);
   }

   // Recursively look for the 'higherThan' and 'lowerThan' precedence
   // groups.
   if (auto *HT = PG->getHigherThanIdent()) {
      PrecedenceGroupDecl *HigherThan;
      if (QC.FindPrecedenceGroup(HigherThan, HT, true, Loc)) {
         return fail();
      }

      PG->setHigherThan(HigherThan);
   }
   if (auto *LT = PG->getLowerThanIdent()) {
      PrecedenceGroupDecl *LowerThan;
      if (QC.FindPrecedenceGroup(LowerThan, LT, true, Loc)) {
         return fail();
      }

      PG->setHigherThan(LowerThan);
   }

   return finish(PG);
}

QueryResult FindOperatorQuery::run()
{
   DC = DC->getDeclModule();

   NamedDecl *LookupRes;
   if (QC.LookupSingle(LookupRes, DC, Name, false, false, Loc)) {
      return fail();
   }

   if (!LookupRes) {
      if (DiagOnError) {
         sema().diagnose(err_undeclared_operator, Name, Loc);
      }

      return fail();
   }

   auto *Op = cast<OperatorDecl>(LookupRes);
   if (!Op->getPrecedenceGroup()) {
      if (!sema().declareStmt(Op)) {
         return fail();
      }
   }

   return finish(Op);
}

QueryResult ExpandMacrosQuery::run()
{
   // Get the raw AST.
   ast::ModuleDecl *Mod;
   if (QC.ParseSourceFile(Mod, SourceFile)) {
      return fail();
   }

   // Iteratively expand all macro declarations.
   std::queue<DeclContext*> ContextsToVisit;
   ContextsToVisit.push(Mod);

   while (!ContextsToVisit.empty()) {
      auto *DC = ContextsToVisit.front();
      ContextsToVisit.pop();

      for (auto *Decl : DC->getDecls()) {
         if (auto *InnerDC = dyn_cast<DeclContext>(Decl)) {
            ContextsToVisit.push(InnerDC);
         }
         else if (auto *MD = dyn_cast<MacroExpansionDecl>(Decl)) {
            ast::Decl *Expansion;
            if (QC.ExpandMacroDecl(Expansion, MD, Loc)) {
               return fail();
            }

            // Expansion will be visited as part of the for loop.
         }
      }
   }

   return finish();
}

QueryResult ExpandMacroQuery::run()
{
   NamedDecl *LookupRes;
   if (QC.LookupSingle(LookupRes, DC, Name, false, false)) {
      return fail();
   }

   auto Macro = dyn_cast_or_null<MacroDecl>(LookupRes);
   parse::ParseResult Result;

   if (!Macro) {
      auto BuiltinResult = parse::Parser::checkBuiltinMacro(
         sema(), Name, SOD, Tokens, (parse::Parser::ExpansionKind)ExpectedKind);

      if (BuiltinResult.second) {
         Result = BuiltinResult.first;
      }
      else {
         sema().diagnoseMemberNotFound(DC, SOD, Name,
                                       err_macro_does_not_exist);

         return fail();
      }
   }
   else if (Delim != Macro->getDelim()) {
      llvm::StringRef ExpectedDelim;
      switch (Macro->getDelim()) {
      case MacroDecl::Paren: ExpectedDelim = "()"; break;
      case MacroDecl::Brace: ExpectedDelim = "{}"; break;
      case MacroDecl::Square: ExpectedDelim = "[]"; break;
      }

      sema().diagnose(SOD, err_macro_expects_delim, SOD.getSourceRange(),
                      Name, ExpectedDelim);
   }

   if (Macro) {
      Result = parse::Parser::expandMacro(
         sema(), Macro, SOD, Tokens,
         (parse::Parser::ExpansionKind) ExpectedKind);
   }

   if (Result.holdsDecl()) {
      return finish(Result.getDecl());
   }
   if (Result.holdsExpr()) {
      return finish(Result.getExpr());
   }
   if (Result.holdsStatement()) {
      return finish(Result.getStatement());
   }

   return finish(StmtOrDecl());
}

QueryResult ExpandMacroDeclQuery::run()
{
   SemaPass::DeclScopeRAII DSR(sema(), Decl->getDeclContext());
   if (!sema().checkNamespaceRef(Decl))
      return fail();

   DeclContext *Ctx = Decl->getDeclContext();
   if (auto *Ident = cast_or_null<IdentifierRefExpr>(Decl->getParentExpr())) {
      if (Ident->getKind() == IdentifierKind::Namespace) {
         Ctx = Ident->getNamespaceDecl();
      }
      else if (Ident->getKind() == IdentifierKind::Import) {
         Ctx = Ident->getImport();
      }
      else if (Ident->getKind() == IdentifierKind::Module) {
         Ctx = Ident->getModule();
      }
   }

   StmtOrDecl Result;
   if (QC.ExpandMacro(Result, Decl, Decl->getMacroName(), Ctx, Decl->getDelim(),
                      Decl->getTokens(),
                      (unsigned)parse::Parser::ExpansionKind::Decl)) {
      return fail();
   }

   if (!Result)
      return finish(nullptr);

   return finish(Result.getDecl());
}

QueryResult ResolveUsingQuery::run()
{
   const MultiLevelLookupResult *LookupRes;
   if (QC.NestedNameLookup(LookupRes, U->getDeclContext(),
                           U->getNestedImportName(), Loc)) {
      return fail();
   }

   if (U->isWildcardImport()) {
      if (LookupRes->size() != 1 || LookupRes->front().size() != 1) {
         sema().diagnose(U, err_using_target_ambiguous, U->getSourceRange());

         NamedDecl *Cand1 = LookupRes->front().front();
         NamedDecl *Cand2;

         if (LookupRes->size() == 1) {
            Cand2 = LookupRes->front()[1];
         }
         else {
            Cand2 = (*LookupRes)[1].front();
         }

         sema().diagnose(note_candidate_here, Cand1->getSourceLoc());
         sema().diagnose(note_candidate_here, Cand2->getSourceLoc());

         return finish(DoneWithError);
      }

      NamedDecl *Target = LookupRes->front().front();
      if (!isa<DeclContext>(Target)) {
         sema().diagnose(U, err_cannot_lookup_member_in,
                         Target->getSpecifierForDiagnostic(),
                         Target->getDeclName());

         return finish(DoneWithError);
      }

      sema().makeDeclsAvailableIn(*U->getDeclContext(),
                                  *cast<DeclContext>(Target));
   }
   else {
      for (NamedDecl *ND : LookupRes->allDecls()) {
         sema().makeDeclAvailable(*U->getDeclContext(), ND);
      }
   }

   return finish();
}

QueryResult ResolveImportQuery::run()
{
   QC.CI.getModuleMgr().ImportModule(I);

   auto *Mod = I->getImportedModule();
   if (!Mod)
      return fail();

   if (I->isWildcardImport()) {
      I->getDeclContext()->addImportedModule(Mod);
      return finish();
   }

   if (I->getNamedImports().empty()) {
      I->addImportedModule(Mod);
      return finish();
   }

   bool Valid = true;
   for (auto &Name : I->getNamedImports()) {
      const SingleLevelLookupResult *LookupRes;
      if (QC.LookupFirst(LookupRes, Mod->getDecl(), Name)) {
         return fail();
      }

      if (LookupRes->empty()) {
         sema().diagnose(I, err_member_not_found, /*module*/ 9, Mod->getName(),
                         Name, I->getSourceRange());

         Valid = false;
         continue;
      }

      for (NamedDecl *ND : *LookupRes) {
         sema().makeDeclAvailable(*I->getDeclContext(), ND);
      }
   }

   return finish(Valid ? Done : DoneWithError);
}

QueryResult ResolveStaticIfQuery::run()
{
   // FIXME move this
   SemaPass::DeclScopeRAII DSR(sema(), Decl->getDeclContext());
   auto Result = sema().declareStmt(Decl);
   if (!Result)
      return fail();

   return finish(Result.get());
}

QueryResult ResolveStaticForQuery::run()
{
   // FIXME move this
   SemaPass::DeclScopeRAII DSR(sema(), Decl->getDeclContext());
   auto Result = sema().declareStmt(Decl);
   if (!Result)
      return fail();

   return finish(Result.get());
}

QueryResult ResolveStaticAssertQuery::run()
{
   // FIXME move this
   SemaPass::DeclScopeRAII DSR(sema(), Decl->getDeclContext());
   auto Result = sema().declareStmt(Decl);
   if (!Result)
      return fail();

   return finish();
}

QueryResult ResolveStaticPrintQuery::run()
{
   // FIXME move this
   SemaPass::DeclScopeRAII DSR(sema(), Decl->getDeclContext());
   auto Result = sema().declareStmt(Decl);
   if (!Result)
      return fail();

   return finish();
}

QueryResult ResolveConformanceToProtocolQuery::run()
{
   auto res = sema().visitSourceType(Conf);
   if (!res)
      return fail();

   auto protoTy = res.get();
   if (!protoTy->isRecordType()) {
      sema().diagnose(Conf.getTypeExpr(), err_conforming_to_non_protocol,
                      protoTy);
      return fail();
   }

   auto Proto = protoTy->getRecord();
   if (!isa<ProtocolDecl>(Proto)) {
      sema().diagnose(Conf.getTypeExpr(), err_conforming_to_non_protocol,
                      Proto->getDeclName());

      return fail();
   }

   QC.CheckAccessibility(T->getRecord(), Proto);

   auto PD = cast<ProtocolDecl>(Proto);
   if (PD->isTemplate())
      return finish(PD, Dependent);

   return finish(PD);
}

QueryResult ResolveDeclaredConformancesQuery::run()
{
   Status S = Done;
   for (auto &Conf : Conformances) {
      ProtocolDecl *PD;
      if (QC.ResolveConformanceToProtocol(PD, T, Conf)) {
         S = DoneWithError;
         continue;
      }

      // Prepare the protocol for lookup.
      if (QC.PrepareNameLookup(PD, Loc)) {
         S = DoneWithError;
         continue;
      }

      auto &Context = QC.CI.getContext();
      auto &ConfTable = Context.getConformanceTable();

      ConfTable.addExplicitConformance(Context, T->getRecord(), PD);

      // Add inherited conformances.
      for (auto *Conf : ConfTable.getExplicitConformances(PD)) {
         if (QC.AddInheritedConformance(T, Conf)) {
            S = DoneWithError;
            continue;
         }
      }
   }

   return finish(S);
}

QueryResult AddInheritedConformanceQuery::run()
{
   if (Proto->isTemplate())
      return finish(Dependent);

   // Prepare the protocol for lookup.
   if (QC.PrepareNameLookup(Proto, Loc)) {
      return fail();
   }

   auto &Context = QC.CI.getContext();
   auto &ConfTable = Context.getConformanceTable();

   ConfTable.addImplicitConformance(Context, T->getRecord(), Proto);

   // Recursively add inherited conformances.
   for (auto *Conf : ConfTable.getExplicitConformances(Proto)) {
      if (QC.AddInheritedConformance(T, Conf)) {
         return fail();
      }
   }

   return finish();
}

QueryResult GetReferencedAssociatedTypesQuery::run()
{
   const IdentifierInfo *LookupIdent;
   auto NestedName = C->getNameQualifier();

   if (NestedName.front()->isStr("Self")) {
      if (NestedName.size() == 1) {
         return finish({}, Done);
      }

      LookupIdent = NestedName[1];
   }
   else {
      LookupIdent = NestedName[0];
   }

   // Make sure extensions are resolved.
   if (QC.PrepareNameLookup(cast<DeclContext>(ConstrainedDecl))) {
      return fail();
   }

   auto *DC = cast<DeclContext>(ConstrainedDecl)->lookThroughExtension();
   const MultiLevelLookupResult *LookupRes;

   if (QC.DirectLookup(LookupRes, DC, LookupIdent, false)) {
      return fail();
   }

   if (LookupRes->empty()) {
      sema().diagnoseMemberNotFound(DC, ConstrainedDecl, LookupIdent);
      return fail();
   }
   if (LookupRes->size() != 1 || LookupRes->front().size() != 1) {
      sema().diagnose(err_ambiguous_reference, LookupIdent);

      for (auto *D : LookupRes->allDecls()) {
         sema().diagnose(note_candidate_here, D->getSourceLoc());
      }

      return fail();
   }

   auto *Decl = LookupRes->front().front();
   if (auto *AT = dyn_cast<AssociatedTypeDecl>(Decl)) {
      return finish({AT});
   }

   if (isa<TemplateParamDecl>(Decl)) {
      return finish({}, Done);
   }

   sema().diagnose(ConstrainedDecl, err_cannot_be_referenced_in_constraint,
                   Decl->getSpecifierForDiagnostic(),
                   Decl->getDeclName(), C->getSourceRange());

   return fail();
}

QueryResult ReferencedAssociatedTypesReadyQuery::run()
{
   RecordDecl *Rec = T->getRecord();
   DeclContext *Ctx = Rec->lookThroughExtension();

   ArrayRef<ExtensionDecl*> Extensions;
   if (auto *R = dyn_cast<RecordDecl>(Ctx)) {
      Extensions = sema().Context.getExtensions(T);
   }

   auto Constraints = QC.CI.getContext().getExtConstraints(ConstrainedDecl);
   for (DeclConstraint *C : Constraints) {
      ArrayRef<AssociatedTypeDecl*> ReferencedATs;
      if (QC.GetReferencedAssociatedTypes(ReferencedATs, ConstrainedDecl, C)) {
         return fail();
      }

      for (auto *AT : ReferencedATs) {
         // Check if the associated type is already visible.
         AssociatedTypeDecl *ATImpl;
         if (QC.GetAssociatedType(ATImpl, Rec, AT->getDeclName(), Extensions))
            return fail();

         if (!ATImpl) {
            return finish(false);
         }
      }
   }

   return finish(true);
}

QueryResult CheckProtocolExtensionApplicabilityQuery::run()
{
   auto Constraints = QC.CI.getContext().getExtConstraints(Ext);
   if (Constraints.empty())
      return finish(DoesApply);

   bool Ready;
   if (QC.ReferencedAssociatedTypesReady(Ready, T, Ext)) {
      return fail();
   }

   if (!Ready)
      return finish(CantTell);

   bool AllSatisfied;
   for (auto *C : Constraints) {
      if (QC.IsConstraintSatisfied(AllSatisfied, C, T, Ext)) {
         return fail();
      }

      if (!AllSatisfied) {
         return finish(DoesNotApply);
      }
   }

   return finish(DoesApply);
}

static DeclContextLookupResult
getAssociatedTypeOrTemplateParamImpl(DeclContext *R,
                                     DeclarationName Name,
                                     ArrayRef<ExtensionDecl*> Extensions) {
   auto Lookup = R->lookup(Name);
   if (!Lookup.empty())
      return Lookup;

   for (auto *Ext : Extensions) {
      Lookup = Ext->lookup(Name);
      if (!Lookup.empty())
         return Lookup;
   }

   return DeclContextLookupResult();
}

QueryResult VerifyConstraintQuery::run()
{
   DeclContext *CurCtx = cast<DeclContext>(ConstrainedDecl)
      ->lookThroughExtension();

   if (C->getKind() == DeclConstraint::Concept) {
      SemaPass::DeclScopeRAII DSR(sema(), CurCtx);

      auto *ConceptRef = C->getConceptRefExpr();
      ConceptRef->setAllowIncompleteTemplateArgs(true);

      auto Result = sema().visitExpr(ConstrainedDecl, ConceptRef);
      if (!Result)
          return fail();

      if (Result.get()->isDependent())
         return finish(Dependent);

      if (ConceptRef->getKind() != IdentifierKind::Alias) {
         sema().diagnose(ConceptRef, err_cannot_be_used_as_concept,
                         ConceptRef->getNamedDecl()->getSpecifierForDiagnostic(),
                         ConceptRef->getNamedDecl()->getDeclName(),
                         ConceptRef->getSourceRange());

         return fail();
      }

      auto *Concept = ConceptRef->getAlias();
      auto Params = Concept->getTemplateParams();

      if (Params.size() != 1 || !Params[0]->isTypeName()) {
         sema().diagnose(ConceptRef, err_not_a_valid_concept, 1,
                         ConceptRef->getSourceRange());

         return fail();
      }

      return finish();
   }

   if (!sema().visitSourceType(ConstrainedDecl, C->getType())) {
      return fail();
   }

   auto &Context = sema().Context;

   unsigned i = 0;
   ArrayRef<IdentifierInfo*> NameQual = C->getNameQualifier();
   unsigned NameQualSize = (unsigned)NameQual.size();

   ArrayRef<ExtensionDecl*> Extensions;

   for (auto *Ident : NameQual) {
      if (Ident->isStr("Self") && i == 0) {
         ++i;
         continue;
      }

      if (auto *R = dyn_cast<RecordDecl>(CurCtx)) {
         Extensions = sema().Context.getExtensions(R->getType());
      }
      else {
         Extensions = {};
      }

      auto Result = getAssociatedTypeOrTemplateParamImpl(CurCtx, Ident,
                                                         Extensions);

      if (!Result) {
         sema().diagnose(ConstrainedDecl, err_member_not_found,
                         cast<NamedDecl>(CurCtx)->getSpecifierForDiagnostic(),
                         cast<NamedDecl>(CurCtx)->getDeclName(),
                         Ident->getIdentifier(), C->getSourceRange());

         return fail();
      }
      if (Result.size() != 1) {
         sema().diagnose(ConstrainedDecl, err_ambiguous_reference,
                         Ident->getIdentifier(), C->getSourceRange());

         return fail();
      }

      if (auto *AT = dyn_cast<AssociatedTypeDecl>(Result.front())) {
         if (i == NameQualSize - 1)
            break;

         auto Cov = Context.getCovariance(AT);
         if (Cov.empty()) {
            sema().diagnose(ConstrainedDecl, err_unconstrained_lookup,
                            AT->getDeclName(), C->getSourceRange());

            return fail();
         }

         // FIXME multiple covariances
         CurCtx = Cov.front();
      }
      else if (auto *P = dyn_cast<TemplateParamDecl>(Result.front())) {
         if (i == NameQualSize - 1)
            break;

         auto Ty = P->getCovariance();
         if (Ty->isUnknownAnyType())
            break;

         if (!Ty->isRecordType()) {
            sema().diagnose(ConstrainedDecl, err_generic_error,
                            "cannot lookup member in type",
                            C->getSourceRange());

            return fail();
         }

         CurCtx = Ty->getRecord();
      }
      else {
         sema().diagnose(ConstrainedDecl,
                         err_cannot_be_referenced_in_constraint,
                         Result.front()->getSpecifierForDiagnostic(),
                         Result.front()->getDeclName(),
                         C->getSourceRange());

         return fail();
      }

      ++i;
   }

   return finish();
}

QueryResult IsConstraintSatisfiedQuery::run()
{
   if (QC.VerifyConstraint(C, OriginalDecl)) {
      return fail();
   }

   // FIXME generalized extensions
   auto *Rec = Self->getRecord();

   auto NameQual = C->getNameQualifier();
   RecordDecl *CurCtx = Rec;
   QualType ConstrainedType;

   unsigned i = 0;
   for (auto *Ident : NameQual) {
      if (Ident->isStr("Self") && i == 0) {
         ++i;
         ConstrainedType = Rec->getType();

         continue;
      }

      // We can't use a lookup query here because this query should never
      // trigger PrepareNameLookup.
      auto LookupRes = getAssociatedTypeOrTemplateParamImpl(
         CurCtx, Ident, CurCtx->getExtensions());

      // Might be a template parameter.
      if (LookupRes.empty()) {
         assert(CurCtx->isInstantiation());

         auto *Arg = CurCtx->getTemplateArgs().getNamedArg(Ident);
         assert(Arg && "invalid constraint");
         assert(Arg->isType() && !Arg->isVariadic() && "invalid constraint!");

         ConstrainedType = Arg->getType();
      }
      else {
         assert(LookupRes
                && LookupRes.size() == 1
                && "invalid constraint!");

         NamedDecl *Decl = LookupRes.front();
         if (auto *AT = dyn_cast<AssociatedTypeDecl>(Decl)) {
            if (!AT || !AT->isImplementation()) {
               llvm_unreachable("constraint is not ready!");
            }

            ConstrainedType = AT->getActualType();
         }
         else {
            assert(isa<TemplateParamDecl>(Decl) && "invalid constraint");

            // Constraint is dependent on a template parameter,
            // can't tell for now.
            return finish(true);
         }
      }

      if (ConstrainedType->isRecordType()) {
         CurCtx = ConstrainedType->getRecord();
      }
      else {
         assert(i == NameQual.size() - 1 && "invalid constraint!");
         break;
      }

      ++i;
   }

   return finish(sema().checkDeclConstraint(Rec, ConstrainedType, C));
}

static AssociatedTypeDecl* makeAssociatedType(SemaPass &Sema,
                                              AssociatedTypeDecl *AT,
                                              RecordDecl *R) {
   QualType Inst;
   if (Sema.QC.SubstAssociatedTypes(Inst, AT->getActualType(), R->getType())) {
      return nullptr;
   }

   auto *InstDecl = AssociatedTypeDecl::Create(Sema.getContext(),
                                               AT->getSourceLoc(), nullptr,
                                               AT->getDeclName(),
                                               SourceType(Inst),
                                               SourceType(), true);

   Sema.addDeclToContext(*R, InstDecl);
   return InstDecl;
}

static void addImplicitSelf(SemaPass &Sema, RecordDecl *R)
{
   auto *SelfII = Sema.getIdentifier("Self");
   if (R->lookupSingle<AssociatedTypeDecl>(SelfII))
      return;

   bool IsImplementation = !isa<ProtocolDecl>(R);

   SourceType ActualType;
   SourceType Covariance;

   QualType CanonicalTy;

   auto &Context = Sema.getContext();
   if (R->isTemplate()) {
      SmallVector<TemplateArgument, 2> TemplateArgs;
      for (auto *P : R->getTemplateParams()) {
         const TemplateArgument *Arg;
         if (Sema.QC.GetDefaultTemplateArg(Arg, P)) {
            return;
         }

         TemplateArgs.emplace_back(Arg->clone());
      }

      auto *List = FinalTemplateArgumentList::Create(Context, TemplateArgs);
      CanonicalTy = Context.getDependentRecordType(R, List);
   }
   else {
      CanonicalTy = Context.getRecordType(R);
   }

   if (IsImplementation) {
      ActualType = CanonicalTy;
   }
   else {
      Covariance = CanonicalTy;
   }

   auto *Self = AssociatedTypeDecl::Create(Context, R->getSourceLoc(),
                                           nullptr, SelfII,
                                           ActualType, Covariance,
                                           IsImplementation);

   Self->setSynthesized(true);

   if (!Self->isImplementation()) {
      Context.getAssociatedType(Self)->setCanonicalType(CanonicalTy);
   }

   Sema.ActOnDecl(R, Self);
}

QueryResult ResolveAssociatedTypesQuery::run()
{
   using ResultKind = CheckProtocolExtensionApplicabilityQuery::ResultKind;

   // FIXME generalized extensions
   auto *Rec = T->getRecord();
   auto &ConfTable = QC.CI.getContext().getConformanceTable();

   addImplicitSelf(sema(), Rec);

   // FIXME generalized extensions
   if (QC.ResolveDeclaredConformances(T, Rec->getConformanceTypes())) {
      return fail();
   }

   if (isa<ProtocolDecl>(Rec))
      return finish();

   SmallVector<AssociatedTypeDecl*, 4> NeededAssociatedTypes;
   SmallVector<ExtensionDecl*, 4> UnresolvedExtensions;
   SmallPtrSet<const IdentifierInfo*, 4> ImplementedATs;

   for (auto &Conf : ConfTable.getAllConformances(Rec)) {
      auto *Proto = Conf->getProto();
      NeededAssociatedTypes.append(Proto->decl_begin<AssociatedTypeDecl>(),
                                   Proto->decl_end<AssociatedTypeDecl>());

      // Make sure extensions are ready.
//      if (QC.FindExtensions()) {
//         return fail();
//      }

      auto Extensions = Proto->getExtensions();
      UnresolvedExtensions.append(Extensions.begin(), Extensions.end());
   }

   SemaPass::DeclScopeRAII DSR(sema(), Rec);
   auto Extensions = Rec->getExtensions();

   // First of all, resolve the associated types that are directly provided
   // by the type.
   for (auto *AT : NeededAssociatedTypes) {
      // Associated types must be visible in the immediate context (or in the
      // immediate context of an extension) to avoid a circular dependency
      // with the lookup here.
      AssociatedTypeDecl *Impl;
      if (QC.GetAssociatedType(Impl, Rec, AT->getDeclName(), Extensions))
         return fail();

      if (!Impl) {
         if (!AT->getActualType())
            continue;

         auto *Inst = makeAssociatedType(sema(), AT, Rec);
         if (!Inst)
            return fail();

         ImplementedATs.insert(AT->getDeclName().getIdentifierInfo());
         continue;
      }

      ImplementedATs.insert(AT->getDeclName().getIdentifierInfo());

      if (QC.PrepareDeclInterface(Impl)) {
         return fail();
      }
   }

   Status S = Done;

   // Resolve extensions until there are no more changes or we resolved all
   // of them.
   unsigned i = 0;
   bool FoundChanges = true;

   while (FoundChanges) {
      FoundChanges = false;

      auto CheckUntil = (unsigned)UnresolvedExtensions.size();
      while (i < CheckUntil) {
         auto *Ext = UnresolvedExtensions[i++];

         bool Ready;
         if (QC.ReferencedAssociatedTypesReady(Ready, T, Ext, Loc)) {
            return fail();
         }

         if (!Ready) {
            UnresolvedExtensions.push_back(Ext);
            continue;
         }

         // All associated types are ready, we can check whether or not the
         // extension applies.
         ResultKind Applies;
         if (QC.CheckProtocolExtensionApplicability(Applies, T, Ext, Loc)) {
            return fail();
         }

         assert(Applies != ResultKind::CantTell && "should not be dependent!");
         if (Applies == ResultKind::DoesNotApply)
            continue;

         // The extension applies, instantiate all default implementations
         // for associated types.
         for (auto *AT : Ext->getDecls<AssociatedTypeDecl>()) {
            auto *Name = AT->getDeclName().getIdentifierInfo();
            if (!ImplementedATs.insert(Name).second)
               continue;

            if (QC.PrepareDeclInterface(AT)) {
               S = DoneWithError;
               continue;
            }

            auto *Inst = makeAssociatedType(sema(), AT, Rec);
            if (!Inst)
               return fail();

            FoundChanges = true;
         }
      }
   }

   if (ImplementedATs.size() != NeededAssociatedTypes.size()) {
      bool FoundMissing = false;
      for (auto *AT : NeededAssociatedTypes) {
         if (ImplementedATs.find(AT->getIdentifierInfo())
               != ImplementedATs.end())
            continue;

         FoundMissing = true;

         sema().diagnose(Rec, err_incorrect_protocol_impl,
                         Rec->getSpecifierForDiagnostic(),
                         Rec->getDeclName(),
                         AT->getRecord()->getDeclName(),
                         Rec->getSourceLoc());

         sema().diagnose(note_associated_type_missing, AT->getDeclName(),
                         AT->getSourceLoc());
      }

      if (FoundMissing) {
         return fail();
      }
   }

   return finish(S);
}

QueryResult CheckConformancesQuery::run()
{
   if (sema::checkConformance(sema(), T->getRecord())) {
      return fail();
   }

   return finish();
}

QueryResult CheckSingleConformanceQuery::run()
{
   if (sema::checkConformanceToProtocol(sema(), T->getRecord(), P)) {
      return fail();
   }

   return finish();
}

QueryResult ConformsToQuery::run()
{
   auto &ConfTable = QC.CI.getContext().getConformanceTable();

   // FIXME generalized extensions
   return finish(ConfTable.conformsTo(T->getRecord(), P));
}

QueryResult IsAccessibleQuery::run()
{
//   if (Bits.InUnitTest && IsTestable(ND))
//      return true;

   auto AccessSpec = ND->getAccess();

   switch (AccessSpec) {
   case AccessSpecifier::Default:
      llvm_unreachable("didn't remove default access specifier from decl!");
   case AccessSpecifier::Public:
      return finish(true);
   case AccessSpecifier::Private: {
      // only visible within the immediate context the symbol was defined in
      auto *Ctx = ND->getDeclContext();

      // All extensions within the same file can access private declarations.
      bool SameFile = ND->getModule() == DC->getDeclModule();

      if (SameFile)
         Ctx = Ctx->lookThroughExtension();

      for (auto *Curr = DC; Curr; Curr = Curr->getParentCtx()) {
         if (SameFile && Curr->lookThroughExtension() == Ctx) {
            return finish(true);
         }
         if (Curr == Ctx) {
            return finish(true);
         }
      }

      return finish(false);
   }
   case AccessSpecifier::Protected: {
      // only visible within declaration context or subclasses (should have
      // been rejected outside of classes)
      auto C = cast<ClassDecl>(ND->getNonTransparentDeclContext());
      auto *Ctx = ND->getDeclContext();
      for (auto *Curr = DC; Curr; Curr = Curr->getParentCtx()) {
         if (Curr->lookThroughExtension() == Ctx->lookThroughExtension())
            return finish(true);

         auto SubClass = dyn_cast<ClassDecl>(Curr);
         if (SubClass && C->isBaseClassOf(SubClass))
            return finish(true);
      }

      return finish(false);
   }
   case AccessSpecifier::FilePrivate: {
      // visible within the file it was declared
      auto &FileMgr = QC.CI.getFileMgr();
      auto DeclID = FileMgr.getSourceId(ND->getSourceLoc());
      auto CurrID = FileMgr.getSourceId(cast<Decl>(DC)->getSourceLoc());

      return finish(DeclID == CurrID || FileMgr.wasIncludedFrom(CurrID, DeclID));
   }
   case AccessSpecifier::Internal: {
      return finish(DC->getDeclModule()->getBaseModule()
             == ND->getModule()->getBaseModule());
   }
   }
}

QueryResult CheckAccessibilityQuery::run()
{
//   if (Bits.InUnitTest && IsTestable(ND))
//      return;

   auto AccessSpec = ND->getAccess();

   switch (AccessSpec) {
   case AccessSpecifier::Default:
      llvm_unreachable("didn't remove default access specifier from decl!");
   case AccessSpecifier::Public:
      return finish();
   case AccessSpecifier::Private: {
      // only visible within the immediate context the symbol was defined in
      auto *Ctx = ND->getDeclContext();

      // All extensions within the same file can access private declarations.
      bool SameFile = ND->getModule() == DC->getDeclModule();

      if (SameFile)
         Ctx = Ctx->lookThroughExtension();

      for (auto *Curr = DC; Curr; Curr = Curr->getParentCtx()) {
         if (SameFile && Curr->lookThroughExtension() == Ctx) {
            return finish();
         }
         if (Curr == Ctx) {
            return finish();
         }
      }

      // declaration is not accessible here
      sema().diagnose(err_private_access, ND->getSpecifierForDiagnostic(),
                      ND->getDeclName(), Loc);

      break;
   }
   case AccessSpecifier::Protected: {
      // only visible within declaration context or subclasses (should have
      // been rejected outside of classes)
      auto C = cast<ClassDecl>(ND->getNonTransparentDeclContext());
      auto *Ctx = ND->getDeclContext();
      for (auto *Curr = DC; Curr; Curr = Curr->getParentCtx()) {
         if (Curr->lookThroughExtension() == Ctx->lookThroughExtension())
            return finish();

         auto SubClass = dyn_cast<ClassDecl>(Curr);
         if (SubClass && C->isBaseClassOf(SubClass))
            return finish();
      }

      // declaration is not accessible here
      sema().diagnose(err_protected_access, ND->getSpecifierForDiagnostic(),
                      ND->getDeclName(), C->getDeclName(), Loc);

      break;
   }
   case AccessSpecifier::FilePrivate: {
      // visible within the file it was declared
      auto &FileMgr = QC.CI.getFileMgr();
      auto DeclID = FileMgr.getSourceId(ND->getSourceLoc());
      auto CurrID = FileMgr.getSourceId(cast<Decl>(DC)->getSourceLoc());

      if (DeclID == CurrID || FileMgr.wasIncludedFrom(CurrID, DeclID))
         return finish();

      // declaration is not accessible here
      sema().diagnose(err_fileprivate_access, ND->getSpecifierForDiagnostic(),
                      ND->getDeclName(), FileMgr.getFileName(DeclID),
                      Loc);

      break;
   }
   case AccessSpecifier::Internal: {
      if (DC->getDeclModule()->getBaseModule()
          == ND->getModule()->getBaseModule()) {
         return finish();
      }

      sema().diagnose(err_internal_access, ND->getSpecifierForDiagnostic(),
                      ND->getDeclName(),
                      ND->getModule()->getBaseModule()->getDeclName(),
                      Loc);

      break;
   }
   }

   sema().diagnose(note_access_spec_here, /*implicitly*/ !ND->getAccessRange(),
                   (int)AccessSpec, ND->getAccessRange(), ND->getSourceLoc());

   return finish(DoneWithError);
}

QueryResult EquivalentSignaturesQuery::run()
{
   auto Args1 = C1->getArgs();
   auto Args2 = C2->getArgs();

   unsigned NumArgs = (unsigned)Args1.size();
   if (Args1.size() != Args2.size())
      return finish(ParamCountMismatch);

   bool Equivalent;
   if (QC.CheckTypeEquivalence(Equivalent, C1->getReturnType(),
                               C2->getReturnType(), Self, C1)) {
      return fail();
   }

   if (!Equivalent)
      return finish(ReturnTypeMismatch);

   if (C1->isStatic() != C2->isStatic())
      return finish(StaticMismatch);

   unsigned i = 0;
   if ((C1->isNonStaticMethod() && C2->isNonStaticMethod())
       || (C1->isBaseInitializer() && C2->isBaseInitializer())) {
      ++i;
   }

   for (; i < NumArgs; ++i) {
      auto &Arg = Args1[i];
      auto &Other = Args2[i];

      if (QC.CheckTypeEquivalence(Equivalent, Arg->getType(), Other->getType(),
                                  Self, C1)) {
         return fail();
      }

      if (!Equivalent)
         return finish(ParamCountMismatch); // incompatible signature

      if (Arg->getLabel() != Other->getLabel())
         return finish(LabelMismatch); // incompatible labels
   }

   if (C1->throws())
      if (!C2->throws())
         return finish(ThrowsMismatch);

   if (C1->isUnsafe())
      if (!C2->isUnsafe())
         return finish(UnsafeMismatch);

   if (C1->isAsync())
      if (!C2->isAsync())
         return finish(AsyncMismatch);

   return finish(ResultKind::Equivalent);
}

QueryResult PrepareDeclInterfaceQuery::run()
{
   SemaPass::DeclScopeRAII DSR(sema(), D->getDeclContext());
   sema().checkDeclAttrs(D, Attr::BeforeDeclaration);

#  define PREPARE_DECL(NAME)                                              \
   case Decl::NAME##DeclID:                                               \
      if (QC.Prepare##NAME##Interface(static_cast<NAME##Decl*>(D), Loc))  \
         return fail();                                                   \
      break;

   switch (D->getKind()) {
   PREPARE_DECL(Function)
   PREPARE_DECL(Method)
   PREPARE_DECL(Init)
   PREPARE_DECL(Deinit)
   PREPARE_DECL(Struct)
   PREPARE_DECL(Class)
   PREPARE_DECL(Enum)
   PREPARE_DECL(Protocol)
   PREPARE_DECL(Extension)
   PREPARE_DECL(FuncArg)
   PREPARE_DECL(TemplateParam)
   PREPARE_DECL(AssociatedType)
   PREPARE_DECL(Field)
   PREPARE_DECL(Prop)
   PREPARE_DECL(Subscript)
   case Decl::LocalVarDeclID:
      return finish();
   default:
      llvm_unreachable("invalid declaration passed to PrepareDeclInterface");
      break;
   }

#  undef PREPARE_DECL

   sema().checkDeclAttrs(D, Attr::AfterDeclaration);
   return finish();
}

QueryResult TypecheckDeclQuery::run()
{
   if (auto *L = dyn_cast<LocalVarDecl>(D)) {
      auto Result = sema().visitLocalVarDecl(L);
      if (!Result)
         return fail();

      return finish();
   }

   if (QC.PrepareDeclInterface(D)) {
      return fail();
   }

   SemaPass::DeclScopeRAII DSR(sema(), D->getDeclContext());
   sema().checkDeclAttrs(D, Attr::BeforeSema);

#  define TYPECHECK_DECL(NAME)                                    \
   case Decl::NAME##DeclID:                                       \
      if (QC.Typecheck##NAME(static_cast<NAME##Decl*>(D), Loc))   \
         return fail();                                           \
      break;

   switch (D->getKind()) {
   TYPECHECK_DECL(Function)
   TYPECHECK_DECL(Method)
   TYPECHECK_DECL(Init)
   TYPECHECK_DECL(Deinit)
   TYPECHECK_DECL(Struct)
   TYPECHECK_DECL(Class)
   TYPECHECK_DECL(Enum)
   TYPECHECK_DECL(Protocol)
   TYPECHECK_DECL(Extension)
   TYPECHECK_DECL(FuncArg)
   TYPECHECK_DECL(TemplateParam)
   TYPECHECK_DECL(AssociatedType)
   TYPECHECK_DECL(Field)
   TYPECHECK_DECL(Prop)
   TYPECHECK_DECL(Subscript)
   default:
      break;
   }

#  undef TYPECHECK_DECL

   sema().checkDeclAttrs(D, Attr::AfterSema);
   return finish();
}

QueryResult TypeCheckDeclContextQuery::run()
{
   for (auto *D : DC->getDecls()) {
      if (QC.TypecheckDecl(D, Loc)) {
         return fail();
      }
   }

   return finish();
}

QueryResult PrepareFuncArgInterfaceQuery::run()
{
   auto &Context = QC.CI.getContext();
   if (D->isSelf()) {
      auto M = cast<MethodDecl>(D->getDeclContext());
      auto R = M->getRecord();

      QualType SelfTy = Context.getRecordType(R);
      ArgumentConvention Conv;

      if (M->isStatic() && !M->isBaseInitializer()) {
         SelfTy = Context.getMetaType(SelfTy);
         Conv = ArgumentConvention::Borrowed;
      }
      else {
         if (M->hasMutableSelf()) {
            SelfTy = Context.getMutableBorrowType(SelfTy);
            Conv = ArgumentConvention::MutableRef;
         }
         else {
            Conv = ArgumentConvention::Borrowed;
         }
      }

      D->setConvention(Conv);
      D->getType().setResolvedType(SelfTy);

      if (SelfTy->isDependentType())
         D->setIsTypeDependent(true);

      return finish();
   }

   auto TypeRes = sema().visitSourceType(D, D->getType());
   if (!TypeRes || TypeRes.get()->isErrorType()) {
      return fail();
   }

   auto &DeclaredArgType = D->getType();
   if (DeclaredArgType->isVoidType()) {
      SourceLocation Loc;
      if (auto *E = D->getType().getTypeExpr()) {
         Loc = E->getSourceLoc();
      }
      else {
         Loc = D->getSourceLoc();
      }

      sema().diagnose(D, err_generic_error,
                      "function arguments may not be of type 'void'",
                      Loc);
   }

   sema().checkIfTypeUsableAsDecl(DeclaredArgType, D);

   if (D->getConvention() == ArgumentConvention::Default) {
      auto Fn = cast<CallableDecl>(D->getDeclContext());
      // initializer arguments are owned by default
      if (isa<InitDecl>(Fn) || isa<EnumCaseDecl>(Fn)) {
         D->setConvention(ArgumentConvention::Owned);
      }
      // otherwise an immutable borrow is the default
      else {
         D->setConvention(ArgumentConvention::Borrowed);
      }
   }

   return finish();
}

static void calculateDepth(TemplateParamDecl *P)
{
   unsigned Depth = 0;
   auto *Ctx = P->getDeclContext();
   while (Ctx) {
      Ctx = Ctx->lookThroughExtension();

      if (auto *ND = dyn_cast<NamedDecl>(Ctx)) {
         Depth += ND->isTemplate();
      }

      Ctx = Ctx->getParentCtx();
   }

   assert(Depth >= 1);
   P->setDepth(Depth - 1);
}

QueryResult PrepareTemplateParamInterfaceQuery::run()
{
   if (D->isSynthesized())
      return finish();

   calculateDepth(D);

   NamedDecl *OtherParam;
   if (QC.LookupSingle(OtherParam, D->getDeclContext()->getParentCtx(),
                       D->getDeclName(), false, false, Loc)) {
      sema().diagnose(D, err_template_param_shadow, D->getName(),
                      D->getSourceRange());
      sema().diagnose(note_template_parameter_here,
                      OtherParam->getSourceRange());
   }

   if (auto &cov = D->getCovariance()) {
      if (!sema().visitSourceType(D, cov))
         return finish();
   }

   if (auto &con = D->getContravariance()) {
      if (!sema().visitSourceType(D, con))
         return finish();
   }

   if (D->getCovariance()->isAutoType()) {
      if (D->isTypeName()) {
         // Make no assumptions about unbounded generics.
         if (D->isUnbounded()) {
            D->getValueType().setResolvedType(sema().UnknownAnyTy);
            cast<Decl>(D->getDeclContext())->setUnboundedTemplate(true);
         }
         // Otherwhise use 'Any' as an upper bound.
         else {
            ProtocolDecl *Any;
            if (QC.GetBuiltinProtocol(Any, GetBuiltinProtocolQuery::Any, Loc)) {
               return fail();
            }

            D->getValueType().setResolvedType(
               sema().Context.getRecordType(Any));
         }
      }
      else {
         // Value arguments default to 'Int'.
         D->getValueType().setResolvedType(sema().Context.getIntTy());
      }
   }

   if (D->isVariadic()) {
      // Variadic templates need a full instantiation.
      cast<Decl>(D->getDeclContext())->setUnboundedTemplate(true);
   }

   // Update the TemplateParamType associated with this declaration
   QC.CI.getContext()
        .getTemplateArgType(D)->setCanonicalType(D->getCovariance());

   // FIXME move this check to the point where it's first needed.
   if (auto Def = D->getDefaultValue()) {
      (void) sema().visitExpr(D, Def);
   }

   return finish();
}

QueryResult TypecheckFuncArgQuery::run()
{
   QualType DeclaredArgType = D->getType();
   if (Expression *DefaultVal = D->getDefaultVal()) {
      SemaPass::DefaultArgumentValueRAII defaultArgumentValueRAII(sema());

      auto Result = sema().getAsOrCast(D, DefaultVal, DeclaredArgType);
      if (Result)
         D->setDefaultVal(Result.get());
   }

   return finish();
}

QueryResult TypecheckTemplateParamQuery::run()
{
   return finish();
}

static bool checkCompileTimeEvaluable(SemaPass &SP, DeclContext *Ctx)
{
   auto *Std = SP.getStdModule()->getDecl();
   while (Ctx) {
      if (Ctx->getPrimaryCtx() == Std)
         return true;

      if (auto *D = dyn_cast<Decl>(Ctx)) {
         if (D->hasAttribute<CompileTimeAttr>()) {
            return true;
         }
         if (auto *Ext = dyn_cast<ExtensionDecl>(Ctx)) {
            if (checkCompileTimeEvaluable(SP, Ext->getExtendedRecord())) {
               return true;
            }
         }
      }

      Ctx = Ctx->getParentCtx();
   }

   return false;
}

QueryResult PrepareAssociatedTypeInterfaceQuery::run()
{
   if (D->getCovariance()) {
      if (D->isImplementation()) {
         sema().diagnose(D, err_generic_error,
                         "cannot define covariance on an associated type "
                         "definition",
                         D->getSourceRange());
      }

      auto TypeRes = sema().visitSourceType(D, D->getCovariance());
      if (!TypeRes)
         return fail();

      // update the AssociatedType associated with this declaration
      sema().Context.getAssociatedType(D)
            ->setCanonicalType(D->getCovariance());
   }
   else if (!D->isImplementation()) {
      ProtocolDecl *Any;
      if (QC.GetBuiltinProtocol(Any, GetBuiltinProtocolQuery::Any)) {
         Any = nullptr;
      }

      if (Any) {
         D->getCovariance().setResolvedType(sema().Context.getRecordType(Any));

         // update the AssociatedType associated with this declaration
         sema().Context.getAssociatedType(D)
               ->setCanonicalType(D->getCovariance());
      }
   }

   if (D->getActualType()) {
      auto TypeRes = sema().visitSourceType(D, D->getActualType());
      if (!TypeRes)
         return fail();

      auto Ty = TypeRes.get();
      if (Ty->isAutoType()) {
         D->getActualType().setResolvedType(sema().UnknownAnyTy);
      }

      if (D->isImplementation()) {
         // update the AssociatedType associated with this declaration
         sema().Context.getAssociatedType(D)
                       ->setCanonicalType(D->getActualType());
      }
   }

   // Validate the constraints first.
   sema().visitConstraints(D);

   return finish();
}

QueryResult TypecheckAssociatedTypeQuery::run()
{
   auto *ATDecl = D;
   auto Rec = ATDecl->getRecord();

   // must be in a dependent extension decl, ignore for now
   if (!Rec)
      return finish();

   if (ATDecl->getDeclName().isStr("Self"))
      return finish();

   auto &Context = sema().Context;
   if (ATDecl->isImplementation()) {
      AssociatedTypeDecl* AT = nullptr;
      ProtocolDecl* Proto = nullptr;

      if (auto *P = dyn_cast<ProtocolDecl>(Rec)) {
         auto MaybeAT = P->getAssociatedType(ATDecl->getDeclName());
         if (MaybeAT) {
            AT = MaybeAT;
            Proto = P;
         }
      }

      if (!AT) {
         auto Conformances = Context.getConformanceTable().getAllConformances(Rec);
         for (auto *Conf : Conformances) {
            auto *CF = Conf->getProto();
            auto MaybeAT = CF->getAssociatedType(ATDecl->getDeclName());
            if (MaybeAT) {
               AT = MaybeAT;
               Proto = CF;

               break;
            }
         }
      }

      if (!AT) {
         sema().diagnose(ATDecl, err_no_such_associated_type,
                         Rec->getSpecifierForDiagnostic(), Rec->getDeclName(),
                         ATDecl->getDeclName(), ATDecl->getSourceLoc());

         return fail();
      }

      if (!ATDecl->getActualType()) {
         if (!AT->getActualType()) {
            sema().diagnose(ATDecl, err_associated_type_no_default,
                            ATDecl->getDeclName(), ATDecl->getSourceLoc());

            return fail();
         }

         ATDecl->setActualType(AT->getActualType());
      }

      // Constraints will be checked during protocol conformance checking.
   }

   return finish();
}

QueryResult PrepareCallableInterfaceQuery::run()
{
   // an error occured while parsing this function
   if (D->isInvalid())
      return fail();

   // Lookup program argument declarations.
   if (D->isMain()) {
      Module *SysMod;
      if (QC.GetBuiltinModule(SysMod, GetBuiltinModuleQuery::Sys)) {
         return fail();
      }

      if (SysMod) {
         NamedDecl *_;
         QC.LookupSingle(_, SysMod->getDecl(), sema().getIdentifier("argc"));
         QC.LookupSingle(_, SysMod->getDecl(), sema().getIdentifier("argv"));
      }
   }

   // Check duplicate extern C functions.
   if (D->isExternC()) {
      if (!isa<FunctionDecl>(D)) {
         sema().diagnose(D, err_generic_error, "methods cannot be @extern(C)",
                         D->getSourceLoc());
      }

      if (!D->getDeclName().isSimpleIdentifier()) {
         sema().diagnose(D, err_cannot_be_extern_C, D->getSourceLoc());
      }
      else if (D->getBody()) {
         auto Ret = QC.ExternCFuncs.try_emplace(
            D->getDeclName().getIdentifierInfo());

         if (!Ret.second) {
            sema().diagnose(D, err_redeclared_extern_c, D->getSourceLoc(),
                            D->getDeclName());

            auto *OtherFn = Ret.first->getSecond();
            sema().diagnose(note_previous_decl, OtherFn->getSourceLoc());
         }
      }
   }

   // Check if this function is callable at compile time.
   if (!D->isCompileTimeEvaluable() && checkCompileTimeEvaluable(sema(), D)) {
      D->setCompileTimeEvaluable(true);
   }

   // FIXME
//   for (auto &TP : D->getTemplateParams())
//      (void) declareStmt(D, TP);

   auto &Context = QC.CI.getContext();

   SemaPass::DeclScopeRAII declContextRAII(sema(), D);
   sema().visitConstraints(D);

   if (D->isOperator()) {
      auto OpName = D->getDeclName();
      if (OpName.getKind() == DeclarationName::InstantiationName)
         OpName = OpName.getInstantiationName();

      auto OpDeclName = Context.getDeclNameTable().getOperatorDeclName(OpName);

      OperatorDecl *Result;
      if (QC.FindOperator(Result, OpDeclName, D->getDeclContext(), false)) {
         return fail();
      }

      if (!Result) {
         sema().diagnose(D, err_undeclared_operator, D->getSourceLoc(), OpName);
      }
      else {
         D->setPrecedenceGroup(Result->getPrecedenceGroup());
      }
   }

   for (FuncArgDecl *ArgDecl : D->getArgs()) {
      if (QC.PrepareFuncArgInterface(ArgDecl, Loc)) {
         return fail();
      }

      if (D->isProtocolRequirement() && !D->isProtocolDefaultImpl()) {
         QualType Type = ArgDecl->getType();
         if (sema().ContainsAssociatedTypeConstraint(Type)) {
            cast<ProtocolDecl>(D->getRecord())
               ->setHasAssociatedTypeConstraint(true);
         }
      }
   }

   auto &Ret = D->getReturnType();
   auto Result = sema().visitSourceType(D, Ret);
   if (!Result)
      return fail();

   auto retTy = Ret.getResolvedType();
   if (retTy->isAutoType()) {
      Ret.setResolvedType(Context.getEmptyTupleType());
   }

   sema().checkIfTypeUsableAsDecl(Ret, D);
   D->setIsNoReturn(retTy->isUnpopulatedType());

   if (D->isProtocolRequirement() && !D->isProtocolDefaultImpl()) {
      if (sema().ContainsAssociatedTypeConstraint(retTy)) {
         cast<ProtocolDecl>(D->getRecord())
            ->setHasAssociatedTypeConstraint(true);
      }
   }

   // Transform the return type into a Future<T>.
//   while (D->isAsync()) {
//      auto *AwaitableDecl = getAwaitableDecl();
//      if (!AwaitableDecl) {
//         diagnose(D, err_no_builtin_decl, 12, D->getSourceLoc());
//         break;
//      }
//
//      QualType RetTy = D->getReturnType();
//      auto &Conformances = Context.getConformanceTable();
//
//      if (!RetTy->isRecordType()
//          || !Conformances.conformsTo(RetTy->getRecord(), AwaitableDecl)) {
//         auto *FutureDecl = getFutureDecl();
//         if (!FutureDecl) {
//            diagnose(D, err_no_builtin_decl, 12, D->getSourceLoc());
//            break;
//         }
//
//         ensureDeclared(FutureDecl);
//
//         TemplateArgument Arg(FutureDecl->getTemplateParams().front(),
//                                 D->getReturnType(), D->getSourceLoc());
//
//         bool isNew = false;
//         auto TemplateArgs = FinalTemplateArgumentList::Create(Context,{ Arg });
//         auto Inst = Instantiator.InstantiateRecord(D, FutureDecl,
//                                                    TemplateArgs, &isNew);
//
//         if (!Inst)
//            break;
//
//         auto *Fut = Inst.get();
//         RetTy = Context.getRecordType(Fut);
//
//         D->getReturnType().setResolvedType(RetTy);
//         D->setImplicitFutureReturn(true);
//      }
//
//      collectCoroutineInfo(RetTy, D);
//      break;
//   }

   if (D->isConversionOp()) {
      auto Name = Context.getDeclNameTable()
                         .getConversionOperatorName(D->getReturnType());

      D->setName(Name);
      sema().makeDeclAvailable(*D->getDeclContext(), D);
   }

   D->createFunctionType(sema());
   sema().checkDuplicateFunctionDeclaration(D, "function");

   if (auto *M = dyn_cast<MethodDecl>(D)) {
      QualType SelfTy = Context.getRecordType(D->getRecord());
      if (D->hasMutableSelf() && !isa<InitDecl>(M) && !isa<DeinitDecl>(M)) {
         SelfTy = Context.getMutableReferenceType(SelfTy);
      }

      M->setSelfType(SelfTy);
   }

   if (QC.TypeCheckDeclContext(D, Loc)) {
      return fail();
   }

   return finish();
}

QueryResult TypecheckCallableQuery::run()
{
   if (D->isInvalid())
      return fail();

   SemaPass::DeclScopeRAII raii(sema(), D);
   SemaPass::ScopeGuard scope(sema(), D);

   for (FuncArgDecl *ArgDecl : D->getArgs()) {
      if (QC.TypecheckFuncArg(ArgDecl, Loc)) {
         return fail();
      }
   }

   if (auto Body = D->getBody()) {
      auto res = sema().visitStmt(D, Body);
      if (!res)
         return fail();

      D->setBody(res.get());
   }

   return finish();
}

QueryResult PrepareFunctionInterfaceQuery::run()
{
   if (QC.PrepareCallableInterface(D, Loc)) {
      return fail();
   }

   return finish();
}

QueryResult TypecheckFunctionQuery::run()
{
   if (!D->getBody()) {
      return finish();
   }

   if (QC.TypecheckCallable(D, Loc)) {
      return fail();
   }

   return finish();
}

QueryResult PrepareMethodInterfaceQuery::run()
{
   // `mutating` is not allowed on class methods.
   if (D->hasMutableSelf() && isa<ClassDecl>(D->getDeclContext())) {
      sema().diagnose(D, err_mutating_on_class, D->getSourceLoc());
      D->setMutating(false);
   }

   if (QC.PrepareCallableInterface(D, Loc)) {
      return fail();
   }

   // Check if this is the `copy` function of this type.
   if (D->getDeclName().isStr("copy")
       && !D->isStatic()
       && D->getArgs().size() == 1
       && D->getReturnType()->isRecordType()
       && D->getReturnType()->getRecord() == D->getRecord()) {
      D->getRecord()->setCopyFn(D);
   }

   // check virtual and override methods after all signatures (most
   // importantly those of base classes) are available.
   sema().checkVirtualOrOverrideMethod(D);

   return finish();
}

QueryResult TypecheckMethodQuery::run()
{
   if (!D->getBody()) {
      return finish();
   }

   if (QC.TypecheckCallable(D, Loc)) {
      return fail();
   }

   return finish();
}

namespace {

class ParamTypeVisitor: public RecursiveTypeVisitor<ParamTypeVisitor>,
                        public RecursiveASTVisitor<ParamTypeVisitor> {
   llvm::SmallPtrSetImpl<TemplateParamDecl*> &Params;

public:
   explicit
   ParamTypeVisitor(llvm::SmallPtrSetImpl<TemplateParamDecl*> &Params)
      : Params(Params)
   { }

   bool visitGenericType(const GenericType *T)
   {
      for (auto P : Params)
         if (P->getName() == T->getGenericTypeName())
            Params.erase(P);

      return false;
   }

   bool visitDependentSizeArrayType(const DependentSizeArrayType *T)
   {
      RecursiveASTVisitor::visit(T->getSizeExpr());
      return true;
   }

   bool visitIdentifierRefExpr(IdentifierRefExpr *E)
   {
      if (E->getParentExpr())
         return false;

      if (E->getKind() == IdentifierKind::TemplateParam)
         Params.erase(E->getTemplateParam());

      return false;
   }
};

} // anonymous namespace

QueryResult AssignInitNameQuery::run()
{
   auto &Context = QC.CI.getContext();

   QualType RecordTy = Context.getRecordType(Init->getRecord());
   if (!Init->getDeclName()) {
      auto DeclName = Context.getDeclNameTable().getConstructorName(RecordTy);

      Init->setName(DeclName);
      sema().makeDeclAvailable(*Init->getRecord(), DeclName, Init);
   }

   return finish();
}

QueryResult PrepareInitInterfaceQuery::run()
{
   auto R = D->getRecord();

   if (D->getArgs().empty() && isa<StructDecl>(R)
       && D->isCompleteInitializer()
       && !D->isMemberwise()) {
      cast<StructDecl>(R)->setParameterlessConstructor(D);
   }

   if (R->isOpaque()) {
      sema().diagnose(R, err_generic_error,
                      "opaque type may not contain an initializer",
                      R->getSourceLoc());

      return fail();
   }

   auto &Context = QC.CI.getContext();

   QualType RecordTy = Context.getRecordType(D->getRecord());
   D->setReturnType(SourceType(Context.getVoidType()));

   if (QC.AssignInitName(D)) {
      return fail();
   }

   if (D->isDefaultInitializer() && D->isCompleteInitializer())
      cast<StructDecl>(R)->setDefaultInitializer(D);

   if (D->isMemberwiseInitializer() && D->isCompleteInitializer())
      cast<StructDecl>(R)->setMemberwiseInitializer(D);

   SmallPtrSet<TemplateParamDecl*, 4> Params;
   Params.insert(D->getTemplateParams().begin(),
                 D->getTemplateParams().end());

   if (QC.PrepareCallableInterface(D, Loc)) {
      return fail();
   }

   ParamTypeVisitor V(Params);
   for (auto &arg : D->getArgs()) {
      V.RecursiveTypeVisitor::visit(arg->getType().getResolvedType());
   }

   if (!Params.empty()) {
      sema().diagnose(D, err_initializer_templ_args_must_be_inferrable,
                      0 /*initializer*/, (*Params.begin())->getDeclName(),
                      (*Params.begin())->getSourceRange());
   }

   if (!isa<ClassDecl>(R))
      D->setMutating(true);

   // declare base initializer
   if (!D->isBaseInitializer()) {
      auto BaseInitName = Context.getDeclNameTable()
                                 .getConstructorName(RecordTy, false);

      SmallVector<FuncArgDecl*, 4> Args{ sema().MakeSelfArg(D->getSourceLoc())};
      Args.append(D->getArgs().begin(), D->getArgs().end());

      ASTVector<TemplateParamDecl*> Params(Context, D->getTemplateParams());
      auto BaseD = InitDecl::Create(Context, D->getAccess(),
                                       D->getSourceLoc(), Args,
                                       move(Params), D->getBody(),
                                       BaseInitName, D->isFallible());

      sema().ActOnDecl(D->getDeclContext(), BaseD, false);
      QC.PrepareInitInterface(BaseD, Loc);

      D->setBaseInit(BaseD);

      BaseD->setSynthesized(true);
      BaseD->setCompleteInit(D);
      BaseD->setBodyTemplate(D->getBodyTemplate());
   }

   if (D->isFallible()) {
      RecordDecl *Opt;
      if (QC.GetBuiltinRecord(Opt, GetBuiltinRecordQuery::Option, Loc)) {
         return fail();
      }

      if (!Opt) {
         sema().diagnose(D, err_no_builtin_decl, D->getSourceLoc(),
                         /*fallible init*/ 10);

         return fail();
      }

      if (D->getRecord()->isTemplate())
         return finish();

      sema::TemplateArgument Arg(Opt->getTemplateParams().front(),
                                    RecordTy, D->getSourceLoc());

      auto TemplateArgs = sema::FinalTemplateArgumentList::Create(Context, Arg);
      auto Inst = sema().InstantiateRecord(D->getSourceLoc(),Opt, TemplateArgs);

      QualType ResultTy;
      if (Inst)
         ResultTy = Context.getRecordType(Inst);
      else
         ResultTy = Context.getRecordType(Opt);

      D->setOptionTy(ResultTy);
   }

   return finish();
}

QueryResult TypecheckInitQuery::run()
{
   if (!D->getBody()) {
      return finish();
   }

   if (QC.TypecheckCallable(D, Loc)) {
      return fail();
   }

   return finish();
}

QueryResult PrepareDeinitInterfaceQuery::run()
{
   auto &Context = QC.CI.getContext();

   D->setReturnType(SourceType(Context.getVoidType()));
   D->setMutating(!isa<ClassDecl>(D->getRecord()));

   if (!D->getDeclName()) {
      QualType RecordTy = Context.getRecordType(D->getRecord());
      auto DeclName = Context.getDeclNameTable().getDestructorName(RecordTy);

      D->setName(DeclName);
      sema().makeDeclAvailable(*D->getRecord(), DeclName, D);
   }

   if (QC.PrepareCallableInterface(D, Loc)) {
      return fail();
   }

   return finish();
}

QueryResult TypecheckDeinitQuery::run()
{
   if (!D->getBody()) {
      return finish();
   }

   if (QC.TypecheckCallable(D, Loc)) {
      return fail();
   }

   return finish();
}