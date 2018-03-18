//
// Created by Jonas Zell on 19.06.17.
//

#include "Expression.h"

#include "AST/ASTContext.h"
#include "NamedDecl.h"
#include "AST/Passes/SemanticAnalysis/Builtin.h"
#include "Support/Casting.h"

using namespace cdot::support;

namespace cdot {
namespace ast {

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
   default:
      break;
   }

   return nullptr;
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

         if (loc)
            break;

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
         llvm_unreachable("TODO!");
      }


      break;
   }

   return false;
}

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
   exprType = this->ConvSeq.getSteps().back().getResultType();
   copyStatusFlags(target);
   setSemanticallyChecked(true);
}

ImplicitCastExpr* ImplicitCastExpr::Create(ASTContext &C,
                                           Expression *target,
                                           ConversionSequence &&ConvSeq) {
   return new(C) ImplicitCastExpr(target, std::move(ConvSeq));
}

TupleTypeExpr::TupleTypeExpr(SourceRange SR,
                             llvm::ArrayRef<SourceType> Tys)
   : TypeExpr(TupleTypeExprID, SR), NumTys((unsigned)Tys.size())
{
   std::copy(Tys.begin(), Tys.end(), getTrailingObjects<SourceType>());
}

TupleTypeExpr* TupleTypeExpr::Create(ASTContext &C,
                                     SourceRange SR,
                                     llvm::ArrayRef<SourceType> Tys) {
   void *Mem = C.Allocate(totalSizeToAlloc<SourceType>(Tys.size()),
                          alignof(TupleTypeExpr));

   return new(Mem) TupleTypeExpr(SR, Tys);
}

FunctionTypeExpr::FunctionTypeExpr(SourceRange SR,
                                   SourceType RetTy,
                                   llvm::ArrayRef<SourceType> Tys)
   : TypeExpr(FunctionTypeExprID, SR), RetTy(RetTy),
     NumArgs((unsigned)Tys.size())
{
   std::copy(Tys.begin(), Tys.end(), getTrailingObjects<SourceType>());
}

FunctionTypeExpr* FunctionTypeExpr::Create(ASTContext &C,
                                           SourceRange SR,
                                           SourceType RetTy,
                                           llvm::ArrayRef<SourceType> Tys) {
   void *Mem = C.Allocate(totalSizeToAlloc<SourceType>(Tys.size()),
                          alignof(FunctionTypeExpr));

   return new(Mem) FunctionTypeExpr(SR, RetTy, Tys);
}

ArrayTypeExpr::ArrayTypeExpr(SourceRange SR,
                             SourceType ElementTy,
                             StaticExpr *SizeExpr)
   : TypeExpr(ArrayTypeExprID, SR), ElementTy(ElementTy), SizeExpr(SizeExpr)
{

}

DeclTypeExpr::DeclTypeExpr(SourceRange SR, Expression *TyExpr)
   : TypeExpr(DeclTypeExprID, SR), TyExpr(TyExpr)
{

}

DeclTypeExpr* DeclTypeExpr::Create(ASTContext &C,
                                   SourceRange SR,
                                   Expression *TyExpr) {
   return new(C) DeclTypeExpr(SR, TyExpr);
}

ArrayTypeExpr* ArrayTypeExpr::Create(ASTContext &C,
                                     SourceRange SR,
                                     SourceType ElementTy,
                                     StaticExpr *SizeExpr) {
   return new(C) ArrayTypeExpr(SR, ElementTy, SizeExpr);
}

PointerTypeExpr::PointerTypeExpr(SourceRange SR,
                                 SourceType SubType)
   : TypeExpr(PointerTypeExprID, SR), SubType(SubType)
{

}

PointerTypeExpr* PointerTypeExpr::Create(ASTContext &C,
                                         SourceRange SR,
                                         SourceType SubType) {
   return new(C) PointerTypeExpr(SR, SubType);
}

ReferenceTypeExpr::ReferenceTypeExpr(SourceRange SR,
                                     SourceType SubType)
   : TypeExpr(ReferenceTypeExprID, SR), SubType(SubType)
{

}

ReferenceTypeExpr* ReferenceTypeExpr::Create(ASTContext &C,
                                             SourceRange SR,
                                             SourceType SubType) {
   return new(C) ReferenceTypeExpr(SR, SubType);
}

OptionTypeExpr::OptionTypeExpr(SourceRange SR,
                               SourceType SubType)
   : TypeExpr(OptionTypeExprID, SR), SubType(SubType)
{

}

OptionTypeExpr* OptionTypeExpr::Create(ASTContext &C,
                                       SourceRange SR,
                                       SourceType SubType) {
   return new(C) OptionTypeExpr(SR, SubType);
}


AttributedExpr::AttributedExpr(Expression *Expr,
                               llvm::ArrayRef<Attr *> Attrs)
   : Expression(AttributedExprID), Expr(Expr)
{
   loc = Expr->getSourceLoc();
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
   : Expression(StaticExprID), expr(expr)
{

}

StaticExpr::StaticExpr(Variant &&V)
   : Expression(StaticExprID), expr(nullptr), evaluatedExpr(std::move(V))
{}

StaticExpr* StaticExpr::Create(ASTContext &C, Expression *Expr)
{
   return new(C) StaticExpr(Expr);
}

StaticExpr* StaticExpr::Create(ASTContext &C, Variant &&V)
{
   return new(C) StaticExpr(std::move(V));
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

SequenceElement::SequenceElement(std::string &&possibleOp,
                                 SourceLocation loc)
   : op(move(possibleOp)), kind(EF_PossibleOperator), loc(loc)
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
      new (&op) std::string(move(other.op));
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
   if (kind == EF_PossibleOperator) {
      op.~string();
   }

   kind = other.kind;
   loc = other.loc;

   if (kind == EF_PossibleOperator) {
      new (&op) std::string(move(other.op));
   }
   else if (kind == EF_Expression) {
      expr = other.expr;
   }
   else {
      operatorKind = other.operatorKind;
   }

   return *this;
}

SequenceElement::~SequenceElement()
{
   if (kind == EF_PossibleOperator) {
      op.~string();
   }
}

ExprSequence::ExprSequence(llvm::MutableArrayRef<SequenceElement> fragments)
   : Expression(ExprSequenceID),
     NumFragments((unsigned)fragments.size())
{
   std::move(fragments.begin(), fragments.end(),
             getTrailingObjects<SequenceElement>());
}

ExprSequence*
ExprSequence::Create(ASTContext &C,
                     llvm::MutableArrayRef<SequenceElement> fragments) {
   void *Mem = C.Allocate(totalSizeToAlloc<SequenceElement>(fragments.size()),
                          alignof(ExprSequence));

   return new (Mem) ExprSequence(fragments);
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

TypePredicateExpr::TypePredicateExpr(SourceLocation operatorLoc,
                                     Expression *LHS,
                                     Expression *RHS,
                                     op::OperatorKind kind = op::Colon)
   : Expression(TypePredicateExprID),
     operatorLoc(operatorLoc), kind(kind), LHS(LHS), RHS(RHS)
{ }

TypePredicateExpr* TypePredicateExpr::Create(ASTContext &C,
                                             SourceLocation operatorLoc,
                                             Expression *LHS, Expression *RHS,
                                             op::OperatorKind kind) {
   return new(C) TypePredicateExpr(operatorLoc, LHS, RHS, kind);
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
                         string &&caseName,
                         llvm::MutableArrayRef<CasePatternArgument> args)
   : PatternExpr(CasePatternID, SR.getEnd()),
     PeriodLoc(SR.getStart()), caseName(move(caseName)),
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
                    string &&caseName,
                    llvm::MutableArrayRef<CasePatternArgument> args) {
   void *Mem = C.Allocate(totalSizeToAlloc<CasePatternArgument>(args.size()),
                          alignof(CasePattern));

   return new(Mem) CasePattern(SR, move(caseName), args);
}

CasePatternArgument::CasePatternArgument(CasePatternArgument &&arg) noexcept
   : sourceLoc(arg.sourceLoc), IsExpr(arg.IsExpr), IsConst(arg.IsConst)
{
   if (IsExpr)
      expr = arg.expr;
   else
      new(&identifier) std::string(move(arg.identifier));
}

CasePatternArgument& CasePatternArgument::operator=(CasePatternArgument &&arg)
noexcept
{
   destroyValue();

   sourceLoc = arg.sourceLoc;
   IsExpr = arg.IsExpr;
   IsConst = arg.IsConst;

   if (IsExpr)
      expr = arg.expr;
   else
      new(&identifier) std::string(move(arg.identifier));

   return *this;
}

void CasePatternArgument::destroyValue()
{
   if (!IsExpr)
      identifier.~string();
}

CasePatternArgument::~CasePatternArgument()
{
   destroyValue();
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

IntegerLiteral::IntegerLiteral(SourceLocation Loc,
                               Type *type,
                               llvm::APSInt &&value,
                               Suffix suffix)
   :  Expression(IntegerLiteralID, true),
      Loc(Loc), value(std::move(value)), type(type), suffix(suffix)
{}

IntegerLiteral *IntegerLiteral::Create(ASTContext &C,
                                       SourceLocation Loc,
                                       Type *type,
                                       llvm::APSInt &&value,
                                       Suffix suffix) {
   return new(C) IntegerLiteral(Loc, type, std::move(value), suffix);
}

FPLiteral::FPLiteral(SourceLocation Loc, Type *type,
                     llvm::APFloat &&value,
                     Suffix suffix = Suffix::None)
   :  Expression(FPLiteralID, true),
      Loc(Loc), value(std::move(value)), type(type), suffix(suffix)
{}

FPLiteral* FPLiteral::Create(ASTContext &C, SourceLocation Loc,
                             Type *type, llvm::APFloat &&value,
                             Suffix suffix) {
   return new(C) FPLiteral(Loc, type, std::move(value), suffix);
}

BoolLiteral::BoolLiteral(SourceLocation Loc, Type *type, bool value)
   : Expression(BoolLiteralID),
     Loc(Loc), value(value), type(type)
{

}

BoolLiteral* BoolLiteral::Create(ASTContext &C,
                                 SourceLocation Loc,
                                 Type *type, bool value) {
   return new(C) BoolLiteral(Loc, type, value);
}

CharLiteral::CharLiteral(SourceLocation Loc, Type *type, char value)
   : Expression(CharLiteralID),
     Loc(Loc), narrow(value), IsWide(false), type(type)
{

}

CharLiteral::CharLiteral(SourceLocation Loc, Type *type, uint32_t value)
   : Expression(CharLiteralID),
     Loc(Loc), wide(value), IsWide(true), type(type)
{

}

CharLiteral* CharLiteral::Create(ASTContext &C,
                                 SourceLocation Loc, Type *type,
                                 char value) {
   return new(C) CharLiteral(Loc, type, value);
}

CharLiteral* CharLiteral::Create(ASTContext &C,
                                 SourceLocation Loc, Type *type,
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

StringLiteral::StringLiteral( SourceLocation Loc, std::string &&str)
   : Expression(StringLiteralID, true),
     Loc(Loc), value(move(str))
{}

StringLiteral* StringLiteral::Create(ASTContext &C,
                                     SourceLocation Loc,
                                     std::string &&str) {
   return new(C) StringLiteral(Loc, move(str));
}

StringInterpolation::StringInterpolation(SourceLocation Loc,
                                         llvm::ArrayRef<Expression *> strings)
   : Expression(StringInterpolationID),
     Loc(Loc), NumStrings((unsigned)strings.size())
{
   std::copy(strings.begin(), strings.end(), getTrailingObjects<Expression*>());
}

StringInterpolation*
StringInterpolation::Create(ASTContext &C,
                            SourceLocation Loc,
                            llvm::ArrayRef<Expression *> strings) {
   void *Mem = C.Allocate(totalSizeToAlloc<Expression*>(strings.size()),
                          alignof(StringInterpolation));

   return new(Mem) StringInterpolation(Loc, strings);
}

LambdaExpr::LambdaExpr(SourceRange Parens,
                       SourceLocation ArrowLoc,
                       SourceType returnType,
                       llvm::ArrayRef<FuncArgDecl* > args,
                       Statement* body)
   : Expression(LambdaExprID, true),
     Parens(Parens), ArrowLoc(ArrowLoc),
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

IdentifierRefExpr::IdentifierRefExpr(SourceLocation Loc,
                                     std::string &&ident,
                                     std::vector<Expression*> &&templateArgs,
                                     DeclContext *DeclCtx,
                                     bool InTypePos)
   : IdentifiedExpr(IdentifierRefExprID, move(ident)),
     Loc(Loc), DeclCtx(DeclCtx), staticLookup(false),
     pointerAccess(false), FoundResult(false), InTypePosition(InTypePos),
     templateArgs(move(templateArgs))
{}

IdentifierRefExpr::IdentifierRefExpr(SourceLocation Loc, IdentifierKind kind,
                                     QualType exprType)
   : IdentifiedExpr(IdentifierRefExprID, ""),
     Loc(Loc), kind(kind), staticLookup(false),
     pointerAccess(false), FoundResult(true), InTypePosition(false)
{
   setSemanticallyChecked(true);
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

MemberRefExpr::MemberRefExpr(SourceLocation Loc,
                             string &&ident,
                             bool pointerAccess)
   : IdentifiedExpr(MemberRefExprID, move(ident)),
     Loc(Loc), pointerAccess(pointerAccess), tupleAccess(false)
{

}

MemberRefExpr::MemberRefExpr(SourceLocation Loc,
                             Expression *ParentExpr,
                             string &&ident,
                             bool pointerAccess)
   : IdentifiedExpr(MemberRefExprID, move(ident)),
     Loc(Loc), ParentExpr(ParentExpr),
     pointerAccess(pointerAccess), tupleAccess(false)
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
                           std::string &&caseName,
                           std::vector<Expression*> &&args)
   : IdentifiedExpr(EnumCaseExprID, move(caseName), true),
     PeriodLoc(PeriodLoc), args(move(args)), en(nullptr)
{}

CallExpr::CallExpr(SourceLocation IdentLoc, SourceRange ParenRange,
                   std::vector<Expression* > &&args,
                   string &&ident)
   : IdentifiedExpr(CallExprID, move(ident)), args(move(args)),
     IdentLoc(IdentLoc), ParenRange(ParenRange),
     PointerAccess(false), IsUFCS(false),
     builtinFnKind(BuiltinFn::None)
{

}

CallExpr::CallExpr(SourceLocation IdentLoc, SourceRange ParenRange,
                   Expression *ParentExpr,
                   std::vector<Expression *> &&args, cdot::string &&name)
   : IdentifiedExpr(CallExprID, move(name)),
     IdentLoc(IdentLoc), ParenRange(ParenRange),
     ParentExpr(ParentExpr), args(move(args)),
     PointerAccess(false), IsUFCS(false),
     builtinFnKind(BuiltinFn::None)
{

}

CallExpr::CallExpr(SourceLocation IdentLoc, SourceRange ParenRange,
                   std::vector<Expression *> &&args, CallableDecl *C)
   : IdentifiedExpr(CallExprID, ""),
     IdentLoc(IdentLoc), ParenRange(ParenRange),
     args(move(args)),
     PointerAccess(false), IsUFCS(false),
     builtinFnKind(BuiltinFn::None)
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
   std::move(args.begin(), args.end(), getTrailingObjects<TraitsArgument>());
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

} // namespace ast
} // namespace cdot
