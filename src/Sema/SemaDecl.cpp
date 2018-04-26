//
// Created by Jonas Zell on 08.09.17.
//

#include "Compiler.h"

#include "AST/AbstractPass.h"
#include "AST/Decl.h"
#include "AST/Transform.h"
#include "AST/TypeVisitor.h"
#include "Basic/FileManager.h"
#include "IL/Constants.h"
#include "ILGen/ILGenPass.h"
#include "Message/Diagnostics.h"
#include "Module/Module.h"
#include "Module/ModuleManager.h"
#include "Lex/Lexer.h"
#include "Parse/Parser.h"
#include "Sema/Builtin.h"
#include "SemaPass.h"
#include "Support/Casting.h"
#include "TemplateInstantiator.h"
#include "Template.h"
#include "Util.h"

#include <cassert>
#include <cstdlib>
#include <llvm/ADT/ScopeExit.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/StringSwitch.h>
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

   return llvm::StringSwitch<Type*>(typeName.getIdentifierInfo()
                                            ->getIdentifier())
      .Case("Void", Context.getVoidType())
      .Case("void", Context.getVoidType())
#     define CDOT_BUILTIN_INT(Name, BW, Unsigned)           \
      .Case(#Name, Context.get##Name##Ty())
#     define CDOT_BUILTIN_FP(Name, Precision)               \
      .Case(#Name, Context.get##Name##Ty())
#     include "Basic/BuiltinTypes.def"
      .Case("isize", Context.getIntTy())
      .Case("usize", Context.getUIntTy())
      .Default(nullptr);
}

void SemaPass::resolvePrecedenceGroups()
{
   auto Order = PrecedenceDependency.constructOrderedList();
   auto *TU = *compilationUnit->getGlobalDeclCtx()
                              .decl_begin<TranslationUnit>();

   if (!Order.second) {
      auto Pair = PrecedenceDependency.getOffendingPair();

      auto Fst = TU->lookupSingle<PrecedenceGroupDecl>(Pair.first);
      auto Snd = TU->lookupSingle<PrecedenceGroupDecl>(Pair.second);
      assert(Fst && Snd && "undeclared precedence group");

      diagnose(err_precedence_cycle, Pair.first->getIdentifier(),
               Pair.second->getIdentifier(), Fst->getSourceLoc());

      return;
   }

   for (auto &Ident : Order.first) {
      auto PG = TU->lookupSingle<PrecedenceGroupDecl>(Ident);
      assert(PG && "undeclared precedence group");

      declareScoped(PG);
   }
}

void SemaPass::visitDelayedDeclsAfterParsing()
{
   llvm::SmallVector<ExtensionDecl*, 8> Extensions;
   for (auto decl : DelayedDecls) {
      if (auto Ctx = dyn_cast<DeclContext>(decl)) {
         DeclScopeRAII declContextRAII(*this, Ctx);
         for (auto &P : cast<NamedDecl>(decl)->getTemplateParams())
            declareTemplateParamDecl(P);
      }

      if (auto Op = dyn_cast<OperatorDecl>(decl)) {
         declareScoped(Op);
      }

      if (auto Ext = dyn_cast<ExtensionDecl>(decl)) {
         Extensions.push_back(Ext);
      }
   }

   for (auto &Ext : Extensions) {
      DeclScopeRAII declContextRAII(*this, Ext);
      AllowIncompleteTemplateTypeRAII raii(*this);

      auto TypeRes = visitSourceType(Ext, Ext->getExtendedType());
      if (!TypeRes || !TypeRes.get()->isRecordType())
         continue;

      Ext->setName(getContext().getDeclNameTable()
                               .getExtensionName(TypeRes.get()));

      Ext->setExtendedRecord(TypeRes.get()->getRecord());
      Ext->getExtendedRecord()->addExtension(Ext);

      declareExtensionDecl(Ext);
   }

   DelayedDecls.clear();
}

NamedDecl* SemaPass::getCurrentDecl() const
{
   for (auto ctx = DeclCtx; ctx; ctx = ctx->getParentCtx())
      if (auto ND = dyn_cast<NamedDecl>(ctx))
         return ND;

   return nullptr;
}

RecordDecl* SemaPass::getCurrentRecordCtx()
{
   for (auto ctx = DeclCtx; ctx; ctx = ctx->getParentCtx())
      if (auto R = dyn_cast<RecordDecl>(ctx))
         return R;

   return nullptr;
}

void SemaPass::noteInstantiationContext()
{
   auto D = getCurrentDecl();
   while (D) {
      if (!D->isInstantiation()) {
         D = dyn_cast<NamedDecl>(D->getDeclContext());
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
             && !D->getInstantiatedWithin()->isInstantiation()) {
            auto Record = M->getRecord();
            diagnose(note_instantiation_of, Record->getInstantiatedFrom(),
                     Record->getSpecifierForDiagnostic(),
                     Record->getSpecializedTemplate()->getDeclName(), true,
                     Record->getTemplateArgs().toString('\0', '\0', true));
         }
      }

      D = D->getInstantiatedWithin();
   }
}

module::Module* SemaPass::getModuleBeingCompiled()
{
   return getCompilationUnit().getCompiledModule();
}

bool SemaPass::inGlobalDeclContext() const
{
   return getDeclContext().isGlobalDeclContext();
}

ast::NamedDecl* SemaPass::lookup(llvm::StringRef name, unsigned lvl) const
{
   assert(lvl > 0 && lvl <= 3 && "invalid lookup level");

//   auto res = getDeclContext().lookup(name);
//   if (res) {
//      assert(res.size() == 1);
//      return res.front();
//   }

//   for (auto M : getImportedModules()) {
//      auto &IT = M->getIdentifierTable();
//      auto lookupRes = IT.lookup(name,
//                                 (module::IdentifierTable::QueryLevel)lvl);
//
//      if (lookupRes)
//         return lookupRes;
//   }

   return nullptr;
}

RecordDecl* SemaPass::getRecord(llvm::StringRef name) const
{
   return lookup<RecordDecl>(name);
}

void SemaPass::ActOnRecordDecl(cdot::ast::RecordDecl *R)
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
   if (!P->getTemplateParams().empty()) {
      diagnose(P, err_protocol_template_params,
               P->getTemplateParams().front()->getSourceLoc());

      P->clearTemplateParams();
   }
}

void SemaPass::ActOnEnumDecl(EnumDecl *E)
{

}

void SemaPass::ActOnUnionDecl(UnionDecl *U)
{

}

void SemaPass::ActOnExtensionDecl(ExtensionDecl *E)
{
   DelayedDecls.push_back(E);
   addDeclToContext(getDeclContext(), E);
}

void SemaPass::ActOnOperatorDecl(OperatorDecl *Op)
{
   addDeclToContext(getDeclContext(), Op);

   if (!isa<TranslationUnit>(Op->getNonTransparentDeclContext())) {
      diagnose(Op, err_must_be_top_level, 1, Op->getSourceRange());
   }

   DelayedDecls.push_back(Op);
}

void SemaPass::ActOnPrecedenceGroupDecl(PrecedenceGroupDecl* PG)
{
   addDeclToContext(getDeclContext(), PG);

   if (!isa<TranslationUnit>(PG->getNonTransparentDeclContext())) {
      diagnose(PG, err_must_be_top_level, 0, PG->getSourceLoc());
   }
   else {
      auto &Vert = PrecedenceDependency.getOrAddVertex(PG->getIdentifierInfo());
      if (auto HigherThan = PG->getHigherThanIdent()) {
         auto &Dep = PrecedenceDependency.getOrAddVertex(HigherThan);
         Dep.addOutgoing(&Vert);
      }
   }
}

void SemaPass::ActOnTypedefDecl(TypedefDecl *TD)
{
   if (TD->isTemplate())
      DelayedDecls.push_back(TD);

   addDeclToContext(getDeclContext(), TD);
}

void SemaPass::ActOnAliasDecl(AliasDecl *alias)
{
   for (auto &P : alias->getTemplateParams())
      ActOnTemplateParamDecl(*alias, P);

   if (alias->isTemplate())
      DelayedDecls.push_back(alias);

   addDeclToContext(getDeclContext(), alias);
}

void SemaPass::ActOnFieldDecl(FieldDecl *F)
{
   addDeclToContext(getDeclContext(), F);

   // field accessor is not visible via normal lookup
   if (auto Acc = F->getAccessor())
      getDeclContext().addDecl((Decl*)Acc);
}

void SemaPass::ActOnFunctionDecl(FunctionDecl *F)
{
   bool isMain = F->getDeclName().isSimpleIdentifier()
                 && F->getDeclName().getIdentifierInfo()->isStr("main")
                 && isa<TranslationUnit>(getDeclContext());

   if (isMain) {
      F->setAccess(AccessSpecifier::Private);
      F->setExternC(true);
      F->setIsMain(true);
   }

   addDeclToContext(getDeclContext(), F);

   for (auto &P : F->getTemplateParams())
      ActOnTemplateParamDecl(*F, P);
}

void SemaPass::ActOnMethodDecl(MethodDecl *M)
{
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
   addDeclToContext(Ctx, P);
}

void SemaPass::ActOnMacroExpansionDecl(MacroExpansionDecl *Decl)
{
   // include!'s need to be handled up-front so their declarations are visible
   if (Decl->getMacroName().getMacroName()->isStr("include")) {
      parse::Parser::checkBuiltinMacro(*this, Decl->getMacroName(),
                                       Decl, Decl->getTokens(),
                                       parse::Parser::ExpansionKind::Decl);
   }
   else {
      addDeclToContext(getDeclContext(), Decl);
   }
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

   if (auto R = dyn_cast<RecordDecl>(ND->getNonTransparentDeclContext())) {
      switch (ND->getKind()) {
      case Decl::PropDeclID:
      case Decl::MethodDeclID:
      case Decl::InitDeclID:
      case Decl::EnumCaseDeclID:
         ND->setAccess(AccessSpecifier::Public);
         break;
      case Decl::FieldDeclID:
      case Decl::StructDeclID:
      case Decl::ClassDeclID:
      case Decl::EnumDeclID:
      case Decl::UnionDeclID:
      case Decl::ProtocolDeclID:
         if (isa<StructDecl>(R) && !isa<ClassDecl>(R)) {
            ND->setAccess(AccessSpecifier::Public);
         }
         else {
            ND->setAccess(AccessSpecifier::Private);
         }

         break;
      default:
         ND->setAccess(AccessSpecifier::Private);
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

TemplateParamDecl* SemaPass::getTemplateParam(DeclarationName name)
{
   return getDeclContext().lookupSingle<TemplateParamDecl>(name);
}

AssociatedTypeDecl* SemaPass::getAssociatedType(DeclarationName name)
{
   return getDeclContext().lookupSingle<AssociatedTypeDecl>(name);
}

bool SemaPass::declareDeclContext(DeclContext *Ctx)
{
   auto D = cast<Decl>(Ctx);

   // resolve static conditions line because they might contain decls that
   // are needed by others
   for (auto &decl : Ctx->getDecls()) {
      switch (decl->getKind()) {
      case Decl::StaticIfDeclID:
      case Decl::StaticForDeclID: {
         auto Res = declareStmt(D, decl);
         if (!Res) {
            return false;
         }

         break;
      }
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

      if (auto ND = dyn_cast<NamedDecl>(Curr)) {
         addDeclToContext(*To, ND);
      }
      else {
         To->addDecl(Curr);
      }

      Curr = Next;
   }

   From->clear();
}

DeclResult SemaPass::declareImportDecl(ImportDecl*)
{
//   auto M = ModuleManager::importModule(*this, node);
//   node->setModule(M);
//
//   return node;
//   ModuleImportsByFile[node->getSourceLoc().getSourceId()].push_back(M);
   llvm_unreachable("TODO!");
}

DeclResult SemaPass::declareModuleDecl(ModuleDecl *Decl)
{
   return Decl;
}

//StmtResult SemaPass::declareModule(module::Module &M)
//{
//   if (auto Stmt = M.getDeclRoot()) {
//      doInitialPass(Stmt);
//      doInitialPass();
//
//      visit(Stmt);
//   }
//
//   for (auto &Sub : M.getSubModules())
//      visitModule(*Sub);
//}

DeclResult SemaPass::declareCompoundDecl(CompoundDecl *D)
{
   DeclContextRAII declContextRAII(*this, D, SetParentContext);
   for (auto &decl : D->getDecls())
      declareStmt(D, decl);

   return D;
}

DeclResult SemaPass::declareNamespaceDecl(NamespaceDecl *NS)
{
   if (alreadyDeclared(NS))
      return NS;

   DeclContextRAII declContextRAII(*this, NS, SetParentContext);
   for (auto &D : NS->getDecls())
      declareStmt(NS, D);

   return NS;
}

DeclResult SemaPass::declareUsingDecl(UsingDecl *UD)
{
   NamedDecl *UsingTarget = nullptr;
   auto *declContext = &getDeclContext();

   size_t i = 0;
   size_t nameDepth = UD->getNestedImportName().size();

   for (auto &Name : UD->getNestedImportName()) {
      auto subDecl = declContext->lookup(Name);
      if (!subDecl) {
         diagnoseMemberNotFound(declContext, UD, Name);
         return UD;
      }

      if (subDecl.size() != 1) {
         diagnose(UD, err_using_target_ambiguous, UD->getSourceRange());

         auto it = subDecl.begin();
         diagnose(note_candidate_here, (*it++)->getSourceLoc());
         diagnose(note_candidate_here, (*it++)->getSourceLoc());

         return UD;
      }

      NamedDecl *ND = subDecl.front();

      declContext = dyn_cast<DeclContext>(subDecl.front());
      if (!declContext && i != nameDepth - 1) {
         diagnose(UD, err_cannot_lookup_member_in,
                  ND->getSpecifierForDiagnostic(), ND->getDeclName());

         return UD;
      }
      else {
         UsingTarget = ND;
      }

      ++i;
   }

   if (UD->isWildCardImport()) {
      makeDeclsAvailableIn(getDeclContext(), *declContext);
      return UD;
   }

   assert(UsingTarget && "should have been diagnosed!");
   auto Res = getDeclContext().makeDeclAvailable(UD->getDeclName(),
                                                 UsingTarget);

   diagnoseRedeclaration(*declContext, Res, UD->getDeclName(), UsingTarget);
   return UD;
}

DeclResult SemaPass::declareOperatorDecl(OperatorDecl *Decl)
{
   if (auto II = Decl->getPrecedenceGroupIdent()) {
      auto lookup = getDeclContext().lookupSingle<PrecedenceGroupDecl>(II);

      if (!lookup) {
         diagnose(Decl, err_precedence_group_does_not_exist,
                  Decl->getSourceLoc(),
                  II->getIdentifier());
      }
      else {
         Decl->setPrecedenceGroup(lookup);
      }

      if (Decl->getDeclName().getDeclaredOperatorName()
              .getDeclarationKind() == DeclarationName::InfixOperatorName) {
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
      auto lookup = getDeclContext().lookupSingle<PrecedenceGroupDecl>(
         HigherThan);

      if (!lookup) {
         diagnose(Decl, err_precedence_group_does_not_exist,
                  Decl->getSourceLoc(), HigherThan->getIdentifier());

         return DeclError();
      }

      Decl->setHigherThan(lookup);
   }
   if (auto LowerThan = Decl->getLowerThanIdent()) {
      auto lookup = getDeclContext().lookupSingle<PrecedenceGroupDecl>(
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

DeclResult SemaPass::declareCallableDecl(CallableDecl *F)
{
   // an error occured while parsing this function
   if (F->isInvalid())
      return DeclError();

   DeclContextRAII declContextRAII(*this, F, SetParentContext);
   checkDefaultAccessibility(F);

   if (F->isOperator()) {
      auto OpName = F->getDeclName();
      if (OpName.getDeclarationKind() == DeclarationName::InstantiationName)
         OpName = OpName.getInstantiationName();

      auto OpDeclName = Context.getDeclNameTable().getOperatorDeclName(OpName);
      auto Result = getDeclContext().lookupSingle<OperatorDecl>(OpDeclName);
      if (!Result) {
         diagnose(F, err_undeclared_operator, F->getSourceLoc(), OpName);
      }
      else {
         ensureDeclared(Result);
         F->setPrecedenceGroup(Result->getPrecedenceGroup());
      }
   }

   for (auto &TP : F->getTemplateParams())
      declareTemplateParamDecl(TP);

   for (auto &C : F->getConstraints())
      visitStaticExpr(C);

   for (const auto &arg : F->getArgs()) {
      if (!declareStmt(F, arg))
         return DeclError();
   }

   if (auto &Ret = F->getReturnType()) {
      auto Result = visitSourceType(F, Ret);
      if (!Result)
         return DeclError();

      auto retTy = Ret.getResolvedType();
      if (F->isMain()) {
         if (!retTy->isInt64Ty() && !retTy->isVoidType() && !retTy->isAutoType())
            diagnose(Ret.getTypeExpr(), warn_main_return_type);

         retTy = Context.getIntTy();
         Ret.setResolvedType(retTy);
      }
      else if (retTy->isAutoType()) {
         Ret.setResolvedType(Context.getVoidType());
      }

      F->setIsNoReturn(retTy->isUnpopulatedType());
   }

   F->createFunctionType(*this);
   checkDuplicateFunctionDeclaration(F, "function");

   return F;
}

DeclResult SemaPass::declareFunctionDecl(FunctionDecl *F)
{
   if (alreadyDeclared(F))
      return F;

   if (!declareCallableDecl(F))
      return DeclError();

   if (!F->isTemplate() && !F->isInvalid())
      ILGen->DeclareFunction(F);

   return F;
}

static
bool templateParamsEffectivelyEqual(llvm::ArrayRef<TemplateParamDecl*> P1,
                                    llvm::ArrayRef<TemplateParamDecl*> P2) {
   if (P1.size() != P2.size())
      return false;

   size_t i = 0;
   for (auto &p1 : P1) {
      auto p2 = P2[i];

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

      ++i;
   }

   return true;
}

void SemaPass::checkDuplicateFunctionDeclaration(CallableDecl *C,
                                                 llvm::StringRef fnKind) {
   for (auto &decl : C->getDeclContext()->lookup(C->getDeclName())) {
      auto Fn = dyn_cast<CallableDecl>(decl);
      if (Fn == C || !Fn)
         continue;

      if (VisitedDecls.find((uintptr_t)Fn) == VisitedDecls.end())
         return;

      if (!templateParamsEffectivelyEqual(Fn->getTemplateParams(),
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
      }

      if (!duplicate)
         continue;

      diagnose(C, err_redeclared_symbol, C->getName(), 0);
      diagnose(Fn, note_previous_decl);
   }
}

DeclResult SemaPass::declareFuncArgDecl(FuncArgDecl *Decl)
{
   auto res = visitSourceType(Decl, Decl->getType());
   if (!res)
      return DeclError();

   auto &declaredType = Decl->getType();

   // be careful not to get the canonical type here
   if (!Decl->isConst() && isa<ReferenceType>(declaredType)) {
      declaredType = Context.getMutableReferenceType(
         cast<ReferenceType>(declaredType)->getReferencedType());
   }
   else if (!Decl->isConst() && isa<PointerType>(declaredType)) {
      declaredType = Context.getMutablePointerType(
         cast<PointerType>(declaredType)->getPointeeType());
   }

   if (auto defaultVal = Decl->getDefaultVal()) {
      DefaultArgumentValueRAII defaultArgumentValueRAII(*this);

      auto Result = getAsOrCast(Decl, defaultVal, declaredType).get();
      Decl->setDefaultVal(Result);
   }

   return Decl;
}

DeclResult SemaPass::declareTemplateParamDecl(TemplateParamDecl *P)
{
   if (alreadyDeclared(P) || P->isSynthesized())
      return P;

   if (auto Param = getDeclContext().getParentCtx()
                                    ->lookupSingle<TemplateParamDecl>(
                                       P->getDeclName())) {
      diagnose(P, err_template_param_shadow, P->getName(), P->getSourceRange());
      diagnose(note_template_parameter_here, Param->getSourceRange());

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
   if (alreadyDeclared(Decl))
      return Decl;

   EnterGlobalVarScope GVS(*this, Decl);
   checkDefaultAccessibility(Decl);

   if (auto ty = Decl->getType()) {
      auto res = visitSourceType(Decl, ty);
      if (!res)
         return Decl;
   }
   else {
      DelayedDecls.push_back(Decl);
   }

   return Decl;
}

StmtResult
SemaPass::declareGlobalDestructuringDecl(GlobalDestructuringDecl *Decl)
{
   if (auto ty = Decl->getType()) {
      auto res = visitSourceType(Decl, ty);
      if (!res)
         return Decl;

      auto declTy = ty.getResolvedType();
      if (!declTy->isTupleType()) {
         diagnose(Decl, err_generic_error,
                  "destructured declaration must have tuple type");

         return Decl;
      }

      auto containedTypes = declTy->asTupleType()->getContainedTypes();
      auto numContainedTys = containedTypes.size();
      size_t i = 0;

      for (auto decl : Decl->getDecls()) {
         if (numContainedTys < i) {
            diagnose(decl, err_generic_error,
                     "expected " + std::to_string(i) + " types, found "
                     + std::to_string(numContainedTys));

            break;
         }
      }
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
      auto Proto = protoTy->getRecord();
      if (Proto->isTemplate())
         break;

      if (!isa<ProtocolDecl>(Proto)) {
         diagnose(prot.getTypeExpr(),
                  err_conforming_to_non_protocol, Proto->getName());

         continue;
      }

      auto PD = cast<ProtocolDecl>(Proto);
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

void SemaPass::registerImplicitAndInheritedConformances(RecordDecl *Rec)
{
   auto &ConfTable = Context.getConformanceTable();
   auto ExplicitConformances = ConfTable.getExplicitConformances(Rec);

   for (auto Conf : ExplicitConformances) {
      for (auto PConf : ConfTable.getAllConformances(Conf))
         ConfTable.addImplicitConformance(Context, Rec, PConf->getProto());
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
   Rec->setOpaque(Rec->hasAttribute<OpaqueAttr>());

   registerExplicitConformances(Rec);

   for (auto &C : Rec->getConstraints())
      visitStaticExpr(C);

   if (!declareDeclContext(Rec))
      return DeclError();

   return Rec;
}

DeclResult SemaPass::declareStructDecl(StructDecl *S)
{
   DeclContextRAII declContextRAII(*this, S, SetParentContext);

   if (!isa<ClassDecl>(S) && alreadyDeclared(S))
      return S;

   if (!declareRecordDecl(S))
      return DeclError();

   declareDefaultInitializer(S);

   if (!S->isClass())
      declareMemberwiseInitializer(S);

   if (!S->getDeinitializer())
      declareDefaultDeinitializer(S);

   return S;
}

DeclResult SemaPass::declareClassDecl(ClassDecl *C)
{
   if (alreadyDeclared(C))
      return C;

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
            C->inherit(cast<ClassDecl>(ParentClass));
            makeDeclsAvailableIn(*C, *ParentClass);

            // base class size needs to be calculated before sub classes
            auto &Vert = LayoutDependency.getOrAddVertex(C);
            auto &BaseVert = LayoutDependency.getOrAddVertex(ParentClass);

            BaseVert.addOutgoing(&Vert);
         }
      }
   }

   return declareStructDecl(C);
}

void SemaPass::declareMemberwiseInitializer(StructDecl *S)
{
   llvm::SmallString<32> argName("arg");
   std::vector<FuncArgDecl*> args;

   size_t i = 0;
   for (auto F : S->getFields()) {
      if (!F->isStatic() && !F->getDefaultVal()) {
         auto res = visitSourceType(S, F->getType());
         if (!res) return;

         argName += i - '1';

         auto Name = &Context.getIdentifiers().get(argName);
         auto arg = FuncArgDecl::Create(Context, S->getSourceLoc(),
                                        S->getSourceLoc(),
                                        Name, F->getType(),
                                        nullptr, F->isVariadic(),
                                        true, false);

         ++i;
         argName.pop_back();
         args.push_back(arg);
      }
   }

   auto MDecl = InitDecl::Create(Context, AccessSpecifier::Public,
                                 S->getSourceLoc(), args, {}, nullptr);

   addDeclToContext(*S, MDecl);
   S->setMemberwiseInitializer(MDecl);

   MDecl->setSynthesized(true);
   MDecl->setMemberwiseInitializer(true);

   if (S->isInstantiation()) {
      MDecl->setMethodID(cast<StructDecl>(S->getSpecializedTemplate())
         ->getMemberwiseInitializer()->getMethodID());
   }

   declareInitDecl(MDecl);
}

void SemaPass::declareDefaultInitializer(StructDecl *S)
{
   SourceType RetTy(Context.getVoidType());
   auto Name = &Context.getIdentifiers().get("__default_init");
   auto Decl = MethodDecl::Create(Context, AccessSpecifier::Private,
                                  S->getSourceLoc(), Name, RetTy, {},
                                  {}, nullptr, false);

   addDeclToContext(*S, Decl);
   S->setDefaultInitializer(Decl);

   Decl->setSynthesized(true);
   Decl->setDefaultInitializer(true);

   if (S->isInstantiation()) {
      Decl->setMethodID(cast<StructDecl>(S->getSpecializedTemplate())
                           ->getDefaultInitializer()->getMethodID());
   }

   declareMethodDecl(Decl);
}

void SemaPass::declareDefaultDeinitializer(RecordDecl *R)
{
   auto DDecl = DeinitDecl::Create(Context, R->getSourceLoc(), nullptr);
   addDeclToContext(*R, DDecl);

   DDecl->setSynthesized(true);
   DDecl->setReturnType(SourceType(Context.getVoidType()));

   if (R->isInstantiation()) {
      DDecl->setMethodID(R->getSpecializedTemplate()
                          ->getDeinitializer()->getMethodID());
   }

   declareDeinitDecl(DDecl);
}

DeclResult SemaPass::declareProtocolDecl(ProtocolDecl *P)
{
   if (alreadyDeclared(P))
      return P;

   DeclContextRAII declContextRAII(*this, P, SetParentContext);
   if (!declareRecordDecl(P))
      return DeclError();

   if (P == getAnyDecl())
      P->setIsAny(true);

   return P;
}

DeclResult SemaPass::declareExtensionDecl(ExtensionDecl *Ext)
{
   if (alreadyDeclared(Ext) || Ext->isInvalid() || !Ext->getExtendedRecord())
      return Ext;

   auto Res = checkConstraints(Ext, Ext->getExtendedRecord());
   if (Res.getFailedConstraint() || Res.isDependent())
      return Ext;

   DeclContextRAII outerDeclContextRAII(*this, Ext->getExtendedRecord());
   registerExplicitConformances(Ext->getExtendedRecord(),
                                Ext->getConformanceTypes());

   if (Ext->getExtendedRecord()->isTemplate()) {
      makeDeclsAvailableIn(*Ext->getExtendedRecord(), *Ext);
   }
   else {
      transferDecls(Ext, Ext->getExtendedRecord());
   }

   if (!declareDeclContext(Ext))
      return DeclError();

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
   if (alreadyDeclared(E))
      return E;

   DeclContextRAII declContextRAII(*this, E, SetParentContext);
   auto &ConfTable = Context.getConformanceTable();

   // all enums are implicitly equatable and copyable
   if (auto Equatable = getEquatableDecl()) {
      ConfTable.addExplicitConformance(Context, E, Equatable);
   }
   if (auto Copyable = getCopyableDecl()) {
      ConfTable.addExplicitConformance(Context, E, Copyable);
   }

   if (!E->getRawType()) {
      auto rawTy = SourceType(Context.getIntTy());
      E->setRawType(rawTy);
   }
   else {
      auto res = visitSourceType(E, E->getRawType());
      if (!res)
         return DeclError();
   }

   long long last;
   bool first = true;
   std::unordered_map<long long, EnumCaseDecl *> caseVals;

   for (const auto &Case : E->getCases()) {
      DeclaredDecls.insert((uintptr_t)Case);
      checkDefaultAccessibility(Case);

      auto &EnumVert = LayoutDependency.getOrAddVertex(E);
      auto &CaseVert = LayoutDependency.getOrAddVertex(Case);

      // every enum is dependent on its cases
      EnumVert.addIncoming(&CaseVert);

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
      }

      if (auto expr = Case->getRawValExpr()) {
         expr->setContextualType(E->getRawType().getResolvedType());

         auto res = evalStaticExpr(E, expr);
         if (!res)
            continue;

         if (!implicitlyCastableTo(expr->getExprType(),
                                   E->getRawType().getResolvedType())) {
            diagnose(Case->getRawValExpr(), err_generic_error,
                     "enum case value must be of the same type as the "
                        "underlying enum type");
         }

         Case->setRawValue(cast<il::ConstantInt>(res.getValue())
            ->getSExtValue());

         last = Case->getRawValue();
      }
      else {
         if (first) {
            last = 0;
         }
         else {
            ++last;
         }

         Case->setRawValue(last);
      }

      auto it = caseVals.find(last);
      if (it != caseVals.end()) {
         diagnose(Case, err_generic_error,
                  "duplicate case value " + std::to_string(last));
         diagnose(it->second, note_duplicate_case);
      }

      Case->setRawValue(last);

      caseVals.emplace(last, Case);
      first = false;

      Case->createFunctionType(*this);
   }

   if (!declareRecordDecl(E))
      return DeclError();

   if (!E->getDeinitializer() && !E->isRawEnum())
      declareDefaultDeinitializer(E);

   return E;
}

DeclResult SemaPass::declareUnionDecl(UnionDecl *U)
{
   if (alreadyDeclared(U))
      return U;

   DeclContextRAII declContextRAII(*this, U, SetParentContext);
   return declareRecordDecl(U);
}

DeclResult SemaPass::declareFieldDecl(FieldDecl *F)
{
   if (alreadyDeclared(F))
      return F;

   auto R = F->getRecord();
   checkDefaultAccessibility(F);

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

      auto typeRes = getAsOrCast(F, defaultVal, fieldType);
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

      declarePropDecl(Prop);
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
   if (alreadyDeclared(Decl))
      return Decl;

   auto R = Decl->getRecord();
   checkDefaultAccessibility(Decl);

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
                                       Decl->getSourceLoc(), DN, ty, {},
                                       {}, Decl->getGetterBody(),
                                       Decl->isStatic());

      Getter->setSynthesized(true);
      Getter->setProperty(true);

      if (auto Template = Decl->getPropTemplate())
         Getter->setBodyTemplate(Template->getGetterMethod());

      addDeclToContext(*R, Getter);
      Decl->setGetterMethod(Getter);

      declareMethodDecl(Getter);
   }

   if (Decl->hasSetter()) {
      auto argType = SourceType(propType);
      auto arg = FuncArgDecl::Create(Context, Decl->getSourceLoc(),
                                     Decl->getSourceLoc(),
                                     Decl->getNewValNameInfo(), argType,
                                     nullptr, false, true);

      DeclarationName DN =
         Context.getDeclNameTable().getAccessorName(*Decl->getIdentifierInfo(),
                                                    DeclarationName::Setter);

      auto retTy = SourceType(Context.getVoidType());
      auto Setter = MethodDecl::Create(Context, Decl->getSetterAccess(),
                                       Decl->getSourceLoc(), DN, retTy,
                                       { arg }, {}, Decl->getSetterBody(),
                                       Decl->isStatic());

      Setter->setSynthesized(true);
      Setter->setProperty(true);

      if (!isa<ClassDecl>(R))
         Setter->setMutating(true);

      if (auto Template = Decl->getPropTemplate())
         Setter->setBodyTemplate(Template->getSetterMethod());

      addDeclToContext(*R, Setter);
      Decl->setSetterMethod(Setter);

      declareMethodDecl(Setter);
   }

   return Decl;
}

DeclResult SemaPass::declareAssociatedTypeDecl(AssociatedTypeDecl *Decl)
{
   if (alreadyDeclared(Decl))
      return Decl;

   checkDefaultAccessibility(Decl);

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
   if (alreadyDeclared(M))
      return M;

   // check virtual and override methods after all signatures (most
   // importantly those of base classes) are available
   DelayedDecls.push_back(M);

   if (!declareCallableDecl(M))
      return DeclError();

   QualType SelfType = Context.getRecordType(M->getRecord());
   if (M->hasMutableSelf()) {
      if (isa<ClassDecl>(M->getRecord())) {
         diagnose(M, err_mutating_on_class, M->getSourceLoc());
         M->setMutating(false);
      }
      else {
         SelfType = Context.getMutableReferenceType(SelfType);
      }
   }

   M->setSelfType(SelfType);
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
   if (alreadyDeclared(Init))
      return Init;

   QualType RecordTy = Context.getRecordType(Init->getRecord());
   Init->setReturnType(SourceType(RecordTy));

   if (!Init->getDeclName()) {
      auto DeclName = Context.getDeclNameTable().getConstructorName(RecordTy);

      Init->setName(DeclName);
      makeDeclAvailable(*Init->getRecord(), DeclName, Init);
   }

   auto R = Init->getRecord();
   if (Init->isMemberwise()) {
      if (auto Str = dyn_cast<StructDecl>(R)) {
         declareMemberwiseInitializer(Str);
         return Init;
      }

      llvm_unreachable("memberwise init should have been rejected");
   }

   if (Init->isDefaultInitializer())
      cast<StructDecl>(R)->setDefaultInitializer(Init);

   if (Init->isMemberwiseInitializer())
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

      auto BaseInit = InitDecl::Create(Context, Init->getAccess(),
                                       Init->getSourceLoc(), Init->getArgs(),
                                       Init->getTemplateParams().vec(),
                                       Init->getBody(), BaseInitName);

      addDeclToContext(getDeclContext(), BaseInit);
      declareInitDecl(BaseInit);

      Init->setBaseInit(BaseInit);

      BaseInit->setSynthesized(true);
      BaseInit->setCompleteInit(Init);
      BaseInit->setBodyTemplate(Init->getBodyTemplate());
   }

   return Init;
}

DeclResult SemaPass::declareDeinitDecl(DeinitDecl *Deinit)
{
   if (alreadyDeclared(Deinit))
      return Deinit;

   Deinit->setReturnType(SourceType(Context.getVoidType()));

   if (!Deinit->getDeclName()) {
      QualType RecordTy = Context.getRecordType(Deinit->getRecord());
      auto DeclName = Context.getDeclNameTable().getDestructorName(RecordTy);

      Deinit->setName(DeclName);
      makeDeclAvailable(*Deinit->getRecord(), DeclName, Deinit);
   }

   return declareCallableDecl(Deinit);
}

DeclResult SemaPass::declareTypedefDecl(TypedefDecl *Decl)
{
   if (alreadyDeclared(Decl))
      return Decl;

   checkDefaultAccessibility(Decl);

   if (!visitSourceType(Decl, Decl->getOriginTy()))
      return DeclError();

   if (auto M = getModuleBeingCompiled())
      if (Decl->isExported())
         M->addDecl(Decl);

   // update the TypedefType associated with this declaration
   Context.getTypedefType(Decl)->setCanonicalType(Decl->getOriginTy());

   return Decl;
}

DeclResult SemaPass::declareAliasDecl(AliasDecl *Decl)
{
   if (alreadyDeclared(Decl))
      return Decl;

   checkDefaultAccessibility(Decl);

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
   if (!Ty.getTypeExpr()) {
      assert(Ty.getResolvedType() && "source ty with no expr or resolved type");
      return Ty.getResolvedType();
   }

   auto Result = visitExpr(Ty.getTypeExpr());
   if (!Result) {
      Ty.setResolvedType(UnknownAnyTy);
      return TypeError();
   }

   QualType ResTy = Result.get()->getExprType();

   // check if we were given an expression instead of a type
   if (Ty.getTypeExpr() && !isa<TypeExpr>(Ty.getTypeExpr())) {
      if (!ResTy->isMetaType() && !ResTy->isUnknownAnyType()) {
         diagnose(Ty.getTypeExpr(), err_expression_in_type_position,
                  Ty.getTypeExpr()->getSourceRange());
      }
   }

   if (auto TE = dyn_cast_or_null<TypeExpr>(Ty.getTypeExpr())) {
      WantMeta |= TE->isMeta();
   }

   if (!WantMeta && ResTy->isMetaType())
      ResTy = cast<cdot::MetaType>(ResTy)->getUnderlyingType();

   Ty.setResolvedType(ResTy);
   return Ty.getResolvedType();
}

ExprResult SemaPass::visitTupleTypeExpr(TupleTypeExpr *Expr)
{
   llvm::SmallVector<QualType, 8> Tys;
   for (auto &Ty : Expr->getContainedTypes()) {
      auto Res = visitSourceType(Expr, Ty);
      if (!Res)
         continue;

      Tys.push_back(Res.get());
   }

   Expr->setExprType(Context.getMetaType(Context.getTupleType(Tys)));
   return Expr;
}

ExprResult SemaPass::visitFunctionTypeExpr(FunctionTypeExpr *Expr)
{
   llvm::SmallVector<QualType, 8> ArgTys;
   for (auto &Ty : Expr->getArgTypes()) {
      auto Res = visitSourceType(Expr, Ty);
      if (!Res)
         continue;

      ArgTys.push_back(Res.get());
   }

   auto RetTyResult = visitSourceType(Expr, Expr->getReturnType());
   QualType RetTy = RetTyResult ? RetTyResult.get() : UnknownAnyTy;

   if (Expr->isThin())
      Expr->setExprType(Context.getFunctionType(RetTy, ArgTys));
   else
      Expr->setExprType(Context.getLambdaType(RetTy, ArgTys));

   Expr->setExprType(Context.getMetaType(Expr->getExprType()));

   return Expr;
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
   return Expr;
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
   return Expr;
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
   return Expr;
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
   return Expr;
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
   return Expr;
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
   if (alreadyDeclared(Stmt))
      return Stmt;

   auto Res = visitExpr(Stmt, Stmt->getCondition());
   if (!Res)
      return DeclError();

   if (Stmt->getCondition()->isDependent()) {
      if (currentScope)
         currentScope->setHasUnresolvedStaticCond(true);

      Stmt->setContinuationPoint(ContinuationPoint(currentScope,
                                                   &getDeclContext()));

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

      {
         const ContinuationPoint &CP = Template->getContinuationPoint();
         DeclScopeRAII declScopeRAII(*this, CP.Ctx);
         ScopeResetRAII scopeResetRAII(*this, CP.S);

         if (CondIsTrue) {
            // make sure to visit the appropriate branch before instantiating
            auto TrueResult = declareStmt(Stmt, Template->getIfDecl());
            if (!TrueResult)
               return DeclError();

            Template->setIfDecl(cast<CompoundDecl>(TrueResult.get()));
         }
         else if (auto Else = Template->getElseDecl()) {
            auto FalseResult = declareStmt(Stmt, Else);
            if (!FalseResult)
               return DeclError();

            Template->setElseDecl(cast<CompoundDecl>(FalseResult.get()));
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

   if (auto Else = Stmt->getElseDecl())
      transferDecls(Else, &getDeclContext());

   return Stmt;
}

DeclResult SemaPass::declareStaticForDecl(StaticForDecl *Stmt)
{
   if (alreadyVisited(Stmt))
      return Stmt;

   auto Res = visitExpr(Stmt, Stmt->getRange());
   if (!Res)
      return DeclError();

   return Stmt;
}

DeclResult SemaPass::declareTranslationUnit(TranslationUnit *Stmt)
{
   if (alreadyVisited(Stmt))
      return Stmt;

   return Stmt;
}

DeclResult SemaPass::declareEnumCaseDecl(EnumCaseDecl *Stmt)
{
   if (alreadyVisited(Stmt))
      return Stmt;

   return Stmt;
}

DeclResult SemaPass::declareMacroExpansionDecl(MacroExpansionDecl *Decl)
{
   auto Result = checkMacroCommon(Decl, Decl->getMacroName(), getDeclContext(),
                                  (MacroDecl::Delimiter )Decl->getDelim(),
                                  Decl->getTokens(),
                                  (unsigned)parse::Parser::ExpansionKind::Decl);

   if (!Result)
      return DeclError();

   if (!Result.asDecl()) {
      diagnose(Decl, err_macro_must_form, 3, Decl->getSourceLoc());
      return DeclError();
   }

   return Result.getDecl();
}

ExprResult SemaPass::visitMacroExpansionExpr(MacroExpansionExpr *Expr)
{
   auto Result = checkMacroCommon(Expr, Expr->getMacroName(), getDeclContext(),
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
   auto Result = checkMacroCommon(Stmt, Stmt->getMacroName(), getDeclContext(),
                                  (MacroDecl::Delimiter )Stmt->getDelim(),
                                  Stmt->getTokens(),
                                  (unsigned)parse::Parser::ExpansionKind::Stmt);

   if (!Result)
      return StmtError();

   if (!Result.asStatement()) {
      diagnose(Stmt, err_macro_must_form, 1, Stmt->getSourceLoc());
      return StmtError();
   }

   return visitStmt(Stmt, Result.getStatement());
}

StmtOrDecl SemaPass::checkMacroCommon(StmtOrDecl SOD,
                                      DeclarationName MacroName,
                                      DeclContext &Ctx,
                                      MacroDecl::Delimiter Delim,
                                      llvm::ArrayRef<lex::Token> Tokens,
                                      unsigned Kind) {
   auto Macro = Ctx.lookupSingle<MacroDecl>(MacroName);
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

   SOD.setIsInvalid(true);
   return nullptr;
}

ClassDecl* SemaPass::getArrayDecl()
{
   if (!ArrayDecl) {
      auto TU = getDeclContext().getTranslationUnit();
      static auto *II = &Context.getIdentifiers().get("Array");
      ArrayDecl = TU->lookupSingle<ClassDecl>(II);
   }

   return ArrayDecl;
}

StructDecl* SemaPass::getArrayViewDecl()
{
   if (!ArrayViewDecl) {
      auto TU = getDeclContext().getTranslationUnit();
      static auto *II = &Context.getIdentifiers().get("ArrayView");
      ArrayViewDecl = TU->lookupSingle<StructDecl>(II);
   }

   return ArrayViewDecl;
}

ClassDecl* SemaPass::getDictionaryDecl()
{
   if (!DictionaryDecl) {
      auto TU = getDeclContext().getTranslationUnit();
      static auto *II = &Context.getIdentifiers().get("Dictionary");
      DictionaryDecl = TU->lookupSingle<ClassDecl>(II);
   }

   return DictionaryDecl;
}

EnumDecl* SemaPass::getOptionDecl()
{
   if (!OptionDecl) {
      auto TU = getDeclContext().getTranslationUnit();
      auto *II = &Context.getIdentifiers().get("Option");
      OptionDecl = TU->lookupSingle<EnumDecl>(II);
   }

   return OptionDecl;
}

ClassDecl* SemaPass::getStringDecl()
{
   if (!StringDecl) {
      auto TU = getDeclContext().getTranslationUnit();
      auto *II = &Context.getIdentifiers().get("String");
      StringDecl = TU->lookupSingle<ClassDecl>(II);
   }

   return StringDecl;
}

StructDecl* SemaPass::getStringViewDecl()
{
   if (!StringViewDecl) {
      auto TU = getDeclContext().getTranslationUnit();
      auto *II = &Context.getIdentifiers().get("StringView");
      StringViewDecl = TU->lookupSingle<StructDecl>(II);
   }

   return StringViewDecl;
}

StructDecl* SemaPass::getTypeInfoDecl()
{
   if (!TypeInfoDecl) {
      auto TU = getDeclContext().getTranslationUnit();
      static auto *II = &Context.getIdentifiers().get("TypeInfo");
      TypeInfoDecl = TU->lookupSingle<StructDecl>(II);
   }

   return TypeInfoDecl;
}

ProtocolDecl *SemaPass::getAnyDecl()
{
   if (!AnyDecl) {
      auto TU = getDeclContext().getTranslationUnit();
      auto *II = &Context.getIdentifiers().get("Any");
      AnyDecl = TU->lookupSingle<ProtocolDecl>(II);
   }

   return AnyDecl;
}

ProtocolDecl *SemaPass::getEquatableDecl()
{
   if (!EquatableDecl) {
      auto TU = getDeclContext().getTranslationUnit();
      auto *II = &Context.getIdentifiers().get("Equatable");
      EquatableDecl = TU->lookupSingle<ProtocolDecl>(II);
   }

   return EquatableDecl;
}

ProtocolDecl* SemaPass::getHashableDecl()
{
   if (!HashableDecl) {
      auto TU = getDeclContext().getTranslationUnit();
      auto *II = &Context.getIdentifiers().get("Hashable");
      HashableDecl = TU->lookupSingle<ProtocolDecl>(II);
   }

   return HashableDecl;
}

ProtocolDecl *SemaPass::getCopyableDecl()
{
   if (!CopyableDecl) {
      auto TU = getDeclContext().getTranslationUnit();
      auto *II = &Context.getIdentifiers().get("Copyable");
      CopyableDecl = TU->lookupSingle<ProtocolDecl>(II);
   }

   return CopyableDecl;
}

ProtocolDecl *SemaPass::getStringRepresentableDecl()
{
   if (!StringRepresentableDecl) {
      auto TU = getDeclContext().getTranslationUnit();
      auto *II = &Context.getIdentifiers().get("StringRepresentable");
      StringRepresentableDecl = TU->lookupSingle<ProtocolDecl>(II);
   }

   return StringRepresentableDecl;
}

ProtocolDecl *SemaPass::getPersistableDecl()
{
   if (!PersistableDecl) {
      auto TU = getDeclContext().getTranslationUnit();
      auto *II = &Context.getIdentifiers().get("Persistable");
      PersistableDecl = TU->lookupSingle<ProtocolDecl>(II);
   }

   return PersistableDecl;
}

PrecedenceGroupDecl* SemaPass::getDefaultPrecedenceGroup()
{
   if (!DefaultPrecedenceGroup) {
      auto TU = getDeclContext().getTranslationUnit();
      auto *II = &Context.getIdentifiers().get("DefaultPrecedence");
      DefaultPrecedenceGroup = TU->lookupSingle<PrecedenceGroupDecl>(II);
   }

   return DefaultPrecedenceGroup;
}

InitDecl *SemaPass::getStringInit()
{
   if (!StringInit) {
      auto S = getStringDecl();
      if (!S)
         return nullptr;

      StringInit = *S->decl_begin<InitDecl>();
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

      auto lookup = S->lookup(DN);
      if (lookup.empty())
         return nullptr;

      StringPlusEqualsString = cast<MethodDecl>(lookup.front());
   }

   return StringPlusEqualsString;
}

} // namespace ast
} // namespace cdot