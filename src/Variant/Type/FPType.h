//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_FPTYPE_H
#define CDOT_FPTYPE_H

#include "PrimitiveType.h"

namespace cdot {

   class FPType : public PrimitiveType {
   protected:
      explicit FPType(unsigned = 64);
      static unordered_map<unsigned, FPType*> Instances;

   public:
      static FPType* get(unsigned precision);
      static FPType* getFloatTy();
      static FPType* getDoubleTy();

      unsigned int getPrecision() const
      {
         return precision;
      }
      
      bool isFloatTy() const override
      {
         return precision == 32;
      }

      bool isDoubleTy() const override
      {
         return precision == 64;
      }

      bool isFPType() const override
      {
         return true;
      }

      BuiltinType* box() const override;

      string toString() const override;
      llvm::Type* getLlvmType() const override;

      BuiltinType* ArithmeticReturnType(string &op, BuiltinType *rhsTy) const override;

      bool implicitlyCastableTo(BuiltinType *destTy) const override;
      bool explicitlyCastableTo(BuiltinType *destTy) const override;

      llvm::Value* getDefaultVal(CodeGen &CGM) const override;
      llvm::Constant* getConstantVal(Variant &val) const override;

      short getAlignment() const override;

      static inline bool classof(FPType const*) { return true; }
      static inline bool classof(BuiltinType const* T) {
         switch(T->getTypeID()) {
            case TypeID::FPTypeID:
            case TypeID::PrimitiveTypeID:
               return true;
            default:
               return false;
         }
      }

   protected:
      int precision;
      string className;
   };

} // namespace cdot


#endif //CDOT_FPTYPE_H
