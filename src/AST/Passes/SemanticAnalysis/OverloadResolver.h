//
// Created by Jonas Zell on 08.11.17.
//

#ifndef CDOT_OVERLOADRESOLVER_H
#define CDOT_OVERLOADRESOLVER_H

#include <vector>
#include "../../../Util.h"

class Expression;
class TypeRef;

namespace cdot {

class Callable;

struct TemplateArg;
struct TemplateConstriant;
struct Argument;
struct Type;

class OverloadResolver {
   typedef std::function<Type(Expression*)> ArgResolverFn;
   typedef std::function<BuiltinType*
      (TypeRef*, const std::vector<TemplateArg>&,
       const std::vector<TemplateConstraint>&)> TypeResolverFn;

public:
   OverloadResolver(const std::vector<Argument> &givenArgs,
                    const std::vector<TemplateArg> &givenTemplateArgs,
                    const ArgResolverFn &argResolver,
                    const TypeResolverFn &typeResolver,
                    const std::vector<TemplateConstraint> &Constraints);

   CallCompatability checkIfViable(Callable *callable);

   typedef std::shared_ptr<TypeRef> TemplateParameter;
   typedef std::vector<pair<size_t, bool>> ArgOrder;

   enum InferenceStatus {
      Inf_Success,
      Inf_CouldNotInfer,
      Inf_SubstituationFailure
   };

   InferenceStatus inferTemplateArgs(const std::vector<Type> &givenArgs,
                                     std::vector<TemplateParameter> &neededArgs,
                                     std::vector<TemplateArg>& templateArgs);

   InferenceStatus inferTemplateArgs(const std::vector<Argument> &givenArgs,
                                     std::vector<TemplateParameter> &neededArgs,
                                     std::vector<TemplateArg>& templateArgs);

   static void isCallCompatible(CallCompatability &comp,
                                const std::vector<Argument> &givenArgs,
                                const std::vector<Argument> &neededArgs,
                                ArgResolverFn &argResolver);

   static void isCallCompatible(CallCompatability &comp,
                                const std::vector<Type> &givenArgs,
                                const std::vector<Argument> &neededArgs,
                                size_t checkUntil = 0);

protected:
   const std::vector<Argument> &givenArgs;
   const std::vector<TemplateArg> &givenTemplateArgs;
   const ArgResolverFn &argResolver;
   const TypeResolverFn &typeResolver;
   const std::vector<TemplateConstraint> &Constraints;

   InferenceStatus inferTemplateArg(BuiltinType* given,
                                    BuiltinType *needed,
                                    std::vector<TemplateArg> &templateArgs);

   void resolveTemplateArgs(std::vector<TemplateParameter> &neededArgs,
                            std::vector<Argument> &resolvedNeededArgs,
                            const std::vector<TemplateArg>& templateArgs);

   static std::vector<Type> resolveContextual(const std::vector<Argument>&given,
                                            const std::vector<Argument> &needed,
                                              const ArgResolverFn &argResolver);

   static void isVarargCallCompatible(CallCompatability &comp,
                                      const std::vector<Type> &givenArgs,
                                      const std::vector<Argument> &neededArgs);

   ArgOrder reorderArgs(const std::vector<Argument>& givenArgs,
                        const std::vector<Argument>& neededArgs);
};

} // namespace cdot

#endif //CDOT_OVERLOADRESOLVER_H