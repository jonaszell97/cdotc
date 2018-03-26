//
// Created by Jonas Zell on 08.09.17.
//

#include "Compiler.h"

#include "AST/Decl.h"
#include "AST/Transform.h"
#include "AST/Passes/SemanticAnalysis/Builtin.h"
#include "AST/Passes/SemanticAnalysis/SemaPass.h"
#include "AST/Passes/SemanticAnalysis/TemplateInstantiator.h"
#include "AST/Passes/SemanticAnalysis/Template.h"
#include "AST/Passes/ILGen/ILGenPass.h"
#include "Files/FileManager.h"
#include "lex/Lexer.h"
#include "Message/Diagnostics.h"
#include "module/Module.h"
#include "module/ModuleManager.h"
#include "parse/Parser.h"
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
   for (auto decl : DelayedDecls) {
      if (auto Ctx = dyn_cast<DeclContext>(decl)) {
         DeclScopeRAII declContextRAII(*this, Ctx);
         for (auto &P : decl->getTemplateParams())
            declareTemplateParamDecl(P);
      }

      if (auto Ext = dyn_cast<ExtensionDecl>(decl)) {
         if (Ext->getRecord())
            continue;

         auto R = getRecord(Ext->getName());
         if (!R) {
            diagnose(Ext, err_class_not_found, Ext->getName());
            continue;
         }

         R->addExtension(Ext);
      }
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
      if (!D->isInstantiation())
         break;

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

StructDecl* SemaPass::getStruct(llvm::StringRef name) const
{
   return lookup<StructDecl>(name);
}

ClassDecl* SemaPass::getClass(llvm::StringRef name) const
{
   return lookup<ClassDecl>(name);
}

EnumDecl* SemaPass::getEnum(llvm::StringRef name) const
{
   return lookup<EnumDecl>(name);
}

UnionDecl* SemaPass::getUnion(llvm::StringRef name) const
{
   return lookup<UnionDecl>(name);
}

ProtocolDecl* SemaPass::getProtocol(llvm::StringRef name) const
{
   return lookup<ProtocolDecl>(name);
}

FunctionDecl* SemaPass::getAnyFn(llvm::StringRef name) const
{
   return lookup<FunctionDecl>(name);
}

GlobalVarDecl* SemaPass::getVariable(llvm::StringRef name) const
{
   return lookup<GlobalVarDecl>(name);
}

NamespaceDecl* SemaPass::getNamespace(llvm::StringRef name) const
{
   return lookup<NamespaceDecl>(name);
}

AliasDecl* SemaPass::getAlias(llvm::StringRef name) const
{
   return lookup<AliasDecl>(name);
}

TypedefDecl* SemaPass::getTypedef(llvm::StringRef name) const
{
   return lookup<TypedefDecl>(name);
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
      case Decl::ExtensionDeclID:
         return ActOnExtensionDecl(cast<ExtensionDecl>(R));
      default:
         llvm_unreachable("bad record decl kind");
   }

   if (R->getAccess() == AccessModifier::DEFAULT) {
      R->setAccess(AccessModifier::PUBLIC);
   }

   if (!isa<ExtensionDecl>(R))
      ILGen->ForwardDeclareRecord(R);

   if (R->isTemplate())
      DelayedDecls.insert(R);

   addDeclToContext(getDeclContext(), R);

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
   auto R = getRecord(E->getName());
   if (!R) {
      DelayedDecls.insert(E);
   }
   else {
      R->addExtension(E);
   }
}

void SemaPass::ActOnTypedefDecl(TypedefDecl *TD)
{
   if (TD->isTemplate())
      DelayedDecls.insert(TD);

   addDeclToContext(getDeclContext(), TD);
}

void SemaPass::ActOnAliasDecl(AliasDecl *alias)
{
   if (alias->isTemplate())
      DelayedDecls.insert(alias);

   addDeclToContext(getDeclContext(), alias);
}

void SemaPass::ActOnFieldDecl(FieldDecl *F)
{
   addDeclToContext(getDeclContext(), F);
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
   for (auto ctx = &getDeclContext(); ctx; ctx = ctx->getParentCtx()) {
      if (auto P = ctx->lookupSingle<TemplateParamDecl>(name))
         return P;
   }

   return nullptr;
}

AssociatedTypeDecl* SemaPass::getAssociatedType(DeclarationName name)
{
   for (auto ctx = &getDeclContext(); ctx; ctx = ctx->getParentCtx()) {
      if (auto AT = ctx->lookupSingle<AssociatedTypeDecl>(name))
         return AT;
   }

   return nullptr;
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
      case Decl::AliasDeclID: {
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

StmtResult SemaPass::declareImportStmt(ImportStmt *node)
{
//   auto M = ModuleManager::importModule(*this, node);
//   node->setModule(M);
//
//   return node;
//   ModuleImportsByFile[node->getSourceLoc().getSourceId()].push_back(M);
   llvm_unreachable("TODO!");
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
   DeclContextRAII declContextRAII(*this, D);
   for (auto &decl : D->getDecls())
      declareStmt(D, decl);

   return D;
}

DeclResult SemaPass::declareNamespaceDecl(NamespaceDecl *NS)
{
   if (alreadyDeclared(NS))
      return NS;

   DeclContextRAII declContextRAII(*this, NS);
   for (auto &D : NS->getDecls())
      declareStmt(NS, D);

   return NS;
}

StmtResult SemaPass::declareUsingStmt(UsingStmt *Stmt)
{
   auto *declContext = &getDeclContext();
   for (auto &ctx : Stmt->getDeclContextSpecifier()) {
      auto subDecl = declContext->lookup(ctx);
      if (!subDecl) {
         diagnose(Stmt, err_generic_error,
                  " does not have a member named " + ctx->getIdentifier());

         return Stmt;
      }
      if (subDecl.size() != 1) {
         diagnose(Stmt, err_generic_error,
                  "reference to member " + ctx->getIdentifier()
                  + " is ambiguous");

         return Stmt;
      }

      declContext = dyn_cast<DeclContext>(subDecl.front());
      if (!declContext) {
         diagnose(Stmt, err_generic_error,
                  ctx->getIdentifier() + " is not a valid declaration context");

         return Stmt;
      }
   }

   if (Stmt->isWildCardImport()) {
      getDeclContext().makeAllDeclsAvailable(declContext);
      return Stmt;
   }

   for (auto &item : Stmt->getImportedItems()) {
      auto decl = declContext->lookup(item);
      if (!decl) {
         diagnose(Stmt, err_generic_error,
                  " does not have a member named " + item->getIdentifier());

         return Stmt;
      }
      if (decl.size() != 1) {
         diagnose(Stmt, err_generic_error,
                  "reference to member " + item->getIdentifier()
                  + " is ambiguous");

         return Stmt;
      }

      getDeclContext().makeDeclAvailable(decl.front());
   }

   return Stmt;
}

void SemaPass::ActOnFunctionDecl(FunctionDecl *node)
{
   bool isMain = node->getDeclName().isSimpleIdentifier()
                 && node->getDeclName().getIdentifierInfo()->isStr("main")
                 && isa<TranslationUnit>(getDeclContext());

   if (isMain) {
      node->setAccess(AccessModifier::PRIVATE);
      node->setExternC(true);
      node->setIsMain(true);
   }

   addDeclToContext(getDeclContext(), node);
}

DeclResult SemaPass::declareCallableDecl(CallableDecl *F)
{
   // an error occured while parsing this function
   if (F->isInvalid())
      return DeclError();

   DeclContextRAII declContextRAII(*this, F);

   for (auto &TP : F->getTemplateParams())
      declareTemplateParamDecl(TP);

   for (const auto &arg : F->getArgs()) {
      if (!declareStmt(F, arg))
         return DeclError();
   }

   F->setLinkageName(mangle.mangleAnyFunction(F));

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

      auto &FstArgs = C->getArgs();
      auto &SndArgs = Fn->getArgs();

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

   if (auto cov = P->getCovariance()) {
      if (!visitSourceType(P, cov))
         return DeclError();
   }
   
   if (auto con = P->getContravariance()) {
      if (!visitSourceType(P, con))
         return DeclError();
   }

   if (P->getCovariance()->isAutoType()) {
      P->getValueType().setResolvedType(Context.getIntTy());
   }

   if (P->getDefaultValue())
      registerTemplateParamWithDefaultVal(P);

   if (auto Param = getTemplateParam(P->getDeclName())) {
      diagnose(P, err_template_param_shadow, P->getName());
      diagnose(Param, note_template_parameter_here);

      return DeclError();
   }

   addDeclToContext(getDeclContext(), P);

   return P;
}

DeclResult SemaPass::declareGlobalVarDecl(GlobalVarDecl *Decl)
{
   if (alreadyDeclared(Decl))
      return Decl;

   if (auto ty = Decl->getType()) {
      auto res = visitSourceType(Decl, ty);
      if (!res)
         return Decl;
   }
   else {
      DelayedDecls.insert(Decl);
   }

   if (Decl->getAccess() == AccessModifier::DEFAULT)
      Decl->setAccess(AccessModifier::PUBLIC);

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

   if (Decl->getAccess() == AccessModifier::DEFAULT)
      Decl->setAccess(AccessModifier::PUBLIC);

   return Decl;
}

DeclResult SemaPass::declareRecordDecl(RecordDecl *Rec)
{
   DeclPrettyStackTraceEntry STE(Rec);

   Rec->setOpaque(Rec->hasAttribute<OpaqueAttr>());

   if (Rec->getName() != "Any") {
      if (auto Any = getProtocol("Any"))
         Rec->addConformance(Any);
   }

   for (const auto &prot : Rec->getConformanceTypes()) {
      auto res = visitSourceType(Rec, prot);
      if (!res) continue;
      
      auto protoTy = res.get();
      auto Proto = protoTy->getRecord();
      if (Proto->isTemplate())
         break;

      if (!isa<ProtocolDecl>(Proto))
         diagnose(prot.getTypeExpr(),
                  err_conforming_to_non_protocol, Proto->getName());

      Rec->addConformance(cast<ProtocolDecl>(Proto));
   }

   if (!declareDeclContext(Rec))
      return DeclError();

   return Rec;
}

DeclResult SemaPass::declareStructDecl(StructDecl *S)
{
   DeclContextRAII declContextRAII(*this, S);

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
      if (parentTy) {
         C->setIsTypeDependent(true);
      }
      else {
         auto ParentClass = parentTy->getRecord();
         if (!ParentClass->isTemplate()) {
            if (!isa<ClassDecl>(ParentClass)) {
               diagnose(parent.getTypeExpr(), err_generic_error,
                        "cannot extend non-class " + ParentClass->getName());
            }
            else {
               C->setParentClass(cast<ClassDecl>(ParentClass));
            }
         }
      }
   }

   return declareStructDecl(C);
}

static void inheritInitTemplateParams(SemaPass &SP, InitDecl *Decl)
{
   for (auto &TP : Decl->getRecord()->getTemplateParams()) {
      auto *Cpy = new (SP.getContext()) TemplateParamDecl(*TP);
      Cpy->setSynthesized(true);
      Cpy->setNextDeclInContext(nullptr);

      Decl->getTemplateParamsRef().insert(Decl->getTemplateParamsRef().begin(),
                                          Cpy);
   }
}

void SemaPass::declareMemberwiseInitializer(StructDecl *S)
{
   llvm::SmallString<32> argName("arg");
   std::vector<FuncArgDecl*> args;

   size_t i = 0;
   for (auto &F : S->getFields()) {
      if (!F->isStatic() && !F->getDefaultVal()) {
         auto res = visitSourceType(S, F->getType());
         if (!res) return;

         argName += i - '1';

         auto Name = &Context.getIdentifiers().get(argName);
         auto arg = FuncArgDecl::Create(Context, S->getSourceLoc(),
                                        S->getSourceLoc(),
                                        Name, F->getType(),
                                        nullptr, false, true, false);

         ++i;
         argName.pop_back();
         args.push_back(arg);
      }
   }

   auto MDecl = InitDecl::Create(Context, S->getAccess(), S->getSourceLoc(),
                                 move(args), {}, nullptr);

   addDeclToContext(*S, MDecl);
   S->setMemberwiseInitializer(MDecl);

   MDecl->setSynthesized(true);
   MDecl->setMemberwiseInitializer(true);

   declareInitDecl(MDecl);
}

void SemaPass::declareDefaultInitializer(StructDecl *S)
{
   SourceType RetTy(Context.getVoidType());
   auto Name = &Context.getIdentifiers().get("__default_init");
   auto Decl = MethodDecl::Create(Context, AccessModifier::PRIVATE,
                                  S->getSourceLoc(), Name, RetTy, {},
                                  {}, nullptr, false);

   addDeclToContext(*S, Decl);
   S->setDefaultInitializer(Decl);

   Decl->setSynthesized(true);
   Decl->setDefaultInitializer(true);

   declareMethodDecl(Decl);
}

void SemaPass::declareDefaultDeinitializer(StructDecl *S)
{
   auto DDecl = DeinitDecl::Create(Context, S->getSourceLoc(), nullptr);
   addDeclToContext(*S, DDecl);

   DDecl->setSynthesized(true);
   DDecl->setReturnType(SourceType(Context.getVoidType()));

   declareDeinitDecl(DDecl);
}

DeclResult SemaPass::declareProtocolDecl(ProtocolDecl *P)
{
   if (alreadyDeclared(P))
      return P;

   DeclContextRAII declContextRAII(*this, P);
   return declareRecordDecl(P);
}

DeclResult SemaPass::declareExtensionDecl(ExtensionDecl *Ext)
{
   if (alreadyDeclared(Ext))
      return Ext;

   if (!Ext->getRecord()->isExternal())
      return Ext;

   DeclContextRAII declContextRAII(*this, Ext->getRecord());
   return declareRecordDecl(Ext);
}

static void inheritInitTemplateParams(SemaPass &SP, EnumCaseDecl *Decl)
{
   for (auto &TP : Decl->getRecord()->getTemplateParams()) {
      auto *Cpy = new (SP.getContext()) TemplateParamDecl(*TP);
      Cpy->setNextDeclInContext(nullptr);

      Decl->getTemplateParamsRef().insert(Decl->getTemplateParamsRef().begin(),
                                          Cpy);
   }
}

DeclResult SemaPass::declareEnumDecl(EnumDecl *E)
{
   if (alreadyDeclared(E))
      return E;

   DeclContextRAII declContextRAII(*this, E);

   // all enums are implicitly equatable
   if (auto Equatable = getProtocol("Equatable")) {
      E->addConformance(Equatable);
      addImplicitConformance(E, ImplicitConformanceKind::Equatable);
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
      inheritInitTemplateParams(*this, Case);

      for (const auto &assoc : Case->getArgs()) {
         auto res = visitSourceType(assoc, assoc->getType());
         if (!res)
            return E;
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

   return declareRecordDecl(E);
}

DeclResult SemaPass::declareUnionDecl(UnionDecl *U)
{
   if (alreadyDeclared(U))
      return U;

   DeclContextRAII declContextRAII(*this, U);
   return declareRecordDecl(U);
}

DeclResult SemaPass::declareFieldDecl(FieldDecl *F)
{
   if (alreadyDeclared(F))
      return F;

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

   auto res = visitSourceType(F, F->getType());
   if (!res)
      return DeclError();

   auto fieldTy = F->getType().getResolvedType();
   if (F->isConst() && F->hasSetter()) {
      diagnose(F, err_constant_field_setter);
   }

   if (F->isStatic()) {
      if (!R->isProtocol()) {
         llvm::SmallString<128> linkageName;
         linkageName += R->getFullName();
         linkageName += ".";
         linkageName += F->getName();

         F->setLinkageName(linkageName.str());
      }

      if (fieldTy->isAutoType())
         DelayedDecls.insert(F);

      return F;
   }

   if (F->getAccess() == AccessModifier::DEFAULT) {
      if (R->isProtocol() || R->isEnum() || R->isStruct()) {
         F->setAccess(AccessModifier::PUBLIC);
      }
      else {
         F->setAccess(AccessModifier::PRIVATE);
      }
   }

   if (F->hasGetter()) {
      string getterName = "__";
      getterName += util::generate_getter_name(F->getName());

      auto Name = &Context.getIdentifiers().get(getterName);
      auto ty = SourceType(fieldTy);
      auto Getter = MethodDecl::Create(Context, F->getAccess(),
                                       F->getSourceLoc(), Name, ty, {},
                                       {}, F->getGetterBody(),
                                       F->isStatic());

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
      auto Setter = MethodDecl::Create(Context, F->getAccess(),
                                       F->getSourceLoc(), Name, retTy,
                                       { arg }, {}, F->getSetterBody(),
                                       F->isStatic());

      addDeclToContext(*R, Setter);
      F->setSetterMethod(Setter);

      declareMethodDecl(Setter);
   }

   if (!F->getType()->isRecordType())
      return F;

   auto Rec = F->getType()->getRecord();
   if (Rec->isClass())
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

   auto res = visitSourceType(Decl, Decl->getType());
   if (!res)
      return Decl;

   auto propType = Decl->getType().getResolvedType();
   if (Decl->getAccess() == AccessModifier::DEFAULT) {
      if (R->isProtocol() || R->isEnum() || R->isStruct()) {
         Decl->setAccess(AccessModifier::PUBLIC);
      }
      else {
         Decl->setAccess(AccessModifier::PRIVATE);
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
         Getter->setBodyTemplate(Template->getGetterBody());

      addDeclToContext(*R, Getter);
      Decl->setGetterMethod(Getter);

      declareMethodDecl(Getter);
   }

   if (Decl->hasSetter()) {
      auto argType = SourceType(propType);
      auto arg = FuncArgDecl::Create(Context, Decl->getSourceLoc(),
                                     Decl->getSourceLoc(),
                                     Decl->getIdentifierInfo(), argType,
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
         Setter->setBodyTemplate(Template->getSetterBody());

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

   if (!visitSourceType(Decl, Decl->getActualType()))
      return DeclError();

   if (!Decl->getProtocolSpecifier().empty()) {
      auto P = getRecord(Decl->getProtocolSpecifier());
      if (P && isa<ProtocolDecl>(P))
         Decl->setProto(cast<ProtocolDecl>(P));
   }

   return Decl;
}

DeclResult SemaPass::declareMethodDecl(MethodDecl *M)
{
   if (alreadyDeclared(M))
      return M;

   return declareCallableDecl(M);
}

DeclResult SemaPass::declareInitDecl(InitDecl *Init)
{
   if (alreadyDeclared(Init))
      return Init;

   QualType RecordTy = Context.getRecordType(Init->getRecord());
   Init->setReturnType(SourceType(RecordTy));

   auto DeclName = Context.getDeclNameTable().getConstructorName(RecordTy);

   Init->setName(DeclName);
   Init->getRecord()->makeDeclAvailable(DeclName, Init);

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

   // Initializers inherit their record's template parameters
   inheritInitTemplateParams(*this, Init);

   return declareCallableDecl(Init);
}

DeclResult SemaPass::declareDeinitDecl(DeinitDecl *Deinit)
{
   if (alreadyDeclared(Deinit))
      return Deinit;

   Deinit->setReturnType(SourceType(Context.getVoidType()));

   QualType RecordTy = Context.getRecordType(Deinit->getRecord());
   auto DeclName = Context.getDeclNameTable().getDestructorName(RecordTy);

   Deinit->setName(DeclName);
   Deinit->getRecord()->makeDeclAvailable(DeclName, Deinit);

   return declareCallableDecl(Deinit);
}

DeclResult SemaPass::declareTypedefDecl(TypedefDecl *Decl)
{
   if (alreadyDeclared(Decl))
      return Decl;

   if (!visitSourceType(Decl, Decl->getOriginTy()))
      return DeclError();

   if (auto M = getModuleBeingCompiled())
      if (Decl->isExported())
         M->addDecl(Decl);

   return Decl;
}

DeclResult SemaPass::declareAliasDecl(AliasDecl *Decl)
{
   if (alreadyDeclared(Decl))
      return Decl;

   if (auto M = getModuleBeingCompiled())
      if (Decl->isExported())
         M->addDecl(Decl);

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

   QualType ResTy = Result.get()->getExprType();
   if (!isa<TypeExpr>(Ty.getTypeExpr()) && ResTy->isMetaType())
      ResTy = ResTy->uncheckedAsMetaType()->getUnderlyingType();

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

   Expr->setExprType(Context.getTupleType(Tys));
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

   if (Expr->hasAttribute<ThinAttr>())
      Expr->setExprType(Context.getFunctionType(RetTy, ArgTys));
   else
      Expr->setExprType(Context.getLambdaType(RetTy, ArgTys));

   return Expr;
}

ExprResult SemaPass::visitArrayTypeExpr(ArrayTypeExpr *Expr)
{
   auto ElementTyResult = visitSourceType(Expr, Expr->getElementTy());
   QualType ElementTy = ElementTyResult ? ElementTyResult.get()
                                        : UnknownAnyTy;

   QualType ResultTy;
   auto SizeExpr = Expr->getSizeExpr();

   // inferred array size
   if (!SizeExpr) {
      ResultTy = Context.getInferredSizeArrayType(ElementTy);
   }
   else if (auto Ident = dyn_cast<IdentifierRefExpr>(SizeExpr->getExpr())) {
      if (getTemplateParam(Ident->getIdentInfo())) {
         ResultTy = Context.getValueDependentSizedArrayType(ElementTy, Ident);
      }
   }

   if (!ResultTy) {
      auto res = Evaluator.evaluate(SizeExpr);
      if (res) {
         auto &expr = res.getResult();

         size_t NumElements = 0;
         if (!expr.isInt()) {
            diagnose(Expr, err_generic_error, "array size must be integral");
         }
         else {
            NumElements = expr.getZExtValue();
         }

         ResultTy = Context.getArrayType(ElementTy, NumElements);
      }
      else {
         ResultTy = UnknownAnyTy;
      }
   }

   Expr->setExprType(ResultTy);
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

   Expr->setExprType(ResultTy);
   return Expr;
}

ExprResult SemaPass::visitPointerTypeExpr(PointerTypeExpr *Expr)
{
   auto TypeRes = visitSourceType(Expr, Expr->getSubType());

   QualType ResultTy;
   if (!TypeRes)
      ResultTy = UnknownAnyTy;
   else
      ResultTy = Context.getPointerType(TypeRes.get());

   Expr->setExprType(ResultTy);
   return Expr;
}

ExprResult SemaPass::visitReferenceTypeExpr(ReferenceTypeExpr *Expr)
{
   auto TypeRes = visitSourceType(Expr, Expr->getSubType());

   QualType ResultTy;
   if (!TypeRes)
      ResultTy = UnknownAnyTy;
   else
      ResultTy = Context.getReferenceType(TypeRes.get());

   Expr->setExprType(ResultTy);
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
            ResultTy = Context.getRecordType(Inst);
         }
      }
      else {
         diagnose(Expr, err_no_builtin_decl, /*Option type*/ 1);
         ResultTy = UnknownAnyTy;
      }
   }

   Expr->setExprType(ResultTy);
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
      DeclContextRAII declContextRAII(*this, &CD);

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

} // namespace ast
} // namespace cdot