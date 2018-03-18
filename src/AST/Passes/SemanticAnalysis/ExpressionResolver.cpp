//
// Created by Jonas Zell on 24.11.17.
//

#include "ExpressionResolver.h"

#include "AST/Expression.h"
#include "AST/Statement/Declaration/Class/NamedDecl.h"
#include "AST/Passes/SemanticAnalysis/SemaPass.h"

#include "AST/Transform.h"
#include "AST/ASTContext.h"

#include "Support/Casting.h"
#include "CandidateSet.h"

#include <llvm/ADT/StringSwitch.h>
#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/Twine.h>

using namespace cdot::support;
using namespace cdot::diag;

namespace cdot {
namespace ast {

using FragList = std::vector<SequenceElement>;

class ExprResolverImpl {
public:
   ExprResolverImpl(SemaPass &SP, ExprSequence *Expr,
                    QualType contextualType,
                    bool ignoreErrors = false)
      : contextualType(contextualType),
        SP(SP), Fragments(Expr->getFragments()), counter(0),
        ignoreErrors(ignoreErrors), PlaceholderExpr({}), ExprSeq(Expr)
   {}

   ExprResolverImpl(SemaPass &SP, ExprSequence *Expr,
                    llvm::ArrayRef<SequenceElement> Fragments,
                    QualType contextualType,
                    bool ignoreErrors = false)
      : contextualType(contextualType),
        SP(SP), Fragments(Fragments), counter(0),
        ignoreErrors(ignoreErrors), PlaceholderExpr({}), ExprSeq(Expr)
   {}

   Expression* ParseExpression(Expression* lhs = nullptr,
                               int minPrecedence = 0);

   bool hasNext(unsigned add = 1) const
   {
      return counter + add <= Fragments.size();
   }

   bool fragmentsLeft(unsigned num) const
   {
      return counter + num == Fragments.size();
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

   SequenceElement const& current()
   {
      return Fragments[counter - 1];
   }

   template<class T, class ...Args>
   T* makeExpr(SourceLocation const &loc, Args&& ...args)
   {
      auto ret = new (SP.getCompilationUnit().getContext())
         T(std::forward<Args&&>(args)...);

      ret->setSourceLoc(loc);

      return ret;
   }

   void setContextualType(Expression* expr,
                          QualType ctxType) {
      if (ctxType->isAutoType())
         ctxType = contextualType;

      expr->setContextualType(ctxType);
   }

   bool hadError() const { return HadError; }

private:
   Expression* ParseUnaryExpression();
   Expression* GetUnaryExpressionTarget();

   struct PrecedenceResult {
      PrecedenceResult() = default;

      PrecedenceResult(std::string &&opName, Expression *RHS = nullptr)
         : RHS(RHS), opName(move(opName))
      {}

      PrecedenceResult(CandidateSet &&CandSet,
                       Expression *RHS,
                       std::string &&opName,
                       SourceLocation opLoc,
                       size_t rhsIdx)
         : CandSet(std::move(CandSet)), RHS(RHS),
           opLoc(opLoc), opName(move(opName)), rhsIdx(rhsIdx)
      {}

      operator bool() const
      {
         return !!CandSet;
      }

      int getPrecedence()
      {
         return CandSet.getBestMatch().getPrecedenceGroup().getPrecedence();
      }

      CandidateSet CandSet;
      Expression *RHS = nullptr;

      SourceLocation opLoc;

      std::string opName;
      size_t beginIdx = 0;
      size_t rhsIdx   = 0;
   };

   PrecedenceResult getBinOpImpl(Expression *lhs);
   PrecedenceResult getBinOp(Expression *lhs);
   Expression *buildBinaryOp(PrecedenceResult &Res, Expression *lhs);

   struct UnaryOpResult {
      UnaryOpResult()
         : Expr(nullptr)
      {}

      UnaryOpResult(CandidateSet &&CandSet,
                    string &&Op,
                    Expression *Expr = nullptr)
         : CandSet(move(CandSet)), Expr(Expr),
           Op(move(Op))
      { }

      operator bool() const
      {
         return (bool)CandSet;
      }

      bool holdsValue() const
      {
         return !Op.empty();
      }

      CandidateSet CandSet;
      Expression *Expr;
      string Op;
   };

   UnaryOpResult UnaryOpExistsOnType(Expression *Expr,
                                     FixKind fix,
                                     llvm::StringRef op,
                                     SourceLocation loc);

   void diagnoseUnexpectedExpr();
   void diagnoseInvalidUnaryOp(UnaryOpResult &Res);
   void diagnoseInvalidBinOpOrRHS(Expression *LHS, PrecedenceResult &PR);

   template<class ...Args>
   void diagnose(SourceLocation loc, MessageKind msg, Args&&... args)
   {
      if (ignoreErrors)
         return;

      SP.diagnose(loc, msg, std::forward<Args>(args)...);
   }

   QualType contextualType;
   SemaPass &SP;
   llvm::ArrayRef<SequenceElement> Fragments;
   size_t counter;

   std::unordered_map<size_t, std::pair<size_t, Expression*>> ExprSubstMap;

   UnaryOpResult LastFailedUnaryOp;

   bool ignoreErrors;
   bool HadError = false;

   BuiltinExpr PlaceholderExpr;
   ExprSequence *ExprSeq;
};

Expression* ExprResolverImpl::GetUnaryExpressionTarget()
{
   auto &Frag = current();
   switch (Frag.getKind()) {
      case SequenceElement::EF_Expression:
         return Frag.getExpr();
      case SequenceElement::EF_Operator:
         return makeExpr<IdentifierRefExpr>(
            Frag.getLoc(), op::toString(Frag.getOperatorKind()));
      case SequenceElement::EF_PossibleOperator:
         return makeExpr<IdentifierRefExpr>(Frag.getLoc(),
                                            string(Frag.getOp()));
   }
}

Expression* ExprResolverImpl::ParseUnaryExpression()
{
   auto SubstIt = ExprSubstMap.find(counter);
   if (SubstIt != ExprSubstMap.end()) {
      counter += SubstIt->second.first;
      return SubstIt->second.second;
   }

   if (!hasNext())
      return GetUnaryExpressionTarget();

   auto beginIdx = counter;

   // get expression target
   Expression *Target = nullptr;
   size_t possibleIdentLoc = string::npos;

   while (!Target) {
      switch (current().getKind()) {
         case SequenceElement::EF_Expression:
            Target = GetUnaryExpressionTarget();
            break;
         case SequenceElement::EF_PossibleOperator:
            if (auto I = SP.wouldBeValidIdentifier(current().getOp())) {
               I->setSourceLoc(current().getLoc());
               Target = I;
            }
            else if (possibleIdentLoc == string::npos) {
               possibleIdentLoc = counter;
            }

            break;
         case SequenceElement::EF_Operator:
            break;
      }

      if (Target)
         break;

      if (!hasNext()) {
         if (possibleIdentLoc != string::npos) {
            counter = possibleIdentLoc;
            Target = GetUnaryExpressionTarget();

            break;
         }
         else {
            SP.diagnose(ExprSeq, err_generic_error, "expected expression");
            return nullptr;
         }
      }

      advance();
   }

   assert(Target && "no target for unary expression!");

   auto SemaRes = SP.visitExpr(ExprSeq, Target);
   if (!SemaRes || SemaRes.get()->isTypeDependent()) {
      return nullptr;
   }

   Target = SemaRes.get();
   auto savedCtr = counter;

   // if we had to skip ahead, all of the previous elements must be prefix
   // operators, evaluate them right to left
   while (counter != beginIdx) {
      assert(counter > beginIdx && "moved backwards when parsing expression!");
      --counter;

      assert(!current().isExpression() && "skipped over expr!");

      auto op = current().asString();
      auto UnOp = UnaryOpExistsOnType(Target, FixKind::Prefix, op,
                                      current().getLoc());

      if (!UnOp) {
         // probably meant to be a binary operator with an invalid left hand side
         // e.g. ident -= 3
         //      ~~~~~ ^
         // where no 'ident' is in scope
         if (possibleIdentLoc != string::npos) {
            counter = possibleIdentLoc;
            SP.diagnose(current().getLoc(), err_undeclared_identifer,
                        current().getOp());

            return nullptr;
         }

         diagnoseInvalidUnaryOp(UnOp);
         return nullptr;
      }

      SemaRes = SP.visitExpr(ExprSeq, UnOp.Expr);
      if (!SemaRes) {
         return nullptr;
      }

      Target = SemaRes.get();
   }

   counter = savedCtr;

   // check for postfix operators
   bool done = false;
   while (!done) {
      if (!hasNext())
         return Target;

      auto &Next = lookahead();
      switch (Next.getKind()) {
         case SequenceElement::EF_Expression:
            done = true;
            break;
         case SequenceElement::EF_PossibleOperator:
         case SequenceElement::EF_Operator: {
            auto op = Next.asString();
            auto UnOp = UnaryOpExistsOnType(Target, FixKind::Postfix, op,
                                            Next.getLoc());

            if (UnOp) {
               SemaRes = SP.visitExpr(ExprSeq, UnOp.Expr);
               if (!SemaRes)
                  return nullptr;

               Target = SemaRes.get();
               advance();
            }
            else {
               LastFailedUnaryOp = move(UnOp);
               done = true;
            }

            break;
         }
      }
   }

   return Target;
}

ExprResolverImpl::UnaryOpResult
ExprResolverImpl::UnaryOpExistsOnType(Expression *Expr,
                                      FixKind fix,
                                      llvm::StringRef op,
                                      SourceLocation loc) {
   llvm::SmallString<32> opName(fix == FixKind::Prefix ? "prefix" : "postfix");
   opName += " ";
   opName += op;

   auto CandSet = SP.lookupMethod(opName.str(), Expr, {}, {}, ExprSeq,
                                  /*suppressDiags=*/ true);

   if (!CandSet) {
      ExprSeq->copyStatusFlags(Expr);
      return UnaryOpResult(move(CandSet), opName.str(), Expr);
   }

   auto &Cand = CandSet.getBestMatch();

   Expression *UnOp;
   auto ArgTypes = Cand.getFunctionType()->getArgTypes();
   if (ArgTypes.size() == 1) {
      Expr = SP.forceCast(Expr, ArgTypes[0]);
   }
   else if (!Cand.Func->hasMutableSelf()) {
      Expr = SP.castToRValue(Expr);
   }

   if (!Cand.Func) {
      UnOp = UnaryOperator::Create(SP.getContext(),
                                   loc, Cand.BuiltinCandidate.OpKind,
                                   Cand.BuiltinCandidate.FuncTy, Expr,
                                   fix == FixKind::Prefix);
   }
   else {
      UnOp = makeExpr<CallExpr>(loc, std::vector<Expression*>{ Expr },
                                Cand.Func);
   }

   return UnaryOpResult(move(CandSet), opName.str(), UnOp);
}

ExprResolverImpl::PrecedenceResult
ExprResolverImpl::getBinOp(Expression *lhs)
{
   auto beginIdx = counter;
   auto PR = getBinOpImpl(lhs);

   PR.beginIdx = beginIdx;
   return PR;
}

ExprResolverImpl::PrecedenceResult
ExprResolverImpl::getBinOpImpl(Expression *lhs)
{
   if (!hasNext(2))
      return PrecedenceResult();

   auto &Next = lookahead();
   if (Next.isExpression())
      return PrecedenceResult();

   auto op = Next.asString();
   auto opLoc = current().getLoc();
   llvm::SmallString<32> funcName("infix ");
   funcName += op;

   auto CandSet = SP.getCandidates(funcName.str(), lhs);
   if (CandSet.Candidates.empty())
      return PrecedenceResult(funcName.str());

   // skip over the operator
   advance();
   advance();

   size_t rhsIdx = counter;

   auto rhs = ParseUnaryExpression();
   if (!rhs)
      return PrecedenceResult(funcName.str());

   if (!rhs->isContextDependent()) {
      auto SemaRes = SP.visitExpr(ExprSeq, rhs, lhs->getExprType());
      if (!SemaRes) {
         HadError = true;
         return PrecedenceResult(funcName.str(), rhs);
      }
      if (rhs->isTypeDependent()) {
         HadError = true;
         return PrecedenceResult(funcName.str(), rhs);
      }

      rhs = SemaRes.get();
   }

   SP.lookupFunction(CandSet, funcName.str(), { lhs, rhs }, {},
                     ExprSeq, /*suppressDiags=*/ true);

   return PrecedenceResult(move(CandSet), rhs, funcName.str(), opLoc, rhsIdx);
}

Expression* ExprResolverImpl::buildBinaryOp(PrecedenceResult &Res,
                                            Expression *lhs) {
   auto &Cand = Res.CandSet.getBestMatch();
   if (Cand.isBuiltinCandidate()) {
      if (lhs->getExprType()->isMetaType()) {
         return TypePredicateExpr::Create(SP.getContext(),
                                          Res.opLoc, lhs, Res.RHS,
                                          Cand.BuiltinCandidate.OpKind);
      }

      return BinaryOperator::Create(SP.getContext(),Res.opLoc,
                                    Cand.BuiltinCandidate.OpKind,
                                    Cand.BuiltinCandidate.FuncTy,
                                    lhs, Res.RHS);
   }
   else {
      auto ArgTys = Cand.getFunctionType()->getArgTypes();

      Expression *rhs;
      if (ArgTys.size() == 2) {
         lhs = SP.forceCast(lhs, ArgTys[0]);
         rhs = Res.RHS = SP.forceCast(Res.RHS, ArgTys[1]);
      }
      else {
         if (!Cand.Func->hasMutableSelf())
            lhs = SP.castToRValue(lhs);

         rhs = Res.RHS = SP.forceCast(Res.RHS, ArgTys[0]);
      }

      return new(SP.getContext()) CallExpr(Res.opLoc, { Res.opLoc },
                                           std::vector<Expression*>{ lhs, rhs },
                                           Cand.Func);
   }
}

Expression* ExprResolverImpl::ParseExpression(Expression* lhs,
                                              int minPrecedence) {
   if (!lhs) {
      advance();

      lhs = ParseUnaryExpression();
      if (!lhs)
         return nullptr;
   }

   auto exprResult = SP.visitExpr(lhs);
   if (!exprResult)
      return nullptr;

   lhs = exprResult.get();

   while (true) {
      if (!hasNext())
         return lhs;

      // can only be meant to be a postfix unary operator, but was not parsed as
      // one (i.e. is invalid)
      if (fragmentsLeft(1)) {
         if (LastFailedUnaryOp.holdsValue()) {
            diagnoseInvalidUnaryOp(LastFailedUnaryOp);
         }
         else {
            diagnoseUnexpectedExpr();
         }

         return nullptr;
      }

      auto BinOpResult = getBinOp(lhs);
      if (!BinOpResult) {
         if (HadError)
            return nullptr;

         counter = BinOpResult.beginIdx;
         if (LastFailedUnaryOp.holdsValue()) {
            diagnoseInvalidUnaryOp(LastFailedUnaryOp);
         }
         else {
            diagnoseInvalidBinOpOrRHS(lhs, BinOpResult);
         }

         return nullptr;
      }

      if (BinOpResult.getPrecedence() < minPrecedence) {
         counter = BinOpResult.beginIdx;
         break;
      }

      auto InnerMinPrec = BinOpResult.getPrecedence();
      while (auto InnerResult = getBinOp(BinOpResult.RHS)) {
         counter = InnerResult.beginIdx;

         if (InnerResult.getPrecedence() <= InnerMinPrec) {
            break;
         }

         BinOpResult.RHS = ParseExpression(BinOpResult.RHS, InnerMinPrec);
         if (!BinOpResult.RHS)
            return nullptr;

         InnerMinPrec = InnerResult.getPrecedence();
      }

      auto binOp = buildBinaryOp(BinOpResult, lhs);
      binOp->setContextualType(lhs->getExprType());

      SP.updateParent(lhs, binOp);
      SP.updateParent(BinOpResult.RHS, binOp);

      exprResult = SP.visitExpr(ExprSeq, binOp);
      if (!exprResult)
         return nullptr;

      lhs = exprResult.get();
      LastFailedUnaryOp = UnaryOpResult();
   }

   if (HadError)
      return nullptr;

   return lhs;
}

LLVM_ATTRIBUTE_UNUSED
static llvm::StringRef dropPrefix(llvm::StringRef op)
{
   if (op.startswith("infix ")) return op.substr(6);
   if (op.startswith("prefix ")) return op.substr(7);
   if (op.startswith("postfix ")) return op.substr(8);

   return op;
}

void ExprResolverImpl::diagnoseInvalidBinOpOrRHS(Expression *LHS,
                                                 PrecedenceResult &PR) {
   assert(hasNext(2) && "don't call this otherwise!");
   advance();
   advance();

   if (!PR.RHS)
      PR.RHS = GetUnaryExpressionTarget();

   auto RHSResult = SP.visitExpr(ExprSeq, PR.RHS);

   // ok, don't issue a diagnostic about the operator, Sema will have
   // complained about a malformed RHS
   if (!RHSResult)
      return;

   PR.RHS = RHSResult.get();

   PR.CandSet.diagnose(SP, PR.opName, { LHS, PR.RHS }, {}, ExprSeq, true);
}

void ExprResolverImpl::diagnoseInvalidUnaryOp(UnaryOpResult &Res)
{
   Res.CandSet.diagnose(SP, Res.Op, { Res.Expr }, {}, ExprSeq, true);
}

void ExprResolverImpl::diagnoseUnexpectedExpr()
{
   SP.diagnose(ExprSeq, err_unexpected_expression, lookahead().getLoc());
}

Expression *ExpressionResolver::resolve(ExprSequence *expr, bool ignoreErrors)
{
   ExprResolverImpl Resolver(SP, expr, expr->getContextualType(), ignoreErrors);
   return Resolver.ParseExpression();
}

} // namespace ast
} // namespace cdot