//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_BUILTIN_TYPE_H
#define CDOT_BUILTIN_TYPE_H

#include <string>

#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/ArrayRef.h>

#include "../../Support/Casting.h"

namespace cdot {

namespace sema {

class TemplateArgList;

} // namespace sema

struct Variant;
class TemplateArg;
class QualType;

namespace cl {

class Record;
class AssociatedType;

} // namespace cl

enum class TypeID : unsigned {
#  define CDOT_TYPE(Name) \
   Name##ID,
#  include "Types.def"
};

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
class NamespaceType;

class Type {
public:
   enum BoxedPrimitive : unsigned short {
#     define CDOT_INTEGER(Name, BW, Unsigned) \
      Name = BW + (100 * Unsigned),
#     define CDOT_FP(Name, BW)                \
      Name = 200 + BW,
#     define CDOT_OTHER(Name, BW)             \
      Name = 300 + BW,

#     include "Primitive.def"
      BP_None = 0
   };

#  ifndef NDEBUG
   virtual
#  endif
   ~Type();

   static Type *get(BoxedPrimitive bp);

   static bool classof(Type const* T) { return true; }
   static void TearDown();

   bool needsLvalueToRvalueConv() const
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
   bool isRawEnum() const;

   bool isSelfTy() const;

   BoxedPrimitive getPrimitiveKind() const;
   bool is(BoxedPrimitive primitive) const;

   bool isNumeric() const
   {
      return isIntegerTy() || isFPType() || isRawEnum();
   }

   bool isDependantType() const;

   Type *getGroupDefault() const;

   sema::TemplateArgList const& getTemplateArgs() const;
   bool hasTemplateArgs() const;

   llvm::StringRef getClassName() const;
   cl::Record *getRecord() const;
   bool isBoxedPrimitive() const;

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

   PointerType* getPointerTo();

   std::string toString() const;
   std::string toUniqueString() const;

   QualType getPointeeType() const;

   bool implicitlyCastableTo(Type const* to) const;
   bool explicitlyCastableTo(Type const* to) const;
   bool needsCastTo(const Type *ty) const;

   Type* unbox() const;
   Type* box() const;

   bool hasDefaultValue() const;

   unsigned short getAlignment() const;
   size_t getSize() const;

   size_t getMemberSize() const;
   unsigned short getMemberAlignment() const;


   TypeID getTypeID() const
   {
      return id;
   }

   bool isStringRepresentable() const;
   bool isSelfComparable() const;
   bool isHashable() const;

   bool isBoxedEquivOf(Type const* ty) const;
   bool isUnsigned() const;
   unsigned int getBitwidth() const;

   bool isPtrSizedInt() const
   {
      return isIntegerTy() && getBitwidth() == sizeof(void*) * 8;
   }

   bool isInt64Ty(bool Unsigned = false) const
   {
      return isIntegerTy() && getBitwidth() == 64 && isUnsigned() == Unsigned;
   }

   bool isInt8Ty(bool Unsigned = false) const
   {
      return isIntegerTy() && getBitwidth() == 8 && isUnsigned() == Unsigned;
   }

   bool isInt1Ty(bool Unsigned = false) const
   {
      return isIntegerTy() && getBitwidth() == 1 && isUnsigned() == Unsigned;
   }

   bool isIntNTy(unsigned n, bool Unsigned = false) const
   {
      return isIntegerTy() && getBitwidth() == n && isUnsigned() == Unsigned;
   }

   bool isDummyObject() const;

   bool isClass() const;
   bool isUnion() const;
   bool isProtocol() const;
   bool isStruct() const;
   bool isEnum() const;

   bool needsStructReturn() const;

   bool isOptionTy() const;
   bool isOptionOf(const std::string&) const;

   bool isRefcounted() const { return isClass(); }

   bool isReferenceType() const
   {
      return isRefcounted();
   }

   bool needsCleanup() const;

   bool isValueType() const
   {
      return isNumeric() || isRawFunctionTy() || isStruct();
   }

   bool needsMemCpy() const;

   operator QualType();

protected:
   static llvm::StringMap<Type*> Instances;
   TypeID id;
};

} // namespace cdot


#endif //CDOT_BUILTIN_TYPE_H
