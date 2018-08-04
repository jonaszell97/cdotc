//
// Created by Jonas Zell on 19.06.17.
//

#include "Expression.h"

#include "AST/ASTContext.h"
#include "Decl.h"
#include "Sema/Builtin.h"
#include "Support/Casting.h"

using namespace cdot::support;

namespace cdot {
namespace ast {

Expression::Expression(NodeType typeID)
   : Statement(typeID),
     IsLHSOfAssignment(false), IsMagicArgumentValue(false),
     AllowTemplate(false)
{}

SourceRange Expression::getEllipsisRange() const
{
   if (!EllipsisLoc)
      return SourceRange();

   return SourceRange(EllipsisLoc, SourceLocation(EllipsisLoc.getOffset() + 2));
}

Expression* Expression::getParentExpr() const
{
   switch (typeID) {
   case IdentifierRefExprID:
      return cast<IdentifierRefExpr>(this)->getParentExpr();
   case SubscriptExprID:
      return cast<SubscriptExpr>(this)->getParentExpr();
   case TupleMemberExprID:
      return cast<TupleMemberExpr>(this)->getParentExpr();
   case CallExprID:
      return cast<CallExpr>(this)->getParentExpr();
   case MacroExpansionExprID:
      return cast<MacroExpansionExpr>(this)->getParentExpr();
   default:
      break;
   }

   return nullptr;
}

void Expression::setParentExpr(Expression *E)
{
   switch (typeID) {
   case IdentifierRefExprID:
      return cast<IdentifierRefExpr>(this)->setParentExpr(E);
   case SubscriptExprID:
      return cast<SubscriptExpr>(this)->setParentExpr(E);
   case TupleMemberExprID:
      return cast<TupleMemberExpr>(this)->setParentExpr(E);
   case CallExprID:
      return cast<CallExpr>(this)->setParentExpr(E);
   case MacroExpansionExprID:
      return cast<MacroExpansionExpr>(this)->setParentExpr(E);
   default:
      break;
   }

   llvm_unreachable("cannot set parent expression!");
}

bool Expression::isConst() const
{
   Expression const* DeclRef = this;
   while (DeclRef) {
      if (auto Ident = dyn_cast<IdentifierRefExpr>(DeclRef)) {
         switch (Ident->getKind()) {
         case IdentifierKind::LocalVar:
            if (Ident->getLocalVar()->isConst()) {
               return true;
            }

            break;
         case IdentifierKind::GlobalVar:
            if (Ident->getGlobalVar()->isConst()) {
               return true;
            }

            break;
         case IdentifierKind::Field:
            if (Ident->getFieldDecl()->isConst()) {
               return true;
            }

            break;
         case IdentifierKind::StaticField:
            if (Ident->getStaticFieldDecl()->isConst()) {
               return true;
            }

            break;
         case IdentifierKind::FunctionArg:
            if (Ident->getFuncArg()->isConst()) {
               return true;
            }

            break;
         default:
            break;
         }

         DeclRef = Ident->getParentExpr();
         continue;
      }
      else if (auto Subscript = dyn_cast<SubscriptExpr>(DeclRef)) {
         DeclRef = Subscript->getParentExpr();
         continue;
      }
      else if (auto Tup = dyn_cast<TupleMemberExpr>(DeclRef)) {
         DeclRef = Tup->getParentExpr();
         continue;
      }
      else if (auto Self = dyn_cast<SelfExpr>(DeclRef)) {
         return false;
      }


      break;
   }

   return false;
}

bool Expression::warnOnUnusedResult() const
{
   if (isDependent() || isInvalid())
      return false;

   switch (typeID) {
   case NodeType::ParenExprID:
   case NodeType::AttributedExprID:
   case NodeType::ImplicitCastExprID:
      return ignoreParensAndImplicitCasts()->warnOnUnusedResult();
   case NodeType::BinaryOperatorID:
      return true;
   case NodeType::AssignExprID:
      return false;
   case NodeType::ExprSequenceID:
      // we can't tell what an unresolved expr sequence will end up being
      return false;
   case NodeType::UnaryOperatorID:
      switch (cast<UnaryOperator>(this)->getKind()) {
      case op::PreInc: case op::PreDec: case op::PostInc: case op::PostDec:
         return false;
      default:
         return true;
      }
   case NodeType::CallExprID: {
      auto Call = cast<CallExpr>(this);
      switch (Call->getKind()) {
      case CallKind::NamedFunctionCall:
      case CallKind::StaticMethodCall:
      case CallKind::MethodCall:
      case CallKind::InitializerCall: {
         if (Call->isDotInit())
            return false;

         QualType RetTy = Call->getExprType();
         return !RetTy->isVoidType()
                && !RetTy->isEmptyTupleType()
                && !RetTy->isUnpopulatedType()
                && !Call->getFunc()->hasAttribute<DiscardableResultAttr>();
      }
      default:
         return !Call->getExprType()->isVoidType()
            && !Call->getExprType()->isEmptyTupleType()
            && !Call->getExprType()->isUnpopulatedType();
      }
   }
   case NodeType::AnonymousCallExprID: {
      return !exprType->isVoidType() && !exprType->isUnpopulatedType()
         && !exprType->isEmptyTupleType();
   }
   default:
      return !exprType->isVoidType() && !exprType->isEmptyTupleType();
   }
}

bool Expression::isContextDependent() const
{
   if (exprType)
      return false;

   switch (getTypeID()) {
   case IntegerLiteralID:
      return cast<IntegerLiteral>(this)->getSuffix()
         == IntegerLiteral::Suffix::None;
   case FPLiteralID:
      return cast<FPLiteral>(this)->getSuffix() == FPLiteral::Suffix::None;
   case StringLiteralID:
   case LambdaExprID:
   case CharLiteralID:
   case BoolLiteralID:
      return true;
   case IdentifierRefExprID:
      return cast<IdentifierRefExpr>(this)->hasLeadingDot();
   case CallExprID:
      return cast<CallExpr>(this)->hasLeadingDot();
   case ParenExprID:
   case AttributedExprID:
      return ignoreParensAndImplicitCasts()->isContextDependent();
   case EnumCaseExprID:
      return cast<EnumCaseExpr>(this)->getCase() == nullptr;
   case BuiltinIdentExprID: {
      auto *E = cast<BuiltinIdentExpr>(this);
      return E->getIdentifierKind() == BuiltinIdentifier::NULLPTR;
   }
   default:
      return false;
   }
}

IdentifiedExpr::IdentifiedExpr(NodeType typeID, DeclarationName Name)
   : Expression(typeID),
     DeclName(Name)
{}

ParenExpr::ParenExpr(SourceRange Parens,
                     Expression *Expr)
   : Expression(ParenExprID),
     Parens(Parens), ParenthesizedExpr(Expr)
{
}

ParenExpr::ParenExpr(EmptyShell)
   : Expression(ParenExprID),
     ParenthesizedExpr(nullptr)
{}

ParenExpr* ParenExpr::Create(ASTContext &C,
                             SourceRange Parens,
                             Expression *Expr) {
   return new(C) ParenExpr(Parens, Expr);
}

ImplicitCastExpr::ImplicitCastExpr(Expression* target,
                                   ConversionSequence *ConvSeq)
   : Expression(ImplicitCastExprID),
     target(target), ConvSeq(ConvSeq)
{
   assert(ConvSeq->isValid() && "invalid conversion sequence");
}

ImplicitCastExpr::ImplicitCastExpr(EmptyShell)
   : Expression(ImplicitCastExprID),
     target(nullptr)
{}

ImplicitCastExpr* ImplicitCastExpr::Create(ASTContext &C,
                                           Expression *target,
                                           ConversionSequence *ConvSeq) {
   return new(C) ImplicitCastExpr(target, ConvSeq);
}

TupleTypeExpr::TupleTypeExpr(SourceRange SR,
                             llvm::ArrayRef<SourceType> Tys,
                             bool IsMeta)
   : TypeExpr(TupleTypeExprID, SR, IsMeta), NumTys((unsigned)Tys.size())
{
   std::copy(Tys.begin(), Tys.end(), getTrailingObjects<SourceType>());
}

TupleTypeExpr* TupleTypeExpr::Create(ASTContext &C,
                                     SourceRange SR,
                                     llvm::ArrayRef<SourceType> Tys,
                                     bool IsMeta) {
   void *Mem = C.Allocate(totalSizeToAlloc<SourceType>(Tys.size()),
                          alignof(TupleTypeExpr));

   return new(Mem) TupleTypeExpr(SR, Tys, IsMeta);
}

TupleTypeExpr::TupleTypeExpr(EmptyShell, unsigned N)
   : TypeExpr(TupleTypeExprID, {}, {}), NumTys(N)
{}

TupleTypeExpr *TupleTypeExpr::CreateEmpty(ASTContext &C, unsigned N)
{
    void *Mem = C.Allocate(totalSizeToAlloc<SourceType>(N), alignof(TupleTypeExpr));
    return new(Mem) TupleTypeExpr(EmptyShell(), N);
}

FunctionTypeExpr::FunctionTypeExpr(SourceRange SR,
                                   SourceType RetTy,
                                   llvm::ArrayRef<SourceType> Tys,
                                   llvm::ArrayRef<FunctionType::ParamInfo> Info,
                                   bool IsMeta, bool IsThin, bool Throws,
                                   bool IsAsync, bool IsUnsafe)
   : TypeExpr(FunctionTypeExprID, SR, IsMeta), RetTy(RetTy),
     NumArgs((unsigned)Tys.size()), Thin(IsThin), Throws(Throws),
     Async(IsAsync), Unsafe(IsUnsafe)
{
   std::copy(Tys.begin(), Tys.end(), getTrailingObjects<SourceType>());
   std::copy(Info.begin(), Info.end(),
             getTrailingObjects<FunctionType::ParamInfo>());
}

FunctionTypeExpr* FunctionTypeExpr::Create(ASTContext &C,
                                  SourceRange SR,
                                  SourceType RetTy,
                                  llvm::ArrayRef<SourceType> Tys,
                                  llvm::ArrayRef<FunctionType::ParamInfo> Info,
                                  bool IsMeta, bool IsThin, bool Throws,
                                  bool IsAsync, bool IsUnsafe) {
   assert(Tys.size() == Info.size());

   void *Mem = C.Allocate(totalSizeToAlloc<SourceType,
                                           FunctionType::ParamInfo>(
                                              Tys.size(), Info.size()),
                          alignof(FunctionTypeExpr));

   return new(Mem) FunctionTypeExpr(SR, RetTy, Tys, Info, IsMeta, IsThin,
                                    Throws, IsAsync, IsUnsafe);
}

FunctionTypeExpr::FunctionTypeExpr(EmptyShell, unsigned N)
   : TypeExpr(FunctionTypeExprID, {}, {}), NumArgs(N),
     Thin(false), Throws(false), Async(false), Unsafe(false)
{}

FunctionTypeExpr *FunctionTypeExpr::CreateEmpty(ASTContext &C, unsigned N)
{
    void *Mem = C.Allocate(
       totalSizeToAlloc<SourceType, FunctionType::ParamInfo>(N, N),
          alignof(FunctionTypeExpr));

    return new(Mem) FunctionTypeExpr(EmptyShell(), N);
}

ArrayTypeExpr::ArrayTypeExpr(SourceRange SR,
                             SourceType ElementTy,
                             StaticExpr *SizeExpr,
                             bool IsMeta)
   : TypeExpr(ArrayTypeExprID, SR, IsMeta),
     ElementTy(ElementTy), SizeExpr(SizeExpr)
{

}

ArrayTypeExpr::ArrayTypeExpr(EmptyShell)
   : TypeExpr(ArrayTypeExprID, {}, {}),
     SizeExpr(nullptr)
{}

DeclTypeExpr::DeclTypeExpr(SourceRange SR, Expression *TyExpr, bool IsMeta)
   : TypeExpr(DeclTypeExprID, SR, IsMeta), TyExpr(TyExpr)
{

}

DeclTypeExpr::DeclTypeExpr(EmptyShell)
   : TypeExpr(DeclTypeExprID, {}, {}),
     TyExpr(nullptr)
{}

DeclTypeExpr* DeclTypeExpr::Create(ASTContext &C,
                                   SourceRange SR,
                                   Expression *TyExpr,
                                   bool IsMeta) {
   return new(C) DeclTypeExpr(SR, TyExpr, IsMeta);
}

ArrayTypeExpr* ArrayTypeExpr::Create(ASTContext &C,
                                     SourceRange SR,
                                     SourceType ElementTy,
                                     StaticExpr *SizeExpr,
                                     bool IsMeta) {
   return new(C) ArrayTypeExpr(SR, ElementTy, SizeExpr, IsMeta);
}

PointerTypeExpr::PointerTypeExpr(SourceRange SR,
                                 SourceType SubType,
                                 bool IsMeta)
   : TypeExpr(PointerTypeExprID, SR, IsMeta), SubType(SubType)
{

}

PointerTypeExpr::PointerTypeExpr(EmptyShell)
   : TypeExpr(PointerTypeExprID, {}, {})
{}

PointerTypeExpr* PointerTypeExpr::Create(ASTContext &C,
                                         SourceRange SR,
                                         SourceType SubType,
                                         bool IsMeta) {
   return new(C) PointerTypeExpr(SR, SubType, IsMeta);
}

ReferenceTypeExpr::ReferenceTypeExpr(SourceRange SR,
                                     SourceType SubType,
                                     bool IsMeta)
   : TypeExpr(ReferenceTypeExprID, SR, IsMeta),
     SubType(SubType)
{

}

ReferenceTypeExpr::ReferenceTypeExpr(EmptyShell)
   : TypeExpr(ReferenceTypeExprID, {}, {})
{}

ReferenceTypeExpr* ReferenceTypeExpr::Create(ASTContext &C,
                                             SourceRange SR,
                                             SourceType SubType,
                                             bool IsMeta) {
   return new(C) ReferenceTypeExpr(SR, SubType, IsMeta);
}

OptionTypeExpr::OptionTypeExpr(SourceRange SR,
                               SourceType SubType,
                               bool IsMeta)
   : TypeExpr(OptionTypeExprID, SR, IsMeta),
     SubType(SubType)
{

}

OptionTypeExpr::OptionTypeExpr(EmptyShell)
   : TypeExpr(OptionTypeExprID, {}, {})
{}

OptionTypeExpr* OptionTypeExpr::Create(ASTContext &C,
                                       SourceRange SR,
                                       SourceType SubType,
                                       bool IsMeta) {
   return new(C) OptionTypeExpr(SR, SubType, IsMeta);
}


AttributedExpr::AttributedExpr(Expression *Expr,
                               llvm::ArrayRef<Attr *> Attrs)
   : Expression(AttributedExprID),
     Expr(Expr), NumAttrs((unsigned)Attrs.size())
{
   std::copy(Attrs.begin(), Attrs.end(), getTrailingObjects<Attr*>());
}

AttributedExpr* AttributedExpr::Create(ASTContext &Ctx,
                                       Expression *Expr,
                                       llvm::ArrayRef<Attr *> Attrs) {
   void *Mem = Ctx.Allocate(totalSizeToAlloc<Attr*>(Attrs.size()),
                            alignof(AttributedExpr));

   return new(Mem) AttributedExpr(Expr, Attrs);
}

AttributedExpr::AttributedExpr(EmptyShell, unsigned N)
   : Expression(AttributedExprID), NumAttrs(N)
{}

AttributedExpr *AttributedExpr::CreateEmpty(ASTContext &C, unsigned N)
{
    void *Mem = C.Allocate(totalSizeToAlloc<Attr*>(N), alignof(AttributedExpr));
    return new(Mem) AttributedExpr(EmptyShell(), N);
}

StaticExpr::StaticExpr(Expression* expr)
   : Expression(StaticExprID),
     Loc(expr ? expr->getSourceRange() : SourceRange()),
     expr(expr), evaluatedExpr(nullptr)
{

}

StaticExpr::StaticExpr(EmptyShell)
   : Expression(StaticExprID),
     evaluatedExpr(nullptr)
{}

StaticExpr::StaticExpr(QualType Type, SourceRange SR, il::Constant *V)
   : Expression(StaticExprID),
     Loc(SR), expr(nullptr), evaluatedExpr(V)
{
   exprType = Type;
}

StaticExpr* StaticExpr::Create(ASTContext &C, Expression *Expr)
{
   return new(C) StaticExpr(Expr);
}

StaticExpr* StaticExpr::Create(ASTContext &C,
                               QualType Type,
                               SourceRange SR,
                               il::Constant *V) {
   return new(C) StaticExpr(Type, SR, V);
}

MixinExpr::MixinExpr(SourceRange Parens, Expression *Expr)
   : Expression(MixinExprID), Parens(Parens), Expr(Expr)
{

}

MixinExpr::MixinExpr(EmptyShell)
   : Expression(MixinExprID),
     Expr(nullptr)
{}

MixinExpr* MixinExpr::Create(ASTContext &C,
                             SourceRange Parens,
                             Expression *Expr) {
   return new(C) MixinExpr(Parens, Expr);
}

SequenceElement::SequenceElement(op::OperatorKind opKind,
                                 SourceLocation loc)
   : operatorKind(opKind), kind(EF_Operator), loc(loc)
{

}

SequenceElement::SequenceElement(IdentifierInfo *possibleOp,
                                 SourceLocation loc)
   : op(possibleOp), kind(EF_PossibleOperator), loc(loc)
{

}

SequenceElement::SequenceElement(Expression* expr)
   : expr(expr), kind(EF_Expression)
{
   if (expr)
      loc = expr->getSourceLoc();
}

SequenceElement::SequenceElement(SequenceElement &&other) noexcept
{
   kind = other.kind;
   loc = other.loc;

   if (kind == EF_PossibleOperator) {
      op = other.op;
   }
   else if (kind == EF_Expression) {
      expr = other.expr;
   }
   else {
      operatorKind = other.operatorKind;
   }
}

SequenceElement& SequenceElement::operator=(SequenceElement &&other) noexcept
{
   kind = other.kind;
   loc = other.loc;

   if (kind == EF_PossibleOperator) {
      op = other.op;
   }
   else if (kind == EF_Expression) {
      expr = other.expr;
   }
   else {
      operatorKind = other.operatorKind;
   }

   return *this;
}

template <unsigned StrLen>
static constexpr unsigned strLen(const char (&Str)[StrLen])
{
   return StrLen;
}

SourceLocation SequenceElement::getEndLoc() const
{
   switch (kind) {
   case EF_PossibleOperator:
      return SourceLocation(loc.getOffset() + getOp()->getLength());
   case EF_Expression:
      return expr->getSourceRange().getEnd();
   case EF_Operator: {
      auto offset = loc.getOffset();
      switch (operatorKind) {
#  define CDOT_OPERATOR(Name, Symbol, PG, Fix)           \
      case op::Name: offset += strLen(Symbol); break;
#  include "Basic/BuiltinOperators.def"
      }

      return SourceLocation(offset);
   }
   }
}

ExprSequence::ExprSequence(llvm::MutableArrayRef<SequenceElement> fragments)
   : Expression(ExprSequenceID),
     NumFragments((unsigned)fragments.size())
{
   auto ptr = getTrailingObjects<SequenceElement>();
   for (auto &F : fragments) {
      new (ptr) SequenceElement(std::move(F));
      ++ptr;
   }
}

ExprSequence*
ExprSequence::Create(ASTContext &C,
                     llvm::MutableArrayRef<SequenceElement> fragments) {
   void *Mem = C.Allocate(totalSizeToAlloc<SequenceElement>(fragments.size()),
                          alignof(ExprSequence));

   return new (Mem) ExprSequence(fragments);
}

ExprSequence::ExprSequence(EmptyShell, unsigned N)
   : Expression(ExprSequenceID), NumFragments(N)
{}

ExprSequence *ExprSequence::CreateEmpty(ASTContext &C, unsigned N)
{
    void *Mem = C.Allocate(totalSizeToAlloc<SequenceElement>(N), alignof(ExprSequence));
    return new(Mem) ExprSequence(EmptyShell(), N);
}

SourceRange ExprSequence::getSourceRange() const
{
   auto frags = getFragments();
   if (frags.empty())
      return SourceRange();

   return { frags.front().getLoc(), frags.back().getEndLoc() };
}

UnaryOperator::UnaryOperator(SourceLocation operatorLoc,
                             op::OperatorKind opKind,
                             FunctionType *FuncTy,
                             Expression* target,
                             bool prefix)
   : Expression(UnaryOperatorID),
     operatorLoc(operatorLoc), kind(opKind), FuncTy(FuncTy), target(target),
     prefix(prefix)
{}

UnaryOperator::UnaryOperator(EmptyShell)
   : Expression(UnaryOperatorID)
{}

UnaryOperator* UnaryOperator::Create(ASTContext &C,
                                     SourceLocation operatorLoc,
                                     op::OperatorKind opKind,
                                     FunctionType *FuncTy,
                                     Expression *target, bool prefix) {
   return new (C) UnaryOperator(operatorLoc, opKind, FuncTy, target, prefix);
}

SourceRange UnaryOperator::getSourceRange() const
{
   return SourceRange(operatorLoc, target->getSourceRange().getEnd());
}

BinaryOperator::BinaryOperator(SourceLocation operatorLoc,
                               op::OperatorKind operatorKind,
                               FunctionType *FuncTy,
                               Expression* lhs,
                               Expression* rhs)
   : Expression(BinaryOperatorID),
     operatorLoc(operatorLoc), kind(operatorKind), FuncTy(FuncTy),
     lhs(lhs), rhs(rhs)
{}

BinaryOperator::BinaryOperator(EmptyShell)
   : Expression(BinaryOperatorID),
     FuncTy(nullptr), lhs(nullptr), rhs(nullptr)
{}

BinaryOperator* BinaryOperator::Create(ASTContext &C,
                                       SourceLocation operatorLoc,
                                       op::OperatorKind operatorKind,
                                       FunctionType *FuncTy,
                                       Expression *lhs, Expression *rhs) {
   return new(C) BinaryOperator(operatorLoc, operatorKind, FuncTy, lhs, rhs);
}

SourceRange BinaryOperator::getSourceRange() const
{
   return SourceRange(lhs->getSourceLoc(), rhs->getSourceRange().getEnd());
}

AssignExpr::AssignExpr(SourceLocation EqualsLoc,
                       Expression *LHS,
                       Expression *RHS,
                       bool IsInitialization)
   : Expression(AssignExprID),
     EqualsLoc(EqualsLoc), LHS(LHS), RHS(RHS),
     IsInitialization(IsInitialization)
{

}

AssignExpr::AssignExpr(EmptyShell)
   : Expression(AssignExprID),
     LHS(nullptr), RHS(nullptr),
     IsInitialization(false)
{}

AssignExpr* AssignExpr::Create(ASTContext &C,
                               SourceLocation EqualsLoc,
                               Expression *LHS,
                               Expression *RHS,
                               bool IsInitialization) {
   return new(C) AssignExpr(EqualsLoc, LHS, RHS, IsInitialization);
}

SourceRange AssignExpr::getSourceRange() const
{
   return SourceRange(LHS->getSourceRange().getStart(),
                      RHS->getSourceRange().getEnd());
}

TypePredicateExpr::TypePredicateExpr(SourceLocation IsLoc,
                                     SourceRange SR,
                                     Expression *LHS,
                                     ConstraintExpr *RHS,
                                     bool Negated)
   : Expression(TypePredicateExprID),
     IsLoc(IsLoc), SR(SR), LHS(LHS), RHS(RHS),
     Result(false), CompileTimeCheck(false), Negated(Negated)
{ }

TypePredicateExpr::TypePredicateExpr(EmptyShell)
   : Expression(TypePredicateExprID),
     LHS(nullptr), RHS(nullptr),
     Result(false), CompileTimeCheck(false), Negated(false)
{}

TypePredicateExpr* TypePredicateExpr::Create(ASTContext &C,
                                             SourceLocation IsLoc,
                                             SourceRange SR,
                                             Expression *LHS,
                                             ConstraintExpr *RHS,
                                             bool Negated) {
   return new(C) TypePredicateExpr(IsLoc, SR, LHS, RHS, Negated);
}

CastExpr::CastExpr(SourceLocation AsLoc,
                   CastStrength strength,
                   Expression *target,
                   SourceType targetType)
   : Expression(CastExprID),
     AsLoc(AsLoc), strength(strength), target(target), targetType(targetType)
{}

CastExpr::CastExpr(EmptyShell)
   : Expression(CastExprID),
     target(nullptr)
{}

CastExpr *CastExpr::Create(ASTContext &C,
                           SourceLocation AsLoc,
                           CastStrength strength,
                           Expression *target,
                           SourceType targetType) {
   return new(C) CastExpr(AsLoc, strength, target, targetType);
}

SourceRange CastExpr::getSourceRange() const
{
   SourceLocation Start = target->getSourceLoc();
   SourceLocation End;

   if (auto E = targetType.getTypeExpr()) {
      E->getSourceRange().getEnd();
   }
   else {
      End = Start;
   }

   return SourceRange(Start, End);
}

IfExpr::IfExpr(SourceLocation IfLoc,
               Expression *Cond,
               Expression *TrueVal, Expression *FalseVal)
   : Expression(IfExprID),
     IfLoc(IfLoc), Cond(Cond), TrueVal(TrueVal), FalseVal(FalseVal)
{}

IfExpr::IfExpr(EmptyShell)
   : Expression(IfExprID),
     Cond(nullptr), TrueVal(nullptr), FalseVal(nullptr)
{}

IfExpr* IfExpr::Create(ASTContext &C,
                       SourceLocation IfLoc,
                       Expression *Cond,
                       Expression *TrueVal, Expression *FalseVal) {
   return new(C) IfExpr(IfLoc, Cond, TrueVal, FalseVal);
}

TryExpr::TryExpr(SourceLocation TryLoc, TryKind Kind, Expression *E)
   : Expression(TryExprID),
   TryLoc(TryLoc), Kind(Kind), E(E)
{

}

TryExpr::TryExpr(EmptyShell)
   : Expression(TryExprID),
     E(nullptr)
{}

TryExpr* TryExpr::Create(ASTContext &C,
                         SourceLocation TryLoc,
                         TryKind Kind,
                         Expression *E) {
   return new(C) TryExpr(TryLoc, Kind, E);
}

SourceRange TryExpr::getSourceRange() const
{
   if (!E)
      return SourceRange(TryLoc);

   return SourceRange(TryLoc, E->getSourceRange().getEnd());
}

AwaitExpr::AwaitExpr(SourceLocation AwaitLoc, Expression *E)
   : Expression(AwaitExprID),
     AwaitLoc(AwaitLoc), E(E)
{

}

AwaitExpr* AwaitExpr::Create(ASTContext &C,
                             SourceLocation AwaitLoc,
                             Expression *E) {
   return new(C) AwaitExpr(AwaitLoc, E);
}

SourceRange AwaitExpr::getSourceRange() const
{
   if (!E)
      return SourceRange(AwaitLoc);

   return SourceRange(AwaitLoc, E->getSourceRange().getEnd());
}

AwaitExpr::AwaitExpr(EmptyShell)
   : Expression(AwaitExprID),
     E(nullptr)
{

}

ExpressionPattern::ExpressionPattern(SourceLocation ColonLoc, Expression* expr)
   : PatternExpr(ExpressionPatternID, ColonLoc),
     expr(expr)
{}

ExpressionPattern::ExpressionPattern(EmptyShell)
   : PatternExpr(ExpressionPatternID, {}),
     expr(nullptr)
{}

ExpressionPattern* ExpressionPattern::Create(ASTContext &C,
                                             SourceLocation ColonLoc,
                                             Expression *E) {
   return new(C) ExpressionPattern(ColonLoc, E);
}

CasePattern::CasePattern(SourceRange SR,
                         enum Kind Kind,
                         Expression *ParentExpr,
                         IdentifierInfo *caseName,
                         MutableArrayRef<IfCondition> args)
   : PatternExpr(CasePatternID, SR.getEnd()),
     SR(SR), K(Kind), ParentExpr(ParentExpr), caseName(caseName),
     NumArgs((unsigned)args.size()), HasBinding(false), HasExpr(false),
     LeadingDot(false)
{
   std::copy(args.begin(), args.end(), getTrailingObjects<IfCondition>());
}

CasePattern*
CasePattern::Create(ASTContext &C,
                    SourceRange SR,
                    enum Kind Kind,
                    Expression *ParentExpr,
                    IdentifierInfo *caseName,
                    MutableArrayRef<IfCondition> args) {
   void *Mem = C.Allocate(totalSizeToAlloc<IfCondition>(args.size()),
                          alignof(CasePattern));

   return new(Mem) CasePattern(SR, Kind, ParentExpr, caseName, args);
}

CasePattern::CasePattern(EmptyShell, unsigned N)
   : PatternExpr(CasePatternID, {}),
     ParentExpr(nullptr), NumArgs(N),
     HasBinding(false), HasExpr(false), LeadingDot(false)
{}

CasePattern *CasePattern::CreateEmpty(ASTContext &C, unsigned N)
{
    void *Mem = C.Allocate(totalSizeToAlloc<IfCondition>(N),
                           alignof(CasePattern));
    return new(Mem) CasePattern(EmptyShell(), N);
}

IsPattern::IsPattern(SourceRange SR, SourceType isType)
   : PatternExpr(IsPatternID, SR.getEnd()),
     IsLoc(SR.getStart()), isType(isType)
{}

IsPattern::IsPattern(EmptyShell)
   : PatternExpr(IsPatternID, {})
{}

IsPattern* IsPattern::Create(ASTContext &C,
                             SourceRange SR,
                             SourceType isType) {
   return new(C) IsPattern(SR, isType);
}

void APNumericStorage::setIntValue(const ASTContext &C,
                                   const llvm::APInt &Val) {
   if (hasAllocation()) {
      C.Deallocate(pVal);
   }

   BitWidth = Val.getBitWidth();
   unsigned NumWords = Val.getNumWords();
   const uint64_t* Words = Val.getRawData();

   if (NumWords > 1) {
      pVal = new (C) uint64_t[NumWords];
      std::copy(Words, Words + NumWords, pVal);
   }
   else if (NumWords == 1) {
      VAL = Words[0];
   }
   else {
      VAL = 0;
   }
}

void APNumericStorage::setIntValue(const ASTContext &C,
                                   const llvm::APSInt &Val) {
   setIntValue(C, static_cast<const llvm::APInt&>(Val));
   Unsigned = Val.isUnsigned();
}

IntegerLiteral::IntegerLiteral(SourceRange Loc,
                               QualType type,
                               Suffix suffix)
   : Expression(IntegerLiteralID),
      Loc(Loc), type(type), suffix(suffix)
{

}

IntegerLiteral::IntegerLiteral(EmptyShell)
   : Expression(IntegerLiteralID)
{}

IntegerLiteral *IntegerLiteral::Create(ASTContext &C,
                                       SourceRange Loc,
                                       QualType type,
                                       const llvm::APSInt &value,
                                       Suffix suffix) {
   auto *I = new(C) IntegerLiteral(Loc, type, suffix);
   I->setValue(C, value);

   return I;
}

FPLiteral::FPLiteral(SourceRange Loc,
                     QualType type,
                     Suffix suffix = Suffix::None)
   : Expression(FPLiteralID),
      Loc(Loc), type(type), suffix(suffix)
{}

FPLiteral::FPLiteral(EmptyShell)
   : Expression(FPLiteralID)
{}

FPLiteral* FPLiteral::Create(ASTContext &C,
                             SourceRange Loc,
                             QualType type,
                             const llvm::APFloat &value,
                             Suffix suffix) {
   auto *FP = new(C) FPLiteral(Loc, type, suffix);
   FP->setValue(C, value);

   return FP;
}

BoolLiteral::BoolLiteral(SourceLocation Loc, QualType type, bool value)
   : Expression(BoolLiteralID),
     Loc(Loc), value(value), type(type)
{

}

BoolLiteral::BoolLiteral(EmptyShell)
   : Expression(BoolLiteralID),
     value(false)
{}

BoolLiteral* BoolLiteral::Create(ASTContext &C,
                                 SourceLocation Loc,
                                 QualType type, bool value) {
   return new(C) BoolLiteral(Loc, type, value);
}

CharLiteral::CharLiteral(SourceRange Loc, QualType type, char value)
   : Expression(CharLiteralID),
     Loc(Loc), narrow(value), IsWide(false), type(type)
{

}

CharLiteral::CharLiteral(SourceRange Loc, QualType type, uint32_t value)
   : Expression(CharLiteralID),
     Loc(Loc), wide(value), IsWide(true), type(type)
{

}

CharLiteral::CharLiteral(EmptyShell)
   : Expression(CharLiteralID),
     narrow(0)
{}

CharLiteral* CharLiteral::Create(ASTContext &C,
                                 SourceRange Loc, QualType type,
                                 char value) {
   return new(C) CharLiteral(Loc, type, value);
}

CharLiteral* CharLiteral::Create(ASTContext &C,
                                 SourceRange Loc, QualType type,
                                 uint32_t value) {
   return new(C) CharLiteral(Loc, type, value);
}

NoneLiteral::NoneLiteral(SourceLocation Loc)
   : Expression(NoneLiteralID),
     Loc(Loc)
{}

NoneLiteral::NoneLiteral(EmptyShell)
   : Expression(NoneLiteralID)
{}

NoneLiteral* NoneLiteral::Create(ASTContext &C,
                                 SourceLocation Loc) {
   return new(C) NoneLiteral(Loc);
}

StringLiteral::StringLiteral(SourceRange Loc, std::string &&str)
   : Expression(StringLiteralID),
     Loc(Loc), value(move(str))
{}

StringLiteral::StringLiteral(EmptyShell)
   : Expression(StringLiteralID)
{}

StringLiteral* StringLiteral::Create(ASTContext &C,
                                     SourceRange Loc,
                                     std::string &&str) {
   return new(C) StringLiteral(Loc, move(str));
}

StringInterpolation::StringInterpolation(SourceRange Loc,
                                         llvm::ArrayRef<Expression*> S)
   : Expression(StringInterpolationID),
     Loc(Loc), NumSegments((unsigned)S.size())
{
   std::copy(S.begin(), S.end(), getTrailingObjects<Expression*>());
}

StringInterpolation*
StringInterpolation::Create(ASTContext &C,
                            SourceRange Loc,
                            llvm::ArrayRef<Expression *> strings) {
   void *Mem =C.Allocate(totalSizeToAlloc<Expression*>(strings.size()),
                         alignof(StringInterpolation));

   return new(Mem) StringInterpolation(Loc, strings);
}

StringInterpolation::StringInterpolation(EmptyShell, unsigned N)
   : Expression(StringInterpolationID), NumSegments(N)
{}

StringInterpolation *StringInterpolation::CreateEmpty(ASTContext &C, unsigned N)
{
    void *Mem = C.Allocate(totalSizeToAlloc<Expression*>(N),
                           alignof(StringInterpolation));
    return new(Mem) StringInterpolation(EmptyShell(), N);
}

LambdaExpr::LambdaExpr(SourceRange Parens,
                       SourceLocation ArrowLoc,
                       SourceType returnType,
                       llvm::ArrayRef<FuncArgDecl* > args,
                       Statement* body)
   : Expression(LambdaExprID),
     Parens(Parens), ArrowLoc(ArrowLoc), NumArgs((unsigned)args.size()),
     returnType(returnType), body(body)
{
   std::copy(args.begin(), args.end(), getTrailingObjects<FuncArgDecl*>());
}

LambdaExpr* LambdaExpr::Create(ASTContext &C,
                               SourceRange Parens,
                               SourceLocation ArrowLoc,
                               SourceType returnType,
                               llvm::ArrayRef<FuncArgDecl *> args,
                               Statement *body) {
   void *Mem = C.Allocate(totalSizeToAlloc<FuncArgDecl*>(args.size()),
                          alignof(LambdaExpr));

   return new(Mem) LambdaExpr(Parens, ArrowLoc, returnType, args, body);
}

LambdaExpr::LambdaExpr(EmptyShell, unsigned N)
   : Expression(LambdaExprID), NumArgs(N)
{}

LambdaExpr *LambdaExpr::CreateEmpty(ASTContext &C, unsigned N)
{
    void *Mem = C.Allocate(totalSizeToAlloc<FuncArgDecl*>(N),
                           alignof(LambdaExpr));
    return new(Mem) LambdaExpr(EmptyShell(), N);
}

SourceRange LambdaExpr::getSourceRange() const
{
   return SourceRange(Parens.getStart(),
                      body ? body->getSourceRange().getEnd() : Parens.getEnd());
}

size_t LambdaExpr::addCapture(ASTContext &C, NamedDecl *ND)
{
   size_t i = 0;
   for (auto &capt : captures) {
      if (capt == ND)
         return i;

      ++i;
   }

   captures.push_back(ND, C);
   return captures.size() - 1;
}

TupleLiteral::TupleLiteral(SourceRange Parens,
                           llvm::ArrayRef<Expression *> Exprs)
   : Expression(TupleLiteralID),
     Parens(Parens), NumElements((unsigned)Exprs.size())
{
   std::copy(Exprs.begin(), Exprs.end(), getTrailingObjects<Expression*>());
}

TupleLiteral* TupleLiteral::Create(ASTContext &C,
                                   SourceRange Parens,
                                   llvm::ArrayRef<Expression *> Exprs) {
   void *Mem = C.Allocate(totalSizeToAlloc<Expression*>(Exprs.size()),
                          alignof(TupleLiteral));

   return new(Mem) TupleLiteral(Parens, Exprs);
}

TupleLiteral::TupleLiteral(EmptyShell, unsigned N)
   : Expression(TupleLiteralID), NumElements(N)
{}

TupleLiteral *TupleLiteral::CreateEmpty(ASTContext &C, unsigned N)
{
    void *Mem = C.Allocate(totalSizeToAlloc<Expression*>(N),
                           alignof(TupleLiteral));
    return new(Mem) TupleLiteral(EmptyShell(), N);
}

ArrayLiteral::ArrayLiteral(SourceRange SquareRange,
                           llvm::ArrayRef<Expression *> exprs)
   : Expression(ArrayLiteralID),
     SquareRange(SquareRange),
     numElements(unsigned(exprs.size()))
{
   std::copy(exprs.begin(), exprs.end(), getTrailingObjects<Expression*>());
}

ArrayLiteral* ArrayLiteral::Create(ASTContext &ASTCtx,
                                   SourceRange SquareRange,
                                   llvm::ArrayRef<Expression *> exprs) {
   void *Mem = ASTCtx.Allocate(totalSizeToAlloc<Expression*>(exprs.size()),
                               alignof(ArrayLiteral));

   return new(Mem) ArrayLiteral(SquareRange, exprs);
}

ArrayLiteral::ArrayLiteral(EmptyShell, unsigned N)
   : Expression(ArrayLiteralID), numElements(N)
{}

ArrayLiteral *ArrayLiteral::CreateEmpty(ASTContext &C, unsigned N)
{
    void *Mem = C.Allocate(totalSizeToAlloc<Expression*>(N),
                           alignof(ArrayLiteral));
    return new(Mem) ArrayLiteral(EmptyShell(), N);
}

DictionaryLiteral::DictionaryLiteral(SourceRange SquareRange,
                                     llvm::ArrayRef<Expression *> keys,
                                     llvm::ArrayRef<Expression *> values)
   : Expression(DictionaryLiteralID),
     SquareRange(SquareRange),
     numKeyValuePairs(unsigned(keys.size()))
{
   assert(keys.size() == values.size());
   std::copy(keys.begin(), keys.end(), getTrailingObjects<Expression*>());
   std::copy(values.begin(), values.end(),
             getTrailingObjects<Expression*>() + size());
}

DictionaryLiteral* DictionaryLiteral::Create(ASTContext &ASTCtx,
                                             SourceRange SquareRange,
                                             llvm::ArrayRef<Expression*> keys,
                                             llvm::ArrayRef<Expression*> vals) {
   void *Mem = ASTCtx.Allocate(totalSizeToAlloc<Expression*>(keys.size() * 2),
                               alignof(DictionaryLiteral));

   return new(Mem) DictionaryLiteral(SquareRange, keys, vals);
}

DictionaryLiteral::DictionaryLiteral(EmptyShell, unsigned N)
   : Expression(DictionaryLiteralID), numKeyValuePairs(N)
{}

DictionaryLiteral *DictionaryLiteral::CreateEmpty(ASTContext &C, unsigned N)
{
    void *Mem = C.Allocate(totalSizeToAlloc<Expression*>(N),
                           alignof(DictionaryLiteral));
    return new(Mem) DictionaryLiteral(EmptyShell(), N);
}

IdentifierRefExpr::IdentifierRefExpr(SourceRange Loc,
                                     DeclarationName Name,
                                     DeclContext *DeclCtx,
                                     bool InTypePos)
   : IdentifiedExpr(IdentifierRefExprID, Name),
     Loc(Loc), DeclCtx(DeclCtx), staticLookup(false),
     pointerAccess(false), FoundResult(false), InTypePosition(InTypePos),
     IsSynthesized(false), IsCapture(false), IsSelf(false),
     AllowIncompleteTemplateArgs(false), AllowNamespaceRef(false),
     LeadingDot(false), IssueDiag(true)
{
   IdentifierInfo *II = nullptr;
   if (Name.isSimpleIdentifier())
      II = Name.getIdentifierInfo();
   else if (Name.getKind() == DeclarationName::LocalVarName)
      II = Name.getLocalVarName().getIdentifierInfo();
   else if (Name.getKind() == DeclarationName::PackExpansionName)
      II = Name.getPackExpansionName().getIdentifierInfo();

   if (II && !Loc.getEnd())
      this->Loc = SourceRange(Loc.getStart(),
                              SourceLocation(Loc.getStart().getOffset()
                                             + II->getIdentifier().size() - 1));
}

IdentifierRefExpr::IdentifierRefExpr(SourceRange Loc,
                                     Expression *ParentExpr,
                                     DeclarationName Name,
                                     bool IsPointerAccess)
   : IdentifiedExpr(IdentifierRefExprID, Name),
     Loc(Loc), ParentExpr(ParentExpr), DeclCtx(nullptr), staticLookup(false),
     pointerAccess(IsPointerAccess), FoundResult(false), InTypePosition(false),
     IsSynthesized(false), IsCapture(false), IsSelf(false),
     AllowIncompleteTemplateArgs(false), AllowNamespaceRef(false),
     LeadingDot(false), IssueDiag(true)
{
   IdentifierInfo *II = nullptr;
   if (Name.isSimpleIdentifier())
      II = Name.getIdentifierInfo();
   else if (Name.getKind() == DeclarationName::LocalVarName)
      II = Name.getLocalVarName().getIdentifierInfo();
   else if (Name.getKind() == DeclarationName::PackExpansionName)
      II = Name.getPackExpansionName().getIdentifierInfo();

   if (II && !Loc.getEnd())
      this->Loc = SourceRange(Loc.getStart(),
                              SourceLocation(Loc.getStart().getOffset()
                                             + II->getIdentifier().size() - 1));
}

IdentifierRefExpr::IdentifierRefExpr(SourceRange Loc, IdentifierKind kind,
                                     QualType exprType)
   : IdentifiedExpr(IdentifierRefExprID, nullptr),
     Loc(Loc), kind(kind), staticLookup(false),
     pointerAccess(false), FoundResult(true), InTypePosition(false),
     IsSynthesized(false), IsCapture(false), IsSelf(false),
     AllowIncompleteTemplateArgs(false), AllowNamespaceRef(false),
     LeadingDot(false), IssueDiag(true)
{
   setSemanticallyChecked(true);
   this->exprType = exprType;
}

IdentifierRefExpr::IdentifierRefExpr(SourceRange Loc,
                                     IdentifierKind kind,
                                     NamedDecl *ND,
                                     QualType exprType)
   : IdentifiedExpr(IdentifierRefExprID, nullptr),
     Loc(Loc), kind(kind), staticLookup(false),
     pointerAccess(false), FoundResult(true), InTypePosition(false),
     IsSynthesized(false), IsCapture(false), IsSelf(false),
     AllowIncompleteTemplateArgs(false), AllowNamespaceRef(false),
     LeadingDot(false), IssueDiag(true)
{
   setSemanticallyChecked(true);
   this->ND = ND;
   this->exprType = exprType;
}

IdentifierRefExpr::IdentifierRefExpr(EmptyShell)
   : IdentifiedExpr(IdentifierRefExprID, nullptr),
     Loc(), kind(IdentifierKind::Unknown), staticLookup(false),
     pointerAccess(false), FoundResult(true), InTypePosition(false),
     IsSynthesized(false), IsCapture(false), IsSelf(false),
     AllowIncompleteTemplateArgs(false), AllowNamespaceRef(false),
     LeadingDot(false), IssueDiag(true)
{}

SourceRange IdentifierRefExpr::getSourceRange() const
{
   if (!ParentExpr)
      return Loc;

   return SourceRange(ParentExpr->getSourceRange().getStart(), Loc.getEnd());
}

SelfExpr::SelfExpr(SourceLocation Loc, bool Uppercase)
   : Expression(SelfExprID), Loc(Loc), Uppercase(Uppercase)
{}

SelfExpr::SelfExpr(EmptyShell)
   : Expression(SelfExprID)
{}

SelfExpr* SelfExpr::Create(ASTContext &C, SourceLocation Loc, bool Uppercase)
{
   return new(C) SelfExpr(Loc, Uppercase);
}

SuperExpr::SuperExpr(SourceLocation Loc)
   : Expression(SuperExprID), Loc(Loc)
{}

SuperExpr::SuperExpr(EmptyShell)
   : Expression(SuperExprID)
{}

SuperExpr* SuperExpr::Create(ASTContext &C, SourceLocation Loc)
{
   return new(C) SuperExpr(Loc);
}

BuiltinIdentExpr::BuiltinIdentExpr(SourceLocation Loc,
                                   BuiltinIdentifier identifier)
   : Expression(BuiltinIdentExprID),
     Loc(Loc), identifierKind(identifier)
{}

BuiltinIdentExpr::BuiltinIdentExpr(EmptyShell)
   : Expression(BuiltinIdentExprID),
     identifierKind((BuiltinIdentifier)0)
{}

BuiltinIdentExpr* BuiltinIdentExpr::Create(ASTContext &C,
                                           SourceLocation Loc,
                                           BuiltinIdentifier identifier) {
   return new(C) BuiltinIdentExpr(Loc, identifier);
}

BuiltinExpr::BuiltinExpr(QualType type)
   : Expression(BuiltinExprID)
{
   exprType = type;
}

BuiltinExpr::BuiltinExpr(EmptyShell)
   : Expression(BuiltinExprID)
{}

BuiltinExpr* BuiltinExpr::Create(ASTContext &C, QualType Ty)
{
   return new(C) BuiltinExpr(Ty);
}

BuiltinExpr BuiltinExpr::CreateTemp(QualType Ty)
{
   alignas(alignof(BuiltinExpr)) BuiltinExpr Expr(Ty);
   return Expr;
}

TupleMemberExpr::TupleMemberExpr(SourceLocation Loc,
                                 Expression *ParentExpr,
                                 unsigned Index,
                                 bool PointerAccess)
   : Expression(TupleMemberExprID),
     Loc(Loc), ParentExpr(ParentExpr), Index(Index),
     PointerAccess(PointerAccess)
{

}

TupleMemberExpr::TupleMemberExpr(EmptyShell)
   : Expression(TupleMemberExprID),
     ParentExpr(nullptr), Index(0),
     PointerAccess(false)
{}

EnumCaseExpr::EnumCaseExpr(SourceLocation PeriodLoc,
                           IdentifierInfo *caseName,
                           ASTVector<Expression*> &&args)
   : IdentifiedExpr(EnumCaseExprID, caseName),
     PeriodLoc(PeriodLoc), args(std::move(args)), Case(nullptr)
{}

EnumCaseExpr::EnumCaseExpr(SourceLocation PeriodLoc,
                           EnumCaseDecl *Case,
                           ASTVector<Expression*> &&args)
   : IdentifiedExpr(EnumCaseExprID, Case->getDeclName()),
     PeriodLoc(PeriodLoc), args(std::move(args)), Case(Case)
{

}

EnumCaseExpr::EnumCaseExpr(EmptyShell)
   : IdentifiedExpr(EnumCaseExprID, nullptr),
     Case(nullptr)
{}

SourceRange EnumCaseExpr::getSourceRange() const
{
   unsigned Length;
   if (Case) {
      Length = Case->getDeclName().getIdentifierInfo()->getLength();
   }
   else {
      Length = getIdentInfo()->getLength();
   }

   return SourceRange(PeriodLoc,
                      SourceLocation(PeriodLoc.getOffset()
                                     + Length));
}

EnumDecl* EnumCaseExpr::getEnum() const
{
   if (!Case)
      return nullptr;

   return cast<EnumDecl>(Case->getRecord());
}

CallExpr::CallExpr(SourceLocation IdentLoc, SourceRange ParenRange,
                   ASTVector<Expression* > &&args,
                   ArrayRef<IdentifierInfo*> Labels,
                   DeclarationName Name,
                   bool IsDotInit,
                   bool IsDotDeinit)
   : Expression(CallExprID),
     IdentLoc(IdentLoc), ParenRange(ParenRange),
     ident(Name), NumLabels((unsigned)Labels.size()),
     args(std::move(args)),
     PointerAccess(false), IsUFCS(false), IsDotInit(IsDotInit),
     IsDotDeinit(IsDotDeinit), IncludesSelf(false), DirectCall(false),
     LeadingDot(false), BuiltinKind(0)
{
   std::copy(Labels.begin(), Labels.end(),
             getTrailingObjects<IdentifierInfo*>());
}

CallExpr* CallExpr::Create(ASTContext &C,
                           SourceLocation IdentLoc,
                           SourceRange ParenRange,
                           ASTVector<Expression *> &&args,
                           ArrayRef<IdentifierInfo *> Labels,
                           DeclarationName Name,
                           bool IsDotInit, bool IsDotDeinit) {
   void *Mem = C.Allocate(totalSizeToAlloc<IdentifierInfo*>(Labels.size()),
                          alignof(CallExpr));

   return new(Mem) CallExpr(IdentLoc, ParenRange, std::move(args), Labels, Name,
                            IsDotInit, IsDotDeinit);
}

CallExpr::CallExpr(SourceLocation IdentLoc, SourceRange ParenRange,
                   Expression *ParentExpr,
                   ASTVector<Expression *> &&args,
                   ArrayRef<IdentifierInfo*> Labels,
                   DeclarationName Name,
                   bool IsDotInit,
                   bool IsDotDeinit)
   : Expression(CallExprID),
     IdentLoc(IdentLoc), ParenRange(ParenRange),
     ident(Name),  NumLabels((unsigned)Labels.size()),
     ParentExpr(ParentExpr), args(std::move(args)),
     PointerAccess(false), IsUFCS(false), IsDotInit(IsDotInit),
     IsDotDeinit(IsDotDeinit), IncludesSelf(false), DirectCall(false),
     LeadingDot(false), BuiltinKind(0)
{
   std::copy(Labels.begin(), Labels.end(),
             getTrailingObjects<IdentifierInfo*>());
}

CallExpr* CallExpr::Create(ASTContext &C,
                           SourceLocation IdentLoc,
                           SourceRange ParenRange,
                           Expression *ParentExpr,
                           ASTVector<Expression *> &&args,
                           ArrayRef<IdentifierInfo *> Labels,
                           DeclarationName Name,
                           bool IsDotInit, bool IsDotDeinit) {
   void *Mem = C.Allocate(totalSizeToAlloc<IdentifierInfo*>(Labels.size()),
                          alignof(CallExpr));

   return new(Mem) CallExpr(IdentLoc, ParenRange, ParentExpr,
                            std::move(args), Labels, Name,
                            IsDotInit, IsDotDeinit);
}

CallExpr::CallExpr(SourceLocation IdentLoc, SourceRange ParenRange,
                   ASTVector<Expression *> &&args, CallableDecl *C)
   : Expression(CallExprID),
     IdentLoc(IdentLoc), ParenRange(ParenRange),
     NumLabels(0), args(std::move(args)),
     PointerAccess(false), IsUFCS(false), IsDotInit(false),
     IsDotDeinit(false), IncludesSelf(false), DirectCall(false),
     LeadingDot(false), BuiltinKind(0)
{
   if (auto F = support::dyn_cast<FunctionDecl>(C)) {
      func = F;
      exprType = func->getReturnType();

      kind = CallKind::NamedFunctionCall;
   }
   else {
      auto M = support::cast<MethodDecl>(C);
      func = M;
      exprType = M->getReturnType();

      if (M->isStatic())
         kind = CallKind::StaticMethodCall;
      else
         kind = CallKind::MethodCall;
   }
}

CallExpr *CallExpr::Create(ASTContext &C, SourceLocation IdentLoc,
                           SourceRange ParenRange,
                           ASTVector<Expression *> &&args,
                           CallableDecl *CD) {
   return new(C) CallExpr(IdentLoc, ParenRange, std::move(args), CD);
}

CallExpr::CallExpr(EmptyShell, unsigned N)
   : Expression(CallExprID),
     NumLabels(N), PointerAccess(false), IsUFCS(false), IsDotInit(false),
     IsDotDeinit(false), IncludesSelf(false), DirectCall(false),
     LeadingDot(false), BuiltinKind(0)
{}

CallExpr* CallExpr::CreateEmpty(ASTContext &C, unsigned N)
{
   void *Mem = C.Allocate(totalSizeToAlloc<IdentifierInfo*>(N),
                          alignof(CallExpr));

   return new(Mem) CallExpr(EmptyShell(), N);
}

SourceRange CallExpr::getSourceRange() const
{
   if (!ParentExpr) {
      if (!IdentLoc) {
         return ParenRange;
      }

      return SourceRange(IdentLoc, ParenRange.getEnd());
   }

   return SourceRange(ParentExpr->getSourceRange().getStart(),
                      ParenRange.getEnd());
}

bool CallExpr::isKnownFunctionCall()
{
   if (kind != CallKind::NamedFunctionCall)
      return false;

   return getFunc()->isKnownFunction();
}

AnonymousCallExpr::AnonymousCallExpr(SourceRange ParenRange,
                                     Expression *ParentExpr,
                                     ArrayRef<Expression*> Args,
                                     ArrayRef<IdentifierInfo*> Labels)
   : Expression(AnonymousCallExprID),
     ParenRange(ParenRange), ParentExpr(ParentExpr), FnTy(nullptr),
     NumArgs((unsigned)Args.size()), IsPrimitiveInit(false)
{
   std::copy(Args.begin(), Args.end(), getTrailingObjects<Expression*>());

   std::memset(getTrailingObjects<IdentifierInfo*>(), 0,
               sizeof(IdentifierInfo*) * Args.size());
   std::copy(Labels.begin(), Labels.end(), getTrailingObjects<IdentifierInfo*>());
}

AnonymousCallExpr* AnonymousCallExpr::Create(ASTContext &C,
                                             SourceRange ParenRange,
                                             Expression *ParentExpr,
                                             llvm::ArrayRef<Expression*> Args,
                                             ArrayRef<IdentifierInfo*> Labels) {
   void *Mem = C.Allocate(totalSizeToAlloc<Expression*, IdentifierInfo*>(
                             Args.size(), Args.size()),
                          alignof(AnonymousCallExpr));

   return new(Mem) AnonymousCallExpr(ParenRange, ParentExpr, Args, Labels);
}

AnonymousCallExpr::AnonymousCallExpr(EmptyShell, unsigned N)
   : Expression(AnonymousCallExprID),
     ParentExpr(nullptr), FnTy(nullptr), NumArgs(N)
{}

AnonymousCallExpr *AnonymousCallExpr::CreateEmpty(ASTContext &C, unsigned N)
{
    void *Mem = C.Allocate(totalSizeToAlloc<Expression*, IdentifierInfo*>(N, N),
                           alignof(AnonymousCallExpr));

    return new(Mem) AnonymousCallExpr(EmptyShell(), N);
}

SubscriptExpr::SubscriptExpr(SourceRange SquareRange,
                             Expression *ParentExpr,
                             llvm::ArrayRef<Expression *> indices)
   : Expression(SubscriptExprID),
     SquareRange(SquareRange), ParentExpr(ParentExpr),
     NumIndices((unsigned)indices.size())
{
   std::copy(indices.begin(), indices.end(), getTrailingObjects<Expression*>());
}

SubscriptExpr* SubscriptExpr::Create(ASTContext &C,
                                     SourceRange SquareRange,
                                     Expression *ParentExpr,
                                     llvm::ArrayRef<Expression *> indices) {
   void *Mem = C.Allocate(totalSizeToAlloc<Expression*>(indices.size()),
                          alignof(SubscriptExpr));

   return new(Mem) SubscriptExpr(SquareRange, ParentExpr, indices);
}

SubscriptExpr::SubscriptExpr(EmptyShell, unsigned N)
   : Expression(SubscriptExprID), NumIndices(N)
{}

SubscriptExpr *SubscriptExpr::CreateEmpty(ASTContext &C, unsigned N)
{
    void *Mem = C.Allocate(totalSizeToAlloc<Expression*>(N),
                           alignof(SubscriptExpr));
    return new(Mem) SubscriptExpr(EmptyShell(), N);
}

SourceRange SubscriptExpr::getSourceRange() const
{
   return SourceRange(ParentExpr->getSourceRange().getStart(),
                      SquareRange.getEnd());
}

TemplateArgListExpr::TemplateArgListExpr(SourceRange AngleRange,
                                         Expression *ParentExpr,
                                         llvm::ArrayRef<Expression*> Exprs)
   : Expression(TemplateArgListExprID),
     AngleRange(AngleRange), ParentExpr(ParentExpr),
     NumTemplateArgs((unsigned)Exprs.size())
{
   std::copy(Exprs.begin(), Exprs.end(), getTrailingObjects<Expression*>());
}

TemplateArgListExpr::TemplateArgListExpr(EmptyShell, unsigned N)
   : Expression(TemplateArgListExprID),
     AngleRange(), ParentExpr(nullptr),
     NumTemplateArgs(N)
{

}

TemplateArgListExpr* TemplateArgListExpr::Create(ASTContext &C,
                                                 SourceRange AngleRange,
                                                 Expression *ParentExpr,
                                                 ArrayRef<Expression *> Exprs) {
   void *Mem = C.Allocate(totalSizeToAlloc<Expression*>(Exprs.size()),
                          alignof(TemplateArgListExpr));
   return new(Mem) TemplateArgListExpr(AngleRange, ParentExpr, Exprs);
}

TemplateArgListExpr* TemplateArgListExpr::CreateEmpty(ASTContext &C, unsigned N)
{
   void *Mem = C.Allocate(totalSizeToAlloc<Expression*>(N),
                          alignof(TemplateArgListExpr));

   return new(Mem) TemplateArgListExpr(EmptyShell(), N);
}

SourceRange TemplateArgListExpr::getSourceRange() const
{
   if (ParentExpr)
      return SourceRange(ParentExpr->getSourceLoc(), AngleRange.getEnd());

   return AngleRange;
}

ConstraintExpr::ConstraintExpr(SourceLocation Loc, Kind kind)
   : Expression(ConstraintExprID),
     Loc(Loc), kind(kind), typeConstraint(nullptr)
{}

ConstraintExpr::ConstraintExpr(SourceLocation Loc, SourceType typeConstraint)
   : Expression(ConstraintExprID),
     Loc(Loc), kind(Type), typeConstraint(typeConstraint)
{}

ConstraintExpr::ConstraintExpr(EmptyShell)
   : Expression(ConstraintExprID),
     typeConstraint(nullptr)
{}

ConstraintExpr* ConstraintExpr::Create(ASTContext &C,
                                       SourceLocation Loc,
                                       Kind kind) {
   return new(C) ConstraintExpr(Loc, kind);
}

ConstraintExpr* ConstraintExpr::Create(ASTContext &C,
                                       SourceLocation Loc,
                                       SourceType typeConstraint) {
   return new(C) ConstraintExpr(Loc, typeConstraint);
}

TraitsExpr::TraitsExpr(SourceLocation TraitsLoc, SourceRange Parens,
                       Kind kind,
                       llvm::MutableArrayRef<TraitsArgument> args)
   : Expression(TraitsExprID),
     TraitsLoc(TraitsLoc), Parens(Parens), kind(kind),
     NumArgs((unsigned)args.size())
{
   auto ptr = getTrailingObjects<TraitsArgument>();
   for (auto &Arg : args) {
      new (ptr) TraitsArgument(std::move(Arg));
      ++ptr;
   }
}

TraitsExpr* TraitsExpr::Create(ASTContext &C,
                               SourceLocation TraitsLoc,
                               SourceRange Parens,
                               TraitsExpr::Kind kind,
                               llvm::MutableArrayRef<TraitsArgument> args) {
   void *Mem = C.Allocate(totalSizeToAlloc<TraitsArgument>(args.size()),
                          alignof(TraitsExpr));

   return new(Mem) TraitsExpr(TraitsLoc, Parens, kind, args);
}

TraitsExpr::TraitsExpr(EmptyShell, unsigned N)
   : Expression(TraitsExprID), NumArgs(N)
{}

TraitsExpr *TraitsExpr::CreateEmpty(ASTContext &C, unsigned N)
{
    void *Mem = C.Allocate(totalSizeToAlloc<TraitsArgument>(N),
                           alignof(TraitsExpr));
    return new(Mem) TraitsExpr(EmptyShell(), N);
}

MacroVariableExpr::MacroVariableExpr(Expression *E)
   : Expression(MacroVariableExprID), E(E)
{

}

MacroVariableExpr::MacroVariableExpr(EmptyShell)
   : Expression(MacroVariableExprID),
     E(nullptr)
{}

MacroVariableExpr* MacroVariableExpr::Create(ASTContext &C,
                                             Expression *E) {
   return new(C) MacroVariableExpr(E);
}

MacroExpansionExpr::MacroExpansionExpr(SourceRange SR,
                                       DeclarationName MacroName,
                                       Delimiter Delim,
                                       llvm::ArrayRef<lex::Token> Toks,
                                       Expression *ParentExpr)
   : Expression(MacroExpansionExprID),
     SR(SR), Delim(Delim), MacroName(MacroName),
     NumTokens((unsigned)Toks.size()), ParentExpr(ParentExpr)
{
   std::copy(Toks.begin(), Toks.end(), getTrailingObjects<lex::Token>());
}

MacroExpansionExpr* MacroExpansionExpr::Create(ASTContext &C,
                                               SourceRange SR,
                                               DeclarationName MacroName,
                                               Delimiter Delim,
                                               llvm::ArrayRef<lex::Token> Toks,
                                               Expression *ParentExpr) {
   void *Mem = C.Allocate(totalSizeToAlloc<lex::Token>(Toks.size()),
                          alignof(MacroExpansionExpr));

   return new(Mem) MacroExpansionExpr(SR, MacroName, Delim, Toks, ParentExpr);
}

MacroExpansionExpr::MacroExpansionExpr(EmptyShell, unsigned N)
   : Expression(MacroExpansionExprID), NumTokens(N)
{}

MacroExpansionExpr *MacroExpansionExpr::CreateEmpty(ASTContext &C, unsigned N)
{
    void *Mem = C.Allocate(totalSizeToAlloc<lex::Token>(N),
                           alignof(MacroExpansionExpr));
    return new(Mem) MacroExpansionExpr(EmptyShell(), N);
}

} // namespace ast
} // namespace cdot
