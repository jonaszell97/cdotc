//
// Created by Jonas Zell on 14.11.17.
//

#include "MetaType.h"

#include "../../AST/SymbolTable.h"
#include "../../AST/Passes/SemanticAnalysis/Record/Record.h"
#include "../../AST/Passes/CodeGen/CodeGen.h"

namespace cdot {

llvm::SmallDenseMap<BuiltinType*, MetaType*> MetaType::Instances;

MetaType::MetaType(BuiltinType *forType) : forType(forType)
{
   assert(forType);
   id = TypeID::MetaTypeID;
   className = "cdot.TypeInfo";// "cdot.TypeInfo<" + forType->toString() + ">";
   is_struct = true;
}

MetaType* MetaType::get(BuiltinType *forType)
{
   auto it = Instances.find(forType);
   if (it == Instances.end()) {
      it->second = new MetaType(forType);
   }

   return it->second;
}

bool MetaType::isMetaType() const
{
   return true;
}

BuiltinType* MetaType::getUnderlyingType() const
{
   return forType;
}

string MetaType::toString() const
{
   return className;
}

size_t MetaType::getSize() const
{
   return getRecord()->getSize();
}

short MetaType::getAlignment() const
{
   return sizeof(int*);
}

llvm::Type* MetaType::getLlvmType() const
{
   return CodeGen::getStructTy(className);
}

llvm::Value* MetaType::getDefaultVal(CodeGen &CGM) const
{
   return CGM.getTypeInfo(forType);
}

} // namespace cdot