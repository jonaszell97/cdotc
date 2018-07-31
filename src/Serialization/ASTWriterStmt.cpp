//
// Created by Jonas Zell on 27.05.18.
//

#include "ASTReaderInternals.h"
#include "ASTWriter.h"
#include "AST/ASTVisitor.h"
#include "ModuleWriter.h"
#include "Sema/ConversionSequence.h"
#include "Sema/Scope/Scope.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::serial;
using namespace cdot::support;

namespace {

class ASTStmtWriter: public ASTVisitor<ASTStmtWriter> {
   ASTWriter &Writer;
   ASTRecordWriter Record;

   Statement::NodeType Code;
   unsigned AbbrevToUse;

public:
   ASTStmtWriter(ASTWriter &Writer, ASTWriter::RecordData &Record)
      : Writer(Writer), Record(Writer, Record),
        Code((Statement::NodeType)(0)), AbbrevToUse(0)
   {
      (void)this->Writer;
   }

   ASTStmtWriter(const ASTStmtWriter&) = delete;

   uint64_t Emit()
   {
      return Record.EmitStmt(static_cast<unsigned>(Code), AbbrevToUse);
   }
   
   void visit(Statement *S);

   void visitStmt(Statement *S);
   void visitExpr(Expression *E);

   void visitPatternExpr(PatternExpr *E);
   void visitTypeExpr(TypeExpr *E);

#  define CDOT_STMT(NAME) void visit##NAME(NAME *S);
#  include "AST/AstNode.def"
};

} // anonymous namespace

void ASTStmtWriter::visit(Statement *S)
{
   ASTVisitor::visit(S);
   Code = S->getTypeID();
}

void ASTStmtWriter::visitStmt(Statement *S)
{
   Record.push_back(S->getSubclassData());
}

void ASTStmtWriter::visitExpr(Expression *E)
{
   visitStmt(E);

   Record.AddTypeRef(E->getExprType());
   Record.AddSourceLocation(E->getEllipsisLoc());

   uint8_t Flags = 0;
   Flags |= E->isLHSOfAssignment();
   Flags |= (E->allowTemplate() << 2);
   Flags |= (E->isMagicArgumentValue() << 3);

   Record.push_back(Flags);
}

void ASTStmtWriter::visitCompoundStmt(CompoundStmt *S)
{
   visitStmt(S);

   Record.push_back(S->size());
   for (auto Stmt : S->getStatements())
      Record.AddStmt(Stmt);

   Record.AddSourceLocation(S->getLBraceLoc());
   Record.AddSourceLocation(S->getRBraceLoc());

   Record.push_back(S->preservesScope());
   Record.push_back(S->isUnsafe());
   Record.push_back(S->containsDeclStmt());
   Record.push_back(S->getScopeID());
}

void ASTStmtWriter::visitDeclStmt(DeclStmt *S)
{
   visitStmt(S);

   Record.AddDeclRef(S->getDecl());
}

void ASTStmtWriter::visitAttributedStmt(AttributedStmt *S)
{
   visitStmt(S);

   Record.AddAttributes(S->getAttributes());
   Record.AddStmt(S->getStatement());
}

void ASTStmtWriter::visitBreakStmt(BreakStmt *S)
{
   visitStmt(S);

   Record.AddSourceLocation(S->getSourceLoc());
   Record.AddIdentifierRef(S->getLabel());
}

void ASTStmtWriter::visitContinueStmt(ContinueStmt *S)
{
   visitStmt(S);

   Record.AddSourceLocation(S->getSourceLoc());
   Record.AddIdentifierRef(S->getLabel());
}

void ASTStmtWriter::visitReturnStmt(ReturnStmt *S)
{
   visitStmt(S);

   Record.AddStmt(S->getReturnValue());
   Record.AddSourceLocation(S->getSourceLoc());
}

void ASTStmtWriter::visitDiscardAssignStmt(DiscardAssignStmt *S)
{
   visitStmt(S);

   Record.AddSourceLocation(S->getUnderscoreLoc());
   Record.AddSourceLocation(S->getEqualsLoc());
   Record.AddStmt(S->getRHS());
}

void ASTStmtWriter::visitCaseStmt(CaseStmt *S)
{
   visitStmt(S);

   Record.AddSourceLocation(S->getSourceLoc());
   Record.AddStmt(S->getPattern());
   Record.AddStmt(S->getBody());
}

void ASTStmtWriter::visitForStmt(ForStmt *S)
{
   visitStmt(S);

   Record.AddSourceLocation(S->getSourceLoc());
   Record.AddStmt(S->getInitialization());
   Record.AddStmt(S->getTermination());
   Record.AddStmt(S->getIncrement());
   Record.AddStmt(S->getBody());
   Record.AddIdentifierRef(S->getLabel());
}

void ASTStmtWriter::visitIfStmt(IfStmt *S)
{
   visitStmt(S);

   Record.AddSourceLocation(S->getSourceLoc());
   Record.AddStmt(S->getCondition());
   Record.AddStmt(S->getIfBranch());
   Record.AddStmt(S->getElseBranch());
   Record.AddIdentifierRef(S->getLabel());
}

void ASTStmtWriter::visitIfLetStmt(IfLetStmt *S)
{
   visitStmt(S);

   Record.AddSourceLocation(S->getSourceLoc());
   Record.AddDeclRef(S->getVarDecl());
   Record.AddStmt(S->getIfBranch());
   Record.AddStmt(S->getElseBranch());
}

void ASTStmtWriter::visitIfCaseStmt(IfCaseStmt *S)
{
   visitStmt(S);

   Record.AddSourceLocation(S->getSourceLoc());
   Record.AddStmt(S->getPattern());
   Record.AddStmt(S->getIfBranch());
   Record.AddStmt(S->getElseBranch());
}

void ASTStmtWriter::visitWhileStmt(WhileStmt *S)
{
   visitStmt(S);

   Record.AddSourceLocation(S->getSourceLoc());
   Record.AddStmt(S->getCondition());
   Record.AddStmt(S->getBody());
   Record.push_back(S->isAtLeastOnce());
   Record.AddIdentifierRef(S->getLabel());
}

void ASTStmtWriter::visitForInStmt(ForInStmt *S)
{
   visitStmt(S);

   Record.AddSourceLocation(S->getSourceLoc());
   Record.AddDeclRef(S->getDecl());
   Record.AddStmt(S->getRangeExpr());
   Record.AddStmt(S->getBody());

   Record.AddDeclRef(S->getGetIteratorFn());
   Record.AddDeclRef(S->getNextFn());
   Record.AddIdentifierRef(S->getLabel());
}

void ASTStmtWriter::visitMatchStmt(MatchStmt *S)
{
   visitStmt(S);

   Record.push_back(S->getCases().size());
   for (auto C : S->getCases())
      Record.AddStmt(C);

   Record.AddSourceLocation(S->getMatchLoc());
   Record.AddSourceRange(S->getBraceRange());
   Record.AddStmt(S->getSwitchValue());
   Record.push_back(S->hasMutableCaseArg());
   Record.AddIdentifierRef(S->getLabel());
}

static void WriteCatchBlock(ASTRecordWriter &Record, CatchBlock &CB)
{
   Record.AddDeclRef(CB.varDecl);
   Record.AddStmt(CB.Body);
   Record.AddStmt(CB.Condition);
}

void ASTStmtWriter::visitDoStmt(DoStmt *S)
{
   visitStmt(S);

   Record.push_back(S->getCatchBlocks().size());
   for (auto &CB : S->getCatchBlocks())
      WriteCatchBlock(Record, CB);

   Record.AddSourceRange(S->getSourceRange());
   Record.AddStmt(S->getBody());
   Record.AddIdentifierRef(S->getLabel());
}

void ASTStmtWriter::visitThrowStmt(ThrowStmt *S)
{
   visitStmt(S);

   Record.AddSourceLocation(S->getSourceLoc());
   Record.AddStmt(S->getThrownVal());
}

void ASTStmtWriter::visitDebugStmt(DebugStmt *S)
{
   visitStmt(S);

   Record.AddSourceLocation(S->getSourceLoc());
}

void ASTStmtWriter::visitNullStmt(NullStmt *S)
{
   visitStmt(S);

   Record.AddSourceLocation(S->getSourceLoc());
}

void ASTStmtWriter::visitMacroExpansionStmt(MacroExpansionStmt *S)
{
   visitStmt(S);

   Record.push_back(S->getTokens().size());
   for (auto &Tok : S->getTokens())
      Record.AddToken(Tok);

   Record.AddSourceRange(S->getSourceRange());
   Record.AddDeclarationName(S->getMacroName());
   Record.push_back(S->getDelim());
   Record.AddStmt(S->getParentExpr());
}

void ASTStmtWriter::visitStaticIfStmt(StaticIfStmt *S)
{
   visitStmt(S);

   Record.AddSourceLocation(S->getStaticLoc());
   Record.AddSourceLocation(S->getIfLoc());
   Record.AddStmt(S->getCondition());
   Record.AddStmt(S->getIfBranch());
   Record.AddStmt(S->getElseBranch());
}

void ASTStmtWriter::visitStaticForStmt(StaticForStmt *S)
{
   visitStmt(S);

   Record.AddSourceLocation(S->getStaticLoc());
   Record.AddSourceLocation(S->getForLoc());
   Record.AddIdentifierRef(S->getElementName());
   Record.AddStmt(S->getRange());
   Record.AddStmt(S->getBody());
}

void ASTStmtWriter::visitMixinStmt(MixinStmt *S)
{
   visitStmt(S);

   Record.AddSourceRange(S->getSourceRange());
   Record.AddStmt(S->getMixinExpr());
}

void ASTStmtWriter::visitParenExpr(ParenExpr *S)
{
   visitExpr(S);

   Record.AddSourceRange(S->getSourceRange());
   Record.AddStmt(S->getParenthesizedExpr());
}

void ASTStmtWriter::visitAttributedExpr(AttributedExpr *S)
{
   visitExpr(S);

   Record.AddAttributes(S->getAttributes());
   Record.AddStmt(S->getExpr());
}

void ASTStmtWriter::visitIntegerLiteral(IntegerLiteral *S)
{
   visitExpr(S);

   Record.AddSourceRange(S->getSourceRange());
   Record.AddTypeRef(S->getType());
   Record.AddAPSInt(S->getValue());
   Record.push_back(static_cast<unsigned char>(S->getSuffix()));
   Record.AddDeclRef(S->getExpressibleByInit());
}

void ASTStmtWriter::visitFPLiteral(FPLiteral *S)
{
   visitExpr(S);

   Record.AddSourceRange(S->getSourceRange());
   Record.AddTypeRef(S->getType());
   Record.AddAPFloat(S->getValue());
   Record.push_back(static_cast<unsigned char>(S->getSuffix()));
   Record.AddDeclRef(S->getExpressibleByInit());
}

void ASTStmtWriter::visitBoolLiteral(BoolLiteral *S)
{
   visitExpr(S);

   Record.AddSourceLocation(S->getSourceLoc());
   Record.AddTypeRef(S->getType());
   Record.push_back(S->getValue());
   Record.AddDeclRef(S->getExpressibleByInit());
}

void ASTStmtWriter::visitCharLiteral(CharLiteral *S)
{
   visitExpr(S);

   Record.AddSourceRange(S->getSourceRange());
   Record.AddTypeRef(S->getType());
   Record.push_back(S->getWide());
   Record.AddDeclRef(S->getExpressibleByInit());
}

void ASTStmtWriter::visitNoneLiteral(NoneLiteral *S)
{
   visitExpr(S);

   Record.AddSourceLocation(S->getSourceLoc());
}

void ASTStmtWriter::visitStringLiteral(StringLiteral *S)
{
   visitExpr(S);

   Record.AddSourceRange(S->getSourceRange());
   Record.AddString(S->getValue());
   Record.AddDeclRef(S->getExpressibleByInit());
}

void ASTStmtWriter::visitStringInterpolation(StringInterpolation *S)
{
   visitExpr(S);

   auto Segs = S->getSegments();
   Record.push_back(Segs.size());

   for (auto &Seg : Segs)
      Record.AddStmt(Seg);

   Record.AddSourceRange(S->getSourceRange());
}

void ASTStmtWriter::visitLambdaExpr(LambdaExpr *S)
{
   visitExpr(S);

   auto Args = S->getArgs();
   Record.push_back(Args.size());

   for (auto Arg : Args)
      Record.AddDeclRef(Arg);

   auto Captures = S->getCaptures();
   Record.push_back(Captures.size());

   for (auto Capt : Captures)
      Record.AddDeclRef(Capt);

   Record.AddDeclRef(S->getFunc());
   Record.AddSourceRange(S->getParenRange());
   Record.AddSourceLocation(S->getArrowLoc());

   Record.AddTypeRef(S->getReturnType());
   Record.AddStmt(S->getBody());
}

void ASTStmtWriter::visitDictionaryLiteral(DictionaryLiteral *S)
{
   visitExpr(S);

   auto Keys = S->getKeys();
   Record.push_back(Keys.size());

   for (auto Key : Keys)
      Record.AddStmt(Key);

   auto Values = S->getValues();
   Record.push_back(Values.size());

   for (auto Val : Values)
      Record.AddStmt(Val);

   Record.AddSourceRange(S->getSourceRange());
   Record.AddDeclRef(S->getInsertFn());
   Record.AddDeclRef(S->getExpressibleByInit());
}

void ASTStmtWriter::visitArrayLiteral(ArrayLiteral *S)
{
   visitExpr(S);

   auto Values = S->getValues();
   Record.push_back(Values.size());

   for (auto Val : Values)
      Record.AddStmt(Val);

   Record.AddSourceRange(S->getSourceRange());
   Record.AddDeclRef(S->getExpressibleByInit());
}

void ASTStmtWriter::visitTupleLiteral(TupleLiteral *S)
{
   visitExpr(S);

   auto Values = S->getElements();
   Record.push_back(Values.size());

   for (auto Val : Values)
      Record.AddStmt(Val);

   Record.AddSourceRange(S->getSourceRange());
}

void ASTStmtWriter::visitIdentifierRefExpr(IdentifierRefExpr *S)
{
   visitExpr(S);

   Record.AddSourceRange(S->getSourceRange());
   Record.AddDeclarationName(S->getDeclName());
   Record.push_back(static_cast<uintptr_t>(S->getKind()));

   if (S->getKind() != IdentifierKind::MetaType) {
      Record.AddDeclRef(S->getNamedDecl());
   }

   uint64_t Flags = 0;
   Flags |= S->isStaticLookup();
   Flags |= (S->isPointerAccess() << 1);
   Flags |= (S->foundResult() << 2);
   Flags |= (S->isInTypePosition() << 3);
   Flags |= (S->isSynthesized() << 4);
   Flags |= (S->isCapture() << 5);
   Flags |= (S->isSelf() << 6);
   Flags |= (S->allowIncompleteTemplateArgs() << 7);
   Flags |= (S->allowNamespaceRef() << 8);
   Flags |= (S->hasLeadingDot() << 8);

   Record.push_back(Flags);
   Record.push_back(S->getCaptureIndex());
   Record.AddStmt(S->getParentExpr());
   Record.AddDeclRef(support::cast_or_null<Decl>(S->getDeclCtx()));
}

void ASTStmtWriter::visitEnumCaseExpr(EnumCaseExpr *S)
{
   visitExpr(S);

   Record.AddSourceLocation(S->getSourceLoc());
   Record.AddDeclarationName(S->getDeclName());

   auto &Args = S->getArgs();
   Record.push_back(Args.size());

   for (auto Arg : Args)
      Record.AddStmt(Arg);

   Record.AddDeclRef(S->getCase());
}

void ASTStmtWriter::visitCallExpr(CallExpr *S)
{
   visitExpr(S);

   auto Labels = S->getLabels();
   Record.push_back(Labels.size());

   for (auto &Label : Labels)
      Record.AddIdentifierRef(Label);

   Record.AddSourceLocation(S->getIdentLoc());
   Record.AddSourceRange(S->getParenRange());

   Record.AddDeclarationName(S->getDeclName());
   Record.push_back(static_cast<uintptr_t>(S->getKind()));

   auto &Args = S->getArgs();
   Record.push_back(Args.size());

   for (auto &Arg : Args)
      Record.AddStmt(Arg);

   Record.push_back(S->isPointerAccess());
   Record.push_back(S->isUFCS());
   Record.push_back(S->isDotInit());
   Record.push_back(S->isDotDeinit());
   Record.push_back(S->includesSelf());
   Record.push_back(S->isDirectCall());
   Record.push_back(S->getBuiltinKind());

   Record.AddDeclRef(S->getFunc());
   Record.AddStmt(S->getParentExpr());
   Record.AddDeclRef(support::cast_or_null<Decl>(S->getContext()));
}

void ASTStmtWriter::visitAnonymousCallExpr(AnonymousCallExpr *S)
{
   visitExpr(S);

   auto Args = S->getArgs();
   Record.push_back(Args.size());

   for (auto Arg : Args)
      Record.AddStmt(Arg);

   auto Labels = S->getLabels();
   Record.push_back(Labels.size());

   for (auto &Label : Labels)
      Record.AddIdentifierRef(Label);

   Record.AddSourceRange(S->getParenRange());
   Record.push_back(S->isPrimitiveInit());
   Record.AddTypeRef(S->getFunctionType());

   Record.AddStmt(S->getParentExpr());
}

void ASTStmtWriter::visitSubscriptExpr(SubscriptExpr *S)
{
   visitExpr(S);

   auto Args = S->getIndices();
   Record.push_back(Args.size());

   for (auto Arg : Args)
      Record.AddStmt(Arg);

   Record.AddSourceRange(S->getSourceRange());
   Record.AddStmt(S->getParentExpr());
}

void ASTStmtWriter::visitTemplateArgListExpr(TemplateArgListExpr *S)
{
   visitExpr(S);

   auto Args = S->getExprs();
   Record.push_back(Args.size());

   for (auto Arg : Args)
      Record.AddStmt(Arg);

   Record.AddSourceRange(S->getAngleRange());
   Record.AddStmt(S->getParentExpr());
}

void ASTStmtWriter::visitBuiltinExpr(BuiltinExpr *S)
{
   visitExpr(S);
}

void ASTStmtWriter::visitTupleMemberExpr(TupleMemberExpr *S)
{
   visitExpr(S);

   Record.AddSourceLocation(S->getSourceLoc());
   Record.push_back(S->getIndex());
   Record.push_back(S->isPointerAccess());

   Record.AddStmt(S->getParentExpr());
}

void ASTStmtWriter::visitSelfExpr(SelfExpr *S)
{
   visitExpr(S);

   Record.AddSourceLocation(S->getSourceLoc());
   Record.AddDeclRef(S->getSelfArg());
   Record.push_back(S->getCaptureIndex());
   Record.push_back(S->isUppercase());
}

void ASTStmtWriter::visitSuperExpr(SuperExpr *S)
{
   visitExpr(S);

   Record.AddSourceLocation(S->getSourceLoc());
   Record.AddDeclRef(S->getSelfArg());
   Record.push_back(S->getCaptureIndex());
}

void ASTStmtWriter::visitBuiltinIdentExpr(BuiltinIdentExpr *S)
{
   visitExpr(S);

   Record.AddSourceLocation(S->getSourceLoc());
   Record.push_back(static_cast<uint64_t>(S->getIdentifierKind()));
}

void ASTStmtWriter::visitPatternExpr(PatternExpr *S)
{
   visitExpr(S);
   Record.AddSourceLocation(S->getColonLoc());
}

void ASTStmtWriter::visitExpressionPattern(ExpressionPattern *S)
{
   visitPatternExpr(S);
   Record.AddStmt(S->getExpr());
}

static void WriteCasePatternArg(ASTRecordWriter &Record,
                                const CasePatternArgument &Arg) {
   Record.push_back(Arg.isExpr());

   if (Arg.isExpr()) {
      Record.AddStmt(Arg.getExpr());
   }
   else {
      Record.AddDeclRef(Arg.getDecl());
   }
}

void ASTStmtWriter::visitCasePattern(CasePattern *S)
{
   visitExpr(S);

   auto Args = S->getArgs();
   Record.push_back(Args.size());

   for (auto &Arg : Args)
      WriteCasePatternArg(Record, Arg);

   Record.AddSourceLocation(S->getColonLoc());
   Record.AddSourceRange(S->getSourceRange());
   Record.AddIdentifierRef(S->getCaseNameIdent());
   Record.push_back(S->hasBinding());
   Record.push_back(S->hasExpr());

   Record.AddDeclRef(S->getCaseDecl());

   auto VarDecls = S->getVarDecls();
   Record.push_back(VarDecls.size());

   for (auto &D : VarDecls)
      Record.AddDeclRef(D);
}

void ASTStmtWriter::visitIsPattern(IsPattern *S)
{
   visitPatternExpr(S);

   Record.push_back(S->getIsLoc());
   Record.AddTypeRef(S->getIsType());
}

static void WriteSequenceElement(ASTRecordWriter &Record,
                                 const SequenceElement &El) {
   Record.AddSourceLocation(El.getLoc());
   Record.push_back(El.getKind());

   switch (El.getKind()) {
   case SequenceElement::EF_Expression:
      Record.AddStmt(El.getExpr());
      break;
   case SequenceElement::EF_PossibleOperator:
      Record.AddIdentifierRef(El.getOp());
      break;
   case SequenceElement::EF_Operator:
      Record.push_back(El.getOperatorKind());
      break;
   }
}

void ASTStmtWriter::visitExprSequence(ExprSequence *S)
{
   visitExpr(S);

   auto Elements = S->getFragments();
   Record.push_back(Elements.size());

   for (auto &El : Elements)
      WriteSequenceElement(Record, El);
}

void ASTStmtWriter::visitUnaryOperator(UnaryOperator *S)
{
   visitExpr(S);

   Record.AddSourceLocation(S->getOperatorLoc());
   Record.push_back(S->getKind());
   Record.AddTypeRef(S->getFunctionType());
   Record.AddStmt(S->getTarget());
   Record.push_back(S->isPrefix());
}

void ASTStmtWriter::visitBinaryOperator(BinaryOperator *S)
{
   visitExpr(S);

   Record.AddSourceLocation(S->getOperatorLoc());
   Record.push_back(S->getKind());
   Record.AddTypeRef(S->getFunctionType());
   Record.AddStmt(S->getLhs());
   Record.AddStmt(S->getRhs());
}

void ASTStmtWriter::visitAssignExpr(AssignExpr *S)
{
   visitExpr(S);

   Record.AddSourceLocation(S->getEqualsLoc());
   Record.AddStmt(S->getLhs());
   Record.AddStmt(S->getRhs());
   Record.push_back(S->isInitialization());
}

static void WriteConvSeq(ASTRecordWriter &Record,
                         const ConversionSequence &Seq) {
   Record.push_back(Seq.getStrength());
   Record.push_back(Seq.getSteps().size());

   for (auto &Step : Seq.getSteps()) {
      Record.push_back(Step.isHalt());
      if (Step.isHalt())
         continue;

      Record.push_back(static_cast<uint64_t>(Step.getKind()));

      if (Step.getKind() == CastKind::ConversionOp) {
         Record.AddDeclRef(Step.getConversionOp());
      }
      else {
         Record.AddTypeRef(Step.getResultType());
      }
   }
}

void ASTStmtWriter::visitCastExpr(CastExpr *S)
{
   visitExpr(S);

   Record.AddSourceLocation(S->getAsLoc());
   Record.push_back(S->getStrength());
   Record.AddStmt(S->getTarget());
   Record.AddTypeRef(S->getTargetType());
   WriteConvSeq(Record, S->getConvSeq());
}

void ASTStmtWriter::visitTypePredicateExpr(TypePredicateExpr *S)
{
   visitExpr(S);

   Record.AddSourceLocation(S->getIsLoc());
   Record.AddSourceRange(S->getSourceRange());
   Record.AddStmt(S->getLHS());
   Record.AddStmt(S->getRHS());

   uint8_t Flags = 0;
   Flags |= S->getResult();
   Flags |= (S->isCompileTimeCheck() << 1);
   Flags |= (S->isNegated() << 2);

   Record.push_back(Flags);
}

void ASTStmtWriter::visitIfExpr(IfExpr *S)
{
   visitExpr(S);

   Record.AddSourceLocation(S->getSourceLoc());
   Record.AddStmt(S->getCond());
   Record.AddStmt(S->getTrueVal());
   Record.AddStmt(S->getFalseVal());
}

void ASTStmtWriter::visitTryExpr(TryExpr *S)
{
   visitExpr(S);

   Record.AddSourceLocation(S->getSourceLoc());
   Record.push_back(S->getKind());
   Record.AddStmt(S->getExpr());
}

void ASTStmtWriter::visitAwaitExpr(AwaitExpr *S)
{
   visitExpr(S);

   Record.AddSourceLocation(S->getAwaitLoc());
   Record.AddStmt(S->getExpr());
   Record.push_back(S->isImmediateReturn());
}

void ASTStmtWriter::visitImplicitCastExpr(ImplicitCastExpr *S)
{
   visitExpr(S);

   Record.AddStmt(S->getTarget());
   WriteConvSeq(Record, S->getConvSeq());
}

void ASTStmtWriter::visitStaticExpr(StaticExpr *S)
{
   visitExpr(S);

   Record.AddSourceRange(S->getSourceRange());
   Record.AddStmt(S->getExpr());
   Record.AddILConstant(S->getEvaluatedExpr());
}

void ASTStmtWriter::visitConstraintExpr(ConstraintExpr *S)
{
   visitExpr(S);

   Record.AddSourceLocation(S->getSourceLoc());
   Record.push_back(S->getKind());

   if (S->getKind() == ConstraintExpr::Type) {
      Record.AddTypeRef(S->getTypeConstraint());
   }
}

static void WriteTraitsArgument(ASTRecordWriter &Record,
                                const TraitsArgument &Arg) {
   Record.push_back(Arg.getKind());
   switch (Arg.getKind()) {
   case TraitsArgument::Type:
      Record.AddTypeRef(Arg.getType());
      break;
   case TraitsArgument::Expr:
      Record.AddStmt(Arg.getExpr());
      break;
   case TraitsArgument::Stmt:
      Record.AddStmt(Arg.getStmt());
      break;
   case TraitsArgument::String:
      Record.AddString(Arg.getStr());
      break;
   }
}

void ASTStmtWriter::visitTraitsExpr(TraitsExpr *S)
{
   visitExpr(S);

   auto Args = S->getArgs();
   Record.push_back(Args.size());

   for (auto &Arg : Args)
      WriteTraitsArgument(Record, Arg);

   Record.AddSourceLocation(S->getTraitsLoc());
   Record.AddSourceRange(S->getParenRange());
   Record.push_back(S->getKind());
}

void ASTStmtWriter::visitMixinExpr(MixinExpr *S)
{
   visitExpr(S);

   Record.AddSourceRange(S->getSourceRange());
   Record.AddStmt(S->getMixinExpr());
}

void ASTStmtWriter::visitMacroVariableExpr(MacroVariableExpr *S)
{
   visitExpr(S);
   Record.AddStmt(S->getExpr());
}

void ASTStmtWriter::visitMacroExpansionExpr(MacroExpansionExpr *S)
{
   visitExpr(S);

   Record.push_back(S->getTokens().size());
   for (auto &Tok : S->getTokens())
      Record.AddToken(Tok);

   Record.AddSourceRange(S->getSourceRange());
   Record.AddDeclarationName(S->getMacroName());
   Record.push_back(S->getDelim());
}

void ASTStmtWriter::visitTypeExpr(TypeExpr *E)
{
   visitExpr(E);

   Record.AddSourceRange(E->getSourceRange());
   Record.push_back(E->isMeta());
}

void ASTStmtWriter::visitTupleTypeExpr(TupleTypeExpr *S)
{
   visitTypeExpr(S);

   auto Tys = S->getContainedTypes();
   Record.push_back(Tys.size());

   for (auto &Ty : Tys)
      Record.AddTypeRef(Ty);
}

void ASTStmtWriter::visitArrayTypeExpr(ArrayTypeExpr *S)
{
   visitTypeExpr(S);

   Record.AddTypeRef(S->getElementTy());
   Record.AddStmt(S->getSizeExpr());
}

void ASTStmtWriter::visitFunctionTypeExpr(FunctionTypeExpr *S)
{
   visitTypeExpr(S);

   auto Tys = S->getArgTypes();
   Record.push_back(Tys.size());

   for (auto &Ty : Tys)
      Record.AddTypeRef(Ty);

   Record.AddTypeRef(S->getReturnType());

   Record.push_back(S->isThin());
   Record.push_back(S->throws());
   Record.push_back(S->isUnsafe());
   Record.push_back(S->isAsync());
}

void ASTStmtWriter::visitDeclTypeExpr(DeclTypeExpr *S)
{
   visitTypeExpr(S);
   Record.AddStmt(S->getTyExpr());
}

void ASTStmtWriter::visitPointerTypeExpr(PointerTypeExpr *S)
{
   visitTypeExpr(S);
   Record.AddTypeRef(S->getSubType());
}

void ASTStmtWriter::visitReferenceTypeExpr(ReferenceTypeExpr *S)
{
   visitTypeExpr(S);
   Record.AddTypeRef(S->getSubType());
}

void ASTStmtWriter::visitOptionTypeExpr(OptionTypeExpr *S)
{
   visitTypeExpr(S);
   Record.AddTypeRef(S->getSubType());
}

void ASTWriter::WriteSubStmt(Statement *S)
{
   RecordData Record;
   ASTStmtWriter Writer(*this, Record);
   ++this->Writer.NumStatements;

   if (!S) {
      Stream.EmitRecord(STMT_NULL_PTR, Record);
      return;
   }

   Writer.visit(S);

   uint64_t Offset = Writer.Emit();
   SubStmtEntries[S] = Offset;
}

void ASTRecordWriter::FlushStmts()
{
   // We expect to be the only consumer of the two temporary statement maps,
   // assert that they are empty.

   for (unsigned I = 0, N = StmtsToEmit.size(); I != N; ++I) {
      Writer->WriteSubStmt(StmtsToEmit[I]);

      assert(N == StmtsToEmit.size() && "record modified while being written!");

      // Note that we are at the end of a full expression. Any
      // expression records that follow this one are part of a different
      // expression.
      Writer->Stream.EmitRecord(STMT_STOP, llvm::ArrayRef<uint32_t>());
   }

   StmtsToEmit.clear();
}

void ASTRecordWriter::FlushSubStmts()
{
   // For a nested statement, write out the substatements in reverse order (so
   // that a simple stack machine can be used when loading), and don't emit a
   // STMT_STOP after each one.
   for (unsigned I = 0, N = StmtsToEmit.size(); I != N; ++I) {
      Writer->WriteSubStmt(StmtsToEmit[N - I - 1]);
      assert(N == StmtsToEmit.size() && "record modified while being written!");
   }

   StmtsToEmit.clear();
}