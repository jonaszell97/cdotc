//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_POINTERTYPE_H
#define CDOT_POINTERTYPE_H

#include "QualType.h"

namespace cdot {

   class PointerType : public Type {
   protected:
      explicit PointerType(const QualType&);

   public:
      static PointerType* get(QualType const& pointee);
      static PointerType* get(Type* pointee);

      QualType getPointeeType() const
      {
         return pointeeType;
      }

      size_t getSize() const
      {
         return sizeof(void*);
      }

      unsigned short getAlignment() const
      {
         return alignof(void*);
      }

      std::string toString() const;

      static bool classof(Type const* T)
      {
         return T->getTypeID() == TypeID::PointerTypeID;
      }

   protected:
      const QualType pointeeType;
   };

} // namespace cdot

#endif //CDOT_POINTERTYPE_H
