//
// Created by Jonas Zell on 14.11.17.
//

#include "MetaType.h"
#include "../../AST/Passes/SemanticAnalysis/Record/Record.h"

using namespace cdot::support;
using std::string;

namespace cdot {

MetaType::MetaType(Type *forType)
   : ObjectType(SymbolTable::getRecord("cdot.TypeInfo")), forType(forType)
{
   assert(forType);
   id = TypeID::MetaTypeID;
}

MetaType* MetaType::get(Type *forType)
{
   auto key = string("cdot.TypeInfo.") + std::to_string((uintptr_t)forType);
   auto it = Instances.find(key);
   if (it == Instances.end()) {
      Instances.try_emplace(key, new MetaType(forType));
   }

   return cast<MetaType>(Instances[key]);
}

Type* MetaType::getUnderlyingType() const
{
   return forType;
}

string MetaType::toString() const
{
   return "cdot.TypeInfo";
}

size_t MetaType::getSize() const
{
   return getRecord()->getSize();
}

unsigned short MetaType::getAlignment() const
{
   return sizeof(int*);
}

NamespaceType* NamespaceType::get(Namespace *NS)
{
   llvm::SmallString<128> str;
   str += "__ns.";
   str += NS->getName();

   auto key = str.str();

   auto it = Instances.find(key);
   if (it != Instances.end())
      return cast<NamespaceType>(it->second);

   auto ty = new NamespaceType(NS);
   Instances[key] = ty;

   return ty;
}

} // namespace cdot