
#include "Statement.h"

#include "ASTContext.h"
#include "Decl.h"
#include "Expression.h"
#include "PrettyPrinter.h"

#include <llvm/Support/raw_ostream.h>

namespace cdot {
namespace ast {

void Statement::dumpFlags() const
{
   printFlags(llvm::errs());
}

void Statement::printFlags(llvm::raw_ostream &OS) const
{
   OS << "TypeDependent = " << (isTypeDependent() ? "true" : "false") << "\n";
   OS << "ValueDependent = " << (isValueDependent() ? "true" : "false") << "\n";
   OS << "HadError = " << (isInvalid() ? "true" : "false") << "\n";
   OS << "SemaChecked = "<<(isSemanticallyChecked() ? "true" : "false") << "\n";
   OS << "GlobalInit = " << (isGlobalInitializer() ? "true" : "false") << "\n";
   OS << "ContainsUnexpandedPack = "
      << (containsUnexpandedParameterPack() ? "true" : "false") << "\n";
}

bool Statement::isTypeDependent() const
{
   return false;
}

bool Statement::isValueDependent() const
{
   return false;
}

bool Statement::needsInstantiation() const
{
//   static constexpr unsigned Mask =
//      Statement::ContainsGenericParam
//      | Statement::ContainsAssociatedType
//      | Statement::TypeDependent
//      | Statement::ValueDependent;

   static constexpr unsigned Mask = Statement::NeedsInstantiation;
   return (SubclassData & Mask) != 0;
}

void Statement::copyStatusFlags(Statement *Stmt)
{
   static unsigned StatusFlags =
      TypeDependent
      | ValueDependent
      | HadError
      | ContainsGenericParam
      | ContainsAssociatedType
      | ContainsUnexpandedPack
      | NeedsInstantiation;

   // if any sub statement is dependent or had an error, this stmt is also
   // dependent (or had an error)
   SubclassData |= (Stmt->SubclassData & StatusFlags);
}

void Statement::copyStatusFlags(Decl *D)
{
   static uint32_t mask = Decl::StatusFlags;
   SubclassData |= (D->getFlags() & mask);
}

SourceRange Statement::getSourceRange() const
{
   // FIXME
//   if constexpr (&Statement::getSourceRange == &SubClass::getSourceRange)
//      return SourceRange(loc);
//   else
//      return support::cast<SubClass>(this)->getSourceRange();

   switch (typeID) {
#  define CDOT_ASTNODE(SubClass)                                              \
   case SubClass##ID:                                                         \
      static_assert(&Statement::getSourceRange != &SubClass::getSourceRange,  \
                    "getSourceRange not implemented by " #SubClass);          \
      return support::cast<SubClass>(this)->getSourceRange();
#  include "AST/AstNode.def"

   default:
      llvm_unreachable("not a statement");
   }
}

void Statement::print(llvm::raw_ostream &OS) const
{
   PrettyPrinter PP(OS);
   PP.print(this);
}

void Statement::dump() const
{
   print(llvm::outs());
}

DeclStmt::DeclStmt(Decl *D)
   : Statement(DeclStmtID), D(D)
{
}

DeclStmt::DeclStmt(EmptyShell Empty)
   : Statement(DeclStmtID), D(nullptr)
{

}

DeclStmt* DeclStmt::Create(ASTContext &C, Decl *D)
{
   return new(C) DeclStmt(D);
}

SourceRange DeclStmt::getSourceRange() const
{
   return D->getSourceRange();
}

AttributedStmt::AttributedStmt(Statement *Stmt,
                               llvm::ArrayRef<Attr *> Attrs)
   : Statement(AttributedStmtID),
     Stmt(Stmt), NumAttrs((unsigned)Attrs.size())
{
   std::copy(Attrs.begin(), Attrs.end(), getTrailingObjects<Attr*>());
}

AttributedStmt::AttributedStmt(EmptyShell, unsigned N)
   : Statement(AttributedStmtID),
     Stmt(nullptr), NumAttrs(N)
{

}

AttributedStmt* AttributedStmt::Create(ASTContext &Ctx,
                                       Statement *Stmt,
                                       llvm::ArrayRef<Attr *> Attrs) {
   void *Mem = Ctx.Allocate(totalSizeToAlloc<Attr*>(Attrs.size()),
                            alignof(AttributedStmt));

   return new(Mem) AttributedStmt(Stmt, Attrs);
}

AttributedStmt* AttributedStmt::CreateEmpty(ASTContext &C,
                                            unsigned N) {
   void *Mem = C.Allocate(totalSizeToAlloc<Attr*>(N),
                          alignof(AttributedStmt));

   return new(Mem) AttributedStmt(EmptyShell(), N);
}

DebugStmt::DebugStmt(EmptyShell)
   : Statement(DebugStmtID)
{}

NullStmt::NullStmt(SourceLocation Loc)
   : Statement(NullStmtID), Loc(Loc)
{

}

NullStmt::NullStmt(EmptyShell)
   : Statement(NullStmtID)
{

}

NullStmt* NullStmt::Create(ASTContext &C, SourceLocation Loc)
{
   return new(C) NullStmt(Loc);
}

CompoundStmt::CompoundStmt(bool preservesScope,
                           SourceLocation LBraceLoc,
                           SourceLocation RBraceLoc)
   : CompoundStmt({}, preservesScope, LBraceLoc, RBraceLoc)
{
}

CompoundStmt::CompoundStmt(llvm::ArrayRef<Statement* > stmts,
                           bool preserveScope,
                           SourceLocation LBraceLoc,
                           SourceLocation RBraceLoc)
   : Statement(CompoundStmtID),
     numStmts(unsigned(stmts.size())), preserveScope(preserveScope),
     ContainsDeclStmt(false),
     LBraceLoc(LBraceLoc), RBraceLoc(RBraceLoc)
{
   std::copy(stmts.begin(), stmts.end(), begin());
}

CompoundStmt::CompoundStmt(EmptyShell, unsigned N)
   : Statement(CompoundStmtID),
   numStmts(N), preserveScope(false), ContainsDeclStmt(false)
{}

CompoundStmt* CompoundStmt::Create(ASTContext &ASTCtx,
                                   bool preserveScope,
                                   SourceLocation LBraceLoc,
                                   SourceLocation RBraceLoc) {
   return Create(ASTCtx, {}, preserveScope, LBraceLoc, RBraceLoc);
}

CompoundStmt* CompoundStmt::Create(ASTContext &ASTCtx,
                                   llvm::ArrayRef<Statement *> stmts,
                                   bool preserveScope,
                                   SourceLocation LBraceLoc,
                                   SourceLocation RBraceLoc) {
   void *Mem = ASTCtx.Allocate(totalSizeToAlloc<Statement*>(stmts.size()),
                               alignof(CompoundStmt));

   return new(Mem) CompoundStmt(stmts, preserveScope, LBraceLoc, RBraceLoc);
}

CompoundStmt* CompoundStmt::CreateEmpty(ASTContext &C, unsigned N)
{
   void *Mem = C.Allocate(totalSizeToAlloc<Statement*>(N),
                          alignof(CompoundStmt));

   return new(Mem) CompoundStmt(EmptyShell(), N);
}

BreakStmt::BreakStmt(SourceLocation Loc, IdentifierInfo *Label)
   : Statement(BreakStmtID), Loc(Loc), Label(Label)
{
   
}

BreakStmt::BreakStmt(EmptyShell)
   : Statement(BreakStmtID)
{}

BreakStmt* BreakStmt::Create(ASTContext &C, SourceLocation Loc,
                             IdentifierInfo *Label) {
   return new(C) BreakStmt(Loc, Label);
}

ContinueStmt::ContinueStmt(SourceLocation Loc, IdentifierInfo *Label)
   : Statement(ContinueStmtID), Loc(Loc), Label(Label)
{

}

ContinueStmt::ContinueStmt(EmptyShell)
   : Statement(ContinueStmtID)
{}

ContinueStmt* ContinueStmt::Create(ASTContext &C, SourceLocation Loc,
                                   IdentifierInfo *Label) {
   return new(C) ContinueStmt(Loc, Label);
}

IfStmt::IfStmt(SourceLocation IfLoc,
               ArrayRef<IfCondition> Conditions,
               Statement* body,
               Statement* elseBody,
               IdentifierInfo *Label)
   : Statement(IfStmtID),
     IfLoc(IfLoc),
     NumConditions(Conditions.size()), ifBranch(body), elseBranch(elseBody),
     Label(Label)
{
   std::copy(Conditions.begin(), Conditions.end(),
             getTrailingObjects<IfCondition>());
}

IfStmt::IfStmt(EmptyShell, unsigned N)
   : Statement(IfStmtID),
     NumConditions(N), ifBranch(nullptr), elseBranch(nullptr)
{

}

IfStmt* IfStmt::CreateEmpty(ASTContext &C, unsigned N)
{
   void *Mem = C.Allocate(totalSizeToAlloc<IfCondition>(N), alignof(IfStmt));
   return new(Mem) IfStmt(EmptyShell(), N);
}

IfStmt* IfStmt::Create(ASTContext &C, SourceLocation IfLoc,
                       ArrayRef<IfCondition> Conditions,
                       Statement *body,
                       Statement *elseBody,
                       IdentifierInfo *Label) {
   void *Mem = C.Allocate(totalSizeToAlloc<IfCondition>(Conditions.size()),
                                                        alignof(IfStmt));

   return new(Mem) IfStmt(IfLoc, Conditions, body, elseBody, Label);
}

WhileStmt::WhileStmt(SourceLocation WhileLoc,
                     ArrayRef<IfCondition> Conditions,
                     Statement *body,
                     IdentifierInfo *Label,
                     bool atLeastOnce)
   : Statement(WhileStmtID),
     WhileLoc(WhileLoc), NumConditions((unsigned)Conditions.size()),
     body(body), atLeastOnce(atLeastOnce),
     Label(Label)
{
   std::copy(Conditions.begin(), Conditions.end(),
             getTrailingObjects<IfCondition>());
}

WhileStmt::WhileStmt(EmptyShell, unsigned N)
   : Statement(WhileStmtID),
     NumConditions(N), body(nullptr), atLeastOnce(false)
{}

WhileStmt* WhileStmt::CreateEmpty(ASTContext &C, unsigned N)
{
   void *Mem = C.Allocate(totalSizeToAlloc<IfCondition>(N), alignof(WhileStmt));
   return new(Mem) WhileStmt(EmptyShell(), N);
}

WhileStmt* WhileStmt::Create(ASTContext &C,
                             SourceLocation WhileLoc,
                             ArrayRef<IfCondition> Conditions,
                             Statement *body,
                             IdentifierInfo *Label,
                             bool atLeastOnce) {
   void *Mem = C.Allocate(totalSizeToAlloc<IfCondition>(Conditions.size()),
                          alignof(WhileStmt));

   return new(Mem) WhileStmt(WhileLoc, Conditions, body, Label, atLeastOnce);
}

ForStmt::ForStmt(SourceLocation ForLoc,
                 Statement* init, Expression* term,
                 Statement* inc, Statement* body,
                 IdentifierInfo *Label)
   : Statement(ForStmtID),
     ForLoc(ForLoc),
     initialization(init), termination(term), increment(inc), body(body),
     Label(Label)
{

}

ForStmt::ForStmt(EmptyShell)
   : Statement(ForStmtID),
     initialization(nullptr), termination(nullptr),
     increment(nullptr), body(nullptr)
{}

ForStmt* ForStmt::Create(ASTContext &C, SourceLocation ForLoc,
                         Statement *init, Expression *term, Statement *inc,
                         Statement *body,
                         IdentifierInfo *Label) {
   return new(C) ForStmt(ForLoc, init, term, inc, body, Label);
}

ForInStmt::ForInStmt(SourceLocation ForLoc,
                     Decl* decl,
                     Expression* range,
                     Statement* body,
                     IdentifierInfo *Label)
   : Statement(ForInStmtID),
     ForLoc(ForLoc), decl(decl), rangeExpr(range), body(body), Label(Label)
{

}

ForInStmt::ForInStmt(EmptyShell)
   : Statement(ForInStmtID),
     decl(nullptr), rangeExpr(nullptr), body(nullptr)
{}

ForInStmt* ForInStmt::Create(ASTContext &C,
                             SourceLocation ForLoc,
                             Decl *decl,
                             Expression *range,
                             Statement *body,
                             IdentifierInfo *Label) {
   return new(C) ForInStmt(ForLoc, decl, range, body, Label);
}

CaseStmt::CaseStmt(SourceLocation CaseLoc,
                   PatternExpr *pattern, Statement *body)
   : Statement(CaseStmtID),
     CaseLoc(CaseLoc), pattern(pattern), body(body)
{

}

CaseStmt::CaseStmt(EmptyShell)
   : Statement(CaseStmtID),
   pattern(nullptr), body(nullptr)
{}

CaseStmt* CaseStmt::Create(ASTContext &C,
                           SourceLocation CaseLoc,
                           PatternExpr *pattern,
                           Statement *body) {
   return new(C) CaseStmt(CaseLoc, pattern, body);
}

SourceRange CaseStmt::getSourceRange() const
{
   if (!body)
      return SourceRange(CaseLoc, pattern->getSourceRange().getEnd());

   return SourceRange(CaseLoc, body->getSourceRange().getEnd());
}

MatchStmt::MatchStmt(SourceLocation MatchLoc, SourceRange Braces,
                     Expression *switchVal,
                     llvm::ArrayRef<CaseStmt*> cases,
                     IdentifierInfo *Label)
   : Statement(MatchStmtID),
     MatchLoc(MatchLoc), Braces(Braces),
     switchValue(switchVal), NumCases((unsigned)cases.size()),
     HasDefault(false), HasMutableCaseArg(false), IntegralSwitch(false),
     Label(Label)
{
   std::copy(cases.begin(), cases.end(), getTrailingObjects<CaseStmt*>());
}

MatchStmt::MatchStmt(EmptyShell, unsigned N)
   : Statement(MatchStmtID),
     switchValue(nullptr), NumCases(N),
     HasDefault(false), HasMutableCaseArg(false), IntegralSwitch(false)
{}

MatchStmt* MatchStmt::Create(ASTContext &C,
                             SourceLocation MatchLoc,
                             SourceRange Braces,
                             Expression *switchVal,
                             llvm::ArrayRef<CaseStmt *> cases,
                             IdentifierInfo *Label) {
   void *Mem = C.Allocate(totalSizeToAlloc<CaseStmt*>(cases.size()),
                          alignof(MatchStmt));

   return new(Mem) MatchStmt(MatchLoc, Braces, switchVal, cases, Label);
}

MatchStmt* MatchStmt::CreateEmpty(ASTContext &C, unsigned N)
{
   void *Mem = C.Allocate(totalSizeToAlloc<CaseStmt*>(N),
                          alignof(MatchStmt));

   return new(Mem) MatchStmt(EmptyShell(), N);
}

ReturnStmt::ReturnStmt(SourceLocation RetLoc, Expression *val)
   : Statement(ReturnStmtID),
     RetLoc(RetLoc), returnValue(val)
{

}

ReturnStmt::ReturnStmt(EmptyShell)
   : Statement(ReturnStmtID),
   returnValue(nullptr)
{}

ReturnStmt* ReturnStmt::Create(ASTContext &C,
                               SourceLocation RetLoc,
                               Expression *val) {
   return new(C) ReturnStmt(RetLoc, val);
}

SourceRange ReturnStmt::getSourceRange() const
{
   return returnValue
          ? SourceRange(RetLoc, returnValue->getSourceRange().getEnd())
          : SourceRange(RetLoc);
}

DiscardAssignStmt::DiscardAssignStmt(SourceLocation UnderscoreLoc,
                                     SourceLocation EqualsLoc,
                                     Expression *RHS)
   : Statement(DiscardAssignStmtID),
     UnderscoreLoc(UnderscoreLoc), EqualsLoc(EqualsLoc), RHS(RHS)
{

}

DiscardAssignStmt::DiscardAssignStmt(EmptyShell)
   : Statement(DiscardAssignStmtID),
     UnderscoreLoc(), EqualsLoc(), RHS(nullptr)
{

}

DiscardAssignStmt* DiscardAssignStmt::Create(ASTContext &C,
                                             SourceLocation UnderscoreLoc,
                                             SourceLocation EqualsLoc,
                                             Expression *RHS) {
   return new(C) DiscardAssignStmt(UnderscoreLoc, EqualsLoc, RHS);
}

DiscardAssignStmt* DiscardAssignStmt::CreateEmpty(ASTContext &C)
{
   return new(C) DiscardAssignStmt(EmptyShell());
}

SourceRange DiscardAssignStmt::getSourceRange() const
{
   if (!RHS)
      return SourceRange(UnderscoreLoc, EqualsLoc);

   return SourceRange(UnderscoreLoc, RHS->getSourceRange().getEnd());
}

DoStmt::DoStmt(SourceRange SR, Statement* body,
               IdentifierInfo *Label)
   : Statement(DoStmtID),
     SR(SR), body(body), NumCatchBlocks(0), Label(Label)
{}

DoStmt::DoStmt(SourceRange SR,
               Statement* body,
               ArrayRef<CatchBlock> catchBlocks,
               IdentifierInfo *Label)
   : Statement(DoStmtID),
     SR(SR), body(body),
     NumCatchBlocks((unsigned)catchBlocks.size()),
     Label(Label)
{
   std::copy(catchBlocks.begin(), catchBlocks.end(),
             getTrailingObjects<CatchBlock>());
}

DoStmt::DoStmt(EmptyShell, unsigned N)
   : Statement(DoStmtID), body(nullptr), NumCatchBlocks(N)
{}

ThrowStmt::ThrowStmt(SourceLocation ThrowLoc, Expression* thrownVal)
   : Statement(ThrowStmtID),
     ThrowLoc(ThrowLoc), thrownVal(thrownVal), thrownType(nullptr)
{}

ThrowStmt::ThrowStmt(EmptyShell)
   : Statement(ThrowStmtID),
   thrownVal(nullptr)
{}

SourceRange ThrowStmt::getSourceRange() const
{
   return thrownVal
          ? SourceRange(ThrowLoc, thrownVal->getSourceRange().getEnd())
          : SourceRange(ThrowLoc);
}

MixinStmt::MixinStmt(SourceRange Parens, Expression *Expr)
   : Statement(MixinStmtID), Parens(Parens), Expr(Expr)
{

}

MixinStmt::MixinStmt(EmptyShell)
   : Statement(MixinStmtID), Expr(nullptr)
{}

MixinStmt* MixinStmt::Create(ASTContext &C,
                             SourceRange Parens,
                             Expression *Expr) {
   return new(C) MixinStmt(Parens, Expr);
}

StaticIfStmt::StaticIfStmt(SourceLocation StaticLoc,
                           SourceLocation IfLoc,
                           StaticExpr *condition,
                           Statement *ifBranch,
                           Statement *elseBranch)
   : Statement(StaticIfStmtID),
     StaticLoc(StaticLoc), IfLoc(IfLoc),
     condition(condition), ifBranch(ifBranch), elseBranch(elseBranch),
     Template(nullptr)
{}

StaticIfStmt::StaticIfStmt(EmptyShell)
   : Statement(StaticIfStmtID),
     condition(nullptr), ifBranch(nullptr), elseBranch(nullptr),
     Template(nullptr)
{}

StaticIfStmt* StaticIfStmt::Create(ASTContext &C,
                                   SourceLocation StaticLoc,
                                   SourceLocation IfLoc,
                                   StaticExpr *condition,
                                   Statement *ifBranch,
                                   Statement *elseBranch) {
   return new(C) StaticIfStmt(StaticLoc, IfLoc, condition, ifBranch,
                              elseBranch);
}

StaticIfStmt::StaticIfStmt(SourceLocation StaticLoc,
                           SourceLocation IfLoc,
                           StaticExpr *condition,
                           StaticIfStmt *Template)
   : Statement(StaticIfStmtID),
     StaticLoc(StaticLoc), IfLoc(IfLoc),
     condition(condition), ifBranch(nullptr), elseBranch(nullptr),
     Template(Template)
{

}

StaticIfStmt* StaticIfStmt::Create(ASTContext &C,
                                   SourceLocation StaticLoc,
                                   SourceLocation IfLoc,
                                   StaticExpr *condition,
                                   StaticIfStmt *Template) {
   return new(C) StaticIfStmt(StaticLoc, IfLoc, condition, Template);
}

SourceRange StaticIfStmt::getSourceRange() const
{
   if (Template)
      return Template->getSourceRange();

   return SourceRange(StaticLoc, elseBranch
                                 ? elseBranch->getSourceRange().getEnd()
                                 : ifBranch->getSourceRange().getEnd());
}

StaticForStmt::StaticForStmt(SourceLocation StaticLoc,
                             SourceLocation ForLoc,
                             IdentifierInfo *elementName,
                             StaticExpr *range,
                             Statement *body)
   : Statement(StaticForStmtID),
     StaticLoc(StaticLoc), ForLoc(ForLoc),
     elementName(elementName), range(range), body(body)
{}

StaticForStmt::StaticForStmt(EmptyShell)
   : Statement(StaticForStmtID),
     range(nullptr), body(nullptr)
{}

StaticForStmt* StaticForStmt::Create(ASTContext &C,
                                     SourceLocation StaticLoc,
                                     SourceLocation IfLoc,
                                     IdentifierInfo *elementName,
                                     StaticExpr *range,
                                     Statement *body) {
   return new(C) StaticForStmt(StaticLoc, IfLoc, elementName, range, body);
}

MacroExpansionStmt::MacroExpansionStmt(SourceRange SR,
                                       DeclarationName MacroName,
                                       Expression *ParentExpr,
                                       Delimiter Delim,
                                       llvm::ArrayRef<lex::Token> Toks)
   : Statement(MacroExpansionStmtID),
     SR(SR), Delim(Delim), MacroName(MacroName),
     NumTokens((unsigned)Toks.size()), ParentExpr(ParentExpr)
{
   std::copy(Toks.begin(), Toks.end(), getTrailingObjects<lex::Token>());
}

MacroExpansionStmt::MacroExpansionStmt(EmptyShell, unsigned N)
   : Statement(MacroExpansionStmtID),
     Delim(Delimiter::Paren), NumTokens(N), ParentExpr(nullptr)
{}

MacroExpansionStmt* MacroExpansionStmt::Create(ASTContext &C,
                                               SourceRange SR,
                                               DeclarationName MacroName,
                                               Expression *ParentExpr,
                                               Delimiter Delim,
                                               llvm::ArrayRef<lex::Token> Toks){
   void *Mem = C.Allocate(totalSizeToAlloc<lex::Token>(Toks.size()),
                          alignof(MacroExpansionStmt));

   return new(Mem) MacroExpansionStmt(SR, MacroName, ParentExpr, Delim, Toks);
}

MacroExpansionStmt* MacroExpansionStmt::CreateEmpty(ASTContext &C,
                                                    unsigned N) {
   void *Mem = C.Allocate(totalSizeToAlloc<lex::Token>(N),
                          alignof(MacroExpansionStmt));

   return new(Mem) MacroExpansionStmt(EmptyShell(), N);
}

} // namespace ast
} // namespace cdot