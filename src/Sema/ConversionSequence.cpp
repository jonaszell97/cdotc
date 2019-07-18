//
// Created by Jonas Zell on 19.02.18.
//

#include "ConversionSequence.h"

#include "AST/ASTContext.h"
#include "AST/Decl.h"

using namespace cdot;

QualType ConversionStep::getResultType() const
{
   if (Kind == CastKind::ConversionOp) {
      return getConversionOp()->getReturnType();
   }

   return QualType::getFromOpaquePtr(Data);
}

unsigned cdot::getPenalty(CastKind kind)
{
   switch (kind) {
   case CastKind::NoOp:
   case CastKind::Move:
   case CastKind::Forward:
   case CastKind::LValueToRValue:
   case CastKind::MutRefToRef:
   case CastKind::RValueToConstRef:
   case CastKind::ToMetaType:
   case CastKind::NoThrowToThrows:
   case CastKind::ThinToThick:
   case CastKind::MetaTypeCast:
   case CastKind::ToVoid:
   case CastKind::ToEmptyTuple:
      return 0;
   case CastKind::Ext:
   case CastKind::FPExt:
   case CastKind::SignFlip:
   case CastKind::EnumToInt:
   case CastKind::IntToEnum:
   case CastKind::Copy:
   case CastKind::MutPtrToPtr:
   case CastKind::IsNull:
   case CastKind::BitCast:
   case CastKind::ExistentialRef:
      return 1;
   case CastKind::ExistentialInit:
   case CastKind::ExistentialCast:
   case CastKind::ExistentialUnwrap:
   case CastKind::UpCast:
   case CastKind::DynCast:
   case CastKind::ProtoUnwrap:
   case CastKind::IntToFP:
   case CastKind::FPToInt:
   case CastKind::Trunc:
   case CastKind::FPTrunc:
   case CastKind::PtrToInt:
   case CastKind::IntToPtr:
      return 2;
   case CastKind::ConversionOp:
   case CastKind::ExistentialUnwrapFallible:
   case CastKind::ExistentialCastFallible:
      return 3;
   default:
      llvm_unreachable("bad implicit cast kind!");
   }
}

static unsigned getConversionPenalty(ArrayRef<ConversionStep> steps)
{
   unsigned penalty = 0;
   for (const auto &C : steps) {
      penalty += getPenalty(C.getKind());
   }

   return penalty;
}

ConversionSequence*
ConversionSequence::Create(ast::ASTContext &C,
                           const ConversionSequenceBuilder &Builder,
                           QualType finalType) {
   void *Mem = C.Allocate(totalSizeToAlloc<ConversionStep>(
                             Builder.getSteps().size()),
                          alignof(ConversionSequence));

   return new(Mem) ConversionSequence(Builder, finalType);
}

ConversionSequence* ConversionSequence::Create(ast::ASTContext &C,
                                               CastStrength Strength,
                                               ArrayRef<ConversionStep> Steps) {
   void *Mem = C.Allocate(totalSizeToAlloc<ConversionStep>(Steps.size()),
                          alignof(ConversionSequence));

   return new(Mem) ConversionSequence(Strength, Steps);
}

ConversionSequence::ConversionSequence(const ConversionSequenceBuilder &Builder,
                                       QualType finalType)
   : Strength(Builder.getStrength()), NumSteps(0),
     Penalty(Builder.getPenalty())
{
   auto Steps = Builder.getSteps();
   NumSteps = (unsigned)Steps.size();

   if (finalType) {
      Steps.back().setResultType(finalType);
   }

   std::copy(Steps.begin(), Steps.end(), getTrailingObjects<ConversionStep>());
}

ConversionSequence::ConversionSequence(CastStrength Strength,
                                       ArrayRef<ConversionStep> Steps)
   : Strength(Strength), NumSteps((unsigned)Steps.size()),
     Penalty(getConversionPenalty(Steps))
{
   std::copy(Steps.begin(), Steps.end(), getTrailingObjects<ConversionStep>());
}