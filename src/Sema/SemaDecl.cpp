//
// Created by Jonas Zell on 08.09.17.
//

#include "Driver/Compiler.h"

#include "AST/Decl.h"
#include "AST/TypeBuilder.h"
#include "AST/TypeVisitor.h"
#include "Basic/FileManager.h"
#include "ConformanceChecker.h"
#include "IL/Constants.h"
#include "ILGen/ILGenPass.h"
#include "Message/Diagnostics.h"
#include "Module/Module.h"
#include "Module/ModuleManager.h"
#include "Lex/Lexer.h"
#include "Parse/Parser.h"
#include "Sema/Builtin.h"
#include "SemaPass.h"
#include "Serialization/ModuleFile.h"
#include "Support/Casting.h"
#include "Support/StringSwitch.h"
#include "TemplateInstantiator.h"
#include "Template.h"

#include <cassert>
#include <cstdlib>
#include <immintrin.h>
#include <llvm/ADT/ScopeExit.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/Twine.h>
#include <llvm/Support/MemoryBuffer.h>
#include <llvm/Support/PrettyStackTrace.h>

using namespace cdot::support;
using namespace cdot::diag;
using namespace cdot::lex;
using namespace cdot::sema;
using namespace cdot::module;

namespace cdot {
namespace ast {

namespace {

class DeclPrettyStackTraceEntry: public llvm::PrettyStackTraceEntry {
   NamedDecl *D;

public:
   DeclPrettyStackTraceEntry(NamedDecl *D)
      : D(D)
   {}

   void print(llvm::raw_ostream &OS) const override
   {
      OS << "while declaring " << D->getDeclName() << "\n";
   }
};

} // anonymous namespace

Type *SemaPass::getBuiltinType(DeclarationName typeName)
{
   if (!typeName.isSimpleIdentifier())
      return nullptr;

   return StringSwitch<Type*>(typeName.getIdentifierInfo()
                                            ->getIdentifier())
#     define CDOT_BUILTIN_INT(Name, BW, Unsigned)           \
      .Case(#Name, Context.get##Name##Ty())
#     define CDOT_BUILTIN_FP(Name, Precision)               \
      .Case(#Name, Context.get##Name##Ty())
#     include "Basic/BuiltinTypes.def"
      .Case("isize", Context.getIntTy())
      .Case("usize", Context.getUIntTy())
      .Default(nullptr);
}

bool SemaPass::resolvePrecedenceGroups()
{
   auto Order = PrecedenceDependency.constructOrderedList();
   if (!Order.second) {
      auto Pair = PrecedenceDependency.getOffendingPair();

      auto Fst = LookupSingle<PrecedenceGroupDecl>(*Pair.first.InModule,
                                                   Pair.first.Name);

      diagnose(err_precedence_cycle, DeclarationName(Pair.first.Name),
               DeclarationName(Pair.second.Name), Fst->getSourceLoc());

      return true;
   }

   for (auto &Ident : Order.first) {
      auto PG = LookupSingle<PrecedenceGroupDecl>(*Ident.InModule, Ident.Name);
      assert(PG && "undeclared precedence group");

      declareScoped(PG);
   }

   return EncounteredError;
}

static void HandleExtension(SemaPass &SP, ExtensionDecl *Ext)
{
   SemaPass::DeclScopeRAII declContextRAII(SP, Ext);
   SemaPass::AllowIncompleteTemplateTypeRAII raii(SP);

   auto TypeRes = SP.visitSourceType(Ext, Ext->getExtendedType());
   if (!TypeRes || !TypeRes.get()->isRecordType())
      return;

   QualType Ty = TypeRes.get();
   if (Ty->isDependentRecordType())
      Ty = SP.getContext().getRecordType(Ty->getRecord());

   Ext->setExtendedRecord(Ty->getRecord());
   Ext->setName(SP.getContext().getDeclNameTable().getExtensionName(Ty));

   SP.registerExtension(Ext);
}

void SemaPass::makeExtensionVisible(ExtensionDecl *Ext)
{
   // The visibility of extensions depends on their access specifier.
   DeclContext *VisibleCtx;
   switch (Ext->getAccess()) {
   case AccessSpecifier::Public:
      VisibleCtx = getCompilationUnit().getCompilationModule()->getDecl();
      break;
   case AccessSpecifier::Private:
      VisibleCtx = Ext->getExtendedRecord();
      break;
   case AccessSpecifier::Internal: {
      auto *ExtMod = Ext->getModule()->getModule()->getBaseModule();
      auto *ThisMod = getCompilationUnit().getCompilationModule();

      if (ExtMod == ThisMod) {
         VisibleCtx = ThisMod->getDecl();
      }
      else {
         VisibleCtx = ExtMod->getDecl();
      }

      break;
   }
   case AccessSpecifier::FilePrivate:
      VisibleCtx = Ext->getModule();
      break;
   default:
      llvm_unreachable("bad access spec");
   }

   makeDeclAvailable(*VisibleCtx, Ext);
}

void SemaPass::registerExtension(ExtensionDecl *Ext)
{
   auto *ExtendedRec = Ext->getExtendedRecord();
   if (Ext->isInvalid() || !ExtendedRec)
      return;

   // Always add the extension in a dependent context.
   if (ExtendedRec->inDependentContext() || isa<ProtocolDecl>(ExtendedRec)) {
      Ext->getExtendedRecord()->addExtension(Ext);
      visitConstraints(Ext);
   }
   else {
      // Otherwise check if all constraints are satisfied.
      auto ConstraintRes = checkConstraints(Ext, ExtendedRec,
                                            Ext->getConstraints());

      assert(!ConstraintRes.isDependent());
      if (ConstraintRes.getFailedConstraint())
         return;

      Ext->getExtendedRecord()->addExtension(Ext);
   }

   makeExtensionVisible(Ext);

   bool IsProto = isa<ProtocolDecl>(ExtendedRec);
   if (!IsProto) {
      registerExplicitConformances(ExtendedRec, Ext->getConformanceTypes());
      registerImplicitAndInheritedConformances(ExtendedRec,
                                               Ext->getConformanceTypes());

      auto &R = *ExtendedRec;
      for (auto *D : Ext->getDecls()) {
         if (auto *M = dyn_cast<MethodDecl>(D)) {
            if (!M->getMethodID()) {
               M->setMethodID(R.getAndIncrementLastMethodID());
            }
         }
      }
   }

   if (ExtendedRec->wasDeclared()) {
      ensureDeclared(Ext);
   }
}

static bool checkProtocolDefaultDecl(SemaPass &SP, ProtocolDecl *P, Decl *D)
{
   if (D->isInvalid())
      return true;

   bool Found = false;
   if (auto *AT = dyn_cast<AssociatedTypeDecl>(D)) {
      auto Result = SP.LookupSingle<AssociatedTypeDecl>(*P, AT->getDeclName());
      if (Result) {
         Found = true;
         SP.getContext().addProtocolDefaultImpl(P, Result, AT);

         SP.getContext().getAssociatedType(AT)
           ->setCanonicalType(SP.getContext().getAssociatedType(Result));
      }
   }
   else if (auto *Prop = dyn_cast<PropDecl>(D)) {
      auto Result = SP.LookupSingle<PropDecl>(*P, Prop->getDeclName());
      if (Result && Result->isInvalid()) {
         Found = true;
         SP.getContext().addProtocolDefaultImpl(P, Result, Prop);
      }
      else if (Result
            && Result->getType() == Prop->getType()
            && (!Prop->hasGetter() || Result->hasGetter())
            && (!Prop->hasSetter() || Result->hasSetter())) {
         Found = true;
         SP.getContext().addProtocolDefaultImpl(P, Result, Prop);
      }
   }
   else if (auto *Sub = dyn_cast<SubscriptDecl>(D)) {
      auto Result = SP.Lookup(*P, Sub->getDeclName());
      for (auto *OtherDecl : Result) {
         auto *OtherSub = dyn_cast<SubscriptDecl>(OtherDecl);
         if (!OtherSub || OtherSub == Sub)
            continue;

         if (OtherSub->isInvalid()) {
            Found = true;
            SP.getContext().addProtocolDefaultImpl(P, OtherSub, Sub);

            break;
         }

         if (OtherSub->hasGetter()) {
            if (!Sub->hasGetter())
               continue;

            if (SP.ensureDeclared(OtherSub)
               && SP.signaturesCompatible(Sub->getGetterMethod(),
                                          OtherSub->getGetterMethod()) != -1) {
               continue;
            }
         }

         if (OtherSub->hasSetter()) {
            if (!Sub->hasSetter())
               continue;

            if (SP.ensureDeclared(OtherSub)
                && SP.signaturesCompatible(Sub->getSetterMethod(),
                                           OtherSub->getSetterMethod()) != -1) {
               continue;
            }
         }

         Found = true;
         SP.getContext().addProtocolDefaultImpl(P, OtherSub, Sub);

         break;
      }
   }
   else if (auto *M = dyn_cast<MethodDecl>(D)) {
      auto Result = SP.Lookup(*P, M->getDeclName());
      for (auto *Impl : Result) {
         if (Impl == M)
            continue;

         auto *OtherMethod = dyn_cast<MethodDecl>(Impl);
         if (OtherMethod && (OtherMethod->isInvalid()
         || (SP.ensureDeclared(OtherMethod)
             && SP.signaturesCompatible(M,
                                        cast<MethodDecl>(OtherMethod))==-1))) {
            Found = true;
            SP.getContext().addProtocolDefaultImpl(P, OtherMethod, M);

            break;
         }
      }
   }

   if (!Found) {
      auto Conformances = SP.getContext().getConformanceTable()
                            .getAllConformances(P);

      for (auto &Conf : Conformances) {
         if (checkProtocolDefaultDecl(SP, Conf->getProto(), D)) {
            return true;
         }
      }

      return false;
   }

   D->setIsProtocolDefaultImpl(true);
   return true;
}

void SemaPass::checkProtocolExtension(ExtensionDecl *Ext, ProtocolDecl *P)
{
   ensureDeclared(P);
   if (P->isInvalid())
      return;

   // Check that every decl overrides a protocol requirement.
   for (auto *D : Ext->getDecls()) {
      auto *ND = dyn_cast<NamedDecl>(D);
      if (!ND) {
         diagnose(err_generic_error, "cannot appear in protocol extension",
                  D->getSourceLoc());

         continue;
      }

      if (!ND->getAccessLoc())
         ND->setAccess(AccessSpecifier::Public);

      if (!checkProtocolDefaultDecl(*this, P, D)) {
         ND->setIsProtocolDefaultImpl(true);
         ND->setIsProtocolRequirement(true);

         Context.addProtocolDefaultImpl(P, ND, ND);
      }
   }

   auto *SelfVertex = &ConformanceDependency.getOrAddVertex(P);

   // Check that all additional conformances are satisfied.
   for (auto &Conf : Ext->getConformanceTypes()) {
      auto res = visitSourceType(Ext, Conf);
      if (!res) continue;

      auto protoTy = res.get();
      if (!protoTy->isRecordType()) {
         diagnose(Conf.getTypeExpr(),
                  err_conforming_to_non_protocol,
                  protoTy.toString());

         continue;
      }

      auto Proto = protoTy->getRecord();
      if (Proto->isTemplate())
         break;

      if (!isa<ProtocolDecl>(Proto)) {
         diagnose(Conf.getTypeExpr(),
                  err_conforming_to_non_protocol, Proto->getName());

         continue;
      }

      auto PD = cast<ProtocolDecl>(Proto);

      ensureDeclared(PD);
      checkAccessibility(PD, Ext);

      auto &Other = ConformanceDependency.getOrAddVertex(PD);
      Other.addOutgoing(SelfVertex);
   }
}

bool SemaPass::visitDelayedDeclsAfterParsing()
{
   while (!TopLevelMacros.empty()) {
      auto *Next = TopLevelMacros.front();
      TopLevelMacros.pop();

      (void) declareScoped(Next);
   }

   if (encounteredError())
      return true;

   llvm::SmallVector<Decl*, 8> OtherDecls;
   unsigned i = 0;

   while (i != DelayedDecls.size()) {
      auto *decl = DelayedDecls[i++];
      if (auto Op = dyn_cast<OperatorDecl>(decl)) {
         declareScoped(Op);
      }
      else if (auto Imp = dyn_cast<ImportDecl>(decl)) {
         declareScoped(Imp);
      }
      else if (auto Use = dyn_cast<UsingDecl>(decl)) {
         declareScoped(Use);
      }
      else if (auto Ext = dyn_cast<ExtensionDecl>(decl)) {
         OtherDecls.push_back(Ext);
      }
      else if (auto Ctx = dyn_cast<DeclContext>(decl)) {
         DeclScopeRAII declContextRAII(*this, Ctx);
         auto *ND = cast<NamedDecl>(decl);

         for (auto &P : ND->getTemplateParams())
            (void)declareStmt(ND, P);
      }
      else {
         OtherDecls.push_back(decl);
      }
   }

   DelayedDecls.clear();

   for (auto *D : OtherDecls) {
      if (auto Ext = dyn_cast<ExtensionDecl>(D)) {
         HandleExtension(*this, Ext);
      }
      else {
         (void) declareScoped(D);
      }
   }

   if (!DelayedDecls.empty())
      return visitDelayedDeclsAfterParsing();

   return EncounteredError;
}

NamedDecl* SemaPass::getCurrentDecl() const
{
   for (auto ctx = DeclCtx; ctx; ctx = ctx->getParentCtx())
      if (auto ND = dyn_cast<NamedDecl>(ctx))
         return ND;

   return nullptr;
}

NamedDecl* SemaPass::getCurrentInstantiationScope()
{
   auto *ND = getCurrentDecl();

   auto *Ctx = ND->getDeclContext();
   while (Ctx) {
      if (auto CtxDecl = dyn_cast<NamedDecl>(Ctx)) {
         if (CtxDecl->isInstantiation())
            return getInstantiationScope(CtxDecl);
      }

      Ctx = Ctx->getParentCtx();
   }

   return ND;
}

RecordDecl* SemaPass::getCurrentRecordCtx()
{
   for (auto ctx = DeclCtx; ctx; ctx = ctx->getParentCtx())
      if (auto R = dyn_cast<RecordDecl>(ctx->lookThroughExtension()))
         return R;

   return nullptr;
}

void SemaPass::noteInstantiationContext()
{
   if (stage == Stage::Parsing)
      return;

   auto D = getCurrentDecl();
   while (D) {
      if (!D->isInstantiation()) {
         D = dyn_cast_or_null<NamedDecl>(D->getDeclContext());
         continue;
      }

      // instantiations of method bodies might not be actual templates, but
      // come from a templated record
      if (D->getSpecializedTemplate()->isTemplate()) {
         diagnose(note_instantiation_of, D->getInstantiatedFrom(),
                  D->getSpecializedTemplate()->getSpecifierForDiagnostic(),
                  D->getSpecializedTemplate()->getDeclName(), true,
                  D->getTemplateArgs().toString('\0', '\0', true));
      }
      else {
         diagnose(note_instantiation_of, D->getInstantiatedFrom(),
                  D->getSpecializedTemplate()->getSpecifierForDiagnostic(),
                  D->getSpecializedTemplate()->getDeclName(), false);
      }

      if (auto M = dyn_cast<MethodDecl>(D)) {
         // only emit this note if this would otherwise be the last one
         if (D->getRecord()->isInstantiation()
             && !getInstantiationScope(D)->isInstantiation()) {
            auto Record = M->getRecord();
            diagnose(note_instantiation_of, Record->getInstantiatedFrom(),
                     Record->getSpecifierForDiagnostic(),
                     Record->getSpecializedTemplate()->getDeclName(), true,
                     Record->getTemplateArgs().toString('\0', '\0', true));
         }
      }

      D = getInstantiationScope(D);
   }
}

bool SemaPass::inGlobalDeclContext() const
{
   return getDeclContext().isGlobalDeclContext();
}

const SemaPass::CoroutineInfo &SemaPass::getCoroutineInfo(CallableDecl *C)
{
   assert(C->isAsync() && "not a coroutine!");
   return getCoroutineInfo(C->getReturnType());
}

static void ActOnCallableDecl(SemaPass &SP, CallableDecl *CD)
{
   for (auto *Arg : CD->getArgs()) {
      if (Arg->getLexicalContext()) {
         SP.makeDeclAvailable(*CD, Arg);
      }
      else {
         SP.addDeclToContext(*CD, Arg);
      }
   }

   SP.checkDefaultAccessibility(CD);
}

void SemaPass::ActOnDecl(DeclContext *Ctx, Decl *D, bool Global)
{
   assert(!D->isImportedFromModule() && "don't call this on imported decls!");

   D->setLexicalContext(Ctx);
   if (auto *DeclCtx = dyn_cast<DeclContext>(D))
      DeclCtx->setParentCtx(Ctx);

   DeclScopeRAII DSR(*this, Ctx);
   switch (D->getKind()) {
   case Decl::StructDeclID:
   case Decl::ClassDeclID:
   case Decl::EnumDeclID:
   case Decl::ProtocolDeclID:
   case Decl::UnionDeclID:
      ActOnRecordDecl(cast<RecordDecl>(D));
      break;
   case Decl::ExtensionDeclID:
      ActOnExtensionDecl(cast<ExtensionDecl>(D));
      break;
   case Decl::OperatorDeclID:
      ActOnOperatorDecl(cast<OperatorDecl>(D));
      break;
   case Decl::PrecedenceGroupDeclID:
      ActOnPrecedenceGroupDecl(cast<PrecedenceGroupDecl>(D));
      break;
   case Decl::FunctionDeclID:
      ActOnFunctionDecl(cast<FunctionDecl>(D));
      break;
   case Decl::TypedefDeclID:
      ActOnTypedefDecl(cast<TypedefDecl>(D));
      break;
   case Decl::AliasDeclID:
      ActOnAliasDecl(cast<AliasDecl>(D));
      break;
   case Decl::FieldDeclID:
      ActOnFieldDecl(cast<FieldDecl>(D));
      break;
   case Decl::MethodDeclID:
      ActOnMethodDecl(cast<MethodDecl>(D));
      break;
   case Decl::InitDeclID:
      ActOnInitDecl(cast<InitDecl>(D));
      break;
   case Decl::DeinitDeclID:
      ActOnDeinitDecl(cast<DeinitDecl>(D));
      break;
   case Decl::TemplateParamDeclID:
      ActOnTemplateParamDecl(*Ctx, cast<TemplateParamDecl>(D));
      break;
   case Decl::MacroExpansionDeclID:
      ActOnMacroExpansionDecl(cast<MacroExpansionDecl>(D), Global);
      break;
   case Decl::StaticIfDeclID:
      ActOnStaticIfDecl(cast<StaticIfDecl>(D), Global);
      break;
   case Decl::StaticForDeclID:
      ActOnStaticForDecl(cast<StaticForDecl>(D), Global);
      break;
   case Decl::MixinDeclID:
      ActOnMixinDecl(cast<MixinDecl>(D), Global);
      break;
   case Decl::ImportDeclID:
      ActOnImportDecl(cast<ImportDecl>(D), Global);
      break;
   case Decl::UsingDeclID:
      ActOnUsingDecl(cast<UsingDecl>(D), Global);
      break;
   case Decl::EnumCaseDeclID:
      ActOnCallableDecl(*this, cast<EnumCaseDecl>(D));
      LLVM_FALLTHROUGH;
   case Decl::GlobalVarDeclID:
   case Decl::AssociatedTypeDeclID:
   case Decl::SubscriptDeclID:
   case Decl::PropDeclID:
   case Decl::NamespaceDeclID:
   case Decl::ModuleDeclID:
   case Decl::MacroDeclID:
      addDeclToContext(*Ctx, cast<NamedDecl>(D));
      checkDefaultAccessibility(cast<NamedDecl>(D));
      break;
   default:
      addDeclToContext(*Ctx, D);
      break;
   }
}

void SemaPass::ActOnRecordDecl(RecordDecl *R)
{
   switch (R->getKind()) {
   case Decl::StructDeclID:
   case Decl::ClassDeclID:
      ActOnStructDecl(cast<StructDecl>(R));
      break;
   case Decl::EnumDeclID:
      ActOnEnumDecl(cast<EnumDecl>(R));
      break;
   case Decl::UnionDeclID:
      ActOnUnionDecl(cast<UnionDecl>(R));
      break;
   case Decl::ProtocolDeclID:
      ActOnProtoDecl(cast<ProtocolDecl>(R));
      break;
   default:
      llvm_unreachable("bad record decl kind");
   }

   addDeclToContext(getDeclContext(), R);

   // if this record is imported, no further work is required
   if (R->isExternal())
      return;

   checkDefaultAccessibility(R);

   for (auto &P : R->getTemplateParams())
      ActOnTemplateParamDecl(*R, P);

   if (R->isTemplate())
      DelayedDecls.push_back(R);

   // register this record in the dependency graph
   (void)LayoutDependency.getOrAddVertex(R);
}

void SemaPass::ActOnStructDecl(StructDecl *S)
{

}

void SemaPass::ActOnProtoDecl(ProtocolDecl *P)
{
//   if (!P->getTemplateParams().empty()) {
//      diagnose(P, err_protocol_template_params,
//               P->getTemplateParams().front()->getSourceLoc());
//
//      P->clearTemplateParams();
//   }
}

void SemaPass::ActOnEnumDecl(EnumDecl *E)
{

}

void SemaPass::ActOnUnionDecl(UnionDecl *U)
{

}

void SemaPass::ActOnExtensionDecl(ExtensionDecl *E)
{
   addDeclToContext(getDeclContext(), (Decl*)E);
   checkDefaultAccessibility(E);

   if (stage <= Stage::Parsing) {
      DelayedDecls.push_back(E);
   }
   else {
      HandleExtension(*this, E);
   }
}

void SemaPass::ActOnOperatorDecl(OperatorDecl *Op)
{
   if (Op->isExternal()) {
      auto OpName = Op->getDeclName().getDeclaredOperatorName();
      switch (OpName.getKind()) {
      case DeclarationName::InfixOperatorName:
         Context.registerInfixOperator(OpName.getIdentifierInfo());
         break;
      case DeclarationName::PrefixOperatorName:
         Context.registerPrefixOperator(OpName.getIdentifierInfo());
         break;
      case DeclarationName::PostfixOperatorName:
         Context.registerPostfixOperator(OpName.getIdentifierInfo());
         break;
      default:
         llvm_unreachable("not an operator name!");
      }
   }
   else {
      addDeclToContext(getDeclContext(), Op);
   }

   if (!isa<ModuleDecl>(Op->getNonTransparentDeclContext())) {
      diagnose(Op, err_must_be_top_level, 1, Op->getSourceRange());
   }

   if (stage == Stage::Parsing) {
      DelayedDecls.push_back(Op);
   }
   else {
      declareScoped(Op);
   }
}

void SemaPass::ActOnPrecedenceGroupDecl(PrecedenceGroupDecl* PG)
{
   if (!PG->isExternal())
      addDeclToContext(getDeclContext(), PG);

   if (!isa<ModuleDecl>(PG->getNonTransparentDeclContext())) {
      diagnose(PG, err_must_be_top_level, 0, PG->getSourceLoc());
   }
   else {
      auto *Mod = PG->getModule();
      auto &Vert = PrecedenceDependency.getOrAddVertex(
         { PG->getIdentifierInfo(), Mod });

      if (auto HigherThan = PG->getHigherThanIdent()) {
         auto &Dep = PrecedenceDependency.getOrAddVertex({HigherThan, Mod});
         Dep.addOutgoing(&Vert);
      }
   }
}

void SemaPass::ActOnTypedefDecl(TypedefDecl *TD)
{
   if (TD->isTemplate())
      DelayedDecls.push_back(TD);

   addDeclToContext(getDeclContext(), TD);
   checkDefaultAccessibility(TD);
}

void SemaPass::ActOnAliasDecl(AliasDecl *alias)
{
   for (auto &P : alias->getTemplateParams())
      ActOnTemplateParamDecl(*alias, P);

   if (alias->isTemplate())
      DelayedDecls.push_back(alias);

   addDeclToContext(getDeclContext(), alias);
   checkDefaultAccessibility(alias);
}

void SemaPass::ActOnFieldDecl(FieldDecl *F)
{
   addDeclToContext(getDeclContext(), F);
   checkDefaultAccessibility(F);

   // field accessor is not visible via normal lookup
   if (auto Acc = F->getAccessor()) {
      Acc->setLexicalContext(&getDeclContext());
   }
}

void SemaPass::ActOnFunctionDecl(FunctionDecl *F)
{
   ActOnCallableDecl(*this, F);

   bool IsMain = F->getDeclName().isStr("main") && F->isExternC();
   if (IsMain) {
      diagnose(F, err_generic_error,
               "'extern C' function may not be called 'main'");
   }

   if (F->getDeclName().isSimpleIdentifier()
      && F->getDeclName().getIdentifierInfo()->getIdentifier()
          .startswith("llvm.")) {
      diagnose(F, err_llvm_function_name_reserved, F->getSourceLoc());
   }

   if (F->isLambda()) {
      addDeclToContext(getDeclContext(), (Decl*)F);
   }
   else {
      addDeclToContext(getDeclContext(), F);
   }

   for (auto &P : F->getTemplateParams()) {
      ActOnTemplateParamDecl(*F, P);
   }
}

void SemaPass::ActOnMethodDecl(MethodDecl *M)
{
   ActOnCallableDecl(*this, M);

   for (auto &P : M->getTemplateParams())
      ActOnTemplateParamDecl(*M, P);

   addDeclToContext(getDeclContext(), M);
}

void SemaPass::ActOnInitDecl(InitDecl *I)
{
   ActOnMethodDecl(I);
}

void SemaPass::ActOnDeinitDecl(DeinitDecl *D)
{
   ActOnMethodDecl(D);
}

void SemaPass::ActOnTemplateParamDecl(DeclContext &Ctx, TemplateParamDecl *P)
{
   if (P->getLexicalContext()) {
      makeDeclAvailable(Ctx, P);
   }
   else {
      addDeclToContext(Ctx, P);
   }
}

void SemaPass::ActOnMacroExpansionDecl(MacroExpansionDecl *Decl, bool Global)
{
   addDeclToContext(getDeclContext(), Decl);

   if (stage == Stage::Parsing)
      TopLevelMacros.push(Decl);
   else
      (void) declareStmt(Decl);
}

void SemaPass::ActOnStaticIfDecl(StaticIfDecl *D, bool Global)
{
   if (Global) {
      DelayedDecls.push_back(D);
   }

   addDeclToContext(getDeclContext(), D);
}

void SemaPass::ActOnStaticForDecl(StaticForDecl *D, bool Global)
{
   if (Global) {
      DelayedDecls.push_back(D);
   }

   addDeclToContext(getDeclContext(), D);
}

void SemaPass::ActOnMixinDecl(MixinDecl *D, bool Global)
{
   if (Global) {
      DelayedDecls.push_back(D);
   }

   addDeclToContext(getDeclContext(), D);
}

void SemaPass::ActOnImportDecl(ImportDecl *D, bool Global)
{
   if (Global) {
      DelayedDecls.push_back(D);
   }

   addDeclToContext(getDeclContext(), D);
   checkDefaultAccessibility(D);
}

void SemaPass::ActOnUsingDecl(UsingDecl *D, bool Global)
{
   if (Global) {
      DelayedDecls.push_back(D);
   }

   addDeclToContext(getDeclContext(), (Decl*)D);
   checkDefaultAccessibility(D);
}

void SemaPass::checkDefaultAccessibility(NamedDecl *ND)
{
   auto AS = ND->getAccess();
   if (AS == AccessSpecifier::Protected) {
      if (!isa<ClassDecl>(ND->getNonTransparentDeclContext())) {
         diagnose(ND, err_protected_outside_class, ND->getAccessRange(),
                  ND->getSourceLoc());

         AS = AccessSpecifier::Default;
      }
   }

   if (AS != AccessSpecifier::Default)
      return;

   if (isa<ExtensionDecl>(ND)) {
      ND->setAccess(AccessSpecifier::Public);
      return;
   }

   auto *Ctx = ND->getNonTransparentDeclContext();
   if (isa<RecordDecl>(Ctx) || isa<ExtensionDecl>(Ctx)) {
      if (isa<ProtocolDecl>(Ctx)) {
         ND->setAccess(AccessSpecifier::Public);
         return;
      }

      auto *CtxDecl = dyn_cast<NamedDecl>(Ctx);
      AccessSpecifier MinVisibility;
      if (CtxDecl->getAccess() != AccessSpecifier::Default) {
         MinVisibility = CtxDecl->getAccess();
      }
      else {
         MinVisibility = AccessSpecifier::Public;
      }

      switch (ND->getKind()) {
      case Decl::PropDeclID:
      case Decl::MethodDeclID:
      case Decl::InitDeclID:
      case Decl::EnumCaseDeclID:
         ND->setAccess(MinVisibility);
         break;
      case Decl::FieldDeclID:
      case Decl::StructDeclID:
      case Decl::ClassDeclID:
      case Decl::EnumDeclID:
      case Decl::UnionDeclID:
      case Decl::ProtocolDeclID:
         if (isa<StructDecl>(Ctx) && !isa<ClassDecl>(Ctx)) {
            ND->setAccess(MinVisibility);
         }
         else {
            ND->setAccess(AccessSpecifier::Internal);
         }

         break;
      default:
         ND->setAccess(AccessSpecifier::Internal);
         break;
      }
   }
   else {
      // all top level declarations default to internal
      ND->setAccess(AccessSpecifier::Internal);
   }
}

void SemaPass::pushDeclContext(DeclContext *Ctx)
{
   DeclCtx = Ctx;
}

void SemaPass::popDeclContext()
{
   DeclCtx = DeclCtx->getParentCtx();
}

bool SemaPass::declareDeclContext(DeclContext *Ctx)
{
   StmtOrDecl D = dyn_cast<Decl>(Ctx);

   // resolve static conditions line because they might contain decls that
   // are needed by others
   for (auto &decl : Ctx->getDecls()) {
      switch (decl->getKind()) {
      case Decl::StaticIfDeclID:
      case Decl::StaticForDeclID: {
         (void)declareStmt(D, decl);
         break;
      }
      case Decl::MacroExpansionDeclID:
         break;
      default:
         break;
      }
   }

   // associated types and aliases must be visited line because their types
   // might be used in fields / method signatures / etc.
   for (auto &decl : Ctx->getDecls()) {
      switch (decl->getKind()) {
      case Decl::TypedefDeclID:
      case Decl::AssociatedTypeDeclID:
      case Decl::AliasDeclID:
      case Decl::UsingDeclID:
      case Decl::ImportDeclID:
      case Decl::ModuleDeclID: {
         (void)declareStmt(D, decl);
         break;
      }
      default:
         break;
      }
   }

   // now we can safely declare the rest
   for (auto &decl : Ctx->getDecls()) {
      switch (decl->getKind()) {
      case Decl::TypedefDeclID:
      case Decl::AssociatedTypeDeclID:
      case Decl::AliasDeclID:
      case Decl::TemplateParamDeclID:
      case Decl::UsingDeclID:
      case Decl::ImportDeclID:
      case Decl::ModuleDeclID:
      case Decl::OperatorDeclID:
      case Decl::PrecedenceGroupDeclID:
      case Decl::StaticIfDeclID:
      case Decl::StaticForDeclID:
      case Decl::MacroExpansionDeclID:
         break;
      default: {
         (void)declareStmt(D, decl);
         break;
      }
      }
   }

   return true;
}

void SemaPass::transferDecls(DeclContext *From, DeclContext *To)
{
   auto Curr = *From->decl_begin();
   while (Curr) {
      auto Next = Curr->getNextDeclInContext();
      Curr->setNextDeclInContext(nullptr);

      To->addDecl(Curr);
      Curr = Next;
   }

   for (auto &DeclList : From->getAllNamedDecls()) {
      for (auto *ND : DeclList.getSecond().getAsLookupResult()) {
         makeDeclAvailable(*To, ND);
      }
   }

   From->clear();
}

DeclResult SemaPass::declareDebugDecl(DebugDecl *D)
{
   return D;
}

DeclResult SemaPass::declareImportDecl(ImportDecl *I)
{
   if (I->getImportedModule())
      return I;

   compilationUnit->getModuleMgr().ImportModule(I);

   auto *Mod = I->getImportedModule();
   if (!Mod)
      return DeclError();

   if (I->isWildcardImport()) {
      getDeclContext().addImportedModule(Mod);
   }
   else if (I->getNamedImports().empty()) {
      I->addImportedModule(Mod);
   }
   else for (auto *Name : I->getNamedImports()) {
      auto Result = LookupOwn(*Mod->getDecl(), Name);
      if (Result.empty()) {
         diagnose(I, err_member_not_found, /*module*/ 9, Mod->getName(),
                  Name->getIdentifier(), I->getSourceRange());

         continue;
      }

      for (NamedDecl *ND : Result) {
         makeDeclAvailable(getDeclContext(), ND);
      }
   }

   return I;
}

DeclResult SemaPass::declareModuleDecl(ModuleDecl *Decl)
{
   DeclContextRAII DC(*this, Decl);

   if (!declareDeclContext(Decl))
      return DeclError();

   return Decl;
}

DeclResult SemaPass::declareCompoundDecl(CompoundDecl *D)
{
   DeclContextRAII declContextRAII(*this, D);
   for (auto &decl : D->getDecls())
      declareStmt(D, decl);

   return D;
}

DeclResult SemaPass::declareNamespaceDecl(NamespaceDecl *NS)
{
   DeclContextRAII declContextRAII(*this, NS);
   for (auto &D : NS->getDecls())
      declareStmt(NS, D);

   return NS;
}

DeclResult SemaPass::declareUnittestDecl(UnittestDecl *D)
{
   return D;
}

DeclResult SemaPass::declareUsingDecl(UsingDecl *UD)
{
   NamedDecl *UsingTarget = nullptr;
   auto *declContext = &getDeclContext();

   size_t i = 0;
   size_t nameDepth = UD->getNestedImportName().size();

   for (auto *Name : UD->getNestedImportName()) {
      auto subDecl = MultiLevelLookup(*declContext, Name);
      if (!subDecl) {
         diagnoseMemberNotFound(declContext, UD, Name);
         return UD;
      }

      if (subDecl.front().size() != 1) {
         diagnose(UD, err_using_target_ambiguous, UD->getSourceRange());

         auto it = subDecl.front().begin();
         diagnose(note_candidate_here, (*it++)->getSourceLoc());
         diagnose(note_candidate_here, (*it++)->getSourceLoc());

         return UD;
      }

      NamedDecl *ND = subDecl.front().front();
      declContext = dyn_cast<DeclContext>(ND);

      if (!declContext && i != nameDepth - 1) {
         diagnose(UD, err_cannot_lookup_member_in,
                  ND->getSpecifierForDiagnostic(), ND->getDeclName());

         return UD;
      }

      UsingTarget = ND;
      ++i;
   }

   if (UD->isWildcardImport()) {
      makeDeclsAvailableIn(getDeclContext(), *declContext);
      return UD;
   }

   assert(UsingTarget && "should have been diagnosed!");
   makeDeclAvailable(getDeclContext(), UD->getDeclName(), UsingTarget);

   return UD;
}

DeclResult SemaPass::declareOperatorDecl(OperatorDecl *Decl)
{
   if (auto II = Decl->getPrecedenceGroupIdent()) {
      auto lookup = LookupSingle<PrecedenceGroupDecl>(getDeclContext(), II);
      if (!lookup) {
         diagnose(Decl, err_precedence_group_does_not_exist,
                  Decl->getSourceLoc(),
                  II->getIdentifier());
      }
      else {
         Decl->setPrecedenceGroup(lookup);
      }

      if (Decl->getDeclName().getDeclaredOperatorName()
              .getKind() == DeclarationName::InfixOperatorName) {
         Context.setInfixOperatorPrecedence(Decl->getDeclName()
                                                .getDeclaredOperatorName()
                                                .getInfixOperatorName(),
                                            lookup);
      }
   }

   if (!Decl->getPrecedenceGroup()) {
      Decl->setPrecedenceGroup(getDefaultPrecedenceGroup());
   }

   return Decl;
}

DeclResult SemaPass::declarePrecedenceGroupDecl(PrecedenceGroupDecl *Decl)
{
   if (auto HigherThan = Decl->getHigherThanIdent()) {
      auto lookup = LookupSingle<PrecedenceGroupDecl>(getDeclContext(),
                                                      HigherThan);

      if (!lookup) {
         diagnose(Decl, err_precedence_group_does_not_exist,
                  Decl->getSourceLoc(), HigherThan->getIdentifier());

         return DeclError();
      }

      Decl->setHigherThan(lookup);
   }
   if (auto LowerThan = Decl->getLowerThanIdent()) {
      auto lookup = LookupSingle<PrecedenceGroupDecl>(getDeclContext(),
                                                      LowerThan);

      if (!lookup) {
         diagnose(Decl, err_precedence_group_does_not_exist,
                  Decl->getSourceLoc(), LowerThan->getIdentifier());

         return DeclError();
      }

      Decl->setLowerThan(lookup);
   }

   return Decl;
}

LLVM_ATTRIBUTE_UNUSED
static void checkMainSignature(SemaPass &SP, CallableDecl *F)
{
   QualType RetTy = F->getReturnType();
   if (!RetTy->isVoidType() && RetTy != SP.getContext().getIntTy()) {
      SP.diagnose(F, err_bad_main_def, 3, F->getSourceLoc());
   }

   F->setReturnType(SourceType(SP.getContext().getIntTy()));

   auto Args = F->getArgs();
   if (Args.empty())
      return;

   bool Valid = Args.size() == 2 || Args.size() == 3;
   if (Valid) {
      unsigned i = 0;
      for (auto Arg : Args) {
         switch (i) {
         case 0:
            Valid &= Arg->getType() == SP.getContext().getIntTy();
            break;
         case 1:
            Valid &= Arg->getType() == SP.getContext().getUInt8PtrTy()
                                         ->getPointerTo(SP.getContext());
            break;
         case 2:
            Valid &= Arg->getType() == SP.getContext().getUInt8PtrTy()
                                         ->getPointerTo(SP.getContext());
            break;
         default:
            llvm_unreachable("bad argument #");
         }

         if (!Valid)
            break;

         ++i;
      }
   }

   if (!Valid)
      SP.diagnose(F, err_bad_main_sig, F->getSourceLoc());
}

static bool checkCompileTimeEvaluable(DeclContext *Ctx)
{
   while (Ctx) {
      if (auto *D = dyn_cast<Decl>(Ctx)) {
         if (D->hasAttribute<CompileTimeAttr>()) {
            return true;
         }
         if (auto *Ext = dyn_cast<ExtensionDecl>(Ctx)) {
            if (checkCompileTimeEvaluable(Ext->getExtendedRecord())) {
               return true;
            }
         }
      }

      Ctx = Ctx->getParentCtx();
   }

   return false;
}

DeclResult SemaPass::declareCallableDecl(CallableDecl *F)
{
   // an error occured while parsing this function
   if (F->isInvalid())
      return DeclError();

   // Lookup program argument declarations.
   if (F->isMain()) {
      auto *SysMod = getSysModule();
      if (SysMod) {
         (void) Lookup(*SysMod->getDecl(), getIdentifier("argc"));
         (void) Lookup(*SysMod->getDecl(), getIdentifier("argv"));
      }
   }

   // Check duplicate extern C functions.
   if (F->isExternC()) {
      if (!isa<FunctionDecl>(F)) {
         diagnose(F, err_generic_error, "methods cannot be @extern(C)",
                  F->getSourceLoc());
      }

      if (!F->getDeclName().isSimpleIdentifier()) {
         diagnose(F, err_cannot_be_extern_C, F->getSourceLoc());
      }
      else if (F->getBody()) {
         auto Ret = ExternCFuncs.try_emplace(
            F->getDeclName().getIdentifierInfo());

         if (!Ret.second) {
            diagnose(F, err_redeclared_extern_c, F->getSourceLoc(),
                     F->getDeclName());

            auto *OtherFn = Ret.first->getSecond();
            diagnose(note_previous_decl, OtherFn->getSourceLoc());
         }
      }
   }

   // Check if this function is callable at compile time.
   if (!F->isCompileTimeEvaluable() && checkCompileTimeEvaluable(F)) {
      F->setCompileTimeEvaluable(true);
   }

   DeclContextRAII declContextRAII(*this, F);
   visitConstraints(F);

   if (F->isOperator()) {
      auto OpName = F->getDeclName();
      if (OpName.getKind() == DeclarationName::InstantiationName)
         OpName = OpName.getInstantiationName();

      auto OpDeclName = Context.getDeclNameTable().getOperatorDeclName(OpName);
      auto Result = LookupSingle<OperatorDecl>(getDeclContext(),
                                               OpDeclName);

      if (!Result) {
         diagnose(F, err_undeclared_operator, F->getSourceLoc(), OpName);
      }
      else {
         ensureDeclared(Result);
         F->setPrecedenceGroup(Result->getPrecedenceGroup());
      }
   }

   for (auto &TP : F->getTemplateParams())
      (void) declareStmt(F, TP);

   for (const auto &arg : F->getArgs()) {
      if (!declareStmt(F, arg))
         return DeclError();
   }

   if (auto &Ret = F->getReturnType()) {
      auto Result = visitSourceType(F, Ret);
      if (!Result)
         return DeclError();

      auto retTy = Ret.getResolvedType();
      if (retTy->isAutoType()) {
         Ret.setResolvedType(Context.getEmptyTupleType());
      }

      F->setIsNoReturn(retTy->isUnpopulatedType());
   }

   // Transform the return type into a Future<T>.
   while (F->isAsync()) {
      auto *AwaitableDecl = getAwaitableDecl();
      if (!AwaitableDecl) {
         diagnose(F, err_no_builtin_decl, 12, F->getSourceLoc());
         break;
      }

      QualType RetTy = F->getReturnType();
      auto &Conformances = Context.getConformanceTable();

      if (!RetTy->isRecordType()
            || !Conformances.conformsTo(RetTy->getRecord(), AwaitableDecl)) {
         auto *FutureDecl = getFutureDecl();
         if (!FutureDecl) {
            diagnose(F, err_no_builtin_decl, 12, F->getSourceLoc());
            break;
         }

         ensureDeclared(FutureDecl);

         ResolvedTemplateArg Arg(FutureDecl->getTemplateParams().front(),
                                 F->getReturnType(), F->getSourceLoc());

         bool isNew = false;
         auto TemplateArgs = FinalTemplateArgumentList::Create(Context,{ Arg });
         auto Inst = Instantiator.InstantiateRecord(F, FutureDecl,
                                                    TemplateArgs, &isNew);

         if (!Inst)
            break;

         auto *Fut = Inst.get();
         RetTy = Context.getRecordType(Fut);

         F->getReturnType().setResolvedType(RetTy);
         F->setImplicitFutureReturn(true);
      }

      collectCoroutineInfo(RetTy, F);
      break;
   }

   if (F->isConversionOp()) {
      auto Name = Context.getDeclNameTable()
                         .getConversionOperatorName(F->getReturnType());

      F->setName(Name);
      makeDeclAvailable(*F->getDeclContext(), F);
   }

   F->createFunctionType(*this);
   checkDuplicateFunctionDeclaration(F, "function");

   if (auto *M = dyn_cast<MethodDecl>(F)) {
      QualType SelfTy = Context.getRecordType(F->getRecord());
      if (F->hasMutableSelf() && !isa<InitDecl>(M) && !isa<DeinitDecl>(M)) {
         SelfTy = Context.getMutableReferenceType(SelfTy);
      }

      M->setSelfType(SelfTy);
   }

   if (!declareDeclContext(F))
      return DeclError();

   return F;
}

void SemaPass::collectCoroutineInfo(QualType Ty, StmtOrDecl D)
{
   auto It = CoroutineInfoMap.find(Ty);
   if (It != CoroutineInfoMap.end())
      return;

   CoroutineInfo &Info = CoroutineInfoMap.try_emplace(Ty)
                                         .first->getSecond();

   auto *Awaitable = Ty->getRecord();
   ensureDeclared(Awaitable);
   
   Info.AwaitableType = Ty;
   Info.AwaitedType = Awaitable->getAssociatedType(getIdentifier("AwaitedType"))
                               ->getActualType();
   Info.AwaiterType = Awaitable->getAssociatedType(getIdentifier("AwaiterType"))
                               ->getActualType();

   // Instantiate the coroutine handle type.
   {
      auto *HandleDecl = getCoroutineHandleDecl();
      if (!HandleDecl) {
         diagnose(D, err_no_builtin_decl, 12, D.getSourceLoc());
         return;
      }

      auto *InitFn = HandleDecl->getMemberwiseInitializer();
      Info.CoroHandleInit = InitFn;
      maybeInstantiateMemberFunction(InitFn, D);
   }

   // Instantiate the needed methods for the Awaitable and the Awaiter.
   auto *InitFn = cast<StructDecl>(Awaitable)->getParameterlessConstructor();
   Info.AwaitableInit = InitFn;
   maybeInstantiateMemberFunction(InitFn, D);

   Info.AwaitableGetAwaiter = InstantiateMethod(Awaitable, "getAwaiter", D);
   Info.AwaitableResolve = InstantiateMethod(Awaitable, "resolve", D);

   assert(Info.AwaiterType->isRecordType() && "non-record conforms to Awaiter");
   auto *Awaiter = Info.AwaiterType->getRecord();

   Info.AwaitSuspend = InstantiateMethod(Awaiter, "awaitSuspend", D);
   Info.AwaitResume = InstantiateMethod(Awaiter, "awaitResume", D);
   Info.AwaitReady = InstantiateProperty(Awaiter, "ready", true, D);
}

DeclResult SemaPass::declareFunctionDecl(FunctionDecl *F)
{
   if (!declareCallableDecl(F))
      return DeclError();

   if (!F->isTemplate() && !F->isInvalid())
      ILGen->DeclareFunction(F);

   return F;
}

bool SemaPass::equivalent(TemplateParamDecl *p1, TemplateParamDecl *p2)
{
   if (p1->isTypeName() != p2->isTypeName())
      return false;

   if (p1->isTypeName()) {
      if (p1->getCovariance() != p2->getCovariance())
         return false;

      if (p1->getContravariance()) {
         if (!p2->getContravariance())
            return false;

         if (p1->getContravariance() != p2->getContravariance())
            return false;
      }
   }
   else {
      if (p1->getValueType() != p2->getValueType())
         return false;
   }

   return true;
}

static
bool templateParamsEffectivelyEqual(SemaPass &SP,
                                    ArrayRef<TemplateParamDecl*> P1,
                                    ArrayRef<TemplateParamDecl*> P2) {
   if (P1.size() != P2.size())
      return false;

   size_t i = 0;
   for (auto &p1 : P1) {
      auto p2 = P2[i];
      if (!SP.equivalent(p1, p2))
         return false;

      ++i;
   }

   return true;
}

namespace {

class SignatureCheckTypeBuilder: public TypeBuilder<SignatureCheckTypeBuilder> {
   CallableDecl *CD;

public:
   SignatureCheckTypeBuilder(SemaPass &SP, CallableDecl *CD)
      : TypeBuilder(SP, CD), CD(CD)
   {

   }


   QualType visitGenericType(GenericType *T)
   {
      // Template argument types do not need to be equal, just equivalent.
      auto *Param = T->getParam();
      auto Idx = Param->getIndex();

      if (CD->getTemplateParams().size() <= Idx)
         return T;

      auto *OtherParam = CD->getTemplateParams()[Idx];
      if (SP.equivalent(Param, OtherParam))
         return SP.getContext().getTemplateArgType(OtherParam);

      return T;
   }
};

} // anonymous namespace

static bool typesCompatible(SemaPass &SP,
                            CallableDecl *CD,
                            const SourceType &LHS,
                            const SourceType &RHS) {
   // Template argument types do not need to be equal, just equivalent.
   QualType RHSType;
   if (LHS->isDependentType()) {
      SignatureCheckTypeBuilder Builder(SP, CD);
      RHSType = Builder.visit(RHS);
   }
   else {
      RHSType = RHS;
   }

   return LHS.getResolvedType().getCanonicalType()
      == RHSType.getCanonicalType();
}

int SemaPass::signaturesCompatible(CallableDecl *C1, CallableDecl *C2)
{
   auto Args1 = C1->getArgs();
   auto Args2 = C2->getArgs();

   unsigned NumArgs = (unsigned)Args1.size();
   if (Args1.size() != Args2.size())
      return 0; // incompatible signature

   if (!typesCompatible(*this, C1, C1->getReturnType(), C2->getReturnType()))
      return 1; // incompatible return type

   unsigned i = 0;
   if (isa<MethodDecl>(C1)) {
      ++i;
   }

   for (; i < NumArgs; ++i) {
      auto &Arg = Args1[i];
      auto &Other = Args2[i];
      if (!typesCompatible(*this, C1, Arg->getType(), Other->getType()))
         return 0; // incompatible signature

      if (Arg->getLabel() != Other->getLabel())
         return 5; // incompatible labels
   }

   if (C1->throws())
      if (!C2->throws())
         return 2; // incompatible 'throws'

   if (C1->isUnsafe())
      if (!C2->isUnsafe())
         return 3; // incompatible 'unsafe'

   if (C1->isAsync())
      if (!C2->isAsync())
         return 4; // incompatible 'async'

   return -1; // valid
}

void SemaPass::checkDuplicateFunctionDeclaration(CallableDecl *C,
                                                 llvm::StringRef fnKind) {
   // Protocol default implementations are allowed to override previous
   // declarations.
   if (isa<ExtensionDecl>(C->getDeclContext())
         && isa<ProtocolDecl>(C->getDeclContext()->lookThroughExtension())) {
      return;
   }

   // We don't check constraints for equality.
   if (!C->getConstraints().empty())
      return;

   for (auto &decl : Lookup(*C->getDeclContext(), C->getDeclName())) {
      auto Fn = dyn_cast<CallableDecl>(decl);
      if (Fn == C || !Fn)
         continue;

      if (!isDeclared(Fn))
         return;

      // We don't check constraints for equality.
      if (!Fn->getConstraints().empty())
         continue;

      if (!templateParamsEffectivelyEqual(*this, Fn->getTemplateParams(),
                                          C->getTemplateParams())) {
         continue;
      }

      auto FstArgs = C->getArgs();
      auto SndArgs = Fn->getArgs();

      if (FstArgs.size() != SndArgs.size())
         continue;

      size_t i = 0;
      bool duplicate = true;

      for (auto &arg : FstArgs) {
         auto &other = SndArgs[i++];
         if (arg->getType() != other->getType()) {
            duplicate = false;
            break;
         }
         if (arg->getLabel() != other->getLabel()) {
            duplicate = false;
            break;
         }
      }

      if (!duplicate)
         continue;

      diagnose(C, err_redeclared_symbol, C->getFullName(), 0,
               C->getSourceLoc());
      diagnose(Fn, note_previous_decl, Fn->getSourceLoc());

      break;
   }
}

DeclResult SemaPass::declareFuncArgDecl(FuncArgDecl *Decl)
{
   if (Decl->isSelf()) {
      auto M = cast<MethodDecl>(Decl->getDeclContext());
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

      Decl->setConvention(Conv);
      Decl->getType().setResolvedType(SelfTy);

      if (SelfTy->isDependentType())
         Decl->setIsTypeDependent(true);

      return Decl;
   }

   auto res = visitSourceType(Decl, Decl->getType());
   if (!res)
      return DeclError();

   auto &declaredType = Decl->getType();
   if (declaredType->isVoidType()) {
      SourceLocation Loc;
      if (auto *E = Decl->getType().getTypeExpr()) {
         Loc = E->getSourceLoc();
      }
      else {
         Loc = Decl->getSourceLoc();
      }

      diagnose(Decl, err_generic_error, "function arguments may not be of "
                                        "type 'void'", Loc);
   }

   if (auto defaultVal = Decl->getDefaultVal()) {
      DefaultArgumentValueRAII defaultArgumentValueRAII(*this);

      auto Result = getAsOrCast(Decl, defaultVal, declaredType);
      if (Result)
         Decl->setDefaultVal(Result.get());
   }

   if (Decl->getConvention() == ArgumentConvention::Default) {
      auto Fn = cast<CallableDecl>(Decl->getDeclContext());
      // initializer arguments are owned by default
      if (isa<InitDecl>(Fn) || isa<EnumCaseDecl>(Fn)) {
         Decl->setConvention(ArgumentConvention::Owned);
      }
      // otherwise an immutable borrow is the default
      else {
         Decl->setConvention(ArgumentConvention::Borrowed);
      }
   }

   return Decl;
}

DeclResult SemaPass::declareTemplateParamDecl(TemplateParamDecl *P)
{
   if (P->isSynthesized())
      return P;

   if (auto Other = LookupSingle<TemplateParamDecl>(*P->getDeclContext()
                                                      ->getParentCtx(),
                                                    P->getDeclName())) {
      diagnose(P, err_template_param_shadow, P->getName(), P->getSourceRange());
      diagnose(note_template_parameter_here, Other->getSourceRange());

      return P;
   }

   if (auto &cov = P->getCovariance()) {
      if (!visitSourceType(P, cov))
         return DeclError();
   }
   
   if (auto &con = P->getContravariance()) {
      if (!visitSourceType(P, con))
         return DeclError();
   }

   if (P->getCovariance()->isAutoType()) {
      if (P->isTypeName()) {
         P->getValueType().setResolvedType(UnknownAnyTy);
      }
      else {
         P->getValueType().setResolvedType(Context.getIntTy());
      }
   }

   // update the TemplateParamType associated with this declaration
   Context.getTemplateArgType(P)->setCanonicalType(P->getCovariance());

   if (auto Def = P->getDefaultValue()) {
      (void)visitExpr(P, Def);
   }

   return P;
}

DeclResult SemaPass::declareGlobalVarDecl(GlobalVarDecl *Decl)
{
   EnterGlobalVarScope GVS(*this, Decl);

   if (auto &ty = Decl->getType()) {
      auto res = visitSourceType(Decl, ty);
      if (!res)
         return Decl;
   }
   else {
      DelayedDecls.push_back(Decl);
   }

   return Decl;
}

void SemaPass::registerExplicitConformances(RecordDecl *Rec,
                                            llvm::ArrayRef<SourceType>
                                                               ConfTypes) {
   auto &ConfTable = Context.getConformanceTable();

   DependencyGraph<ProtocolDecl*>::Vertex *SelfVertex = nullptr;
   if (auto SelfProto = dyn_cast<ProtocolDecl>(Rec))
      SelfVertex = &ConformanceDependency.getOrAddVertex(SelfProto);

   for (const auto &prot : ConfTypes) {
      auto res = visitSourceType(Rec, prot);
      if (!res) continue;

      auto protoTy = res.get();
      if (!protoTy->isRecordType()) {
         diagnose(prot.getTypeExpr(),
                  err_conforming_to_non_protocol,
                  protoTy.toString());

         continue;
      }

      auto Proto = protoTy->getRecord();
      if (Proto->isTemplate())
         break;

      if (!isa<ProtocolDecl>(Proto)) {
         diagnose(prot.getTypeExpr(),
                  err_conforming_to_non_protocol, Proto->getName());

         continue;
      }

      auto PD = cast<ProtocolDecl>(Proto);

      ensureDeclared(PD);
      checkAccessibility(PD, Rec);

      ConfTable.addExplicitConformance(Context, Rec, PD);

      if (SelfVertex) {
         auto &Other = ConformanceDependency.getOrAddVertex(PD);
         Other.addOutgoing(SelfVertex);
      }
   }
}

void SemaPass::registerExplicitConformances(RecordDecl *Rec)
{
   return registerExplicitConformances(Rec, Rec->getConformanceTypes());
}

static void addSingleConformance(RecordDecl *Rec,
                                 ProtocolDecl *Conf,
                                 ASTContext &Context,
                                 ConformanceTable &ConfTable) {
   ConfTable.addImplicitConformance(Context, Rec, Conf);

   for (auto PConf : ConfTable.getAllConformances(Conf))
      addSingleConformance(Rec, PConf->getProto(), Context, ConfTable);
}

void SemaPass::registerImplicitAndInheritedConformances(RecordDecl *Rec,
                                               ArrayRef<SourceType> ConfTypes) {
   auto &ConfTable = Context.getConformanceTable();

   for (auto &Conf : ConfTypes) {
      if (!Conf->isRecordType())
         continue;

      auto Proto = Conf->getRecord();
      if (Proto->isTemplate())
         break;

      if (!isa<ProtocolDecl>(Proto)) {
         continue;
      }

      auto PD = cast<ProtocolDecl>(Proto);
      for (auto PConf : ConfTable.getAllConformances(PD)) {
         addSingleConformance(Rec, PConf->getProto(), Context, ConfTable);
      }
   }
}

void SemaPass::registerImplicitAndInheritedConformances(RecordDecl *Rec)
{
   auto &ConfTable = Context.getConformanceTable();
   auto ExplicitConformances = ConfTable.getExplicitConformances(Rec);

   for (auto Conf : ExplicitConformances) {
      for (auto PConf : ConfTable.getAllConformances(Conf)) {
         addSingleConformance(Rec, PConf->getProto(), Context, ConfTable);
      }
   }

   if (auto C = dyn_cast<ClassDecl>(Rec)) {
      auto Base = C->getParentClass();
      while (Base) {
         auto BaseConf = ConfTable.getAllConformances(Base);
         for (auto &Conf : BaseConf) {
            ConfTable.addInheritedConformance(Context, Rec, Conf->getProto());
         }

         Base = Base->getParentClass();
      }
   }
}

DeclResult SemaPass::declareRecordDecl(RecordDecl *Rec)
{
   DeclPrettyStackTraceEntry STE(Rec);

   Rec->setType(Context.getRecordType(Rec));
   Rec->setOpaque(Rec->hasAttribute<OpaqueAttr>());

   if (auto *Any = getAnyDecl()) {
      addDependency(Rec, Any);
   }

   registerExplicitConformances(Rec);
   registerImplicitAndInheritedConformances(Rec);

   for (auto &C : Rec->getConstraints())
      visitStaticExpr(C);

   if (!declareDeclContext(Rec))
      return DeclError();

   for (auto *Ext : Rec->getExtensions()) {
      ensureDeclared(Ext);
   }

   auto *Ty = Context.getRecordType(Rec);
   Ty->setDependent(Ty->isDependentType() || Rec->isTemplateOrInTemplate());

   if (!Rec->isTemplateOrInTemplate()) {
      checkProtocolConformance(Rec);
   }

   return Rec;
}

DeclResult SemaPass::declareStructDecl(StructDecl *S)
{
   DeclContextRAII declContextRAII(*this, S);

   if (!declareRecordDecl(S))
      return DeclError();

   declareDefaultInitializer(S);

   if (!S->isClass() && S->decl_begin<InitDecl>() == S->decl_end<InitDecl>())
      declareMemberwiseInitializer(S);

   if (!S->getDeinitializer())
      declareDefaultDeinitializer(S);

   return S;
}

DeclResult SemaPass::declareClassDecl(ClassDecl *C)
{
   auto parent = C->getParentType();
   if (parent) {
      auto res = visitSourceType(C, parent);
      if (!res)
         return DeclError();

      auto parentTy = parent.getResolvedType();
      auto ParentClass = parentTy->getRecord();
      if (!ParentClass->isTemplate()) {
         if (!isa<ClassDecl>(ParentClass)) {
            diagnose(parent.getTypeExpr(), err_generic_error,
                     "cannot extend non-class " + ParentClass->getName());
         }
         else {
            auto *Base = cast<ClassDecl>(ParentClass);
            ensureDeclared(Base);

            C->inherit(Base);
            C->getStoredFields().insert(C->getStoredFields().begin(),
                                        Base->getStoredFields().begin(),
                                        Base->getStoredFields().end());

            // base class size needs to be calculated before sub classes
            auto &Vert = LayoutDependency.getOrAddVertex(C);
            auto &BaseVert = LayoutDependency.getOrAddVertex(ParentClass);

            BaseVert.addOutgoing(&Vert);
         }
      }
   }

   return declareStructDecl(C);
}

void SemaPass::declareMemberwiseInitializer(StructDecl *S,
                                            InitDecl *ExplicitDecl) {
   std::vector<FuncArgDecl*> args;

   for (auto F : S->getFields()) {
      if (!F->isStatic() && !F->getDefaultVal()) {
         auto res = visitSourceType(S, F->getType());
         if (!res) return;

         auto *Lbl = F->getDeclName().getIdentifierInfo();
         auto arg = FuncArgDecl::Create(Context, S->getSourceLoc(),
                                        S->getSourceLoc(),
                                        Lbl, Lbl, ArgumentConvention::Owned,
                                        F->getType(), nullptr, F->isVariadic());

         args.push_back(arg);
      }
   }

   auto MDecl = InitDecl::Create(Context, AccessSpecifier::Public,
                                 S->getSourceLoc(), args, {}, nullptr);

   if (ExplicitDecl) {
      S->replaceDecl(ExplicitDecl, MDecl);
      ActOnCallableDecl(*this, MDecl);

      MDecl->setParentCtx(ExplicitDecl->getParentCtx());
      MDecl->setLexicalContext(ExplicitDecl->getLexicalContext());
      MDecl->setLogicalContext(ExplicitDecl->getDeclContext());
   }
   else {
      ActOnDecl(S, MDecl, false);
   }

   S->setMemberwiseInitializer(MDecl);

   MDecl->setSynthesized(true);
   MDecl->setMemberwiseInitializer(true);

   if (S->isInstantiation()) {
      MDecl->setMethodID(cast<StructDecl>(S->getSpecializedTemplate())
         ->getMemberwiseInitializer()->getMethodID());
   }

   (void)declareStmt(MDecl);
}

void SemaPass::declareDefaultInitializer(StructDecl *S)
{
   auto Name = &Context.getIdentifiers().get("__default_init");
   if (auto *MF = S->getModFile()) {
      MF->PerformExternalLookup(*S, Name);

      auto Result = S->lookup(Name);
      if (!Result.empty()) {
         S->setDefaultInitializer(cast<MethodDecl>(Result.front()));
         return;
      }
   }

   SourceType RetTy(Context.getVoidType());
   auto Decl = MethodDecl::Create(Context, AccessSpecifier::Private,
                                  S->getSourceLoc(), Name, RetTy,
                                  { MakeSelfArg(S->getSourceLoc()) },
                                  {}, nullptr, false);

   ActOnDecl(S, Decl, false);
   S->setDefaultInitializer(Decl);

   Decl->setSynthesized(true);
   Decl->setDefaultInitializer(true);

   if (S->isInstantiation()) {
      Decl->setMethodID(cast<StructDecl>(S->getSpecializedTemplate())
                           ->getDefaultInitializer()->getMethodID());
   }

   (void)declareStmt(Decl);
}

FuncArgDecl* SemaPass::MakeSelfArg(SourceLocation Loc)
{
   auto *SelfII = &Context.getIdentifiers().get("self");
   return FuncArgDecl::Create(Context, Loc, Loc, DeclarationName(SelfII),
                              nullptr,
                              ArgumentConvention::Default, SourceType(),
                              nullptr, false, false, /*isSelf=*/true);
}

void SemaPass::declareDefaultDeinitializer(RecordDecl *R)
{
   auto DDecl = DeinitDecl::Create(Context, R->getSourceLoc(), nullptr, {});
   ActOnDecl(R, DDecl, 0);

   DDecl->setSynthesized(true);
   DDecl->setReturnType(SourceType(Context.getVoidType()));

   if (R->isInstantiation()) {
      DDecl->setMethodID(R->getSpecializedTemplate()
                          ->getDeinitializer()->getMethodID());
   }

   (void)declareStmt(DDecl);
}

DeclResult SemaPass::declareProtocolDecl(ProtocolDecl *P)
{
   DeclContextRAII declContextRAII(*this, P);
   if (!declareRecordDecl(P))
      return DeclError();

   if (P == getAnyDecl())
      P->setIsAny(true);

   return P;
}

DeclResult SemaPass::declareExtensionDecl(ExtensionDecl *Ext)
{
   DeclContextRAII declContextRAII(*this, Ext);
   if (!declareDeclContext(Ext))
      return DeclError();

   auto *ExtendedRec = Ext->getExtendedRecord();
   if (auto *P = dyn_cast<ProtocolDecl>(ExtendedRec)) {
      checkProtocolExtension(Ext, P);
   }

   return Ext;
}

static void addDependencies(QualType FieldType,
                            DependencyGraph<NamedDecl*> &Dep,
                            DependencyGraph<NamedDecl*>::Vertex &FieldVert) {
   if (FieldType->isTupleType()) {
      for (auto &Cont : FieldType->asTupleType()->getContainedTypes()) {
         addDependencies(Cont, Dep, FieldVert);
      }
   }
   else if (FieldType->isRecordType()) {
      auto R = FieldType->getRecord();
      if (!isa<ClassDecl>(R)) {
         auto &RecVert = Dep.getOrAddVertex(R);
         FieldVert.addIncoming(&RecVert);
      }
   }
}

static bool checkIndirectCase(EnumCaseDecl *E, QualType Ty)
{
   if (Ty->isTupleType()) {
      for (auto &Cont : Ty->asTupleType()->getContainedTypes()) {
         if (checkIndirectCase(E, Cont))
            return true;
      }
   }
   else if (Ty->isRecordType()) {
      return E->getRecord() == Ty->getRecord();
   }

   return false;
}

DeclResult SemaPass::declareEnumDecl(EnumDecl *E)
{
   DeclContextRAII declContextRAII(*this, E);

   QualType CaseValTy;
   if (auto &Ty = E->getRawType()) {
      auto res = visitSourceType(E, E->getRawType());
      if (!res)
         return DeclError();

      CaseValTy = res.get();
   }
   else {
      CaseValTy = Context.getIntTy();
   }

   long long NextCaseVal = 0ll;
   llvm::DenseMap<long long, EnumCaseDecl *> caseVals;

   for (const auto &Case : E->getCases()) {
      if (auto expr = Case->getRawValExpr()) {
         expr->setContextualType(CaseValTy);

         (void) getAsOrCast(E, expr, CaseValTy);
         if (expr->isInvalid())
            continue;

         auto res = evalStaticExpr(E, expr);
         if (!res)
            continue;

         Case->setILValue(res.getValue());
         Case->setRawValue(cast<il::ConstantInt>(res.getValue())
                              ->getSExtValue());

         NextCaseVal = Case->getRawValue();
      }
      else {
         Case->setRawValue(NextCaseVal);
      }

      auto it = caseVals.find(Case->getRawValue());
      if (it != caseVals.end()) {
         diagnose(Case, err_generic_error,
                  "duplicate case value " + std::to_string(NextCaseVal),
                  Case->getSourceLoc());
         diagnose(note_duplicate_case, 0, it->getSecond()->getSourceLoc());
      }

      caseVals.try_emplace(NextCaseVal, Case);
      ++NextCaseVal;
   }

   if (!E->getRawType()) {
      // Get the smallest integer type that can represent all case values.
      QualType RawTy;
      if (NextCaseVal <= INT8_MAX) {
         RawTy = Context.getInt8Ty();
      }
      else if (NextCaseVal <= INT16_MAX) {
         RawTy = Context.getInt16Ty();
      }
      else if (NextCaseVal <= INT32_MAX) {
         RawTy = Context.getInt32Ty();
      }
      else {
         RawTy = Context.getInt64Ty();
      }

      E->getRawType().setResolvedType(RawTy);
   }

   bool NoAssociatedValues = true;
   for (const auto &Case : E->getCases()) {
      if (!Case->getILValue()) {
         Case->setILValue(ILGen->Builder.GetConstantInt(
            E->getRawType(), Case->getRawValue()));
      }
      else {
         auto *CI = cast<il::ConstantInt>(Case->getILValue());
         if (CI->getType() != E->getRawType()) {
            Case->setILValue(ILGen->Builder.GetConstantInt(
               E->getRawType(), CI->getZExtValue()));
         }
      }

      auto &EnumVert = LayoutDependency.getOrAddVertex(E);
      auto &CaseVert = LayoutDependency.getOrAddVertex(Case);

      // every enum is dependent on its cases
      EnumVert.addIncoming(&CaseVert);

      bool AllEquatable = true;
      bool AllCopyable  = true;

      for (const auto &assoc : Case->getArgs()) {
         auto res = declareStmt(Case, assoc);
         if (!res)
            return E;

         // check if this case is indirect, i.e. it contains a value of this
         // enum in some way
         if (!Case->isIndirect() && checkIndirectCase(Case, assoc->getType())) {
            Case->setIndirect(true);
         }

         if (!Case->isIndirect()) {
            addDependencies(assoc->getType(), LayoutDependency, CaseVert);
         }

         AllEquatable &= IsEquatableType(assoc->getType());
         AllCopyable &= IsCopyableType(assoc->getType());

         NoAssociatedValues = false;
      }

      if (AllEquatable) {
         if (auto Equatable = getEquatableDecl()) {
            Context.getConformanceTable()
                   .addExplicitConformance(Context, E, Equatable);

            addDependency(E, Equatable);
         }
      }

      if (AllCopyable) {
         if (auto Copyable = getCopyableDecl()) {
            Context.getConformanceTable()
                   .addExplicitConformance(Context, E, Copyable);

            addDependency(E, Copyable);
         }
      }

      (void)declareStmt(E, Case);
   }

   if (NoAssociatedValues) {
      if (auto P = getImplicitlyCopyableDecl()) {
         Context.getConformanceTable().addExplicitConformance(Context, E, P);
      }
   }

   if (!declareRecordDecl(E))
      return DeclError();

   if (!E->getDeinitializer() && !E->isRawEnum())
      declareDefaultDeinitializer(E);

   return E;
}

DeclResult SemaPass::declareUnionDecl(UnionDecl *U)
{
   DeclContextRAII declContextRAII(*this, U);
   return declareRecordDecl(U);
}

DeclResult SemaPass::declareFieldDecl(FieldDecl *F)
{
   auto R = F->getRecord();

   if (!F->isStatic()) {
      if (isa<EnumDecl>(R)) {
         diagnose(F, err_generic_error,
                  "enums may only contain static fields");
      }
      if (isa<ProtocolDecl>(R)) {
         diagnose(F, err_generic_error,
                  "protocols may only contain static fields");
      }
      if (auto U = dyn_cast<UnionDecl>(R)) {
         if (U->isConst() && !F->isConst())
            diagnose(F, err_generic_error,
                     "expected union field to be constant");

         U->isConst(F->isConst());
      }
   }

   auto &fieldType = F->getType();

   auto res = visitSourceType(F, fieldType);
   if (!res)
      return DeclError();

   if (auto defaultVal = F->getDefaultVal()) {
      defaultVal->setContextualType(fieldType);

      ExprResult typeRes;
      if (F->getType()->isAutoType()) {
         typeRes = visitExpr(F, defaultVal, fieldType);
      }
      else {
         typeRes = getAsOrCast(F, defaultVal, fieldType);
      }

      if (typeRes) {
         F->setValue(typeRes.get());

         auto givenType = typeRes.get()->getExprType();
         if (fieldType->isAutoType()) {
            fieldType.setResolvedType(givenType);

            F->getType().setResolvedType(givenType);
            R->getField(F->getDeclName())
             ->getType().setResolvedType(fieldType);
         }
      }
   }

   if (F->isVariadic()) {
      if (!F->getType().getTypeExpr()) {
         diagnose(F, err_variadic_field_type, F->getSourceRange());
         F->setVariadic(false);
      }
      else if (!F->getType().getTypeExpr()->isVariadicArgPackExpansion()) {
         diagnose(F, err_variadic_field_type,
                  F->getType().getTypeExpr()->getSourceRange());
         F->setVariadic(false);
      }
   }

   auto fieldTy = F->getType().getResolvedType();
   if (auto Prop = F->getAccessor()) {
      if (F->isConst() && Prop->hasSetter()) {
         diagnose(F, err_constant_field_setter);
      }

      Prop->setType(F->getType());
      (void)declareStmt(F, Prop);
   }

   if (F->isStatic()) {
      if (fieldTy->isAutoType())
         DelayedDecls.push_back(F);

      return F;
   }

   auto &RecVert = LayoutDependency.getOrAddVertex(F->getRecord());
   auto &FieldVert = LayoutDependency.getOrAddVertex(F);

   // every struct is dependent on its fields
   RecVert.addIncoming(&FieldVert);

   addDependencies(F->getType(), LayoutDependency, FieldVert);

   return F;
}

DeclResult SemaPass::declarePropDecl(PropDecl *Decl)
{
   auto R = Decl->getRecord();

   auto res = visitSourceType(Decl, Decl->getType());
   if (!res)
      return Decl;

   auto propType = Decl->getType().getResolvedType();
   if (Decl->hasGetter()) {
      DeclarationName DN =
         Context.getDeclNameTable().getAccessorName(*Decl->getIdentifierInfo(),
                                                    DeclarationName::Getter);

      auto ty = SourceType(propType);
      auto Getter = MethodDecl::Create(Context, Decl->getGetterAccess(),
                                       Decl->getSourceLoc(), DN, ty,
                                       { MakeSelfArg(Decl->getSourceLoc()) },
                                       {}, Decl->getGetterBody(),
                                       Decl->isStatic());

      Getter->setSynthesized(true);
      Getter->setProperty(true);

      ActOnDecl(R, Getter);
      Getter->setLexicalContext(Decl->getLexicalContext());
      Getter->setLogicalContext(Decl->getDeclContext());

      if (auto Template = Decl->getPropTemplate())
         Getter->setBodyTemplate(Template->getGetterMethod());

      Decl->setGetterMethod(Getter);

      (void)declareStmt(Getter);
   }

   if (Decl->hasSetter()) {
      auto argType = SourceType(propType);
      auto arg = FuncArgDecl::Create(Context, Decl->getSourceLoc(),
                                     Decl->getSourceLoc(),
                                     Decl->getNewValNameInfo(), nullptr,
                                     ArgumentConvention::Owned,
                                     argType, nullptr, false);

      arg->setSynthesized(true);

      DeclarationName DN =
         Context.getDeclNameTable().getAccessorName(*Decl->getIdentifierInfo(),
                                                    DeclarationName::Setter);

      auto retTy = SourceType(Context.getVoidType());

      FuncArgDecl *Args[]{ MakeSelfArg(Decl->getSourceLoc()), arg};
      auto Setter = MethodDecl::Create(Context, Decl->getSetterAccess(),
                                       Decl->getSourceLoc(), DN, retTy,
                                       Args, {}, Decl->getSetterBody(),
                                       Decl->isStatic());

      Setter->setSynthesized(true);
      Setter->setProperty(true);

      ActOnDecl(R, Setter);
      Setter->setLexicalContext(Decl->getLexicalContext());
      Setter->setLogicalContext(Decl->getDeclContext());

      if (!isa<ClassDecl>(R))
         Setter->setMutating(true);

      if (auto Template = Decl->getPropTemplate())
         Setter->setBodyTemplate(Template->getSetterMethod());

      Decl->setSetterMethod(Setter);

      (void)declareStmt(Setter);
   }

   return Decl;
}

DeclResult SemaPass::declareSubscriptDecl(SubscriptDecl *Decl)
{
   auto R = Decl->getRecord();

   auto res = visitSourceType(Decl, Decl->getType());
   if (!res)
      return Decl;

   llvm::SmallVector<FuncArgDecl*, 8> Args;
   Args.resize(Decl->getArgs().size() + 1);

   std::copy(Decl->getArgs().begin(), Decl->getArgs().end(), Args.begin() + 1);

   auto propType = Decl->getType().getResolvedType();
   if (Decl->hasGetter()) {
      DeclarationName DN =
         Context.getDeclNameTable().getSubscriptName(
            DeclarationName::SubscriptKind::Getter);

      Args[0] = MakeSelfArg(Decl->getSourceLoc());

      auto ty = SourceType(propType);
      auto Getter = MethodDecl::Create(Context, Decl->getGetterAccess(),
                                       Decl->getSourceLoc(), DN, ty,
                                       Args, {}, Decl->getGetterBody(),
                                       Decl->isStatic());

      Getter->setSynthesized(true);
      Getter->setSubscript(true);

      ActOnDecl(R, Getter);
      Getter->setLexicalContext(Decl->getLexicalContext());
      Getter->setLogicalContext(Decl->getDeclContext());

      if (auto Template = Decl->getTemplate())
         Getter->setBodyTemplate(Template->getGetterMethod());

      Decl->setGetterMethod(Getter);

      (void)declareStmt(Getter);
   }

   if (Decl->hasSetter()) {
      auto argType = SourceType(propType);

      // the argument needs a dummy default value for overload resolution
      auto DefVal = BuiltinExpr::Create(Context, argType);
      auto arg = FuncArgDecl::Create(Context, Decl->getSourceLoc(),
                                     Decl->getSourceLoc(),
                                     Decl->getNewValNameInfo(), nullptr,
                                     ArgumentConvention::Owned,
                                     argType, DefVal, false);

      arg->setSynthesized(true);

      DeclarationName DN =
         Context.getDeclNameTable().getSubscriptName(
            DeclarationName::SubscriptKind::Setter);


      auto retTy = SourceType(Context.getVoidType());

      Args[0] = MakeSelfArg(Decl->getSourceLoc());
      Args.push_back(arg);

      auto Setter = MethodDecl::Create(Context, Decl->getSetterAccess(),
                                       Decl->getSourceLoc(), DN, retTy,
                                       Args, {}, Decl->getSetterBody(),
                                       Decl->isStatic());

      Setter->setSynthesized(true);
      Setter->setSubscript(true);

      ActOnDecl(R, Setter);
      Setter->setLexicalContext(Decl->getLexicalContext());
      Setter->setLogicalContext(Decl->getDeclContext());

      if (!isa<ClassDecl>(R))
         Setter->setMutating(true);

      if (auto Template = Decl->getTemplate())
         Setter->setBodyTemplate(Template->getSetterMethod());

      Decl->setSetterMethod(Setter);

      (void)declareStmt(Setter);
   }

   return Decl;
}

DeclResult SemaPass::declareAssociatedTypeDecl(AssociatedTypeDecl *Decl)
{
   visitConstraints(Decl);

   if (!Decl->getActualType())
      return Decl;

   auto TypeRes = visitSourceType(Decl, Decl->getActualType());
   if (!TypeRes)
      return DeclError();

   auto Ty = TypeRes.get();
   if (Ty->isAutoType()) {
      Decl->getActualType().setResolvedType(UnknownAnyTy);
   }

   if (Decl->isImplementation()) {
      // update the AssociatedType associated with this declaration
      Context.getAssociatedType(Decl)->setCanonicalType(Decl->getActualType());
   }

   return Decl;
}

DeclResult SemaPass::declareMethodDecl(MethodDecl *M)
{
   if (M->hasMutableSelf() && isa<ClassDecl>(M->getDeclContext())) {
      diagnose(M, err_mutating_on_class, M->getSourceLoc());
      M->setMutating(false);
   }

   if (!declareCallableDecl(M))
      return DeclError();

   if (M->getDeclName().isStr("copy")
         && !M->isStatic()
         && M->getArgs().size() == 1
         && M->getReturnType()->isRecordType()
         && M->getReturnType()->getRecord() == M->getRecord()) {
      M->getRecord()->setCopyFn(M);
   }

   // check virtual and override methods after all signatures (most
   // importantly those of base classes) are available
   if (stage <= Stage::Declaration) {
      DelayedDecls.push_back(M);
   }
   else {
      checkVirtualOrOverrideMethod(M);
   }

   return M;
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

DeclResult SemaPass::declareInitDecl(InitDecl *Init)
{
   auto R = Init->getRecord();
   if (Init->getArgs().empty() && isa<StructDecl>(R)
         && Init->isCompleteInitializer()) {
      cast<StructDecl>(R)->setParameterlessConstructor(Init);
   }

   if (Init->isMemberwise()) {
      if (auto Str = dyn_cast<StructDecl>(R)) {
         declareMemberwiseInitializer(Str, Init);
         return DeclError();
      }

      llvm_unreachable("memberwise init should have been rejected");
   }

   QualType RecordTy = Context.getRecordType(Init->getRecord());
   Init->setReturnType(SourceType(Context.getVoidType()));

   if (!Init->getDeclName()) {
      auto DeclName = Context.getDeclNameTable().getConstructorName(RecordTy);

      Init->setName(DeclName);
      makeDeclAvailable(*Init->getRecord(), DeclName, Init);
   }

   if (Init->isDefaultInitializer() && Init->isCompleteInitializer())
      cast<StructDecl>(R)->setDefaultInitializer(Init);

   if (Init->isMemberwiseInitializer() && Init->isCompleteInitializer())
      cast<StructDecl>(R)->setMemberwiseInitializer(Init);

   llvm::SmallPtrSet<TemplateParamDecl*, 4> Params;
   Params.insert(Init->getTemplateParams().begin(),
                 Init->getTemplateParams().end());

   if (!declareCallableDecl(Init))
      return DeclError();

   ParamTypeVisitor V(Params);
   for (auto &arg : Init->getArgs()) {
      V.RecursiveTypeVisitor::visit(arg->getType().getResolvedType());
   }

   if (!Params.empty()) {
      diagnose(Init, err_initializer_templ_args_must_be_inferrable,
               0 /*initializer*/, (*Params.begin())->getDeclName(),
               (*Params.begin())->getSourceRange());
   }

   if (!isa<ClassDecl>(R))
      Init->setMutating(true);

   // declare base initializer
   if (!Init->isBaseInitializer()) {
      auto BaseInitName = Context.getDeclNameTable()
                                 .getConstructorName(RecordTy, false);

      SmallVector<FuncArgDecl*, 4> Args{ MakeSelfArg(Init->getSourceLoc() )};
      Args.append(Init->getArgs().begin(), Init->getArgs().end());

      ASTVector<TemplateParamDecl*> Params(Context, Init->getTemplateParams());
      auto BaseInit = InitDecl::Create(Context, Init->getAccess(),
                                       Init->getSourceLoc(), Args,
                                       move(Params), Init->getBody(),
                                       BaseInitName, Init->isFallible());

      ActOnDecl(&getDeclContext(), BaseInit, false);
      (void)declareStmt(BaseInit);

      Init->setBaseInit(BaseInit);

      BaseInit->setSynthesized(true);
      BaseInit->setCompleteInit(Init);
      BaseInit->setBodyTemplate(Init->getBodyTemplate());
   }

   if (Init->isFallible()) {
      auto Opt = getOptionDecl();
      if (!Opt) {
         diagnose(Init, err_no_builtin_decl, Init->getSourceLoc(),
            /*fallible init*/ 10);

         return DeclError();
      }

      if (Init->getRecord()->isTemplate())
         return Init;

      sema::ResolvedTemplateArg Arg(Opt->getTemplateParams().front(),
                                    RecordTy, Init->getSourceLoc());

      auto TemplateArgs = sema::FinalTemplateArgumentList::Create(Context, Arg);
      auto Inst = Instantiator.InstantiateRecord(Init, Opt,
                                                 move(TemplateArgs));

      QualType ResultTy;
      if (Inst.hasValue())
         ResultTy = Context.getRecordType(Inst.getValue());
      else
         ResultTy = Context.getRecordType(Opt);

      Init->setOptionTy(ResultTy);
   }

   return Init;
}

DeclResult SemaPass::declareDeinitDecl(DeinitDecl *Deinit)
{
   Deinit->setReturnType(SourceType(Context.getVoidType()));
   Deinit->setMutating(!isa<ClassDecl>(Deinit->getRecord()));

   if (!Deinit->getDeclName()) {
      QualType RecordTy = Context.getRecordType(Deinit->getRecord());
      auto DeclName = Context.getDeclNameTable().getDestructorName(RecordTy);

      Deinit->setName(DeclName);
      makeDeclAvailable(*Deinit->getRecord(), DeclName, Deinit);
   }

   if (!declareCallableDecl(Deinit))
      return DeclError();

   return Deinit;
}

DeclResult SemaPass::declareTypedefDecl(TypedefDecl *Decl)
{
   if (!visitSourceType(Decl, Decl->getOriginTy()))
      return DeclError();

   // update the TypedefType associated with this declaration
   Context.getTypedefType(Decl)->setCanonicalType(Decl->getOriginTy());

   return Decl;
}

DeclResult SemaPass::declareAliasDecl(AliasDecl *Decl)
{
   if (!Decl->getAliasExpr() && !Decl->hasAttribute<_BuiltinAttr>()) {
      diagnose(Decl, err_alias_without_value, Decl->getSourceRange());
   }

   DeclContextRAII declContextRAII(*this, Decl);
   for (auto &C : Decl->getConstraints())
      visitStaticExpr(C);

   return Decl;
}

TypeResult SemaPass::visitSourceType(Decl *D, const SourceType &Ty,
                                     bool WantMeta) {
   auto Result = visitSourceType(Ty, WantMeta);
   if (Ty.getTypeExpr())
      D->copyStatusFlags(Ty.getTypeExpr());

   return Result;
}

TypeResult SemaPass::visitSourceType(Statement *S, const SourceType &Ty,
                                     bool WantMeta) {
   auto Result = visitSourceType(Ty, WantMeta);
   if (Ty.getTypeExpr())
      S->copyStatusFlags(Ty.getTypeExpr());

   return Result;
}

TypeResult SemaPass::visitSourceType(const SourceType &Ty, bool WantMeta)
{
   if (!Ty.getTypeExpr() || Ty.isResolved()) {
      assert(Ty.getResolvedType() && "source ty with no expr or resolved type");
      return Ty.getResolvedType();
   }

   auto Result = visitExpr(Ty.getTypeExpr());
   if (!Result) {
      Ty.setResolvedType(UnknownAnyTy);
      return TypeError();
   }

   QualType ResTy = Result.get()->getExprType();
   Ty.setTypeExpr(Result.get());

   // check if we were given an expression instead of a type
//   if (Ty.getTypeExpr() && !isa<TypeExpr>(Ty.getTypeExpr())) {
//      if (!ResTy->isMetaType() && !ResTy->isUnknownAnyType()) {
//         diagnose(Ty.getTypeExpr(), err_expression_in_type_position,
//                  Ty.getTypeExpr()->getSourceRange());
//      }
//   }

   if (WantMeta && !ResTy->isMetaType() && isa<TypeExpr>(Ty.getTypeExpr())) {
      ResTy = Context.getMetaType(ResTy);
   }
   else if (!WantMeta && ResTy->isMetaType()) {
      ResTy = cast<MetaType>(ResTy)->getUnderlyingType();
   }

   Ty.setResolvedType(ResTy);
   return Ty.getResolvedType();
}

ExprResult SemaPass::visitTypeExpr(TypeExpr *Expr)
{
   QualType ResTy = Expr->getExprType();
   if (!Expr->isMeta() && ResTy->isMetaType()) {
      Expr->setExprType(cast<cdot::MetaType>(ResTy)->getUnderlyingType());
   }

   return Expr;
}

ExprResult SemaPass::visitTupleTypeExpr(TupleTypeExpr *Expr)
{
   SmallVector<QualType, 8> Tys;
   for (auto &Ty : Expr->getContainedTypes()) {
      auto Res = visitSourceType(Expr, Ty);
      if (!Res)
         continue;

      Tys.push_back(Res.get());
   }

   Expr->setExprType(Context.getMetaType(Context.getTupleType(Tys)));
   return visitTypeExpr(Expr);
}

ExprResult SemaPass::visitFunctionTypeExpr(FunctionTypeExpr *Expr)
{
   llvm::SmallVector<QualType, 4> ArgTys;
   for (auto &Ty : Expr->getArgTypes()) {
      auto Res = visitSourceType(Expr, Ty);
      if (Res) {
         ArgTys.push_back(Res.get());
      }
      else {
         ArgTys.push_back(UnknownAnyTy);
      }
   }

   SmallVector<FunctionType::ParamInfo, 4> ParamInfo;
   for (auto &I : Expr->getParamInfo()) {
      if (I.getConvention() == ArgumentConvention::Default) {
         ParamInfo.push_back(ArgumentConvention::Borrowed);
      }
      else {
         ParamInfo.push_back(I);
      }
   }

   auto RetTyResult = visitSourceType(Expr, Expr->getReturnType());
   QualType RetTy = RetTyResult ? RetTyResult.get() : UnknownAnyTy;

   if (Expr->isThin())
      Expr->setExprType(Context.getFunctionType(RetTy, ArgTys, ParamInfo));
   else
      Expr->setExprType(Context.getLambdaType(RetTy, ArgTys, ParamInfo));

   Expr->setExprType(Context.getMetaType(Expr->getExprType()));
   return visitTypeExpr(Expr);
}

TypeResult SemaPass::resolveArrayTypeExpr(Statement *DependentExpr,
                                          const SourceType &ElementType,
                                          StaticExpr *SizeExpr) {
   auto ElementTyResult = visitSourceType(DependentExpr, ElementType);
   QualType ElementTy = ElementTyResult ? ElementTyResult.get()
                                        : UnknownAnyTy;

   QualType ResultTy;

   // inferred array size
   if (!SizeExpr) {
      ResultTy = Context.getInferredSizeArrayType(ElementTy);
   }
   else {
      auto SizeRes = visitExpr(DependentExpr, SizeExpr);
      if (!SizeRes)
         return TypeError();

      if (SizeRes.get()->isDependent()) {
         ResultTy = Context.getValueDependentSizedArrayType(ElementTy,
                                                            SizeExpr);
      }
   }

   if (!ResultTy) {
      assert(SizeExpr != nullptr && "no array size given!");

      auto res = Evaluator.evaluate(SizeExpr);
      if (res) {
         auto Val = res.getResult();

         // this is the same limit that clang uses
         constexpr uint64_t MaxArraySize = (1llu << 59) - 1;

         size_t NumElements = 0;
         auto CI = dyn_cast<il::ConstantInt>(Val);

         if (!CI) {
            diagnose(DependentExpr, err_array_size_must_be_integral,
                     SizeExpr->getSourceRange());

            NumElements = 1;
         }
         else if (CI->getValue().ugt(MaxArraySize)) {
            diagnose(DependentExpr, err_array_size_too_large,
                     llvm::APInt(64, MaxArraySize, false), CI->getValue());

            NumElements = MaxArraySize;
         }
         else {
            NumElements = CI->getZExtValue();
         }

         if (NumElements == 0) {
            diagnose(DependentExpr, err_zero_size_array,
                     SizeExpr->getSourceRange());

            NumElements = 1;
         }

         ResultTy = Context.getArrayType(ElementTy, NumElements);
      }
      else {
         ResultTy = UnknownAnyTy;
      }
   }

   return ResultTy;
}

ExprResult SemaPass::visitArrayTypeExpr(ArrayTypeExpr *Expr)
{
   auto TypeRes = resolveArrayTypeExpr(Expr, Expr->getElementTy(),
                                       Expr->getSizeExpr());

   if (!TypeRes)
      return ExprError();

   Expr->setExprType(Context.getMetaType(TypeRes.get()));
   return visitTypeExpr(Expr);
}

ExprResult SemaPass::visitDeclTypeExpr(DeclTypeExpr *Expr)
{
   auto ExprRes = visitExpr(Expr, Expr->getTyExpr());
   QualType ResultTy;

   if (!ExprRes)
      ResultTy = UnknownAnyTy;
   else
      ResultTy = ExprRes.get()->getExprType()->stripReference();

   Expr->setExprType(Context.getMetaType(ResultTy));
   return visitTypeExpr(Expr);
}

static bool inExternCXXSignature(SemaPass &SP)
{
   for (auto *Ctx = &SP.getDeclContext(); Ctx; Ctx = Ctx->getParentCtx()) {
      auto C = dyn_cast<CallableDecl>(Ctx);
      if (C && C->isExternCXX()) {
         return true;
      }
   }

   return false;
}

ExprResult SemaPass::visitPointerTypeExpr(PointerTypeExpr *Expr)
{
   auto TypeRes = visitSourceType(Expr, Expr->getSubType());

   QualType ResultTy;
   if (!TypeRes) {
      ResultTy = UnknownAnyTy;
   }
   else {
      auto Pointee = TypeRes.get();
      if (Pointee->isVoidType() && !inExternCXXSignature(*this)) {
         diagnose(Expr, err_pointer_to_void, Expr->getSourceRange());
         Pointee = Context.getUInt8Ty();
      }

      ResultTy = Context.getPointerType(Pointee);
   }

   Expr->setExprType(Context.getMetaType(ResultTy));
   return visitTypeExpr(Expr);
}

ExprResult SemaPass::visitReferenceTypeExpr(ReferenceTypeExpr *Expr)
{
   auto TypeRes = visitSourceType(Expr, Expr->getSubType());

   QualType ResultTy;
   if (!TypeRes) {
      ResultTy = UnknownAnyTy;
   }
   else {
      auto ReferencedTy = TypeRes.get();
      if (ReferencedTy->isVoidType()) {
         diagnose(Expr, err_cannot_reference_type, ReferencedTy,
                  Expr->getSourceRange());

         ReferencedTy = UnknownAnyTy;
      }

      ResultTy = Context.getReferenceType(ReferencedTy);
   }

   Expr->setExprType(Context.getMetaType(ResultTy));
   return visitTypeExpr(Expr);
}

ExprResult SemaPass::visitOptionTypeExpr(OptionTypeExpr *Expr)
{
   auto TypeRes = visitSourceType(Expr, Expr->getSubType());

   QualType ResultTy;
   if (!TypeRes) {
      ResultTy = UnknownAnyTy;
   }
   else {
      auto Opt = getOptionDecl();
      if (Opt) {
         ResolvedTemplateArg Arg(Opt->getTemplateParams().front(),
                                 TypeRes.get(), Expr->getSubType().getTypeExpr()
                                                    ->getSourceLoc());

         auto TemplateArgs = FinalTemplateArgumentList::Create(Context,
                                                               { Arg });

         if (TypeRes.get()->isDependentType()) {
            ResultTy = Context.getDependentRecordType(Opt, move(TemplateArgs));
         }
         else {
            auto Inst = Instantiator.InstantiateRecord(Expr, Opt,
                                                       move(TemplateArgs));

            if (Inst.hasValue())
               ResultTy = Context.getRecordType(Inst.getValue());
            else
               ResultTy = Context.getRecordType(Opt);
         }
      }
      else {
         diagnose(Expr, err_no_builtin_decl, /*Option type*/ 1);
         ResultTy = UnknownAnyTy;
      }
   }

   Expr->setExprType(Context.getMetaType(ResultTy));
   return visitTypeExpr(Expr);
}

StmtResult SemaPass::declareDebugStmt(DebugStmt *Stmt)
{
   if (!Stmt->isUnreachable()) {
      int i = 3;
      (void)i;
   }

   return Stmt;
}

namespace {

class MixinPrettyStackTrace: public llvm::PrettyStackTraceEntry {
public:
   MixinPrettyStackTrace(llvm::StringRef Str, bool isParse)
      : MixinStr(Str), IsParse(isParse)
   {
   }

   void print(llvm::raw_ostream &OS) const override
   {
      OS << "while " << (IsParse ? "parsing" : "evaluating")
         << " mixin string '" << MixinStr << "'\n";
   }

private:
   llvm::StringRef MixinStr;
   bool IsParse;
};

} // anonymous namespace

DeclResult SemaPass::declareMixinDecl(MixinDecl *Decl)
{
   auto Res = evalStaticExpr(Decl, Decl->getMixinExpr());
   if (!Res)
      return Decl;

   Decl->setMixinExpr(Res.getExpr());

   auto V = Res.getValue();
   llvm::StringRef Str;

   if (!getStringValue(Decl->getMixinExpr(), V, Str)) {
      diagnose(Decl, err_mixin_expr_must_be_string,
               Decl->getMixinExpr()->getSourceRange(),
               Decl->getMixinExpr()->getExprType());

      return DeclError();
   }

   auto Buf = Diags.getFileMgr()->getBufferForString(Str);
   assert(Buf.Buf && "invalid buffer for string?");

   {
      MixinPrettyStackTrace PST(Str, true);
      lex::Lexer lex(Context.getIdentifiers(), Diags, Buf.Buf, Buf.SourceId,
                     Buf.BaseOffset);

      parse::Parser parser(Context, &lex, *this);
      parser.parseWithKind(Decl->getSourceLoc(),
                           parse::Parser::ExpansionKind::Decl, true);
   }

   return Decl;
}

ExprResult SemaPass::visitMixinExpr(MixinExpr *Expr)
{
   auto Res = evalStaticExpr(Expr, Expr->getMixinExpr());
   if (!Res) {
      Expr->setExprType(UnknownAnyTy);
      return ExprError();
   }

   auto V = Res.getValue();
   llvm::StringRef Str;

   if (!getStringValue(Expr->getMixinExpr(), V, Str)) {
      diagnose(Expr, err_mixin_expr_must_be_string,
               Expr->getMixinExpr()->getSourceRange(),
               Expr->getMixinExpr()->getExprType());

      return ExprError();
   }

   if (Str.empty()) {
      diagnose(Expr, err_invalid_mixin_kind, 0 /*expression*/,
               Expr->getMixinExpr()->getSourceRange());
      return ExprError();
   }

   auto Buf = Diags.getFileMgr()->getBufferForString(Str);
   assert(Buf.Buf && "invalid buffer for string?");

   parse::ParseResult Result;
   {
      MixinPrettyStackTrace PST(Str, true);
      lex::Lexer lex(Context.getIdentifiers(), Diags, Buf.Buf, Buf.SourceId,
                     Buf.BaseOffset);

      parse::Parser parser(Context, &lex, *this);
      Result = parser.parseWithKind(Expr->getSourceLoc(),
                                    parse::Parser::ExpansionKind::Expr);

      if (!Result) {
         Expr->setIsInvalid(true);
         return ExprError();
      }
   }

   MixinPrettyStackTrace PST(Str, false);
   return visitExpr(Expr, Result.getExpr());
}

StmtResult SemaPass::visitMixinStmt(MixinStmt *Stmt)
{
   auto Res = evalStaticExpr(Stmt, Stmt->getMixinExpr());
   if (!Res)
      return StmtError();

   Stmt->setMixinExpr(Res.getExpr());

   auto V = Res.getValue();
   llvm::StringRef Str;

   if (!getStringValue(Stmt->getMixinExpr(), V, Str)) {
      diagnose(Stmt, err_mixin_expr_must_be_string,
               Stmt->getMixinExpr()->getSourceRange(),
               Stmt->getMixinExpr()->getExprType());

      return StmtError();
   }

   auto Buf = Diags.getFileMgr()->getBufferForString(Str);
   assert(Buf.Buf && "invalid buffer for string?");

   parse::ParseResult Result;
   {
      MixinPrettyStackTrace PST(Str, true);
      lex::Lexer lex(Context.getIdentifiers(), Diags, Buf.Buf, Buf.SourceId,
                     Buf.BaseOffset);

      parse::Parser parser(Context, &lex, *this);
      Result = parser.parseWithKind(Stmt->getSourceLoc(),
         parse::Parser::ExpansionKind::Stmt);

      if (!Result) {
         Stmt->setIsInvalid(true);
         return StmtError();
      }
   }

   MixinPrettyStackTrace PST(Str, false);
   return visitStmt(Stmt, Result.getStatement());
}

DeclResult SemaPass::declareStaticAssertStmt(StaticAssertStmt *Stmt)
{
   // will be evaluated in Sema
   return Stmt;
}

DeclResult SemaPass::declareStaticPrintStmt(StaticPrintStmt *Stmt)
{
   return Stmt;
}

DeclResult SemaPass::declareStaticIfDecl(StaticIfDecl *Stmt)
{
   auto Res = visitExpr(Stmt, Stmt->getCondition());
   if (!Res)
      return DeclError();

   if (Stmt->getCondition()->isDependent()) {
      currentScope->setHasUnresolvedStaticCond(true);

      IgnoreDiagsRAII IDR(*this);
      (void) visitDecl(Stmt->getIfDecl());

      if (auto *Else = Stmt->getElseDecl()) {
         (void) visitDecl(Else);
      }

      return Stmt;
   }

   auto res = evaluateAsBool(Stmt, Stmt->getCondition());
   if (!res)
      return Stmt;

   bool CondIsTrue = cast<il::ConstantInt>(res.getValue())->getBoolValue();
   if (auto Template = Stmt->getTemplate()) {
      // collect the template arguments at the point of instantiation
      MultiLevelFinalTemplateArgList TemplateArgs;
      for (auto Ctx = &getDeclContext(); Ctx; Ctx = Ctx->getParentCtx()) {
         if (auto ND = dyn_cast<NamedDecl>(Ctx)) {
            if (ND->isInstantiation())
               TemplateArgs.addOuterList(ND->getTemplateArgs());
         }
      }

      DeclResult Inst;
      if (CondIsTrue) {
         Inst = Instantiator.InstantiateDecl(Stmt->getStaticLoc(),
                                             Template->getIfDecl(),
                                             move(TemplateArgs));
      }
      else if (auto Else = Template->getElseDecl()) {
         Inst = Instantiator.InstantiateDecl(Stmt->getStaticLoc(),
                                             Else, move(TemplateArgs));
      }

      if (Inst) {
         auto DeclRes = declareStmt(Stmt, Inst.getValue());
         if (!DeclRes)
            return DeclRes;

         transferDecls(cast<CompoundDecl>(DeclRes.getValue()),
                       &getDeclContext());
      }

      return Stmt;
   }

   if (CondIsTrue) {
      transferDecls(Stmt->getIfDecl(), &getDeclContext());
   }
   else if (auto Else = Stmt->getElseDecl())
      transferDecls(Else, &getDeclContext());

   return Stmt;
}

DeclResult SemaPass::declareStaticForDecl(StaticForDecl *Stmt)
{
   auto Res = visitExpr(Stmt, Stmt->getRange());
   if (!Res)
      return DeclError();

   return Stmt;
}

DeclResult SemaPass::declareEnumCaseDecl(EnumCaseDecl *Case)
{
   Case->createFunctionType(*this);
   return Case;
}

DeclResult SemaPass::declareMacroExpansionDecl(MacroExpansionDecl *Decl)
{
   if (!checkNamespaceRef(Decl))
      return DeclError();

   DeclContext *Ctx = &getDeclContext();
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

   auto Result = checkMacroCommon(Decl, Decl->getMacroName(), *Ctx,
                                  (MacroDecl::Delimiter )Decl->getDelim(),
                                  Decl->getTokens(),
                                  (unsigned)parse::Parser::ExpansionKind::Decl);

   if (!Result)
      return DeclError();

   if (!Result.asDecl()) {
      diagnose(Decl, err_macro_must_form, 3, Decl->getSourceLoc());
      return DeclError();
   }

   if (stage == Stage::Declaration) {
      ensureDeclared(Result.getDecl());
      return Result.getDecl();
   }
   if (stage >= Stage::Sema) {
      ensureVisited(Result.getDecl());
      return Result.getDecl();
   }

   return Result.getDecl();
}

ExprResult SemaPass::visitMacroExpansionExpr(MacroExpansionExpr *Expr)
{
   if (!checkNamespaceRef(Expr))
      return ExprError();

   DeclContext *Ctx = &getDeclContext();
   if (auto *Ident = cast_or_null<IdentifierRefExpr>(Expr->getParentExpr())) {
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

   auto Result = checkMacroCommon(Expr, Expr->getMacroName(), *Ctx,
                                  (MacroDecl::Delimiter)Expr->getDelim(),
                                  Expr->getTokens(),
                                  (unsigned)parse::Parser::ExpansionKind::Expr);

   if (!Result)
      return ExprError();

   if (!dyn_cast_or_null<Expression>(Result.asStatement())) {
      diagnose(Expr, err_macro_must_form, 0, Expr->getSourceLoc());
      return ExprError();
   }

   return visitExpr(Expr, cast<Expression>(Result.getStatement()));
}

StmtResult SemaPass::visitMacroExpansionStmt(MacroExpansionStmt *Stmt)
{
   if (!checkNamespaceRef(Stmt))
      return StmtError();

   DeclContext *Ctx = &getDeclContext();
   if (auto *Ident = cast_or_null<IdentifierRefExpr>(Stmt->getParentExpr())) {
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

   auto Result = checkMacroCommon(Stmt, Stmt->getMacroName(), *Ctx,
                                  (MacroDecl::Delimiter )Stmt->getDelim(),
                                  Stmt->getTokens(),
                                  (unsigned)parse::Parser::ExpansionKind::Stmt);

   if (!Result)
      return StmtError();

   if (!Result.asStatement()) {
      diagnose(Stmt, err_macro_must_form, 1, Stmt->getSourceLoc());
      return StmtError();
   }

   auto StmtRes = visitStmt(Stmt, Result.getStatement());
   if (!StmtRes)
      return StmtError();

   if (stage >= Stage::Sema) {
      finalizeRecordDecls();
   }

   return StmtRes;
}

StmtOrDecl SemaPass::checkMacroCommon(StmtOrDecl SOD,
                                      DeclarationName MacroName,
                                      DeclContext &Ctx,
                                      MacroDecl::Delimiter Delim,
                                      llvm::ArrayRef<lex::Token> Tokens,
                                      unsigned Kind) {
   auto Macro = LookupSingle<MacroDecl>(Ctx, MacroName);
   parse::ParseResult Result;

   if (!Macro) {
      auto BuiltinResult = parse::Parser::checkBuiltinMacro(*this, MacroName,
         SOD, Tokens, (parse::Parser::ExpansionKind)Kind);

      if (BuiltinResult.second) {
         Result = BuiltinResult.first;
      }
      else {
         diagnoseMemberNotFound(&Ctx, SOD, MacroName, err_macro_does_not_exist);
         return StmtOrDecl();
      }
   }
   else if (Delim != Macro->getDelim()) {
      llvm::StringRef ExpectedDelim;
      switch (Macro->getDelim()) {
      case MacroDecl::Paren: ExpectedDelim = "()"; break;
      case MacroDecl::Brace: ExpectedDelim = "{}"; break;
      case MacroDecl::Square: ExpectedDelim = "[]"; break;
      }

      diagnose(SOD, err_macro_expects_delim, SOD.getSourceRange(),
               MacroName, ExpectedDelim);
   }

   if (Macro) {
      Result = parse::Parser::expandMacro(*this, Macro, SOD, Tokens,
                                          (parse::Parser::ExpansionKind) Kind);
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

Module* SemaPass::getStdModule()
{
   if (!StdModule) {
      auto Loc = getCompilationUnit().getMainFileLoc();
      StdModule = getCompilationUnit().getModuleMgr()
                                  .LookupModule(Loc, Loc, getIdentifier("std"));
   }

   return StdModule;
}

Module* SemaPass::getPreludeModule()
{
   if (!PreludeModule) {
      IdentifierInfo *Name[] = {
         getIdentifier("std"), getIdentifier("prelude")
      };

      PreludeModule = getCompilationUnit().getModuleMgr().GetModule(Name);
   }

   return PreludeModule;
}

Module* SemaPass::getBuiltinModule()
{
   if (!BuiltinModule) {
      IdentifierInfo *Name[] = {
         getIdentifier("std"), getIdentifier("builtin")
      };

      BuiltinModule = getCompilationUnit().getModuleMgr().GetModule(Name);
   }

   return BuiltinModule;
}

Module* SemaPass::getReflectModule()
{
   if (!ReflectModule) {
      IdentifierInfo *Name[] = {
         getIdentifier("std"), getIdentifier("reflect")
      };

      ReflectModule = getCompilationUnit().getModuleMgr().GetModule(Name);
   }

   return ReflectModule;
}

Module* SemaPass::getSysModule()
{
   if (!SysModule) {
      IdentifierInfo *Name[] = {
         getIdentifier("std"), getIdentifier("sys")
      };

      SysModule = getCompilationUnit().getModuleMgr().GetModule(Name);
   }

   return SysModule;
}

Module* SemaPass::getAsyncModule()
{
   if (!AsyncModule) {
      IdentifierInfo *Name[] = {
         getIdentifier("std"), getIdentifier("async")
      };

      AsyncModule = getCompilationUnit().getModuleMgr().GetModule(Name);
   }

   return AsyncModule;
}

bool SemaPass::isInStdModule(Decl *D)
{
   return D->getModule()->getModule() == getStdModule();
}

bool SemaPass::isInBuiltinModule(Decl *D)
{
   return D->getModule()->getModule() == getBuiltinModule();
}

bool SemaPass::isInReflectModule(Decl *D)
{
   return D->getModule()->getModule() == getReflectModule();
}

FunctionDecl* SemaPass::getPureVirtualDecl()
{
   if (!PureVirtual) {
      auto *Prelude = getPreludeModule();
      if (!Prelude)
         return nullptr;

      auto *II = &Context.getIdentifiers().get("_cdot_PureVirtual");
      PureVirtual = LookupSingle<FunctionDecl>(*Prelude->getDecl(), II);
   }

   return PureVirtual;
}

ClassDecl* SemaPass::getArrayDecl()
{
   if (!ArrayDecl) {
      auto Prelude = getPreludeModule();
      if (!Prelude)
         return nullptr;
      
      auto *II = &Context.getIdentifiers().get("Array");
      ArrayDecl = LookupSingle<ClassDecl>(*Prelude->getDecl(), II);
   }

   return ArrayDecl;
}

StructDecl* SemaPass::getArrayViewDecl()
{
   if (!ArrayViewDecl) {
      auto Prelude = getPreludeModule();
      if (!Prelude)
         return nullptr;
      
      auto *II = &Context.getIdentifiers().get("ArrayView");
      ArrayViewDecl = LookupSingle<StructDecl>(*Prelude->getDecl(), II);
   }

   return ArrayViewDecl;
}

ClassDecl* SemaPass::getDictionaryDecl()
{
   if (!DictionaryDecl) {
      auto Prelude = getPreludeModule();
      if (!Prelude)
         return nullptr;
      
      auto *II = &Context.getIdentifiers().get("Dictionary");
      DictionaryDecl = LookupSingle<ClassDecl>(*Prelude->getDecl(), II);
   }

   return DictionaryDecl;
}

EnumDecl* SemaPass::getOptionDecl()
{
   if (!OptionDecl) {
      auto Prelude = getPreludeModule();
      if (!Prelude)
         return nullptr;
      
      auto *II = &Context.getIdentifiers().get("Option");
      OptionDecl = LookupSingle<EnumDecl>(*Prelude->getDecl(), II);
   }

   return OptionDecl;
}

ClassDecl* SemaPass::getStringDecl()
{
   if (!StringDecl) {
      auto Prelude = getPreludeModule();
      if (!Prelude)
         return nullptr;
      
      auto *II = &Context.getIdentifiers().get("String");
      StringDecl = LookupSingle<ClassDecl>(*Prelude->getDecl(), II);
   }

   return StringDecl;
}

StructDecl* SemaPass::getStringViewDecl()
{
   if (!StringViewDecl) {
      auto Prelude = getPreludeModule();
      if (!Prelude)
         return nullptr;
      
      auto *II = &Context.getIdentifiers().get("StringView");
      StringViewDecl = LookupSingle<StructDecl>(*Prelude->getDecl(), II);
   }

   return StringViewDecl;
}

StructDecl* SemaPass::getTypeInfoDecl()
{
   if (!TypeInfoDecl) {
      auto Prelude = getPreludeModule();
      if (!Prelude)
         return nullptr;
      
      auto *II = &Context.getIdentifiers().get("TypeInfo");
      TypeInfoDecl = LookupSingle<StructDecl>(*Prelude->getDecl(), II);
   }

   return TypeInfoDecl;
}

StructDecl* SemaPass::getBoxDecl()
{
   if (!BoxDecl) {
      auto Prelude = getPreludeModule();
      if (!Prelude)
         return nullptr;
      
      auto *II = &Context.getIdentifiers().get("Box");
      BoxDecl = LookupSingle<StructDecl>(*Prelude->getDecl(), II);
   }

   return BoxDecl;
}

ClassDecl* SemaPass::getPromiseDecl()
{
   if (!PromiseDecl) {
      auto Async = getAsyncModule();
      if (!Async)
         return nullptr;

      auto *II = &Context.getIdentifiers().get("Promise");
      PromiseDecl = LookupSingle<ClassDecl>(*Async->getDecl(), II);
   }

   return PromiseDecl;
}

ClassDecl* SemaPass::getFutureDecl()
{
   if (!FutureDecl) {
      auto Async = getAsyncModule();
      if (!Async)
         return nullptr;

      auto *II = &Context.getIdentifiers().get("Future");
      FutureDecl = LookupSingle<ClassDecl>(*Async->getDecl(), II);
   }

   return FutureDecl;
}

StructDecl* SemaPass::getCoroutineHandleDecl()
{
   if (!CoroHandleDecl) {
      auto Async = getAsyncModule();
      if (!Async)
         return nullptr;

      auto *II = &Context.getIdentifiers().get("CoroutineHandle");
      CoroHandleDecl = LookupSingle<StructDecl>(*Async->getDecl(), II);
   }

   return CoroHandleDecl;
}

ProtocolDecl *SemaPass::getAnyDecl()
{
   if (!AnyDecl) {
      auto Prelude = getPreludeModule();
      if (!Prelude)
         return nullptr;
      
      auto *II = &Context.getIdentifiers().get("Any");
      AnyDecl = LookupSingle<ProtocolDecl>(*Prelude->getDecl(), II);
   }

   return AnyDecl;
}

ProtocolDecl *SemaPass::getEquatableDecl()
{
   if (!EquatableDecl) {
      auto Prelude = getPreludeModule();
      if (!Prelude)
         return nullptr;
      
      auto *II = &Context.getIdentifiers().get("Equatable");
      EquatableDecl = LookupSingle<ProtocolDecl>(*Prelude->getDecl(), II);
   }

   return EquatableDecl;
}

ProtocolDecl* SemaPass::getHashableDecl()
{
   if (!HashableDecl) {
      auto Prelude = getPreludeModule();
      if (!Prelude)
         return nullptr;
      
      auto *II = &Context.getIdentifiers().get("Hashable");
      HashableDecl = LookupSingle<ProtocolDecl>(*Prelude->getDecl(), II);
   }

   return HashableDecl;
}

ProtocolDecl *SemaPass::getCopyableDecl()
{
   if (!CopyableDecl) {
      auto Prelude = getPreludeModule();
      if (!Prelude)
         return nullptr;
      
      auto *II = &Context.getIdentifiers().get("Copyable");
      CopyableDecl = LookupSingle<ProtocolDecl>(*Prelude->getDecl(), II);
   }

   return CopyableDecl;
}

ProtocolDecl *SemaPass::getImplicitlyCopyableDecl()
{
   if (!ImplicitlyCopyableDecl) {
      auto Prelude = getPreludeModule();
      if (!Prelude)
         return nullptr;
      
      auto *II = &Context.getIdentifiers().get("ImplicitlyCopyable");
      ImplicitlyCopyableDecl = LookupSingle<ProtocolDecl>(*Prelude->getDecl(), II);
   }

   return ImplicitlyCopyableDecl;
}

ProtocolDecl *SemaPass::getStringRepresentableDecl()
{
   if (!StringRepresentableDecl) {
      auto Prelude = getPreludeModule();
      if (!Prelude)
         return nullptr;
      
      auto *II = &Context.getIdentifiers().get("StringRepresentable");
      StringRepresentableDecl = LookupSingle<ProtocolDecl>(*Prelude->getDecl(), II);
   }

   return StringRepresentableDecl;
}

ProtocolDecl *SemaPass::getPersistableDecl()
{
   if (!PersistableDecl) {
      auto Prelude = getPreludeModule();
      if (!Prelude)
         return nullptr;
      
      auto *II = &Context.getIdentifiers().get("Persistable");
      PersistableDecl = LookupSingle<ProtocolDecl>(*Prelude->getDecl(), II);
   }

   return PersistableDecl;
}

ProtocolDecl *SemaPass::getAwaitableDecl()
{
   if (!AwaitableDecl) {
      auto AsyncMod = getAsyncModule();
      if (!AsyncMod)
         return nullptr;

      auto *II = &Context.getIdentifiers().get("Awaitable");
      AwaitableDecl = LookupSingle<ProtocolDecl>(*AsyncMod->getDecl(), II);
   }

   return AwaitableDecl;
}

ProtocolDecl *SemaPass::getAwaiterDecl()
{
   if (!AwaiterDecl) {
      auto AsyncMod = getAsyncModule();
      if (!AsyncMod)
         return nullptr;

      auto *II = &Context.getIdentifiers().get("Awaiter");
      AwaiterDecl = LookupSingle<ProtocolDecl>(*AsyncMod->getDecl(), II);
   }

   return AwaiterDecl;
}

PrecedenceGroupDecl* SemaPass::getDefaultPrecedenceGroup()
{
   if (!DefaultPrecedenceGroup) {
      auto Prelude = getPreludeModule();
      if (!Prelude)
         return nullptr;
      
      auto *II = &Context.getIdentifiers().get("DefaultPrecedence");
      DefaultPrecedenceGroup = LookupSingle<PrecedenceGroupDecl>(
         *Prelude->getDecl(), II);
   }

   return DefaultPrecedenceGroup;
}

ProtocolDecl *SemaPass::getInitializableByDecl(InitializableByKind Kind)
{
   if (auto Decl = InitializableBy[(unsigned)Kind])
      return Decl;

   const IdentifierInfo *II;
   switch (Kind) {
   case InitializableByKind::Integer:
      II = &Context.getIdentifiers().get("ExpressibleByIntegerLiteral");
      break;
   case InitializableByKind::Float:
      II = &Context.getIdentifiers().get("ExpressibleByFloatingPointLiteral");
      break;
   case InitializableByKind::Bool:
      II = &Context.getIdentifiers().get("ExpressibleByBoolLiteral");
      break;
   case InitializableByKind::Char:
      II = &Context.getIdentifiers().get("ExpressibleByCharacterLiteral");
      break;
   case InitializableByKind::String:
      II = &Context.getIdentifiers().get("ExpressibleByStringLiteral");
      break;
   case InitializableByKind::Array:
      II = &Context.getIdentifiers().get("ExpressibleByArrayLiteral");
      break;
   }

   auto Prelude = getPreludeModule();
   if (!Prelude)
      return nullptr;
   
   auto Decl = LookupSingle<ProtocolDecl>(*Prelude->getDecl(), II);

   InitializableBy[(unsigned)Kind] = Decl;
   return Decl;
}

InitDecl *SemaPass::getStringInit()
{
   if (!StringInit) {
      auto S = getStringDecl();
      if (!S)
         return nullptr;

      ensureDeclared(S);

      // lookup the initializer to load the external lookup table if necessary
      DeclarationName DN = Context.getDeclNameTable()
                                  .getConstructorName(Context.getRecordType(S));

      auto Result = Lookup(*S, DN);
      if (Result.empty())
         return nullptr;

      StringInit = cast<InitDecl>(getBuiltinDecl("StringInit"));
   }

   return StringInit;
}

MethodDecl *SemaPass::getStringPlusEqualsString()
{
   if (!StringPlusEqualsString) {
      auto S = getStringDecl();
      if (!S)
         return nullptr;

      auto &II = Context.getIdentifiers().get("+=");
      DeclarationName DN = Context.getDeclNameTable().getInfixOperatorName(II);

      auto lookup = Lookup(*S, DN);
      if (lookup.empty())
         return nullptr;

      StringPlusEqualsString = cast<MethodDecl>(lookup.front());
   }

   return StringPlusEqualsString;
}

} // namespace ast
} // namespace cdot