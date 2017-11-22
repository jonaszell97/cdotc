//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_FPTYPE_H
#define CDOT_FPTYPE_H

#include "PrimitiveType.h"

namespace cdot {

   class FPType : public PrimitiveType {
   protected:
      explicit FPType(unsigned short precision = 64);

   public:
      static FPType* get(unsigned short precision);
      static FPType* getFloatTy();
      static FPType* getDoubleTy();

      unsigned short getPrecision() const
      {
         return precision;
      }

      Type* box() const override;

      string toString() const override;
      llvm::Type* getLlvmType() const override;

      Type* ArithmeticReturnType(const string &op, Type *rhsTy) const override;

      bool implicitlyCastableTo(Type *destTy) const override;
      bool explicitlyCastableTo(Type *destTy) const override;

      llvm::Value* getDefaultVal(ast::CodeGen &CGM) const override;
      llvm::Constant* getConstantVal(Variant &val) const override;

      short getAlignment() const override;

      static bool classof(Type const* T)
      {
         return T->getTypeID() == TypeID::FPTypeID;
      }

   protected:
      unsigned short precision;
      string className;
   };

} // namespace cdot


#endif //CDOT_FPTYPE_H
