#ifndef CDOT_NULLASTVISITOR_H
#define CDOT_NULLASTVISITOR_H

#include "AST/Expression.h"
#include "AST/Statement.h"
#include "AST/Decl.h"
#include "Sema/ActionResult.h"

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
                                                                           \
            return Cont;                                                   \
         }
#     include "AST/AstNode.def"

      default:
         llvm_unreachable("bad node kind!");
      }
   }

   bool visitStmt(Statement *node, ParamTys... params)
   {
      return visit(node, std::forward<ParamTys>(params)...);
   }

protected:
   bool visitCompoundStmt(CompoundStmt* Stmt)
   {
      for (auto S : Stmt->getStatements()) {
         if (!static_cast<SubClass*>(this)->visitStmt(S))
            break;
      }
      
      return true;
   }

   bool visitBreakStmt(BreakStmt*) { return true; }
   bool visitContinueStmt(ContinueStmt*) { return true; }

   bool visitReturnStmt(ReturnStmt* Stmt)
   {
      if (auto RetVal = Stmt->getReturnValue())
         static_cast<SubClass*>(this)->visitStmt(RetVal);
      
      return true;
   }

   bool visitDiscardAssignStmt(DiscardAssignStmt *Stmt)
   {
      if (auto *RHS = Stmt->getRHS())
         static_cast<SubClass*>(this)->visitStmt(RHS);

      return true;
   }

   bool visitCaseStmt(CaseStmt* Stmt)
   {
      if (auto Pat = Stmt->getPattern())
         if (!static_cast<SubClass*>(this)->visitStmt(Pat))
            return true;

      if (auto Body = Stmt->getBody())
         static_cast<SubClass*>(this)->visitStmt(Body);
      
      return true;
   }

   bool visitForStmt(ForStmt* Stmt)
   {
      if (!static_cast<SubClass*>(this)->visitStmt(Stmt->getInitialization()))
         return true;
      
      if (!static_cast<SubClass*>(this)->visitStmt(Stmt->getTermination()))
         return true;

      if (!static_cast<SubClass*>(this)->visitStmt(Stmt->getIncrement()))
         return true;

      static_cast<SubClass*>(this)->visitStmt(Stmt->getBody());
      return true;
   }

   bool visitIfStmt(IfStmt* Stmt)
   {
      for (auto &C : Stmt->getConditions()) {
         switch (C.K) {
         case IfCondition::Expression:
            if (!static_cast<SubClass*>(this)->visitStmt(C.ExprData.Expr))
               return true;

            break;
         case IfCondition::Binding:
            break;
         case IfCondition::Pattern:
            if (!static_cast<SubClass*>(this)->visitStmt(C.PatternData.Pattern))
               return true;
            if (!static_cast<SubClass*>(this)->visitStmt(C.PatternData.Expr))
               return true;

            break;
         }
      }

      if (!static_cast<SubClass*>(this)->visitStmt(Stmt->getIfBranch()))
         return true;

      if (auto Else = Stmt->getElseBranch())
         static_cast<SubClass*>(this)->visitStmt(Else);

      return true;
   }

   bool visitWhileStmt(WhileStmt* Stmt)
   {
      for (auto &C : Stmt->getConditions()) {
         switch (C.K) {
         case IfCondition::Expression:
            if (!static_cast<SubClass*>(this)->visitStmt(C.ExprData.Expr))
               return true;

            break;
         case IfCondition::Binding:
            break;
         case IfCondition::Pattern:
            if (!static_cast<SubClass*>(this)->visitStmt(C.PatternData.Pattern))
               return true;
            if (!static_cast<SubClass*>(this)->visitStmt(C.PatternData.Expr))
               return true;

            break;
         }
      }

      if (auto Body = Stmt->getBody())
         static_cast<SubClass*>(this)->visitStmt(Body);

      return true;
   }

   bool visitForInStmt(ForInStmt* Stmt)
   {
      if (!static_cast<SubClass*>(this)->visitStmt(Stmt->getRangeExpr()))
         return true;
      
      static_cast<SubClass*>(this)->visitStmt(Stmt->getBody());
      return true;
   }

   bool visitMatchStmt(MatchStmt* Stmt)
   {
      if (!static_cast<SubClass*>(this)->visitStmt(Stmt->getSwitchValue()))
         return true;
      
      for (auto C : Stmt->getCases())
         if (!static_cast<SubClass*>(this)->visitStmt(C))
            return true;
         
      return true;
   }

   bool visitDoStmt(DoStmt* Stmt)
   {
      if (!static_cast<SubClass*>(this)->visitStmt(Stmt->getBody()))
         return true;
      
      for (auto &c : Stmt->getCatchBlocks()) {
         if (!static_cast<SubClass*>(this)->visitStmt(c.Body))
            return true;
      }
      
      return true;
   }

   bool visitTryExpr(TryExpr* Expr)
   {
      static_cast<SubClass*>(this)->visitStmt(Expr->getExpr());
      return true;
   }

   bool visitAwaitExpr(AwaitExpr* Expr)
   {
      static_cast<SubClass*>(this)->visitStmt(Expr->getExpr());
      return true;
   }

   bool visitThrowStmt(ThrowStmt* Stmt)
   {
      static_cast<SubClass*>(this)->visitStmt(Stmt->getThrownVal());
      return true;
   }

   bool visitDebugStmt(DebugStmt*) { return true; }
   bool visitNullStmt(NullStmt*) { return true; }

   bool visitStaticIfStmt(StaticIfStmt* Stmt)
   {
      if (!static_cast<SubClass*>(this)->visitStmt(Stmt->getCondition()))
         return true;

      if (!static_cast<SubClass*>(this)->visitStmt(Stmt->getIfBranch()))
         return true;

      if (auto Else = Stmt->getElseBranch())
         static_cast<SubClass*>(this)->visitStmt(Else);

      return true;
   }

   bool visitStaticForStmt(StaticForStmt* Stmt)
   {
      if (!static_cast<SubClass*>(this)->visitStmt(Stmt->getRange()))
         return true;

      static_cast<SubClass*>(this)->visitStmt(Stmt->getBody());
      return true;
   }

   bool visitParenExpr(ParenExpr* Stmt)
   {
      static_cast<SubClass*>(this)->visitStmt(Stmt->getParenthesizedExpr());
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
         if (!static_cast<SubClass*>(this)->visitStmt(Seg))
            return true;

      return true;
   }

   bool visitLambdaExpr(LambdaExpr* Expr)
   {
      static_cast<SubClass*>(this)->visitStmt(Expr->getBody());
      return true;
   }

   bool visitDictionaryLiteral(DictionaryLiteral* Stmt)
   {
      for (auto &E : Stmt->getKeys())
         if (!static_cast<SubClass*>(this)->visitStmt(E))
            return true;

      for (auto &E : Stmt->getValues())
         if (!static_cast<SubClass*>(this)->visitStmt(E))
            return true;

      return true;
   }

   bool visitArrayLiteral(ArrayLiteral* Stmt)
   {
      for (auto &E : Stmt->getValues())
         if (!static_cast<SubClass*>(this)->visitStmt(E))
            return true;

      return true;
   }

   bool visitTupleLiteral(TupleLiteral* Stmt)
   {
      for (auto &E : Stmt->getElements())
         if (!static_cast<SubClass*>(this)->visitStmt(E))
            return true;

      return true;
   }

   bool visitIdentifierRefExpr(IdentifierRefExpr* Stmt)
   {
      if (auto E = Stmt->getParentExpr())
         static_cast<SubClass*>(this)->visitStmt(E);

      return true;
   }

   bool visitBuiltinIdentExpr(BuiltinIdentExpr*) { return true; }
   bool visitSelfExpr(SelfExpr*) { return true; }
   bool visitSuperExpr(SuperExpr*) { return true; }

   bool visitTupleMemberExpr(TupleMemberExpr* Stmt)
   {
      if (auto E = Stmt->getParentExpr())
         static_cast<SubClass*>(this)->visitStmt(E);

      return true;
   }

   bool visitCallExpr(CallExpr* Stmt)
   {
      if (auto E = Stmt->getParentExpr())
         static_cast<SubClass*>(this)->visitStmt(E);

      for (auto &E : Stmt->getArgs())
         if (!static_cast<SubClass*>(this)->visitStmt(E))
            return true;

      return true;
   }

   bool visitAnonymousCallExpr(AnonymousCallExpr* Stmt)
   {
      if (auto E = Stmt->getParentExpr())
         static_cast<SubClass*>(this)->visitStmt(E);

      for (auto &E : Stmt->getArgs())
         if (!static_cast<SubClass*>(this)->visitStmt(E))
            return true;

      return true;
   }

   bool visitEnumCaseExpr(EnumCaseExpr* Stmt)
   {
      for (auto &E : Stmt->getArgs())
         if (!static_cast<SubClass*>(this)->visitStmt(E))
            return true;

      return true;
   }

   bool visitSubscriptExpr(SubscriptExpr* Stmt)
   {
      if (auto E = Stmt->getParentExpr())
         static_cast<SubClass*>(this)->visitStmt(E);

      for (auto &E : Stmt->getIndices())
         if (!static_cast<SubClass*>(this)->visitStmt(E))
            return true;

      return true;
   }

   bool visitTemplateArgListExpr(TemplateArgListExpr *Expr)
   {
      if (auto E = Expr->getParentExpr())
         static_cast<SubClass*>(this)->visitStmt(E);

      for (auto &E : Expr->getExprs())
         if (!static_cast<SubClass*>(this)->visitStmt(E))
            return true;

      return true;
   }

   bool visitBuiltinExpr(BuiltinExpr*) { return true; }

   bool visitExpressionPattern(ExpressionPattern* Stmt)
   {
      static_cast<SubClass*>(this)->visitStmt(Stmt->getExpr());
      return true;
   }

   bool visitCasePattern(CasePattern* Stmt)
   {
      for (auto &C : Stmt->getArgs()) {
         switch (C.K) {
         case IfCondition::Expression:
            if (!static_cast<SubClass*>(this)->visitStmt(C.ExprData.Expr))
               return true;

            break;
         case IfCondition::Binding:
            break;
         case IfCondition::Pattern:
            if (!static_cast<SubClass*>(this)->visitStmt(C.PatternData.Pattern))
               return true;
            if (!static_cast<SubClass*>(this)->visitStmt(C.PatternData.Expr))
               return true;

            break;
         }
      }

      return true;
   }

   bool visitIsPattern(IsPattern* Stmt)
   {
      if (auto E = Stmt->getIsType().getTypeExpr())
         static_cast<SubClass*>(this)->visitStmt(E);
      
      return true;
   }

   bool visitExprSequence(ExprSequence* Stmt) {
      for (auto &el : Stmt->getFragments())
         if (el.getKind() == SequenceElement::EF_Expression)
            if (!static_cast<SubClass*>(this)->visitStmt(el.getExpr()))
               return true;
            
      return true;
   }

   bool visitUnaryOperator(UnaryOperator* Stmt)
   {
      static_cast<SubClass*>(this)->visitStmt(Stmt->getTarget());
      return true;
   }

   bool visitBinaryOperator(BinaryOperator* Stmt)
   {
      if (!static_cast<SubClass*>(this)->visitStmt(Stmt->getLhs()))
         return true;

      static_cast<SubClass*>(this)->visitStmt(Stmt->getRhs());
      return true;
   }

   bool visitAssignExpr(AssignExpr* Stmt)
   {
      if (!static_cast<SubClass*>(this)->visitStmt(Stmt->getLhs()))
         return true;

      static_cast<SubClass*>(this)->visitStmt(Stmt->getRhs());
      return true;
   }

   bool visitTypePredicateExpr(TypePredicateExpr* Stmt)
   {
      if (!static_cast<SubClass*>(this)->visitStmt(Stmt->getLHS()))
         return true;

      static_cast<SubClass*>(this)->visitStmt(Stmt->getRHS());
      return true;
   }

   bool visitCastExpr(CastExpr* Stmt)
   {
      static_cast<SubClass*>(this)->visitStmt(Stmt->getTarget());
      return true;
   }

   bool visitAddrOfExpr(AddrOfExpr* Stmt)
   {
      static_cast<SubClass*>(this)->visitStmt(Stmt->getTarget());
      return true;
   }

   bool visitImplicitCastExpr(ImplicitCastExpr* Stmt)
   {
      static_cast<SubClass*>(this)->visitStmt(Stmt->getTarget());
      return true;
   }

   bool visitIfExpr(IfExpr* Stmt)
   {
      auto &C = Stmt->getCond();
      switch (C.K) {
      case IfCondition::Expression:
         if (!static_cast<SubClass*>(this)->visitStmt(C.ExprData.Expr))
            return true;

         break;
      case IfCondition::Binding:
         break;
      case IfCondition::Pattern:
         if (!static_cast<SubClass*>(this)->visitStmt(C.PatternData.Pattern))
            return true;
         if (!static_cast<SubClass*>(this)->visitStmt(C.PatternData.Expr))
            return true;

         break;
      }

      if (!static_cast<SubClass*>(this)->visitStmt(Stmt->getTrueVal()))
         return true;

      static_cast<SubClass*>(this)->visitStmt(Stmt->getFalseVal());
      return true;
   }

   bool visitDeclStmt(DeclStmt*) { return true; }

   bool visitStaticExpr(StaticExpr* Stmt)
   {
      static_cast<SubClass*>(this)->visitStmt(Stmt->getExpr());
      return true;
   }

   bool visitConstraintExpr(ConstraintExpr* Stmt)
   {
      if (Stmt->getKind() == ConstraintExpr::Type)
         if (auto E = Stmt->getTypeConstraint().getTypeExpr())
            static_cast<SubClass*>(this)->visitStmt(E);

      return true;
   }

   bool visitTraitsExpr(TraitsExpr* Stmt)
   {
      for (auto &arg : Stmt->getArgs()) {
         switch (arg.getKind()) {
         case TraitsArgument::Type:
            if (auto E = arg.getType().getTypeExpr())
               static_cast<SubClass*>(this)->visitStmt(E);

            break;
         case TraitsArgument::Stmt:
            if (!static_cast<SubClass*>(this)->visitStmt(arg.getStmt()))
               return true;

            break;
         case TraitsArgument::Expr:
            if (!static_cast<SubClass*>(this)->visitStmt(arg.getExpr()))
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
      static_cast<SubClass*>(this)->visitStmt(Stmt->getMixinExpr());
      return true;
   }

   bool visitVariadicExpansionExpr(VariadicExpansionExpr *Expr)
   {
      static_cast<SubClass*>(this)->visitStmt(Expr->getExpr());
      return true;
   }

   bool visitAttributedStmt(AttributedStmt* Stmt)
   {
      static_cast<SubClass*>(this)->visitStmt(Stmt->getStatement());
      return true;
   }

   bool visitAttributedExpr(AttributedExpr* Expr)
   {
      static_cast<SubClass*>(this)->visitStmt(Expr->getExpr());
      return true;
   }

   bool visitFunctionTypeExpr(FunctionTypeExpr* Expr)
   {
      for (auto &Ty : Expr->getArgTypes())
         if (auto E = Ty.getTypeExpr())
            if (!static_cast<SubClass*>(this)->visitStmt(E))
               return true;

      static_cast<SubClass*>(this)->visitStmt(Expr->getReturnType().getTypeExpr());
      return true;
   }

   bool visitTupleTypeExpr(TupleTypeExpr* Expr)
   {
      for (auto &Ty : Expr->getContainedTypes())
         if (auto E = Ty.getTypeExpr())
            if (!static_cast<SubClass*>(this)->visitStmt(E))
               return true;

      return true;
   }

   bool visitExistentialTypeExpr(ExistentialTypeExpr* Expr)
   {
      for (auto &Ty : Expr->getExistentials())
         if (auto E = Ty.getTypeExpr())
            if (!static_cast<SubClass*>(this)->visitStmt(E))
               return true;

      return true;
   }

   bool visitArrayTypeExpr(ArrayTypeExpr* Expr)
   {
      if (auto E = Expr->getElementTy().getTypeExpr())
         static_cast<SubClass*>(this)->visitStmt(E);

      return true;
   }

   bool visitDeclTypeExpr(DeclTypeExpr* Expr)
   {
      static_cast<SubClass*>(this)->visitStmt(Expr->getTyExpr());
      return true;
   }

   bool visitReferenceTypeExpr(ReferenceTypeExpr* Expr)
   {
      if (auto E = Expr->getSubType().getTypeExpr())
         static_cast<SubClass*>(this)->visitStmt(E);

      return true;
   }

   bool visitPointerTypeExpr(PointerTypeExpr* Expr)
   {
      if (auto E = Expr->getSubType().getTypeExpr())
         static_cast<SubClass*>(this)->visitStmt(E);

      return true;
   }

   bool visitOptionTypeExpr(OptionTypeExpr* Expr)
   {
      if (auto E = Expr->getSubType().getTypeExpr())
         static_cast<SubClass*>(this)->visitStmt(E);

      return true;
   }

   bool visitMixinStmt(MixinStmt* Expr)
   {
      static_cast<SubClass*>(this)->visitStmt(Expr->getMixinExpr());
      return true;
   }

   bool visitMacroExpansionExpr(MacroExpansionExpr*) { return true; }
   bool visitMacroVariableExpr(MacroVariableExpr*) { return true; }
   bool visitMacroExpansionStmt(MacroExpansionStmt*) { return true; }

   bool visitDeclRefExpr(DeclRefExpr*) { return true; }
   bool visitMemberRefExpr(MemberRefExpr*) { return true; }
   bool visitOverloadedDeclRefExpr(OverloadedDeclRefExpr*) { return true; }
};


template<class CallbackFn, class ...StmtClass>
class SpecificASTVisitor: public RecursiveASTVisitor<SpecificASTVisitor<CallbackFn, StmtClass...>> {
   CallbackFn fn;

   template<class T>
   void tryVisit(Statement *stmt)
   {
      if (auto *specificNode = support::dyn_cast<T>(stmt)) {
         this->fn(specificNode);
      }
   }

public:
   explicit SpecificASTVisitor(CallbackFn &&fn)
      : fn(std::move(fn))
   {}

   bool visitStmt(Statement *stmt)
   {
      (tryVisit<StmtClass>(stmt), ...);
      return RecursiveASTVisitor<SpecificASTVisitor<CallbackFn, StmtClass...>>::visit(stmt);
   }
};

template<class ...StmtClass, class CallbackFn>
void visitSpecificStatement(CallbackFn &&fn, Statement *stmt)
{
   SpecificASTVisitor<CallbackFn, StmtClass...>(std::move(fn)).visitStmt(stmt);
}

template<class SubClass>
class StmtBuilder {
protected:
   StmtBuilder() = default;

   void visitType(const SourceType &T)
   {
      if (auto *E = T.getTypeExpr()) {
         if (auto Result = visitExpr(E)) {
            T.setTypeExpr(Result.get());
         }
      }
   }

   StmtResult visitCompoundStmt(CompoundStmt* Stmt)
   {
      for (auto &S : Stmt->getStatements()) {
         auto Result = visitStmt(S);
         if (Result) {
            S = Result.get();
         }
      }

      return Stmt;
   }

   StmtResult visitBreakStmt(BreakStmt *Stmt)
   {
      return Stmt;
   }

   StmtResult visitContinueStmt(ContinueStmt *Stmt)
   {
      return Stmt;
   }

   StmtResult visitReturnStmt(ReturnStmt* Stmt)
   {
      if (auto RetVal = Stmt->getReturnValue()) {
         auto Result = visitExpr(RetVal);
         if (Result) {
            Stmt->setReturnValue(Result.get());
         }
      }

      return Stmt;
   }

   StmtResult visitDiscardAssignStmt(DiscardAssignStmt *Stmt)
   {
      if (auto Val = Stmt->getRHS()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Stmt->setRHS(Result.get());
         }
      }

      return Stmt;
   }

   StmtResult visitCaseStmt(CaseStmt* Stmt)
   {
      if (auto Val = Stmt->getPattern()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Stmt->setPattern(support::cast<PatternExpr>(Result.get()));
         }
      }

      if (auto Val = Stmt->getBody()) {
         auto Result = visitStmt(Val);
         if (Result) {
            Stmt->setBody(Result.get());
         }
      }

      return Stmt;
   }

   StmtResult visitForStmt(ForStmt* Stmt)
   {
      if (auto Val = Stmt->getInitialization()) {
         auto Result = visitStmt(Val);
         if (Result) {
            Stmt->setInitialization(Result.get());
         }
      }

      if (auto Val = Stmt->getTermination()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Stmt->setTermination(Result.get());
         }
      }

      if (auto Val = Stmt->getIncrement()) {
         auto Result = visitStmt(Val);
         if (Result) {
            Stmt->setIncrement(Result.get());
         }
      }

      if (auto Val = Stmt->getBody()) {
         auto Result = visitStmt(Val);
         if (Result) {
            Stmt->setBody(Result.get());
         }
      }

      return Stmt;
   }

   void visitIfCondition(IfCondition &C)
   {
      switch (C.K) {
      case IfCondition::Expression:
         if (auto Val = C.ExprData.Expr) {
            auto Result = visitExpr(Val);
            if (Result) {
               C.ExprData.Expr = Result.get();
            }
         }

         break;
      case IfCondition::Binding:
         break;
      case IfCondition::Pattern:
         if (auto Val = C.PatternData.Pattern) {
            auto Result = visitExpr(Val);
            if (Result) {
               C.PatternData.Pattern = support::cast<PatternExpr>(Result.get());
            }
         }
         if (auto Val = C.PatternData.Expr) {
            auto Result = visitExpr(Val);
            if (Result) {
               C.PatternData.Expr = Result.get();
            }
         }

         break;
      }
   }

   StmtResult visitIfStmt(IfStmt* Stmt)
   {
      for (auto &C : Stmt->getConditions()) {
         visitIfCondition(C);
      }

      if (auto Val = Stmt->getIfBranch()) {
         auto Result = visitStmt(Val);
         if (Result) {
            Stmt->setIfBranch(Result.get());
         }
      }

      if (auto Val = Stmt->getElseBranch()) {
         auto Result = visitStmt(Val);
         if (Result) {
            Stmt->setElseBranch(Result.get());
         }
      }

      return Stmt;
   }

   StmtResult visitWhileStmt(WhileStmt* Stmt)
   {
      for (auto &C : Stmt->getConditions()) {
         visitIfCondition(C);
      }

      if (auto Val = Stmt->getBody()) {
         auto Result = visitStmt(Val);
         if (Result) {
            Stmt->setBody(Result.get());
         }
      }

      return Stmt;
   }

   StmtResult visitForInStmt(ForInStmt* Stmt)
   {
      if (auto Val = Stmt->getRangeExpr()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Stmt->setRangeExpr(Result.get());
         }
      }

      if (auto Val = Stmt->getBody()) {
         auto Result = visitStmt(Val);
         if (Result) {
            Stmt->setBody(Result.get());
         }
      }

      return Stmt;
   }

   StmtResult visitMatchStmt(MatchStmt* Stmt)
   {
      if (auto Val = Stmt->getSwitchValue()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Stmt->setSwitchValue(Result.get());
         }
      }

      for (auto &C : Stmt->getCases()) {
         auto Result = visitStmt(C);
         if (Result) {
            C = support::cast<CaseStmt>(Result.get());
         }
      }

      return Stmt;
   }

   StmtResult visitDoStmt(DoStmt* Stmt)
   {
      if (auto Val = Stmt->getBody()) {
         auto Result = visitStmt(Val);
         if (Result) {
            Stmt->setBody(Result.get());
         }
      }

      for (auto &CB : Stmt->getCatchBlocks()) {
         auto Result = visitStmt(CB.Body);
         if (Result) {
            CB.Body = Result.get();
         }

         if (auto &Cond = CB.Condition) {
            auto Result = visitExpr(Cond);
            if (Result) {
               Cond = Result.get();
            }
         }
      }

      return Stmt;
   }

   StmtResult visitDebugStmt(DebugStmt *Stmt)
   {
      return Stmt;
   }

   StmtResult visitNullStmt(NullStmt *Stmt)
   {
      return Stmt;
   }

   StmtResult visitStaticIfStmt(StaticIfStmt* Stmt)
   {
      if (auto Val = Stmt->getCondition()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Stmt->setCondition(support::cast<StaticExpr>(Result.get()));
         }
      }

      if (auto Val = Stmt->getIfBranch()) {
         auto Result = visitStmt(Val);
         if (Result) {
            Stmt->setIfBranch(Result.get());
         }
      }

      if (auto Val = Stmt->getElseBranch()) {
         auto Result = visitStmt(Val);
         if (Result) {
            Stmt->setElseBranch(Result.get());
         }
      }

      return Stmt;
   }

   StmtResult visitStaticForStmt(StaticForStmt* Stmt)
   {
      if (auto Val = Stmt->getRange()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Stmt->setRange(support::cast<StaticExpr>(Result.get()));
         }
      }

      if (auto Val = Stmt->getBody()) {
         auto Result = visitStmt(Val);
         if (Result) {
            Stmt->setBody(Result.get());
         }
      }

      return Stmt;
   }

   StmtResult visitMixinStmt(MixinStmt* Stmt)
   {
      if (auto Val = Stmt->getMixinExpr()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Stmt->setMixinExpr(Result.get());
         }
      }

      return Stmt;
   }

   StmtResult visitMacroExpansionStmt(MacroExpansionStmt *Stmt)
   {
      return Stmt;
   }

   StmtResult visitDeclStmt(DeclStmt *Stmt)
   {
      return Stmt;
   }

   StmtResult visitAttributedStmt(AttributedStmt* Stmt)
   {
      if (auto Val = Stmt->getStatement()) {
         auto Result = visitStmt(Val);
         if (Result) {
            Stmt->setStatement(Result.get());
         }
      }

      return Stmt;
   }

   StmtResult visitThrowStmt(ThrowStmt* Stmt)
   {
      if (auto Val = Stmt->getThrownVal()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Stmt->setThrownVal(Result.get());
         }
      }

      return Stmt;
   }

   ExprResult visitTryExpr(TryExpr* Expr)
   {
      if (auto Val = Expr->getExpr()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setExpr(Result.get());
         }
      }

      return Expr;
   }

   ExprResult visitAwaitExpr(AwaitExpr* Expr)
   {
      if (auto Val = Expr->getExpr()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setExpr(Result.get());
         }
      }

      return Expr;
   }

   ExprResult visitParenExpr(ParenExpr* Expr)
   {
      if (auto Val = Expr->getParenthesizedExpr()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setParenthesizedExpr(Result.get());
         }
      }

      return Expr;
   }

   ExprResult visitIntegerLiteral(IntegerLiteral *Expr) { return Expr; }
   ExprResult visitFPLiteral(FPLiteral *Expr) { return Expr; }
   ExprResult visitBoolLiteral(BoolLiteral *Expr) { return Expr; }
   ExprResult visitCharLiteral(CharLiteral *Expr) { return Expr; }
   ExprResult visitNoneLiteral(NoneLiteral *Expr) { return Expr; }
   ExprResult visitStringLiteral(StringLiteral *Expr) { return Expr; }

   ExprResult visitStringInterpolation(StringInterpolation* Stmt)
   {
      for (auto &Val : Stmt->getSegments()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Val = Result.get();
         }
      }

      return Stmt;
   }

   ExprResult visitLambdaExpr(LambdaExpr* Expr)
   {
      if (auto Val = Expr->getBody()) {
         auto Result = visitStmt(Val);
         if (Result) {
            Expr->setBody(Result.get());
         }
      }

      for (auto &Arg : Expr->getArgs()) {
         visitType(Arg->getType());
      }

      visitType(Expr->getReturnType());
      return Expr;
   }

   ExprResult visitDictionaryLiteral(DictionaryLiteral* Expr)
   {
      for (auto &Val : Expr->getValues()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Val = Result.get();
         }
      }

      for (auto &Val : Expr->getKeys()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Val = Result.get();
         }
      }

      return Expr;
   }

   ExprResult visitArrayLiteral(ArrayLiteral* Expr)
   {
      for (auto &Val : Expr->getValues()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Val = Result.get();
         }
      }

      return Expr;
   }

   ExprResult visitTupleLiteral(TupleLiteral* Expr)
   {
      for (auto &Val : Expr->getElements()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Val = Result.get();
         }
      }

      return Expr;
   }

   ExprResult visitIdentifierRefExpr(IdentifierRefExpr* Expr)
   {
      if (auto Val = Expr->getParentExpr()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setParentExpr(Result.get());
         }
      }

      return Expr;
   }

   ExprResult visitBuiltinIdentExpr(BuiltinIdentExpr *Expr) { return Expr; }
   ExprResult visitSelfExpr(SelfExpr *Expr) { return Expr; }
   ExprResult visitSuperExpr(SuperExpr *Expr) { return Expr; }

   ExprResult visitTupleMemberExpr(TupleMemberExpr* Expr)
   {
      if (auto Val = Expr->getParentExpr()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setParentExpr(Result.get());
         }
      }

      return Expr;
   }

   ExprResult visitCallExpr(CallExpr* Expr)
   {
      if (auto Val = Expr->getParentExpr()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setParentExpr(Result.get());
         }
      }

      for (auto &Val : Expr->getArgs()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Val = Result.get();
         }
      }

      return Expr;
   }

   ExprResult visitAnonymousCallExpr(AnonymousCallExpr* Expr)
   {
      if (auto Val = Expr->getParentExpr()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setParentExpr(Result.get());
         }
      }

      for (auto &Val : Expr->getArgs()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Val = Result.get();
         }
      }

      return Expr;
   }

   ExprResult visitEnumCaseExpr(EnumCaseExpr* Expr)
   {
      for (auto &Val : Expr->getArgs()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Val = Result.get();
         }
      }

      return Expr;
   }

   ExprResult visitSubscriptExpr(SubscriptExpr* Expr)
   {
      if (auto Val = Expr->getParentExpr()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setParentExpr(Result.get());
         }
      }

      for (auto &Val : Expr->getIndices()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Val = Result.get();
         }
      }

      return Expr;
   }

   ExprResult visitTemplateArgListExpr(TemplateArgListExpr *Expr)
   {
      if (auto Val = Expr->getParentExpr()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setParentExpr(Result.get());
         }
      }

      for (auto &Val : Expr->getExprs()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Val = Result.get();
         }
      }

      return Expr;
   }

   ExprResult visitBuiltinExpr(BuiltinExpr *Expr) { return Expr; }

   ExprResult visitExpressionPattern(ExpressionPattern* Expr)
   {
      if (auto Val = Expr->getExpr()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setExpr(Result.get());
         }
      }

      return Expr;
   }

   ExprResult visitCasePattern(CasePattern* Expr)
   {
      for (auto &C : Expr->getArgs()) {
         visitIfCondition(C);
      }

      return Expr;
   }

   ExprResult visitIsPattern(IsPattern* Expr)
   {
      visitType(Expr->getIsType());
      return Expr;
   }

   ExprResult visitExprSequence(ExprSequence* Expr)
   {
      for (auto &Frag : Expr->getFragments()) {
         if (Frag.isExpression()) {
            auto Result = visitExpr(Frag.getExpr());
            if (Result) {
               Frag.setExpr(Result.get());
            }
         }
      }

      return Expr;
   }

   ExprResult visitUnaryOperator(UnaryOperator* Expr)
   {
      if (auto Val = Expr->getTarget()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setTarget(Result.get());
         }
      }

      return Expr;
   }

   ExprResult visitBinaryOperator(BinaryOperator* Expr)
   {
      if (auto Val = Expr->getLhs()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setLhs(Result.get());
         }
      }

      if (auto Val = Expr->getRhs()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setRhs(Result.get());
         }
      }

      return Expr;
   }

   ExprResult visitAssignExpr(AssignExpr* Expr)
   {
      if (auto Val = Expr->getLhs()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setLhs(Result.get());
         }
      }

      if (auto Val = Expr->getRhs()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setRhs(Result.get());
         }
      }

      return Expr;
   }

   ExprResult visitTypePredicateExpr(TypePredicateExpr* Expr)
   {
      if (auto Val = Expr->getLHS()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setLHS(Result.get());
         }
      }

      if (auto Val = Expr->getRHS()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setRHS(support::cast<ConstraintExpr>(Result.get()));
         }
      }

      return Expr;
   }

   ExprResult visitCastExpr(CastExpr* Expr)
   {
      if (auto Val = Expr->getTarget()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setTarget(Result.get());
         }
      }

      visitType(Expr->getTargetType());
      return Expr;
   }

   ExprResult visitAddrOfExpr(AddrOfExpr* Expr)
   {
      if (auto Val = Expr->getTarget()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setTarget(Result.get());
         }
      }

      return Expr;
   }

   ExprResult visitImplicitCastExpr(ImplicitCastExpr* Expr)
   {
      if (auto Val = Expr->getTarget()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setTarget(Result.get());
         }
      }

      return Expr;
   }

   ExprResult visitIfExpr(IfExpr* Expr)
   {
      auto &C = Expr->getCond();
      visitIfCondition(C);

      if (auto Val = Expr->getTrueVal()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setTrueVal(Result.get());
         }
      }

      if (auto Val = Expr->getFalseVal()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setFalseVal(Result.get());
         }
      }

      return Expr;
   }

   ExprResult visitStaticExpr(StaticExpr* Expr)
   {
      if (auto Val = Expr->getExpr()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setExpr(Result.get());
         }
      }

      return Expr;
   }

   ExprResult visitConstraintExpr(ConstraintExpr* Expr)
   {
      return Expr;
   }

   ExprResult visitTraitsExpr(TraitsExpr* Expr)
   {
      for (auto &arg : Expr->getArgs()) {
         switch (arg.getKind()) {
         case TraitsArgument::Type:
            visitType(arg.getType());
            break;
         case TraitsArgument::Stmt:
            if (auto Result = visitStmt(arg.getStmt())) {
               arg.setStmt(Result.get());
            }

            break;
         case TraitsArgument::Expr:
            if (auto Result = visitExpr(arg.getExpr())) {
               arg.setExpr(Result.get());
            }

            break;
         default:
            break;
         }
      }

      return Expr;
   }

   ExprResult visitMixinExpr(MixinExpr* Expr)
   {
      if (auto Val = Expr->getMixinExpr()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setMixinExpr(Result.get());
         }
      }

      return Expr;
   }

   ExprResult visitVariadicExpansionExpr(VariadicExpansionExpr *Expr)
   {
      if (auto Val = Expr->getExpr()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setExpr(Result.get());
         }
      }

      return Expr;
   }

   ExprResult visitAttributedExpr(AttributedExpr* Expr)
   {
      if (auto Val = Expr->getExpr()) {
         auto Result = visitExpr(Val);
         if (Result) {
            Expr->setExpr(Result.get());
         }
      }

      return Expr;
   }

   ExprResult visitFunctionTypeExpr(FunctionTypeExpr* Expr)
   {
      for (auto &Arg : Expr->getArgTypes()) {
         visitType(Arg);
      }

      visitType(Expr->getReturnType());
      return Expr;
   }

   ExprResult visitTupleTypeExpr(TupleTypeExpr* Expr)
   {
      for (auto &Arg : Expr->getContainedTypes()) {
         visitType(Arg);
      }

      return Expr;
   }

   ExprResult visitExistentialTypeExpr(ExistentialTypeExpr* Expr)
   {
      for (auto &Arg : Expr->getExistentials()) {
         visitType(Arg);
      }

      return Expr;
   }

   ExprResult visitArrayTypeExpr(ArrayTypeExpr* Expr)
   {
      visitType(Expr->getElementTy());
      return Expr;
   }

   ExprResult visitDeclTypeExpr(DeclTypeExpr* Expr)
   {
      return Expr;
   }

   ExprResult visitReferenceTypeExpr(ReferenceTypeExpr* Expr)
   {
      visitType(Expr->getSubType());
      return Expr;
   }

   ExprResult visitPointerTypeExpr(PointerTypeExpr* Expr)
   {
      visitType(Expr->getSubType());
      return Expr;
   }

   ExprResult visitOptionTypeExpr(OptionTypeExpr* Expr)
   {
      visitType(Expr->getSubType());
      return Expr;
   }

   ExprResult visitMacroExpansionExpr(MacroExpansionExpr *Expr) { return Expr; }
   ExprResult visitMacroVariableExpr(MacroVariableExpr *Expr) { return Expr; }

   ExprResult visitDeclRefExpr(DeclRefExpr *Expr) { return Expr; }
   ExprResult visitMemberRefExpr(MemberRefExpr *Expr) { return Expr; }
   ExprResult visitOverloadedDeclRefExpr(OverloadedDeclRefExpr *Expr)
   { return Expr; }

public:
   ExprResult visitExpr(Expression *E)
   {
      switch (E->getTypeID()) {
#     define CDOT_EXPR(NAME)                                            \
         case AstNode::NAME##ID:                                        \
            return static_cast<SubClass*>(this)                         \
               ->visit##NAME(static_cast<NAME*>(E));
#     include "AST/AstNode.def"

      default:
         llvm_unreachable("not an expression!");
      }
   }

   StmtResult visitStmt(Statement *S)
   {
      switch (S->getTypeID()) {
#     define CDOT_EXPR(NAME)                                            \
         case AstNode::NAME##ID: {                                      \
            auto Res = static_cast<SubClass*>(this)                     \
               ->visit##NAME(static_cast<NAME*>(S));                    \
            return Res ? StmtResult(Res.get()) : StmtError();           \
         }
#     define CDOT_STMT(NAME)                                            \
         case AstNode::NAME##ID:                                        \
            return static_cast<SubClass*>(this)                         \
               ->visit##NAME(static_cast<NAME*>(S));
#     include "AST/AstNode.def"

      default:
         llvm_unreachable("not an expression!");
      }
   }
};

template<class SubClass>
class ASTTypeVisitor {
public:
   ASTTypeVisitor(bool VisitSubDecls = true) : VisitSubDecls(VisitSubDecls)
   {}

   void visitType(const SourceType &Ty)
   {
      static_cast<SubClass*>(this)->visitType(Ty);
   }

protected:
   bool VisitSubDecls;

   void visit(Decl *D)
   {
      switch (D->getKind()) {
#     define CDOT_DECL(Name)                                            \
         case Decl::Name##ID:                                           \
            return visit##Name(static_cast<Name*>(D));
#     include "AST/Decl.def"

      default:
         llvm_unreachable("not a decl!");
      }
   }

   void visitDeclContext(DeclContext *DC)
   {
      if (!VisitSubDecls) {
         return;
      }

      for (auto *D : DC->getDecls())
         visit(D);
   }

   void visitSourceFileDecl(SourceFileDecl *D)
   {

   }

   void visitDebugDecl(DebugDecl *D)
   {

   }

   void visitStaticAssertDecl(StaticAssertDecl *D)
   {

   }

   void visitStaticPrintDecl(StaticPrintDecl *D)
   {

   }

   void visitStaticIfDecl(StaticIfDecl *D)
   {
      visit(D->getIfDecl());
      visit(D->getElseDecl());
   }

   void visitStaticForDecl(StaticForDecl *D)
   {

   }

   void visitMixinDecl(MixinDecl *D)
   {

   }

   void visitCompoundDecl(CompoundDecl *D)
   {
      visitDeclContext(D);
   }

   void visitMacroExpansionDecl(MacroExpansionDecl *D)
   {

   }

   void visitAssociatedTypeDecl(AssociatedTypeDecl *D)
   {
      visitType(D->getDefaultType());
   }

   void visitPropDecl(PropDecl *D)
   {
      visitType(D->getType());
   }

   void visitSubscriptDecl(SubscriptDecl *D)
   {
      visitType(D->getType());
   }

   void visitTypedefDecl(TypedefDecl *D)
   {
      llvm_unreachable("NO!");
   }

   void visitNamespaceDecl(NamespaceDecl *D)
   {
      visitDeclContext(D);
   }

   void visitUnittestDecl(UnittestDecl *D)
   {

   }

   void visitAliasDecl(AliasDecl *D)
   {
      visitType(D->getType());
      visitDeclContext(D);
   }

   void visitPrecedenceGroupDecl(PrecedenceGroupDecl *D)
   {

   }

   void visitOperatorDecl(OperatorDecl *D)
   {

   }

   void visitMacroDecl(MacroDecl *D)
   {

   }

   void visitModuleDecl(ModuleDecl *D)
   {
      visitDeclContext(D);
   }

   void visitImportDecl(ImportDecl *D)
   {

   }

   void visitUsingDecl(UsingDecl *D)
   {

   }

   void visitVarDecl(VarDecl *D)
   {
      visitType(D->getType());
   }

   void visitLocalVarDecl(LocalVarDecl *D)
   {
      visitVarDecl(D);
   }

   void visitGlobalVarDecl(GlobalVarDecl *D)
   {
      visitVarDecl(D);
   }

   void visitDestructuringDecl(DestructuringDecl *D)
   {
      for (auto *Var : D->getDecls())
         visit(Var);

      visitType(D->getType());
   }

   void visitFuncArgDecl(FuncArgDecl *D)
   {
      visitVarDecl(D);
   }

   void visitFieldDecl(FieldDecl *D)
   {
      visitVarDecl(D);
   }

   void visitTemplateParamDecl(TemplateParamDecl *D)
   {
      visitType(D->getCovariance());
      visitType(D->getContravariance());
   }

   void visitRecordDecl(RecordDecl *D)
   {
      visitDeclContext(D);

      for (auto &Conf : D->getConformanceTypes())
         visitType(Conf);
   }

   void visitStructDecl(StructDecl *D)
   {
      visitRecordDecl(D);
   }

   void visitClassDecl(ClassDecl *D)
   {
      visitStructDecl(D);
      visitType(D->getParentType());
   }

   void visitEnumDecl(EnumDecl *D)
   {
      visitRecordDecl(D);
      visitType(D->getRawType());
   }

   void visitUnionDecl(UnionDecl *D)
   {
      visitRecordDecl(D);
   }

   void visitProtocolDecl(ProtocolDecl *D)
   {
      visitRecordDecl(D);
   }

   void visitExtensionDecl(ExtensionDecl *D)
   {
      visitDeclContext(D);

      for (auto &Conf : D->getConformanceTypes())
         visitType(Conf);
   }

   void visitCallableDecl(CallableDecl *D)
   {
      visitType(D->getReturnType());
      visitDeclContext(D);
   }

   void visitFunctionDecl(FunctionDecl *D)
   {
      visitCallableDecl(D);
   }

   void visitMethodDecl(MethodDecl *D)
   {
      visitCallableDecl(D);
   }

   void visitInitDecl(InitDecl *D)
   {
      visitMethodDecl(D);
   }

   void visitDeinitDecl(DeinitDecl *D)
   {
      visitMethodDecl(D);
   }

   void visitEnumCaseDecl(EnumCaseDecl *D)
   {
      visitCallableDecl(D);
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_NULLASTVISITOR_H
