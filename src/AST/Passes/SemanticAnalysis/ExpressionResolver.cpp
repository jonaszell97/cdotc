//
// Created by Jonas Zell on 24.11.17.
//

#include "ExpressionResolver.h"
#include "CandidateSet.h"
#include "SemaPass.h"

#include "Support/Casting.h"

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
        ignoreErrors(ignoreErrors), ExprSeq(Expr)
   {}

   ExprResolverImpl(SemaPass &SP, ExprSequence *Expr,
                    llvm::ArrayRef<SequenceElement> Fragments,
                    QualType contextualType,
                    bool ignoreErrors = false)
      : contextualType(contextualType),
        SP(SP), Fragments(Fragments), counter(0),
        ignoreErrors(ignoreErrors), ExprSeq(Expr)
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

   SequenceElement const& lookahead(unsigned count = 1) const
   {
      assert(hasNext(count) && "cant lookahead");
      return Fragments[counter + count - 1];
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
   Expression* ParseUnaryExpression(QualType ContextualType = QualType());
   Expression* GetUnaryExpressionTarget();

   struct PrecedenceResult {
      PrecedenceResult() = default;

      PrecedenceResult(DeclarationName opName, SourceLocation OpLoc,
                       Expression *RHS = nullptr)
         : RHS(RHS), OpLoc(OpLoc), OpName(opName)
      {}

      PrecedenceResult(CandidateSet &&CandSet,
                       Expression *RHS,
                       DeclarationName opName,
                       SourceLocation opLoc,
                       size_t rhsIdx)
         : CandSet(std::move(CandSet)), RHS(RHS),
           OpLoc(opLoc), OpName(opName), RhsIdx(rhsIdx)
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

      SourceLocation OpLoc;

      DeclarationName OpName;
      size_t BeginIdx = 0;
      size_t RhsIdx   = 0;
   };

   PrecedenceResult getBinOpImpl(Expression *lhs);
   PrecedenceResult getBinOp(Expression *lhs);
   Expression *buildBinaryOp(PrecedenceResult &Res, Expression *lhs);

   struct UnaryOpResult {
      UnaryOpResult()
         : Expr(nullptr)
      {}

      UnaryOpResult(CandidateSet &&CandSet,
                    DeclarationName Op,
                    SourceLocation OpLoc,
                    Expression *Expr = nullptr)
         : CandSet(move(CandSet)), Expr(Expr),
           Op(move(Op)), OpLoc(OpLoc)
      { }

      operator bool() const
      {
         return (bool)CandSet;
      }

      bool holdsValue() const
      {
         return !!Op;
      }

      CandidateSet CandSet;
      Expression *Expr;
      DeclarationName Op;
      SourceLocation OpLoc;
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

   ExprSequence *ExprSeq;
};

Expression* ExprResolverImpl::GetUnaryExpressionTarget()
{
   auto &Frag = current();
   switch (Frag.getKind()) {
      case SequenceElement::EF_Expression:
         return Frag.getExpr();
      case SequenceElement::EF_Operator: {
         auto *II = &SP.getContext().getIdentifiers().get(
            op::toString(Frag.getOperatorKind()));

         return new(SP.getContext()) IdentifierRefExpr(Frag.getLoc(), II);
      }
      case SequenceElement::EF_PossibleOperator:
         return new(SP.getContext()) IdentifierRefExpr(Frag.getLoc(),
                                                       Frag.getOp());
   }
}

Expression* ExprResolverImpl::ParseUnaryExpression(QualType ContextualType)
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
            if (auto I = SP.wouldBeValidIdentifier(current().getLoc(),
                                                   current().getOp())) {
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

   auto SemaRes = SP.visitExpr(ExprSeq, Target, ContextualType);
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
            SP.diagnose(err_undeclared_identifer, current().getLoc(),
                        current().getOp());

            return nullptr;
         }

         diagnoseInvalidUnaryOp(UnOp);
         return nullptr;
      }

      SemaRes = SP.visitExpr(ExprSeq, UnOp.Expr, ContextualType);
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
               SemaRes = SP.visitExpr(ExprSeq, UnOp.Expr, ContextualType);
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
   auto &NameTable = SP.getContext().getDeclNameTable();
   DeclarationName DeclName =
      (fix == FixKind::Prefix)
        ? NameTable.getPrefixOperatorName(SP.getContext().getIdentifiers()
                                            .get(op))
        : NameTable.getPostfixOperatorName(SP.getContext().getIdentifiers()
                                             .get(op));

   auto CandSet = SP.lookupMethod(DeclName, Expr, {}, {}, ExprSeq,
                                  /*suppressDiags=*/ true);

   if (!CandSet) {
      ExprSeq->copyStatusFlags(Expr);
      return UnaryOpResult(move(CandSet), DeclName, loc, Expr);
   }

   auto &Cand = CandSet.getBestMatch();

   Expression *UnOp;
   auto ArgTypes = Cand.getFunctionType()->getParamTypes();
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
      UnOp = new(SP.getContext()) CallExpr(loc, SourceRange(loc),
                                           { Expr }, Cand.Func);
   }

   return UnaryOpResult(move(CandSet), DeclName, loc, UnOp);
}

ExprResolverImpl::PrecedenceResult
ExprResolverImpl::getBinOp(Expression *lhs)
{
   auto beginIdx = counter;
   auto PR = getBinOpImpl(lhs);

   PR.BeginIdx = beginIdx;
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

   // invalidate saved unary operator so we diagnose the bad binary operator
   // instead
   if (!lookahead(2).isOperator()) {
      LastFailedUnaryOp = UnaryOpResult();
   }

   auto op = Next.asString();
   auto opLoc = Next.getLoc();

   auto *II = &SP.getContext().getIdentifiers().get(op);
   auto DeclName = SP.getContext().getDeclNameTable().getInfixOperatorName(*II);

   auto CandSet = SP.getCandidates(DeclName, lhs);
   if (CandSet.Candidates.empty())
      return PrecedenceResult(DeclName, opLoc);

   // skip over the operator
   advance();
   advance();

   size_t rhsIdx = counter;

   auto rhs = ParseUnaryExpression(lhs->getExprType());
   if (!rhs)
      return PrecedenceResult(DeclName, opLoc);

   if (!rhs->isContextDependent()) {
      auto SemaRes = SP.visitExpr(ExprSeq, rhs, lhs->getExprType());
      if (!SemaRes) {
         HadError = true;
         return PrecedenceResult(DeclName, opLoc, rhs);
      }
      if (rhs->isTypeDependent()) {
         HadError = true;
         return PrecedenceResult(DeclName, opLoc, rhs);
      }

      rhs = SemaRes.get();
   }

   SP.lookupFunction(CandSet, DeclName, { lhs, rhs }, {},
                     ExprSeq, /*suppressDiags=*/ true);

   return PrecedenceResult(move(CandSet), rhs, DeclName, opLoc, rhsIdx);
}

Expression* ExprResolverImpl::buildBinaryOp(PrecedenceResult &Res,
                                            Expression *lhs) {
   auto &Cand = Res.CandSet.getBestMatch();
   if (Cand.isBuiltinCandidate()) {
      if (lhs->getExprType()->isMetaType()) {
         return TypePredicateExpr::Create(SP.getContext(),
                                          Res.OpLoc, lhs, Res.RHS,
                                          Cand.BuiltinCandidate.OpKind);
      }

      return BinaryOperator::Create(SP.getContext(), Res.OpLoc,
                                    Cand.BuiltinCandidate.OpKind,
                                    Cand.BuiltinCandidate.FuncTy,
                                    lhs, Res.RHS);
   }
   else {
      auto ArgTys = Cand.getFunctionType()->getParamTypes();

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

      return new(SP.getContext()) CallExpr(Res.OpLoc, SourceRange(Res.OpLoc),
                                           { lhs, rhs }, Cand.Func);
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

         counter = BinOpResult.BeginIdx;
         if (LastFailedUnaryOp.holdsValue()) {
            diagnoseInvalidUnaryOp(LastFailedUnaryOp);
         }
         else if (BinOpResult.OpName) {
            diagnoseInvalidBinOpOrRHS(lhs, BinOpResult);
         }
         else {
            diagnoseUnexpectedExpr();
         }

         return nullptr;
      }

      if (BinOpResult.getPrecedence() < minPrecedence) {
         counter = BinOpResult.BeginIdx;
         break;
      }

      auto InnerMinPrec = BinOpResult.getPrecedence();
      while (auto InnerResult = getBinOp(BinOpResult.RHS)) {
         counter = InnerResult.BeginIdx;

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

   PR.CandSet.diagnose(SP, PR.OpName, { LHS, PR.RHS }, {}, ExprSeq, true,
                       PR.OpLoc);
}

void ExprResolverImpl::diagnoseInvalidUnaryOp(UnaryOpResult &Res)
{
   Res.CandSet.diagnose(SP, Res.Op, { Res.Expr }, {}, ExprSeq, true,
                        Res.OpLoc);
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