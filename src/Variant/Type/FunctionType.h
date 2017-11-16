//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_FUNCTIONTYPE_H
#define CDOT_FUNCTIONTYPE_H


#include "BuiltinType.h"
#include "../../AST/Passes/AbstractPass.h"
#include "Type.h"

using std::pair;

namespace cdot {

   struct Argument;
   class ObjectType;

   class FunctionType : public BuiltinType {
   protected:
      FunctionType(Type& returnType, std::vector<Argument>& argTypes, bool raw);

      static unordered_map<string, FunctionType*> Instances;
      static string typesToString(
         const Type& returnType,
         const std::vector<Argument>& argTypes
      );

   public:
      static FunctionType *get(
         Type& returnType,
         std::vector<Argument>& argTypes,
         bool isRawFunctionTy
      );

      Type getReturnType() const
      {
         return returnType;
      }

      const std::vector<Argument>& getArgTypes() const
      {
         return argTypes;
      }

      bool isFunctionTy() const override
      {
         return true;
      }

      bool isRawFunctionTy() const override
      {
         return isRawFunctionTy_;
      }

      bool needsMemCpy() const override
      {
         return !isRawFunctionTy_;
      }

      bool needsLvalueToRvalueConv() const override
      {
         return isRawFunctionTy_;
      }

      size_t getSize() const override;

      string toString() const override;
      llvm::Type* getLlvmType() const override;
      llvm::Type* getLlvmFunctionType() const override;

      bool implicitlyCastableTo(BuiltinType*) const override;

      static inline bool classof(FunctionType const*) { return true; }
      static inline bool classof(BuiltinType const* T) {
         switch(T->getTypeID()) {
            case TypeID::FunctionTypeID:
               return true;
            default:
               return false;
         }
      }

      typedef std::unique_ptr<FunctionType> UniquePtr;
      typedef std::shared_ptr<FunctionType> SharedPtr;

   protected:
      Type returnType;
      std::vector<Argument> argTypes;

      bool isRawFunctionTy_ = false;
   };

} // namespace cdot

#endif //CDOT_FUNCTIONTYPE_H
