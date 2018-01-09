//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_FUNCTIONTYPE_H
#define CDOT_FUNCTIONTYPE_H

#include "QualType.h"

namespace cdot {

struct Argument;
class ObjectType;

class FunctionType : public Type {
protected:
   FunctionType(const QualType& returnType,
                const std::vector<Argument>& argTypes,
                bool raw);

   FunctionType(const QualType& returnType,
                std::vector<Argument>&& argTypes,
                bool raw);

   static std::string typesToString(
      const QualType& returnType,
      const std::vector<Argument>& argTypes
   );

public:
   static FunctionType *get(
      const QualType& returnType,
      const std::vector<Argument>& argTypes,
      bool isRawFunctionTy
   );

   static FunctionType *get(
      const QualType& returnType,
      std::vector<Argument> &&argTypes,
      bool isRawFunctionTy
   );

   static FunctionType *get(llvm::ArrayRef<Type*> tys);

   static FunctionType *get(
      const QualType& returnType,
      const std::vector<Type*>& argTypes,
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

   bool isRawFunctionTy() const { return isRawFunctionTy_; }

   size_t getSize() const;
   unsigned short getAlignment() const
   {
      return sizeof(void*);
   }

   std::string toString() const;

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::FunctionTypeID;
   }

protected:
   QualType returnType;
   std::vector<Argument> argTypes;

   bool isRawFunctionTy_ = false;
};

} // namespace cdot

#endif //CDOT_FUNCTIONTYPE_H
