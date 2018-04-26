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
   
   struct ExprProxy {
      /*implicit*/ ExprProxy(Expression *E,
                             const SequenceElement *Seq = nullptr)
         : E(E), Seq(Seq)
      { }
      
      /*implicit*/ operator Expression*() const { return E; }
      Expression *operator->() const { return E; }

      ExprProxy &operator=(Expression *E)
      {
         this->E = E;
         if (Seq)
            Seq->setExpr(E);
         
         return *this;
      }
      
      Expression *E;
      const SequenceElement *Seq;
   };

   struct PrecedenceResult {
      PrecedenceResult(DeclarationName OpName = DeclarationName(),
                       SourceLocation OpLoc = SourceLocation(),
                       PrecedenceGroupDecl *PG = nullptr)
         : PG(PG), OpName(OpName), OpLoc(OpLoc)
      {}

      operator bool() const
      {
         return PG != nullptr;
      }

      PrecedenceGroupDecl *PG;
      DeclarationName OpName;
      SourceLocation OpLoc;
   };

   ExprProxy ParseUnaryExpression(QualType ContextualType = QualType());
   ExprProxy GetUnaryExpressionTarget();
   ExprProxy ParseBinOpRightHandSide(PrecedenceResult &Prec);

   Expression* ParseExpression(Expression *LHS = nullptr,
                               PrecedenceGroupDecl *MinPrec = nullptr);

   PrecedenceResult getBinOp();
   PrecedenceResult precedenceFollowsImpl(PrecedenceGroupDecl *MinPrec,
                                          bool HigherOrEqual);

   PrecedenceResult higherOrEqualPrecedenceFollows(PrecedenceGroupDecl *MinPrec)
   {
      return precedenceFollowsImpl(MinPrec, true);
   }

   PrecedenceResult higherPrecedenceFollows(PrecedenceGroupDecl *MinPrec)
   {
      return precedenceFollowsImpl(MinPrec, false);
   }

   Expression *buildBinaryOp(PrecedenceResult &Res,
                             Expression *LHS, Expression *RHS);
   
   Expression *checkAccesssor(PrecedenceResult &Res,
                              CandidateSet::Candidate &Cand,
                              Expression *LHS, Expression *RHS);

   Expression *buildCastExpr(PrecedenceResult &Res,
                             CandidateSet::Candidate &Cand,
                             Expression *LHS, Expression *RHS);

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

ExprResolverImpl::ExprProxy
ExprResolverImpl::GetUnaryExpressionTarget()
{
   auto &Frag = current();
   switch (Frag.getKind()) {
      case SequenceElement::EF_Expression:
         return ExprProxy(Frag.getExpr(), &Frag);
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

ExprResolverImpl::ExprProxy 
ExprResolverImpl::ParseUnaryExpression(QualType ContextualType)
{
   auto SubstIt = ExprSubstMap.find(counter);
   if (SubstIt != ExprSubstMap.end()) {
      counter += SubstIt->second.first;
      return SubstIt->second.second;
   }

   if (!hasNext()) {
      auto Expr = GetUnaryExpressionTarget();
      Expr->setContextualType(ContextualType);

      return Expr;
   }

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
         case SequenceElement::EF_Expression: {
            auto NextExpr = GetUnaryExpressionTarget();

            Target = NextExpr.E;
            ExprElement = NextExpr.Seq;

            Target->setIsLHSOfAssignment(IsAssignment);

            ExprElement = &current();
            break;
         }
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

      if (ExprElement)
         ExprElement->setExpr(Target);
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

               if (ExprElement)
                  ExprElement->setExpr(Target);

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
   auto &II = SP.getContext().getIdentifiers().get(op);

   // fast path if there exists no prefix / postfix operator with this name
   // in the whole program (which is the case most times when calling this)
   if (fix == FixKind::Prefix && !SP.getContext().isPrefixOperator(&II)) {
      return UnaryOpResult();
   }
   if (fix == FixKind::Postfix && !SP.getContext().isPostfixOperator(&II)) {
      return UnaryOpResult();
   }

   auto &NameTable = SP.getContext().getDeclNameTable();
   DeclarationName DeclName =
      (fix == FixKind::Prefix)
        ? NameTable.getPrefixOperatorName(II)
        : NameTable.getPostfixOperatorName(II);

   auto CandSet = SP.lookupFunction(DeclName, { Expr }, {}, ExprSeq,
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

ExprResolverImpl::PrecedenceResult ExprResolverImpl::getBinOp()
{
   if (!hasNext(2))
      return PrecedenceResult();

   auto &Next = lookahead();
   if (Next.isExpression())
      return PrecedenceResult();

   auto OpStr = Next.asString();
   auto *II = &SP.getContext().getIdentifiers().get(OpStr);
   auto OpName = SP.getContext().getDeclNameTable().getInfixOperatorName(*II);

   auto Decl = SP.getContext().getInfixOperator(II);
   if (!Decl)
      return PrecedenceResult(OpName, Next.getLoc());

   return PrecedenceResult(OpName, Next.getLoc(), Decl);
}

Expression* ExprResolverImpl::checkAccesssor(PrecedenceResult &Res,
                                             CandidateSet::Candidate &Cand,
                                             Expression *LHS,
                                             Expression *RHS) {
   assert(Cand.BuiltinCandidate.OpKind == op::Assign);

   auto Ident = dyn_cast<IdentifierRefExpr>(LHS);
   if (!Ident)
      return nullptr;

   if (Ident->getKind() != IdentifierKind::Accessor)
      return nullptr;

   auto Setter = Ident->getAccessor()->getSetterMethod();
   if (!Setter) {
      // Sema should have issued a diagnostic about the missing setter
      assert(Ident->isInvalid() && "didn't complain about missing setter!");
      return nullptr;
   }

   // build a call to the appropriate accessor method
   return new(SP.getContext()) CallExpr(Res.OpLoc, SourceRange(Res.OpLoc),
                                        { LHS, RHS },
                                        Ident->getAccessor()
                                             ->getSetterMethod());
}

static CastStrength getCastStrength(op::OperatorKind Kind)
{
   switch (Kind) {
   case op::As: return CastStrength::Normal;
   case op::AsExclaim: return CastStrength::Force;
   case op::AsQuestion: return CastStrength::Fallible;
   default:
      llvm_unreachable("not a conversion operator!");
   }
}

Expression* ExprResolverImpl::buildCastExpr(PrecedenceResult &Res,
                                            CandidateSet::Candidate &Cand,
                                            Expression *LHS,
                                            Expression *RHS) {
   return CastExpr::Create(SP.getContext(), Res.OpLoc,
                           getCastStrength(Cand.BuiltinCandidate.OpKind),
                           LHS, SourceType(RHS));
}

Expression* ExprResolverImpl::buildBinaryOp(PrecedenceResult &Res,
                                            Expression *LHS,
                                            Expression *RHS) {
   auto CandSet = SP.lookupFunction(Res.OpName, { LHS, RHS }, {}, ExprSeq);
   if (!CandSet) {
      HadError = true;
      return nullptr;
   }
   
   auto &Cand = CandSet.getBestMatch();

   Expression *Exprs[2] = { LHS, RHS };
   SP.ApplyCasts(Exprs, ExprSeq, CandSet);

   LHS = Exprs[0];
   RHS = Exprs[1];

   if (Cand.isBuiltinCandidate()) {
      // check for meta type comparison
      if (LHS->getExprType()->isMetaType()) {
         bool CmpRes;
         if (Res.OpName.getInfixOperatorName()->isStr("==")) {
            CmpRes = LHS->getExprType() == RHS->getExprType();
         }
         else {
            assert(Res.OpName.getInfixOperatorName()->isStr("!=")
                   && "bad meta type operator!");

            CmpRes = LHS->getExprType() != RHS->getExprType();
         }

         return BoolLiteral::Create(SP.getContext(), Res.OpLoc,
                                    SP.getContext().getBoolTy(), CmpRes);
      }

      switch (Cand.BuiltinCandidate.OpKind) {
      case op::Assign:
         if (auto E = checkAccesssor(Res, Cand, LHS, RHS))
            return E;

         return AssignExpr::Create(SP.getContext(), Res.OpLoc, LHS, RHS);
      case op::As: case op::AsQuestion: case op::AsExclaim:
         return buildCastExpr(Res, Cand, LHS, RHS);
      default:
         return BinaryOperator::Create(SP.getContext(), Res.OpLoc,
                                       Cand.BuiltinCandidate.OpKind,
                                       Cand.BuiltinCandidate.FuncTy,
                                       LHS, RHS);
      }
   }

   return new(SP.getContext()) CallExpr(Res.OpLoc, SourceRange(Res.OpLoc),
                                        { LHS, RHS }, Cand.Func);
}

ExprResolverImpl::PrecedenceResult
ExprResolverImpl::precedenceFollowsImpl(PrecedenceGroupDecl *MinPrec,
                                        bool HigherOrEqual) {
   auto NextPrec = getBinOp();
   if (!NextPrec || !MinPrec)
      return NextPrec;

   auto Rel = NextPrec.PG->getRelationTo(MinPrec);
   switch (Rel) {
   case PrecedenceGroupDecl::Undefined:
      SP.diagnose(ExprSeq, err_undefined_precedence, NextPrec.PG->getDeclName(),
                  MinPrec->getDeclName(), ExprSeq->getSourceRange());

      return PrecedenceResult();
   case PrecedenceGroupDecl::Equal:
      if (!HigherOrEqual)
         return PrecedenceResult(NextPrec.OpName);

      LLVM_FALLTHROUGH;
   case PrecedenceGroupDecl::Higher:
      return NextPrec;
   case PrecedenceGroupDecl::Lower:
      return PrecedenceResult(NextPrec.OpName);
   }
}

Expression* ExprResolverImpl::ParseExpression(Expression *LHS,
                                              PrecedenceGroupDecl *MinPrec) {
   if (!LHS) {
      advance();

      LHS = ParseUnaryExpression();
      if (!LHS)
         return nullptr;

      auto exprResult = SP.visitExpr(LHS);
      if (!exprResult)
         return nullptr;

      LHS = exprResult.get();
      if (LHS->isTypeDependent()) {
         TypeDependent = true;
         return nullptr;
      }
   }

   while (auto NextPrec = higherOrEqualPrecedenceFollows(MinPrec)) {
      // skip over the operator
      advance();
      advance();

      // get the right hand side
      auto RHS = ParseUnaryExpression(LHS->getExprType()->stripReference());
      if (!RHS)
         return nullptr;

      auto rhsResult = SP.visitExpr(RHS);
      if (!rhsResult)
         return nullptr;

      RHS = rhsResult.get();
      if (RHS->isTypeDependent()) {
         TypeDependent = true;
         return nullptr;
      }

      // check if a higher precedence operator follows
      while (auto FollowingPrec = higherPrecedenceFollows(NextPrec.PG)) {
         RHS = ParseExpression(RHS, FollowingPrec.PG);
         if (!RHS)
            return nullptr;
      }

      // build the resulting binary operator expression
      auto binOp = buildBinaryOp(NextPrec, LHS, RHS);
      if (!binOp)
         return nullptr;

      binOp->setContextualType(LHS->getExprType());

      auto binOpResult = SP.visitExpr(ExprSeq, binOp);
      if (!binOpResult)
         return nullptr;

      LHS = binOpResult.get();
      if (LHS->isTypeDependent()) {
         TypeDependent = true;
         return nullptr;
      }

      LastFailedUnaryOp = UnaryOpResult();
   }

   // can only be meant to be a postfix unary operator, but was not parsed as
   // one (i.e. is invalid))
   if (!MinPrec && !fragmentsLeft(0)) {
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

      HadError = true;

      if (lookahead().isExpression()) {
         diagnoseUnexpectedExpr();
      }

      SP.diagnose(ExprSeq, err_generic_error, lookahead().getLoc(),
                  lookahead().asString() + " is not an operator");

      return nullptr;
   }

   assert(!HadError && !TypeDependent && "should have returned before!");
   return LHS;
}

LLVM_ATTRIBUTE_UNUSED
static llvm::StringRef dropPrefix(llvm::StringRef op)
{
   if (op.startswith("infix ")) return op.substr(6);
   if (op.startswith("prefix ")) return op.substr(7);
   if (op.startswith("postfix ")) return op.substr(8);

   return op;
}

//void ExprResolverImpl::diagnoseInvalidBinOpOrRHS(Expression *LHS,
//                                                 PrecedenceResult &PR) {
//   assert(hasNext(2) && "don't call this otherwise!");
//   advance();
//   advance();
//
//   if (!PR.RHS)
//      PR.RHS = GetUnaryExpressionTarget();
//
//   auto RHSResult = SP.visitExpr(ExprSeq, PR.RHS, LHS->getExprType());
//
//   // ok, don't issue a diagnostic about the operator, Sema will have
//   // complained about a malformed RHS
//   if (!RHSResult)
//      return;
//
//   PR.RHS = RHSResult.get();
//   if (PR.RHS->isTypeDependent()) {
//      TypeDependent = true;
//      return;
//   }
//
//   PR.CandSet.diagnose(SP, PR.OpName, { LHS, PR.RHS }, {}, ExprSeq, true,
//                       PR.OpLoc);
//}

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

   // visit all expressions so they're ready for instantiation
   // FIXME find a better way to do this
   if (Resolver.TypeDependent) {
      expr->setIsTypeDependent(true);

      for (auto &SeqEl : expr->getFragments()) {
         if (SeqEl.isExpression()) {
            auto E = SeqEl.getExpr();
            if (!E->isContextDependent()) {
               auto Res = SP.visitExpr(expr, E);
               if (Res)
                  SeqEl.setExpr(Res.get());
            }
         }
      }
   }

   return Result;
}

} // namespace ast
} // namespace cdot