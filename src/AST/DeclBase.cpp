#include "cdotc/AST/DeclBase.h"

#include "cdotc/AST/ASTContext.h"
#include "cdotc/AST/Decl.h"
#include "cdotc/AST/Statement.h"
#include "cdotc/Driver/Compiler.h"
#include "cdotc/Module/Module.h"
#include "cdotc/Serialization/ModuleFile.h"

#include <llvm/Support/raw_ostream.h>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::support;

static_assert((int)Decl::DF_ContainsGenericParam
                  == (int)Statement::ContainsGenericParam,
              "flag must be the same!");

static_assert((int)Decl::DF_ContainsAssociatedType
                  == (int)Statement::ContainsAssociatedType,
              "flag must be the same!");

static_assert((int)Decl::DF_ContainsUnexpandedPack
                  == (int)Statement::ContainsUnexpandedPack,
              "flag must be the same!");

Decl::Decl(DeclKind kind, unsigned flags) : kind(kind), flags(flags) {}

Decl::~Decl() {}

#ifndef NDEBUG

void Decl::verifyID(DeclKind K)
{
   switch (K) {
#define CDOT_DECL(NAME)                                                        \
   case NAME##ID:                                                              \
      break;
#include "cdotc/AST/Decl.def"

   default:
      llvm_unreachable("invalid decl kind!");
   }
}

#endif

void Decl::dumpFlags() const { printFlags(llvm::errs()); }

void Decl::printFlags(llvm::raw_ostream& OS) const
{
   OS << "TypeDependent = " << (isTypeDependent() ? "true" : "false") << "\n";
   OS << "ValueDependent = " << (isValueDependent() ? "true" : "false") << "\n";
   OS << "IsInvalid = " << (isInvalid() ? "true" : "false") << "\n";
   OS << "SemaChecked = " << (isSemanticallyChecked() ? "true" : "false")
      << "\n";
   OS << "Static = " << (isStatic() ? "true" : "false") << "\n";
   OS << "HasDefinition = " << (hasDefinition() ? "true" : "false") << "\n";
   OS << "External = " << (isExternal() ? "true" : "false") << "\n";
   OS << "WasDeclared = " << (wasDeclared() ? "true" : "false") << "\n";
   OS << "Synthesized = " << (isSynthesized() ? "true" : "false") << "\n";
   OS << "CheckedAttrs = " << (checkedAttrs() ? "true" : "false") << "\n";
}

DeclContext* Decl::getNonTransparentDeclContext() const
{
   for (auto* Ctx = getDeclContext(); Ctx; Ctx = Ctx->getParentCtx())
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

void Decl::setLexicalContext(DeclContext* Ctx)
{
   if (ContextUnion.is<MultipleDeclContext*>()) {
      ContextUnion.get<MultipleDeclContext*>()->LexicalDC = Ctx;
   }
   else {
      ContextUnion = Ctx;
   }
}

void Decl::setLogicalContext(DeclContext* Ctx)
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

void Decl::setLexicalContextUnchecked(DeclContext* Ctx)
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
   if (isa<DeclContext>()) {
      ctx = cast<DeclContext>();
   }
   else {
      ctx = getDeclContext();
   }

   while (ctx) {
      ctx = ctx->lookThroughExtension();
      if (support::isa<NamedDecl>(ctx)
          && support::cast<NamedDecl>(ctx)->isInstantiation())
         return support::cast<NamedDecl>(const_cast<DeclContext*>(ctx));

      ctx = ctx->getParentCtx();
   }

   return nullptr;
}

NamedDecl* Decl::getProtocolDefaultImplScope() const
{
   DeclContext const* ctx;
   if (isa<DeclContext>()) {
      ctx = cast<DeclContext>();
   }
   else {
      ctx = getDeclContext();
   }

   while (ctx) {
      ctx = ctx->lookThroughExtension();

      if (support::isa<NamedDecl>(ctx)
          && support::cast<NamedDecl>(ctx)
                 ->instantiatedFromProtocolDefaultImpl())
         return support::cast<NamedDecl>(const_cast<DeclContext*>(ctx));

      ctx = ctx->getParentCtx();
   }

   return nullptr;
}

SourceRange Decl::getSourceRange() const
{
   switch (kind) {
#define CDOT_DECL(SubClass)                                                    \
   case SubClass##ID:                                                          \
      static_assert(&Decl::getSourceRange != &SubClass::getSourceRange,        \
                    "getSourceRange not implemented by " #SubClass);           \
      return support::cast<SubClass>(this)->getSourceRange();
#include "cdotc/AST/Decl.def"

   default:
      llvm_unreachable("not a decl");
   }
}

ASTContext& Decl::getASTCtx() const
{
   DeclContext const* ctx;
   if (support::isa<DeclContext>(this)) {
      ctx = support::cast<DeclContext>(this);
   }
   else {
      ctx = getDeclContext();
   }

   while (!support::isa<GlobalDeclContext>(ctx)) {
      ctx = ctx->getParentCtx();
      assert(ctx && "decl without a translation unit!");
   }

   return support::cast<GlobalDeclContext>(ctx)
       ->getCompilerInstance()
       .getContext();
}

bool Decl::isNestedInstantiation() const
{
   return isInstantiation()
     && !support::cast<NamedDecl>(this)->getSpecializedTemplate()->isTemplate();
}

bool Decl::hasTrailingObjects() const
{
   switch (getKind()) {
   case Decl::MacroExpansionDeclID:
   case Decl::AliasDeclID:
   case Decl::MacroDeclID:
   case Decl::ImportDeclID:
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

void Decl::addAttributes(llvm::ArrayRef<Attr*> attrs) const
{
   getASTCtx().addAttributes(this, attrs);
}

void Decl::addAttribute(Attr* A) const { getASTCtx().addAttribute(this, A); }

void Decl::copyStatusFlags(Statement* D)
{
#ifndef NDEBUG
   if (D->isInvalid()) {
      setIsInvalid(true);
   }
#endif

   flags |= (D->getSubclassData() & StatusFlags);
}

void Decl::copyStatusFlags(Decl* D)
{
#ifndef NDEBUG
   if (D->isInvalid()) {
      setIsInvalid(true);
   }
#endif

   flags |= (D->flags & StatusFlags);
}

void Decl::setIsInvalid(bool error)
{
   setDeclFlag(DF_IsInvalid, error);
}

bool Decl::isInExtension() const
{
   auto Ctx = getNonTransparentDeclContext();
   return support::isa<ExtensionDecl>(Ctx);
}

DeclContext* Decl::castToDeclContext(const Decl* D)
{
   switch (D->getKind()) {
#define CDOT_DECL_CONTEXT(Name)                                                \
   case Name##ID:                                                              \
      return static_cast<Name*>(const_cast<Decl*>(D));
#include "cdotc/AST/Decl.def"

   default:
      llvm_unreachable("not a decl context");
   }
}

Decl* Decl::castFromDeclContext(const DeclContext* Ctx)
{
   switch (Ctx->getDeclKind()) {
#define CDOT_DECL_CONTEXT(Name)                                                \
   case Name##ID:                                                              \
      return static_cast<Name*>(const_cast<DeclContext*>(Ctx));
#include "cdotc/AST/Decl.def"

   default:
      llvm_unreachable("not a decl");
   }
}

RecordDecl* Decl::getRecord() const
{
   return support::dyn_cast<RecordDecl>(
       getNonTransparentDeclContext()->lookThroughExtension());
}

ModuleDecl* Decl::getModule() const
{
   DeclContext* Ctx = support::dyn_cast<DeclContext>(const_cast<Decl*>(this));
   if (!Ctx) {
      Ctx = getDeclContext();
   }

   for (; Ctx; Ctx = Ctx->getParentCtx())
      if (auto Mod = support::dyn_cast<ModuleDecl>(Ctx))
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
      if (auto NS = support::dyn_cast<NamespaceDecl>(ctx))
         if (NS->getName() == "std"
             && support::isa<ModuleDecl>(NS->getParentCtx()))
            return true;

   return false;
}

bool Decl::isGlobalDecl() const
{
   return support::isa<ModuleDecl>(getDeclContext());
}

void Decl::dumpName() const
{
   if (auto ND = support::dyn_cast<NamedDecl>(this))
      return ND->dumpName();

   llvm::outs() << "<unnamed decl>"
                << "\n";
}

void Decl::dumpSourceLine()
{
   SourceRange SR = getSourceRange();
   SourceLocation Start = SR.getStart();
   SourceLocation End = SR.getEnd();

   if (Start && End) {
      getASTCtx().CI.getFileMgr().dumpSourceRange(SR);
   }
   else if (Start) {
      getASTCtx().CI.getFileMgr().dumpSourceLine(Start);
   }
   else {
      llvm::outs() << "<invalid source location>";
   }
}

std::string Decl::getNameAsString() const
{
   if (auto ND = support::dyn_cast<NamedDecl>(this))
      return ND->getFullName();

   return "<unnamed decl>";
}

StringRef Decl::getFileName() const
{
   return getASTCtx().CI.getFileMgr().getFileName(getSourceLoc());
}

std::string Decl::getSourceLocAsString() const
{
   return getASTCtx().CI.getFileMgr().getSourceLocationAsString(getSourceLoc());
}

FullSourceLoc Decl::getFullSourceLoc() const
{
   return getASTCtx().CI.getFileMgr().getFullSourceLoc(getSourceLoc());
}

NamedDecl::NamedDecl(DeclKind typeID, AccessSpecifier access,
                     DeclarationName DN)
    : Decl(typeID), access(access), Name(DN)
{
}

ConstraintSet* NamedDecl::getConstraints() const
{
   return getASTCtx().getExtConstraints(this);
}

bool NamedDecl::isExported() const
{
   if (access == AccessSpecifier::Private)
      return false;

   if (auto NS = support::dyn_cast<NamespaceDecl>(getDeclContext()))
      return !NS->isAnonymousNamespace();

   return true;
}

std::string NamedDecl::getJoinedName(char join, bool includeFile,
                                     bool includeSignatures,
                                     bool includeAllModules) const
{
   std::string joinedName;
   llvm::raw_string_ostream OS(joinedName);

   auto* Ctx = getNonTransparentDeclContext()->lookThroughExtension();
   if (support::isa<NamedDecl>(Ctx)) {
      bool printCtx = true;

      if (auto *fn = support::dyn_cast<FunctionDecl>(Ctx)) {
         printCtx &= !fn->isMain();
      }
      else if (auto *Mod = support::dyn_cast<ModuleDecl>(Ctx)) {
         printCtx &= includeAllModules || Mod->getModule()->isImported();
         printCtx &= includeAllModules
             || !Mod->getModule()->getBaseModule()->getName()->isStr("core");
      }

      if (printCtx) {
         OS << support::cast<NamedDecl>(Ctx)->getJoinedName(join, includeFile);
         OS << join;
      }
   }

   if (auto* E = support::dyn_cast<ExtensionDecl>(this);
       E && E->getExtendedRecord()) {
      joinedName += "extension ";
      OS << E->getExtendedRecord()->getDeclName();
   }
   else if (support::isa<ImportDecl>(this)) {
      OS << "<import>";
   }
   else {
      OS << Name;
   }

   auto* C = support::dyn_cast<CallableDecl>(this);
   if (!C && support::isa<SubscriptDecl>(this)) {
      auto* S = support::dyn_cast<SubscriptDecl>(this);
      C = S->getGetterMethod();

      if (!C) {
         C = S->getSetterMethod();
      }
   }

   if (C && includeSignatures) {
      unsigned i = 0;
      OS << "(";

      for (auto* Arg : C->getArgs()) {
         if (i++ != 0)
            OS << ", ";

         if (Arg->getLabel()) {
            if (!Arg->getDeclName().isSimpleIdentifier()
                || Arg->getLabel() != Arg->getDeclName().getIdentifierInfo()) {
               OS << Arg->getLabel()->getIdentifier() << " ";
            }
         }
         else {
            OS << "_ ";
         }

         OS << Arg->getDeclName() << ": ";

         switch (Arg->getConvention()) {
         case ArgumentConvention::Borrowed:
         case ArgumentConvention::Default:
            break;
         case ArgumentConvention::Owned:
            OS << "owned ";
            break;
         case ArgumentConvention::ImmutableRef:
            OS << "ref ";
            break;
         case ArgumentConvention::MutableRef:
            OS << "mut ref ";
            break;
         }

         OS << Arg->getType();
      }

      OS << ") -> " << C->getReturnType();
   }

   return OS.str();
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

void NamedDecl::setName(DeclarationName DN) { Name = DN; }

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

bool NamedDecl::isTemplate() const { return !getTemplateParams().empty(); }

bool NamedDecl::isNestedTemplate() const
{
   return !isTemplate() && isTemplateOrInTemplate();
}

bool NamedDecl::inDependentContext() const
{
   if (isUnboundedTemplate())
      return true;

   for (auto Ctx = getDeclContext(); Ctx; Ctx = Ctx->getParentCtx()) {
      if (auto ND = support::dyn_cast<NamedDecl>(Ctx)) {
         if (ND->isUnboundedTemplate())
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
      if (auto ND = support::dyn_cast<NamedDecl>(Ctx)) {
         if (ND->isTemplate())
            return true;

         if (auto Ext = support::dyn_cast<ExtensionDecl>(ND)) {
            if (Ext->getExtendedRecord()->isTemplateOrInTemplate()) {
               return true;
            }
         }
      }
   }

   return false;
}

NamedDecl* NamedDecl::getOuterTemplate() const
{
   for (auto Ctx = getDeclContext(); Ctx; Ctx = Ctx->getParentCtx()) {
      if (auto ND = support::dyn_cast<NamedDecl>(Ctx)) {
         if (ND->isTemplate())
            return ND;

         if (auto Ext = support::dyn_cast<ExtensionDecl>(ND)) {
            if (Ext->getExtendedRecord()->isTemplateOrInTemplate()) {
               return Ext->getExtendedRecord();
            }
         }
      }
   }

   return nullptr;
}

bool NamedDecl::isInUnboundedTemplate() const
{
   if (isUnboundedTemplate())
      return true;

   for (auto Ctx = getDeclContext(); Ctx; Ctx = Ctx->getParentCtx()) {
      if (auto ND = support::dyn_cast<NamedDecl>(Ctx)) {
         if (ND->isUnboundedTemplate())
            return true;

         if (auto Ext = support::dyn_cast<ExtensionDecl>(ND)) {
            if (Ext->getExtendedRecord()->isInUnboundedTemplate()) {
               return true;
            }
         }
      }
   }

   return false;
}

llvm::ArrayRef<TemplateParamDecl*> NamedDecl::getTemplateParams() const
{
   switch (kind) {
   case AliasDeclID:
      return support::cast<AliasDecl>(this)->getTemplateParams();
   case TypedefDeclID:
      return support::cast<TypedefDecl>(this)->getTemplateParams();
   case StructDeclID:
   case ClassDeclID:
   case EnumDeclID:
   case UnionDeclID:
   case ProtocolDeclID:
      return support::cast<RecordDecl>(this)->getTemplateParams();
   case FunctionDeclID:
   case MethodDeclID:
   case InitDeclID:
   case DeinitDeclID:
   case EnumCaseDeclID:
      return support::cast<CallableDecl>(this)->getTemplateParams();
   default:
      return {};
   }
}

sema::FinalTemplateArgumentList& NamedDecl::getTemplateArgs() const
{
   switch (kind) {
   case AliasDeclID:
      return support::cast<AliasDecl>(this)->getTemplateArgs();
   case StructDeclID:
   case ClassDeclID:
   case EnumDeclID:
   case UnionDeclID:
   case ProtocolDeclID:
      return support::cast<RecordDecl>(this)->getTemplateArgs();
   case FunctionDeclID:
   case MethodDeclID:
   case InitDeclID:
   case DeinitDeclID:
   case EnumCaseDeclID:
      return support::cast<CallableDecl>(this)->getTemplateArgs();
   default:
      llvm_unreachable("not a template instantiation!");
   }
}

bool NamedDecl::isShallowInstantiation() const
{
   if (!isInstantiation())
      return false;

   return !getSpecializedTemplate()->isInUnboundedTemplate();
}

NamedDecl* NamedDecl::getSpecializedTemplate() const
{
   switch (kind) {
   case AliasDeclID:
      return support::cast<AliasDecl>(this)->getSpecializedTemplate();
   case StructDeclID:
   case ClassDeclID:
   case EnumDeclID:
   case UnionDeclID:
   case ProtocolDeclID:
      return support::cast<RecordDecl>(this)->getSpecializedTemplate();
   case FunctionDeclID:
   case MethodDeclID:
   case InitDeclID:
   case DeinitDeclID:
   case EnumCaseDeclID:
      return support::cast<CallableDecl>(this)->getSpecializedTemplate();
   default:
      return {};
   }
}

SourceLocation NamedDecl::getInstantiatedFrom() const
{
   switch (kind) {
   case AliasDeclID:
      return support::cast<AliasDecl>(this)->getInstantiatedFrom();
   case StructDeclID:
   case ClassDeclID:
   case EnumDeclID:
   case UnionDeclID:
   case ProtocolDeclID:
      return support::cast<RecordDecl>(this)->getInstantiatedFrom();
   case FunctionDeclID:
   case MethodDeclID:
   case InitDeclID:
   case DeinitDeclID:
   case EnumCaseDeclID:
      return support::cast<CallableDecl>(this)->getInstantiatedFrom();
   default:
      return {};
   }
}

void NamedDecl::dumpName() const { llvm::outs() << getFullName() << "\n"; }

void DeclList::appendDecl(NamedDecl* Decl)
{
   if (auto Single = getAsDecl()) {
      auto Vec = new VecTy{Single, Decl};
      Data = DataTy(Vec);
   }
   else {
      getAsVec()->push_back(Decl);
   }

}

void DeclList::removeDecl(NamedDecl* Decl)
{
   auto& Vec = *getAsVec();
   auto It = std::find(Vec.begin(), Vec.end(), Decl);

   if (It != Vec.end()) {
      Vec.erase(It);
   }
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

void DeclContext::addDecl(Decl* decl)
{
   assert(!decl->getNextDeclInContext());

   decl->setLexicalContext(this);
   if (auto Ctx = support::dyn_cast<DeclContext>(decl)) {
      Ctx->setParentCtx(this);
   }

   if (!firstDecl) {
      firstDecl = decl;
   }

   if (lastAddedDecl) {
      assert(!lastAddedDecl->getNextDeclInContext());
      assert(lastAddedDecl != decl && "adding the same decl twice");
      lastAddedDecl->setNextDeclInContext(decl);
   }

   lastAddedDecl = decl;
   registerVisibleDeclChange();
}

DeclContext::AddDeclResultKind DeclContext::addDecl(NamedDecl* decl)
{
   return addDecl(decl->getDeclName(), decl);
}

DeclContext::AddDeclResultKind DeclContext::addDecl(DeclarationName Name,
                                                    NamedDecl* decl)
{
   AddDeclResultKind Result = ADR_Success;
   if (Name) {
      Result = primaryCtx->makeDeclAvailable(Name, decl);
   }

   addDecl((Decl*)decl);
   return Result;
}

DeclContextLookupResult DeclContext::lookup(DeclarationName name)
{
   if (auto *MF = getModFile()) {
      MF->PerformExternalLookup(*this, name);
   }

   auto it = primaryCtx->namedDecls.find(name);
   if (it != primaryCtx->namedDecls.end())
      return it->getSecond().getAsLookupResult();

   return {};
}

NamedDecl* DeclContext::lookupSingle(DeclarationName name)
{
   auto res = lookup(name);
   if (res.size() != 1)
      return nullptr;

   return res.front();
}

DeclContext::AddDeclResultKind DeclContext::makeDeclAvailable(NamedDecl* decl)
{
   return makeDeclAvailable(decl->getDeclName(), decl);
}

static bool shouldAddToOuterContext(DeclContext* This, NamedDecl* Decl)
{
   // 'using' and 'import' declarations are private to the source file.
   if (!support::isa<SourceFileDecl>(This)) {
      return true;
   }

   switch (Decl->getKind()) {
   case Decl::ImportDeclID:
      return false;
   case Decl::MacroDeclID:
      // fileprivate macros are not visible in any other file.
      return Decl->getAccess() != AccessSpecifier::FilePrivate
             && Decl->getAccess() != AccessSpecifier::Private;
   default:
      return true;
   }
}

DeclContext::AddDeclResultKind
DeclContext::makeDeclAvailable(DeclarationName Name, NamedDecl* decl,
                               bool IsImport)
{
   if (!Name) {
      return ADR_Success;
   }

   auto it = primaryCtx->namedDecls.find(Name);
   if (it == primaryCtx->namedDecls.end()) {
      primaryCtx->namedDecls.try_emplace(Name, decl);
      registerVisibleDeclChange();
   }
   else {
      auto lookup = it->getSecond().getAsLookupResult();
      assert(!lookup.empty());

      if (lookup.front()->getKind() != decl->getKind()) {
         return ADR_DuplicateDifferentKind;
      }

      if (!lookup.front()->isOverloadable()) {
         if (lookup.front()->isRedeclarable()) {
            return ADR_Success;
         }

         return ADR_Duplicate;
      }

      it->getSecond().appendDecl(decl);
      registerVisibleDeclChange();
   }

   if (!IsImport && isTransparent() && shouldAddToOuterContext(this, decl)) {
      return parentCtx->makeDeclAvailable(Name, decl);
   }

   return ADR_Success;
}

bool DeclContext::isTransparent() const
{
   switch (declKind) {
   case Decl::NamespaceDeclID:
      return support::cast<NamespaceDecl>(this)->isAnonymousNamespace();
   case Decl::CompoundDeclID:
      return support::cast<CompoundDecl>(this)->isTransparent();
   case Decl::SourceFileDeclID:
      return true;
   default:
      return false;
   }
}

bool DeclContext::isAnonymousNamespace() const
{
   auto NS = support::dyn_cast<NamespaceDecl>(this);
   return NS && NS->isAnonymousNamespace();
}

void DeclContext::setParentCtx(DeclContext* parent)
{
   assert(parent != this && "context is parent of itself?");
   parentCtx = parent;
}

void DeclContext::setParentCtxUnchecked(cdot::ast::DeclContext* parent)
{
   assert(parent != this && "context is parent of itself?");
   parentCtx = parent;
}

DeclContext* DeclContext::lookThroughExtension() const
{
   if (auto* Ext = support::dyn_cast<ExtensionDecl>(this)) {
      auto* R = Ext->getExtendedRecord();
      if (R)
         return R;
   }

   return const_cast<DeclContext*>(this);
}

bool DeclContext::isLocalContext() const
{
   return support::isa<CallableDecl>(this);
}

bool DeclContext::inLocalContext() const
{
   for (auto* Ctx = this; Ctx; Ctx = Ctx->getParentCtx()) {
      if (Ctx->isLocalContext())
         return true;
   }

   return false;
}

void DeclContext::setPrimaryCtx(DeclContext* V)
{
   assert(V->isPrimaryCtx());
   primaryCtx = V;
}

serial::ModuleFile* DeclContext::getModFile() const
{
   if (!primaryCtx->ExtStorage)
      return nullptr;

   return primaryCtx->ExtStorage->ModFile;
}

void DeclContext::setModFile(serial::ModuleFile* V)
{
   if (!primaryCtx->ExtStorage)
      primaryCtx->ExtStorage = new (getDeclASTContext()) ExternalStorage;

   primaryCtx->ExtStorage->ModFile = V;
}

ArrayRef<Module*> DeclContext::getImportedModules() const
{
   if (!primaryCtx->ExtStorage)
      return {};

   return primaryCtx->ExtStorage->ImportedModules;
}

void DeclContext::addImportedModule(Module* Mod)
{
   if (!primaryCtx->ExtStorage)
      primaryCtx->ExtStorage = new (getDeclASTContext()) ExternalStorage;

   auto& Imports = primaryCtx->ExtStorage->ImportedModules;
   if (std::find(Imports.begin(), Imports.end(), Mod) == Imports.end())
      Imports.push_back(Mod);
}

ASTContext& DeclContext::getDeclASTContext() const
{
   DeclContext const* ctx = this;
   while (!support::isa<GlobalDeclContext>(ctx)) {
      ctx = ctx->getParentCtx();
      assert(ctx && "decl without a translation unit!");
   }

   return support::cast<GlobalDeclContext>(ctx)
       ->getCompilerInstance()
       .getContext();
}

void DeclContext::removeVisibleDecl(NamedDecl* D)
{
   removeVisibleDecl(D, D->getDeclName());
}

void DeclContext::removeVisibleDecl(NamedDecl* D, DeclarationName VisibleName)
{
   auto DeclIt = primaryCtx->namedDecls.find(VisibleName);
   if (DeclIt == primaryCtx->namedDecls.end()) {
      return;
   }

   DeclList& Decls = DeclIt->getSecond();
   if (Decls.getAsDecl()) {
      primaryCtx->namedDecls.erase(DeclIt);
   }
   else {
      Decls.removeDecl(D);
   }

   registerVisibleDeclChange();
}

void DeclContext::replaceDecl(Decl* Orig, Decl* Rep)
{
   if (Orig == Rep) {
      return;
   }

   if (Orig == firstDecl) {
      Rep->setNextDeclInContext(Orig->getNextDeclInContext());
      firstDecl = Rep;

      registerVisibleDeclChange();
      return;
   }

   bool found = false;
   for (auto& D : getDecls()) {
      if (D->getNextDeclInContext() == Orig) {
         Rep->setLexicalContext(this);
         Rep->setNextDeclInContext(
             D->getNextDeclInContext()->getNextDeclInContext());
         D->setNextDeclInContext(Rep);

         found = true;
         break;
      }
   }

   assert(found && "original decl not found");
   if (lastAddedDecl == Orig) {
      lastAddedDecl = Rep;
   }

   registerVisibleDeclChange();
}

ModuleDecl* DeclContext::getDeclModule() const
{
   auto current = const_cast<DeclContext*>(this);
   while (!support::isa<ModuleDecl>(current)) {
      current = current->getParentCtx();
      assert(current && "no module!");
   }

   return support::cast<ModuleDecl>(current);
}

NamespaceDecl* DeclContext::getClosestNamespace() const
{
   auto current = const_cast<DeclContext*>(this);
   while (current && !support::isa<NamespaceDecl>(current)) {
      current = current->getParentCtx();
   }

   if (!current)
      return nullptr;

   return support::cast<NamespaceDecl>(current);
}

bool DeclContext::isGlobalDeclContext() const
{
   for (auto ctx = this; ctx; ctx = ctx->getParentCtx()) {
      switch (ctx->getDeclKind()) {
      case Decl::NamespaceDeclID:
      case Decl::ModuleDeclID:
      case Decl::CompoundDeclID:
      case Decl::NotDecl:
      case Decl::SourceFileDeclID:
         break;
      default:
         return false;
      }
   }

   return true;
}

void DeclContext::dumpName() const
{
   if (auto ND = support::dyn_cast<NamedDecl>(this))
      return ND->dumpName();

   llvm::outs() << "<unnamed decl>"
                << "\n";
}

void DeclContext::dumpSourceLine()
{
   if (auto ND = support::dyn_cast<NamedDecl>(this)) {
      return ND->dumpSourceLine();
   }

   llvm::outs() << "<invalid source location>"
                << "\n";
}

std::string DeclContext::getNameAsString() const
{
   if (auto ND = support::dyn_cast<NamedDecl>(this))
      return ND->getFullName();

   return "<unnamed decl context>";
}