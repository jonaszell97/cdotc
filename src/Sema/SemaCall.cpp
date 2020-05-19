#include "cdotc/Sema/SemaPass.h"

#include "cdotc/Basic/NestedNameSpecifier.h"
#include "cdotc/ILGen/ILGenPass.h"
#include "cdotc/Diagnostics/Diagnostics.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/Builtin.h"
#include "cdotc/Sema/OverloadResolver.h"
#include "cdotc/Sema/TemplateInstantiator.h"
#include "cdotc/Serialization/ModuleFile.h"
#include "cdotc/Support/StringSwitch.h"

#include <llvm/ADT/SmallString.h>

using namespace cdot::diag;
using namespace cdot::sema;
using namespace cdot::support;

namespace cdot {
namespace ast {

NamedDecl* SemaPass::getTypeDecl(QualType Ty)
{
   CanType T = Ty->getDesugaredType()->getCanonicalType();

   if (auto Meta = T->asMetaType()) {
      T = Meta->getUnderlyingType();
   }

   if (auto* RecTy = T->asRecordType()) {
      return RecTy->getRecord();
   }
   if (auto* Alias = T->asTypedefType()) {
      return Alias->getTypedef();
   }

   return nullptr;
}

MethodDecl* SemaPass::getEquivalentMethod(MethodDecl* Orig, RecordDecl* Inst)
{
   if (Orig->isMemberwiseInitializer())
      return cast<StructDecl>(Inst)->getMemberwiseInitializer();

   if (isa<DeinitDecl>(Orig))
      return Inst->getDeinitializer();

   DeclarationName Name = Orig->getDeclName();
   switch (Name.getKind()) {
   case DeclarationName::ConstructorName:
   case DeclarationName::BaseConstructorName:
      Name = Context.getDeclNameTable().getConstructorName(
          Context.getRecordType(Inst),
          Name.getKind() == DeclarationName::ConstructorName);

      break;
   case DeclarationName::DestructorName:
      Name = Context.getDeclNameTable().getDestructorName(
          Context.getRecordType(Inst));
      break;
   default:
      break;
   }

   const MultiLevelLookupResult* LookupRes;
   if (QC.MultiLevelLookup(LookupRes, Inst, Name)) {
      return Orig;
   }

   for (auto* D : LookupRes->allDecls()) {
      auto* M = dyn_cast<MethodDecl>(D);
      if (!M)
         continue;

      if (M->getMethodID() == Orig->getMethodID())
         return M;
   }

   llvm_unreachable("no equivalent method!");
}

LLVM_ATTRIBUTE_UNUSED
static bool wouldSelfBeValid(SemaPass& SP, bool& Static)
{
   Static = false;

   auto* Ctx = &SP.getDeclContext();
   switch (Ctx->getDeclKind()) {
   case Decl::MethodDeclID:
      Static = cast<MethodDecl>(Ctx)->isStatic();
      LLVM_FALLTHROUGH;
   case Decl::InitDeclID:
   case Decl::DeinitDeclID:
      return true;
   default:
      return false;
   }
}

Expression* SemaPass::convertCStyleVarargParam(Expression* Expr)
{
   Expr = castToRValue(Expr);
   QualType type = Expr->getExprType();

   // (unsigned) char / short are promoted to int for c-style vararg functions.
   if (type->isIntegerType() && type->getBitwidth() < 32) {
      QualType promotedType
          = type->isUnsigned() ? Context.getUInt32Ty() : Context.getInt32Ty();

      return forceCast(Expr, promotedType);
   }

   // float is promoted to double for c-style vararg functions.
   if (type->isFloatTy()) {
      return forceCast(Expr, Context.getDoubleTy());
   }

   return Expr;
}

ExprResult SemaPass::visitCallExpr(CallExpr* Call, TemplateArgListExpr* ArgExpr)
{
   CallableDecl* C = Call->getFunc();
   C = maybeInstantiateMemberFunction(C, Call);

   if (QC.PrepareDeclInterface(C)) {
      Call->setIsInvalid(true);
      return Call;
   }

   if (C->throws()) {
      if (TryScopeStack.empty()) {
         diagnose(err_throwing_call_not_in_try, C->getDeclName(),
                  Call->getSourceRange());
      }
      else {
         TryScopeStack.back() = true;
      }
   }

   bool importedFromClang = C->isImportedFromClang();
   if (!Call->getExprType()) {
      QualType ExprType;
      if (auto* F = dyn_cast<FunctionDecl>(C)) {
         ExprType = F->getReturnType();
      }
      else if (auto* Case = dyn_cast<EnumCaseDecl>(C)) {
         ExprType = Context.getRecordType(Case->getRecord());
      }
      else {
         auto M = cast<MethodDecl>(C);
         if (M->isCompleteInitializer()) {
            ExprType = Context.getRecordType(M->getRecord());
         }
         else if (M->isStatic()) {
            ExprType = M->getReturnType();
         }
         else {
            ExprType = M->getReturnType();
         }
      }

      if (C->isTemplate()) {
         if (QC.SubstTemplateParamTypes(ExprType, ExprType,
                                        *Call->getTemplateArgs(),
                                        Call->getSourceRange())) {
            Call->setIsInvalid(true);
         }
      }

      if (importedFromClang) {
         ExprType = TransformImportedCType(ExprType);
      }

      Call->setExprType(ExprType);
   }

   unsigned i = 0;
   MutableArrayRef<FuncArgDecl*> params = C->getArgs();
   bool isTemplate = C->isTemplateOrInTemplate();

   for (Expression*& argVal : Call->getArgs()) {
      QualType neededType;
      bool cstyleVararg = false;
      bool isVariadic = false;

      if (i < params.size() || !C->isCstyleVararg()) {
         auto *param = i < params.size() ? params[i] : params.back();
         neededType = param->getType();
         isVariadic = param->isVariadic();

         if (param->hasAttribute<AutoClosureAttr>()) {
            neededType = Context.getLambdaType(neededType, {}, {});
         }
      }
      else {
         cstyleVararg = true;
      }

      if (isTemplate && Call->getTemplateArgs() && !isVariadic) {
         if (QC.SubstTemplateParamTypes(neededType, neededType,
                                        *Call->getTemplateArgs(),
                                         Call->getSourceRange())) {
            ++i;
            Call->setIsInvalid(true);

            continue;
         }
      }

      auto typecheckResult = typecheckExpr(argVal, neededType, Call);
      if (!typecheckResult) {
         Call->setIsInvalid(true);
         ++i;

         continue;
      }

      if (cstyleVararg) {
         argVal = convertCStyleVarargParam(argVal);
      }
      else {
         argVal = implicitCastIfNecessary(argVal, neededType);
      }

      ++i;
   }

   if (isInBuiltinModule(C)) {
      HandleBuiltinCall(Call);
   }

   return Call;
}

LLVM_ATTRIBUTE_UNUSED
static void
addTemplateArgs(SmallVectorImpl<Expression*>& Exprs,
                SmallVectorImpl<std::unique_ptr<BuiltinExpr>>& PlaceholderExprs,
                const sema::TemplateArgument& Arg)
{
   if (Arg.isVariadic()) {
      for (auto& VA : Arg.getVariadicArgs()) {
         addTemplateArgs(Exprs, PlaceholderExprs, VA);
      }

      return;
   }

   if (Arg.isValue()) {
      Exprs.push_back(Arg.getValueExpr());
      return;
   }

   PlaceholderExprs.emplace_back(
       std::make_unique<BuiltinExpr>(BuiltinExpr::CreateTemp(Arg.getType())));
   Exprs.push_back(PlaceholderExprs.back().get());
}

CallExpr* SemaPass::CreateCall(CallableDecl* C, ArrayRef<Expression*> Args,
                               SourceLocation Loc)
{
   ASTVector<Expression*> Vec(Context, Args);
   return CreateCall(C, move(Vec), Loc);
}

CallExpr* SemaPass::CreateCall(CallableDecl* C, ASTVector<Expression*>&& Args,
                               SourceLocation Loc)
{
   CallKind K;
   if (auto F = dyn_cast<FunctionDecl>(C)) {
      K = CallKind::NamedFunctionCall;
   }
   else if (isa<EnumCaseDecl>(C)) {
      K = CallKind::NamedFunctionCall;
   }
   else {
      auto M = cast<MethodDecl>(C);
      if (M->isCompleteInitializer()) {
         K = CallKind::InitializerCall;
      }
      else if (M->isStatic()) {
         K = CallKind::StaticMethodCall;
      }
      else {
         K = CallKind::MethodCall;
      }
   }

   auto* callExpr = CallExpr::Create(Context, Loc, SourceRange(Loc), move(Args),
                                     C, K, QualType());

   callExpr->setIsDotInit(C->isBaseInitializer());
   return callExpr;
}

ExprResult SemaPass::visitAnonymousCallExpr(AnonymousCallExpr* Call)
{
   if (!Call->getFunctionType()) {
      return ExprError();
   }

   if (Call->getFunctionType()->throws()) {
      if (TryScopeStack.empty()) {
         diagnose(err_throwing_call_not_in_try, DeclarationName(),
                  Call->getSourceRange());
      }
      else {
         TryScopeStack.back() = true;
      }
   }

   return Call;
}

} // namespace ast
} // namespace cdot