//
// Created by Jonas Zell on 28.05.18.
//

#include "BitCodes.h"
#include "ASTReader.h"
#include "ASTReaderInternals.h"
#include "AST/ASTVisitor.h"
#include "ModuleReader.h"
#include "Sema/Scope/Scope.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::serial;
using namespace cdot::support;
using namespace cdot::serial::reader;

namespace {

class ASTStmtReader: public ASTVisitor<ASTStmtReader> {
   ASTRecordReader &Record;
   llvm::BitstreamCursor &DeclsCursor;

   SourceLocation ReadSourceLocation()
   {
      return Record.readSourceLocation();
   }

   SourceRange ReadSourceRange()
   {
      return Record.readSourceRange();
   }

   std::string ReadString()
   {
      return Record.readString();
   }

   Decl *ReadDecl()
   {
      return Record.readDecl();
   }

   template<typename T>
   T *ReadDeclAs()
   {
      return Record.readDeclAs<T>();
   }

public:
   ASTStmtReader(ASTRecordReader &Record, llvm::BitstreamCursor &Cursor)
      : Record(Record), DeclsCursor(Cursor)
   {
      (void)DeclsCursor;
   }

   /// The number of record fields required for the Stmt class
   /// itself.
   static const unsigned NumStmtFields = 1;

   /// The number of record fields required for the Expr class
   /// itself.
   static const unsigned NumExprFields = NumStmtFields + 3;

   /// The number of record fields required for the TypeExpr class
   /// itself.
   static const unsigned NumTypeExprFields = NumExprFields + 3;

   void visitStmt(Statement *S);
   void visitExpr(Expression *E);

   void visitPatternExpr(PatternExpr *E);
   void visitTypeExpr(TypeExpr *E);

   ConversionSequence *ReadConvSeq();
   IfCondition ReadIfCondition();

#  define CDOT_STMT(NAME) void visit##NAME(NAME *S);
#  include "AST/AstNode.def"
};

} // anonymous namespace

void ASTStmtReader::visitStmt(Statement *S)
{
   unsigned SubclassData = Record.readInt();
   S->setSubclassData(SubclassData);
}

void ASTStmtReader::visitExpr(Expression *E)
{
   visitStmt(E);

   E->setExprType(Record.readType());
   E->setEllipsisLoc(Record.readSourceLocation());

   uint64_t Flags = Record.readInt();
   E->setIsLHSOfAssignment((Flags & 1) != 0);
   E->setAllowTemplate((Flags & (1 << 1)) != 0);
   E->setIsMagicArgumentValue((Flags & (1 << 2)) != 0);
}

void ASTStmtReader::visitCompoundStmt(CompoundStmt *S)
{
   visitStmt(S);

   SmallVector<Statement*, 16> Stmts;
   unsigned NumStmts = Record.readInt();
   while (NumStmts--)
      Stmts.push_back(Record.readSubStmt());

   std::copy(Stmts.begin(), Stmts.end(), S->getStatements().data());

   S->setLBraceLoc(Record.readSourceLocation());
   S->setRBraceLoc(Record.readSourceLocation());

   S->setPreserveScope(Record.readInt());
   S->setUnsafe(Record.readInt());
   S->setContainsDeclStmt(Record.readInt());
   S->setScopeID(Record.readInt());
}

void ASTStmtReader::visitDeclStmt(DeclStmt *S)
{
   visitStmt(S);

   S->setDecl(Record.readDecl());
}

void ASTStmtReader::visitAttributedStmt(AttributedStmt *S)
{
   visitStmt(S);

   SmallVector<Attr*, 4> Attrs;
   Record.readAttributes(Attrs);

   std::copy(Attrs.begin(), Attrs.end(),
             const_cast<Attr**>(S->getAttributes().data()));

   S->setStatement(Record.readSubStmt());
}

void ASTStmtReader::visitBreakStmt(BreakStmt *S)
{
   visitStmt(S);
   S->setLoc(Record.readSourceLocation());
   S->setLabel(Record.getIdentifierInfo());
}

void ASTStmtReader::visitContinueStmt(ContinueStmt *S)
{
   visitStmt(S);
   S->setLoc(Record.readSourceLocation());
   S->setLabel(Record.getIdentifierInfo());
}

void ASTStmtReader::visitReturnStmt(ReturnStmt *S)
{
   visitStmt(S);

   S->setReturnValue(Record.readSubExpr());
   S->setRetLoc(Record.readSourceLocation());
}

void ASTStmtReader::visitDiscardAssignStmt(DiscardAssignStmt *S)
{
   visitStmt(S);

   S->setUnderscoreLoc(Record.readSourceLocation());
   S->setEqualsLoc(Record.readSourceLocation());
   S->setRHS(Record.readSubExpr());
}

void ASTStmtReader::visitCaseStmt(CaseStmt *S)
{
   visitStmt(S);

   S->setCaseLoc(Record.readSourceLocation());
   S->setPattern(cast_or_null<PatternExpr>(Record.readSubExpr()));
   S->setBody(Record.readSubStmt());
}

void ASTStmtReader::visitForStmt(ForStmt *S)
{
   visitStmt(S);

   S->setForLoc(Record.readSourceLocation());
   S->setInitialization(Record.readSubStmt());
   S->setTermination(Record.readSubExpr());
   S->setIncrement(Record.readSubStmt());
   S->setBody(Record.readSubStmt());
   S->setLabel(Record.getIdentifierInfo());
}

void ASTStmtReader::visitIfStmt(IfStmt *S)
{
   visitStmt(S);

   auto *Ptr = S->getConditions().data();
   auto NumConditions = Record.readInt();
   while (NumConditions--) {
      *Ptr++ = ReadIfCondition();
   }

   S->setIfLoc(Record.readSourceLocation());
   S->setIfBranch(Record.readSubStmt());
   S->setElseBranch(Record.readSubStmt());
   S->setLabel(Record.getIdentifierInfo());
}

void ASTStmtReader::visitWhileStmt(WhileStmt *S)
{
   visitStmt(S);

   auto *Ptr = S->getConditions().data();
   auto NumConditions = Record.readInt();
   while (NumConditions--) {
      *Ptr++ = ReadIfCondition();
   }

   S->setWhileLoc(Record.readSourceLocation());
   S->setBody(Record.readSubStmt());
   S->setAtLeastOnce(Record.readBool());
   S->setLabel(Record.getIdentifierInfo());
}

void ASTStmtReader::visitForInStmt(ForInStmt *S)
{
   visitStmt(S);

   S->setForLoc(Record.readSourceLocation());
   S->setDecl(Record.readDecl());
   S->setRangeExpr(Record.readSubExpr());
   S->setBody(Record.readSubStmt());

   S->setGetIteratorFn(Record.readDeclAs<MethodDecl>());
   S->setNextFn(Record.readDeclAs<MethodDecl>());
   S->setLabel(Record.getIdentifierInfo());
}

void ASTStmtReader::visitMatchStmt(MatchStmt *S)
{
   visitStmt(S);

   auto Cases = S->getCases();
   auto NumCases = Record.readInt();
   unsigned i = 0;

   while (NumCases--) {
      auto *NextCase = cast<CaseStmt>(Record.readSubStmt());
      Cases[i++] = NextCase;
   }

   S->setMatchLoc(Record.readSourceLocation());
   S->setBraces(Record.readSourceRange());
   S->setSwitchValue(Record.readSubExpr());
   S->setHasMutableCaseArg(Record.readBool());
   S->setIntegralSwitch(Record.readBool());
   S->setLabel(Record.getIdentifierInfo());
}

void ASTStmtReader::visitDoStmt(DoStmt *S)
{
   visitStmt(S);

   auto *Ptr = S->getCatchBlocks().begin();
   auto NumCatchBlocks = Record.readInt();
   while (NumCatchBlocks--) {
      auto *VD = Record.readDeclAs<LocalVarDecl>();
      auto *Body = Record.readSubStmt();
      auto *Cond = Record.readSubExpr();

      new (Ptr++) CatchBlock(VD, Body, Cond);
   }

   S->setSourceRange(Record.readSourceRange());
   S->setBody(Record.readSubStmt());
   S->setLabel(Record.getIdentifierInfo());
}

void ASTStmtReader::visitThrowStmt(ThrowStmt *S)
{
   visitStmt(S);

   S->setThrowLoc(Record.readSourceLocation());
   S->setThrownVal(Record.readSubExpr());
}

void ASTStmtReader::visitDebugStmt(DebugStmt *S)
{
   visitStmt(S);
   S->setLoc(Record.readSourceLocation());
}

void ASTStmtReader::visitNullStmt(NullStmt *S)
{
   visitStmt(S);
   S->setLoc(Record.readSourceLocation());
}

void ASTStmtReader::visitMacroExpansionStmt(MacroExpansionStmt *S)
{
   visitStmt(S);

   SmallVector<lex::Token, 8> Toks;
   auto NumToks = Record.readInt();

   while (NumToks--)
      Toks.push_back(Record.readToken());

   std::copy(Toks.begin(), Toks.end(),
             const_cast<lex::Token*>(S->getTokens().data()));

   S->setSourceRange(Record.readSourceRange());
   S->setMacroName(Record.readDeclarationName());
   S->setDelim(Record.readEnum<MacroExpansionStmt::Delimiter>());
   S->setParentExpr(Record.readExpr());
}

void ASTStmtReader::visitStaticIfStmt(StaticIfStmt *S)
{
   visitStmt(S);

   S->setStaticLoc(Record.readSourceLocation());
   S->setIfLoc(Record.readSourceLocation());
   S->setCondition(cast<StaticExpr>(Record.readSubExpr()));
   S->setIfBranch(Record.readSubStmt());
   S->setElseBranch(Record.readSubStmt());
}

void ASTStmtReader::visitStaticForStmt(StaticForStmt *S)
{
   visitStmt(S);

   S->setStaticLoc(Record.readSourceLocation());
   S->setForLoc(Record.readSourceLocation());
   S->setElementName(Record.getIdentifierInfo());
   S->setRange(cast<StaticExpr>(Record.readSubExpr()));
   S->setBody(Record.readSubStmt());
   S->setVariadic(Record.readBool());

   if (S->isVariadic()) {
      S->setVariadicDecl(ReadDeclAs<NamedDecl>());
   }
}

void ASTStmtReader::visitMixinStmt(MixinStmt *S)
{
   visitStmt(S);

   S->setParens(Record.readSourceRange());
   S->setMixinExpr(Record.readSubExpr());
}

void ASTStmtReader::visitParenExpr(ParenExpr *S)
{
   visitExpr(S);

   S->setParens(Record.readSourceRange());
   S->setParenthesizedExpr(Record.readSubExpr());
}

void ASTStmtReader::visitAttributedExpr(AttributedExpr *S)
{
   visitExpr(S);

   SmallVector<Attr*, 4> Attrs;
   Record.readAttributes(Attrs);

   std::copy(Attrs.begin(), Attrs.end(),
             const_cast<Attr**>(S->getAttributes().data()));

   S->setExpr(Record.readSubExpr());
}

void ASTStmtReader::visitIntegerLiteral(IntegerLiteral *S)
{
   visitExpr(S);

   S->setSourceRange(Record.readSourceRange());
   S->setType(Record.readType());
   S->setValue(Record.getReader()->getContext(), Record.readAPSInt());
   S->setSuffix(Record.readEnum<IntegerLiteral::Suffix>());
   S->setExpressibleByInit(Record.readDeclAs<InitDecl>());
}

void ASTStmtReader::visitFPLiteral(FPLiteral *S)
{
   visitExpr(S);

   S->setSourceRange(Record.readSourceRange());
   S->setType(Record.readType());
   S->setValue(Record.getReader()->getContext(),
               Record.readAPFloat(llvm::APFloat::IEEEdouble()));
   S->setSuffix(Record.readEnum<FPLiteral::Suffix>());
   S->setExpressibleByInit(Record.readDeclAs<InitDecl>());
}

void ASTStmtReader::visitBoolLiteral(BoolLiteral *S)
{
   visitExpr(S);

   S->setSourceLocation(Record.readSourceLocation());
   S->setType(Record.readType());
   S->setValue(Record.readBool());
   S->setExpressibleByInit(Record.readDeclAs<InitDecl>());
}

void ASTStmtReader::visitCharLiteral(CharLiteral *S)
{
   visitExpr(S);

   S->setSourceRange(Record.readSourceRange());
   S->setType(Record.readType());
   S->setWide(Record.readInt());
   S->setExpressibleByInit(Record.readDeclAs<InitDecl>());
}

void ASTStmtReader::visitNoneLiteral(NoneLiteral *S)
{
   visitExpr(S);
   S->setSourceRange(Record.readSourceLocation());
}

void ASTStmtReader::visitStringLiteral(StringLiteral *S)
{
   visitExpr(S);

   S->setSourceRange(Record.readSourceRange());
   S->setValue(Record.readString());
   S->setExpressibleByInit(Record.readDeclAs<InitDecl>());
}

void ASTStmtReader::visitStringInterpolation(StringInterpolation *S)
{
   visitExpr(S);

   SmallVector<Expression*, 8> Strings;
   auto NumStrings = Record.readInt();

   while (NumStrings--)
      Strings.push_back(Record.readSubExpr());

   std::copy(Strings.begin(), Strings.end(), S->getSegments().data());

   S->setSourceRange(Record.readSourceRange());
}

void ASTStmtReader::visitLambdaExpr(LambdaExpr *S)
{
   visitExpr(S);

   auto *ArgPtr = S->arg_begin();
   auto NumArgs = Record.readInt();
   while (NumArgs--)
      *ArgPtr++ = Record.readDeclAs<FuncArgDecl>();

   auto NumCaptures = Record.readInt();
   while (NumCaptures--)
      S->addCapture(Record.getReader()->getContext(),
                    Record.readDeclAs<NamedDecl>());

   S->setFunc(Record.readDeclAs<FunctionDecl>());
   S->setParenRange(Record.readSourceRange());
   S->setArrowLoc(Record.readSourceLocation());
   S->setReturnType(Record.readSourceType());
   S->setBody(Record.readSubStmt());
}

void ASTStmtReader::visitDictionaryLiteral(DictionaryLiteral *S)
{
   visitExpr(S);

   SmallVector<Expression*, 4> Vec;
   auto NumValues = Record.readInt();

   for (unsigned i = 0; i < NumValues; ++i)
      Vec.push_back(Record.readSubExpr());

   std::copy(Vec.begin(), Vec.end(), S->getKeys().data());

   Vec.clear();

   for (unsigned i = 0; i < NumValues; ++i)
      Vec.push_back(Record.readSubExpr());

   std::copy(Vec.begin(), Vec.end(), S->getValues().data());

   S->setSquareRange(Record.readSourceRange());
   S->setInsertFn(Record.readDeclAs<MethodDecl>());
   S->setExpressibleByInit(Record.readDeclAs<InitDecl>());
}

void ASTStmtReader::visitArrayLiteral(ArrayLiteral *S)
{
   visitExpr(S);

   SmallVector<Expression*, 4> Vec;
   auto NumValues = Record.readInt();

   for (unsigned i = 0; i < NumValues; ++i)
      Vec.push_back(Record.readSubExpr());

   std::copy(Vec.begin(), Vec.end(), S->getValues().data());

   S->setSquareRange(Record.readSourceRange());
   S->setExpressibleByInit(Record.readDeclAs<InitDecl>());
}

void ASTStmtReader::visitTupleLiteral(TupleLiteral *S)
{
   visitExpr(S);

   SmallVector<Expression*, 4> Vec;
   auto NumValues = Record.readInt();

   for (unsigned i = 0; i < NumValues; ++i)
      Vec.push_back(Record.readSubExpr());

   std::copy(Vec.begin(), Vec.end(), S->getElements().data());
   S->setParenRange(Record.readSourceRange());
}

void ASTStmtReader::visitIdentifierRefExpr(IdentifierRefExpr *S)
{
   visitExpr(S);

   S->setLoc(Record.readSourceRange());
   S->setIdent(Record.readDeclarationName());
   S->setKind(Record.readEnum<IdentifierKind>());

   if (S->getKind() != IdentifierKind::MetaType) {
      S->setNamedDecl(Record.readDeclAs<NamedDecl>());
   }

   uint64_t Flags = Record.readInt();
   S->setStaticLookup((Flags & 1) != 0);
   S->setIsPointerAccess((Flags & (1 << 1)) != 0);
   S->setOnlyForLookup((Flags & (1 << 2)) != 0);
   S->setInTypePos((Flags & (1 << 3)) != 0);
   S->setIsSynthesized((Flags & (1 << 4)) != 0);
   S->setIsCapture((Flags & (1 << 5)) != 0);
   S->setSelf((Flags & (1 << 6)) != 0);
   S->setAllowIncompleteTemplateArgs((Flags & (1 << 7)) != 0);
   S->setAllowNamespaceRef((Flags & (1 << 8)) != 0);
   S->setAllowOverloadRef((Flags & (1 << 9)) != 0);
   S->setAllowVariadicRef((Flags & (1 << 10)) != 0);
   S->setLeadingDot((Flags & (1 << 11)) != 0);
   S->setCalled((Flags & (1 << 12)) != 0);

   S->setCaptureIndex(Record.readInt());
   S->setParentExpr(Record.readSubExpr());
   S->setDeclCtx(Record.readDeclAs<DeclContext>());
}

void ASTStmtReader::visitDeclRefExpr(DeclRefExpr *S)
{
   visitExpr(S);

   S->setDecl(Record.readDeclAs<NamedDecl>());
   S->setSourceRange(Record.readSourceRange());

   uint64_t Flags = Record.readInt();
   S->setAllowModuleRef((Flags & 1) != 0);
}

void ASTStmtReader::visitMemberRefExpr(MemberRefExpr *S)
{
   visitExpr(S);

   S->setParentExpr(Record.readSubExpr());
   S->setMemberDecl(Record.readDeclAs<NamedDecl>());
   S->setSourceRange(Record.readSourceRange());
   S->setCalled(Record.readBool());
}

void ASTStmtReader::visitOverloadedDeclRefExpr(OverloadedDeclRefExpr *S)
{
   visitExpr(S);

   unsigned NumOverloads = Record.readInt();
   auto *Ptr = S->getOverloads().data();

   while (NumOverloads--) {
      *Ptr++ = Record.readDeclAs<NamedDecl>();
   }

   S->setSourceRange(Record.readSourceRange());
   S->setParentExpr(Record.readSubExpr());
}

void ASTStmtReader::visitEnumCaseExpr(EnumCaseExpr *S)
{
   visitExpr(S);

   S->setPeriodLoc(Record.readSourceLocation());
   S->setIdent(Record.readDeclarationName());

   auto &Args = S->getArgs();
   auto NumArgs = Record.readInt();
   Args.reserve(Record.getReader()->getContext(), NumArgs);

   while (NumArgs--)
      Args.push_back(Record.readSubExpr(), Record.getReader()->getContext());

   S->setCase(Record.readDeclAs<EnumCaseDecl>());
}

void ASTStmtReader::visitCallExpr(CallExpr *S)
{
   visitExpr(S);

   auto *LabelPtr = S->getLabels().data();
   auto NumLabels = Record.readInt();

   while (NumLabels--)
      *LabelPtr++ = Record.getIdentifierInfo();

   S->setIdentLoc(Record.readSourceLocation());
   S->setParenRange(Record.readSourceRange());

   S->setIdent(Record.readDeclarationName());
   S->setKind(Record.readEnum<CallKind>());

   auto &Args = S->getArgs();
   auto NumArgs = Record.readInt();
   Args.reserve(Record.getReader()->getContext(), NumArgs);

   while (NumArgs--)
      Args.push_back(Record.readSubExpr(), Record.getReader()->getContext());

   uint64_t Flags = Record.readInt();
   S->setIsPointerAccess((Flags & 1) != 0);
   S->setIsUFCS((Flags & (1 << 1)) != 0);
   S->setIsDotInit((Flags & (1 << 2)) != 0);
   S->setIsDotDeinit((Flags & (1 << 3)) != 0);
   S->setIncludesSelf((Flags & (1 << 4)) != 0);
   S->setDirectCall((Flags & (1 << 5)) != 0);
   S->setLeadingDot((Flags & (1 << 6)) != 0);

   S->setBuiltinKind(Record.readInt());
   S->setFunc(Record.readDeclAs<CallableDecl>());
   S->setParentExpr(Record.readSubExpr());

   bool hasTemplateArgs = Record.readBool();
   if (hasTemplateArgs) {
      S->setTemplateArgs(Record.readTemplateArgumentList());
   }
}

void ASTStmtReader::visitAnonymousCallExpr(AnonymousCallExpr *S)
{
   visitExpr(S);

   SmallVector<Expression*, 4> Args;
   auto NumArgs = Record.readInt();
   while (NumArgs--)
      Args.push_back(Record.readSubExpr());

   std::copy(Args.begin(), Args.end(), S->getArgs().data());

   auto *LabelPtr = S->getLabels().data();
   auto NumLabels = Record.readInt();

   while (NumLabels--)
      *LabelPtr++ = Record.getIdentifierInfo();

   S->setParenRange(Record.readSourceRange());
   S->setIsPrimitiveInit(Record.readBool());
   S->setFunctionType(cast_or_null<FunctionType>(
      Record.readType().getBuiltinTy()));

   S->setParentExpr(Record.readSubExpr());
}

void ASTStmtReader::visitSubscriptExpr(SubscriptExpr *S)
{
   visitExpr(S);

   SmallVector<Expression*, 4> Args;
   auto NumArgs = Record.readInt();
   while (NumArgs--)
      Args.push_back(Record.readSubExpr());

   std::copy(Args.begin(), Args.end(), S->getIndices().data());

   S->setSquareRange(Record.readSourceRange());
   S->setParentExpr(Record.readSubExpr());
}

void ASTStmtReader::visitTemplateArgListExpr(TemplateArgListExpr *S)
{
   visitExpr(S);

   SmallVector<Expression*, 4> Args;
   auto NumArgs = Record.readInt();
   while (NumArgs--)
      Args.push_back(Record.readSubExpr());

   std::copy(Args.begin(), Args.end(), S->getExprs().data());

   S->setAngleRange(Record.readSourceRange());
   S->setParentExpr(Record.readSubExpr());
}

void ASTStmtReader::visitBuiltinExpr(BuiltinExpr *S)
{
   visitExpr(S);
}

void ASTStmtReader::visitTupleMemberExpr(TupleMemberExpr *S)
{
   visitExpr(S);

   S->setLoc(Record.readSourceLocation());
   S->setIndex(Record.readInt());
   S->setPointerAccess(Record.readBool());
   S->setParentExpr(Record.readSubExpr());
}

void ASTStmtReader::visitSelfExpr(SelfExpr *S)
{
   visitExpr(S);

   S->setLoc(Record.readSourceLocation());
   S->setSelfArg(Record.readDeclAs<FuncArgDecl>());
   S->setCaptureIndex(Record.readInt());
   S->setUppercase(Record.readBool());
}

void ASTStmtReader::visitSuperExpr(SuperExpr *S)
{
   visitExpr(S);

   S->setLoc(Record.readSourceLocation());
   S->setSelfArg(Record.readDeclAs<FuncArgDecl>());
   S->setCaptureIndex(Record.readInt());
}

void ASTStmtReader::visitBuiltinIdentExpr(BuiltinIdentExpr *S)
{
   visitExpr(S);

   S->setLoc(Record.readSourceLocation());
   S->setIdentifierKind(Record.readEnum<BuiltinIdentifier>());
}

void ASTStmtReader::visitPatternExpr(PatternExpr *E)
{
   visitExpr(E);
   E->setColonLoc(Record.readSourceLocation());
}

void ASTStmtReader::visitExpressionPattern(ExpressionPattern *S)
{
   visitPatternExpr(S);
   S->setExpr(Record.readSubExpr());
   S->setComparisonOp(Record.readDeclAs<CallableDecl>());
}

void ASTStmtReader::visitCasePattern(CasePattern *S)
{
   visitExpr(S);

   auto *Ptr = S->getTrailingObjects<IfCondition>();
   auto NumArgs = Record.readInt();
   while (NumArgs--) {
      *Ptr++ = ReadIfCondition();
   }

   S->setColonLoc(Record.readSourceLocation());
   S->setSourceRange(Record.readSourceRange());

   S->setKind(Record.readEnum<CasePattern::Kind>());

   S->setParentExpr(Record.readSubExpr());
   S->setCaseName(Record.getIdentifierInfo());

   S->setHasBinding(Record.readBool());
   S->setHasExpr(Record.readBool());
   S->setLeadingDot(Record.readBool());

   S->setCaseDecl(Record.readDeclAs<EnumCaseDecl>());
}

void ASTStmtReader::visitIsPattern(IsPattern *S)
{
   visitPatternExpr(S);

   S->setIsLoc(ReadSourceLocation());
   S->setIsType(Record.readSourceType());
}

void ASTStmtReader::visitExprSequence(ExprSequence *S)
{
   visitExpr(S);

   SequenceElement *Ptr = S->getTrailingObjects<SequenceElement>();
   auto NumElements = Record.readInt();

   while (NumElements--) {
      SourceLocation SR = ReadSourceLocation();
      auto Kind = Record.readEnum<SequenceElement::Kind>();

      switch (Kind) {
      case SequenceElement::EF_Expression: {
         new (Ptr++) SequenceElement(Record.readExpr());
         break;
      }
      case SequenceElement::EF_PossibleOperator: {
         uint8_t whitespace = (uint8_t)Record.readInt();
         new (Ptr++) SequenceElement(Record.getIdentifierInfo(), whitespace,
                                     SR);
         break;
      }
      case SequenceElement::EF_Operator: {
         uint8_t whitespace = (uint8_t)Record.readInt();
         new (Ptr++) SequenceElement(Record.readEnum<op::OperatorKind>(),
                                     whitespace, SR);
         break;
      }
      }
   }
}

void ASTStmtReader::visitUnaryOperator(UnaryOperator *S)
{
   visitExpr(S);

   S->setOperatorLoc(ReadSourceLocation());
   S->setKind(Record.readEnum<op::OperatorKind>());
   S->setFuncTy(cast<FunctionType>(*Record.readType()));
   S->setTarget(Record.readSubExpr());
   S->setPrefix(Record.readBool());
}

void ASTStmtReader::visitBinaryOperator(BinaryOperator *S)
{
   visitExpr(S);

   S->setOperatorLoc(ReadSourceLocation());
   S->setKind(Record.readEnum<op::OperatorKind>());
   S->setFunctionType(cast<FunctionType>(*Record.readType()));
   S->setLhs(Record.readSubExpr());
   S->setRhs(Record.readSubExpr());
}

void ASTStmtReader::visitAssignExpr(AssignExpr *S)
{
   visitExpr(S);

   S->setEqualsLoc(ReadSourceLocation());
   S->setLhs(Record.readSubExpr());
   S->setRhs(Record.readSubExpr());
   S->setIsInitialization(Record.readBool());
}

ConversionSequence* ASTStmtReader::ReadConvSeq()
{
   SmallVector<ConversionStep, 2> Steps;
   auto Strength = Record.readEnum<CastStrength>();
   auto NumSteps = Record.readInt();

   while (NumSteps--) {
      bool isHalt = Record.readBool();
      if (isHalt) {
         Steps.emplace_back();
         continue;
      }

      auto Kind = Record.readEnum<CastKind>();
      if (Kind == CastKind::ConversionOp) {
         auto *Op = Record.readDeclAs<CallableDecl>();
         Steps.emplace_back(Op);
      }
      else {
         auto Ty = Record.readType();
         Steps.emplace_back(Kind, Ty);
      }
   }

   return ConversionSequence::Create(Record.getReader()->getContext(),
                                     Strength, Steps);
}

IfCondition ASTStmtReader::ReadIfCondition()
{
   auto K = Record.readEnum<IfCondition::Kind>();
   switch (K) {
   case IfCondition::Expression:
      return IfCondition(Record.readExpr());
   case IfCondition::Binding: {
      auto *D = Record.readDeclAs<LocalVarDecl>();

      CallableDecl *TryUnwrapFn = nullptr;
      if (Record.readBool()) {
         TryUnwrapFn = Record.readDeclAs<CallableDecl>();
      }

      CallableDecl *HasValueFn = nullptr;
      if (Record.readBool()) {
         HasValueFn = Record.readDeclAs<CallableDecl>();
      }

      return IfCondition(D, TryUnwrapFn, HasValueFn);
   }
   case IfCondition::Pattern: {
      auto *Pat = cast<PatternExpr>(Record.readExpr());
      auto *E = Record.readExpr();

      return IfCondition(Pat, E);
   }
   }
}

void ASTStmtReader::visitCastExpr(CastExpr *S)
{
   visitExpr(S);

   S->setAsLoc(ReadSourceLocation());
   S->setStrength(Record.readEnum<CastStrength>());
   S->setTarget(Record.readSubExpr());
   S->setTargetType(Record.readSourceType());
   S->setConvSeq(ReadConvSeq());
}

void ASTStmtReader::visitAddrOfExpr(AddrOfExpr *S)
{
   visitExpr(S);

   S->setAmpLoc(ReadSourceLocation());
   S->setTarget(Record.readSubExpr());
}

void ASTStmtReader::visitTypePredicateExpr(TypePredicateExpr *S)
{
   visitExpr(S);

   S->setIsLoc(ReadSourceLocation());
   S->setSourceRange(ReadSourceRange());
   S->setLHS(Record.readSubExpr());
   S->setRHS(cast<ConstraintExpr>(Record.readSubExpr()));

   uint64_t Flags = Record.readInt();
   S->setResult((Flags & 1) != 0);
   S->setIsCompileTimeCheck((Flags & (1 << 1)) != 0);
   S->setNegated((Flags & (1 << 2)) != 0);
}

void ASTStmtReader::visitIfExpr(IfExpr *S)
{
   visitExpr(S);

   S->setIfLoc(ReadSourceLocation());
   S->setCond(ReadIfCondition());
   S->setTrueVal(Record.readSubExpr());
   S->setFalseVal(Record.readSubExpr());
}

void ASTStmtReader::visitTryExpr(TryExpr *S)
{
   visitExpr(S);

   S->setTryLoc(ReadSourceLocation());
   S->setKind(Record.readEnum<TryExpr::TryKind>());
   S->setExpr(Record.readSubExpr());
}

void ASTStmtReader::visitAwaitExpr(AwaitExpr *S)
{
   visitExpr(S);

   S->setAwaitLoc(ReadSourceLocation());
   S->setExpr(Record.readSubExpr());
   S->setImmediateReturn(Record.readBool());
}

void ASTStmtReader::visitImplicitCastExpr(ImplicitCastExpr *S)
{
   visitExpr(S);

   S->setTarget(Record.readSubExpr());
   S->setConvSeq(ReadConvSeq());
}

void ASTStmtReader::visitStaticExpr(StaticExpr *S)
{
   visitExpr(S);

   S->setLoc(Record.readSourceRange());
   S->setExpr(Record.readSubExpr());
   S->setEvaluatedExpr(Record.readILConstant());
}

void ASTStmtReader::visitConstraintExpr(ConstraintExpr *S)
{
   visitExpr(S);

   S->setLoc(ReadSourceLocation());
   S->setKind(Record.readEnum<ConstraintExpr::Kind>());

   if (S->getKind() == ConstraintExpr::Type)
      S->setTypeConstraint(Record.readSourceType());
}

void ASTStmtReader::visitTraitsExpr(TraitsExpr *S)
{
   visitExpr(S);

   auto *Ptr = S->getTrailingObjects<TraitsArgument>();
   auto NumArgs = Record.readInt();

   while (NumArgs--) {
      auto Kind = Record.readEnum<TraitsArgument::Kind>();
      switch (Kind) {
      case TraitsArgument::Type: {
         new (Ptr++) TraitsArgument(Record.readSourceType());
         break;
      }
      case TraitsArgument::Expr: {
         new (Ptr++) TraitsArgument(Record.readSubExpr());
         break;
      }
      case TraitsArgument::Stmt: {
         new (Ptr++) TraitsArgument(Record.readSubStmt());
         break;
      }
      case TraitsArgument::String: {
         new (Ptr++) TraitsArgument(Record.readString());
         break;
      }
      }
   }

   S->setTraitsLoc(ReadSourceLocation());
   S->setParens(ReadSourceRange());
   S->setKind(Record.readEnum<TraitsExpr::Kind>());
}

void ASTStmtReader::visitMixinExpr(MixinExpr *S)
{
   visitExpr(S);

   S->setParens(ReadSourceRange());
   S->setMixinExpr(Record.readSubExpr());
}

void ASTStmtReader::visitVariadicExpansionExpr(VariadicExpansionExpr *S)
{
   visitExpr(S);

   S->setExpr(Record.readSubExpr());
   S->setEllipsisLoc(ReadSourceLocation());
   S->setParameterPack(ReadDeclAs<NamedDecl>());
   S->setElementDecl(ReadDeclAs<NamedDecl>());
}

void ASTStmtReader::visitMacroVariableExpr(MacroVariableExpr *S)
{
   visitExpr(S);
   S->setExpr(Record.readSubExpr());
}

void ASTStmtReader::visitMacroExpansionExpr(MacroExpansionExpr *S)
{
   visitExpr(S);

   auto *Ptr = S->getTrailingObjects<lex::Token>();
   auto NumTokens = Record.readInt();

   while (NumTokens--) {
      *Ptr++ = Record.readToken();
   }

   S->setSourceRange(ReadSourceRange());
   S->setMacroName(Record.readDeclarationName());
   S->setDelim(Record.readEnum<MacroExpansionExpr::Delimiter>());
}

void ASTStmtReader::visitTypeExpr(TypeExpr *E)
{
   visitExpr(E);

   E->setSourceRange(ReadSourceRange());
   E->setIsMeta(Record.readBool());
}

void ASTStmtReader::visitTupleTypeExpr(TupleTypeExpr *S)
{
   visitTypeExpr(S);

   auto *Ptr = S->getTrailingObjects<SourceType>();
   auto NumTys = Record.readInt();

   while (NumTys--) {
      *Ptr++ = Record.readSourceType();
   }
}

void ASTStmtReader::visitArrayTypeExpr(ArrayTypeExpr *S)
{
   visitTypeExpr(S);

   S->setElementTy(Record.readSourceType());
   S->setSizeExpr(cast_or_null<StaticExpr>(Record.readSubExpr()));
}

void ASTStmtReader::visitFunctionTypeExpr(FunctionTypeExpr *S)
{
   visitTypeExpr(S);

   auto *Ptr = S->getTrailingObjects<SourceType>();
   auto *InfoPtr = S->getTrailingObjects<FunctionType::ParamInfo>();
   auto NumArgs = Record.readInt();

   while (NumArgs--) {
      *Ptr++ = Record.readSourceType();
      *InfoPtr++ = FunctionType::ParamInfo(
         Record.readEnum<ArgumentConvention>());
   }

   S->setRetTy(Record.readSourceType());
   S->setThin(Record.readBool());
   S->setThrows(Record.readBool());
   S->setUnsafe(Record.readBool());
   S->setAsync(Record.readBool());
}

void ASTStmtReader::visitExistentialTypeExpr(ExistentialTypeExpr *S)
{
   visitTypeExpr(S);

   auto *Ptr = S->getExistentials().data();
   auto NumArgs = Record.readInt();

   while (NumArgs--) {
      *Ptr++ = Record.readSourceType();
   }
}

void ASTStmtReader::visitDeclTypeExpr(DeclTypeExpr *S)
{
   visitTypeExpr(S);
   S->setTyExpr(Record.readSubExpr());
}

void ASTStmtReader::visitPointerTypeExpr(PointerTypeExpr *S)
{
   visitTypeExpr(S);
   S->setSubType(Record.readSourceType());
}

void ASTStmtReader::visitReferenceTypeExpr(ReferenceTypeExpr *S)
{
   visitTypeExpr(S);
   S->setSubType(Record.readSourceType());
}

void ASTStmtReader::visitOptionTypeExpr(OptionTypeExpr *S)
{
   visitTypeExpr(S);
   S->setSubType(Record.readSourceType());
}

Statement *ASTReader::ReadStmt()
{
   switch (ReadingKind) {
   case Read_None:
      llvm_unreachable("should not call this when not reading anything");
   case Read_Decl:
      return ReadStmtFromStream(DeclsCursor);
   case Read_Type:
      return ReadStmtFromStream(DeclsCursor);
   case Read_Stmt:
      return ReadSubStmt();
   }

   llvm_unreachable("ReadingKind not set?");
}

Expression *ASTReader::ReadExpr()
{
   return cast_or_null<Expression>(ReadStmt());
}

Expression *ASTReader::ReadSubExpr()
{
   return cast_or_null<Expression>(ReadSubStmt());
}

Statement* ASTReader::ReadStmtFromStream(llvm::BitstreamCursor &Cursor)
{
   ReadingKindTracker ReadingKind(Read_Stmt, *this);

   // Map of offset to previously deserialized stmt. The offset points
   // just after the stmt record.
   llvm::DenseMap<uint64_t, Statement*> StmtEntries;

#ifndef NDEBUG
   unsigned PrevNumStmts = StmtStack.size();
#endif

   ASTRecordReader Record(*this);
   ASTStmtReader Reader(Record, Cursor);
   Statement::EmptyShell Empty;
   ASTContext &C = Context;

   while (true) {
      llvm::BitstreamEntry Entry = Cursor.advanceSkippingSubblocks();
      bool Finished = false;

      switch (Entry.Kind) {
      case llvm::BitstreamEntry::SubBlock: // Handled for us already.
      case llvm::BitstreamEntry::Error:
         Error("malformed block record in AST file");
         return nullptr;
      case llvm::BitstreamEntry::EndBlock:
         Finished = true;
         break;
      case llvm::BitstreamEntry::Record:
         // The interesting case.
         break;
      }

      if (Finished)
         break;

      Statement *S = nullptr;
      bool IsStmtReference = false;

      switch (Record.readRecord(Cursor, Entry.ID)) {
      case STMT_STOP:
         Finished = true;
         break;

      case STMT_REF_PTR:
         IsStmtReference = true;
         assert(StmtEntries.find(Record[0]) != StmtEntries.end() &&
                "No stmt was recorded for this offset reference!");

         S = StmtEntries[Record.readInt()];
         break;

      case STMT_NULL_PTR:
         S = nullptr;
         break;
      case Statement::CompoundStmtID:
         S = CompoundStmt::CreateEmpty(C, Record[ASTStmtReader::NumStmtFields]);
         break;
      case Statement::DeclStmtID:
         S = new(C) DeclStmt(Empty);
         break;
      case Statement::AttributedStmtID:
         S = AttributedStmt::CreateEmpty(C, Record[ASTStmtReader::NumStmtFields]);
         break;
      case Statement::BreakStmtID:
         S = new(C) BreakStmt(Empty);
         break;
      case Statement::ContinueStmtID:
         S = new(C) ContinueStmt(Empty);
         break;
      case Statement::ReturnStmtID:
         S = new(C) ReturnStmt(Empty);
         break;
      case Statement::DiscardAssignStmtID:
         S = DiscardAssignStmt::CreateEmpty(C);
         break;
      case Statement::CaseStmtID:
         S = new(C) CaseStmt(Empty);
         break;
      case Statement::ForStmtID:
         S = new(C) ForStmt(Empty);
         break;
      case Statement::IfStmtID:
         S = IfStmt::CreateEmpty(C, Record[ASTStmtReader::NumStmtFields]);
         break;
      case Statement::WhileStmtID:
         S = WhileStmt::CreateEmpty(C, Record[ASTStmtReader::NumStmtFields]);
         break;
      case Statement::ForInStmtID:
         S = new(C) ForInStmt(Empty);
         break;
      case Statement::MatchStmtID:
         S = MatchStmt::CreateEmpty(C, Record[ASTStmtReader::NumStmtFields]);
         break;
      case Statement::DoStmtID:
         S = new(C) DoStmt(Empty, Record[ASTStmtReader::NumStmtFields]);
         break;
      case Statement::ThrowStmtID:
         S = new(C) ThrowStmt(Empty);
         break;
      case Statement::DebugStmtID:
         S = new(C) DebugStmt(Empty);
         break;
      case Statement::NullStmtID:
         S = new(C) NullStmt(Empty);
         break;
      case Statement::MacroExpansionStmtID:
         S = MacroExpansionStmt::CreateEmpty(C,
                                          Record[ASTStmtReader::NumStmtFields]);
         break;
      case Statement::StaticIfStmtID:
         S = new(C) StaticIfStmt(Empty);
         break;
      case Statement::StaticForStmtID:
         S = new(C) StaticForStmt(Empty);
         break;
      case Statement::MixinStmtID:
         S = new(C) MixinStmt(Empty);
         break;
      case Statement::ParenExprID:
         S = new(C) ParenExpr(Empty);
         break;
      case Statement::AttributedExprID:
         S = AttributedExpr::CreateEmpty(C,
                                         Record[ASTStmtReader::NumExprFields]);
         break;
      case Statement::IntegerLiteralID:
         S = new(C) IntegerLiteral(Empty);
         break;
      case Statement::FPLiteralID:
         S = new(C) FPLiteral(Empty);
         break;
      case Statement::BoolLiteralID:
         S = new(C) BoolLiteral(Empty);
         break;
      case Statement::CharLiteralID:
         S = new(C) CharLiteral(Empty);
         break;
      case Statement::NoneLiteralID:
         S = new(C) NoneLiteral(Empty);
         break;
      case Statement::StringLiteralID:
         S = new(C) StringLiteral(Empty);
         break;
      case Statement::StringInterpolationID:
         S = StringInterpolation::CreateEmpty(C,
                                          Record[ASTStmtReader::NumExprFields]);
         break;
      case Statement::LambdaExprID:
         S = LambdaExpr::CreateEmpty(C, Record[ASTStmtReader::NumExprFields]);
         break;
      case Statement::DictionaryLiteralID:
         S = DictionaryLiteral::CreateEmpty(C,
                                          Record[ASTStmtReader::NumExprFields]);
         break;
      case Statement::ArrayLiteralID:
         S = ArrayLiteral::CreateEmpty(C, Record[ASTStmtReader::NumExprFields]);
         break;
      case Statement::TupleLiteralID:
         S = TupleLiteral::CreateEmpty(C, Record[ASTStmtReader::NumExprFields]);
         break;
      case Statement::IdentifierRefExprID:
         S = new(C) IdentifierRefExpr(Empty);
         break;
      case Statement::EnumCaseExprID:
         S = new(C) EnumCaseExpr(Empty);
         break;
      case Statement::CallExprID:
         S = CallExpr::CreateEmpty(C, Record[ASTStmtReader::NumExprFields]);
         break;
      case Statement::AnonymousCallExprID:
         S = AnonymousCallExpr::CreateEmpty(C,
                                          Record[ASTStmtReader::NumExprFields]);
         break;
      case Statement::SubscriptExprID:
         S = SubscriptExpr::CreateEmpty(C, Record[ASTStmtReader::NumExprFields]);
         break;
      case Statement::TemplateArgListExprID:
         S = TemplateArgListExpr::CreateEmpty(
            C, Record[ASTStmtReader::NumExprFields]);
         break;
      case Statement::BuiltinExprID:
         S = new(C) BuiltinExpr(Empty);
         break;
      case Statement::TupleMemberExprID:
         S = new(C) TupleMemberExpr(Empty);
         break;
      case Statement::DeclRefExprID:
         S = new(C) DeclRefExpr(Empty);
         break;
      case Statement::MemberRefExprID:
         S = new(C) MemberRefExpr(Empty);
         break;
      case Statement::OverloadedDeclRefExprID:
         S = OverloadedDeclRefExpr::CreateEmpty(
            Context, Record[ASTStmtReader::NumExprFields]);
         break;
      case Statement::SelfExprID:
         S = new(C) SelfExpr(Empty);
         break;
      case Statement::SuperExprID:
         S = new(C) SuperExpr(Empty);
         break;
      case Statement::BuiltinIdentExprID:
         S = new(C) BuiltinIdentExpr(Empty);
         break;
      case Statement::ExpressionPatternID:
         S = new(C) ExpressionPattern(Empty);
         break;
      case Statement::CasePatternID:
         S = CasePattern::CreateEmpty(C, Record[ASTStmtReader::NumExprFields]);
         break;
      case Statement::IsPatternID:
         S = new(C) IsPattern(Empty);
         break;
      case Statement::ExprSequenceID:
         S = ExprSequence::CreateEmpty(C, Record[ASTStmtReader::NumExprFields]);
         break;
      case Statement::UnaryOperatorID:
         S = new(C) UnaryOperator(Empty);
         break;
      case Statement::BinaryOperatorID:
         S = new(C) BinaryOperator(Empty);
         break;
      case Statement::AssignExprID:
         S = new(C) AssignExpr(Empty);
         break;
      case Statement::CastExprID:
         S = new(C) CastExpr(Empty);
         break;
      case Statement::AddrOfExprID:
         S = new(C) AddrOfExpr(Empty);
         break;
      case Statement::TypePredicateExprID:
         S = new(C) TypePredicateExpr(Empty);
         break;
      case Statement::IfExprID:
         S = new(C) IfExpr(Empty);
         break;
      case Statement::TryExprID:
         S = new(C) TryExpr(Empty);
         break;
      case Statement::AwaitExprID:
         S = new(C) AwaitExpr(Empty);
         break;
      case Statement::ImplicitCastExprID:
         S = new(C) ImplicitCastExpr(Empty);
         break;
      case Statement::StaticExprID:
         S = new(C) StaticExpr(Empty);
         break;
      case Statement::ConstraintExprID:
         S = new(C) ConstraintExpr(Empty);
         break;
      case Statement::TraitsExprID:
         S = TraitsExpr::CreateEmpty(C, Record[ASTStmtReader::NumExprFields]);
         break;
      case Statement::MixinExprID:
         S = new(C) MixinExpr(Empty);
         break;
      case Statement::MacroVariableExprID:
         S = new(C) MacroVariableExpr(Empty);
         break;
      case Statement::MacroExpansionExprID:
         S = MacroExpansionExpr::CreateEmpty(
            C, Record[ASTStmtReader::NumExprFields]);
         break;
      case Statement::TupleTypeExprID:
         S = TupleTypeExpr::CreateEmpty(
            C, Record[ASTStmtReader::NumTypeExprFields]);
         break;
      case Statement::ArrayTypeExprID:
         S = new(C) ArrayTypeExpr(Empty);
         break;
      case Statement::FunctionTypeExprID:
         S = FunctionTypeExpr::CreateEmpty(
            C, Record[ASTStmtReader::NumTypeExprFields]);
         break;
      case Statement::ExistentialTypeExprID:
         S = ExistentialTypeExpr::CreateEmpty(
            C, Record[ASTStmtReader::NumTypeExprFields]);
         break;
      case Statement::DeclTypeExprID:
         S = new(C) DeclTypeExpr(Empty);
         break;
      case Statement::PointerTypeExprID:
         S = new(C) PointerTypeExpr(Empty);
         break;
      case Statement::ReferenceTypeExprID:
         S = new(C) ReferenceTypeExpr(Empty);
         break;
      case Statement::OptionTypeExprID:
         S = new(C) OptionTypeExpr(Empty);
         break;
      default:
         llvm_unreachable("bad record kind");
      }

      // We hit a STMT_STOP, so we're done with this expression.
      if (Finished)
         break;

      ++this->Reader.NumStatementsRead;

      if (S && !IsStmtReference) {
         Reader.visit(S);
         StmtEntries[Cursor.GetCurrentBitNo()] = S;
      }

      assert(Record.getIdx() == Record.size() &&
             "Invalid deserialization of statement");

      StmtStack.push_back(S);
   }

   assert(StmtStack.size() > PrevNumStmts && "Read too many sub-stmts!");
   assert(StmtStack.size() == PrevNumStmts + 1 && "Extra expressions on stack!");

   return StmtStack.pop_back_val();
}

namespace {

class ASTScopeReader {
   ASTRecordReader &Record;
   ASTContext &Ctx;

#  define CDOT_SCOPE(NAME)          \
   NAME *read##NAME();
#  include "Sema/Scope/Scopes.def"

public:
   ASTScopeReader(ASTRecordReader &Record)
      : Record(Record), Ctx(Record.getReader()->getContext())
   { }

   Scope *readScope(Scope::TypeID Kind);
};

} // anonymous namespace

Scope* ASTScopeReader::readScope(Scope::TypeID Kind)
{
   switch (Kind) {
#  define CDOT_SCOPE(NAME) \
   case Scope::NAME##ID: return read##NAME();
#  include "Sema/Scope/Scopes.def"
   }
}

BlockScope *ASTScopeReader::readBlockScope()
{
   auto *EnclosingScope = Record.ReadScope();
   auto HadError = Record.readBool();
   auto UnresolvedStaticCond = Record.readBool();
   auto ID = (unsigned)Record.readInt();

   auto *S = new(Ctx) BlockScope(ID, nullptr, EnclosingScope);
   S->setHadError(HadError);
   S->setHasUnresolvedStaticCond(UnresolvedStaticCond);

   return S;
}

FunctionScope *ASTScopeReader::readFunctionScope()
{
   auto *EnclosingScope = Record.ReadScope();
   auto HadError = Record.readBool();
   auto UnresolvedStaticCond = Record.readBool();
   auto *Fn = Record.readDeclAs<CallableDecl>();
   auto InferrableReturnType = Record.readBool();

   auto *S = new(Ctx) FunctionScope(Fn, InferrableReturnType, EnclosingScope);
   S->setHadError(HadError);
   S->setHasUnresolvedStaticCond(UnresolvedStaticCond);

   return S;
}

MethodScope *ASTScopeReader::readMethodScope()
{
   auto *EnclosingScope = Record.ReadScope();
   auto HadError = Record.readBool();
   auto UnresolvedStaticCond = Record.readBool();
   auto *Fn = Record.readDeclAs<MethodDecl>();
   auto InferrableReturnType = Record.readBool();

   auto *S = new(Ctx) MethodScope(Fn, InferrableReturnType, EnclosingScope);
   S->setHadError(HadError);
   S->setHasUnresolvedStaticCond(UnresolvedStaticCond);

   return S;
}

LambdaScope *ASTScopeReader::readLambdaScope()
{
   llvm_unreachable("nah");
}

LoopScope *ASTScopeReader::readLoopScope()
{
   auto *EnclosingScope = Record.ReadScope();
   auto HadError = Record.readBool();
   auto UnresolvedStaticCond = Record.readBool();
   auto Flags = Record.readInt();

   auto *S = new(Ctx) LoopScope(Flags & 0x1, Flags & 0x2, Flags & 0x4,
                                Flags & 0x8, EnclosingScope);
   S->setHadError(HadError);
   S->setHasUnresolvedStaticCond(UnresolvedStaticCond);

   return S;
}

StaticForScope *ASTScopeReader::readStaticForScope()
{
   auto *EnclosingScope = Record.ReadScope();
   auto HadError = Record.readBool();
   auto UnresolvedStaticCond = Record.readBool();
   auto *ElementName = Record.getIdentifierInfo();
   auto ElementTy = Record.readType();

   auto *S = new(Ctx) StaticForScope(ElementName, ElementTy, EnclosingScope);
   S->setHadError(HadError);
   S->setHasUnresolvedStaticCond(UnresolvedStaticCond);

   return S;
}

Scope* ASTReader::ReadScopeRecord(unsigned ID)
{
   unsigned Index = ID - 1;
   unsigned Loc = ScopeCursorForID(ID);

   // Keep track of where we are in the stream, then jump back there
   // after reading this scope.
   SavedStreamPosition SavedPosition(DeclsCursor);
   DeclsCursor.JumpToBit(Loc);

   ASTRecordReader Record(*this);

   unsigned Code = DeclsCursor.ReadCode();
   auto Kind = Record.readRecord(DeclsCursor, Code);

   ASTScopeReader Reader(Record);
   auto *S = Reader.readScope(static_cast<Scope::TypeID>(Kind));

   LoadedScopes[Index] = S;
   return S;
}