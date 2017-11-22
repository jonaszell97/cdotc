//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_VALUE_H
#define CDOT_VALUE_H

#include <cassert>
#include <string>
#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/SmallVector.h>

#include "../ILType.h"

#include "../../lex/SourceLocation.h"

namespace cdot {

class Type;

namespace il {

class MDSet;
class MetaData;
enum MDKind : unsigned;

class Value {
public:
   enum TypeID : unsigned short {
#     define CDOT_INCLUDE_ALL
#     define CDOT_INSTRUCTION(name) name##ID,
#     include "Instructions.def"
   };

protected:
   Value(TypeID id, Type *ty,
         const std::string &name = "",
         const SourceLocation &loc = {});

   Value(TypeID id, ILType ty,
         const std::string &name = "",
         const SourceLocation &loc = {});

   TypeID id : 16;
   unsigned uses : 16;

   unsigned Flags : 16;
   unsigned SubclassData : 16;

   ILType type;
   std::string name;

   MDSet *metaData;

   enum Flag {

   };

private:
   static llvm::SmallVector<Value*, 16> CreatedValues;

public:
   static bool classof(Value const* T) { return true; }
   static void cleanup();

   void deleteValue();

   TypeID getTypeID() const;
   ILType getType() const;

   void setFlag(Flag f, bool value);
   bool getFlag(Flag f) const;

   bool isLvalue() const;
   void setIsLvalue(bool lvalue);

   void addUse();
   unsigned getNumUses() const;

   llvm::StringRef getName() const;
   void setName(const std::string &name);
   bool hasName() const;

   const SourceLocation &getLocation() const;
   void setLocation(const SourceLocation &location);

   MDSet *getMetaData() const;
   bool hasMetaData(MDKind kind) const;

   MetaData *getMetaData(MDKind kind) const;

   template<class T>
   T* getAs()
   {
      return static_cast<T*>(this);
   }

   template<class T>
   T* getAsOrNull()
   {
      if (!T::classof(this)) {
         return nullptr;
      }

      return static_cast<T*>(this);
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_VALUE_H
