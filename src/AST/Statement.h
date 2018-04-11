//
// Created by Jonas Zell on 19.06.17.
//

#ifndef STATEMENT_H
#define STATEMENT_H

#include "AST/AstNode.h"
#include "AST/SourceType.h"
#include "Basic/DeclarationName.h"

#include <llvm/Support/TrailingObjects.h>

namespace cdot {

namespace module {
   class Module;
} // namespace module

namespace ast {

class Decl;
class LocalVarDecl;
class CallableDecl;
class VarDecl;
class Expression;
class CallableDecl;
class MethodDecl;
class Expression;
class PatternExpr;
class StaticExpr;

class LLVM_ALIGNAS(sizeof(void*)) Statement: public AstNode {
public:
   enum Flags: uint32_t {
      TypeDependent          = 1u,
      ValueDependent         = TypeDependent  << 1u,
      HadError               = ValueDependent << 1u,
      SemanticallyChecked    = HadError << 1u,
      GlobalInitializer      = SemanticallyChecked  << 1u,
      ContainsUnexpandedPack = GlobalInitializer << 1u,

      StatusFlags       = TypeDependent | ValueDependent | HadError
                          | ContainsUnexpandedPack,
   };

   void dumpFlags() const;
   void printFlags(llvm::raw_ostream &OS) const;

   bool isDependent() const
   {
      return isTypeDependent() || isValueDependent();
   }

   bool isTypeDependent() const
   {
      return flagIsSet(TypeDependent);
   }

   bool isValueDependent() const
   {
      return flagIsSet(ValueDependent);
   }

   bool containsUnexpandedParameterPack() const
   {
      return flagIsSet(ContainsUnexpandedPack);
   }

   void setIsTypeDependent(bool typeDependant)
   {
      setFlag(TypeDependent, typeDependant);
   }

   void setIsValueDependent(bool valueDependant)
   {
      setFlag(ValueDependent, valueDependant);
   }

   void setContainsUnexpandedParameterPack(bool unexp)
   {
      setFlag(ContainsUnexpandedPack, unexp);
   }

   bool isGlobalInitializer() const
   {
      return flagIsSet(GlobalInitializer);
   }

   void setIsGlobalInitializer(bool globalInit)
   {
      setFlag(GlobalInitializer, globalInit);
   }

   bool isInvalid() const
   {
      return flagIsSet(HadError);
   }

   void setIsInvalid(bool error)
   {
      setFlag(HadError, error);
   }

   bool isSemanticallyChecked() const { return flagIsSet(SemanticallyChecked); }
   void setSemanticallyChecked(bool chk = true)
   {
      setFlag(SemanticallyChecked, chk);
   }

   bool alreadyCheckedOrHasError() const
   {
      static uint32_t mask = HadError | SemanticallyChecked;
      return (SubclassData & mask) != 0;
   }

   void copyStatusFlags(Decl *D);
   void copyStatusFlags(Statement *Stmt);

   SourceRange getSourceRange() const;
   SourceLocation getSourceLoc() const
   {
      return getSourceRange().getStart();
   }

   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return true; }

protected:
   explicit Statement(NodeType typeID)
      : AstNode(typeID)
   {}
};

class DeclStmt: public Statement {
public:
   static DeclStmt *Create(ASTContext &C, Decl *D);

   Decl *getDecl() const { return D; }
   void setDecl(Decl *D) { this->D = D; }

   SourceRange getSourceRange() const;

   static bool classofKind(NodeType kind) { return kind == DeclStmtID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

private:
   explicit DeclStmt(Decl *D);

   Decl *D;
};

class AttributedStmt final:
   public Statement,
   llvm::TrailingObjects<AttributedStmt, Attr*> {
public:
   static bool classofKind(NodeType kind) { return kind == AttributedStmtID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static AttributedStmt *Create(ASTContext &Ctx,
                                 Statement *Stmt,
                                 llvm::ArrayRef<Attr*> Attrs);

   llvm::ArrayRef<Attr*> getAttributes() const
   {
      return { getTrailingObjects<Attr*>(), NumAttrs };
   }

   SourceRange getSourceRange() const { return Stmt->getSourceRange(); }

   Statement *getStatement() const { return Stmt; }
   void setStatement(Statement *S) { Stmt = S; }

   friend TrailingObjects;

private:
   AttributedStmt(Statement *Stmt, llvm::ArrayRef<Attr*> Attrs);

   Statement *Stmt;
   unsigned NumAttrs;
};

class MixinStmt: public Statement {
   MixinStmt(SourceRange Parens, Expression *Expr);

   SourceRange Parens;
   Expression *Expr;

public:
   static bool classofKind(NodeType kind) { return kind == MixinStmtID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   SourceRange getSourceRange() const { return Parens; }
   Expression *getMixinExpr() const { return Expr; }
   void setMixinExpr(Expression *E) { Expr = E; }

   static MixinStmt *Create(ASTContext &C, SourceRange Parens,
                            Expression *Expr);
};

class DebugStmt : public Statement {
public:
   explicit DebugStmt(SourceLocation Loc, bool unreachable = false)
      : Statement(DebugStmtID), Loc(Loc), unreachable(unreachable) {

   }

   bool isUnreachable() const { return unreachable; }

private:
   SourceLocation Loc;
   bool unreachable;

public:
   static bool classofKind(NodeType kind) { return kind == DebugStmtID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   SourceRange getSourceRange() const
   {
      unsigned EndOffset = Loc.getOffset() + (unreachable ? 13 : 7);
      return SourceRange(Loc, SourceLocation(EndOffset));
   }
};

class NullStmt: public Statement {
   explicit NullStmt(SourceLocation Loc);

   SourceLocation Loc;

public:
   static bool classofKind(NodeType kind) { return kind == NullStmtID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static NullStmt *Create(ASTContext &C, SourceLocation Loc);

   SourceRange getSourceRange() const { return SourceRange(Loc); }
};

class CompoundStmt final: public Statement,
                          llvm::TrailingObjects<CompoundStmt, Statement*> {
public:
   static CompoundStmt *Create(ASTContext &ASTCtx,
                               bool preserveScope,
                               SourceLocation LBraceLoc,
                               SourceLocation RBraceLoc);

   static CompoundStmt *Create(ASTContext &ASTCtx,
                               llvm::ArrayRef<Statement*> stmts,
                               bool preserveScope,
                               SourceLocation LBraceLoc,
                               SourceLocation RBraceLoc);

   using stmt_iterator        = Statement**;
   using iterator_range       = llvm::MutableArrayRef<Statement*>;
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

   static bool classofKind(NodeType kind) { return kind == CompoundStmtID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   friend TrailingObjects;

private:
   CompoundStmt(bool preservesScope,
                SourceLocation LBraceLoc,
                SourceLocation RBraceLoc);

   CompoundStmt(llvm::ArrayRef<Statement* > stmts,
                bool preserveScope,
                SourceLocation LBraceLoc,
                SourceLocation RBraceLoc);

   unsigned numStmts : 31;
   bool preserveScope : 1;

   SourceLocation LBraceLoc;
   SourceLocation RBraceLoc;
};

class BreakStmt : public Statement {
   explicit BreakStmt(SourceLocation Loc);

   SourceLocation Loc;

public:
   static bool classofKind(NodeType kind) { return kind == BreakStmtID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static BreakStmt *Create(ASTContext &C, SourceLocation Loc);

   SourceRange getSourceRange() const { return SourceRange(Loc); }
};

class ContinueStmt : public Statement {
   explicit ContinueStmt(SourceLocation Loc);

   SourceLocation Loc;

public:
   static bool classofKind(NodeType kind) { return kind == ContinueStmtID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static ContinueStmt *Create(ASTContext &C, SourceLocation Loc);

   SourceRange getSourceRange() const { return SourceRange(Loc); }
};

class GotoStmt: public Statement {
   GotoStmt(SourceLocation Loc, IdentifierInfo *label);

   SourceLocation Loc;
   IdentifierInfo* label;

public:
   static bool classofKind(NodeType kind) { return kind == GotoStmtID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static GotoStmt *Create(ASTContext &C, SourceLocation Loc,
                           IdentifierInfo *label);

   SourceRange getSourceRange() const { return SourceRange(Loc); }
   IdentifierInfo *getLabel() const { return label; }
   llvm::StringRef getLabelName() const { return label->getIdentifier(); }
};

class LabelStmt : public Statement {
   LabelStmt(SourceLocation Loc, IdentifierInfo *label);

   SourceLocation Loc;
   IdentifierInfo* label;

public:
   static bool classofKind(NodeType kind) { return kind == LabelStmtID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static LabelStmt *Create(ASTContext &C, SourceLocation Loc,
                            IdentifierInfo *label);

   SourceRange getSourceRange() const { return SourceRange(Loc); }
   IdentifierInfo *getLabel() const { return label; }
   llvm::StringRef getLabelName() const { return label->getIdentifier(); }
};

class IfStmt : public Statement {
   IfStmt(SourceLocation IfLoc,
          Expression* cond,
          Statement* body, Statement* elseBody);

   SourceLocation IfLoc;

   Expression* condition;
   Statement* ifBranch;
   Statement* elseBranch;

public:
   static bool classofKind(NodeType kind) { return kind == IfStmtID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static IfStmt *Create(ASTContext &C,
                         SourceLocation IfLoc,
                         Expression* cond,
                         Statement* body, Statement* elseBody);

   SourceRange getSourceRange() const
   {
      return SourceRange(IfLoc,
                         elseBranch ? elseBranch->getSourceRange().getEnd()
                                    : ifBranch->getSourceRange().getEnd());
   }

   Expression* getCondition() const { return condition; }
   Statement* getIfBranch() const { return ifBranch; }
   Statement* getElseBranch() const { return elseBranch; }

   void setCondition(Expression *C) { condition = C; }
   void setIfBranch(Statement *If) { ifBranch = If; }
   void setElseBranch(Statement* Else) { elseBranch = Else; }
};

class ForStmt: public Statement {
   ForStmt(SourceLocation ForLoc,
           Statement* init, Expression* term,
           Statement* inc, Statement *body);

   SourceLocation ForLoc;
   Statement* initialization;
   Expression* termination;
   Statement* increment;
   Statement* body;

public:
   static bool classofKind(NodeType kind) { return kind == ForStmtID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static ForStmt *Create(ASTContext &C,
                          SourceLocation ForLoc,
                          Statement* init, Expression* term,
                          Statement* inc, Statement *body);

   SourceRange getSourceRange() const
   {
      return SourceRange(ForLoc, body->getSourceRange().getEnd());
   }

   Statement* getInitialization() const { return initialization; }
   void setInitialization(Statement* Init) { initialization = Init; }

   Expression* getTermination() const { return termination; }
   void setTermination(Expression* Term) { termination = Term; }

   Statement* getIncrement() const { return increment; }
   void setIncrement(Statement* Inc) { increment = Inc; }

   Statement* getBody() const { return body; }
   void setBody(Statement *S) { body = S; }
};

class ForInStmt: public Statement {
   ForInStmt(SourceLocation ForLoc,
             LocalVarDecl* decl,
             Expression* range,
             Statement* body);

   SourceLocation ForLoc;
   LocalVarDecl* decl;
   Expression* rangeExpr;
   Statement* body;

   CallableDecl *getIteratorFn = nullptr;
   CallableDecl *nextFn = nullptr;

public:
   static bool classofKind(NodeType kind) { return kind == ForInStmtID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static ForInStmt *Create(ASTContext &C,
                            SourceLocation ForLoc, LocalVarDecl* decl,
                            Expression* range, Statement* body);

   SourceRange getSourceRange() const
   {
      return SourceRange(ForLoc, body->getSourceRange().getEnd());
   }

   LocalVarDecl* getDecl() const { return decl; }
   Expression* getRangeExpr() const { return rangeExpr; }
   Statement* getBody() const { return body; }

   void setDecl(LocalVarDecl *D) { decl = D; }
   void setRangeExpr(Expression *E) { rangeExpr = E; }
   void setBody(Statement *B) { body = B; }

   CallableDecl *getGetIteratorFn() const { return getIteratorFn; }
   void setGetIteratorFn(CallableDecl *fn) { getIteratorFn = fn; }

   CallableDecl *getNextFn() const { return nextFn; }
   void setNextFn(CallableDecl *fn) { nextFn = fn; }
};

class WhileStmt: public Statement {
   WhileStmt(SourceLocation WhileLoc,
             Expression* cond,
             Statement* body,
             bool atLeastOnce);

   SourceLocation WhileLoc;
   Expression* condition;
   Statement* body;
   bool atLeastOnce;

public:
   static bool classofKind(NodeType kind) { return kind == WhileStmtID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static WhileStmt *Create(ASTContext &C,
                            SourceLocation WhileLoc,
                            Expression* cond, Statement* body,
                            bool atLeastOnce = false);

   SourceRange getSourceRange() const
   {
      return SourceRange(WhileLoc, body->getSourceRange().getEnd());
   }

   Expression* getCondition() const { return condition; }
   Statement* getBody() const { return body; }

   void setCondition(Expression *C) { condition = C; }
   void setBody(Statement *B) { body = B; }
   bool isAtLeastOnce() const { return atLeastOnce; }
};

class CaseStmt: public Statement {
public:
   CaseStmt(SourceLocation CaseLoc,
            PatternExpr* pattern,
            Statement* body);

protected:
   SourceLocation CaseLoc;
   PatternExpr* pattern;
   Statement* body;
   CallableDecl *comparisonOp = nullptr;

public:
   static bool classofKind(NodeType kind) { return kind == CaseStmtID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static CaseStmt *Create(ASTContext &C, SourceLocation CaseLoc,
                           PatternExpr* pattern, Statement* body = nullptr);

   SourceRange getSourceRange() const
   {
      return SourceRange(CaseLoc, body->getSourceRange().getEnd());
   }

   bool isDefault() const { return pattern == nullptr; }
   PatternExpr* getPattern() const { return pattern; }
   Statement* getBody() const { return body; }

   void setPattern(PatternExpr *P) { pattern = P; }
   void setBody(Statement *body) { CaseStmt::body = body; }

   CallableDecl *getComparisonOp() const { return comparisonOp; }
   void setComparisonOp(CallableDecl *op) { comparisonOp = op; }
};

class MatchStmt final: public Statement,
                       llvm::TrailingObjects<MatchStmt, CaseStmt*> {
   MatchStmt(SourceLocation MatchLoc,
             SourceRange Braces,
             Expression* switchVal,
             llvm::ArrayRef<CaseStmt*> cases);

protected:
   SourceLocation MatchLoc;
   SourceRange Braces;
   Expression* switchValue;
   unsigned NumCases;
   bool hasDefault = false;

public:
   static bool classofKind(NodeType kind) { return kind == MatchStmtID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   friend TrailingObjects;

   static MatchStmt *Create(ASTContext &Ctx,
                            SourceLocation MatchLoc,
                            SourceRange Braces,
                            Expression* switchVal,
                            llvm::ArrayRef<CaseStmt*> cases);

   SourceLocation getMatchLoc() const { return MatchLoc; }
   SourceRange getBraceRange() const { return Braces; }
   SourceRange getSourceRange() const
   {
      return SourceRange(MatchLoc, Braces.getEnd());
   }

   Expression* getSwitchValue() const { return switchValue; }
   void setSwitchValue(Expression *SV) { switchValue = SV; }

   llvm::ArrayRef<CaseStmt*> getCases() const
   {
      return { getTrailingObjects<CaseStmt*>(), NumCases };
   }

   bool isHasDefault() const { return hasDefault; }
   void setHasDefault(bool hasDefault) { this->hasDefault = hasDefault; }
};

class ReturnStmt : public Statement {
   ReturnStmt(SourceLocation RetLoc, Expression* val);

   SourceLocation RetLoc;
   Expression* returnValue;
   LocalVarDecl *NRVOCand = nullptr;

public:
   static bool classofKind(NodeType kind) { return kind == ReturnStmtID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static ReturnStmt *Create(ASTContext &C, SourceLocation RetLoc,
                             Expression* val = nullptr);

   SourceRange getSourceRange() const;

   Expression* getReturnValue() const { return returnValue; }
   void setReturnValue(Expression *Val) { returnValue = Val; }

   LocalVarDecl *getNRVOCand() const { return NRVOCand; }
   void setNRVOCand(LocalVarDecl *Cand) { NRVOCand = Cand; }
};

struct CatchBlock {
   CatchBlock(LocalVarDecl *varDecl, Statement* body)
      : varDecl(varDecl),
        body(body)
   { }

   CatchBlock() = default;

   LocalVarDecl *varDecl;
   Statement* body;
};

class TryStmt: public Statement {
   SourceRange SR;
   Statement* body;
   std::vector<CatchBlock> catchBlocks;
   Statement* finallyBlock = nullptr;

public:
   static bool classofKind(NodeType kind) { return kind == TryStmtID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   TryStmt(SourceRange SR, Statement* body);
   TryStmt(SourceRange SR,
           Statement* body,
           std::vector<CatchBlock> &&catchBlocks,
           Statement* finally);

   SourceRange getSourceRange() const { return SR; }

   Statement* getBody() const { return body; }
   void setBody(Statement *body) { TryStmt::body = body; }

   std::vector<CatchBlock> &getCatchBlocks() { return catchBlocks; }
   std::vector<CatchBlock> const& getCatchBlocks() const
   {
      return catchBlocks;
   }

   void addCatch(CatchBlock const& catchBlock)
   {
      catchBlocks.push_back(catchBlock);
   }

   void setFinally(Statement* FB) { finallyBlock = FB; }
   Statement* getFinallyBlock() const { return finallyBlock; }
};

class ThrowStmt: public Statement {
   SourceLocation ThrowLoc;
   Expression* thrownVal;
   QualType thrownType;
   MethodDecl *descFn = nullptr;

public:
   static bool classofKind(NodeType kind) { return kind == ThrowStmtID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   ThrowStmt(SourceLocation ThrowLoc, Expression* thrownVal);

   SourceRange getSourceRange() const;

   Expression *getThrownVal() const { return thrownVal; }
   void setThrownVal(Expression *Val) { thrownVal = Val; }

   void setThrownType(QualType Ty) { thrownType = Ty; }
   QualType getThrownType() const { return thrownType; }

   MethodDecl *getDescFn() const { return descFn; }
   void setDescFn(MethodDecl *fn) { descFn = fn; }
};

class DestructuringDecl: public Statement {
protected:
   DestructuringDecl(NodeType typeID,
                     SourceRange SR,
                     unsigned NumDecls,
                     AccessSpecifier access,
                     bool isConst,
                     SourceType type,
                     Expression *value);

   SourceRange SR;
   AccessSpecifier access;
   bool IsConst;

   unsigned NumDecls;

   SourceType type;
   Expression *value;

   CallableDecl *destructuringFn = nullptr;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind)
   {
      switch (kind) {
      case LocalDestructuringDeclID:
      case GlobalDestructuringDeclID:
         return true;
      default:
         return false;
      }
   }

   SourceRange getSourceRange() const { return SR; }

   AccessSpecifier getAccess() const { return access; }
   void setAccess(AccessSpecifier A) { access = A; }

   bool isConst() const { return IsConst; }

   const SourceType &getType() const { return type; }
   void setType(SourceType Ty) { type = Ty; }

   Expression *getValue() const { return value; }
   void setValue(Expression *V) { value = V; }

   CallableDecl *getDestructuringFn() const { return destructuringFn; }
   void setDestructuringFn(CallableDecl *fn) { destructuringFn = fn; }

   unsigned getNumDecls() const { return NumDecls; }
   llvm::ArrayRef<VarDecl*> getDecls() const;
};

class LocalDestructuringDecl final:
   public DestructuringDecl,
   llvm::TrailingObjects<LocalDestructuringDecl, VarDecl*> {
private:
   LocalDestructuringDecl(SourceRange SR,
                          AccessSpecifier access,
                          bool isConst,
                          llvm::ArrayRef<VarDecl*> Decls,
                          SourceType type,
                          Expression *value);

public:
   friend TrailingObjects;

   static LocalDestructuringDecl *Create(ASTContext &C,
                                         SourceRange SR,
                                         AccessSpecifier access,
                                         bool isConst,
                                         llvm::ArrayRef<VarDecl*> Decls,
                                         SourceType type,
                                         Expression *value);

   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind)
   {
      return kind == LocalDestructuringDeclID;
   }

   llvm::ArrayRef<VarDecl*> getDecls() const
   {
      return { getTrailingObjects<VarDecl*>(), NumDecls };
   }
};

class GlobalDestructuringDecl:
   public DestructuringDecl,
   llvm::TrailingObjects<LocalDestructuringDecl, VarDecl*> {
private:
   GlobalDestructuringDecl(SourceRange SR,
                           AccessSpecifier access,
                           bool isConst,
                           llvm::ArrayRef<VarDecl*> Decls,
                           SourceType type,
                           Expression *value);

   size_t globalOrdering = 0;

public:
   friend TrailingObjects;

   static GlobalDestructuringDecl *Create(ASTContext &C,
                                          SourceRange SR,
                                          AccessSpecifier access,
                                          bool isConst,
                                          llvm::ArrayRef<VarDecl*> Decls,
                                          SourceType type,
                                          Expression *value);

   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

   static bool classofKind(NodeType kind)
   {
      return kind == GlobalDestructuringDeclID;
   }

   llvm::ArrayRef<VarDecl*> getDecls() const
   {
      return { getTrailingObjects<VarDecl*>(), NumDecls };
   }

   size_t getGlobalOrdering() const { return globalOrdering; }
   void setGlobalOrdering(size_t GO) { globalOrdering = GO; }
};

class StaticIfStmt: public Statement {
public:
   StaticIfStmt(SourceLocation StaticLoc,
                SourceLocation IfLoc,
                StaticExpr *condition,
                Statement *ifBranch,
                Statement *elseBranch);

   static bool classofKind(NodeType kind) { return kind == StaticIfStmtID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

private:
   SourceLocation StaticLoc;
   SourceLocation IfLoc;

   StaticExpr *condition;
   Statement *ifBranch;
   Statement *elseBranch;

public:
   SourceLocation getStaticLoc() const { return StaticLoc; }
   SourceLocation getIfLoc() const { return IfLoc; }
   SourceRange getSourceRange() const
   {
      return SourceRange(StaticLoc, elseBranch
                                    ? elseBranch->getSourceRange().getEnd()
                                    : ifBranch->getSourceRange().getEnd());
   }

   StaticExpr* getCondition() const { return condition; }
   Statement* getIfBranch() const { return ifBranch; }
   Statement* getElseBranch() const { return elseBranch; }

   void setCondition(StaticExpr *C) { condition = C; }
   void setIfBranch(Statement *If) { ifBranch = If; }
   void setElseBranch(Statement *Else) { elseBranch = Else; }
};

class StaticForStmt: public Statement {
public:
   StaticForStmt(SourceLocation StaticLoc,
                 SourceLocation IfLoc,
                 IdentifierInfo *elementName,
                 StaticExpr *range,
                 Statement *body);

   static bool classofKind(NodeType kind) { return kind == StaticForStmtID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

private:
   SourceLocation StaticLoc;
   SourceLocation ForLoc;
   IdentifierInfo *elementName;
   StaticExpr *range;
   Statement *body;

public:
   SourceLocation getStaticLoc() const { return StaticLoc; }
   SourceLocation getForLoc() const { return ForLoc; }
   SourceRange getSourceRange() const
   {
      return SourceRange(StaticLoc, body->getSourceRange().getEnd());
   }

   IdentifierInfo * getElementName() const { return elementName; }
   Statement* getBody() const { return body; }
   StaticExpr *getRange() const { return range; }

   void setRange(StaticExpr *R) { range = R; }
   void setBody(Statement *B) { body = B; }
};

} // namespace ast
} // namespace cdot


#endif //STATEMENT_H
