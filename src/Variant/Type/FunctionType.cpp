//
// Created by Jonas Zell on 13.08.17.
//

#include "FunctionType.h"
#include "ObjectType.h"
#include "../../AST/Expression/TypeRef.h"

namespace cdot {

   FunctionType::FunctionType(std::shared_ptr<TypeRef> returnType,
      std::vector<pair<string, std::shared_ptr<TypeRef>>>&& argTypes,
      std::vector<ObjectType*>&& genericTypes) :
      rawReturnType(returnType),
      rawArgTypes(argTypes),
      genericTypes(genericTypes)
   {
      id = TypeID::FunctionTypeID;
      className = "__lambda";
   }

   FunctionType::FunctionType(std::shared_ptr<TypeRef> returnType,
      std::vector<pair<string, std::shared_ptr<TypeRef>>>& argTypes) :
      rawReturnType(returnType),
      rawArgTypes(argTypes),
      isLambda_(true)
   {
      id = TypeID::FunctionTypeID;
      className = "__lambda";
   }

   FunctionType::FunctionType(Type* returnType, std::vector<Type*>&& argTypes,
      std::vector<ObjectType*>&& genericTypes) :
      returnType(returnType),
      argTypes(argTypes),
      genericTypes(genericTypes)
   {
      id = TypeID::FunctionTypeID;
      className = "__lambda";
   }

   FunctionType::FunctionType(Type* returnType, std::vector<Type*>& argTypes,
      std::vector<ObjectType*> &genericTypes) :
      returnType(returnType),
      argTypes(argTypes),
      genericTypes(genericTypes)
   {
      id = TypeID::FunctionTypeID;
      className = "__lambda";
   }

   FunctionType::FunctionType(Type* returnType, std::vector<Type*>& argTypes) :
      returnType(returnType),
      argTypes(argTypes),
      isLambda_(true)
   {
      id = TypeID::FunctionTypeID;
      className = "__lambda";
   }

   FunctionType::~FunctionType() {
      delete returnType;
      for (const auto& gen : genericTypes) {
         delete gen;
      }
      for (const auto& gen : argTypes) {
         delete gen;
      }
   }

   void FunctionType::visitContained(TypeCheckVisitor &v) {
      rawReturnType->accept(v);
      returnType = rawReturnType->getType()->deepCopy();

      for (const auto& arg : rawArgTypes) {
         arg.second->accept(v);
         argTypes.push_back(arg.second->getType()->deepCopy());
      }
   }

   bool FunctionType::operator==(Type *&other) {
      switch (other->getTypeID()) {
         case TypeID::FunctionTypeID: {
            auto asFun = cast<FunctionType>(other);
            if (!Type::operator==(other)) {
               return false;
            }

            if (*returnType != asFun->returnType) {
               return false;
            }

            size_t i = 0;
            for (const auto& arg : argTypes) {
               if (*arg != asFun->argTypes.at(i)) {
                  return false;
               }
               ++i;
            }

            i = 0;
            for (auto gen : genericTypes) {
               Type* otherGen = asFun->genericTypes.at(i);
               if (*gen != otherGen) {
                  return false;
               }
               ++i;
            }

            return true;
         }
         default:
            return false;
      }
   }

   std::vector<Type*> FunctionType::getContainedTypes(bool includeSelf) {
      std::vector<Type*> cont;

      if (includeSelf) {
         cont.push_back(this);
      }

      cont.push_back(returnType);
      cont.insert(cont.end(), argTypes.begin(), argTypes.end());

      return cont;
   }

   std::vector<Type**> FunctionType::getTypeReferences() {
      std::vector<Type**> cont{&returnType};

      for (auto& arg : argTypes) {
         cont.push_back(&arg);
      }

      return cont;
   }

   Type* FunctionType::deepCopy() {
      auto newTy = new FunctionType(*this);
      newTy->returnType = returnType->deepCopy();

      size_t i = 0;
      for (const auto& arg : argTypes) {
         newTy->argTypes[i] = arg->deepCopy();
         ++i;
      }

      i = 0;
      for (const auto& gen : newTy->genericTypes) {
         newTy->genericTypes[i] = cast<ObjectType>(gen->deepCopy());
         ++i;
      }

      return newTy;
   }

   bool FunctionType::implicitlyCastableTo(Type *other) {
      switch (other->getTypeID()) {
         case TypeID::AutoTypeID:
            return true;
         case TypeID::FunctionTypeID: {
            auto asFun = cast<FunctionType>(other);

            if (argTypes.size() != asFun->argTypes.size()) {
               return false;
            }

            if (!returnType->implicitlyCastableTo(asFun->returnType)) {
               return false;
            }

            size_t i = 0;
            for (const auto& arg : argTypes) {
               if (!arg->implicitlyCastableTo(asFun->argTypes.at(i))) {
                  return false;
               }
            }

            return true;
         }
         case TypeID::PointerTypeID:
            return false;
         case TypeID::ObjectTypeID:
            return false;
         case TypeID::CollectionTypeID:
            return false;
         case TypeID::IntegerTypeID:
            return false;
         case TypeID::FPTypeID:
            return false;
         default:
            return false;
      }
   }

   llvm::Type* FunctionType::_getLlvmType() {
      llvm::Type* ret = returnType->getLlvmType();
      std::vector<llvm::Type*> args{ Builder->getInt8PtrTy()->getPointerTo() };

      for (const auto& arg : argTypes) {
         args.push_back(arg->getLlvmType());
      }

      return llvm::FunctionType::get(ret, args, false);
   }

   string FunctionType::toString() {
      string res = "(";

      size_t i = 0;
      for (const auto& arg : argTypes) {
         res += arg->toString();
         if (i < argTypes.size() - 1) {
            res += ", ";
         }
      }

      return res + ") -> " + returnType->toString();
   }

} // namespace cdot