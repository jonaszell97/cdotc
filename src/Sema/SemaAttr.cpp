#include "cdotc/Sema/SemaPass.h"

#include "cdotc/IL/Constants.h"
#include "cdotc/Query/QueryContext.h"

using namespace cdot::diag;
using namespace cdot::support;

namespace cdot {
namespace ast {

void SemaPass::checkDeclAttrs(Decl* D, Attr::VisitationPoint VP)
{
   for (auto& A : D->getAttributes()) {
      if (VP != A->getVisitationPoint())
         continue;

      switch (A->getKind()) {
#define CDOT_DECL_ATTR(Name, Spelling)                                         \
   case AttrKind::Name:                                                        \
      check##Name##Attr(D, cast<Name##Attr>(A));                               \
      break;
#include "cdotc/AST/Attributes.def"

      default:
         break;
      }
   }

   D->setCheckedAttrs(true);
}

StmtResult SemaPass::visitAttributedStmt(AttributedStmt* Stmt)
{
   auto S = Stmt->getStatement();
   for (auto& A : Stmt->getAttributes()) {
      switch (A->getKind()) {
#define CDOT_STMT_ATTR(Name, Spelling)                                         \
   case AttrKind::Name:                                                        \
      check##Name##Attr(S, cast<Name##Attr>(A));                               \
      break;
#include "cdotc/AST/Attributes.def"

      default:
         break;
      }
   }

   auto Res = visitStmt(Stmt, S);
   if (!Res)
      return StmtError();

   Stmt->setStatement(Res.get());
   return Stmt;
}

ExprResult SemaPass::visitAttributedExpr(AttributedExpr* Expr)
{
   auto E = Expr->getExpr();
   for (auto& A : Expr->getAttributes()) {
      switch (A->getKind()) {
#define CDOT_EXPR_ATTR(Name, Spelling)                                         \
   case AttrKind::Name:                                                        \
      check##Name##Attr(E, cast<Name##Attr>(A));                               \
      break;
#define CDOT_TYPE_ATTR(Name, Spelling)                                         \
   case AttrKind::Name:                                                        \
      check##Name##Attr(E, cast<Name##Attr>(A));                               \
      break;
#include "cdotc/AST/Attributes.def"

      default:
         break;
      }
   }

   if (E->isInvalid()) {
      E->setExprType(ErrorTy);
      Expr->setIsInvalid(true);

      return ExprError();
   }

   auto Res = visitExpr(Expr, E);
   if (!Res || Res.get()->isInvalid())
      return ExprError();

   Expr->setExpr(Res.get());
   Expr->setExprType(Res.get()->getExprType());

   return Expr;
}

void SemaPass::checkExternAttr(Decl* D, ExternAttr* A)
{
   switch (A->getLang()) {
   case ExternAttr::C:
      D->setExternC(true);
      break;
   case ExternAttr::CXX:
      D->setExternCXX(true);
      break;
   }
}

void SemaPass::checkInlineAttr(Decl* D, InlineAttr* A) {}

void SemaPass::checkAlignAttr(Decl* D, AlignAttr* A)
{
   static constexpr auto MaxAlign = 268'435'456llu;

   auto ValueResult = evalStaticExpr(D, A->getAlignment());
   if (!ValueResult)
      return;

   auto Val = ValueResult.getValue();
   if (!isa<il::ConstantInt>(Val)) {
      return diagnose(A->getAlignment(), err_attr_align_bad_arg,
                      /*must be integral*/ 0);
   }

   auto& APS = cast<il::ConstantInt>(Val)->getValue();

   if (APS > MaxAlign) {
      return diagnose(A->getAlignment(), err_attr_align_too_high);
   }

   if (APS <= 0) {
      return diagnose(A->getAlignment(), err_attr_align_bad_arg,
                      /*<= 0>*/ 2);
   }

   if (APS != 1 && APS.getZExtValue() % 2 != 0) {
      return diagnose(A->getAlignment(), err_attr_align_bad_arg,
                      /*must be power of two*/ 1);
   }

   unsigned short NaturalAlignment;
   if (auto VD = cast<VarDecl>(D)) {
      NaturalAlignment = Context.getTargetInfo().getAlignOfType(VD->getType());
   }
   else {
      QC.GetTypeAlignment(NaturalAlignment, VD->getType());
   }

   if (APS < NaturalAlignment) {
      return diagnose(A->getAlignment(), err_attr_align_lower_than_natural,
                      NaturalAlignment, APS);
   }
}

void SemaPass::checkImplicitAttr(Decl* D, ImplicitAttr*)
{
   auto C = cast<CallableDecl>(D);
   if (!isa<InitDecl>(C) && !C->isConversionOp()) {
      return diagnose(C, err_implicit_attr_not_valid, C->getSourceLoc());
   }
}

void SemaPass::checkThinAttr(Expression* E, ThinAttr* A)
{
   if (auto Fn = dyn_cast_or_null<FunctionTypeExpr>(E)) {
      Fn->setThin(true);
      return;
   }

   diagnose(E, err_attr_can_only_be_used, "thin", /*function types*/ 3,
            E->getSourceRange());
}

void SemaPass::check_BuiltinAttr(Decl* D, _BuiltinAttr* A)
{
   auto ND = cast<NamedDecl>(D);
   if (A->getBuiltinName().empty()) {
      BuiltinDecls[ND->getDeclName()] = ND;
   }
   else {
      BuiltinDecls[Context.getIdentifiers().get(A->getBuiltinName())] = ND;
   }
}

void SemaPass::check_SemanticsAttr(Decl* D, _SemanticsAttr* A)
{
   StringRef K = A->getSemanticsKind();
   if (K == "builtin_integer_type") {
      const RecordMetaInfo* Info;
      if (QC.GetRecordMeta(Info, cast<RecordDecl>(D), false)) {
         return;
      }

      Info->IsBuiltinIntegerType = true;
   }
   else if (K == "builtin_bool_type") {
      const RecordMetaInfo* Info;
      if (QC.GetRecordMeta(Info, cast<RecordDecl>(D), false)) {
         return;
      }

      Info->IsBuiltinBoolType = true;
   }
   else if (K == "builtin_fp_type") {
      const RecordMetaInfo* Info;
      if (QC.GetRecordMeta(Info, cast<RecordDecl>(D), false)) {
         return;
      }

      Info->IsBuiltinFloatingPointType = true;
   }
}

void SemaPass::checkStrongAttr(Decl* D, StrongAttr*)
{
   cast<AliasDecl>(D)->setStrong(true);
}

void SemaPass::checkVersionStmtAttr(Statement*, VersionStmtAttr*) {}

void SemaPass::checkVersionDeclAttr(Decl*, VersionDeclAttr*) {}

void SemaPass::checkInstantiateAttr(Decl *D, InstantiateAttr *A)
{
}

void SemaPass::check_DebugAttr(Decl *D, _DebugAttr *A) {}

#define CDOT_ATTR_SEMA
#include "cdotc/AST/Attr.inc"

} // namespace ast
} // namespace cdot