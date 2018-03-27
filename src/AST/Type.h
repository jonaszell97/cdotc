//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_BUILTIN_TYPE_H
#define CDOT_BUILTIN_TYPE_H

#include <string>

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/FoldingSet.h>
#include <llvm/ADT/PointerIntPair.h>
#include <llvm/ADT/StringMap.h>

namespace cdot {

enum {
   TypeAlignmentInBits = 4,
   TypeAlignment = 1 << TypeAlignmentInBits
};

class Type;
class QualType;

} // namespace cdot

namespace llvm {

template <typename T>
class PointerLikeTypeTraits;

template<>
class PointerLikeTypeTraits< ::cdot::Type*> {
public:
   static inline void *getAsVoidPointer(::cdot::Type *P) { return P; }

   static inline ::cdot::Type *getFromVoidPointer(void *P)
   {
      return static_cast< ::cdot::Type*>(P);
   }

   enum { NumLowBitsAvailable = cdot::TypeAlignmentInBits };
};

template <>
struct isPodLike<cdot::QualType> { static const bool value = true; };

} // namespace llvm

namespace cdot {

namespace ast {
   class ASTContext;
   class Expression;
} // namespace ast

namespace diag {
   class DiagnosticBuilder;
} // namespace diag

class Type;
class QualType;

enum QualMask : unsigned {

};

class Qualifiers {
public:
   explicit Qualifiers(unsigned Quals)
      :  Quals(Quals)
   {
      assert(Quals == 0);
   }

   Qualifiers() : Quals(0)
   {}

   unsigned getRaw() const { return Quals; }

private:
   unsigned Quals;
};

namespace sema {
   class TemplateArgList;
} // namespace sema

namespace ast {
   class RecordDecl;
} // namespace ast

struct Variant;
class QualType;

enum class TypeID : unsigned char {
#  define CDOT_TYPE(Name) \
   Name##ID,
#  include "Types.def"
};

#define CDOT_TYPE(Name) \
class Name;

#define CDOT_BASE_TYPE(Name) CDOT_TYPE(Name)
#include "Types.def"

class Type {
public:
   enum BuiltinKind : unsigned char {
#  define CDOT_BUILTIN_TYPE(Name)   \
      Name,
#  include "Basic/BuiltinTypes.def"
   };

#  ifndef NDEBUG
   virtual
#  endif
   ~Type();

   static bool classof(Type const* T) { return true; }

   bool needsLvalueToRvalueConv() const
   {
      return !isStruct() && !isProtocol();
   }

#  define CDOT_TYPE(Name)        \
   bool is##Name() const;        \
   Name *as##Name();             \
   Name *as##Name() const;       \
   Name *uncheckedAs##Name() const;

#  define CDOT_BASE_TYPE(Name) CDOT_TYPE(Name)
#  include "Types.def"

   bool isIntegerType() const;
   bool isFPType() const;
   bool isUnknownAnyType() const;
   bool isVoidType() const;
   bool isAutoType() const;

   bool isLargeInteger() const { return isIntegerType() && getBitwidth() > 64; }
   bool isLargeFP() const { return isFPType() && getPrecision() > 64; }

   bool isFloatTy() const;
   bool isDoubleTy() const;

   bool isRawFunctionTy() const;
   bool isRawEnum() const;

   bool isRealTypedefType() const; // isTypedefType queries the aliased type
   TypedefType *asRealTypedefType();
   TypedefType const* asRealTypedefType() const;

   bool isCanonical() const { return this == CanonicalType; }
   Type *getCanonicalType() const { return CanonicalType; }

   Type *getDesugaredType() const;

   bool isSelfTy() const;

   bool isNumeric() const
   {
      return isIntegerType() || isFPType() || isRawEnum();
   }

   bool isDependentType() const { return Bits.Dependent; }
   bool isUnpopulatedType() const;

   sema::TemplateArgList const& getTemplateArgs() const;
   bool hasTemplateArgs() const;

   llvm::StringRef getClassName() const;
   ast::RecordDecl *getRecord() const;

   std::string toString() const;
   std::string toUniqueString() const;

   QualType getPointeeType() const;
   QualType getReferencedType() const;
   QualType stripReference() const;

   unsigned short getAlignment() const;
   size_t getSize() const;

   size_t getMemberSize() const;
   unsigned short getMemberAlignment() const;


   TypeID getTypeID() const { return Bits.id; }

   PointerType *getPointerTo(ast::ASTContext &Ctx) const;

   bool isStringRepresentable() const;
   bool isSelfComparable() const;
   bool isHashable() const;

   bool isUnsigned() const;
   unsigned short getBitwidth() const;

   Type *getSignedOfSameWidth(ast::ASTContext &Ctx) const;
   Type *getUnsignedOfSameWidth(ast::ASTContext &Ctx) const;

   unsigned short getPrecision() const;

   bool isPtrSizedInt() const
   {
      return isIntegerType() && getBitwidth() == sizeof(void*) * 8;
   }

   bool isInt64Ty(bool Unsigned) const
   {
      return isIntegerType() && getBitwidth() == 64 && isUnsigned() == Unsigned;
   }

   bool isInt8Ty(bool Unsigned) const
   {
      return isIntegerType() && getBitwidth() == 8 && isUnsigned() == Unsigned;
   }

   bool isInt1Ty(bool Unsigned) const
   {
      return isIntegerType() && getBitwidth() == 1 && isUnsigned() == Unsigned;
   }

   bool isIntNTy(unsigned n, bool Unsigned) const
   {
      return isIntegerType() && getBitwidth() == n && isUnsigned() == Unsigned;
   }

   bool isInt64Ty() const
   {
      return isIntegerType() && getBitwidth() == 64;
   }

   bool isInt8Ty() const
   {
      return isIntegerType() && getBitwidth() == 8;
   }

   bool isInt1Ty() const
   {
      return isIntegerType() && getBitwidth() == 1;
   }

   bool isIntNTy(unsigned n) const
   {
      return isIntegerType() && getBitwidth() == n;
   }

   bool isClass() const;
   bool isUnion() const;
   bool isProtocol() const;
   bool isStruct() const;
   bool isEnum() const;

   bool needsStructReturn() const;

   bool isOptionTy() const;

   bool isRefcounted() const { return isClass(); }

   bool needsCleanup() const;

   bool isValueType() const
   {
      return isNumeric() || isRawFunctionTy() || isStruct();
   }

   bool needsMemCpy() const;

   operator QualType();

protected:
   Type(TypeID id, Type *CanonicalType, bool Dependent = false)
      : CanonicalType(CanonicalType ? CanonicalType : this)
   {
      Bits.id = id;
      Bits.Dependent = Dependent;
   }

   struct TypeBits {
      TypeID id      : 8;
      bool Dependent : 1;
   };

   enum { TypeUsedBits = 9 };

   struct BuiltinTypeBits {
      unsigned : TypeUsedBits;
      BuiltinKind kind : 8;
   };

   struct ArrayTypeBits {
      unsigned : TypeUsedBits;
   };

   struct FunctionTypeBits {
      unsigned : TypeUsedBits;
      unsigned flags : 16;
   };

   struct RecordTypeBits {
      unsigned : TypeUsedBits;
   };

   union {
      TypeBits Bits;
      BuiltinTypeBits BuiltinBits;
      ArrayTypeBits ArrayBits;
      FunctionTypeBits FuncBits;
      RecordTypeBits RecordBits;
   };

   Type *CanonicalType;
};

class BuiltinType: public Type {
public:
   BuiltinKind getKind() const { return BuiltinBits.kind; }

   unsigned short getIntegerBitwidth() const;
   bool isUnsignedInteger() const;

   unsigned short getFloatingPointPrecision() const;

   bool isAnyIntegerType() const;
   bool isIntNTy(unsigned short n);
   bool isIntNTy(unsigned short n, bool isUnsigned);

   bool isAnyFloatingPointType() const;
   bool isFloatTy()  const { return getKind() == f32; }
   bool isDoubleTy() const { return getKind() == f64; }

#  define CDOT_BUILTIN_TYPE(Name)   \
   bool is##Name##Ty() const { return getKind() == Name; }
#  include "Basic/BuiltinTypes.def"

   static bool classof(Type const *T)
   {
      return T->getTypeID() == TypeID::BuiltinTypeID;
   }

   friend class ast::ASTContext;

private:
   explicit BuiltinType(BuiltinKind kind)
      : Type(TypeID::BuiltinTypeID, this, kind == UnknownAny)
   {
      BuiltinBits.kind = kind;
   }
};

class QualType {
   using ValueTy = llvm::PointerIntPair<Type*, TypeAlignmentInBits>;
   ValueTy Value;

   enum class FromOpaque {
      Placeholder
   };

   QualType(void *Ptr, FromOpaque)
      : Value(ValueTy::getFromOpaqueValue(Ptr))
   {}

public:
   QualType() : Value(nullptr)
   {}

   QualType(Type *ty)
      : Value(ty)
   {

   }

   QualType(Type *ty, Qualifiers quals)
      : Value(ty, quals.getRaw())
   {
      assert(quals.getRaw() == 0);
   }

   QualType(Type *ty, unsigned quals)
      : Value(ty, quals)
   {
      assert(quals == 0);
   }

   bool operator==(const QualType& rhs) const
   {
      return Value == rhs.Value;
   }

   bool operator!=(const QualType& rhs) const
   {
      return !operator==(rhs);
   }

   QualType without(unsigned Quals) const
   {
      return QualType(Value.getPointer(), Value.getInt() & ~Quals);
   }

   std::string toString() const;

   bool isCanonical() const
   {
      return (**this)->isCanonical();
   }

   QualType getCanonicalType() const
   {
      return QualType((**this)->getCanonicalType(), Value.getInt());
   }

   Type* operator->() const
   {
      return Value.getPointer();
   }

   Type* operator*() const
   {
      return Value.getPointer();
   }

   Type *getBuiltinTy() const
   {
      return Value.getPointer();
   }

   QualType getPointerTo(ast::ASTContext &Ctx) const;

   operator Type*()
   {
      return Value.getPointer();
   }

   operator bool() const
   {
      return !isNull();
   }

   bool isNull() const
   {
      return Value.getPointer() == nullptr;
   }

   bool isUnknownAny() const
   {
      return !isNull() && Value.getPointer()->isUnknownAnyType();
   }

   QualType &operator=(Type *ty)
   {
      Value.setPointer(ty);
      return *this;
   }

   void *getAsOpaquePtr() const
   {
      return Value.getOpaqueValue();
   }

   static QualType getFromOpaquePtr(void *Ptr)
   {
      return QualType(Ptr, FromOpaque::Placeholder);
   }
};

llvm::raw_ostream &operator<<(llvm::raw_ostream &OS, QualType Ty);

} // namespace cdot

namespace std {
   template<> struct hash<::cdot::QualType>
   {
      size_t operator()(const ::cdot::QualType &T) const
      {
         return reinterpret_cast<size_t>(T.getAsOpaquePtr());
      }
   };
} // namespace std

namespace llvm {

template<class T> struct simplify_type;
template<class T> struct DenseMapInfo;

template<> struct simplify_type<::cdot::QualType> {
   using SimpleType = const ::cdot::Type*;

   static SimpleType getSimplifiedValue(::cdot::QualType Val)
   {
      return *Val;
   }
};

template<>
class PointerLikeTypeTraits<::cdot::QualType> {
public:
   static inline void *getAsVoidPointer(::cdot::QualType P)
   {
      return P.getAsOpaquePtr();
   }

   static inline ::cdot::QualType getFromVoidPointer(void *P)
   {
      return ::cdot::QualType::getFromOpaquePtr(P);
   }

   // Various qualifiers go in low bits.
   enum { NumLowBitsAvailable = 0 };
};

template<> struct DenseMapInfo<::cdot::QualType> {
   static ::cdot::QualType getEmptyKey() {
      uintptr_t Val = static_cast<uintptr_t>(-1);
      Val <<= ::cdot::TypeAlignmentInBits;
      return ::cdot::QualType::getFromOpaquePtr((void*)Val);
   }

   static ::cdot::QualType getTombstoneKey() {
      uintptr_t Val = static_cast<uintptr_t>(-2);
      Val <<= ::cdot::TypeAlignmentInBits;
      return ::cdot::QualType::getFromOpaquePtr((void*)Val);
   }

   static int getHashValue(const ::cdot::QualType &P) {
      return (int)(uintptr_t)P.getAsOpaquePtr();
   }

   static bool isEqual(const ::cdot::QualType &LHS,
                       const ::cdot::QualType &RHS) {
      return LHS == RHS;
   }
};

} // namespace llvm

namespace cdot {

class PointerType: public Type, public llvm::FoldingSetNode {
protected:
   explicit PointerType(QualType pointee, Type *CanonicalType)
      : Type(TypeID::PointerTypeID, CanonicalType, pointee->isDependentType()),
        pointeeType(pointee)
   {}

public:
   QualType getPointeeType() const
   {
      return pointeeType;
   }

   size_t getSize() const
   {
      return sizeof(void*);
   }

   unsigned short getAlignment() const
   {
      return alignof(void*);
   }

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, getPointeeType());
   }

   static void Profile(llvm::FoldingSetNodeID &ID, QualType pointee)
   {
      ID.AddPointer(pointee.getAsOpaquePtr());
   }

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::PointerTypeID;
   }

   friend class ast::ASTContext;

protected:
   QualType pointeeType;
};

class ReferenceType: public Type, public llvm::FoldingSetNode {
protected:
   ReferenceType(QualType referencedType, Type *CanonicalType)
      : Type(TypeID::ReferenceTypeID, CanonicalType,
             referencedType->isDependentType()),
        referencedType(referencedType)
   {}

public:
   QualType getReferencedType() const
   {
      return referencedType;
   }

   size_t getSize() const
   {
      return sizeof(void*);
   }

   unsigned short getAlignment() const
   {
      return alignof(void*);
   }

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, getReferencedType());
   }

   static void Profile(llvm::FoldingSetNodeID &ID, QualType referencedTy)
   {
      ID.AddPointer(referencedTy.getAsOpaquePtr());
   }

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::ReferenceTypeID;
   }

   friend class ast::ASTContext;

protected:
   QualType referencedType;
};

class MovedType: public Type, public llvm::FoldingSetNode {
protected:
   MovedType(QualType referencedType, Type *CanonicalType)
      : Type(TypeID::MovedTypeID, CanonicalType,
             referencedType->isDependentType()),
        referencedType(referencedType)
   {}

public:
   QualType getReferencedType() const
   {
      return referencedType;
   }

   size_t getSize() const
   {
      return sizeof(void*);
   }

   unsigned short getAlignment() const
   {
      return alignof(void*);
   }

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, getReferencedType());
   }

   static void Profile(llvm::FoldingSetNodeID &ID, QualType referencedTy)
   {
      ID.AddPointer(referencedTy.getAsOpaquePtr());
   }

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::MovedTypeID;
   }

   friend class ast::ASTContext;

protected:
   QualType referencedType;
};

class ArrayType: public Type, public llvm::FoldingSetNode {
public:
   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, getElementType(), getNumElements());
   }

   static void Profile(llvm::FoldingSetNodeID &ID, QualType elementType,
                       size_t numElements) {
      ID.AddPointer(elementType.getAsOpaquePtr());
      ID.AddInteger(numElements);
   }

   static bool classof(Type const* T)
   {
      switch (T->getTypeID()) {
         case TypeID::ArrayTypeID:
         case TypeID::DependentSizeArrayTypeID:
         case TypeID::InferredSizeArrayTypeID:
            return true;
         default:
            return false;
      }
   }

   friend class ast::ASTContext;

   size_t getSize() const
   {
      return elementType->getSize() * numElements;
   }

   unsigned short getAlignment() const
   {
      return elementType->getAlignment();
   }

protected:
   ArrayType(QualType elementType, size_t numElements, Type *CanonicalType)
      : Type(TypeID::ArrayTypeID, CanonicalType,
             elementType->isDependentType()),
        elementType(elementType), numElements(numElements)
   {}

   ArrayType(TypeID typeID, QualType elementType, Type *CanonicalType)
      : Type(typeID, CanonicalType, elementType->isDependentType()),
        elementType(elementType), numElements(0)
   {}

   QualType elementType;
   size_t numElements;

public:
   QualType getElementType() const
   {
      return elementType;
   }

   size_t getNumElements() const
   {
      return numElements;
   }
};

class DependentSizeArrayType: public ArrayType {
public:
   ast::Expression *getDependentExpr() const
   {
      return DependentExpr;
   }

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, getElementType(), getDependentExpr());
   }

   static void Profile(llvm::FoldingSetNodeID &ID, QualType elementType,
                       ast::Expression *DependentExpr) {
      ID.AddPointer(elementType.getAsOpaquePtr());
      ID.AddPointer(DependentExpr);
   }

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::DependentSizeArrayTypeID;
   }

   friend class ast::ASTContext;

private:
   DependentSizeArrayType(QualType elementType, ast::Expression *DependentExpr,
                          Type *CanonicalType)
      : ArrayType(TypeID::DependentSizeArrayTypeID, elementType, CanonicalType),
        DependentExpr(DependentExpr)
   {}

   ast::Expression *DependentExpr;
};

class InferredSizeArrayType: public ArrayType {
   InferredSizeArrayType(QualType elementTy, Type *CanonicalTy)
      : ArrayType(TypeID::InferredSizeArrayTypeID, elementTy, CanonicalTy)
   {

   }

public:
   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::InferredSizeArrayTypeID;
   }

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, getElementType());
   }

   static void Profile(llvm::FoldingSetNodeID &ID, QualType elementType)
   {
      ID.AddPointer(elementType.getAsOpaquePtr());
   }

   friend class ast::ASTContext;
};

class FunctionType: public Type, public llvm::FoldingSetNode {
public:
   enum ExtFlags : unsigned {
      None         = 0u,
      Vararg       = 1u,
      CStyleVararg = Vararg << 1u,
      NoThrow      = CStyleVararg << 1u,
   };

private:
   FunctionType(QualType returnType,
                llvm::ArrayRef<QualType> argTypes,
                ExtFlags flags,
                Type *CanonicalType,
                bool Dependent);

protected:
   FunctionType(TypeID typeID,
                QualType returnType,
                llvm::ArrayRef<QualType> argTypes,
                ExtFlags flags,
                Type *CanonicalType,
                bool Dependent);

   QualType returnType;
   unsigned NumParams;

public:
   QualType getReturnType() const
   {
      return returnType;
   }

   using iterator = const QualType*;

   iterator param_begin() const { return reinterpret_cast<iterator>(this + 1); }
   iterator param_end() const
   {
      return param_begin() + NumParams;
   }

   llvm::ArrayRef<QualType> getParamTypes() const
   {
      return { param_begin(), NumParams };
   }

   bool isRawFunctionTy() const { return Bits.id == TypeID::FunctionTypeID; }
   bool isVararg()        const { return (FuncBits.flags & Vararg) != 0; }
   bool isCStyleVararg()  const { return (FuncBits.flags & CStyleVararg) != 0; }
   bool isNoThrow()       const { return (FuncBits.flags & NoThrow) != 0; }

   unsigned getRawFlags() const { return FuncBits.flags; }

   size_t getSize() const
   {
      if (isRawFunctionTy())
         return sizeof(void(*)());

      // size of lambda object
      return 2 * sizeof(void*);
   }

   unsigned short getAlignment() const
   {
      return sizeof(void*);
   }

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, getReturnType(), getParamTypes(), FuncBits.flags,
              Bits.id == TypeID::LambdaTypeID);
   }

   static void Profile(llvm::FoldingSetNodeID &ID, QualType returnType,
                       llvm::ArrayRef<QualType> argTypes,
                       unsigned flags, bool isLambda) {
      ID.AddPointer(returnType.getAsOpaquePtr());
      for (auto ty : argTypes)
         ID.AddPointer(ty.getAsOpaquePtr());

      ID.AddBoolean(isLambda);
      ID.AddInteger(flags);
   }

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::FunctionTypeID
             || T->getTypeID() == TypeID::LambdaTypeID;
   }

   friend class ast::ASTContext;
};

class LambdaType: public FunctionType {
   LambdaType(QualType returnType,
              llvm::ArrayRef<QualType> argTypes,
              ExtFlags flags,
              Type *CanonicalType,
              bool Dependent)
      : FunctionType(TypeID::LambdaTypeID, returnType, argTypes, flags,
                     CanonicalType, Dependent)
   {

   }

public:
   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::LambdaTypeID;
   }

   friend class ast::ASTContext;
};

static_assert(sizeof(LambdaType) == sizeof(FunctionType),
              "all function types must be the same size!");

class TupleType final: public Type, public llvm::FoldingSetNode {
   TupleType(llvm::ArrayRef<QualType> containedTypes,
             Type *CanonicalType,
             bool Dependent);

   unsigned NumTys;

public:
   using iterator = const QualType*;

   iterator type_begin() const { return reinterpret_cast<iterator>(this + 1); }
   iterator type_end()   const { return type_begin() + NumTys; }

   QualType getContainedType(size_t i) const
   {
      assert(i < NumTys && "type index out of bounds");
      return *(type_begin() + i);
   }

   llvm::ArrayRef<QualType> getContainedTypes() const
   {
      return { type_begin(), NumTys };
   }

   size_t getArity() const { return NumTys; }

   unsigned short getAlignment() const
   {
      unsigned short align = 1;
      for (auto ty : getContainedTypes()) {
         auto al = ty->getMemberAlignment();
         if (al > align)
            align = al;
      }

      return align;
   }

   size_t getSize() const
   {
      size_t size = 0;
      for (auto ty : getContainedTypes())
         size += ty->getMemberSize();

      return size;
   }

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, getContainedTypes());
   }

   static void Profile(llvm::FoldingSetNodeID &ID,
                       llvm::ArrayRef<QualType> containedTypes) {
      for (auto ty : containedTypes)
         ID.AddPointer(ty.getAsOpaquePtr());
   }

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::TupleTypeID;
   }

   friend class ast::ASTContext;
};

namespace ast {
class RecordDecl;
} // namespace ast

class RecordType: public Type, public llvm::FoldingSetNode {
protected:
   explicit RecordType(ast::RecordDecl *record)
      : Type(TypeID::RecordTypeID, nullptr), Rec(record)
   {}

   RecordType(TypeID typeID,
              ast::RecordDecl *record,
              bool Dependent)
      : Type(typeID, nullptr, Dependent), Rec(record)
   {}

   ast::RecordDecl *Rec;

public:
   bool isRawEnum() const;

   sema::TemplateArgList const& getTemplateArgs() const;
   bool hasTemplateArgs() const;

   ast::RecordDecl *getRecord() const
   {
      return Rec;
   }

   unsigned short getAlignment() const;
   size_t getSize() const;

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, getRecord());
   }

   static void Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl *R)
   {
      ID.AddPointer(R);
   }

   static bool classof(RecordType const*) { return true; }
   static bool classof(Type const* T)
   {
      switch(T->getTypeID()) {
#     define CDOT_OBJ_TYPE(Name) \
         case TypeID::Name##ID: return true;
#     include "Types.def"

         default: return false;
      }
   }

   friend class ast::ASTContext;
};

// to be used when resolving template arguments and the actual underlying
// record of the type might not exist
class DependentRecordType: public RecordType {
protected:
   DependentRecordType(ast::RecordDecl *record,
                        sema::TemplateArgList *templateArgs)
      : RecordType(TypeID::DependentRecordTypeID, record, true),
        templateArgs(templateArgs)
   {}

   sema::TemplateArgList *templateArgs;

public:
   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, getRecord(), templateArgs);
   }

   static void Profile(llvm::FoldingSetNodeID &ID,
                       ast::RecordDecl *R,
                       sema::TemplateArgList* templateArgs);

   static bool classof (Type const* T)
   {
      return T->getTypeID() == TypeID::DependentRecordTypeID;
   }

   friend class ast::ASTContext;

   const sema::TemplateArgList& getTemplateArgs() const
   {
      return *templateArgs;
   }

   bool hasTemplateArgs() const
   {
      return true;
   }
};

class GenericType: public Type, public llvm::FoldingSetNode {
protected:
   GenericType(QualType actualType,
               llvm::StringRef genericTypeName)
      : Type(TypeID::GenericTypeID, nullptr, true),
        actualType(actualType), genericTypeName(genericTypeName)
   {}

   QualType actualType;
   llvm::StringRef genericTypeName;

public:
   llvm::StringRef getGenericTypeName() const
   {
      return genericTypeName;
   }

   QualType getActualType() const { return actualType; }

   unsigned short getAlignment() const { return actualType->getAlignment(); }
   size_t getSize() const { return actualType->getSize(); }

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, getActualType(), getGenericTypeName());
   }

   static void Profile(llvm::FoldingSetNodeID &ID, QualType actualType,
                       llvm::StringRef typeName) {
      ID.AddPointer(actualType.getAsOpaquePtr());
      ID.AddString(typeName);
   }

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::GenericTypeID;
   }

   friend class ast::ASTContext;
};

namespace ast {
   class TypedefDecl;
   class AliasDecl;
   class NamespaceDecl;
} // namespace ast

class MetaType: public Type, public llvm::FoldingSetNode {
public:
   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, getUnderlyingType());
   }

   static void Profile(llvm::FoldingSetNodeID &ID, QualType underlyingType)
   {
      ID.AddPointer(underlyingType.getAsOpaquePtr());
   }

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::MetaTypeID;
   }

   friend class ast::ASTContext;

   QualType getUnderlyingType() const { return forType; }

   size_t getSize() const { llvm_unreachable("should never be a member"); }

   unsigned short getAlignment() const
   { llvm_unreachable("should never be a member"); }

protected:
   explicit MetaType(QualType forType, Type *CanonicalType)
      : Type(TypeID::MetaTypeID, CanonicalType, forType->isDependentType()),
        forType(forType)
   {}

   QualType forType;
};

class NamespaceType: public Type, public llvm::FoldingSetNode {
public:
   ast::NamespaceDecl *getNamespace() const
   {
      return NS;
   }

   llvm::StringRef getNamespaceName() const;

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, getNamespace());
   }

   static void Profile(llvm::FoldingSetNodeID &ID, ast::NamespaceDecl *NS)
   {
      ID.AddPointer(NS);
   }

   static bool classof(Type const *T)
   {
      return T->getTypeID() == TypeID::NamespaceTypeID;
   }

   friend class ast::ASTContext;

private:
   explicit NamespaceType(ast::NamespaceDecl *NS)
      : Type(TypeID::NamespaceTypeID, this), NS(NS)
   {

   }

   ast::NamespaceDecl *NS;
};

class TypedefType: public Type, public llvm::FoldingSetNode {
public:
   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, getTypedef());
   }

   static void Profile(llvm::FoldingSetNodeID &ID, ast::TypedefDecl *TD)
   {
      ID.AddPointer(TD);
   }

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::TypedefTypeID;
   }

   friend class ast::ASTContext;

   ast::TypedefDecl *getTypedef() const
   {
      return td;
   }

   QualType getAliasedType() const;
   llvm::StringRef getAliasName() const;

private:
   explicit TypedefType(ast::TypedefDecl *td)
      : Type(TypeID::TypedefTypeID, nullptr),
        td(td)
   {
      CanonicalType = getAliasedType();
   }

   ast::TypedefDecl *td;
};

} // namespace cdot

namespace llvm {

template <class X> inline bool
isa(::cdot::Type*) = delete;
template <class X> inline typename llvm::cast_retty<X, ::cdot::Type*>::ret_type
cast(::cdot::Type*) = delete;
template <class X> inline typename llvm::cast_retty<X, ::cdot::Type*>::ret_type
dyn_cast(::cdot::Type*) = delete;
template <class X> inline typename llvm::cast_retty<X, ::cdot::Type*>::ret_type
dyn_cast_or_null(::cdot::Type*) = delete;

} // namespace llvm

#endif //CDOT_BUILTIN_TYPE_H
