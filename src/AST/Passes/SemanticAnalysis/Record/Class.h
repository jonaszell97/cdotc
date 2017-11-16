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

#include "Record.h"

class ClassDecl;
class Expression;

using namespace cdot;
using std::string;
using std::map;
using std::unordered_map;
using std::unordered_multimap;

namespace cdot {
   struct TemplateConstraint;

namespace cl {

   class Class;

   struct Field {
      Field(string name, BuiltinType* type, AccessModifier access_modifier,
            Expression::SharedPtr defaultVal, bool isConst, FieldDecl*);

      typedef std::unique_ptr<Field> UniquePtr;
      typedef std::shared_ptr<Field> SharedPtr;

      string fieldName;
      string mangledName;
      BuiltinType* fieldType;
      AccessModifier accessModifier;
      Expression::SharedPtr defaultVal;

      bool isConst = false;
      bool isStatic = false;

      bool hasGetter() const;
      bool hasSetter() const;

      Method *getter = nullptr;
      Method *setter = nullptr;

      bool isInheritedField = false;

      FieldDecl* declaration;
      llvm::Type* llvmType = nullptr;
      size_t layoutOffset;

      Class* owningClass = nullptr;
   };

   struct Method;


   class Class: public Record {
      typedef unordered_multimap<string, std::shared_ptr<Method>>::iterator MethodIterator;

   public:
      Class(
         AccessModifier am,
         const string& className,
         RecordDecl *decl,
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
         FieldDecl* declaration
      );

      Method* declareMethod(
         const string &methodName,
         const Type& ret_type,
         AccessModifier access,
         std::vector<Argument>&& args,
         bool isStatic,
         MethodDecl* declaration,
         SourceLocation loc
      ) override;

      void pushConstraintSet(std::vector<ExtensionConstraint> &&constraints);
      void popConstraintSet();

      bool checkConstraint(
         const ExtensionConstraint& constraint,
         BuiltinType*& caller
      ) const override;

      const ExtensionConstraint* checkConstraints(
         Method* method,
         BuiltinType* caller
      ) const override;

      const std::vector<ExtensionConstraint>& getConstraintSet(unsigned i);

      void inheritProtocols(Class* current);
      void checkProtocolConformance(Class *protoObj);
      void finalize() override;

      bool declareMemberwiseInitializer();

      bool hasField(const string &field_name);

      Method* getMethod(const string &method_name) override;
      Method* getMethod(unsigned id) override;
      Field* getField(const string &field_name);

      pair<MethodIterator, MethodIterator> getOverloads(const string &methodName);

      const unordered_map<string, Method*>& getMethods() {
         return mangledMethods;
      };

      bool isAbstract() {
         return is_abstract;
      }

      bool isNonUnion() const override
      {
         return true;
      }

      bool isClass() const override
      {
         return is_class;
      }

      bool isStruct() const override
      {
         return is_struct;
      }

      bool isProtocol() const override
      {
         return is_protocol;
      }

      bool isRefcounted() const override
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

      bool isEmptyProtocol();
      string getUnqualifiedName();

      bool protectedPropAccessibleFrom(const string &class_context);
      bool privatePropAccessibleFrom(const string &class_context);

      bool isBaseClassOf(const string &child);
      bool isBaseClassOf(Record *child);

      Class* getParent() {
         return parentClass;
      }

      void findVirtualMethods();
      bool isVirtual(Method *);

      void setDefaultConstructor(llvm::Function* constr)
      {
         defaultConstructor = constr;
      }

      llvm::Function* getDefaultContructor() const
      {
         return defaultConstructor;
      }

      const std::vector<pair<string, Field::SharedPtr>>& getFields() const
      {
         return fields;
      }

      const std::map<string, std::vector<string>>& getInterfMethods() const
      {
         return protocolMethods;
      }

      size_t getVTableOffset(const string& interface_name) const
      {
         return vtableOffsets.at(interface_name);
      }

      size_t getFieldOffset(const string& fieldName);
      size_t getMethodOffset(const string& methodName) const;

      const std::vector<Method*>& getConstructors() const
      {
         return constructors;
      }

      const std::vector<llvm::Type*>& getMemoryLayout() const
      {
         return memoryLayout;
      }

      llvm::Constant* getVtable(CodeGen &CGM);
      llvm::GlobalVariable* getProtocolVtable(const string& protoName, CodeGen &CGM);

      bool isEmpty() const
      {
         return emptyLayout;
      }

      const size_t getBaseClassOffset(const string& className) const
      {
         return baseClassOffsets.at(className);
      }

      Method* getMemberwiseInitializer() const
      {
         return memberwiseInitializer;
      }

      string& getOriginalProtocol(string& methodName);

      void setParentClass(Class* parent);

      void generateTypeInfo(CodeGen &CGM) override;
      void generateMemoryLayout(CodeGen &CGM) override;
      void generateProtocolMemoryLayout(CodeGen &CGM) override;
      void generateVTables(CodeGen &CGM) override;

      virtual void collectProtocolVTableOffsets(Class *proto, size_t &pos);

      llvm::Function* getDestructor(CodeGen &CGM);

      const std::vector<pair<size_t, string>>& getRefCountedFields() const
      {
         return refCountedFields;
      }

      void setDestructor(llvm::Function* destr)
      {
         destructor = destr;
      }

      llvm::Constant* getTypeInfo(CodeGen &CGM);

      size_t& getOutstandingExtensions()
      {
         return outstandingExtensions;
      }

      Method* getParameterlessConstructor() const
      {
         return parameterlessConstructor;
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

      Class* parentClass = nullptr;
      std::vector<Class*> extendedBy;
      unordered_map<string, size_t> baseClassOffsets;

      map<string, std::vector<string>> protocolMethods;

      std::vector<pair<string, Field::SharedPtr>> fields;
      std::vector<Method*> constructors;

      std::vector<std::vector<ExtensionConstraint>> ConstraintSets;
      unordered_map<string, string> inheritedProtocolMethods;

      Method* parameterlessConstructor = nullptr;
      Method* memberwiseInitializer = nullptr;
      llvm::Function* defaultConstructor;

      unordered_map<string, Method*> mangledMethods;

      unordered_map<string, size_t> methodOffsets;
      unordered_map<string, size_t> vtableOffsets;
      size_t refCountOffset;
      std::vector<pair<size_t, string>> refCountedFields;

      std::vector<pair<string, string>> virtualMethods;

      bool is_abstract = false;
      bool is_protocol = false;
      bool is_struct = false;
      bool is_class = false;
      bool hasAssociatedTypes_ = false;

      bool has_nonempty_deinit = false;

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
