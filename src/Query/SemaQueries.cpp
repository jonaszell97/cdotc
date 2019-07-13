//
// Created by Jonas Zell on 24.08.18.
//

#include "Query.h"

#include "AST/Decl.h"
#include "Basic/NestedNameSpecifier.h"
#include "ILGen/ILGenPass.h"
#include "Module/Module.h"
#include "Module/ModuleManager.h"
#include "Parse/Parser.h"
#include "QueryContext.h"
#include "Sema/SemaPass.h"
#include "Support/Log.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::sema;
using namespace cdot::support;

QueryResult GetDefaultTemplateArgQuery::run()
{
   if (P->isVariadic()) {
      return finish(TemplateArgument(P, P->isTypeName(), {},
                                     P->getSourceLoc()));
   }
   if (P->isTypeName()) {
      return finish(TemplateArgument(P, QC.Sema->Context.getTemplateArgType(P),
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
   auto *DC = QC.CI.getCompilationModule()->getDecl();

   const MultiLevelLookupResult *LookupRes;
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

   auto *PG = dyn_cast<PrecedenceGroupDecl>(LookupRes->front().front());
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
   auto *DC = QC.CI.getCompilationModule()->getDecl();

   const MultiLevelLookupResult *LookupRes;
   if (QC.DirectLookup(LookupRes, DC, Name)) {
      return fail();
   }

   if (LookupRes->empty()) {
      if (DiagOnError) {
         QC.Sema->diagnose(err_undeclared_operator, Name, Loc);
      }

      return fail();
   }

   auto *Op = cast<OperatorDecl>(LookupRes->front().front());
   if (auto II = Op->getPrecedenceGroupIdent()) {
      PrecedenceGroupDecl *PG;
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

      if (Op->getDeclName().getDeclaredOperatorName()
            .getKind() == DeclarationName::InfixOperatorName) {
         QC.Sema->Context.setInfixOperatorPrecedence(Op->getDeclName()
                                                     .getDeclaredOperatorName()
                                                     .getInfixOperatorName(),
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
//   // Get the raw AST.
//   ast::SourceFileDecl *Mod;
//   if (QC.ParseSourceFile(Mod, SourceFile)) {
//      return fail();
//   }
//
//   // Iteratively expand all macro declarations.
//   std::queue<DeclContext*> ContextsToVisit;
//   ContextsToVisit.push(Mod);
//
//   while (!ContextsToVisit.empty()) {
//      auto *DC = ContextsToVisit.front();
//      ContextsToVisit.pop();
//
//      for (auto *Decl : DC->getDecls()) {
//         if (auto *InnerDC = dyn_cast<DeclContext>(Decl)) {
//            ContextsToVisit.push(InnerDC);
//         }
//         else if (auto *MD = dyn_cast<MacroExpansionDecl>(Decl)) {
//            ast::Decl *Expansion;
//            if (QC.ExpandMacroDecl(Expansion, MD)) {
//               return fail();
//            }
//
//            // Expansion will be visited as part of the for loop.
//         }
//      }
//   }
//
//   return finish();
}

QueryResult ExpandMacroQuery::run()
{
   NamedDecl *LookupRes;
   if (QC.LookupSingle(LookupRes, DC, Name, LookupOpts::None)) {
      return fail();
   }

   auto Macro = dyn_cast_or_null<MacroDecl>(LookupRes);
   parse::ParseResult Result;

   if (!Macro) {
      QC.Sema->diagnoseMemberNotFound(DC, SOD, Name,
                                      err_macro_does_not_exist);

      return fail();
   }
   else if (Delim != Macro->getDelim()) {
      llvm::StringRef ExpectedDelim;
      switch (Macro->getDelim()) {
      case MacroDecl::Paren: ExpectedDelim = "()"; break;
      case MacroDecl::Brace: ExpectedDelim = "{}"; break;
      case MacroDecl::Square: ExpectedDelim = "[]"; break;
      }

      QC.Sema->diagnose(SOD, err_macro_expects_delim, SOD.getSourceRange(),
                      Name, ExpectedDelim);
   }

   if (Macro) {
      LOG(MacroExpansions, "expanding macro ", Name);

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
   finish(nullptr);

   SemaPass::DeclScopeRAII DSR(sema(), Decl->getDeclContext());
   if (!QC.Sema->checkNamespaceRef(Decl))
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

   if (!Result) {
      return finish(nullptr);
   }

   return finish(Result.getDecl());
}

QueryResult ResolveUsingQuery::run()
{
   finish();

   const MultiLevelLookupResult *LookupRes;
   if (QC.NestedNameLookup(LookupRes, U->getDeclContext(),
                           U->getNestedImportName(),
                           DefaultLookupOpts | LookupOpts::IssueDiag,
                           U->getSourceLoc())) {
      return fail();
   }

   if (U->isWildcardImport()) {
      if (LookupRes->size() != 1 || LookupRes->front().size() != 1) {
         QC.Sema->diagnose(U, err_using_target_ambiguous, U->getSourceRange());

         NamedDecl *Cand1 = LookupRes->front().front();
         NamedDecl *Cand2;

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

      NamedDecl *Target = LookupRes->front().front();
      if (!isa<DeclContext>(Target)) {
         QC.Sema->diagnose(U, err_cannot_lookup_member_in,
                         Target->getSpecifierForDiagnostic(),
                         Target->getDeclName());

         return finish(DoneWithError);
      }

      if (auto *I = dyn_cast<ImportDecl>(Target)) {
         Target = I->getImportedModule()->getDecl();
      }

      LOG(UsingStatements, "made ", Target->getFullName(), " visible in ",
          U->getDeclContext()->getNameAsString());

      QC.Sema->makeDeclsAvailableIn(*U->getDeclContext(),
                                    *cast<DeclContext>(Target));
   }
   else {
      for (NamedDecl *ND : LookupRes->allDecls()) {
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

   auto *Mod = I->getImportedModule();
   if (!Mod)
      return fail();

   if (I->isWildcardImport() || Mod->allImplicitlyExported()) {
      LOG(ImportStatements, "imported all declarations of ", Mod->getFullName(),
          " into ", I->getDeclContext()->getNameAsString());

      I->getDeclContext()->addImportedModule(Mod);
      return finish();
   }

   if (I->getNamedImports().empty()) {
      LOG(ImportStatements, "imported ", Mod->getFullName(),
          " into ", I->getDeclContext()->getNameAsString());

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
         QC.Sema->diagnose(I, err_member_not_found, /*module*/ 9, Mod->getName(),
                         Name, I->getSourceRange());

         Valid = false;
         continue;
      }

      for (NamedDecl *ND : *LookupRes) {
         LOG(ImportStatements, "imported ", ND->getFullName(),
             " into ", I->getDeclContext()->getNameAsString());

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

      ast::Decl *Inst;
      if (CondIsTrue) {
         if (auto Err = QC.InstantiateDecl(Inst, Template->getIfDecl(),
                                           TemplateArgs)) {
            return Query::finish(Err);
         }
      }
      else if (auto Else = Template->getElseDecl()) {
         if (auto Err = QC.InstantiateDecl(Inst, Else, TemplateArgs)) {
            return Query::finish(Err);
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

   ast::Decl *D;
   if (CondIsTrue) {
      D = Decl->getIfDecl();
   }
   else if (auto *Else = Decl->getElseDecl()) {
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

QueryResult GetReferencedAssociatedTypesQuery::run()
{
   const IdentifierInfo *LookupIdent;
   auto NestedName = C->getNameQualifier();

   if (NestedName.front()->isStr("Self")) {
      if (NestedName.size() == 1) {
         LookupIdent = NestedName[0];
      }
      else {
         LookupIdent = NestedName[1];
      }
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
      QC.Sema->diagnoseMemberNotFound(DC, ConstrainedDecl, LookupIdent);
      return fail();
   }
   if (LookupRes->size() != 1 || LookupRes->front().size() != 1) {
      QC.Sema->diagnose(err_ambiguous_reference, LookupIdent);

      for (auto *D : LookupRes->allDecls()) {
         QC.Sema->diagnose(note_candidate_here, D->getSourceLoc());
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

   QC.Sema->diagnose(ConstrainedDecl, err_cannot_be_referenced_in_constraint,
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
      Extensions = QC.Sema->Context.getExtensions(T);
   }

   ResultKind RK = Ready;

   auto Constraints = QC.CI.getContext().getExtConstraints(ConstrainedDecl);
   for (DeclConstraint *C : Constraints) {
      ArrayRef<AssociatedTypeDecl*> ReferencedATs;
      if (QC.GetReferencedAssociatedTypes(ReferencedATs, ConstrainedDecl, C)) {
         return fail();
      }

      for (auto *AT : ReferencedATs) {
         // Check if the associated type is already visible.
         AssociatedTypeDecl *ATImpl;
         if (QC.GetAssociatedType(ATImpl, Rec, AT->getDeclName(), Extensions)) {
            return fail();
         }

         if (!ATImpl) {
            return finish(NotReady);
         }

         if (ATImpl->isSelf()) {
            RK = ReferencesSelf;
         }
      }
   }

   return finish(RK);
}

QueryResult CheckProtocolExtensionApplicabilityQuery::run()
{
   using ReadyKind = ReferencedAssociatedTypesReadyQuery::ResultKind;

   if (QC.PrepareDeclInterface(Ext)) {
      return fail();
   }

   auto Constraints = QC.CI.getContext().getExtConstraints(Ext);
   if (Constraints.empty()) {
      LOG(ExtensionApplicability, "extension @", Ext->getSourceLocAsString(),
          " applies to ", T.toDiagString());

      return finish(DoesApply);
   }

   ReadyKind IsReady;
   if (QC.ReferencedAssociatedTypesReady(IsReady, T, Ext)) {
      return fail();
   }

   if (IsReady == ReadyKind::NotReady) {
      return finish(CantTell);
   }

   bool AllSatisfied;
   for (auto *C : Constraints) {
      if (auto Err = QC.IsConstraintSatisfied(AllSatisfied, C, T, Ext)) {
         if (Err.isDependent()) {
            return finish(CantTell);
         }

         return fail();
      }

      if (!AllSatisfied) {
         LOG(ExtensionApplicability, "extension @", Ext->getSourceLocAsString(),
             " does not apply to ", T.toDiagString());

         return finish(DoesNotApply);
      }
   }

   LOG(ExtensionApplicability, "extension @", Ext->getSourceLocAsString(),
       " applies to ", T.toDiagString());

   return finish(DoesApply);
}

static DeclContextLookupResult
getAssociatedTypeOrTemplateParamImpl(QueryContext &QC,
                                     DeclContext *DC,
                                     DeclarationName Name) {
   auto *AT = DC->lookupSingle<AssociatedTypeDecl>(Name);
   if (AT && !AT->isImplementation()) {
      return AT;
   }

   auto *Param = DC->lookupSingle<TemplateParamDecl>(Name);
   if (Param) {
      return Param;
   }

   auto *R = dyn_cast<RecordDecl>(DC);
   if (!R) {
      return DeclContextLookupResult();
   }

   for (auto *Ext : R->getExtensions()) {
      auto *AT = Ext->lookupSingle<AssociatedTypeDecl>(Name);
      if (AT && !AT->isImplementation()) {
         return AT;
      }

      auto *Param = Ext->lookupSingle<TemplateParamDecl>(Name);
      if (Param) {
         return Param;
      }
   }

   auto *P = dyn_cast<ProtocolDecl>(R);
   if (!P) {
      return DeclContextLookupResult();
   }

   // Make sure unconditional conformances are declared.
   if (QC.ResolveExplicitConformances(QC.Context.getRecordType(P))) {
      return DeclContextLookupResult();
   }

   // If this is a protocol, look in conformances.
   auto Conformances = QC.Context.getConformanceTable().getAllConformances(R);
   for (auto *Conf : Conformances) {
      auto *AT = Conf->getProto()->lookupSingle<AssociatedTypeDecl>(Name);
      if (AT && !AT->isImplementation()) {
         return AT;
      }

      auto *Param = Conf->getProto()->lookupSingle<TemplateParamDecl>(Name);
      if (Param) {
         return Param;
      }
   }

   return DeclContextLookupResult();
}

static DeclContextLookupResult
getAssociatedTypeOrTemplateParamImpl(QueryContext &QC,
                                     QualType T,
                                     DeclarationName Name) {
   if (QualType RT = T->asRecordType()) {
      return getAssociatedTypeOrTemplateParamImpl(QC, RT->getRecord(), Name);
   }

   if (auto *Ext = T->asExistentialType()) {
      for (QualType ET : Ext->getExistentials()) {
         auto Result = getAssociatedTypeOrTemplateParamImpl(QC, ET, Name);
         if (!Result.empty()) {
            return Result;
         }
      }
   }

   return DeclContextLookupResult();
}

QueryResult VerifyConstraintQuery::run()
{
   DeclContext *CurCtx;
   if (auto *DC = dyn_cast<DeclContext>(ConstrainedDecl)) {
      CurCtx = DC->lookThroughExtension();
   }
   else {
      CurCtx = ConstrainedDecl->getDeclContext()->lookThroughExtension();
   }

   SemaPass::DeclScopeRAII DSR(*QC.Sema, CurCtx);
   if (C->getKind() == DeclConstraint::Concept) {
      auto *ConceptRef = C->getConceptRefExpr();
      ConceptRef->setAllowIncompleteTemplateArgs(true);

      auto Result = QC.Sema->typecheckExpr(ConceptRef, {}, ConstrainedDecl);
      if (!Result) {
         return fail();
      }

      if (Result.get()->isDependent()) {
         return finish(Dependent);
      }

      if (ConceptRef->getKind() != IdentifierKind::Alias) {
         QC.Sema->diagnose(ConceptRef, err_cannot_be_used_as_concept,
                         ConceptRef->getNamedDecl()->getSpecifierForDiagnostic(),
                         ConceptRef->getNamedDecl()->getDeclName(),
                         ConceptRef->getSourceRange());

         return fail();
      }

      auto *Concept = ConceptRef->getAlias();
      auto Params = Concept->getTemplateParams();

      if (Params.size() != 1 || !Params[0]->isTypeName()) {
         QC.Sema->diagnose(ConceptRef, err_not_a_valid_concept, 1,
                         ConceptRef->getSourceRange());

         return fail();
      }
   }
   else if (!QC.Sema->visitSourceType(ConstrainedDecl, C->getType())) {
      return fail();
   }

   ArrayRef<IdentifierInfo*> NameQual = C->getNameQualifier();
   unsigned NameQualSize = (unsigned)NameQual.size();

   bool Dependent = false;
   AssociatedType *CurrAT = nullptr;
   NestedNameSpecifier *Name = nullptr;
   auto &Tbl = QC.Context.getDeclNameTable();

   QualType LookupTy;

   unsigned i = 0;
   for (auto *Ident : NameQual) {
      DeclContextLookupResult Result;

      if (i == 0) {
         Result = getAssociatedTypeOrTemplateParamImpl(QC, CurCtx, Ident);
      }
      else {
         Result = getAssociatedTypeOrTemplateParamImpl(QC, LookupTy, Ident);
      }

      if (!Result) {
         if (i == 0) {
            QC.Sema->diagnose(ConstrainedDecl, err_member_not_found,
                              cast<NamedDecl>(
                                 CurCtx)->getSpecifierForDiagnostic(),
                              cast<NamedDecl>(CurCtx)->getDeclName(),
                              Ident->getIdentifier(), C->getSourceRange());
         }
         else {
            QC.Sema->diagnose(ConstrainedDecl, err_generic_error,
                              LookupTy.toDiagString()
                                 + " does not have a member named "
                                 + Ident->getIdentifier().str(),
                              C->getSourceRange());
         }

         return fail();
      }

      if (Result.size() != 1) {
         QC.Sema->diagnose(ConstrainedDecl, err_ambiguous_reference,
                         Ident->getIdentifier(), C->getSourceRange());

         for (auto *ND : Result) {
            QC.Sema->diagnose(note_candidate_here, ND->getSourceLoc());
         }

         return fail();
      }

      if (auto *AT = dyn_cast<AssociatedTypeDecl>(Result.front())) {
         if (auto Err = QC.PrepareDeclInterface(AT)) {
            return Query::finish(Err);
         }

         CurrAT = QC.Context.getAssociatedType(AT, CurrAT);
         Name = NestedNameSpecifier::Create(Tbl, AT, Name);

         if (i == NameQualSize - 1) {
            break;
         }

         LookupTy = AT->getCovariance();
      }
      else if (auto *P = dyn_cast<TemplateParamDecl>(Result.front())) {
         if (i == NameQualSize - 1) {
            C->setConstrainedType(QC.Context.getTemplateArgType(P));
            return finish();
         }

         Dependent = true;
         Name = NestedNameSpecifier::Create(Tbl, P, Name);

         LookupTy = P->getCovariance();
      }
      else {
         QC.Sema->diagnose(ConstrainedDecl,
                          err_cannot_be_referenced_in_constraint,
                          Result.front()->getSpecifierForDiagnostic(),
                          Result.front()->getDeclName(),
                          C->getSourceRange());

         return fail();
      }

      ++i;
   }

   if (Dependent) {
      while (i < NameQualSize) {
         Name = NestedNameSpecifier::Create(Tbl, NameQual[i++], Name);
      }

      SmallVector<SourceRange, 2> Locs;
      Locs.resize(NameQualSize);

      auto *WithLoc = NestedNameSpecifierWithLoc::Create(Tbl, Name, Locs);
      C->setConstrainedType(QC.Context.getDependentNameType(WithLoc));
   }
   else {
      C->setConstrainedType(CurrAT);
   }

   return finish();
}

QueryResult GetConstrainedTypeQuery::run()
{
   if (QueryResult Err = QC.VerifyConstraint(C, OriginalDecl)) {
      return Query::finish(Err);
   }

   SourceRange SR = C->getSourceRange();
   QualType ConstrainedType = C->getConstrainedType();

   if (ConstrainedType->containsAssociatedType()) {
      if (auto Err = QC.SubstAssociatedTypes(ConstrainedType, ConstrainedType,
                                             Self, SR)) {
         return Query::finish(Err);
      }
   }
   if (ConstrainedType->containsGenericType()
         && Self->isRecordType()
         && Self->getRecord()->isInstantiation()) {
      if (auto Err = QC.SubstGenericTypes(ConstrainedType, ConstrainedType,
                                          Self->getRecord()->getTemplateArgs(),
                                          SR)) {
         return Query::finish(Err);
      }
   }

   if (ConstrainedType->isDependentType()) {
      return finish(ConstrainedType, Dependent);
   }

   return finish(ConstrainedType);
}

QueryResult IsConstraintSatisfiedQuery::run()
{
   QualType ConstrainedType;
   if (auto Err = QC.GetConstrainedType(ConstrainedType, C, Self,
                                        OriginalDecl)) {
      return Query::finish(Err);
   }

   if (ConstrainedType->isDependentType()
   || ConstrainedType->containsAssociatedType()) {
      return finish(true, Dependent);
   }

   auto *Rec = Self->getRecord();
   SemaPass::DeclScopeRAII DSR(*QC.Sema, Rec);

   CanType Canon = ConstrainedType->getCanonicalType();

   switch (C->getKind()) {
   case DeclConstraint::Concept: {
      IdentifierRefExpr *ConceptRef = C->getConceptRefExpr();
      AliasDecl *Concept = ConceptRef->getAlias();

      TemplateArgument Arg(Concept->getTemplateParams().front(),
                           Canon, C->getSourceRange().getStart());

      auto *FinalList = sema::FinalTemplateArgumentList::Create(QC.Context,
                                                                Arg);

      AliasDecl *Inst;
      if (QC.InstantiateAlias(Inst, Concept, FinalList,
                              C->getSourceRange().getStart())) {
         return finish(true, DoneWithError);
      }

      if (QC.TypecheckDecl(Inst)) {
         return finish(true, DoneWithError);
      }

      if (Inst->getAliasExpr()->getExprType() != QC.Context.getBoolTy()) {
         QC.Sema->diagnose(Rec, err_concept_must_be_bool,
                           Inst->getDeclName(), Inst->getSourceRange());

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
         if (QC.SubstAssociatedTypes(RHSType, RHSType, Self,
                                     C->getSourceRange())) {
            return finish(true, DoneWithError);
         }
      }

      if (Canon->isDependentType() || RHSType->isDependentType()) {
         return finish(true, Dependent);
      }

      if (C->getKind() == DeclConstraint::TypeEquality) {
         return finish(Canon == RHSType.getCanonicalType());
      }
      if (C->getKind() == DeclConstraint::TypeInequality) {
         return finish(Canon != RHSType.getCanonicalType());
      }

      ArrayRef<QualType> Types;
      if (auto *Ext = RHSType->asExistentialType()) {
         Types = Ext->getExistentials();
      }
      else {
         Types = RHSType;
      }

      bool Result;
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
            if (auto Err = QC.ConformsTo(Result, Canon,
                                         cast<ProtocolDecl>(ET->getRecord()))) {
               return Query::finish(Err);
            }
         }

         if (!Result) {
            break;
         }
      }

      if (C->getKind() == DeclConstraint::TypePredicateNegated) {
         Result = !Result;
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
      QC.Sema->diagnose(err_private_access, ND->getSpecifierForDiagnostic(),
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
      QC.Sema->diagnose(err_protected_access, ND->getSpecifierForDiagnostic(),
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
      QC.Sema->diagnose(err_fileprivate_access, ND->getSpecifierForDiagnostic(),
                      ND->getDeclName(), FileMgr.getFileName(DeclID),
                      Loc);

      break;
   }
   case AccessSpecifier::Internal: {
      if (DC->getDeclModule()->getBaseModule()
          == ND->getModule()->getBaseModule()) {
         return finish();
      }

      QC.Sema->diagnose(err_internal_access, ND->getSpecifierForDiagnostic(),
                      ND->getDeclName(),
                      ND->getModule()->getBaseModule()->getDeclName(),
                      Loc);

      break;
   }
   }

   QC.Sema->diagnose(note_access_spec_here, /*implicitly*/ !ND->getAccessRange(),
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
   for (auto &TP : Decl->getTemplateParams()) {
      if (QC.PrepareTemplateParamInterface(TP)) {
         St = DoneWithError;
      }
   }

   return finish(St);
}

namespace {

struct BeingEvaluatedRAII {
   explicit BeingEvaluatedRAII(Decl *D)
      : D(D)
   {
      D->setBeingEvaluated(true);
   }

   ~BeingEvaluatedRAII()
   {
      D->setBeingEvaluated(false);
   }

private:
   Decl *D;
};

} // anonymous namespace

static void setDeclStatus(Decl *D, const QueryResult &Res)
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

#  define PREPARE_DECL(NAME)                                                         \
   case Decl::NAME##DeclID:                                                          \
      if (auto Err = QC.Prepare##NAME##Interface(static_cast<NAME##Decl*>(D))) {     \
         setDeclStatus(D, Err);                                                      \
         return Query::finish(Err);                                                  \
      }                                                                              \
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

#  undef PREPARE_DECL

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

#  define TYPECHECK_DECL(NAME)                                               \
   case Decl::NAME##DeclID:                                                  \
      if (auto Err = QC.Typecheck##NAME(static_cast<NAME##Decl*>(D))) {      \
         setDeclStatus(D, Err);                                              \
         return Query::finish(Err);                                          \
      }                                                                      \
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

#  undef TYPECHECK_DECL

   QC.Sema->checkDeclAttrs(D, Attr::AfterSema);

   return finish();
}

QueryResult TypeCheckDeclContextQuery::run()
{
   SemaPass::DeclScopeRAII DSR(sema(), DC);

   if (QC.PrepareNameLookup(DC)) {
      return fail();
   }

   for (auto *D : DC->getDecls()) {
      QC.TypecheckDecl(D);
   }

   return finish();
}

QueryResult TypecheckConstraintsQuery::run()
{
   SemaPass::DeclScopeRAII DSR(*QC.Sema, cast<DeclContext>(ND));

   auto Constraints = ND->getConstraints();
   for (auto *C : Constraints) {
      QC.Sema->typecheckExpr(C, SourceType(), ND);
   }

   return finish();
}

QueryResult PreparePrecedenceGroupInterfaceQuery::run()
{
   if (auto HigherThan = D->getHigherThanIdent()) {
      PrecedenceGroupDecl *HT;
      if (QC.FindPrecedenceGroup(HT, HigherThan)) {
         return finish(DoneWithError);
      }

      D->setHigherThan(HT);
   }

   if (auto LowerThan = D->getLowerThanIdent()) {
      PrecedenceGroupDecl *LT;
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
      PrecedenceGroupDecl *PG;
      if (QC.FindPrecedenceGroup(PG, II)) {
         return finish(DoneWithError);
      }

      if (!PG) {
         D->setPrecedenceGroup(PG);
      }

      if (D->getDeclName().getDeclaredOperatorName()
           .getKind() == DeclarationName::InfixOperatorName) {
         QC.Sema->Context.setInfixOperatorPrecedence(D->getDeclName()
                                                    .getDeclaredOperatorName()
                                                    .getInfixOperatorName(),
                                                   PG);
      }
   }

   if (!D->getPrecedenceGroup()) {
      PrecedenceGroupDecl *DefaultPG;
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
   const SourceType &T = D->getType();
   auto res = QC.Sema->visitSourceType(D, T);
   if (!res) {
      return fail();
   }

   if (T->isAutoType())  {
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
      D->getType().setResolvedType(QC.Context.getUnknownAnyTy());

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
   auto &Context = QC.CI.getContext();
   if (D->isSelf()) {
      auto M = cast<MethodDecl>(D->getDeclContext());
      auto R = M->getRecord();

      QC.DeclareSelfAlias(R);

      ArgumentConvention Conv;
      auto *AT = R->getAssociatedType(QC.Sema->getIdentifier("Self"));

      QualType SelfTy;
      if (isa<ProtocolDecl>(R)) {
         SelfTy = Context.getAssociatedType(AT);
      }
      else {
         SelfTy = Context.getAssociatedType(AT);
      }

      QC.ApplyCapabilites(SelfTy, SelfTy, D->getDeclContext());

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

      if (SelfTy->isDependentType()) {
         D->setIsTypeDependent(true);
      }

      M->setSelfType(SelfTy);
      return finish();
   }

   auto TypeRes = QC.Sema->visitSourceType(D, D->getType());
   if (!TypeRes || TypeRes.get()->isErrorType()) {
      D->setVariadicArgPackExpansion(false);
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

      QC.Sema->diagnose(D, err_generic_error,
                      "function arguments may not be of type 'void'",
                      Loc);
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

   DeclContext *RealCtx = D->getDeclContext()->getParentCtx();
   calculateDepth(D);

   NamedDecl *OtherParam;
   if (QC.LookupSingle(OtherParam, RealCtx, D->getDeclName())) {
      QC.Sema->diagnose(D, err_template_param_shadow, D->getName(),
                      D->getSourceRange());
      QC.Sema->diagnose(note_template_parameter_here,
                      OtherParam->getSourceRange());
   }

   SemaPass::DeclScopeRAII DSR(*QC.Sema, RealCtx);

   if (auto &cov = D->getCovariance()) {
      if (!QC.Sema->visitSourceType(D, cov)) {
         return finish();
      }
   }

   if (auto &con = D->getContravariance()) {
      if (!QC.Sema->visitSourceType(D, con)) {
         return finish();
      }
   }

   if (D->getCovariance()->isAutoType()) {
      if (D->isTypeName()) {
         // Otherwhise use 'Any' as an upper bound.
         ProtocolDecl *Any;
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
   const SourceType &DeclaredArgType = D->getType();
   if (Expression *DefaultVal = D->getDefaultVal()) {
      SemaPass::DefaultArgumentValueRAII defaultArgumentValueRAII(sema());

      auto Result = QC.Sema->typecheckExpr(DefaultVal, DeclaredArgType, D);
      if (Result) {
         D->setDefaultVal(Result.get());
      }
   }

   return finish();
}

QueryResult TypecheckTemplateParamQuery::run()
{
   return finish();
}

static bool checkCompileTimeEvaluable(SemaPass &SP, DeclContext *Ctx)
{
   if (Ctx->getDeclModule()->getModule()->isCompileTimeByDefault()) {
      return true;
   }

   while (Ctx) {
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
   Status S = Done;
   if (auto &Cov = D->getCovariance()) {
      if (D->isImplementation()) {
         QC.Sema->diagnose(D, err_generic_error,
                         "cannot define covariance on an associated type "
                         "definition",
                         D->getSourceRange());
      }

      auto TypeRes = QC.Sema->visitSourceType(D, Cov);
      if (!TypeRes) {
         S = DoneWithError;
      }
   }
   else if (!D->isImplementation()) {
      ProtocolDecl *Any = nullptr;
      QC.GetBuiltinProtocol(Any, GetBuiltinProtocolQuery::Any);

      assert(Any && "no Any protocol?");
      D->getCovariance().setResolvedType(QC.Sema->Context.getRecordType(Any));
   }

   if (auto &T = D->getActualType()) {
      auto TypeRes = QC.Sema->visitSourceType(D, T);
      if (!TypeRes)
         return fail();

      auto Ty = TypeRes.get();
      if (Ty->isAutoType()) {
         D->getActualType().setResolvedType(QC.Sema->UnknownAnyTy);
      }

      if (D->isImplementation()) {
         // Verify that the AssociatedType instance for this declaration has not
         // been used yet.
         assert(QC.Context.getAssociatedType(D)->getCanonicalType()
            == D->getActualType()->getCanonicalType());
      }
   }

   return finish(S);
}

QueryResult TypecheckAssociatedTypeQuery::run()
{
   auto *ATDecl = D;
   auto Rec = ATDecl->getRecord();

   // must be in a dependent extension decl, ignore for now
   if (!Rec || Rec->isTemplate())
      return finish();

   if (ATDecl->getDeclName().isStr("Self"))
      return finish();

   auto &Context = QC.Sema->Context;
   if (ATDecl->isImplementation()) {
      AssociatedTypeDecl* AT = nullptr;

      if (auto *P = dyn_cast<ProtocolDecl>(Rec)) {
         auto MaybeAT = P->getAssociatedType(ATDecl->getDeclName());
         if (MaybeAT) {
            AT = MaybeAT;
         }
      }

      if (!AT) {
         auto Conformances = Context.getConformanceTable().getAllConformances(Rec);
         for (auto *Conf : Conformances) {
            auto *CF = Conf->getProto();
            auto MaybeAT = CF->getAssociatedType(ATDecl->getDeclName());
            if (MaybeAT) {
               AT = MaybeAT;

               break;
            }
         }
      }

      if (!AT) {
         QC.Sema->diagnose(ATDecl, err_no_such_associated_type,
                         Rec->getSpecifierForDiagnostic(), Rec->getDeclName(),
                         ATDecl->getDeclName(), ATDecl->getSourceLoc());

         return fail();
      }

      if (!ATDecl->getActualType()) {
         if (!AT->getActualType()) {
            QC.Sema->diagnose(ATDecl, err_associated_type_no_default,
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
   if (D->isInvalid()) {
      return fail();
   }

   // Verify that a template provides a body.
   if (D->isTemplate()
         && !D->getBody() && !D->getBodyTemplate()
         && !D->hasAttribute<_BuiltinAttr>()
         && !D->isProtocolRequirement()) {
      QC.Sema->diagnose(D, err_generic_error, D->getSourceLoc(),
                        "template function cannot be declared");
   }

   // Lookup program argument declarations.
   if (D->isMain() && !QC.CI.getOptions().noPrelude()) {
      Module *SysMod;
      if (QC.GetBuiltinModule(SysMod, GetBuiltinModuleQuery::Sys)) {
         return fail();
      }

      if (SysMod) {
         NamedDecl *_;
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

            auto *OtherFn = Ret.first->getSecond();
            QC.Sema->diagnose(note_previous_decl, OtherFn->getSourceLoc());
         }
      }
   }

   // Check if this function is callable at compile time.
   if (!D->isCompileTimeEvaluable() && checkCompileTimeEvaluable(sema(), D)) {
      D->setCompileTimeEvaluable(true);
   }

   for (auto &TP : D->getTemplateParams()) {
      if (QC.PrepareDeclInterface(TP)) {
         return fail();
      }

      D->copyStatusFlags(TP);
   }

   auto &Context = QC.CI.getContext();

   SemaPass::DeclScopeRAII declContextRAII(sema(), D);
   QC.Sema->visitConstraints(D);

   if (D->isOperator()) {
      auto OpName = D->getDeclName();
      if (OpName.getKind() == DeclarationName::InstantiationName)
         OpName = OpName.getInstantiationName();

      auto OpDeclName = Context.getDeclNameTable().getOperatorDeclName(OpName);

      OperatorDecl *Result;
      if (QC.FindOperator(Result, OpDeclName, true, D->getSourceRange())) {
         return fail();
      }

      if (!Result) {
         QC.Sema->diagnose(D, err_undeclared_operator, D->getSourceLoc(), OpName);
      }
      else {
         D->setPrecedenceGroup(Result->getPrecedenceGroup());
      }
   }

   for (FuncArgDecl *ArgDecl : D->getArgs()) {
      if (QC.PrepareFuncArgInterface(ArgDecl)) {
         return fail();
      }

      if (!ArgDecl->isSelf()
            && D->isProtocolRequirement()
            && !D->isProtocolDefaultImpl()) {
         QualType Type = ArgDecl->getType();
         if (QC.Sema->ContainsAssociatedTypeConstraint(Type)) {
            cast<ProtocolDecl>(D->getRecord())
               ->setHasAssociatedTypeConstraint(true);
         }
      }

      D->copyStatusFlags(ArgDecl);
   }

   auto &Ret = D->getReturnType();
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

   for (auto *InnerDecl : D->getDecls()) {
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
   for (auto *InnerDecl : D->getDecls()) {
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
   for (auto *InnerDecl : D->getDecls()) {
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

   auto TypeRes = QC.Sema->visitSourceType(D->getType());
   if (!TypeRes) {
      return finish(DoneWithError);
   }

   if (!D->getAliasExpr()) {
      if (!D->hasAttribute<_BuiltinAttr>()) {
         QC.Sema->diagnose(D, err_alias_without_value, D->getSourceRange());
         return fail();
      }

      QC.Sema->SetBuiltinAliasType(D);
   }

   // If the type is inferred, we need to typecheck the expression already.
   if (!TypeRes.get()->isAutoType() || !D->getAliasExpr()) {
      return finish();
   }

   // Set status to 'Done' now, since this declaration can be reffered to in
   // the following expression.
   finish(Done);

   auto ExprRes = QC.Sema->typecheckExpr(D->getAliasExpr(), SourceType(), D);
   if (!ExprRes) {
      return fail();
   }

   D->setAliasExpr(cast<StaticExpr>(ExprRes.get()));

   QualType valueType = ExprRes.get()->getExprType();
   D->getType().setResolvedType(valueType);

   return finish();
}

QueryResult TypecheckAliasQuery::run()
{
   if (auto Err = QC.TypecheckConstraints(D)) {
      return Query::finish(Err);
   }

   StaticExpr *Expr = D->getAliasExpr();
   if (!Expr || Expr->isSemanticallyChecked()) {
      return finish();
   }

   SemaPass::DeclScopeRAII DSR(sema(), D);

   auto ExprRes = QC.Sema->typecheckExpr(Expr, D->getType(), D);
   if (!ExprRes) {
      return fail();
   }

   Expression *Val = ExprRes.get();

   QualType GivenType = Val->getExprType();
   QC.Sema->checkDeclaredVsGivenType(D, Val, D->getType(),
                                     D->getType(), GivenType, true,
                                     D->getSourceLoc());

   D->getAliasExpr()->setExpr(Val);
   return finish();
}