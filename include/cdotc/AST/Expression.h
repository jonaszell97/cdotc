#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "cdotc/AST/Decl.h"
#include "cdotc/AST/SourceType.h"
#include "cdotc/AST/Statement.h"
#include "cdotc/Basic/Precedence.h"
#include "cdotc/Basic/Variant.h"
#include "cdotc/Lex/Token.h"
#include "cdotc/Sema/ConversionSequence.h"

#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APSInt.h>
#include <llvm/Support/TrailingObjects.h>

namespace cdot {

class NestedNameSpecifier;
class NestedNameSpecifierWithLoc;

class FunctionType;
enum class KnownFunction : unsigned char;

enum class BuiltinIdentifier : unsigned char;

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
   void setExprType(QualType ty);

   bool isUnknownAny() const
   {
      return exprType && exprType->isUnknownAnyType();
   }

   bool isContextDependent() const;

   bool isLHSOfAssignment() const { return IsLHSOfAssignment; }
   void setIsLHSOfAssignment(bool IsLHS) { IsLHSOfAssignment = IsLHS; }

   bool isConst() const;
   bool isLValue() const { return exprType && exprType->isReferenceType(); }

   bool isVariadicArgPackExpansion() const { return EllipsisLoc.isValid(); }

   bool isMagicArgumentValue() const { return IsMagicArgumentValue; }
   void setIsMagicArgumentValue(bool magic) { IsMagicArgumentValue = magic; }

   bool allowTemplate() const { return AllowTemplate; }
   void setAllowTemplate(bool V) { AllowTemplate = V; }

   bool isInTypePosition() const { return flagIsSet(InTypePosition); }
   void setIsInTypePosition(bool b) { setFlag(InTypePosition, b); }

   bool allowNamespaceRef() const;
   void setAllowNamespaceRef(bool V);

   SourceRange getEllipsisRange() const;
   SourceLocation getEllipsisLoc() const { return EllipsisLoc; }
   void setEllipsisLoc(SourceLocation Loc) { EllipsisLoc = Loc; }

   inline Expression* ignoreParens();
   inline Expression* ignoreParensAndImplicitCasts();

   inline Expression* ignoreParens() const;
   inline Expression* ignoreParensAndImplicitCasts() const;

   Expression* ignoreTemplateArgs() const;

   Expression* getParentExpr() const;
   void setParentExpr(Expression* E);

protected:
   explicit Expression(NodeType typeID);

   QualType exprType;

   SourceLocation EllipsisLoc;
   bool IsLHSOfAssignment : 1;
   bool IsMagicArgumentValue : 1;
   bool AllowTemplate : 1;
};

class IdentifiedExpr : public Expression {
public:
   IdentifiedExpr(NodeType typeID, DeclarationName Name);

   DeclarationName getDeclName() const { return DeclName; }
   IdentifierInfo* getIdentInfo() const { return DeclName.getIdentifierInfo(); }
   llvm::StringRef getIdent() const { return getIdentInfo()->getIdentifier(); }

   void setIdent(DeclarationName Name) { DeclName = Name; }

   static bool classofKind(NodeType kind)
   {
      return kind > _firstIdentifiedExprID && kind < _lastIdentifiedExprID;
   }

   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

protected:
   DeclarationName DeclName;
};

class ParenExpr : public Expression {
public:
   static ParenExpr* Create(ASTContext& C, SourceRange Parens,
                            Expression* Expr);

   ParenExpr(EmptyShell Empty);

   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == ParenExprID; }

private:
   ParenExpr(SourceRange Parens, Expression* Expr);

   SourceRange Parens;
   Expression* ParenthesizedExpr;

public:
   SourceRange getSourceRange() const { return Parens; }
   Expression* getParenthesizedExpr() const { return ParenthesizedExpr; }
   void setParenthesizedExpr(Expression* E) { ParenthesizedExpr = E; }

   void setParens(const SourceRange& Parens) { ParenExpr::Parens = Parens; }
};

class TypeExpr : public Expression {
public:
   SourceRange getSourceRange() const { return Loc; }

   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind)
   {
      return kind > _firstTypeExprID && kind < _lastTypeExprID;
   }

   void setSourceRange(SourceRange SR) { Loc = SR; }

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

class TupleTypeExpr final : public TypeExpr,
                            llvm::TrailingObjects<TupleTypeExpr, SourceType> {
   explicit TupleTypeExpr(SourceRange SR, llvm::ArrayRef<SourceType> Tys,
                          bool IsMeta);

   TupleTypeExpr(EmptyShell Empty, unsigned N);

   unsigned NumTys;

public:
   friend TrailingObjects;

   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == TupleTypeExprID; }

   static TupleTypeExpr* Create(ASTContext& C, SourceRange SR,
                                llvm::ArrayRef<SourceType> Tys,
                                bool IsMeta = true);

   static TupleTypeExpr* CreateEmpty(ASTContext& C, unsigned N);

   llvm::ArrayRef<SourceType> getContainedTypes() const
   {
      return {getTrailingObjects<SourceType>(), NumTys};
   }

   using TrailingObjects::getTrailingObjects;
};

class FunctionTypeExpr final
    : public TypeExpr,
      llvm::TrailingObjects<FunctionTypeExpr, SourceType,
                            FunctionType::ParamInfo> {
   explicit FunctionTypeExpr(SourceRange SR, SourceType RetTy,
                             llvm::ArrayRef<SourceType> Tys,
                             llvm::ArrayRef<FunctionType::ParamInfo> Info,
                             bool IsMeta, bool IsThin, bool Throws,
                             bool IsAsync, bool IsUnsafe);

   FunctionTypeExpr(EmptyShell Empty, unsigned N);

   SourceType RetTy;
   unsigned NumArgs : 28;
   bool Thin : 1;
   bool Throws : 1;
   bool Async : 1;
   bool Unsafe : 1;

   size_t numTrailingObjects(OverloadToken<SourceType>) const
   {
      return NumArgs;
   }

public:
   friend TrailingObjects;

   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == FunctionTypeExprID; }

   static FunctionTypeExpr* Create(ASTContext& C, SourceRange SR,
                                   SourceType RetTy,
                                   llvm::ArrayRef<SourceType> Tys,
                                   llvm::ArrayRef<FunctionType::ParamInfo> Info,
                                   bool IsMeta = true, bool IsThin = false,
                                   bool Throws = false, bool IsAsync = false,
                                   bool IsUnsafe = false);

   static FunctionTypeExpr* CreateEmpty(ASTContext& C, unsigned N);

   const SourceType& getReturnType() const { return RetTy; }

   llvm::ArrayRef<SourceType> getArgTypes() const
   {
      return {getTrailingObjects<SourceType>(), NumArgs};
   }

   llvm::ArrayRef<FunctionType::ParamInfo> getParamInfo() const
   {
      return {getTrailingObjects<FunctionType::ParamInfo>(), NumArgs};
   }

   bool isThin() const { return Thin; }
   bool throws() const { return Throws; }
   bool isAsync() const { return Async; }
   bool isUnsafe() const { return Unsafe; }

   void setRetTy(SourceType RetTy) { FunctionTypeExpr::RetTy = RetTy; }
   void setThin(bool T) { Thin = T; }
   void setThrows(bool Throws) { FunctionTypeExpr::Throws = Throws; }
   void setAsync(bool Async) { FunctionTypeExpr::Async = Async; }
   void setUnsafe(bool Unsafe) { FunctionTypeExpr::Unsafe = Unsafe; }

   using TrailingObjects::getTrailingObjects;
};

class ArrayTypeExpr : public TypeExpr {
   ArrayTypeExpr(SourceRange SR, SourceType ElementTy, StaticExpr* SizeExpr,
                 bool IsMeta);

   SourceType ElementTy;
   StaticExpr* SizeExpr;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == ArrayTypeExprID; }

   static ArrayTypeExpr* Create(ASTContext& C, SourceRange SR,
                                SourceType ElementTy, StaticExpr* SizeExpr,
                                bool IsMeta = true);

   ArrayTypeExpr(EmptyShell Empty);

   const SourceType& getElementTy() const { return ElementTy; }
   void setElementTy(SourceType Ty) { ElementTy = Ty; }

   StaticExpr* getSizeExpr() const { return SizeExpr; }
   void setSizeExpr(StaticExpr* E) { SizeExpr = E; }
};

class DeclTypeExpr : public TypeExpr {
   DeclTypeExpr(SourceRange SR, Expression* TyExpr, bool IsMeta);

   Expression* TyExpr;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == DeclTypeExprID; }

   static DeclTypeExpr* Create(ASTContext& C, SourceRange SR,
                               Expression* TyExpr, bool IsMeta = true);

   DeclTypeExpr(EmptyShell Empty);

   Expression* getTyExpr() const { return TyExpr; }
   void setTyExpr(Expression* E) { TyExpr = E; }
};

class PointerTypeExpr : public TypeExpr {
   PointerTypeExpr(SourceRange SR, SourceType SubType, bool IsMeta);

   SourceType SubType;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == PointerTypeExprID; }

   static PointerTypeExpr* Create(ASTContext& C, SourceRange SR,
                                  SourceType SubType, bool IsMeta = true);

   PointerTypeExpr(EmptyShell Empty);

   const SourceType& getSubType() const { return SubType; }
   void setSubType(SourceType Ty) { SubType = Ty; }
};

class ReferenceTypeExpr : public TypeExpr {
   ReferenceTypeExpr(SourceRange SR, SourceType SubType, bool IsMeta);

   SourceType SubType;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind)
   {
      return kind == ReferenceTypeExprID;
   }

   static ReferenceTypeExpr* Create(ASTContext& C, SourceRange SR,
                                    SourceType SubType, bool IsMeta = true);

   ReferenceTypeExpr(EmptyShell Empty);

   const SourceType& getSubType() const { return SubType; }
   void setSubType(SourceType Ty) { SubType = Ty; }
};

class OptionTypeExpr : public TypeExpr {
   OptionTypeExpr(SourceRange SR, SourceType SubType, bool IsMeta);

   SourceType SubType;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == OptionTypeExprID; }

   static OptionTypeExpr* Create(ASTContext& C, SourceRange SR,
                                 SourceType SubType, bool IsMeta = true);

   OptionTypeExpr(EmptyShell Empty);

   const SourceType& getSubType() const { return SubType; }
   void setSubType(SourceType Ty) { SubType = Ty; }
};

class ExistentialTypeExpr final
    : public TypeExpr,
      TrailingObjects<ExistentialTypeExpr, SourceType> {
   ExistentialTypeExpr(SourceRange SR, ArrayRef<SourceType> Existentials,
                       bool IsMeta);

   ExistentialTypeExpr(unsigned N);

   unsigned NumExistentials;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType K) { return K == ExistentialTypeExprID; }

   static ExistentialTypeExpr* Create(ASTContext& C, SourceRange SR,
                                      ArrayRef<SourceType> Existentials,
                                      bool IsMeta = true);

   static ExistentialTypeExpr* CreateEmpty(ASTContext& C, unsigned N);

   ArrayRef<SourceType> getExistentials() const;
   MutableArrayRef<SourceType> getExistentials();

   friend TrailingObjects;
};

class AttributedExpr final : public Expression,
                             llvm::TrailingObjects<AttributedExpr, Attr*> {
public:
   static bool classofKind(NodeType kind) { return kind == AttributedExprID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static AttributedExpr* Create(ASTContext& Ctx, Expression* Expr,
                                 llvm::ArrayRef<Attr*> Attrs);

   static AttributedExpr* CreateEmpty(ASTContext& C, unsigned N);

   llvm::ArrayRef<Attr*> getAttributes() const
   {
      return {getTrailingObjects<Attr*>(), NumAttrs};
   }

   SourceRange getSourceRange() const { return Expr->getSourceRange(); }

   Expression* getExpr() const { return Expr; }
   void setExpr(Expression* E) { Expr = E; }

   friend TrailingObjects;

private:
   AttributedExpr(Expression* Expr, llvm::ArrayRef<Attr*> Attrs);
   AttributedExpr(EmptyShell Empty, unsigned N);

   Expression* Expr;
   unsigned NumAttrs;
};

class ImplicitCastExpr : public Expression {
public:
   static ImplicitCastExpr* Create(ASTContext& C, Expression* target,
                                   const ConversionSequence* ConvSeq);

   ImplicitCastExpr(EmptyShell Empty);

   static bool classofKind(NodeType kind) { return kind == ImplicitCastExprID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

private:
   ImplicitCastExpr(Expression* target, const ConversionSequence* ConvSeq);

   Expression* target;
   const ConversionSequence* ConvSeq = nullptr;

public:
   SourceRange getSourceRange() const { return target->getSourceRange(); }
   Expression* getTarget() const { return target; }
   const ConversionSequence& getConvSeq() const { return *ConvSeq; }

   void setTarget(Expression* target) { ImplicitCastExpr::target = target; }
   void setConvSeq(const ConversionSequence* Seq) { ConvSeq = Seq; }
};

class SequenceElement {
public:
   explicit SequenceElement(Expression* expr);

   SequenceElement(IdentifierInfo* possibleOp, uint8_t whitespace,
                   SourceLocation loc);

   SequenceElement(op::OperatorKind opKind, uint8_t whitespace,
                   SourceLocation loc);

   SequenceElement(const SequenceElement&) = delete;
   SequenceElement& operator=(const SequenceElement&) = delete;

   SequenceElement(SequenceElement&&) noexcept;
   SequenceElement& operator=(SequenceElement&&) noexcept;

   enum Kind : unsigned char {
      EF_Expression,
      EF_PossibleOperator,
      EF_Operator
   };

   bool isExpression() const { return kind == EF_Expression; }

   bool isPossibleOperator() const { return kind == EF_PossibleOperator; }

   bool isOperator() const { return kind == EF_Operator; }

   Expression* getExpr() const { return expr; }
   void setExpr(Expression* E) const { expr = E; }

   op::OperatorKind getOperatorKind() const { return operatorKind; }
   IdentifierInfo* getOp() const { return op; }

   Kind getKind() const { return kind; }

   SourceLocation getLoc() const { return loc; }
   SourceLocation getEndLoc() const;
   SourceRange getSourceRange() const { return SourceRange(loc, getEndLoc()); }

   llvm::StringRef asString() const
   {
      assert(!isExpression() && "cannot stringify expression");
      return isOperator() ? op::toString(operatorKind) : op->getIdentifier();
   }

   enum WhitespacePos : uint8_t {
      None = 0,
      Left = 1,
      Right = 2,
   };

   bool hasLeftWhiteSpace() const { return (whitespace & Left) != 0; }
   bool hasRightWhiteSpace() const { return (whitespace & Right) != 0; }
   uint8_t getWhitespace() const { return whitespace; }

protected:
   union {
      mutable Expression* expr;
      op::OperatorKind operatorKind;
      IdentifierInfo* op;
   };

   Kind kind;
   uint8_t whitespace;
   SourceLocation loc;
};

class ExprSequence final
    : public Expression,
      llvm::TrailingObjects<ExprSequence, SequenceElement> {
   explicit ExprSequence(llvm::MutableArrayRef<SequenceElement> fragments);
   ExprSequence(EmptyShell Empty, unsigned N);

   unsigned NumFragments;

public:
   static ExprSequence*
   Create(ASTContext& C, llvm::MutableArrayRef<SequenceElement> fragments);

   static ExprSequence* CreateEmpty(ASTContext& C, unsigned N);

   SourceRange getSourceRange() const;

   unsigned getNumFragments() const { return NumFragments; }

   llvm::ArrayRef<SequenceElement> getFragments() const
   {
      return {getTrailingObjects<SequenceElement>(), NumFragments};
   }

   using TrailingObjects::getTrailingObjects;

   static bool classofKind(NodeType kind) { return kind == ExprSequenceID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   friend TrailingObjects;
};

class UnaryOperator : public Expression {
   UnaryOperator(SourceLocation operatorLoc, op::OperatorKind opKind,
                 FunctionType* FuncTy, Expression* target, bool prefix);

   SourceLocation operatorLoc;
   op::OperatorKind kind;
   FunctionType* FuncTy;
   Expression* target;
   bool prefix;

public:
   static bool classofKind(NodeType kind) { return kind == UnaryOperatorID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static UnaryOperator* Create(ASTContext& C, SourceLocation operatorLoc,
                                op::OperatorKind opKind, FunctionType* FuncTy,
                                Expression* target, bool prefix);

   UnaryOperator(EmptyShell Empty);

   SourceLocation getOperatorLoc() const { return operatorLoc; }
   void setOperatorLoc(SourceLocation Loc) { operatorLoc = Loc; }

   SourceRange getSourceRange() const;

   op::OperatorKind getKind() const { return kind; }
   bool isPrefix() const { return prefix; }
   FunctionType* getFunctionType() const { return FuncTy; }

   void setKind(op::OperatorKind kind) { UnaryOperator::kind = kind; }
   void setFuncTy(FunctionType* FuncTy) { UnaryOperator::FuncTy = FuncTy; }
   void setPrefix(bool prefix) { UnaryOperator::prefix = prefix; }

   Expression* getTarget() const { return target; }
   void setTarget(Expression* T) { target = T; }
};

class BinaryOperator : public Expression {
   BinaryOperator(SourceLocation operatorLoc, op::OperatorKind operatorKind,
                  FunctionType* FuncTy, Expression* lhs, Expression* rhs);

   SourceLocation operatorLoc;

   op::OperatorKind kind;
   FunctionType* FuncTy;

   Expression* lhs;
   Expression* rhs;

public:
   static bool classofKind(NodeType kind) { return kind == BinaryOperatorID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static BinaryOperator* Create(ASTContext& C, SourceLocation operatorLoc,
                                 op::OperatorKind operatorKind,
                                 FunctionType* FuncTy, Expression* lhs,
                                 Expression* rhs);

   BinaryOperator(EmptyShell Empty);

   SourceRange getSourceRange() const;
   SourceLocation getOperatorLoc() const { return operatorLoc; }
   void setOperatorLoc(SourceLocation Loc) { operatorLoc = Loc; }

   Expression* getLhs() const { return lhs; }
   Expression* getRhs() const { return rhs; }

   void setRhs(Expression* RHS) { rhs = RHS; }
   void setLhs(Expression* LHS) { lhs = LHS; }

   op::OperatorKind getKind() const { return kind; }
   void setKind(op::OperatorKind K) { kind = K; }

   FunctionType* getFunctionType() const { return FuncTy; }
   void setFunctionType(FunctionType* Ty) { FuncTy = Ty; }
};

class AssignExpr : public Expression {
   AssignExpr(SourceLocation EqualsLoc, Expression* LHS, Expression* RHS,
              bool IsInitialization);

   SourceLocation EqualsLoc;
   Expression* LHS;
   Expression* RHS;

   bool IsInitialization : 1;
   bool IsMove : 1;

public:
   static AssignExpr* Create(ASTContext& C, SourceLocation EqualsLoc,
                             Expression* LHS, Expression* RHS,
                             bool IsInitialization = false);

   AssignExpr(EmptyShell Empty);

   static bool classofKind(NodeType kind) { return kind == AssignExprID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   SourceRange getSourceRange() const;
   SourceLocation getEqualsLoc() const { return EqualsLoc; }
   void setEqualsLoc(SourceLocation Loc) { EqualsLoc = Loc; }

   Expression* getLhs() const { return LHS; }
   Expression* getRhs() const { return RHS; }

   void setRhs(Expression* rhs) { RHS = rhs; }
   void setLhs(Expression* lhs) { LHS = lhs; }

   bool isInitialization() const { return IsInitialization; }
   void setIsInitialization(bool I) { IsInitialization = I; }

   bool isMove() const { return IsMove; }
   void setIsMove(bool IsMove) { AssignExpr::IsMove = IsMove; }
};

class TypePredicateExpr : public Expression {
   TypePredicateExpr(SourceLocation IsLoc, SourceRange SR, Expression* LHS,
                     ConstraintExpr* RHS, bool Negated);

   SourceLocation IsLoc;
   SourceRange SR;
   Expression* LHS;
   ConstraintExpr* RHS;

   bool Result : 1;
   bool CompileTimeCheck : 1;
   bool Negated : 1;

public:
   static bool classofKind(NodeType kind)
   {
      return kind == TypePredicateExprID;
   }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static TypePredicateExpr* Create(ASTContext& C, SourceLocation IsLoc,
                                    SourceRange SR, Expression* LHS,
                                    ConstraintExpr* RHS, bool Negated);

   TypePredicateExpr(EmptyShell Empty);

   SourceLocation getIsLoc() const { return IsLoc; }
   void setIsLoc(SourceLocation Loc) { IsLoc = Loc; }

   SourceRange getSourceRange() const { return SR; }
   void setSourceRange(SourceRange S) { SR = S; }

   Expression* getLHS() const { return LHS; }
   ConstraintExpr* getRHS() const { return RHS; }

   void setLHS(Expression* L) { LHS = L; }
   void setRHS(ConstraintExpr* R) { RHS = R; }

   bool getResult() const { return Result; }
   void setResult(bool Result) { TypePredicateExpr::Result = Result; }

   bool isCompileTimeCheck() const { return CompileTimeCheck; }
   void setIsCompileTimeCheck(bool Comp) { CompileTimeCheck = Comp; }

   bool isNegated() const { return Negated; }
   void setNegated(bool V) { Negated = V; }
};

class CastExpr : public Expression {
   CastExpr(SourceLocation AsLoc, CastStrength strength, Expression* target,
            SourceType targetType);

   SourceLocation AsLoc;
   CastStrength strength;
   Expression* target;
   SourceType targetType;
   ConversionSequence* ConvSeq = nullptr;

public:
   static bool classofKind(NodeType kind) { return kind == CastExprID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static CastExpr* Create(ASTContext& C, SourceLocation AsLoc,
                           CastStrength strength, Expression* target,
                           SourceType targetType);

   CastExpr(EmptyShell Empty);

   SourceLocation getAsLoc() const { return AsLoc; }
   SourceRange getSourceRange() const;
   void setAsLoc(SourceLocation Loc) { AsLoc = Loc; }

   CastStrength getStrength() const { return strength; }
   Expression* getTarget() const { return target; }
   const SourceType& getTargetType() { return targetType; }
   const ConversionSequence& getConvSeq() const { return *ConvSeq; }

   void setStrength(CastStrength strength) { CastExpr::strength = strength; }
   void setTargetType(SourceType Ty) { targetType = Ty; }
   void setTarget(Expression* T) { target = T; }
   void setConvSeq(ConversionSequence* Seq) { ConvSeq = Seq; }
};

class AddrOfExpr : public Expression {
   AddrOfExpr(SourceLocation AmpLoc, Expression* Target);

   SourceLocation AmpLoc;
   Expression* Target;

public:
   static bool classofKind(NodeType kind) { return kind == AddrOfExprID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static AddrOfExpr* Create(ASTContext& C, SourceLocation AmpLoc,
                             Expression* Target);

   AddrOfExpr(EmptyShell Empty);

   SourceLocation getAmpLoc() const { return AmpLoc; }
   void setAmpLoc(SourceLocation Loc) { AmpLoc = Loc; }

   SourceRange getSourceRange() const;

   Expression* getTarget() const { return Target; }
   void setTarget(Expression* T) { Target = T; }
};

class IfExpr : public Expression {
   IfExpr(SourceLocation IfLoc, Expression* Cond, Expression* TrueVal,
          Expression* FalseVal);

   SourceLocation IfLoc;

   IfCondition Cond;
   Expression* TrueVal;
   Expression* FalseVal;

public:
   static bool classofKind(NodeType kind) { return kind == IfExprID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static IfExpr* Create(ASTContext& C, SourceLocation IfLoc, Expression* Cond,
                         Expression* TrueVal, Expression* FalseVal);

   IfExpr(EmptyShell Empty);

   SourceLocation getIfLoc() const { return IfLoc; }
   void setIfLoc(SourceLocation Loc) { IfLoc = Loc; }

   SourceRange getSourceRange() const
   {
      return SourceRange(IfLoc, FalseVal->getSourceRange().getEnd());
   }

   IfCondition& getCond() { return Cond; }
   Expression* getTrueVal() const { return TrueVal; }
   Expression* getFalseVal() const { return FalseVal; }

   void setCond(IfCondition C) { Cond = C; }
   void setTrueVal(Expression* Val) { TrueVal = Val; }
   void setFalseVal(Expression* Val) { FalseVal = Val; }
};

class TryExpr : public Expression {
public:
   enum TryKind : unsigned char {
      Normal,
      Fallible,
      Force,
   };

private:
   TryExpr(SourceLocation TryLoc, TryKind Kind, Expression* E);

   SourceLocation TryLoc;
   TryKind Kind;
   Expression* E;

public:
   static bool classofKind(NodeType kind) { return kind == TryExprID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static TryExpr* Create(ASTContext& C, SourceLocation TryLoc, TryKind Kind,
                          Expression* E);

   TryExpr(EmptyShell Empty);

   SourceRange getSourceRange() const;
   void setTryLoc(SourceLocation Loc) { TryLoc = Loc; }

   Expression* getExpr() const { return E; }
   void setExpr(Expression* E) { this->E = E; }

   TryKind getKind() const { return Kind; }
   void setKind(TryKind K) { Kind = K; }
};

class AwaitExpr : public Expression {
   AwaitExpr(SourceLocation AwaitLoc, Expression* E);

   SourceLocation AwaitLoc;
   Expression* E;

   bool ImmediateReturn = false;

public:
   static bool classofKind(NodeType kind) { return kind == AwaitExprID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static AwaitExpr* Create(ASTContext& C, SourceLocation AwaitLoc,
                            Expression* E);

   AwaitExpr(EmptyShell Empty);

   SourceRange getSourceRange() const;

   SourceLocation getAwaitLoc() const { return AwaitLoc; }
   void setAwaitLoc(SourceLocation V) { AwaitLoc = V; }

   Expression* getExpr() const { return E; }
   void setExpr(Expression* V) { E = V; }

   bool isImmediateReturn() const { return ImmediateReturn; }
   void setImmediateReturn(bool V) { ImmediateReturn = V; }
};

class PatternExpr : public Expression {
public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
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
   void setColonLoc(SourceLocation Loc) { ColonLoc = Loc; }

protected:
   explicit PatternExpr(NodeType type, SourceLocation ColonLoc)
       : Expression(type), ColonLoc(ColonLoc)
   {
   }

   SourceLocation ColonLoc;
};

class ExpressionPattern : public PatternExpr {
   explicit ExpressionPattern(SourceLocation ColonLoc, Expression* expr);

   Expression* expr;
   CallableDecl* comparisonOp = nullptr;

public:
   static bool classofKind(NodeType kind)
   {
      return kind == ExpressionPatternID;
   }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static ExpressionPattern* Create(ASTContext& C, SourceLocation ColonLoc,
                                    Expression* E);

   ExpressionPattern(EmptyShell Empty);

   Expression* getExpr() const { return expr; }
   void setExpr(Expression* E) { expr = E; }

   SourceRange getSourceRange() const { return expr->getSourceRange(); }

   CallableDecl* getComparisonOp() const { return comparisonOp; }
   void setComparisonOp(CallableDecl* op) { comparisonOp = op; }
};

class CasePattern final : public PatternExpr,
                          TrailingObjects<CasePattern, IfCondition> {
public:
   enum Kind {
      K_EnumOrStruct,
      K_Tuple,
      K_Array,
   };

private:
   CasePattern(SourceRange SR, enum Kind Kind, Expression* ParentExpr,
               IdentifierInfo* caseName, MutableArrayRef<IfCondition> args);

   CasePattern(EmptyShell Empty, unsigned N);

   SourceRange SR;

   Kind K;
   Expression* ParentExpr;
   IdentifierInfo* caseName;
   unsigned NumArgs;

   bool HasBinding : 1;
   bool HasExpr : 1;
   bool LeadingDot : 1;

   EnumCaseDecl* CaseDecl = nullptr;

public:
   static bool classofKind(NodeType kind) { return kind == CasePatternID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static CasePattern* Create(ASTContext& C, SourceRange SR, enum Kind Kind,
                              Expression* ParentExpr, IdentifierInfo* caseName,
                              MutableArrayRef<IfCondition> args);

   static CasePattern* CreateEmpty(ASTContext& C, unsigned N);

   SourceRange getSourceRange() const { return SR; }

   enum Kind getKind() const { return K; }
   void setKind(enum Kind V) { K = V; }

   void setSourceRange(const SourceRange& SR) { CasePattern::SR = SR; }
   void setCaseName(IdentifierInfo* Name) { CasePattern::caseName = Name; }

   Expression* getParentExpr() const { return ParentExpr; }
   void setParentExpr(Expression* V) { ParentExpr = V; }

   llvm::StringRef getCaseName() const { return caseName->getIdentifier(); }
   IdentifierInfo* getCaseNameIdent() const { return caseName; }

   llvm::MutableArrayRef<IfCondition> getArgs()
   {
      return {getTrailingObjects<IfCondition>(), NumArgs};
   }

   EnumCaseDecl* getCaseDecl() const { return CaseDecl; }
   void setCaseDecl(EnumCaseDecl* CD) { CaseDecl = CD; }

   bool hasBinding() const { return HasBinding; }
   bool hasExpr() const { return HasExpr; }

   void setHasBinding(bool HasBinding) { CasePattern::HasBinding = HasBinding; }
   void setHasExpr(bool HasExpr) { CasePattern::HasExpr = HasExpr; }

   bool hasLeadingDot() const { return LeadingDot; }
   void setLeadingDot(bool V) { LeadingDot = V; }

   SourceLocation getPeriodLoc() const { return SR.getStart(); }

   using TrailingObjects::getTrailingObjects;
   friend TrailingObjects;
};

class IsPattern : public PatternExpr {
   IsPattern(SourceRange SR, SourceType isType);

   SourceLocation IsLoc;
   SourceType isType;

public:
   static bool classofKind(NodeType kind) { return kind == IsPatternID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static IsPattern* Create(ASTContext& C, SourceRange SR, SourceType isType);
   IsPattern(EmptyShell Empty);

   SourceLocation getIsLoc() const { return IsLoc; }
   SourceRange getSourceRange() const { return {IsLoc, ColonLoc}; }

   SourceType getIsType() const { return isType; }

   void setIsLoc(const SourceLocation& IsLoc) { IsPattern::IsLoc = IsLoc; }
   void setIsType(const SourceType& isType) { IsPattern::isType = isType; }
};

class InitDecl;

/// Used by IntegerLiteral/FloatingLiteral to store the numeric without
/// leaking memory.
///
/// For large floats/integers, APFloat/APInt will allocate memory from the heap
/// to represent these numbers.  Unfortunately, when we use a BumpPtrAllocator
/// to allocate IntegerLiteral/FloatingLiteral nodes the memory associated with
/// the APFloat/APInt values will never get freed. APNumericStorage uses
/// ASTContext's allocator for memory allocation.
class APNumericStorage {
   union {
      uint64_t VAL;   ///< Used to store the <= 64 bits integer value.
      uint64_t* pVal; ///< Used to store the >64 bits integer value.
   };

   unsigned BitWidth;

   bool hasAllocation() const { return llvm::APInt::getNumWords(BitWidth) > 1; }

   APNumericStorage(const APNumericStorage&) = delete;
   void operator=(const APNumericStorage&) = delete;

protected:
   APNumericStorage() : VAL(0), BitWidth(0), Unsigned(false) {}

   bool Unsigned;

   llvm::APSInt getIntValue() const
   {
      unsigned NumWords = llvm::APInt::getNumWords(BitWidth);
      if (NumWords > 1)
         return llvm::APSInt(llvm::APInt(BitWidth, NumWords, pVal), Unsigned);
      else
         return llvm::APSInt(llvm::APInt(BitWidth, VAL, !Unsigned), Unsigned);
   }

   void setIntValue(const ASTContext& C, const llvm::APSInt& Val);
   void setIntValue(const ASTContext& C, const llvm::APInt& Val);
};

class APIntStorage : private APNumericStorage {
public:
   llvm::APSInt getValue() const { return getIntValue(); }
   void setValue(const ASTContext& C, const llvm::APSInt& Val)
   {
      setIntValue(C, Val);
   }

   void setUnsigned(bool Unsigned) { this->Unsigned = Unsigned; }
};

class APFloatStorage : private APNumericStorage {
public:
   llvm::APFloat getValue(const llvm::fltSemantics& Semantics) const
   {
      return llvm::APFloat(Semantics, getIntValue());
   }
   void setValue(const ASTContext& C, const llvm::APFloat& Val)
   {
      setIntValue(C, Val.bitcastToAPInt());
   }
};

class IntegerLiteral : public Expression {
public:
   enum class Suffix : unsigned char {
      None,
      u,
      i,
      i1,
      u1,
      i8,
      u8,
      i16,
      u16,
      i32,
      u32,
      i64,
      u64,
      i128,
      u128
   };

private:
   IntegerLiteral(SourceRange Loc, QualType type, Suffix suffix);

   SourceRange Loc;
   APIntStorage value;
   QualType type;
   Suffix suffix;
   InitDecl* ExpressibleByInit = nullptr;

public:
   static bool classofKind(NodeType kind) { return kind == IntegerLiteralID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static IntegerLiteral* Create(ASTContext& C, SourceRange Loc, QualType type,
                                 const llvm::APSInt& value,
                                 Suffix suffix = Suffix::None);

   IntegerLiteral(EmptyShell Empty);

   SourceRange getSourceRange() const { return Loc; }
   void setSourceRange(SourceRange SR) { Loc = SR; }

   llvm::APSInt getValue() { return value.getValue(); }
   void setValue(ASTContext& C, const llvm::APSInt& Val)
   {
      value.setValue(C, Val);
   }

   void setUnsigned(bool Unsigned) { value.setUnsigned(Unsigned); }

   QualType getType() const { return type; }
   void setType(QualType type) { IntegerLiteral::type = type; }

   Suffix getSuffix() const { return suffix; }
   void setSuffix(Suffix suffix) { IntegerLiteral::suffix = suffix; }

   InitDecl* getExpressibleByInit() const { return ExpressibleByInit; }
   void setExpressibleByInit(InitDecl* I) { ExpressibleByInit = I; }
};

class FPLiteral : public Expression {
public:
   enum class Suffix : unsigned char { None, d, f };

private:
   FPLiteral(SourceRange Loc, QualType type, Suffix suffix);

   SourceRange Loc;
   APFloatStorage value;
   QualType type;
   Suffix suffix;
   InitDecl* ExpressibleByInit = nullptr;

public:
   static bool classofKind(NodeType kind) { return kind == FPLiteralID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static FPLiteral* Create(ASTContext& C, SourceRange Loc, QualType type,
                            const llvm::APFloat& value,
                            Suffix suffix = Suffix::None);

   FPLiteral(EmptyShell Empty);

   SourceRange getSourceRange() const { return Loc; }
   void setSourceRange(SourceRange SR) { Loc = SR; }

   llvm::APFloat getValue(const llvm::fltSemantics& Sem
                          = llvm::APFloat::IEEEdouble()) const
   {
      return value.getValue(Sem);
   }

   void setValue(ASTContext& C, const llvm::APFloat& value)
   {
      this->value.setValue(C, value);
   }

   QualType getType() const { return type; }
   void setType(QualType type) { FPLiteral::type = type; }

   Suffix getSuffix() const { return suffix; }
   void setSuffix(Suffix suffix) { FPLiteral::suffix = suffix; }

   InitDecl* getExpressibleByInit() const { return ExpressibleByInit; }
   void setExpressibleByInit(InitDecl* I) { ExpressibleByInit = I; }
};

class BoolLiteral : public Expression {
   BoolLiteral(SourceLocation Loc, QualType type, bool value);

   SourceLocation Loc;
   bool value;
   QualType type;
   InitDecl* ExpressibleByInit = nullptr;

public:
   static bool classofKind(NodeType kind) { return kind == BoolLiteralID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static BoolLiteral* Create(ASTContext& C, SourceLocation Loc, QualType type,
                              bool value);

   BoolLiteral(EmptyShell Empty);

   SourceRange getSourceRange() const
   {
      SourceLocation EndLoc(Loc.getOffset() + (value ? 3 : 4));
      return SourceRange(Loc, EndLoc);
   }

   void setSourceLocation(SourceLocation SR) { Loc = SR; }

   bool getValue() const { return value; }
   void setValue(bool V) { value = V; }

   QualType getType() const { return type; }
   void setType(Type* Ty) { type = Ty; }

   InitDecl* getExpressibleByInit() const { return ExpressibleByInit; }
   void setExpressibleByInit(InitDecl* I) { ExpressibleByInit = I; }
};

class CharLiteral : public Expression {
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
   InitDecl* ExpressibleByInit = nullptr;

public:
   static bool classofKind(NodeType kind) { return kind == CharLiteralID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static CharLiteral* Create(ASTContext& C, SourceRange Loc, QualType type,
                              char value);
   static CharLiteral* Create(ASTContext& C, SourceRange Loc, QualType type,
                              uint32_t value);

   CharLiteral(EmptyShell Empty);

   SourceRange getSourceRange() const { return Loc; }
   void setSourceRange(SourceRange SR) { Loc = SR; }

   char getNarrow() const { return narrow; }

   uint32_t getWide() const { return wide; }
   bool isWide() const { return IsWide; }

   void setNarrow(char narrow) { CharLiteral::narrow = narrow; }
   void setWide(uint32_t wide) { CharLiteral::wide = wide; }

   QualType getType() const { return type; }
   void setType(QualType Ty) { type = Ty; }

   InitDecl* getExpressibleByInit() const { return ExpressibleByInit; }
   void setExpressibleByInit(InitDecl* I) { ExpressibleByInit = I; }
};

class NoneLiteral : public Expression {
   explicit NoneLiteral(SourceLocation Loc);

   SourceLocation Loc;

public:
   static bool classofKind(NodeType kind) { return kind == NoneLiteralID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static NoneLiteral* Create(ASTContext& C, SourceLocation Loc);
   NoneLiteral(EmptyShell Empty);

   SourceRange getSourceRange() const
   {
      return SourceRange(Loc, SourceLocation(Loc.getOffset() + 4));
   }

   void setSourceRange(SourceLocation SR) { Loc = SR; }
};

class StringLiteral : public Expression {
   StringLiteral(SourceRange Loc, std::string&& str);

   SourceRange Loc;
   std::string value;
   bool cstring = false;
   InitDecl* ExpressibleByInit = nullptr;

public:
   static bool classofKind(NodeType kind) { return kind == StringLiteralID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static StringLiteral* Create(ASTContext& C, SourceRange Loc,
                                std::string&& str);

   StringLiteral(EmptyShell Empty);

   SourceRange getSourceRange() const { return Loc; }
   void setSourceRange(SourceRange SR) { Loc = SR; }

   llvm::StringRef getValue() const { return value; }
   bool isCString() const { return cstring; }
   void setCString(bool raw) { cstring = raw; }

   void setValue(string&& value) { StringLiteral::value = move(value); }

   InitDecl* getExpressibleByInit() const { return ExpressibleByInit; }
   void setExpressibleByInit(InitDecl* I) { ExpressibleByInit = I; }
};

class StringInterpolation final
    : public Expression,
      llvm::TrailingObjects<StringInterpolation, Expression*> {
   StringInterpolation(SourceRange Loc, llvm::ArrayRef<Expression*> strings);

   StringInterpolation(EmptyShell Empty, unsigned N);

   SourceRange Loc;
   unsigned NumSegments;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind)
   {
      return kind == StringInterpolationID;
   }

   static StringInterpolation* Create(ASTContext& C, SourceRange Loc,
                                      llvm::ArrayRef<Expression*> strings);

   static StringInterpolation* CreateEmpty(ASTContext& C, unsigned N);

   SourceRange getSourceRange() const { return Loc; }
   void setSourceRange(SourceRange SR) { Loc = SR; }

   llvm::MutableArrayRef<Expression*> getSegments()
   {
      return {getTrailingObjects<Expression*>(), NumSegments};
   }

   friend TrailingObjects;
};

class LambdaExpr final : public Expression,
                         llvm::TrailingObjects<LambdaExpr, FuncArgDecl*> {
   LambdaExpr(SourceRange Parens, SourceLocation ArrowLoc,
              SourceType returnType, llvm::ArrayRef<FuncArgDecl*> args,
              Statement* body);

   LambdaExpr(EmptyShell Empty, unsigned N);

protected:
   SourceRange Parens;
   SourceLocation ArrowLoc;

   unsigned NumArgs;

   SourceType returnType;
   Statement* body;

   ASTVector<NamedDecl*> captures;
   FunctionDecl* func;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == LambdaExprID; }

   static LambdaExpr* Create(ASTContext& C, SourceRange Parens,
                             SourceLocation ArrowLoc, SourceType returnType,
                             llvm::ArrayRef<FuncArgDecl*> args,
                             Statement* body);

   static LambdaExpr* CreateEmpty(ASTContext& C, unsigned N);

   friend TrailingObjects;

   SourceRange getParenRange() const { return Parens; }
   SourceRange getSourceRange() const;

   void setParenRange(SourceRange SR) { Parens = SR; }
   void setArrowLoc(SourceLocation Loc) { ArrowLoc = Loc; }

   SourceLocation getSourceLoc() const { return Parens.getStart(); }
   SourceLocation getArrowLoc() const { return ArrowLoc; }

   bool isTrailingClosure() const { return !ArrowLoc; }

   unsigned getNumArgs() const { return NumArgs; }

   const SourceType& getReturnType() const { return returnType; }

   llvm::ArrayRef<FuncArgDecl*> getArgs() const
   {
      return {getTrailingObjects<FuncArgDecl*>(), NumArgs};
   }

   FuncArgDecl** arg_begin() { return getTrailingObjects<FuncArgDecl*>(); }

   void setReturnType(SourceType Ty) { returnType = Ty; }

   Statement* getBody() const { return body; }
   void setBody(Statement* B) { body = B; }

   ArrayRef<NamedDecl*> getCaptures() const { return captures; }
   size_t addCapture(ASTContext& C, NamedDecl* ND);

   FunctionDecl* getFunc() const { return func; }
   void setFunc(FunctionDecl* fn) { func = fn; }
};

class TupleLiteral final : public Expression,
                           llvm::TrailingObjects<TupleLiteral, Expression*> {
public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == TupleLiteralID; }

   static TupleLiteral* Create(ASTContext& C, SourceRange Parens,
                               llvm::ArrayRef<Expression*> Exprs);

   static TupleLiteral* CreateEmpty(ASTContext& C, unsigned N);

   InitDecl* getExpressibleByInit() const { return ExpressibleByInit; }
   void setExpressibleByInit(InitDecl* I) { ExpressibleByInit = I; }

   SourceRange getSourceRange() const { return Parens; }
   void setParenRange(SourceRange SR) { Parens = SR; }

   llvm::ArrayRef<Expression*> getElements() const
   {
      return {getTrailingObjects<Expression*>(), NumElements};
   }

   llvm::MutableArrayRef<Expression*> getElements()
   {
      return {getTrailingObjects<Expression*>(), NumElements};
   }

   friend TrailingObjects;

private:
   TupleLiteral(SourceRange Parens, llvm::ArrayRef<Expression*> Exprs);
   TupleLiteral(EmptyShell Empty, unsigned N);

   SourceRange Parens;
   unsigned NumElements;
   InitDecl* ExpressibleByInit = nullptr;
};

class ArrayLiteral final : public Expression,
                           llvm::TrailingObjects<ArrayLiteral, Expression*> {
public:
   static ArrayLiteral* Create(ASTContext& ASTCtx, SourceRange SquareRange,
                               llvm::ArrayRef<Expression*> exprs);

   static ArrayLiteral* CreateEmpty(ASTContext& C, unsigned N);

   InitDecl* getExpressibleByInit() const { return ExpressibleByInit; }
   void setExpressibleByInit(InitDecl* I) { ExpressibleByInit = I; }

   SourceRange getSourceRange() const { return SquareRange; }
   void setSquareRange(SourceRange SR) { SquareRange = SR; }

   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == ArrayLiteralID; }

   friend TrailingObjects;

private:
   ArrayLiteral(SourceRange SquareRange, llvm::ArrayRef<Expression*> exprs);

   ArrayLiteral(EmptyShell Empty, unsigned N);

   SourceRange SquareRange;
   unsigned numElements;
   InitDecl* InitFn = nullptr;
   InitDecl* ExpressibleByInit = nullptr;

public:
   size_t size() const { return numElements; }

   llvm::MutableArrayRef<Expression*> getValues()
   {
      return llvm::MutableArrayRef<Expression*>(
          getTrailingObjects<Expression*>(), numElements);
   }

   llvm::ArrayRef<Expression*> getValues() const
   {
      return llvm::ArrayRef<Expression*>(getTrailingObjects<Expression*>(),
                                         numElements);
   }

   InitDecl* getInitFn() const { return InitFn; }
   void setInitFn(InitDecl* V) { InitFn = V; }
};

class DictionaryLiteral final
    : public Expression,
      llvm::TrailingObjects<DictionaryLiteral, Expression*> {
public:
   static DictionaryLiteral* Create(ASTContext& ASTCtx, SourceRange SquareRange,
                                    llvm::ArrayRef<Expression*> keys,
                                    llvm::ArrayRef<Expression*> values);

   static DictionaryLiteral* CreateEmpty(ASTContext& C, unsigned N);

   SourceRange getSourceRange() const { return SquareRange; }
   void setSquareRange(SourceRange SR) { SquareRange = SR; }

   InitDecl* getExpressibleByInit() const { return ExpressibleByInit; }
   void setExpressibleByInit(InitDecl* I) { ExpressibleByInit = I; }

   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind)
   {
      return kind == DictionaryLiteralID;
   }

   friend TrailingObjects;

private:
   DictionaryLiteral(SourceRange SquareRange, llvm::ArrayRef<Expression*> keys,
                     llvm::ArrayRef<Expression*> values);

   DictionaryLiteral(EmptyShell Empty, unsigned N);

   SourceRange SquareRange;
   unsigned numKeyValuePairs;
   InitDecl* InitFn = nullptr;
   MethodDecl* InsertFn = nullptr;
   InitDecl* ExpressibleByInit = nullptr;

public:
   size_t size() const { return numKeyValuePairs; }

   llvm::MutableArrayRef<Expression*> getKeys()
   {
      return llvm::MutableArrayRef<Expression*>(
          getTrailingObjects<Expression*>(), size());
   }

   llvm::ArrayRef<Expression*> getKeys() const
   {
      return llvm::ArrayRef<Expression*>(getTrailingObjects<Expression*>(),
                                         size());
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

   MethodDecl* getInsertFn() const { return InsertFn; }
   void setInsertFn(MethodDecl* Fn) { InsertFn = Fn; }

   InitDecl* getInitFn() const { return InitFn; }
   void setInitFn(InitDecl* V) { InitFn = V; }
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
   Import,
   Module,
   MetaType,
   Alias,

   Accessor,
   UnionAccess,
   Type,
   TypeOf,
   BuiltinArraySize,
   StaticField,
   Field,
   EnumRawValue,
   PartiallyAppliedMethod,
};

class IdentifierRefExpr : public IdentifiedExpr {
   SourceRange Loc;
   IdentifierKind kind = IdentifierKind::Unknown;
   Expression* ParentExpr = nullptr;
   DeclContext* DeclCtx = nullptr;
   NestedNameSpecifierWithLoc* NameSpec = nullptr;

   union {
      Type* builtinType = nullptr;
      MetaType* metaType;
      NamedDecl* ND;
      VarDecl* varDecl;
      CallableDecl* callable;
      LocalVarDecl* localVar;
      GlobalVarDecl* globalVar;
      FuncArgDecl* funcArg;
      NamespaceDecl* namespaceDecl;
      AliasDecl* Alias;
      FieldDecl* staticFieldDecl;
      FieldDecl* fieldDecl;
      PropDecl* accessor;
      MethodDecl* partiallyAppliedMethod;
      TemplateParamDecl* templateParam;
      ImportDecl* Import;
      ModuleDecl* Module;
   };

   BuiltinIdentifier builtinKind;

   bool staticLookup : 1;
   bool pointerAccess : 1;
   bool OnlyForLookup : 1;
   bool InTypePosition : 1;
   bool IsSynthesized : 1;
   bool IsCapture : 1;
   bool called : 1;
   bool IsSelf : 1;
   bool AllowIncompleteTemplateArgs : 1;
   bool AllowNamespaceRef : 1;
   bool AllowOverloadRef : 1;
   bool AllowVariadicRef : 1;
   bool LeadingDot : 1;
   bool IssueDiag : 1;

   size_t captureIndex = 0;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind)
   {
      return kind == IdentifierRefExprID;
   }

   IdentifierRefExpr(SourceRange Loc, DeclarationName Name,
                     DeclContext* DeclCtx = nullptr, bool InTypePos = false);

   IdentifierRefExpr(SourceRange Loc, Expression* ParentExpr,
                     DeclarationName Name, bool IsPointerAccess = false);

   IdentifierRefExpr(SourceRange Loc, IdentifierKind kind, QualType exprType);

   IdentifierRefExpr(SourceRange Loc, IdentifierKind kind, NamedDecl* ND,
                     QualType exprType);

   IdentifierRefExpr(IdentifierRefExpr&& expr) = default;

   IdentifierRefExpr(EmptyShell Empty);

   SourceRange getSourceRange() const;
   void setLoc(const SourceRange& Loc) { IdentifierRefExpr::Loc = Loc; }

   Expression* getParentExpr() const { return ParentExpr; }
   void setParentExpr(Expression* PE) { ParentExpr = PE; }

   Type* getBuiltinType() const { return builtinType; }
   void setBuiltinType(Type* BT) { builtinType = BT; }

   MetaType* getMetaType() const { return metaType; }
   void setMetaType(MetaType* MT) { metaType = MT; }

   BuiltinIdentifier getBuiltinKind() const { return builtinKind; }
   void setBuiltinKind(BuiltinIdentifier BK) { builtinKind = BK; }

   bool isSelf() const { return IsSelf; }
   void setSelf(bool V) { IsSelf = V; }

   bool isCalled() const { return called; }
   void setCalled(bool b) { called = b; }

   AliasDecl* getAlias() const { return Alias; }
   void setAlias(AliasDecl* Alias) { IdentifierRefExpr::Alias = Alias; }

   CallableDecl* getCallable() const { return callable; }
   void setCallable(CallableDecl* callable)
   {
      kind = IdentifierKind::Function;
      IdentifierRefExpr::callable = callable;
   }

   IdentifierKind getKind() const { return kind; }
   void setKind(IdentifierKind IK) { kind = IK; }

   NamedDecl* getNamedDecl() const
   {
      assert(kind != IdentifierKind::MetaType && "does not hold a decl!");
      return ND;
   }

   VarDecl* getVarDecl() const { return varDecl; }

   LocalVarDecl* getLocalVar() const { return localVar; }
   void setLocalVar(LocalVarDecl* LV) { localVar = LV; }

   GlobalVarDecl* getGlobalVar() const { return globalVar; }
   void setGlobalVar(GlobalVarDecl* GV) { globalVar = GV; }

   FuncArgDecl* getFuncArg() const { return funcArg; }
   void setFuncArg(FuncArgDecl* Arg) { funcArg = Arg; }

   NamespaceDecl* getNamespaceDecl() const { return namespaceDecl; }
   void setNamespaceDecl(NamespaceDecl* NS) { namespaceDecl = NS; }

   DeclContext* getDeclCtx() const { return DeclCtx; }
   void setDeclCtx(DeclContext* Ctx) { DeclCtx = Ctx; }

   FieldDecl* getStaticFieldDecl() const { return staticFieldDecl; }
   void setStaticFieldDecl(FieldDecl* staticFieldDecl)
   {
      kind = IdentifierKind::StaticField;
      IdentifierRefExpr::staticFieldDecl = staticFieldDecl;
   }

   FieldDecl* getFieldDecl() const { return fieldDecl; }
   void setFieldDecl(FieldDecl* fieldDecl)
   {
      kind = IdentifierKind::Field;
      IdentifierRefExpr::fieldDecl = fieldDecl;
   }

   PropDecl* getAccessor() const { return accessor; }
   void setAccessor(PropDecl* A)
   {
      kind = IdentifierKind::Accessor;
      accessor = A;
   }

   ImportDecl* getImport() const { return Import; }
   ModuleDecl* getModule() const { return Module; }

   void setImport(ImportDecl* Import)
   {
      kind = IdentifierKind::Import;
      IdentifierRefExpr::Import = Import;
   }

   void setModule(ModuleDecl* Module)
   {
      kind = IdentifierKind::Module;
      IdentifierRefExpr::Module = Module;
   }

   void setNamedDecl(NamedDecl* ND) { this->ND = ND; }

   TemplateParamDecl* getTemplateParam() const { return templateParam; }
   void setTemplateParam(TemplateParamDecl* P) { templateParam = P; }

   NestedNameSpecifierWithLoc* getNameSpec() const { return NameSpec; }
   void setNameSpec(NestedNameSpecifierWithLoc* V) { NameSpec = V; }

   bool isStaticLookup() const { return staticLookup; }
   void setStaticLookup(bool SL) { staticLookup = SL; }

   bool isOnlyForLookup() const { return OnlyForLookup; }
   void setOnlyForLookup(bool B) { OnlyForLookup = B; }

   bool isPointerAccess() const { return pointerAccess; }
   void setIsPointerAccess(bool pointer) { pointerAccess = pointer; }

   bool isInTypePosition() const { return InTypePosition; }
   void setInTypePos(bool pos) { InTypePosition = pos; }

   bool isSynthesized() const { return IsSynthesized; }
   void setIsSynthesized(bool Synth) { IsSynthesized = Synth; }

   bool allowIncompleteTemplateArgs() const
   {
      return AllowIncompleteTemplateArgs;
   }
   void setAllowIncompleteTemplateArgs(bool allow)
   {
      AllowIncompleteTemplateArgs = allow;
   }

   bool allowOverloadRef() const { return AllowOverloadRef; }
   void setAllowOverloadRef(bool V) { AllowOverloadRef = V; }

   bool allowNamespaceRef() const { return AllowNamespaceRef; }
   void setAllowNamespaceRef(bool V) { AllowNamespaceRef = V; }

   bool allowVariadicRef() const { return AllowVariadicRef; }
   void setAllowVariadicRef(bool V) { AllowVariadicRef = V; }

   bool isCapture() const { return IsCapture; }
   void setIsCapture(bool C) { IsCapture = C; }

   bool hasLeadingDot() const { return LeadingDot; }
   void setLeadingDot(bool V) { LeadingDot = V; }

   bool shouldIssueDiag() const { return IssueDiag; }
   void setIssueDiag(bool V) { IssueDiag = V; }

   MethodDecl* getPartiallyAppliedMethod() const
   {
      return partiallyAppliedMethod;
   }

   void setPartiallyAppliedMethod(MethodDecl* partiallyAppliedMethod)
   {
      kind = IdentifierKind::PartiallyAppliedMethod;
      IdentifierRefExpr::partiallyAppliedMethod = partiallyAppliedMethod;
   }

   size_t getCaptureIndex() const { return captureIndex; }
   void setCaptureIndex(size_t CI) { captureIndex = CI; }
};

class DeclRefExpr : public Expression {
   DeclRefExpr(NamedDecl* Decl, SourceRange SR);

   /// The referenced declaration.
   NamedDecl* Decl;

   /// The source range of the expression the reference comes from.
   SourceRange SR;

   /// The (optional) template parameters.
   TemplateArgListExpr *TemplateArgs = nullptr;

   /// The index of the capture, if any.
   unsigned CaptureIdx : 28;

   /// True iff this declaration is allowed to refer to a namespace (or module).
   bool AllowModuleRef : 1;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == DeclRefExprID; }

   static DeclRefExpr* Create(ASTContext& C, NamedDecl* Decl, SourceRange SR);
   explicit DeclRefExpr(EmptyShell Empty);

   NamedDecl* getDecl() const { return Decl; }
   void setDecl(NamedDecl* ND) { Decl = ND; }

   SourceRange getSourceRange() const { return SR; }
   void setSourceRange(SourceRange SR) { this->SR = SR; }

   unsigned getCaptureIndex() const { return CaptureIdx; }
   void setCaptureIndex(unsigned Idx) { CaptureIdx = Idx; }

   bool allowModuleRef() const { return AllowModuleRef; }
   void setAllowModuleRef(bool B) { AllowModuleRef = B; }

   TemplateArgListExpr *getTemplateArgs() const
   {
      return TemplateArgs;
   }

   void setTemplateArgs(TemplateArgListExpr *TemplateArgs)
   {
      this->TemplateArgs = TemplateArgs;
   }

   bool isCapture() const;
};

class MemberRefExpr : public Expression {
   MemberRefExpr(Expression* ParentExpr, NamedDecl* MemberDecl, SourceRange SR);

   /// The expression the member is accessed on.
   Expression* ParentExpr;

   /// The referenced declaration.
   NamedDecl* MemberDecl;

   /// The source range of the expression the reference comes from.
   SourceRange SR;

   /// Set to true iff this expression is called directly.
   bool called = false;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == MemberRefExprID; }

   static MemberRefExpr* Create(ASTContext& C, Expression* ParentExpr,
                                NamedDecl* MemberDecl, SourceRange SR);

   explicit MemberRefExpr(EmptyShell Empty);

   Expression* getParentExpr() const { return ParentExpr; }
   void setParentExpr(Expression* PE) { ParentExpr = PE; }

   NamedDecl* getMemberDecl() const { return MemberDecl; }
   void setMemberDecl(NamedDecl* ND) { MemberDecl = ND; }

   SourceRange getSourceRange() const { return SR; }
   void setSourceRange(SourceRange SR) { this->SR = SR; }

   bool isCalled() const { return called; }
   void setCalled(bool b) { called = b; }

   static bool needsMemberRefExpr(NamedDecl *ND);
};

class OverloadedDeclRefExpr final
    : public Expression,
      TrailingObjects<OverloadedDeclRefExpr, NamedDecl*> {
   OverloadedDeclRefExpr(ArrayRef<NamedDecl*> Decls, SourceRange SR,
                         Expression* ParentExpr);

   OverloadedDeclRefExpr(unsigned N);

   /// The number of overloads.
   unsigned NumOverloads;

   /// The source range of the expression the reference comes from.
   SourceRange SR;

   /// The expression the member is accessed on.
   Expression* ParentExpr = nullptr;

   /// The (optional) template parameters.
   TemplateArgListExpr *TemplateArgs = nullptr;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind)
   {
      return kind == OverloadedDeclRefExprID;
   }

   static OverloadedDeclRefExpr* Create(ASTContext& C,
                                        ArrayRef<NamedDecl*> Decls,
                                        SourceRange SR,
                                        Expression* ParentExpr = nullptr);

   static OverloadedDeclRefExpr* CreateEmpty(ASTContext& C, unsigned N);

   friend TrailingObjects;

   unsigned getNumOverloads() const { return NumOverloads; }

   ArrayRef<NamedDecl*> getOverloads() const
   {
      return {getTrailingObjects<NamedDecl*>(), NumOverloads};
   }

   MutableArrayRef<NamedDecl*> getOverloads()
   {
      return {getTrailingObjects<NamedDecl*>(), NumOverloads};
   }

   TemplateArgListExpr *getTemplateArgs() const
   {
      return TemplateArgs;
   }

   void setTemplateArgs(TemplateArgListExpr *TemplateArgs)
   {
      this->TemplateArgs = TemplateArgs;
   }

   Expression* getParentExpr() const { return ParentExpr; }
   void setParentExpr(Expression* PE) { ParentExpr = PE; }

   SourceRange getSourceRange() const { return SR; }
   void setSourceRange(SourceRange SR) { this->SR = SR; }
};

class SelfExpr : public Expression {
   explicit SelfExpr(SourceLocation Loc, bool Uppercase);

   SourceLocation Loc;
   FuncArgDecl* SelfArg = nullptr;

   bool Uppercase;
   unsigned CaptureIdx = unsigned(-1);

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == SelfExprID; }

   static SelfExpr* Create(ASTContext& C, SourceLocation Loc, bool Uppercase);
   SelfExpr(EmptyShell Empty);

   unsigned int getCaptureIndex() const { return CaptureIdx; }
   void setCaptureIndex(unsigned int Idx) { CaptureIdx = Idx; }
   bool isCapture() const { return CaptureIdx != -1; }

   bool isUppercase() const { return Uppercase; }
   void setUppercase(bool V) { Uppercase = V; }

   FuncArgDecl* getSelfArg() const { return SelfArg; }
   void setSelfArg(FuncArgDecl* SelfArg) { SelfExpr::SelfArg = SelfArg; }

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const
   {
      return SourceRange(Loc, SourceLocation(Loc.getOffset() + 3));
   }

   void setLoc(const SourceLocation& Loc) { SelfExpr::Loc = Loc; }
};

class SuperExpr : public Expression {
   explicit SuperExpr(SourceLocation Loc);

   SourceLocation Loc;
   FuncArgDecl* SelfArg = nullptr;
   unsigned CaptureIdx = unsigned(-1);

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == SuperExprID; }

   static SuperExpr* Create(ASTContext& C, SourceLocation Loc);
   SuperExpr(EmptyShell Empty);

   unsigned int getCaptureIndex() const { return CaptureIdx; }
   void setCaptureIndex(unsigned int Idx) { CaptureIdx = Idx; }
   bool isCapture() const { return CaptureIdx != -1; }

   FuncArgDecl* getSelfArg() const { return SelfArg; }
   void setSelfArg(FuncArgDecl* SelfArg) { SuperExpr::SelfArg = SelfArg; }

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const
   {
      return SourceRange(Loc, SourceLocation(Loc.getOffset() + 4));
   }

   void setLoc(SourceLocation Loc) { SuperExpr::Loc = Loc; }
};

class BuiltinIdentExpr : public Expression {
   BuiltinIdentExpr(SourceLocation Loc, BuiltinIdentifier identifier);

   SourceLocation Loc;
   BuiltinIdentifier identifierKind;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == BuiltinIdentExprID; }

   static BuiltinIdentExpr* Create(ASTContext& C, SourceLocation Loc,
                                   BuiltinIdentifier identifier);

   BuiltinIdentExpr(EmptyShell Empty);

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const { return SourceRange(Loc); }
   BuiltinIdentifier getIdentifierKind() const { return identifierKind; }

   void setLoc(const SourceLocation& Loc) { BuiltinIdentExpr::Loc = Loc; }
   void setIdentifierKind(BuiltinIdentifier IK) { identifierKind = IK; }
};

class BuiltinExpr : public Expression {
   explicit BuiltinExpr(QualType type);
   explicit BuiltinExpr(SourceType T);

   SourceType T;
   SourceRange SR;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == BuiltinExprID; }

   static BuiltinExpr* Create(ASTContext& C, QualType Ty);
   static BuiltinExpr* Create(ASTContext& C, SourceType Ty);

   static BuiltinExpr CreateTemp(QualType Ty);
   BuiltinExpr(EmptyShell Empty);

   const SourceType& getType() const { return T; }
   void setType(SourceType T) { this->T = T; }

   void setSourceRange(SourceRange SR) { this->SR = SR; }
   SourceRange getSourceRange() const { return SR; }
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

class TupleMemberExpr : public Expression {
   SourceLocation Loc;
   Expression* ParentExpr;

   unsigned Index : 31;
   bool PointerAccess : 1;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == TupleMemberExprID; }

   TupleMemberExpr(SourceLocation Loc, Expression* ParentExpr, unsigned Index,
                   bool PointerAccess = false);

   TupleMemberExpr(EmptyShell Empty);

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const { return SourceRange(Loc); }

   void setLoc(const SourceLocation& Loc) { TupleMemberExpr::Loc = Loc; }

   Expression* getParentExpr() const { return ParentExpr; }
   void setParentExpr(Expression* Expr) { ParentExpr = Expr; }

   unsigned getIndex() const { return Index; }
   bool isPointerAccess() const { return PointerAccess; }

   void setIndex(unsigned int Index) { TupleMemberExpr::Index = Index; }
   void setPointerAccess(bool b) { PointerAccess = b; }
};

enum class CallKind : unsigned {
   Unknown,
   Builtin,
   NamedFunctionCall,
   DependentFunctionCall,
   MethodCall,
   UnionInitializer,
   InitializerCall,
   GenericInitializerCall,
   StaticMethodCall,
   CallOperator,
   UnsafeTupleGet,
   VariadicSizeof,
   PrimitiveInitializer,
   GlobalVariableCall,
   LocalVariableCall,
};

class CallExpr final : public Expression,
                       TrailingObjects<CallExpr, IdentifierInfo*> {
   CallExpr(SourceLocation IdentLoc, SourceRange ParenRange,
            ASTVector<Expression*>&& args, ArrayRef<IdentifierInfo*> Labels,
            DeclarationName Name = DeclarationName(), bool IsDotInit = false,
            bool IsDotDeinit = false);

   CallExpr(SourceLocation IdentLoc, SourceRange ParenRange,
            Expression* ParentExpr, ASTVector<Expression*>&& args,
            ArrayRef<IdentifierInfo*> Labels,
            DeclarationName Name = DeclarationName(), bool IsDotInit = false,
            bool IsDotDeinit = false);

   CallExpr(SourceLocation IdentLoc, SourceRange ParenRange,
            ASTVector<Expression*>&& args, CallableDecl* C, CallKind K,
            QualType ExprType);

   CallExpr(EmptyShell Empty, unsigned N);

   SourceLocation IdentLoc;
   SourceRange ParenRange;

   DeclarationName FuncName;
   unsigned NumLabels = 0;

   CallKind kind = CallKind::Unknown;
   Expression* ParentExpr = nullptr;

   ASTVector<Expression*> args;
   sema::FinalTemplateArgumentList* templateArgs = nullptr;

   bool PointerAccess : 1;
   bool IsUFCS : 1;
   bool IsDotInit : 1;
   bool IsDotDeinit : 1;
   bool IncludesSelf : 1;
   bool DirectCall : 1;
   bool LeadingDot : 1;
   unsigned BuiltinKind : 10;

   CallableDecl* func = nullptr;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == CallExprID; }

   friend TrailingObjects;

   static CallExpr* Create(ASTContext& C, SourceLocation IdentLoc,
                           SourceRange ParenRange,
                           ASTVector<Expression*>&& args,
                           ArrayRef<IdentifierInfo*> Labels,
                           DeclarationName Name = DeclarationName(),
                           bool IsDotInit = false, bool IsDotDeinit = false);

   static CallExpr* Create(ASTContext& C, SourceLocation IdentLoc,
                           SourceRange ParenRange, Expression* ParentExpr,
                           ASTVector<Expression*>&& args,
                           ArrayRef<IdentifierInfo*> Labels,
                           DeclarationName Name = DeclarationName(),
                           bool IsDotInit = false, bool IsDotDeinit = false);

   static CallExpr* Create(ASTContext& C, SourceLocation IdentLoc,
                           SourceRange ParenRange,
                           ASTVector<Expression*>&& args, CallableDecl* Fn,
                           CallKind K, QualType ExprType);

   static CallExpr* CreateEmpty(ASTContext& C, unsigned N);

   SourceLocation getIdentLoc() const { return IdentLoc; }
   SourceRange getParenRange() const { return ParenRange; }
   SourceRange getSourceRange() const;

   void setIdentLoc(const SourceLocation& IdentLoc)
   {
      CallExpr::IdentLoc = IdentLoc;
   }

   void setParenRange(const SourceRange& ParenRange)
   {
      CallExpr::ParenRange = ParenRange;
   }

   ArrayRef<IdentifierInfo*> getLabels() const
   {
      return {getTrailingObjects<IdentifierInfo*>(), NumLabels};
   }

   MutableArrayRef<IdentifierInfo*> getLabels()
   {
      return {getTrailingObjects<IdentifierInfo*>(), NumLabels};
   }

   DeclarationName getDeclName() const { return FuncName; }
   void setIdent(DeclarationName ident) { this->FuncName = ident; }

   Expression* getParentExpr() const { return ParentExpr; }
   void setParentExpr(Expression* E) { ParentExpr = E; }

   ASTVector<Expression*>& getArgs() { return args; }
   llvm::ArrayRef<Expression*> getArgs() const { return args; }

   sema::FinalTemplateArgumentList* getTemplateArgs() const
   {
      return templateArgs;
   }
   void setTemplateArgs(sema::FinalTemplateArgumentList* templateArgs)
   {
      this->templateArgs = templateArgs;
   }

   CallableDecl* getFunc() const { return func; }
   void setFunc(CallableDecl* func) { this->func = func; }

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
   void setIsDotDeinit(bool b) { IsDotDeinit = b; }

   bool includesSelf() const { return IncludesSelf; }
   void setIncludesSelf(bool S) { IncludesSelf = S; }

   bool isDirectCall() const { return DirectCall; }
   void setDirectCall(bool b) { DirectCall = b; }

   bool hasLeadingDot() const { return LeadingDot; }
   void setLeadingDot(bool V) { LeadingDot = V; }

   unsigned getBuiltinKind() const { return BuiltinKind; }
   void setBuiltinKind(unsigned Kind) { BuiltinKind = Kind; }

   bool isKnownFunctionCall();
};

class AnonymousCallExpr final
    : public Expression,
      llvm::TrailingObjects<AnonymousCallExpr, Expression*, IdentifierInfo*> {
   AnonymousCallExpr(SourceRange ParenRange, Expression* ParentExpr,
                     ArrayRef<Expression*> Args,
                     ArrayRef<IdentifierInfo*> Labels);

   AnonymousCallExpr(EmptyShell Empty, unsigned N);

   size_t numTrailingObjects(OverloadToken<Expression*>) const
   {
      return NumArgs;
   }

   SourceRange ParenRange;

   Expression* ParentExpr;
   Expression* SelfVal;
   FunctionType* FnTy;

   unsigned NumArgs : 31;
   bool IsPrimitiveInit : 1;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind)
   {
      return kind == AnonymousCallExprID;
   }

   friend TrailingObjects;

   static AnonymousCallExpr* Create(ASTContext& C, SourceRange ParenRange,
                                    Expression* ParentExpr,
                                    ArrayRef<Expression*> Args,
                                    ArrayRef<IdentifierInfo*> Labels);

   static AnonymousCallExpr* CreateEmpty(ASTContext& C, unsigned N);

   SourceRange getSourceRange() const;
   SourceRange getParenRange() const { return ParenRange; }

   void setParenRange(SourceRange SR) { ParenRange = SR; }

   Expression* getParentExpr() const { return ParentExpr; }
   void setParentExpr(Expression* PE) { ParentExpr = PE; }

   Expression* getSelfVal() const { return SelfVal; }
   void setSelfVal(Expression* E) { SelfVal = E; }

   FunctionType* getFunctionType() const { return FnTy; }
   void setFunctionType(FunctionType* FnTy) { AnonymousCallExpr::FnTy = FnTy; }

   llvm::MutableArrayRef<Expression*> getArgs()
   {
      return {getTrailingObjects<Expression*>(), NumArgs};
   }

   ArrayRef<IdentifierInfo*> getLabels() const
   {
      return {getTrailingObjects<IdentifierInfo*>(), NumArgs};
   }

   MutableArrayRef<IdentifierInfo*> getLabels()
   {
      return {getTrailingObjects<IdentifierInfo*>(), NumArgs};
   }

   bool isPrimitiveInit() const { return IsPrimitiveInit; }
   void setIsPrimitiveInit(bool PI) { IsPrimitiveInit = PI; }
};

class SubscriptExpr final : public Expression,
                            llvm::TrailingObjects<SubscriptExpr, Expression*> {
   SubscriptExpr(SourceRange SquareRange, Expression* ParentExpr,
                 llvm::ArrayRef<Expression*> indices);

   SubscriptExpr(EmptyShell Empty, unsigned N);

   SourceRange SquareRange;
   Expression* ParentExpr;
   SubscriptDecl *Subscript;
   Expression *Call;
   unsigned NumIndices;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == SubscriptExprID; }

   friend TrailingObjects;

   static SubscriptExpr* Create(ASTContext& C, SourceRange SquareRange,
                                Expression* ParentExpr,
                                llvm::ArrayRef<Expression*> indices);

   static SubscriptExpr* CreateEmpty(ASTContext& C, unsigned N);

   SourceRange getSourceRange() const;
   void setSquareRange(SourceRange SR) { SquareRange = SR; }

   Expression* getParentExpr() const { return ParentExpr; }
   void setParentExpr(Expression* PE) { ParentExpr = PE; }

   llvm::ArrayRef<Expression*> getIndices() const
   {
      return {getTrailingObjects<Expression*>(), NumIndices};
   }

   llvm::MutableArrayRef<Expression*> getIndices()
   {
      return {getTrailingObjects<Expression*>(), NumIndices};
   }

   bool hasSingleIndex() const { return NumIndices == 1; }

   SubscriptDecl *getSubscriptDecl() const { return Subscript; }
   void setSubscriptDecl(SubscriptDecl *D) { Subscript = D; }

   Expression *getCallExpr() const { return Call; }
   void setCallExpr(Expression *Call) { this->Call = Call; }
};

class TemplateArgListExpr final
    : public Expression,
      llvm::TrailingObjects<TemplateArgListExpr, Expression*> {
   TemplateArgListExpr(SourceRange AngleRange, Expression* ParentExpr,
                       ArrayRef<Expression*> Exprs);

   TemplateArgListExpr(SourceRange AngleRange, Expression* ParentExpr,
                       sema::FinalTemplateArgumentList* Exprs);

   TemplateArgListExpr(EmptyShell, unsigned N);

   SourceRange AngleRange;
   Expression* ParentExpr;
   sema::FinalTemplateArgumentList* TemplateArgs = nullptr;
   unsigned NumTemplateArgs;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind)
   {
      return kind == TemplateArgListExprID;
   }

   friend TrailingObjects;

   static TemplateArgListExpr* Create(ASTContext& C, SourceRange AngleRange,
                                      Expression* ParentExpr,
                                      ArrayRef<Expression*> Exprs);

   static TemplateArgListExpr* Create(ASTContext& C, SourceRange AngleRange,
                                      Expression* ParentExpr,
                                      sema::FinalTemplateArgumentList* Exprs);

   static TemplateArgListExpr* CreateEmpty(ASTContext& C, unsigned N);

   SourceRange getSourceRange() const;

   SourceRange getAngleRange() const { return AngleRange; }
   void setAngleRange(SourceRange V) { AngleRange = V; }

   Expression* getParentExpr() const { return ParentExpr; }
   void setParentExpr(Expression* V) { ParentExpr = V; }

   sema::FinalTemplateArgumentList* getTemplateArgs() const
   {
      return TemplateArgs;
   }
   void setTemplateArgs(sema::FinalTemplateArgumentList* V)
   {
      TemplateArgs = V;
   }

   unsigned getNumTemplateArgs() const { return NumTemplateArgs; }

   MutableArrayRef<Expression*> getExprs()
   {
      return {getTrailingObjects<Expression*>(), NumTemplateArgs};
   }

   ArrayRef<Expression*> getExprs() const
   {
      return {getTrailingObjects<Expression*>(), NumTemplateArgs};
   }
};

class StaticExpr : public Expression {
public:
   static StaticExpr* Create(ASTContext& C, Expression* Expr);
   static StaticExpr* Create(ASTContext& C, QualType Type, SourceRange SR,
                             il::Constant* V);

   StaticExpr(EmptyShell Empty);

   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == StaticExprID; }

private:
   explicit StaticExpr(Expression* expr);
   explicit StaticExpr(QualType Type, SourceRange SR, il::Constant* V);

   SourceRange Loc;
   Expression* expr;
   il::Constant* evaluatedExpr;

public:
   SourceLocation getSourceLoc() const { return Loc.getStart(); }
   SourceRange getSourceRange() const { return Loc; }
   void setLoc(SourceRange V) { Loc = V; }

   Expression* getExpr() const { return expr; }
   void setExpr(Expression* E) { expr = E; }

   il::Constant* getEvaluatedExpr() const { return evaluatedExpr; }
   void setEvaluatedExpr(il::Constant* V) { evaluatedExpr = V; }
};

class ConstraintExpr : public Expression {
public:
   enum Kind {
      Struct,
      Class,
      Enum,
      Union,
      Function,
      DefaultConstructible,
      Pointer,
      Reference,
      Type,
   };

   Kind getKind() const { return kind; }
   const SourceType& getTypeConstraint() const { return typeConstraint; }

   SourceLocation getSourceLoc() const { return Loc; }
   SourceRange getSourceRange() const { return SourceRange(Loc); }

   void setLoc(SourceLocation Loc) { ConstraintExpr::Loc = Loc; }
   void setKind(Kind kind) { ConstraintExpr::kind = kind; }
   void setTypeConstraint(SourceType C) { typeConstraint = C; }

private:
   ConstraintExpr(SourceLocation Loc, Kind kind);
   ConstraintExpr(SourceLocation Loc, SourceType typeConstraint);

   SourceLocation Loc;
   Kind kind;
   SourceType typeConstraint;

public:
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
   static bool classofKind(NodeType kind) { return kind == ConstraintExprID; }

   static ConstraintExpr* Create(ASTContext& C, SourceLocation Loc, Kind kind);
   static ConstraintExpr* Create(ASTContext& C, SourceLocation Loc,
                                 SourceType typeConstraint);

   ConstraintExpr(EmptyShell Empty);
};

struct TraitsArgument {
   enum Kind { Stmt, Expr, Type, String };

   explicit TraitsArgument(Statement* stmt) : kind(Stmt), stmt(stmt) {}

   explicit TraitsArgument(Expression* expr) : kind(Expr), expr(expr) {}

   explicit TraitsArgument(SourceType Ty) : kind(Type), Ty(Ty) {}

   explicit TraitsArgument(std::string&& str) : kind(String), str(move(str)) {}

   TraitsArgument(TraitsArgument&& other) noexcept : kind(other.kind)
   {
      copyData(std::move(other));
   }

   TraitsArgument& operator=(TraitsArgument&& other) noexcept
   {
      destroyValue();
      copyData(std::move(other));

      return *this;
   }

   ~TraitsArgument() { destroyValue(); }

   Kind getKind() const { return kind; }

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

   const SourceType& getType() const
   {
      assert(kind == Type);
      return Ty;
   }

   const string& getStr() const
   {
      assert(kind == String);
      return str;
   }

   void setStmt(Statement* S) { stmt = S; }
   void setExpr(Expression* E) { expr = E; }
   void setType(SourceType T) { Ty = T; }
   void setStr(string&& str) { this->str = move(str); }

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

   void copyData(TraitsArgument&& other)
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

class TraitsExpr final : public Expression,
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
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   static TraitsExpr* Create(ASTContext& C, SourceLocation TraitsLoc,
                             SourceRange Parens, Kind kind,
                             llvm::MutableArrayRef<TraitsArgument> args);

   static TraitsExpr* CreateEmpty(ASTContext& C, unsigned N);

   SourceLocation getSourceLoc() const { return TraitsLoc; }
   SourceLocation getTraitsLoc() const { return TraitsLoc; }

   void setTraitsLoc(SourceLocation Loc) { TraitsLoc = Loc; }
   void setParens(SourceRange SR) { Parens = SR; }

   SourceRange getParenRange() const { return Parens; }
   SourceRange getSourceRange() const
   {
      return SourceRange(TraitsLoc, Parens.getEnd());
   }

   Kind getKind() const { return kind; }
   void setKind(Kind K) { kind = K; }

   llvm::ArrayRef<TraitsArgument> getArgs() const
   {
      return {getTrailingObjects<TraitsArgument>(), NumArgs};
   }

   llvm::MutableArrayRef<TraitsArgument> getArgs()
   {
      return {getTrailingObjects<TraitsArgument>(), NumArgs};
   }

   using TrailingObjects::getTrailingObjects;
   friend TrailingObjects;

private:
   TraitsExpr(SourceLocation TraitsLoc, SourceRange Parens, Kind kind,
              llvm::MutableArrayRef<TraitsArgument> args);

   TraitsExpr(EmptyShell Empty, unsigned N);

   SourceLocation TraitsLoc;
   SourceRange Parens;
   Kind kind;
   unsigned NumArgs;
};

class MixinExpr : public Expression {
public:
   static MixinExpr* Create(ASTContext& C, SourceRange Parens,
                            Expression* Expr);

   explicit MixinExpr(EmptyShell Empty);

   SourceRange getSourceRange() const { return Parens; }
   void setParens(SourceRange SR) { Parens = SR; }

   Expression* getMixinExpr() const { return Expr; }
   void setMixinExpr(Expression* E) { Expr = E; }

   static bool classofKind(NodeType kind) { return kind == MixinExprID; }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

private:
   MixinExpr(SourceRange Parens, Expression* Expr);

   SourceRange Parens;
   Expression* Expr;
};

class VariadicExpansionExpr : public Expression {
   VariadicExpansionExpr(SourceLocation EllipsisLoc, Expression* Expr);

   SourceLocation EllipsisLoc;
   Expression* Expr;

   NamedDecl* parameterPack = nullptr;
   NamedDecl* elementDecl = nullptr;

public:
   static VariadicExpansionExpr*
   Create(ASTContext& C, SourceLocation EllipsisLoc, Expression* Expr);

   explicit VariadicExpansionExpr(EmptyShell Empty);

   SourceLocation getEllipsisLoc() const { return EllipsisLoc; }
   void setEllipsisLoc(SourceLocation loc) { this->EllipsisLoc = loc; }

   SourceRange getSourceRange() const;

   Expression* getExpr() const { return Expr; }
   void setExpr(Expression* E) { Expr = E; }

   NamedDecl* getParameterPack() const { return parameterPack; }
   void setParameterPack(NamedDecl* D) { parameterPack = D; }

   NamedDecl* getElementDecl() const { return elementDecl; }
   void setElementDecl(NamedDecl* D) { elementDecl = D; }

   static bool classofKind(NodeType kind)
   {
      return kind == VariadicExpansionExprID;
   }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }
};

class MacroVariableExpr : public Expression {
   explicit MacroVariableExpr(Expression* E);

   Expression* E;

public:
   static MacroVariableExpr* Create(ASTContext& C, Expression* E);
   explicit MacroVariableExpr(EmptyShell Empty);

   static bool classofKind(NodeType kind)
   {
      return kind == MacroVariableExprID;
   }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   SourceRange getSourceRange() const { return E->getSourceRange(); }
   Expression* getExpr() const { return E; }

   void setExpr(Expression* E) { MacroVariableExpr::E = E; }
};

class MacroExpansionExpr final
    : public Expression,
      llvm::TrailingObjects<MacroExpansionExpr, lex::Token> {
public:
   enum Delimiter {
      Brace,
      Square,
      Paren,
   };

   friend TrailingObjects;

private:
   MacroExpansionExpr(SourceRange SR, DeclarationName MacroName,
                      Delimiter Delim, llvm::ArrayRef<lex::Token> Toks,
                      Expression* ParentExpr);

   MacroExpansionExpr(EmptyShell Empty, unsigned N);

   SourceRange SR;
   Delimiter Delim;
   DeclarationName MacroName;
   unsigned NumTokens;

   Expression* ParentExpr;

public:
   static MacroExpansionExpr* Create(ASTContext& C, SourceRange SR,
                                     DeclarationName MacroName, Delimiter Delim,
                                     llvm::ArrayRef<lex::Token> Toks,
                                     Expression* ParentExpr = nullptr);

   static MacroExpansionExpr* CreateEmpty(ASTContext& C, unsigned N);

   static bool classofKind(NodeType kind)
   {
      return kind == MacroExpansionExprID;
   }
   static bool classof(AstNode const* T) { return classofKind(T->getTypeID()); }

   SourceRange getSourceRange() const { return SR; }
   DeclarationName getMacroName() const { return MacroName; }
   Delimiter getDelim() const { return Delim; }

   void setSourceRange(SourceRange SR) { this->SR = SR; }
   void setDelim(Delimiter D) { Delim = D; }
   void setMacroName(DeclarationName N) { MacroName = N; }

   Expression* getParentExpr() const { return ParentExpr; }
   void setParentExpr(Expression* E) { ParentExpr = E; }

   llvm::ArrayRef<lex::Token> getTokens() const
   {
      return {getTrailingObjects<lex::Token>(), NumTokens};
   }

   using TrailingObjects::getTrailingObjects;
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

   if (auto A = support::dyn_cast<AttributedExpr>(this))
      return A->getExpr()->ignoreParensAndImplicitCasts();

   return this;
}

inline Expression* Expression::ignoreParens() const
{
   return const_cast<Expression*>(this)->ignoreParens();
}

inline Expression* Expression::ignoreParensAndImplicitCasts() const
{
   return const_cast<Expression*>(this)->ignoreParensAndImplicitCasts();
}

} // namespace ast
} // namespace cdot

#endif // EXPRESSION_H
