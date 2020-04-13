#ifndef STATEMENT_H
#define STATEMENT_H

#include "cdotc/AST/AstNode.h"
#include "cdotc/AST/ContinuationPoint.h"
#include "cdotc/AST/SourceType.h"
#include "cdotc/Basic/DeclarationName.h"
#include "cdotc/Lex/Token.h"
#include "cdotc/Sema/ConversionSequence.h"

#include <llvm/Support/TrailingObjects.h>

namespace cdot {

class BlockScope;

namespace ast {

class Decl;
class NamedDecl;
class LocalVarDecl;
class CallableDecl;
class VarDecl;
class Expression;
class CallableDecl;
class MethodDecl;
class Expression;
class PatternExpr;
class StaticExpr;

class LLVM_ALIGNAS(sizeof(void*)) Statement : public AstNode {
public:
   struct EmptyShell {
   };

   enum Flags : uint32_t {
      TypeDependent = 0x1,
      ContainsGenericParam = 0x2,
      ContainsAssociatedType = 0x4,
      ContainsUnexpandedPack = 0x8,
      ValueDependent = 0x10,
      HadError = 0x20,
      SemanticallyChecked = 0x40,
      GlobalInitializer = 0x80,
      Ignored = 0x100,
      Unsafe = 0x200,
      SpecializationCandidate = 0x400,
      NeedsInstantiation = 0x800,

      _lastFlag = SpecializationCandidate,
   };

   static_assert((int)ContainsGenericParam
                     == (int)TypeProperties::ContainsTemplateParamType,
                 "type and statement flags must match!");

   static_assert((int)ContainsAssociatedType
                     == (int)TypeProperties::ContainsAssociatedType,
                 "type and statement flags must match!");

   static_assert((int)ContainsUnexpandedPack
                     == (int)TypeProperties::ContainsUnexpandedParameterPack,
                 "type and statement flags must match!");

   static_assert(_lastFlag <= (1 << 23), "too many flags!");

   void dumpFlags() const;
   void printFlags(llvm::raw_ostream& OS) const;

   bool isDependent() const { return isTypeDependent() || isValueDependent(); }

   bool isTypeDependent() const;
   bool isValueDependent() const;

   bool containsUnexpandedParameterPack() const
   {
      return flagIsSet(ContainsUnexpandedPack);
   }

   void setIsTypeDependent(bool typeDependent)
   {
      setFlag(TypeDependent, typeDependent);
   }

   void setIsValueDependent(bool valueDependent)
   {
      setFlag(ValueDependent, valueDependent);
   }

   bool needsInstantiation() const;
   void setNeedsInstantiation(bool b) { setFlag(TypeDependent, b); }

   bool containsGenericParam() const { return flagIsSet(ContainsGenericParam); }
   void setContainsGenericParam(bool b) { setFlag(ContainsGenericParam, b); }

   bool containsAssociatedType() const
   {
      return flagIsSet(ContainsAssociatedType);
   }
   void setContainsAssociatedType(bool b)
   {
      setFlag(ContainsAssociatedType, b);
   }

   void setContainsUnexpandedParameterPack(bool unexp)
   {
      setFlag(ContainsUnexpandedPack, unexp);
   }

   bool isGlobalInitializer() const { return flagIsSet(GlobalInitializer); }

   void setIsGlobalInitializer(bool globalInit)
   {
      setFlag(GlobalInitializer, globalInit);
   }

   bool isInvalid() const { return flagIsSet(HadError); }
   void setIsInvalid(bool error);

   bool isUnsafe() const { return flagIsSet(Unsafe); }
   void setUnsafe(bool unsafe) { setFlag(Unsafe, unsafe); }

   bool isIgnored() const { return flagIsSet(Ignored); }
   void setIgnored(bool b) { setFlag(Ignored, b); }

   bool isSemanticallyChecked() const { return flagIsSet(SemanticallyChecked); }
   void setSemanticallyChecked(bool chk = true)
   {
      setFlag(SemanticallyChecked, chk);
   }

   bool isSpecializationCandidate() const
   {
      return flagIsSet(SpecializationCandidate);
   }

   void setSpecializationCandidate(bool V)
   {
      setFlag(SpecializationCandidate, V);
   }

   bool alreadyCheckedOrHasError() const
   {
      static uint32_t mask = HadError | SemanticallyChecked;
      return (SubclassData & mask) != 0;
   }

   void copyStatusFlags(Decl* D);
   void copyStatusFlags(Statement* Stmt);

   SourceRange getSourceRange() const;
   SourceLocation getSourceLoc() const { return getSourceRange().getStart(); }

   void print(llvm::raw_ostream& OS) const;
   void dump() const;

   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return true; }

protected:
   explicit Statement(NodeType typeID) : AstNode(typeID) {}
};

class DeclStmt : public Statement {
public:
   static DeclStmt* Create(ASTContext& C, Decl* D);
   DeclStmt(EmptyShell Empty);

   Decl* getDecl() const { return D; }
   void setDecl(Decl* D) { this->D = D; }

   SourceRange getSourceRange() const;

   static bool classofKind(NodeType kind) { return kind == DeclStmtID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

private:
   explicit DeclStmt(Decl* D);

   Decl* D;
};

class AttributedStmt final : public Statement,
                             llvm::TrailingObjects<AttributedStmt, Attr*> {
public:
   static bool classofKind(NodeType kind) { return kind == AttributedStmtID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static AttributedStmt* Create(ASTContext& Ctx, Statement* Stmt,
                                 llvm::ArrayRef<Attr*> Attrs);

   static AttributedStmt* CreateEmpty(ASTContext& C, unsigned NumAttrs);

   llvm::ArrayRef<Attr*> getAttributes() const
   {
      return {getTrailingObjects<Attr*>(), NumAttrs};
   }

   SourceRange getSourceRange() const { return Stmt->getSourceRange(); }

   Statement* getStatement() const { return Stmt; }
   void setStatement(Statement* S) { Stmt = S; }

   friend TrailingObjects;

private:
   AttributedStmt(Statement* Stmt, llvm::ArrayRef<Attr*> Attrs);
   AttributedStmt(EmptyShell, unsigned N);

   Statement* Stmt;
   unsigned NumAttrs;
};

class MixinStmt : public Statement {
   MixinStmt(SourceRange Parens, Expression* Expr);

   SourceRange Parens;
   Expression* Expr;

public:
   static bool classofKind(NodeType kind) { return kind == MixinStmtID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   SourceRange getSourceRange() const { return Parens; }
   Expression* getMixinExpr() const { return Expr; }
   void setMixinExpr(Expression* E) { Expr = E; }

   void setParens(const SourceRange& Parens) { MixinStmt::Parens = Parens; }

   static MixinStmt* Create(ASTContext& C, SourceRange Parens,
                            Expression* Expr);

   MixinStmt(EmptyShell Empty);
};

class DebugStmt : public Statement {
public:
   explicit DebugStmt(SourceLocation Loc, bool unreachable = false)
       : Statement(DebugStmtID), Loc(Loc), unreachable(unreachable)
   {
   }

   DebugStmt(EmptyShell Empty);

   bool isUnreachable() const { return unreachable; }
   void setLoc(const SourceLocation& Loc) { DebugStmt::Loc = Loc; }

private:
   SourceLocation Loc;
   bool unreachable;

public:
   static bool classofKind(NodeType kind) { return kind == DebugStmtID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   SourceRange getSourceRange() const
   {
      unsigned EndOffset = Loc.getOffset() + (unreachable ? 13 : 7);
      return SourceRange(Loc, SourceLocation(EndOffset));
   }
};

class NullStmt : public Statement {
   explicit NullStmt(SourceLocation Loc);

   SourceLocation Loc;

public:
   static bool classofKind(NodeType kind) { return kind == NullStmtID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static NullStmt* Create(ASTContext& C, SourceLocation Loc);
   NullStmt(EmptyShell Empty);

   SourceRange getSourceRange() const { return SourceRange(Loc); }
   void setLoc(const SourceLocation& Loc) { NullStmt::Loc = Loc; }
};

class CompoundStmt final : public Statement,
                           llvm::TrailingObjects<CompoundStmt, Statement*> {
public:
   static CompoundStmt* Create(ASTContext& ASTCtx, bool preserveScope,
                               SourceLocation LBraceLoc,
                               SourceLocation RBraceLoc);

   static CompoundStmt* Create(ASTContext& ASTCtx,
                               llvm::ArrayRef<Statement*> stmts,
                               bool preserveScope, SourceLocation LBraceLoc,
                               SourceLocation RBraceLoc);

   static CompoundStmt* CreateEmpty(ASTContext& C, unsigned N);

   using stmt_iterator = Statement**;
   using iterator_range = llvm::MutableArrayRef<Statement*>;
   using const_iterator_range = llvm::ArrayRef<Statement*>;

   size_t size() const { return numStmts; }
   stmt_iterator begin() { return getTrailingObjects<Statement*>(); }
   stmt_iterator end() { return begin() + size(); }

   iterator_range getStatements() { return iterator_range(begin(), size()); }
   const_iterator_range getStatements() const
   {
      return const_iterator_range(getTrailingObjects<Statement*>(), size());
   }

   bool preservesScope() const { return preserveScope; }
   void setPreserveScope(bool preserve) { preserveScope = preserve; }

   SourceRange getSourceRange() const
   {
      return SourceRange(LBraceLoc, RBraceLoc);
   }

   SourceLocation getLBraceLoc() const { return LBraceLoc; }
   SourceLocation getRBraceLoc() const { return RBraceLoc; }

   void setLBraceLoc(SourceLocation Loc) { LBraceLoc = Loc; }
   void setRBraceLoc(SourceLocation Loc) { RBraceLoc = Loc; }

   unsigned getScopeID() const { return ScopeID; }
   void setScopeID(unsigned V) { ScopeID = V; }

   bool containsDeclStmt() const { return ContainsDeclStmt; }
   void setContainsDeclStmt(bool V) { ContainsDeclStmt = V; }

   static bool classofKind(NodeType kind) { return kind == CompoundStmtID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   friend TrailingObjects;

private:
   CompoundStmt(bool preservesScope, SourceLocation LBraceLoc,
                SourceLocation RBraceLoc);

   CompoundStmt(llvm::ArrayRef<Statement*> stmts, bool preserveScope,
                SourceLocation LBraceLoc, SourceLocation RBraceLoc);

   CompoundStmt(EmptyShell Empty, unsigned N);

   unsigned numStmts : 30;
   bool preserveScope : 1;
   bool ContainsDeclStmt : 1;

   unsigned ScopeID = 0;

   SourceLocation LBraceLoc;
   SourceLocation RBraceLoc;
};

class BreakStmt : public Statement {
   explicit BreakStmt(SourceLocation Loc, IdentifierInfo* Label);

   SourceLocation Loc;
   IdentifierInfo* Label;

public:
   static bool classofKind(NodeType kind) { return kind == BreakStmtID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static BreakStmt* Create(ASTContext& C, SourceLocation Loc,
                            IdentifierInfo* Label);

   BreakStmt(EmptyShell Empty);

   SourceRange getSourceRange() const { return SourceRange(Loc); }
   void setLoc(SourceLocation L) { Loc = L; }

   IdentifierInfo* getLabel() const { return Label; }
   void setLabel(IdentifierInfo* V) { Label = V; }
};

class ContinueStmt : public Statement {
   explicit ContinueStmt(SourceLocation Loc, IdentifierInfo* Label);

   SourceLocation Loc;
   IdentifierInfo* Label;

public:
   static bool classofKind(NodeType kind) { return kind == ContinueStmtID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static ContinueStmt* Create(ASTContext& C, SourceLocation Loc,
                               IdentifierInfo* Label);

   ContinueStmt(EmptyShell Empty);

   SourceRange getSourceRange() const { return SourceRange(Loc); }
   void setLoc(SourceLocation L) { Loc = L; }

   IdentifierInfo* getLabel() const { return Label; }
   void setLabel(IdentifierInfo* V) { Label = V; }
};

struct IfCondition {
   enum Kind {
      Expression,
      Binding,
      Pattern,
   };

   IfCondition(ast::Expression* E) : K(Expression), ExprData{E} {}

   IfCondition(LocalVarDecl* D, CallableDecl* TryUnwrapFn = nullptr,
               CallableDecl* HasValueFn = nullptr)
       : K(Binding), BindingData{D, TryUnwrapFn, HasValueFn}
   {
   }

   IfCondition(PatternExpr* Pat, ast::Expression* E = nullptr)
       : K(Pattern), PatternData{Pat, E}
   {
   }

   Kind K;

   struct ExprDataType {
      ast::Expression* Expr = nullptr;
      CallableDecl* TruthValueFn = nullptr;
   };

   struct BindingDataType {
      LocalVarDecl* Decl = nullptr;
      CallableDecl* TryUnwrapFn = nullptr;
      CallableDecl* HasValueFn = nullptr;
   };

   struct PatternDataType {
      PatternExpr* Pattern = nullptr;
      ast::Expression* Expr = nullptr;
   };

   union {
      ExprDataType ExprData;
      BindingDataType BindingData;
      PatternDataType PatternData;
   };
};

class IfStmt final : public Statement, TrailingObjects<IfStmt, IfCondition> {
   IfStmt(SourceLocation IfLoc, ArrayRef<IfCondition> Conditions,
          Statement* body, Statement* elseBody, IdentifierInfo* Label);

   IfStmt(EmptyShell Empty, unsigned N);

   SourceLocation IfLoc;

   unsigned NumConditions;
   Statement* ifBranch;
   Statement* elseBranch;

   IdentifierInfo* Label = nullptr;

public:
   static bool classofKind(NodeType kind) { return kind == IfStmtID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   friend TrailingObjects;

   static IfStmt* Create(ASTContext& C, SourceLocation IfLoc,
                         ArrayRef<IfCondition> Conditions, Statement* body,
                         Statement* elseBody, IdentifierInfo* Label);

   static IfStmt* CreateEmpty(ASTContext& C, unsigned N);

   SourceRange getSourceRange() const
   {
      return SourceRange(IfLoc, elseBranch
                                    ? elseBranch->getSourceRange().getEnd()
                                    : ifBranch->getSourceRange().getEnd());
   }

   void setIfLoc(SourceLocation L) { IfLoc = L; }

   Statement* getIfBranch() const { return ifBranch; }
   Statement* getElseBranch() const { return elseBranch; }

   void setIfBranch(Statement* If) { ifBranch = If; }
   void setElseBranch(Statement* Else) { elseBranch = Else; }

   ArrayRef<IfCondition> getConditions() const
   {
      return {getTrailingObjects<IfCondition>(), NumConditions};
   }

   MutableArrayRef<IfCondition> getConditions()
   {
      return {getTrailingObjects<IfCondition>(), NumConditions};
   }

   IdentifierInfo* getLabel() const { return Label; }
   void setLabel(IdentifierInfo* V) { Label = V; }
};

class WhileStmt final : public Statement,
                        TrailingObjects<WhileStmt, IfCondition> {
   WhileStmt(SourceLocation WhileLoc, ArrayRef<IfCondition> Conditions,
             Statement* body, IdentifierInfo* Label, bool atLeastOnce);

   WhileStmt(EmptyShell Empty, unsigned N);

   SourceLocation WhileLoc;
   unsigned NumConditions;
   Statement* body;
   bool atLeastOnce;
   IdentifierInfo* Label = nullptr;

public:
   static bool classofKind(NodeType kind) { return kind == WhileStmtID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   friend TrailingObjects;

   static WhileStmt* Create(ASTContext& C, SourceLocation WhileLoc,
                            ArrayRef<IfCondition> Conditions, Statement* body,
                            IdentifierInfo* Label, bool atLeastOnce = false);

   static WhileStmt* CreateEmpty(ASTContext& C, unsigned N);

   SourceRange getSourceRange() const
   {
      return SourceRange(WhileLoc, body->getSourceRange().getEnd());
   }

   void setWhileLoc(SourceLocation L) { WhileLoc = L; }

   Statement* getBody() const { return body; }
   void setBody(Statement* B) { body = B; }

   bool isAtLeastOnce() const { return atLeastOnce; }
   void setAtLeastOnce(bool b) { atLeastOnce = b; }

   ArrayRef<IfCondition> getConditions() const
   {
      return {getTrailingObjects<IfCondition>(), NumConditions};
   }

   MutableArrayRef<IfCondition> getConditions()
   {
      return {getTrailingObjects<IfCondition>(), NumConditions};
   }

   IdentifierInfo* getLabel() const { return Label; }
   void setLabel(IdentifierInfo* V) { Label = V; }
};

class ForStmt : public Statement {
   ForStmt(SourceLocation ForLoc, Statement* init, Expression* term,
           Statement* inc, Statement* body, IdentifierInfo* Label);

   SourceLocation ForLoc;
   Statement* initialization;
   Expression* termination;
   Statement* increment;
   Statement* body;
   IdentifierInfo* Label = nullptr;

public:
   static bool classofKind(NodeType kind) { return kind == ForStmtID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static ForStmt* Create(ASTContext& C, SourceLocation ForLoc, Statement* init,
                          Expression* term, Statement* inc, Statement* body,
                          IdentifierInfo* Label);

   ForStmt(EmptyShell Empty);

   SourceRange getSourceRange() const
   {
      return SourceRange(ForLoc, body->getSourceRange().getEnd());
   }

   void setForLoc(SourceLocation L) { ForLoc = L; }

   Statement* getInitialization() const { return initialization; }
   void setInitialization(Statement* Init) { initialization = Init; }

   Expression* getTermination() const { return termination; }
   void setTermination(Expression* Term) { termination = Term; }

   Statement* getIncrement() const { return increment; }
   void setIncrement(Statement* Inc) { increment = Inc; }

   Statement* getBody() const { return body; }
   void setBody(Statement* S) { body = S; }

   IdentifierInfo* getLabel() const { return Label; }
   void setLabel(IdentifierInfo* V) { Label = V; }
};

class ForInStmt : public Statement {
   ForInStmt(SourceLocation ForLoc, Decl* decl, Expression* range,
             Statement* body, IdentifierInfo* Label);

   SourceLocation ForLoc;
   Decl* decl;
   Expression* rangeExpr;
   Statement* body;

   CallableDecl* getIteratorFn = nullptr;
   CallableDecl* nextFn = nullptr;

   IdentifierInfo* Label = nullptr;

public:
   static bool classofKind(NodeType kind) { return kind == ForInStmtID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static ForInStmt* Create(ASTContext& C, SourceLocation ForLoc, Decl* decl,
                            Expression* range, Statement* body,
                            IdentifierInfo* Label);

   ForInStmt(EmptyShell Empty);

   SourceRange getSourceRange() const
   {
      return SourceRange(ForLoc, body->getSourceRange().getEnd());
   }

   void setForLoc(SourceLocation L) { ForLoc = L; }

   Decl* getDecl() const { return decl; }
   Expression* getRangeExpr() const { return rangeExpr; }
   Statement* getBody() const { return body; }

   void setDecl(Decl* D) { decl = D; }
   void setRangeExpr(Expression* E) { rangeExpr = E; }
   void setBody(Statement* B) { body = B; }

   CallableDecl* getGetIteratorFn() const { return getIteratorFn; }
   void setGetIteratorFn(CallableDecl* fn) { getIteratorFn = fn; }

   CallableDecl* getNextFn() const { return nextFn; }
   void setNextFn(CallableDecl* fn) { nextFn = fn; }

   IdentifierInfo* getLabel() const { return Label; }
   void setLabel(IdentifierInfo* V) { Label = V; }
};

class CaseStmt : public Statement {
   CaseStmt(SourceLocation CaseLoc, PatternExpr* pattern, Statement* body);

   SourceLocation CaseLoc;
   PatternExpr* pattern;
   Statement* body;

public:
   static bool classofKind(NodeType kind) { return kind == CaseStmtID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static CaseStmt* Create(ASTContext& C, SourceLocation CaseLoc,
                           PatternExpr* pattern, Statement* body = nullptr);

   CaseStmt(EmptyShell Empty);

   SourceRange getSourceRange() const;
   void setCaseLoc(SourceLocation L) { CaseLoc = L; }

   bool isDefault() const { return pattern == nullptr; }
   PatternExpr* getPattern() const { return pattern; }
   Statement* getBody() const { return body; }

   void setPattern(PatternExpr* P) { pattern = P; }
   void setBody(Statement* body) { CaseStmt::body = body; }
};

class MatchStmt final : public Statement,
                        llvm::TrailingObjects<MatchStmt, CaseStmt*> {
   MatchStmt(SourceLocation MatchLoc, SourceRange Braces, Expression* switchVal,
             ArrayRef<CaseStmt*> cases, IdentifierInfo* Label);

   MatchStmt(EmptyShell Empty, unsigned N);

protected:
   SourceLocation MatchLoc;
   SourceRange Braces;
   Expression* switchValue;
   unsigned NumCases;

   bool HasDefault : 1;
   bool HasMutableCaseArg : 1;
   bool IntegralSwitch : 1;

   IdentifierInfo* Label = nullptr;

public:
   static bool classofKind(NodeType kind) { return kind == MatchStmtID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   friend TrailingObjects;

   static MatchStmt* Create(ASTContext& Ctx, SourceLocation MatchLoc,
                            SourceRange Braces, Expression* switchVal,
                            llvm::ArrayRef<CaseStmt*> cases,
                            IdentifierInfo* Label);

   static MatchStmt* CreateEmpty(ASTContext& C, unsigned N);

   SourceLocation getMatchLoc() const { return MatchLoc; }
   void setMatchLoc(SourceLocation L) { MatchLoc = L; }

   void setBraces(const SourceRange& Braces) { MatchStmt::Braces = Braces; }

   SourceRange getBraceRange() const { return Braces; }
   SourceRange getSourceRange() const
   {
      return SourceRange(MatchLoc, Braces.getEnd());
   }

   Expression* getSwitchValue() const { return switchValue; }
   void setSwitchValue(Expression* SV) { switchValue = SV; }

   llvm::ArrayRef<CaseStmt*> getCases() const
   {
      return {getTrailingObjects<CaseStmt*>(), NumCases};
   }

   llvm::MutableArrayRef<CaseStmt*> getCases()
   {
      return {getTrailingObjects<CaseStmt*>(), NumCases};
   }

   bool isHasDefault() const { return HasDefault; }
   void setHasDefault(bool hasDefault) { this->HasDefault = hasDefault; }

   bool hasMutableCaseArg() const { return HasMutableCaseArg; }
   void setHasMutableCaseArg(bool V) { HasMutableCaseArg = V; }

   bool isIntegralSwitch() const { return IntegralSwitch; }
   void setIntegralSwitch(bool V) { IntegralSwitch = V; }

   IdentifierInfo* getLabel() const { return Label; }
   void setLabel(IdentifierInfo* V) { Label = V; }
};

class ReturnStmt : public Statement {
   ReturnStmt(SourceLocation RetLoc, Expression* val);

   SourceLocation RetLoc;
   Expression* returnValue;
   LocalVarDecl* NRVOCand = nullptr;
   bool IsFallibleInitReturn = false;

public:
   static bool classofKind(NodeType kind) { return kind == ReturnStmtID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static ReturnStmt* Create(ASTContext& C, SourceLocation RetLoc,
                             Expression* val = nullptr);

   ReturnStmt(EmptyShell Empty);

   SourceRange getSourceRange() const;
   void setRetLoc(SourceLocation Loc) { RetLoc = Loc; }

   Expression* getReturnValue() const { return returnValue; }
   void setReturnValue(Expression* Val) { returnValue = Val; }

   LocalVarDecl* getNRVOCand() const { return NRVOCand; }
   void setNRVOCand(LocalVarDecl* Cand) { NRVOCand = Cand; }

   bool isFallibleInitReturn() const { return IsFallibleInitReturn; }
   void setIsFallibleInitReturn(bool B) { IsFallibleInitReturn = B; }
};

class DiscardAssignStmt : public Statement {
   DiscardAssignStmt(SourceLocation UnderscoreLoc, SourceLocation EqualsLoc,
                     Expression* RHS);

   DiscardAssignStmt(EmptyShell Empty);

   SourceLocation UnderscoreLoc;
   SourceLocation EqualsLoc;
   Expression* RHS;

public:
   static DiscardAssignStmt* Create(ASTContext& C, SourceLocation UnderscoreLoc,
                                    SourceLocation EqualsLoc, Expression* RHS);

   static DiscardAssignStmt* CreateEmpty(ASTContext& C);

   static bool classofKind(NodeType kind)
   {
      return kind == DiscardAssignStmtID;
   }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   SourceRange getSourceRange() const;

   SourceLocation getUnderscoreLoc() const { return UnderscoreLoc; }
   void setUnderscoreLoc(SourceLocation V) { UnderscoreLoc = V; }

   SourceLocation getEqualsLoc() const { return EqualsLoc; }
   void setEqualsLoc(SourceLocation V) { EqualsLoc = V; }

   Expression* getRHS() const { return RHS; }
   void setRHS(Expression* V) { RHS = V; }
};

struct CatchBlock {
   CatchBlock(LocalVarDecl* varDecl, Statement* Body,
              Expression* Cond = nullptr)
       : varDecl(varDecl), Body(Body), Condition(Cond)
   {
   }

   CatchBlock() = default;

   LocalVarDecl* varDecl;
   Statement* Body;
   Expression* Condition;
};

class DoStmt final : public Statement,
                     llvm::TrailingObjects<DoStmt, CatchBlock> {
   SourceRange SR;
   Statement* body;
   unsigned NumCatchBlocks;
   IdentifierInfo* Label = nullptr;

public:
   static bool classofKind(NodeType kind) { return kind == DoStmtID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   friend TrailingObjects;

   DoStmt(SourceRange SR, Statement* body, IdentifierInfo* Label);
   DoStmt(SourceRange SR, Statement* body, ArrayRef<CatchBlock> catchBlocks,
          IdentifierInfo* Label);

   DoStmt(EmptyShell Empty, unsigned N);

   SourceRange getSourceRange() const { return SR; }
   void setSourceRange(SourceRange SR) { DoStmt::SR = SR; }

   Statement* getBody() const { return body; }
   void setBody(Statement* body) { DoStmt::body = body; }

   MutableArrayRef<CatchBlock> getCatchBlocks()
   {
      return {getTrailingObjects<CatchBlock>(), NumCatchBlocks};
   }

   ArrayRef<CatchBlock> getCatchBlocks() const
   {
      return {getTrailingObjects<CatchBlock>(), NumCatchBlocks};
   }

   IdentifierInfo* getLabel() const { return Label; }
   void setLabel(IdentifierInfo* V) { Label = V; }
};

class ThrowStmt : public Statement {
   SourceLocation ThrowLoc;
   Expression* thrownVal;
   QualType thrownType;
   MethodDecl* descFn = nullptr;

public:
   static bool classofKind(NodeType kind) { return kind == ThrowStmtID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   ThrowStmt(SourceLocation ThrowLoc, Expression* thrownVal);
   ThrowStmt(EmptyShell Empty);

   SourceRange getSourceRange() const;
   void setThrowLoc(SourceLocation Loc) { ThrowLoc = Loc; }

   Expression* getThrownVal() const { return thrownVal; }
   void setThrownVal(Expression* Val) { thrownVal = Val; }

   void setThrownType(QualType Ty) { thrownType = Ty; }
   QualType getThrownType() const { return thrownType; }

   MethodDecl* getDescFn() const { return descFn; }
   void setDescFn(MethodDecl* fn) { descFn = fn; }
};

class StaticIfStmt : public Statement {
   StaticIfStmt(SourceLocation StaticLoc, SourceLocation IfLoc,
                StaticExpr* condition, Statement* ifBranch,
                Statement* elseBranch);

   StaticIfStmt(SourceLocation StaticLoc, SourceLocation IfLoc,
                StaticExpr* condition, StaticIfStmt* Template);

   SourceLocation StaticLoc;
   SourceLocation IfLoc;

   StaticExpr* condition;
   Statement* ifBranch;
   Statement* elseBranch;

   StaticIfStmt* Template;

public:
   static StaticIfStmt* Create(ASTContext& C, SourceLocation StaticLoc,
                               SourceLocation IfLoc, StaticExpr* condition,
                               Statement* ifBranch, Statement* elseBranch);

   static StaticIfStmt* Create(ASTContext& C, SourceLocation StaticLoc,
                               SourceLocation IfLoc, StaticExpr* condition,
                               StaticIfStmt* Template);

   StaticIfStmt(EmptyShell Empty);

   static bool classofKind(NodeType kind) { return kind == StaticIfStmtID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   SourceLocation getStaticLoc() const { return StaticLoc; }
   SourceLocation getIfLoc() const { return IfLoc; }
   SourceRange getSourceRange() const;

   void setStaticLoc(SourceLocation Loc) { StaticLoc = Loc; }
   void setIfLoc(SourceLocation Loc) { IfLoc = Loc; }

   StaticExpr* getCondition() const { return condition; }
   Statement* getIfBranch() const { return ifBranch; }
   Statement* getElseBranch() const { return elseBranch; }

   void setCondition(StaticExpr* C) { condition = C; }
   void setIfBranch(Statement* If) { ifBranch = If; }
   void setElseBranch(Statement* Else) { elseBranch = Else; }

   StaticIfStmt* getTemplate() const { return Template; }
};

class StaticForStmt : public Statement {
   StaticForStmt(SourceLocation StaticLoc, SourceLocation ForLoc,
                 IdentifierInfo* elementName, StaticExpr* range,
                 Statement* body);

   SourceLocation StaticLoc;
   SourceLocation ForLoc;
   IdentifierInfo* elementName;
   StaticExpr* range;
   Statement* body;

   bool variadic;
   NamedDecl* variadicDecl = nullptr;

public:
   static StaticForStmt* Create(ASTContext& C, SourceLocation StaticLoc,
                                SourceLocation ForLoc,
                                IdentifierInfo* elementName, StaticExpr* range,
                                Statement* body);

   StaticForStmt(EmptyShell Empty);

   static bool classofKind(NodeType kind) { return kind == StaticForStmtID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   SourceLocation getStaticLoc() const { return StaticLoc; }
   SourceLocation getForLoc() const { return ForLoc; }
   SourceRange getSourceRange() const
   {
      return SourceRange(StaticLoc, body->getSourceRange().getEnd());
   }

   void setStaticLoc(SourceLocation Loc) { StaticLoc = Loc; }
   void setForLoc(SourceLocation Loc) { ForLoc = Loc; }

   IdentifierInfo* getElementName() const { return elementName; }
   Statement* getBody() const { return body; }
   StaticExpr* getRange() const { return range; }

   void setElementName(IdentifierInfo* EN) { elementName = EN; }
   void setRange(StaticExpr* R) { range = R; }
   void setBody(Statement* B) { body = B; }

   bool isVariadic() const { return variadic; }
   void setVariadic(bool v) { variadic = v; }

   NamedDecl* getVariadicDecl() const { return variadicDecl; }
   void setVariadicDecl(NamedDecl* D) { variadicDecl = D; }
};

class MacroExpansionStmt final
    : public Statement,
      llvm::TrailingObjects<MacroExpansionStmt, lex::Token> {
public:
   enum Delimiter {
      Brace,
      Square,
      Paren,
   };

   friend TrailingObjects;

private:
   MacroExpansionStmt(SourceRange SR, DeclarationName MacroName,
                      Expression* ParentExpr, Delimiter Delim,
                      ArrayRef<lex::Token> Toks);

   MacroExpansionStmt(EmptyShell Empty, unsigned N);

   SourceRange SR;
   Delimiter Delim;
   DeclarationName MacroName;
   unsigned NumTokens;

   Expression* ParentExpr;

public:
   static MacroExpansionStmt* Create(ASTContext& C, SourceRange SR,
                                     DeclarationName MacroName,
                                     Expression* ParentExpr, Delimiter Delim,
                                     llvm::ArrayRef<lex::Token> Toks);

   static MacroExpansionStmt* CreateEmpty(ASTContext& C, unsigned N);

   static bool classofKind(NodeType kind)
   {
      return kind == MacroExpansionStmtID;
   }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   SourceRange getSourceRange() const { return SR; }
   DeclarationName getMacroName() const { return MacroName; }
   Delimiter getDelim() const { return Delim; }

   void setSourceRange(SourceRange SR) { MacroExpansionStmt::SR = SR; }
   void setMacroName(DeclarationName Name) { MacroName = Name; }
   void setDelim(Delimiter Delim) { MacroExpansionStmt::Delim = Delim; }

   Expression* getParentExpr() const { return ParentExpr; }
   void setParentExpr(Expression* V) { ParentExpr = V; }

   llvm::ArrayRef<lex::Token> getTokens() const
   {
      return {getTrailingObjects<lex::Token>(), NumTokens};
   }
};

} // namespace ast
} // namespace cdot

#endif // STATEMENT_H
