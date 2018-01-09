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
#include <llvm/ADT/ilist_node.h>

#include "../GlobalVariable.h"

namespace llvm {

class StructType;

} // namespace llvm

namespace cdot {

class QualType;

namespace cl {

class Enum;

} // namespace cl

namespace il {

class Module;
class Method;
class Initializer;
class ConstantStruct;
class ConstantArray;
class ConstantInt;

class AggregateType: public GlobalObject,
                     public llvm::ilist_node_with_parent<AggregateType,
                        Module> {
public:
   typedef llvm::SmallVector<Method*, 4> MethodList;

   struct Property {
      llvm::StringRef name;
      Method *Getter = nullptr;
      Method *Setter = nullptr;
   };

   typedef llvm::StringMap<Property> PropertyMap;
   typedef llvm::SmallVector<Initializer*, 4> InitializerList;

   AggregateType(llvm::StringRef name,
                 TypeID id,
                 Module *m);

   Module *getParent() const { return parent; }
   void setParent(Module *m) { parent = m; }

   const MethodList &getMethods() const;

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

   void addInitializer(Initializer *Init);
   Initializer *getInitializer(llvm::StringRef name);

   llvm::StructType *getLlvmTy() const
   {
      return llvmTy;
   }

   void setLlvmTy(llvm::StructType *llvmTy) const
   {
      AggregateType::llvmTy = llvmTy;
   }

   GlobalVariable *getPTable() const
   {
      return PTable;
   }

   void setPTable(GlobalVariable *PTable)
   {
      AggregateType::PTable = PTable;
   }

   size_t getSize() const;
   unsigned short getAlignment() const;

   Method *getDeinitializer() const
   {
      return deinitializer;
   }

   void setDeinitializer(Method *deinitializer)
   {
      AggregateType::deinitializer = deinitializer;
   }

protected:
   MethodList Methods;
   PropertyMap Properties;
   InitializerList Initializers;

   mutable size_t size;
   mutable unsigned short alignment;

   std::set<llvm::StringRef> Conformances;

   ConstantStruct *TypeInfo = nullptr;
   llvm::StringMap<ConstantArray*> ProtocolTables;

   il::GlobalVariable *PTable = nullptr;
   il::Method *deinitializer = nullptr;

   mutable llvm::StructType *llvmTy;

   enum Flag {
      ForwardDeclared = 0x1,
   };

public:
   static bool classof(AggregateType const* T) { return true; }
   static bool classof(Value const* T)
   {
      switch (T->getTypeID()) {
#        define CDOT_AGGR_TYPE(Name) \
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
      QualType type;
      bool isStatic;
   };

   typedef llvm::SmallVector<Field, 4> FieldList;

   StructType(llvm::StringRef name,
              Module *m);

   const FieldList &getFields() const;

   void addField(Field &&F);
   const Field &getField(llvm::StringRef name) const;

   unsigned getFieldOffset(llvm::StringRef fieldName) const;

   Method *getDefaultInitializer() const
   {
      return defaultInitializer;
   }

   void setDefaultInitializer(Method *defaultInitializer)
   {
      StructType::defaultInitializer = defaultInitializer;
   }

protected:
   FieldList Fields;
   Method *defaultInitializer;

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
   ClassType(llvm::StringRef name,
             llvm::StringRef parentClass,
             Module *m);

   llvm::StringRef getParentClass() const
   {
      return ParentClass;
   }

   GlobalVariable *getVTable() const
   {
      return VTable;
   }

   void setVTable(GlobalVariable *VTable)
   {
      ClassType::VTable = VTable;
   }

   void addVirtualMethod(llvm::StringRef M)
   {
      VirtualMethods.insert(M);
   }

   const llvm::SmallDenseSet<llvm::StringRef, 4> &getVirtualMethods() const
   {
      return VirtualMethods;
   }

protected:
   llvm::StringRef ParentClass;

   llvm::SmallDenseSet<llvm::StringRef, 4> VirtualMethods;
   GlobalVariable *VTable = nullptr;

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
      std::vector<QualType> AssociatedTypes;
      ConstantInt *caseVal;
   };

   typedef llvm::SmallVector<Case, 4> CaseList;

   EnumType(llvm::StringRef name,
            Type *rawType,
            Module *m);

   Type *getRawType() const;
   const CaseList &getCases() const;

   const Case &getCase(llvm::StringRef name) const;

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
   UnionType(llvm::StringRef name,
             Module *m);

   QualType getFieldType(llvm::StringRef fieldName) const;

   static bool classof(Value const* T)
   {
      return T->getTypeID() == UnionTypeID;
   }
};

class ProtocolType: public AggregateType {
public:
   ProtocolType(llvm::StringRef name,
                Module *m);

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == ProtocolTypeID;
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_CLASSTYPE_H
