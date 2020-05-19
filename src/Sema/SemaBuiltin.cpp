#include "cdotc/Basic/Builtins.h"
#include "cdotc/IL/Constants.h"
#include "cdotc/IL/ILBuilder.h"
#include "cdotc/ILGen/ILGenPass.h"
#include "cdotc/Module/Module.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/SemaPass.h"

using namespace cdot::builtin;
using namespace cdot::diag;
using namespace cdot::support;

namespace cdot {
namespace ast {

void SemaPass::initBuiltinIdents()
{
   static_assert(sizeof(BuiltinIdents) / sizeof(IdentifierInfo*)
                     > (unsigned)_lastBuiltin,
                 "not enough space for Builtin identifiers!");

   auto& Idents = Context.getIdentifiers();

   BuiltinIdents[allocStack] = &Idents.get("allocStack");
   BuiltinIdents[addressOf] = &Idents.get("addressOf");
   BuiltinIdents[reinterpretCast] = &Idents.get("reinterpretCast");
   BuiltinIdents[unsafeConstCast] = &Idents.get("unsafeConstCast");
   BuiltinIdents[loadFromPointer] = &Idents.get("loadFromPointer");
   BuiltinIdents[storeToPointer] = &Idents.get("storeToPointer");
   BuiltinIdents[constructInPlace] = &Idents.get("constructInPlace");
   BuiltinIdents[copy] = &Idents.get("copy");
   BuiltinIdents[deinit] = &Idents.get("deinit");
   BuiltinIdents[builtin::move] = &Idents.get("move");
   BuiltinIdents[consume] = &Idents.get("consume");
   BuiltinIdents[retainValue] = &Idents.get("retainValue");
   BuiltinIdents[releaseValue] = &Idents.get("releaseValue");
   BuiltinIdents[printCTFEStacktrace] = &Idents.get("printCTFEStacktrace");
   BuiltinIdents[llvm_intrinsic] = &Idents.get("llvm_intrinsic");

   BuiltinIdents[atomic_load] = &Idents.get("atomic_load");
   BuiltinIdents[atomic_store] = &Idents.get("atomic_store");
   BuiltinIdents[atomic_cmpxchg] = &Idents.get("atomic_cmpxchg");
   BuiltinIdents[atomic_rmw] = &Idents.get("atomic_rmw");

   BuiltinIdents[builtin::memset] = &Idents.get("memset");
   BuiltinIdents[builtin::memcpy] = &Idents.get("memcpy");
   BuiltinIdents[builtin::likely] = &Idents.get("likely");
   BuiltinIdents[builtin::unlikely] = &Idents.get("unlikely");

   BuiltinIdents[builtin::TokenType] = &Idents.get("TokenType");
   BuiltinIdents[builtin::CVoid] = &Idents.get("CVoid");
   BuiltinIdents[builtin::RawPointer] = &Idents.get("RawPointer");
   BuiltinIdents[builtin::MutableRawPointer] = &Idents.get("MutableRawPointer");
   BuiltinIdents[builtin::Int1] = &Idents.get("i1");
   BuiltinIdents[builtin::Int8] = &Idents.get("i8");
   BuiltinIdents[builtin::UInt8] = &Idents.get("u8");
   BuiltinIdents[builtin::Int16] = &Idents.get("i16");
   BuiltinIdents[builtin::UInt16] = &Idents.get("u16");
   BuiltinIdents[builtin::Int32] = &Idents.get("i32");
   BuiltinIdents[builtin::UInt32] = &Idents.get("u32");
   BuiltinIdents[builtin::Int64] = &Idents.get("i64");
   BuiltinIdents[builtin::UInt64] = &Idents.get("u64");
   BuiltinIdents[builtin::Int128] = &Idents.get("i128");
   BuiltinIdents[builtin::UInt128] = &Idents.get("u128");
   BuiltinIdents[builtin::Float32] = &Idents.get("f32");
   BuiltinIdents[builtin::Float64] = &Idents.get("f64");

   BuiltinIdents[builtin::undefValue] = &Idents.get("undefValue");

   BuiltinIdentsInitialized = true;
}

ExprResult SemaPass::HandleBuiltinCall(CallExpr* C)
{
   if (!BuiltinIdentsInitialized)
      initBuiltinIdents();

   auto* Fn = C->getFunc();
   if (!isa<FunctionDecl>(Fn) || Fn->willHaveDefinition()) {
      return C;
   }

   const IdentifierInfo* II;
   if (Fn->getDeclName().isSimpleIdentifier()) {
      II = C->getFunc()->getDeclName().getIdentifierInfo();
   }
   else if (Fn->getDeclName().getKind() == DeclarationName::InstantiationName) {
      II = Fn->getDeclName().getInstantiationName().getIdentifierInfo();
   }
   else {
      return C;
   }

   unsigned i = 0;
   for (auto& Ident : BuiltinIdents) {
      if (Ident == II) {
         C->setBuiltinKind(i);
         break;
      }

      i++;
   }

   switch ((builtin::Builtin)C->getBuiltinKind()) {
   case addressOf: {
      auto Ty
          = C->getArgs().front()->ignoreParensAndImplicitCasts()->getExprType();

      if (!Ty->isReferenceType()) {
         diagnose(C, diag::err_type_mismatch,
                  C->getArgs().front()->getSourceLoc(), Ty,
                  Context.getReferenceType(Ty));
      }

      break;
   }
   case llvm_intrinsic:
      assert(support::isa<StringLiteral>(
          C->getArgs().front()->ignoreParensAndImplicitCasts()));
      break;
   default:
      break;
   }

   C->setKind(CallKind::Builtin);
   return C;
}

ExprResult SemaPass::HandleBuiltinAlias(AliasDecl* Alias, Expression* Expr,
                                        ArrayRef<Expression*> DependentTemplateArgs)
{
   if (!BuiltinIdentsInitialized) {
      initBuiltinIdents();
   }

   if (auto AE = Alias->getAliasExpr()) {
      Expr->setExprType(AE->getExprType());
      return Expr;
   }

   auto It = ReflectionValues.find(Alias);
   if (It != ReflectionValues.end())
      return It->getSecond();

   auto* BuiltinMod = getBuiltinModule();
   if (!BuiltinMod) {
      diagnose(Expr, err_compiler_ns_unknown_entity, Alias->getDeclName(),
               Alias->getSourceRange());

      return ExprError();
   }

   auto Name = Alias->getDeclName().getManglingName();
   if (!Name.isSimpleIdentifier()) {
      diagnose(Expr, err_compiler_ns_unknown_entity, Alias->getDeclName(),
               Alias->getSourceRange());

      return ExprError();
   }

   auto* II = Name.getIdentifierInfo();

   // look the declaration up to make sure it's deserialized.
   const MultiLevelLookupResult* LookupRes;
   if (QC.DirectLookup(LookupRes, BuiltinMod->getDecl(), II)) {
      return ExprError();
   }

   QualType TemplateArgType;
   if (Alias->isInstantiation()) {
      if (Alias->getTemplateArgs().size() != 1
      || !Alias->getTemplateArgs().front().isType()) {
         diagnose(Expr, err_compiler_ns_bad_def, Alias->getDeclName(),
                  Alias->getSourceRange());
      }
      else {
         TemplateArgType = Alias->getTemplateArgs().front().getType();
      }
   }
   else if (!DependentTemplateArgs.empty()) {
      if (DependentTemplateArgs.size() != 1
          || !DependentTemplateArgs.front()->getExprType()->isMetaType()) {
         diagnose(Expr, err_compiler_ns_bad_def, Alias->getDeclName(),
                  Alias->getSourceRange());
      }
      else {
         TemplateArgType
             = DependentTemplateArgs.front()->getExprType()->removeMetaType();
      }
   }

   Expression* ResultExpr = nullptr;
   if (II == BuiltinIdents[builtin::TokenType]) {
      ResultExpr = new (Context)
          IdentifierRefExpr(Expr->getSourceRange(), IdentifierKind::MetaType,
                            Context.getMetaType(Context.getTypedefType(Alias)));
   }
   else if (II == BuiltinIdents[builtin::CVoid]) {
      ResultExpr = new (Context)
          IdentifierRefExpr(Expr->getSourceRange(), IdentifierKind::MetaType,
                            Context.getMetaType(Context.getVoidType()));
   }
   else if ((II == BuiltinIdents[builtin::RawPointer]
   || II == BuiltinIdents[builtin::MutableRawPointer]) && !TemplateArgType) {
      QualType PtrTy;
      if (II == BuiltinIdents[builtin::MutableRawPointer]) {
         PtrTy = Context.getMutablePointerType(Context.getVoidType());
      }
      else {
         PtrTy = Context.getPointerType(Context.getVoidType());
      }

      ResultExpr = new (Context)
          IdentifierRefExpr(
              Expr->getSourceRange(), IdentifierKind::MetaType,
              Context.getMetaType(PtrTy));
   }
   else if (II == BuiltinIdents[builtin::RawPointer]
   || II == BuiltinIdents[builtin::MutableRawPointer]) {
      QualType PtrTy;
      if (II == BuiltinIdents[builtin::MutableRawPointer]) {
         PtrTy = Context.getMutablePointerType(TemplateArgType);
      }
      else {
         PtrTy = Context.getPointerType(TemplateArgType);
      }

      ResultExpr = new (Context)
          IdentifierRefExpr(Expr->getSourceRange(), IdentifierKind::MetaType,
                            Context.getMetaType(PtrTy));
   }
#define CDOT_BUILTIN_TYPE(NAME)                                                \
   else if (II == BuiltinIdents[builtin::NAME])                                \
   {                                                                           \
      ResultExpr = new (Context) IdentifierRefExpr(                            \
          Expr->getSourceRange(), IdentifierKind::MetaType,                    \
          Context.getMetaType(Context.getTypedefType(Alias)));                 \
   }
   CDOT_BUILTIN_TYPE(Int1)
   CDOT_BUILTIN_TYPE(Int8)
   CDOT_BUILTIN_TYPE(UInt8)
   CDOT_BUILTIN_TYPE(Int16)
   CDOT_BUILTIN_TYPE(UInt16)
   CDOT_BUILTIN_TYPE(Int32)
   CDOT_BUILTIN_TYPE(UInt32)
   CDOT_BUILTIN_TYPE(Int64)
   CDOT_BUILTIN_TYPE(UInt64)
   CDOT_BUILTIN_TYPE(Int128)
   CDOT_BUILTIN_TYPE(UInt128)
   CDOT_BUILTIN_TYPE(Float32)
   CDOT_BUILTIN_TYPE(Float64)
#undef CDOT_BUILTIN_TYPE
   else if (II == BuiltinIdents[builtin::undefValue])
   {
      if (TemplateArgType->isDependentType()) {
         Expr->setIsTypeDependent(true);
         Expr->setExprType(Context.getUIntTy());

         return Expr;
      }

      il::Constant* Val = ILGen->Builder.GetUndefValue(TemplateArgType);
      ResultExpr = StaticExpr::Create(Context, TemplateArgType,
                                      Expr->getSourceRange(), Val);
   }

   if (!ResultExpr) {
      diagnose(Expr, err_compiler_ns_unknown_entity, Alias->getDeclName(),
               Alias->getSourceRange());

      return ExprError();
   }

   return visitExpr(ResultExpr);
}

void SemaPass::SetBuiltinAliasType(AliasDecl* A)
{
   if (!BuiltinIdentsInitialized)
      initBuiltinIdents();

   auto* II = A->getDeclName().getIdentifierInfo();
   if (II == BuiltinIdents[builtin::TokenType]) {
      A->setType(SourceType(Context.getMetaType(Context.getTokenType())));
   }
   else if (II == BuiltinIdents[builtin::CVoid]) {
      A->setType(SourceType(Context.getMetaType(Context.getVoidType())));
   }
   else if ((II == BuiltinIdents[builtin::RawPointer]
   || II == BuiltinIdents[builtin::MutableRawPointer])
   && !A->isInstantiation() && !A->isTemplate()) {
      QualType PtrTy;
      if (II == BuiltinIdents[builtin::MutableRawPointer]) {
         PtrTy = Context.getMutablePointerType(Context.getVoidType());
      }
      else {
         PtrTy = Context.getPointerType(Context.getVoidType());
      }

      A->setType(SourceType(Context.getMetaType(PtrTy)));
   }
   else if ((II == BuiltinIdents[builtin::RawPointer]
   || II == BuiltinIdents[builtin::MutableRawPointer])
   && A->isTemplate()) {
      QualType Ty = Context.getTemplateArgType(A->getTemplateParams().front());
      QualType PtrTy;
      if (II == BuiltinIdents[builtin::MutableRawPointer]) {
         PtrTy = Context.getMutablePointerType(Ty);
      }
      else {
         PtrTy = Context.getPointerType(Ty);
      }

      A->setType(SourceType(Context.getMetaType(PtrTy)));
   }
   else if (II == BuiltinIdents[builtin::RawPointer]
   || II == BuiltinIdents[builtin::MutableRawPointer]) {
      if (!A->isInstantiation() || A->getTemplateArgs().size() != 1
          || !A->getTemplateArgs().front().isType()) {
         diagnose(err_compiler_ns_bad_def, A->getDeclName(),
                  A->getSourceRange());

         return;
      }

      QualType Ty = A->getTemplateArgs().front().getType();
      QualType PtrTy;
      if (II == BuiltinIdents[builtin::MutableRawPointer]) {
         PtrTy = Context.getMutablePointerType(Ty);
      }
      else {
         PtrTy = Context.getPointerType(Ty);
      }

      A->setType(SourceType(Context.getMetaType(PtrTy)));
   }
#define CDOT_BUILTIN_TYPE(NAME)                                                \
   else if (II == BuiltinIdents[builtin::NAME])                                \
   {                                                                           \
      A->setType(SourceType(Context.getMetaType(Context.get##NAME##Ty())));    \
   }
   CDOT_BUILTIN_TYPE(Int1)
   CDOT_BUILTIN_TYPE(Int8)
   CDOT_BUILTIN_TYPE(UInt8)
   CDOT_BUILTIN_TYPE(Int16)
   CDOT_BUILTIN_TYPE(UInt16)
   CDOT_BUILTIN_TYPE(Int32)
   CDOT_BUILTIN_TYPE(UInt32)
   CDOT_BUILTIN_TYPE(Int64)
   CDOT_BUILTIN_TYPE(UInt64)
   CDOT_BUILTIN_TYPE(Int128)
   CDOT_BUILTIN_TYPE(UInt128)
   CDOT_BUILTIN_TYPE(Float32)
   CDOT_BUILTIN_TYPE(Float64)
#undef CDOT_BUILTIN_TYPE
}

} // namespace ast
} // namespace cdot