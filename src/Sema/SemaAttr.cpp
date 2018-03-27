//
// Created by Jonas Zell on 17.03.18.
//

#include "SemaPass.h"

using namespace cdot::diag;
using namespace cdot::support;

namespace cdot {
namespace ast {

void SemaPass::checkDeclAttrs(Decl *D, Attr::VisitationPoint VP)
{
   for (auto &A : D->getAttributes()) {
      if (VP < A->getVisitationPoint())
         continue;

      switch (A->getKind()) {
#     define CDOT_DECL_ATTR(Name, Spelling)                                   \
      case AttrKind::Name: check##Name##Attr(D, cast<Name##Attr>(A)); break;
#     include "AST/Attributes.def"

      default:
         llvm_unreachable("bad expr attr");
      }
   }

   D->setCheckedAttrs(true);
}

StmtResult SemaPass::visitAttributedStmt(AttributedStmt *Stmt)
{
   for (auto &A : Stmt->getAttributes()) {
      switch (A->getKind()) {
#     define CDOT_STMT_ATTR(Name, Spelling)                                    \
      case AttrKind::Name: check##Name##Attr(Stmt, cast<Name##Attr>(A)); break;
#     include "AST/Attributes.def"

      default:
         llvm_unreachable("bad expr attr");
      }
   }

   auto Res = visitStmt(Stmt, Stmt->getStatement());
   if (!Res)
      return StmtError();

   return Stmt;
}

ExprResult SemaPass::visitAttributedExpr(AttributedExpr *Expr)
{
   for (auto &A : Expr->getAttributes()) {
      switch (A->getKind()) {
#     define CDOT_EXPR_ATTR(Name, Spelling)                                    \
      case AttrKind::Name: check##Name##Attr(Expr, cast<Name##Attr>(A)); break;
#     include "AST/Attributes.def"

      default:
         llvm_unreachable("bad expr attr");
      }
   }

   auto Res = visitExpr(Expr, Expr->getExpr());
   if (!Res)
      return ExprError();

   Expr->setExprType(Expr->getExpr()->getExprType());
   return Expr;
}

void SemaPass::checkExternAttr(Decl *D, ExternAttr *A)
{
   if (A->getLang() == ExternAttr::C)
      cast<CallableDecl>(D)->setExternC(true);
}

void SemaPass::checkInlineAttr(Decl *D, InlineAttr *A)
{

}

void SemaPass::checkAlignAttr(Decl *D, AlignAttr *A)
{
   static constexpr auto MaxAlign = 268'435'456llu;

   auto ValueResult = evalStaticExpr(D, A->getAlignment());
   if (!ValueResult)
      return;

   auto &Val = ValueResult.getValue();
   if (!Val.isInt()) {
      return diagnose(A->getAlignment(), err_attr_align_bad_arg,
                      /*must be integral*/ 0);
   }

   if (Val.getAPSInt() > MaxAlign) {
      return diagnose(A->getAlignment(), err_attr_align_too_high);
   }

   if (Val.getAPSInt() <= 0) {
      return diagnose(A->getAlignment(), err_attr_align_bad_arg,
                      /*<= 0>*/ 2);
   }

   if (Val.getAPSInt() != 1 && Val.getAPSInt().getZExtValue() % 2 != 0) {
      return diagnose(A->getAlignment(), err_attr_align_bad_arg,
                      /*must be power of two*/ 1);
   }

   unsigned short NaturalAlignment;
   if (auto VD = cast<VarDecl>(D)) {
      NaturalAlignment = Context.getTargetInfo()
                                .getAlignOfType(VD->getType());
   }
   else {
      NaturalAlignment = cast<RecordDecl>(D)->getAlignment();
   }

   if (Val.getAPSInt() < NaturalAlignment) {
      return diagnose(A->getAlignment(), err_attr_align_lower_than_natural,
                      NaturalAlignment, Val.getAPSInt());
   }
}

void SemaPass::checkImplicitAttr(Decl *D, ImplicitAttr*)
{
   auto C = cast<CallableDecl>(D);
   if (!isa<InitDecl>(C) && !C->isOperator()) {
      return diagnose(D, err_implicit_attr_not_valid);
   }

   C->getOperator().setImplicit(true);
}

void SemaPass::checkOpaqueAttr(Decl *D, OpaqueAttr *A)
{

}

void SemaPass::checkThinAttr(SourceType Ty, ThinAttr *A)
{

}

void SemaPass::checkDiscardableResultAttr(Decl *D,
                                          DiscardableResultAttr *A) {

}

} // namespace ast
} // namespace cdot