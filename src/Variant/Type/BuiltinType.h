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
      TupleTypeID = 9
   };

   using cdot::cl::EnumCase;

   class BuiltinType {
   public:
      static bool classof(BuiltinType const* T) { return true; }

      virtual llvm::Type* getLlvmType() = 0;

      virtual bool needsLvalueToRvalueConv()
      {
         return !isStruct() && !isProtocol();
      }

      virtual bool isAutoTy()
      {
         return false;
      }

      virtual bool isVoidTy()
      {
         return false;
      }

      virtual bool isNumeric()
      {
         return false;
      }

      virtual std::vector<GenericType*>& getConcreteGenericTypes()
      {
         llvm_unreachable("Call isObject first");
      }

      virtual string& getClassName()
      {
         return className;
      }

      virtual cl::Record *getRecord() const
      {
         llvm_unreachable("Call isObject first");
      }

      virtual bool isGeneric()
      {
         return false;
      }

      virtual bool isBoxedPrimitive();

      PointerType* asPointerTy();
      IntegerType* asIntegerTy();
      FPType* asFloatingTy();
      FunctionType* asFunctionTy();
      TupleType* asTupleTy();
      virtual ObjectType* asObjTy();
      GenericType* asGenericTy();
      virtual bool isPointerToStruct() {
         return false;
      }

      virtual PointerType* getPointerTo();
      virtual string toString() = 0;
      virtual string toUniqueString()
      {
         return toString();
      }

      virtual bool implicitlyCastableTo(BuiltinType*)
      {
         return false;
      }

      virtual bool explicitlyCastableTo(BuiltinType*)
      {
         return false;
      }

      virtual BuiltinType* unbox() {
         return this;
      }

      virtual BuiltinType* box() {
         return this;
      }

      virtual bool hasDefaultValue() {
         return false;
      }

      virtual llvm::Value* getDefaultVal() {
         return nullptr;
      }

      virtual llvm::Constant* getConstantVal(Variant&) {
         llvm_unreachable("Can't emit constant val for type");
      }

      virtual short getAlignment();
      virtual size_t getSize() {
         return getAlignment();
      }

      virtual TypeID getTypeID() const {
         return id;
      }

      virtual bool isBoxedEquivOf(BuiltinType*) {
         return false;
      }

      virtual bool isPointerTy() {
         return false;
      }

      virtual bool isIntegerTy() {
         return false;
      }

      virtual bool isUnsigned() {
         return false;
      }

      virtual unsigned int getBitwidth()
      {
         llvm_unreachable("Not an integer type");
      }

      virtual bool isPtrSizedInt() {
         return false;
      }

      virtual bool isInt64Ty(bool isUnsigned = false) {
         return false;
      }

      virtual bool isInt8Ty(bool isUnsigned = false) {
         return false;
      }

      virtual bool isInt1Ty(bool isUnsigned = false) {
         return false;
      }

      virtual bool isIntNTy(unsigned n, bool isUnsigned = false) {
         return false;
      }

      virtual bool isFloatTy() {
         return false;
      }

      virtual bool isDoubleTy() {
         return false;
      }

      virtual bool isFPType() {
         return false;
      }

      virtual bool isObject() const {
         return false;
      }

      virtual bool isProtocol() {
         return false;
      }

      virtual bool isStruct() {
         return false;
      }

      virtual bool isEnum() {
         return false;
      }

      virtual bool needsStructReturn()
      {
         return isStruct() || isProtocol() || isEnum();
      }

      virtual void isEnum(bool) {

      }
      virtual bool isOptionTy() {
         return false;
      }

      virtual bool isOptionOf(string&) {
         return false;
      }

      virtual bool isFunctionTy() {
         return false;
      }

      virtual bool isRawFunctionTy()
      {
         return false;
      }

      virtual bool isTupleTy() {
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

      virtual llvm::Type* getLlvmFunctionType() {
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
   T* cast(BuiltinType* t) {
      assert(T::classof(t) && "Check isa<> before casting");
      return static_cast<T*>(t);
   }

} // namespace cdot


#endif //CDOT_BUILTIN_TYPE_H
