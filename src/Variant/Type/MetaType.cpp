//
// Created by Jonas Zell on 14.11.17.
//

#include "MetaType.h"

#include "../../AST/SymbolTable.h"
#include "../../AST/Passes/SemanticAnalysis/Record/Record.h"
#include "../../AST/Passes/CodeGen/CodeGen.h"

namespace cdot {

MetaType::MetaType(Type *forType) : forType(forType)
{
   assert(forType);
   id = TypeID::MetaTypeID;
   className = "cdot.TypeInfo";// "cdot.TypeInfo<" + forType->toString() + ">";
   is_struct = true;
}

MetaType* MetaType::get(Type *forType)
{
   auto key = string("cdot.TypeInfo.") + std::to_string((uintptr_t)forType);
   auto it = Instances.find(key);
   if (it == Instances.end()) {
      it->second = new MetaType(forType);
   }

   return cast<MetaType>(it->second);
}

Type* MetaType::getUnderlyingType() const
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
   return ast::CodeGen::getStructTy(className);
}

llvm::Value* MetaType::getDefaultVal(ast::CodeGen &CGM) const
{
   return CGM.getTypeInfo(forType);
}

} // namespace cdot