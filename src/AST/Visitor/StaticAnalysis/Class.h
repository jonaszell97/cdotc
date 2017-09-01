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

      bool hasGetter = false;
      bool hasSetter = false;
      string getterName;
      string setterName;

      bool isInheritedField = false;

      FieldDecl* declaration;
      llvm::Type* llvmType = nullptr;
   };

   struct Method {
      Method(string name, Type* ret_type, AccessModifier access_modifier, std::vector<string>,
         std::vector<Type*>, std::vector<std::shared_ptr<Expression>>, std::vector<ObjectType*>& generics, bool,
         MethodDecl*);

      Method(string name, Type* ret_type, std::vector<Type*>, std::vector<ObjectType*>& generics, MethodDecl*);

      typedef std::unique_ptr<Method> UniquePtr;
      typedef std::shared_ptr<Method> SharedPtr;

      string methodName;
      string mangledName;
      Type* returnType;

      std::vector<string> argumentNames;
      std::vector<Type*> argumentTypes;
      std::vector<std::shared_ptr<Expression>> argumentDefaults;
      AccessModifier  accessModifier;
      std::vector<ObjectType*> generics = {};

      bool isStatic = false;
      bool isProtocolMethod = false;
      string protocolName;

      MethodDecl* declaration;
      llvm::Function* llvmFunc;
      bool hasHiddenParam = false;
   };

   struct MethodResult {
      CompatibilityType compatibility = CompatibilityType::FUNC_NOT_FOUND;
      Method* method = nullptr;

      std::unordered_map<size_t, pair<Type*, Type*>> neededCasts;
      string expectedType;
      string foundType;
      size_t incompArg = 0;
   };

   class Class {
   public:
      Class(string&, ObjectType*, std::vector<ObjectType*>&, std::vector<ObjectType*>&, ClassDecl*, bool = false);
      Class(string &, std::vector<ObjectType *> &, std::vector<ObjectType *> &, bool, ClassDecl *);

      Field* declareField(
         string name,
         Type *type,
         AccessModifier access,
         Expression::SharedPtr def_val,
         bool isConst,
         FieldDecl* declaration
      );

      Method* declareMethod(
         string name,
         Type *ret_type,
         AccessModifier access,
         std::vector<string> arg_names,
         std::vector<Type *> arg_types,
         std::vector<Expression::SharedPtr> arg_defaults,
         std::vector<ObjectType *> generics,
         bool isStatic,
         MethodDecl* declaration
      );

      void finalize();

      MethodResult hasMethod(
         string method_name,
         std::vector<Type *> args,
         std::vector<Type *> &concrete_generics,
         bool check_parent = true,
         bool checkProtocols = true,
         bool strict = false,
         bool swap = false
      );

      MethodResult hasMethod(
         string method_name,
         std::vector<Type *> args,
         std::unordered_map<string, Type*> &concrete_generics,
         std::vector<Type *> methodGenerics = {},
         bool check_parent = true,
         bool checkProtocols = true,
         bool strict = false,
         bool swap = false
      );

      MethodResult hasMethod(
         string method_name,
         std::vector<Type *> args,
         bool check_parent = true,
         bool checkProtocols = true,
         bool strict = false,
         bool swap = false
      );

      Method* declareMemberwiseInitializer();

      MethodResult ancestorHasMethod(string &name, std::vector<Type *> &args);

      void addConformity(ObjectType* proto) {
         conformsTo_.push_back(proto);
      }

      bool hasField(string &field_name);

      Method* getMethod(string method_name);
      Field* getField(string &field_name);

      unordered_map<string, Method*>& getMethods() {
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

      void defineParentClass();

      short getAlignment() {
         return alignment;
      }

      string& getName() {
         return className;
      }

      string& getTypeName() {
         return typeName;
      }

      size_t getTypeID() {
         return typeID;
      }

      bool conformsTo(string &name);

      bool protectedPropAccessibleFrom(string &class_context);
      bool privatePropAccessibleFrom(string &class_context);
      bool isBaseClassOf(string &child);

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

      std::vector<pair<string, Field::SharedPtr>>& getFields() {
         return fields;
      }

      std::map<string, std::vector<string>>& getInterfMethods() {
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

      unsigned int getDepth() {
         return depth;
      }

      std::vector<Method*>& getConstructors() {
         return constructors;
      }

      std::vector<llvm::Type*>& getMemoryLayout() {
         return memoryLayout;
      }

      unordered_map<string, size_t>& getFieldOffsets() {
         return fieldOffsets;
      }

      llvm::GlobalVariable* getVtable() {
         return vtable;
      }

      unordered_map<string, llvm::GlobalVariable*>& getProtocolVtables() {
         return protocolVtables;
      }

      llvm::GlobalVariable*& getProtocolVtable(string& protoName) {
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

      virtual llvm::Constant*  generateTypeInfo(llvm::IRBuilder<>& Builder);
      virtual void generateMemoryLayout(llvm::IRBuilder<>& Builder);
      virtual void generateProtocolMemoryLayout(llvm::IRBuilder<>& Builder);
      virtual void generateVTables(llvm::IRBuilder<> &Builder, llvm::Module &Module);

      void needsTypeInfoGen(bool gen) {
         needsTypeInfo = gen;
         if (parentClass != nullptr) {
            parentClass->needsTypeInfoGen(gen);
         }
      }

      static llvm::StructType* TypeInfoType;

      ObjectType* getType();

      typedef std::unique_ptr<Class> UniquePtr;

   protected:
      static size_t lastTypeID;
      unsigned int depth = 0;

      size_t typeID;
      string className;
      string typeName;

      ObjectType* extends = nullptr;
      Class* parentClass = nullptr;
      std::vector<Class*> extendedBy;
      unordered_map<string, size_t> baseClassOffsets;

      ClassDecl* declaration;

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

      std::vector<pair<string, string>> virtualMethods;

      ObjectType* type;

      std::vector<ObjectType*> generics;
      bool is_abstract = false;
      bool is_protocol = false;
      bool is_struct = false;

      bool finalized = false;

      std::vector<llvm::Type*> memoryLayout;
      bool layoutGenerated = false;
      bool emptyLayout = false;

      bool needsTypeInfo;

      size_t occupiedBytes = 0;
      short alignment = 1;
      llvm::GlobalVariable* vtable = nullptr;
      llvm::GlobalVariable* typeInfo = nullptr;
      unordered_map<string, llvm::GlobalVariable*> protocolVtables;

   };

} // namespace cl
} // namespace cdot

#endif //CDOT_CLASS_H
