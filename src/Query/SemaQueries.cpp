#include "cdotc/Query/Query.h"

#include "cdotc/AST/Decl.h"
#include "cdotc/Basic/NestedNameSpecifier.h"
#include "cdotc/ILGen/ILGenPass.h"
#include "cdotc/Module/Module.h"
#include "cdotc/Module/ModuleManager.h"
#include "cdotc/Parse/Parser.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Sema/TemplateInstantiator.h"
#include "cdotc/Support/Log.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::sema;
using namespace cdot::support;

QueryResult GetDefaultTemplateArgQuery::run()
{
   if (P->isVariadic()) {
      return finish(
          TemplateArgument(P, P->isTypeName(), {}, P->getSourceLoc()));
   }
   if (P->isTypeName()) {
      return finish(TemplateArgument(P, QC.Sema->Context.getTemplateArgType(P),
                                     P->getSourceLoc()));
   }

   return finish(TemplateArgument(P, nullptr, P->getSourceLoc()));
}

QueryResult CreateSelfArgumentQuery::run()
{
   auto& Context = QC.CI.getContext();
   auto* SelfII = &Context.getIdentifiers().get("self");
   auto* Arg = FuncArgDecl::Create(
       Context, SelfLoc, SelfLoc, DeclarationName(SelfII), nullptr,
       ArgumentConvention::Default, SourceType(Self), nullptr, false, false,
       /*isSelf=*/true);

   return finish(Arg);
}

QueryResult FindPrecedenceGroupQuery::run()
{
   auto* DC = QC.CI.getCompilationModule()->getDecl();

   const MultiLevelLookupResult* LookupRes;
   if (QC.DirectLookup(LookupRes, DC, Name)) {
      return fail();
   }

   if (LookupRes->empty()) {
      if (DiagOnError) {
         QC.Sema->diagnose(err_precedence_group_does_not_exist, Name, Loc);
         return fail();
      }

      return finish(nullptr);
   }

   auto* PG = dyn_cast<PrecedenceGroupDecl>(LookupRes->front().front());
   if (!PG) {
      return finish(nullptr);
   }

   if (QC.PrepareDeclInterface(PG)) {
      return fail();
   }

   return finish(PG);
}

QueryResult FindOperatorQuery::run()
{
   auto* DC = QC.CI.getCompilationModule()->getDecl();

   const MultiLevelLookupResult* LookupRes;
   if (QC.DirectLookup(LookupRes, DC, Name)) {
      return fail();
   }

   if (LookupRes->empty()) {
      if (DiagOnError) {
         QC.Sema->diagnose(err_undeclared_operator, Name, Loc);
      }

      return fail();
   }

   auto* Op = cast<OperatorDecl>(LookupRes->front().front());
   if (auto II = Op->getPrecedenceGroupIdent()) {
      PrecedenceGroupDecl* PG;
      if (QC.FindPrecedenceGroup(PG, II)) {
         return fail();
      }

      if (!PG) {
         QC.Sema->diagnose(Op, err_precedence_group_does_not_exist,
                           Op->getSourceLoc(), II->getIdentifier());
      }
      else {
         Op->setPrecedenceGroup(PG);
      }

      if (Op->getDeclName().getDeclaredOperatorName().getKind()
          == DeclarationName::InfixOperatorName) {
         QC.Sema->Context.setInfixOperatorPrecedence(
             Op->getDeclName().getDeclaredOperatorName().getInfixOperatorName(),
             PG);
      }
   }

   if (!Op->getPrecedenceGroup()) {
      Op->setPrecedenceGroup(QC.Sema->getDefaultPrecedenceGroup());
   }

   return finish(Op);
}

QueryResult ExpandMacrosQuery::run()
{
   llvm_unreachable("needed?");
}

QueryResult ExpandMacroQuery::run()
{
   NamedDecl* LookupRes;
   if (QC.LookupSingle(LookupRes, DC, Name, LookupOpts::Restricted)) {
      return fail();
   }

   auto Macro = dyn_cast_or_null<MacroDecl>(LookupRes);
   parse::ParseResult Result;

   if (!Macro) {
      QC.Sema->diagnoseMemberNotFound(DC, SOD, Name, err_macro_does_not_exist);
      return fail();
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

      QC.Sema->diagnose(SOD, err_macro_expects_delim, SOD.getSourceRange(),
                        Name, ExpectedDelim);
   }

   if (Macro) {
      LOG(MacroExpansions, "expanding macro ", Name);

      Result = parse::Parser::expandMacro(
          sema(), Macro, SOD, Tokens,
          (parse::Parser::ExpansionKind)ExpectedKind);
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
   finish(nullptr);

   SemaPass::DeclScopeRAII DSR(sema(), Decl->getDeclContext());
   if (!QC.Sema->checkNamespaceRef(Decl))
      return fail();

   DeclContext* Ctx = Decl->getDeclContext();
   if (auto* Ident = cast_or_null<IdentifierRefExpr>(Decl->getParentExpr())) {
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

   if (!Result) {
      return finish(nullptr);
   }

   return finish(Result.getDecl());
}

QueryResult ResolveUsingQuery::run()
{
   finish();

   const MultiLevelLookupResult* LookupRes;
   if (QC.NestedNameLookup(
           LookupRes, U->getDeclContext(), U->getNestedImportName(),
           DefaultLookupOpts | LookupOpts::IssueDiag, U->getSourceLoc())) {
      return fail();
   }

   if (U->isWildcardImport()) {
      if (LookupRes->size() != 1 || LookupRes->front().size() != 1) {
         QC.Sema->diagnose(U, err_using_target_ambiguous, U->getSourceRange());

         NamedDecl* Cand1 = LookupRes->front().front();
         NamedDecl* Cand2;

         if (LookupRes->size() == 1) {
            Cand2 = LookupRes->front()[1];
         }
         else {
            Cand2 = (*LookupRes)[1].front();
         }

         QC.Sema->diagnose(note_candidate_here, Cand1->getSourceLoc());
         QC.Sema->diagnose(note_candidate_here, Cand2->getSourceLoc());

         return finish(DoneWithError);
      }

      NamedDecl* Target = LookupRes->front().front();
      if (!isa<DeclContext>(Target)) {
         QC.Sema->diagnose(U, err_cannot_lookup_member_in, Target,
                           Target->getDeclName());

         return finish(DoneWithError);
      }

      if (auto* I = dyn_cast<ImportDecl>(Target)) {
         Target = I->getImportedModule()->getDecl();
      }

      LOG(UsingStatements, "made ", Target->getFullName(), " visible in ",
          U->getDeclContext()->getNameAsString());

      QC.Sema->makeDeclsAvailableIn(*U->getDeclContext(),
                                    *cast<DeclContext>(Target));
   }
   else {
      for (NamedDecl* ND : LookupRes->allDecls()) {
         LOG(UsingStatements, "made ", ND->getFullName(), " visible in ",
             U->getDeclContext()->getNameAsString());

         QC.Sema->makeDeclAvailable(*U->getDeclContext(), ND);

         if (FoundDecls) {
            FoundDecls->push_back(ND);
         }
      }
   }

   return finish();
}

QueryResult ResolveImportQuery::run()
{
   finish();

   QC.CI.getModuleMgr().ImportModule(I);

   auto* Mod = I->getImportedModule();
   if (!Mod)
      return fail();

   if (I->isWildcardImport() || Mod->allImplicitlyExported()) {
      LOG(ImportStatements, "imported all declarations of ", Mod->getFullName(),
          " into ", I->getDeclContext()->getNameAsString());

      I->getDeclContext()->addImportedModule(Mod);
      return finish();
   }

   if (I->getNamedImports().empty()) {
      LOG(ImportStatements, "imported ", Mod->getFullName(), " into ",
          I->getDeclContext()->getNameAsString());

      I->addImportedModule(Mod);
      return finish();
   }

   bool Valid = true;
   for (auto& Name : I->getNamedImports()) {
      const SingleLevelLookupResult* LookupRes;
      if (QC.LookupFirst(LookupRes, Mod->getDecl(), Name)) {
         return fail();
      }

      if (LookupRes->empty()) {
         QC.Sema->diagnose(I, err_member_not_found, Decl::ModuleDeclID,
                           Mod->getName(), Name, I->getSourceRange());

         Valid = false;
         continue;
      }

      for (NamedDecl* ND : *LookupRes) {
         LOG(ImportStatements, "imported ", ND->getFullName(), " into ",
             I->getDeclContext()->getNameAsString());

         QC.Sema->makeDeclAvailable(*I->getDeclContext(), ND);

         if (FoundDecls) {
            FoundDecls->push_back(ND);
         }
      }
   }

   return finish(Valid ? Done : DoneWithError);
}

QueryResult ResolveStaticIfQuery::run()
{
   auto Res = QC.Sema->typecheckExpr(Decl->getCondition(), {}, Decl);
   if (!Res) {
      return fail();
   }

   if (Decl->getCondition()->needsInstantiation()) {
      return Query::finish(Dependent);
   }

   auto BoolRes = QC.Sema->evaluateAsBool(Decl, Decl->getCondition());
   if (!BoolRes) {
      return fail();
   }

   bool CondIsTrue = BoolRes.getValue();
   if (auto Template = Decl->getTemplate()) {
      if (Template->isInvalid()) {
         Decl->setIsInvalid(true);
         return fail();
      }

      // collect the template arguments at the point of instantiation
      MultiLevelFinalTemplateArgList TemplateArgs;
      for (auto Ctx = &QC.Sema->getDeclContext(); Ctx;
           Ctx = Ctx->getParentCtx()) {
         if (auto ND = dyn_cast<NamedDecl>(Ctx)) {
            if (ND->isInstantiation()) {
               TemplateArgs.addOuterList(ND->getTemplateArgs());
            }
         }
      }

      ast::Decl* Inst;
      if (CondIsTrue) {
         Inst = QC.Sema->getInstantiator().InstantiateDecl(Template->getIfDecl(), &TemplateArgs,
                                                           Template->getSourceLoc());
         if (!Inst) {
            return fail();
         }
      }
      else if (auto Else = Template->getElseDecl()) {
         Inst = QC.Sema->getInstantiator().InstantiateDecl(Else, &TemplateArgs,
                                                           Template->getSourceLoc());
         if (!Inst) {
            return fail();
         }
      }
      else {
         return finish(nullptr);
      }

      if (auto Err = QC.PrepareDeclInterface(Inst)) {
         return Query::finish(Err);
      }

      return finish(Inst);
   }

   ast::Decl* D;
   if (CondIsTrue) {
      D = Decl->getIfDecl();
   }
   else if (auto* Else = Decl->getElseDecl()) {
      D = Else;
   }
   else {
      return finish(nullptr);
   }

   if (auto Err = QC.PrepareDeclInterface(D)) {
      return Query::finish(Err);
   }

   return finish(D);
}

QueryResult ResolveStaticForQuery::run()
{
   if (Decl->isVariadic()) {
      auto result = QC.Sema->visitVariadicForDecl(Decl);
      if (!result) {
         return fail();
      }

      return finish(result.get());
   }

   llvm_unreachable("TODO!");
}

QueryResult ResolveStaticAssertQuery::run()
{
   auto Res = QC.Sema->typecheckExpr(Decl->getExpr(), {}, Decl);
   if (!Res) {
      return fail();
   }

   if (Decl->getExpr()->needsInstantiation()) {
      return Query::finish(Dependent);
   }

   auto BoolRes = QC.Sema->evaluateAsBool(Decl, Decl->getExpr());
   if (!BoolRes) {
      return fail();
   }

   bool CondIsTrue = BoolRes.getValue();
   if (!CondIsTrue) {
      auto msg = Decl->getMessage();
      QC.Sema->diagnose(Decl, err_static_assert_failed, Decl->getSourceLoc(),
                        !msg.empty(), msg);
   }

   return finish();
}

QueryResult ResolveStaticPrintQuery::run()
{
   auto Res = QC.Sema->typecheckExpr(Decl->getExpr(), {}, Decl);
   if (!Res) {
      return fail();
   }

   auto StaticRes = QC.Sema->evalStaticExpr(Decl, Decl->getExpr());
   if (!StaticRes) {
      return fail();
   }

   if (Decl->getExpr()->needsInstantiation()) {
      return Query::finish(Dependent);
   }

   std::string str;
   llvm::raw_string_ostream OS(str);
   OS << *StaticRes.getValue();

   QC.Sema->diagnose(note_static_print, Decl->getSourceLoc(), OS.str());
   return finish();
}

QueryResult CheckProtocolExtensionApplicabilityQuery::run()
{
   using ReadyKind = ReferencedAssociatedTypesReadyQuery::ResultKind;

   if (QC.PrepareDeclInterface(Ext)) {
      return fail();
   }

   ConstraintSet* Constraints;
   if (QC.VerifyConstraints(Constraints, Ext)) {
      return fail();
   }

   if (!Constraints || Constraints->empty()) {
      LOG(ExtensionApplicability, "extension [", Ext->getFullSourceLoc(),
          "] applies to ", T.toDiagString());

      return finish(DoesApply);
   }

   ReadyKind IsReady;
   if (QC.ReferencedAssociatedTypesReady(IsReady, T, Constraints)) {
      return fail();
   }

   if (IsReady == ReadyKind::NotReady) {
      LOG(ExtensionApplicability, "extension [", Ext->getFullSourceLoc(),
          "] might apply to ", T.toDiagString(), " (constraints not ready)");

      return finish(CantTell);
   }

   bool AllSatisfied;
   for (auto* C : *Constraints) {
      if (auto Err = QC.IsConstraintSatisfied(AllSatisfied, C, T, Ext)) {
         if (Err.isDependent()) {
            LOG(ExtensionApplicability, "extension [", Ext->getFullSourceLoc(),
                "] might apply to ", T.toDiagString(), " (dependent)");

            return finish(CantTell);
         }

         return fail();
      }

      if (!AllSatisfied) {
         LOG(ExtensionApplicability, "extension [", Ext->getFullSourceLoc(),
             "] does not apply to ", T.toDiagString());

         return finish(DoesNotApply);
      }
   }

   LOG(ExtensionApplicability, "extension [", Ext->getFullSourceLoc(),
       "] applies to ", T.toDiagString());

   return finish(DoesApply);
}

static NamedDecl*
findInUnconditionalConformances(QueryContext& QC, CanType Self,
                                ProtocolDecl* proto, IdentifierInfo* name,
                                SmallPtrSetImpl<ProtocolDecl*>& testSet);

static NamedDecl* findInUnconditionalConformances(
    QueryContext& QC, QualType Self, MutableArrayRef<SourceType> conformanceTypes,
    IdentifierInfo* name, SmallPtrSetImpl<ProtocolDecl*>& testSet)
{
   ArrayRef<ProtocolDecl*> protocols;
   if (QC.ResolveConformancesToProtocols(protocols, Self, conformanceTypes)) {
      return nullptr;
   }

   for (auto* conformance : protocols) {
      auto* result = conformance->lookupSingle(name);
      if (result) {
         return result;
      }

      if ((result = findInUnconditionalConformances(QC, Self, conformance, name,
                                                    testSet))) {
         return result;
      }
   }

   return nullptr;
}

static NamedDecl*
findInUnconditionalConformances(QueryContext& QC, CanType Self,
                                ProtocolDecl* proto, IdentifierInfo* name,
                                SmallPtrSetImpl<ProtocolDecl*>& testSet)
{
   if (!testSet.insert(proto).second) {
      return nullptr;
   }

   if (auto* result = findInUnconditionalConformances(
           QC, Self, proto->getConformanceTypes(), name, testSet)) {
      return result;
   }

   if (QC.FindExtensions(Self)) {
      return nullptr;
   }

   auto extensions = QC.Context.getExtensions(Self);
   for (auto* ext : extensions) {
      if (!QC.Context.getParsedConstraints(ext).empty()) {
         continue;
      }

      if (auto* result = findInUnconditionalConformances(
              QC, Self, ext->getConformanceTypes(), name, testSet)) {
         return result;
      }
   }

   return nullptr;
}

static NamedDecl* findInUnconditionalConformances(QueryContext& QC,
                                                  ProtocolDecl* proto,
                                                  IdentifierInfo* name)
{
   SmallPtrSet<ProtocolDecl*, 4> testSet;
   QualType Self = QC.Context.getRecordType(proto);

   return findInUnconditionalConformances(QC, Self, proto, name, testSet);
}

static NamedDecl* findReferencedDecl(QueryContext& QC,
                                     DeclContext* constrainedDecl,
                                     IdentifierInfo* name,
                                     NamedDecl* originalDecl)
{
   if (!constrainedDecl) {
      return nullptr;
   }

   if (auto* ext = dyn_cast<ExtensionDecl>(constrainedDecl)) {
      auto* R = ext->getExtendedRecord();
      if (auto* proto = R->dyn_cast<ProtocolDecl>()) {
         return findReferencedDecl(QC, proto, name, originalDecl);
      }
      else {
         // If this extension is not of a protocol, the constrained decl must be
         // a template parameter.
         return R->lookupSingle(name);
      }
   }
   else if (auto* AT = dyn_cast<AssociatedTypeDecl>(constrainedDecl)) {
      return findReferencedDecl(QC, AT->getRecord(), name, originalDecl);
   }
   else if (auto* proto = dyn_cast<ProtocolDecl>(constrainedDecl)) {
      // The referenced decl must be a template parameter or an
      // associated type.
      auto* result = proto->lookupSingle(name);
      if (result) {
         return result;
      }

      // Only look in unconditional conformances.
      // Note: We need to manually find the conformances here because they may
      // not be registered yet.
      return findInUnconditionalConformances(QC, proto, name);
   }

   auto* result = constrainedDecl->lookupSingle(name);
   if (result) {
      return result;
   }

   return findReferencedDecl(QC, constrainedDecl->getParentCtx(), name,
                             originalDecl);
}

static NamedDecl* findReferencedDecl(QueryContext& QC, CanType T,
                                     IdentifierInfo* name,
                                     NamedDecl* originalDecl)
{
   if (QualType RT = T->asRecordType()) {
      return findReferencedDecl(QC, RT->getRecord(), name, originalDecl);
   }
   if (auto* Ext = T->asExistentialType()) {
      for (QualType ET : Ext->getExistentials()) {
         auto Result = findReferencedDecl(QC, ET, name, originalDecl);
         if (Result) {
            return Result;
         }
      }
   }

   return nullptr;
}

QueryResult VerifyConstraintQuery::run()
{
   DeclContext* CurCtx;
   if (auto* DC = dyn_cast<DeclContext>(ConstrainedDecl)) {
      CurCtx = DC;
   }
   else {
      CurCtx = ConstrainedDecl->getDeclContext();
   }

   DeclContext* baseCtx = CurCtx;
   QualType CurrentType;
   QualType LookupTy;

   unsigned i = 0;
   ArrayRef<IdentifierInfo*> NameQual = C->NameQual;

   for (auto* Ident : NameQual) {
      NamedDecl* Result;
      if (i == 0) {
         Result = findReferencedDecl(QC, CurCtx, Ident, ConstrainedDecl);
      }
      else {
         Result = findReferencedDecl(QC, LookupTy, Ident, ConstrainedDecl);
      }

      if (!Result) {
         if (i == 0) {
            QC.Sema->diagnose(ConstrainedDecl, err_member_not_found,
                              cast<NamedDecl>(CurCtx),
                              cast<NamedDecl>(CurCtx)->getDeclName(),
                              Ident->getIdentifier(), C->SR);
         }
         else {
            QC.Sema->diagnose(ConstrainedDecl, err_generic_error,
                              LookupTy.toDiagString()
                                  + " does not have a member named "
                                  + Ident->getIdentifier().str(),
                              C->SR);
         }

         return fail();
      }

      if (auto* AT = Result->dyn_cast<AssociatedTypeDecl>()) {
//         QualType Self = QC.Context.getRecordType(AT->getRecord());
//         if (QC.ResolveAssociatedTypes(Self)) {
//            return fail();
//         }
         if (QC.PrepareDeclInterface(AT)) {
            return fail();
         }

         CurrentType = QC.Context.getAssociatedType(AT, CurrentType);
         LookupTy = AT->getCovariance();
      }
      else if (auto* P = Result->dyn_cast<TemplateParamDecl>()) {
         if (QC.PrepareTemplateParameters(cast<NamedDecl>(P->getDeclContext())))
            return fail();

         assert(i == 0 && "should only appear as the first part of the type!");
         CurrentType = QC.Context.getTemplateArgType(P);
         LookupTy = P->getCovariance();
      }
      else {
         QC.Sema->diagnose(ConstrainedDecl,
                           err_cannot_be_referenced_in_constraint, Result,
                           Result->getDeclName(), C->SR);

         return fail();
      }

      ++i;
   }

   // Don't apply capabilities while verifying this constraint because it may
   // lead to this query being called again.
   SemaPass::DeclScopeRAII declScope(*QC.Sema, baseCtx);
   SemaPass::DontApplyCapabilitiesRAII capabilitiesRaii(*QC.Sema);

   AliasDecl* Concept = nullptr;
   QualType Type;

   switch (C->K) {
   case DeclConstraint::Concept: {
      auto* ConceptRef = cast<IdentifierRefExpr>(C->TypeOrConceptExpr);
      ConceptRef->setAllowIncompleteTemplateArgs(true);
      ConceptRef->setAllowOverloadRef(true);

      auto semaRes = QC.Sema->visitIdentifierRefExpr(ConceptRef);
      if (!semaRes) {
         return fail();
      }

      auto* refExpr = semaRes.get();

      NamedDecl* referencedDecl = nullptr;
      if (auto* declRef = dyn_cast<DeclRefExpr>(refExpr)) {
         referencedDecl = declRef->getDecl();
      }
      else if (auto* memberRef = dyn_cast<MemberRefExpr>(refExpr)) {
         referencedDecl = memberRef->getMemberDecl();
      }
      else if (auto* overloadRef = dyn_cast<OverloadedDeclRefExpr>(refExpr)) {
         referencedDecl = overloadRef->getOverloads().front();
      }
      else {
         llvm_unreachable("should not happen!");
      }

      Concept = dyn_cast<AliasDecl>(referencedDecl);
      if (!Concept) {
         QC.Sema->diagnose(ConceptRef, err_cannot_be_used_as_concept,
                           ConceptRef->getNamedDecl(),
                           ConceptRef->getNamedDecl()->getDeclName(),
                           ConceptRef->getSourceRange());

         return fail();
      }

      auto Params = Concept->getTemplateParams();
      if (Params.size() != 1 || !Params[0]->isTypeName()) {
         QC.Sema->diagnose(ConceptRef, err_not_a_valid_concept, 1,
                           ConceptRef->getSourceRange());

         return fail();
      }

      break;
   }
   case DeclConstraint::TypePredicate:
   case DeclConstraint::TypePredicateNegated:
   case DeclConstraint::TypeEquality:
   case DeclConstraint::TypeInequality: {
      SourceType T(C->TypeOrConceptExpr);
      if (!QC.Sema->visitSourceType(ConstrainedDecl, T)) {
         return fail();
      }

      Type = T.getResolvedType();
      break;
   }
   case DeclConstraint::Class:
   case DeclConstraint::Enum:
   case DeclConstraint::Struct:
      break;
   }

   DeclConstraint* DC;
   if (Concept) {
      DC = DeclConstraint::Create(QC.Context, CurrentType, Concept);
   }
   else {
      DC = DeclConstraint::Create(QC.Context, C->K, CurrentType, Type);
   }

   return finish(DC);
}

QueryResult VerifyConstraintsQuery::run()
{
   auto parsedConstraints = QC.Context.getParsedConstraints(ConstrainedDecl);
   if (parsedConstraints.empty()) {
      return finish(QC.Context.EmptyConstraintSet);
   }

   // FIXME circular-dep HACK this is needed to allow lookups during constraint verification.
//   finish(QC.Context.EmptyConstraintSet);

   SmallVector<DeclConstraint*, 2> constraints;
   for (auto& PC : parsedConstraints) {
      DeclConstraint* DC;
      if (QC.VerifyConstraint(DC, &PC, ConstrainedDecl)) {
         return finish(QC.Context.EmptyConstraintSet);
      }

      constraints.push_back(DC);
   }

   auto* CS = ConstraintSet::Create(QC.Context, constraints);
   QC.Context.setConstraints(ConstrainedDecl, CS);
   QC.Context.updateConstraintLocs(ConstrainedDecl, parsedConstraints,
                                   constraints);

   return finish(CS);
}

QueryResult GetConstrainedTypeQuery::run()
{
   QualType currentType = C->getConstrainedType();
   SmallVector<NamedDecl*, 2> decls;

   while (currentType) {
      if (auto* AT = currentType->asAssociatedType()) {
         decls.push_back(AT->getDecl());
         currentType = AT->getOuterAT();
      }
      else {
         decls.push_back(currentType->asTemplateParamType()->getParam());
         break;
      }
   }

   QualType currentLookupType = Self;
   for (auto it = decls.rbegin(), end = decls.rend(); it != end; ++it) {
      NamedDecl* declToFind = *it;

      if (auto* param = dyn_cast<TemplateParamDecl>(declToFind)) {
         assert(Self->isRecordType() && "protocol not correctly implemented!");
         assert(Self->hasTemplateArgs()
                && "protocol not correctly implemented!");

         auto& templateArgs = Self->getTemplateArgs();
         auto* impl = templateArgs.getArgForParam(param);
         assert(impl && impl->isType()
                && "protocol not correctly implemented!");

         currentLookupType = impl->getType();
         continue;
      }

      const MultiLevelLookupResult* result;
      if (auto Err = QC.MultiLevelTypeLookup(result, currentLookupType,
                                             declToFind->getDeclName())) {
         return Query::finish(Err);
      }

      auto* impl = result->front().front();
      if (auto Err = QC.PrepareDeclInterface(impl)) {
         return Query::finish(Err);
      }

      QualType nextType;
      if (auto* AT = dyn_cast<AliasDecl>(impl)) {
         nextType = AT->getAliasedType();
      }
      else {
         nextType = cast<AssociatedTypeDecl>(impl)->getCovariance();
      }

      if (auto *DR = currentLookupType->asDependentRecordType()) {
         if (auto Err = QC.SubstTemplateParamTypes(nextType, nextType, DR->getTemplateArgs(), SourceRange())) {
            return Query::finish(Err);
         }

         if (auto Err = QC.SubstAssociatedTypes(nextType, nextType, currentLookupType, SourceRange())) {
            return Query::finish(Err);
         }
      }

      currentLookupType = nextType;
   }

   return finish(currentLookupType);
}

QueryResult IsConstraintSatisfiedQuery::run()
{
   QualType ConstrainedType;
   if (auto Err
       = QC.GetConstrainedType(ConstrainedType, C, Self, OriginalDecl)) {
      return Query::finish(Err);
   }

   auto* Rec = Self->getRecord();
   SemaPass::DeclScopeRAII DSR(*QC.Sema, Rec);

   CanType Canon = ConstrainedType->getCanonicalType();
   switch (C->getKind()) {
   case DeclConstraint::Concept: {
      AliasDecl* Concept = C->getConcept();
      TemplateArgument Arg(
          Concept->getTemplateParams().front(), Canon,
          QC.Context.getConstraintLoc(OriginalDecl, C).getStart());

      auto* FinalList
          = sema::FinalTemplateArgumentList::Create(QC.Context, Arg);

      AliasDecl* Inst = QC.Sema->getInstantiator().InstantiateAlias(
         Concept, FinalList,
         QC.Context.getConstraintLoc(OriginalDecl, C).getStart());

      if (!Inst) {
         return finish(true, DoneWithError);
      }

      if (QC.TypecheckDecl(Inst)) {
         return finish(true, DoneWithError);
      }

      if (Inst->getAliasExpr()->getExprType() != QC.Context.getBoolTy()) {
         QC.Sema->diagnose(Rec, err_concept_must_be_bool, Inst->getDeclName(),
                           Inst->getSourceRange());

         return finish(true, DoneWithError);
      }

      bool Value;
      if (QC.GetBoolValue(Value, Inst->getAliasExpr()->getEvaluatedExpr())) {
         return finish(true, DoneWithError);
      }

      return finish(Value);
   }
   case DeclConstraint::TypeEquality:
   case DeclConstraint::TypeInequality:
   case DeclConstraint::TypePredicate:
   case DeclConstraint::TypePredicateNegated: {
      QualType RHSType = C->getType();
      if (RHSType->containsAssociatedType()) {
         if (QC.SubstAssociatedTypes(
                 RHSType, RHSType, Self,
                 QC.Context.getConstraintLoc(OriginalDecl, C))) {
            return finish(true, DoneWithError);
         }
      }

      bool Result;
      if (C->getKind() == DeclConstraint::TypeEquality) {
         Result = Canon == RHSType.getCanonicalType();
      }
      else if (C->getKind() == DeclConstraint::TypeInequality) {
         Result = Canon != RHSType.getCanonicalType();
      }
      else {
         Result = true;

         ArrayRef<QualType> Types;
         if (auto *Ext = RHSType->asExistentialType()) {
            Types = Ext->getExistentials();
         }
         else {
            Types = RHSType;
         }

         for (QualType ET : Types) {
            if (ET->isClass()) {
               if (Canon->isClass()) {
                  auto SelfClass = cast<ClassDecl>(Canon->getRecord());
                  auto OtherClass = cast<ClassDecl>(ET->getRecord());

                  Result = OtherClass->isBaseClassOf(SelfClass);
               }
               else {
                  Result = false;
               }
            }
            else {
               Result = QC.Sema->ConformsTo(Canon,
                                            cast<ProtocolDecl>(
                                               ET->getRecord()));
            }

            if (!Result) {
               break;
            }
         }
      }

      if (C->getKind() == DeclConstraint::TypePredicateNegated) {
         Result = !Result;
      }

      if (!Result && Canon->containsTemplateParamType()) {
         return finish(true, Dependent);
      }

      return finish(Result);
   }
   case DeclConstraint::Struct: {
      return finish(Canon->isStruct());
   }
   case DeclConstraint::Enum: {
      return finish(Canon->isEnum());
   }
   case DeclConstraint::Class: {
      return finish(Canon->isClass());
   }
   }
}

// FIXME 'is RangeReplaceableCollection' is a superset of 'is MutableCollection'
QueryResult IsSupersetOfQuery::run()
{
   assert(C1 && C2 && "should have early-exited!");

   SmallPtrSet<DeclConstraint*, 2> neededConstraints(C2->begin(), C2->end());
   for (auto* DC : *C1) {
      neededConstraints.erase(DC);
   }

   return finish(neededConstraints.empty());
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
      auto* Ctx = ND->getDeclContext();

      // All extensions within the same file can access private declarations.
      bool SameFile = ND->getModule() == DC->getDeclModule();

      if (SameFile)
         Ctx = Ctx->lookThroughExtension();

      for (auto* Curr = DC; Curr; Curr = Curr->getParentCtx()) {
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
      auto* Ctx = ND->getDeclContext();
      for (auto* Curr = DC; Curr; Curr = Curr->getParentCtx()) {
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
      auto& FileMgr = QC.CI.getFileMgr();
      auto DeclID = FileMgr.getSourceId(ND->getSourceLoc());
      auto CurrID = FileMgr.getSourceId(cast<Decl>(DC)->getSourceLoc());

      return finish(DeclID == CurrID
                    || FileMgr.wasIncludedFrom(CurrID, DeclID));
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
      auto* Ctx = ND->getDeclContext();

      // All extensions within the same file can access private declarations.
      bool SameFile = ND->getModule() == DC->getDeclModule();

      if (SameFile)
         Ctx = Ctx->lookThroughExtension();

      for (auto* Curr = DC; Curr; Curr = Curr->getParentCtx()) {
         if (SameFile && Curr->lookThroughExtension() == Ctx) {
            return finish();
         }
         if (Curr == Ctx) {
            return finish();
         }
      }

      // declaration is not accessible here
      QC.Sema->diagnose(err_private_access, ND, ND->getDeclName(), Loc);

      break;
   }
   case AccessSpecifier::Protected: {
      // only visible within declaration context or subclasses (should have
      // been rejected outside of classes)
      auto C = cast<ClassDecl>(ND->getNonTransparentDeclContext());
      auto* Ctx = ND->getDeclContext();
      for (auto* Curr = DC; Curr; Curr = Curr->getParentCtx()) {
         if (Curr->lookThroughExtension() == Ctx->lookThroughExtension())
            return finish();

         auto SubClass = dyn_cast<ClassDecl>(Curr);
         if (SubClass && C->isBaseClassOf(SubClass))
            return finish();
      }

      // declaration is not accessible here
      QC.Sema->diagnose(err_protected_access, ND, ND->getDeclName(),
                        C->getDeclName(), Loc);

      break;
   }
   case AccessSpecifier::FilePrivate: {
      // visible within the file it was declared
      auto& FileMgr = QC.CI.getFileMgr();
      auto DeclID = FileMgr.getSourceId(ND->getSourceLoc());
      auto CurrID = FileMgr.getSourceId(cast<Decl>(DC)->getSourceLoc());

      if (DeclID == CurrID || FileMgr.wasIncludedFrom(CurrID, DeclID))
         return finish();

      // declaration is not accessible here
      QC.Sema->diagnose(err_fileprivate_access, ND, ND->getDeclName(),
                        FileMgr.getFileName(DeclID), Loc);

      break;
   }
   case AccessSpecifier::Internal: {
      if (DC->getDeclModule()->getBaseModule()
          == ND->getModule()->getBaseModule()) {
         return finish();
      }

      QC.Sema->diagnose(err_internal_access, ND, ND->getDeclName(),
                        ND->getModule()->getBaseModule()->getDeclName(), Loc);

      break;
   }
   }

   QC.Sema->diagnose(note_access_spec_here,
                     /*implicitly*/ !ND->getAccessRange(), (int)AccessSpec,
                     ND->getAccessRange(), ND->getSourceLoc());

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
                               C2->getReturnType(), Self, C1, C1)) {
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
      auto& Arg = Args1[i];
      auto& Other = Args2[i];

      if (QC.CheckTypeEquivalence(Equivalent, Arg->getType(), Other->getType(),
                                  Self, C1, C1)) {
         return fail();
      }

      if (!Equivalent)
         return finish(ParamTypeMismatch); // incompatible signature

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

QueryResult PrepareTemplateParametersQuery::run()
{
   Status St = Done;
   for (auto& TP : Decl->getTemplateParams()) {
      if (QC.PrepareTemplateParamInterface(TP)) {
         St = DoneWithError;
      }
   }

   return finish(St);
}

namespace {

struct BeingEvaluatedRAII {
   explicit BeingEvaluatedRAII(Decl* D) : D(D) { D->setBeingEvaluated(true); }

   ~BeingEvaluatedRAII() { D->setBeingEvaluated(false); }

private:
   Decl* D;
};

} // anonymous namespace

static void setDeclStatus(Decl* D, const QueryResult& Res)
{
   switch (Res.K) {
   case QueryResult::Error:
      D->setIsInvalid(true);
      break;
   case QueryResult::Dependent:
      D->setIsTypeDependent(true);
      break;
   default:
      llvm_unreachable("unexpected query status");
   }
}

QueryResult PrepareDeclInterfaceQuery::run()
{
   SemaPass::DeclScopeRAII DSR(*QC.Sema, D->getDeclContext());
   QC.Sema->checkDeclAttrs(D, Attr::BeforeDeclaration);

   BeingEvaluatedRAII BER(D);

#define PREPARE_DECL(NAME)                                                     \
   case Decl::NAME##DeclID:                                                    \
      if (auto Err                                                             \
          = QC.Prepare##NAME##Interface(static_cast<NAME##Decl*>(D))) {        \
         setDeclStatus(D, Err);                                                \
         return Query::finish(Err);                                            \
      }                                                                        \
      break;

   switch (D->getKind()) {
      PREPARE_DECL(PrecedenceGroup)
      PREPARE_DECL(Operator)
      PREPARE_DECL(GlobalVar)
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
      PREPARE_DECL(Alias)
      PREPARE_DECL(Field)
      PREPARE_DECL(Prop)
      PREPARE_DECL(Subscript)
      PREPARE_DECL(EnumCase)
      PREPARE_DECL(Namespace)
      PREPARE_DECL(Compound)
      PREPARE_DECL(Module)
   case Decl::ImportDeclID:
   case Decl::UsingDeclID:
   case Decl::LocalVarDeclID:
   case Decl::MacroDeclID:
   case Decl::MacroExpansionDeclID:
   case Decl::StaticIfDeclID:
   case Decl::StaticForDeclID:
   case Decl::StaticPrintDeclID:
   case Decl::StaticAssertDeclID:
   case Decl::DestructuringDeclID:
   case Decl::SourceFileDeclID:
      return QueryResult(QueryResult::Success);
   default:
      llvm_unreachable("invalid declaration passed to PrepareDeclInterface");
      break;
   }

#undef PREPARE_DECL

   QC.Sema->checkDeclAttrs(D, Attr::AfterDeclaration);
   return finish();
}

QueryResult TypecheckDeclQuery::run()
{
   if (auto Err = QC.PrepareDeclInterface(D)) {
      return Err;
   }

   SemaPass::DeclScopeRAII DSR(*QC.Sema, D->getDeclContext());
   QC.Sema->checkDeclAttrs(D, Attr::BeforeSema);

#define TYPECHECK_DECL(NAME)                                                   \
   case Decl::NAME##DeclID:                                                    \
      if (auto Err = QC.Typecheck##NAME(static_cast<NAME##Decl*>(D))) {        \
         setDeclStatus(D, Err);                                                \
         return Query::finish(Err);                                            \
      }                                                                        \
      break;

   switch (D->getKind()) {
      TYPECHECK_DECL(GlobalVar)
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
      TYPECHECK_DECL(Alias)
      TYPECHECK_DECL(Field)
      TYPECHECK_DECL(Prop)
      TYPECHECK_DECL(Subscript)
      TYPECHECK_DECL(Namespace)
      TYPECHECK_DECL(Compound)
      TYPECHECK_DECL(Module)
   case Decl::LocalVarDeclID:
   case Decl::DestructuringDeclID:
      // Handled in Sema::visitDeclStmt
      break;
   case Decl::SourceFileDeclID:
      return Query::finish(QC.TypeCheckDeclContext(cast<SourceFileDecl>(D)));
   default:
      break;
   }

#undef TYPECHECK_DECL

   QC.Sema->checkDeclAttrs(D, Attr::AfterSema);

   return finish();
}

QueryResult TypeCheckDeclContextQuery::run()
{
   SemaPass::DeclScopeRAII DSR(sema(), DC);

   for (auto* D : DC->getDecls()) {
      QC.TypecheckDecl(D);
   }

   return finish();
}

QueryResult TypecheckConstraintsQuery::run() { return finish(); }

QueryResult PreparePrecedenceGroupInterfaceQuery::run()
{
   if (auto HigherThan = D->getHigherThanIdent()) {
      PrecedenceGroupDecl* HT;
      if (QC.FindPrecedenceGroup(HT, HigherThan)) {
         return finish(DoneWithError);
      }

      D->setHigherThan(HT);
   }

   if (auto LowerThan = D->getLowerThanIdent()) {
      PrecedenceGroupDecl* LT;
      if (QC.FindPrecedenceGroup(LT, LowerThan)) {
         return finish(DoneWithError);
      }

      D->setHigherThan(LT);
   }

   return finish();
}

QueryResult TypecheckPrecedenceGroupQuery::run()
{
   llvm_unreachable("no need to run this!");
}

QueryResult PrepareOperatorInterfaceQuery::run()
{
   if (auto II = D->getPrecedenceGroupIdent()) {
      PrecedenceGroupDecl* PG;
      if (QC.FindPrecedenceGroup(PG, II)) {
         return finish(DoneWithError);
      }

      if (!PG) {
         D->setPrecedenceGroup(PG);
      }

      if (D->getDeclName().getDeclaredOperatorName().getKind()
          == DeclarationName::InfixOperatorName) {
         QC.Sema->Context.setInfixOperatorPrecedence(
             D->getDeclName().getDeclaredOperatorName().getInfixOperatorName(),
             PG);
      }
   }

   if (!D->getPrecedenceGroup()) {
      PrecedenceGroupDecl* DefaultPG;
      if (QC.FindPrecedenceGroup(DefaultPG,
                                 QC.Sema->getIdentifier("DefaultPrecedence"))) {
         return finish(DoneWithError);
      }

      D->setPrecedenceGroup(DefaultPG);
   }

   return finish();
}

QueryResult TypecheckOperatorQuery::run()
{
   llvm_unreachable("no need to run this!");
}

QueryResult PrepareGlobalVarInterfaceQuery::run()
{
   const SourceType& T = D->getType();
   auto res = QC.Sema->visitSourceType(D, T);
   if (!res) {
      return fail();
   }

   if (T->isAutoType()) {
      // We need to typecheck the value to know its type.
      finish();
      return QC.TypecheckDecl(D);
   }

   return finish();
}

QueryResult TypecheckGlobalVarQuery::run()
{
   auto valid = QC.Sema->visitVarDecl(D);
   if (!valid) {
      D->setIsInvalid(true);
      D->getType().setResolvedType(QC.Context.getErrorTy());

      return finish(DoneWithError);
   }

   if (D->getType()->isAutoType()) {
      D->getType().setResolvedType(D->getValue()->getExprType());
   }

   QC.Sema->getILGen().DeclareGlobalVariable(D);
   return finish();
}

QueryResult PrepareFuncArgInterfaceQuery::run()
{
   if (D->getDeclName().isStr("functionPointer")) {
       NO_OP;
   }
   auto& Context = QC.CI.getContext();
   if (D->isSelf()) {
      auto M = cast<MethodDecl>(D->getDeclContext());
      auto R = M->getRecord();

      QC.DeclareSelfAlias(R);

      QualType SelfTy;
      if (isa<ProtocolDecl>(R)) {
         auto* AT = R->getAssociatedType(QC.Sema->getIdentifier("Self"));
         SelfTy = Context.getAssociatedType(AT);
      }
      else {
         auto* AT = R->lookupSingle<AliasDecl>(QC.Sema->getIdentifier("Self"));
         SelfTy = Context.getTypedefType(AT);
      }

      SelfTy = QC.Sema->ApplyCapabilities(SelfTy, D->getDeclContext());

      ArgumentConvention Conv;
      if (M->isStatic() && !M->isBaseInitializer()) {
         SelfTy = Context.getMetaType(SelfTy);
         Conv = ArgumentConvention::Borrowed;
      }
      else {
         if (M->hasMutableSelf()) {
            SelfTy = Context.getMutableReferenceType(SelfTy);
            Conv = ArgumentConvention::MutableRef;
         }
         else {
            Conv = ArgumentConvention::Borrowed;
         }
      }

      D->setConvention(Conv);
      D->getType().setResolvedType(SelfTy);

      if (SelfTy->isDependentType()) {
         D->setIsTypeDependent(true);
      }

      M->setSelfType(SelfTy);
      return finish();
   }

   // Allow variadic parameter reference in argument type.
   auto* typeExpr = D->getType().getTypeExpr();
   if (typeExpr) {
      if (auto* ident = dyn_cast<IdentifierRefExpr>(typeExpr->ignoreParens())) {
         ident->setAllowVariadicRef(true);
      }
   }

   auto TypeRes = QC.Sema->visitSourceType(D, D->getType());
   if (!TypeRes || TypeRes.get()->isErrorType()) {
      D->setVariadicArgPackExpansion(false);
      return fail();
   }

   auto& DeclaredArgType = D->getType();
   if (DeclaredArgType->isVoidType()) {
      SourceLocation Loc;
      if (auto* E = D->getType().getTypeExpr()) {
         Loc = E->getSourceLoc();
      }
      else {
         Loc = D->getSourceLoc();
      }

      QC.Sema->diagnose(D, err_generic_error,
                        "function arguments may not be of type 'void'", Loc);
   }

   // Check if this argument is variadic.
   if (auto* paramType = DeclaredArgType->asTemplateParamType()) {
      if (paramType->getParam()->isVariadic()) {
         QualType argType = DeclaredArgType.getResolvedType();
         bool isReallyVariadic = true;

         if (auto* td = dyn_cast<TypedefType>(argType)) {
            if (td->getTypedef()->isVariadicForDecl()) {
               isReallyVariadic = false;
            }
         }

         D->setVariadic(isReallyVariadic);
      }
   }

   QC.Sema->checkIfTypeUsableAsDecl(DeclaredArgType, D);

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

   auto* fn = cast<CallableDecl>(D->getDeclContext());
   if (!fn->isInstantiation()
   && !fn->instantiatedFromProtocolDefaultImpl()
   && !D->isImportedFromModule()) {
      switch (D->getConvention()) {
      case ArgumentConvention::ImmutableRef:
         DeclaredArgType.setResolvedType(
             QC.Sema->Context.getReferenceType(DeclaredArgType));
         break;
      case ArgumentConvention::MutableRef:
         DeclaredArgType.setResolvedType(
             QC.Sema->Context.getMutableReferenceType(DeclaredArgType));
         break;
      default:
         break;
      }
   }

   return finish();
}

static void calculateDepth(TemplateParamDecl* P)
{
   unsigned Depth = 0;
   auto* Ctx = P->getDeclContext();
   while (Ctx) {
      Ctx = Ctx->lookThroughExtension();

      if (auto* ND = dyn_cast<NamedDecl>(Ctx)) {
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

   DeclContext* RealCtx = D->getDeclContext()->getParentCtx();
   calculateDepth(D);

   NamedDecl* OtherParam;
   if (QC.LookupSingle(OtherParam, RealCtx, D->getDeclName(),
                        LookupOpts::Restricted | LookupOpts::IssueDiag)) {
      QC.Sema->diagnose(D, err_template_param_shadow, D->getName(),
                        D->getSourceRange());
      QC.Sema->diagnose(note_template_parameter_here,
                        OtherParam->getSourceRange());
   }

   SemaPass::DeclScopeRAII DSR(*QC.Sema, RealCtx);

   if (auto& cov = D->getCovariance()) {
      if (!QC.Sema->visitSourceType(D, cov)) {
         return finish();
      }
   }

   if (auto& con = D->getContravariance()) {
      if (!QC.Sema->visitSourceType(D, con)) {
         return finish();
      }
   }

   if (D->getCovariance()->isAutoType()) {
      if (D->isTypeName()) {
         // Otherwhise use 'Any' as an upper bound.
         ProtocolDecl* Any;
         if (QC.GetBuiltinProtocol(Any, GetBuiltinProtocolQuery::Any)) {
            return fail();
         }

         D->getValueType().setResolvedType(QC.Sema->Context.getRecordType(Any));
      }
      else {
         // Value arguments default to 'Int'.
         D->getValueType().setResolvedType(QC.Sema->Context.getIntTy());
      }
   }

   if (D->isVariadic()) {
      // Variadic templates need a full instantiation.
      cast<Decl>(D->getDeclContext())->setUnboundedTemplate(true);
   }

   // FIXME move this check to the point where it's first needed.
   if (auto Def = D->getDefaultValue()) {
      auto Res = QC.Sema->typecheckExpr(Def, SourceType(), D);
      if (Res) {
         D->setDefaultValue(Res.get());
      }
   }

   return finish();
}

QueryResult TypecheckFuncArgQuery::run()
{
   const SourceType& DeclaredArgType = D->getType();
   if (Expression* DefaultVal = D->getDefaultVal()) {
      SemaPass::DefaultArgumentValueRAII defaultArgumentValueRAII(sema());

      auto Result = QC.Sema->typecheckExpr(DefaultVal, DeclaredArgType, D);
      if (Result) {
         D->setDefaultVal(Result.get());
      }
   }

   return finish();
}

QueryResult TypecheckTemplateParamQuery::run() { return finish(); }

static bool checkCompileTimeEvaluable(SemaPass& SP, DeclContext* Ctx)
{
   if (Ctx->getDeclModule()->getModule()->isCompileTimeByDefault()) {
      return true;
   }

   while (Ctx) {
      if (auto* D = dyn_cast<Decl>(Ctx)) {
         if (D->hasAttribute<CompileTimeAttr>()) {
            return true;
         }
         if (auto* Ext = dyn_cast<ExtensionDecl>(Ctx)) {
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
   Status S = Done;
   if (auto& Cov = D->getCovariance()) {
      auto TypeRes = QC.Sema->visitSourceType(D, Cov);
      if (!TypeRes) {
         S = DoneWithError;
      }
   }
   else {
      ProtocolDecl* Any = nullptr;
      QC.GetBuiltinProtocol(Any, GetBuiltinProtocolQuery::Any);

      assert(Any && "no Any protocol?");
      D->getCovariance().setResolvedType(QC.Sema->Context.getRecordType(Any));
   }

   if (auto& T = D->getDefaultType()) {
      auto TypeRes = QC.Sema->visitSourceType(D, T);
      if (!TypeRes)
         return fail();

      auto Ty = TypeRes.get();
      if (Ty->isAutoType()) {
         D->getDefaultType().setResolvedType(QC.Sema->ErrorTy);
      }
   }

   return finish(S);
}

QueryResult TypecheckAssociatedTypeQuery::run()
{
   return finish();
}

QueryResult PrepareCallableInterfaceQuery::run()
{
   // an error occured while parsing this function
   if (D->isInvalid()) {
      return fail();
   }

   // Verify that a template provides a body.
   if (D->isTemplate() && !D->getBody() && !D->getBodyTemplate()
       && !D->hasAttribute<_BuiltinAttr>() && !D->isProtocolRequirement()) {
      QC.Sema->diagnose(D, err_generic_error, D->getSourceLoc(),
                        "template function cannot be declared");
   }

   // Lookup program argument declarations.
   if (D->isMain() && !QC.CI.getOptions().noPrelude()) {
      Module* SysMod;
      if (QC.GetBuiltinModule(SysMod, GetBuiltinModuleQuery::Sys)) {
         return fail();
      }

      if (SysMod) {
         NamedDecl* _;
         QC.LookupSingle(_, SysMod->getDecl(), QC.Sema->getIdentifier("argc"));
         QC.LookupSingle(_, SysMod->getDecl(), QC.Sema->getIdentifier("argv"));
      }
   }

   // Check duplicate extern C functions.
   if (D->isExternC()) {
      if (!isa<FunctionDecl>(D)) {
         QC.Sema->diagnose(D, err_generic_error, "methods cannot be @extern(C)",
                           D->getSourceLoc());
      }

      if (!D->getDeclName().isSimpleIdentifier()) {
         QC.Sema->diagnose(D, err_cannot_be_extern_C, D->getSourceLoc());
      }
      else if (D->getBody() && !D->isLambda()) {
         auto Ret = QC.ExternCFuncs.try_emplace(
             D->getDeclName().getIdentifierInfo());

         if (!Ret.second) {
            QC.Sema->diagnose(D, err_redeclared_extern_c, D->getSourceLoc(),
                              D->getDeclName());

            auto* OtherFn = Ret.first->getSecond();
            QC.Sema->diagnose(note_previous_decl, OtherFn->getSourceLoc());
         }
      }
   }

   // Check if this function is callable at compile time.
   if (!D->isCompileTimeEvaluable() && checkCompileTimeEvaluable(sema(), D)) {
      D->setCompileTimeEvaluable(true);
   }

   for (auto& TP : D->getTemplateParams()) {
      if (QC.PrepareDeclInterface(TP)) {
         return fail();
      }

      D->copyStatusFlags(TP);
   }

   auto& Context = QC.CI.getContext();

   SemaPass::DeclScopeRAII declContextRAII(sema(), D);
   if (D->isOperator()) {
      auto OpName = D->getDeclName();
      if (OpName.getKind() == DeclarationName::InstantiationName)
         OpName = OpName.getInstantiationName();

      auto OpDeclName = Context.getDeclNameTable().getOperatorDeclName(OpName);

      OperatorDecl* Result;
      if (QC.FindOperator(Result, OpDeclName, true, D->getSourceRange())) {
         return fail();
      }

      if (!Result) {
         QC.Sema->diagnose(D, err_undeclared_operator, D->getSourceLoc(),
                           OpName);
      }
      else {
         D->setPrecedenceGroup(Result->getPrecedenceGroup());
      }
   }

   for (FuncArgDecl* ArgDecl : D->getArgs()) {
      if (QC.PrepareFuncArgInterface(ArgDecl)) {
         return fail();
      }

      if (!ArgDecl->isSelf() && D->isProtocolRequirement()
          && !D->isProtocolDefaultImpl()) {
         QualType Type = ArgDecl->getType();
         if (QC.Sema->ContainsAssociatedTypeConstraint(Type)) {
            cast<ProtocolDecl>(D->getRecord())
                ->setHasAssociatedTypeConstraint(true);
         }
      }

      D->copyStatusFlags(ArgDecl);
   }

   auto& Ret = D->getReturnType();
   auto Result = QC.Sema->visitSourceType(D, Ret);
   if (!Result) {
      return fail();
   }

   auto retTy = Ret.getResolvedType();
   if (retTy->isAutoType()) {
      Ret.setResolvedType(Context.getEmptyTupleType());
   }

   QC.Sema->checkIfTypeUsableAsDecl(Ret, D);

   bool NoReturn;
   if (QC.IsUnpopulated(NoReturn, retTy)) {
      NoReturn = false;
   }

   D->setIsNoReturn(NoReturn);

   if (D->isProtocolRequirement() && !D->isProtocolDefaultImpl()) {
      if (QC.Sema->ContainsAssociatedTypeConstraint(retTy)) {
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
   //         auto TemplateArgs = FinalTemplateArgumentList::Create(Context,{
   //         Arg }); auto Inst = Instantiator.InstantiateRecord(D, FutureDecl,
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
      auto Name = Context.getDeclNameTable().getConversionOperatorName(
          D->getReturnType());

      D->setName(Name);
      QC.Sema->makeDeclAvailable(*D->getDeclContext(), D);
   }

   D->createFunctionType(sema());
   QC.Sema->checkDuplicateFunctionDeclaration(D, "function");

   return finish();
}

QueryResult TypecheckCallableQuery::run()
{
   if (auto Err = QC.TypecheckConstraints(D)) {
      return Query::finish(Err);
   }

   if (D->isInvalid()) {
      return fail();
   }

   if (QC.TypeCheckDeclContext(D)) {
      return fail();
   }

   SemaPass::DeclScopeRAII raii(sema(), D);
   SemaPass::ScopeGuard scope(sema(), D);

   if (auto Body = D->getBody()) {
      auto res = QC.Sema->visitStmt(D, Body);
      if (!res)
         return fail();

      D->setBody(res.get());
   }

   return finish();
}

QueryResult PrepareFunctionInterfaceQuery::run()
{
   if (D->isLambda()) {
      return finish();
   }
   if (QC.PrepareCallableInterface(D)) {
      return fail();
   }

   return finish();
}

QueryResult TypecheckFunctionQuery::run()
{
   if (D->isLambda()) {
      return finish();
   }
   if (!D->getBody()) {
      return finish();
   }
   if (QC.TypecheckCallable(D)) {
      return fail();
   }

   return finish();
}

QueryResult PrepareModuleInterfaceQuery::run()
{
   SemaPass::DeclScopeRAII DSR(sema(), D);

   for (auto* InnerDecl : D->getDecls()) {
      if (QC.PrepareDeclInterface(InnerDecl)) {
         return fail();
      }
   }

   return finish();
}

QueryResult TypecheckModuleQuery::run()
{
   if (QC.TypeCheckDeclContext(D)) {
      return fail();
   }

   return finish();
}

QueryResult PrepareNamespaceInterfaceQuery::run()
{
   for (auto* InnerDecl : D->getDecls()) {
      if (QC.PrepareDeclInterface(InnerDecl)) {
         return fail();
      }
   }

   return finish();
}

QueryResult TypecheckNamespaceQuery::run()
{
   if (QC.TypeCheckDeclContext(D)) {
      return fail();
   }

   return finish();
}

QueryResult PrepareCompoundInterfaceQuery::run()
{
   for (auto* InnerDecl : D->getDecls()) {
      if (QC.PrepareDeclInterface(InnerDecl)) {
         return fail();
      }
   }

   return finish();
}

QueryResult TypecheckCompoundQuery::run()
{
   if (QC.TypeCheckDeclContext(D)) {
      return fail();
   }

   return finish();
}

QueryResult PrepareAliasInterfaceQuery::run()
{
   SemaPass::DeclScopeRAII DSR(sema(), D);

   auto TypeRes = QC.Sema->visitSourceType(D->getType(), true);
   if (!TypeRes) {
      return finish(DoneWithError);
   }

   if (!D->getAliasExpr()) {
      if (D->isImplOfProtocolRequirement()) {
         return finish();
      }

      if (D->hasAttribute<_BuiltinAttr>()) {
         QC.Sema->SetBuiltinAliasType(D);
      }
      else if (!D->isVariadicForDecl()) {
         if (!D->isInvalid()) {
            QC.Sema->diagnose(D, err_alias_without_value, D->getSourceRange());
         }

         return fail();
      }
   }

   // If the type is inferred, we need to typecheck the expression already.
   if (!TypeRes.get()->isAutoType() || !D->getAliasExpr()) {
      return finish();
   }

   SemaPass::DontApplyCapabilitiesRAII NoCapabilities(*QC.Sema);

   // Set status to 'Done' now, since this declaration can be reffered to in
   // the following expression.
   finish(Done);

   StaticExpr* Expr = D->getAliasExpr();
   auto ExprRes = QC.Sema->typecheckExpr(Expr->getExpr(), SourceType(), D);
   if (!ExprRes) {
      return fail();
   }

   D->getAliasExpr()->setExpr(ExprRes.get());
   D->getAliasExpr()->copyStatusFlags(ExprRes.get());

   QualType valueType = ExprRes.get()->getExprType();
   D->getType().setResolvedType(valueType);

   return finish();
}

QueryResult TypecheckAliasQuery::run()
{
   if (auto Err = QC.TypecheckConstraints(D)) {
      return Query::finish(Err);
   }

   StaticExpr* Expr = D->getAliasExpr();
   if (!Expr || Expr->isSemanticallyChecked()) {
      return finish();
   }

   SemaPass::DeclScopeRAII DSR(*QC.Sema, D);
   SemaPass::DontApplyCapabilitiesRAII NoCapabilities(*QC.Sema);

   auto ExprRes = QC.Sema->typecheckExpr(Expr->getExpr(), D->getType(), D);
   if (!ExprRes) {
      return fail();
   }

   Expression* Val = ExprRes.get();
   QualType GivenType = Val->getExprType();

   QC.Sema->checkDeclaredVsGivenType(D, Val, D->getType(), D->getType(),
                                     GivenType, true, D->getSourceLoc());

   D->getAliasExpr()->setExpr(Val);
   D->getAliasExpr()->copyStatusFlags(Val);

   return finish();
}