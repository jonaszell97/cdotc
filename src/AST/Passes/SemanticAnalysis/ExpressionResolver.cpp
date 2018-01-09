//
// Created by Jonas Zell on 24.11.17.
//

#include <llvm/ADT/StringSwitch.h>
#include "ExpressionResolver.h"

#include "../../Operator/ExprSequence.h"
#include "../../Operator/BinaryOperator.h"
#include "../../Operator/TertiaryOperator.h"
#include "../../Operator/UnaryOperator.h"
#include "../../Expression/RefExpr/IdentifierRefExpr.h"

#include "SemaPass.h"
#include "Record/Record.h"

#include "../../../Support/Casting.h"
#include "../../../lex/Lexer.h"
#include "../../SymbolTable.h"

#include "../../../Variant/Type/PrimitiveType.h"
#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/VoidType.h"
#include "../../../Variant/Type/IntegerType.h"

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

   std::shared_ptr<Expression> ParseExpression(bool isTertiary = false);

   std::shared_ptr<Expression> ParseExpressionImpl(
      QualType lhsTy = {},
      std::shared_ptr<Expression> lhs = nullptr,
      int minPrecedence = 0
   );

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

   QualType visitExpr(std::shared_ptr<Expression> const& expr)
   {
      auto result = SP.visitExpr(ExprSeq, expr);
      if (result.hadError()) {
         had_error = true;
         return {};
      }

      return result.getType();
   }

   template<class T, class ...Args>
   std::shared_ptr<T> makeExpr(SourceLocation const &loc, Args&& ...args)
   {
      auto ret = std::shared_ptr<T>::make_shared(std::forward<Args>(args)...);
      ret->setSourceLoc(loc);

      return std::move(ret);
   }

   SequenceElement const* getUnusedElement()
   {
      if (counter == Fragments.size())
         return nullptr;

      return &Fragments[counter];
   }

   void setContextualType(const std::shared_ptr<Expression> &expr,
                          QualType ctxType) {
      if (ctxType->isAutoTy())
         ctxType = contextualType;

      expr->setContextualType(ctxType);
   }

   bool hadError() const
   {
      return had_error;
   }

private:
   std::shared_ptr<Expression> ParseUnaryExpression(
      bool lhsOfAssignment = false, QualType lhsTy = {});

   std::shared_ptr<Expression> GetUnaryExpressionTarget(QualType lhsTy);

   bool higherPrecedenceFollows(int &minPrecedence,
                                QualType ty,
                                bool higherOrEqual,
                                string &opName);

   std::shared_ptr<Expression> ParseTertiaryOperator
      (std::shared_ptr<Expression> &&cond);

   // patterns

   std::shared_ptr<Expression> HandleOpThenMaybeOp(QualType lhsTy);
   std::shared_ptr<Expression> HandleOpThenExpr(QualType lhsTy);
   std::shared_ptr<Expression> HandleOpThenOp(QualType lhsTy); // never valid

   std::shared_ptr<Expression> HandleExprThenOp(QualType lhsTy);
   std::shared_ptr<Expression> HandleExprThenMaybeOp(QualType lhsTy);
   std::shared_ptr<Expression> HandleExprThenExpr(QualType lhsTy); // never valid

   std::shared_ptr<Expression> HandleMaybeOpThenOp(QualType lhsTy);
   std::shared_ptr<Expression> HandleMaybeOpThenExpr(QualType lhsTy);
   std::shared_ptr<Expression> HandleMaybeOpThenMaybeOp(QualType lhsTy);

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

std::shared_ptr<Expression>
ExprResolverImpl::GetUnaryExpressionTarget(QualType lhsTy)
{
   auto &Frag = Fragments[counter - 1];
   std::shared_ptr<Expression> res;

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

   res->setContextualType(lhsTy);

   return res;
}

std::shared_ptr<Expression>
ExprResolverImpl::HandleOpThenMaybeOp(QualType lhsTy)
{
   using Kind = ExprSequence::SequenceElement::Kind;

   // the only option is a prefix unary operator and an expression
   auto &Op = current();
   advance();

   auto Expr = GetUnaryExpressionTarget(lhsTy);
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

   return makeExpr<UnaryOperator>(Op.getLoc(), move(OpStr), move(Expr),
                                  "prefix");
}

std::shared_ptr<Expression> ExprResolverImpl::HandleOpThenExpr(QualType lhsTy)
{
   // same as op -> maybe op
   return HandleOpThenMaybeOp(lhsTy);
}

std::shared_ptr<Expression> ExprResolverImpl::HandleOpThenOp(QualType lhsTy)
{
   err(err_generic_error)
      << "unexpected operator "
         + operatorToString(lookahead().getOperatorKind())
      << lookahead().getLoc();

   return nullptr;
}

std::shared_ptr<Expression>
ExprResolverImpl::HandleExprThenMaybeOp(QualType lhsTy)
{
   auto Expr = GetUnaryExpressionTarget(lhsTy);
   auto Ty = visitExpr(Expr);

   if (!Ty)
      return nullptr;

   auto &Next = lookahead();

   // option 1 - postfix operator
   auto MaybePostfix = UnaryOpExistsOnType(Ty, FixKind::Postfix, Next.getOp());
   if (MaybePostfix.isValid) {
      advance();
      return makeExpr<UnaryOperator>(Expr->getSourceLoc(), string(Next.getOp()),
                                     move(Expr), "postfix");
   }

   // option 2 - binary operator follows, no action here
   return Expr;
}

std::shared_ptr<Expression> ExprResolverImpl::HandleExprThenExpr(QualType lhsTy)
{
   err(err_generic_error)
      << "unexpected expression"
      << lookahead().getLoc();

   return nullptr;
}

std::shared_ptr<Expression> ExprResolverImpl::HandleExprThenOp(QualType lhsTy)
{
   auto Expr = GetUnaryExpressionTarget(lhsTy);

   auto &Next = lookahead();

   // option 1 - pack expansion
   if (Next.getOperatorKind() == OperatorKind::PackExpansion) {
      advance();

      Expr->setIsVariadicArgPackExpansion(true);
      if (Expr->isTypeDependant())
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
                                     move(Expr), "postfix");
   }

   // option 3 - binary operator follows, no action here
   return Expr;
}

std::shared_ptr<Expression>
ExprResolverImpl::HandleMaybeOpThenMaybeOp(QualType lhsTy)
{
   auto &Current = current();
   auto &Next = lookahead();

   // option 1 - expression -> postfix unary op | binary op
   if (SP.wouldBeValidIdentifier(Current.getOp())) {
      return HandleExprThenMaybeOp(lhsTy);
   }

   // option 2 - prefix unary op -> expression
   if (SP.wouldBeValidIdentifier(Next.getOp())) {
      return HandleMaybeOpThenExpr(lhsTy);
   }

   err(err_generic_error)
      << "unexpected identifier " + Next.getOp()
      << Next.getLoc();

   return nullptr;
}

std::shared_ptr<Expression>
ExprResolverImpl::HandleMaybeOpThenExpr(QualType lhsTy)
{
   // same as op -> expr
   return HandleOpThenExpr(lhsTy);
}

std::shared_ptr<Expression>
ExprResolverImpl::HandleMaybeOpThenOp(QualType lhsTy)
{
   // maybe op must be an expr
   return HandleExprThenOp(lhsTy);
}

std::shared_ptr<Expression>
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
         .Case("!",  { true, QualType(IntegerType::getBoolTy()) })
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
         .Case("*",  { true, cast<PointerType>(*ty)->getPointeeType() })
         .Case("+",  { true, ty })
         .Case("!",  { true, QualType(IntegerType::getBoolTy()) })
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
      return { true, QualType(ObjectType::get("cdot.TypeInfo")) };
   }
   if (fix == FixKind::Prefix && ty.isLvalue() && op == "&") {
      return { true, QualType(ty->getPointerTo()) };
   }
   else if (ty->isIntegerTy()) {
      return UnaryOpExistsOnIntegerType(ty, fix, op);
   }
   else if (ty->isFPType()) {
      return UnaryOpExistsOnFPType(ty, fix, op);
   }
   else if (ty->isPointerTy() && (op == "*" || op == "++" || op == "--")) {
      return UnaryOpExistsOnPtrType(ty, fix, op);
   }
   else if (ty->isObjectTy()) {
      auto Record = SymbolTable::getRecord(ty->getClassName());
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
                                               string &opName) {
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
      if (Next.getOp() == "?" || Next.getOp() == ":") {
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

std::shared_ptr<Expression> ExprResolverImpl::ParseExpressionImpl(
                                                QualType lhsTy,
                                                std::shared_ptr<Expression> lhs,
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
   }

   lhsTy = visitExpr(lhs);
   if (!lhsTy)
      return nullptr;

   if (!hasNext()) {
      return lhs;
   }

   string opName;
   int savedPrecedence = minPrecedence;

   while (higherPrecedenceFollows(minPrecedence, lhsTy, true, opName)) {
      advance();

      if (!hasNext()) {
        err(err_unary_op_not_applicable)
           << opName << 0 << lhsTy << current().getLoc();

         return nullptr;
      }

      bool isCast = false;
      if (current().isOperator()) {
         auto op = current().getOperatorKind();
         isCast = op == OperatorKind::As || op == OperatorKind::AsQuestion
                  || op == OperatorKind::AsExclaim;
      }

      auto operatorLoc = current().getLoc();

      if (opName == "?") {
         advance();
         return ParseTertiaryOperator(move(lhs));
      }
      if (opName == ":") {
         advance();
         return lhs;
      }

      advance();

      auto rhs = ParseUnaryExpression(opName == "=", lhsTy);
      if (!rhs)
         return nullptr;

      lhsTy = visitExpr(rhs);
      if (!lhsTy)
         return nullptr;

      string next;
      int savedInnerPrecedence = minPrecedence;

      if (!isCast) {
         while (higherPrecedenceFollows(minPrecedence, lhsTy, false, next)) {
            rhs = ParseExpressionImpl(lhsTy, rhs, minPrecedence);
            minPrecedence = savedInnerPrecedence;

            if (!rhs)
               return nullptr;
         }
      }

      lhs = makeExpr<BinaryOperator>(operatorLoc, move(opName),
                                     move(lhs), move(rhs));

      lhs->setContextualType(lhsTy);

      lhsTy = visitExpr(lhs);
      if (!lhsTy)
         return nullptr;

      minPrecedence = savedPrecedence;
   }

   if (hasNext()) {
      if (isQuestion(lookahead())) {
         advance();
         return ParseTertiaryOperator(move(lhs));
      }
      if (isColon(lookahead())) {
         advance();
      }
   }

   return lhs;
}

std::shared_ptr<Expression>
ExprResolverImpl::ParseTertiaryOperator(std::shared_ptr<Expression> &&cond)
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

   auto expr = makeExpr<TertiaryOperator>(cond->getSourceLoc(), move(cond),
                                          move(lhs), move(rhs));

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

std::shared_ptr<Expression> ExprResolverImpl::ParseExpression(bool isTertiary)
{
   auto res = ParseExpressionImpl();
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

   return { std::move(res), std::move(Resolver.getDiagnostics()),
      Resolver.hadError() };
}

} // namespace ast
} // namespace cdot