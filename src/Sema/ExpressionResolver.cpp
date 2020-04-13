#include "cdotc/Sema/ExpressionResolver.h"

#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/CandidateSet.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Support/Casting.h"
#include "cdotc/Support/StringSwitch.h"

#include <llvm/ADT/SmallString.h>
#include <llvm/ADT/Twine.h>

using namespace cdot::support;
using namespace cdot::diag;

namespace cdot {
namespace ast {

using FragList = std::vector<SequenceElement>;

class ExprResolverImpl {
public:
   ExprResolverImpl(SemaPass& SP, ExprSequence* Expr, QualType contextualType,
                    bool ignoreErrors = false)
       : contextualType(contextualType), SP(SP),
         Fragments(Expr->getFragments()), counter(0),
         ignoreErrors(ignoreErrors), HadError(false), TypeDependent(false),
         ExprSeq(Expr)
   {
   }

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

   SequenceElement const& advance()
   {
      assert(hasNext() && "cant advance");
      return Fragments[counter++];
   }

   SequenceElement const& current() { return Fragments[counter - 1]; }

   struct PrecedenceResult {
      PrecedenceResult(DeclarationName OpName = DeclarationName(),
                       SourceRange OpLoc = SourceRange(),
                       PrecedenceGroupDecl* PG = nullptr)
          : PG(PG), OpName(OpName), OpLoc(OpLoc)
      {
      }

      operator bool() const { return PG != nullptr; }

      PrecedenceGroupDecl* PG;
      DeclarationName OpName;
      SourceRange OpLoc;
      op::OperatorKind OpKind = op::UnknownOp;
   };

   Expression* ParseUnaryExpression();
   Expression* GetUnaryExpressionTarget();

   Expression* ParseExpression(Expression* LHS = nullptr,
                               PrecedenceGroupDecl* MinPrec = nullptr);

   PrecedenceResult getBinOp();
   PrecedenceResult precedenceFollowsImpl(PrecedenceGroupDecl* MinPrec,
                                          bool HigherOrEqual);

   PrecedenceResult higherOrEqualPrecedenceFollows(PrecedenceGroupDecl* MinPrec)
   {
      return precedenceFollowsImpl(MinPrec, true);
   }

   PrecedenceResult higherPrecedenceFollows(PrecedenceGroupDecl* MinPrec)
   {
      return precedenceFollowsImpl(MinPrec, false);
   }

   Expression* buildBinaryOp(PrecedenceResult& Res, Expression* LHS,
                             Expression* RHS);

   Expression* buildCastExpr(PrecedenceResult& Res, Expression* LHS,
                             Expression* RHS);

   Expression* buildMetaTypeUnionExpr(PrecedenceResult& Res,
                                      CandidateSet::Candidate& Cand,
                                      Expression* LHS, Expression* RHS);

   void diagnoseUnexpectedExpr();

   template<class... Args>
   void diagnose(SourceLocation loc, MessageKind msg, Args&&... args)
   {
      if (ignoreErrors)
         return;

      SP.diagnose(loc, msg, std::forward<Args>(args)...);
   }

   QualType contextualType;
   SemaPass& SP;
   llvm::ArrayRef<SequenceElement> Fragments;
   size_t counter;

   std::unordered_map<size_t, std::pair<size_t, Expression*>> ExprSubstMap;

   bool ignoreErrors : 1;
   bool HadError : 1;
   bool TypeDependent : 1;

   ExprSequence* ExprSeq;
};

Expression* ExprResolverImpl::GetUnaryExpressionTarget()
{
   auto& Frag = current();
   switch (Frag.getKind()) {
   case SequenceElement::EF_Expression:
      return Frag.getExpr();
   case SequenceElement::EF_Operator: {
      auto* II = &SP.getContext().getIdentifiers().get(
          op::toString(Frag.getOperatorKind()));

      return new (SP.getContext()) IdentifierRefExpr(Frag.getLoc(), II);
   }
   case SequenceElement::EF_PossibleOperator:
      return new (SP.getContext())
          IdentifierRefExpr(Frag.getLoc(), Frag.getOp());
   }
}

Expression* ExprResolverImpl::ParseUnaryExpression()
{
   auto SubstIt = ExprSubstMap.find(counter);
   if (SubstIt != ExprSubstMap.end()) {
      counter += SubstIt->second.first;
      return SubstIt->second.second;
   }

   if (!hasNext()) {
      return GetUnaryExpressionTarget();
   }

   auto beginIdx = counter;

   // there are only two ways an accessor can appear on the LHS of an
   // assignment:
   //  1. as an unqualified identifier, e.g. `val = 3`
   //  2. as a nested expression, e.g. `func().some.prop = 16`
   // in both scenarios the `=` directly follows a single sequence element,
   // so we can easily detect it here
   bool IsAssignment = lookahead().isOperator()
                       && lookahead().getOperatorKind() == op::Assign;

   Expression* Target = nullptr;
   size_t possibleIdentLoc = string::npos;

   while (true) {
      switch (current().getKind()) {
      case SequenceElement::EF_Expression: {
         Target = GetUnaryExpressionTarget();
         break;
      }
      case SequenceElement::EF_PossibleOperator: {
         auto* Op = current().getOp();
         if (SP.Context.isPrefixOperator(Op) && current().hasLeftWhiteSpace()
             && !current().hasRightWhiteSpace()) {
            if (possibleIdentLoc == string::npos) {
               possibleIdentLoc = counter;
            }

            break;
         }

         SourceLocation Loc = current().getLoc();
         SourceRange SR(Loc, Loc.offsetBy(Op->getLength()));

         Target = new (SP.Context) IdentifierRefExpr(SR, Op);
         break;
      }
      case SequenceElement::EF_Operator: {
         if (!current().hasRightWhiteSpace()) {
            if (possibleIdentLoc == string::npos) {
               possibleIdentLoc = counter;
            }

            break;
         }

         Target = GetUnaryExpressionTarget();
         break;
      }
      }

      if (Target) {
         break;
      }

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
   Target->setIsLHSOfAssignment(IsAssignment);

   auto savedCtr = counter;

   // If we had to skip ahead, all of the previous elements must be prefix
   // operators, evaluate them right to left.
   while (counter != beginIdx) {
      assert(counter > beginIdx && "moved backwards when parsing expression!");
      --counter;

      assert(!current().isExpression() && "skipped over expr!");

      const IdentifierInfo* OpName;
      if (current().isPossibleOperator()) {
         OpName = current().getOp();
      }
      else {
         OpName = &SP.Context.getIdentifiers().get(current().asString());
      }

      if (OpName->isStr("&")) {
         Target = AddrOfExpr::Create(SP.Context, current().getLoc(), Target);
      }
      else {
         SourceLocation Loc = current().getLoc();
         DeclarationName DN
             = SP.Context.getDeclNameTable().getPrefixOperatorName(*OpName);

         SourceRange SR(Loc, Loc.offsetBy(OpName->getLength()));
         auto* Ident = new (SP.Context) IdentifierRefExpr(SR, Target, DN);

         SR = SourceRange(Loc, Target->getSourceRange().getEnd());
         Target = AnonymousCallExpr::Create(SP.Context, SR, Ident, {}, {});
      }
   }

   counter = savedCtr;

   // Check for postfix operators.
   bool done = false;
   while (!done) {
      if (!hasNext()) {
         return Target;
      }

      auto& Next = lookahead();
      if (Next.hasLeftWhiteSpace() || !Next.hasRightWhiteSpace()) {
         break;
      }

      switch (Next.getKind()) {
      case SequenceElement::EF_Expression:
         done = true;
         break;
      case SequenceElement::EF_PossibleOperator:
      case SequenceElement::EF_Operator: {
         const IdentifierInfo* OpName;
         if (Next.isPossibleOperator()) {
            OpName = Next.getOp();
         }
         else {
            OpName = &SP.Context.getIdentifiers().get(Next.asString());
         }

         if (!SP.Context.isPostfixOperator(OpName)) {
            done = true;
            break;
         }

         SourceLocation Loc = Next.getLoc();
         DeclarationName DN
             = SP.Context.getDeclNameTable().getPostfixOperatorName(*OpName);

         SourceRange SR(Loc, Loc.offsetBy(OpName->getLength()));
         auto* Ident = new (SP.Context) IdentifierRefExpr(SR, Target, DN);

         SR = SourceRange(Target->getSourceLoc(), SR.getEnd());
         Target = AnonymousCallExpr::Create(SP.Context, SR, Ident, {}, {});

         advance();
         break;
      }
      }
   }

   return Target;
}

ExprResolverImpl::PrecedenceResult ExprResolverImpl::getBinOp()
{
   if (!hasNext(2))
      return PrecedenceResult();

   auto& Next = lookahead();
   if (Next.isExpression()) {
      return PrecedenceResult();
   }

   op::OperatorKind OpKind = op::UnknownOp;
   auto& DeclNames = SP.getContext().getDeclNameTable();
   if (Next.isOperator()) {
      // Handle conversions before evaluating the left hand side.
      switch (Next.getOperatorKind()) {
      case op::As:
      case op::AsExclaim:
      case op::AsQuestion: {
         auto* II = &SP.getContext().getIdentifiers().get(Next.asString());
         auto OpName = DeclNames.getInfixOperatorName(*II);

         int length = Next.getOperatorKind() == op::As ? 2 : 3;
         PrecedenceResult Result(
             OpName,
             SourceRange(Next.getLoc(), Next.getLoc().offsetBy(length - 1)),
             nullptr);

         Result.OpKind = Next.getOperatorKind();
         if (SP.QC.FindPrecedenceGroup(Result.PG,
                                       SP.getIdentifier("CastingPrecedence"))) {
            llvm_unreachable("no 'CastingPrecedence' decl!");
         }

         return Result;
      }
      default:
         break;
      }

      OpKind = Next.getOperatorKind();
   }

   auto OpStr = Next.asString();
   auto* II = &SP.getContext().getIdentifiers().get(OpStr);

   auto OpName = DeclNames.getInfixOperatorName(*II);
   auto OpDeclName = DeclNames.getOperatorDeclName(OpName);

   OperatorDecl* Decl;
   PrecedenceResult Res;
   SourceRange SR(Next.getLoc(), Next.getLoc().offsetBy(OpStr.size() - 1));

   if (SP.QC.FindOperator(Decl, OpDeclName, false) || !Decl) {
      Res = PrecedenceResult(OpName, SR);
   }
   else {
      Res = PrecedenceResult(OpName, SR, Decl->getPrecedenceGroup());
   }

   Res.OpKind = OpKind;
   return Res;
}

static CastStrength getCastStrength(op::OperatorKind Kind)
{
   switch (Kind) {
   case op::As:
      return CastStrength::Normal;
   case op::AsExclaim:
      return CastStrength::Force;
   case op::AsQuestion:
      return CastStrength::Fallible;
   default:
      llvm_unreachable("not a conversion operator!");
   }
}

Expression* ExprResolverImpl::buildCastExpr(PrecedenceResult& Res,
                                            Expression* LHS, Expression* RHS)
{
   return CastExpr::Create(SP.getContext(), Res.OpLoc.getStart(),
                           getCastStrength(Res.OpKind), LHS, SourceType(RHS));
}

Expression*
ExprResolverImpl::buildMetaTypeUnionExpr(PrecedenceResult& Res,
                                         CandidateSet::Candidate& Cand,
                                         Expression* LHS, Expression* RHS)
{
   SourceType Types[]{SourceType(LHS->getExprType()->removeMetaType()),
                      SourceType(RHS->getExprType()->removeMetaType())};

   SourceRange SR(LHS->getSourceLoc(), RHS->getSourceRange().getEnd());
   return ExistentialTypeExpr::Create(SP.Context, SR, Types, true);
}

Expression* ExprResolverImpl::buildBinaryOp(PrecedenceResult& Res,
                                            Expression* LHS, Expression* RHS)
{
   // Handle builtin operators that can't be overloaded.
   switch (Res.OpKind) {
   case op::As:
   case op::AsExclaim:
   case op::AsQuestion: {
      return buildCastExpr(Res, LHS, RHS);
   }
   case op::Assign: {
      return AssignExpr::Create(SP.getContext(), Res.OpLoc.getStart(), LHS,
                                RHS);
   }
   default: {
      auto* Ident
          = new (SP.Context) IdentifierRefExpr(Res.OpLoc, LHS, Res.OpName);

      SourceRange SR(LHS->getSourceLoc(), RHS->getSourceRange().getEnd());
      return AnonymousCallExpr::Create(SP.Context, SR, Ident, {RHS}, {});
   }
   }
}

ExprResolverImpl::PrecedenceResult
ExprResolverImpl::precedenceFollowsImpl(PrecedenceGroupDecl* MinPrec,
                                        bool HigherOrEqual)
{
   auto NextPrec = getBinOp();
   if (!NextPrec || !MinPrec) {
      return NextPrec;
   }

   auto Rel = NextPrec.PG->getRelationTo(MinPrec);
   switch (Rel) {
   case PrecedenceGroupDecl::Undefined:
      SP.diagnose(ExprSeq, err_undefined_precedence, NextPrec.PG->getDeclName(),
                  MinPrec->getDeclName(), ExprSeq->getSourceRange());

      return PrecedenceResult();
   case PrecedenceGroupDecl::Equal: {
      if (!HigherOrEqual) {
         return PrecedenceResult(NextPrec.OpName, NextPrec.OpLoc);
      }

      LLVM_FALLTHROUGH;
   }
   case PrecedenceGroupDecl::Higher:
      return NextPrec;
   case PrecedenceGroupDecl::Lower:
      return PrecedenceResult(NextPrec.OpName, NextPrec.OpLoc);
   }
}

Expression* ExprResolverImpl::ParseExpression(Expression* LHS,
                                              PrecedenceGroupDecl* MinPrec)
{
   if (!LHS) {
      advance();

      LHS = ParseUnaryExpression();
      if (!LHS) {
         return nullptr;
      }
   }

   while (auto NextPrec = higherOrEqualPrecedenceFollows(MinPrec)) {
      // skip over the operator
      advance();
      advance();

      // get the right hand side
      auto RHS = ParseUnaryExpression();
      if (!RHS) {
         return nullptr;
      }

      // Check if a higher precedence operator follows.
      while (auto FollowingPrec = higherPrecedenceFollows(NextPrec.PG)) {
         RHS = ParseExpression(RHS, FollowingPrec.PG);
         if (!RHS)
            return nullptr;
      }

      // Build the resulting binary operator expression.
      auto binOp = buildBinaryOp(NextPrec, LHS, RHS);
      if (!binOp) {
         return nullptr;
      }

      LHS = binOp;
   }

   // can only be meant to be a postfix unary operator, but was not parsed as
   // one (i.e. is invalid))
   if (!MinPrec && !fragmentsLeft(0)) {
      if (fragmentsLeft(1)) {
         if (HadError || TypeDependent)
            return nullptr;

         diagnoseUnexpectedExpr();
         return nullptr;
      }

      HadError = true;

      if (lookahead().isExpression()) {
         diagnoseUnexpectedExpr();
      }
      else {
         SP.diagnose(ExprSeq, err_generic_error, lookahead().getLoc(),
                     lookahead().asString() + " is not an operator");
      }

      return nullptr;
   }

   assert(!HadError && !TypeDependent && "should have returned before!");
   return LHS;
}

void ExprResolverImpl::diagnoseUnexpectedExpr()
{
   SP.diagnose(ExprSeq, err_unexpected_expression, lookahead().getLoc());
}

static void visitContextDependentExpr(SemaPass& SP, Expression* E)
{
   switch (E->getTypeID()) {
   case Statement::EnumCaseExprID: {
      auto* EC = cast<EnumCaseExpr>(E);
      EC->setIsTypeDependent(true);

      for (auto* Arg : EC->getArgs()) {
         auto Result = SP.visitExpr(EC, Arg);
         if (Result) {
            Arg = Result.get();
         }
      }

      break;
   }
   case Statement::CallExprID: {
      auto* CE = cast<CallExpr>(E);
      CE->setIsTypeDependent(true);

      for (auto& Arg : CE->getArgs()) {
         auto Result = SP.visitExpr(CE, Arg);
         if (Result) {
            Arg = Result.get();
         }
      }

      break;
   }
   case Statement::LambdaExprID: {
      auto* LE = cast<LambdaExpr>(E);
      LE->setIsTypeDependent(true);

      auto Result = SP.visitStmt(LE, LE->getBody());
      if (Result) {
         LE->setBody(Result.get());
      }

      break;
   }
   default:
      break;
   }
}

Expression* ExpressionResolver::resolve(ExprSequence* ExprSeq,
                                        bool ignoreErrors)
{
   ExprResolverImpl Resolver(SP, ExprSeq, ExprSeq->getContextualType(),
                             ignoreErrors);

   auto Result = Resolver.ParseExpression();

   // Visit all expressions so they're ready for instantiation.
   if (Resolver.TypeDependent) {
      ExprSeq->setIsTypeDependent(true);

      for (auto& SeqEl : ExprSeq->getFragments()) {
         if (SeqEl.isExpression()) {
            auto E = SeqEl.getExpr();
            if (E->isContextDependent()) {
               visitContextDependentExpr(SP, E);
            }
            else {
               E->setIsTypeDependent(true);

               auto Res = SP.visitExpr(ExprSeq, E);
               if (Res)
                  SeqEl.setExpr(Res.get());
            }
         }
      }
   }
   else {
      ExprSeq->setIsInvalid(!Result || ExprSeq->isInvalid()
                            || Resolver.HadError);
   }

   return Result;
}

} // namespace ast
} // namespace cdot