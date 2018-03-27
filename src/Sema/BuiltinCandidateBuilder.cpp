//
// Created by Jonas Zell on 18.02.18.
//

#include "BuiltinCandidateBuilder.h"

#include "AST/Expression.h"
#include "SemaPass.h"

namespace cdot {
namespace ast {

void
BuiltinCandidateBuilder::addBuiltinCandidates(CandidateSet &CandSet,
                                              DeclarationName opName,
                                              llvm::ArrayRef<Expression*> args){
   if (args.empty())
      return;

   return addBuiltinCandidates(CandSet, opName, args.front()->getExprType());
}

static bool isIntFPOrPointerType(QualType ty, bool allowPtr = true)
{
   switch (ty->getTypeID()) {
   case TypeID::PointerTypeID:
      return allowPtr;
   case TypeID::BuiltinTypeID:
      switch (ty->uncheckedAsBuiltinType()->getKind()) {
#     define CDOT_BUILTIN_INT(Name, BW, Unsigned)           \
      case BuiltinType::Name: return true;
#     define CDOT_BUILTIN_FP(Name, Precision)               \
      case BuiltinType::Name: return true;
#     include "Basic/BuiltinTypes.def"
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

void BuiltinCandidateBuilder::addBuiltinCandidates(CandidateSet &CandSet,
                                                   DeclarationName op,
                                                   QualType lhsType) {
   op::OperatorKind opKind;
   FixKind fix;

   getOpKindAndFix(op, opKind, fix);

   if (opKind == op::UnknownOp)
      return;

   auto TypeCache = Cache[lhsType];
   auto OpIt = TypeCache.find(opKind);
   if (OpIt == TypeCache.end())
      OpIt = fillCache(TypeCache, lhsType, opKind);

   for (auto &Cand : OpIt->second) {
      CandSet.addCandidate(Cand.Ty, Cand.PG, opKind);
   }
}

BuiltinCandidateBuilder::BuiltinKindMap::iterator
BuiltinCandidateBuilder::fillCache(BuiltinKindMap &Map,
                                   QualType lhsType,
                                   op::OperatorKind opKind) {
   auto &Context = SP.getContext();
   prec::PrecedenceLevel prec = prec::Unknown;
   Associativity assoc = Associativity::Left;
   
   CacheVec Vec;

   switch (opKind) {
   case op::Assign: {
      if (!lhsType->isReferenceType())
         break;

      FunctionType *FnTy = Context.getFunctionType(
         Context.getVoidType(),
         { lhsType, lhsType->getReferencedType() });

      Vec.emplace_back(FnTy, PrecedenceGroup(prec::Assignment,
                                             Associativity::Right));

      break;
   }
   case op::AddAssign: case op::SubAssign: case op::MulAssign: {
      if (!lhsType->isReferenceType())
         break;
      if (!isIntFPOrPointerType(lhsType->getReferencedType()))
         break;

      // pointer arithmetic
      if (lhsType->getReferencedType()->isPointerType()
          && opKind != op::MulAssign) {
         auto FnTy = Context.getFunctionType(
            Context.getVoidType(),
            { lhsType, Context.getUIntTy() });

         Vec.emplace_back(FnTy, PrecedenceGroup(prec::Assignment,
                                                Associativity::Right));

         if (opKind == op::SubAssign) {
            FnTy = Context.getFunctionType(
               Context.getVoidType(),
               { lhsType, lhsType->getReferencedType() });

            Vec.emplace_back(FnTy, PrecedenceGroup(prec::Assignment,
                                                   Associativity::Left));
         }
      }
      else if (!lhsType->getReferencedType()->isPointerType()) {
         FunctionType *FnTy = Context.getFunctionType(
            Context.getVoidType(),
            { lhsType, lhsType->getReferencedType() });

         Vec.emplace_back(FnTy, PrecedenceGroup(prec::Assignment,
                                                Associativity::Right));
      }

      break;
   }
   case op::DivAssign: case op::ModAssign: case op::ExpAssign: {
      if (!lhsType->isReferenceType())
         break;
      if (!isIntOrFPType(lhsType->getReferencedType()))
         break;

      FunctionType *FnTy = Context.getFunctionType(
         Context.getVoidType(),
         { lhsType, lhsType->getReferencedType() });

      Vec.emplace_back(FnTy, PrecedenceGroup(prec::Assignment,
                                             Associativity::Right));

      break;
   }
      // Bitwise compound assignment
   case op::AndAssign: case op::OrAssign: case op::XorAssign:
   case op::ShlAssign: case op::LShrAssign: case op::AShrAssign: {
      if (!lhsType->isReferenceType())
         break;
      if (!lhsType->getReferencedType()->isIntegerType())
         break;

      FunctionType *FnTy = Context.getFunctionType(
         Context.getVoidType(),
         { lhsType, lhsType->getReferencedType() });

      Vec.emplace_back(FnTy, PrecedenceGroup(prec::Assignment,
                                             Associativity::Right));

      break;
   }
      // Addition, Subtraction, Multiplication
   case op::Mul:
      prec = prec::Multiplicative;
      goto case_add_sub_mul;
   case op::Add: case op::Sub:
      prec = prec::Additive;
      goto case_add_sub_mul;
   case_add_sub_mul: {
      if (lhsType->isReferenceType())
         lhsType = lhsType->getReferencedType();

      if (!isIntFPOrPointerType(lhsType))
         break;

      // pointer arithmetic
      if (lhsType->isPointerType() && opKind != op::Mul) {
         auto FnTy = Context.getFunctionType(
            lhsType, { lhsType, Context.getUIntTy() });

         Vec.emplace_back(FnTy, PrecedenceGroup(prec, Associativity::Left));

         if (opKind == op::Sub) {
            FnTy = Context.getFunctionType(
               Context.getUIntTy(), { lhsType, lhsType });

            Vec.emplace_back(FnTy, PrecedenceGroup(prec, Associativity::Left));
         }
      }
      else if (!lhsType->isPointerType()) {
         FunctionType *FnTy = Context.getFunctionType(
            lhsType, { lhsType, lhsType });

         Vec.emplace_back(FnTy, PrecedenceGroup(prec, Associativity::Left));
      }

      break;
   }
      // Division, Modulo, Exponentiation
   case op::Exp:
      prec = prec::Exponentiation;
      assoc = Associativity::Right;
      goto case_div_mod_exp;
   case op::Div: case op::Mod:
      prec = prec::Multiplicative;
      goto case_div_mod_exp;
   case_div_mod_exp: {
      if (lhsType->isReferenceType())
         lhsType = lhsType->getReferencedType();

      if (!isIntOrFPType(lhsType))
         break;

      FunctionType *FnTy = Context.getFunctionType(
         lhsType, { lhsType, lhsType });

      Vec.emplace_back(FnTy, PrecedenceGroup(prec, assoc));
      break;
   }
      // Bitwise Ops
   case op::And:
      prec = prec::BitwiseAnd;
      goto case_bitwise;
   case op::Or:
      prec = prec::InclusiveOr;
      goto case_bitwise;
   case op::Xor:
      prec = prec::ExclusiveOr;
      goto case_bitwise;
   case op::Shl:
   case op::LShr:
   case op::AShr:
      prec = prec::Shift;
      goto case_bitwise;
   case_bitwise: {
      if (lhsType->isReferenceType())
         lhsType = lhsType->getReferencedType();

      if (!lhsType->isIntegerType())
         break;

      FunctionType *FnTy = Context.getFunctionType(
         lhsType, { lhsType, lhsType });

      Vec.emplace_back(FnTy, PrecedenceGroup(prec, Associativity::Left));
      break;
   }
      // Comparison / Relational Ops
   case op::CompGE: case op::CompLE:
   case op::CompGT: case op::CompLT:
   case op::Spaceship:
      prec = prec::Relational;
      goto case_eq_rel;
   case op::CompEQ: case op::CompNE:
      prec = prec::Equality;
      goto case_eq_rel;
   case_eq_rel: {
      if (lhsType->isReferenceType())
         lhsType = lhsType->getReferencedType();

      if (lhsType->isMetaType()) {
         if (opKind == op::CompEQ || opKind == op::CompNE) {
            FunctionType *FnTy = Context.getFunctionType(
               Context.getBoolTy(), { lhsType, lhsType });

            Vec.emplace_back(FnTy, PrecedenceGroup(prec, Associativity::Left));
         }

         break;
      }

      if (!isIntFPOrPointerType(lhsType))
         break;

      FunctionType *FnTy = Context.getFunctionType(
         Context.getBoolTy(), { lhsType, lhsType });

      Vec.emplace_back(FnTy, PrecedenceGroup(prec, Associativity::Left));
      break;
   }
      // reference equality
   case op::CompRefEQ: case op::CompRefNE: {
      if (!lhsType->isReferenceType() && !lhsType->isClass())
         break;

      FunctionType *FnTy = Context.getFunctionType(
         Context.getBoolTy(), { lhsType, lhsType });

      Vec.emplace_back(FnTy, PrecedenceGroup(prec::Equality,
                                             Associativity::Left));
      break;
   }
      // Short-circuit ops
   case op::LAnd:
      prec = prec::LogicalAnd;
      goto case_land_lor;
   case op::LOr:
      prec = prec::LogicalOr;
      goto case_land_lor;
   case_land_lor: {
      if (lhsType->isReferenceType())
         lhsType = lhsType->getReferencedType();
      if (!lhsType->isInt1Ty())
         break;

      FunctionType *FnTy = Context.getFunctionType(
         Context.getBoolTy(), { lhsType, lhsType });

      Vec.emplace_back(FnTy, PrecedenceGroup(prec, Associativity::Left));
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
         FunctionType *FnTy = Context.getFunctionType(SignedTy, { SignedTy });
         Vec.emplace_back(FnTy, PrecedenceGroup());
      }
      else {
         FunctionType *FnTy = Context.getFunctionType(lhsType, { lhsType });
         Vec.emplace_back(FnTy, PrecedenceGroup());
      }

      break;
   }
   case op::UnaryNot: {
      if (lhsType->isReferenceType())
         lhsType = lhsType->getReferencedType();
      if (!lhsType->isIntegerType())
         break;

      FunctionType *FnTy = Context.getFunctionType(lhsType, { lhsType });
      Vec.emplace_back(FnTy, PrecedenceGroup());

      break;
   }
   case op::UnaryLNot: {
      if (lhsType->isReferenceType())
         lhsType = lhsType->getReferencedType();
      if (!lhsType->isInt1Ty())
         break;

      FunctionType *FnTy = Context.getFunctionType(lhsType, { lhsType });
      Vec.emplace_back(FnTy, PrecedenceGroup());

      break;
   }
   case op::PreDec:
   case op::PreInc: {
      if (!lhsType->isReferenceType())
         break;
      if (!isIntFPOrPointerType(lhsType->getReferencedType()))
         break;

      FunctionType *FnTy = Context.getFunctionType(
         lhsType->getReferencedType(), { lhsType });

      Vec.emplace_back(FnTy, PrecedenceGroup());
      break;
   }
   case op::PostInc:
   case op::PostDec: {
      if (!lhsType->isReferenceType())
         break;
      if (!isIntFPOrPointerType(lhsType->getReferencedType()))
         break;

      FunctionType *FnTy = Context.getFunctionType(lhsType->getReferencedType(),
                                                   { lhsType });

      Vec.emplace_back(FnTy, PrecedenceGroup());
      break;
   }
   case op::Deref: {
      if (lhsType->isReferenceType())
         lhsType = lhsType->getReferencedType();
      if (!lhsType->isPointerType())
         break;

      FunctionType *FnTy = Context.getFunctionType(
         Context.getReferenceType(lhsType->getPointeeType()),
         { lhsType });

      Vec.emplace_back(FnTy, PrecedenceGroup());
      break;
   }
   case op::AddrOf: {
      if (!lhsType->isReferenceType())
         break;

      FunctionType *FnTy =
         Context.getFunctionType(lhsType->getReferencedType()
                                        ->getPointerTo(Context),
                                 { lhsType });

      Vec.emplace_back(FnTy, PrecedenceGroup());
      break;
   }
   case op::BlockAddrOf: {
      if (!lhsType->isBuiltinType())
         break;
      if (!lhsType->uncheckedAsBuiltinType()->isAnyLabelTy())
         break;

      FunctionType *FnTy = Context.getFunctionType(lhsType, { lhsType });
      Vec.emplace_back(FnTy, PrecedenceGroup());

      break;
   }
   case op::TypeOf: {
      FunctionType *FnTy = Context.getFunctionType(Context.getMetaType(lhsType),
                                                   { lhsType });

      Vec.emplace_back(FnTy, PrecedenceGroup());
      break;
   }
   case op::UnaryPtr: {
      if (!lhsType->isMetaType())
         break;

      FunctionType *FnTy =
         Context.getFunctionType(
            Context.getMetaType(
               Context.getPointerType(lhsType->uncheckedAsMetaType()
                                             ->getUnderlyingType())),
            { lhsType });

      Vec.emplace_back(FnTy, PrecedenceGroup());
      break;
   }
   case op::UnaryOption: {
      llvm_unreachable("TODO!");
   }
   default:
      llvm_unreachable("bad builtin operator kind!");;
   }

   return Map.emplace(opKind, move(Vec)).first;
}

void BuiltinCandidateBuilder::getOpKindAndFix(DeclarationName op,
                                              op::OperatorKind &opKind,
                                              FixKind &fix) {
   auto Kind = op.getDeclarationKind();
   switch (Kind) {
   case DeclarationName::InfixOperatorName:
      opKind = op::fromString(FixKind::Infix, op.getInfixOperatorName()
                                                ->getIdentifier());

      fix = FixKind::Infix;
      break;
   case DeclarationName::PrefixOperatorName:
      opKind = op::fromString(FixKind::Prefix, op.getPrefixOperatorName()
                                                 ->getIdentifier());

      fix = FixKind::Prefix;
      break;
   case DeclarationName::PostfixOperatorName:
      opKind = op::fromString(FixKind::Postfix, op.getPostfixOperatorName()
                                                  ->getIdentifier());

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