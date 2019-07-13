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
   : Strength(Builder.getStrength()), NumSteps(0)
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
   : Strength(Strength), NumSteps((unsigned)Steps.size())
{
   std::copy(Steps.begin(), Steps.end(), getTrailingObjects<ConversionStep>());
}