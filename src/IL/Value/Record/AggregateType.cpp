//
// Created by Jonas Zell on 16.11.17.
//

#include "AggregateType.h"

#include "../../Module/Context.h"
#include "../../Module/Module.h"
#include "../Function/Method.h"

#include "../../../AST/Passes/SemanticAnalysis/Record/Enum.h"
#include "../../../Variant/Type/ObjectType.h"

using namespace cdot::support;

namespace cdot {
namespace il {

AggregateType::AggregateType(llvm::StringRef name,
                             TypeID id,
                             Module *m)
   : GlobalObject(id, nullptr, m, name)
{
   SubclassData |= Flag::ForwardDeclared;
   if (m) {
      m->insertType(this);
   }
}

void AggregateType::addMethod(Method *M)
{
   Methods.push_back(M);
}

Method* AggregateType::getMethod(llvm::StringRef name)
{
   for (const auto &M : Methods) {
      if (M->getName() == name) {
         return M;
      }
   }
   for (const auto &M : Initializers) {
      if (M->getName() == name) {
         return M;
      }
   }

   if (auto ClassTy = dyn_cast<ClassType>(this)) {
      auto parentName = ClassTy->getParentClass();
      if (!parentName.empty()) {
         return parent->getType(parentName)->getMethod(name);
      }
   }

   return nullptr;
}

const AggregateType::MethodList &AggregateType::getMethods() const
{
   return Methods;
}

void AggregateType::addProperty(AggregateType::Property &&P)
{
   Properties.try_emplace(P.name, P);
}

const AggregateType::Property &
AggregateType::getProperty(llvm::StringRef name) const
{
   auto it = Properties.find(name);
   assert(it != Properties.end());

   return it->second;
}

const AggregateType::PropertyMap &AggregateType::getProperties() const
{
   return Properties;
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

void AggregateType::addInitializer(Initializer *Init)
{
   Initializers.push_back(Init);
}

Initializer * AggregateType::getInitializer(llvm::StringRef name)
{
   for (const auto &I : Initializers) {
      if (name.equals(I->getName())) {
         return I;
      }
   }

   return nullptr;
}

const AggregateType::InitializerList &AggregateType::getInitializers() const
{
   return Initializers;
}

size_t AggregateType::getSize() const
{
   if (!size) {
      auto R = SymbolTable::getRecord(name);
      size = R->getSize();
      alignment = R->getAlignment();
   }

   return size;
}

unsigned short AggregateType::getAlignment() const
{
   if (!alignment) {
      auto R = SymbolTable::getRecord(name);
      size = R->getSize();
      alignment = R->getAlignment();
   }

   return alignment;
}

ClassType::ClassType(llvm::StringRef name,
                     llvm::StringRef parentClass,
                     Module *m)
   : StructType(name, m), ParentClass(parentClass)
{
   id = ClassTypeID;
}

StructType::StructType(llvm::StringRef name, Module *m)
   : AggregateType(name, StructTypeID, m)
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

const StructType::FieldList &StructType::getFields() const
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

EnumType::EnumType(llvm::StringRef name,
                   Type *rawType, Module *m)
   : AggregateType(name, EnumTypeID, m), rawType(rawType)
{

}

void EnumType::addCase(Case &&C)
{
   if (C.AssociatedTypes.size() > maxAssociatedValues) {
      maxAssociatedValues = C.AssociatedTypes.size();
   }

   cases.push_back(std::move(C));
}

Type *EnumType::getRawType() const
{
   return rawType;
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

UnionType::UnionType(llvm::StringRef name, Module *m)
   : StructType(name, m)
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

ProtocolType::ProtocolType(llvm::StringRef name,
                           Module *m)
   : AggregateType(name, ProtocolTypeID, m)
{

}

} // namespace il
} // namespace cdot