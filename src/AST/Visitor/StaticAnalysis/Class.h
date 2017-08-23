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
        Field(string name, Type* type, AccessModifier access_modifier, Expression::SharedPtr);
        typedef std::unique_ptr<Field> UniquePtr;
        typedef std::shared_ptr<Field> SharedPtr;

        string fieldName;
        string mangledName;
        Type* fieldType;
        AccessModifier  accessModifier;
        Expression::SharedPtr defaultVal;

        bool hasGetter = false;
        bool hasSetter = false;
        string getterName;
        string setterName;

        bool isInheritedField = false;

        llvm::Type* llvmType = nullptr;
    };

    struct Method {
        Method(string name, Type* ret_type, AccessModifier access_modifier, std::vector<string>,
            std::vector<Type*>, std::vector<std::shared_ptr<Expression>>, std::vector<GenericType*>& generics, bool);

        Method(string name, Type* ret_type, std::vector<Type*>, std::vector<GenericType*>& generics);

        typedef std::unique_ptr<Method> UniquePtr;
        typedef std::shared_ptr<Method> SharedPtr;

        string methodName;
        string mangledName;
        Type* returnType;

        std::vector<string> argumentNames;
        std::vector<Type*> argumentTypes;
        std::vector<std::shared_ptr<Expression>> argumentDefaults;
        AccessModifier  accessModifier;
        std::vector<GenericType*> generics = {};

        bool isStatic = false;
        bool isProtocolMethod = false;
        string protocolName;

        llvm::Function* llvmFunc;
        bool hasHiddenParam = false;
    };

    struct MethodResult {
        CompatibilityType compatibility = CompatibilityType::FUNC_NOT_FOUND;
        Method* method = nullptr;

        std::unordered_map<size_t, Type*> neededCasts;
    };

    class Class {
    public:
        Class(string&, ObjectType*, std::vector<ObjectType*>&, std::vector<GenericType*>&, ClassDecl*, bool = false);
        Class(string &, std::vector<ObjectType *> &, std::vector<GenericType *> &, bool, ClassDecl *);



        Field* declareField(
            string name,
            Type *type,
            AccessModifier access,
            Expression::SharedPtr def_val
        );

        Method* declareMethod(
            string name,
            Type *ret_type,
            AccessModifier access,
            std::vector<string> arg_names,
            std::vector<Type *> arg_types,
            std::vector<Expression::SharedPtr> arg_defaults,
            std::vector<GenericType *> generics,
            bool isStatic
        );

        void finalize();

        MethodResult hasMethod(
            string method_name,
            std::vector<Type *> args,
            std::vector<Type *> &concrete_generics,
            bool check_parent = true,
            bool checkProtocols = true,
            bool strict = false
        );

        MethodResult hasMethod(
            string method_name,
            std::vector<Type *> args,
            std::unordered_map<string, Type*> &concrete_generics,
            bool check_parent = true,
            bool checkProtocols = true,
            bool strict = false
        );

        MethodResult hasMethod(
            string method_name,
            std::vector<Type *> args,
            bool check_parent = true,
            bool checkProtocols = true,
            bool strict = false
        );

        Method* declareMemberwiseInitializer();

        MethodResult ancestorHasMethod(string &name, std::vector<Type *> &args);

        void addConformity(ObjectType* proto) {
            conformsTo_.push_back(proto);
        }

        bool hasField(string &field_name);

        Method* getMethod(string method_name);
        Field* getField(string &field_name);

        inline unordered_map<string, Method*>& getMethods() {
            return mangledMethods;
        };

        inline bool isAbstract() {
            return is_abstract;
        }

        inline bool isProtocol() {
            return is_protocol;
        }

        inline bool isStruct() {
            return is_struct;
        }

        inline bool isGeneric() {
            return !generics.empty();
        }

        inline std::vector<GenericType*>& getGenerics() {
            return generics;
        }

        inline ClassDecl* getDeclaration() {
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

        inline string& getName() {
            return className;
        }

        bool conformsTo(string &name);

        bool protectedPropAccessibleFrom(string &class_context);
        bool privatePropAccessibleFrom(string &class_context);
        bool is_base_class_of(string& child);

        inline Class* get_parent() {
            return parentClass;
        }

        void findVirtualMethods();
        bool is_virtual(Method*);

        inline void setDefaultConstructor(llvm::Function* constr) {
            defaultConstructor = constr;
        }

        inline llvm::Function* getDefaultContructor() {
            return defaultConstructor;
        }

        inline std::vector<pair<string, Field::SharedPtr>>& getFields() {
            return fields;
        }

        inline std::map<string, std::vector<string>>& getInterfMethods() {
            return protocolMethods;
        }

        inline size_t getVTableOffset(string& interface_name) {
            return vtableOffsets[interface_name];
        }

        inline size_t getVTableOffset(const string& interface_name) {
            return vtableOffsets[interface_name];
        }

        inline size_t getFieldOffset(string& fieldName) {
            return fieldOffsets[fieldName];
        }

        inline size_t getMethodOffset(string& methodName) {
            return methodOffsets[methodName];
        }

        inline unsigned int getDepth() {
            return depth;
        }

        inline std::vector<Method*>& getConstructors() {
            return constructors;
        }

        inline std::vector<llvm::Type*>& getMemoryLayout() {
            return memoryLayout;
        }

        inline unordered_map<string, size_t> getFieldOffsets() {
            return fieldOffsets;
        }

        inline llvm::GlobalVariable* getVtable() {
            return vtable;
        }

        inline unordered_map<string, llvm::GlobalVariable*>& getProtocolVtables() {
            return protocolVtables;
        }

        llvm::GlobalVariable*& getProtocolVtable(string& protoName) {
            return protocolVtables[protoName];
        }

        inline bool isEmpty() {
            return emptyLayout;
        }

        inline size_t getBaseClassOffset(string& className) {
            return baseClassOffsets[className];
        }

        inline void defineConcreteGeneric(string genericClassName, Type* type) {
            concreteGenerics.emplace(genericClassName, type);
        }

        inline unordered_map<string, Type*> getConcreteGenerics() {
            return concreteGenerics;
        }

        inline Method*& getMemberwiseInitializer() {
            return memberwiseInitializer;
        }

        inline string& getOriginalProtocol(string& methodName) {
            if (inheritedProtocolMethods.find(methodName) != inheritedProtocolMethods.end()) {
                return SymbolTable::getClass(inheritedProtocolMethods[methodName])
                    ->getOriginalProtocol(methodName);
            }

            return className;
        }

        std::vector<ObjectType*>& getConformedToProtocols() {
            return conformsTo_;
        }

        void generateMemoryLayout(llvm::IRBuilder<>& Builder);
        void generateProtocolMemoryLayout(llvm::IRBuilder<>& Builder);
        void generateVTables(llvm::IRBuilder<> &Builder, llvm::Module &Module);

        ObjectType* getType();

        typedef std::unique_ptr<Class> UniquePtr;

    protected:
        unsigned int depth = 0;

        string className;

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

        std::vector<GenericType*> generics;
        bool is_abstract = false;
        bool is_protocol = false;
        bool is_struct = false;

        bool finalized = false;

        std::vector<llvm::Type*> memoryLayout;
        bool layoutGenerated = false;
        bool emptyLayout = false;

        short alignment = 8;
        llvm::GlobalVariable* vtable = nullptr;
        unordered_map<string, llvm::GlobalVariable*> protocolVtables;

    };

} // namespace cl
} // namespace cdot

#endif //CDOT_CLASS_H
