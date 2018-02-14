//
// Created by Jonas Zell on 24.11.17.
//

#include "ExpressionResolver.h"

#include "AST/Operator/ExprSequence.h"
#include "AST/Operator/BinaryOperator.h"
#include "AST/Operator/TertiaryOperator.h"
#include "AST/Operator/UnaryOperator.h"

#include "AST/Expression/RefExpr/IdentifierRefExpr.h"
#include "AST/Statement/Declaration/Class/RecordDecl.h"

#include "AST/Passes/SemanticAnalysis/SemaPass.h"

#include "AST/Transform.h"
#include "AST/ASTContext.h"

#include "Support/Casting.h"

#include <llvm/ADT/StringSwitch.h>

using namespace cdot::support;
using namespace cdot::diag;

namespace cdot {
namespace ast {

using SequenceElement = ExprSequence::SequenceElement;
using FragList = std::vector<SequenceElement>;

class ExprResolverImpl: public DiagnosticIssuer {
public:
   ExprResolverImpl(SemaPass &SP, ExprSequence *Expr,
                    QualType contextualType)
      : contextualType(contextualType),
        SP(SP), Fragments(Expr->getFragments()), counter(0), ExprSeq(Expr)
   {}

   ExprResolverImpl(SemaPass &SP, ExprSequence *Expr,
                    llvm::ArrayRef<SequenceElement> Fragments,
                    QualType contextualType)
      : contextualType(contextualType),
        SP(SP), Fragments(Fragments), counter(0), ExprSeq(Expr)
   {}

   Expression* ParseExpression(bool isTertiary = false);

   Expression* ParseExpressionImpl(QualType lhsTy = {},
                                   bool parsingTernaryOp = false,
                                   Expression* lhs = nullptr,
                                   int minPrecedence = 0);

   bool hasNext(unsigned add = 1) const
   {
      return counter + add <= Fragments.size();
   }

   SequenceElement const& lookahead() const
   {
      assert(hasNext() && "cant lookahead");
      return Fragments[counter];
   }

   SequenceElement const &advance()
   {
      assert(hasNext() && "cant advance");
      return Fragments[counter++];
   }

   void backtrack()
   {
      assert(counter);
      --counter;
   }

   SequenceElement const& current()
   {
      return Fragments[counter - 1];
   }

   QualType visitExpr(Expression* const& expr)
   {
      auto result = SP.visitExpr(ExprSeq, expr);
      if (result.hadError()) {
         had_error = true;
         return {};
      }

      return result.getType();
   }

   template<class T, class ...Args>
   T* makeExpr(SourceLocation const &loc, Args&& ...args)
   {
      auto ret = new (SP.getCompilationUnit().getContext())
         T(std::forward<Args&&>(args)...);

      ret->setSourceLoc(loc);

      return ret;
   }

   SequenceElement const* getUnusedElement()
   {
      if (counter == Fragments.size())
         return nullptr;

      return &Fragments[counter];
   }

   void setContextualType(Expression* expr,
                          QualType ctxType) {
      if (ctxType->isAutoType())
         ctxType = contextualType;

      expr->setContextualType(ctxType);
   }

   bool hadError() const
   {
      return had_error;
   }

private:
   Expression* ParseUnaryExpression(bool lhsOfAssignment = false,
                                    QualType lhsTy = {});

   Expression* GetUnaryExpressionTarget(QualType lhsTy);

   bool higherPrecedenceFollows(int &minPrecedence,
                                QualType ty,
                                bool higherOrEqual,
                                string &opName,
                                bool parsingTernaryOp = false);

   Expression* ParseTertiaryOperator(Expression* cond);

   // patterns

   Expression* HandleOpThenMaybeOp(QualType lhsTy, Expression *Expr = nullptr);
   Expression* HandleOpThenExpr(QualType lhsTy, Expression *Expr = nullptr);
   Expression* HandleOpThenOp(QualType lhsTy); // never valid

   Expression* HandleExprThenOp(QualType lhsTy);
   Expression* HandleExprThenMaybeOp(QualType lhsTy,
                                     Expression *Expr = nullptr);
   Expression* HandleExprThenExpr(QualType lhsTy); // never valid

   Expression* HandleMaybeOpThenOp(QualType lhsTy);
   Expression* HandleMaybeOpThenExpr(QualType lhsTy,
                                     Expression *Expr = nullptr);
   Expression* HandleMaybeOpThenMaybeOp(QualType lhsTy);

   bool isPossibleOp(const SequenceElement &El, llvm::StringRef op) const
   {
      return El.getKind() == SequenceElement::EF_PossibleOperator
             && op.equals(El.getOp());
   }

   bool isQuestion(const SequenceElement &El) const
   {
      return isPossibleOp(El, "?");
   }

   bool isColon(const SequenceElement &El) const
   {
      return isPossibleOp(El, ":");
   }

   struct OperatorResult {
      bool isValid;
      QualType returnType;
   };

   OperatorResult UnaryOpExistsOnIntegerType(QualType ty,
                                             FixKind fix,
                                             const string &op) const;

   OperatorResult UnaryOpExistsOnFPType(QualType ty,
                                        FixKind fix,
                                        const string &op) const;

   OperatorResult UnaryOpExistsOnPtrType(QualType ty,
                                         FixKind fix,
                                         const string &op) const;

   OperatorResult UnaryOpExistsOnType(QualType ty,
                                      FixKind fix,
                                      const string &op) const;

   DiagnosticBuilder &err(MessageKind msg)
   {
      had_error = true;
      return DiagnosticIssuer::err(msg);
   }

   QualType contextualType;
   SemaPass &SP;
   llvm::ArrayRef<SequenceElement> Fragments;
   size_t counter;
   bool had_error = false;

   ExprSequence *ExprSeq;
};

namespace {

bool maybeMetaTypePredicateOperator(SequenceElement const& el)
{
   if (el.isOperator()) {
      switch (el.getOperatorKind()) {
         case OperatorKind::CompEQ:
         case OperatorKind::CompNE:
            return true;
         default:
            return false;
      }
   }

   return el.isPossibleOperator() && el.getOp() == ":";
}

} // anonymous namespace

Expression*
ExprResolverImpl::GetUnaryExpressionTarget(QualType lhsTy)
{
   auto &Frag = Fragments[counter - 1];
   Expression* res = nullptr;

   switch (Frag.getKind()) {
      case SequenceElement::EF_Expression:
         res = Frag.getExpr();
         break;
      case SequenceElement::EF_Operator:
         res = makeExpr<IdentifierRefExpr>(
                                    Frag.getLoc(),
                                    operatorToString(Frag.getOperatorKind()));
         break;
      case SequenceElement::EF_PossibleOperator:
         res =  makeExpr<IdentifierRefExpr>(Frag.getLoc(),
                                            string(Frag.getOp()));
         break;
   }

   if (hasNext() && maybeMetaTypePredicateOperator(lookahead()))
      res->setMetaTypeAllowed(true);

   res->setContextualType(lhsTy);
   return res;
}

Expression*
ExprResolverImpl::HandleOpThenMaybeOp(QualType lhsTy, Expression *Expr)
{
   using Kind = ExprSequence::SequenceElement::Kind;

   // the only option is a prefix unary operator and an expression
   auto &Op = current();
   advance();

   if (!Expr)
      Expr = GetUnaryExpressionTarget(lhsTy);

   auto Ty = visitExpr(Expr);

   if (!Ty)
      return nullptr;

   // check here because this function is called by other patterns
   auto OpStr = Op.getKind() == Kind::EF_Operator
                   ? operatorToString(Op.getOperatorKind())
                   : Op.getOp();

   auto Res = UnaryOpExistsOnType(Ty, FixKind::Prefix, OpStr);

   if (!Res.isValid) {
      err(err_unary_op_not_applicable)
         << OpStr << size_t(Ty.isLvalue()) << Ty << Op.getLoc();

      return nullptr;
   }

   return makeExpr<UnaryOperator>(Op.getLoc(), move(OpStr), Expr,
                                  /*prefix=*/true);
}

Expression* ExprResolverImpl::HandleOpThenExpr(QualType lhsTy, Expression *Expr)
{
   // same as op -> maybe op
   return HandleOpThenMaybeOp(lhsTy, Expr);
}

Expression* ExprResolverImpl::HandleOpThenOp(QualType lhsTy)
{
   err(err_generic_error)
      << "unexpected operator "
         + operatorToString(lookahead().getOperatorKind())
      << lookahead().getLoc();

   return nullptr;
}

Expression*
ExprResolverImpl::HandleExprThenMaybeOp(QualType lhsTy, Expression *Expr)
{
   if (!Expr)
      Expr = GetUnaryExpressionTarget(lhsTy);

   auto Ty = visitExpr(Expr);

   if (!Ty)
      return nullptr;

   auto &Next = lookahead();

   // option 1 - postfix operator
   auto MaybePostfix = UnaryOpExistsOnType(Ty, FixKind::Postfix, Next.getOp());
   if (MaybePostfix.isValid) {
      advance();
      return makeExpr<UnaryOperator>(Expr->getSourceLoc(), string(Next.getOp()),
                                     Expr, /*prefix=*/false);
   }

   // option 2 - binary operator follows, no action here
   return Expr;
}

Expression* ExprResolverImpl::HandleExprThenExpr(QualType lhsTy)
{
   err(err_generic_error)
      << "unexpected expression"
      << lookahead().getLoc();

   return nullptr;
}

Expression* ExprResolverImpl::HandleExprThenOp(QualType lhsTy)
{
   auto Expr = GetUnaryExpressionTarget(lhsTy);

   auto &Next = lookahead();

   // option 1 - pack expansion
   if (Next.getOperatorKind() == OperatorKind::PackExpansion) {
      advance();

      Expr->setIsVariadicArgPackExpansion(true);
      if (Expr->isTypeDependent())
         return nullptr;

      return Expr;
   }

   auto Ty = visitExpr(Expr);
   if (!Ty)
      return nullptr;

   // option 2 - postfix operator
   auto OpStr = operatorToString(Next.getOperatorKind());
   auto MaybePostfix = UnaryOpExistsOnType(Ty, FixKind::Postfix, OpStr);
   if (MaybePostfix.isValid) {
      advance();
      return makeExpr<UnaryOperator>(Expr->getSourceLoc(), move(OpStr),
                                     Expr, /*prefix=*/false);
   }

   // option 3 - binary operator follows, no action here
   return Expr;
}

Expression*
ExprResolverImpl::HandleMaybeOpThenMaybeOp(QualType lhsTy)
{
   auto &Current = current();
   auto &Next = lookahead();

   // option 1 - expression -> postfix unary op | binary op
   if (auto I = SP.wouldBeValidIdentifier(Current.getOp())) {
      return HandleExprThenMaybeOp(lhsTy, I);
   }

   // option 2 - prefix unary op -> expression
   if (auto I = SP.wouldBeValidIdentifier(Next.getOp())) {
      return HandleMaybeOpThenExpr(lhsTy, I);
   }

   err(err_generic_error)
      << "unexpected identifier " + Next.getOp()
      << Next.getLoc();

   return nullptr;
}

Expression*
ExprResolverImpl::HandleMaybeOpThenExpr(QualType lhsTy, Expression *Expr)
{
   // same as op -> expr
   return HandleOpThenExpr(lhsTy, Expr);
}

Expression*
ExprResolverImpl::HandleMaybeOpThenOp(QualType lhsTy)
{
   // maybe op must be an expr
   return HandleExprThenOp(lhsTy);
}

Expression*
ExprResolverImpl::ParseUnaryExpression(bool lhsOfAssignment,
                                       QualType lhsTy) {
   using Kind = ExprSequence::SequenceElement::Kind;

   if (!hasNext())
      return GetUnaryExpressionTarget(lhsTy);

   auto CurrKind = current().getKind();
   auto NextKind = lookahead().getKind();

   switch (CurrKind) {
      case Kind::EF_Operator:
         if (current().getOperatorKind() == OperatorKind::PackExpansion) {
            return makeExpr<IdentifierRefExpr>(current().getLoc(), "...");
         }

         switch (NextKind) {
            case Kind::EF_Operator:
               return HandleOpThenOp(lhsTy);
            case Kind::EF_PossibleOperator:
               return HandleOpThenMaybeOp(lhsTy);
            case Kind::EF_Expression:
               return HandleOpThenExpr(lhsTy);
         }
      case Kind::EF_PossibleOperator:
         switch (NextKind) {
            case Kind::EF_Operator:
               return HandleMaybeOpThenOp(lhsTy);
            case Kind::EF_PossibleOperator:
               return HandleMaybeOpThenMaybeOp(lhsTy);
            case Kind::EF_Expression:
               return HandleMaybeOpThenExpr(lhsTy);
         }
      case Kind::EF_Expression:
         switch (NextKind) {
            case Kind::EF_Operator:
               return HandleExprThenOp(lhsTy);
            case Kind::EF_PossibleOperator:
               return HandleExprThenMaybeOp(lhsTy);
            case Kind::EF_Expression:
               return HandleExprThenExpr(lhsTy);
         }
   }
}

ExprResolverImpl::OperatorResult
ExprResolverImpl::UnaryOpExistsOnIntegerType(QualType ty,
                                             FixKind fix,
                                             const string &op) const {
   if (fix == FixKind::Prefix)
      return llvm::StringSwitch<OperatorResult>(op)
         .Case("+",  { true, ty })
         .Case("-",  { true, ty })
         .Case("~",  { true, ty })
         .Case("!",  { true, QualType(SP.getContext().getBoolTy()) })
         .Case("++", { true, ty })
         .Case("--", { true, ty })
         .Default({ false });

   return llvm::StringSwitch<OperatorResult>(op)
      .Case("++", { true, ty })
      .Case("--", { true, ty })
      .Default({ false });
}

ExprResolverImpl::OperatorResult
ExprResolverImpl::UnaryOpExistsOnFPType(QualType ty,
                                        FixKind fix,
                                        const string &op) const {
   if (fix == FixKind::Prefix)
      return llvm::StringSwitch<OperatorResult>(op)
         .Case("+",  { true, ty })
         .Case("-",  { true, ty })
         .Case("++", { true, ty })
         .Case("--", { true, ty })
         .Default({ false });

   return llvm::StringSwitch<OperatorResult>(op)
      .Case("++", { true, ty })
      .Case("--", { true, ty })
      .Default({ false });
}

ExprResolverImpl::OperatorResult
ExprResolverImpl::UnaryOpExistsOnPtrType(QualType ty,
                                         FixKind fix,
                                         const string &op) const {
   if (fix == FixKind::Prefix)
      return llvm::StringSwitch<OperatorResult>(op)
         .Case("*",  { true, ty->asPointerType()->getPointeeType() })
         .Case("+",  { true, ty })
         .Case("!",  { true, QualType(SP.getContext().getBoolTy()) })
         .Case("++", { true, ty })
         .Case("--", { true, ty })
         .Default({ false });

   return llvm::StringSwitch<OperatorResult>(op)
      .Case("++", { true, ty })
      .Case("--", { true, ty })
      .Default({ false });
}

ExprResolverImpl::OperatorResult
ExprResolverImpl::UnaryOpExistsOnType(QualType ty,
                                      FixKind fix,
                                      const string &op) const {
   if (fix == FixKind::Prefix && op == "typeof") {
      return { true, QualType(SP.getObjectTy("cdot.TypeInfo")) };
   }
   if (fix == FixKind::Prefix && ty.isLvalue() && op == "&") {
      return { true, QualType(ty->getPointerTo(SP.getContext())) };
   }
   else if (ty->isIntegerType()) {
      return UnaryOpExistsOnIntegerType(ty, fix, op);
   }
   else if (ty->isFPType()) {
      return UnaryOpExistsOnFPType(ty, fix, op);
   }
   else if (ty->isPointerType() && (op == "*" || op == "++" || op == "--")) {
      return UnaryOpExistsOnPtrType(ty, fix, op);
   }
   else if (ty->isObjectType()) {
      auto Record = ty->getRecord();
      auto prec = Record->getOperatorPrecedence(fix, op);

      if (prec) {
         return { true, prec->getReturnType() };
      }
   }

   return { false };
}

bool ExprResolverImpl::higherPrecedenceFollows(int &minPrecedence,
                                               QualType ty,
                                               bool higherOrEqual,
                                               string &opName,
                                               bool parsingTernaryOp) {
   if (!hasNext())
      return false;

   auto &Next = lookahead();
   if (Next.getKind() == SequenceElement::EF_Expression)
      return false;


   PrecedenceGroup PG;
   if (Next.getKind() == SequenceElement::EF_Operator) {
      opName = operatorToString(Next.getOperatorKind());
      PG = SP.getPrecedence(ty, Next.getOperatorKind());
   }
   else {
      if (Next.getOp() == "?" || (Next.getOp() == ":" && parsingTernaryOp)) {
         minPrecedence = -1;
         return false;
      }

      opName = Next.getOp();
      PG = SP.getPrecedence(ty, opName);
   }

   if (!PG.isValid()) {
      return false;
   }

   bool result;
   if (PG.getPrecedence() == minPrecedence) {
      result = higherOrEqual;
   }
   else {
      result = PG.getPrecedence() > minPrecedence;
   }

   minPrecedence = PG.getPrecedence();

   return result;
}

Expression* ExprResolverImpl::ParseExpressionImpl(QualType lhsTy,
                                                  bool parsingTernaryOp,
                                                  Expression* lhs,
                                                  int minPrecedence) {
   if (!lhs) {
      advance();

      bool isLhsOfAssignment = false;
      if (hasNext()) {
         isLhsOfAssignment = lookahead().isAssignmentOperator();
      }

      lhs = ParseUnaryExpression(isLhsOfAssignment, contextualType);
      if (!lhs)
         return nullptr;

      replaceExpressionWith(SP, ExprSeq, lhs);
   }

   lhsTy = visitExpr(lhs);
   if (!lhsTy || lhsTy.isUnknownAny())
      return nullptr;

   if (!hasNext()) {
      return lhs;
   }

   string opName;
   int savedPrecedence = minPrecedence;

   while (higherPrecedenceFollows(minPrecedence, lhsTy, true, opName,
                                  parsingTernaryOp)) {
      advance();

      if (!hasNext()) {
        err(err_unary_op_not_applicable)
           << opName << 0 << lhsTy << current().getLoc();

         return nullptr;
      }

      auto operatorLoc = current().getLoc();
      advance();

      // type constraint expression
      Expression *rhs;
      if (lhsTy->isMetaType() && opName == ":") {
         llvm_unreachable("not yet");
      }
      else {
         rhs = ParseUnaryExpression(opName == "=", lhsTy);
      }


      lhsTy = visitExpr(rhs);
      if (!lhsTy || lhsTy.isUnknownAny())
         return nullptr;

      string next;
      int savedInnerPrecedence = minPrecedence;

      while (higherPrecedenceFollows(minPrecedence, lhsTy, false, next,
                                     parsingTernaryOp)) {
         rhs = ParseExpressionImpl(lhsTy, parsingTernaryOp, rhs,
                                   minPrecedence);

         minPrecedence = savedInnerPrecedence;
         if (!rhs)
            return nullptr;
      }

      auto binOp = makeExpr<BinaryOperator>(operatorLoc, move(opName),
                                            lhs, rhs);

      replaceExpressionWith(SP, lhs, binOp);
      binOp->setContextualType(lhsTy);

      SP.updateParent(lhs, binOp);
      SP.updateParent(rhs, binOp);

      lhsTy = visitExpr(binOp);
      if (!lhsTy || lhsTy.isUnknownAny())
         return nullptr;

      lhs = binOp;
      minPrecedence = savedPrecedence;
   }

   if (hasNext()) {
      if (isQuestion(lookahead())) {
         advance();
         return ParseTertiaryOperator(lhs);
      }
      if (isColon(lookahead())) {
         advance();
      }
   }

   return lhs;
}

Expression*
ExprResolverImpl::ParseTertiaryOperator(Expression* cond)
{
   assert(isQuestion(current()));

   ExprResolverImpl LhsParser(SP, ExprSeq,
                              { Fragments.begin() + counter,
                                Fragments.size() - counter },
                              contextualType);

   auto lhs = LhsParser.ParseExpression(true);
   if (!lhs)
      return nullptr;

   if (!isColon(LhsParser.current())) {
      err(err_generic_error)
         << "expected ':' to match '?' in tertiary operator"
         << current().getLoc();

      return nullptr;
   }

   auto lhsTy = visitExpr(lhs);
   if (!lhsTy)
      return nullptr;

   ExprResolverImpl RhsParser(SP, ExprSeq,
                              { Fragments.begin() + LhsParser.counter + counter,
                               Fragments.size() - LhsParser.counter - counter },
                              lhsTy);

   auto rhs = RhsParser.ParseExpression();
   if (!rhs)
      return nullptr;

   auto expr = makeExpr<TertiaryOperator>(cond->getSourceLoc(), cond,
                                          lhs, rhs);

   expr->setContextualType(contextualType);
   counter += LhsParser.counter + RhsParser.counter;

   return expr;
}

namespace {

string sequenceElementToString(const SequenceElement &El)
{
   string s;
   switch (El.getKind()) {
      case SequenceElement::EF_Operator:
         s += "operator ";
         s += operatorToString(El.getOperatorKind());
         break;
      case SequenceElement::EF_PossibleOperator:
         s += "identifier ";
         s += El.getOp();
         break;
      case SequenceElement::EF_Expression:
         s += "expression";
         break;
   }

   return s;
}

} // anonymous namespace

Expression* ExprResolverImpl::ParseExpression(bool isTertiary)
{
   auto res = ParseExpressionImpl({}, isTertiary);
   if (!isTertiary && res && hasNext()) {
      auto ty = SP.visit(res);
      auto &next = lookahead();
      advance();

      if (next.isOperator() || next.isPossibleOperator()) {
         err(err_unary_op_not_applicable)
            << (next.isOperator() ? operatorToString(next.getOperatorKind())
                                  : next.getOp())
            << 0 << ty << next.getLoc();
      }
      else {
         err(err_generic_error)
            << "unexpected " + sequenceElementToString(Fragments[counter])
            << Fragments[counter].getLoc();
      }

      had_error = true;
      return nullptr;
   }
   if (!res)
      had_error = true;

   return res;
}

ExpressionResolver::ExprResult ExpressionResolver::resolve(ExprSequence *expr)
{
   ExprResolverImpl Resolver(SP, expr, expr->getContextualType());
   auto res = Resolver.ParseExpression();

   return { res, std::move(Resolver.getDiagnostics()),
      Resolver.hadError() };
}

} // namespace ast
} // namespace cdot