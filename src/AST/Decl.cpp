//
// Created by Jonas Zell on 26.01.18.
//

#include "Decl.h"

#include "AST/ASTContext.h"
#include "AST/Expression.h"
#include "AST/Statement.h"
#include "AST/Passes/SemanticAnalysis/Builtin.h"
#include "AST/Passes/SemanticAnalysis/SemaPass.h"

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

void Decl::setDeclContext(DeclContext *ctx)
{
   declContext = ctx;
   if (auto DC = dyn_cast<DeclContext>(this))
      DC->setParentCtxUnchecked(ctx);

   assert((!isa<GlobalDeclContext>(ctx) || isa<TranslationUnit>(this)));
}

void Decl::setDeclContextUnchecked(DeclContext *ctx)
{
   declContext = ctx;
}

SourceRange Decl::getSourceRange() const
{
   // FIXME
//   if constexpr (&Statement::getSourceRange == &SubClass::getSourceRange)
//      return SourceRange(loc);
//   else
//      return support::cast<SubClass>(this)->getSourceRange();

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
                     AccessModifier access,
                     DeclarationName DN)
   : Decl(typeID), access(access), Name(DN)
{}

llvm::ArrayRef<StaticExpr*> NamedDecl::getConstraints() const
{
   return getASTCtx().getConstraints(this);
}

TemplateParamDecl::TemplateParamDecl(IdentifierInfo *II,
                                     SourceType covariance,
                                     SourceType contravariance,
                                     Expression *defaultValue,
                                     SourceLocation TypeNameOrValueLoc,
                                     SourceLocation NameLoc,
                                     SourceLocation EllipsisLoc)
   : NamedDecl(TemplateParamDeclID, (AccessModifier)0, II),
     covariance(covariance), contravariance(contravariance),
     typeName(true), defaultValue(defaultValue),
     TypeNameOrValueLoc(TypeNameOrValueLoc), NameLoc(NameLoc),
     EllipsisLoc(EllipsisLoc)
{}

TemplateParamDecl::TemplateParamDecl(IdentifierInfo *II,
                                     SourceType valueType,
                                     Expression *defaultValue,
                                     SourceLocation TypeNameOrValueLoc,
                                     SourceLocation NameLoc,
                                     SourceLocation EllipsisLoc)
   : NamedDecl(TemplateParamDeclID, (AccessModifier)0, II),
     covariance(valueType), contravariance(nullptr),
     typeName(false),
     defaultValue(defaultValue),
     TypeNameOrValueLoc(TypeNameOrValueLoc), NameLoc(NameLoc),
     EllipsisLoc(EllipsisLoc)
{}

TemplateParamDecl* TemplateParamDecl::Create(ASTContext &C,
                                             IdentifierInfo *II,
                                             SourceType covariance,
                                             SourceType contravariance,
                                             Expression *defaultValue,
                                             SourceLocation TypeNameOrValueLoc,
                                             SourceLocation NameLoc,
                                             SourceLocation EllipsisLoc) {
   return new(C) TemplateParamDecl(II, covariance, contravariance,
                                   defaultValue, TypeNameOrValueLoc, NameLoc,
                                   EllipsisLoc);
}

TemplateParamDecl* TemplateParamDecl::Create(ASTContext &C,
                                             IdentifierInfo *II,
                                             SourceType valueType,
                                             Expression *defaultValue,
                                             SourceLocation TypeNameOrValueLoc,
                                             SourceLocation NameLoc,
                                             SourceLocation EllipsisLoc) {
   return new(C) TemplateParamDecl(II, valueType, defaultValue,
                                   TypeNameOrValueLoc, NameLoc, EllipsisLoc);
}

VarDecl::VarDecl(DeclKind id,
                 AccessModifier access,
                 SourceLocation VarOrLetLoc,
                 SourceLocation ColonLoc,
                 bool isConst,
                 IdentifierInfo *II,
                 SourceType type,
                 Expression* value)
   : NamedDecl(id, access, II),
     VarOrLetLoc(VarOrLetLoc), ColonLoc(ColonLoc),
     type(type), value(value), CanElideCopy(false)
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

LocalVarDecl::LocalVarDecl(AccessModifier access,
                           SourceLocation VarOrLetLoc,
                           SourceLocation ColonLoc,
                           bool isConst,
                           IdentifierInfo *II,
                           SourceType type,
                           Expression* value)
   : VarDecl(LocalVarDeclID, access, VarOrLetLoc, ColonLoc,
             isConst, II, type, value),
     IsNRVOCand(false)
{}

LocalVarDecl* LocalVarDecl::Create(ASTContext &C,
                                   AccessModifier access,
                                   SourceLocation VarOrLetLoc,
                                   SourceLocation ColonLoc,
                                   bool isConst,
                                   IdentifierInfo *II,
                                   SourceType type,
                                   Expression *value) {
   return new(C) LocalVarDecl(access, VarOrLetLoc, ColonLoc,
                              isConst, II, type, value);
}

GlobalVarDecl::GlobalVarDecl(AccessModifier access,
                             SourceLocation VarOrLetLoc,
                             SourceLocation ColonLoc,
                             bool isConst,
                             IdentifierInfo *II,
                             SourceType type,
                             Expression* value)
   : VarDecl(GlobalVarDeclID, access, VarOrLetLoc, ColonLoc,
             isConst, II, type, value)
{}

GlobalVarDecl* GlobalVarDecl::Create(ASTContext &C,
                                     AccessModifier access,
                                     SourceLocation VarOrLetLoc,
                                     SourceLocation ColonLoc,
                                     bool isConst,
                                     IdentifierInfo *II,
                                     SourceType type,
                                     Expression *value) {
   return new(C) GlobalVarDecl(access, VarOrLetLoc, ColonLoc,
                               isConst, II, type, value);
}

FuncArgDecl::FuncArgDecl(SourceLocation VarOrLetLoc,
                         SourceLocation ColonLoc,
                         IdentifierInfo *II,
                         SourceType argType,
                         Expression* defaultValue,
                         bool variadicArgPackExpansion,
                         bool isConst,
                         bool cstyleVararg)
   : VarDecl(FuncArgDeclID, AccessModifier::PUBLIC, VarOrLetLoc, ColonLoc,
             isConst, II, argType, defaultValue),
     VariadicArgPackExpansion(variadicArgPackExpansion),
     IsConst(isConst), Vararg(cstyleVararg), CstyleVararg(cstyleVararg)
{}

FuncArgDecl* FuncArgDecl::Create(ASTContext &C,
                                 SourceLocation VarOrLetLoc,
                                 SourceLocation ColonLoc,
                                 IdentifierInfo *II,
                                 SourceType argType,
                                 Expression *defaultValue,
                                 bool variadicArgPackExpansion,
                                 bool isConst, bool cstyleVararg) {
   return new(C) FuncArgDecl(VarOrLetLoc, ColonLoc, II, argType, defaultValue,
                             variadicArgPackExpansion, isConst, cstyleVararg);
}

void DeclContext::addDecl(Decl *decl)
{
   decl->setDeclContext(this);
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
   if (Name) {
      auto it = namedDecls.find(Name);
      if (it == namedDecls.end()) {
         namedDecls.try_emplace(Name, decl);
      }
      // don't check as-of-yet unnamed decls
      else {
         auto lookup = it->getSecond().getAsLookupResult();
         assert(!lookup.empty());

         if (lookup.front()->getKind() != decl->getKind())
            return ADR_DuplicateDifferentKind;

         if (!lookup.front()->isOverloadable())
            return ADR_Duplicate;

         it->getSecond().appendDecl(decl);
      }

      if (isTransparent())
         parentCtx->makeDeclAvailable(Name, decl);
   }

   addDecl((Decl*)decl);
   return ADR_Success;
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

void DeclContext::makeAllDeclsAvailable(DeclContext *Ctx)
{
   for (auto D : Ctx->getDecls())
      if (auto ND = support::dyn_cast<NamedDecl>(D))
         makeDeclAvailable(ND);
}

void DeclContext::makeDeclAvailable(NamedDecl *decl)
{
   return makeDeclAvailable(decl->getDeclName(), decl);
}

void DeclContext::makeDeclAvailable(DeclarationName Name, NamedDecl *decl)
{
   auto it = namedDecls.find(Name);
   if (it == namedDecls.end()) {
      namedDecls.try_emplace(Name, decl);
   }
   else {
      it->getSecond().appendDecl(decl);
   }
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

void DeclContext::addTransparentDecls()
{
   addTransparentDecls(*this);
}

void DeclContext::addTransparentDecls(DeclContext &Ctx)
{
   for (auto &D : Ctx.getDecls()) {
      if (auto SubCtx = dyn_cast<DeclContext>(D)) {
         if (SubCtx->isTransparent()) {
            makeAllDeclsAvailable(SubCtx);
            addTransparentDecls(*SubCtx);
         }
      }
   }
}

ASTContext& Decl::getASTCtx() const
{
   DeclContext const* ctx;
   if (isa<DeclContext>(this)) {
      ctx = cast<DeclContext>(this);
   }
   else {
      ctx = declContext;
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
   return support::dyn_cast<RecordDecl>(declContext);
}

bool NamedDecl::isExported() const
{
   if (access == AccessModifier::PRIVATE)
      return false;

   if (auto NS = support::dyn_cast<NamespaceDecl>(declContext))
      return !NS->isAnonymousNamespace();

   return true;
}

bool Decl::inAnonymousNamespace() const
{
   for (auto ctx = getDeclContext(); ctx; ctx = ctx->getParentCtx())
      if (auto NS = dyn_cast<NamespaceDecl>(ctx))
         if (NS->isAnonymousNamespace())
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
   return isa<TranslationUnit>(declContext);
}

bool NamedDecl::isOverloadable() const
{
   switch (kind) {
      case AliasDeclID:
      case FunctionDeclID:
      case MethodDeclID:
      case InitDeclID:
      // note that DeinitDeclID is missing - deinitializers cannot be overloaded
         return true;
      default:
         return false;
   }
}

bool NamedDecl::isTemplate() const
{
   return !getTemplateParams().empty();
}

bool NamedDecl::isTemplateOrInTemplate() const
{
   if (isTemplate())
      return true;

   switch (declContext->getDeclKind()) {
   case StructDeclID:
   case ClassDeclID:
   case EnumDeclID:
   case UnionDeclID:
   case ProtocolDeclID:
      return cast<RecordDecl>(declContext)->isTemplate();
   case NamespaceDeclID:
      return cast<NamespaceDecl>(declContext)->isTemplate();
   default:
      return false;
   }
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
   case EnumCaseDeclID:
      return cast<CallableDecl>(this)->getTemplateParams();
   default:
      return {};
   }
}

bool NamedDecl::isInstantiation() const
{
   switch (kind) {
   case AliasDeclID:
      return cast<AliasDecl>(this)->isInstantiation();
   case StructDeclID:
   case ClassDeclID:
   case EnumDeclID:
   case UnionDeclID:
      return cast<RecordDecl>(this)->isInstantiation();
   case FunctionDeclID:
   case MethodDeclID:
   case InitDeclID:
   case EnumCaseDeclID:
      return cast<CallableDecl>(this)->isInstantiation();
   default:
      return false;
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
      ND->setDeclContextUnchecked(parent);

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
   if (isa<TranslationUnit>(this) && !includeFile)
      return joinedName;

   llvm::raw_string_ostream OS(joinedName);
   for (auto ctx = declContext; ctx; ctx = ctx->getParentCtx()) {
      if (auto ND = dyn_cast<NamedDecl>(ctx)) {
         OS << ND->getJoinedName(join, includeFile);

         OS.flush();
         if (!joinedName.empty())
            OS << join;

         break;
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
   default:
      llvm_unreachable("not yet");
   }
}

TranslationUnit::TranslationUnit(ASTContext &ASTCtx,
                                 IdentifierInfo *fileName,
                                 size_t sourceId,
                                 llvm::ArrayRef<ImportStmt*> imports)
   : NamedDecl(TranslationUnitID, (AccessModifier)0, fileName),
     DeclContext(TranslationUnitID),
     ASTCtx(ASTCtx), sourceId(sourceId),
     numImports(unsigned(imports.size()))
{
   std::copy(imports.begin(), imports.end(), import_begin());
}

TranslationUnit* TranslationUnit::Create(ASTContext &ASTCtx,
                                         IdentifierInfo *fileName,
                                         size_t sourceId,
                                         llvm::ArrayRef<ImportStmt*> imports) {
   void *Mem = ASTCtx.Allocate(totalSizeToAlloc<ImportStmt*>(imports.size()),
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
                             IdentifierInfo *II,
                             bool isAnonymous)
   : NamedDecl(NamespaceDeclID, (AccessModifier)0, II),
     DeclContext(NamespaceDeclID),
     NamespaceLoc(NamespaceLoc), Braces(LBrace),
     anonymousNamespace(isAnonymous)
{

}

NamespaceDecl* NamespaceDecl::Create(ASTContext &C,
                                     SourceLocation NamespaceLoc,
                                     SourceLocation LBrace,
                                     IdentifierInfo *II,
                                     bool isAnonymous) {
   return new(C) NamespaceDecl(NamespaceLoc, LBrace, II, isAnonymous);
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
                           AccessModifier am,
                           SourceLocation DefLoc,
                           DeclarationName Name,
                           SourceType returnType,
                           std::vector<FuncArgDecl* > &&args,
                           Statement* body,
                           OperatorInfo op,
                           std::vector<TemplateParamDecl*> &&templateParams)
   : NamedDecl(typeID, am, Name),
     DeclContext(typeID),
     DefLoc(DefLoc), returnType(returnType), args(move(args)),
     body(body), templateParams(move(templateParams)),
     op(op), Flags(0)
{
   for (auto &arg : this->args) {
      arg->setDeclContext(this);
      makeDeclAvailable(arg);
   }
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
   for (auto &arg : this->args)
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

FunctionDecl::FunctionDecl(AccessModifier am,
                           SourceLocation DefLoc,
                           DeclarationName II,
                           std::vector<FuncArgDecl* > &&args,
                           SourceType returnType,
                           Statement* body,
                           OperatorInfo op,
                           std::vector<TemplateParamDecl*> &&templateParams)
   : CallableDecl(FunctionDeclID, am, DefLoc, II, returnType,
                  move(args), body, op, move(templateParams))
{

}

FunctionDecl* FunctionDecl::Create(ASTContext &C,
                                   AccessModifier am,
                                   SourceLocation DefLoc,
                                   DeclarationName II,
                                   std::vector<FuncArgDecl *> &&args,
                                   SourceType returnType,
                                   Statement *body,
                                   OperatorInfo op,
                                   std::vector<TemplateParamDecl*> &&templateParams) {
   return new(C) FunctionDecl(am, DefLoc, II, move(args), returnType, body, op,
                              move(templateParams));
}

TypedefDecl::TypedefDecl(AccessModifier access,
                         SourceLocation Loc,
                         IdentifierInfo *II,
                         SourceType origin,
                         std::vector<TemplateParamDecl*> &&templateParams)
   : NamedDecl(TypedefDeclID, access, II),
     Loc(Loc), origin(origin), templateParams(move(templateParams))
{}

TypedefDecl*
TypedefDecl::Create(ASTContext &C,
                    AccessModifier access,
                    SourceLocation Loc,
                    IdentifierInfo *II,
                    SourceType origin,
                    std::vector<TemplateParamDecl*> &&templateParams) {
   return new(C) TypedefDecl(access, Loc, II, origin, move(templateParams));
}

AliasDecl::AliasDecl(SourceLocation Loc,
                     IdentifierInfo *II,
                     StaticExpr* aliasExpr,
                     std::vector<TemplateParamDecl*> &&templateParams)
   : NamedDecl(AliasDeclID, AccessModifier::DEFAULT, II),
     DeclContext(AliasDeclID),
     Loc(Loc), aliasExpr(aliasExpr), templateParams(move(templateParams))
{}

AliasDecl* AliasDecl::Create(ASTContext &C,
                             SourceLocation Loc,
                             IdentifierInfo *II,
                             StaticExpr *aliasExpr,
                             std::vector<TemplateParamDecl *> &&templateParams){
   return new(C) AliasDecl(Loc, II, aliasExpr, move(templateParams));
}

const Variant& AliasDecl::getVal() const
{
   return aliasExpr->getEvaluatedExpr();
}

RecordDecl::RecordDecl(DeclKind typeID,
                       AccessModifier access,
                       SourceLocation KeywordLoc,
                       IdentifierInfo *II,
                       std::vector<SourceType> &&conformanceTypes,
                       std::vector<TemplateParamDecl*> &&templateParams)
   : NamedDecl(typeID, access, II),
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
   decl->setDeclContext(this);

   if (auto I = dyn_cast<InitDecl>(decl)) {
      if (I->getArgs().empty())
         if (auto S = dyn_cast<StructDecl>(this))
            S->setParameterlessConstructor(I);
   }

   if (auto D = dyn_cast<DeinitDecl>(decl))
      deinitializer = D;

   if (auto E = dyn_cast<EnumCaseDecl>(decl)) {
      auto EDecl = cast<EnumDecl>(this);
      if (E->getArgs().size() > EDecl->maxAssociatedTypes)
         EDecl->maxAssociatedTypes = E->getArgs().size();
   }

   if (auto F = dyn_cast<FieldDecl>(decl)) {
      if (!F->isStatic())
         if (auto S = dyn_cast<StructDecl>(this))
            S->fields.push_back(F);
   }

   if (auto C = dyn_cast<EnumCaseDecl>(decl)) {
      if (auto E = dyn_cast<EnumDecl>(this))
         E->cases.push_back(C);
   }


   if (auto R = dyn_cast<RecordDecl>(decl))
      addInnerRecord(R);

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
   auto *II = &Ctx.getIdentifiers().get("==");
   auto DeclName = Ctx.getDeclNameTable().getInfixOperatorName(*II);

   return lookupSingle<MethodDecl>(DeclName);
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

MethodDecl* RecordDecl::getMethod(DeclarationName name, bool checkParent) const
{
   auto M = dyn_cast_or_null<MethodDecl>(lookupSingle(name));
   if (M)
      return M;

   for (auto &decl : getDecls()) {
      auto MDecl = dyn_cast<MethodDecl>(decl);
      if (MDecl && MDecl->getLinkageName()
                   == name.getIdentifierInfo()->getIdentifier())
         return MDecl;
   }

   if (checkParent) {
      if (auto C = dyn_cast<ClassDecl>(this)) {
         if (auto P = C->getParentClass()) {
            return P->getMethod(name, checkParent);
         }
      }
   }

   return nullptr;
}

MethodDecl* RecordDecl::getMethod(size_t id) const
{
   for (auto &decl : getDecls()) {
      auto Method = dyn_cast<MethodDecl>(decl);
      if (Method && Method->getMethodID() == id)
         return Method;
   }

   if (auto C = dyn_cast<ClassDecl>(this)) {
      if (auto P = C->getParentClass()) {
         return P->getMethod(id);
      }
   }

   return nullptr;
}

MethodDecl* RecordDecl::getOwnMethod(DeclarationName name)
{
   return lookupOwn<MethodDecl>(name);
}

bool RecordDecl::conformsToBaseTemplate(ProtocolDecl *P) const
{
   for (auto C : conformances)
      if (C->getSpecializedTemplate() == P)
         return true;

   return false;
}

sema::ResolvedTemplateArg const* RecordDecl::getTemplateArg(
   llvm::StringRef name) const
{
   assert(isInstantiation());
   return instantiationInfo->templateArgs.getNamedArg(name);
}

void RecordDecl::addExtension(ExtensionDecl *E)
{
   E->setDeclContext(this);

   for (auto &decl : E->getDecls())
      DeclContext::addDecl(decl);

   conformanceTypes.insert(conformanceTypes.end(),
                           E->getConformanceTypes().begin(),
                           E->getConformanceTypes().end());
}

StructDecl::StructDecl(AccessModifier access,
                       SourceLocation KeywordLoc,
                       IdentifierInfo *II,
                       std::vector<SourceType> &&conformanceTypes,
                       std::vector<TemplateParamDecl*> &&templateParams)
   : RecordDecl(StructDeclID, access, KeywordLoc, II, move(conformanceTypes),
                move(templateParams))
{}

StructDecl* StructDecl::Create(ASTContext &C,
                               AccessModifier access,
                               SourceLocation KeywordLoc,
                               IdentifierInfo *II,
                               std::vector<SourceType> &&conformanceTypes,
                               std::vector<TemplateParamDecl*>&&templateParams){
   return new(C) StructDecl(access, KeywordLoc, II, move(conformanceTypes),
                            move(templateParams));
}

StructDecl::StructDecl(DeclKind typeID,
                       AccessModifier access,
                       SourceLocation KeywordLoc,
                       IdentifierInfo *II,
                       std::vector<SourceType> &&conformanceTypes,
                       std::vector<TemplateParamDecl*> &&templateParams)
   : RecordDecl(typeID, access, KeywordLoc, II, move(conformanceTypes),
                move(templateParams))
{}

ClassDecl::ClassDecl(AccessModifier access,
                     SourceLocation KeywordLoc,
                     IdentifierInfo *II,
                     std::vector<SourceType> &&conformanceTypes,
                     std::vector<TemplateParamDecl*> &&templateParams,
                     SourceType parentClass,
                     bool isAbstract)
   : StructDecl(ClassDeclID, access, KeywordLoc, II, move(conformanceTypes),
                move(templateParams)),
     parentType(parentClass), IsAbstract(isAbstract)
{}

ClassDecl* ClassDecl::Create(ASTContext &C,
                             AccessModifier access,
                             SourceLocation KeywordLoc,
                             IdentifierInfo *II,
                             std::vector<SourceType> &&conformanceTypes,
                             std::vector<TemplateParamDecl *> &&templateParams,
                             SourceType parentClass, bool isAbstract) {
   return new(C) ClassDecl(access, KeywordLoc, II, move(conformanceTypes),
                           move(templateParams), parentClass, isAbstract);
}

EnumDecl::EnumDecl(AccessModifier access,
                   SourceLocation KeywordLoc,
                   IdentifierInfo *II,
                   std::vector<SourceType> &&conformanceTypes,
                   std::vector<TemplateParamDecl*> &&templateParams,
                   SourceType rawType)
   : RecordDecl(EnumDeclID, access, KeywordLoc, II, move(conformanceTypes),
                move(templateParams)),
     rawType(rawType)
{}

EnumDecl* EnumDecl::Create(ASTContext &C,
                           AccessModifier access,
                           SourceLocation KeywordLoc,
                           IdentifierInfo *II,
                           std::vector<SourceType> &&conformanceTypes,
                           std::vector<TemplateParamDecl *> &&templateParams,
                           SourceType rawType) {
   return new(C) EnumDecl(access, KeywordLoc, II, move(conformanceTypes),
                          move(templateParams), rawType);
}

EnumCaseDecl* EnumDecl::hasCase(DeclarationName name)
{
   return lookupSingle<EnumCaseDecl>(name);
}

UnionDecl::UnionDecl(AccessModifier access,
                     SourceLocation KeywordLoc,
                     IdentifierInfo *II,
                     std::vector<SourceType> &&conformanceTypes,
                     std::vector<TemplateParamDecl*> &&templateParams)
   : RecordDecl(UnionDeclID, access, KeywordLoc, II, move(conformanceTypes),
                move(templateParams))
{}

UnionDecl* UnionDecl::Create(ASTContext &C,
                             AccessModifier access,
                             SourceLocation KeywordLoc,
                             IdentifierInfo *II,
                             std::vector<SourceType> &&conformanceTypes,
                             std::vector<TemplateParamDecl*> &&templateParams) {
   return new(C) UnionDecl(access, KeywordLoc, II, move(conformanceTypes),
                           move(templateParams));
}

ProtocolDecl::ProtocolDecl(AccessModifier access,
                           SourceLocation KeywordLoc,
                           IdentifierInfo *II,
                           std::vector<SourceType> &&conformanceTypes,
                           std::vector<TemplateParamDecl*> &&templateParams)
   : RecordDecl(ProtocolDeclID, access, KeywordLoc, II, move(conformanceTypes),
                move(templateParams))
{}

ProtocolDecl* ProtocolDecl::Create(ASTContext &C,
                                   AccessModifier access,
                                   SourceLocation KeywordLoc,
                                   IdentifierInfo *II,
                                   std::vector<SourceType> &&conformanceTypes,
                                   std::vector<TemplateParamDecl*>
                                                            &&templateParams) {
   return new(C) ProtocolDecl(access, KeywordLoc, II, move(conformanceTypes),
                              move(templateParams));
}

ExtensionDecl::ExtensionDecl(AccessModifier access,
                             SourceLocation KeywordLoc,
                             IdentifierInfo *II,
                             std::vector<SourceType> &&conformanceTypes,
                             std::vector<TemplateParamDecl*> &&templateParams)
   : RecordDecl(ExtensionDeclID, access, KeywordLoc, II,
                move(conformanceTypes), move(templateParams))
{}

ExtensionDecl *ExtensionDecl::Create(ASTContext &C,
                                     AccessModifier access,
                                     SourceLocation KeywordLoc,
                                     IdentifierInfo *II,
                                     std::vector<SourceType> &&conformanceTypes,
                                     std::vector<TemplateParamDecl *>
                                                             &&templateParams) {
   return new(C) ExtensionDecl(access, KeywordLoc, II, move(conformanceTypes),
                               move(templateParams));
}

MethodDecl::MethodDecl(AccessModifier access,
                       SourceLocation DefLoc,
                       DeclarationName II,
                       SourceType returnType,
                       std::vector<FuncArgDecl *> &&args,
                       std::vector<TemplateParamDecl *> &&templateParams,
                       Statement *body,
                       bool isStatic)
   : CallableDecl(MethodDeclID, access, DefLoc, II, returnType, move(args),
                  body, OperatorInfo(), move(templateParams)) {
   setFlag(Abstract, !body);
   setDeclFlag(DF_Static, isStatic);
}

MethodDecl* MethodDecl::Create(ASTContext &C,
                               AccessModifier access,
                               SourceLocation DefLoc,
                               DeclarationName II,
                               SourceType returnType,
                               std::vector<FuncArgDecl *> &&args,
                               std::vector<TemplateParamDecl*> &&templateParams,
                               Statement *body, bool isStatic) {
   return new(C) MethodDecl(access, DefLoc, II, returnType, move(args),
                            move(templateParams), body, isStatic);
}

MethodDecl::MethodDecl(AccessModifier access,
                       SourceLocation DefLoc,
                       DeclarationName OperatorName,
                       SourceType returnType,
                       std::vector<FuncArgDecl *> &&args,
                       std::vector<TemplateParamDecl *> &&templateParams,
                       Statement *body,
                       OperatorInfo op, bool isStatic)
   : CallableDecl(MethodDeclID, access, DefLoc, OperatorName, returnType,
                  move(args), body, op, move(templateParams)) {
   setFlag(Abstract, !body);
   setDeclFlag(DF_Static, isStatic);
}

MethodDecl* MethodDecl::CreateOperator(ASTContext &C,
                                       AccessModifier access,
                                       SourceLocation DefLoc,
                                       DeclarationName OperatorName,
                                       SourceType returnType,
                                       std::vector<FuncArgDecl *> &&args,
                                       std::vector<TemplateParamDecl*>
                                                               &&templateParams,
                                       Statement *body,
                                       OperatorInfo op, bool isStatic) {
   return new(C) MethodDecl(access, DefLoc, OperatorName, returnType,
                            move(args), move(templateParams), body, op,
                            isStatic);
}

MethodDecl::MethodDecl(AccessModifier access,
                       SourceLocation DefLoc,
                       SourceType returnType,
                       std::vector<FuncArgDecl *> &&args,
                       std::vector<TemplateParamDecl *> &&templateParams,
                       Statement *body)
   : CallableDecl(MethodDeclID, access, DefLoc, DeclarationName(),
                  returnType, move(args), body, OperatorInfo(),
                  move(templateParams)) {
   setFlag(Abstract, !body);
   setFlag(ConvOp, true);
}

MethodDecl* MethodDecl::CreateConversionOp(ASTContext &C,
                                           AccessModifier access,
                                           SourceLocation DefLoc,
                                           SourceType returnType,
                                           std::vector<FuncArgDecl *> &&args,
                                           std::vector<TemplateParamDecl*>
                                                               &&templateParams,
                                           Statement *body) {
   return new(C) MethodDecl(access, DefLoc, returnType, move(args),
                            move(templateParams), body);
}

MethodDecl::MethodDecl(DeclKind typeID,
                       AccessModifier access,
                       SourceLocation DefLoc,
                       DeclarationName Name,
                       SourceType returnType,
                       std::vector<FuncArgDecl *> &&args,
                       std::vector<TemplateParamDecl *> &&templateParams,
                       Statement *body)
   : CallableDecl(typeID, access, DefLoc, Name,
                  returnType, move(args), body, OperatorInfo(),
                  move(templateParams)) {
   setFlag(Abstract, !body);
}

bool MethodDecl::isTemplatedInitializer() const
{
   if (!support::isa<InitDecl>(this))
      return false;

   return getTemplateParams().size()
          > support::cast<RecordDecl>(declContext)->getTemplateParams().size();
}

InitDecl::InitDecl(AccessModifier am,
                   SourceLocation Loc,
                   DeclarationName Name)
   : MethodDecl(InitDeclID, am, Loc, Name, SourceType(), {}, {}, {}),
     ExplicitMemberwise(true)
{
   setDeclFlag(DF_Static, true);
}

InitDecl* InitDecl::CreateMemberwise(ASTContext &C,
                                     AccessModifier am,
                                     SourceLocation Loc,
                                     QualType InitializedType) {
   DeclarationName Name;
   if (InitializedType)
      Name = C.getDeclNameTable().getConstructorName(InitializedType);

   return new(C) InitDecl(am, Loc, Name);
}

InitDecl::InitDecl(AccessModifier am,
                   SourceLocation Loc,
                   std::vector<FuncArgDecl *> &&args,
                   std::vector<TemplateParamDecl *> &&templateParams,
                   Statement *body, DeclarationName Name)
   : MethodDecl(InitDeclID, am, Loc, Name, SourceType(), move(args),
                move(templateParams), body), ExplicitMemberwise(false) {
   setDeclFlag(DF_Static, true);
}

InitDecl* InitDecl::Create(ASTContext &C,
                           AccessModifier am,
                           SourceLocation Loc,
                           std::vector<FuncArgDecl *> &&args,
                           std::vector<TemplateParamDecl *> &&templateParams,
                           Statement *body,
                           QualType InitializedType) {
   DeclarationName Name;
   if (InitializedType)
      Name = C.getDeclNameTable().getConstructorName(InitializedType);

   return new(C) InitDecl(am, Loc, move(args), move(templateParams),
                          body, Name);
}

DeinitDecl::DeinitDecl(SourceLocation Loc,
                       Statement *body,
                       DeclarationName Name)
   : MethodDecl(DeinitDeclID, AccessModifier::PUBLIC, Loc,
                Name, SourceType(), {}, {}, body)
{
   setDeclFlag(DF_Static, true);
}

DeinitDecl* DeinitDecl::Create(ASTContext &C,
                               SourceLocation Loc,
                               Statement *body,
                               QualType DeinitializedType) {
   DeclarationName Name;
   if (DeinitializedType)
      Name = C.getDeclNameTable().getDestructorName(DeinitializedType);

   return new(C) DeinitDecl(Loc, body, Name);
}

FieldDecl::FieldDecl(AccessModifier Access,
                     SourceLocation VarOrLetLoc,
                     SourceLocation ColonLoc,
                     IdentifierInfo *II,
                     SourceType Type,
                     bool IsStatic,
                     bool IsConst,
                     Expression* DefaultVal)
   : VarDecl(FieldDeclID, Access, VarOrLetLoc, ColonLoc, IsConst, II,
             Type, DefaultVal)
{
   setDeclFlag(DF_Static, IsStatic);
}

FieldDecl* FieldDecl::Create(ASTContext &C,
                             AccessModifier Access,
                             SourceLocation VarOrLetLoc,
                             SourceLocation ColonLoc,
                             IdentifierInfo *II,
                             SourceType Type,
                             bool IsStatic, bool IsConst,
                             Expression *DefaultVal) {
   return new(C) FieldDecl(Access, VarOrLetLoc, ColonLoc, II, Type, IsStatic,
                           IsConst, DefaultVal);
}

AssociatedTypeDecl::AssociatedTypeDecl(SourceLocation Loc,
                                       IdentifierInfo *ProtoSpec,
                                       IdentifierInfo *Name,
                                       SourceType actualType)
   : NamedDecl(AssociatedTypeDeclID, AccessModifier::PUBLIC, Name),
     protocolSpecifier(ProtoSpec), actualType(actualType)
{

}

AssociatedTypeDecl* AssociatedTypeDecl::Create(ASTContext &C,
                                               SourceLocation Loc,
                                               IdentifierInfo *ProtoSpec,
                                               IdentifierInfo *Name,
                                               SourceType actualType) {
   return new(C) AssociatedTypeDecl(Loc, ProtoSpec, Name, actualType);
}

PropDecl::PropDecl(AccessModifier access,
                   SourceLocation Loc,
                   IdentifierInfo *propName,
                   SourceType type,
                   bool isStatic, bool hasDefinition, bool hasGetter,
                   bool hasSetter,
                   CompoundStmt *getter, CompoundStmt *setter,
                   IdentifierInfo *newValName)
   : NamedDecl(PropDeclID, access, propName),
     type(type),
     getterBody(getter), setterBody(setter),
     HasGetter(hasGetter), HasSetter(hasSetter), newValName(newValName)
{
   setDeclFlag(DF_Static, isStatic);
   setDeclFlag(DF_HasDefinition, hasDefinition);
}

PropDecl* PropDecl::Create(ASTContext &C,
                           AccessModifier access,
                           SourceLocation Loc,
                           IdentifierInfo *propName,
                           SourceType type,
                           bool isStatic, bool hasDefinition, bool hasGetter,
                           bool hasSetter,
                           CompoundStmt *getter, CompoundStmt *setter,
                           IdentifierInfo *newValName) {
   return new(C) PropDecl(access, Loc, propName, type, isStatic,
                          hasDefinition, hasGetter, hasSetter, getter,
                          setter, newValName);
}

EnumCaseDecl::EnumCaseDecl(AccessModifier AS,
                           SourceLocation CaseLoc,
                           SourceLocation IdentLoc,
                           IdentifierInfo *II,
                           StaticExpr *rawValue,
                           std::vector<FuncArgDecl *> &&associatedTypes)
   : CallableDecl(EnumCaseDeclID, AS, CaseLoc, II, SourceType(),
                  move(associatedTypes), nullptr, OperatorInfo(), {}),
     CaseLoc(CaseLoc), IdentLoc(IdentLoc), rawValExpr(rawValue)
{

}

EnumCaseDecl* EnumCaseDecl::Create(ASTContext &C,
                                   AccessModifier AS,
                                   SourceLocation CaseLoc,
                                   SourceLocation IdentLoc,
                                   IdentifierInfo *II,
                                   StaticExpr *rawValue,
                                   std::vector<FuncArgDecl*> &&associatedTypes){
   return new(C) EnumCaseDecl(AS, CaseLoc, IdentLoc, II, rawValue,
                              move(associatedTypes));
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