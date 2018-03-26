//
// Created by Jonas Zell on 16.11.17.
//

#include "AggregateType.h"

#include "AST/Decl.h"
#include "IL/Module/Context.h"
#include "IL/Module/Module.h"
#include "IL/Value/Function/Method.h"

using namespace cdot::support;

namespace cdot {
namespace il {

AggregateType::AggregateType(RecordType *Ty,
                             llvm::StringRef name,
                             TypeID id,
                             Module *m)
   : GlobalObject(id, Ty, m, name)
{
   SubclassData |= Flag::ForwardDeclared;

   if (Ty->getRecord()->isExternal())
      SubclassData |= Flag::External;

   if (m) {
      m->insertType(this);
   }
}

ConstantStruct *AggregateType::getTypeInfo() const
{
   return TypeInfo;
}

const llvm::StringMap<ConstantArray*> &AggregateType::getProtocolTables() const
{
   return ProtocolTables;
}

void AggregateType::addProtocolTable(llvm::StringRef protoName,
                                     ConstantArray *Table) {
   assert(ProtocolTables.find(protoName) == ProtocolTables.end());
   ProtocolTables.try_emplace(protoName, Table);
}

const std::set<llvm::StringRef> &AggregateType::getConformances() const
{
   return Conformances;
}

void AggregateType::addConformance(llvm::StringRef protocolName)
{
   Conformances.insert(protocolName);
}

void AggregateType::setTypeInfo(ConstantStruct *TypeInfo)
{
   AggregateType::TypeInfo = TypeInfo;
}

ClassType::ClassType(RecordType *Ty,
                     llvm::StringRef name,
                     Module *m)
   : StructType(Ty, name, m), ParentClass(nullptr)
{
   id = ClassTypeID;
}

StructType::StructType(RecordType *Ty, llvm::StringRef name, Module *m)
   : AggregateType(Ty, name, StructTypeID, m)
{

}

void StructType::addField(Field &&F)
{
   Fields.push_back(std::move(F));
}

const StructType::Field & StructType::getField(llvm::StringRef name) const
{
   for (const auto &F: Fields) {
      if (name.equals(F.name)) {
         return F;
      }
   }

   llvm_unreachable("field does not exist");
}

llvm::ArrayRef<StructType::Field> StructType::getFields() const
{
   return Fields;
}

unsigned StructType::getFieldOffset(llvm::StringRef fieldName) const
{
   unsigned i = 0;
   for (const auto &F: Fields) {
      if (fieldName.equals(F.name))
         return i;
      else
         ++i;
   }

   llvm_unreachable("field does not exist");
}

EnumType::EnumType(RecordType *Ty,
                   llvm::StringRef name,
                   Module *m)
   : AggregateType(Ty, name, EnumTypeID, m), rawType(nullptr)
{

}

void EnumType::addCase(Case &&C)
{
   if (C.AssociatedTypes.size() > maxAssociatedValues) {
      maxAssociatedValues = C.AssociatedTypes.size();
   }

   cases.push_back(std::move(C));
}

const EnumType::CaseList &EnumType::getCases() const
{
   return cases;
}

const EnumType::Case & EnumType::getCase(llvm::StringRef name) const
{
   for (const auto &C : cases)
      if (name.equals(C.name))
         return C;

   llvm_unreachable("case not found");
}

size_t EnumType::getMaxAssociatedValues() const
{
   return maxAssociatedValues;
}

UnionType::UnionType(RecordType *Ty, llvm::StringRef name, Module *m)
   : StructType(Ty, name, m)
{
   id = UnionTypeID;
}

QualType UnionType::getFieldType(llvm::StringRef fieldName) const
{
   for (const auto &F: Fields) {
      if (fieldName.equals(F.name)) {
         return F.type;
      }
   }

   return nullptr;
}

ProtocolType::ProtocolType(RecordType *Ty,
                           llvm::StringRef name,
                           Module *m)
   : AggregateType(Ty, name, ProtocolTypeID, m)
{

}

} // namespace il
} // namespace cdot