//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_OBJECTTYPE_H
#define CDOT_OBJECTTYPE_H


#include <unordered_map>
#include "Type.h"

using std::unordered_map;

namespace cdot {

   class PointerType;

   namespace cl {
      class Class;
   }

   class ObjectType : public Type {
   public:
      ObjectType(string&&, unordered_map<string, Type*>&);
      ObjectType(string&&);
      ObjectType(string&);

      ~ObjectType() override;

      static ObjectType* get(string className);
      static ObjectType* getOptionOf(Type *T);
      static ObjectType* getAnyTy();

      inline string& getClassName() override {
         return className;
      }

      inline bool isObject() override {
         return true;
      }

      inline void isStruct(bool str) {
         is_struct = str;
      }

      inline bool isStruct() override {
         return is_struct;
      }

      bool isProtocol() override;

      inline unordered_map<string, Type*>& getConcreteGenericTypes() override {
         return concreteGenericTypes;
      }

      inline Type* getConcreteGeneric(string genericName) {
         return concreteGenericTypes[genericName];
      }

      inline std::vector<Type*>& getUnqualifiedGenerics() {
         return unqualifiedGenerics;
      }

      inline void specifyGenericType(string genericName, Type* type) {
         type->isGeneric(true);
         if (type->isObject() && type->getGenericClassName().empty()) {
            type->setGenericClassName(type->getClassName());
         }

         concreteGenericTypes.emplace(genericName, type);
      }

      inline void setUnqualGenerics(std::vector<Type*> unqual) {
         unqualifiedGenerics = unqual;
      }

      inline Type* toRvalue() override {
         lvalue = false;
         return this;
      }

      bool isRefcounted() override;

      bool hasDefaultValue() override;

      bool isBoxedEquivOf(Type*& other) override;

      bool operator==(Type*& other) override;
      inline bool operator!=(Type*& other) override {
         return !operator==(other);
      }

      std::vector<Type*> getContainedTypes(bool includeSelf = false) override;
      std::vector<Type**> getTypeReferences() override;

      string _toString() override;
      llvm::Type* _getLlvmType() override;

      bool implicitlyCastableTo(Type*) override;
      bool explicitlyCastableTo(Type*) override;

      short getAlignment() override;
      size_t getSize() override;

      Type* deepCopy() override;

      llvm::Value* castTo(llvm::Value*, Type*) override;

      bool isGeneric() override {
         return isGeneric_;
      }

      void isGeneric(bool gen) override {
         isGeneric_ = gen;
      }

      Type* getContravariance() override {
         return contravariance;
      }

      string& getGenericClassName() override {
         return genericClassName;
      }

      void setContravariance(Type* con) override {
         contravariance = con;
      }

      void setGenericClassName(string name) override {
         genericClassName = name;
      }

      void setClassName(string name) {
         className = name;
      }

      bool isEnum() override {
         return is_enum;
      }

      void isEnum(bool en) override {
         is_enum = en;
         is_struct = en;
      }

      void setKnownEnumCase(EnumCase *eCase,
         std::vector<pair<string, std::shared_ptr<Expression>>> associatedValues = {},
         std::vector<Type*> argTypes = {}) override
      {
         knownCase = eCase;
         this->associatedValues = associatedValues;
         enumCaseTypes = argTypes;
      }

      std::vector<pair<string, std::shared_ptr<Expression>>>& getAssociatedTypes() override {
         return associatedValues;
      }

      std::vector<Type*>& getKnownEnumCaseTypes() override {
         return enumCaseTypes;
      }

      EnumCase* getKnownEnumCase() override {
         return knownCase;
      }

      bool hasKnownEnumCase() override {
         return knownCase != nullptr;
      }

      bool isOptionTy() override {
         return is_enum && className == "Option";
      }

      bool isOptionOf(string& className) override {
         return isOptionTy()
            && concreteGenericTypes.find("T") != concreteGenericTypes.end()
            && concreteGenericTypes["T"]->getClassName() == className;
      }

      bool hasSelfRequirement() override {
         return hasSelfRequirement_;
      }

      void hasSelfRequirement(bool selfReq) override {
         hasSelfRequirement_ = selfReq;
      }

      Type* unbox() override;

      llvm::Value* getDefaultVal() override;

      static std::unordered_map<std::string, llvm::StructType*> StructureTypes;

      static inline void declareStructureType(string& name, llvm::StructType* type) {
         StructureTypes.emplace(name, type);
      }

      static inline bool hasStructureType(string& name) {
         return StructureTypes.find(name) != StructureTypes.end();
      }

      static inline llvm::StructType* getStructureType(string name) {
         assert(StructureTypes.find(name) != StructureTypes.end() && "Undeclared structure type");
         return StructureTypes[name];
      }

      static inline bool classof(ObjectType const*) { return true; }
      static inline bool classof(Type const* T) {
         switch(T->getTypeID()) {
            case TypeID::ObjectTypeID:
            case TypeID::CollectionTypeID:
               return true;
            default:
               return false;
         }
      }

      typedef std::unique_ptr<ObjectType> UniquePtr;
      typedef std::shared_ptr<ObjectType> SharedPtr;

   protected:
      bool is_struct = false;
      bool is_enum = false;
      unordered_map<string, Type*> concreteGenericTypes;
      cl::Class* declaredClass = nullptr;
      
      bool isGeneric_ = false;
      Type *contravariance = nullptr;
      string genericClassName;

      bool hasSelfRequirement_ = false;

      EnumCase *knownCase = nullptr;
      std::vector<pair<string, std::shared_ptr<Expression>>> associatedValues;
      std::vector<Type*> enumCaseTypes;

      std::vector<Type*> unqualifiedGenerics;
   };

} // namespace cdot


#endif //CDOT_OBJECTTYPE_H
