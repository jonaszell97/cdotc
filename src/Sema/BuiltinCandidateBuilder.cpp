#include "cdotc/Sema/BuiltinCandidateBuilder.h"

#include "cdotc/AST/Expression.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/SemaPass.h"

using namespace cdot::support;

namespace cdot {
namespace ast {

void BuiltinCandidateBuilder::addBuiltinCandidates(
    CandidateSet& CandSet, DeclarationName opName,
    llvm::ArrayRef<Expression*> args)
{
   if (args.empty())
      return;

   return addBuiltinCandidates(CandSet, opName, args.front()->getExprType());
}

static bool isIntFPOrPointerType(QualType ty, bool allowPtr = true)
{
   switch (ty->getTypeID()) {
   case Type::PointerTypeID:
   case Type::MutablePointerTypeID:
      return allowPtr;
   case Type::BuiltinTypeID:
      switch (ty->uncheckedAsBuiltinType()->getKind()) {
#define CDOT_BUILTIN_INT(Name, BW, Unsigned)                                   \
   case BuiltinType::Name:                                                     \
      return true;
#define CDOT_BUILTIN_FP(Name, Precision)                                       \
   case BuiltinType::Name:                                                     \
      return true;
#include "cdotc/Basic/BuiltinTypes.def"
         return true;
      default:
         return false;
      }
   default:
      return false;
   }
}

static bool isIntOrFPType(QualType ty)
{
   return isIntFPOrPointerType(ty, false);
}

void BuiltinCandidateBuilder::addBuiltinCandidates(CandidateSet& CandSet,
                                                   DeclarationName op,
                                                   QualType lhsType)
{
   if (!lhsType)
      return;

   lhsType = lhsType->getCanonicalType();

   op::OperatorKind opKind;
   FixKind fix;

   getOpKindAndFix(op, opKind, fix);

   if (opKind == op::UnknownOp)
      return;

   auto TypeCache = Cache[lhsType];
   auto OpIt = TypeCache.find(opKind);
   if (OpIt == TypeCache.end())
      OpIt = fillCache(TypeCache, lhsType, opKind);

   //   for (auto &Cand : OpIt->second) {
   //      CandSet.addCandidate(Cand.Ty, Cand.PG, opKind);
   //   }
}

static QualType getMutableReference(ast::ASTContext& Context, QualType Ty)
{
   if (Ty->isMutableReferenceType())
      return Ty;

   if (Ty->isReferenceType())
      return Context.getMutableReferenceType(Ty->getReferencedType());

   return Context.getMutableReferenceType(Ty);
}

static FunctionType::ParamInfo AssignParamInfo[2] = {
    // the left hand side is mutably borrowed
    FunctionType::ParamInfo{ArgumentConvention::MutableRef},

    // the right hand side is consumed by this operator
    FunctionType::ParamInfo{ArgumentConvention::Owned}};

static FunctionType::ParamInfo CompoundAssignParamInfo[2] = {
    // the left hand side is mutably borrowed
    FunctionType::ParamInfo{ArgumentConvention::MutableRef},

    // the right hand side is immutably borrowed
    FunctionType::ParamInfo{ArgumentConvention::Borrowed}};

BuiltinCandidateBuilder::BuiltinKindMap::iterator
BuiltinCandidateBuilder::fillCache(BuiltinKindMap& Map, QualType lhsType,
                                   op::OperatorKind opKind)
{
   auto& Context = SP.getContext();

   CacheVec Vec;

   switch (opKind) {
   case op::Assign: {
      // add overloads anyway so we can issue better diagnostics
      lhsType = getMutableReference(Context, lhsType);

      FunctionType* FnTy = Context.getFunctionType(
          Context.getVoidType(), {lhsType, lhsType->getReferencedType()},
          AssignParamInfo);

      Vec.emplace_back(FnTy);
      break;
   }
   case op::AddAssign:
   case op::SubAssign:
   case op::MulAssign: {
      // add overloads anyway so we can issue better diagnostics
      lhsType = getMutableReference(Context, lhsType);

      if (!isIntFPOrPointerType(lhsType->getReferencedType()))
         break;

      // pointer arithmetic
      if (lhsType->getReferencedType()->isPointerType()
          && opKind != op::MulAssign) {
         auto FnTy = Context.getFunctionType(Context.getVoidType(),
                                             {lhsType, Context.getUIntTy()},
                                             CompoundAssignParamInfo);

         Vec.emplace_back(FnTy);

         if (opKind == op::SubAssign) {
            FnTy = Context.getFunctionType(
                Context.getVoidType(), {lhsType, lhsType->getReferencedType()},
                CompoundAssignParamInfo);

            Vec.emplace_back(FnTy);
         }
      }
      else if (!lhsType->getReferencedType()->isPointerType()) {
         FunctionType* FnTy = Context.getFunctionType(
             Context.getVoidType(), {lhsType, lhsType->getReferencedType()},
             CompoundAssignParamInfo);

         Vec.emplace_back(FnTy);
      }

      break;
   }
   case op::DivAssign:
   case op::ModAssign:
   case op::ExpAssign: {
      // add overloads anyway so we can issue better diagnostics
      lhsType = getMutableReference(Context, lhsType);

      if (!isIntOrFPType(lhsType->getReferencedType()))
         break;

      FunctionType* FnTy = Context.getFunctionType(
          Context.getVoidType(), {lhsType, lhsType->getReferencedType()},
          CompoundAssignParamInfo);

      Vec.emplace_back(FnTy);
      break;
   }
   // Bitwise compound assignment
   case op::AndAssign:
   case op::OrAssign:
   case op::XorAssign:
   case op::ShlAssign:
   case op::LShrAssign:
   case op::AShrAssign: {
      // add overloads anyway so we can issue better diagnostics
      lhsType = getMutableReference(Context, lhsType);

      if (!lhsType->getReferencedType()->isIntegerType())
         break;

      FunctionType* FnTy = Context.getFunctionType(
          Context.getVoidType(), {lhsType, lhsType->getReferencedType()},
          CompoundAssignParamInfo);

      Vec.emplace_back(FnTy);
      break;
   }
   // Addition, Subtraction, Multiplication
   case op::Mul:
   case op::Add:
   case op::Sub: {
      if (lhsType->isReferenceType())
         lhsType = lhsType->getReferencedType();

      if (!isIntFPOrPointerType(lhsType))
         break;

      // pointer arithmetic
      if (lhsType->isPointerType() && opKind != op::Mul) {
         auto FnTy
             = Context.getFunctionType(lhsType, {lhsType, Context.getUIntTy()});

         Vec.emplace_back(FnTy);

         if (opKind == op::Sub) {
            FnTy = Context.getFunctionType(Context.getUIntTy(),
                                           {lhsType, lhsType});

            Vec.emplace_back(FnTy);
         }
      }
      else if (!lhsType->isPointerType()) {
         FunctionType* FnTy
             = Context.getFunctionType(lhsType, {lhsType, lhsType});

         Vec.emplace_back(FnTy);
      }

      break;
   }
   // Division, Modulo, Exponentiation
   case op::Exp:
      // floating point values also have an overload for integer types on the
      // right hand side
      if (lhsType->isFPType()) {
         FunctionType* FnTy
             = Context.getFunctionType(lhsType, {lhsType, Context.getu32Ty()});

         Vec.emplace_back(FnTy);

         FnTy = Context.getFunctionType(lhsType, {lhsType, Context.getu64Ty()});

         Vec.emplace_back(FnTy);
      }

      LLVM_FALLTHROUGH;
   case op::Div:
   case op::Mod: {
      if (lhsType->isReferenceType())
         lhsType = lhsType->getReferencedType();

      if (!isIntOrFPType(lhsType))
         break;

      FunctionType* FnTy = Context.getFunctionType(lhsType, {lhsType, lhsType});

      Vec.emplace_back(FnTy);
      break;
   }
   // Bitwise / Type union
   case op::And: {
      if (lhsType->isMetaType()) {
         FunctionType* FnTy = Context.getFunctionType(
             lhsType, {lhsType, Context.getUnknownAnyTy()});

         Vec.emplace_back(FnTy);
      }

      LLVM_FALLTHROUGH;
   }
   // Bitwise Ops
   case op::Or:
   case op::Xor:
   case op::Shl:
   case op::LShr:
   case op::AShr: {
      if (lhsType->isReferenceType())
         lhsType = lhsType->getReferencedType();

      if (!lhsType->isIntegerType())
         break;

      FunctionType* FnTy = Context.getFunctionType(lhsType, {lhsType, lhsType});

      Vec.emplace_back(FnTy);

      break;
   }
   // Comparison / Relational Ops
   case op::CompGE:
   case op::CompLE:
   case op::CompGT:
   case op::CompLT:
   case op::Spaceship:
   case op::CompEQ:
   case op::CompNE: {
      if (lhsType->isReferenceType())
         lhsType = lhsType->getReferencedType();

      if (lhsType->isMetaType()) {
         if (opKind == op::CompEQ || opKind == op::CompNE) {
            FunctionType* FnTy = Context.getFunctionType(
                Context.getBoolTy(), {lhsType, Context.getUnknownAnyTy()});

            Vec.emplace_back(FnTy);
         }

         break;
      }

      if (!isIntFPOrPointerType(lhsType))
         break;

      QualType RetTy;
      if (opKind == op::Spaceship) {
         RetTy = Context.getIntTy();
      }
      else {
         RetTy = Context.getBoolTy();
      }

      FunctionType* FnTy = Context.getFunctionType(RetTy, {lhsType, lhsType});
      Vec.emplace_back(FnTy);

      break;
   }
   // reference equality
   case op::CompRefEQ:
   case op::CompRefNE: {
      if (!lhsType->isReferenceType() && !lhsType->isClass())
         break;

      FunctionType* FnTy
          = Context.getFunctionType(Context.getBoolTy(), {lhsType, lhsType});

      Vec.emplace_back(FnTy);
      break;
   }
   // Short-circuit ops
   case op::LAnd:
   case op::LOr: {
      if (lhsType->isReferenceType())
         lhsType = lhsType->getReferencedType();
      if (!lhsType->isInt1Ty())
         break;

      FunctionType* FnTy
          = Context.getFunctionType(Context.getBoolTy(), {lhsType, lhsType});

      Vec.emplace_back(FnTy);
      break;
   }
   case op::As:
   case op::AsExclaim:
   case op::AsQuestion: {
      FunctionType* FnTy = Context.getFunctionType(
          Context.getUnknownAnyTy(),
          {lhsType->removeReference(), Context.getUnknownAnyTy()});

      Vec.emplace_back(FnTy);
      break;
   }
   case op::UnaryPlus:
   case op::UnaryMin: {
      if (lhsType->isReferenceType())
         lhsType = lhsType->getReferencedType();

      if (!isIntOrFPType(lhsType))
         break;

      if (lhsType->isIntegerType()) {
         // add this candidate for better diagnostics
         auto SignedTy = lhsType->getSignedOfSameWidth(SP.getContext());
         FunctionType* FnTy = Context.getFunctionType(SignedTy, {SignedTy});
         Vec.emplace_back(FnTy);
      }
      else {
         FunctionType* FnTy = Context.getFunctionType(lhsType, {lhsType});
         Vec.emplace_back(FnTy);
      }

      break;
   }
   case op::UnaryNot: {
      if (lhsType->isReferenceType())
         lhsType = lhsType->getReferencedType();
      if (!lhsType->isIntegerType())
         break;

      FunctionType* FnTy = Context.getFunctionType(lhsType, {lhsType});
      Vec.emplace_back(FnTy);

      break;
   }
   case op::UnaryLNot: {
      if (lhsType->isReferenceType())
         lhsType = lhsType->getReferencedType();

      if (!lhsType->isInt1Ty() && !lhsType->isPointerType())
         break;

      FunctionType* FnTy
          = Context.getFunctionType(Context.getBoolTy(), {lhsType});

      Vec.emplace_back(FnTy);
      break;
   }
   case op::PreDec:
   case op::PreInc: {
      if (!lhsType->isReferenceType())
         break;
      if (!isIntFPOrPointerType(lhsType->getReferencedType()))
         break;

      QualType RefTy = lhsType;
      if (!lhsType->isMutableReferenceType())
         RefTy = Context.getMutableReferenceType(lhsType->getReferencedType());

      FunctionType* FnTy
          = Context.getFunctionType(lhsType->getReferencedType(), {RefTy});

      Vec.emplace_back(FnTy);
      break;
   }
   case op::PostInc:
   case op::PostDec: {
      if (!lhsType->isReferenceType())
         break;
      if (!isIntFPOrPointerType(lhsType->getReferencedType()))
         break;

      QualType RefTy = lhsType;
      if (!lhsType->isMutableReferenceType())
         RefTy = Context.getMutableReferenceType(lhsType->getReferencedType());

      FunctionType* FnTy
          = Context.getFunctionType(lhsType->getReferencedType(), {RefTy});

      Vec.emplace_back(FnTy);
      break;
   }
   case op::Deref: {
      if (lhsType->isReferenceType())
         lhsType = lhsType->getReferencedType();

      QualType ReferenceTy;
      if (lhsType->isMutablePointerType()) {
         ReferenceTy
             = Context.getMutableReferenceType(lhsType->getPointeeType());
      }
      else if (lhsType->isPointerType()) {
         ReferenceTy = Context.getReferenceType(lhsType->getPointeeType());
      }
      else {
         break;
      }

      FunctionType* FnTy = Context.getFunctionType(ReferenceTy, {lhsType});
      Vec.emplace_back(FnTy);

      break;
   }
   case op::AddrOf: {
      if (!lhsType->isMutableReferenceType())
         lhsType = Context.getMutableReferenceType(lhsType->removeReference());

      auto ResultTy
          = Context.getMutableReferenceType(lhsType->getReferencedType());

      FunctionType* FnTy = Context.getFunctionType(ResultTy, {lhsType});
      Vec.emplace_back(FnTy);

      break;
   }
   case op::BlockAddrOf: {
      if (!lhsType->isBuiltinType())
         break;
      if (!lhsType->uncheckedAsBuiltinType()->isLabelTy())
         break;

      FunctionType* FnTy = Context.getFunctionType(lhsType, {lhsType});
      Vec.emplace_back(FnTy);

      break;
   }
   case op::TypeOf: {
      lhsType = lhsType->removeReference();
      FunctionType* FnTy
          = Context.getFunctionType(Context.getMetaType(lhsType), {lhsType});

      Vec.emplace_back(FnTy);
      break;
   }
   case op::UnaryPtr: {
      if (!lhsType->isMetaType())
         break;

      FunctionType* FnTy = Context.getFunctionType(
          Context.getMetaType(Context.getPointerType(
              lhsType->uncheckedAsMetaType()->getUnderlyingType())),
          {lhsType});

      Vec.emplace_back(FnTy);
      break;
   }
   case op::UnaryOption: {
      if (!lhsType->isMetaType())
         break;

      FunctionType* FnTy = Context.getFunctionType(
          Context.getUInt8PtrTy(), // will never actually be used
          {lhsType});

      Vec.emplace_back(FnTy);
      break;
   }
   default:
      llvm_unreachable("bad builtin operator kind!");
      ;
   }

   auto* PG = getPrecedenceGroup(opKind);
   for (auto& Cand : Vec) {
      Cand.PG = PG;
   }

   return Map.emplace(opKind, move(Vec)).first;
}

static bool isInfix(op::OperatorKind Kind)
{
#define Infix true
#define Postfix false
#define Prefix false

   switch (Kind) {
#define CDOT_OPERATOR(Name, Spelling, Prec, Fix)                               \
   case op::Name:                                                              \
      return Fix;
#include "cdotc/Basic/BuiltinOperators.def"
   }

#undef Infix
#undef Prefix
#undef Postfix
}

ast::PrecedenceGroupDecl*
BuiltinCandidateBuilder::getPrecedenceGroup(op::OperatorKind opKind)
{
   if (!isInfix(opKind))
      return nullptr;

   auto OpStr = op::toString(opKind);
   auto& II = SP.getContext().getIdentifiers().get(OpStr);
   auto OpName = SP.getContext().getDeclNameTable().getInfixOperatorName(II);
   auto OpDecl = SP.getContext().getDeclNameTable().getOperatorDeclName(OpName);

   OperatorDecl* Op;
   if (SP.QC.FindOperator(Op, OpDecl, false)) {
      llvm_unreachable("builtin operator not found!");
   }

   return Op->getPrecedenceGroup();
}

void BuiltinCandidateBuilder::getOpKindAndFix(DeclarationName op,
                                              op::OperatorKind& opKind,
                                              FixKind& fix)
{
   auto Kind = op.getKind();
   switch (Kind) {
   case DeclarationName::InfixOperatorName:
      opKind = op::fromString(FixKind::Infix,
                              op.getInfixOperatorName()->getIdentifier());

      fix = FixKind::Infix;
      break;
   case DeclarationName::PrefixOperatorName:
      opKind = op::fromString(FixKind::Prefix,
                              op.getPrefixOperatorName()->getIdentifier());

      fix = FixKind::Prefix;
      break;
   case DeclarationName::PostfixOperatorName:
      opKind = op::fromString(FixKind::Postfix,
                              op.getPostfixOperatorName()->getIdentifier());

      fix = FixKind::Postfix;
      break;
   default:
      opKind = op::UnknownOp;
      fix = FixKind::Infix;
      break;
   }
}

} // namespace ast
} // namespace cdot