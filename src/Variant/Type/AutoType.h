//
// Created by Jonas Zell on 14.08.17.
//

#ifndef CDOT_AUTOTYPE_H
#define CDOT_AUTOTYPE_H


#include "Type.h"

namespace cdot {

   class AutoType : public Type {
   protected:
      AutoType();
      static AutoType *Instance;

   public:
      static AutoType *get();

      std::string toString() const;

      static bool classof(Type const* T)
      {
         return T->getTypeID() == TypeID::AutoTypeID;
      }
   };

} // namespace cdot


#endif //CDOT_AUTOTYPE_H
