//
// Created by Jonas Zell on 19.02.18.
//

#ifndef CDOT_CONVERSIONSEQUENCE_H
#define CDOT_CONVERSIONSEQUENCE_H

#include "Basic/CastKind.h"
#include "AST/Type.h"

namespace cdot {
namespace ast {
   class CallableDecl;
} // namespace ast

class ConversionStep {
   CastKind Kind : 7;
   bool Halt : 1;

   void *Data;

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

   QualType getResultType() const
   {
      assert(Kind != CastKind::ConversionOp && "does not store a result type!");
      return QualType::getFromOpaquePtr(Data);
   }

   ast::CallableDecl *getConversionOp() const
   {
      assert(Kind == CastKind::ConversionOp && "not a conversion op!");
      return reinterpret_cast<ast::CallableDecl*>(Data);
   }
};

class ConversionSequence {
   CastStrength Strength;
   std::vector<ConversionStep> Steps;

public:
   ConversionSequence()
      : Strength(CastStrength::Implicit)
   { }

   ConversionSequence(const ConversionSequence&) = delete;
   ConversionSequence(ConversionSequence&&)      = default;

   ConversionSequence &operator=(const ConversionSequence&) = delete;
   ConversionSequence &operator=(ConversionSequence&&)      = default;

   void addStep(CastKind kind, QualType resultType)
   {
      Steps.emplace_back(kind, resultType);
   }

   void addStep(CastKind kind, QualType resultType, CastStrength Strength)
   {
      Steps.emplace_back(kind, resultType);
      updateStrength(Strength);
   }

   void addStep(ast::CallableDecl *Decl, CastStrength Strength)
   {
      Steps.emplace_back(Decl);
      updateStrength(Strength);
   }

   void addHalt()
   {
      Steps.emplace_back();
   }

   void updateStrength(CastStrength Strength)
   {
      if (Strength > this->Strength)
         ConversionSequence::Strength = Strength;
   }

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

   bool isImplicit() const
   {
      if (!isValid())
         return false;

      return Strength == CastStrength::Implicit;
   }
};

} // namespace cdot


#endif //CDOT_CONVERSIONSEQUENCE_H
