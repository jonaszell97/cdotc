
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

void Statement::copyStatusFlags(Statement *Stmt)
{
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
                           SourceLocation RBraceLoc,
                           bool Unsafe)
   : CompoundStmt({}, preservesScope, LBraceLoc, RBraceLoc, Unsafe)
{
}

CompoundStmt::CompoundStmt(llvm::ArrayRef<Statement* > stmts,
                           bool preserveScope,
                           SourceLocation LBraceLoc,
                           SourceLocation RBraceLoc,
                           bool Unsafe)
   : Statement(CompoundStmtID),
     numStmts(unsigned(stmts.size())), preserveScope(preserveScope),
     Unsafe(Unsafe), ContainsDeclStmt(false),
     LBraceLoc(LBraceLoc), RBraceLoc(RBraceLoc)
{
   std::copy(stmts.begin(), stmts.end(), begin());
}

CompoundStmt::CompoundStmt(EmptyShell, unsigned N)
   : Statement(CompoundStmtID),
   numStmts(N), preserveScope(false), Unsafe(false), ContainsDeclStmt(false)
{}

CompoundStmt* CompoundStmt::Create(ASTContext &ASTCtx,
                                   bool preserveScope,
                                   SourceLocation LBraceLoc,
                                   SourceLocation RBraceLoc,
                                   bool Unsafe) {
   return Create(ASTCtx, {}, preserveScope, LBraceLoc, RBraceLoc, Unsafe);
}

CompoundStmt* CompoundStmt::Create(ASTContext &ASTCtx,
                                   llvm::ArrayRef<Statement *> stmts,
                                   bool preserveScope,
                                   SourceLocation LBraceLoc,
                                   SourceLocation RBraceLoc,
                                   bool Unsafe) {
   void *Mem = ASTCtx.Allocate(totalSizeToAlloc<Statement*>(stmts.size()),
                               alignof(CompoundStmt));

   return new(Mem) CompoundStmt(stmts, preserveScope, LBraceLoc, RBraceLoc,
                                Unsafe);
}

CompoundStmt* CompoundStmt::CreateEmpty(ASTContext &C, unsigned N)
{
   void *Mem = C.Allocate(totalSizeToAlloc<Statement*>(N),
                          alignof(CompoundStmt));

   return new(Mem) CompoundStmt(EmptyShell(), N);
}

BreakStmt::BreakStmt(SourceLocation Loc)
   : Statement(BreakStmtID), Loc(Loc)
{
   
}

BreakStmt::BreakStmt(EmptyShell)
   : Statement(BreakStmtID)
{}

BreakStmt* BreakStmt::Create(ASTContext &C, SourceLocation Loc)
{
   return new(C) BreakStmt(Loc);
}

ContinueStmt::ContinueStmt(SourceLocation Loc)
   : Statement(ContinueStmtID), Loc(Loc)
{

}

ContinueStmt::ContinueStmt(EmptyShell)
   : Statement(ContinueStmtID)
{}

ContinueStmt* ContinueStmt::Create(ASTContext &C, SourceLocation Loc)
{
   return new(C) ContinueStmt(Loc);
}

GotoStmt::GotoStmt(SourceLocation Loc, IdentifierInfo *label)
   : Statement(GotoStmtID), Loc(Loc), label(label)
{
   
}

GotoStmt::GotoStmt(EmptyShell)
   : Statement(GotoStmtID), label(nullptr)
{}

GotoStmt* GotoStmt::Create(ASTContext &C,
                           SourceLocation Loc,
                           IdentifierInfo *label) {
   return new(C) GotoStmt(Loc, label);
}

LabelStmt::LabelStmt(SourceLocation Loc, IdentifierInfo *label)
   : Statement(LabelStmtID), Loc(Loc), label(label)
{

}

LabelStmt::LabelStmt(EmptyShell)
   : Statement(LabelStmtID), label(nullptr)
{}

LabelStmt* LabelStmt::Create(ASTContext &C,
                             SourceLocation Loc,
                             IdentifierInfo *label) {
   return new(C) LabelStmt(Loc, label);
}

IfStmt::IfStmt(SourceLocation IfLoc,
               Expression* cond,
               Statement* body,
               Statement* elseBody)
   : Statement(IfStmtID),
     IfLoc(IfLoc),
     condition(cond), ifBranch(body), elseBranch(elseBody)
{

}

IfStmt::IfStmt(EmptyShell)
   : Statement(IfStmtID),
   condition(nullptr), ifBranch(nullptr), elseBranch(nullptr)
{}

IfStmt* IfStmt::Create(ASTContext &C, SourceLocation IfLoc,
                       Expression *cond, Statement *body,
                       Statement *elseBody) {
   return new(C) IfStmt(IfLoc, cond, body, elseBody);
}

IfLetStmt::IfLetStmt(SourceLocation IfLoc,
                     LocalVarDecl *VarDecl,
                     Statement *IfBranch,
                     Statement *ElseBranch)
   : Statement(IfLetStmtID),
     IfLoc(IfLoc), VarDecl(VarDecl), IfBranch(IfBranch), ElseBranch(ElseBranch)
{

}

IfLetStmt::IfLetStmt(EmptyShell)
   : Statement(IfLetStmtID),
     VarDecl(nullptr), IfBranch(nullptr), ElseBranch(nullptr)
{}

IfLetStmt* IfLetStmt::Create(ASTContext &C,
                             SourceLocation IfLoc,
                             LocalVarDecl *VarDecl,
                             Statement *IfBranch,
                             Statement *ElseBranch) {
   return new(C) IfLetStmt(IfLoc, VarDecl, IfBranch, ElseBranch);
}

IfCaseStmt::IfCaseStmt(SourceLocation IfLoc,
                       PatternExpr *Pattern,
                       Expression *Val,
                       Statement *IfBranch,
                       Statement *ElseBranch)
   : Statement(IfCaseStmtID),
     IfLoc(IfLoc), Pattern(Pattern), Val(Val),
     IfBranch(IfBranch), ElseBranch(ElseBranch)
{

}

IfCaseStmt::IfCaseStmt(EmptyShell)
   : Statement(IfCaseStmtID),
     Pattern(nullptr), Val(nullptr),
     IfBranch(nullptr), ElseBranch(nullptr)
{}

IfCaseStmt* IfCaseStmt::Create(ASTContext &C,
                               SourceLocation IfLoc,
                               PatternExpr *Pattern,
                               Expression *Val,
                               Statement *IfBranch,
                               Statement *ElseBranch) {
   return new(C) IfCaseStmt(IfLoc, Pattern, Val, IfBranch, ElseBranch);
}

ForStmt::ForStmt(SourceLocation ForLoc,
                 Statement* init, Expression* term,
                 Statement* inc, Statement* body)
   : Statement(ForStmtID),
     ForLoc(ForLoc),
     initialization(init), termination(term), increment(inc), body(body)
{

}

ForStmt::ForStmt(EmptyShell)
   : Statement(ForStmtID),
     initialization(nullptr), termination(nullptr),
     increment(nullptr), body(nullptr)
{}

ForStmt* ForStmt::Create(ASTContext &C, SourceLocation ForLoc,
                         Statement *init, Expression *term, Statement *inc,
                         Statement *body) {
   return new(C) ForStmt(ForLoc, init, term, inc, body);
}

ForInStmt::ForInStmt(SourceLocation ForLoc,
                     LocalVarDecl* decl,
                     Expression* range,
                     Statement* body)
   : Statement(ForInStmtID),
     ForLoc(ForLoc), decl(decl), rangeExpr(range), body(body)
{

}

ForInStmt::ForInStmt(EmptyShell)
   : Statement(ForInStmtID),
     decl(nullptr), rangeExpr(nullptr), body(nullptr)
{}

ForInStmt* ForInStmt::Create(ASTContext &C,
                             SourceLocation ForLoc,
                             LocalVarDecl *decl,
                             Expression *range,
                             Statement *body) {
   return new(C) ForInStmt(ForLoc, decl, range, body);
}

WhileStmt::WhileStmt(SourceLocation WhileLoc, Expression *cond,
                     Statement *body, bool atLeastOnce)
   : Statement(WhileStmtID),
     WhileLoc(WhileLoc), condition(cond), body(body), atLeastOnce(atLeastOnce)
{

}

WhileStmt::WhileStmt(EmptyShell)
   : Statement(WhileStmtID),
     condition(nullptr), body(nullptr), atLeastOnce(false)
{}

WhileStmt* WhileStmt::Create(ASTContext &C,
                             SourceLocation WhileLoc,
                             Expression *cond,
                             Statement *body, bool atLeastOnce) {
   return new(C) WhileStmt(WhileLoc, cond, body, atLeastOnce);
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
                     llvm::ArrayRef<CaseStmt*> cases)
   : Statement(MatchStmtID),
     MatchLoc(MatchLoc), Braces(Braces),
     switchValue(switchVal), NumCases((unsigned)cases.size())
{
   std::copy(cases.begin(), cases.end(), getTrailingObjects<CaseStmt*>());
}

MatchStmt::MatchStmt(EmptyShell, unsigned N)
   : Statement(MatchStmtID),
     switchValue(nullptr), NumCases(N)
{}

MatchStmt* MatchStmt::Create(ASTContext &C,
                             SourceLocation MatchLoc,
                             SourceRange Braces,
                             Expression *switchVal,
                             llvm::ArrayRef<CaseStmt *> cases) {
   void *Mem = C.Allocate(totalSizeToAlloc<CaseStmt*>(cases.size()),
                          alignof(MatchStmt));

   return new(Mem) MatchStmt(MatchLoc, Braces, switchVal, cases);
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

DoStmt::DoStmt(SourceRange SR, Statement* body)
   : Statement(DoStmtID),
     SR(SR), body(body), NumCatchBlocks(0)
{}

DoStmt::DoStmt(SourceRange SR,
               Statement* body,
               ArrayRef<CatchBlock> catchBlocks)
   : Statement(DoStmtID),
     SR(SR), body(body),
     NumCatchBlocks((unsigned)catchBlocks.size())
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
                                       Delimiter Delim,
                                       llvm::ArrayRef<lex::Token> Toks)
   : Statement(MacroExpansionStmtID),
     SR(SR), Delim(Delim), MacroName(MacroName),
     NumTokens((unsigned)Toks.size())
{
   std::copy(Toks.begin(), Toks.end(), getTrailingObjects<lex::Token>());
}

MacroExpansionStmt::MacroExpansionStmt(EmptyShell, unsigned N)
   : Statement(MacroExpansionStmtID),
     Delim(Delimiter::Paren), NumTokens(N)
{}

MacroExpansionStmt* MacroExpansionStmt::Create(ASTContext &C,
                                               SourceRange SR,
                                               DeclarationName MacroName,
                                               Delimiter Delim,
                                               llvm::ArrayRef<lex::Token> Toks){
   void *Mem = C.Allocate(totalSizeToAlloc<lex::Token>(Toks.size()),
                          alignof(MacroExpansionStmt));

   return new(Mem) MacroExpansionStmt(SR, MacroName, Delim, Toks);
}

MacroExpansionStmt* MacroExpansionStmt::CreateEmpty(ASTContext &C,
                                                    unsigned N) {
   void *Mem = C.Allocate(totalSizeToAlloc<lex::Token>(N),
                          alignof(MacroExpansionStmt));

   return new(Mem) MacroExpansionStmt(EmptyShell(), N);
}

} // namespace ast
} // namespace cdot