//
// Created by Jonas Zell on 05.06.18.
//

#include "DeclBase.h"

#include "ASTContext.h"
#include "Driver/Compiler.h"
#include "Decl.h"
#include "Serialization/ModuleFile.h"
#include "Statement.h"

#include <llvm/Support/raw_ostream.h>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::support;

Decl::Decl(DeclKind kind, unsigned flags)
   : kind(kind), flags(flags)
{}

Decl::~Decl()
{

}

#ifndef NDEBUG

void Decl::verifyID(DeclKind K)
{
   switch (K) {
#  define CDOT_DECL(NAME) case NAME##ID: break;
#  include "Decl.def"

   default:
      llvm_unreachable("invalid decl kind!");
   }
}

#endif

void Decl::dumpFlags() const
{
   printFlags(llvm::errs());
}

void Decl::printFlags(llvm::raw_ostream &OS) const
{
   OS << "TypeDependent = " << (isTypeDependent() ? "true" : "false") << "\n";
   OS << "ValueDependent = " << (isValueDependent() ? "true" : "false") << "\n";
   OS << "IsInvalid = " << (isInvalid() ? "true" : "false") << "\n";
   OS << "SemaChecked = "<<(isSemanticallyChecked() ? "true" : "false") << "\n";
   OS << "Static = " << (isStatic() ? "true" : "false") << "\n";
   OS << "Const = " << (isConst() ? "true" : "false") << "\n";
   OS << "HasDefinition = " << (hasDefinition() ? "true" : "false") << "\n";
   OS << "External = " << (isExternal() ? "true" : "false") << "\n";
   OS << "WasDeclared = " << (wasDeclared() ? "true" : "false") << "\n";
   OS << "Synthesized = " << (isSynthesized() ? "true" : "false") << "\n";
   OS << "CheckedAttrs = " << (checkedAttrs() ? "true" : "false") << "\n";
}

DeclContext* Decl::getNonTransparentDeclContext() const
{
   for (auto *Ctx = getDeclContext(); Ctx; Ctx = Ctx->getParentCtx())
      if (!Ctx->isTransparent())
         return Ctx;

   llvm_unreachable("no non-transparent decl context?");
}

DeclContext* Decl::getDeclContext() const
{
   if (ContextUnion.isNull())
      return nullptr;

   if (auto Ctx = ContextUnion.dyn_cast<DeclContext*>())
      return Ctx;

   return ContextUnion.get<MultipleDeclContext*>()->LogicalDC;
}

DeclContext* Decl::getLexicalContext() const
{
   if (ContextUnion.isNull())
      return nullptr;

   if (auto Ctx = ContextUnion.dyn_cast<DeclContext*>())
      return Ctx;

   return ContextUnion.get<MultipleDeclContext*>()->LexicalDC;
}

void Decl::setLexicalContext(DeclContext *Ctx)
{
   if (ContextUnion.is<MultipleDeclContext*>()) {
      ContextUnion.get<MultipleDeclContext*>()->LexicalDC = Ctx;
   }
   else {
      ContextUnion = Ctx;
   }
}

void Decl::setLogicalContext(DeclContext *Ctx)
{
   if (ContextUnion.is<MultipleDeclContext*>()) {
      ContextUnion.get<MultipleDeclContext*>()->LogicalDC = Ctx;
   }
   else {
      auto Multiple = new (getASTCtx()) MultipleDeclContext;
      Multiple->LogicalDC = Ctx;
      Multiple->LexicalDC = getLexicalContext();

      ContextUnion = Multiple;
   }
}

void Decl::setLexicalContextUnchecked(DeclContext *Ctx)
{
   if (ContextUnion.is<MultipleDeclContext*>()) {
      ContextUnion.get<MultipleDeclContext*>()->LexicalDC = Ctx;
   }
   else {
      ContextUnion = Ctx;
   }
}

NamedDecl* Decl::getInstantiationScope() const
{
   DeclContext const* ctx;
   if (isa<DeclContext>(this)) {
      ctx = cast<DeclContext>(this);
   }
   else {
      ctx = getDeclContext();
   }

   while (ctx) {
      ctx = ctx->lookThroughExtension();
      if (isa<NamedDecl>(ctx) && cast<NamedDecl>(ctx)->isInstantiation())
         return cast<NamedDecl>(const_cast<DeclContext*>(ctx));

      ctx = ctx->getParentCtx();
   }

   return nullptr;
}

NamedDecl* Decl::getProtocolDefaultImplScope() const
{
   DeclContext const* ctx;
   if (isa<DeclContext>(this)) {
      ctx = cast<DeclContext>(this);
   }
   else {
      ctx = getDeclContext();
   }

   while (ctx) {
      ctx = ctx->lookThroughExtension();

      if (isa<NamedDecl>(ctx)
            && cast<NamedDecl>(ctx)->instantiatedFromProtocolDefaultImpl())
         return cast<NamedDecl>(const_cast<DeclContext*>(ctx));

      ctx = ctx->getParentCtx();
   }

   return nullptr;
}

SourceRange Decl::getSourceRange() const
{
   switch (kind) {
#  define CDOT_DECL(SubClass)                                                 \
   case SubClass##ID:                                                         \
      static_assert(&Decl::getSourceRange != &SubClass::getSourceRange,       \
                    "getSourceRange not implemented by " #SubClass);          \
      return support::cast<SubClass>(this)->getSourceRange();
#  include "AST/Decl.def"

   default:
      llvm_unreachable("not a decl");
   }
}

ASTContext& Decl::getASTCtx() const
{
   DeclContext const* ctx;
   if (isa<DeclContext>(this)) {
      ctx = cast<DeclContext>(this);
   }
   else {
      ctx = getDeclContext();
   }

   while (!isa<GlobalDeclContext>(ctx)) {
      ctx = ctx->getParentCtx();
      assert(ctx && "decl without a translation unit!");
   }

   return cast<GlobalDeclContext>(ctx)->getCompilerInstance().getContext();
}

bool Decl::hasTrailingObjects() const
{
   switch (getKind()) {
   case Decl::MacroExpansionDeclID:
   case Decl::SubscriptDeclID:
   case Decl::AliasDeclID:
   case Decl::MacroDeclID:
   case Decl::ImportDeclID:
   case Decl::UsingDeclID:
   case Decl::ExtensionDeclID:
   case Decl::FunctionDeclID:
   case Decl::MethodDeclID:
   case Decl::InitDeclID:
   case Decl::DeinitDeclID:
   case Decl::EnumCaseDeclID:
   case Decl::DestructuringDeclID:
      return true;
   default:
      return false;
   }
}

llvm::ArrayRef<Attr*> Decl::getAttributes() const
{
   return getASTCtx().getAttributes(this);
}

void Decl::setAttributes(llvm::ArrayRef<Attr*> attrs) const
{
   return getASTCtx().setAttributes(this, attrs);
}

void Decl::addAttributes(llvm::ArrayRef<Attr *> attrs) const
{
   getASTCtx().addAttributes(this, attrs);
}

void Decl::addAttribute(Attr *A) const
{
   getASTCtx().addAttribute(this, A);
}

void Decl::copyStatusFlags(Statement *D)
{
   flags |= (D->getSubclassData() & StatusFlags);
}

void Decl::copyStatusFlags(Decl *D)
{
   flags |= (D->flags & StatusFlags);
}

bool Decl::instantiatedFromProtocolDefaultImpl() const
{
   return isa<ProtocolDecl>(getLexicalContext()->lookThroughExtension())
      && !isa<ProtocolDecl>(getDeclContext());
}

bool Decl::isInExtension() const
{
   auto Ctx = getNonTransparentDeclContext();
   return isa<ExtensionDecl>(Ctx);
}

DeclContext* Decl::castToDeclContext(const Decl *D)
{
   switch (D->getKind()) {
#  define CDOT_DECL_CONTEXT(Name)                               \
      case Name##ID:                                            \
         return static_cast<Name*>(const_cast<Decl*>(D));
#  include "AST/Decl.def"

   default:
      llvm_unreachable("not a decl context");
   }
}

Decl* Decl::castFromDeclContext(const DeclContext *Ctx)
{
   switch (Ctx->getDeclKind()) {
#  define CDOT_DECL_CONTEXT(Name)                                   \
      case Name##ID:                                                \
         return static_cast<Name*>(const_cast<DeclContext*>(Ctx));
#  include "AST/Decl.def"

   default:
      llvm_unreachable("not a decl");
   }
}

RecordDecl* Decl::getRecord() const
{
   return dyn_cast<RecordDecl>(
      getNonTransparentDeclContext()->lookThroughExtension());
}

ModuleDecl* Decl::getModule() const
{
   for (auto ctx = getDeclContext(); ctx; ctx = ctx->getParentCtx())
      if (auto Mod = dyn_cast<ModuleDecl>(ctx))
         return Mod;

   llvm_unreachable("Decl without a module!");
}

bool Decl::inAnonymousNamespace() const
{
   for (auto ctx = getDeclContext(); ctx; ctx = ctx->getParentCtx())
      if (ctx->isAnonymousNamespace())
         return true;

   return false;
}

bool Decl::inStdNamespace() const
{
   for (auto ctx = getDeclContext(); ctx; ctx = ctx->getParentCtx())
      if (auto NS = dyn_cast<NamespaceDecl>(ctx))
         if (NS->getName() == "std" && isa<ModuleDecl>(NS->getParentCtx()))
            return true;

   return false;
}

bool Decl::isGlobalDecl() const
{
   return isa<ModuleDecl>(getDeclContext());
}

void Decl::dumpName() const
{
   if (auto ND = dyn_cast<NamedDecl>(this))
      return ND->dumpName();

   llvm::outs() << "<unnamed decl>" << "\n";
}

NamedDecl::NamedDecl(DeclKind typeID,
                     AccessSpecifier access,
                     DeclarationName DN)
   : Decl(typeID), access(access), Name(DN)
{}

llvm::ArrayRef<StaticExpr*> NamedDecl::getConstraints() const
{
   return getASTCtx().getConstraints(this);
}

bool NamedDecl::isExported() const
{
   if (access == AccessSpecifier::Private)
      return false;

   if (auto NS = support::dyn_cast<NamespaceDecl>(getDeclContext()))
      return !NS->isAnonymousNamespace();

   return true;
}

std::string NamedDecl::getJoinedName(char join, bool includeFile) const
{
   std::string joinedName;
   llvm::raw_string_ostream OS(joinedName);

   auto *Ctx = getNonTransparentDeclContext()->lookThroughExtension();
   if (isa<NamedDecl>(Ctx)) {
      OS << cast<NamedDecl>(Ctx)->getJoinedName(join, includeFile);
      OS << join;
   }

   if (auto *E = dyn_cast<ExtensionDecl>(this)) {
      joinedName += "extension ";
      OS << E->getExtendedRecord()->getDeclName();
   }
   else if (isa<ImportDecl>(this)) {
      OS << "<import>";
   }
   else {
      assert(Name && "invalid declaration name");
      OS << Name;
   }

   if (auto *C = dyn_cast<CallableDecl>(this)) {
      unsigned i = 0;
      OS << "(";

      for (auto *Arg : C->getArgs()) {
         if (i++ != 0) OS << ", ";
         OS << Arg->getDeclName() << ": ";
         OS << Arg->getType();
      }

      OS << ")";
   }

   return OS.str();
}

size_t NamedDecl::getSpecifierForDiagnostic()
{
   switch (kind) {
   case StructDeclID:
   case ClassDeclID:
   case EnumDeclID:
   case UnionDeclID:
   case ProtocolDeclID:
      return cast<RecordDecl>(this)->getSpecifierForDiagnostic();
   case FunctionDeclID:
      return 5;
   case MethodDeclID:
   case InitDeclID:
   case DeinitDeclID:
      return 6;
   case AliasDeclID:
      return 7;
   case NamespaceDeclID:
      return 8;
   case FieldDeclID:
      return 9;
   case PropDeclID:
      return 10;
   case TypedefDeclID:
      return 11;
   default:
      llvm_unreachable("missing diagnostic specifier!");
   }
}

void NamedDecl::setAccessLoc(cdot::SourceLocation Loc)
{
   AccessLoc = Loc;
}

SourceRange NamedDecl::getAccessRange() const
{
   unsigned length = 0;
   switch (access) {
   case AccessSpecifier::Default:
      break;
   case AccessSpecifier::Public:
      length += 5;
      break;
   case AccessSpecifier::Private:
      length += 6;
      break;
   case AccessSpecifier::Internal:
      length += 7;
      break;
   case AccessSpecifier::Protected:
      length += 8;
      break;
   case AccessSpecifier::FilePrivate:
      length += 10;
      break;
   }

   return SourceRange(AccessLoc,
                      SourceLocation(AccessLoc.getOffset() + length));
}

void NamedDecl::setName(DeclarationName DN)
{
   Name = DN;
}

bool NamedDecl::isOverloadable() const
{
   switch (kind) {
   case AliasDeclID:
   case FunctionDeclID:
   case MethodDeclID:
   case InitDeclID:
   case ExtensionDeclID:
   case SubscriptDeclID:
      return true;
   default:
      return false;
   }
}

bool NamedDecl::isRedeclarable() const
{
   switch (kind) {
   case ModuleDeclID:
   case NamespaceDeclID:
      return true;
   default:
      return false;
   }
}

bool NamedDecl::isTemplate() const
{
   return !getTemplateParams().empty();
}

bool NamedDecl::inDependentContext() const
{
   if (isTemplate())
      return true;

   for (auto Ctx = getDeclContext(); Ctx; Ctx = Ctx->getParentCtx()) {
      if (auto ND = dyn_cast<NamedDecl>(Ctx)) {
         if (ND->isTemplate() || isa<ExtensionDecl>(ND))
            return true;
      }
   }

   return false;
}

bool NamedDecl::isTemplateOrInTemplate() const
{
   if (isTemplate())
      return true;

   for (auto Ctx = getDeclContext(); Ctx; Ctx = Ctx->getParentCtx()) {
      if (auto ND = dyn_cast<NamedDecl>(Ctx)) {
         if (ND->isTemplate())
            return true;

         if (auto Ext = dyn_cast<ExtensionDecl>(ND))
            if (Ext->getExtendedRecord()->isTemplateOrInTemplate())
               return true;
      }
   }

   return false;
}

llvm::ArrayRef<TemplateParamDecl*> NamedDecl::getTemplateParams() const
{
   switch (kind) {
   case AliasDeclID:
      return cast<AliasDecl>(this)->getTemplateParams();
   case TypedefDeclID:
      return cast<TypedefDecl>(this)->getTemplateParams();
   case StructDeclID:
   case ClassDeclID:
   case EnumDeclID:
   case UnionDeclID:
      return cast<RecordDecl>(this)->getTemplateParams();
   case FunctionDeclID:
   case MethodDeclID:
   case InitDeclID:
   case DeinitDeclID:
   case EnumCaseDeclID:
      return cast<CallableDecl>(this)->getTemplateParams();
   default:
      return {};
   }
}

sema::FinalTemplateArgumentList& NamedDecl::getTemplateArgs() const
{
   switch (kind) {
   case AliasDeclID:
      return cast<AliasDecl>(this)->getTemplateArgs();
   case StructDeclID:
   case ClassDeclID:
   case EnumDeclID:
   case UnionDeclID:
      return cast<RecordDecl>(this)->getTemplateArgs();
   case FunctionDeclID:
   case MethodDeclID:
   case InitDeclID:
   case DeinitDeclID:
   case EnumCaseDeclID:
      return cast<CallableDecl>(this)->getTemplateArgs();
   default:
      llvm_unreachable("not a template instantiation!");
   }
}

NamedDecl* NamedDecl::getSpecializedTemplate() const
{
   switch (kind) {
   case AliasDeclID:
      return cast<AliasDecl>(this)->getSpecializedTemplate();
   case StructDeclID:
   case ClassDeclID:
   case EnumDeclID:
   case UnionDeclID:
      return cast<RecordDecl>(this)->getSpecializedTemplate();
   case FunctionDeclID:
   case MethodDeclID:
   case InitDeclID:
   case DeinitDeclID:
   case EnumCaseDeclID:
      return cast<CallableDecl>(this)->getSpecializedTemplate();
   default:
      return {};
   }
}

SourceLocation NamedDecl::getInstantiatedFrom() const
{
   switch (kind) {
   case AliasDeclID:
      return cast<AliasDecl>(this)->getInstantiatedFrom();
   case StructDeclID:
   case ClassDeclID:
   case EnumDeclID:
   case UnionDeclID:
      return cast<RecordDecl>(this)->getInstantiatedFrom();
   case FunctionDeclID:
   case MethodDeclID:
   case InitDeclID:
   case DeinitDeclID:
   case EnumCaseDeclID:
      return cast<CallableDecl>(this)->getInstantiatedFrom();
   default:
      return {};
   }
}

void NamedDecl::dumpName() const
{
   llvm::outs() << getFullName() << "\n";
}

bool DeclContext::hasExternalStorage() const
{
   switch (getDeclKind()) {
   case Decl::ImportDeclID:
   case Decl::ModuleDeclID:
      return true;
   default:
      return false;
   }
}

void DeclContext::addDecl(Decl *decl)
{
   assert(!decl->getNextDeclInContext());

   decl->setLexicalContext(this);
   if (auto Ctx = support::dyn_cast<DeclContext>(decl))
      Ctx->setParentCtx(this);

   if (!firstDecl)
      firstDecl = decl;

   if (lastAddedDecl) {
      assert(!lastAddedDecl->getNextDeclInContext());
      assert(lastAddedDecl != decl && "adding the same decl twice");
      lastAddedDecl->setNextDeclInContext(decl);
   }

   lastAddedDecl = decl;
}

DeclContext::AddDeclResultKind DeclContext::addDecl(NamedDecl *decl)
{
   return addDecl(decl->getDeclName(), decl);
}

DeclContext::AddDeclResultKind DeclContext::addDecl(DeclarationName Name,
                                                    NamedDecl *decl) {
   AddDeclResultKind Result = ADR_Success;
   if (Name) {
      Result = primaryCtx->makeDeclAvailable(Name, decl);
   }

   addDecl((Decl*)decl);
   return Result;
}

DeclContextLookupResult DeclContext::lookup(DeclarationName name)
{
   auto it = primaryCtx->namedDecls.find(name);
   if (it != primaryCtx->namedDecls.end())
      return it->getSecond().getAsLookupResult();

   return {};
}

NamedDecl *DeclContext::lookupSingle(DeclarationName name)
{
   auto res = lookup(name);
   if (res.size() != 1)
      return nullptr;

   return res.front();
}

DeclContext::AddDeclResultKind DeclContext::makeDeclAvailable(NamedDecl *decl)
{
   return makeDeclAvailable(decl->getDeclName(), decl);
}

DeclContext::AddDeclResultKind
DeclContext::makeDeclAvailable(DeclarationName Name,
                               NamedDecl *decl) {
   if (!Name)
      return ADR_Success;

   auto it = primaryCtx->namedDecls.find(Name);
   if (it == primaryCtx->namedDecls.end()) {
      primaryCtx->namedDecls.try_emplace(Name, decl);
   }
   else {
      auto lookup = it->getSecond().getAsLookupResult();
      assert(!lookup.empty());

      if (lookup.front()->getKind() != decl->getKind())
         return ADR_DuplicateDifferentKind;

      if (!lookup.front()->isOverloadable()) {
         if (lookup.front()->isRedeclarable()) {
            return ADR_Success;
         }

         return ADR_Duplicate;
      }

      it->getSecond().appendDecl(decl);
   }

   if (isTransparent()) {
      return parentCtx->makeDeclAvailable(Name, decl);
   }

   return ADR_Success;
}

bool DeclContext::isTransparent() const
{
   switch (declKind) {
   case Decl::NamespaceDeclID:
      return cast<NamespaceDecl>(this)->isAnonymousNamespace();
   case Decl::CompoundDeclID:
      return cast<CompoundDecl>(this)->isTransparent();
   default:
      return false;
   }
}

bool DeclContext::isAnonymousNamespace() const
{
   auto NS = dyn_cast<NamespaceDecl>(this);
   return NS && NS->isAnonymousNamespace();
}

void DeclContext::setParentCtx(DeclContext *parent)
{
   assert(parent != this && "context is parent of itself?");
   parentCtx = parent;
}

void DeclContext::setParentCtxUnchecked(cdot::ast::DeclContext *parent)
{
   assert(parent != this && "context is parent of itself?");
   parentCtx = parent;
}

DeclContext* DeclContext::lookThroughExtension() const
{
   if (auto *Ext = dyn_cast<ExtensionDecl>(this))
      return Ext->getExtendedRecord();

   return const_cast<DeclContext*>(this);
}

void DeclContext::setPrimaryCtx(DeclContext *V)
{
   assert(V->isPrimaryCtx());
   primaryCtx = V;
}

serial::ModuleFile *DeclContext::getModFile() const
{
   if (!primaryCtx->ExtStorage)
      return nullptr;

   return primaryCtx->ExtStorage->ModFile;
}

void DeclContext::setModFile(serial::ModuleFile *V)
{
   if (!primaryCtx->ExtStorage)
      primaryCtx->ExtStorage = new(getDeclASTContext()) ExternalStorage;

   primaryCtx->ExtStorage->ModFile = V;
}

ArrayRef<Module*> DeclContext::getImportedModules() const
{
   if (!primaryCtx->ExtStorage)
      return {};

   return primaryCtx->ExtStorage->ImportedModules;
}

void DeclContext::addImportedModule(Module *Mod)
{
   if (!primaryCtx->ExtStorage)
      primaryCtx->ExtStorage = new(getDeclASTContext()) ExternalStorage;

   auto &Imports = primaryCtx->ExtStorage->ImportedModules;
   if (std::find(Imports.begin(), Imports.end(), Mod) == Imports.end())
      Imports.push_back(Mod);
}

ASTContext& DeclContext::getDeclASTContext() const
{
   DeclContext const* ctx = this;
   while (!isa<GlobalDeclContext>(ctx)) {
      ctx = ctx->getParentCtx();
      assert(ctx && "decl without a translation unit!");
   }

   return cast<GlobalDeclContext>(ctx)->getCompilerInstance().getContext();
}

void DeclContext::replaceDecl(Decl *Orig, Decl *Rep)
{
   if (Orig == Rep)
      return;

   if (Orig == firstDecl) {
      Rep->setNextDeclInContext(Orig->getNextDeclInContext());
      firstDecl = Rep;

      return;
   }

   bool found = false;
   for (auto &D : getDecls()) {
      if (D->getNextDeclInContext() == Orig) {
         Rep->setLexicalContext(this);
         Rep->setNextDeclInContext(D->getNextDeclInContext()
                                    ->getNextDeclInContext());
         D->setNextDeclInContext(Rep);

         found = true;
         break;
      }
   }

   assert(found && "original decl not found");

   if (lastAddedDecl == Orig)
      lastAddedDecl = Rep;
}

ModuleDecl *DeclContext::getDeclModule() const
{
   auto current = const_cast<DeclContext*>(this);
   while (!isa<ModuleDecl>(current)) {
      current = current->getParentCtx();
      assert(current && "no module!");
   }

   return cast<ModuleDecl>(current);
}

NamespaceDecl* DeclContext::getClosestNamespace() const
{
   auto current = const_cast<DeclContext*>(this);
   while (current && !isa<NamespaceDecl>(current)) {
      current = current->getParentCtx();
   }

   if (!current)
      return nullptr;

   return cast<NamespaceDecl>(current);
}

bool DeclContext::isGlobalDeclContext() const
{
   for (auto ctx = this; ctx; ctx = ctx->getParentCtx()) {
      switch (ctx->getDeclKind()) {
      case Decl::NamespaceDeclID:
      case Decl::ModuleDeclID:
      case Decl::CompoundDeclID:
      case Decl::NotDecl:
         break;
      default:
         return false;
      }
   }

   return true;
}

void DeclContext::dumpName() const
{
   if (auto ND = dyn_cast<NamedDecl>(this))
      return ND->dumpName();

   llvm::outs() << "<unnamed decl>" << "\n";
}