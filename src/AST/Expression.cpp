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

Expression::Expression(NodeType typeID, bool contextDependent)
   : Statement(typeID),
     contextDependent(contextDependent),
     IsLHSOfAssignment(false), IsMagicArgumentValue(false)
{}

SourceRange Expression::getEllipsisRange() const
{
   if (!EllipsisLoc)
      return SourceRange();

   return SourceRange(EllipsisLoc, SourceLocation(EllipsisLoc.getOffset() + 2));
}

Expression* Expression::maybeGetParentExpr() const
{
   switch (typeID) {
   case IdentifierRefExprID:
      return cast<IdentifierRefExpr>(this)->getParentExpr();
   case MemberRefExprID:
      return cast<MemberRefExpr>(this)->getParentExpr();
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
   case MemberRefExprID:
      return cast<MemberRefExpr>(this)->setParentExpr(E);
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
      return cast<ParenExpr>(this)->getParenthesizedExpr()
                                  ->warnOnUnusedResult();
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
         QualType RetTy = Call->getExprType();
         return !RetTy->isVoidType()
                && !RetTy->isUnpopulatedType()
                && !Call->getFunc()->hasAttribute<DiscardableResultAttr>();
      }
      default:
         return !Call->getExprType()->isVoidType()
            && !Call->getExprType()->isUnpopulatedType();
      }
   }
   case NodeType::AnonymousCallExprID: {
      return !exprType->isVoidType() && !exprType->isUnpopulatedType();
   }
   default:
      return true;
   }
}

IdentifiedExpr::IdentifiedExpr(NodeType typeID, DeclarationName Name,
                               bool contextDependent)
   : Expression(typeID, contextDependent),
     DeclName(Name)
{}

ParenExpr::ParenExpr(SourceRange Parens,
                     Expression *Expr)
   : Expression(ParenExprID, Expr->isContextDependent()),
     Parens(Parens), ParenthesizedExpr(Expr)
{
}

ParenExpr* ParenExpr::Create(ASTContext &C,
                             SourceRange Parens,
                             Expression *Expr) {
   return new(C) ParenExpr(Parens, Expr);
}

ImplicitCastExpr::ImplicitCastExpr(Expression* target,
                                   ConversionSequence &&ConvSeq)
   : Expression(ImplicitCastExprID),
     target(target), ConvSeq(std::move(ConvSeq))
{
   assert(this->ConvSeq.isValid() && "invalid conversion sequence");
}

ImplicitCastExpr* ImplicitCastExpr::Create(ASTContext &C,
                                           Expression *target,
                                           ConversionSequence &&ConvSeq) {
   return new(C) ImplicitCastExpr(target, std::move(ConvSeq));
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

FunctionTypeExpr::FunctionTypeExpr(SourceRange SR,
                                   SourceType RetTy,
                                   llvm::ArrayRef<SourceType> Tys,
                                   bool IsMeta)
   : TypeExpr(FunctionTypeExprID, SR, IsMeta), RetTy(RetTy),
     NumArgs((unsigned)Tys.size())
{
   std::copy(Tys.begin(), Tys.end(), getTrailingObjects<SourceType>());
}

FunctionTypeExpr* FunctionTypeExpr::Create(ASTContext &C,
                                           SourceRange SR,
                                           SourceType RetTy,
                                           llvm::ArrayRef<SourceType> Tys,
                                           bool IsMeta) {
   void *Mem = C.Allocate(totalSizeToAlloc<SourceType>(Tys.size()),
                          alignof(FunctionTypeExpr));

   return new(Mem) FunctionTypeExpr(SR, RetTy, Tys, IsMeta);
}

ArrayTypeExpr::ArrayTypeExpr(SourceRange SR,
                             SourceType ElementTy,
                             StaticExpr *SizeExpr,
                             bool IsMeta)
   : TypeExpr(ArrayTypeExprID, SR, IsMeta),
     ElementTy(ElementTy), SizeExpr(SizeExpr)
{

}

DeclTypeExpr::DeclTypeExpr(SourceRange SR, Expression *TyExpr, bool IsMeta)
   : TypeExpr(DeclTypeExprID, SR, IsMeta), TyExpr(TyExpr)
{

}

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

StaticExpr::StaticExpr(Expression* expr)
   : Expression(StaticExprID), expr(expr), evaluatedExpr(nullptr)
{

}

StaticExpr::StaticExpr(QualType Type, il::Constant *V)
   : Expression(StaticExprID), expr(nullptr), evaluatedExpr(V)
{
   exprType = Type;
}

StaticExpr* StaticExpr::Create(ASTContext &C, Expression *Expr)
{
   return new(C) StaticExpr(Expr);
}

StaticExpr* StaticExpr::Create(ASTContext &C,
                               QualType Type,
                               il::Constant *V) {
   return new(C) StaticExpr(Type, V);
}

SourceRange StaticExpr::getSourceRange() const
{
   return expr ? expr->getSourceRange() : SourceRange();
}

MixinExpr::MixinExpr(SourceRange Parens, Expression *Expr)
   : Expression(MixinExprID), Parens(Parens), Expr(Expr)
{

}

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
                                     ConstraintExpr *RHS)
   : Expression(TypePredicateExprID),
     IsLoc(IsLoc), SR(SR), LHS(LHS), RHS(RHS),
     Result(false), CompileTimeCheck(false)
{ }

TypePredicateExpr* TypePredicateExpr::Create(ASTContext &C,
                                             SourceLocation IsLoc,
                                             SourceRange SR,
                                             Expression *LHS,
                                             ConstraintExpr *RHS) {
   return new(C) TypePredicateExpr(IsLoc, SR, LHS, RHS);
}

CastExpr::CastExpr(SourceLocation AsLoc,
                   CastStrength strength,
                   Expression *target,
                   SourceType targetType)
   : Expression(CastExprID),
     AsLoc(AsLoc), strength(strength), target(target), targetType(targetType)
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

IfExpr* IfExpr::Create(ASTContext &C,
                       SourceLocation IfLoc,
                       Expression *Cond,
                       Expression *TrueVal, Expression *FalseVal) {
   return new(C) IfExpr(IfLoc, Cond, TrueVal, FalseVal);
}

ExpressionPattern::ExpressionPattern(SourceLocation ColonLoc, Expression* expr)
   : PatternExpr(ExpressionPatternID, ColonLoc),
     expr(expr)
{}

ExpressionPattern* ExpressionPattern::Create(ASTContext &C,
                                             SourceLocation ColonLoc,
                                             Expression *E) {
   return new(C) ExpressionPattern(ColonLoc, E);
}

CasePattern::CasePattern(SourceRange SR,
                         IdentifierInfo *caseName,
                         llvm::MutableArrayRef<CasePatternArgument> args)
   : PatternExpr(CasePatternID, SR.getEnd()),
     SR(SR), caseName(caseName),
     NumArgs((unsigned)args.size()), HasBinding(false), HasExpr(false)
{
   auto ptr = getTrailingObjects<CasePatternArgument>();
   for (auto &arg : args) {
      HasBinding |= !arg.isExpr();
      HasExpr |= arg.isExpr();

      new (ptr) CasePatternArgument(std::move(arg));
      ++ptr;
   }
}

CasePattern*
CasePattern::Create(ASTContext &C,
                    SourceRange SR,
                    IdentifierInfo *caseName,
                    llvm::MutableArrayRef<CasePatternArgument> args) {
   void *Mem = C.Allocate(totalSizeToAlloc<CasePatternArgument>(args.size()),
                          alignof(CasePattern));

   return new(Mem) CasePattern(SR, caseName, args);
}

IsPattern::IsPattern(SourceRange SR, SourceType isType)
   : PatternExpr(IsPatternID, SR.getEnd()),
     IsLoc(SR.getStart()), isType(isType)
{}

IsPattern* IsPattern::Create(ASTContext &C,
                             SourceRange SR,
                             SourceType isType) {
   return new(C) IsPattern(SR, isType);
}

IntegerLiteral::IntegerLiteral(SourceRange Loc,
                               QualType type,
                               llvm::APSInt &&value,
                               Suffix suffix)
   :  Expression(IntegerLiteralID, suffix == Suffix::None),
      Loc(Loc), value(std::move(value)), type(type), suffix(suffix)
{}

IntegerLiteral *IntegerLiteral::Create(ASTContext &C,
                                       SourceRange Loc,
                                       QualType type,
                                       llvm::APSInt &&value,
                                       Suffix suffix) {
   return new(C) IntegerLiteral(Loc, type, std::move(value), suffix);
}

FPLiteral::FPLiteral(SourceRange Loc, QualType type,
                     llvm::APFloat &&value,
                     Suffix suffix = Suffix::None)
   :  Expression(FPLiteralID, suffix == Suffix::None),
      Loc(Loc), value(std::move(value)), type(type), suffix(suffix)
{}

FPLiteral* FPLiteral::Create(ASTContext &C, SourceRange Loc,
                             QualType type, llvm::APFloat &&value,
                             Suffix suffix) {
   return new(C) FPLiteral(Loc, type, std::move(value), suffix);
}

BoolLiteral::BoolLiteral(SourceLocation Loc, QualType type, bool value)
   : Expression(BoolLiteralID),
     Loc(Loc), value(value), type(type)
{

}

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

NoneLiteral* NoneLiteral::Create(ASTContext &C,
                                 SourceLocation Loc) {
   return new(C) NoneLiteral(Loc);
}

StringLiteral::StringLiteral(SourceRange Loc, std::string &&str)
   : Expression(StringLiteralID, true),
     Loc(Loc), value(move(str))
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

LambdaExpr::LambdaExpr(SourceRange Parens,
                       SourceLocation ArrowLoc,
                       SourceType returnType,
                       llvm::ArrayRef<FuncArgDecl* > args,
                       Statement* body)
   : Expression(LambdaExprID, true),
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

SourceRange LambdaExpr::getSourceRange() const
{
   return SourceRange(Parens.getStart(),
                      body ? body->getSourceRange().getEnd() : Parens.getEnd());
}

size_t LambdaExpr::addCapture(NamedDecl *ND)
{
   size_t i = 0;
   for (auto &capt : captures) {
      if (capt == ND)
         return i;

      ++i;
   }

   captures.push_back(ND);
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

IdentifierRefExpr::IdentifierRefExpr(SourceRange Loc,
                                     DeclarationName Name,
                                     std::vector<Expression*> &&templateArgs,
                                     DeclContext *DeclCtx,
                                     bool InTypePos)
   : IdentifiedExpr(IdentifierRefExprID, Name),
     Loc(Loc), DeclCtx(DeclCtx), staticLookup(false),
     pointerAccess(false), FoundResult(false), InTypePosition(InTypePos),
     IsSynthesized(false), IsCapture(false),
     AllowIncompleteTemplateArgs(false),
     templateArgs(move(templateArgs))
{
   IdentifierInfo *II = nullptr;
   if (Name.isSimpleIdentifier())
      II = Name.getIdentifierInfo();
   else if (Name.getDeclarationKind() == DeclarationName::LocalVarName)
      II = Name.getLocalVarName().getIdentifierInfo();
   else if (Name.getDeclarationKind() == DeclarationName::PackExpansionName)
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
     IsSynthesized(false), IsCapture(false),
     AllowIncompleteTemplateArgs(false)
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
     IsSynthesized(false), IsCapture(false),
     AllowIncompleteTemplateArgs(false)
{
   setSemanticallyChecked(true);
   this->ND = ND;
   this->exprType = exprType;
}

SelfExpr::SelfExpr(SourceLocation Loc)
   : Expression(SelfExprID), Loc(Loc)
{}

SelfExpr* SelfExpr::Create(ASTContext &C, SourceLocation Loc)
{
   return new(C) SelfExpr(Loc);
}

SuperExpr::SuperExpr(SourceLocation Loc)
   : Expression(SuperExprID), Loc(Loc)
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

BuiltinExpr* BuiltinExpr::Create(ASTContext &C, QualType Ty)
{
   return new(C) BuiltinExpr(Ty);
}

BuiltinExpr BuiltinExpr::CreateTemp(QualType Ty)
{
   alignas(alignof(BuiltinExpr)) BuiltinExpr Expr(Ty);
   return Expr;
}

MemberRefExpr::MemberRefExpr(SourceLocation Loc,
                             DeclarationName Name,
                             bool pointerAccess)
   : IdentifiedExpr(MemberRefExprID, Name),
     Loc(Loc), pointerAccess(pointerAccess)
{

}

MemberRefExpr::MemberRefExpr(SourceLocation Loc,
                             Expression *ParentExpr,
                             DeclarationName Name,
                             bool pointerAccess)
   : IdentifiedExpr(MemberRefExprID, Name),
     Loc(Loc), ParentExpr(ParentExpr),
     pointerAccess(pointerAccess)
{

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

EnumCaseExpr::EnumCaseExpr(SourceLocation PeriodLoc,
                           IdentifierInfo *caseName,
                           std::vector<Expression*> &&args)
   : IdentifiedExpr(EnumCaseExprID, caseName, true),
     PeriodLoc(PeriodLoc), args(move(args)), en(nullptr)
{}

EnumCaseExpr::EnumCaseExpr(SourceLocation PeriodLoc,
                           EnumDecl *E,
                           IdentifierInfo *caseName,
                           std::vector<Expression*> &&args)
   : IdentifiedExpr(EnumCaseExprID, caseName, false),
     PeriodLoc(PeriodLoc), args(move(args)), en(E)
{}

CallExpr::CallExpr(SourceLocation IdentLoc, SourceRange ParenRange,
                   std::vector<Expression* > &&args,
                   DeclarationName Name,
                   bool IsDotInit,
                   bool IsDotDeinit)
   : Expression(CallExprID),
     IdentLoc(IdentLoc), ParenRange(ParenRange),
     ident(Name), args(move(args)),
     PointerAccess(false), IsUFCS(false), IsDotInit(IsDotInit),
     IsDotDeinit(IsDotDeinit), builtinFnKind(BuiltinFn::None)
{

}

CallExpr::CallExpr(SourceLocation IdentLoc, SourceRange ParenRange,
                   Expression *ParentExpr,
                   std::vector<Expression *> &&args,
                   DeclarationName Name,
                   bool IsDotInit,
                   bool IsDotDeinit)
   : Expression(CallExprID),
     IdentLoc(IdentLoc), ParenRange(ParenRange),
     ident(Name), ParentExpr(ParentExpr), args(move(args)),
     PointerAccess(false), IsUFCS(false), IsDotInit(IsDotInit),
     IsDotDeinit(IsDotDeinit), builtinFnKind(BuiltinFn::None)
{

}

CallExpr::CallExpr(SourceLocation IdentLoc, SourceRange ParenRange,
                   std::vector<Expression *> &&args, CallableDecl *C)
   : Expression(CallExprID),
     IdentLoc(IdentLoc), ParenRange(ParenRange),
     args(move(args)),
     PointerAccess(false), IsUFCS(false), IsDotInit(false),
     IsDotDeinit(false), builtinFnKind(BuiltinFn::None)
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

bool CallExpr::isKnownFunctionCall()
{
   if (kind != CallKind::NamedFunctionCall)
      return false;

   return getFunc()->isKnownFunction();
}

AnonymousCallExpr::AnonymousCallExpr(SourceRange ParenRange,
                                     Expression *ParentExpr,
                                     llvm::ArrayRef<Expression*> Args)
   : Expression(AnonymousCallExprID),
     ParenRange(ParenRange), ParentExpr(ParentExpr),
     NumArgs((unsigned)Args.size()), IsPrimitiveInit(false)
{
   std::copy(Args.begin(), Args.end(), getTrailingObjects<Expression*>());
}

AnonymousCallExpr* AnonymousCallExpr::Create(ASTContext &C,
                                             SourceRange ParenRange,
                                             Expression *ParentExpr,
                                             llvm::ArrayRef<Expression*> Args) {
   void *Mem = C.Allocate(totalSizeToAlloc<Expression*>(Args.size()),
                          alignof(AnonymousCallExpr));

   return new(Mem) AnonymousCallExpr(ParenRange, ParentExpr, Args);
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

ConstraintExpr::ConstraintExpr(SourceLocation Loc, Kind kind)
   : Expression(ConstraintExprID),
     Loc(Loc), kind(kind), typeConstraint(nullptr)
{}

ConstraintExpr::ConstraintExpr(SourceLocation Loc, SourceType typeConstraint)
   : Expression(ConstraintExprID),
     Loc(Loc), kind(Type), typeConstraint(typeConstraint)
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

MacroVariableExpr::MacroVariableExpr(Expression *E)
   : Expression(MacroVariableExprID), E(E)
{

}

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

} // namespace ast
} // namespace cdot
