//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_FUNCTIONTYPE_H
#define CDOT_FUNCTIONTYPE_H


#include "Type.h"
#include "../../AST/Passes/AbstractPass.h"
#include "QualType.h"

using std::pair;

namespace cdot {

   struct Argument;
   class ObjectType;

   class FunctionType : public Type {
   protected:
      FunctionType(const QualType& returnType,
                   const std::vector<Argument>& argTypes,
                   bool raw);

      static string typesToString(
         const QualType& returnType,
         const std::vector<Argument>& argTypes
      );

   public:
      static FunctionType *get(
         const QualType& returnType,
         const std::vector<Argument>& argTypes,
         bool isRawFunctionTy
      );

      QualType getReturnType() const
      {
         return returnType;
      }

      const std::vector<Argument>& getArgTypes() const
      {
         return argTypes;
      }

      bool isRawFunctionTy() const
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

      bool implicitlyCastableTo(Type*) const override;

      static bool classof(Type const* T)
      {
         return T->getTypeID() == TypeID::FunctionTypeID;
      }

      typedef std::unique_ptr<FunctionType> UniquePtr;
      typedef std::shared_ptr<FunctionType> SharedPtr;

   protected:
      QualType returnType;
      std::vector<Argument> argTypes;

      bool isRawFunctionTy_ = false;
   };

} // namespace cdot

#endif //CDOT_FUNCTIONTYPE_H
