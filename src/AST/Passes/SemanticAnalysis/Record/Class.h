//
// Created by Jonas Zell on 09.07.17.
//

#ifndef CDOT_CLASS_H
#define CDOT_CLASS_H

#include <string>
#include <unordered_map>
#include "../../../../Variant/Variant.h"
#include "../../../../Util.h"
#include "../../../SymbolTable.h"
#include "../../../Expression/Expression.h"
#include "../../../Statement/Declaration/FuncArgDecl.h"
#include "../../../../Variant/Type/ObjectType.h"
#include "../../../Statement/Declaration/Class/ClassDecl.h"

#include "Record.h"

class ClassDecl;
class Expression;

using namespace cdot;
using std::string;
using std::map;
using std::unordered_map;
using std::unordered_multimap;
using std::vector;

namespace cdot {
   struct GenericConstraint;

namespace cl {

   class Class;

   struct Field {
      Field(string name, BuiltinType* type, AccessModifier access_modifier, Expression::SharedPtr,
         bool isConst, FieldDecl*);
      typedef std::unique_ptr<Field> UniquePtr;
      typedef std::shared_ptr<Field> SharedPtr;

      string fieldName;
      string mangledName;
      BuiltinType* fieldType;
      AccessModifier accessModifier;
      Expression::SharedPtr defaultVal;

      bool isConst = false;
      bool isStatic = false;
      bool isProp = false;

      bool hasGetter = false;
      bool hasSetter = false;
      string getterName;
      string setterName;

      bool isInheritedField = false;

      FieldDecl* declaration;
      llvm::Type* llvmType = nullptr;
      size_t layoutOffset;

      Class* owningClass = nullptr;
   };

   struct Method;

   enum class ImplicitConformance {
      StringRepresentable,
      Hashable,
      Equatable
   };

   class Class: public Record {
      typedef unordered_multimap<string, std::shared_ptr<Method>>::iterator MethodIterator;

   public:
      Class(
         AccessModifier am,
         const string& className,
         std::vector<GenericConstraint> &generics,
         ClassDecl *decl,
         const SourceLocation &loc,
         bool isAbstract,
         bool isProto,
         bool isStruct
      );

      Field* declareField(
         const string &name,
         BuiltinType *type,
         AccessModifier access,
         Expression::SharedPtr def_val,
         bool isConst,
         bool isStatic,
         bool isProp,
         FieldDecl* declaration
      );

      Method* declareMethod(
         const string &methodName,
         Type& ret_type,
         AccessModifier access,
         std::vector<Argument>&& args,
         std::vector<GenericConstraint> generics,
         bool isStatic,
         MethodDecl* declaration,
         SourceLocation loc
      );

      void pushConstraintSet(std::vector<ExtensionConstraint> &&constraints);
      void popConstraintSet();

      const std::vector<ExtensionConstraint>& getConstraintSet(unsigned i);

      void inheritProtocols(std::vector<ObjectType*>& protocols, ObjectType* current, bool initial = true);
      void checkProtocolConformance(ObjectType *protoObj);
      void finalize();

      bool checkConstraint(
         const ExtensionConstraint& constraint,
         BuiltinType*& caller
      );

      const ExtensionConstraint* checkConstraints(
         Method* method,
         BuiltinType* caller
      );

      CallCompatability hasMethod(
         const string &method_name,
         std::vector<Argument> args = {},
         std::vector<GenericType*> givenGenerics = {},
         BuiltinType* caller = nullptr,
         bool check_parent = true,
         bool checkProtocols = true,
         bool strict = false
      );

      bool declareMemberwiseInitializer();

      void addConformance(ObjectType *proto) {
         conformsTo_.push_back(proto);
      }

      bool hasField(const string &field_name);

      Method* getMethod(const string &method_name);
      Field* getField(const string &field_name);

      pair<MethodIterator, MethodIterator> getOverloads(const string &methodName);

      const unordered_map<string, Method*>& getMethods() {
         return mangledMethods;
      };

      bool isAbstract() {
         return is_abstract;
      }

      bool isNonUnion() override
      {
         return true;
      }

      bool isClass() override
      {
         return is_class;
      }

      bool isStruct() override
      {
         return is_struct;
      }

      bool isProtocol() override
      {
         return is_protocol;
      }

      bool isGeneric() override
      {
         return !generics.empty();
      }

      bool isRefcounted() override
      {
         return is_class;
      }

      bool hasNonEmptyDeinitializer() const
      {
         return has_nonempty_deinit;
      }

      void hasNonEmptyDeinitializer(bool nonempty)
      {
         has_nonempty_deinit = nonempty;
      }

      std::vector<GenericConstraint>& getGenerics()
      {
         return generics;
      }

      ClassDecl* getDeclaration()
      {
         return declaration;
      }

      void declareMethodAlias(string& name, string& mangledOriginal) {
         for (const auto& method : methods) {
            if (method.second->getMangledName() == mangledOriginal) {
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

      string getUnqualifiedName();

      bool conformsTo(const string &name) override;

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

      size_t getVTableOffset(const string& interface_name) {
         return vtableOffsets[interface_name];
      }

      size_t getFieldOffset(const string& fieldName);

      size_t getMethodOffset(const string& methodName) {
         return methodOffsets[methodName];
      }

      const std::vector<Method*>& getConstructors() {
         return constructors;
      }

      const std::vector<llvm::Type*>& getMemoryLayout() {
         return memoryLayout;
      }

      llvm::Constant* getVtable(CodeGen &CGM);
      llvm::GlobalVariable* getProtocolVtable(const string& protoName, CodeGen &CGM);

      bool isEmpty() {
         return emptyLayout;
      }

      size_t getBaseClassOffset(string& className) {
         return baseClassOffsets[className];
      }

      void defineConcreteGeneric(string genericClassName, BuiltinType* type) {
         concreteGenerics.emplace(genericClassName, type);
      }

      unordered_map<string, BuiltinType*> getConcreteGenerics() {
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

         return recordName;
      }

      std::vector<ObjectType*>& getConformedToProtocols() {
         return conformsTo_;
      }

      void setConformedToProtocols(std::vector<ObjectType*>& conf)
      {
         conformsTo_ = conf;
      }

      void setParentClass(Class* parent);

      void generateTypeInfo(CodeGen &CGM) override;
      void generateMemoryLayout(CodeGen &CGM) override;
      void generateProtocolMemoryLayout(CodeGen &CGM) override;
      void generateVTables(CodeGen &CGM) override;

      virtual void collectProtocolVTableOffsets(ObjectType *proto, size_t &pos);

      void needsTypeInfoGen(bool gen) {
         needsTypeInfo = gen;
         if (parentClass != nullptr) {
            parentClass->needsTypeInfoGen(gen);
         }
      }

      llvm::Function* getDestructor(CodeGen &CGM);

      std::vector<pair<size_t, string>>& getRefCountedFields() {
         return refCountedFields;
      }

      void setDestructor(llvm::Function* destr) {
         destructor = destr;
      }

      llvm::Constant* getTypeInfo(CodeGen &CGM);

      size_t& getOutstandingExtensions() {
         return outstandingExtensions;
      }

      bool hasInnerClass(const string &inner);

      void addInnerClass(Class* inner) {
         innerDeclarations.push_back(inner);
         inner->outerClass = this;
      }

      Method*& getParameterlessConstructor()
      {
         return parameterlessConstructor;
      }

      void addImplicitConformance(ImplicitConformance kind);

      std::vector<ImplicitConformance>& getImplicitConformances()
      {
         return implicitConformances;
      }

      typedef std::unique_ptr<Class> UniquePtr;

      static unsigned int ProtoVtblPos;
      static unsigned int ProtoObjPos;
      static unsigned int ProtoSizePos;

      static unsigned int VTableMethodPos;
      static unsigned int VTableIsProtoDefPos;

      static size_t ProtocolSize;

   protected:
      bool activeConstraints = false;
      size_t fieldCount = 0;

      size_t outstandingExtensions = 0;

      std::vector<Class*> innerDeclarations;
      Class* outerClass = nullptr;

      Class* parentClass = nullptr;
      std::vector<Class*> extendedBy;
      unordered_map<string, size_t> baseClassOffsets;

      ClassDecl* declaration = nullptr;

      std::vector<ObjectType*> conformsTo_;
      map<string, std::vector<string>> protocolMethods;

      std::vector<pair<string, Field::SharedPtr>> fields;
      unordered_multimap<string, std::shared_ptr<Method>> methods;
      std::vector<Method*> constructors;

      std::vector<std::vector<ExtensionConstraint>> ConstraintSets;
      unordered_map<string, string> inheritedProtocolMethods;

      unordered_map<string, BuiltinType*> concreteGenerics;

      Method* parameterlessConstructor = nullptr;
      Method* memberwiseInitializer = nullptr;
      llvm::Function* defaultConstructor;

      unordered_map<string, Method*> mangledMethods;

      unordered_map<string, size_t> methodOffsets;
      unordered_map<string, size_t> vtableOffsets;
      size_t refCountOffset;
      std::vector<pair<size_t, string>> refCountedFields;

      std::vector<pair<string, string>> virtualMethods;

      std::vector<GenericConstraint> generics;
      bool is_abstract = false;
      bool is_protocol = false;
      bool is_struct = false;
      bool is_class = false;
      bool hasAssociatedTypes_ = false;

      bool has_nonempty_deinit = false;

      std::vector<ImplicitConformance> implicitConformances;

      bool finalized = false;

      std::vector<llvm::Type*> memoryLayout;
      bool layoutGenerated = false;
      bool emptyLayout = false;

      llvm::StructType* classLlvmType = nullptr;
      llvm::PointerType* opaquePtr = nullptr;

      llvm::Function* destructor = nullptr;

      bool needsTypeInfo;

      llvm::Constant* vtable = nullptr;
      llvm::GlobalVariable* typeInfo = nullptr;
      unordered_map<string, llvm::GlobalVariable*> protocolVtables;

   };

} // namespace cl
} // namespace cdot

#endif //CDOT_CLASS_H
