//
// Created by Jonas Zell on 19.06.17.
//

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "Sema/ConversionSequence.h"
#include "AST/SourceType.h"
#include "AST/Statement.h"
#include "Basic/Precedence.h"
#include "Basic/Variant.h"
#include "Lex/Token.h"

#include <llvm/ADT/APSInt.h>
#include <llvm/ADT/APFloat.h>
#include <llvm/Support/TrailingObjects.h>

namespace cdot {

class FunctionType;
enum class BuiltinFn : unsigned char;
enum class KnownFunction : unsigned char;

enum class BuiltinIdentifier: unsigned char {
   None, NULLPTR, FUNC, MANGLED_FUNC, FLOAT_QNAN, DOUBLE_QNAN,
   FLOAT_SNAN, DOUBLE_SNAN, __ctfe
};

namespace il {
   class Constant;
} // namespace il

namespace ast {

class MethodDecl;
class LocalVarDecl;
class EnumCaseDecl;
class StaticExpr;
class CallExpr;
class VarDecl;
class ConstraintExpr;
class IdentifierRefExpr;
class CallableDecl;
class MethodDecl;
class FunctionDecl;
class UnionDecl;
class EnumDecl;
class GlobalVarDecl;
class LocalVarDecl;
class MemberRefExpr;
class GlobalVarDecl;
class AssociatedTypeDecl;
class NamespaceDecl;
class AliasDecl;
class FuncArgDecl;
class DeclContext;
class FieldDecl;
class PropDecl;
class NamedDecl;

using TemplateArgVec = std::vector<Expression*>;

class Expression : public Statement {
public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind)
   {
      return kind > _firstExprID && kind < _lastExprID;
   }

   QualType getExprType() const { return exprType; }
   void setExprType(QualType ty) { exprType = ty; }

   bool isUnknownAny() const
   {
      return exprType && exprType->isUnknownAnyType();
   }

   bool isContextDependent() const { return contextDependent; }

   bool isLHSOfAssignment() const { return IsLHSOfAssignment; }
   void setIsLHSOfAssignment(bool IsLHS) { IsLHSOfAssignment = IsLHS; }

   bool isConst() const;
   bool isLValue() const { return exprType && exprType->isReferenceType(); }

   bool isVariadicArgPackExpansion() const
   {
      return EllipsisLoc.isValid();
   }

   bool isMagicArgumentValue() const { return IsMagicArgumentValue; }
   void setIsMagicArgumentValue(bool magic) { IsMagicArgumentValue = magic; }

   SourceRange getEllipsisRange() const;
   SourceLocation getEllipsisLoc() const { return EllipsisLoc; }
   void setEllipsisLoc(SourceLocation Loc) { EllipsisLoc = Loc; }

   inline Expression *ignoreParens();
   inline Expression *ignoreParensAndImplicitCasts();

   Expression *maybeGetParentExpr() const;
   void setParentExpr(Expression *E);

   bool warnOnUnusedResult() const;

protected:
   explicit Expression(NodeType typeID, bool contextDependent = false);

   QualType exprType;

   SourceLocation EllipsisLoc;
   bool contextDependent           : 1;
   bool IsLHSOfAssignment          : 1;
   bool IsMagicArgumentValue       : 1;
};

class IdentifiedExpr: public Expression {
public:
   IdentifiedExpr(NodeType typeID, DeclarationName Name,
                  bool contextDependent = false);

   DeclarationName getDeclName() const { return DeclName; }
   IdentifierInfo *getIdentInfo() const { return DeclName.getIdentifierInfo(); }
   llvm::StringRef getIdent() const { return getIdentInfo()->getIdentifier(); }

   void setIdent(DeclarationName Name) { DeclName = Name; }

   static bool classofKind(NodeType kind)
   {
      return kind > _firstIdentifiedExprID && kind < _lastIdentifiedExprID;
   }

   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

protected:
   DeclarationName DeclName;
};

class ParenExpr: public Expression {
public:
   static ParenExpr *Create(ASTContext &C,
                            SourceRange Parens,
                            Expression *Expr);

   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == ParenExprID; }

private:
   ParenExpr(SourceRange Parens,
             Expression *Expr);

   SourceRange Parens;
   Expression *ParenthesizedExpr;

public:
   SourceRange getSourceRange() const { return Parens; }
   Expression *getParenthesizedExpr() const { return ParenthesizedExpr; }
   void setParenthesizedExpr(Expression *E) { ParenthesizedExpr = E; }
};

class TypeExpr: public Expression {
public:
   SourceRange getSourceRange() const { return Loc; }

   static bool classof(AstNode const *T){ return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind)
   {
      return kind > _firstTypeExprID && kind < _lastTypeExprID;
   }

   bool isMeta() const { return IsMeta; }
   void setIsMeta(bool M) { IsMeta = M; }

protected:
   TypeExpr(NodeType kind, SourceRange SR, bool IsMeta)
      : Expression(kind), Loc(SR), IsMeta(IsMeta)
   {

   }

   SourceRange Loc;
   bool IsMeta;
};

class TupleTypeExpr final: public TypeExpr,
                           llvm::TrailingObjects<TupleTypeExpr, SourceType> {
   explicit TupleTypeExpr(SourceRange SR,
                          llvm::ArrayRef<SourceType> Tys,
                          bool IsMeta);

   unsigned NumTys;

public:
   friend TrailingObjects;

   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == TupleTypeExprID; }

   static TupleTypeExpr *Create(ASTContext &C,
                                SourceRange SR,
                                llvm::ArrayRef<SourceType> Tys,
                                bool IsMeta = true);

   llvm::ArrayRef<SourceType> getContainedTypes() const
   {
      return { getTrailingObjects<SourceType>(), NumTys };
   }
};

class FunctionTypeExpr final: public TypeExpr,
                              llvm::TrailingObjects<FunctionTypeExpr,
                                                    SourceType> {
   explicit FunctionTypeExpr(SourceRange SR,
                             SourceType RetTy,
                             llvm::ArrayRef<SourceType> Tys,
                             bool IsMeta);

   bool Thin = false;
   SourceType RetTy;
   unsigned NumArgs;

public:
   friend TrailingObjects;

   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind)
   {
      return kind == FunctionTypeExprID;
   }

   static FunctionTypeExpr *Create(ASTContext &C,
                                   SourceRange SR,
                                   SourceType RetTy,
                                   llvm::ArrayRef<SourceType> Tys,
                                   bool IsMeta = true);

   const SourceType &getReturnType() const { return RetTy; }

   llvm::ArrayRef<SourceType> getArgTypes() const
   {
      return { getTrailingObjects<SourceType>(), NumArgs };
   }

   bool isThin() const { return Thin; }
   void setThin(bool T) { Thin = T; }
};

class ArrayTypeExpr: public TypeExpr {
   ArrayTypeExpr(SourceRange SR, SourceType ElementTy,
                 StaticExpr *SizeExpr, bool IsMeta);

   SourceType ElementTy;
   StaticExpr *SizeExpr;

public:
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind)
   {
      return kind == ArrayTypeExprID;
   }

   static ArrayTypeExpr *Create(ASTContext &C,
                                SourceRange SR,
                                SourceType ElementTy,
                                StaticExpr *SizeExpr,
                                bool IsMeta = true);

   const SourceType &getElementTy() const { return ElementTy; }

   StaticExpr *getSizeExpr() const { return SizeExpr; }
   void setSizeExpr(StaticExpr *E) { SizeExpr = E; }
};

class DeclTypeExpr: public TypeExpr {
   DeclTypeExpr(SourceRange SR, Expression *TyExpr, bool IsMeta);

   Expression *TyExpr;

public:
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind)
   {
      return kind == DeclTypeExprID;
   }

   static DeclTypeExpr *Create(ASTContext &C,
                               SourceRange SR,
                               Expression *TyExpr,
                               bool IsMeta = true);

   Expression *getTyExpr() const { return TyExpr; }
};

class PointerTypeExpr: public TypeExpr {
   PointerTypeExpr(SourceRange SR, SourceType SubType, bool IsMeta);

   SourceType SubType;

public:
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind)
   {
      return kind == PointerTypeExprID;
   }

   static PointerTypeExpr *Create(ASTContext &C,
                                  SourceRange SR,
                                  SourceType SubType,
                                  bool IsMeta = true);

   const SourceType &getSubType() const { return SubType; }
};

class ReferenceTypeExpr: public TypeExpr {
   ReferenceTypeExpr(SourceRange SR, SourceType SubType, bool IsMeta);

   SourceType SubType;

public:
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind)
   {
      return kind == ReferenceTypeExprID;
   }

   static ReferenceTypeExpr *Create(ASTContext &C,
                                    SourceRange SR,
                                    SourceType SubType,
                                    bool IsMeta = true);

   const SourceType &getSubType() const { return SubType; }
};

class OptionTypeExpr: public TypeExpr {
   OptionTypeExpr(SourceRange SR, SourceType SubType, bool IsMeta);

   SourceType SubType;

public:
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == OptionTypeExprID; }

   static OptionTypeExpr *Create(ASTContext &C,
                                 SourceRange SR,
                                 SourceType SubType,
                                 bool IsMeta = true);

   const SourceType &getSubType() const { return SubType; }
};

class AttributedExpr final:
   public Expression,
   llvm::TrailingObjects<AttributedExpr, Attr*> {
public:
   static bool classofKind(NodeType kind) { return kind == AttributedExprID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static AttributedExpr *Create(ASTContext &Ctx,
                                 Expression *Expr,
                                 llvm::ArrayRef<Attr*> Attrs);

   llvm::ArrayRef<Attr*> getAttributes() const
   {
      return { getTrailingObjects<Attr*>(), NumAttrs };
   }

   SourceRange getSourceRange() const { return Expr->getSourceRange(); }

   Expression *getExpr() const { return Expr; }
   void setExpr(Expression *E) { Expr = E; }

   friend TrailingObjects;

private:
   AttributedExpr(Expression *Expr, llvm::ArrayRef<Attr*> Attrs);

   Expression *Expr;
   unsigned NumAttrs;
};

class ImplicitCastExpr: public Expression {
public:
   static ImplicitCastExpr *Create(ASTContext &C,
                                   Expression* target,
                                   ConversionSequence &&ConvSeq);

   static bool classofKind(NodeType kind) { return kind == ImplicitCastExprID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

private:
   ImplicitCastExpr(Expression* target,
                    ConversionSequence &&ConvSeq);

   Expression* target;
   ConversionSequence ConvSeq;

public:
   SourceRange getSourceRange() const { return target->getSourceRange(); }
   Expression *getTarget() const { return target; }
   const ConversionSequence &getConvSeq() const { return ConvSeq; }
};

class SequenceElement {
public:
   explicit SequenceElement(Expression* expr);
   SequenceElement(IdentifierInfo *possibleOp,
                   SourceLocation loc);
   SequenceElement(op::OperatorKind opKind,
                   SourceLocation loc);

   SequenceElement(const SequenceElement&) = delete;
   SequenceElement &operator=(const SequenceElement&) = delete;

   SequenceElement(SequenceElement&&) noexcept;
   SequenceElement &operator=(SequenceElement&&) noexcept;

   enum Kind : unsigned char {
      EF_Expression,
      EF_PossibleOperator,
      EF_Operator
   };

   bool isExpression() const
   {
      return kind == EF_Expression;
   }

   bool isPossibleOperator() const
   {
      return kind == EF_PossibleOperator;
   }

   bool isOperator() const
   {
      return kind == EF_Operator;
   }

   Expression* getExpr() const { return expr; }
   void setExpr(Expression *E) const { expr = E; }

   op::OperatorKind getOperatorKind() const { return operatorKind; }
   IdentifierInfo *getOp() const { return op; }

   Kind getKind() const { return kind; }

   SourceLocation getLoc() const { return loc; }
   SourceLocation getEndLoc() const;
   SourceRange getSourceRange() const { return SourceRange(loc, getEndLoc()); }

   llvm::StringRef asString() const
   {
      assert(!isExpression() && "cannot stringify expression");
      return isOperator() ? op::toString(operatorKind)
                          : op->getIdentifier();
   }

protected:
   union {
      mutable Expression* expr;
      op::OperatorKind operatorKind;
      IdentifierInfo *op;
   };

   Kind kind;
   SourceLocation loc;
};

class ExprSequence final: public Expression,
                          llvm::TrailingObjects<ExprSequence, SequenceElement> {
   explicit ExprSequence(llvm::MutableArrayRef<SequenceElement> fragments);

   unsigned NumFragments;

public:
   static ExprSequence *Create(ASTContext &C,
                               llvm::MutableArrayRef<SequenceElement>fragments);

   SourceRange getSourceRange() const;

   unsigned getNumFragments() const { return NumFragments; }

   llvm::ArrayRef<SequenceElement> getFragments() const
   {
      return { getTrailingObjects<SequenceElement>(), NumFragments };
   }

   static bool classofKind(NodeType kind) { return kind == ExprSequenceID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   friend TrailingObjects;
};

class UnaryOperator: public Expression {
   UnaryOperator(SourceLocation operatorLoc,
                 op::OperatorKind opKind,
                 FunctionType *FuncTy,
                 Expression* target,
                 bool prefix);

   SourceLocation operatorLoc;
   op::OperatorKind kind;
   FunctionType *FuncTy;
   Expression* target;
   bool prefix;

public:
   static bool classofKind(NodeType kind) { return kind == UnaryOperatorID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static UnaryOperator *Create(ASTContext &C,
                                SourceLocation operatorLoc,
                                op::OperatorKind opKind,
                                FunctionType *FuncTy,
                                Expression* target,
                                bool prefix);

   SourceRange getSourceRange() const;

   op::OperatorKind getKind() const { return kind; }
   bool isPrefix() const { return prefix; }
   FunctionType *getFunctionType() const { return FuncTy; }

   Expression* getTarget() const { return target; }
   void setTarget(Expression *T) { target = T; }
};

class BinaryOperator: public Expression {
   BinaryOperator(SourceLocation operatorLoc,
                  op::OperatorKind operatorKind,
                  FunctionType *FuncTy,
                  Expression* lhs, Expression* rhs);

   SourceLocation operatorLoc;

   op::OperatorKind kind;
   FunctionType *FuncTy;

   Expression* lhs;
   Expression* rhs;

public:
   static bool classofKind(NodeType kind) { return kind == BinaryOperatorID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static BinaryOperator *Create(ASTContext &C,
                                 SourceLocation operatorLoc,
                                 op::OperatorKind operatorKind,
                                 FunctionType *FuncTy,
                                 Expression* lhs, Expression* rhs);

   SourceRange getSourceRange() const;
   SourceLocation getOperatorLoc() const { return operatorLoc; }

   Expression* getLhs() const { return lhs; }
   Expression* getRhs() const { return rhs; }

   void setRhs(Expression *RHS) { rhs = RHS; }
   void setLhs(Expression *LHS) { lhs = LHS; }

   op::OperatorKind getKind() const { return kind; }
   void setKind(op::OperatorKind K) { kind = K; }

   FunctionType *getFunctionType() const { return FuncTy; }
   void setFunctionType(FunctionType *Ty) { FuncTy = Ty; }
};

class AssignExpr: public Expression {
   AssignExpr(SourceLocation EqualsLoc,
              Expression *LHS,
              Expression *RHS,
              bool IsInitialization);

   SourceLocation EqualsLoc;
   Expression* LHS;
   Expression* RHS;

   bool IsInitialization : 1;

public:
   static AssignExpr *Create(ASTContext &C,
                             SourceLocation EqualsLoc,
                             Expression *LHS,
                             Expression *RHS,
                             bool IsInitialization = false);

   static bool classofKind(NodeType kind) { return kind == AssignExprID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   SourceRange getSourceRange() const;
   SourceLocation getEqualsLoc() const { return EqualsLoc; }

   Expression* getLhs() const { return LHS; }
   Expression* getRhs() const { return RHS; }

   void setRhs(Expression *rhs) { RHS = rhs; }
   void setLhs(Expression *lhs) { LHS = lhs; }

   bool isInitialization() const { return IsInitialization; }
   void setIsInitialization(bool I) { IsInitialization = I; }
};

class TypePredicateExpr: public Expression {
   TypePredicateExpr(SourceLocation IsLoc, SourceRange SR,
                     Expression *LHS, ConstraintExpr *RHS);

   SourceLocation IsLoc;
   SourceRange SR;
   Expression *LHS;
   ConstraintExpr *RHS;

   bool Result           : 1;
   bool CompileTimeCheck : 1;

public:
   static bool classofKind(NodeType kind){ return kind == TypePredicateExprID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static TypePredicateExpr *Create(ASTContext &C,
                                    SourceLocation IsLoc, SourceRange SR,
                                    Expression *LHS, ConstraintExpr *RHS);

   SourceLocation getIsLoc() const { return IsLoc; }
   SourceRange getSourceRange() const { return SR; }

   Expression *getLHS() const { return LHS; }
   ConstraintExpr *getRHS() const { return RHS; }

   void setLHS(Expression *L) { LHS = L; }

   bool getResult() const { return Result; }
   void setResult(bool Result) { TypePredicateExpr::Result = Result; }

   bool isCompileTimeCheck() const { return CompileTimeCheck; }
   void setIsCompileTimeCheck(bool Comp) { CompileTimeCheck = Comp; }
};

class CastExpr: public Expression {
   CastExpr(SourceLocation AsLoc,
            CastStrength strength,
            Expression *target,
            SourceType targetType);

   SourceLocation AsLoc;
   CastStrength strength;
   Expression *target;
   SourceType targetType;
   ConversionSequence ConvSeq;

public:
   static bool classofKind(NodeType kind){ return kind == CastExprID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static CastExpr *Create(ASTContext &C,
                           SourceLocation AsLoc,
                           CastStrength strength,
                           Expression *target,
                           SourceType targetType);

   SourceLocation getAsLoc() const { return AsLoc; }
   SourceRange getSourceRange() const;

   CastStrength getStrength() const { return strength; }
   Expression *getTarget() const { return target; }
   const SourceType &getTargetType() { return targetType; }

   const ConversionSequence &getConvSeq() const { return ConvSeq; }

   void setTarget(Expression *T) { target = T; }
   void setConvSeq(ConversionSequence &&Seq) { ConvSeq = std::move(Seq); }
};

class IfExpr: public Expression {
   IfExpr(SourceLocation IfLoc,
          Expression *Cond,
          Expression *TrueVal, Expression *FalseVal);

   SourceLocation IfLoc;

   Expression *Cond;
   Expression *TrueVal;
   Expression *FalseVal;

public:
   static bool classofKind(NodeType kind) { return kind == IfExprID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static IfExpr *Create(ASTContext &C, SourceLocation IfLoc,
                         Expression *Cond,
                         Expression *TrueVal, Expression *FalseVal);

   SourceLocation getIfLoc() const { return IfLoc; }
   SourceRange getSourceRange() const
   {
      return SourceRange(IfLoc, FalseVal->getSourceRange().getEnd());
   }

   Expression *getCond()     const { return Cond; }
   Expression *getTrueVal()  const { return TrueVal; }
   Expression *getFalseVal() const { return FalseVal; }

   void setCond(Expression *C) { Cond = C; }
   void setTrueVal(Expression *Val) { TrueVal = Val; }
   void setFalseVal(Expression *Val) { FalseVal = Val; }
};

class PatternExpr: public Expression {
public:
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind)
   {
      switch (kind) {
      case ExpressionPatternID:
      case IsPatternID:
      case CasePatternID:
         return true;
      default:
         return false;
      }
   }

   SourceLocation getColonLoc() const { return ColonLoc; }

protected:
   explicit PatternExpr(NodeType type, SourceLocation ColonLoc)
      : Expression(type),
        ColonLoc(ColonLoc)
   {}

   SourceLocation ColonLoc;
};

class ExpressionPattern: public PatternExpr {
   explicit ExpressionPattern(SourceLocation ColonLoc, Expression* expr);

   Expression* expr;

public:
   static bool classofKind(NodeType kind){ return kind == ExpressionPatternID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static ExpressionPattern *Create(ASTContext &C,
                                    SourceLocation ColonLoc,
                                    Expression *E);

   Expression* getExpr() const { return expr; }
   void setExpr(Expression *E) { expr = E; }

   SourceRange getSourceRange() const { return expr->getSourceRange(); }
};

struct CasePatternArgument {
   explicit CasePatternArgument(Expression* expr,
                                SourceLocation loc = {})
      : expr(expr), sourceLoc(this->expr->getSourceLoc()),
        IsExpr(true), IsConst(false)
   {}

   CasePatternArgument(IdentifierInfo *II, bool isConst, SourceLocation loc)
      : identifier(II), sourceLoc(loc), IsExpr(false),
        IsConst(isConst)
   {}

   Expression* getExpr() const { return expr; }
   IdentifierInfo *getIdentifier() const { return identifier; }
   bool isExpr() const { return IsExpr; }
   bool isConst() const { return IsConst; }
   const SourceLocation &getSourceLoc() const { return sourceLoc; }

   void setExpr(Expression *E) const
   {
      assert(isExpr());
      expr = E;
   }

private:
   union {
      mutable Expression* expr = nullptr;
      IdentifierInfo *identifier;
   };

   SourceLocation sourceLoc;

   bool IsExpr;
   bool IsConst;
};

class CasePattern final:
   public PatternExpr,
   llvm::TrailingObjects<CasePattern, CasePatternArgument> {
private:
   CasePattern(SourceRange SR,
               IdentifierInfo *caseName,
               llvm::MutableArrayRef<CasePatternArgument> args);

   SourceRange SR;

   IdentifierInfo *caseName;
   unsigned NumArgs;

   bool HasBinding : 1;
   bool HasExpr    : 1;

   EnumCaseDecl *CaseDecl = nullptr;
   std::vector<LocalVarDecl*> varDecls;

public:
   static bool classofKind(NodeType kind){ return kind == CasePatternID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static CasePattern *Create(ASTContext &C,
                              SourceRange SR,
                              IdentifierInfo *caseName,
                              llvm::MutableArrayRef<CasePatternArgument> args);

   SourceRange getSourceRange() const { return SR; }

   llvm::StringRef getCaseName() const { return caseName->getIdentifier(); }
   IdentifierInfo *getCaseNameIdent() const { return caseName; }

   llvm::ArrayRef<CasePatternArgument> getArgs() const
   {
      return { getTrailingObjects<CasePatternArgument>(), NumArgs };
   }

   llvm::ArrayRef<LocalVarDecl*> getVarDecls() const { return varDecls; }
   void setVarDecls(std::vector<LocalVarDecl*> &&decls)
   {
      varDecls = move(decls);
   }

   EnumCaseDecl *getCaseDecl() const { return CaseDecl; }
   void setCaseDecl(EnumCaseDecl *CD) { CaseDecl = CD; }

   bool hasBinding() const { return HasBinding; }
   bool hasExpr() const { return HasExpr; }

   SourceLocation getPeriodLoc() const { return SR.getStart(); }

   friend TrailingObjects;
};

class IsPattern: public PatternExpr {
   IsPattern(SourceRange SR, SourceType isType);

   SourceLocation IsLoc;
   SourceType isType;

public:
   static bool classofKind(NodeType kind){ return kind == IsPatternID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static IsPattern *Create(ASTContext &C, SourceRange SR, SourceType isType);

   SourceLocation getIsLoc() const { return IsLoc; }
   SourceRange getSourceRange() const { return { IsLoc, ColonLoc }; }

   SourceType getIsType() const { return isType; }
};

class IntegerLiteral : public Expression {
public:
   enum class Suffix: unsigned char {
      None, u, i, i1, u1, i8, u8, i16, u16, i32, u32, i64, u64, i128, u128
   };

private:
   IntegerLiteral(SourceRange Loc,
                  QualType type,
                  llvm::APSInt &&value,
                  Suffix suffix);

   SourceRange Loc;
   llvm::APSInt value;
   QualType type;
   Suffix suffix;

public:
   static bool classofKind(NodeType kind){ return kind == IntegerLiteralID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static IntegerLiteral *Create(ASTContext &C,
                                 SourceRange Loc,
                                 QualType type,
                                 llvm::APSInt &&value,
                                 Suffix suffix = Suffix::None);

   SourceRange getSourceRange() const { return Loc; }

   const llvm::APSInt &getValue() const { return value; }
   void setValue(llvm::APSInt &&Val) { value = std::move(Val); }

   QualType getType() const { return type; }
   void setType(QualType type) { IntegerLiteral::type = type; }

   Suffix getSuffix() const { return suffix; }
};

class FPLiteral: public Expression {
public:
   enum class Suffix: unsigned char {
      None, d, f
   };

private:
   FPLiteral(SourceRange Loc,
             QualType type,
             llvm::APFloat &&value,
             Suffix suffix);

   SourceRange Loc;
   llvm::APFloat value;
   QualType type;
   Suffix suffix;

public:
   static bool classofKind(NodeType kind){ return kind == FPLiteralID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static FPLiteral *Create(ASTContext &C,
                            SourceRange Loc,
                            QualType type,
                            llvm::APFloat &&value,
                            Suffix suffix = Suffix::None);

   SourceRange getSourceRange() const { return Loc; }

   const llvm::APFloat &getValue() const { return value; }

   QualType getType() const { return type; }
   void setType(QualType type) { FPLiteral::type = type; }

   Suffix getSuffix() const { return suffix; }
};

class BoolLiteral: public Expression {
   BoolLiteral(SourceLocation Loc, QualType type, bool value);

   SourceLocation Loc;
   bool value;
   QualType type;

public:
   static bool classofKind(NodeType kind){ return kind == BoolLiteralID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static BoolLiteral *Create(ASTContext &C, SourceLocation Loc,
                              QualType type, bool value);

   SourceRange getSourceRange() const
   {
      SourceLocation EndLoc(Loc.getOffset() + (value ? 3 : 4));
      return SourceRange(Loc, EndLoc);
   }

   bool getValue() const { return value; }
   QualType getType() const { return type; }
   void setType(Type *Ty) { type = Ty; }
};

class CharLiteral: public Expression {
private:
   CharLiteral(SourceRange Loc, QualType type, char value);
   CharLiteral(SourceRange Loc, QualType type, uint32_t value);

   SourceRange Loc;

   union {
      char narrow;
      uint32_t wide;
   };

   bool IsWide;
   QualType type;

public:
   static bool classofKind(NodeType kind){ return kind == CharLiteralID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static CharLiteral *Create(ASTContext &C, SourceRange Loc,
                              QualType type, char value);
   static CharLiteral *Create(ASTContext &C, SourceRange Loc,
                              QualType type, uint32_t value);

   SourceRange getSourceRange() const { return Loc; }

   char getNarrow() const { return narrow; }

   uint32_t getWide() const { return wide; }
   bool isWide() const { return IsWide; }

   QualType getType() const { return type; }
   void setType(QualType Ty) { type = Ty; }
};

class NoneLiteral: public Expression {
   explicit NoneLiteral(SourceLocation Loc);

   SourceLocation Loc;

public:
   static bool classofKind(NodeType kind){ return kind == NoneLiteralID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static NoneLiteral *Create(ASTContext &C, SourceLocation Loc);

   SourceRange getSourceRange() const
   {
      return SourceRange(Loc, SourceLocation(Loc.getOffset() + 4));
   }
};

class StringLiteral: public Expression {
   StringLiteral(SourceRange Loc, std::string &&str);

   SourceRange Loc;
   std::string value;
   bool cstring = false;

public:
   static bool classofKind(NodeType kind){ return kind == StringLiteralID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static StringLiteral *Create(ASTContext &C,  SourceRange Loc,
                                std::string &&str);

   SourceRange getSourceRange() const { return Loc; }

   llvm::StringRef getValue() const { return value; }
   bool isCString() const { return cstring; }
   void setCString(bool raw) { cstring = raw; }
};

struct InterpolationSegment {
   Expression *Expr = nullptr;
   CallableDecl *ToStringFn = nullptr;
};

class StringInterpolation final: public Expression,
                                 llvm::TrailingObjects<StringInterpolation,
                                                       Expression*> {
   StringInterpolation(SourceRange Loc,
                       llvm::ArrayRef<Expression*> strings);

   SourceRange Loc;
   unsigned NumSegments;

public:
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind)
   {
      return kind == StringInterpolationID;
   }

   static StringInterpolation *Create(ASTContext &C,
                                      SourceRange Loc,
                                      llvm::ArrayRef<Expression*> strings);

   SourceRange getSourceRange() const { return Loc; }

   llvm::MutableArrayRef<Expression*> getSegments()
   {
      return { getTrailingObjects<Expression*>(), NumSegments };
   }

   friend TrailingObjects;
};

class LambdaExpr final: public Expression,
                        llvm::TrailingObjects<LambdaExpr, FuncArgDecl*> {
   LambdaExpr(SourceRange Parens,
              SourceLocation ArrowLoc,
              SourceType returnType,
              llvm::ArrayRef<FuncArgDecl* > args,
              Statement* body);

protected:
   SourceRange Parens;
   SourceLocation ArrowLoc;

   unsigned NumArgs;

   SourceType returnType;
   Statement* body;

   std::vector<NamedDecl*> captures;
   FunctionDecl *func;

public:
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == LambdaExprID; }

   static LambdaExpr *Create(ASTContext &C,
                             SourceRange Parens, SourceLocation ArrowLoc,
                             SourceType returnType,
                             llvm::ArrayRef<FuncArgDecl* > args,
                             Statement* body);

   friend TrailingObjects;

   SourceRange getParenRange() const { return Parens; }
   SourceRange getSourceRange() const;

   SourceLocation getSourceLoc() const { return Parens.getStart(); }
   SourceLocation getArrowLoc() const { return ArrowLoc; }

   unsigned getNumArgs() const { return NumArgs; }

   const SourceType &getReturnType() const { return returnType; }
   llvm::ArrayRef<FuncArgDecl* > getArgs() const
   {
      return { getTrailingObjects<FuncArgDecl*>(), NumArgs };
   }

   Statement* getBody() const { return body; }
   void setBody(Statement *B) { body = B; }

   llvm::ArrayRef<NamedDecl*> getCaptures() const { return captures; }
   size_t addCapture(NamedDecl *ND);

   FunctionDecl *getFunc() const { return func; }
   void setFunc(FunctionDecl *fn) { func = fn; }
};

class TupleLiteral final: public Expression,
                          llvm::TrailingObjects<TupleLiteral, Expression*> {
public:
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == TupleLiteralID; }

   static TupleLiteral *Create(ASTContext &C,
                               SourceRange Parens,
                               llvm::ArrayRef<Expression*> Exprs);

   SourceRange getSourceRange() const { return Parens; }

   llvm::ArrayRef<Expression*> getElements() const
   {
      return { getTrailingObjects<Expression*>(), NumElements };
   }

   llvm::MutableArrayRef<Expression*> getElements()
   {
      return { getTrailingObjects<Expression*>(), NumElements };
   }

   friend TrailingObjects;

private:
   TupleLiteral(SourceRange Parens, llvm::ArrayRef<Expression*> Exprs);

   SourceRange Parens;
   unsigned NumElements;
};


class ArrayLiteral final: public Expression,
                          llvm::TrailingObjects<ArrayLiteral, Expression*> {
public:
   static ArrayLiteral *Create(ASTContext &ASTCtx,
                               SourceRange SquareRange,
                               llvm::ArrayRef<Expression*> exprs);

   SourceRange getSourceRange() const { return SquareRange; }

   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind)
   {
      return kind == ArrayLiteralID;
   }

   friend TrailingObjects;

private:
   ArrayLiteral(SourceRange SquareRange,
                llvm::ArrayRef<Expression*> exprs);

   SourceRange SquareRange;
   unsigned numElements;

public:
   size_t size() const { return numElements; }

   llvm::MutableArrayRef<Expression*> getValues()
   {
      return llvm::MutableArrayRef<Expression*>(
         getTrailingObjects<Expression*>(), numElements);
   }

   llvm::ArrayRef<Expression*> getValues() const
   {
      return llvm::ArrayRef<Expression*>(
         getTrailingObjects<Expression*>(), numElements);
   }
};

class DictionaryLiteral final:
   public Expression,
   llvm::TrailingObjects<DictionaryLiteral, Expression*> {
public:
   static DictionaryLiteral *Create(ASTContext &ASTCtx,
                                    SourceRange SquareRange,
                                    llvm::ArrayRef<Expression*> keys,
                                    llvm::ArrayRef<Expression*> values);

   SourceRange getSourceRange() const { return SquareRange; }

   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind)
   {
      return kind == DictionaryLiteralID;
   }

   friend TrailingObjects;

private:
   DictionaryLiteral(SourceRange SquareRange,
                     llvm::ArrayRef<Expression*> keys,
                     llvm::ArrayRef<Expression*> values);

   SourceRange SquareRange;
   unsigned numKeyValuePairs;
   MethodDecl *InsertFn = nullptr;

public:
   size_t size() const { return numKeyValuePairs; }

   llvm::MutableArrayRef<Expression*> getKeys()
   {
      return llvm::MutableArrayRef<Expression*>(
         getTrailingObjects<Expression*>(), size());
   }

   llvm::ArrayRef<Expression*> getKeys() const
   {
      return llvm::ArrayRef<Expression*>(
         getTrailingObjects<Expression*>(), size());
   }

   llvm::MutableArrayRef<Expression*> getValues()
   {
      return llvm::MutableArrayRef<Expression*>(
         getTrailingObjects<Expression*>() + size(), size());
   }

   llvm::ArrayRef<Expression*> getValues() const
   {
      return llvm::ArrayRef<Expression*>(
         getTrailingObjects<Expression*>() + size(), size());
   }

   MethodDecl *getInsertFn() const { return InsertFn; }
   void setInsertFn(MethodDecl *Fn) { InsertFn = Fn; }
};

enum class IdentifierKind {
   Unknown,
   LocalVar,
   GlobalVar,
   FunctionArg,
   Function,
   TemplateParam,
   AssociatedType,
   Namespace,
   MetaType,
   Alias,

   Accessor,
   UnionAccess,
   Type,
   TypeOf,
   StaticField,
   Field,
   EnumRawValue,
   PartiallyAppliedMethod,
};

class IdentifierRefExpr : public IdentifiedExpr {
   SourceRange Loc;
   IdentifierKind kind = IdentifierKind::Unknown;
   Expression *ParentExpr = nullptr;
   DeclContext *DeclCtx = nullptr;

   union {
      Type *builtinType = nullptr;
      MetaType *metaType;
      NamedDecl *ND;
      VarDecl *varDecl;
      CallableDecl *callable;
      LocalVarDecl *localVar;
      GlobalVarDecl *globalVar;
      FuncArgDecl *funcArg;
      NamespaceDecl *namespaceDecl;
      AliasDecl *Alias;
      FieldDecl *staticFieldDecl;
      FieldDecl *fieldDecl;
      PropDecl *accessor;
      MethodDecl *partiallyAppliedMethod;
      TemplateParamDecl *templateParam;
   };

   BuiltinIdentifier builtinKind;

   bool staticLookup    : 1;
   bool pointerAccess   : 1;
   bool FoundResult     : 1;
   bool InTypePosition  : 1;
   bool IsSynthesized   : 1;
   bool IsCapture       : 1;
   bool AllowIncompleteTemplateArgs : 1;

   size_t captureIndex = 0;
   std::vector<Expression*> templateArgs;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind){ return kind == IdentifierRefExprID; }

   explicit IdentifierRefExpr(SourceRange Loc,
                              DeclarationName Name,
                              std::vector<Expression*> &&templateArgs = {},
                              DeclContext *DeclCtx = nullptr,
                              bool InTypePos = false);

   IdentifierRefExpr(SourceRange Loc,
                     IdentifierKind kind,
                     QualType exprType);

   IdentifierRefExpr(SourceRange Loc,
                     IdentifierKind kind,
                     NamedDecl *ND,
                     QualType exprType);

   IdentifierRefExpr(IdentifierRefExpr &&expr) = default;

   SourceRange getSourceRange() const { return Loc; }

   Expression *getParentExpr() const { return ParentExpr; }
   void setParentExpr(Expression *PE) { ParentExpr = PE; }

   Type *getBuiltinType() const { return builtinType; }
   void setBuiltinType(Type *BT) { builtinType = BT; }

   MetaType *getMetaType() const { return metaType; }
   void setMetaType(MetaType *MT) { metaType = MT; }

   BuiltinIdentifier getBuiltinKind() const { return builtinKind; }
   void setBuiltinKind(BuiltinIdentifier BK) { builtinKind = BK; }

   llvm::ArrayRef<Expression*> getTemplateArgs() const { return templateArgs; }
   std::vector<Expression*> &getTemplateArgRef() { return templateArgs; }
   void setTemplateArgs(std::vector<Expression*> &&TAs)
   {
      templateArgs = move(TAs);
   }

   AliasDecl *getAlias() const { return Alias; }
   void setAlias(AliasDecl *Alias) { IdentifierRefExpr::Alias = Alias; }

   CallableDecl *getCallable() const { return callable; }
   void setCallable(CallableDecl *callable)
   {
      kind = IdentifierKind::Function;
      IdentifierRefExpr::callable = callable;
   }

   IdentifierKind getKind() const { return kind; }
   void setKind(IdentifierKind IK) { kind = IK; }

   NamedDecl *getNamedDecl() const { return ND; }

   VarDecl *getVarDecl() const { return varDecl; }

   LocalVarDecl *getLocalVar() const { return localVar; }
   void setLocalVar(LocalVarDecl *LV) { localVar = LV; }

   GlobalVarDecl *getGlobalVar() const { return globalVar; }
   void setGlobalVar(GlobalVarDecl *GV) { globalVar = GV; }

   FuncArgDecl *getFuncArg() const { return funcArg; }
   void setFuncArg(FuncArgDecl *Arg) { funcArg = Arg; }

   NamespaceDecl *getNamespaceDecl() const { return namespaceDecl; }
   void setNamespaceDecl(NamespaceDecl *NS) { namespaceDecl = NS; }

   DeclContext *getDeclCtx() const { return DeclCtx; }
   void setDeclCtx(DeclContext *Ctx) { DeclCtx = Ctx; }

   FieldDecl *getStaticFieldDecl() const { return staticFieldDecl; }
   void setStaticFieldDecl(FieldDecl *staticFieldDecl)
   {
      kind = IdentifierKind::StaticField;
      IdentifierRefExpr::staticFieldDecl = staticFieldDecl;
   }

   FieldDecl *getFieldDecl() const { return fieldDecl; }
   void setFieldDecl(FieldDecl *fieldDecl)
   {
      kind = IdentifierKind::Field;
      IdentifierRefExpr::fieldDecl = fieldDecl;
   }

   PropDecl *getAccessor() const { return accessor; }
   void setAccessor(PropDecl *A)
   {
      kind = IdentifierKind::Accessor;
      accessor = A;
   }

   TemplateParamDecl *getTemplateParam() const { return templateParam; }
   void setTemplateParam(TemplateParamDecl *P) { templateParam = P; }

   bool isStaticLookup() const { return staticLookup; }
   void setStaticLookup(bool SL) { staticLookup = SL; }

   bool foundResult() const { return FoundResult; }
   void setFoundResult(bool found) { FoundResult = found; }

   bool isPointerAccess() const { return pointerAccess; }
   void setPointerAccess(bool pointer) { pointerAccess = pointer; }

   bool isInTypePosition() const { return InTypePosition; }
   void setInTypePos(bool pos) { InTypePosition = pos; }

   bool isSynthesized() const { return IsSynthesized; }
   void setIsSynthesized(bool Synth) { IsSynthesized = Synth; }

   bool allowIncompleteTemplateArgs()const {return AllowIncompleteTemplateArgs;}
   void setAllowIncompleteTemplateArgs(bool allow)
   {
      AllowIncompleteTemplateArgs = allow;
   }

   bool isCapture() const { return IsCapture; }
   void setIsCapture(bool C) { IsCapture = C; }

   MethodDecl *getPartiallyAppliedMethod() const
   {
      return partiallyAppliedMethod;
   }

   void setPartiallyAppliedMethod(MethodDecl *partiallyAppliedMethod)
   {
      kind = IdentifierKind::PartiallyAppliedMethod;
      IdentifierRefExpr::partiallyAppliedMethod = partiallyAppliedMethod;
   }

   size_t getCaptureIndex() const { return captureIndex; }
   void setCaptureIndex(size_t CI) { captureIndex = CI; }
};

class SelfExpr: public Expression {
   explicit SelfExpr(SourceLocation Loc);

   SourceLocation Loc;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == SelfExprID; }

   static SelfExpr *Create(ASTContext &C, SourceLocation Loc);

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const
   {
      return SourceRange(Loc, SourceLocation(Loc.getOffset() + 3));
   }
};

class SuperExpr: public Expression {
   explicit SuperExpr(SourceLocation Loc);

   SourceLocation Loc;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == SuperExprID; }

   static SuperExpr *Create(ASTContext &C, SourceLocation Loc);

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const
   {
      return SourceRange(Loc, SourceLocation(Loc.getOffset() + 4));
   }
};

class BuiltinIdentExpr: public Expression {
   BuiltinIdentExpr(SourceLocation Loc, BuiltinIdentifier identifier);

   SourceLocation Loc;
   BuiltinIdentifier identifierKind;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == BuiltinIdentExprID; }

   static BuiltinIdentExpr *Create(ASTContext &C, SourceLocation Loc,
                                   BuiltinIdentifier identifier);

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const { return SourceRange(Loc); }
   BuiltinIdentifier getIdentifierKind() const { return identifierKind; }
};

class BuiltinExpr: public Expression {
   explicit BuiltinExpr(QualType type);

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == BuiltinExprID; }

   static BuiltinExpr *Create(ASTContext &C, QualType Ty);
   static BuiltinExpr CreateTemp(QualType Ty);

   SourceRange getSourceRange() const { return SourceRange(); }
};

enum class MemberKind : unsigned {
   Unknown,
   Alias,
   Accessor,
   UnionAccess,
   Type,
   GlobalVariable,
   StaticField,
   Namespace,
   Field,
   EnumRawValue,
   AssociatedType,
   Function,
   TypeOf,
};

class MemberRefExpr: public IdentifiedExpr {
   SourceLocation Loc;
   MemberKind kind = MemberKind::Unknown;
   Expression *ParentExpr = nullptr;
   DeclContext *Context = nullptr;

   std::vector<Expression*> templateArgs;

   RecordDecl *record = nullptr;
   QualType fieldType;

   bool pointerAccess : 1;

   union {
      Type *metaType = nullptr;
      CallableDecl *callable;
      VarDecl *globalVar;
      FieldDecl *staticFieldDecl;
      FieldDecl *fieldDecl;
      Variant *aliasVal;
      MethodDecl *accessorMethod;
   };

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == MemberRefExprID; }

   MemberRefExpr(SourceLocation Loc,
                 DeclarationName Name,
                 bool pointerAccess = false);

   MemberRefExpr(SourceLocation Loc,
                 Expression *ParentExpr,
                 DeclarationName Name,
                 bool pointerAccess = false);

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const
   {
      return SourceRange(Loc, SourceLocation(Loc.getOffset()
                                             + getIdent().size() - 1));
   }

   Expression *getParentExpr() const { return ParentExpr; }
   void setParentExpr(Expression *Expr) { ParentExpr = Expr; }

   DeclContext *getContext() const { return Context; }
   void setContext(DeclContext *C) { Context = C; }

   RecordDecl *getRecord() const { return record; }
   void setRecord(RecordDecl *R) { record = R; }

   llvm::ArrayRef<Expression*> getTemplateArgs() const { return templateArgs; }
   std::vector<Expression*>& getTemplateArgRef() { return templateArgs; }
   void setTemplateArgs(std::vector<Expression*> &&TAs)
   {
      templateArgs = std::move(TAs);
   }

   const Variant &getAliasVal() const
   {
      assert(aliasVal && "not an alias");
      return *aliasVal;
   }

   void setAliasVal(Variant *aliasVal)
   {
      this->aliasVal = aliasVal;
      kind = MemberKind::Alias;
   }

   MemberKind getKind() const { return kind; }
   void setKind(MemberKind MK) { kind = MK; }

   CallableDecl *getCallable() const { return callable; }
   void setCallable(CallableDecl *C) { callable = C; }

   QualType getFieldType() const { return fieldType; }
   void setFieldType(QualType Ty) { fieldType = Ty; }

   bool isPointerAccess() const { return pointerAccess; }
   void setIsPointerAccess(bool PA) { this->pointerAccess = PA; }

   Type *getMetaType() const { return metaType; }
   void setMetaType(Type *Ty) { metaType = Ty; }

   VarDecl *getGlobalVar() const { return globalVar; }
   void setGlobalVar(VarDecl *globalVar)
   {
      kind = MemberKind::GlobalVariable;
      MemberRefExpr::globalVar = globalVar;
   }

   FieldDecl *getStaticFieldDecl() const { return staticFieldDecl; }
   void setGlobalFieldDecl(FieldDecl *staticFieldDecl)
   {
      kind = MemberKind::StaticField;
      MemberRefExpr::staticFieldDecl = staticFieldDecl;
   }

   FieldDecl *getFieldDecl() const { return fieldDecl; }
   void setFieldDecl(FieldDecl *fieldDecl)
   {
      kind = MemberKind::Field;
      MemberRefExpr::fieldDecl = fieldDecl;
   }

   MethodDecl *getAccessorMethod() const { return accessorMethod; }
   void setAccessorMethod(MethodDecl *AM) { accessorMethod = AM; }
};

class TupleMemberExpr: public Expression {
   SourceLocation Loc;
   Expression *ParentExpr;

   unsigned Index     : 31;
   bool PointerAccess : 1;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == TupleMemberExprID; }

   TupleMemberExpr(SourceLocation Loc,
                   Expression *ParentExpr,
                   unsigned Index,
                   bool PointerAccess = false);

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const { return SourceRange(Loc); }

   Expression *getParentExpr() const { return ParentExpr; }
   void setParentExpr(Expression *Expr) { ParentExpr = Expr; }

   unsigned getIndex() const { return Index; }
   bool isPointerAccess() const { return PointerAccess; }
};

class EnumCaseExpr: public IdentifiedExpr {
   SourceLocation PeriodLoc;
   std::vector<Expression*> args;

   EnumDecl *en;
   EnumCaseDecl *Case;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == EnumCaseExprID; }

   EnumCaseExpr(SourceLocation PeriodLoc,
                IdentifierInfo *ident,
                std::vector<Expression*> &&args = {});

   EnumCaseExpr(SourceLocation PeriodLoc,
                EnumDecl *E,
                IdentifierInfo *ident,
                std::vector<Expression*> &&args = {});

   SourceLocation getSourceLoc() const { return PeriodLoc; }
   SourceRange getSourceRange() const
   {
      return SourceRange(PeriodLoc,
                         SourceLocation(PeriodLoc.getOffset()
                                        + getIdent().size()));
   }

   std::vector<Expression*> &getArgs() { return args; }
   llvm::ArrayRef<Expression*> getArgs() const { return args; }

   EnumDecl *getEnum() const { return en; }
   void setEnum(EnumDecl *E) { en = E; }

   EnumCaseDecl *getCase() const { return Case; }
   void setCase(EnumCaseDecl *Case) { EnumCaseExpr::Case = Case; }
};

enum class CallKind: unsigned {
   Unknown,
   Builtin,
   NamedFunctionCall,
   MethodCall,
   UnionInitializer,
   InitializerCall,
   StaticMethodCall,
   CallOperator,
   UnsafeTupleGet,
   VariadicSizeof,
   PrimitiveInitializer,
   GlobalVariableCall,
   LocalVariableCall,
};

class CallExpr: public Expression {
   SourceLocation IdentLoc;
   SourceRange ParenRange;

   DeclarationName ident;

   CallKind kind = CallKind::Unknown;
   Expression *ParentExpr = nullptr;
   DeclContext *Context = nullptr;

   std::vector<Expression*> args;

   bool PointerAccess : 1;
   bool IsUFCS        : 1;
   bool IsDotInit     : 1;
   bool IsDotDeinit   : 1;

   BuiltinFn builtinFnKind = BuiltinFn(0);
   CallableDecl *func = nullptr;

   std::vector<Expression*> templateArgs;

   union {
      Type *builtinArgType = nullptr;
      UnionDecl *U;
   };

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == CallExprID; }

   CallExpr(SourceLocation IdentLoc, SourceRange ParenRange,
            std::vector<Expression* > &&args,
            DeclarationName Name = DeclarationName(),
            bool IsDotInit = false,
            bool IsDotDeinit = false);

   CallExpr(SourceLocation IdentLoc, SourceRange ParenRange,
            Expression *ParentExpr,
            std::vector<Expression* > &&args,
            DeclarationName Name = DeclarationName(),
            bool IsDotInit = false,
            bool IsDotDeinit = false);

   CallExpr(SourceLocation IdentLoc, SourceRange ParenRange,
            std::vector<Expression* > &&args, CallableDecl *C);

   CallExpr(const CallExpr &) = default;

   SourceLocation getIdentLoc() const { return IdentLoc; }
   SourceRange getParenRange() const { return ParenRange; }
   SourceRange getSourceRange() const
   {
      if (IdentLoc)
         return { IdentLoc, ParenRange.getEnd() };

      return ParenRange;
   }

   DeclarationName getDeclName() const { return ident; }
   void setIdent(DeclarationName ident) { this->ident = ident; }

   bool isNamedCall() const { return ident.isSimpleIdentifier(); }

   Expression *getParentExpr() const { return ParentExpr; }
   void setParentExpr(Expression *E) { ParentExpr = E; }

   DeclContext *getContext() const { return Context; }
   void setContext(DeclContext *C) { Context = C; }

   std::vector<Expression*> &getArgs() { return args; }
   llvm::ArrayRef<Expression*> getArgs() const { return args; }

   CallableDecl *getFunc() const { return func; }
   void setFunc(CallableDecl *func) { this->func = func; }

   std::vector<Expression*> &getTemplateArgs() { return templateArgs; }
   llvm::ArrayRef<Expression*> getTemplateArgs() const { return templateArgs; }
   void setTemplateArgs(std::vector<Expression*> &&TAs)
   {
      templateArgs = std::move(TAs);
   }

   Type *getBuiltinArgType() const { return builtinArgType; }
   void setBuiltinArgType(Type *Ty) { builtinArgType = Ty; }

   UnionDecl *getUnion() const { return U; }
   void setUnion(UnionDecl *U) { CallExpr::U = U; }

   CallKind getKind() const { return kind; }
   void setKind(CallKind CK) { kind = CK; }

   bool isPointerAccess() const { return PointerAccess; }
   void setIsPointerAccess(bool access) { PointerAccess = access; }

   bool isUFCS() const { return IsUFCS; }
   void setIsUFCS(bool IsUFCS) { CallExpr::IsUFCS = IsUFCS; }

   QualType getReturnType() { return exprType; }
   void setReturnType(QualType ty) { exprType = ty; }

   bool isDotInit() const { return IsDotInit; }
   bool isDotDeinit() const { return IsDotDeinit; }
   void setIsDotInit(bool b) { IsDotInit = b; }

   BuiltinFn getBuiltinFnKind() const { return builtinFnKind; }
   void setBuiltinFnKind(BuiltinFn kind) { builtinFnKind = kind; }

   bool isKnownFunctionCall();
};

class AnonymousCallExpr final: public Expression,
                               llvm::TrailingObjects<AnonymousCallExpr,
                                                     Expression*> {
   AnonymousCallExpr(SourceRange ParenRange,
                     Expression *ParentExpr,
                     llvm::ArrayRef<Expression*> Args);

   SourceRange ParenRange;

   Expression *ParentExpr;
   FunctionType *FnTy;

   unsigned NumArgs     : 31;
   bool IsPrimitiveInit : 1;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) {return kind == AnonymousCallExprID; }

   friend TrailingObjects;

   static AnonymousCallExpr *Create(ASTContext &C,
                                    SourceRange ParenRange,
                                    Expression *ParentExpr,
                                    llvm::ArrayRef<Expression*> Args);

   SourceRange getSourceRange() const { return ParenRange; }
   SourceRange getParenRange() const { return ParenRange; }

   Expression *getParentExpr() const { return ParentExpr; }
   void setParentExpr(Expression *PE) { ParentExpr = PE; }

   FunctionType *getFunctionType() const { return FnTy; }
   void setFunctionType(FunctionType *FnTy) { AnonymousCallExpr::FnTy = FnTy; }

   llvm::MutableArrayRef<Expression*> getArgs()
   {
      return { getTrailingObjects<Expression*>(), NumArgs };
   }

   bool isPrimitiveInit() const { return IsPrimitiveInit; }
   void setIsPrimitiveInit(bool PI) { IsPrimitiveInit = PI; }
};

class SubscriptExpr final: public Expression,
                           llvm::TrailingObjects<SubscriptExpr, Expression*> {
   SubscriptExpr(SourceRange SquareRange,
                 Expression *ParentExpr,
                 llvm::ArrayRef<Expression*> indices);

   SourceRange SquareRange;
   Expression *ParentExpr;
   unsigned NumIndices;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == SubscriptExprID; }

   friend TrailingObjects;

   static SubscriptExpr *Create(ASTContext &C, SourceRange SquareRange,
                                Expression *ParentExpr,
                                llvm::ArrayRef<Expression*> indices);

   SourceRange getSourceRange() const { return SquareRange; }

   Expression *getParentExpr() const { return ParentExpr; }
   void setParentExpr(Expression *PE) { ParentExpr = PE; }

   llvm::ArrayRef<Expression*> getIndices() const
   {
      return { getTrailingObjects<Expression*>(), NumIndices };
   }

   llvm::MutableArrayRef<Expression*> getIndices()
   {
      return { getTrailingObjects<Expression*>(), NumIndices };
   }

   bool hasSingleIndex() const { return NumIndices == 1; }
};


class StaticExpr: public Expression {
public:
   static StaticExpr *Create(ASTContext &C, Expression *Expr);
   static StaticExpr *Create(ASTContext &C,
                             QualType Type,
                             il::Constant *V);

   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == StaticExprID; }

   SourceRange getSourceRange() const;

private:
   explicit StaticExpr(Expression* expr);
   explicit StaticExpr(QualType Type, il::Constant *V);

   Expression* expr;
   il::Constant *evaluatedExpr;

public:
   Expression* getExpr() const { return expr; }
   void setExpr(Expression *E) { expr = E; }

   il::Constant *getEvaluatedExpr() const { return evaluatedExpr; }
   void setEvaluatedExpr(il::Constant *V) { evaluatedExpr = V; }
};

class ConstraintExpr: public Expression {
public:
   enum Kind {
      Struct, Class,
      Enum, Union, Function,
      DefaultConstructible, Pointer, Reference, Type
   };

   Kind getKind() const { return kind; }
   const SourceType &getTypeConstraint() const { return typeConstraint; }

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const { return SourceRange(Loc); }

private:
   ConstraintExpr(SourceLocation Loc, Kind kind);
   ConstraintExpr(SourceLocation Loc, SourceType typeConstraint);

   SourceLocation Loc;
   Kind kind;
   SourceType typeConstraint;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == ConstraintExprID; }

   static ConstraintExpr *Create(ASTContext &C, SourceLocation Loc, Kind kind);
   static ConstraintExpr *Create(ASTContext &C, SourceLocation Loc,
                                 SourceType typeConstraint);
};

struct TraitsArgument {
   enum Kind {
      Stmt, Expr, Type, String
   };

   explicit TraitsArgument(Statement* stmt)
      : kind(Stmt), stmt(stmt)
   {}

   explicit TraitsArgument(Expression* expr)
      : kind(Expr), expr(expr)
   {}

   explicit TraitsArgument(SourceType Ty)
      : kind(Type), Ty(Ty)
   {}

   explicit TraitsArgument(std::string &&str)
      : kind(String), str(move(str))
   {}

   TraitsArgument(TraitsArgument &&other) noexcept
      : kind(other.kind)
   {
      copyData(std::move(other));
   }

   TraitsArgument &operator=(TraitsArgument &&other) noexcept
   {
      destroyValue();
      copyData(std::move(other));

      return *this;
   }

   ~TraitsArgument()
   {
      destroyValue();
   }

   Kind getKind() const
   {
      return kind;
   }

   Statement* getStmt() const
   {
      assert(kind == Stmt);
      return stmt;
   }

   Expression* getExpr() const
   {
      assert(kind == Expr);
      return expr;
   }

   const SourceType &getType() const
   {
      assert(kind == Type);
      return Ty;
   }

   const string &getStr() const
   {
      assert(kind == String);
      return str;
   }

private:
   Kind kind;

   union {
      Statement* stmt;
      Expression* expr;
      std::string str;
   };

   SourceType Ty;

   void destroyValue()
   {
      switch (kind) {
      default:
         break;
      case String:
         str.~string();
         break;
      }
   }

   void copyData(TraitsArgument &&other)
   {
      kind = other.kind;

      switch (kind) {
      case Expr:
         expr = other.expr;
         break;
      case Stmt:
         stmt = other.stmt;
         break;
      case Type:
         Ty = other.Ty;
         break;
      case String:
         new (&str) std::string(move(other.str));
         break;
      }
   }
};

class TraitsExpr final: public Expression,
                        llvm::TrailingObjects<TraitsExpr, TraitsArgument> {
public:
   enum Kind : unsigned {
      Compiles = 0,
      CompileErrors,
      HasMember,
      HasProperty,
      HasStaticMember,
      HasStaticProperty,
      HasMethod,
      HasStaticMethod,
      ValidIdentifier,
      ValidFunction,

      IsInteger,
      IsFloat,
      IntegerBitwidth,
      IsUnsigned,
      FPPrecision,

      Arity,

      Invalid,
   };

   static bool classofKind(NodeType kind) { return kind == TraitsExprID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static TraitsExpr *Create(ASTContext &C,
                             SourceLocation TraitsLoc, SourceRange Parens,
                             Kind kind,
                             llvm::MutableArrayRef<TraitsArgument> args);

   SourceLocation getSourceLoc() const { return TraitsLoc; }
   SourceLocation getTraitsLoc() const { return TraitsLoc; }

   SourceRange getParenRange() const { return Parens; }
   SourceRange getSourceRange() const
   {
      return SourceRange(TraitsLoc, Parens.getEnd());
   }

   Kind getKind() const { return kind; }

   llvm::ArrayRef<TraitsArgument> getArgs() const
   {
      return { getTrailingObjects<TraitsArgument>(), NumArgs };
   }

   friend TrailingObjects;

private:
   TraitsExpr(SourceLocation TraitsLoc, SourceRange Parens,
              Kind kind,
              llvm::MutableArrayRef<TraitsArgument> args);

   SourceLocation TraitsLoc;
   SourceRange Parens;
   Kind kind;
   unsigned NumArgs;
};

class MixinExpr: public Expression {
public:
   static MixinExpr *Create(ASTContext &C, SourceRange Parens,
                            Expression *Expr);

   SourceRange getSourceRange() const { return Parens; }

   Expression *getMixinExpr() const { return Expr; }
   void setMixinExpr(Expression *E) { Expr = E; }

   static bool classofKind(NodeType kind) { return kind == MixinExprID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

private:
   MixinExpr(SourceRange Parens, Expression *Expr);

   SourceRange Parens;
   Expression *Expr;
};

class MacroVariableExpr: public Expression {
   explicit MacroVariableExpr(Expression *E);

   Expression *E;

public:
   static MacroVariableExpr *Create(ASTContext &C, Expression *E);

   static bool classofKind(NodeType kind){ return kind == MacroVariableExprID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   SourceRange getSourceRange() const { return E->getSourceRange(); }
   Expression *getExpr() const { return E; }
};

class MacroExpansionExpr final:
   public Expression,
   llvm::TrailingObjects<MacroExpansionExpr, lex::Token>{
public:
   enum Delimiter {
      Brace, Square, Paren,
   };

   friend TrailingObjects;

private:
   MacroExpansionExpr(SourceRange SR,
                      DeclarationName MacroName,
                      Delimiter Delim,
                      llvm::ArrayRef<lex::Token> Toks,
                      Expression *ParentExpr);

   SourceRange SR;
   Delimiter Delim;
   DeclarationName MacroName;
   unsigned NumTokens;

   Expression *ParentExpr;

public:
   static MacroExpansionExpr *Create(ASTContext &C,
                                     SourceRange SR,
                                     DeclarationName MacroName,
                                     Delimiter Delim,
                                     llvm::ArrayRef<lex::Token> Toks,
                                     Expression *ParentExpr = nullptr);

   static bool classofKind(NodeType kind) {return kind == MacroExpansionExprID;}
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   SourceRange getSourceRange() const { return SR; }
   DeclarationName getMacroName() const { return MacroName; }
   Delimiter getDelim() const { return Delim; }

   Expression *getParentExpr() const { return ParentExpr; }
   void setParentExpr(Expression *E) { ParentExpr = E; }

   llvm::ArrayRef<lex::Token> getTokens() const
   {
      return { getTrailingObjects<lex::Token>(), NumTokens };
   }
};

inline Expression* Expression::ignoreParens()
{
   if (auto P = support::dyn_cast<ParenExpr>(this))
      return P->getParenthesizedExpr()->ignoreParens();

   return this;
}

inline Expression* Expression::ignoreParensAndImplicitCasts()
{
   if (auto P = support::dyn_cast<ParenExpr>(this))
      return P->getParenthesizedExpr()->ignoreParensAndImplicitCasts();

   if (auto P = support::dyn_cast<ImplicitCastExpr>(this))
      return P->getTarget()->ignoreParensAndImplicitCasts();

   return this;
}

} // namespace ast
} // namespace cdot

#endif //EXPRESSION_H
