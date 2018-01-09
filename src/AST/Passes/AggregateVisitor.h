//
// Created by Jonas Zell on 24.12.17.
//

#ifndef CDOT_AGGREGATEVISITOR_H
#define CDOT_AGGREGATEVISITOR_H

#include <memory>
#include <llvm/ADT/ArrayRef.h>

#include "ASTIncludes.h"

#include "../../Message/Diagnostics.h"

using namespace cdot::support;
using namespace cdot::diag;

namespace cdot {
namespace ast {

template<class TargetNode, typename Predicate>
class AggregateVisitor {
public:
   AggregateVisitor(std::shared_ptr<Expression> const& expr,
                    Predicate const& pred)
      : pred(pred)
   {
      visit(expr.get());
   }

   llvm::ArrayRef<TargetNode*> getFoundNodes()
   {
      return FoundNodes;
   }

   void visit(Expression *expr)
   {
      if (auto target = dyn_cast<TargetNode>(expr)) {
         if (pred(target))
            FoundNodes.push_back(target);

         return;
      }

      switch (expr->getTypeID()) {
#     define CDOT_EXPR(Name)     \
         case AstNode::Name##ID: \
            return visit##Name(static_cast<Name*>(expr));
#     include "../AstNode.def"

         default:
            llvm_unreachable("not an expression");
      }
   }

   void visitIdentifierRefExpr(IdentifierRefExpr *node) {}
   void visitNonTypeTemplateArgExpr(NonTypeTemplateArgExpr *node) {}
   void visitSubscriptExpr(SubscriptExpr *node) {}

   void visitCallExpr(CallExpr *node)
   {
      for (const auto &arg : node->getArgs())
         visit(arg.get());
   }

   void visitMemberRefExpr(MemberRefExpr *node) {}

   void visitEnumCaseExpr(EnumCaseExpr *node)
   {
      for (const auto &arg : node->getArgs())
         visit(arg.get());
   }

   void visitIntegerLiteral(IntegerLiteral *node) {}
   void visitFPLiteral(FPLiteral *node) {}
   void visitBoolLiteral(BoolLiteral *node) {}
   void visitCharLiteral(CharLiteral *node) {}

   void visitDictionaryLiteral(DictionaryLiteral *node)
   {
      for (const auto &str : node->getKeys())
         visit(str.get());

      for (const auto &str : node->getValues())
         visit(str.get());
   }

   void visitArrayLiteral(ArrayLiteral *node)
   {
      for (const auto &str : node->getValues())
         visit(str.get());
   }

   void visitNoneLiteral(NoneLiteral *node) {}
   void visitStringLiteral(StringLiteral *node) {}

   void visitStringInterpolation(StringInterpolation *node)
   {
      for (const auto &str : node->getStrings())
         visit(str.get());
   }

   void visitTupleLiteral(TupleLiteral *node)
   {
      for (const auto &str : node->getElements())
         visit(str.second.get());
   }

   void visitExprSequence(ExprSequence *node)
   {
      for (const auto &F : node->getFragments()) {
         switch (F.getKind()) {
            case ExprSequence::SequenceElement::EF_Expression:
               visit(F.getExpr().get());
               break;
            case ExprSequence::SequenceElement::EF_PossibleOperator: {
               IdentifierRefExpr refExpr(string(F.getOp()));
               visit(&refExpr);
               break;
            }
            default:
               break;
         }
      }
   }

   void visitBinaryOperator(BinaryOperator *node)
   {
      visit(node->getLhs().get());
      visit(node->getRhs().get());
   }

   void visitTertiaryOperator(TertiaryOperator *node)
   {
      visit(node->getCondition().get());
      visit(node->getLhs().get());
      visit(node->getRhs().get());
   }

   void visitUnaryOperator(UnaryOperator *node)
   {
      visit(node->getTarget().get());
   }

   void visitLambdaExpr(LambdaExpr *node) {}

   void visitExpressionPattern(ExpressionPattern *node) {}
   void visitCasePattern(CasePattern *node) {}
   void visitIsPattern(IsPattern *node) {}

   void visitImplicitCastExpr(ImplicitCastExpr *node)
   {
      visit(node->getTarget().get());
   }

   void visitConstraintExpr(ConstraintExpr *node) {}

   void visitLvalueToRvalue(LvalueToRvalue *node)
   {
      visit(node->getTarget().get());
   }

   void visitTypeRef(TypeRef *node)
   {

   }

   void visitStaticExpr(StaticExpr *node) {}

   void visitTraitsExpr(TraitsExpr *node) {}

private:
   llvm::SmallVector<TargetNode*, 8> FoundNodes;
   Predicate const& pred;
};

template<class TargetNode, typename Predicate>
AggregateVisitor<TargetNode, Predicate>
makeAggrVisitor(std::shared_ptr<Expression> const& expr,
                Predicate const& pred) {
   return { expr, pred };
}

} // namespace ast
} // namespace cdot

#endif //CDOT_AGGREGATEVISITOR_H
