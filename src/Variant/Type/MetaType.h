//
// Created by Jonas Zell on 14.11.17.
//

#ifndef CDOT_METATYPE_H
#define CDOT_METATYPE_H

#include "ObjectType.h"

namespace cdot {

class MetaType: public ObjectType {
public:
   static MetaType *get(Type *forType);

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::MetaTypeID;
   }

public:
   Type *getUnderlyingType() const;

   string toString() const override;
   llvm::Type* getLlvmType() const override;

   llvm::Value* getDefaultVal(ast::CodeGen &CGM) const override;

   size_t getSize() const override;
   short getAlignment() const override;

protected:
   explicit MetaType(Type *forType);

   Type *forType;
};

} // namespace cdot


#endif //CDOT_METATYPE_H
