//
// Created by Jonas Zell on 08.11.17.
//

#ifndef CDOT_OVERLOADRESOLVER_H
#define CDOT_OVERLOADRESOLVER_H

#include <vector>
#include "../../../Util.h"
#include "../../../Message/Diagnostics.h"
#include "Template.h"

namespace cdot {

class Callable;

class TemplateArg;
struct TemplateConstriant;
struct Argument;
class QualType;
class FunctionType;

namespace cl {

struct EnumCase;

} // namespace cl

namespace ast {

class Expression;
class TypeRef;
class SemaPass;
class StaticExpr;

enum class FailureReason : unsigned char {
   None,
   IncompatibleArgCount,
   IncompatibleArgument,
   IncompatibleTemplateArg,
   CouldNotInferTemplateArg,
   FailedConstraint
};

struct CallCompatability {
   bool isCompatible() const
   {
      return failureReason == FailureReason::None;
   }

   bool isPerfectMatch()   const { return perfectMatch; }
   size_t getCastPenalty() const { return castPenalty; }

   FailureReason getFailureReason() const
   {
      return failureReason;
   }

   FailureReason failureReason = FailureReason::None;

   bool perfectMatch = false;

   union {
      size_t castPenalty = 0;
      StaticExpr *failedConstraint;
   };

   std::vector<QualType> resolvedArgs;
   std::vector<Argument> resolvedNeededArgs;
   sema::TemplateArgList templateArgList;
   sema::TemplateArgList initializerTemplateArgList;

   size_t incompatibleArg;
   llvm::SmallVector<diag::DiagnosticBuilder, 4> diagnostics;
};

class OverloadResolver {
public:
   OverloadResolver(SemaPass &SP,
                    const std::vector<Argument> &givenArgs,
                    const std::vector<TemplateArg> &givenTemplateArgs,
                    SourceLocation callerLoc = {});

   CallCompatability checkIfViable(Callable *callable);
   CallCompatability checkIfViable(FunctionType *funcTy);
   CallCompatability checkIfViable(cl::EnumCase const &Case);

   typedef std::vector<pair<size_t, bool>> ArgOrder;

   enum InferenceStatus {
      Inf_Success,
      Inf_CouldNotInfer,
      Inf_SubstituationFailure
   };

   static void isCallCompatible(CallCompatability &comp,
                                const std::vector<Argument> &givenArgs,
                                const std::vector<Argument> &neededArgs);

   static void isCallCompatible(CallCompatability &comp,
                                const std::vector<QualType> &givenArgs,
                                const std::vector<Argument> &neededArgs,
                                size_t checkUntil = 0);

protected:
   SemaPass &SP;
   const std::vector<Argument> &givenArgs;
   const std::vector<TemplateArg> &givenTemplateArgs;
   SourceLocation callerLoc;

   void resolveTemplateArgs(std::vector<Argument> &resolvedNeededArgs,
                            sema::TemplateArgList const& templateArgs);

   static std::vector<QualType> resolveContextual(
                                          const std::vector<Argument>&given,
                                          const std::vector<Argument> &needed);

   static void isVarargCallCompatible(CallCompatability &comp,
                                      const std::vector<QualType> &givenArgs,
                                      const std::vector<Argument> &neededArgs);

   ArgOrder reorderArgs(const std::vector<Argument>& givenArgs,
                        const std::vector<Argument>& neededArgs);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_OVERLOADRESOLVER_H
