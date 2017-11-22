//
// Created by Jonas Zell on 16.11.17.
//

#include "AggregateType.h"

#include "../../Module/Context.h"
#include "../../Module/Module.h"
#include "../Function/Method.h"

#include "../../../AST/Passes/SemanticAnalysis/Record/Enum.h"

namespace cdot {
namespace il {

AggregateType::AggregateType(TypeID id, const std::string &name,
                             SourceLocation loc, Module *m)
   : Constant(id, nullptr, name, loc),
     parent(m)
{
   SubclassData |= Flag::ForwardDeclared;
   if (m) {
      m->getContext().registerType(this);
   }
}

void AggregateType::setIsForwardDeclared(bool b)
{
   SubclassData |= Flag::ForwardDeclared;
}

void AggregateType::addMethod(Method *M)
{
   Methods.push_back(M);
}

Method* AggregateType::getMethod(llvm::StringRef name)
{
   for (const auto &M : Methods) {
      if (M->getName() == name
          || (M->hasCallable() && name.equals(M->getMangledName()))) {
         return M;
      }
   }
   for (const auto &M : Initializers) {
      if (M->getName() == name
          || (M->hasCallable() && name.equals(M->getMangledName()))) {
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

bool AggregateType::isForwardDeclared() const
{
   return (SubclassData & Flag::ForwardDeclared) != 0;
}

Module* AggregateType::getParent() const
{
   return parent;
}

const AggregateType::MethodList &AggregateType::getMethods() const
{
   return Methods;
}

AggregateType::MethodList AggregateType::getMethodsInModule(
   Module *Mod) const
{
   MethodList list;
   bool isProtocol = isa<ProtocolType>(this);
   for (const auto &M : Methods) {
      if (M->isProperty()) {
         continue;
      }

      if (isProtocol || Mod->getOwnFunction(M->getMangledName())) {
         list.push_back(M);
      }
   }

   return list;
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

AggregateType::InitializerList
AggregateType::getInitializersInModule(Module *M) const
{
   InitializerList list;
   for (const auto &I : Initializers) {
      if (M->getOwnFunction(I->getMangledName())) {
         list.push_back(I);
      }
   }

   return list;
}

ClassType::ClassType(const std::string &name, llvm::StringRef parentClass,
                     Module *m, SourceLocation loc)
   : StructType(name, m, loc), ParentClass(parentClass)
{
   id = ClassTypeID;
}

const llvm::StringRef &ClassType::getParentClass() const
{
   return ParentClass;
}

ConstantArray *ClassType::getVTable() const
{
   return VTable;
}

void ClassType::setVTable(ConstantArray *VTable)
{
   ClassType::VTable = VTable;
}

const llvm::SmallDenseSet<llvm::StringRef, 4> &ClassType::getVirtualMethods()
const
{
   return VirtualMethods;
}

void ClassType::addVirtualMethod(llvm::StringRef M)
{
   VirtualMethods.insert(M);
}

StructType::StructType(const std::string &name, Module *m, SourceLocation loc)
   : AggregateType(StructTypeID, name, loc, m)
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

EnumType::EnumType(const std::string &name, Type *rawType, Module *m,
                   SourceLocation loc)
   : AggregateType(EnumTypeID, name, loc, m), rawType(rawType)
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

size_t EnumType::getMaxAssociatedValues() const
{
   return maxAssociatedValues;
}

UnionType::UnionType(const std::string &name, Module *m, SourceLocation loc)
   : StructType(name, m, loc)
{
   id = UnionTypeID;
}

Type * UnionType::getFieldType(llvm::StringRef fieldName) const
{
   for (const auto &F: Fields) {
      if (fieldName.equals(F.name)) {
         return F.type;
      }
   }

   return nullptr;
}

ProtocolType::ProtocolType(const std::string &name, Module *m,
                           SourceLocation loc)
   : AggregateType(ProtocolTypeID, name, loc, m)
{

}

} // namespace il
} // namespace cdot