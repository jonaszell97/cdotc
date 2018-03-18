//
// Created by Jonas Zell on 19.06.17.
//

#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "SourceType.h"

#include "Statement.h"
#include "AST/Passes/SemanticAnalysis/ConversionSequence.h"
#include "Basic/Precedence.h"
#include "Variant/Variant.h"

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
class NamedDecl;

using TemplateArgVec = std::vector<Expression*>;

class Expression : public Statement {
public:
   QualType getExprType() const
   {
      return exprType;
   }

   void setExprType(QualType exprType)
   {
      Expression::exprType = exprType;
   }

   bool isContextDependent() const
   {
      return contextDependent;
   }

   static bool classofKind(NodeType kind)
   {
      return kind > _firstExprID && kind < _lastExprID;
   }

   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

   bool isMetaTypeAllowed() const
   {
      return metaTypeAllowed;
   }

   void setMetaTypeAllowed(bool metaTypeAllowed)
   {
      Expression::metaTypeAllowed = metaTypeAllowed;
   }

   bool isLHSOfAssignment() const
   {
      return IsLHSOfAssignment;
   }

   void setIsLHSOfAssignment(bool IsLHSOfAssignment)
   {
      Expression::IsLHSOfAssignment = IsLHSOfAssignment;
   }

   bool isConst() const;
   bool isLValue() const { return exprType && exprType->isReferenceType(); }

   inline Expression *ignoreParens();
   Expression *maybeGetParentExpr() const;

protected:
   explicit Expression(NodeType typeID, bool contextDependent = false)
      : Statement(typeID),
        variadicArgPackExpansion(false),
        metaTypeAllowed(false), contextDependent(contextDependent),
        IsLHSOfAssignment(false)
   {}

   QualType exprType;

   bool variadicArgPackExpansion        : 1;
   bool metaTypeAllowed                 : 1;
   bool contextDependent                : 1;
   bool IsLHSOfAssignment               : 1;

public:
   bool isVariadicArgPackExpansion() const
   {
      return variadicArgPackExpansion;
   }

   void setIsVariadicArgPackExpansion(bool variadicArgPackExpansion)
   {
      this->variadicArgPackExpansion = variadicArgPackExpansion;
   }
};

class IdentifiedExpr: public Expression {
public:
   IdentifiedExpr(NodeType typeID, std::string &&ident,
                  bool contextDependent = false)
      : Expression(typeID, contextDependent),
        ident(move(ident))
   {}

   const std::string &getIdent() const
   {
      return ident;
   }

   std::string &stealIdent()
   {
      return ident;
   }

   void setIdent(std::string &&ident)
   {
      IdentifiedExpr::ident = move(ident);
   }

   static bool classofKind(NodeType kind)
   {
      return kind > _firstIdentifiedExprID && kind < _lastIdentifiedExprID;
   }

   static bool classof(AstNode const* T)
   {
      return classofKind(T->getTypeID());
   }

protected:
   std::string ident;
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

inline Expression* Expression::ignoreParens()
{
   if (auto P = support::dyn_cast<ParenExpr>(this))
      return P->getParenthesizedExpr();

   return this;
}

class TypeExpr: public Expression {
public:
   SourceRange getSourceRange() const { return Loc; }

   static bool classof(AstNode const *T){ return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind)
   {
      return kind > _firstTypeExprID && kind < _lastTypeExprID;
   }

protected:
   TypeExpr(NodeType kind, SourceRange SR)
      : Expression(kind), Loc(SR)
   {

   }

   SourceRange Loc;
};

class TupleTypeExpr final: public TypeExpr,
                           llvm::TrailingObjects<TupleTypeExpr, SourceType> {
   explicit TupleTypeExpr(SourceRange SR,
                          llvm::ArrayRef<SourceType> Tys);

   unsigned NumTys;

public:
   friend TrailingObjects;

   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == TupleTypeExprID; }

   static TupleTypeExpr *Create(ASTContext &C,
                                SourceRange SR,
                                llvm::ArrayRef<SourceType> Tys);

   llvm::ArrayRef<SourceType> getContainedTypes() const
   {
      return { getTrailingObjects<SourceType>(), NumTys };
   }
};

class FunctionTypeExpr final: public TypeExpr,
                              llvm::TrailingObjects<TupleTypeExpr, SourceType> {
   explicit FunctionTypeExpr(SourceRange SR,
                             SourceType RetTy,
                             llvm::ArrayRef<SourceType> Tys);

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
                                   llvm::ArrayRef<SourceType> Tys);

   SourceType getReturnType() const { return RetTy; }

   llvm::ArrayRef<SourceType> getArgTypes() const
   {
      return { getTrailingObjects<SourceType>(), NumArgs };
   }
};

class ArrayTypeExpr: public TypeExpr {
   ArrayTypeExpr(SourceRange SR, SourceType ElementTy,
                 StaticExpr *SizeExpr);

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
                                StaticExpr *SizeExpr);

   SourceType getElementTy() const { return ElementTy; }

   StaticExpr *getSizeExpr() const { return SizeExpr; }
   void setSizeExpr(StaticExpr *E) { SizeExpr = E; }
};

class DeclTypeExpr: public TypeExpr {
   DeclTypeExpr(SourceRange SR, Expression *TyExpr);

   Expression *TyExpr;

public:
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind)
   {
      return kind == DeclTypeExprID;
   }

   static DeclTypeExpr *Create(ASTContext &C,
                               SourceRange SR,
                               Expression *TyExpr);

   Expression *getTyExpr() const { return TyExpr; }
};

class PointerTypeExpr: public TypeExpr {
   PointerTypeExpr(SourceRange SR, SourceType SubType);

   SourceType SubType;

public:
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind)
   {
      return kind == PointerTypeExprID;
   }

   static PointerTypeExpr *Create(ASTContext &C,
                                  SourceRange SR,
                                  SourceType SubType);

   SourceType getSubType() const { return SubType; }
};

class ReferenceTypeExpr: public TypeExpr {
   ReferenceTypeExpr(SourceRange SR, SourceType SubType);

   SourceType SubType;

public:
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind)
   {
      return kind == ReferenceTypeExprID;
   }

   static ReferenceTypeExpr *Create(ASTContext &C,
                                    SourceRange SR,
                                    SourceType SubType);

   SourceType getSubType() const { return SubType; }
};

class OptionTypeExpr: public TypeExpr {
   OptionTypeExpr(SourceRange SR, SourceType SubType);

   SourceType SubType;

public:
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == OptionTypeExprID; }

   static OptionTypeExpr *Create(ASTContext &C,
                                 SourceRange SR,
                                 SourceType SubType);

   SourceType getSubType() const { return SubType; }
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
   SequenceElement(Expression* expr);
   SequenceElement(std::string &&possibleOp,
                   SourceLocation loc);
   SequenceElement(op::OperatorKind opKind,
                   SourceLocation loc);

   ~SequenceElement();

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
   op::OperatorKind getOperatorKind() const { return operatorKind; }

   string &getOp() { return op; }
   string const& getOp() const { return op; }

   Kind getKind() const { return kind; }
   const SourceLocation &getLoc() const { return loc; }

   llvm::StringRef asString() const
   {
      assert(!isExpression() && "cannot stringify expression");
      return isOperator() ? op::toString(operatorKind)
                          : llvm::StringRef(op);
   }

protected:
   union {
      Expression* expr;
      op::OperatorKind operatorKind;
      std::string op;
   };

   Kind kind;
   SourceLocation loc;
};

class ExprSequence final: public Expression,
                          llvm::TrailingObjects<ExprSequence, SequenceElement> {
   ExprSequence(llvm::MutableArrayRef<SequenceElement> fragments);

   unsigned NumFragments;

public:
   static ExprSequence *Create(ASTContext &C,
                               llvm::MutableArrayRef<SequenceElement>fragments);

   SourceRange getSourceRange() const
   {
      auto frags = getFragments();
      return { frags.front().getLoc(), frags.back().getLoc() };
   }

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

   SourceRange getSourceRange() const { return SourceRange(operatorLoc); }

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

   SourceRange getSourceRange() const { return SourceRange(operatorLoc); }

   Expression* getLhs() const { return lhs; }
   Expression* getRhs() const { return rhs; }

   void setRhs(Expression *RHS) { rhs = RHS; }
   void setLhs(Expression *LHS) { lhs = LHS; }

   op::OperatorKind getKind() const { return kind; }
   void setKind(op::OperatorKind K) { kind = K; }

   FunctionType *getFunctionType() const { return FuncTy; }
};

class TypePredicateExpr: public Expression {
   TypePredicateExpr(SourceLocation operatorLoc,
                     Expression *LHS, Expression *RHS,
                     op::OperatorKind kind = op::Colon);

   SourceLocation operatorLoc;
   op::OperatorKind kind;
   Expression *LHS;
   Expression *RHS;

   bool Result = false;

public:
   static bool classofKind(NodeType kind){ return kind == TypePredicateExprID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static TypePredicateExpr *Create(ASTContext &C,
                                    SourceLocation operatorLoc,
                                    Expression *LHS, Expression *RHS,
                                    op::OperatorKind kind = op::Colon);

   SourceRange getSourceRange() const { return SourceRange(operatorLoc); }

   op::OperatorKind getKind() const { return kind; }
   Expression *getLHS() const { return LHS; }
   Expression *getRHS() const { return RHS; }

   void setRHS(Expression *R) { RHS = R; }
   void setLHS(Expression *L) { LHS = L; }

   bool getResult() const { return Result; }
   void setResult(bool Result) { TypePredicateExpr::Result = Result; }
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
   static bool classofKind(NodeType kind){ return kind == CallExprID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static CastExpr *Create(ASTContext &C,
                           SourceLocation AsLoc,
                           CastStrength strength,
                           Expression *target,
                           SourceType targetType);

   SourceRange getSourceRange() const { return SourceRange(AsLoc); }

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
   SourceRange getSourceRange() const { return { ColonLoc }; }

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
};

struct CasePatternArgument {
   explicit CasePatternArgument(Expression* expr,
                                SourceLocation loc = {})
      : expr(expr), sourceLoc(this->expr->getSourceLoc()),
        IsExpr(true), IsConst(false)
   {}

   CasePatternArgument(std::string &&ident, bool isConst, SourceLocation loc)
      : identifier(move(ident)), sourceLoc(loc), IsExpr(false),
        IsConst(isConst)
   {}

   CasePatternArgument(CasePatternArgument &&arg) noexcept;
   CasePatternArgument &operator=(CasePatternArgument &&arg) noexcept;

   ~CasePatternArgument();

   Expression* getExpr() const { return expr; }
   const string &getIdentifier() const { return identifier; }
   bool isExpr() const { return IsExpr; }
   bool isConst() const { return IsConst; }
   const SourceLocation &getSourceLoc() const { return sourceLoc; }

   void setExpr(Expression *E) const
   {
      assert(isExpr());
      expr = E;
   }

private:
   void destroyValue();

   union {
      mutable Expression* expr = nullptr;
      std::string identifier;
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
               string &&caseName,
               llvm::MutableArrayRef<CasePatternArgument> args);

   SourceLocation PeriodLoc;

   std::string caseName;
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
                              string &&caseName,
                              llvm::MutableArrayRef<CasePatternArgument> args);

   llvm::StringRef getCaseName() const
   {
      return caseName;
   }

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

   SourceLocation getPeriodLoc() const { return PeriodLoc; }
   SourceRange getSourceRange() const { return { PeriodLoc, ColonLoc }; }

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
      None, u, i, i1, i8, u8, i16, u16, i32, u32, i64, u64, i128, u128
   };

private:
   IntegerLiteral(SourceLocation Loc,
                  Type *type,
                  llvm::APSInt &&value,
                  Suffix suffix);

   SourceLocation Loc;
   llvm::APSInt value;
   Type* type = nullptr;
   Suffix suffix;

public:
   static bool classofKind(NodeType kind){ return kind == IntegerLiteralID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static IntegerLiteral *Create(ASTContext &C,
                                 SourceLocation Loc,
                                 Type *type,
                                 llvm::APSInt &&value,
                                 Suffix suffix = Suffix::None);

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const { return { Loc }; }

   const llvm::APSInt &getValue() const { return value; }
   void setValue(llvm::APSInt &&Val) { value = std::move(Val); }

   Type *getType() const { return type; }
   void setType(Type *type) { IntegerLiteral::type = type; }

   Suffix getSuffix() const { return suffix; }
};

class FPLiteral: public Expression {
public:
   enum class Suffix: unsigned char {
      None, d, f
   };

private:
   FPLiteral(SourceLocation Loc, Type *type,
             llvm::APFloat &&value,
             Suffix suffix);

   SourceLocation Loc;
   llvm::APFloat value;
   Type *type;
   Suffix suffix;

public:
   static bool classofKind(NodeType kind){ return kind == FPLiteralID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static FPLiteral *Create(ASTContext &C,
                            SourceLocation Loc, Type *type,
                            llvm::APFloat &&value,
                            Suffix suffix = Suffix::None);

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const { return { Loc }; }

   const llvm::APFloat &getValue() const { return value; }

   Type *getType() const { return type; }
   void setType(Type *type) { FPLiteral::type = type; }

   Suffix getSuffix() const { return suffix; }
};

class BoolLiteral: public Expression {
   BoolLiteral(SourceLocation Loc, Type *type, bool value);

   SourceLocation Loc;
   bool value;
   Type *type;

public:
   static bool classofKind(NodeType kind){ return kind == BoolLiteralID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static BoolLiteral *Create(ASTContext &C, SourceLocation Loc,
                              Type *type, bool value);

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const { return { Loc }; }

   bool getValue() const { return value; }
   Type *getType() const { return type; }
   void setType(Type *Ty) { type = Ty; }
};

class CharLiteral: public Expression {
private:
   CharLiteral(SourceLocation Loc, Type *type, char value);
   CharLiteral(SourceLocation Loc, Type *type, uint32_t value);

   union {
      char narrow;
      uint32_t wide;
   };

   SourceLocation Loc;
   bool IsWide;
   Type *type;

public:
   static bool classofKind(NodeType kind){ return kind == CharLiteralID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static CharLiteral *Create(ASTContext &C, SourceLocation Loc,
                              Type *type, char value);
   static CharLiteral *Create(ASTContext &C, SourceLocation Loc,
                              Type *type, uint32_t value);

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const { return { Loc }; }

   char getNarrow() const { return narrow; }

   uint32_t getWide() const { return wide; }
   bool isWide() const { return IsWide; }

   Type *getType() const { return type; }
   void setType(Type *Ty) { type = Ty; }
};

class NoneLiteral: public Expression {
   NoneLiteral(SourceLocation Loc);

   SourceLocation Loc;

public:
   static bool classofKind(NodeType kind){ return kind == NoneLiteralID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static NoneLiteral *Create(ASTContext &C, SourceLocation Loc);

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const
   {
      return SourceRange(Loc, SourceLocation(Loc.getOffset() + 4));
   }
};

class StringLiteral: public Expression {
   StringLiteral( SourceLocation Loc, std::string &&str);

   SourceLocation Loc;
   std::string value;
   bool cstring = false;

public:
   static bool classofKind(NodeType kind){ return kind == StringLiteralID; }
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }

   static StringLiteral *Create(ASTContext &C,  SourceLocation Loc,
                                std::string &&str);

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const { return { Loc }; }

   llvm::StringRef getValue() const { return value; }
   bool isCString() const { return cstring; }
   void setCString(bool raw) { cstring = raw; }
};

class StringInterpolation: public Expression,
                           llvm::TrailingObjects<StringInterpolation,
                                                 Expression*> {
   StringInterpolation(SourceLocation Loc,
                       llvm::ArrayRef<Expression*> strings);

   SourceLocation Loc;
   unsigned NumStrings;

public:
   static bool classof(AstNode const *T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind)
   {
      return kind == StringInterpolationID;
   }

   static StringInterpolation *Create(ASTContext &C,
                                      SourceLocation Loc,
                                      llvm::ArrayRef<Expression*> strings);

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const { return { Loc }; }

   llvm::ArrayRef<Expression*> getStrings() const
   {
      return { getTrailingObjects<Expression*>(), NumStrings };
   }

   llvm::MutableArrayRef<Expression*> getStrings()
   {
      return { getTrailingObjects<Expression*>(), NumStrings };
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
   SourceRange getSourceRange() const
   {
      return SourceRange(Parens.getStart(), body->getSourceRange().getEnd());
   }

   SourceLocation getSourceLoc() const { return Parens.getStart(); }
   SourceLocation getArrowLoc() const { return ArrowLoc; }

   unsigned getNumArgs() const { return NumArgs; }

   SourceType getReturnType() const { return returnType; }
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
};

enum class IdentifierKind {
   Unknown,
   LocalVar,
   CapturedLocalVar,
   GlobalVar,
   FunctionArg,
   CapturedFunctionArg,
   Function,
   TemplateArg,
   TemplateParam,
   AssociatedType,
   Namespace,
   MetaType,
   Alias,
   BuiltinValue,

   Accessor,
   UnionAccess,
   Type,
   StaticField,
   Field,
   EnumRawValue,
   PartiallyAppliedMethod,
};

class IdentifierRefExpr : public IdentifiedExpr {
   SourceLocation Loc;
   IdentifierKind kind = IdentifierKind::Unknown;
   Expression *ParentExpr = nullptr;
   DeclContext *DeclCtx = nullptr;

   union {
      Type *builtinType = nullptr;
      MetaType *metaType;
      CallableDecl *callable;
      LocalVarDecl *localVar;
      GlobalVarDecl *globalVar;
      FuncArgDecl *funcArg;
      NamespaceDecl *namespaceDecl;
      Variant *aliasVal;
      FieldDecl *staticFieldDecl;
      FieldDecl *fieldDecl;
      MethodDecl *accessorMethod;
      MethodDecl *partiallyAppliedMethod;
   };

   BuiltinIdentifier builtinKind;

   bool staticLookup    : 1;
   bool pointerAccess   : 1;
   bool FoundResult     : 1;
   bool InTypePosition  : 1;

   size_t captureIndex = 0;
   std::vector<Expression*> templateArgs;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind){ return kind == IdentifierRefExprID; }

   explicit IdentifierRefExpr(SourceLocation Loc,
                              std::string &&ident,
                              std::vector<Expression*> &&templateArgs = {},
                              DeclContext *DeclCtx = nullptr,
                              bool InTypePos = false);

   IdentifierRefExpr(SourceLocation Loc,
                     IdentifierKind kind,
                     QualType exprType);

   IdentifierRefExpr(IdentifierRefExpr &&expr) = default;

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const
   {
      return SourceRange(Loc, SourceLocation(Loc.getOffset() + ident.size()));
   }

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

   const Variant &getAliasVal() const { return *aliasVal; }
   void setAliasVal(Variant *aliasVal)
   {
      IdentifierRefExpr::aliasVal = aliasVal;
      kind = IdentifierKind ::Alias;
   }

   CallableDecl *getCallable() const { return callable; }
   void setCallable(CallableDecl *callable)
   {
      kind = IdentifierKind::Function;
      IdentifierRefExpr::callable = callable;
   }

   IdentifierKind getKind() const { return kind; }
   void setKind(IdentifierKind IK) { kind = IK; }

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

   MethodDecl *getAccessorMethod() const { return accessorMethod; }
   void setAccessorMethod(MethodDecl *accessorMethod)
   {
      kind = IdentifierKind::Accessor;
      IdentifierRefExpr::accessorMethod = accessorMethod;
   }

   bool isStaticLookup() const { return staticLookup; }
   void setStaticLookup(bool SL) { staticLookup = SL; }

   bool foundResult() const { return FoundResult; }
   void setFoundResult(bool found) { FoundResult = found; }

   bool isPointerAccess() const { return pointerAccess; }
   void setPointerAccess(bool pointer) { pointerAccess = pointer; }

   bool isInTypePosition() const { return InTypePosition; }
   void setInTypePos(bool pos) { InTypePosition = pos; }

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
      return SourceRange(Loc, SourceLocation(Loc.getOffset() + 4));
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
      return SourceRange(Loc, SourceLocation(Loc.getOffset() + 5));
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
   SourceRange getSourceRange() const { return { Loc }; }
   BuiltinIdentifier getIdentifierKind() const { return identifierKind; }
};

class BuiltinExpr: public Expression {
   explicit BuiltinExpr(QualType type);

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == BuiltinExprID; }

   static BuiltinExpr *Create(ASTContext &C, QualType Ty);
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
};

class MemberRefExpr: public IdentifiedExpr {
   SourceLocation Loc;
   MemberKind kind = MemberKind::Unknown;
   Expression *ParentExpr = nullptr;

   std::vector<Expression*> templateArgs;

   RecordDecl *record = nullptr;
   QualType fieldType;

   bool pointerAccess : 1;
   bool tupleAccess : 1;

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
                 string &&ident,
                 bool pointerAccess = false);

   MemberRefExpr(SourceLocation Loc,
                 Expression *ParentExpr,
                 string &&ident,
                 bool pointerAccess = false);

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const
   {
      return SourceRange(Loc, SourceLocation(Loc.getOffset() + ident.size()));
   }

   Expression *getParentExpr() const { return ParentExpr; }
   void setParentExpr(Expression *Expr) { ParentExpr = Expr; }

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
   SourceRange getSourceRange() const { return { Loc }; }

   Expression *getParentExpr() const { return ParentExpr; }
   void setParentExpr(Expression *Expr) { ParentExpr = Expr; }

   unsigned getIndex() const { return Index; }
   bool isPointerAccess() const { return PointerAccess; }
};

class EnumCaseExpr: public IdentifiedExpr {
   SourceLocation PeriodLoc;
   std::vector<Expression*> args;
   EnumDecl *en;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == EnumCaseExprID; }

   EnumCaseExpr(SourceLocation PeriodLoc,
                std::string &&caseName,
                std::vector<Expression*> &&args = {});

   SourceLocation getSourceLoc() const { return PeriodLoc; }
   SourceRange getSourceRange() const
   {
      return SourceRange(PeriodLoc,
                         SourceLocation(PeriodLoc.getOffset() + ident.size()));
   }

   std::vector<Expression*> &getArgs() { return args; }
   llvm::ArrayRef<Expression*> getArgs() const { return args; }

   EnumDecl *getEnum() const { return en; }
   void setEnum(EnumDecl *E) { en = E; }
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
   AnonymousCall,
   UnsafeTupleGet,
   VariadicSizeof,
   PrimitiveInitializer,
   GlobalVariableCall,
   LocalVariableCall,
};

class CallExpr: public IdentifiedExpr {
   SourceLocation IdentLoc;
   SourceRange ParenRange;

   CallKind kind = CallKind::Unknown;
   Expression *ParentExpr = nullptr;

   std::vector<Expression*> args;

   bool PointerAccess : 1;
   bool IsUFCS : 1;

   BuiltinFn builtinFnKind = BuiltinFn(0);

   union {
      CallableDecl *func = nullptr;
      GlobalVarDecl *globalVar;
      LocalVarDecl *localVar;
   };

   IdentifierRefExpr *identExpr = nullptr;

   std::vector<Expression*> templateArgs;

   union {
      Type *builtinArgType = nullptr;
      FunctionType* functionType;
      UnionDecl *U;
   };

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == CallExprID; }

   CallExpr(SourceLocation IdentLoc, SourceRange ParenRange,
            std::vector<Expression* > &&args,
            string &&name = "");

   CallExpr(SourceLocation IdentLoc, SourceRange ParenRange,
            Expression *ParentExpr,
            std::vector<Expression* > &&args,
            string &&name = "");

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

   Expression *getParentExpr() const { return ParentExpr; }
   void setParentExpr(Expression *E) { ParentExpr = E; }

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

   FunctionType *getFunctionType() const { return functionType; }
   void setFunctionType(FunctionType *FT) { functionType = FT; }

   Type *getBuiltinArgType() const { return builtinArgType; }
   void setBuiltinArgType(Type *Ty) { builtinArgType = Ty; }

   IdentifierRefExpr *getIdentExpr() const { return identExpr; }
   void setIdentExpr(IdentifierRefExpr *ID) { identExpr = ID; }

   UnionDecl *getUnion() const { return U; }
   void setUnion(UnionDecl *U) { CallExpr::U = U; }

   GlobalVarDecl *getGlobalVar() const { return globalVar; }
   void setGlobalVar(GlobalVarDecl *globalVar)
   {
      kind = CallKind::GlobalVariableCall;
      CallExpr::globalVar = globalVar;
   }

   LocalVarDecl *getLocalVar() const { return localVar; }
   void setLocalVar(LocalVarDecl *localVar)
   {
      kind = CallKind::LocalVariableCall;
      CallExpr::localVar = localVar;
   }

   CallKind getKind() const { return kind; }
   void setKind(CallKind CK) { kind = CK; }

   bool isPointerAccess() const { return PointerAccess; }
   void setIsPointerAccess(bool access) { PointerAccess = access; }

   bool isUFCS() const { return IsUFCS; }
   void setIsUFCS(bool IsUFCS) { CallExpr::IsUFCS = IsUFCS; }

   QualType getReturnType() { return exprType; }
   void setReturnType(QualType ty) { exprType = ty; }

   BuiltinFn getBuiltinFnKind() const { return builtinFnKind; }
   void setBuiltinFnKind(BuiltinFn kind) { builtinFnKind = kind; }

   bool isKnownFunctionCall();
};

class SubscriptExpr: public Expression,
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

   static SubscriptExpr *Create(ASTContext &C, SourceRange SquareRange,
                                Expression *ParentExpr,
                                llvm::ArrayRef<Expression*> indices);

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
   static StaticExpr *Create(ASTContext &C, Variant &&V);

   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == StaticExprID; }

   SourceRange getSourceRange() const;

private:
   explicit StaticExpr(Expression* expr);
   explicit StaticExpr(Variant &&V);

   Expression* expr;
   Variant evaluatedExpr;

public:
   Expression* getExpr() const { return expr; }
   void setExpr(Expression *E) { expr = E; }

   const Variant &getEvaluatedExpr() const { return evaluatedExpr; }
   void setEvaluatedExpr(Variant &&V) { evaluatedExpr = std::move(V); }
};

class ConstraintExpr: public Expression {
public:
   enum Kind {
      Struct, Class,
      Enum, Union, Function,
      DefaultConstructible, Pointer, Reference, Type
   };

   Kind getKind() const { return kind; }
   SourceType getTypeConstraint() const { return typeConstraint; }

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const { return { Loc }; }

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

   SourceType getType() const
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

class TraitsExpr: public Expression,
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


} // namespace ast
} // namespace cdot

#endif //EXPRESSION_H
