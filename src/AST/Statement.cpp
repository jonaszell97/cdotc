
#include "Statement.h"
#include "Decl.h"

#include "AST/ASTContext.h"
#include "AST/Decl.h"
#include "AST/Expression.h"
#include "AST/Statement.h"

namespace cdot {
namespace ast {

void Statement::copyStatusFlags(Statement *Stmt)
{
   // if any sub statement is dependent or had an error, this stmt is also
   // dependent (or had an error)
   SubclassData |= (Stmt->SubclassData & StatusFlags);

   // if any sub statement was not fully semantically checked, this stmt
   // was not fully checked either
   if ((Stmt->SubclassData & SemanticallyChecked) == 0)
      SubclassData &= ~SemanticallyChecked;
}

void Statement::copyStatusFlags(Decl *D)
{
   static uint32_t mask = Decl::StatusFlags;
   SubclassData |= (D->getFlags() & mask);

   if ((D->getFlags() & Decl::DF_SemanticallyChecked) == 0)
      SubclassData &= ~SemanticallyChecked;
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

DeclStmt::DeclStmt(Decl *D)
   : Statement(DeclStmtID), D(D)
{}

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
   : Statement(AttributedStmtID), Stmt(Stmt)
{
   std::copy(Attrs.begin(), Attrs.end(), getTrailingObjects<Attr*>());
}

AttributedStmt* AttributedStmt::Create(ASTContext &Ctx,
                                       Statement *Stmt,
                                       llvm::ArrayRef<Attr *> Attrs) {
   void *Mem = Ctx.Allocate(totalSizeToAlloc<Attr*>(Attrs.size()),
                            alignof(AttributedStmt));

   return new(Mem) AttributedStmt(Stmt, Attrs);
}

NullStmt::NullStmt(SourceLocation Loc)
   : Statement(NullStmtID), Loc(Loc)
{

}

NullStmt* NullStmt::Create(ASTContext &C, SourceLocation Loc)
{
   return new(C) NullStmt(Loc);
}

UsingStmt::UsingStmt(SourceRange Loc,
                     llvm::ArrayRef<IdentifierInfo *> declContextSpecifier,
                     llvm::ArrayRef<IdentifierInfo *> importedItems,
                     bool wildCardImport)
   : Statement(UsingStmtID), Loc(Loc),
     NumSpecifierNames((unsigned)declContextSpecifier.size()),
     NumItems((unsigned)importedItems.size()),
     IsWildCard(wildCardImport)
{
   std::copy(declContextSpecifier.begin(), declContextSpecifier.end(),
             getTrailingObjects<IdentifierInfo*>());
   std::copy(importedItems.begin(), importedItems.end(),
             getTrailingObjects<IdentifierInfo*>() + NumSpecifierNames);
}

UsingStmt* UsingStmt::Create(ASTContext &C, SourceRange Loc,
                             llvm::ArrayRef<IdentifierInfo *> declContextSpecifier,
                             llvm::ArrayRef<IdentifierInfo *> importedItems,
                             bool wildCardImport) {
   void *Mem = C.Allocate(
      totalSizeToAlloc<IdentifierInfo*>(
         declContextSpecifier.size() + importedItems.size()),
      alignof(UsingStmt));

   return new(Mem) UsingStmt(Loc, declContextSpecifier, importedItems,
                             wildCardImport);
}

ImportStmt::ImportStmt(SourceRange Loc,
                       llvm::ArrayRef<IdentifierInfo *> moduleName)
   : Statement(ImportStmtID), Loc(Loc),
     NumNameQuals((unsigned)moduleName.size())
{
   std::copy(moduleName.begin(), moduleName.end(),
             getTrailingObjects<IdentifierInfo*>());
}

ImportStmt* ImportStmt::Create(ASTContext &C, SourceRange Loc,
                               llvm::ArrayRef<IdentifierInfo *> moduleName) {
   void *Mem = C.Allocate(totalSizeToAlloc<IdentifierInfo*>(moduleName.size()),
                          alignof(ImportStmt));

   return new(Mem) ImportStmt(Loc, moduleName);
}

ModuleStmt::ModuleStmt(SourceRange Loc,
                       llvm::ArrayRef<IdentifierInfo *> moduleName)
   : Statement(ModuleStmtID), Loc(Loc),
     NumNameQuals((unsigned)moduleName.size())
{
   std::copy(moduleName.begin(), moduleName.end(),
             getTrailingObjects<IdentifierInfo*>());
}

ModuleStmt* ModuleStmt::Create(ASTContext &C, SourceRange Loc,
                               llvm::ArrayRef<IdentifierInfo *> moduleName) {
   void *Mem = C.Allocate(totalSizeToAlloc<IdentifierInfo*>(moduleName.size()),
                          alignof(ModuleStmt));

   return new(Mem) ModuleStmt(Loc, moduleName);
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
     LBraceLoc(LBraceLoc), RBraceLoc(RBraceLoc)
{
   std::copy(stmts.begin(), stmts.end(), begin());
}

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

BreakStmt::BreakStmt(SourceLocation Loc)
   : Statement(BreakStmtID), Loc(Loc)
{
   
}

BreakStmt* BreakStmt::Create(ASTContext &C, SourceLocation Loc)
{
   return new(C) BreakStmt(Loc);
}

ContinueStmt::ContinueStmt(SourceLocation Loc)
   : Statement(ContinueStmtID), Loc(Loc)
{

}

ContinueStmt* ContinueStmt::Create(ASTContext &C, SourceLocation Loc)
{
   return new(C) ContinueStmt(Loc);
}

GotoStmt::GotoStmt(SourceLocation Loc, IdentifierInfo *label)
   : Statement(GotoStmtID), Loc(Loc), label(label)
{
   
}

GotoStmt* GotoStmt::Create(ASTContext &C,
                           SourceLocation Loc,
                           IdentifierInfo *label) {
   return new(C) GotoStmt(Loc, label);
}

LabelStmt::LabelStmt(SourceLocation Loc, IdentifierInfo *label)
   : Statement(LabelStmtID), Loc(Loc), label(label)
{

}

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

IfStmt* IfStmt::Create(ASTContext &C, SourceLocation IfLoc,
                       Expression *cond, Statement *body,
                       Statement *elseBody) {
   return new(C) IfStmt(IfLoc, cond, body, elseBody);
}

ForStmt::ForStmt(SourceLocation ForLoc,
                 Statement* init, Expression* term,
                 Statement* inc, Statement* body)
   : Statement(ForStmtID),
     ForLoc(ForLoc),
     initialization(init), termination(term), increment(inc), body(body)
{

}

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

CaseStmt* CaseStmt::Create(ASTContext &C,
                           SourceLocation CaseLoc,
                           PatternExpr *pattern,
                           Statement *body) {
   return new(C) CaseStmt(CaseLoc, pattern, body);
}

MatchStmt::MatchStmt(SourceLocation MatchLoc, SourceRange Braces,
                     Expression *switchVal,
                     llvm::ArrayRef<CaseStmt*> cases)
   : Statement(MatchStmtID),
     Braces(Braces), switchValue(switchVal), NumCases((unsigned)cases.size())
{
   std::copy(cases.begin(), cases.end(), getTrailingObjects<CaseStmt*>());
}

MatchStmt* MatchStmt::Create(ASTContext &C,
                             SourceLocation MatchLoc,
                             SourceRange Braces,
                             Expression *switchVal,
                             llvm::ArrayRef<CaseStmt *> cases) {
   void *Mem = C.Allocate(totalSizeToAlloc<CaseStmt*>(cases.size()),
                          alignof(MatchStmt));

   return new(Mem) MatchStmt(MatchLoc, Braces, switchVal, cases);
}

ReturnStmt::ReturnStmt(SourceLocation RetLoc, Expression *val)
   : Statement(ReturnStmtID),
     RetLoc(RetLoc), returnValue(val)
{

}

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

TryStmt::TryStmt(SourceRange SR, Statement* body)
   : Statement(TryStmtID),
     SR(SR), body(body)
{}

TryStmt::TryStmt(SourceRange SR,
                 Statement* body,
                 std::vector<CatchBlock> &&catchBlocks,
                 Statement* finally)
   : Statement(TryStmtID),
     SR(SR), body(body),
     catchBlocks(std::move(catchBlocks)), finallyBlock(finally)
{}

ThrowStmt::ThrowStmt(SourceLocation ThrowLoc, Expression* thrownVal)
   : Statement(ThrowStmtID),
     ThrowLoc(ThrowLoc), thrownVal(thrownVal), thrownType(nullptr)
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

MixinStmt* MixinStmt::Create(ASTContext &C,
                             SourceRange Parens,
                             Expression *Expr) {
   return new(C) MixinStmt(Parens, Expr);
}

llvm::ArrayRef<VarDecl*> DestructuringDecl::getDecls() const
{
   if (auto LG = support::dyn_cast<LocalDestructuringDecl>(this))
      return LG->getDecls();

   return support::cast<GlobalDestructuringDecl>(this)->getDecls();
}

DestructuringDecl::DestructuringDecl(NodeType typeID,
                                     SourceRange SR,
                                     unsigned NumDecls,
                                     AccessModifier access,
                                     bool isConst,
                                     SourceType type,
                                     Expression *value)
   : Statement(typeID),
     SR(SR), access(access), IsConst(isConst), NumDecls(NumDecls),
     type(type), value(value)
{}

LocalDestructuringDecl::LocalDestructuringDecl(SourceRange SR,
                                               AccessModifier access,
                                               bool isConst,
                                               llvm::ArrayRef<VarDecl*> Decls,
                                               SourceType type,
                                               Expression *value)
   : DestructuringDecl(LocalDestructuringDeclID, SR, (unsigned)Decls.size(),
                       access, isConst, type, value)
{
   std::copy(Decls.begin(), Decls.end(), getTrailingObjects<VarDecl*>());
}

LocalDestructuringDecl*
LocalDestructuringDecl::Create(ASTContext &C,
                               SourceRange SR,
                               AccessModifier access,
                               bool isConst,
                               llvm::ArrayRef<VarDecl *> Decls,
                               SourceType type,
                               Expression *value) {
   void *Mem = C.Allocate(totalSizeToAlloc<VarDecl*>(Decls.size()),
                          alignof(LocalDestructuringDecl));

   return new(Mem) LocalDestructuringDecl(SR, access, isConst, Decls, type,
                                          value);
}

GlobalDestructuringDecl::GlobalDestructuringDecl(SourceRange SR,
                                                 AccessModifier access,
                                                 bool isConst,
                                                 llvm::ArrayRef<VarDecl*> Decls,
                                                 SourceType type,
                                                 Expression *value)
   : DestructuringDecl(GlobalDestructuringDeclID, SR, (unsigned)Decls.size(),
                       access, isConst, type, value)
{
   std::copy(Decls.begin(), Decls.end(), getTrailingObjects<VarDecl*>());
}

GlobalDestructuringDecl*
GlobalDestructuringDecl::Create(ASTContext &C,
                                SourceRange SR,
                                AccessModifier access,
                                bool isConst,
                                llvm::ArrayRef<VarDecl*> Decls,
                                SourceType type,
                                Expression *value) {
   void *Mem = C.Allocate(totalSizeToAlloc<VarDecl*>(Decls.size()),
                          alignof(GlobalDestructuringDecl));

   return new(Mem) GlobalDestructuringDecl(SR, access, isConst, Decls, type,
                                           value);
}

StaticIfStmt::StaticIfStmt(SourceLocation StaticLoc,
                           SourceLocation IfLoc,
                           StaticExpr *condition,
                           Statement *ifBranch,
                           Statement *elseBranch)
   : Statement(StaticIfStmtID),
     StaticLoc(StaticLoc), IfLoc(IfLoc),
     condition(condition), ifBranch(ifBranch), elseBranch(elseBranch)
{}

StaticForStmt::StaticForStmt(SourceLocation StaticLoc,
                             SourceLocation IfLoc,
                             std::string &&elementName,
                             StaticExpr *range,
                             Statement *body)
   : Statement(StaticForStmtID),
     elementName(move(elementName)), range(range), body(body)
{}

} // namespace ast
} // namespace cdot