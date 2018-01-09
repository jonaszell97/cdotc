//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_VALUE_H
#define CDOT_VALUE_H

#include <cassert>
#include <string>
#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/SmallVector.h>

#include "../../Variant/Type/QualType.h"
#include "../../lex/SourceLocation.h"
#include "Use.h"

namespace cdot {

class Type;

namespace il {

class MDSet;
class MetaData;
enum MDKind : unsigned;
class ValueSymbolTable;
class MDLocation;

class Value {
public:
   enum TypeID : unsigned short {
#     define CDOT_ALL(name) name##ID,
#     include "Instructions.def"
   };

   friend class ValueSymbolTable;

protected:
   Value(TypeID id, Type *ty);
   Value(TypeID id, QualType ty);

#  ifndef NDEBUG
   virtual
#  endif
   ~Value();

   TypeID id : 16;
   unsigned Flags : 16;
   unsigned SubclassData : 32;

   QualType type;
   std::string name;

   Use *uses;
   MDSet *metaData;

   enum Flag {

   };

private:
   static llvm::SmallVector<Value*, 256> CreatedValues;
   void setNameNoCheck(llvm::StringRef name) { this->name = name.str(); }

public:
   static bool classof(Value const* T) { return true; }
   static void cleanup();

   void deleteValue();
   void checkIfStillInUse();

   TypeID getTypeID() const;
   QualType getType() const;

   void setFlag(Flag f, bool value);
   bool getFlag(Flag f) const;

   bool isLvalue() const;
   void setIsLvalue(bool lvalue);

   bool isSelf() const;

   Use const* getUses() const
   {
      return uses;
   }

   Use * getUses()
   {
      return uses;
   }

   Use::iterator use_begin() { return uses ? uses->begin() : Use::iterator(); }
   Use::iterator use_end()   { return uses ? uses->end() : Use::iterator(); }

//   Use::const_iterator use_begin() const { return uses->begin(); }
//   Use::const_iterator use_end()   const { return uses->end(); }

   void removeFromParent();

   void addUse(Value *User);
   void removeUser(Value *User);

   size_t getNumUses() const;
   bool isUnused() const;

   void replaceAllUsesWith(Value *V);

   llvm::StringRef getName() const;
   void setName(llvm::StringRef name);
   bool hasName() const;

   MDLocation *getLocation() const;
   void setLocation(const SourceLocation &location);

   SourceLocation getSourceLoc() const;

   MDSet *getMetaData() const;
   bool hasMetaData(MDKind kind) const;

   void addMetaData(MetaData *MD);

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
