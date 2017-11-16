//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_INTEGERTYPE_H
#define CDOT_INTEGERTYPE_H

#include <unordered_map>
#include "PrimitiveType.h"

using std::unordered_map;

namespace cdot {

   class IntegerType : public PrimitiveType {
   protected:
      explicit IntegerType(unsigned int = sizeof(int*) * 8, bool = false);
      static unordered_map<size_t, IntegerType*> Instances;

   public:
      static IntegerType* get(unsigned int = sizeof(int*) * 8, bool = false);
      static IntegerType* getBoolTy();
      static IntegerType* getCharTy();

      unsigned int getBitwidth() const override
      {
         return bitWidth;
      }

      bool isUnsigned() const override
      {
         return is_unsigned;
      }

      void isUnsigned(bool uns) {
         is_unsigned = uns;
      }

      bool isIntegerTy() const override
      {
         return true;
      }

      bool isInt64Ty(bool isUnsigned) const override
      {
         return bitWidth == 64 && is_unsigned == isUnsigned;
      }

      bool isInt8Ty(bool isUnsigned) const override
      {
         return bitWidth == 8 && is_unsigned == isUnsigned;
      }

      bool isInt1Ty(bool isUnsigned) const override
      {
         return bitWidth == 1 && is_unsigned == isUnsigned;
      }

      bool isIntNTy(unsigned n, bool isUnsigned) const override
      {
         return bitWidth == n && is_unsigned == isUnsigned;
      }

      bool isPtrSizedInt() const override
      {
         return bitWidth == sizeof(int*) * 8;
      }

      string toString() const override;
      llvm::Type* getLlvmType() const override;

      BuiltinType* ArithmeticReturnType(string&, BuiltinType*) const override;

      BuiltinType* box() const override;

      bool implicitlyCastableTo(BuiltinType*) const override;
      bool explicitlyCastableTo(BuiltinType*) const override;

      llvm::Value* getDefaultVal(CodeGen &CGM) const override;
      llvm::Constant* getConstantVal(Variant& val) const override;

      short getAlignment() const override;

      static inline bool classof(IntegerType const*) { return true; }
      static inline bool classof(BuiltinType const* T) {
         switch(T->getTypeID()) {
            case TypeID::IntegerTypeID:
            case TypeID::PrimitiveTypeID:
               return true;
            default:
               return false;
         }
      }

      typedef std::unique_ptr<IntegerType> UniquePtr;
      typedef std::shared_ptr<IntegerType> SharedPtr;

   protected:
      int bitWidth;
      bool is_unsigned;
   };

} // namespace cdot

#endif //CDOT_INTEGERTYPE_H
