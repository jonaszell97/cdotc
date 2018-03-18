//
// Created by Jonas Zell on 26.01.18.
//

#include "NamedDecl.h"

#include "AST/ASTContext.h"
#include "AST/Passes/SemanticAnalysis/Builtin.h"
#include "AST/Passes/SemanticAnalysis/SemaPass.h"

#include <llvm/ADT/StringSwitch.h>
#include <llvm/ADT/SmallString.h>

using namespace cdot::support;

namespace cdot {
namespace ast {

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
      if constexpr (&Decl::getSourceRange == &SubClass::getSourceRange)       \
         return SourceRange(sourceLoc);                                       \
      else                                                                    \
         return support::cast<SubClass>(this)->getSourceRange();
#  include "AST/Decl.def"

   default:
      llvm_unreachable("not a decl");
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

llvm::StringRef NamedDecl::getNameWithoutNamespace() const
{
   return name;
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

   for (auto ctx = declContext; ctx; ctx = ctx->getParentCtx()) {
      if (auto ND = dyn_cast<NamedDecl>(ctx)) {
         joinedName += ND->getJoinedName(join, includeFile);

         if (!joinedName.empty())
            joinedName += join;

         break;
      }
   }

   joinedName += name;
   return joinedName;
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
                                 std::string &&fileName,
                                 size_t sourceId,
                                 llvm::ArrayRef<ImportStmt*> imports)
   : NamedDecl(TranslationUnitID, (AccessModifier)0, move(fileName)),
     DeclContext(TranslationUnitID),
     ASTCtx(ASTCtx), sourceId(sourceId),
     numImports(unsigned(imports.size()))
{
   std::copy(imports.begin(), imports.end(), import_begin());
}

TranslationUnit* TranslationUnit::Create(ASTContext &ASTCtx,
                                         llvm::StringRef fileName,
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


CallableDecl::CallableDecl(DeclKind typeID,
                           AccessModifier am,
                           std::string &&name,
                           SourceType returnType,
                           std::vector<FuncArgDecl* > &&args,
                           std::vector<StaticExpr* > &&Constraints,
                           Statement* body,
                           OperatorInfo op)
   : NamedDecl(typeID, am, move(name), move(Constraints)),
     DeclContext(typeID),
     returnType(returnType), args(move(args)),
     body(body), op(op), Flags(0)
{
   for (auto &arg : this->args) {
      arg->setDeclContext(this);
      makeDeclAvailable(arg);
   }
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
      args.push_back(arg->getTypeRef());

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

llvm::StringRef CallableDecl::getNameWithoutFix() const
{
   if (!isOperator())
      return getNameWithoutNamespace();

   size_t opLen;
   switch (op.getFix()) {
   case FixKind::Infix:
      opLen = 6; // "infix "
      break;
   case FixKind::Prefix:
      opLen = 7; // "prefix "
      break;
   case FixKind::Postfix:
      opLen = 8; // "postfix "
      break;
   }

   return getNameWithoutNamespace().substr(opLen);
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

   knownFnKind = llvm::StringSwitch<KnownFunction>(name)
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


void RecordDecl::addInnerRecord(RecordDecl *R)
{
   R->setOuterRecord(this);
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

llvm::StringRef RecordDecl::getOwnName() const
{
   if (!outerRecord)
      return name;

   return llvm::StringRef(
      name.data() + outerRecord->getName().size() + 1,
      name.size() - outerRecord->getName().size() - 1);
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

AssociatedTypeDecl* RecordDecl::getAssociatedType(llvm::StringRef name,
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
   for (auto &decl : getDecls()) {
      auto Method = dyn_cast<MethodDecl>(decl);
      if (!Method)
         continue;

      if (!Method->isCastOp())
         continue;

      if (Method->getReturnType() == toType)
         return Method;
   }

   if (auto C = dyn_cast<ClassDecl>(this)) {
      if (auto P = C->getParentClass()) {
         return P->getConversionOperator(toType);
      }
   }

   return nullptr;
}

MethodDecl* RecordDecl::getComparisonOperator(QualType withType) const
{
   for (auto &decl : getDecls()) {
      auto Method = dyn_cast<MethodDecl>(decl);
      if (!Method)
         continue;

      if (Method->getNameWithoutFix() != "==" || Method->getArgs().size() != 1)
         continue;

      if (Method->getArgs().front()->getArgType() == withType)
         return Method;
   }

   if (auto C = dyn_cast<ClassDecl>(this)) {
      if (auto P = C->getParentClass()) {
         return P->getComparisonOperator(withType);
      }
   }

   return nullptr;
}

bool RecordDecl::hasMethodWithName(llvm::StringRef name) const
{
   for (auto &decl : getDecls()) {
      auto Method = dyn_cast<MethodDecl>(decl);
      if (Method && Method->getName() == name)
         return true;
   }

   if (auto C = dyn_cast<ClassDecl>(this)) {
      if (auto P = C->getParentClass()) {
         return P->hasMethodWithName(name);
      }
   }

   return false;
}

bool RecordDecl::hasMethodTemplate(llvm::StringRef name) const
{
   for (auto &decl : getDecls()) {
      auto Method = dyn_cast<MethodDecl>(decl);
      if (Method && Method->getName() == name && Method->isTemplate())
         return true;
   }

   if (auto C = dyn_cast<ClassDecl>(this)) {
      if (auto P = C->getParentClass()) {
         return P->hasMethodTemplate(name);
      }
   }

   return false;
}

PropDecl* RecordDecl::getProperty(llvm::StringRef name) const
{
   return dyn_cast_or_null<PropDecl>(lookupSingle(name));
}

FieldDecl* RecordDecl::getField(llvm::StringRef name) const
{
   return dyn_cast_or_null<FieldDecl>(lookupSingle(name));
}

MethodDecl* RecordDecl::getMethod(llvm::StringRef name, bool checkParent) const
{
   auto M = dyn_cast_or_null<MethodDecl>(lookupSingle(name));
   if (M)
      return M;

   for (auto &decl : getDecls()) {
      auto MDecl = dyn_cast<MethodDecl>(decl);
      if (MDecl && MDecl->getLinkageName() == name)
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

MethodDecl* RecordDecl::getOwnMethod(llvm::StringRef name)
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

EnumCaseDecl* EnumDecl::hasCase(llvm::StringRef name)
{
   return lookupSingle<EnumCaseDecl>(name);
}

bool MethodDecl::isTemplatedInitializer() const
{
   if (!support::isa<InitDecl>(this))
      return false;

   return getTemplateParams().size()
          > support::cast<RecordDecl>(declContext)->getTemplateParams().size();
}

MixinDecl::MixinDecl(SourceRange Parens, Expression *MixinExpr)
   : Decl(MixinDeclID),
     Parens(Parens), MixinExpr(MixinExpr)
{}

MixinDecl* MixinDecl::Create(ASTContext &C,
                             SourceRange Parens,
                             Expression *MixinExpr) {
   return new(C) MixinDecl(Parens, MixinExpr);
}

} // namespace ast
} // namespace cdot