//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_BUILTIN_TYPE_H
#define CDOT_BUILTIN_TYPE_H


#include <llvm/IR/IRBuilder.h>
#include <string>
#include <unordered_map>

#include "../../Support/Casting.h"

using std::string;
using std::unordered_map;
using std::pair;

namespace cdot {

namespace ast {

class SemaPass;
class DeclPass;
class CodeGen;
class Expression;

} // namespace ast

class Variant;
class PointerType;
class ObjectType;
class GenericType;
class IntegerType;
class FPType;
class FunctionType;
class TupleType;

struct TemplateArg;

class QualType;

namespace cl {
   class Record;
   struct EnumCase;
}

enum class TypeID : unsigned {
   AutoTypeID = 0,
   VoidTypeID = 1,
   PrimitiveTypeID = 2,
   FunctionTypeID = 3,
   GenericTypeID = 4,
   PointerTypeID = 5,
   ObjectTypeID = 6,
   IntegerTypeID = 7,
   FPTypeID = 8,
   TupleTypeID = 9,
   TypeGroupID = 10,
   MetaTypeID = 11,
   ArrayTypeID = 12,
};

using cdot::cl::EnumCase;

class AutoType;
class PointerType;
class VoidType;
class ObjectType;
class ArrayType;
class TupleType;
class FunctionType;
class IntegerType;
class FPType;
class MetaType;
class TypeGroup;
class GenericType;

class Type {
public:
   static bool classof(Type const* T) { return true; }
   static void TearDown();

   virtual llvm::Type* getLlvmType() const
   {
      llvm_unreachable("type does not have an llvm equivalent");
   }

   virtual bool needsLvalueToRvalueConv() const
   {
      return !isStruct() && !isProtocol();
   }

   bool isAutoTy() const;
   bool isVoidTy() const;
   bool isTypeGroup() const;
   bool isArrayTy() const;
   bool isMetaType() const;
   bool isGenericTy() const;
   bool isObjectTy() const;
   bool isTupleTy() const;

   bool isFPType() const;
   bool isFloatTy() const;
   bool isDoubleTy() const;
   bool isIntegerTy() const;
   bool isPointerTy() const;

   bool isFunctionTy() const;
   bool isRawFunctionTy() const;

   virtual bool isNumeric() const
   {
      return false;
   }

   virtual Type *getGroupDefault() const
   {
      llvm_unreachable("call isTypeGroup first");
   }

   virtual const std::vector<TemplateArg>& getTemplateArgs() const
   {
      llvm_unreachable("Call isObject first");
   }

   virtual bool hasTemplateArgs() const
   {
      return false;
   }

   virtual const string& getClassName() const
   {
      return className;
   }

   virtual cl::Record *getRecord() const
   {
      llvm_unreachable("Call isObject first");
   }

   virtual bool isBoxedPrimitive() const;

   const PointerType* asPointerTy() const;
   PointerType* asPointerTy();

   const IntegerType* asIntegerTy() const;
   IntegerType* asIntegerTy();

   const FPType* asFloatingTy() const;
   FPType* asFloatingTy();

   const FunctionType* asFunctionTy() const;
   FunctionType* asFunctionTy();

   const TupleType* asTupleTy() const;
   TupleType* asTupleTy();

   const ObjectType* asObjTy() const;
   ObjectType* asObjTy();

   const GenericType* asGenericTy() const;
   GenericType* asGenericTy();

   virtual bool isPointerToStruct() const
   {
      return false;
   }

   PointerType* getPointerTo();

   virtual string toString() const = 0;
   virtual string toUniqueString() const
   {
      return toString();
   }

   virtual bool implicitlyCastableTo(Type*) const
   {
      return false;
   }

   virtual bool explicitlyCastableTo(Type*) const
   {
      return false;
   }

   virtual bool needsCastTo(const Type *ty) const;

   virtual Type* unbox() const
   {
      llvm_unreachable("call isObjectTy first!");
   }

   virtual Type* box() const
   {
      llvm_unreachable("call isIntegerTy or isFPTy first");
   }

   virtual bool hasDefaultValue() const
   {
      return false;
   }

   virtual llvm::Value* getDefaultVal(ast::CodeGen &CGM) const
   {
      return nullptr;
   }

   virtual llvm::Constant* getConstantVal(Variant&) const
   {
      llvm_unreachable("Can't emit constant val for type");
   }

   virtual short getAlignment() const;
   virtual size_t getSize() const
   {
      return getAlignment();
   }

   virtual TypeID getTypeID() const
   {
      return id;
   }

   virtual bool isBoxedEquivOf(Type*) const
   {
      return false;
   }

   virtual bool isUnsigned() const
   {
      return false;
   }

   virtual unsigned int getBitwidth() const
   {
      llvm_unreachable("Not an integer type");
   }

   virtual bool isPtrSizedInt() const
   {
      return false;
   }

   virtual bool isInt64Ty(bool isUnsigned = false) const
   {
      return false;
   }

   virtual bool isInt8Ty(bool isUnsigned = false) const
   {
      return false;
   }

   virtual bool isInt1Ty(bool isUnsigned = false) const
   {
      return false;
   }

   virtual bool isIntNTy(unsigned n, bool isUnsigned = false) const
   {
      return false;
   }

   virtual bool isDummyObject() const
   {
      return false;
   }

   virtual bool isProtocol() const
   {
      return false;
   }

   virtual bool isStruct() const
   {
      return false;
   }

   virtual bool isEnum() const
   {
      return false;
   }

   virtual bool needsStructReturn() const
   {
      return isStruct() || isProtocol() || isEnum();
   }

   virtual void isEnum(bool) const
   {

   }

   virtual bool isOptionTy() const
   {
      return false;
   }

   virtual bool isOptionOf(const string&) const
   {
      return false;
   }

   virtual bool isRefcounted() const
   {
      return false;
   }

   bool isReferenceType() const
   {
      return isRefcounted();
   }

   virtual bool needsCleanup() const
   {
      return false;
   }

   virtual bool isValueType() const
   {
      return true;
   }

   virtual bool needsMemCpy() const
   {
      return false;
   }

   virtual llvm::Type* getLlvmFunctionType() const
   {
      llvm_unreachable("Not a function type!");
   }

protected:
   static llvm::StringMap<Type*> Instances;

   TypeID id;
   string className;
};

} // namespace cdot


#endif //CDOT_BUILTIN_TYPE_H
