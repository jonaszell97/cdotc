//
// Created by Jonas Zell on 26.01.18.
//

#include "Decl.h"

#include "AST/ASTContext.h"
#include "AST/Expression.h"
#include "AST/Statement.h"
#include "Sema/Builtin.h"
#include "Sema/SemaPass.h"

#include <llvm/ADT/StringSwitch.h>
#include <llvm/ADT/SmallString.h>

using namespace cdot::support;

namespace cdot {
namespace ast {

Decl::Decl(DeclKind kind, unsigned flags)
   : kind(kind), flags(flags)
{}

Decl::~Decl()
{

}

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
   if (auto Ctx = ContextUnion.dyn_cast<DeclContext*>())
      return Ctx;

   return ContextUnion.get<MultipleDeclContext*>()->LogicalDC;
}

DeclContext* Decl::getLexicalContext() const
{
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

   if (auto DC = dyn_cast<DeclContext>(this))
      DC->setParentCtxUnchecked(Ctx);

   assert((!isa<GlobalDeclContext>(Ctx) || isa<TranslationUnit>(this)));
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

NamedDecl::NamedDecl(DeclKind typeID,
                     AccessSpecifier access,
                     DeclarationName DN)
   : Decl(typeID), access(access), Name(DN)
{}

llvm::ArrayRef<StaticExpr*> NamedDecl::getConstraints() const
{
   return getASTCtx().getConstraints(this);
}


UsingDecl::UsingDecl(SourceRange Loc,
                     AccessSpecifier Access,
                     DeclarationName Name,
                     llvm::ArrayRef<IdentifierInfo*> NestedImportName,
                     bool wildCardImport)
   : NamedDecl(UsingDeclID, Access, Name),
     Loc(Loc), NumSpecifierNames((unsigned)NestedImportName.size()),
     IsWildCard(wildCardImport)
{
   std::copy(NestedImportName.begin(), NestedImportName.end(),
             getTrailingObjects<IdentifierInfo*>());
}

UsingDecl* UsingDecl::Create(ASTContext &C,
                             SourceRange Loc,
                             AccessSpecifier Access,
                             DeclarationName Name,
                             llvm::ArrayRef<IdentifierInfo*> NestedImportName,
                             bool wildCardImport) {
   void *Mem = C.Allocate(
      totalSizeToAlloc<IdentifierInfo*>(NestedImportName.size()),
      alignof(UsingDecl));

   return new(Mem) UsingDecl(Loc, Access, Name, NestedImportName,
                             wildCardImport);
}

ImportDecl::ImportDecl(SourceRange Loc,
                       AccessSpecifier Access,
                       llvm::ArrayRef<IdentifierInfo *> moduleName)
   : NamedDecl(ImportDeclID, Access, moduleName.back()),
     Loc(Loc), NumNameQuals((unsigned)moduleName.size())
{
   std::copy(moduleName.begin(), moduleName.end(),
             getTrailingObjects<IdentifierInfo*>());
}

ImportDecl* ImportDecl::Create(ASTContext &C, SourceRange Loc,
                               AccessSpecifier Access,
                               llvm::ArrayRef<IdentifierInfo *> moduleName) {
   void *Mem = C.Allocate(totalSizeToAlloc<IdentifierInfo*>(moduleName.size()),
                          alignof(ImportDecl));

   return new(Mem) ImportDecl(Loc, Access, moduleName);
}

ModuleDecl::ModuleDecl(SourceRange Loc,
                       AccessSpecifier Access,
                       llvm::ArrayRef<IdentifierInfo *> moduleName)
   : NamedDecl(ModuleDeclID, Access, moduleName.back()),
     Loc(Loc), NumNameQuals((unsigned)moduleName.size())
{
   std::copy(moduleName.begin(), moduleName.end(),
             getTrailingObjects<IdentifierInfo*>());
}

ModuleDecl* ModuleDecl::Create(ASTContext &C, SourceRange Loc,
                               AccessSpecifier Access,
                               llvm::ArrayRef<IdentifierInfo *> moduleName) {
   void *Mem = C.Allocate(totalSizeToAlloc<IdentifierInfo*>(moduleName.size()),
                          alignof(ModuleDecl));

   return new(Mem) ModuleDecl(Loc, Access, moduleName);
}


TemplateParamDecl::TemplateParamDecl(DeclarationName Name,
                                     SourceType covariance,
                                     SourceType contravariance,
                                     Expression *defaultValue,
                                     unsigned Index,
                                     SourceLocation TypeNameOrValueLoc,
                                     SourceLocation NameLoc,
                                     SourceLocation EllipsisLoc)
   : NamedDecl(TemplateParamDeclID, (AccessSpecifier)0, Name),
     covariance(covariance), contravariance(contravariance),
     typeName(true), defaultValue(defaultValue), Index(Index),
     TypeNameOrValueLoc(TypeNameOrValueLoc), NameLoc(NameLoc),
     EllipsisLoc(EllipsisLoc)
{}

TemplateParamDecl::TemplateParamDecl(DeclarationName Name,
                                     SourceType valueType,
                                     Expression *defaultValue,
                                     unsigned Index,
                                     SourceLocation TypeNameOrValueLoc,
                                     SourceLocation NameLoc,
                                     SourceLocation EllipsisLoc)
   : NamedDecl(TemplateParamDeclID, (AccessSpecifier)0, Name),
     covariance(valueType), contravariance(nullptr),
     typeName(false), defaultValue(defaultValue), Index(Index),
     TypeNameOrValueLoc(TypeNameOrValueLoc), NameLoc(NameLoc),
     EllipsisLoc(EllipsisLoc)
{}

TemplateParamDecl* TemplateParamDecl::Create(ASTContext &C,
                                             DeclarationName Name,
                                             SourceType covariance,
                                             SourceType contravariance,
                                             Expression *defaultValue,
                                             unsigned Index,
                                             SourceLocation TypeNameOrValueLoc,
                                             SourceLocation NameLoc,
                                             SourceLocation EllipsisLoc) {
   return new(C) TemplateParamDecl(Name, covariance, contravariance,
                                   defaultValue, Index,
                                   TypeNameOrValueLoc, NameLoc, EllipsisLoc);
}

TemplateParamDecl* TemplateParamDecl::Create(ASTContext &C,
                                             DeclarationName Name,
                                             SourceType valueType,
                                             Expression *defaultValue,
                                             unsigned Index,
                                             SourceLocation TypeNameOrValueLoc,
                                             SourceLocation NameLoc,
                                             SourceLocation EllipsisLoc) {
   return new(C) TemplateParamDecl(Name, valueType, defaultValue, Index,
                                   TypeNameOrValueLoc, NameLoc, EllipsisLoc);
}

SourceRange TemplateParamDecl::getSourceRange() const
{
   SourceLocation Begin;
   SourceLocation End;

   if (EllipsisLoc) {
      Begin = EllipsisLoc;
   }
   else {
      Begin = NameLoc;
   }

   if (defaultValue) {
      End = defaultValue->getSourceRange().getEnd();
   }
   else if (Expression *E = covariance.getTypeExpr()) {
      End = E->getSourceRange().getEnd();
   }
   else {
      End = NameLoc;
   }

   return SourceRange(Begin, End);
}

VarDecl::VarDecl(DeclKind id,
                 AccessSpecifier access,
                 SourceLocation VarOrLetLoc,
                 SourceLocation ColonLoc,
                 bool isConst,
                 DeclarationName Name,
                 SourceType type,
                 Expression* value)
   : NamedDecl(id, access, Name),
     VarOrLetLoc(VarOrLetLoc), ColonLoc(ColonLoc),
     type(type), value(value),
     CanElideCopy(false), Variadic(false), Captured(false)
{
   setDeclFlag(DeclFlags::DF_Const, isConst);
}

SourceRange VarDecl::getSourceRange() const
{
   SourceLocation End;
   if (value) {
      End = value->getSourceRange().getEnd();
   }
   else if (type && type.getTypeExpr()) {
      End = type.getTypeExpr()->getSourceRange().getEnd();
   }
   else {
      End = VarOrLetLoc;
   }

   return SourceRange(VarOrLetLoc, End);
}

LocalVarDecl::LocalVarDecl(AccessSpecifier access,
                           SourceLocation VarOrLetLoc,
                           SourceLocation ColonLoc,
                           bool isConst,
                           DeclarationName Name,
                           SourceType type,
                           Expression* value)
   : VarDecl(LocalVarDeclID, access, VarOrLetLoc, ColonLoc,
             isConst, Name, type, value),
     IsNRVOCand(false)
{}

LocalVarDecl* LocalVarDecl::Create(ASTContext &C,
                                   AccessSpecifier access,
                                   SourceLocation VarOrLetLoc,
                                   SourceLocation ColonLoc,
                                   bool isConst,
                                   DeclarationName Name,
                                   SourceType type,
                                   Expression *value) {
   return new(C) LocalVarDecl(access, VarOrLetLoc, ColonLoc,
                              isConst, Name, type, value);
}

GlobalVarDecl::GlobalVarDecl(AccessSpecifier access,
                             SourceLocation VarOrLetLoc,
                             SourceLocation ColonLoc,
                             bool isConst,
                             DeclarationName Name,
                             SourceType type,
                             Expression* value)
   : VarDecl(GlobalVarDeclID, access, VarOrLetLoc, ColonLoc,
             isConst, Name, type, value)
{}

GlobalVarDecl* GlobalVarDecl::Create(ASTContext &C,
                                     AccessSpecifier access,
                                     SourceLocation VarOrLetLoc,
                                     SourceLocation ColonLoc,
                                     bool isConst,
                                     DeclarationName Name,
                                     SourceType type,
                                     Expression *value) {
   return new(C) GlobalVarDecl(access, VarOrLetLoc, ColonLoc,
                               isConst, Name, type, value);
}

FuncArgDecl::FuncArgDecl(SourceLocation VarOrLetLoc,
                         SourceLocation ColonLoc,
                         DeclarationName Name,
                         SourceType argType,
                         Expression* defaultValue,
                         bool variadicArgPackExpansion,
                         bool isConst,
                         bool cstyleVararg)
   : VarDecl(FuncArgDeclID, AccessSpecifier::Public, VarOrLetLoc, ColonLoc,
             isConst, Name, argType, defaultValue),
     VariadicArgPackExpansion(variadicArgPackExpansion),
     IsConst(isConst), Vararg(cstyleVararg), CstyleVararg(cstyleVararg)
{}

FuncArgDecl* FuncArgDecl::Create(ASTContext &C,
                                 SourceLocation VarOrLetLoc,
                                 SourceLocation ColonLoc,
                                 DeclarationName Name,
                                 SourceType argType,
                                 Expression *defaultValue,
                                 bool variadicArgPackExpansion,
                                 bool isConst, bool cstyleVararg) {
   return new(C) FuncArgDecl(VarOrLetLoc, ColonLoc, Name, argType, defaultValue,
                             variadicArgPackExpansion, isConst, cstyleVararg);
}

void DeclContext::addDecl(Decl *decl)
{
   decl->setLexicalContext(this);
   if (auto Ctx = support::dyn_cast<DeclContext>(decl))
      Ctx->setParentCtx(this);

   if (!firstDecl)
      firstDecl = decl;

   if (lastAddedDecl) {
      assert(!lastAddedDecl->getNextDeclInContext());
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
      Result = makeDeclAvailable(Name, decl);
   }

   addDecl((Decl*)decl);
   return Result;
}

LookupResult DeclContext::lookup(DeclarationName name) const
{
   auto res = lookupOwn(name);
   if (res)
      return res;

   if (auto P = parentCtx) {
      res = P->lookup(name);
      if (res)
         return res;
   }

   return {};
}

NamedDecl *DeclContext::lookupSingle(DeclarationName name) const
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
   auto it = namedDecls.find(Name);
   if (it == namedDecls.end()) {
      namedDecls.try_emplace(Name, decl);
   }
   else {
      auto lookup = it->getSecond().getAsLookupResult();
      assert(!lookup.empty());

      if (lookup.front()->getKind() != decl->getKind())
         return ADR_DuplicateDifferentKind;

      if (!lookup.front()->isOverloadable())
         return ADR_Duplicate;

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

ASTContext& Decl::getASTCtx() const
{
   DeclContext const* ctx;
   if (isa<DeclContext>(this)) {
      ctx = cast<DeclContext>(this);
   }
   else {
      ctx = getDeclContext();
   }

   while (!isa<TranslationUnit>(ctx)) {
      ctx = ctx->getParentCtx();
      assert(ctx && "decl without a translation unit!");
   }

   return cast<TranslationUnit>(ctx)->getASTCtx();
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
   if ((D->getSubclassData() & Statement::SemanticallyChecked) == 0)
      flags &= ~DF_SemanticallyChecked;
}

void Decl::copyStatusFlags(Decl *D)
{
   flags |= (D->flags & StatusFlags);

   if ((D->flags & DF_SemanticallyChecked) == 0)
      flags &= ~DF_SemanticallyChecked;

   if ((D->flags & DF_WasDeclared) == 0)
      flags &= ~DF_WasDeclared;
}

bool Decl::isInStdNamespace() const
{
   auto NS = dyn_cast<NamespaceDecl>(getNonTransparentDeclContext());

   return NS && isa<TranslationUnit>(NS->getParentCtx())
      && !NS->isAnonymousNamespace()
      && NS->getDeclName().isStr("std");
}

bool Decl::isInCompilerNamespace() const
{
   auto NS = dyn_cast<NamespaceDecl>(getNonTransparentDeclContext());

   return NS && isa<TranslationUnit>(NS->getParentCtx())
          && !NS->isAnonymousNamespace()
          && NS->getDeclName().isStr("Compiler");
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
   auto Ctx = getNonTransparentDeclContext();
   if (auto R = dyn_cast<RecordDecl>(Ctx))
      return R;

   return nullptr;
}

bool NamedDecl::isExported() const
{
   if (access == AccessSpecifier::Private)
      return false;

   if (auto NS = support::dyn_cast<NamespaceDecl>(getDeclContext()))
      return !NS->isAnonymousNamespace();

   return true;
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
         if (NS->getName() == "std" && isa<TranslationUnit>(NS->getParentCtx()))
            return true;

   return false;
}

bool Decl::isGlobalDecl() const
{
   return isa<TranslationUnit>(getDeclContext());
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

bool NamedDecl::isOverloadable() const
{
   switch (kind) {
      case AliasDeclID:
      case FunctionDeclID:
      case MethodDeclID:
      case InitDeclID:
      case ExtensionDeclID:
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

const sema::TemplateArgList& NamedDecl::getTemplateArgs() const
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

NamedDecl* NamedDecl::getInstantiatedWithin() const
{
   switch (kind) {
   case AliasDeclID:
      return cast<AliasDecl>(this)->getInstantiatedWithin();
   case StructDeclID:
   case ClassDeclID:
   case EnumDeclID:
   case UnionDeclID:
      return cast<RecordDecl>(this)->getInstantiatedWithin();
   case FunctionDeclID:
   case MethodDeclID:
   case InitDeclID:
   case DeinitDeclID:
   case EnumCaseDeclID:
      return cast<CallableDecl>(this)->getInstantiatedWithin();
   default:
      return {};
   }
}

void DeclContext::setParentCtx(DeclContext *parent)
{
   assert(parent != this && "context is parent of itself?");
   parentCtx = parent;

   if (auto ND = dyn_cast<Decl>(this))
      ND->setLexicalContextUnchecked(parent);

   assert((!isa<GlobalDeclContext>(parent) || isa<TranslationUnit>(this)));
}

void DeclContext::setParentCtxUnchecked(cdot::ast::DeclContext *parent)
{
   assert(parent != this && "context is parent of itself?");
   parentCtx = parent;
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

TranslationUnit* DeclContext::getTranslationUnit() const
{
   auto current = const_cast<DeclContext*>(this);
   while (!isa<TranslationUnit>(current)) {
      current = current->getParentCtx();
      assert(current && "no translation unit!");
   }

   return cast<TranslationUnit>(current);
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
         case Decl::TranslationUnitID:
         case Decl::CompoundDeclID:
         case Decl::NotDecl:
            break;
         default:
            return false;
      }
   }

   return true;
}

std::string NamedDecl::getJoinedName(char join, bool includeFile) const
{
   std::string joinedName;
   llvm::raw_string_ostream OS(joinedName);

   auto *Ctx = getNonTransparentDeclContext();
   if (isa<NamedDecl>(Ctx)) {
      if (!isa<TranslationUnit>(Ctx) || includeFile) {
         OS << cast<NamedDecl>(Ctx)->getJoinedName(join, includeFile);
         OS << join;
      }
   }

   OS << Name;
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

TranslationUnit::TranslationUnit(ASTContext &ASTCtx,
                                 IdentifierInfo *fileName,
                                 size_t sourceId,
                                 llvm::ArrayRef<ImportDecl*> imports)
   : NamedDecl(TranslationUnitID, (AccessSpecifier)0, fileName),
     DeclContext(TranslationUnitID),
     ASTCtx(ASTCtx), sourceId(sourceId),
     numImports(unsigned(imports.size()))
{
   std::copy(imports.begin(), imports.end(), import_begin());
}

TranslationUnit* TranslationUnit::Create(ASTContext &ASTCtx,
                                         IdentifierInfo *fileName,
                                         size_t sourceId,
                                         llvm::ArrayRef<ImportDecl*> imports) {
   void *Mem = ASTCtx.Allocate(totalSizeToAlloc<ImportDecl*>(imports.size()),
                               alignof(TranslationUnit));

   return new(Mem) TranslationUnit(ASTCtx, fileName, sourceId, imports);
}

GlobalDeclContext::GlobalDeclContext()
   : DeclContext(Decl::NotDecl)
{
}

GlobalDeclContext* GlobalDeclContext::Create(ASTContext &C)
{
   return new(C) GlobalDeclContext;
}

NamespaceDecl::NamespaceDecl(SourceLocation NamespaceLoc,
                             SourceLocation LBrace,
                             DeclarationName Name)
   : NamedDecl(NamespaceDeclID, (AccessSpecifier)0, Name),
     DeclContext(NamespaceDeclID),
     NamespaceLoc(NamespaceLoc), Braces(LBrace)
{

}

NamespaceDecl* NamespaceDecl::Create(ASTContext &C,
                                     SourceLocation NamespaceLoc,
                                     SourceLocation LBrace,
                                     DeclarationName Name) {
   return new(C) NamespaceDecl(NamespaceLoc, LBrace, Name);
}

CompoundDecl::CompoundDecl(SourceLocation LBraceLoc,
                           bool Transparent)
   : Decl(CompoundDeclID), DeclContext(CompoundDeclID),
     SR(LBraceLoc), Transparent(Transparent)
{}

CompoundDecl* CompoundDecl::Create(ASTContext &C,
                                   SourceLocation LBraceLoc,
                                   bool Transparent) {
   return new(C) CompoundDecl(LBraceLoc, Transparent);
}

CallableDecl::CallableDecl(DeclKind typeID,
                           AccessSpecifier am,
                           SourceLocation DefLoc,
                           DeclarationName Name,
                           SourceType returnType,
                           llvm::ArrayRef<FuncArgDecl*> args,
                           Statement* body,
                           OperatorInfo op,
                           std::vector<TemplateParamDecl*> &&templateParams)
   : NamedDecl(typeID, am, Name),
     DeclContext(typeID),
     DefLoc(DefLoc), returnType(returnType), NumArgs((unsigned)args.size()),
     body(body), templateParams(move(templateParams)),
     op(op), Flags(0)
{
   for (auto &arg : args) {
      arg->setLexicalContext(this);
      (void)makeDeclAvailable(arg);
   }

   std::copy(args.begin(), args.end(), arg_begin());
}

CallableDecl::arg_iterator CallableDecl::arg_begin()
{
   unsigned Size;
   switch (getDeclKind()) {
   case DeclKind::FunctionDeclID: Size = sizeof(FunctionDecl); break;
   case DeclKind::MethodDeclID: Size = sizeof(MethodDecl); break;
   case DeclKind::InitDeclID: Size = sizeof(InitDecl); break;
   case DeclKind::DeinitDeclID: Size = sizeof(DeinitDecl); break;
   case DeclKind::EnumCaseDeclID: Size = sizeof(EnumCaseDecl); break;
   default:
      llvm_unreachable("not a callable decl!");
   }

   return reinterpret_cast<arg_iterator>(reinterpret_cast<char*>(this) + Size);
}

CallableDecl::const_arg_iterator CallableDecl::arg_begin() const
{
   unsigned Size;
   switch (getDeclKind()) {
   case DeclKind::FunctionDeclID: Size = sizeof(FunctionDecl); break;
   case DeclKind::MethodDeclID: Size = sizeof(MethodDecl); break;
   case DeclKind::InitDeclID: Size = sizeof(InitDecl); break;
   case DeclKind::DeinitDeclID: Size = sizeof(DeinitDecl); break;
   case DeclKind::EnumCaseDeclID: Size = sizeof(EnumCaseDecl); break;
   default:
      llvm_unreachable("not a callable decl!");
   }

   return reinterpret_cast<const_arg_iterator>(
      reinterpret_cast<const char*>(this) + Size);
}

SourceRange CallableDecl::getSourceRange() const
{
   SourceLocation End;
   if (body) {
      End = body->getSourceRange().getEnd();
   }
   else if (returnType && returnType.getTypeExpr()) {
      End = returnType.getTypeExpr()->getSourceRange().getEnd();
   }
   else {
      End = DefLoc;
   }

   return SourceRange(DefLoc, End);
}

void CallableDecl::createFunctionType(SemaPass &SP, unsigned flags, bool lambda)
{
   if (functionType)
      return;

   QualType retTy;
   if (isa<InitDecl>(this)) {
      retTy = SP.getContext().getVoidType();
   }
   else if (isa<DeinitDecl>(this)) {
      retTy = SP.getContext().getVoidType();
   }
   else if (isa<EnumCaseDecl>(this)) {
      retTy = SP.getContext().getRecordType(getRecord());
   }
   else {
      retTy = returnType.getResolvedType();
   }

   std::vector<QualType> args;
   for (auto &arg : getArgs())
      args.push_back(arg->getType());

   if (isVararg())
      flags |= FunctionType::Vararg;

   if (isCstyleVararg())
      flags |= FunctionType::CStyleVararg;

   functionType = SP.getContext().getFunctionType(retTy, args, flags,
                                                  lambda);
}

bool CallableDecl::isNonStaticMethod() const
{
   if (auto M = dyn_cast<MethodDecl>(this))
      return !M->isStatic();

   return false;
}

bool CallableDecl::willHaveDefinition() const
{
   if (body) return true;
   if (auto M = dyn_cast<MethodDecl>(this)) {
      if (M->getBodyTemplate())
         return true;
   }

   // FIXME loaded from module
   return false;
}

KnownFunction CallableDecl::getKnownFnKind()
{
   checkKnownFnKind();
   return knownFnKind;
}

bool CallableDecl::isKnownFunction()
{
   return getKnownFnKind() != KnownFunction::None;
}

void CallableDecl::checkKnownFnKind()
{
   if (knownFnKind != KnownFunction::Unchecked)
      return;

   if (!Name.isSimpleIdentifier()) {
      knownFnKind = KnownFunction::None;
      return;
   }

   knownFnKind = llvm::StringSwitch<KnownFunction>(getName())
      .Case("__cdot_malloc", KnownFunction::Malloc)
      .Case("malloc", KnownFunction::Malloc)
      .Case("__cdot_free", KnownFunction::Free)
      .Case("free", KnownFunction::Free)
      .Case("printf", KnownFunction::Printf)
      .Case("memcpy", KnownFunction::MemCpy)
      .Case("memset", KnownFunction::MemSet)
      .Case("memcmp", KnownFunction::MemCmp)
      .Case("exit", KnownFunction::Exit)
      .Case("abort", KnownFunction::Abort)
      .Case("system", KnownFunction::System)
      .Case("srand", KnownFunction::Srand)
      .Case("rand", KnownFunction::Rand)
      .Case("time", KnownFunction::Time)
#     ifndef _WIN32
      .Case("sleep", KnownFunction::Sleep)
#     endif
      .Case("isalpha", KnownFunction::IsAlpha)
      .Case("isdigit", KnownFunction::IsDigit)
      .Case("isprint", KnownFunction::IsPrint)
      .Case("llvm.sqrt.f32", KnownFunction::llvm_sqrt_f32)
      .Case("llvm.sqrt.f64", KnownFunction::llvm_sqrt_f64)
      .Case("llvm.powi.f32", KnownFunction::llvm_powi_f32)
      .Case("llvm.powi.f64", KnownFunction::llvm_powi_f64)
      .Case("llvm.pow.f32", KnownFunction::llvm_pow_f32)
      .Case("llvm.pow.f64", KnownFunction::llvm_pow_f64)
      .Case("llvm.log10.f64", KnownFunction::llvm_log10_f64)
      .Case("llvm.log2.f64", KnownFunction::llvm_log2_f64)
      .Case("llvm.log.f64", KnownFunction::llvm_log_f64)
      .Case("llvm.floor.f64", KnownFunction::llvm_floor_f64)
      .Case("llvm.ceil.f64", KnownFunction::llvm_ceil_f64)
      .Case("llvm.floor.f32", KnownFunction::llvm_floor_f32)
      .Case("llvm.ceil.f32", KnownFunction::llvm_ceil_f32)
      .Default(KnownFunction::None);
}

bool CallableDecl::isInitializerOfTemplate() const
{
   auto Init = dyn_cast<InitDecl>(this);
   return Init && Init->getRecord()->isTemplate();
}

bool CallableDecl::isCaseOfTemplatedEnum() const
{
   auto Case = dyn_cast<EnumCaseDecl>(this);
   return Case && Case->getRecord()->isTemplate();
}

FunctionDecl::FunctionDecl(AccessSpecifier am,
                           SourceLocation DefLoc,
                           DeclarationName Name,
                           llvm::ArrayRef<FuncArgDecl*> args,
                           SourceType returnType,
                           Statement* body,
                           OperatorInfo op,
                           std::vector<TemplateParamDecl*> &&templateParams)
   : CallableDecl(FunctionDeclID, am, DefLoc, Name, returnType,
                  move(args), body, op, move(templateParams))
{

}

FunctionDecl* FunctionDecl::Create(ASTContext &C,
                                   AccessSpecifier am,
                                   SourceLocation DefLoc,
                                   DeclarationName Name,
                                   llvm::ArrayRef<FuncArgDecl*> args,
                                   SourceType returnType,
                                   Statement *body,
                                   OperatorInfo op,
                                   std::vector<TemplateParamDecl*> &&templateParams) {
   void *Mem = C.Allocate(sizeof(FunctionDecl)
                             + args.size() * sizeof(FuncArgDecl*),
                          alignof(FunctionDecl));

   return new(Mem) FunctionDecl(am, DefLoc, Name, move(args), returnType, body,
                                op, move(templateParams));
}

TypedefDecl::TypedefDecl(AccessSpecifier access,
                         SourceLocation Loc,
                         DeclarationName Name,
                         SourceType origin,
                         std::vector<TemplateParamDecl*> &&templateParams)
   : NamedDecl(TypedefDeclID, access, Name),
     Loc(Loc), origin(origin), templateParams(move(templateParams))
{}

TypedefDecl*
TypedefDecl::Create(ASTContext &C,
                    AccessSpecifier access,
                    SourceLocation Loc,
                    DeclarationName Name,
                    SourceType origin,
                    std::vector<TemplateParamDecl*> &&templateParams) {
   return new(C) TypedefDecl(access, Loc, Name, origin, move(templateParams));
}

AliasDecl::AliasDecl(SourceLocation Loc,
                     AccessSpecifier AccessSpec,
                     DeclarationName Name,
                     SourceType Type,
                     StaticExpr* aliasExpr,
                     llvm::ArrayRef<TemplateParamDecl*> templateParams)
   : NamedDecl(AliasDeclID, AccessSpec, Name),
     DeclContext(AliasDeclID),
     Loc(Loc), Type(Type), aliasExpr(aliasExpr),
     NumParams((unsigned)templateParams.size())
{
   std::copy(templateParams.begin(), templateParams.end(),
             getTrailingObjects<TemplateParamDecl*>());
}

AliasDecl* AliasDecl::Create(ASTContext &C,
                             SourceLocation Loc,
                             AccessSpecifier AccessSpec,
                             DeclarationName Name,
                             SourceType Type,
                             StaticExpr *aliasExpr,
                             llvm::ArrayRef<TemplateParamDecl*> templateParams){
   auto Size = totalSizeToAlloc<TemplateParamDecl*>(templateParams.size());
   void *Mem = C.Allocate(Size, alignof(AliasDecl));

   return new(Mem) AliasDecl(Loc, AccessSpec, Name, Type, aliasExpr,
                             templateParams);
}

SourceRange AliasDecl::getSourceRange() const
{
   return SourceRange(Loc,
                      aliasExpr ? aliasExpr->getSourceRange().getEnd()
                                : Loc);
}

const Variant& AliasDecl::getVal() const
{
   return aliasExpr->getEvaluatedExpr();
}

RecordDecl::RecordDecl(DeclKind typeID,
                       AccessSpecifier access,
                       SourceLocation KeywordLoc,
                       DeclarationName Name,
                       std::vector<SourceType> &&conformanceTypes,
                       std::vector<TemplateParamDecl*> &&templateParams)
   : NamedDecl(typeID, access, Name),
     DeclContext(typeID),
     KeywordLoc(KeywordLoc), conformanceTypes(move(conformanceTypes)),
     templateParams(move(templateParams)),
     manualAlignment(false), opaque(false),
     implicitlyEquatable(false), implicitlyHashable(false),
     implicitlyStringRepresentable(false)
{}

void RecordDecl::addInnerRecord(RecordDecl *R)
{
   innerRecords.insert(R);
}

DeclContext::AddDeclResultKind RecordDecl::addDecl(NamedDecl *decl)
{
   decl->setLexicalContext(this);

   switch (decl->getKind()) {
   case InitDeclID: {
      auto I = cast<InitDecl>(decl);
      if (I->getArgs().empty())
         if (auto S = dyn_cast<StructDecl>(this))
            S->setParameterlessConstructor(I);

      goto case_method;
   }
   case DeinitDeclID:
      deinitializer = cast<DeinitDecl>(decl);
      goto case_method;
   case MethodDeclID:
   case_method: {
      auto M = cast<MethodDecl>(decl);
      if (!M->getMethodID())
         M->setMethodID(lastMethodID++);

      break;
   }
   case FieldDeclID: {
      auto F = cast<FieldDecl>(decl);
      if (!F->isStatic()) {
         auto S = cast<StructDecl>(this);
         S->StoredFields.push_back(F);
      }

      break;
   }
   case EnumCaseDeclID: {
      auto E = cast<EnumCaseDecl>(decl);
      auto EDecl = cast<EnumDecl>(this);
      EDecl->Unpopulated = false;

      if (E->getArgs().size() > EDecl->maxAssociatedTypes)
         EDecl->maxAssociatedTypes = E->getArgs().size();

      break;
   }
   case StructDeclID:
   case ClassDeclID:
   case EnumDeclID:
   case UnionDeclID:
   case ProtocolDeclID:
      addInnerRecord(cast<RecordDecl>(decl));
      break;
   default:
      break;
   }

   return DeclContext::addDecl(decl);
}

#define CDOT_RECORD_IS_X(Name) \
bool RecordDecl::is##Name() const { return isa<Name##Decl>(this); }

CDOT_RECORD_IS_X(Struct)
CDOT_RECORD_IS_X(Class)
CDOT_RECORD_IS_X(Enum)
CDOT_RECORD_IS_X(Union)
CDOT_RECORD_IS_X(Protocol)

#undef CDOT_RECORD_IS_X

bool RecordDecl::isRawEnum() const
{
   if (auto E = dyn_cast<EnumDecl>(this))
      return E->getMaxAssociatedTypes() == 0;

   return false;
}

int RecordDecl::getNameSelector() const
{
   switch (kind) {
   case ClassDeclID: return 0;
   case StructDeclID: return 1;
   case EnumDeclID: return 2;
   case UnionDeclID: return 3;
   case ProtocolDeclID: return 4;
   default:
      llvm_unreachable("bad record decl");
   }
}

AssociatedTypeDecl* RecordDecl::getAssociatedType(DeclarationName name,
                                                  ProtocolDecl *P) const {
   auto AT = dyn_cast_or_null<AssociatedTypeDecl>(lookupSingle(name));
   if (AT) {
      if (!AT->getProto() || AT->getProto() == P)
         return AT;
   }

   return nullptr;
}

MethodDecl* RecordDecl::getConversionOperator(QualType toType) const
{
   auto &Ctx = getASTCtx();
   auto DeclName = Ctx.getDeclNameTable().getConversionOperatorName(toType);

   return lookupSingle<MethodDecl>(DeclName);
}

MethodDecl* RecordDecl::getComparisonOperator(QualType withType) const
{
   auto &Ctx = getASTCtx();
   auto *Name = &Ctx.getIdentifiers().get("==");
   auto DeclName = Ctx.getDeclNameTable().getInfixOperatorName(*Name);

   return lookupSingle<MethodDecl>(DeclName);
}

llvm::ArrayRef<ExtensionDecl*> RecordDecl::getExtensions() const
{
   return getASTCtx().getExtensions(this);
}

void RecordDecl::addExtension(ExtensionDecl *E) const
{
   getASTCtx().addExtension(this, E);
}

bool RecordDecl::hasMethodWithName(DeclarationName name) const
{
   for (auto &decl : getDecls()) {
      auto Method = dyn_cast<MethodDecl>(decl);
      if (Method && Method->getDeclName() == name)
         return true;
   }

   if (auto C = dyn_cast<ClassDecl>(this)) {
      if (auto P = C->getParentClass()) {
         return P->hasMethodWithName(name);
      }
   }

   return false;
}

bool RecordDecl::hasMethodTemplate(DeclarationName name) const
{
   for (auto &decl : getDecls()) {
      auto Method = dyn_cast<MethodDecl>(decl);
      if (Method && Method->getDeclName() == name && Method->isTemplate())
         return true;
   }

   if (auto C = dyn_cast<ClassDecl>(this)) {
      if (auto P = C->getParentClass()) {
         return P->hasMethodTemplate(name);
      }
   }

   return false;
}

PropDecl* RecordDecl::getProperty(DeclarationName name) const
{
   return dyn_cast_or_null<PropDecl>(lookupSingle(name));
}

FieldDecl* RecordDecl::getField(DeclarationName name) const
{
   return dyn_cast_or_null<FieldDecl>(lookupSingle(name));
}

sema::ResolvedTemplateArg const* RecordDecl::getTemplateArg(
   llvm::StringRef name) const
{
   assert(isInstantiation());
   return instantiationInfo->templateArgs.getNamedArg(name);
}

StructDecl::StructDecl(AccessSpecifier access,
                       SourceLocation KeywordLoc,
                       DeclarationName Name,
                       std::vector<SourceType> &&conformanceTypes,
                       std::vector<TemplateParamDecl*> &&templateParams)
   : RecordDecl(StructDeclID, access, KeywordLoc, Name, move(conformanceTypes),
                move(templateParams))
{}

StructDecl* StructDecl::Create(ASTContext &C,
                               AccessSpecifier access,
                               SourceLocation KeywordLoc,
                               DeclarationName Name,
                               std::vector<SourceType> &&conformanceTypes,
                               std::vector<TemplateParamDecl*>&&templateParams){
   return new(C) StructDecl(access, KeywordLoc, Name, move(conformanceTypes),
                            move(templateParams));
}

StructDecl::StructDecl(DeclKind typeID,
                       AccessSpecifier access,
                       SourceLocation KeywordLoc,
                       DeclarationName Name,
                       std::vector<SourceType> &&conformanceTypes,
                       std::vector<TemplateParamDecl*> &&templateParams)
   : RecordDecl(typeID, access, KeywordLoc, Name, move(conformanceTypes),
                move(templateParams))
{}

ClassDecl::ClassDecl(AccessSpecifier access,
                     SourceLocation KeywordLoc,
                     DeclarationName Name,
                     std::vector<SourceType> &&conformanceTypes,
                     std::vector<TemplateParamDecl*> &&templateParams,
                     SourceType parentClass,
                     bool isAbstract)
   : StructDecl(ClassDeclID, access, KeywordLoc, Name, move(conformanceTypes),
                move(templateParams)),
     parentType(parentClass), IsAbstract(isAbstract)
{}

ClassDecl* ClassDecl::Create(ASTContext &C,
                             AccessSpecifier access,
                             SourceLocation KeywordLoc,
                             DeclarationName Name,
                             std::vector<SourceType> &&conformanceTypes,
                             std::vector<TemplateParamDecl *> &&templateParams,
                             SourceType parentClass, bool isAbstract) {
   return new(C) ClassDecl(access, KeywordLoc, Name, move(conformanceTypes),
                           move(templateParams), parentClass, isAbstract);
}

void ClassDecl::inherit(ast::ClassDecl *C)
{
   parentClass = C;
   StoredFields.insert(StoredFields.begin(),
                       C->StoredFields.begin(),
                       C->StoredFields.end());
}

EnumDecl::EnumDecl(AccessSpecifier access,
                   SourceLocation KeywordLoc,
                   DeclarationName Name,
                   std::vector<SourceType> &&conformanceTypes,
                   std::vector<TemplateParamDecl*> &&templateParams,
                   SourceType rawType)
   : RecordDecl(EnumDeclID, access, KeywordLoc, Name, move(conformanceTypes),
                move(templateParams)),
     rawType(rawType)
{}

EnumDecl* EnumDecl::Create(ASTContext &C,
                           AccessSpecifier access,
                           SourceLocation KeywordLoc,
                           DeclarationName Name,
                           std::vector<SourceType> &&conformanceTypes,
                           std::vector<TemplateParamDecl *> &&templateParams,
                           SourceType rawType) {
   return new(C) EnumDecl(access, KeywordLoc, Name, move(conformanceTypes),
                          move(templateParams), rawType);
}

EnumCaseDecl* EnumDecl::hasCase(DeclarationName name)
{
   return lookupSingle<EnumCaseDecl>(name);
}

UnionDecl::UnionDecl(AccessSpecifier access,
                     SourceLocation KeywordLoc,
                     DeclarationName Name,
                     std::vector<SourceType> &&conformanceTypes,
                     std::vector<TemplateParamDecl*> &&templateParams)
   : RecordDecl(UnionDeclID, access, KeywordLoc, Name, move(conformanceTypes),
                move(templateParams))
{}

UnionDecl* UnionDecl::Create(ASTContext &C,
                             AccessSpecifier access,
                             SourceLocation KeywordLoc,
                             DeclarationName Name,
                             std::vector<SourceType> &&conformanceTypes,
                             std::vector<TemplateParamDecl*> &&templateParams) {
   return new(C) UnionDecl(access, KeywordLoc, Name, move(conformanceTypes),
                           move(templateParams));
}

ProtocolDecl::ProtocolDecl(AccessSpecifier access,
                           SourceLocation KeywordLoc,
                           DeclarationName Name,
                           std::vector<SourceType> &&conformanceTypes,
                           std::vector<TemplateParamDecl*> &&templateParams)
   : RecordDecl(ProtocolDeclID, access, KeywordLoc, Name, move(conformanceTypes),
                move(templateParams))
{}

ProtocolDecl* ProtocolDecl::Create(ASTContext &C,
                                   AccessSpecifier access,
                                   SourceLocation KeywordLoc,
                                   DeclarationName Name,
                                   std::vector<SourceType> &&conformanceTypes,
                                   std::vector<TemplateParamDecl*>
                                                            &&templateParams) {
   return new(C) ProtocolDecl(access, KeywordLoc, Name, move(conformanceTypes),
                              move(templateParams));
}

ExtensionDecl::ExtensionDecl(AccessSpecifier access,
                             SourceLocation KeywordLoc,
                             SourceType ExtendedType,
                             llvm::ArrayRef<SourceType> conformanceTypes)
   : NamedDecl(ExtensionDeclID, access, DeclarationName()),
     DeclContext(ExtensionDeclID),
     ExtLoc(KeywordLoc),
     ExtendedType(ExtendedType),
     NumConformances((unsigned)conformanceTypes.size())
{
   std::copy(conformanceTypes.begin(), conformanceTypes.end(),
             reinterpret_cast<SourceType*>(this + 1));
}

ExtensionDecl::ExtensionDecl(AccessSpecifier access,
                             SourceLocation KeywordLoc,
                             RecordDecl *R,
                             llvm::ArrayRef<SourceType> conformanceTypes)
   : NamedDecl(ExtensionDeclID, access, DeclarationName()),
     DeclContext(ExtensionDeclID),
     ExtLoc(KeywordLoc),
     ExtendedRecord(R),
     NumConformances((unsigned)conformanceTypes.size())
{
   std::copy(conformanceTypes.begin(), conformanceTypes.end(),
             reinterpret_cast<SourceType*>(this + 1));
}

ExtensionDecl *ExtensionDecl::Create(ASTContext &C,
                                     AccessSpecifier access,
                                     SourceLocation KeywordLoc,
                                     SourceType ExtendedType,
                                     llvm::ArrayRef<SourceType>conformanceTypes){
   auto Size = totalSizeToAlloc<SourceType>(conformanceTypes.size());
   void *Mem = C.Allocate(Size, alignof(ExtensionDecl));

   return new(Mem) ExtensionDecl(access, KeywordLoc, ExtendedType,
                                 move(conformanceTypes));
}

ExtensionDecl *ExtensionDecl::Create(ASTContext &C,
                                     AccessSpecifier access,
                                     SourceLocation KeywordLoc,
                                     RecordDecl *R,
                                     llvm::ArrayRef<SourceType>conformanceTypes){
   auto Size = totalSizeToAlloc<SourceType>(conformanceTypes.size());
   void *Mem = C.Allocate(Size, alignof(ExtensionDecl));

   return new(Mem) ExtensionDecl(access, KeywordLoc, R, move(conformanceTypes));
}

MethodDecl::MethodDecl(AccessSpecifier access,
                       SourceLocation DefLoc,
                       DeclarationName Name,
                       SourceType returnType,
                       llvm::ArrayRef<FuncArgDecl*> args,
                       std::vector<TemplateParamDecl *> &&templateParams,
                       Statement *body,
                       bool isStatic)
   : CallableDecl(MethodDeclID, access, DefLoc, Name, returnType, move(args),
                  body, OperatorInfo(), move(templateParams)) {
   setFlag(Abstract, !body);
   setDeclFlag(DF_Static, isStatic);
}

MethodDecl* MethodDecl::Create(ASTContext &C,
                               AccessSpecifier access,
                               SourceLocation DefLoc,
                               DeclarationName Name,
                               SourceType returnType,
                               llvm::ArrayRef<FuncArgDecl*> args,
                               std::vector<TemplateParamDecl*> &&templateParams,
                               Statement *body, bool isStatic) {
   void *Mem = C.Allocate(sizeof(MethodDecl)
                          + args.size() * sizeof(FuncArgDecl*),
                          alignof(MethodDecl));

   return new(Mem) MethodDecl(access, DefLoc, Name, returnType, args,
                              move(templateParams), body, isStatic);
}

MethodDecl::MethodDecl(AccessSpecifier access,
                       SourceLocation DefLoc,
                       DeclarationName OperatorName,
                       SourceType returnType,
                       llvm::ArrayRef<FuncArgDecl*> args,
                       std::vector<TemplateParamDecl *> &&templateParams,
                       Statement *body,
                       OperatorInfo op, bool isStatic)
   : CallableDecl(MethodDeclID, access, DefLoc, OperatorName, returnType,
                  move(args), body, op, move(templateParams)) {
   setFlag(Abstract, !body);
   setDeclFlag(DF_Static, isStatic);
}

MethodDecl* MethodDecl::CreateOperator(ASTContext &C,
                                       AccessSpecifier access,
                                       SourceLocation DefLoc,
                                       DeclarationName OperatorName,
                                       SourceType returnType,
                                       llvm::ArrayRef<FuncArgDecl*> args,
                                       std::vector<TemplateParamDecl*>
                                                               &&templateParams,
                                       Statement *body,
                                       OperatorInfo op, bool isStatic) {
   void *Mem = C.Allocate(sizeof(MethodDecl)
                          + args.size() * sizeof(FuncArgDecl*),
                          alignof(MethodDecl));

   return new(Mem) MethodDecl(access, DefLoc, OperatorName, returnType,
                              args, move(templateParams), body, op, isStatic);
}

MethodDecl::MethodDecl(AccessSpecifier access,
                       SourceLocation DefLoc,
                       SourceType returnType,
                       llvm::ArrayRef<FuncArgDecl*> args,
                       std::vector<TemplateParamDecl *> &&templateParams,
                       Statement *body)
   : CallableDecl(MethodDeclID, access, DefLoc, DeclarationName(),
                  returnType, move(args), body, OperatorInfo(),
                  move(templateParams)) {
   setFlag(Abstract, !body);
   setFlag(ConvOp, true);
}

MethodDecl* MethodDecl::CreateConversionOp(ASTContext &C,
                                           AccessSpecifier access,
                                           SourceLocation DefLoc,
                                           SourceType returnType,
                                           llvm::ArrayRef<FuncArgDecl*> args,
                                           std::vector<TemplateParamDecl*>
                                                               &&templateParams,
                                           Statement *body) {
   void *Mem = C.Allocate(sizeof(MethodDecl)
                          + args.size() * sizeof(FuncArgDecl*),
                          alignof(MethodDecl));

   return new(Mem) MethodDecl(access, DefLoc, returnType, args,
                              move(templateParams), body);
}

MethodDecl::MethodDecl(DeclKind typeID,
                       AccessSpecifier access,
                       SourceLocation DefLoc,
                       DeclarationName Name,
                       SourceType returnType,
                       llvm::ArrayRef<FuncArgDecl*> args,
                       std::vector<TemplateParamDecl *> &&templateParams,
                       Statement *body)
   : CallableDecl(typeID, access, DefLoc, Name,
                  returnType, move(args), body, OperatorInfo(),
                  move(templateParams)) {
   setFlag(Abstract, !body);
}

InitDecl::InitDecl(AccessSpecifier am,
                   SourceLocation Loc,
                   DeclarationName Name)
   : MethodDecl(InitDeclID, am, Loc, Name, SourceType(), {}, {}, {}),
     ExplicitMemberwise(true)
{
   setDeclFlag(DF_Static, true);
}

InitDecl* InitDecl::CreateMemberwise(ASTContext &C,
                                     AccessSpecifier am,
                                     SourceLocation Loc,
                                     DeclarationName Name) {
   return new(C) InitDecl(am, Loc, Name);
}

InitDecl::InitDecl(AccessSpecifier am,
                   SourceLocation Loc,
                   llvm::ArrayRef<FuncArgDecl*> args,
                   std::vector<TemplateParamDecl *> &&templateParams,
                   Statement *body, DeclarationName Name)
   : MethodDecl(InitDeclID, am, Loc, Name, SourceType(), move(args),
                move(templateParams), body), ExplicitMemberwise(false) {
   setDeclFlag(DF_Static, true);
}

InitDecl* InitDecl::Create(ASTContext &C,
                           AccessSpecifier am,
                           SourceLocation Loc,
                           llvm::ArrayRef<FuncArgDecl*> args,
                           std::vector<TemplateParamDecl *> &&templateParams,
                           Statement *body,
                           DeclarationName Name) {
   void *Mem = C.Allocate(sizeof(InitDecl)
                          + args.size() * sizeof(FuncArgDecl*),
                          alignof(InitDecl));

   return new(Mem) InitDecl(am, Loc, args, move(templateParams), body, Name);
}

DeinitDecl::DeinitDecl(SourceLocation Loc,
                       Statement *body,
                       DeclarationName Name)
   : MethodDecl(DeinitDeclID, AccessSpecifier::Public, Loc,
                Name, SourceType(), {}, {}, body)
{
   setDeclFlag(DF_Static, true);
}

DeinitDecl* DeinitDecl::Create(ASTContext &C,
                               SourceLocation Loc,
                               Statement *body,
                               DeclarationName Name) {
   return new(C) DeinitDecl(Loc, body, Name);
}

FieldDecl::FieldDecl(AccessSpecifier Access,
                     SourceLocation VarOrLetLoc,
                     SourceLocation ColonLoc,
                     DeclarationName Name,
                     SourceType Type,
                     bool IsStatic,
                     bool IsConst,
                     Expression* DefaultVal)
   : VarDecl(FieldDeclID, Access, VarOrLetLoc, ColonLoc, IsConst, Name,
             Type, DefaultVal)
{
   setDeclFlag(DF_Static, IsStatic);
}

FieldDecl* FieldDecl::Create(ASTContext &C,
                             AccessSpecifier Access,
                             SourceLocation VarOrLetLoc,
                             SourceLocation ColonLoc,
                             DeclarationName Name,
                             SourceType Type,
                             bool IsStatic, bool IsConst,
                             Expression *DefaultVal) {
   return new(C) FieldDecl(Access, VarOrLetLoc, ColonLoc, Name, Type, IsStatic,
                           IsConst, DefaultVal);
}

AssociatedTypeDecl::AssociatedTypeDecl(SourceLocation Loc,
                                       IdentifierInfo *ProtoSpec,
                                       DeclarationName Name,
                                       SourceType actualType)
   : NamedDecl(AssociatedTypeDeclID, AccessSpecifier::Public, Name),
     protocolSpecifier(ProtoSpec), actualType(actualType)
{

}

AssociatedTypeDecl* AssociatedTypeDecl::Create(ASTContext &C,
                                               SourceLocation Loc,
                                               IdentifierInfo *ProtoSpec,
                                               DeclarationName Name,
                                               SourceType actualType) {
   return new(C) AssociatedTypeDecl(Loc, ProtoSpec, Name, actualType);
}

bool AssociatedTypeDecl::isImplementation() const
{
   return !isa<ProtocolDecl>(getNonTransparentDeclContext());
}

PropDecl::PropDecl(AccessSpecifier access,
                   SourceRange Loc,
                   DeclarationName propName,
                   SourceType type,
                   bool isStatic, bool hasDefinition, bool hasGetter,
                   bool hasSetter,
                   CompoundStmt *getter, CompoundStmt *setter,
                   IdentifierInfo *newValName)
   : NamedDecl(PropDeclID, access, propName),
     Loc(Loc), type(type), getterBody(getter), setterBody(setter),
     HasGetter(hasGetter), HasSetter(hasSetter), newValName(newValName)
{
   setDeclFlag(DF_Static, isStatic);
   setDeclFlag(DF_HasDefinition, hasDefinition);
}

PropDecl* PropDecl::Create(ASTContext &C,
                           AccessSpecifier access,
                           SourceRange Loc,
                           DeclarationName propName,
                           SourceType type,
                           bool isStatic, bool hasDefinition, bool hasGetter,
                           bool hasSetter,
                           CompoundStmt *getter, CompoundStmt *setter,
                           IdentifierInfo *newValName) {
   return new(C) PropDecl(access, Loc, propName, type, isStatic,
                          hasDefinition, hasGetter, hasSetter, getter,
                          setter, newValName);
}

EnumCaseDecl::EnumCaseDecl(AccessSpecifier AS,
                           SourceLocation CaseLoc,
                           SourceLocation IdentLoc,
                           DeclarationName Name,
                           StaticExpr *rawValue,
                           llvm::ArrayRef<FuncArgDecl*> args)
   : CallableDecl(EnumCaseDeclID, AS, CaseLoc, Name, SourceType(),
                  args, nullptr, OperatorInfo(), {}),
     CaseLoc(CaseLoc), IdentLoc(IdentLoc), rawValExpr(rawValue)
{

}

EnumCaseDecl* EnumCaseDecl::Create(ASTContext &C,
                                   AccessSpecifier AS,
                                   SourceLocation CaseLoc,
                                   SourceLocation IdentLoc,
                                   DeclarationName Name,
                                   StaticExpr *rawValue,
                                   llvm::ArrayRef<FuncArgDecl*> args) {
   void *Mem = C.Allocate(sizeof(EnumCaseDecl)
                          + args.size() * sizeof(FuncArgDecl*),
                          alignof(EnumCaseDecl));

   return new(Mem) EnumCaseDecl(AS, CaseLoc, IdentLoc, Name, rawValue, args);
}

StaticIfDecl::StaticIfDecl(SourceLocation StaticLoc,
                           SourceLocation RBRaceLoc,
                           StaticExpr *condition,
                           CompoundDecl *IfDecl,
                           CompoundDecl *ElseDecl)
   : Decl(StaticIfDeclID),
     StaticLoc(StaticLoc), RBRaceLoc(RBRaceLoc),
     condition(condition), IfDecl(IfDecl), ElseDecl(ElseDecl)
{

}

StaticIfDecl* StaticIfDecl::Create(ASTContext &C,
                                   SourceLocation StaticLoc,
                                   SourceLocation RBRaceLoc,
                                   StaticExpr *condition,
                                   CompoundDecl *IfDecl,
                                   CompoundDecl *ElseDecl) {
   return new(C) StaticIfDecl(StaticLoc, RBRaceLoc, condition, IfDecl,
                              ElseDecl);
}

StaticForDecl::StaticForDecl(SourceLocation StaticLoc,
                             SourceLocation RBRaceLoc,
                             IdentifierInfo *elementName,
                             StaticExpr *range,
                             CompoundDecl *BodyDecl)
   : Decl(StaticForDeclID),
     StaticLoc(StaticLoc), RBRaceLoc(RBRaceLoc),
     elementName(elementName), range(range), BodyDecl(BodyDecl)
{

}

StaticForDecl* StaticForDecl::Create(ASTContext &C,
                                     SourceLocation StaticLoc,
                                     SourceLocation RBRaceLoc,
                                     IdentifierInfo *elementName,
                                     StaticExpr *range,
                                     CompoundDecl *BodyDecl) {
   return new(C) StaticForDecl(StaticLoc, RBRaceLoc, elementName, range,
                               BodyDecl);
}

StaticAssertStmt::StaticAssertStmt(SourceLocation Loc,
                                   SourceRange Parens,
                                   StaticExpr* expr,
                                   std::string &&message)
   : Decl(StaticAssertStmtID),
     Loc(Loc), Parens(Parens),
     expr(expr), message(move(message))
{}

StaticAssertStmt* StaticAssertStmt::Create(ASTContext &C,
                                           SourceLocation Loc,
                                           SourceRange Parens,
                                           StaticExpr *expr,
                                           std::string &&message) {
   return new(C) StaticAssertStmt(Loc, Parens, expr, move(message));
}

StaticPrintStmt::StaticPrintStmt(SourceLocation Loc,
                                 SourceRange Parens,
                                 StaticExpr* expr)
   : Decl(StaticPrintStmtID),
     Loc(Loc), Parens(Parens), expr(expr)
{}

StaticPrintStmt* StaticPrintStmt::Create(ASTContext &C,
                                         SourceLocation Loc,
                                         SourceRange Parens,
                                         StaticExpr *E) {
   return new(C) StaticPrintStmt(Loc, Parens, E);
}

MixinDecl::MixinDecl(SourceLocation Loc,
                     SourceRange Parens,
                     Expression *MixinExpr)
   : Decl(MixinDeclID),
     Loc(Loc), Parens(Parens), MixinExpr(MixinExpr)
{}

MixinDecl* MixinDecl::Create(ASTContext &C,
                             SourceLocation Loc,
                             SourceRange Parens,
                             Expression *MixinExpr) {
   return new(C) MixinDecl(Loc, Parens, MixinExpr);
}

} // namespace ast
} // namespace cdot