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

      inline unsigned int getPrecision() {
         return precision;
      }
      
      bool isFloatTy() override {
         return precision == 32;
      }

      bool isDoubleTy() override {
         return precision == 64;
      }

      bool isFPType() override {
         return true;
      }

      BuiltinType* box() override;

      string toString() override;
      llvm::Type* getLlvmType() override;

      BuiltinType* ArithmeticReturnType(string &op, BuiltinType *rhsTy) override;

      bool implicitlyCastableTo(BuiltinType *destTy) override;
      bool explicitlyCastableTo(BuiltinType *destTy) override;

      llvm::Value* getDefaultVal() override;
      llvm::Constant* getConstantVal(Variant &val) override;

      short getAlignment() override;

      inline string& getClassName() override {
         return className;
      }

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
