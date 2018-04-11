//
// Created by Jonas Zell on 08.09.17.
//

#include "Compiler.h"

#include "AST/Decl.h"
#include "AST/Transform.h"
#include "AST/TypeVisitor.h"
#include "Sema/Builtin.h"
#include "SemaPass.h"
#include "TemplateInstantiator.h"
#include "Template.h"
#include "ILGen/ILGenPass.h"
#include "Basic/FileManager.h"
#include "Lex/Lexer.h"
#include "Message/Diagnostics.h"
#include "Module/Module.h"
#include "Module/ModuleManager.h"
#include "Parse/Parser.h"
#include "Support/Casting.h"
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
      .Case("size", Context.getIntTy())
      .Case("usize", Context.getUIntTy())
      .Default(nullptr);
}

void SemaPass::visitDelayedDeclsAfterParsing()
{
   llvm::SmallVector<ExtensionDecl*, 8> Extensions;
   for (auto decl : DelayedDecls) {
      if (auto Ctx = dyn_cast<DeclContext>(decl)) {
         DeclScopeRAII declContextRAII(*this, Ctx);
         for (auto &P : decl->getTemplateParams())
            declareTemplateParamDecl(P);
      }

      if (auto Ext = dyn_cast<ExtensionDecl>(decl)) {
         Extensions.push_back(Ext);
      }
   }

   for (auto &Ext : Extensions) {
      DeclScopeRAII declContextRAII(*this, Ext);
      AllowIncompleteTemplateTypeRAII raii(*this);

      auto TypeRes = visitSourceType(Ext, Ext->getExtendedType());
      if (!TypeRes)
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
      case Decl::ModuleDeclID:{
         auto Res = declareStmt(D, decl);
         if (Res && Res.get() != decl) {
            Ctx->replaceDecl(decl, Res.get());
         }

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
      case Decl::StaticIfDeclID:
      case Decl::StaticForDeclID:
         break;
      default: {
         auto Res = declareStmt(D, decl);
         if (Res && Res.get() != decl) {
            Ctx->replaceDecl(decl, Res.get());
         }

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

DeclResult SemaPass::declareCallableDecl(CallableDecl *F)
{
   // an error occured while parsing this function
   if (F->isInvalid())
      return DeclError();

   DeclContextRAII declContextRAII(*this, F, SetParentContext);
   checkDefaultAccessibility(F);

   for (auto &TP : F->getTemplateParams())
      declareTemplateParamDecl(TP);

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

   if (P->getDefaultValue())
      registerTemplateParamWithDefaultVal(P);

   return P;
}

DeclResult SemaPass::declareGlobalVarDecl(GlobalVarDecl *Decl)
{
   if (alreadyDeclared(Decl))
      return Decl;

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

static void addConformance(SemaPass &SP, RecordDecl *Rec, ProtocolDecl *P)
{
   SP.ensureDeclared(P);
   Rec->addConformance(P);

   for (auto Conf : P->getConformances()) {
      addConformance(SP, Rec, Conf);
   }
}

DeclResult SemaPass::declareRecordDecl(RecordDecl *Rec)
{
   DeclPrettyStackTraceEntry STE(Rec);
   Rec->setOpaque(Rec->hasAttribute<OpaqueAttr>());

   auto Any = getAnyDecl();
   if (Any && Rec != Any) {
      Rec->addConformance(Any);
   }

   for (const auto &prot : Rec->getConformanceTypes()) {
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

      addConformance(*this, Rec, cast<ProtocolDecl>(Proto));
   }

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
   return declareRecordDecl(P);
}

DeclResult SemaPass::declareExtensionDecl(ExtensionDecl *Ext)
{
   if (alreadyDeclared(Ext) || Ext->isInvalid())
      return Ext;

   auto Res = checkConstraints(Ext->getSourceLoc(), Ext->getExtendedRecord());
   if (Res.getFailedConstraint() || Res.isDependent())
      return Ext;

   DeclContextRAII outerDeclContextRAII(*this, Ext->getExtendedRecord());
   for (const auto &prot : Ext->getConformanceTypes()) {
      auto res = visitSourceType(Ext, prot);
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

      addConformance(*this, Ext->getExtendedRecord(),
                     cast<ProtocolDecl>(Proto));
   }

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

DeclResult SemaPass::declareEnumDecl(EnumDecl *E)
{
   if (alreadyDeclared(E))
      return E;

   DeclContextRAII declContextRAII(*this, E, SetParentContext);

   // all enums are implicitly equatable
//   if (auto Equatable = getEquatableDecl()) {
//      E->addConformance(Equatable);
//      addImplicitConformance(E, ImplicitConformanceKind::Equatable);
//   }

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

      for (const auto &assoc : Case->getArgs()) {
         auto res = visitSourceType(assoc, assoc->getType());
         if (!res)
            return E;

         if (!res.get()->isRecordType())
            continue;

         auto &Vert = LayoutDependency.getOrAddVertex(Case->getRecord());
         auto &Dep  = LayoutDependency.getOrAddVertex(res.get()->getRecord());

         Dep.addOutgoing(&Vert);
      }

      if (auto expr = Case->getRawValExpr()) {
         expr->setContextualType(E->getRawType().getResolvedType());

         auto res = evalStaticExpr(expr);
         if (!res)
            continue;

         if (!implicitlyCastableTo(expr->getExprType(),
                                   E->getRawType().getResolvedType())) {
            diagnose(Case->getRawValExpr(), err_generic_error,
                     "enum case value must be of the same type as the "
                        "underlying enum type");
         }

         Case->setRawValue(res.getValue().getSExtValue());
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
         F->setDefaultVal(typeRes.get());

         auto givenType = typeRes.get()->getExprType();
         if (fieldType->isAutoType()) {
            fieldType.setResolvedType(givenType);

            F->getType().setResolvedType(givenType);
            R->getField(F->getDeclName())
             ->getType().setResolvedType(fieldType);

            if (!F->isStatic()) {
               auto ty = fieldType;
               if (F->hasGetter()) {
                  F->getGetterMethod()->getReturnType().setResolvedType(ty);
               }
               if (F->hasSetter()) {
                  F->getSetterMethod()->getArgs().front()
                    ->getType().setResolvedType(ty);
               }
            }
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
   if (F->isConst() && F->hasSetter()) {
      diagnose(F, err_constant_field_setter);
   }

   if (F->isStatic()) {
      if (fieldTy->isAutoType())
         DelayedDecls.push_back(F);

      return F;
   }

   if (F->hasGetter()) {
      string getterName = "__";
      getterName += util::generate_getter_name(F->getName());

      auto Name = &Context.getIdentifiers().get(getterName);
      auto ty = SourceType(fieldTy);
      auto Getter = MethodDecl::Create(Context, AccessSpecifier::Public,
                                       F->getSourceLoc(), Name, ty, {},
                                       {}, F->getGetterBody(),
                                       F->isStatic());

      Getter->setSynthesized(true);

      addDeclToContext(*R, Getter);
      F->setGetterMethod(Getter);

      declareMethodDecl(Getter);
   }

   if (F->hasSetter()) {
      auto argType = SourceType(fieldTy);
      auto arg = FuncArgDecl::Create(Context, F->getSourceLoc(),
                                     F->getSourceLoc(),
                                     F->getIdentifierInfo(), argType,
                                     nullptr, false, true);

      string setterName = "__";
      setterName += util::generate_setter_name(F->getName());

      auto Name = &Context.getIdentifiers().get(setterName);
      auto retTy = SourceType(Context.getVoidType());
      auto Setter = MethodDecl::Create(Context, AccessSpecifier::Public,
                                       F->getSourceLoc(), Name, retTy,
                                       { arg }, {}, F->getSetterBody(),
                                       F->isStatic());

      Setter->setSynthesized(true);

      addDeclToContext(*R, Setter);
      F->setSetterMethod(Setter);

      declareMethodDecl(Setter);
   }

   if (!F->getType()->isRecordType())
      return F;

   auto Rec = F->getType()->getRecord();
   if (Rec->isClass() || Rec->getSize() != 0)
      return F;

   auto &Vert = LayoutDependency.getOrAddVertex(F->getRecord());
   auto &Dep  = LayoutDependency.getOrAddVertex(Rec);

   Dep.addOutgoing(&Vert);

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
   if (Decl->getAccess() == AccessSpecifier::Default) {
      if (R->isProtocol() || R->isEnum() || R->isStruct()) {
         Decl->setAccess(AccessSpecifier::Public);
      }
      else {
         Decl->setAccess(AccessSpecifier::Private);
      }
   }

   if (Decl->hasGetter()) {
      string getterName = "__";
      getterName += util::generate_getter_name(Decl->getName());

      auto Name = &Context.getIdentifiers().get(getterName);
      auto ty = SourceType(propType);
      auto Getter = MethodDecl::Create(Context, Decl->getAccess(),
                                       Decl->getSourceLoc(), Name, ty, {},
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

      string setterName = "__";
      setterName += util::generate_setter_name(Decl->getName());

      auto Name = &Context.getIdentifiers().get(setterName);
      auto retTy = SourceType(Context.getVoidType());
      auto Setter = MethodDecl::Create(Context, Decl->getAccess(),
                                       Decl->getSourceLoc(), Name, retTy,
                                       { arg }, {}, Decl->getSetterBody(),
                                       Decl->isStatic());

      Setter->setSynthesized(true);
      Setter->setProperty(true);

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

   return declareCallableDecl(M);
}

namespace {

class ParamTypeVisitor: public RecursiveTypeVisitor<ParamTypeVisitor> {
   llvm::SmallPtrSetImpl<TemplateParamDecl*> &Params;

public:
   explicit
   ParamTypeVisitor(llvm::SmallPtrSetImpl<TemplateParamDecl *> &Params)
      : Params(Params)
   { }

   bool visitGenericType(const GenericType *T)
   {
      for (auto P : Params)
         if (P->getName() == T->getGenericTypeName())
            Params.erase(P);

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
      V.visit(arg->getType().getResolvedType());
   }

   if (!Params.empty()) {
      diagnose(Init, err_initializer_templ_args_must_be_inferrable,
               0 /*initializer*/, (*Params.begin())->getDeclName(),
               (*Params.begin())->getSourceRange());
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

   return Decl;
}

TypeResult SemaPass::visitSourceType(Decl *D, const SourceType &Ty)
{
   auto Result = visitSourceType(Ty);
   if (Ty.getTypeExpr())
      D->copyStatusFlags(Ty.getTypeExpr());

   return Result;
}

TypeResult SemaPass::visitSourceType(Statement *S, const SourceType &Ty)
{
   auto Result = visitSourceType(Ty);
   if (Ty.getTypeExpr())
      S->copyStatusFlags(Ty.getTypeExpr());

   return Result;
}

TypeResult SemaPass::visitSourceType(const SourceType &Ty)
{
   if (!Ty.getTypeExpr()) {
      assert(Ty.getResolvedType() && "source ty with no expr or resolved type");
      return Ty.getResolvedType();
   }

   auto Result = visitExpr(Ty.getTypeExpr());
   if (!Result)
      return TypeError();

   bool WantMeta = false;
   if (auto TE = dyn_cast_or_null<TypeExpr>(Ty.getTypeExpr())) {
      WantMeta = TE->isMeta();
   }

   QualType ResTy = Result.get()->getExprType();
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
         auto &expr = res.getResult();

         // this is the same limit that clang uses
         constexpr uint64_t MaxArraySize = (1llu << 59) - 1;

         size_t NumElements = 0;
         if (!expr.isInt()) {
            diagnose(DependentExpr, err_array_size_must_be_integral,
                     SizeExpr->getSourceRange());

            NumElements = 1;
         }
         else if (expr.getAPSInt().ugt(MaxArraySize)) {
            diagnose(DependentExpr, err_array_size_too_large,
                     llvm::APInt(64, MaxArraySize, false), expr.getAPSInt());

            NumElements = MaxArraySize;
         }
         else {
            NumElements = expr.getZExtValue();
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
      ResultTy = ExprRes.get()->getExprType();

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
         TemplateArgList list(*this, Opt);
         list.insert("T", TypeRes.get());

         if (TypeRes.get()->isDependentType()) {
            ResultTy = Context.getDependentRecordType(Opt, move(list));
         }
         else {
            auto Inst = Instantiator.InstantiateRecord(Expr, Opt, move(list));
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

void SemaPass::parseMixinExpr(Expression *E, llvm::StringRef Str,
                              llvm::SmallVectorImpl<Statement*> &Stmts) {
   auto Buf = Diags.getFileMgr()->getBufferForString(Str);
   assert(Buf.Buf && "invalid buffer for string?");

   MixinPrettyStackTrace PST(Str, true);

   lex::Lexer lex(Context.getIdentifiers(), Diags, Buf.Buf, Buf.SourceId,
                  Buf.BaseOffset);

   parse::Parser parser(Context, &lex, *this);
   parser.parse(Stmts);
}

DeclResult SemaPass::declareMixinDecl(MixinDecl *Decl)
{
   auto Res = evalStaticExpr(Decl->getMixinExpr());
   if (!Res)
      return Decl;

   Decl->setMixinExpr(Res.getExpr());

   auto &V = Res.getValue();
   llvm::StringRef Str;

   if (!getStringValue(Decl->getMixinExpr(), V, Str))
      llvm_unreachable("mxmx");

   CompoundDecl CD(SourceLocation(), false);
   {
      DeclContextRAII declContextRAII(*this, &CD, SetParentContext);

      llvm::SmallVector<Statement *, 4> Stmts;
      parseMixinExpr(Decl->getMixinExpr(), Str, Stmts);

      for (auto &D : CD.getDecls()) {
         (void) visitStmt(Decl, D);
      }
   }

   transferDecls(&CD, &getDeclContext());

   return Decl;
}

ExprResult SemaPass::visitMixinExpr(MixinExpr *Expr)
{
   auto Res = evalStaticExpr(Expr->getMixinExpr());
   if (!Res)
      return ExprError();

   Expr->setMixinExpr(Res.getExpr());

   auto &V = Res.getValue();
   llvm::StringRef Str;

   if (!getStringValue(Expr->getMixinExpr(), V, Str))
      llvm_unreachable("mxmx");

   auto Buf = Diags.getFileMgr()->getBufferForString(Str);
   assert(Buf.Buf && "invalid buffer for string?");

   Expression *ResultExpr;

   {
      MixinPrettyStackTrace PST(Str, true);
      lex::Lexer lex(Context.getIdentifiers(), Diags, Buf.Buf, Buf.SourceId,
                     Buf.BaseOffset);

      parse::Parser parser(Context, &lex, *this);
      auto ParseRes = parser.parseExprSequence();

      if (!ParseRes)
         return ExprError();

      if (!ParseRes.holdsExpr()) {
         diagnose(Expr, err_invalid_mixin_kind, 0 /*expression*/);
         return ExprError();
      }

      ResultExpr = ParseRes.getExpr();
   }

   MixinPrettyStackTrace PST(Str, false);
   return visitExpr(Expr, ResultExpr);
}

StmtResult SemaPass::visitMixinStmt(MixinStmt *Stmt)
{
   auto Res = evalStaticExpr(Stmt->getMixinExpr());
   if (!Res)
      return StmtError();

   Stmt->setMixinExpr(Res.getExpr());

   auto &V = Res.getValue();
   llvm::StringRef Str;

   if (!getStringValue(Stmt->getMixinExpr(), V, Str))
      llvm_unreachable("mxmx");

   auto Buf = Diags.getFileMgr()->getBufferForString(Str);
   assert(Buf.Buf && "invalid buffer for string?");

   CompoundStmt *Compound;
   {
      MixinPrettyStackTrace PST(Str, true);
      lex::Lexer lex(Context.getIdentifiers(), Diags, Buf.Buf, Buf.SourceId,
                     Buf.BaseOffset);

      parse::Parser parser(Context, &lex, *this);

      llvm::SmallVector<Statement *, 8> Stmts;
      parser.parseStmts(Stmts);

      Compound = CompoundStmt::Create(Context, Stmts, /*preserveScope*/ true,
                                      SourceLocation(), SourceLocation());
   }

   MixinPrettyStackTrace PST(Str, false);
   return visitStmt(Stmt, Compound);
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

   auto res = evaluateAsBool(Stmt->getCondition());
   if (!res)
      return Stmt;

   auto &expr = res.getValue();
   if (expr.getZExtValue()) {
      transferDecls(Stmt->getIfDecl(), &getDeclContext());
   }
   else if (auto ElseDecl = Stmt->getElseDecl()) {
      transferDecls(ElseDecl, &getDeclContext());
   }

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

ClassDecl* SemaPass::getArrayDecl()
{
   if (!ArrayDecl) {
      auto TU = getDeclContext().getTranslationUnit();
      static auto *II = &Context.getIdentifiers().get("Array");
      ArrayDecl = TU->lookupSingle<ClassDecl>(II);
   }

   return ArrayDecl;
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

ProtocolDecl *SemaPass::getStringRepresentableDecl()
{
   if (!StringRepresentableDecl) {
      auto TU = getDeclContext().getTranslationUnit();
      auto *II = &Context.getIdentifiers().get("StringRepresentable");
      StringRepresentableDecl = TU->lookupSingle<ProtocolDecl>(II);
   }

   return StringRepresentableDecl;
}

} // namespace ast
} // namespace cdot