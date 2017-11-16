//
// Created by Jonas Zell on 13.08.17.
//

#include "FunctionType.h"
#include "ObjectType.h"
#include "PointerType.h"

#include "../../AST/Passes/SemanticAnalysis/Function.h"
#include "../../AST/Expression/TypeRef.h"
#include "../../AST/Passes/CodeGen/CodeGen.h"

namespace cdot {

unordered_map<string, FunctionType*> FunctionType::Instances;

FunctionType* FunctionType::get(
   Type &returnType,
   std::vector<Argument> &argTypes,
   bool isRawFunctionTy)
{
   auto key = typesToString(returnType, argTypes) + std::to_string((int)isRawFunctionTy);
   if (Instances.find(key) == Instances.end()) {
      Instances[key] = new FunctionType(returnType, argTypes, isRawFunctionTy);
   }

   return Instances[key];
}

string FunctionType::typesToString(
   const Type &returnType,
   const std::vector<Argument> &argTypes)
{
   string res = "(";
   size_t i = 0;
   for (const auto& arg : argTypes) {
      res += arg.type.toString();
      if (i < argTypes.size() - 1) {
         res += ", ";
      }

      ++i;
   }

   return res + ") -> " + returnType->toString();
}

FunctionType::FunctionType(Type& returnType, std::vector<Argument>& argTypes, bool raw) :
   returnType(returnType),
   argTypes(argTypes),
   isRawFunctionTy_(raw)
{
   id = TypeID::FunctionTypeID;
   className = "__lambda";
}

bool FunctionType::implicitlyCastableTo(BuiltinType *other) const
{
   switch (other->getTypeID()) {
      case TypeID::AutoTypeID:
         return true;
      case TypeID::FunctionTypeID: {
         auto asFun = cast<FunctionType>(other);

         if (argTypes.size() != asFun->argTypes.size()) {
            return false;
         }

         if (!returnType.implicitlyCastableTo(asFun->returnType)) {
            return false;
         }

         size_t i = 0;
         for (auto& arg : argTypes) {
            if (!arg.type.implicitlyCastableTo(asFun->argTypes.at(i).type)) {
               return false;
            }

            ++i;
         }

         return true;
      }
      case TypeID::PointerTypeID: {
         return isRawFunctionTy_ &&
            implicitlyCastableTo(*other->asPointerTy()->getPointeeType());
      }
      case TypeID::IntegerTypeID: {
         return isRawFunctionTy_;
      }
      default:
         return false;
   }
}

size_t FunctionType::getSize() const
{
   if (!isRawFunctionTy_) {
      return 2 * sizeof(int *);
   }

   return sizeof(void (*)(void));
}

llvm::Type* FunctionType::getLlvmType() const
{
   if (!isRawFunctionTy_) {
      return CodeGen::LambdaTy;
   }

   return getLlvmFunctionType()->getPointerTo();
}

llvm::Type* FunctionType::getLlvmFunctionType() const
{
   std::vector<llvm::Type*> args;
   if (!isRawFunctionTy_) {
      args.push_back(CodeGen::Int8PtrTy->getPointerTo());
   }

   for (auto& arg : argTypes) {
      auto llvmTy = arg.type->getLlvmType();
      if (llvmTy->isStructTy()) {
         llvmTy = llvmTy->getPointerTo();
      }

      args.push_back(llvmTy);
   }

   llvm::Type* ret = returnType->getLlvmType();
   if (returnType->needsStructReturn()) {
      args.insert(++args.begin(), ret->getPointerTo());
      ret = llvm::Type::getVoidTy(CodeGen::Context);
   }

   return llvm::FunctionType::get(ret, args, false);
}

string FunctionType::toString() const
{
   return typesToString(returnType, argTypes);
}

} // namespace cdot