//
// Created by Jonas Zell on 19.02.18.
//

#ifndef CDOT_CONVERSIONSEQUENCE_H
#define CDOT_CONVERSIONSEQUENCE_H

#include "AST/Type.h"
#include "Basic/CastKind.h"

#include <llvm/Support/TrailingObjects.h>

namespace cdot {
namespace ast {
   class ASTContext;
   class CallableDecl;
} // namespace ast

unsigned getPenalty(CastKind kind);

class ConversionStep {
   CastKind Kind : 7;
   bool Halt : 1;

   mutable void *Data;

public:
   ConversionStep(CastKind Kind, QualType ty)
      : Kind(Kind), Halt(false),
        Data(ty.getAsOpaquePtr())
   { }

   explicit ConversionStep(ast::CallableDecl *ConvOp)
      : Kind(CastKind::ConversionOp), Halt(false),
        Data(ConvOp)
   { }

   ConversionStep()
      : Kind(CastKind::NoOp), Halt(true), Data{}
   {}

   CastKind getKind() const { return Kind; }
   bool isHalt() const { return Halt; }

   QualType getResultType() const;
   void setResultType(QualType Ty) const { Data = Ty.getAsOpaquePtr(); }

   ast::CallableDecl *getConversionOp() const
   {
      assert(Kind == CastKind::ConversionOp && "not a conversion op!");
      return reinterpret_cast<ast::CallableDecl*>(Data);
   }
};

class ConversionSequenceBuilder {
   CastStrength Strength : 4;
   bool Dependent : 1;
   std::vector<ConversionStep> Steps;
   unsigned Penalty = 0;

public:
   ConversionSequenceBuilder()
      : Strength(CastStrength::Implicit), Dependent(false)
   { }

   ConversionSequenceBuilder(ConversionSequenceBuilder&&)            = default;
   ConversionSequenceBuilder &operator=(ConversionSequenceBuilder&&) = default;

   ConversionSequenceBuilder(const ConversionSequenceBuilder&)            = delete;
   ConversionSequenceBuilder &operator=(const ConversionSequenceBuilder&) = delete;

   static ConversionSequenceBuilder MakeNoop(QualType T)
   {
      ConversionSequenceBuilder Seq;
      Seq.addStep(CastKind::NoOp, T);

      return Seq;
   }

   void setDependent(bool D) { Dependent = D; }
   bool isDependent() const { return Dependent; }

   unsigned getPenalty() const { return Penalty; }

   void addStep(CastKind kind, QualType resultType)
   {
      Steps.emplace_back(kind, resultType);
      Penalty += ::cdot::getPenalty(kind);
   }

   void addStep(CastKind kind, QualType resultType, CastStrength Strength)
   {
      Steps.emplace_back(kind, resultType);
      updateStrength(Strength);
      Penalty += ::cdot::getPenalty(kind);
   }

   void addStep(ast::CallableDecl *Decl, CastStrength Strength)
   {
      Steps.emplace_back(Decl);
      updateStrength(Strength);
      Penalty += 3;
   }

   void addHalt()
   {
      Steps.emplace_back();
   }

   void updateStrength(CastStrength Strength)
   {
      if (Strength > this->Strength)
         this->Strength = Strength;
   }

   void setStrength(CastStrength S) { Strength = S; }

   CastStrength getStrength() const { return Strength; }
   llvm::ArrayRef<ConversionStep> getSteps() const { return Steps; }
   bool isValid() const { return !Steps.empty(); }
   void invalidate() { Steps.clear(); }

   bool isNoOp() const
   {
      if (!isValid())
         return false;

      for (auto &Step : Steps)
         if (Step.getKind() != CastKind::NoOp)
            return false;

      return true;
   }

   bool isLValueToRValue() const
   {
      if (!isValid())
         return false;

      return Steps.size() == 1
         && Steps.front().getKind() == CastKind::LValueToRValue;
   }

   bool isImplicit() const
   {
      if (!isValid())
         return false;

      return Strength == CastStrength::Implicit;
   }
};

class ConversionSequence final: TrailingObjects<ConversionSequence,
                                                ConversionStep> {
   CastStrength Strength;
   unsigned NumSteps;
   unsigned Penalty;

   ConversionSequence(const ConversionSequenceBuilder &Builder,
                      QualType finalType);

   ConversionSequence(CastStrength Strength, ArrayRef<ConversionStep> Steps);

public:
   static ConversionSequence *Create(ast::ASTContext &C,
                                     const ConversionSequenceBuilder &Builder,
                                     QualType finalType = QualType());

   static ConversionSequence *Create(ast::ASTContext &C,
                                     CastStrength Strength,
                                     ArrayRef<ConversionStep> Steps);

   friend TrailingObjects;

   ConversionSequence(ConversionSequence&&)            = delete;
   ConversionSequence &operator=(ConversionSequence&&) = delete;

   ConversionSequence(const ConversionSequence&)            = delete;
   ConversionSequence &operator=(const ConversionSequence&) = delete;

   CastStrength getStrength() const { return Strength; }
   llvm::ArrayRef<ConversionStep> getSteps() const
   {
      return { getTrailingObjects<ConversionStep>(), NumSteps };
   }

   bool isValid() const { return NumSteps != 0; }
   unsigned getPenalty() const { return Penalty; }

   bool isNoOp() const
   {
      if (!isValid())
         return false;

      for (auto &Step : getSteps())
         if (Step.getKind() != CastKind::NoOp)
            return false;

      return true;
   }

   bool isImplicit() const
   {
      if (!isValid())
         return false;

      return Strength == CastStrength::Implicit;
   }
};

} // namespace cdot


#endif //CDOT_CONVERSIONSEQUENCE_H
