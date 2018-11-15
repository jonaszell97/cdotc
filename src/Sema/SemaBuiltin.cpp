//
// Created by Jonas Zell on 29.04.18.
//

#include "SemaPass.h"

#include "Basic/Builtins.h"
#include "IL/Constants.h"
#include "IL/ILBuilder.h"
#include "ILGen/ILGenPass.h"
#include "Module/Module.h"
#include "Query/QueryContext.h"

using namespace cdot::builtin;
using namespace cdot::diag;

namespace cdot {
namespace ast {

void SemaPass::initBuiltinIdents()
{
   static_assert(sizeof(BuiltinIdents) / sizeof(IdentifierInfo*)
                    > (unsigned)_lastBuiltin,
                 "not enough space for Builtin identifiers!");

   auto &Idents = Context.getIdentifiers();

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

   BuiltinIdents[builtin::TokenType] = &Idents.get("TokenType");
   BuiltinIdents[builtin::CVoid] = &Idents.get("CVoid");
   BuiltinIdents[builtin::RawPointer] = &Idents.get("RawPointer");
   BuiltinIdents[builtin::Int8] = &Idents.get("Int8");
   BuiltinIdents[builtin::UInt8] = &Idents.get("UInt8");
   BuiltinIdents[builtin::Int16] = &Idents.get("Int16");
   BuiltinIdents[builtin::UInt16] = &Idents.get("UInt16");
   BuiltinIdents[builtin::Int32] = &Idents.get("Int32");
   BuiltinIdents[builtin::UInt32] = &Idents.get("UInt32");
   BuiltinIdents[builtin::Int64] = &Idents.get("Int64");
   BuiltinIdents[builtin::UInt64] = &Idents.get("UInt64");
   BuiltinIdents[builtin::Int128] = &Idents.get("Int128");
   BuiltinIdents[builtin::UInt128] = &Idents.get("UInt128");

   BuiltinIdents[builtin::undefValue] = &Idents.get("undefValue");

   BuiltinIdentsInitialized = true;
}

ExprResult SemaPass::HandleBuiltinCall(CallExpr *C)
{
   if (!BuiltinIdentsInitialized)
      initBuiltinIdents();

   const IdentifierInfo *II;
   if (C->getFunc()->getDeclName().isSimpleIdentifier()) {
      II = C->getFunc()->getDeclName().getIdentifierInfo();
   }
   else {
      assert(C->getFunc()->getDeclName().getKind()
             == DeclarationName::InstantiationName);

      II = C->getFunc()->getDeclName().getInstantiationName()
            .getIdentifierInfo();
   }

   unsigned i = 0;
   for (auto &Ident : BuiltinIdents) {
      if (Ident == II) {
         C->setBuiltinKind(i);
         break;
      }

      i++;
   }

   switch ((builtin::Builtin)C->getBuiltinKind()) {
   case addressOf: {
      auto Ty = C->getArgs().front()->ignoreParensAndImplicitCasts()
         ->getExprType();

      if (!Ty->isReferenceType()) {
         diagnose(C, diag::err_type_mismatch,
                  C->getArgs().front()->getSourceLoc(),
                  Ty, Context.getReferenceType(Ty));
      }

      break;
   }
   case llvm_intrinsic:
      assert(support::isa<StringLiteral>(C->getArgs().front()));
      break;
   case NoBuiltin:
      C->setKind(CallKind::NamedFunctionCall);
      return C;
   default:
      break;
   }

   C->setKind(CallKind::Builtin);
   return C;
}

ExprResult SemaPass::HandleBuiltinAlias(AliasDecl *Alias, Expression *Expr)
{
   if (!BuiltinIdentsInitialized)
      initBuiltinIdents();

   if (auto AE = Alias->getAliasExpr()) {
      Expr->setExprType(AE->getExprType());
      return Expr;
   }

   auto It = ReflectionValues.find(Alias);
   if (It != ReflectionValues.end())
      return It->getSecond();

   auto *BuiltinMod = getBuiltinModule();
   if (!BuiltinMod) {
      diagnose(Expr, err_compiler_ns_unknown_entity, Alias->getDeclName(),
               Alias->getSourceRange());

      return ExprError();
   }

//   if (ensureDeclared(BuiltinMod)) {
//      return ExprError();
//   }

   auto Name = Alias->getDeclName().getManglingName();
   if (!Name.isSimpleIdentifier()) {
      diagnose(Expr, err_compiler_ns_unknown_entity, Alias->getDeclName(),
               Alias->getSourceRange());

      return ExprError();
   }

   auto *II = Name.getIdentifierInfo();

   // look the declaration up to make sure it's deserialized.
   const MultiLevelLookupResult *LookupRes;
   if (QC.DirectLookup(LookupRes, BuiltinMod->getDecl(), II)) {
      return ExprError();
   }

   bool DoCache = true;
   Expression *ResultExpr = nullptr;

   if (II == BuiltinIdents[builtin::TokenType]) {
      ResultExpr = new(Context) IdentifierRefExpr(
         Expr->getSourceRange(), IdentifierKind::MetaType,
         Context.getMetaType(Context.getTokenType()));
   }
   else if (II == BuiltinIdents[builtin::CVoid]) {
      ResultExpr = new(Context) IdentifierRefExpr(
         Expr->getSourceRange(), IdentifierKind::MetaType,
         Context.getMetaType(Context.getVoidType()));
   }
   else if (II == BuiltinIdents[builtin::RawPointer] && !Alias->isInstantiation()) {
      ResultExpr = new(Context) IdentifierRefExpr(
         Expr->getSourceRange(), IdentifierKind::MetaType,
         Context.getMetaType(Context.getVoidType()->getPointerTo(Context)));
   }
   else if (II == BuiltinIdents[builtin::RawPointer]) {
      if (!Alias->isInstantiation() || Alias->getTemplateArgs().size() != 1
          || !Alias->getTemplateArgs().front().isType()) {
         diagnose(Expr, err_compiler_ns_bad_def, Alias->getDeclName(),
                  Alias->getSourceRange());

         return ExprError();
      }

      QualType Ty = Alias->getTemplateArgs().front().getType();
      if (Ty->isDependentType()) {
         Expr->setIsTypeDependent(true);
         Expr->setExprType(Context.getUIntTy());

         return Expr;
      }

      ResultExpr = new(Context) IdentifierRefExpr(
         Expr->getSourceRange(), IdentifierKind::MetaType,
         Context.getMetaType(Ty->getPointerTo(Context)));
   }
#  define CDOT_BUILTIN_TYPE(NAME)                                              \
   else if (II == BuiltinIdents[builtin::NAME]) {                              \
      ResultExpr = new(Context) IdentifierRefExpr(                             \
         Expr->getSourceRange(), IdentifierKind::MetaType,                     \
         Context.getMetaType(Context.get##NAME##Ty()));                        \
   }
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
#  undef CDOT_BUILTIN_TYPE
   else if (II == BuiltinIdents[builtin::undefValue]) {
      if (!Alias->isInstantiation() || Alias->getTemplateArgs().size() != 1
          || !Alias->getTemplateArgs().front().isType()) {
         diagnose(Expr, err_compiler_ns_bad_def, Alias->getDeclName(),
                  Alias->getSourceRange());

         return ExprError();
      }

      QualType Ty = Alias->getTemplateArgs().front().getType();
      if (Ty->isDependentType()) {
         Expr->setIsTypeDependent(true);
         Expr->setExprType(Context.getUIntTy());

         return Expr;
      }

      il::Constant *Val = ILGen->Builder.GetUndefValue(Ty);
      ResultExpr = StaticExpr::Create(Context, Ty, Expr->getSourceRange(), Val);
   }

   if (!ResultExpr) {
      diagnose(Expr, err_compiler_ns_unknown_entity, Alias->getDeclName(),
               Alias->getSourceRange());

      return ExprError();
   }

   auto Res = visitExpr(ResultExpr);
   if (DoCache)
      ReflectionValues.try_emplace(Alias, Res.get());

   return Res;
}

} // namespace ast
} // namespace cdot