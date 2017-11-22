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
class GenericType;

namespace cl {
class Record;
}

class ObjectType : public Type {
protected:
   ObjectType() = default;
   explicit ObjectType(const string &className);

public:
   static ObjectType *get(const string& className);
   static ObjectType* getOptionOf(Type *T);
   static ObjectType* getAnyTy();

   void isStruct(bool str)
   {
      is_struct = str;
   }

   bool isStruct() const override
   {
      return is_struct;
   }

   bool isProtocol() const override
   {
      return is_protocol;
   }

   void isProtocol(bool proto)
   {
      is_protocol = proto;
   }

   bool isNumeric() const override
   {
      return isRawEnum();
   }

   bool isRawEnum() const
   {
      return is_raw_enum;
   }

   unsigned getBitwidth() const override;
   bool isUnsigned() const override;

   const std::vector<TemplateArg>& getTemplateArgs() const override;
   bool hasTemplateArgs() const override;
   GenericType *const getNamedTemplateArg(const string &genericName) const;
   cl::Record* getRecord() const override;

   bool isRefcounted() const override;
   bool isValueType() const override;

   bool needsMemCpy() const override
   {
      return is_struct || is_protocol || (is_enum && !is_raw_enum);
   }

   bool needsStructReturn() const override
   {
      return Type::needsStructReturn() && !is_raw_enum;
   }

   bool hasDefaultValue() const override;

   bool needsCleanup() const override;

   bool isBoxedEquivOf(Type* other) const override;

   string toString() const override;
   llvm::Type* getLlvmType() const override;

   bool implicitlyCastableTo(Type*) const override;
   bool explicitlyCastableTo(Type*) const override;

   short getAlignment() const override;
   size_t getSize() const override;

   bool isEnum() const override
   {
      return is_enum;
   }

   void isEnum(bool en)
   {
      is_enum = en;
      is_struct = en;
   }

   bool isOptionTy() const override;
   bool isOptionOf(const string& className) const override;

   Type* unbox() const override;

   llvm::Value* getDefaultVal(ast::CodeGen &CGM) const override;
   llvm::Constant* getConstantVal(Variant&) const override;

   static bool classof(ObjectType const*) { return true; }
   static bool classof(Type const* T) {
      switch(T->getTypeID()) {
         case TypeID::ObjectTypeID:
         case TypeID::GenericTypeID:
         case TypeID::MetaTypeID:
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
};

// to be used when resolving template arguments and the actual underlying
// record of the type might not exist
class DummyObjectType: public ObjectType {
protected:
   DummyObjectType(
      const string &className, std::vector<TemplateArg> &templateArgs);

   std::vector<TemplateArg> &templateArgs;

public:
   static DummyObjectType *get(const string &className,
                               std::vector<TemplateArg> &templateArgs);

   bool isDummyObject() const override
   {
      return true;
   }

   const std::vector<TemplateArg>& getTemplateArgs() const override
   {
      return templateArgs;
   }

   bool hasTemplateArgs() const override
   {
      return true;
   }
};

} // namespace cdot


#endif //CDOT_OBJECTTYPE_H
