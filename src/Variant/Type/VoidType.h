//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_VOIDTYPE_H
#define CDOT_VOIDTYPE_H

#include "Type.h"

namespace cdot {

class VoidType : public Type {
protected:
   VoidType();
   static VoidType* Instance;

public:
   static VoidType* get();

   std::string toString() const
   {
      return "Void";
   }

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::VoidTypeID;
   }
};

} // namespace cdot

#endif //CDOT_VOIDTYPE_H
