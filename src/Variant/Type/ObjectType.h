//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_OBJECTTYPE_H
#define CDOT_OBJECTTYPE_H


#include <unordered_map>
#include "BuiltinType.h"

using std::unordered_map;

namespace cdot {

   class PointerType;
   class GenericType;

   namespace cl {
      class Record;
   }

   class ObjectType : public BuiltinType {
   protected:
      ObjectType() = default;
      ObjectType(const string &className, std::vector<GenericType*> &generics);
      explicit ObjectType(const string &className);

      static unordered_map<string, ObjectType*> Instances;

   public:
      static ObjectType *get(const string& className);
      static ObjectType *get(string className, std::vector<GenericType*> &generics);

      static ObjectType* getOptionOf(BuiltinType *T);
      static ObjectType* getAnyTy();

      string& getClassName() override {
         return className;
      }

      bool isObject() const override {
         return true;
      }

      void isStruct(bool str) {
         is_struct = str;
      }

      bool isStruct() override {
         return is_struct;
      }

      bool isProtocol() override {
         return is_protocol;
      }

      void isProtocol(bool proto) {
         is_protocol = proto;
      }

      bool isNumeric() override
      {
         return is_raw_enum;
      }

      bool isIntegerTy() override
      {
         return is_raw_enum;
      }

      std::vector<GenericType*>& getConcreteGenericTypes() override
      {
         return concreteGenericTypes;
      }

      GenericType* getConcreteGeneric(string genericName);
      cl::Record* getRecord() const override;

      bool isRefcounted() const override;
      bool isValueType() const override;

      bool needsMemCpy() const override
      {
         return is_struct || is_protocol || (is_enum && !is_raw_enum);
      }

      bool needsStructReturn() override
      {
         return BuiltinType::needsStructReturn() && !is_raw_enum;
      }

      bool hasDefaultValue() override;

      bool needsCleanup() const override;

      bool isBoxedEquivOf(BuiltinType* other) override;

      string toString() override;
      llvm::Type* getLlvmType() override;

      bool implicitlyCastableTo(BuiltinType*) override;
      bool explicitlyCastableTo(BuiltinType*) override;

      short getAlignment() override;
      size_t getSize() override;

      bool isEnum() override {
         return is_enum;
      }

      void isEnum(bool en) override {
         is_enum = en;
         is_struct = en;
      }

      bool isOptionTy() override {
         return is_enum && className == "Option";
      }

      bool isOptionOf(string& className) override;

      BuiltinType* unbox() override;

      llvm::Value* getDefaultVal() override;
      llvm::Constant* getConstantVal(Variant&) override;

      static bool classof(ObjectType const*) { return true; }
      static bool classof(BuiltinType const* T) {
         switch(T->getTypeID()) {
            case TypeID::ObjectTypeID:
            case TypeID::GenericTypeID:
               return true;
            default:
               return false;
         }
      }

   protected:
      bool is_struct = false;
      bool is_enum = false;
      bool is_union = false;
      bool is_protocol = false;
      bool is_raw_enum = false;

      std::vector<GenericType*> concreteGenericTypes;
   };

} // namespace cdot


#endif //CDOT_OBJECTTYPE_H
