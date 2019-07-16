//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_BUILTIN_TYPE_H
#define CDOT_BUILTIN_TYPE_H

#include "Support/Casting.h"
#include "Support/LLVM.h"

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
class CanType;
class QualType;
class NestedNameSpecifier;
class NestedNameSpecifierWithLoc;

} // namespace cdot

namespace llvm {

template <typename T>
struct PointerLikeTypeTraits;

template<>
struct PointerLikeTypeTraits< ::cdot::Type*> {
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

template <>
struct isPodLike<cdot::CanType> { static const bool value = true; };

class raw_ostream;

} // namespace llvm

namespace cdot {

namespace ast {
   class ASTContext;
   class Expression;
   class StaticExpr;
   class TemplateParamDecl;
   class AssociatedTypeDecl;
   class ProtocolDecl;
   struct SourceType;
} // namespace ast

namespace diag {
   class DiagnosticBuilder;
} // namespace diag

class IdentifierInfo;
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
   class FinalTemplateArgumentList;
} // namespace sema

namespace ast {
   class RecordDecl;
} // namespace ast

struct Variant;
class QualType;

#define CDOT_TYPE(Name, Parent) \
class Name;

#define CDOT_BASE_TYPE(Name) CDOT_TYPE(Name, "")
#include "Types.def"

struct TypeProperties {
public:
   enum Property : uint16_t {
      /// This type contains an unconstrained generic type.
      ContainsUnconstrainedGeneric        = 0x1,

      /// This type contains a generic type.
      ContainsTemplateParamType                 = 0x2,

      /// This type containts an associated type or Self constraint.
      ContainsAssociatedType              = 0x4,

      /// This type contains an unexpanded parameter pack.
      ContainsUnexpandedParameterPack     = 0x8,

      /// This type contains a dependent name type.
      ContainsDependentNameType           = 0x10,

      /// This type contains a dependently sized array type.
      ContainsDependentSizeArrayType      = 0x20,

      /// This type contains an UnknownAny type.
      ContainsUnknownAny                  = 0x40,

      /// This type contains a runtime generic parameter.
      ContainsRuntimeGenericParam         = 0x80,

      /// This type contains a template with no specified template parameters.
      ContainsTemplate                    = 0x100,

      /// This type contains a type variable type.
      ContainsTypeVariable                = 0x200,

      _lastProp = ContainsTypeVariable
   };

   static_assert(_lastProp <= (1 << 15), "too many type properties!");

private:
   uint16_t Props;

public:
   /*implicit*/ TypeProperties(uint16_t Props = 0) : Props(Props)
   {}

   uint16_t getRawProperties() const { return Props; }

   bool operator&(Property P) { return (Props & P) != 0; }
   bool operator&(TypeProperties P) { return (Props & P.Props) != 0; }

   TypeProperties operator|(Property P) { return TypeProperties(Props | P); }
   TypeProperties &operator|=(Property P) { Props |= P; return *this; }

   TypeProperties operator|(TypeProperties P)
   { return TypeProperties(Props | P.Props); }

   TypeProperties &operator|=(TypeProperties P)
   { Props |= P.Props; return *this; }

   TypeProperties operator-(Property P) { return TypeProperties(Props & ~P); }

   bool isDependent() const;
   bool containsTemplateParamType() const;
   bool containsAssociatedType() const;
   bool containsTemplate() const;
   bool containsUnexpandedParameterPack() const;
   bool containsRuntimeGenericParam() const;
   bool containsTypeVariable() const;
};

class Type {
public:
   enum TypeID : unsigned char {
#  define CDOT_TYPE(Name, Parent) \
   Name##ID,
#  include "Types.def"
   };

#ifndef NDEBUG
   static void verifyID(TypeID ID);
#endif

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

   template<class T>
   const T *getAs() const;

   template<class T>
   bool isa() const;

#  define CDOT_TYPE(Name, Parent)  \
   bool is##Name() const;          \
   Name *as##Name();               \
   Name *as##Name() const;         \
   Name *uncheckedAs##Name() const;

#  define CDOT_BASE_TYPE(Name) CDOT_TYPE(Name, "")
#  include "Types.def"

   bool isNonMutableReferenceType() const
   {
      return isReferenceType() && !isMutableReferenceType();
   }

   bool isNonMutablePointerType() const
   {
      return isPointerType() && !isMutablePointerType();
   }

   bool isIntegerType() const;
   bool isFPType() const;
   bool isUnknownAnyType() const;
   bool isVoidType() const;
   bool isAutoType() const;
   bool isEmptyTupleType() const;
   bool isErrorType() const;

   bool isLargeInteger() const { return isIntegerType() && getBitwidth() > 64; }
   bool isLargeFP() const { return isFPType() && getPrecision() > 64; }

   bool isFloatTy() const;
   bool isDoubleTy() const;

   bool isThinFunctionTy() const;
   bool isRawEnum() const;

   bool isRealTypedefType() const; // isTypedefType queries the aliased type
   TypedefType *asRealTypedefType();
   TypedefType const* asRealTypedefType() const;

   bool isCanonical() const { return this == CanonicalType; }
   CanType getCanonicalType() const;
   QualType getDesugaredType() const;

   bool isNumeric() const
   {
      return isIntegerType() || isFPType() || isRawEnum();
   }

   bool isDependentType() const { return properties().isDependent(); }
   bool containsTemplateParamType() const
   { return properties().containsTemplateParamType(); }

   bool containsAssociatedType() const
   { return properties().containsAssociatedType(); }

   bool containsRuntimeGenericParam() const
   { return properties().containsRuntimeGenericParam(); }

   bool containsTypeVariable() const
   { return properties().containsTypeVariable(); }

   sema::FinalTemplateArgumentList& getTemplateArgs() const;
   bool hasTemplateArgs() const;

   ast::RecordDecl *getRecord() const;

   std::string toString() const;
   std::string toDiagString() const;

   QualType getPointeeType() const;
   QualType getReferencedType() const;
   QualType getBorrowedType() const;
   QualType getBoxedType() const;

   QualType removeReference() const;
   QualType removeMetaType() const;

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

   bool isOptionTy() const;

   bool isRefcounted() const;
   bool needsCleanup() const;

   bool isValueType() const
   {
      return isNumeric() || isThinFunctionTy() || isStruct();
   }

   TypeProperties properties() const { return Bits.Props; }

   operator QualType();

   using child_iterator       = const QualType*;
   using child_iterator_range = llvm::iterator_range<child_iterator>;

   child_iterator child_begin()    const;
   child_iterator child_end()      const;
   child_iterator_range children() const;

protected:
   Type(TypeID id, Type *CanonicalType)
      : CanonicalType(CanonicalType ? CanonicalType : this)
   {
      Bits.id = id;
      Bits.Props = TypeProperties();
   }

   struct TypeBits {
      TypeProperties Props;
      TypeID id : 8;

      // Make sure no padding is inserted between id and Props.
      unsigned : 8;
   };

   enum { TypeUsedBits = 24 };

   struct BuiltinTypeBits {
      unsigned : TypeUsedBits;
      BuiltinKind kind : 8;
   };

   struct FunctionTypeBits {
      unsigned : TypeUsedBits;
      unsigned flags : 8;
   };

   union {
      TypeBits Bits;
      BuiltinTypeBits BuiltinBits;
      FunctionTypeBits FuncBits;
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

   child_iterator child_begin() const { return child_iterator{}; }
   child_iterator child_end()   const { return child_iterator{}; }

   static bool classof(Type const *T)
   {
      return T->getTypeID() == TypeID::BuiltinTypeID;
   }

   friend class ast::ASTContext;

private:
   explicit BuiltinType(BuiltinKind kind);
};

class QualType {
protected:
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

   bool operator==(QualType rhs) const { return Value == rhs.Value; }
   bool operator!=(QualType rhs) const { return !operator==(rhs); }

   bool operator==(Type *rhs) const { return Value.getPointer() == rhs; }
   bool operator!=(Type *rhs) const { return !operator==(rhs); }

   QualType without(unsigned Quals) const
   {
      return QualType(Value.getPointer(), Value.getInt() & ~Quals);
   }

   std::string toString() const;
   std::string toDiagString() const;

   bool isCanonical() const
   {
      return (**this)->isCanonical();
   }

   CanType getCanonicalType() const;
   /*implicit*/ operator CanType() const;

   Type *operator->() const { return Value.getPointer(); }
   Type *operator *() const { return Value.getPointer(); }

   Type *getBuiltinTy() const { return Value.getPointer(); }

   Qualifiers getQuals() const { return Qualifiers(Value.getInt()); }

   QualType getPointerTo(ast::ASTContext &Ctx) const;

   operator Type*() const { return Value.getPointer(); }
   operator bool()  const { return !isNull(); }

   bool isNull() const { return Value.getPointer() == nullptr; }

   bool isUnknownAny() const
   {
      return !isNull() && Value.getPointer()->isUnknownAnyType();
   }

   QualType &operator=(Type *ty)
   {
      Value.setPointer(ty);
      return *this;
   }

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      ID.AddPointer(getAsOpaquePtr());
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

/// A QualType that is statically guaranteed to be canonical.
class CanType final: public QualType {
   /// Construct from a Type that must be canonical.
   explicit CanType(Type *T);

   /// Construct from a QualType that must be canonical.
   explicit CanType(QualType T);

   /// (Unchecked) constructor for tombstone keys.
   explicit CanType(void *Ptr);

public:
   friend class Type;
   friend class QualType;

   static CanType getFromOpaquePtr(void *Ptr);
   static CanType getFromOpaquePtrUnchecked(void *Ptr);

   CanType() = default;

   /// Constructors from types that are always canonical.
#  define CDOT_CAN_TYPE(TYPE, PARENT)                       \
   /*implicit*/ CanType(TYPE *T) : CanType((Type*)T) {}
#  define CDOT_CAN_DEPENDENT_TYPE(TYPE, PARENT) CDOT_CAN_TYPE(TYPE, PARENT)
#  include "Types.def"
};

inline CanType Type::getCanonicalType() const
{
   return CanType(CanonicalType);
}

inline CanType QualType::getCanonicalType() const
{
   return (*this)->getCanonicalType();
//   return CanType(QualType(getBuiltinTy()->getCanonicalType(), getQuals()));
}

inline QualType::operator CanType() const
{
   return getCanonicalType();
}

llvm::raw_ostream &operator<<(llvm::raw_ostream &OS, QualType Ty);
llvm::raw_ostream &operator<<(llvm::raw_ostream &OS, const ast::SourceType &Ty);

diag::DiagnosticBuilder &operator<<(diag::DiagnosticBuilder &Diag, QualType Ty);
diag::DiagnosticBuilder &operator<<(diag::DiagnosticBuilder &Diag,
                                    const ast::SourceType &Ty);

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

// teach isa etc. to treat QualType like a type
template<> struct simplify_type<::cdot::QualType> {
   using SimpleType = ::cdot::Type*;

   static SimpleType getSimplifiedValue(::cdot::QualType Val)
   {
      return *Val;
   }
};

// teach isa etc. to treat CanType like a type
template<> struct simplify_type<::cdot::CanType> {
   using SimpleType = ::cdot::Type*;

   static SimpleType getSimplifiedValue(::cdot::CanType Val)
   {
      return *Val;
   }
};

template<>
struct PointerLikeTypeTraits<::cdot::QualType> {
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

template<>
struct PointerLikeTypeTraits<::cdot::CanType> {
public:
   static inline void *getAsVoidPointer(::cdot::CanType P)
   {
      return P.getAsOpaquePtr();
   }

   static inline ::cdot::CanType getFromVoidPointer(void *P)
   {
      return ::cdot::CanType::getFromOpaquePtrUnchecked(P);
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

template<> struct DenseMapInfo<::cdot::CanType> {
   static ::cdot::CanType getEmptyKey() {
      uintptr_t Val = static_cast<uintptr_t>(-1);
      Val <<= ::cdot::TypeAlignmentInBits;
      return ::cdot::CanType::getFromOpaquePtrUnchecked((void*)Val);
   }

   static ::cdot::CanType getTombstoneKey() {
      uintptr_t Val = static_cast<uintptr_t>(-2);
      Val <<= ::cdot::TypeAlignmentInBits;
      return ::cdot::CanType::getFromOpaquePtrUnchecked((void*)Val);
   }

   static int getHashValue(const ::cdot::CanType &P) {
      return (int)(uintptr_t)P.getAsOpaquePtr();
   }

   static bool isEqual(const ::cdot::CanType &LHS,
                       const ::cdot::CanType &RHS) {
      return LHS == RHS;
   }
};

} // namespace llvm

namespace cdot {

class PointerType: public Type, public llvm::FoldingSetNode {
private:
   PointerType(QualType pointee, Type *CanonicalType);

protected:
   PointerType(TypeID typeID, QualType pointee, Type *CanonicalType);

public:
   QualType getPointeeType() const { return pointeeType; }

   size_t getSize() const
   {
      return sizeof(void*);
   }

   unsigned short getAlignment() const
   {
      return alignof(void*);
   }

   child_iterator child_begin() const { return &pointeeType; }
   child_iterator child_end()   const { return &pointeeType + 1; }

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
      switch (T->getTypeID()) {
      case PointerTypeID:
      case MutablePointerTypeID:
         return true;
      default:
         return false;
      }
   }

   friend class ast::ASTContext;

protected:
   QualType pointeeType;
};

class MutablePointerType: public PointerType {
   MutablePointerType(QualType pointeeType, Type *CanonicalType);

public:
   static bool classof(Type const* T)
   {
      return T->getTypeID() == MutablePointerTypeID;
   }

   friend class ast::ASTContext;
};

class ReferenceType: public Type, public llvm::FoldingSetNode {
private:
   ReferenceType(QualType referencedType, Type *CanonicalType);

protected:
   ReferenceType(TypeID typeID, QualType referencedType, Type *CanonicalType);

public:
   QualType getReferencedType() const { return referencedType; }

   size_t getSize() const
   {
      return sizeof(void*);
   }

   unsigned short getAlignment() const
   {
      return alignof(void*);
   }

   child_iterator child_begin() const { return &referencedType; }
   child_iterator child_end()   const { return &referencedType + 1; }

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
      switch (T->getTypeID()) {
      case ReferenceTypeID:
      case MutableReferenceTypeID:
      case MutableBorrowTypeID:
         return true;
      default:
         return false;
      }
   }

   friend class ast::ASTContext;

protected:
   QualType referencedType;
};

class MutableReferenceType: public ReferenceType {
   MutableReferenceType(QualType referencedType, Type *CanonicalType);

public:
   static bool classof(Type const* T)
   {
      return T->getTypeID() == MutableReferenceTypeID
             || T->getTypeID() == MutableBorrowTypeID;
   }

   friend class ast::ASTContext;

protected:
   MutableReferenceType(TypeID ID,
                        QualType referencedType,
                        Type *CanonicalType);
};

class MutableBorrowType: public MutableReferenceType {
   MutableBorrowType(QualType borrowedType, Type *CanonicalType);

public:
   static bool classof(Type const* T)
   {
      return T->getTypeID() == MutableBorrowTypeID;
   }

   friend class ast::ASTContext;
};

class BoxType: public Type {
   QualType BoxedTy;

   BoxType(QualType BoxedTy, Type *CanonicalTy);

public:
   enum : unsigned { MemberCount = 4u };
   enum : unsigned {
      StrongRefcount = 0u,
      WeakRefcount,
      Deinitializer,
      ObjPtr,
   };

   QualType getBoxedType() const { return BoxedTy; }

   child_iterator child_begin() const { return &BoxedTy; }
   child_iterator child_end()   const { return &BoxedTy + 1; }

   static bool classof(Type const* T) { return T->getTypeID() == BoxTypeID; }
   friend class ast::ASTContext;
};

class TokenType: public Type {
   TokenType();

public:
   child_iterator child_begin() const { return nullptr; }
   child_iterator child_end()   const { return nullptr; }

   static bool classof(Type const* T) { return T->getTypeID() == TokenTypeID; }
   friend class ast::ASTContext;
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

   child_iterator child_begin() const { return &elementType; }
   child_iterator child_end() const { return &elementType + 1; }

   size_t getSize() const
   {
      return elementType->getSize() * numElements;
   }

   unsigned short getAlignment() const
   {
      return elementType->getAlignment();
   }

protected:
   ArrayType(QualType elementType, unsigned numElements, Type *CanonicalType);
   ArrayType(TypeID typeID, QualType elementType, Type *CanonicalType);

   QualType elementType;
   unsigned numElements;

public:
   QualType getElementType() const { return elementType; }
   unsigned getNumElements() const { return numElements; }
};

class DependentSizeArrayType: public ArrayType {
public:
   ast::StaticExpr *getSizeExpr() const
   {
      return DependentExpr;
   }

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, getElementType(), getSizeExpr());
   }

   static void Profile(llvm::FoldingSetNodeID &ID, QualType elementType,
                       ast::StaticExpr *DependentExpr) {
      ID.AddPointer(elementType.getAsOpaquePtr());
      ID.AddPointer(DependentExpr);
   }

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::DependentSizeArrayTypeID;
   }

   friend class ast::ASTContext;

private:
   DependentSizeArrayType(QualType elementType, ast::StaticExpr *DependentExpr,
                          Type *CanonicalType);

   ast::StaticExpr *DependentExpr;
};

class InferredSizeArrayType: public ArrayType {
   InferredSizeArrayType(QualType elementTy, Type *CanonicalTy);

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

class ExistentialType final: public Type, public llvm::FoldingSetNode {
   explicit ExistentialType(ArrayRef<QualType> Existentials,
                            Type *CanonicalType,
                            TypeProperties Props);

   /// The number of protocols this existential type entails.
   unsigned NumExistentials;

public:
   ArrayRef<QualType> getExistentials() const;

   void Profile(llvm::FoldingSetNodeID &ID);
   static void Profile(llvm::FoldingSetNodeID &ID,
                       ArrayRef<QualType> Existentials);

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::ExistentialTypeID;
   }

   child_iterator child_begin() const
   { return reinterpret_cast<child_iterator>(this + 1); }
   child_iterator child_end() const { return child_begin() + NumExistentials; }

   friend class ast::ASTContext;
};

enum class ArgumentConvention : unsigned char {
   Owned,
   Borrowed,
   ImmutableRef,
   MutableRef,
   Default,
};

class FunctionType: public Type, public llvm::FoldingSetNode {
public:
   struct ParamInfo {
      ParamInfo();
      ParamInfo(ArgumentConvention Conv, IdentifierInfo *Label = nullptr)
         : Conv(Conv), Label(Label)
      {}

      ArgumentConvention getConvention() const { return Conv; }
      IdentifierInfo *getLabel() const { return Label; }

      void Profile(llvm::FoldingSetNodeID &ID) const;
      bool operator==(const ParamInfo &I) const;
      bool operator!=(const ParamInfo &I) const { return !(*this == I); }

   private:
      ArgumentConvention Conv : 4;
      IdentifierInfo *Label;
   };

   enum ExtFlags : unsigned {
      None         = 0u,
      Vararg       = 1u,
      CStyleVararg = Vararg << 1u,
      Throws       = CStyleVararg << 1u,
      Async        = Throws << 1u,
      Unsafe       = Async << 1u,
   };

private:
   FunctionType(QualType returnType,
                llvm::ArrayRef<QualType> argTypes,
                llvm::ArrayRef<ParamInfo> paramInfo,
                ExtFlags flags,
                Type *CanonicalType,
                TypeProperties Props);

protected:
   FunctionType(TypeID typeID,
                QualType returnType,
                llvm::ArrayRef<QualType> argTypes,
                llvm::ArrayRef<ParamInfo> paramInfo,
                ExtFlags flags,
                Type *CanonicalType,
                TypeProperties Props);

   unsigned NumParams;
   QualType returnType; // must come last for 'children' to work!

public:
   QualType getReturnType() const
   {
      return returnType;
   }

   using iterator            = const QualType*;
   using param_info_iterator = const ParamInfo*;

   iterator param_begin() const { return reinterpret_cast<iterator>(this + 1); }
   iterator param_end() const
   {
      return param_begin() + NumParams;
   }

   llvm::ArrayRef<QualType> getParamTypes() const
   {
      return { param_begin(), NumParams };
   }

   param_info_iterator param_info_begin() const
   {
      return reinterpret_cast<param_info_iterator>(
         reinterpret_cast<iterator>(this + 1) + NumParams);
   }

   param_info_iterator param_info_end() const
   {
      return param_info_begin() + NumParams;
   }

   llvm::ArrayRef<ParamInfo> getParamInfo() const
   {
      return { param_info_begin(), NumParams };
   }

   unsigned getNumParams() const { return NumParams; }

   child_iterator child_begin() const { return &returnType; }
   child_iterator child_end() const { return param_end(); }

   bool isThinFunctionTy() const { return Bits.id == TypeID::FunctionTypeID; }
   bool isVararg()        const { return (FuncBits.flags & Vararg) != 0; }
   bool isCStyleVararg()  const { return (FuncBits.flags & CStyleVararg) != 0; }
   bool throws()          const { return (FuncBits.flags & Throws) != 0; }
   bool isAsync()         const { return (FuncBits.flags & Async) != 0; }
   bool isUnsafe()        const { return (FuncBits.flags & Unsafe) != 0; }

   unsigned getRawFlags() const { return FuncBits.flags; }

   size_t getSize() const
   {
      if (isThinFunctionTy())
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
      Profile(ID, getReturnType(), getParamTypes(), getParamInfo(),
              FuncBits.flags, Bits.id == TypeID::LambdaTypeID);
   }

   static void Profile(llvm::FoldingSetNodeID &ID, QualType returnType,
                       llvm::ArrayRef<QualType> argTypes,
                       llvm::ArrayRef<FunctionType::ParamInfo> paramInfo,
                       unsigned flags, bool isLambda) {
      ID.AddPointer(returnType.getAsOpaquePtr());
      for (auto ty : argTypes) {
         ID.AddPointer(ty.getAsOpaquePtr());
      }
      for (auto &Info : paramInfo) {
         Info.Profile(ID);
      }
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
              llvm::ArrayRef<ParamInfo> paramInfo,
              ExtFlags flags,
              Type *CanonicalType,
              TypeProperties Props);

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
             TypeProperties Props);

   unsigned NumTys;

public:
   using iterator = const QualType*;

   iterator type_begin() const { return reinterpret_cast<iterator>(this + 1); }
   iterator type_end()   const { return type_begin() + NumTys; }

   child_iterator child_begin() const { return type_begin(); }
   child_iterator child_end() const { return type_end(); }

   QualType getContainedType(size_t i) const
   {
      assert(i < NumTys && "type index out of bounds");
      return *(type_begin() + i);
   }

   llvm::ArrayRef<QualType> getContainedTypes() const
   {
      return { type_begin(), NumTys };
   }

   unsigned getArity() const { return NumTys; }

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
   explicit RecordType(ast::RecordDecl *record);
   RecordType(TypeID typeID,
              ast::RecordDecl *record,
              bool Dependent);

   ast::RecordDecl *Rec;

public:
   bool isRawEnum() const;

   sema::FinalTemplateArgumentList& getTemplateArgs() const;
   bool hasTemplateArgs() const;

   ast::RecordDecl *getRecord() const { return Rec; }

   void setDependent(bool dep);

   unsigned short getAlignment() const;
   size_t getSize() const;

   child_iterator child_begin() const { return child_iterator(); }
   child_iterator child_end() const { return child_iterator(); }

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
      case TypeID::RecordTypeID:
      case TypeID::DependentRecordTypeID:
         return true;
      default:
         return false;
      }
   }

   friend class ast::ASTContext;
};

class DependentRecordType: public RecordType {
protected:
   DependentRecordType(ast::RecordDecl *record,
                       sema::FinalTemplateArgumentList *templateArgs,
                       QualType Parent,
                       Type *CanonicalType);

   QualType Parent;
   mutable sema::FinalTemplateArgumentList *templateArgs;

public:
   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, getRecord(), templateArgs, Parent);
   }

   static void Profile(llvm::FoldingSetNodeID &ID,
                       ast::RecordDecl *R,
                       sema::FinalTemplateArgumentList* templateArgs,
                       QualType Parent);

   static bool classof (Type const* T)
   {
      return T->getTypeID() == TypeID::DependentRecordTypeID;
   }

   friend class ast::ASTContext;

   QualType getParent() const { return Parent; }
   sema::FinalTemplateArgumentList& getTemplateArgs() const { return *templateArgs; }
   bool hasTemplateArgs() const { return true; }
};

class TemplateParamType: public Type, public llvm::FoldingSetNode {
   explicit TemplateParamType(ast::TemplateParamDecl *Param);

   ast::TemplateParamDecl *P;

public:
   ast::TemplateParamDecl *getParam() const { return P; }
   llvm::StringRef getTemplateParamTypeName() const;
   QualType getCovariance() const;
   QualType getContravariance() const;
   unsigned getIndex() const;
   bool isVariadic() const;

   child_iterator child_begin() const { return child_iterator{}; }
   child_iterator child_end() const { return child_iterator{}; }

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, P);
   }

   static void Profile(llvm::FoldingSetNodeID &ID, ast::TemplateParamDecl *P)
   {
      ID.AddPointer(P);
   }

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::TemplateParamTypeID;
   }

   friend class ast::ASTContext;
};

class AssociatedType: public Type, public llvm::FoldingSetNode {
   AssociatedType(ast::AssociatedTypeDecl *AT, AssociatedType *OuterAT);

   ast::AssociatedTypeDecl *AT;
   AssociatedType *OuterAT;

public:
   ast::AssociatedTypeDecl *getDecl() const { return AT; }
   AssociatedType *getOuterAT() const { return OuterAT; }

   QualType getActualType() const;

   child_iterator child_begin() const { return child_iterator{}; }
   child_iterator child_end() const { return child_iterator{}; }

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, AT, OuterAT);
   }

   static void Profile(llvm::FoldingSetNodeID &ID, ast::AssociatedTypeDecl *P,
                       AssociatedType *OuterAT) {
      ID.AddPointer(P);
      ID.AddPointer(OuterAT);
   }

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::AssociatedTypeID;
   }

   friend class ast::ASTContext;
};

namespace ast {
   class AliasDecl;
   class NamespaceDecl;
} // namespace ast

class MetaType: public Type, public llvm::FoldingSetNode {
public:
   enum : unsigned { MemberCount = 6u };
   enum : unsigned {
      BaseClass = 0u,
      VTable,
      PTable,
      Name,
      Deinitializer,
      Conformances,
   };

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

   child_iterator child_begin() const { return &forType; }
   child_iterator child_end() const { return &forType + 1; }

   QualType getUnderlyingType() const { return forType; }

   size_t getSize() const { llvm_unreachable("should never be a member"); }

   unsigned short getAlignment() const
   { llvm_unreachable("should never be a member"); }

protected:
   explicit MetaType(QualType forType, Type *CanonicalType);

   QualType forType;
};

class TypedefType: public Type, public llvm::FoldingSetNode {
protected:
   TypedefType(TypeID typeID,
               ast::AliasDecl *td,
               bool Dependent);

public:
   // HACK - on creation, the actual underlying type of the
   // TypedefDecl might not be known yet. We have to violate type
   // immutability to update the canonical type before any actual type
   // checking is done
   void setCanonicalType(QualType CanonicalType);

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, getTypedef());
   }

   static void Profile(llvm::FoldingSetNodeID &ID, ast::AliasDecl *TD)
   {
      ID.AddPointer(TD);
   }

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::TypedefTypeID
         || T->getTypeID() == TypeID::DependentTypedefTypeID;
   }

   friend class ast::ASTContext;

   child_iterator child_begin() const { return child_iterator{}; }
   child_iterator child_end() const { return child_iterator{}; }

   ast::AliasDecl *getTypedef() const { return td; }

   QualType getAliasedType() const;
   llvm::StringRef getAliasName() const;

private:
   explicit TypedefType(ast::AliasDecl *td);

   ast::AliasDecl *td;
};

class DependentTypedefType: public TypedefType {
protected:
   DependentTypedefType(ast::AliasDecl *td,
                        sema::FinalTemplateArgumentList *templateArgs,
                        QualType Parent,
                        Type *CanonicalType);

   QualType Parent;
   mutable sema::FinalTemplateArgumentList *templateArgs;

public:
   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, getTypedef(), templateArgs, Parent);
   }

   static void Profile(llvm::FoldingSetNodeID &ID,
                       ast::AliasDecl *td,
                       sema::FinalTemplateArgumentList* templateArgs,
                       QualType Parent);

   static bool classof (Type const* T)
   {
      return T->getTypeID() == TypeID::DependentTypedefTypeID;
   }

   friend class ast::ASTContext;

   QualType getParent() const { return Parent; }
   sema::FinalTemplateArgumentList& getTemplateArgs() const { return *templateArgs; }
   bool hasTemplateArgs() const { return true; }
};

class DependentNameType: public Type, public llvm::FoldingSetNode {
   explicit DependentNameType(NestedNameSpecifierWithLoc *NameSpec);

   /// The dependent nested name specifier. Since the same name specifier can
   /// refer to different declarations in different places, these can't be
   /// reasonably uniqued without a source location.
   NestedNameSpecifierWithLoc *NameSpec;

public:
   friend class ast::ASTContext;

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::DependentNameTypeID;
   }

   void Profile(llvm::FoldingSetNodeID &ID);
   static void Profile(llvm::FoldingSetNodeID &ID,
                       NestedNameSpecifierWithLoc *Name);

   child_iterator child_begin() const { return child_iterator{}; }
   child_iterator child_end() const { return child_iterator{}; }

   NestedNameSpecifierWithLoc *getNameSpecWithLoc() const { return NameSpec; }
   NestedNameSpecifier *getNameSpec() const;
};

class TypeVariableType: public Type {
   TypeVariableType(unsigned ID);

   /// The unique ID of this type variable.
   unsigned ID;

public:
   friend class ast::ASTContext;

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::TypeVariableTypeID;
   }

   child_iterator child_begin() const { return child_iterator{}; }
   child_iterator child_end() const { return child_iterator{}; }

   unsigned getVariableID() const { return ID; }
};

template<class T> const T* Type::getAs() const
{
   if (auto Ty = support::dyn_cast<T>(this))
      return Ty;

   return support::dyn_cast<T>(CanonicalType);
}

template<class T> bool Type::isa() const
{
   if (support::isa<T>(this))
      return true;

   return support::isa<T>(CanonicalType);
}

} // namespace cdot

#endif //CDOT_BUILTIN_TYPE_H
