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
   auto S = Stmt->getStatement();
   for (auto &A : Stmt->getAttributes()) {
      switch (A->getKind()) {
#     define CDOT_STMT_ATTR(Name, Spelling)                                    \
      case AttrKind::Name: check##Name##Attr(S, cast<Name##Attr>(A)); break;
#     include "AST/Attributes.def"

      default:
         llvm_unreachable("bad expr attr");
      }
   }

   auto Res = visitStmt(Stmt, S);
   if (!Res)
      return StmtError();

   return Stmt;
}

ExprResult SemaPass::visitAttributedExpr(AttributedExpr *Expr)
{
   auto E = Expr->getExpr();
   for (auto &A : Expr->getAttributes()) {
      switch (A->getKind()) {
#     define CDOT_EXPR_ATTR(Name, Spelling)                                    \
      case AttrKind::Name: check##Name##Attr(E, cast<Name##Attr>(A)); break;
#     define CDOT_TYPE_ATTR(Name, Spelling)                                    \
      case AttrKind::Name: check##Name##Attr(E, cast<Name##Attr>(A)); break;
#     include "AST/Attributes.def"

      default:
         llvm_unreachable("bad expr attr");
      }
   }

   auto Res = visitExpr(Expr, E);
   if (!Res)
      return ExprError();

   Expr->setExprType(E->getExprType());
   return Expr;
}

void SemaPass::checkExternAttr(Decl *D, ExternAttr *A)
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

void SemaPass::checkThinAttr(Expression *E, ThinAttr *A)
{
   if (auto Fn = dyn_cast_or_null<FunctionTypeExpr>(E)) {
      Fn->setThin(true);
      return;
   }

   diagnose(E, err_attr_can_only_be_used, "thin", /*function types*/3,
            E->getSourceRange());
}

#define CDOT_ATTR_SEMA
#include "AST/Attr.inc"

} // namespace ast
} // namespace cdot