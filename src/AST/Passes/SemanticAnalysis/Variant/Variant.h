//
// Created by Jonas Zell on 04.01.18.
//

#ifndef CDOT_VARIANT_H
#define CDOT_VARIANT_H

namespace cdot {
namespace sema {

class Variant {
public:
   enum TypeID {
#  define CDOT_VARIANT(Name) \
      Name##ID,
#  include "Variant.def"
   };

   TypeID getTypeID() const { return typeID; }

   static bool classof(Variant const *V) { return true; }

protected:
   Variant(TypeID id)
      : typeID(id)
   {}

   TypeID typeID;
};

class IntVal {
public:

};

} // namespace sema
} // namespace cdot

#endif //CDOT_VARIANT_H
