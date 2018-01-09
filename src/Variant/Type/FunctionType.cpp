//
// Created by Jonas Zell on 13.08.17.
//

#include <llvm/Support/raw_ostream.h>
#include <sstream>

#include "FunctionType.h"
#include "ObjectType.h"
#include "PointerType.h"
#include "GenericType.h"

#include "../../AST/Passes/SemanticAnalysis/Function.h"
#include "../../AST/Expression/TypeRef.h"
#include "../../AST/Passes/CodeGen/CodeGen.h"

using namespace cdot::support;

namespace cdot {

FunctionType* FunctionType::get(const QualType &returnType,
                                const std::vector<Argument> &argTypes,
                                bool isRawFunctionTy) {
   return get(returnType,
              std::vector<Argument>(argTypes),
              isRawFunctionTy);
}

FunctionType* FunctionType::get(const QualType &returnType,
                                std::vector<Argument> &&argTypes,
                                bool isRawFunctionTy) {
   auto key = typesToString(returnType, argTypes)
              + std::to_string((int)isRawFunctionTy) + ".__fun";

   if (Instances.find(key) == Instances.end()) {
      Instances.try_emplace(key, new FunctionType(returnType, move(argTypes),
                                                  isRawFunctionTy));
   }

   assert(Instances.find(key) != Instances.end());

   return cast<FunctionType>(Instances[key]);
}

FunctionType* FunctionType::get(const QualType &returnType,
                                const std::vector<Type *> &argTypes,
                                bool isRawFunctionTy) {
   std::vector<Argument> args;
   args.reserve(argTypes.size());
   for (const auto &ty : argTypes) {
      args.emplace_back("", QualType(ty));
   }

   return get(returnType, args, isRawFunctionTy);
}

FunctionType* FunctionType::get(llvm::ArrayRef<Type *> tys)
{
   auto it = tys.begin();

   QualType ret(*it);
   ++it;

   std::vector<Argument> args;
   for (; it != tys.end(); ++it) {
      args.emplace_back("", QualType(*it));
   }

   return get(ret, move(args), true);
}

string FunctionType::typesToString(const QualType &returnType,
                                   const std::vector<Argument> &argTypes) {
   std::ostringstream res;
   res << "(";

   size_t i = 0;
   for (const auto& arg : argTypes) {
      if (arg.type.isLvalue())
         res << "ref ";

      res << arg.type->toUniqueString();

      if (i < argTypes.size() - 1) {
         res << ", ";
      }

      ++i;
   }

   res << ") -> " << returnType->toUniqueString();
   return res.str();
}

FunctionType::FunctionType(const QualType& returnType,
                           const std::vector<Argument>&argTypes,
                           bool raw) :
   returnType(returnType),
   argTypes(argTypes),
   isRawFunctionTy_(raw)
{
   id = TypeID::FunctionTypeID;
}

FunctionType::FunctionType(const QualType &returnType,
                           std::vector<Argument> &&argTypes, bool raw)
   : returnType(returnType),
     argTypes(move(argTypes)),
     isRawFunctionTy_(raw)
{
   id = TypeID::FunctionTypeID;
}

size_t FunctionType::getSize() const
{
   if (!isRawFunctionTy_) {
      return 2 * sizeof(void*);
   }

   return sizeof(void*);
}

string FunctionType::toString() const
{
   return typesToString(returnType, argTypes);
}

} // namespace cdot