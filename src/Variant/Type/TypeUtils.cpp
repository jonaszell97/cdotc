//
// Created by Jonas Zell on 07.11.17.
//

#include "TypeUtils.h"

#include "../../AST/Passes/SemanticAnalysis/Record/Class.h"

#include "GenericType.h"
#include "PointerType.h"
#include "TupleType.h"
#include "FunctionType.h"


namespace cdot {
namespace util {

bool isSubTypeOf(BuiltinType *ty, BuiltinType *of)
{
   return *ty < *of;
}

bool operator<=(const BuiltinType &ty, const BuiltinType &of)
{
   if (ty.getTypeID() != of.getTypeID()) {
      return false;
   }

   if (ty.isAutoTy()) {
      return false;
   }

   if (ty.isVoidTy()) {
      return true;
   }

   // an integer type is considered a sub type of another integer type iff
   // their sign is the same and the bitwidth is lower
   if (ty.isIntegerTy()) {
      return ty.isUnsigned() == of.isUnsigned()
             && ty.getBitwidth() <= of.getBitwidth();
   }

   // float is a subtype of double
   if (ty.isFPType()) {
      return ty.isFloatTy();
   }

   // a pointer type is a subtype of another iff their pointee types
   // are subtypes
   if (ty.isPointerTy()) {
      return *static_cast<const PointerType*>(&ty)->getPointeeType()
             <= *static_cast<const PointerType*>(&of)->getPointeeType();
   }

   // a tuple type is a subtype of another tuple type if all elements are
   // subtypes
   if (ty.isTupleTy()) {
      auto &elementTys = static_cast<const TupleType*>(&ty)
         ->getContainedTypes();
      auto &otherElementTys = static_cast<const TupleType*>(&of)
         ->getContainedTypes();

      if (elementTys.size() != otherElementTys.size()) {
         return false;
      }

      size_t i = 0;
      for (const auto &ty : elementTys) {
         if (!(*ty.second <= *otherElementTys[i].second)) {
            return false;
         }
      }

      return true;
   }

   // function type - same as tuple, but the return type also has to be
   // a subtype
   if (ty.isFunctionTy()) {
      auto ret = static_cast<const FunctionType*>(&ty)->getReturnType();
      auto otherRet = static_cast<const FunctionType*>(&of)->getReturnType();

      if (!(*ret <= *otherRet)) {
         return false;
      }

      auto &argTys = static_cast<const FunctionType*>(&ty)->getArgTypes();
      auto &otherArgTys = static_cast<const FunctionType*>(&of)->getArgTypes();

      if (argTys.size() != otherArgTys.size()) {
         return false;
      }

      size_t i = 0;
      for (const auto &ty : argTys) {
         if (!(*ty.type <= *otherArgTys[i].type)) {
            return false;
         }
      }

      return true;
   }

   assert(ty.isObject());

   auto rec1 = ty.getRecord();
   auto rec2 = of.getRecord();

   if (rec1 == rec2) {
      return true;
   }

   if (!rec2->isClass()) {
      return false;
   }

   auto cl2 = rec2->getAs<Class>();
   if (!cl2->isBaseClassOf(rec1)) {
      return false;
   }

   auto cl1 = rec1->getAs<Class>();
   if (cl1->isTemplated()) {
      if (!cl2->isTemplated()) {
         return false;
      }

      auto &fstTemplateArgs = cl1->getTemplateArgs();
      auto &sndTemplateArgs = cl2->getTemplateArgs();

      if (fstTemplateArgs.size() != sndTemplateArgs.size()) {
         return false;
      }

      size_t i = 0;
      for (const auto &arg : fstTemplateArgs) {
         if (!arg.isTypeName()) {
            continue;
         }

         if (!(*arg.getGenericTy() <= *sndTemplateArgs[i].getGenericTy())) {
            return false;
         }

         ++i;
      }
   }

   return true;
}

bool isSubTypeOrEqual(BuiltinType *ty, BuiltinType *of)
{
   return *ty <= *of;
}

bool operator<(const BuiltinType &ty, const BuiltinType &of)
{
   return ty <= of && ty != of;
}

bool isSuperTypeOf(BuiltinType *ty, BuiltinType *of)
{
   return *ty > *of;
}

bool operator>(const BuiltinType &ty, const BuiltinType &of)
{
   return !(ty <= of);
}

bool isSuperTypeOrEqual(BuiltinType *ty, BuiltinType *of)
{
   return *ty >= *of;
}

bool operator>=(const BuiltinType &ty, const BuiltinType &of)
{
   return ty > of || ty == of;
}

bool isSameAs(BuiltinType *ty, BuiltinType *as)
{
   return *ty == *as;
}

bool operator==(const BuiltinType &ty, const BuiltinType &as)
{
   return &ty == &as;
}

bool isDifferentThan(BuiltinType *ty, BuiltinType *than)
{
   return *ty != *than;
}

bool operator!=(const BuiltinType &ty, const BuiltinType &than)
{
   return &ty != &than;
}

} // namespace util
} // namespace cdot