//
// Created by Jonas Zell on 14.08.17.
//

#ifndef CDOT_AUTOTYPE_H
#define CDOT_AUTOTYPE_H


#include "Type.h"

namespace cdot {

   class AutoType : public Type {
   public:
      explicit AutoType();

      string _toString() override;
      llvm::Type* _getLlvmType() override;

      Type* deepCopy() override;

      bool implicitlyCastableTo(Type*) override;

      llvm::Value* getDefaultVal() override;
      llvm::Value* castTo(llvm::Value*, Type*) override;

      bool operator==(Type*& other) override;
      inline bool operator!=(Type*& other) override {
         return !operator==(other);
      }

      static inline bool classof(AutoType const*) { return true; }
      static inline bool classof(Type const* T) {
         switch(T->getTypeID()) {
            case TypeID::AutoTypeID:
               return true;
            default:
               return false;
         }
      }

      typedef std::unique_ptr<AutoType> UniquePtr;
      typedef std::shared_ptr<AutoType> SharedPtr;
   };

} // namespace cdot


#endif //CDOT_AUTOTYPE_H
