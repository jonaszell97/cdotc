//
// Created by Jonas Zell on 07.02.18.
//

#ifndef CDOT_NULLASTVISITOR_H
#define CDOT_NULLASTVISITOR_H

#include "AST/Expression.h"
#include "AST/Statement.h"
#include "AST/Decl.h"

namespace cdot {
namespace ast {

template<class SubClass,
         class ExprRetTy = void,
         class StmtRetTy = ExprRetTy,
         class DeclRetTy = void,
         class ...ParamTys>
class ASTVisitor {
public:
#  define CDOT_STMT(Name)                                               \
   StmtRetTy visit##Name(Name*, ParamTys...) { return StmtRetTy(); }
#  define CDOT_EXPR(Name)                                               \
   ExprRetTy visit##Name(Name*, ParamTys...) { return ExprRetTy(); }
#  include "AST/AstNode.def"

#  define CDOT_DECL(Name)                                               \
   DeclRetTy visit##Name(Name*, ParamTys...) { return DeclRetTy(); }
#  include "AST/Decl.def"

   ExprRetTy visit(Expression *node, ParamTys... params)
   {
      switch (node->getTypeID()) {
#     define CDOT_EXPR(Name)                                            \
         case AstNode::Name##ID:                                        \
            return static_cast<SubClass*>(this)                         \
               ->visit##Name(static_cast<Name*>(node),                  \
                             std::forward<ParamTys>(params)...);
#     include "AST/AstNode.def"

      default:
         llvm_unreachable("not an expression!");
      }
   }

   StmtRetTy visit(Statement *node, ParamTys... params)
   {
      switch (node->getTypeID()) {
#     define CDOT_EXPR(Name)                                            \
         case AstNode::Name##ID:                                        \
            static_cast<SubClass*>(this)                                \
               ->visit##Name(static_cast<Name*>(node),                  \
                             std::forward<ParamTys>(params)...);        \
            return StmtRetTy();
#     define CDOT_STMT(Name)                                            \
         case AstNode::Name##ID:                                        \
            return static_cast<SubClass*>(this)                         \
               ->visit##Name(static_cast<Name*>(node),                  \
                             std::forward<ParamTys>(params)...);
#     include "AST/AstNode.def"

      default:
         llvm_unreachable("bad node kind!");
      }
   }

   DeclRetTy visit(Decl *D, ParamTys... params)
   {
      switch (D->getKind()) {
#     define CDOT_DECL(Name)                                            \
         case Decl::Name##ID:                                           \
            return static_cast<SubClass*>(this)                         \
               ->visit##Name(static_cast<Name*>(D),                     \
                             std::forward<ParamTys>(params)...);
#     include "AST/Decl.def"

      default:
         llvm_unreachable("not a decl!");
      }
   }
};

template<class SubClass, class ...ParamTys>
class RecursiveASTVisitor {
public:
   bool visit(Expression *node, ParamTys... params)
   {
      switch (node->getTypeID()) {
#     define CDOT_EXPR(Name)                                               \
         case AstNode::Name##ID: {                                         \
            bool Cont = static_cast<SubClass*>(this)                       \
               ->visit##Name(static_cast<Name*>(node),                     \
                             std::forward<ParamTys>(params)...);           \
            if (Cont)                                                      \
               RecursiveASTVisitor::visit##Name(static_cast<Name*>(node),  \
                                       std::forward<ParamTys>(params)...); \
                                                                           \
            return Cont;                                                   \
         }
#     include "AST/AstNode.def"

      default:
         llvm_unreachable("not an expression!");
      }
   }

   bool visit(Statement *node, ParamTys... params)
   {
      switch (node->getTypeID()) {
#     define CDOT_STMT(Name)                                               \
         case AstNode::Name##ID: {                                         \
            bool Cont = static_cast<SubClass*>(this)                       \
               ->visit##Name(static_cast<Name*>(node),                     \
                             std::forward<ParamTys>(params)...);           \
            if (Cont)                                                      \
               RecursiveASTVisitor::visit##Name(static_cast<Name*>(node),  \
                                       std::forward<ParamTys>(params)...); \
                                                                           \
            return Cont;                                                   \
         }
#     include "AST/AstNode.def"

      default:
         llvm_unreachable("bad node kind!");
      }
   }

protected:
   bool visitChildren(Statement *S, ParamTys... params)
   {
      switch (S->getTypeID()) {
#     define CDOT_STMT(Name)                                                  \
         case AstNode::Name##ID: {                                            \
            return RecursiveASTVisitor::visit##Name(static_cast<Name*>(S),    \
                                       std::forward<ParamTys>(params)...);    \
         }
#     include "AST/AstNode.def"

      default:
         llvm_unreachable("bad node kind!");
      }
   }

   bool visitDestructuringDecl(DestructuringDecl *D)
   {
      for (auto S : D->getDecls()) {
         if (!visit(S))
            break;
      }

      return true;
   }

   bool visitCompoundStmt(CompoundStmt* Stmt)
   {
      for (auto S : Stmt->getStatements()) {
         if (!visit(S))
            break;
      }
      
      return true;
   }

   bool visitBreakStmt(BreakStmt*) { return true; }
   bool visitContinueStmt(ContinueStmt*) { return true; }

   bool visitReturnStmt(ReturnStmt* Stmt)
   {
      if (auto RetVal = Stmt->getReturnValue())
         visit(RetVal);
      
      return true;
   }

   bool visitDiscardAssignStmt(DiscardAssignStmt *Stmt)
   {
      if (auto *RHS = Stmt->getRHS())
         visit(RHS);

      return true;
   }

   bool visitCaseStmt(CaseStmt* Stmt)
   {
      if (auto Pat = Stmt->getPattern())
         if (!visit(Pat))
            return true;

      if (auto Body = Stmt->getBody())
         visit(Body);
      
      return true;
   }

   bool visitForStmt(ForStmt* Stmt)
   {
      if (!visit(Stmt->getInitialization()))
         return true;
      
      if (!visit(Stmt->getTermination()))
         return true;

      if (!visit(Stmt->getIncrement()))
         return true;

      visit(Stmt->getBody());
      return true;
   }

   bool visitIfStmt(IfStmt* Stmt)
   {
      if (!visit(Stmt->getCondition()))
         return true;

      if (!visit(Stmt->getIfBranch()))
         return true;

      if (auto Else = Stmt->getElseBranch())
         visit(Else);

      return true;
   }

   bool visitIfLetStmt(IfLetStmt* Stmt)
   {
      if (!visit(Stmt->getIfBranch()))
         return true;

      if (auto Else = Stmt->getElseBranch())
         visit(Else);

      return true;
   }

   bool visitIfCaseStmt(IfCaseStmt* Stmt)
   {
      if (!visit(Stmt->getPattern()))
         return true;
      
      if (!visit(Stmt->getIfBranch()))
         return true;

      if (auto Else = Stmt->getElseBranch())
         visit(Else);

      return true;
   }

   bool visitWhileStmt(WhileStmt* Stmt)
   {
      if (!visit(Stmt->getCondition()))
         return true;

      if (auto Body = Stmt->getBody())
         visit(Body);

      return true;
   }

   bool visitForInStmt(ForInStmt* Stmt)
   {
      if (!visit(Stmt->getRangeExpr()))
         return true;
      
      visit(Stmt->getBody());
      return true;
   }

   bool visitMatchStmt(MatchStmt* Stmt)
   {
      if (!visit(Stmt->getSwitchValue()))
         return true;
      
      for (auto C : Stmt->getCases())
         if (!visit(C))
            return true;
         
      return true;
   }

   bool visitDoStmt(DoStmt* Stmt)
   {
      if (!visit(Stmt->getBody()))
         return true;
      
      for (auto &c : Stmt->getCatchBlocks()) {
         if (!visit(c.Body))
            return true;
      }
      
      return true;
   }

   bool visitTryExpr(TryExpr* Expr)
   {
      visit(Expr->getExpr());
      return true;
   }

   bool visitAwaitExpr(AwaitExpr* Expr)
   {
      visit(Expr->getExpr());
      return true;
   }

   bool visitThrowStmt(ThrowStmt* Stmt)
   {
      visit(Stmt->getThrownVal());
      return true;
   }

   bool visitDebugStmt(DebugStmt*) { return true; }
   bool visitNullStmt(NullStmt*) { return true; }

   bool visitStaticIfStmt(StaticIfStmt* Stmt)
   {
      if (!visit(Stmt->getCondition()))
         return true;

      if (!visit(Stmt->getIfBranch()))
         return true;

      if (auto Else = Stmt->getElseBranch())
         visit(Else);

      return true;
   }

   bool visitStaticForStmt(StaticForStmt* Stmt)
   {
      if (!visit(Stmt->getRange()))
         return true;

      visit(Stmt->getBody());
      return true;
   }

   bool visitParenExpr(ParenExpr* Stmt)
   {
      visit(Stmt->getParenthesizedExpr());
      return true;
   }

   bool visitIntegerLiteral(IntegerLiteral*) { return true; }
   bool visitFPLiteral(FPLiteral*) { return true; }
   bool visitBoolLiteral(BoolLiteral*) { return true; }
   bool visitCharLiteral(CharLiteral*) { return true; }
   bool visitNoneLiteral(NoneLiteral*) { return true; }
   bool visitStringLiteral(StringLiteral*) { return true; }

   bool visitStringInterpolation(StringInterpolation* Stmt)
   {
      for (auto &Seg : Stmt->getSegments())
         if (!visit(Seg))
            return true;

      return true;
   }

   bool visitLambdaExpr(LambdaExpr* Expr)
   {
      visit(Expr->getBody());
      return true;
   }

   bool visitDictionaryLiteral(DictionaryLiteral* Stmt)
   {
      for (auto &E : Stmt->getKeys())
         if (!visit(E))
            return true;

      for (auto &E : Stmt->getValues())
         if (!visit(E))
            return true;

      return true;
   }

   bool visitArrayLiteral(ArrayLiteral* Stmt)
   {
      for (auto &E : Stmt->getValues())
         if (!visit(E))
            return true;

      return true;
   }

   bool visitTupleLiteral(TupleLiteral* Stmt)
   {
      for (auto &E : Stmt->getElements())
         if (!visit(E))
            return true;

      return true;
   }

   bool visitIdentifierRefExpr(IdentifierRefExpr* Stmt)
   {
      if (auto E = Stmt->getParentExpr())
         visit(E);

      return true;
   }

   bool visitBuiltinIdentExpr(BuiltinIdentExpr*) { return true; }
   bool visitSelfExpr(SelfExpr*) { return true; }
   bool visitSuperExpr(SuperExpr*) { return true; }

   bool visitTupleMemberExpr(TupleMemberExpr* Stmt)
   {
      if (auto E = Stmt->getParentExpr())
         visit(E);

      return true;
   }

   bool visitCallExpr(CallExpr* Stmt)
   {
      if (auto E = Stmt->getParentExpr())
         visit(E);

      for (auto &E : Stmt->getArgs())
         if (!visit(E))
            return true;

      return true;
   }

   bool visitAnonymousCallExpr(AnonymousCallExpr* Stmt)
   {
      if (auto E = Stmt->getParentExpr())
         visit(E);

      for (auto &E : Stmt->getArgs())
         if (!visit(E))
            return true;

      return true;
   }

   bool visitEnumCaseExpr(EnumCaseExpr* Stmt)
   {
      for (auto &E : Stmt->getArgs())
         if (!visit(E))
            return true;

      return true;
   }

   bool visitSubscriptExpr(SubscriptExpr* Stmt)
   {
      if (auto E = Stmt->getParentExpr())
         visit(E);

      for (auto &E : Stmt->getIndices())
         if (!visit(E))
            return true;

      return true;
   }

   bool visitTemplateArgListExpr(TemplateArgListExpr *Expr)
   {
      if (auto E = Expr->getParentExpr())
         visit(E);

      for (auto &E : Expr->getExprs())
         if (!visit(E))
            return true;

      return true;
   }

   bool visitBuiltinExpr(BuiltinExpr*) { return true; }

   bool visitExpressionPattern(ExpressionPattern* Stmt)
   {
      visit(Stmt->getExpr());
      return true;
   }

   bool visitCasePattern(CasePattern* Stmt)
   {
      for (auto &E : Stmt->getArgs())
         if (E.isExpr())
            if (!visit(E.getExpr()))
               return true;

      return true;
   }

   bool visitIsPattern(IsPattern* Stmt)
   {
      if (auto E = Stmt->getIsType().getTypeExpr())
         visit(E);
      
      return true;
   }

   bool visitExprSequence(ExprSequence* Stmt) {
      for (auto &el : Stmt->getFragments())
         if (el.getKind() == SequenceElement::EF_Expression)
            if (!visit(el.getExpr()))
               return true;
            
      return true;
   }

   bool visitUnaryOperator(UnaryOperator* Stmt)
   {
      visit(Stmt->getTarget());
      return true;
   }

   bool visitBinaryOperator(BinaryOperator* Stmt)
   {
      if (!visit(Stmt->getLhs()))
         return true;

      visit(Stmt->getRhs());
      return true;
   }

   bool visitAssignExpr(AssignExpr* Stmt)
   {
      if (!visit(Stmt->getLhs()))
         return true;

      visit(Stmt->getRhs());
      return true;
   }

   bool visitTypePredicateExpr(TypePredicateExpr* Stmt)
   {
      if (!visit(Stmt->getLHS()))
         return true;

      visit(Stmt->getRHS());
      return true;
   }

   bool visitCastExpr(CastExpr* Stmt)
   {
      visit(Stmt->getTarget());
      return true;
   }

   bool visitImplicitCastExpr(ImplicitCastExpr* Stmt)
   {
      visit(Stmt->getTarget());
      return true;
   }

   bool visitIfExpr(IfExpr* Stmt)
   {
      if (!visit(Stmt->getCond()))
         return true;

      if (!visit(Stmt->getTrueVal()))
         return true;

      visit(Stmt->getFalseVal());
      return true;
   }

   bool visitDeclStmt(DeclStmt*) { return true; }

   bool visitStaticExpr(StaticExpr* Stmt)
   {
      visit(Stmt->getExpr());
      return true;
   }

   bool visitConstraintExpr(ConstraintExpr* Stmt)
   {
      if (Stmt->getKind() == ConstraintExpr::Type)
         if (auto E = Stmt->getTypeConstraint().getTypeExpr())
            visit(E);

      return true;
   }

   bool visitTraitsExpr(TraitsExpr* Stmt)
   {
      for (auto &arg : Stmt->getArgs()) {
         switch (arg.getKind()) {
         case TraitsArgument::Type:
            if (auto E = arg.getType().getTypeExpr())
               visit(E);

            break;
         case TraitsArgument::Stmt:
            if (!visit(arg.getStmt()))
               return true;

            break;
         case TraitsArgument::Expr:
            if (!visit(arg.getExpr()))
               return true;

            break;
         default:
            break;
         }
      }

      return true;
   }

   bool visitMixinExpr(MixinExpr* Stmt)
   {
      visit(Stmt->getMixinExpr());
      return true;
   }

   bool visitAttributedStmt(AttributedStmt* Stmt)
   {
      visit(Stmt->getStatement());
      return true;
   }

   bool visitAttributedExpr(AttributedExpr* Expr)
   {
      visit(Expr->getExpr());
      return true;
   }

   bool visitFunctionTypeExpr(FunctionTypeExpr* Expr)
   {
      for (auto &Ty : Expr->getArgTypes())
         if (auto E = Ty.getTypeExpr())
            if (!visit(E))
               return true;

      visit(Expr->getReturnType().getTypeExpr());
      return true;
   }

   bool visitTupleTypeExpr(TupleTypeExpr* Expr)
   {
      for (auto &Ty : Expr->getContainedTypes())
         if (auto E = Ty.getTypeExpr())
            if (!visit(E))
               return true;

      return true;
   }

   bool visitArrayTypeExpr(ArrayTypeExpr* Expr)
   {
      if (auto E = Expr->getElementTy().getTypeExpr())
         visit(E);

      return true;
   }

   bool visitDeclTypeExpr(DeclTypeExpr* Expr)
   {
      visit(Expr->getTyExpr());
      return true;
   }

   bool visitReferenceTypeExpr(ReferenceTypeExpr* Expr)
   {
      if (auto E = Expr->getSubType().getTypeExpr())
         visit(E);

      return true;
   }

   bool visitPointerTypeExpr(PointerTypeExpr* Expr)
   {
      if (auto E = Expr->getSubType().getTypeExpr())
         visit(E);

      return true;
   }

   bool visitOptionTypeExpr(OptionTypeExpr* Expr)
   {
      if (auto E = Expr->getSubType().getTypeExpr())
         visit(E);

      return true;
   }

   bool visitMixinStmt(MixinStmt* Expr)
   {
      visit(Expr->getMixinExpr());
      return true;
   }

   bool visitMacroExpansionExpr(MacroExpansionExpr*) { return true; }
   bool visitMacroVariableExpr(MacroVariableExpr*) { return true; }
   bool visitMacroExpansionStmt(MacroExpansionStmt*) { return true; }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_NULLASTVISITOR_H
