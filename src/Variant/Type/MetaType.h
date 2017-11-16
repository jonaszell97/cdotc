//
// Created by Jonas Zell on 14.11.17.
//

#ifndef CDOT_METATYPE_H
#define CDOT_METATYPE_H

#include "ObjectType.h"

namespace cdot {

class MetaType: public ObjectType {
public:
   static MetaType *get(BuiltinType *forType);

   static inline bool classof(MetaType const*) { return true; }
   static inline bool classof(BuiltinType const* T) {
      switch(T->getTypeID()) {
         case TypeID::MetaTypeID:
         case TypeID::ObjectTypeID:
            return true;
         default:
            return false;
      }
   }

protected:
   static llvm::SmallDenseMap<BuiltinType*, MetaType*> Instances;

public:
   bool isMetaType() const override;
   BuiltinType *getUnderlyingType() const;

   string toString() const override;
   llvm::Type* getLlvmType() const override;

   llvm::Value* getDefaultVal(CodeGen &CGM) const override;

   size_t getSize() const override;
   short getAlignment() const override;

protected:
   explicit MetaType(BuiltinType *forType);

   BuiltinType *forType;
};

} // namespace cdot


#endif //CDOT_METATYPE_H
