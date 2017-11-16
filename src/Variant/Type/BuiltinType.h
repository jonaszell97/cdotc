//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_BUILTIN_TYPE_H
#define CDOT_BUILTIN_TYPE_H


#include <llvm/IR/IRBuilder.h>
#include <string>
#include <unordered_map>

using std::string;
using std::unordered_map;
using std::pair;

class Expression;
class SemaPass;
class DeclPass;
class CodeGen;

namespace cdot {

class Variant;
class PointerType;
class ObjectType;
class GenericType;
class IntegerType;
class FPType;
class FunctionType;
class TupleType;

struct TemplateArg;

class Type;

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
   MetaTypeID = 11
};

using cdot::cl::EnumCase;

class BuiltinType {
public:
   static bool classof(BuiltinType const* T) { return true; }

   virtual llvm::Type* getLlvmType() const
   {
      llvm_unreachable("type does not have an llvm equivalent");
   }

   virtual bool needsLvalueToRvalueConv() const
   {
      return !isStruct() && !isProtocol();
   }

   virtual bool isAutoTy() const
   {
      return false;
   }

   virtual bool isVoidTy() const
   {
      return false;
   }

   virtual bool isNumeric() const
   {
      return false;
   }

   virtual bool isTypeGroup() const
   {
      return false;
   }

   virtual BuiltinType *getGroupDefault() const
   {
      llvm_unreachable("call isTypeGroup first");
   }

   virtual bool isMetaType() const
   {
      return false;
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

   virtual bool isGenericTy() const
   {
      return false;
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

   virtual bool implicitlyCastableTo(BuiltinType*) const
   {
      return false;
   }

   virtual bool explicitlyCastableTo(BuiltinType*) const
   {
      return false;
   }

   virtual bool needsCastTo(const BuiltinType *ty) const;

   virtual BuiltinType* unbox() const
   {
      llvm_unreachable("call isObjectTy first!");
   }

   virtual BuiltinType* box() const
   {
      llvm_unreachable("call isIntegerTy or isFPTy first");
   }

   virtual bool hasDefaultValue() const
   {
      return false;
   }

   virtual llvm::Value* getDefaultVal(CodeGen &CGM) const
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

   virtual bool isBoxedEquivOf(BuiltinType*) const
   {
      return false;
   }

   virtual bool isPointerTy() const
   {
      return false;
   }

   virtual bool isIntegerTy() const
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

   virtual bool isFloatTy() const
   {
      return false;
   }

   virtual bool isDoubleTy() const
   {
      return false;
   }

   virtual bool isFPType() const
   {
      return false;
   }

   virtual bool isObject() const
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

   virtual bool isFunctionTy() const
   {
      return false;
   }

   virtual bool isRawFunctionTy() const
   {
      return false;
   }

   virtual bool isTupleTy() const
   {
      return false;
   }

   virtual bool isRefcounted() const
   {
      return false;
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
   TypeID id;
   string className;
};

template <class T>
bool isa(const BuiltinType* t) {
   return t == nullptr ? false : T::classof(t);
}

template<class T>
const T* cast(const BuiltinType* t) {
   assert(T::classof(t) && "Check isa<> before casting");
   return static_cast<const T*>(t);
}

template<class T>
T* cast(BuiltinType* t) {
   assert(T::classof(t) && "Check isa<> before casting");
   return static_cast<T*>(t);
}

} // namespace cdot


#endif //CDOT_BUILTIN_TYPE_H
