//
// Created by Jonas Zell on 09.07.17.
//

#ifndef CDOT_CLASS_H
#define CDOT_CLASS_H

#include <string>
#include <unordered_map>
#include "../../../Variant/Variant.h"
#include "../../../Util.h"
#include "../../SymbolTable.h"
#include "../../Expression/Expression.h"
#include "../../Statement/Declaration/FuncArgDecl.h"
#include "../../../Variant/Type/ObjectType.h"

class ClassDecl;
class Expression;

using namespace cdot;
using std::string;
using std::map;
using std::unordered_map;
using std::unordered_multimap;
using std::vector;

namespace cdot {
namespace cl {

   class Class;

   struct Field {
      Field(string name, Type* type, AccessModifier access_modifier, Expression::SharedPtr, bool isConst, FieldDecl*);
      typedef std::unique_ptr<Field> UniquePtr;
      typedef std::shared_ptr<Field> SharedPtr;

      string fieldName;
      string mangledName;
      Type* fieldType;
      AccessModifier  accessModifier;
      Expression::SharedPtr defaultVal;
      bool isConst = false;

      bool isStatic = false;

      bool hasGetter = false;
      bool hasSetter = false;
      string getterName;
      string setterName;

      bool isInheritedField = false;

      FieldDecl* declaration;
      llvm::Type* llvmType = nullptr;

      Class* owningClass = nullptr;
   };

   struct Method {
      Method(string name, Type* ret_type, AccessModifier access_modifier, std::vector<Argument>&& args,
         std::vector<ObjectType*>& generics, bool, MethodDecl *decl);

      Method(string name, Type* ret_type, std::vector<Argument>&& args, std::vector<ObjectType*>& generics,
         MethodDecl *decl);

      typedef std::unique_ptr<Method> UniquePtr;
      typedef std::shared_ptr<Method> SharedPtr;

      string methodName;
      string mangledName;
      Type* returnType;

      std::vector<Argument> arguments;
      AccessModifier  accessModifier;
      std::vector<ObjectType*> generics;

      bool isStatic = false;
      bool isProtocolMethod = false;
      bool isVirtual = false;
      bool mutableSelf = false;

      size_t uses = 0;

      bool isProtocolDefaultImpl = false;
      bool hasDefinition = false;
      string protocolName;

      MethodDecl* declaration;
      llvm::Function* llvmFunc;
      bool hasStructReturn = false;

      Class* owningClass = nullptr;
   };

   class Class {
      typedef unordered_multimap<string, Method::SharedPtr>::iterator MethodIterator;

   public:
      Class(AccessModifier am,
         string&,
         ObjectType*,
         std::vector<ObjectType*>&,
         std::vector<ObjectType*>&,
         ClassDecl*,
         bool = false
      );
      Class(
         AccessModifier am,
         string &,
         std::vector<ObjectType *> &,
         std::vector<ObjectType *> &,
         bool,
         ClassDecl *
      );

      Field* declareField(
         string name,
         Type *type,
         AccessModifier access,
         Expression::SharedPtr def_val,
         bool isConst,
         bool isStatic,
         FieldDecl* declaration
      );

      Method* declareMethod(
         string name,
         Type *ret_type,
         AccessModifier access,
         std::vector<Argument>&& args,
         std::vector<ObjectType *> generics,
         bool isStatic,
         MethodDecl* declaration
      );

      void inheritProtocols(std::vector<ObjectType*>& protocols, ObjectType* current, bool initial = true);
      void checkProtocolConformance(ObjectType *protoObj);
      void finalize();

      CallCompatability hasMethod(
         string method_name,
         std::vector<Argument> args = {},
         std::vector<Type*> givenGenerics = {},
         unordered_map<string, Type*> classGenerics = {},
         bool check_parent = true,
         bool checkProtocols = true,
         bool strict = false,
         bool swap = false
      );

      bool declareMemberwiseInitializer();

      void addConformance(ObjectType *proto) {
         conformsTo_.push_back(proto);
      }

      bool hasField(string &field_name);

      Method* getMethod(string method_name);
      Field* getField(string &field_name);

      pair<MethodIterator, MethodIterator> getOverloads(string &methodName);

      const unordered_map<string, Method*>& getMethods() {
         return mangledMethods;
      };

      bool isAbstract() {
         return is_abstract;
      }

      bool isProtocol() {
         return is_protocol;
      }

      bool isStruct() {
         return is_struct;
      }

      bool isClass() {
         return is_class;
      }

      bool isGeneric() {
         return !generics.empty();
      }

      virtual bool isEnum() {
         return false;
      }

      std::vector<ObjectType*>& getGenerics() {
         return generics;
      }

      ClassDecl* getDeclaration() {
         return declaration;
      }

      void declareMethodAlias(string& name, string& mangledOriginal) {
         for (const auto& method : methods) {
            if (method.second->mangledName == mangledOriginal) {
               methods.emplace(name, method.second);
               break;
            }
         }
      }

      bool isEmptyProtocol() {
         if (!is_protocol || !methods.empty()) {
            return false;
         }
         for (const auto& prot : conformsTo_) {
            if (!SymbolTable::getClass(prot->getClassName())->isEmptyProtocol()) {
               return false;
            }
         }

         return true;
      }

      short getAlignment() {
         return alignment;
      }

      const string& getName() {
         return className;
      }

      const string& getTypeName() {
         return typeName;
      }

      size_t getTypeID() {
         return typeID;
      }

      bool isPrivate() {
         return access == AccessModifier::PRIVATE;
      }

      bool isProtected() {
         return access == AccessModifier::PROTECTED;
      }

      bool conformsTo(string name);

      bool protectedPropAccessibleFrom(const string &class_context);
      bool privatePropAccessibleFrom(const string &class_context);
      bool isBaseClassOf(const string &child);

      Class* getParent() {
         return parentClass;
      }

      void findVirtualMethods();
      bool isVirtual(Method *);

      void setDefaultConstructor(llvm::Function* constr) {
         defaultConstructor = constr;
      }

      llvm::Function* getDefaultContructor() {
         return defaultConstructor;
      }

      const std::vector<pair<string, Field::SharedPtr>>& getFields() {
         return fields;
      }

      const std::map<string, std::vector<string>>& getInterfMethods() {
         return protocolMethods;
      }

      size_t getVTableOffset(string& interface_name) {
         return vtableOffsets[interface_name];
      }

      size_t getVTableOffset(const string& interface_name) {
         return vtableOffsets[interface_name];
      }

      size_t getFieldOffset(string& fieldName) {
         return fieldOffsets[fieldName];
      }

      size_t getMethodOffset(string& methodName) {
         return methodOffsets[methodName];
      }

      const std::vector<Method*>& getConstructors() {
         return constructors;
      }

      const std::vector<llvm::Type*>& getMemoryLayout() {
         return memoryLayout;
      }

      unordered_map<string, size_t>& getFieldOffsets() {
         return fieldOffsets;
      }

      llvm::Constant* getVtable() {
         return vtable;
      }

      unordered_map<string, llvm::GlobalVariable*>& getProtocolVtables() {
         return protocolVtables;
      }

      llvm::GlobalVariable*& getProtocolVtable(const string& protoName) {
         return protocolVtables[protoName];
      }

      bool isEmpty() {
         return emptyLayout;
      }

      size_t getBaseClassOffset(string& className) {
         return baseClassOffsets[className];
      }

      void defineConcreteGeneric(string genericClassName, Type* type) {
         concreteGenerics.emplace(genericClassName, type);
      }

      unordered_map<string, Type*> getConcreteGenerics() {
         return concreteGenerics;
      }

      Method*& getMemberwiseInitializer() {
         return memberwiseInitializer;
      }

      string& getOriginalProtocol(string& methodName) {
         if (inheritedProtocolMethods.find(methodName) != inheritedProtocolMethods.end()) {
            return SymbolTable::getClass(inheritedProtocolMethods[methodName])
               ->getOriginalProtocol(methodName);
         }

         return className;
      }

      size_t getOccupiedBytes(bool packed = true) {
         if (packed) {
            return occupiedBytes;
         }

         return (short)(occupiedBytes + (8 - occupiedBytes % 8));
      }

      std::vector<ObjectType*>& getConformedToProtocols() {
         return conformsTo_;
      }

      const string& getDeclarationNamespace() {
         return declarationNamespace;
      }

      virtual void generateTypeInfo(llvm::IRBuilder<>& Builder);
      virtual void generateMemoryLayout(llvm::IRBuilder<>& Builder);
      virtual void generateProtocolMemoryLayout(llvm::IRBuilder<>& Builder);
      virtual void generateVTables(llvm::IRBuilder<> &Builder, llvm::Module &Module);

      virtual void collectProtocolVTableOffsets(ObjectType *proto, size_t &pos);

      void needsTypeInfoGen(bool gen) {
         needsTypeInfo = gen;
         if (parentClass != nullptr) {
            parentClass->needsTypeInfoGen(gen);
         }
      }

      void isDeclared(bool decl) {
         is_declared = decl;
      }

      bool isDeclared() {
         return is_declared;
      }

      llvm::Function* getDestructor() {
         return destructor;
      }

      std::vector<pair<size_t, string>>& getRefCountedFields() {
         return refCountedFields;
      }

      void setDestructor(llvm::Function* destr) {
         destructor = destr;
      }

      size_t getRefCountOffset() {
         return refCountOffset;
      }

      llvm::Value* getTypeInfo() {
         return typeInfo;
      }

      bool hasAssociatedTypes() {
         return hasAssociatedTypes_;
      }

      void hasAssociatedTypes(bool assoc) {
         hasAssociatedTypes_ = assoc;
      }

      ObjectType* getType();

      size_t& getOutstandingExtensions() {
         return outstandingExtensions;
      }

      llvm::PointerType* getOpaquePointer() {
         return opaquePtr;
      }

      bool hasInnerClass(const string &inner);

      void addInnerClass(Class* inner) {
         innerDeclarations.push_back(inner);
         inner->outerClass = this;
      }

      void addUse() {
         ++uses;
      }

      size_t& getNumUses() {
         return uses;
      }

      typedef std::unique_ptr<Class> UniquePtr;

      static unsigned int ProtoVtblPos;
      static unsigned int ProtoObjPos;
      static unsigned int ProtoSizePos;

      static unsigned int VTableMethodPos;
      static unsigned int VTableIsProtoDefPos;

      static size_t ProtocolSize;

   protected:
      static size_t lastTypeID;
      size_t uses = 0;

      AccessModifier access;

      size_t typeID;
      string className;
      string typeName;

      bool is_declared = false;

      string declarationNamespace;

      size_t outstandingExtensions = 0;

      std::vector<Class*> innerDeclarations;
      Class* outerClass = nullptr;

      ObjectType* extends = nullptr;
      Class* parentClass = nullptr;
      std::vector<Class*> extendedBy;
      unordered_map<string, size_t> baseClassOffsets;

      ClassDecl* declaration = nullptr;

      std::vector<ObjectType*> conformsTo_;
      map<string, std::vector<string>> protocolMethods;

      std::vector<pair<string, Field::SharedPtr>> fields;
      unordered_multimap<string, Method::SharedPtr> methods;
      std::vector<Method*> constructors;

      unordered_map<string, string> inheritedProtocolMethods;

      unordered_map<string, Type*> concreteGenerics;

      Method* memberwiseInitializer = nullptr;
      llvm::Function* defaultConstructor;

      unordered_map<string, Method*> mangledMethods;

      unordered_map<string, size_t> fieldOffsets;
      unordered_map<string, size_t> methodOffsets;
      unordered_map<string, size_t> vtableOffsets;
      size_t refCountOffset;
      std::vector<pair<size_t, string>> refCountedFields;

      std::vector<pair<string, string>> virtualMethods;

      ObjectType* type = nullptr;

      std::vector<ObjectType*> generics;
      bool is_abstract = false;
      bool is_protocol = false;
      bool is_struct = false;
      bool is_class = false;
      bool hasAssociatedTypes_ = false;

      bool finalized = false;

      std::vector<llvm::Type*> memoryLayout;
      bool layoutGenerated = false;
      bool emptyLayout = false;

      llvm::StructType* classLlvmType = nullptr;
      llvm::PointerType* opaquePtr = nullptr;

      llvm::Function* destructor = nullptr;

      bool needsTypeInfo;

      size_t occupiedBytes = 0;
      short alignment = 1;
      llvm::Constant* vtable = nullptr;
      llvm::GlobalVariable* typeInfo = nullptr;
      unordered_map<string, llvm::GlobalVariable*> protocolVtables;

   };

} // namespace cl
} // namespace cdot

#endif //CDOT_CLASS_H
