//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_CLASSTYPE_H
#define CDOT_CLASSTYPE_H

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/ArrayRef.h>
#include <vector>
#include <set>
#include <llvm/ADT/DenseSet.h>

#include "../Constant/Constant.h"

namespace cdot {

struct QualType;

namespace cl {

class Enum;

} // namespace cl

namespace il {

class Module;
class Method;
class Initializer;
class ConstantStruct;
class ConstantArray;

class AggregateType: public Constant {
public:
   typedef llvm::SmallVector<Method*, 4> MethodList;

   struct Property {
      llvm::StringRef name;
      Method *Getter = nullptr;
      Method *Setter = nullptr;
   };

   typedef llvm::StringMap<Property> PropertyMap;
   typedef llvm::SmallVector<Initializer*, 4> InitializerList;

   AggregateType(TypeID id,
                 const std::string &name,
                 SourceLocation loc,
                 Module *m);

   Module *getParent() const;
   bool isForwardDeclared() const;

   MethodList getMethodsInModule(Module *M) const;
   const MethodList &getMethods() const;
   void setIsForwardDeclared(bool b);

   void addMethod(Method *M);
   Method *getMethod(llvm::StringRef name);

   void addProperty(Property &&P);
   const Property &getProperty(llvm::StringRef name) const;
   const PropertyMap &getProperties() const;

   ConstantStruct *getTypeInfo() const;

   void setTypeInfo(ConstantStruct *TypeInfo);

   const llvm::StringMap<ConstantArray*> &getProtocolTables() const;
   void addProtocolTable(llvm::StringRef protoName, ConstantArray *Table);

   void addConformance(llvm::StringRef protocolName);
   const std::set<llvm::StringRef> &getConformances() const;

   const InitializerList &getInitializers() const;
   InitializerList getInitializersInModule(Module *M) const;
   void addInitializer(Initializer *Init);
   Initializer *getInitializer(llvm::StringRef name);

protected:
   Module *parent;

   MethodList Methods;
   PropertyMap Properties;
   InitializerList Initializers;

   std::set<llvm::StringRef> Conformances;

   ConstantStruct *TypeInfo = nullptr;
   llvm::StringMap<ConstantArray*> ProtocolTables;

   enum Flag {
      ForwardDeclared = 0x1,
   };

public:
   static bool classof(AggregateType const* T) { return true; }
   static bool classof(Value const* T)
   {
      switch (T->getTypeID()) {
#        define CDOT_INCLUDE_TYPES
#        define CDOT_INSTRUCTION(Name) \
            case Name##ID:

#        include "../Instructions.def"
            return true;
         default:
            return false;
      }
   }
};

class StructType: public AggregateType {
public:
   struct Field {
      std::string name;
      Type *type;
      bool isStatic;
   };

   typedef llvm::SmallVector<Field, 4> FieldList;

   StructType(const std::string &name, Module *m,
              SourceLocation loc);

   const FieldList &getFields() const;

   void addField(Field &&F);
   const Field &getField(llvm::StringRef name) const;

protected:
   FieldList Fields;

public:
   static bool classof(StructType const* T) { return true; }
   static inline bool classof(Value const* T)
   {
      switch(T->getTypeID()) {
         case StructTypeID:
         case ClassTypeID:
            return true;
         default:
            return false;
      }
   }
};

class ClassType: public StructType {
public:
   ClassType(const std::string &name,
             llvm::StringRef parentClass,
             Module *m,
             SourceLocation loc);

   const llvm::StringRef &getParentClass() const;

   ConstantArray *getVTable() const;
   void setVTable(ConstantArray *VTable);

   void addVirtualMethod(llvm::StringRef M);
   const llvm::SmallDenseSet<llvm::StringRef, 4> &getVirtualMethods() const;

protected:
   llvm::StringRef ParentClass;

   llvm::SmallDenseSet<llvm::StringRef, 4> VirtualMethods;
   ConstantArray *VTable = nullptr;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == ClassTypeID;
   }
};

class EnumType: public AggregateType {
public:
   struct Case {
      std::string name;
      std::vector<std::pair<std::string, QualType>> AssociatedTypes;
   };

   typedef llvm::SmallVector<Case, 4> CaseList;

   EnumType(const std::string &name, Type *rawType,
            Module *m, SourceLocation loc);

   Type *getRawType() const;
   const CaseList &getCases() const;

   void addCase(Case &&C);
   size_t getMaxAssociatedValues() const;

protected:
   Type *rawType;
   CaseList cases;
   size_t maxAssociatedValues = 0;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == EnumTypeID;
   }
};

class UnionType: public StructType {
public:
   UnionType(const std::string &name,
             Module *m,
             SourceLocation loc);

   Type *getFieldType(llvm::StringRef fieldName) const;

   static bool classof(Value const* T)
   {
      return T->getTypeID() == UnionTypeID;
   }
};

class ProtocolType: public AggregateType {
public:
   ProtocolType(const std::string &name,
                Module *m,
                SourceLocation loc);

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == ProtocolTypeID;
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_CLASSTYPE_H
