//
// Created by Jonas Zell on 14.08.17.
//

#ifndef CDOT_AUTOTYPE_H
#define CDOT_AUTOTYPE_H


#include "BuiltinType.h"

namespace cdot {

   class AutoType : public BuiltinType {
   protected:
      AutoType();
      static AutoType *Instance;

   public:
      static AutoType *get();

      string toString() override;
      llvm::Type* getLlvmType() override;

      bool isAutoTy() override
      {
         return true;
      }

      bool implicitlyCastableTo(BuiltinType*) override;

      llvm::Value* getDefaultVal() override;

      static inline bool classof(AutoType const*) { return true; }
      static inline bool classof(BuiltinType const* T) {
         switch(T->getTypeID()) {
            case TypeID::AutoTypeID:
               return true;
            default:
               return false;
         }
      }
   };

} // namespace cdot


#endif //CDOT_AUTOTYPE_H
