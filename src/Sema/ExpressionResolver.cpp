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
        ignoreErrors(ignoreErrors), HadError(false), TypeDependent(false),
        ExprSeq(Expr)
   {}

   ExprResolverImpl(SemaPass &SP, ExprSequence *Expr,
                    llvm::ArrayRef<SequenceElement> Fragments,
                    QualType contextualType,
                    bool ignoreErrors = false)
      : contextualType(contextualType),
        SP(SP), Fragments(Fragments), counter(0),
        ignoreErrors(ignoreErrors), HadError(false), TypeDependent(false),
        ExprSeq(Expr)
   {}

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

   Expression* ParseUnaryExpression(QualType ContextualType = QualType());
   Expression* GetUnaryExpressionTarget();

   struct PrecedenceResult {
      PrecedenceResult() {}

      PrecedenceResult(DeclarationName opName, SourceLocation OpLoc,
                       Expression *RHS = nullptr)
         : RHS(RHS), OpLoc(OpLoc), OpName(opName)
      {}

      PrecedenceResult(CandidateSet &&CandSet,
                       Expression *RHS,
                       DeclarationName opName,
                       SourceLocation opLoc,
                       size_t endIdx)
         : CandSet(std::move(CandSet)), RHS(RHS),
           OpLoc(opLoc), OpName(opName), EndIdx(endIdx)
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
      size_t EndIdx   = 0;
   };

   Expression* ParseExpression(Expression *lhs = nullptr,
                               unsigned minPrecedence = 0,
                               PrecedenceResult &&Previous
                                 = PrecedenceResult());

   PrecedenceResult getBinOpImpl(Expression *lhs);
   PrecedenceResult getBinOp(Expression *lhs);

   bool reCheckBinaryOp(PrecedenceResult &Previous, Expression *lhs);

   Expression *buildBinaryOp(PrecedenceResult &Res, Expression *lhs);
   Expression *checkAccesssor(PrecedenceResult &Res,
                              CandidateSet::Candidate &Cand,
                              Expression *lhs);

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

   Expression *buildUnaryOp(Expression *Expr,
                            DeclarationName DeclName,
                            SourceLocation OpLoc,
                            CandidateSet::Candidate &Cand,
                            bool prefix);

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

   bool ignoreErrors  : 1;
   bool HadError      : 1;
   bool TypeDependent : 1;

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

   // there are only two ways an accessor can appear on the LHS of an
   // assignment:
   //  1. as an unqualified idenntifier, e.g. `val = 3`
   //  2. as a nested expression, e.g. `func().some.prop = 16`
   // in both scenarios the `=` directly follows a single sequence element,
   // so we can easily detect it here
   bool IsAssignment = lookahead().isOperator()
                       && lookahead().getOperatorKind() == op::Assign;

   // get expression target
   Expression *Target = nullptr;
   size_t possibleIdentLoc = string::npos;
   const SequenceElement *ExprElement = nullptr;

   while (true) {
      switch (current().getKind()) {
         case SequenceElement::EF_Expression:
            Target = GetUnaryExpressionTarget();
            Target->setIsLHSOfAssignment(IsAssignment);

            ExprElement = &current();
            break;
         case SequenceElement::EF_PossibleOperator:
            if (auto I = SP.wouldBeValidIdentifier(current().getLoc(),
                                                   current().getOp(),
                                                   IsAssignment)) {
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
            Target->setIsLHSOfAssignment(IsAssignment);

            break;
         }

         SP.diagnose(ExprSeq, err_generic_error, "expected expression");
         return nullptr;
      }

      advance();
   }

   assert(Target && "no target for unary expression!");

   auto SemaRes = SP.visitExpr(ExprSeq, Target, ContextualType);
   if (!SemaRes)
      return nullptr;

   Target = SemaRes.get();

   // update expression for possible template instantiations
   if (ExprElement)
      ExprElement->setExpr(Target);

   if (Target->isTypeDependent()) {
      TypeDependent = true;
      return nullptr;
   }

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
      if (!SemaRes)
         return nullptr;

      Target = SemaRes.get();
      if (Target->isTypeDependent()) {
         TypeDependent = true;
         return nullptr;
      }
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
               if (Target->isTypeDependent()) {
                  TypeDependent = true;
                  return nullptr;
               }

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

   if (CandSet.isDependent()) {
      TypeDependent = true;
      return UnaryOpResult();
   }

   if (!CandSet) {
      ExprSeq->copyStatusFlags(Expr);
      return UnaryOpResult(move(CandSet), DeclName, loc, Expr);
   }

   auto &Cand = CandSet.getBestMatch();

   auto ArgTypes = Cand.getFunctionType()->getParamTypes();
   if (ArgTypes.size() == 1) {
      Expr = SP.forceCast(Expr, ArgTypes[0]);
   }
   else if (!Cand.Func->hasMutableSelf()) {
      Expr = SP.castToRValue(Expr);
   }

   Expression *UnOp = buildUnaryOp(Expr, DeclName, loc, Cand,
                                   fix == FixKind::Prefix);

   return UnaryOpResult(move(CandSet), DeclName, loc, UnOp);
}

Expression* ExprResolverImpl::buildUnaryOp(Expression *Expr,
                                           DeclarationName DeclName,
                                           SourceLocation OpLoc,
                                           CandidateSet::Candidate &Cand,
                                           bool prefix) {
   Expression *UnOp;

   SourceRange SR;
   if (prefix) {
      SR = SourceRange(OpLoc, Expr->getSourceRange().getEnd());
   }
   else {
      SR = SourceRange(Expr->getSourceRange().getStart(), OpLoc);
   }

   if (!Cand.Func) {
      if (Expr->getExprType()->isMetaType() && !prefix) {
         const IdentifierInfo *II = DeclName.getPostfixOperatorName();

         if (II->isStr("*")) {
            auto Ptr = PointerTypeExpr::Create(SP.getContext(), SR,
                                               SourceType(Expr));

            Ptr->setIsMeta(true);
            UnOp = Ptr;
         }
         else if (II->isStr("?")) {
            static auto *Opt = &SP.getContext().getIdentifiers().get("Option");
            auto OptTy = new(SP.getContext())
               IdentifierRefExpr(SR, Opt, { Expr });

            OptTy->setIsSynthesized(true);
            UnOp = OptTy;
         }
         else {
            llvm_unreachable("bad meta type operator!");
         }
      }
      else {
         UnOp = UnaryOperator::Create(
            SP.getContext(), OpLoc, Cand.BuiltinCandidate.OpKind,
            Cand.BuiltinCandidate.FuncTy, Expr, prefix);
      }
   }
   else {
      UnOp = new(SP.getContext()) CallExpr(prefix ? OpLoc
                                                  : Expr->getSourceLoc(),
                                           SR, { Expr }, Cand.Func);
   }

   return UnOp;
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

   auto rhs = ParseUnaryExpression(lhs->getExprType()->stripReference());
   if (!rhs)
      return PrecedenceResult(DeclName, opLoc);

   if (!rhs->isContextDependent()) {
      auto SemaRes = SP.visitExpr(ExprSeq, rhs, lhs->getExprType()
                                                   ->stripReference());

      if (!SemaRes) {
         HadError = true;
         return PrecedenceResult(DeclName, opLoc, rhs);
      }

      rhs = SemaRes.get();
      if (rhs->isTypeDependent()) {
         TypeDependent = true;
         return PrecedenceResult(DeclName, opLoc, rhs);
      }
   }

   SP.lookupFunction(CandSet, DeclName, { lhs, rhs }, {},
                     ExprSeq, /*suppressDiags=*/ true);

   if (CandSet.isDependent()) {
      TypeDependent = true;
      return PrecedenceResult();
   }

   return PrecedenceResult(move(CandSet), rhs, DeclName, opLoc, counter);
}

bool ExprResolverImpl::reCheckBinaryOp(PrecedenceResult &Previous,
                                       Expression *lhs) {
   auto CandSet = SP.lookupFunction(Previous.OpName, { lhs, Previous.RHS },
                                    {}, ExprSeq, /*suppressDiags=*/ false);

   if (CandSet.isDependent()) {
      TypeDependent = true;
      Previous = PrecedenceResult();

      return false;
   }

   if (!CandSet)
      return false;

   Previous.CandSet = move(CandSet);
   return true;
}

Expression* ExprResolverImpl::checkAccesssor(PrecedenceResult &Res,
                                             CandidateSet::Candidate &Cand,
                                             Expression *lhs) {
   if (Cand.BuiltinCandidate.OpKind != op::Assign)
      return nullptr;

   auto Ident = dyn_cast<IdentifierRefExpr>(lhs);
   if (!Ident)
      return nullptr;

   if (Ident->getKind() != IdentifierKind::Accessor)
      return nullptr;

   // build a call to the appropriate accessor method
   return new(SP.getContext()) CallExpr(Res.OpLoc, SourceRange(Res.OpLoc),
                                        { lhs, Res.RHS },
                                        Ident->getAccessor()
                                             ->getSetterMethod());
}

Expression* ExprResolverImpl::buildBinaryOp(PrecedenceResult &Res,
                                            Expression *lhs) {
   auto &Cand = Res.CandSet.getBestMatch();

   Expression *Exprs[2] = { lhs, Res.RHS };
   SP.ApplyCasts(Exprs, ExprSeq, Res.CandSet);

   lhs = Exprs[0];
   Res.RHS = Exprs[1];

   if (Cand.isBuiltinCandidate()) {
      // check for meta type comparison
      if (lhs->getExprType()->isMetaType()) {
         bool CmpRes;
         if (Res.OpName.getInfixOperatorName()->isStr("==")) {
            CmpRes = lhs->getExprType() == Res.RHS->getExprType();
         }
         else {
            assert(Res.OpName.getInfixOperatorName()->isStr("!=")
                   && "bad meta type operator!");

            CmpRes = lhs->getExprType() != Res.RHS->getExprType();
         }

         return BoolLiteral::Create(SP.getContext(), Res.OpLoc,
                                    SP.getContext().getBoolTy(), CmpRes);
      }

      if (auto E = checkAccesssor(Res, Cand, lhs))
         return E;

      return BinaryOperator::Create(SP.getContext(), Res.OpLoc,
                                    Cand.BuiltinCandidate.OpKind,
                                    Cand.BuiltinCandidate.FuncTy,
                                    lhs, Res.RHS);
   }

   return new(SP.getContext()) CallExpr(Res.OpLoc, SourceRange(Res.OpLoc),
                                        { lhs, Res.RHS }, Cand.Func);
}

Expression* ExprResolverImpl::ParseExpression(Expression *lhs,
                                              unsigned minPrecedence,
                                              PrecedenceResult &&BinOpResult) {
   if (!BinOpResult) {
      advance();

      lhs = ParseUnaryExpression();
      if (!lhs)
         return nullptr;

      auto exprResult = SP.visitExpr(lhs);
      if (!exprResult)
         return nullptr;

      lhs = exprResult.get();
      if (lhs->isTypeDependent()) {
         TypeDependent = true;
         return nullptr;
      }
   }

   struct {
      PrecedenceResult Res;
      unsigned End = 0;

      operator bool() { return End != 0; }
      void clear() { End = 0; }

   } SavedBinaryOp;

   while (true) {
      if (!hasNext())
         return lhs;

      // can only be meant to be a postfix unary operator, but was not parsed as
      // one (i.e. is invalid)
      if (fragmentsLeft(1)) {
         if (HadError || TypeDependent)
            return nullptr;

         if (LastFailedUnaryOp.holdsValue()) {
            diagnoseInvalidUnaryOp(LastFailedUnaryOp);
         }
         else {
            diagnoseUnexpectedExpr();
         }

         return nullptr;
      }

      if (SavedBinaryOp) {
         BinOpResult = move(SavedBinaryOp.Res);
         counter = SavedBinaryOp.End;

         SavedBinaryOp.clear();
      }
      else if (!BinOpResult) {
         BinOpResult = getBinOp(lhs);
      }
      else {
         counter = BinOpResult.EndIdx;
      }

      if (!BinOpResult) {
         if (HadError || TypeDependent)
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

      QualType RHSType = BinOpResult.RHS->getExprType();

      auto InnerMinPrec = BinOpResult.getPrecedence();
      while (auto InnerResult = getBinOp(BinOpResult.RHS)) {
         unsigned End = (unsigned)counter;
         counter = InnerResult.BeginIdx;

         if (InnerResult.getPrecedence() <= InnerMinPrec) {
            SavedBinaryOp = { move(InnerResult), End };
            break;
         }

         InnerMinPrec = InnerResult.getPrecedence();
         BinOpResult.RHS = ParseExpression(BinOpResult.RHS,
                                           BinOpResult.getPrecedence(),
                                           move(InnerResult));
      }

      // if the RHS type changed, we need to re-check the binary operator
      if (BinOpResult.RHS->getExprType() != RHSType) {
         if (!reCheckBinaryOp(BinOpResult, lhs)) {
            return nullptr;
         }
      }

      RHSType = BinOpResult.RHS->getExprType();

      auto binOp = buildBinaryOp(BinOpResult, lhs);
      binOp->setContextualType(lhs->getExprType());

      auto exprResult = SP.visitExpr(ExprSeq, binOp);
      if (!exprResult)
         return nullptr;

      lhs = exprResult.get();
      if (lhs->isTypeDependent()) {
         TypeDependent = true;
         return nullptr;
      }

      // can't reuse the saved candidate if the type changed
      if (binOp->getExprType() != RHSType) {
         SavedBinaryOp.clear();
      }

      LastFailedUnaryOp = UnaryOpResult();
      BinOpResult = PrecedenceResult();
   }

   assert(!HadError && !TypeDependent && "should have returned before!");
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

   auto RHSResult = SP.visitExpr(ExprSeq, PR.RHS, LHS->getExprType());

   // ok, don't issue a diagnostic about the operator, Sema will have
   // complained about a malformed RHS
   if (!RHSResult)
      return;

   PR.RHS = RHSResult.get();
   if (PR.RHS->isTypeDependent()) {
      TypeDependent = true;
      return;
   }

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
   auto Result = Resolver.ParseExpression();

   expr->setIsInvalid(Resolver.HadError);
   expr->setIsTypeDependent(Resolver.TypeDependent);

   return Result;
}

} // namespace ast
} // namespace cdot