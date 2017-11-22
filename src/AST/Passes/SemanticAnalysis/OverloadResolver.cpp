//
// Created by Jonas Zell on 08.11.17.
//

#include "OverloadResolver.h"

#include "Function.h"
#include "Record/Class.h"

#include "../../../Variant/Type/PointerType.h"
#include "../../../Variant/Type/FunctionType.h"
#include "../../../Variant/Type/FPType.h"
#include "../../../Variant/Type/TupleType.h"
#include "../../../Variant/Type/GenericType.h"

namespace cdot {

OverloadResolver::OverloadResolver(
   const std::vector<Argument> &givenArgs,
   const std::vector<TemplateArg> &givenTemplateArgs,
   const ArgResolverFn &argResolver,
   const TypeResolverFn &typeResolver,
   const std::vector<TemplateConstraint> &Constraints)
   : givenArgs(givenArgs), givenTemplateArgs(givenTemplateArgs),
     argResolver(argResolver), typeResolver(typeResolver),
     Constraints(Constraints)
{

}

CallCompatability OverloadResolver::checkIfViable(Callable *callable)
{
   CallCompatability comp;
   comp.compatibility = CompatibilityType::NO_MATCHING_CALL;

   auto &neededArgs = callable->getArguments();
   auto resolvedGivenArgs = resolveContextual(givenArgs,
                                              neededArgs,
                                              argResolver);

   // if the function is templated and no generics are given, try to infer
   // the needed template parameters, then resolve the needed arguments with
   // the given template type parameters
   // if the type substitution would fail, no error is thrown but this
   // overload will not be considered viable (like C++ SFINAE)
   auto resolvedNeededArgs = neededArgs;
   if (callable->isTemplate()) {
      auto &Constraints = callable->getTemplate()->constraints;

      bool isViable;
      if (givenTemplateArgs.empty()) {
         std::vector<TemplateArg> inferredTemplateArgs;
         auto Status = inferTemplateArgs(resolvedGivenArgs,
                                         callable->getTemplate()->args,
                                         inferredTemplateArgs);

         if (Status != Inf_Success) {
            return comp;
         }

         resolveTemplateArgs(callable->getTemplate()->args,
                             resolvedNeededArgs, inferredTemplateArgs);

         isViable = resolvedNeededArgs.size() == callable->getTemplate()
                                                         ->args.size();

         // check if the return type would be viable
         auto &retType = callable->getTemplate()->returnType;
         if (isViable && retType) {
            isViable =
               typeResolver(retType.get(),
                            inferredTemplateArgs, Constraints) != nullptr;
         }

         comp.generics = std::move(inferredTemplateArgs);
      }
      else {
         resolveTemplateArgs(callable->getTemplate()->args,
                             resolvedNeededArgs, givenTemplateArgs);

         isViable = resolvedNeededArgs.size() == callable->getTemplate()
                                                         ->args.size();

         // check if the return type would be viable
         auto &retType = callable->getTemplate()->returnType;
         if (isViable && retType) {
            isViable = typeResolver(retType.get(),
                                    givenTemplateArgs, Constraints) != nullptr;
         }

         comp.generics = givenTemplateArgs;
      }

      if (!isViable) {
         comp.compatibility = CompatibilityType::NO_MATCHING_CALL;
         return comp;
      }
   }

   isCallCompatible(comp, resolvedGivenArgs, resolvedNeededArgs);

   comp.argOrder = reorderArgs(givenArgs, resolvedNeededArgs);
   comp.resolvedArgs = std::move(resolvedGivenArgs);
   comp.resolvedNeededArgs = std::move(resolvedNeededArgs);

   return comp;
}

std::vector<QualType> OverloadResolver::resolveContextual(
   const std::vector<Argument> &givenArgs,
   const std::vector<Argument> &neededArgs,
   const ArgResolverFn &argResolver)
{
   std::vector<QualType> resolvedGivenArgs;
   size_t i = 0;

   // Resolve passed arguments that depend on contextual information
   for (const auto &neededArg : neededArgs) {
      if (i >= givenArgs.size()) {
         break;
      }

      const auto &given = givenArgs[i];
//      const auto &argVal = given.defaultVal;

//      if (argVal && argVal->needsContextualInformation()) {
//         argVal->setContextualType(neededArg.type);
//         resolvedGivenArgs.push_back(argResolver(argVal.get()));
//      }
//      else {
         resolvedGivenArgs.push_back(given.type);
//      }

      ++i;
   }

   // resolve varargs that depend on contextual information
   for (; i < givenArgs.size(); ++i) {
      const auto &given = givenArgs[i];
      const auto &argVal = given.defaultVal;

      if (argVal && argVal->needsContextualInformation()) {
         resolvedGivenArgs.push_back(argResolver(argVal.get()));
      }
      else {
         resolvedGivenArgs.push_back(given.type);
      }
   }

   return resolvedGivenArgs;
}

void
OverloadResolver::isCallCompatible(CallCompatability &comp,
                                   const std::vector<Argument> &givenArgs,
                                   const std::vector<Argument> &neededArgs,
                                   ArgResolverFn &argResolver) {
   return isCallCompatible(comp, resolveContextual(givenArgs, neededArgs,
                                                   argResolver), neededArgs);
}

namespace {

size_t castPenalty(const QualType &from, const QualType &to)
{
   size_t penalty = 0;
   switch (from->getTypeID()) {
      case TypeID::IntegerTypeID: {
         if (to->isIntegerTy()) {
            auto fromBW = from->getBitwidth();
            auto toBW = to->getBitwidth();
            auto fromUnsigned = from->isUnsigned();
            auto toUnsigned = to->isUnsigned();

            if (fromBW > toBW) {
               ++penalty;
            }
            else if (fromUnsigned != toUnsigned) {
               ++penalty;
            }
         }
         else if (to->isFPType()) {
            penalty += 2;
         }
         else if (to->isBoxedPrimitive() && to->unbox()->isIntegerTy()) {
            if (to->unbox() != *from) {
               ++penalty;
            }
         }
         else {
            penalty += 3;
         }
         break;
      }
      case TypeID::FPTypeID: {
         if (to->isFPType()) {
            if (to->asFloatingTy()->getPrecision()
                < from->asFloatingTy()->getPrecision()) {
               ++penalty;
            }
         }
         else if (to->isIntegerTy()) {
            penalty += 2;
         }
         else if (to->isBoxedPrimitive() && to->unbox()->isFloatTy()) {
            if (to->unbox() != *from) {
               ++penalty;
            }
         }
         else {
            penalty += 3;
         }
         break;
      }
      case TypeID::PointerTypeID: {
         if (to->isPointerTy() || to->isRawFunctionTy()) {
            ++penalty;
         }
         else if (to->isIntegerTy()) {
            penalty += 2;
         }
         else {
            penalty += 3;
         }
         break;
      }
      case TypeID::ObjectTypeID: {
         auto fromObj = from->asObjTy();

         if (to->isObjectTy()) {
            auto fromRec = from->getRecord()->getAs<Class>();
            auto toRec = to->getRecord()->getAs<Class>();

            if (fromRec->getName() == toRec->getName()) {
               ++penalty;
            }
               // upcast
            else if (toRec->isBaseClassOf(fromRec->getName())) {
               ++penalty;
            }
               // downcast
            else if (fromRec->isBaseClassOf(toRec->getName())) {
               penalty += 3;
            }
            else if (fromRec->isProtocol() == toRec->isProtocol()) {
               penalty += 2;
            }
            else {
               penalty += 3;
            }
         }
         else if (from->isBoxedPrimitive() && to->isNumeric()) {
            if (from->unbox() == *to) {
               ++penalty;
            }
            else {
               penalty += 2;
            }
         }
         else {
            penalty += 3;
         }

         break;
      }
      case TypeID::TupleTypeID: {
         if (to->isTupleTy()) {
            penalty += 2;
         }
         else {
            penalty += 3;
         }

         break;
      }
      default:penalty += 3;
         break;
   }

   return penalty;
}

} // anonympus namespace

void
OverloadResolver::isCallCompatible(CallCompatability &comp,
                                   const std::vector<QualType> &givenArgs,
                                   const std::vector<Argument> &neededArgs,
                                   size_t checkUntil) {
   comp.incompatibleArg = 0;

   size_t numGivenArgs = givenArgs.size();
   size_t numNeededArgs = neededArgs.size();
   size_t i = 0;
   bool perfect_match = true;

   if (numGivenArgs == 0 && numNeededArgs == 0) {
      comp.compatibility = CompatibilityType::COMPATIBLE;
      comp.perfectMatch = true;

      return;
   }

   if (!checkUntil) {
      auto isVararg = numNeededArgs > 0 && neededArgs.back().isVararg;
      if (numGivenArgs > numNeededArgs && !isVararg) {
         return;
      }

      if (isVararg) {
         return isVarargCallCompatible(comp, givenArgs, neededArgs);
      }
   }

   for (auto &neededArg : neededArgs) {
      if (checkUntil && i >= checkUntil) {
         break;
      }

      auto& needed = neededArg;
      if (i < numGivenArgs && !givenArgs[i]->isAutoTy()) {
         auto& givenTy = givenArgs.at(i);

         if (!givenTy.implicitlyCastableTo(needed.type)) {
            comp.incompatibleArg = i;
            return;
         }
         else if (givenTy != needed.type && !givenTy->isTypeGroup()) {
            perfect_match = false;
            comp.castPenalty += castPenalty(givenTy, needed.type);
         }
      }
      else if (needed.defaultVal == nullptr) {
         comp.incompatibleArg = i;
         return;
      }

      ++i;
   }

   comp.compatibility = CompatibilityType::COMPATIBLE;
   comp.perfectMatch = perfect_match;
}

void
OverloadResolver::isVarargCallCompatible(CallCompatability &comp,
                                         const std::vector<QualType> &givenArgs,
                                      const std::vector<Argument> &neededArgs) {
   comp.incompatibleArg = 0;

   if (givenArgs.size() < neededArgs.size() - 1) {
      return;
   }

   // check arguments up until vararg
   size_t varargIndex = neededArgs.size() - 1;
   CallCompatability withoutVararg;
   isCallCompatible(withoutVararg, givenArgs, neededArgs, varargIndex);

   if (!withoutVararg.isCompatible()) {
      comp = withoutVararg;
      return;
   }

   bool cstyle = neededArgs.back().cstyleVararg;
   if (cstyle) {
      comp.compatibility = CompatibilityType::COMPATIBLE;
      return;
   }

   bool perfectMatch = withoutVararg.perfectMatch;
   auto vaTy = neededArgs.back().type;
   for (size_t i = 0; i < givenArgs.size(); ++i) {
      auto& given = givenArgs[i];

      if (!given.implicitlyCastableTo(vaTy)) {
         comp.incompatibleArg = i;
         return;
      }
      else if (!vaTy->isAutoTy() && given != vaTy && !given->isTypeGroup()) {
         comp.castPenalty += castPenalty(given, vaTy);
         perfectMatch = false;
      }
   }

   comp.compatibility = CompatibilityType::COMPATIBLE;
   comp.perfectMatch = perfectMatch;
}

OverloadResolver::InferenceStatus
OverloadResolver::inferTemplateArg(Type *given,
                                   Type *needed,
                                   std::vector<TemplateArg> &templateArgs) {
   if (needed->isPointerTy()) {
      if (!given->isPointerTy()) {
         return Inf_SubstituationFailure;
      }

      auto givenPointee = *given->asPointerTy()->getPointeeType();
      auto neededPointee = *needed->asPointerTy()->getPointeeType();

      auto Status = inferTemplateArg(givenPointee, neededPointee, templateArgs);
      if (Status == Inf_SubstituationFailure) {
         return Inf_SubstituationFailure;
      }
   }

   if (needed->isFunctionTy()) {
      if (!given->isFunctionTy()) {
         return Inf_SubstituationFailure;
      }

      auto givenFunc = given->asFunctionTy();
      auto neededFunc = needed->asFunctionTy();

      if (givenFunc->getArgTypes().size() != neededFunc->getArgTypes().size()) {
         return Inf_SubstituationFailure;
      }

      auto givenRet = *givenFunc->getReturnType();
      auto neededRet = *neededFunc->getReturnType();

      auto Status = inferTemplateArg(givenRet, neededRet, templateArgs);
      if (Status == Inf_SubstituationFailure) {
         return Inf_SubstituationFailure;
      }

      size_t i = 0;
      auto &neededArgs = neededFunc->getArgTypes();
      for (auto &given : givenFunc->getArgTypes()) {
         Status = inferTemplateArg(*given.type, *neededArgs[i].type,
                                   templateArgs);
         if (Status == Inf_SubstituationFailure) {
            return Inf_SubstituationFailure;
         }

         ++i;
      }

      return Status;
   }

   if (needed->isTupleTy()) {
      if (!given->isTupleTy()) {
         return Inf_SubstituationFailure;
      }

      auto givenTuple = given->asTupleTy();
      auto neededTuple = needed->asTupleTy();
      InferenceStatus Status = Inf_Success;
      size_t i = 0;

      auto &neededTys = neededTuple->getContainedTypes();
      for (auto &given : givenTuple->getContainedTypes()) {
         Status = inferTemplateArg(given.second, neededTys[i].second,
                                   templateArgs);
         if (Status == Inf_SubstituationFailure) {
            return Inf_SubstituationFailure;
         }

         ++i;
      }

      return Status;
   }

   if (needed->isObjectTy() && !needed->isGenericTy()) {
      if (!given->isObjectTy()) {
         return Inf_SubstituationFailure;
      }

      InferenceStatus Status = Inf_SubstituationFailure;
      if (given->hasTemplateArgs() && needed->hasTemplateArgs()) {
         auto &givenConcrete = given->getTemplateArgs();
         auto &neededConcrete = needed->getTemplateArgs();

         if (givenConcrete.size() != neededConcrete.size()) {
            return Inf_SubstituationFailure;
         }

         Status = Inf_Success;
         size_t i = 0;
         for (const auto &gen : givenConcrete) {
            if (!gen.isTypeName()) {
               continue;
            }

            Status = inferTemplateArg(gen.getGenericTy(),
                                      neededConcrete[i].getGenericTy(),
                                      templateArgs);

            if (Status == Inf_SubstituationFailure) {
               return Inf_SubstituationFailure;
            }

            ++i;
         }
      }

      return Status;
   }

   if (!needed->isGenericTy()) {
      return Inf_CouldNotInfer;
   }

   auto neededGen = needed->asGenericTy();
   auto &neededTypeName = neededGen->getClassName();

   auto it = std::find_if(Constraints.begin(), Constraints.end(),
                          [neededTypeName](const TemplateConstraint &constr) {
                             return constr.genericTypeName == neededTypeName;
                          });

   assert(it != Constraints.end());
   auto index = std::distance(Constraints.begin(), it);

   bool isNew = !templateArgs[index].getGenericTy();
   if (isNew) {
      templateArgs[index].resolveType(GenericType::get(neededTypeName, given));
   }

   if (!GenericTypesCompatible(templateArgs[index].getGenericTy(),
                               Constraints[index])) {
      return Inf_SubstituationFailure;
   }

   if (isNew) {
      return Inf_Success;
   }
   else {
      return Inf_CouldNotInfer;
   }
}

OverloadResolver::InferenceStatus
OverloadResolver::inferTemplateArgs(const std::vector<QualType> &givenArgs,
                                    std::vector<TemplateParameter>& neededArgs,
                                    std::vector<TemplateArg>& templateArgs) {
   if (neededArgs.empty() || !templateArgs.empty()) {
      return Inf_Success;
   }

   size_t needed = 0;
   for (const auto &Const : Constraints) {
      if (Const.kind == TemplateConstraint::TypeName) {
         ++needed;
         templateArgs.emplace_back();
      }
   }

   size_t inferredTypes = 0;
   size_t i = 0;
   bool isSubstFailure = false;

   for (const auto &arg : givenArgs) {
      if (i >= neededArgs.size()) {
         break;
      }
      if (inferredTypes == needed) {
         break;
      }

      auto needed = typeResolver(neededArgs[i].get(), templateArgs,
                                 Constraints);
      if (!needed) {
         return Inf_SubstituationFailure;
      }

      auto Status = inferTemplateArg(*arg, needed, templateArgs);

      if (Status == Inf_SubstituationFailure) {
         isSubstFailure = true;
         goto cleanup;
      }
      else if (Status == Inf_Success) {
         ++inferredTypes;
      }

      ++i;
   }

   cleanup:

   auto it = templateArgs.begin();
   while (it != templateArgs.end()) {
      auto &gen = *it;
      if (gen.isTypeName() && !gen.getGenericTy()) {
         it = templateArgs.erase(it);
      }
      else {
         ++it;
      }
   }

   if (isSubstFailure) {
      return Inf_SubstituationFailure;
   }

   if (inferredTypes != needed) {
      return Inf_CouldNotInfer;
   }

   return Inf_Success;
}

OverloadResolver::InferenceStatus
OverloadResolver::inferTemplateArgs(const std::vector<Argument> &givenArgs,
                                    std::vector<TemplateParameter>& neededArgs,
                                    std::vector<TemplateArg>& templateArgs) {
   std::vector<QualType> tys;
   for (const auto &arg : givenArgs) {
      if (arg.type->isAutoTy() && arg.defaultVal) {
         tys.push_back(argResolver(arg.defaultVal.get()));
      }
      else {
         tys.push_back(arg.type);
      }
   }

   return inferTemplateArgs(tys, neededArgs, templateArgs);
}

void OverloadResolver::resolveTemplateArgs(
   std::vector<TemplateParameter> &neededArgs,
   std::vector<Argument> &resolvedNeededArgs,
   const std::vector<TemplateArg>& templateArgs)
{
   for (auto& arg : neededArgs) {
      resolvedNeededArgs.emplace_back();

      auto &next = resolvedNeededArgs.back();
      *next.type = typeResolver(arg.get(), templateArgs, Constraints);

      if (!*next.type) {
         resolvedNeededArgs.pop_back();
         return;
      }

      resolveGenerics(next.type, templateArgs);
   }
}

OverloadResolver::ArgOrder
OverloadResolver::reorderArgs(const std::vector<Argument> &givenArgs,
                              const std::vector<Argument> &neededArgs) {
   size_t i = 0;
   ArgOrder order;
   std::vector<pair<string, std::shared_ptr<Expression>>> orderedArgs;
   orderedArgs.reserve(neededArgs.size());

   // if no labels are supplied, just return the arguments in order
   bool hasLabels = false;
   auto firstLabelIndex = std::find_if(givenArgs.begin(), givenArgs.end(),
                                       [](const Argument &given) {
      return !given.label.empty();
   });

   if (firstLabelIndex == givenArgs.end()) {
      hasLabels = false;
   }

   for (const auto& arg : neededArgs) {
      auto labelIndex = std::find_if(givenArgs.begin(), givenArgs.end(),
                                     [arg](const Argument &given) {
         return given.label == arg.label;
      });

      // argument with this label was supplied
      if (hasLabels && labelIndex != givenArgs.end()) {
         auto distance = std::distance(givenArgs.begin(),labelIndex);
         orderedArgs.emplace_back(arg.label, givenArgs[distance].defaultVal);

         order.emplace_back(distance, false);
      }
         // no arg with label found
      else if (hasLabels) {
         return order;
      }
         // argument exists at position
      else if (givenArgs.size() > i) {
         orderedArgs.emplace_back(arg.label, givenArgs[i].defaultVal);
         order.emplace_back(i, false);
      }
         // default arg exists
      else if (arg.defaultVal != nullptr) {
         orderedArgs.emplace_back(arg.label, arg.defaultVal);
         order.emplace_back(i, true);
      }
         // incompatible
      else {
         return order;
      }

      ++i;
   }

   while (order.size() < givenArgs.size()) {
      order.emplace_back(i++, false);
   }

   return order;
}

} // namespace cdot