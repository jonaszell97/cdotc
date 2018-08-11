//
// Created by Jonas Zell on 26.01.18.
//

#include "Decl.h"

#include "AST/ASTContext.h"
#include "AST/Expression.h"
#include "AST/Statement.h"
#include "Sema/Builtin.h"
#include "Sema/SemaPass.h"
#include "Serialization/ModuleFile.h"
#include "Support/StringSwitch.h"

#include <llvm/ADT/SmallString.h>

using namespace cdot::support;

namespace cdot {
namespace ast {

DeclConstraint::DeclConstraint(Kind K,
                               SourceRange SR,
                               ArrayRef<IdentifierInfo *> NameQual,
                               SourceType RHS)
   : K(K), SR(SR), NameQualifierSize((unsigned)NameQual.size()), Type(RHS)
{
   std::copy(NameQual.begin(), NameQual.end(),
             getTrailingObjects<IdentifierInfo*>());
}

DeclConstraint::DeclConstraint(SourceRange SR,
                               ArrayRef<IdentifierInfo*> NameQual,
                               IdentifierRefExpr *ConceptRef)
   : K(Concept), SR(SR), NameQualifierSize((unsigned)NameQual.size()),
     ConceptRef(ConceptRef)
{
   std::copy(NameQual.begin(), NameQual.end(),
             getTrailingObjects<IdentifierInfo*>());
}

DeclConstraint* DeclConstraint::Create(ASTContext &C,
                                       Kind K,
                                       SourceRange SR,
                                       ArrayRef<IdentifierInfo*> NameQual,
                                       SourceType RHS) {
   void *Mem = C.Allocate(
      totalSizeToAlloc<IdentifierInfo*>(NameQual.size()),
      alignof(DeclConstraint));

   return new(Mem) DeclConstraint(K, SR, NameQual, RHS);
}

DeclConstraint* DeclConstraint::Create(ASTContext &C,
                                       SourceRange SR,
                                       ArrayRef<IdentifierInfo*> NameQual,
                                       IdentifierRefExpr *ConceptRef) {
   void *Mem = C.Allocate(totalSizeToAlloc<IdentifierInfo*>(NameQual.size()),
                          alignof(DeclConstraint));

   return new(Mem) DeclConstraint(SR, NameQual, ConceptRef);
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

UsingDecl::UsingDecl(EmptyShell, unsigned N)
   : NamedDecl(UsingDeclID, AccessSpecifier::Default, DeclarationName()),
     NumSpecifierNames(N), IsWildCard(false)
{}

UsingDecl *UsingDecl::CreateEmpty(ASTContext &C, unsigned N)
{
    void *Mem = C.Allocate(totalSizeToAlloc<IdentifierInfo*>(N),
                           alignof(UsingDecl));
    return new(Mem) UsingDecl(EmptyShell(), N);
}

ImportDecl::ImportDecl(SourceRange Loc,
                       AccessSpecifier Access,
                       llvm::ArrayRef<IdentifierInfo*> moduleName,
                       llvm::ArrayRef<IdentifierInfo*> namedImports,
                       bool IsWildcardImport)
   : NamedDecl(ImportDeclID, Access,
               !IsWildcardImport && namedImports.empty() ? moduleName.back()
                                                         : DeclarationName()),
     DeclContext(ImportDeclID),
     Loc(Loc), WildcardImport(IsWildcardImport),
     NumNameQuals((unsigned) moduleName.size()),
     NumNamedImports((unsigned)namedImports.size())
{
   std::copy(moduleName.begin(), moduleName.end(),
             getTrailingObjects<IdentifierInfo*>());

   std::copy(namedImports.begin(), namedImports.end(),
             getTrailingObjects<IdentifierInfo*>() + NumNameQuals);
}

ImportDecl* ImportDecl::Create(ASTContext &C, SourceRange Loc,
                               AccessSpecifier Access,
                               llvm::ArrayRef<IdentifierInfo*> moduleName,
                               llvm::ArrayRef<IdentifierInfo*> namedImports,
                               bool IsWildcardImport) {
   void *Mem = C.Allocate(
      totalSizeToAlloc<IdentifierInfo*>(moduleName.size()+namedImports.size()),
      alignof(ImportDecl));

   return new(Mem) ImportDecl(Loc, Access, moduleName, namedImports,
                              IsWildcardImport);
}

ImportDecl::ImportDecl(EmptyShell, unsigned N)
   : NamedDecl(ImportDeclID, AccessSpecifier::Default, DeclarationName()),
     DeclContext(ImportDeclID),
     WildcardImport(false), NumNameQuals(N),
     NumNamedImports(0)
{}

ImportDecl *ImportDecl::CreateEmpty(ASTContext &C, unsigned N) {
    void *Mem = C.Allocate(
       totalSizeToAlloc<IdentifierInfo*>(N),
       alignof(ImportDecl));

    return new(Mem) ImportDecl(EmptyShell(), N);
}

ModuleDecl::ModuleDecl(SourceRange Loc,
                       DeclarationName moduleName)
   : NamedDecl(ModuleDeclID, AccessSpecifier::Public, moduleName),
     DeclContext(ModuleDeclID),
     Loc(Loc)
{

}

ModuleDecl* ModuleDecl::Create(ASTContext &C, SourceRange Loc,
                               DeclarationName moduleName) {
   return new(C) ModuleDecl(Loc, moduleName);
}

ModuleDecl::ModuleDecl(EmptyShell)
   : NamedDecl(ModuleDeclID, AccessSpecifier::Default, DeclarationName()),
     DeclContext(ModuleDeclID)
{}

ModuleDecl* ModuleDecl::CreateEmpty(ASTContext &C)
{
   return new(C) ModuleDecl(EmptyShell());
}

ModuleDecl *ModuleDecl::getPrimaryModule() const
{
   return cast<ModuleDecl>(primaryCtx);
}

ModuleDecl* ModuleDecl::getBaseModule() const
{
   auto Mod = const_cast<ModuleDecl*>(this);
   while (auto Base = Mod->getParentModule())
      Mod = Base;

   return Mod->getPrimaryModule();
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

TemplateParamDecl::TemplateParamDecl(EmptyShell)
   : NamedDecl(TemplateParamDeclID, AccessSpecifier::Default, DeclarationName()),
     covariance(nullptr), contravariance(nullptr),
     typeName(false), defaultValue(nullptr), Index(0)
{}

TemplateParamDecl *TemplateParamDecl::CreateEmpty(ASTContext &C)
{
    return new(C) TemplateParamDecl(EmptyShell());
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
     CanElideCopy(false), Variadic(false), Captured(false),
     IsMovedFrom(false)
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
     IsNRVOCand(false), InitIsMove(false)
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

LocalVarDecl::LocalVarDecl(EmptyShell)
   : VarDecl(LocalVarDeclID, AccessSpecifier::Default, {}, {}, false, 
             DeclarationName(), SourceType(), nullptr),
     IsNRVOCand(false), InitIsMove(false)
{}

LocalVarDecl *LocalVarDecl::CreateEmpty(ASTContext &C)
{
    return new(C) LocalVarDecl(EmptyShell());
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

GlobalVarDecl::GlobalVarDecl(EmptyShell)
   : VarDecl(GlobalVarDeclID, AccessSpecifier::Default, {}, {}, false,
             DeclarationName(), SourceType(), nullptr)
{}

GlobalVarDecl *GlobalVarDecl::CreateEmpty(ASTContext &C)
{
   return new(C) GlobalVarDecl(EmptyShell());
}

DestructuringDecl::DestructuringDecl(SourceRange Parens,
                                     ArrayRef<VarDecl *> Decls,
                                     SourceType Type,
                                     Expression *value)
   : Decl(DestructuringDeclID),
     Kind(Unknown), Parens(Parens), Type(Type), Val(value),
     NumDecls((unsigned)Decls.size())
{
   std::copy(Decls.begin(), Decls.end(), getTrailingObjects<VarDecl*>());
}

DestructuringDecl::DestructuringDecl(EmptyShell, unsigned N)
   : Decl(DestructuringDeclID),
     Kind(Unknown), Parens(), Type(), Val(nullptr),
     NumDecls(N)
{

}

DestructuringDecl* DestructuringDecl::Create(ASTContext &C,
                                             SourceRange Parens,
                                             ArrayRef<VarDecl *> Decls,
                                             SourceType Type,
                                             Expression *value) {
   void *Mem = C.Allocate(totalSizeToAlloc<VarDecl*>(Decls.size()),
                          alignof(DestructuringDecl));

   return new(Mem) DestructuringDecl(Parens, Decls, Type, value);
}

DestructuringDecl* DestructuringDecl::CreateEmpty(ASTContext &C,
                                                  unsigned N) {
   void *Mem = C.Allocate(totalSizeToAlloc<VarDecl*>(N),
                          alignof(DestructuringDecl));

   return new(Mem) DestructuringDecl(EmptyShell(), N);
}

FuncArgDecl::FuncArgDecl(SourceLocation OwnershipLoc,
                         SourceLocation ColonLoc,
                         DeclarationName Name,
                         IdentifierInfo *Label,
                         ArgumentConvention Conv,
                         SourceType argType,
                         Expression* defaultValue,
                         bool variadicArgPackExpansion,
                         bool cstyleVararg,
                         bool isSelf)
   : VarDecl(FuncArgDeclID, AccessSpecifier::Public, OwnershipLoc, ColonLoc,
             Conv != ArgumentConvention::MutableRef,
             Name, argType, defaultValue),
     Label(Label), VariadicArgPackExpansion(variadicArgPackExpansion),
     Vararg(cstyleVararg), CstyleVararg(cstyleVararg), IsSelf(isSelf),
     Conv(Conv)
{}

FuncArgDecl* FuncArgDecl::Create(ASTContext &C,
                                 SourceLocation OwnershipLoc,
                                 SourceLocation ColonLoc,
                                 DeclarationName Name,
                                 IdentifierInfo *Label,
                                 ArgumentConvention Conv,
                                 SourceType argType,
                                 Expression *defaultValue,
                                 bool variadicArgPackExpansion,
                                 bool cstyleVararg,
                                 bool isSelf) {
   return new(C) FuncArgDecl(OwnershipLoc, ColonLoc, Name, Label, Conv,
                             argType, defaultValue, variadicArgPackExpansion,
                             cstyleVararg, isSelf);
}

FuncArgDecl::FuncArgDecl(EmptyShell)
   : VarDecl(FuncArgDeclID, AccessSpecifier::Default, {}, {}, false,
             DeclarationName(), SourceType(), nullptr)
{}

FuncArgDecl *FuncArgDecl::CreateEmpty(ASTContext &C)
{
    return new(C) FuncArgDecl(EmptyShell());
}

SourceRange FuncArgDecl::getSourceRange() const
{
   if (!ColonLoc && type.getTypeExpr()) {
      return type.getTypeExpr()->getSourceRange();
   }

   return ColonLoc;
}

GlobalDeclContext::GlobalDeclContext(CompilerInstance &CI)
   : DeclContext(Decl::NotDecl),
     CI(CI)
{
}

GlobalDeclContext* GlobalDeclContext::Create(ASTContext &C,
                                             CompilerInstance &CI) {
   return new(C) GlobalDeclContext(CI);
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

NamespaceDecl::NamespaceDecl(EmptyShell)
   : NamedDecl(NamespaceDeclID, AccessSpecifier::Default, DeclarationName()),
     DeclContext(NamespaceDeclID)
{}

NamespaceDecl *NamespaceDecl::CreateEmpty(ASTContext &C)
{
    return new(C) NamespaceDecl(EmptyShell());
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

CompoundDecl::CompoundDecl(EmptyShell)
   : Decl(CompoundDeclID), DeclContext(CompoundDeclID),
   Transparent(false)
{}

CompoundDecl *CompoundDecl::CreateEmpty(ASTContext &C)
{
    return new(C) CompoundDecl(EmptyShell());
}

CallableDecl::CallableDecl(DeclKind typeID,
                           AccessSpecifier am,
                           SourceLocation DefLoc,
                           DeclarationName Name,
                           SourceType returnType,
                           llvm::ArrayRef<FuncArgDecl*> args,
                           Statement* body,
                           ASTVector<TemplateParamDecl*> &&templateParams)
   : NamedDecl(typeID, am, Name),
     DeclContext(typeID),
     DefLoc(DefLoc), returnType(returnType), NumArgs((unsigned)args.size()),
     body(body), templateParams(move(templateParams)),
     Flags(0)
{
   std::copy(args.begin(), args.end(), arg_begin());

   switch (Name.getKind()) {
   case DeclarationName::InfixOperatorName:
   case DeclarationName::PrefixOperatorName:
   case DeclarationName::PostfixOperatorName:
      setFlag(IsOperator, true);
      break;
   default:
      break;
   }
}

CallableDecl::CallableDecl(EmptyShell E,
                           DeclKind typeID,
                           unsigned N)
   : NamedDecl(typeID, AccessSpecifier::Default, {}),
     DeclContext(typeID),
     DefLoc(), returnType(), NumArgs(N),
     body(nullptr), templateParams(),
     Flags(0)
{

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

   llvm::SmallVector<QualType, 4> args;
   llvm::SmallVector<FunctionType::ParamInfo, 4> paramInfo;

   for (auto &arg : getArgs()) {
      args.push_back(arg->getType());
      paramInfo.emplace_back(arg->getConvention());
   }

   if (isVararg())
      flags |= FunctionType::Vararg;

   if (isCstyleVararg())
      flags |= FunctionType::CStyleVararg;

   if (throws())
      flags |= FunctionType::Throws;

   if (isAsync())
      flags |= FunctionType::Async;

   functionType = SP.getContext().getFunctionType(retTy, args, paramInfo,
                                                  flags, lambda);
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

   knownFnKind = StringSwitch<KnownFunction>(getName())
      .Case("malloc", KnownFunction::Malloc)
      .Case("free", KnownFunction::Free)
      .Case("realloc", KnownFunction::Realloc)
      .Case("printf", KnownFunction::Printf)
      .Case("putchar", KnownFunction::PutChar)
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
      .Case("llvm.ctlz.i32", KnownFunction::llvm_ctlz_i32)
      .Case("llvm.ctlz.i64", KnownFunction::llvm_ctlz_i64)
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

bool CallableDecl::isFallibleInit() const
{
   return isa<InitDecl>(this) && cast<InitDecl>(this)->isFallible();
}

bool CallableDecl::isCompleteInitializer() const
{
   if (auto *I = dyn_cast<InitDecl>(this))
      return I->isCompleteInitializer();

   return false;
}

bool CallableDecl::isBaseInitializer() const
{
   if (auto *I = dyn_cast<InitDecl>(this))
      return I->isBaseInitializer();

   return false;
}

FunctionDecl::FunctionDecl(AccessSpecifier am,
                           SourceLocation DefLoc,
                           DeclarationName Name,
                           llvm::ArrayRef<FuncArgDecl*> args,
                           SourceType returnType,
                           Statement* body,
                           ASTVector<TemplateParamDecl*> &&templateParams)
   : CallableDecl(FunctionDeclID, am, DefLoc, Name, returnType,
                  move(args), body, move(templateParams))
{

}

FunctionDecl* FunctionDecl::Create(ASTContext &C,
                             AccessSpecifier am,
                             SourceLocation DefLoc,
                             DeclarationName Name,
                             llvm::ArrayRef<FuncArgDecl*> args,
                             SourceType returnType,
                             Statement *body,
                             ASTVector<TemplateParamDecl*> &&templateParams) {
   void *Mem = C.Allocate(sizeof(FunctionDecl)
                             + args.size() * sizeof(FuncArgDecl*),
                          alignof(FunctionDecl));

   return new(Mem) FunctionDecl(am, DefLoc, Name, move(args), returnType, body,
                                move(templateParams));
}

FunctionDecl::FunctionDecl(EmptyShell E, unsigned N)
   : CallableDecl(E, FunctionDeclID, N)
{}

FunctionDecl *FunctionDecl::CreateEmpty(ASTContext &C, unsigned N)
{
   void *Mem = C.Allocate(sizeof(FunctionDecl)
                          + N * sizeof(FuncArgDecl*),
                          alignof(FunctionDecl));
   
    return new(Mem) FunctionDecl(EmptyShell(), N);
}

MethodDecl::MethodDecl(EmptyShell E, unsigned N)
   : CallableDecl(E, MethodDeclID, N)
{}

MethodDecl::MethodDecl(EmptyShell E, DeclKind typeID, unsigned N)
   : CallableDecl(E, typeID, N)
{}

MethodDecl *MethodDecl::CreateEmpty(ASTContext &C, unsigned N)
{
   void *Mem = C.Allocate(sizeof(MethodDecl)
                          + N * sizeof(FuncArgDecl*),
                          alignof(MethodDecl));

   return new(Mem) MethodDecl(EmptyShell(), N);
}

InitDecl::InitDecl(EmptyShell E, unsigned N)
   : MethodDecl(E, InitDeclID, N)
{}

InitDecl *InitDecl::CreateEmpty(ASTContext &C, unsigned N)
{
   void *Mem = C.Allocate(sizeof(InitDecl)
                          + N * sizeof(FuncArgDecl*),
                          alignof(InitDecl));

   return new(Mem) InitDecl(EmptyShell(), N);
}

DeinitDecl::DeinitDecl(EmptyShell E, unsigned N)
   : MethodDecl(E, DeinitDeclID, N)
{}

DeinitDecl *DeinitDecl::CreateEmpty(ASTContext &C, unsigned N)
{
   void *Mem = C.Allocate(sizeof(DeinitDecl)
                          + N * sizeof(FuncArgDecl*),
                          alignof(DeinitDecl));

   return new(Mem) DeinitDecl(EmptyShell(), N);
}

TypedefDecl::TypedefDecl(AccessSpecifier access,
                         SourceLocation Loc,
                         DeclarationName Name,
                         SourceType origin,
                         ASTVector<TemplateParamDecl*> &&templateParams)
   : NamedDecl(TypedefDeclID, access, Name),
     Loc(Loc), origin(origin), templateParams(move(templateParams))
{}

TypedefDecl*
TypedefDecl::Create(ASTContext &C,
                    AccessSpecifier access,
                    SourceLocation Loc,
                    DeclarationName Name,
                    SourceType origin,
                    ASTVector<TemplateParamDecl*> &&templateParams) {
   return new(C) TypedefDecl(access, Loc, Name, origin, move(templateParams));
}

TypedefDecl::TypedefDecl(EmptyShell)
   : NamedDecl(TypedefDeclID, AccessSpecifier::Default, DeclarationName())
{}

TypedefDecl *TypedefDecl::CreateEmpty(ASTContext &C)
{
    return new(C) TypedefDecl(EmptyShell());
}

AliasDecl::AliasDecl(SourceLocation Loc,
                     AccessSpecifier AccessSpec,
                     DeclarationName Name,
                     SourceType Type,
                     StaticExpr* aliasExpr,
                     ArrayRef<TemplateParamDecl*> templateParams)
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

AliasDecl::AliasDecl(EmptyShell, unsigned N)
   : NamedDecl(AliasDeclID, AccessSpecifier::Default, DeclarationName()),
     DeclContext(AliasDeclID),
     aliasExpr(nullptr), NumParams(N)
{}

AliasDecl *AliasDecl::CreateEmpty(ASTContext &C, unsigned N)
{
    void *Mem = C.Allocate(totalSizeToAlloc<TemplateParamDecl*>(N),
                           alignof(AliasDecl));
    return new(Mem) AliasDecl(EmptyShell(), N);
}

SourceRange AliasDecl::getSourceRange() const
{
   return SourceRange(Loc,
                      aliasExpr ? aliasExpr->getSourceRange().getEnd()
                                : Loc);
}

RecordDecl::RecordDecl(DeclKind typeID,
                       AccessSpecifier access,
                       SourceLocation KeywordLoc,
                       DeclarationName Name,
                       ASTVector<SourceType> &&conformanceTypes,
                       ASTVector<TemplateParamDecl*> &&templateParams)
   : NamedDecl(typeID, access, Name),
     DeclContext(typeID),
     KeywordLoc(KeywordLoc), conformanceTypes(move(conformanceTypes)),
     templateParams(move(templateParams)),
     manualAlignment(false), opaque(false),
     implicitlyEquatable(false), implicitlyHashable(false),
     implicitlyCopyable(false), implicitlyStringRepresentable(false),
     NeedsRetainOrRelease(false)
{}

RecordDecl::RecordDecl(EmptyShell E,
                       DeclKind typeID)
   : NamedDecl(typeID, AccessSpecifier::Default, {}),
     DeclContext(typeID),
     KeywordLoc(), conformanceTypes(),
     templateParams(),
     manualAlignment(false), opaque(false),
     implicitlyEquatable(false), implicitlyHashable(false),
     implicitlyCopyable(false), implicitlyStringRepresentable(false),
     NeedsRetainOrRelease(false)
{

}

DeclContext::AddDeclResultKind RecordDecl::addDecl(NamedDecl *decl)
{
   switch (decl->getKind()) {
   case DeinitDeclID:
      deinitializer = cast<DeinitDecl>(decl);
      goto case_method;
   case MethodDeclID:
   case InitDeclID:
   case_method: {
      auto M = cast<MethodDecl>(decl);
      if (!M->getMethodID())
         M->setMethodID(lastMethodID++);

      break;
   }
   case FieldDeclID: {
      auto F = cast<FieldDecl>(decl);
      if (!F->isStatic() && isa<StructDecl>(this)) {
         auto S = cast<StructDecl>(this);
         S->StoredFields.push_back(F);
      }

      break;
   }
   case EnumCaseDeclID: {
      if (!isa<EnumDecl>(this))
         break;

      auto E = cast<EnumCaseDecl>(decl);
      auto EDecl = cast<EnumDecl>(this);
      EDecl->Unpopulated = false;

      if (E->getArgs().size() > EDecl->maxAssociatedTypes)
         EDecl->maxAssociatedTypes = E->getArgs().size();

      break;
   }
   default:
      break;
   }

   if (isa<ProtocolDecl>(this))
      decl->setIsProtocolRequirement(true);

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
                                                  ProtocolDecl *P) {
   auto AT = dyn_cast_or_null<AssociatedTypeDecl>(lookupSingle(name));
   if (AT) {
      if (!AT->getProto() || AT->getProto() == P)
         return AT;
   }

   return nullptr;
}

MethodDecl* RecordDecl::getConversionOperator(QualType toType)
{
   auto &Ctx = getASTCtx();
   auto DeclName = Ctx.getDeclNameTable().getConversionOperatorName(toType);

   return lookupSingle<MethodDecl>(DeclName);
}

MethodDecl* RecordDecl::getComparisonOperator(QualType withType)
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

PropDecl* RecordDecl::getProperty(DeclarationName name)
{
   auto *Prop = dyn_cast_or_null<PropDecl>(lookupSingle(name));
   if (!Prop) {
      if (auto C = dyn_cast<ClassDecl>(this)) {
         if (auto P = C->getParentClass()) {
            return P->getProperty(name);
         }
      }
   }

   return Prop;
}

FieldDecl* RecordDecl::getField(DeclarationName name)
{
   auto *F = dyn_cast_or_null<FieldDecl>(lookupSingle(name));
   if (!F) {
      if (auto C = dyn_cast<ClassDecl>(this)) {
         if (auto P = C->getParentClass()) {
            return P->getField(name);
         }
      }
   }

   return F;
}

bool RecordDecl::isNonUnionStruct() const
{
   switch (kind) {
   case StructDeclID:
   case ClassDeclID:
      return true;
   default:
      return false;
   }
}

StructDecl* RecordDecl::asNonUnionStruct() const
{
   switch (kind) {
   case StructDeclID:
   case ClassDeclID:
      return cast<StructDecl>(const_cast<RecordDecl*>(this));
   default:
      return nullptr;
   }
}

StructDecl::StructDecl(AccessSpecifier access,
                       SourceLocation KeywordLoc,
                       DeclarationName Name,
                       ASTVector<SourceType> &&conformanceTypes,
                       ASTVector<TemplateParamDecl*> &&templateParams)
   : RecordDecl(StructDeclID, access, KeywordLoc, Name, move(conformanceTypes),
                move(templateParams))
{}

StructDecl* StructDecl::Create(ASTContext &C,
                               AccessSpecifier access,
                               SourceLocation KeywordLoc,
                               DeclarationName Name,
                               ASTVector<SourceType> &&conformanceTypes,
                               ASTVector<TemplateParamDecl*>&&templateParams){
   return new(C) StructDecl(access, KeywordLoc, Name, move(conformanceTypes),
                            move(templateParams));
}

StructDecl::StructDecl(DeclKind typeID,
                       AccessSpecifier access,
                       SourceLocation KeywordLoc,
                       DeclarationName Name,
                       ASTVector<SourceType> &&conformanceTypes,
                       ASTVector<TemplateParamDecl*> &&templateParams)
   : RecordDecl(typeID, access, KeywordLoc, Name, move(conformanceTypes),
                move(templateParams))
{}

StructDecl::StructDecl(EmptyShell E)
   : RecordDecl(E, StructDeclID)
{}

StructDecl::StructDecl(EmptyShell E, DeclKind typeID)
   : RecordDecl(E, typeID)
{}

StructDecl *StructDecl::CreateEmpty(ASTContext &C)
{
    return new(C) StructDecl(EmptyShell());
}

ClassDecl::ClassDecl(AccessSpecifier access,
                     SourceLocation KeywordLoc,
                     DeclarationName Name,
                     ASTVector<SourceType> &&conformanceTypes,
                     ASTVector<TemplateParamDecl*> &&templateParams,
                     SourceType parentClass,
                     bool isAbstract)
   : StructDecl(ClassDeclID, access, KeywordLoc, Name, move(conformanceTypes),
                move(templateParams)),
     parentType(parentClass)
{
   setAbstract(isAbstract);
}

ClassDecl* ClassDecl::Create(ASTContext &C,
                             AccessSpecifier access,
                             SourceLocation KeywordLoc,
                             DeclarationName Name,
                             ASTVector<SourceType> &&conformanceTypes,
                             ASTVector<TemplateParamDecl*> &&templateParams,
                             SourceType parentClass, bool isAbstract) {
   return new(C) ClassDecl(access, KeywordLoc, Name, move(conformanceTypes),
                           move(templateParams), parentClass, isAbstract);
}

ClassDecl::ClassDecl(EmptyShell E)
   : StructDecl(E, ClassDeclID)
{}

ClassDecl *ClassDecl::CreateEmpty(ASTContext &C)
{
    return new(C) ClassDecl(EmptyShell());
}

void ClassDecl::inherit(ast::ClassDecl *C)
{
   parentClass = C;
}

EnumDecl::EnumDecl(AccessSpecifier access,
                   SourceLocation KeywordLoc,
                   DeclarationName Name,
                   ASTVector<SourceType> &&conformanceTypes,
                   ASTVector<TemplateParamDecl*> &&templateParams,
                   SourceType rawType)
   : RecordDecl(EnumDeclID, access, KeywordLoc, Name, move(conformanceTypes),
                move(templateParams)),
     rawType(rawType)
{}

EnumDecl* EnumDecl::Create(ASTContext &C,
                           AccessSpecifier access,
                           SourceLocation KeywordLoc,
                           DeclarationName Name,
                           ASTVector<SourceType> &&conformanceTypes,
                           ASTVector<TemplateParamDecl*> &&templateParams,
                           SourceType rawType) {
   return new(C) EnumDecl(access, KeywordLoc, Name, move(conformanceTypes),
                          move(templateParams), rawType);
}

EnumDecl::EnumDecl(EmptyShell E)
   : RecordDecl(E, EnumDeclID)
{}

EnumDecl *EnumDecl::CreateEmpty(ASTContext &C)
{
    return new(C) EnumDecl(EmptyShell());
}

EnumCaseDecl *EnumDecl::getCase(StringRef Name)
{
   auto *II = &getASTCtx().getIdentifiers().get(Name);
   return lookupSingle<EnumCaseDecl>(II);
}

EnumCaseDecl* EnumDecl::hasCase(DeclarationName name)
{
   return lookupSingle<EnumCaseDecl>(name);
}

UnionDecl::UnionDecl(AccessSpecifier access,
                     SourceLocation KeywordLoc,
                     DeclarationName Name,
                     ASTVector<SourceType> &&conformanceTypes,
                     ASTVector<TemplateParamDecl*> &&templateParams)
   : StructDecl(UnionDeclID, access, KeywordLoc, Name, move(conformanceTypes),
                move(templateParams))
{}

UnionDecl* UnionDecl::Create(ASTContext &C,
                             AccessSpecifier access,
                             SourceLocation KeywordLoc,
                             DeclarationName Name,
                             ASTVector<SourceType> &&conformanceTypes,
                             ASTVector<TemplateParamDecl*> &&templateParams) {
   return new(C) UnionDecl(access, KeywordLoc, Name, move(conformanceTypes),
                           move(templateParams));
}

UnionDecl::UnionDecl(EmptyShell Empty)
   : StructDecl(Empty, UnionDeclID)
{}

UnionDecl *UnionDecl::CreateEmpty(ASTContext &C)
{
    return new(C) UnionDecl(EmptyShell());
}

ProtocolDecl::ProtocolDecl(AccessSpecifier access,
                           SourceLocation KeywordLoc,
                           DeclarationName Name,
                           ASTVector<SourceType> &&conformanceTypes,
                           ASTVector<TemplateParamDecl*> &&templateParams)
   : RecordDecl(ProtocolDeclID, access, KeywordLoc, Name, move(conformanceTypes),
                move(templateParams)),
     IsAny(false)
{}

ProtocolDecl* ProtocolDecl::Create(ASTContext &C,
                                   AccessSpecifier access,
                                   SourceLocation KeywordLoc,
                                   DeclarationName Name,
                                   ASTVector<SourceType> &&conformanceTypes,
                                   ASTVector<TemplateParamDecl*>
                                                            &&templateParams) {
   return new(C) ProtocolDecl(access, KeywordLoc, Name, move(conformanceTypes),
                              move(templateParams));
}

ProtocolDecl::ProtocolDecl(EmptyShell Empty)
   : RecordDecl(Empty, ProtocolDeclID)
{}

ProtocolDecl *ProtocolDecl::CreateEmpty(ASTContext &C)
{
    return new(C) ProtocolDecl(EmptyShell());
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

ExtensionDecl::ExtensionDecl(EmptyShell, unsigned N)
   : NamedDecl(ExtensionDeclID, AccessSpecifier::Default, DeclarationName()),
     DeclContext(ExtensionDeclID),
     ExtendedRecord(nullptr), NumConformances(N)
{}

ExtensionDecl *ExtensionDecl::CreateEmpty(ASTContext &C, unsigned N)
{
    void *Mem = C.Allocate(totalSizeToAlloc<SourceType>(N), alignof(ExtensionDecl));
    return new(Mem) ExtensionDecl(EmptyShell(), N);
}

MethodDecl* MethodDecl::Create(ASTContext &C,
                               AccessSpecifier access,
                               SourceLocation DefLoc,
                               DeclarationName Name,
                               SourceType returnType,
                               llvm::ArrayRef<FuncArgDecl*> args,
                               ASTVector<TemplateParamDecl*> &&templateParams,
                               Statement *body, bool isStatic) {
   assert(isStatic || (!args.empty() && args.front()->isSelf())
                      && "no self argument!");

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
                       ASTVector<TemplateParamDecl*> &&templateParams,
                       Statement *body,
                       bool isStatic)
   : CallableDecl(MethodDeclID, access, DefLoc, OperatorName, returnType,
                  move(args), body, move(templateParams)) {
   setDeclFlag(DF_Static, isStatic);
}

MethodDecl* MethodDecl::CreateOperator(ASTContext &C,
                              AccessSpecifier access,
                              SourceLocation DefLoc,
                              DeclarationName OperatorName,
                              SourceType returnType,
                              llvm::ArrayRef<FuncArgDecl*> args,
                              ASTVector<TemplateParamDecl*> &&templateParams,
                              Statement *body,
                              bool isStatic) {
   assert(isStatic || (!args.empty() && args.front()->isSelf())
                      && "no self argument!");

   void *Mem = C.Allocate(sizeof(MethodDecl)
                          + args.size() * sizeof(FuncArgDecl*),
                          alignof(MethodDecl));

   return new(Mem) MethodDecl(access, DefLoc, OperatorName, returnType,
                              args, move(templateParams), body,
                              isStatic);
}

MethodDecl::MethodDecl(AccessSpecifier access,
                       SourceLocation DefLoc,
                       SourceType returnType,
                       llvm::ArrayRef<FuncArgDecl*> args,
                       ASTVector<TemplateParamDecl*> &&templateParams,
                       Statement *body)
   : CallableDecl(MethodDeclID, access, DefLoc, DeclarationName(),
                  returnType, move(args), body, move(templateParams)) {
   setFlag(ConvOp, true);
}

MethodDecl* MethodDecl::CreateConversionOp(ASTContext &C,
                               AccessSpecifier access,
                               SourceLocation DefLoc,
                               SourceType returnType,
                               llvm::ArrayRef<FuncArgDecl*> args,
                               ASTVector<TemplateParamDecl*> &&templateParams,
                               Statement *body) {
   assert(!args.empty() && args.front()->isSelf() && "no self argument!");

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
                       ASTVector<TemplateParamDecl*> &&templateParams,
                       Statement *body)
   : CallableDecl(typeID, access, DefLoc, Name,
                  returnType, move(args), body, move(templateParams)) {

}

InitDecl::InitDecl(AccessSpecifier am,
                   SourceLocation Loc,
                   DeclarationName Name)
   : MethodDecl(InitDeclID, am, Loc, Name, SourceType(), {}, {}, {}),
     Kind(Name.getKind() == DeclarationName::BaseConstructorName
            ? ConstructorKind::Base : ConstructorKind::Complete),
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
                   ASTVector<TemplateParamDecl*> &&templateParams,
                   Statement *body, DeclarationName Name,
                   bool IsFallible)
   : MethodDecl(InitDeclID, am, Loc, Name, SourceType(), move(args),
                move(templateParams), body),
     Kind(Name.getKind() == DeclarationName::BaseConstructorName
          ? ConstructorKind::Base : ConstructorKind::Complete),
     ExplicitMemberwise(false), IsFallible(IsFallible)
{
   setDeclFlag(DF_Static, true);
}

InitDecl* InitDecl::Create(ASTContext &C,
                           AccessSpecifier am,
                           SourceLocation Loc,
                           llvm::ArrayRef<FuncArgDecl*> args,
                           ASTVector<TemplateParamDecl*> &&templateParams,
                           Statement *body,
                           DeclarationName Name,
                           bool IsFallible) {
   void *Mem = C.Allocate(sizeof(InitDecl)
                             + args.size() * sizeof(FuncArgDecl*),
                          alignof(InitDecl));

   return new(Mem) InitDecl(am, Loc, args, move(templateParams), body, Name,
                            IsFallible);
}

DeinitDecl::DeinitDecl(SourceLocation Loc,
                       Statement *body,
                       llvm::ArrayRef<FuncArgDecl*> args,
                       DeclarationName Name)
   : MethodDecl(DeinitDeclID, AccessSpecifier::Public, Loc,
                Name, SourceType(), args, {}, body)
{

}

DeinitDecl* DeinitDecl::Create(ASTContext &C,
                               SourceLocation Loc,
                               Statement *body,
                               llvm::ArrayRef<FuncArgDecl*> args,
                               DeclarationName Name) {
   void *Mem = C.Allocate(sizeof(DeinitDecl)
                          + args.size() * sizeof(FuncArgDecl*),
                          alignof(DeinitDecl));

   return new(Mem) DeinitDecl(Loc, body, args, Name);
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

FieldDecl::FieldDecl(EmptyShell)
   : VarDecl(FieldDeclID, AccessSpecifier::Default, {}, {}, false,
             DeclarationName(), SourceType(), nullptr)
{}

FieldDecl *FieldDecl::CreateEmpty(ASTContext &C)
{
    return new(C) FieldDecl(EmptyShell());
}

AssociatedTypeDecl::AssociatedTypeDecl(SourceLocation Loc,
                                       IdentifierInfo *ProtoSpec,
                                       DeclarationName Name,
                                       SourceType actualType,
                                       bool Implementation)
   : NamedDecl(AssociatedTypeDeclID, AccessSpecifier::Public, Name),
     Loc(Loc), protocolSpecifier(ProtoSpec), actualType(actualType),
     Implementation(Implementation)
{

}

AssociatedTypeDecl* AssociatedTypeDecl::Create(ASTContext &C,
                                               SourceLocation Loc,
                                               IdentifierInfo *ProtoSpec,
                                               DeclarationName Name,
                                               SourceType actualType,
                                               bool Implementation) {
   return new(C) AssociatedTypeDecl(Loc, ProtoSpec, Name, actualType,
                                    Implementation);
}

AssociatedTypeDecl::AssociatedTypeDecl(EmptyShell)
   : NamedDecl(AssociatedTypeDeclID, AccessSpecifier::Default,
               DeclarationName()),
     protocolSpecifier(nullptr), Implementation(false)
{}

AssociatedTypeDecl *AssociatedTypeDecl::CreateEmpty(ASTContext &C)
{
    return new(C) AssociatedTypeDecl(EmptyShell());
}

PropDecl::PropDecl(AccessSpecifier access,
                   SourceRange Loc,
                   DeclarationName propName,
                   SourceType type,
                   bool isStatic,
                   MethodDecl *GetterMethod,
                   MethodDecl *SetterMethod)
   : NamedDecl(PropDeclID, access, propName),
     Loc(Loc), type(type),
     getterMethod(GetterMethod), setterMethod(SetterMethod)
{
   setDeclFlag(DF_Static, isStatic);
}

PropDecl* PropDecl::Create(ASTContext &C,
                           AccessSpecifier access,
                           SourceRange Loc,
                           DeclarationName propName,
                           SourceType type,
                           bool isStatic,
                           MethodDecl *GetterMethod,
                           MethodDecl *SetterMethod) {
   return new(C) PropDecl(access, Loc, propName, type, isStatic,
                          GetterMethod, SetterMethod);
}

PropDecl::PropDecl(EmptyShell)
   : NamedDecl(PropDeclID, AccessSpecifier::Default, DeclarationName())
{}

PropDecl *PropDecl::CreateEmpty(ASTContext &C)
{
    return new(C) PropDecl(EmptyShell());
}

SubscriptDecl::SubscriptDecl(AccessSpecifier access,
                             SourceRange Loc,
                             DeclarationName Name,
                             SourceType type,
                             MethodDecl *GetterMethod,
                             MethodDecl *SetterMethod)
   : NamedDecl(SubscriptDeclID, access, Name),
     Loc(Loc), type(type),
     getterMethod(GetterMethod), setterMethod(SetterMethod)
{

}

SubscriptDecl* SubscriptDecl::Create(ASTContext &C,
                                     AccessSpecifier access,
                                     SourceRange Loc,
                                     SourceType type,
                                     MethodDecl *GetterMethod,
                                     MethodDecl *SetterMethod) {
   return new(C) SubscriptDecl(access, Loc,
                                 C.getDeclNameTable().getSubscriptName(
                                    DeclarationName::SubscriptKind::General),
                                 type, GetterMethod, SetterMethod);
}

SubscriptDecl::SubscriptDecl(EmptyShell)
   : NamedDecl(SubscriptDeclID, AccessSpecifier::Default, DeclarationName())
{}

SubscriptDecl *SubscriptDecl::CreateEmpty(ASTContext &C)
{
    return new(C) SubscriptDecl(EmptyShell());
}

EnumCaseDecl::EnumCaseDecl(AccessSpecifier AS,
                           SourceLocation CaseLoc,
                           SourceLocation IdentLoc,
                           DeclarationName Name,
                           StaticExpr *rawValue,
                           llvm::ArrayRef<FuncArgDecl*> args)
   : CallableDecl(EnumCaseDeclID, AS, CaseLoc, Name, SourceType(),
                  args, nullptr, {}),
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

EnumCaseDecl::EnumCaseDecl(EmptyShell E, unsigned N)
   : CallableDecl(E, EnumCaseDeclID, N)
{}

EnumCaseDecl *EnumCaseDecl::CreateEmpty(ASTContext &C, unsigned N)
{
   void *Mem = C.Allocate(sizeof(EnumCaseDecl)
                          + N * sizeof(FuncArgDecl*),
                          alignof(EnumCaseDecl));

   return new(Mem) EnumCaseDecl(EmptyShell(), N);
}

StaticIfDecl::StaticIfDecl(SourceLocation StaticLoc,
                           SourceLocation RBRaceLoc,
                           StaticExpr *condition,
                           CompoundDecl *IfDecl,
                           CompoundDecl *ElseDecl)
   : Decl(StaticIfDeclID),
     StaticLoc(StaticLoc), RBRaceLoc(RBRaceLoc),
     condition(condition), IfDecl(IfDecl), ElseDecl(ElseDecl), Template(nullptr)
{

}

StaticIfDecl::StaticIfDecl(SourceLocation StaticLoc,
                           SourceLocation RBRaceLoc,
                           StaticExpr *condition,
                           StaticIfDecl *Template)
   : Decl(StaticIfDeclID),
     StaticLoc(StaticLoc), RBRaceLoc(RBRaceLoc),
     condition(condition), IfDecl(nullptr), ElseDecl(nullptr),
     Template(Template)
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

StaticIfDecl* StaticIfDecl::Create(ASTContext &C,
                                   SourceLocation StaticLoc,
                                   SourceLocation RBRaceLoc,
                                   StaticExpr *condition,
                                   StaticIfDecl *Template) {
   return new(C) StaticIfDecl(StaticLoc, RBRaceLoc, condition, Template);
}

StaticIfDecl::StaticIfDecl(EmptyShell)
   : Decl(StaticIfDeclID)
{}

StaticIfDecl *StaticIfDecl::CreateEmpty(ASTContext &C)
{
    return new(C) StaticIfDecl(EmptyShell());
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

StaticForDecl::StaticForDecl(EmptyShell)
   : Decl(StaticForDeclID)
{}

StaticForDecl *StaticForDecl::CreateEmpty(ASTContext &C)
{
    return new(C) StaticForDecl(EmptyShell());
}

StaticAssertStmt::StaticAssertStmt(SourceLocation Loc,
                                   SourceRange Parens,
                                   StaticExpr* expr,
                                   StringRef message)
   : Decl(StaticAssertStmtID),
     Loc(Loc), Parens(Parens),
     expr(expr), message(message)
{}

StaticAssertStmt* StaticAssertStmt::Create(ASTContext &C,
                                           SourceLocation Loc,
                                           SourceRange Parens,
                                           StaticExpr *expr,
                                           StringRef message) {
   return new(C) StaticAssertStmt(Loc, Parens, expr, message);
}

StaticAssertStmt::StaticAssertStmt(EmptyShell)
   : Decl(StaticAssertStmtID)
{}

StaticAssertStmt *StaticAssertStmt::CreateEmpty(ASTContext &C)
{
    return new(C) StaticAssertStmt(EmptyShell());
}

StaticPrintStmt::StaticPrintStmt(SourceLocation Loc,
                                 SourceRange Parens,
                                 Expression* expr)
   : Decl(StaticPrintStmtID),
     Loc(Loc), Parens(Parens), expr(expr)
{}

StaticPrintStmt* StaticPrintStmt::Create(ASTContext &C,
                                         SourceLocation Loc,
                                         SourceRange Parens,
                                         Expression *E) {
   return new(C) StaticPrintStmt(Loc, Parens, E);
}

StaticPrintStmt::StaticPrintStmt(EmptyShell)
   : Decl(StaticPrintStmtID)
{}

StaticPrintStmt *StaticPrintStmt::CreateEmpty(ASTContext &C)
{
    return new(C) StaticPrintStmt(EmptyShell());
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

MixinDecl::MixinDecl(EmptyShell)
   : Decl(MixinDeclID)
{}

MixinDecl *MixinDecl::CreateEmpty(ASTContext &C)
{
    return new(C) MixinDecl(EmptyShell());
}

PrecedenceGroupDecl::PrecedenceGroupDecl(SourceRange SR,
                                         AccessSpecifier AS,
                                         DeclarationName DN,
                                         Associativity Assoc,
                                         const IdentifierInfo *HigherThanIdent,
                                         const IdentifierInfo *LowerThanIdent,
                                         bool IsAssignment)
   : NamedDecl(PrecedenceGroupDeclID, AS, DN),
     SR(SR), Assoc(Assoc), IsAssignment(IsAssignment),
     HigherThanIdent(HigherThanIdent), LowerThanIdent(LowerThanIdent)
{

}

PrecedenceGroupDecl*
PrecedenceGroupDecl::Create(ASTContext &C,
                            SourceRange SR,
                            AccessSpecifier AS,
                            DeclarationName DN,
                            Associativity Assoc,
                            const IdentifierInfo *HigherThanIdent,
                            const IdentifierInfo *LowerThanIdent,
                            bool IsAssignment) {
   return new(C) PrecedenceGroupDecl(SR, AS, DN, Assoc, HigherThanIdent,
                                     LowerThanIdent, IsAssignment);
}

PrecedenceGroupDecl::PrecedenceGroupDecl(EmptyShell)
   : NamedDecl(PrecedenceGroupDeclID, AccessSpecifier::Default,
               DeclarationName()),
     SR(), Assoc(), IsAssignment(false),
     HigherThanIdent(nullptr), LowerThanIdent(nullptr)
{}

PrecedenceGroupDecl *PrecedenceGroupDecl::CreateEmpty(ASTContext &C)
{
    return new(C) PrecedenceGroupDecl(EmptyShell());
}

static PrecedenceGroupDecl::Relationship
Reverse(PrecedenceGroupDecl::Relationship Rel)
{
   switch (Rel) {
   case PrecedenceGroupDecl::Lower:
      return PrecedenceGroupDecl::Higher;
   case PrecedenceGroupDecl::Higher:
      return PrecedenceGroupDecl::Lower;
   default:
      return Rel;
   }
}

PrecedenceGroupDecl::Relationship
PrecedenceGroupDecl::getRelationTo(PrecedenceGroupDecl *Other,
                                   bool CheckReverse) {
   if (Other == this)
      return Equal;

   auto Curr = HigherThan;
   while (Curr) {
      if (Curr == Other) {
         return Higher;
      }

      Curr = Curr->HigherThan;
   }

   Curr = LowerThan;
   while (Curr) {
      if (Curr == Other) {
         return Lower;
      }

      Curr = Curr->LowerThan;
   }

   if (CheckReverse)
      return Reverse(Other->getRelationTo(this, false));

   return Undefined;
}

OperatorDecl::OperatorDecl(SourceRange SR,
                           AccessSpecifier AS,
                           DeclarationName Name,
                           const IdentifierInfo *PGIdent)
   : NamedDecl(OperatorDeclID, AS, Name),
     SR(SR), OperatorName(Name.getDeclaredOperatorName()),
     PrecedenceGroupIdent(PGIdent)
{

}

OperatorDecl* OperatorDecl::Create(ASTContext &C,
                                   SourceRange SR,
                                   AccessSpecifier AS,
                                   DeclarationName OperatorName,
                                   const IdentifierInfo *PGIdent) {
   auto DN = C.getDeclNameTable().getOperatorDeclName(OperatorName);
   return new(C) OperatorDecl(SR, AS, DN, PGIdent);
}

OperatorDecl::OperatorDecl(EmptyShell)
   : NamedDecl(OperatorDeclID, AccessSpecifier::Default, DeclarationName()),
     SR(), OperatorName(),
     PrecedenceGroupIdent(nullptr)
{}

OperatorDecl *OperatorDecl::CreateEmpty(ASTContext &C)
{
    return new(C) OperatorDecl(EmptyShell());
}

PatternFragment PatternFragment::ErrorState(Error);

PatternFragment::PatternFragment(FragmentKind Kind)
   : Kind(Kind),  HasDelimiterTok(false)
{

}

PatternFragment::PatternFragment(SourceLocation Loc,
                                 llvm::ArrayRef<lex::Token> Tokens)
   : Kind(FragmentKind::Tokens), HasDelimiterTok(false),
     Loc(Loc), TokData{ (unsigned)Tokens.size() }
{
   std::copy(Tokens.begin(), Tokens.end(),
             reinterpret_cast<lex::Token*>(this + 1));
}

PatternFragment::PatternFragment(SourceLocation Loc,
                                 VariableKind VarKind,
                                 const IdentifierInfo *VarName)
   : Kind(Variable), HasDelimiterTok(false),
     Loc(Loc), VarData{ VarKind, VarName }
{

}

PatternFragment::PatternFragment(SourceLocation Loc,
                                 FragmentKind RepetitionKind,
                                 PatternFragment *BeginState,
                                 PatternFragment *EndState,
                                 const lex::Token &Delimiter)
   : Kind(RepetitionKind),
     HasDelimiterTok(Delimiter.getKind() != lex::tok::sentinel),
     Loc(Loc), RepData{ BeginState, EndState }
{
   if (HasDelimiterTok) {
      *(reinterpret_cast<lex::Token*>(this + 1)) = Delimiter;
   }
}

PatternFragment* PatternFragment::Create(ASTContext &C)
{
   return new(C) PatternFragment(Empty);
}

PatternFragment* PatternFragment::Create(ASTContext &C,
                                         SourceLocation Loc,
                                         llvm::ArrayRef<lex::Token> Tokens) {
   void *Mem = C.Allocate(sizeof(PatternFragment)
                          + sizeof(lex::Token) * Tokens.size(),
                          alignof(PatternFragment));

   return new(Mem) PatternFragment(Loc, Tokens);
}

PatternFragment* PatternFragment::Create(ASTContext &C,
                                         SourceLocation Loc,
                                         VariableKind VarKind,
                                         const IdentifierInfo *VarName) {
   return new(C) PatternFragment(Loc, VarKind, VarName);
}

PatternFragment* PatternFragment::Create(ASTContext &C,
                                         SourceLocation Loc,
                                         FragmentKind RepetitionKind,
                                         PatternFragment *BeginState,
                                         PatternFragment *EndState,
                                         const lex::Token &Delimiter) {
   size_t Size = sizeof(PatternFragment);

   if (Delimiter.getKind() != lex::tok::sentinel) {
      Size += sizeof(lex::Token);
   }

   void *Mem = C.Allocate(Size, alignof(PatternFragment));
   return new(Mem) PatternFragment(Loc, RepetitionKind, BeginState, EndState,
                                   Delimiter);
}

ExpansionFragment::ExpansionFragment(SourceLocation Loc,
                                     llvm::ArrayRef<lex::Token> Tokens)
   : Kind(FragmentKind::Tokens), Loc(Loc), TokData{ (unsigned)Tokens.size() }
{
   std::copy(Tokens.begin(), Tokens.end(),
             reinterpret_cast<lex::Token*>(this + 1));
}

ExpansionFragment::ExpansionFragment(SourceLocation Loc,
                                     const IdentifierInfo *VarName)
   : Kind(Variable), Loc(Loc), VarData{ VarName }
{

}

ExpansionFragment::ExpansionFragment(SourceLocation Loc,
                                  llvm::ArrayRef<ExpansionFragment *> Fragments,
                                     const IdentifierInfo *ExpandedVariable)
   : Kind(Expansion), Loc(Loc),
     RepData{ (unsigned)Fragments.size(), ExpandedVariable }
{
   std::copy(Fragments.begin(), Fragments.end(),
             reinterpret_cast<ExpansionFragment**>(this + 1));
}

ExpansionFragment::ExpansionFragment(SourceLocation Loc,
                                     llvm::ArrayRef<ExpansionFragment*> Fragments)
   : Kind(ConcatExpr), Loc(Loc),
     RepData{ (unsigned)Fragments.size() }
{
   std::copy(Fragments.begin(), Fragments.end(),
             reinterpret_cast<ExpansionFragment**>(this + 1));
}

ExpansionFragment* ExpansionFragment::Create(ASTContext &C,
                                         SourceLocation Loc,
                                         llvm::ArrayRef<lex::Token> Tokens) {
   void *Mem = C.Allocate(sizeof(ExpansionFragment)
                          + sizeof(lex::Token) * Tokens.size(),
                          alignof(ExpansionFragment));

   return new(Mem) ExpansionFragment(Loc, Tokens);
}

ExpansionFragment* ExpansionFragment::Create(ASTContext &C,
                                             SourceLocation Loc,
                                             const IdentifierInfo *VarName) {
   return new(C) ExpansionFragment(Loc, VarName);
}

ExpansionFragment*
ExpansionFragment::Create(ASTContext &C,
                        SourceLocation Loc,
                        llvm::ArrayRef<ExpansionFragment*> Fragments,
                          const IdentifierInfo *ExpandedVariable) {
   size_t Size = sizeof(ExpansionFragment)
                 + sizeof(ExpansionFragment*) * Fragments.size();

   void *Mem = C.Allocate(Size, alignof(ExpansionFragment));
   return new(Mem) ExpansionFragment(Loc, Fragments, ExpandedVariable);
}

ExpansionFragment*
ExpansionFragment::Create(ASTContext &C,
                          SourceLocation Loc,
                          llvm::ArrayRef<ExpansionFragment*> Fragments) {
   size_t Size = sizeof(ExpansionFragment)
                 + sizeof(ExpansionFragment*) * Fragments.size();

   void *Mem = C.Allocate(Size, alignof(ExpansionFragment));
   return new(Mem) ExpansionFragment(Loc, Fragments);
}

MacroPattern::MacroPattern(SourceLocation Loc,
                           PatternFragment* Pattern,
                           SourceLocation ExpansionLoc,
                           llvm::ArrayRef<ExpansionFragment *> Expansion,
                           unsigned SourceLength)
   : Loc(Loc), ExpansionLoc(ExpansionLoc),
     Pattern(Pattern), SourceLength(SourceLength),
     NumExpansionFragments((unsigned)Expansion.size())
{
   std::copy(Expansion.begin(), Expansion.end(),
             getTrailingObjects<ExpansionFragment*>());
}

MacroPattern* MacroPattern::Create(ASTContext &C,
                                   SourceLocation Loc,
                                   PatternFragment* Pattern,
                                   SourceLocation ExpansionLoc,
                                   llvm::ArrayRef<ExpansionFragment*>Expansion,
                                   unsigned SourceLength){
   void *Mem = C.Allocate(sizeof(MacroPattern)
                          + sizeof(ExpansionFragment*) * Expansion.size(),
                          alignof(MacroPattern));

   return new(Mem) MacroPattern(Loc, Pattern, ExpansionLoc, Expansion,
                                SourceLength);
}

SourceRange MacroPattern::getSourceRange() const
{
   SourceLocation End;
   if (!NumExpansionFragments) {
      End = Loc;
   }
   else {
      End = getExpansion().back()->getLoc();
   }

   return SourceRange(Loc, End);
}

MacroDecl::MacroDecl(SourceRange SR,
                     AccessSpecifier AS,
                     DeclarationName Name,
                     Delimiter Delim,
                     llvm::ArrayRef<MacroPattern *> Patterns)
   : NamedDecl(MacroDeclID, AS, Name),
     SR(SR), Delim(Delim), NumPatterns((unsigned)Patterns.size())
{
   std::copy(Patterns.begin(), Patterns.end(),
             getTrailingObjects<MacroPattern*>());
}

MacroDecl* MacroDecl::Create(ASTContext &C,
                             SourceRange SR,
                             AccessSpecifier AS,
                             DeclarationName Name,
                             Delimiter Delim,
                             llvm::ArrayRef<MacroPattern *> Patterns) {
   void *Mem = C.Allocate(totalSizeToAlloc<MacroPattern*>(Patterns.size()),
                          alignof(MacroDecl));

   return new(Mem) MacroDecl(SR, AS, Name, Delim, Patterns);
}

MacroDecl::MacroDecl(EmptyShell, unsigned N)
   : NamedDecl(MacroDeclID, AccessSpecifier::Default, DeclarationName()),
     Delim(Delimiter::Paren), NumPatterns(N)
{}

MacroDecl *MacroDecl::CreateEmpty(ASTContext &C, unsigned N)
{
    void *Mem = C.Allocate(totalSizeToAlloc<MacroPattern*>(N),
                           alignof(MacroDecl));
    return new(Mem) MacroDecl(EmptyShell(), N);
}

MacroExpansionDecl::MacroExpansionDecl(SourceRange SR,
                                       DeclarationName MacroName,
                                       Expression *ParentExpr,
                                       Delimiter Delim,
                                       llvm::ArrayRef<lex::Token> Toks)
   : Decl(MacroExpansionDeclID),
     SR(SR), Delim(Delim), MacroName(MacroName),
     NumTokens((unsigned)Toks.size()), ParentExpr(ParentExpr)
{
   std::copy(Toks.begin(), Toks.end(), getTrailingObjects<lex::Token>());
}

MacroExpansionDecl* MacroExpansionDecl::Create(ASTContext &C,
                                               SourceRange SR,
                                               DeclarationName MacroName,
                                               Expression *ParentExpr,
                                               Delimiter Delim,
                                               llvm::ArrayRef<lex::Token> Toks){
   void *Mem = C.Allocate(totalSizeToAlloc<lex::Token>(Toks.size()),
                          alignof(MacroExpansionDecl));

   return new(Mem) MacroExpansionDecl(SR, MacroName, ParentExpr, Delim, Toks);
}

MacroExpansionDecl::MacroExpansionDecl(EmptyShell, unsigned N)
   : Decl(MacroExpansionDeclID),
     Delim(Delimiter::Paren), NumTokens(N), ParentExpr(nullptr)
{}

MacroExpansionDecl *MacroExpansionDecl::CreateEmpty(ASTContext &C, unsigned N)
{
    void *Mem = C.Allocate(totalSizeToAlloc<lex::Token>(N),
                           alignof(MacroExpansionDecl));
    return new(Mem) MacroExpansionDecl(EmptyShell(), N);
}

UnittestDecl::UnittestDecl(SourceLocation KeywordLoc,
                           SourceRange BraceRange,
                           IdentifierInfo *Name,
                           Statement *Body)
   : Decl(UnittestDeclID), DeclContext(UnittestDeclID),
     KeywordLoc(KeywordLoc), BraceRange(BraceRange), Name(Name), Body(Body)
{

}

UnittestDecl::UnittestDecl()
   : Decl(UnittestDeclID), DeclContext(UnittestDeclID),
     KeywordLoc(), BraceRange(), Name(nullptr), Body(nullptr)
{

}

UnittestDecl* UnittestDecl::Create(ASTContext &C,
                                   SourceLocation KeywordLoc,
                                   SourceRange BraceRange,
                                   IdentifierInfo *Name,
                                   Statement *Body) {
   return new(C) UnittestDecl(KeywordLoc, BraceRange, Name, Body);
}

UnittestDecl* UnittestDecl::CreateEmpty(ASTContext &C)
{
   return new(C) UnittestDecl;
}

SourceRange UnittestDecl::getSourceRange() const
{
   return SourceRange(KeywordLoc, BraceRange.getEnd());
}

DebugDecl::DebugDecl(SourceLocation Loc)
   : Decl(DebugDeclID),
     Loc(Loc)
{

}

DebugDecl* DebugDecl::Create(ASTContext &C, SourceLocation Loc)
{
   return new(C) DebugDecl(Loc);
}

DebugDecl* DebugDecl::CreateEmpty(ASTContext &C)
{
   return new(C) DebugDecl(SourceLocation());
}

SourceRange DebugDecl::getSourceRange() const
{
   return SourceRange(Loc, SourceLocation(Loc.getOffset() + 7));
}

} // namespace ast
} // namespace cdot