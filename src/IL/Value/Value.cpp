//
// Created by Jonas Zell on 16.11.17.
//

#include "Value.h"
#include "Use.h"
#include "MetaData/MetaData.h"

#define CDOT_VALUE_INCLUDE
#include "ValueIncludes.def"
#include "../../Variant/Type/VoidType.h"

namespace cdot {
namespace il {

llvm::SmallVector<Value*, 16> Value::CreatedValues;

void Value::cleanup()
{
   auto it = CreatedValues.begin();
   while (it != CreatedValues.end()) {
      if ((*it)->getNumUses() == 0) {
         (*it)->deleteValue();
         it = CreatedValues.erase(it);
      }
      else {
         ++it;
      }
   }
}

Value::Value(TypeID id, Type *ty,
             const std::string &name,
             const SourceLocation &loc)
   : id(id), type(ILType(ty)), name(name), uses(0), SubclassData(0), Flags(0),
     metaData(new MDSet)
{
   if (!ty) {
      *type = VoidType::get();
   }

   metaData->addNode(new MDLocation(loc));
}

Value::Value(TypeID id, ILType ty,
             const std::string &name,
             const SourceLocation &loc)
   : id(id), type(ty), name(name), uses(0), SubclassData(0), Flags(0),
     metaData(new MDSet)
{
   metaData->addNode(new MDLocation(loc));
}

void Value::deleteValue()
{
   switch (id) {
#define CDOT_INCLUDE_ALL
#define CDOT_INCLUDE_NO_ABSTRACT
#define CDOT_INSTRUCTION(name) \
      case name##ID:           \
         delete static_cast<name*>(this); break;
#include "Instructions.def"

      default:
         llvm_unreachable("unknown value kind");
   }
}

Value::TypeID Value::getTypeID() const
{
   return id;
}

ILType Value::getType() const
{
   return type;
}

bool Value::getFlag(Flag f) const
{
   return (Flags & f) != 0;
}

void Value::setFlag(Flag f, bool value)
{
   if (value) {
      Flags |= f;
   }
   else {
      Flags &= ~f;
   }
}

bool Value::isLvalue() const
{
   return type.isLvalue();
}

void Value::setIsLvalue(bool lvalue)
{
   type.setIsLvalue(lvalue);
}

void Value::addUse(Value *User)
{
   if (!uses) {
      uses = new Use(User); return;
   }

   uses->addUseAtEnd(new Use(User));
}

size_t Value::getNumUses() const
{
   if (!uses) return 0;
   return uses->count();
}

bool Value::isUnused() const
{
   return !getNumUses();
}

llvm::StringRef Value::getName() const
{
   return name;
}

void Value::setName(const std::string &name)
{
   Value::name = name;
}

bool Value::hasName() const
{
   return !name.empty();
}

const SourceLocation &Value::getLocation() const
{
   return cast<MDLocation>(getMetaData(MDLocationID))->getLocation();
}

void Value::setLocation(const SourceLocation &location)
{
   cast<MDLocation>(getMetaData(MDLocationID))->setLocation(location);
}

MDSet *Value::getMetaData() const
{
   return metaData;
}

bool Value::hasMetaData(MDKind kind) const
{
   return metaData->hasNode(kind);
}

MetaData* Value::getMetaData(MDKind kind) const
{
   return metaData->getNode(kind);
}

} // namespace il
} // namespace cdot
