//
// Created by Jonas Zell on 13.08.17.
//

#ifndef CDOT_BUILTIN_TYPE_H
#define CDOT_BUILTIN_TYPE_H

#include <string>

#include <llvm/ADT/StringMap.h>
#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/FoldingSet.h>
#include <llvm/ADT/PointerIntPair.h>
#include <llvm/Support/Casting.h>

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
   Q_Const  = 0x1,
   Q_LValue = 0x2,
};

class Qualifiers {
public:
   explicit Qualifiers(unsigned Quals)
      :  Quals(Quals)
   {}

   Qualifiers(bool lvalue = false, bool Const = false)
      : Quals(0)
   {
      isLvalue(lvalue);
      isConst(Const);
   }

   bool isLvalue() const
   {
      return (Quals & Q_LValue) != 0;
   }

   bool isConst() const
   {
      return (Quals & Q_Const) != 0;
   }

   void isLvalue(bool lval)
   {
      if (lval)
         Quals |= Q_LValue;
      else
         Quals &= ~Q_LValue;
   }

   void isConst(bool C)
   {
      if (C)
         Quals |= Q_Const;
      else
         Quals &= ~Q_Const;
   }

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

   static bool classof(Type const* T) { return true; }

   bool needsLvalueToRvalueConv() const
   {
      return !isStruct() && !isProtocol();
   }

#  define CDOT_TYPE(Name)        \
   bool is##Name() const;        \
   Name *as##Name();             \
   Name const* as##Name() const;

#  define CDOT_BASE_TYPE(Name) CDOT_TYPE(Name)
#  include "Types.def"

   bool isFloatTy() const;
   bool isDoubleTy() const;

   bool isRawFunctionTy() const;
   bool isRawEnum() const;

   bool isRealTypedefType() const; // isTypedefType queries the aliased type
   TypedefType *asRealTypedefType();
   TypedefType const* asRealTypedefType() const;

   Type *getCanonicalType(ast::ASTContext &Ctx);
   Type *getCanonicalType(ast::ASTContext &Ctx) const;

   bool isSelfTy() const;

   BoxedPrimitive getPrimitiveKind() const;
   bool isBoxedPrimitive(BoxedPrimitive primitive) const;

   bool isBoxedInteger() const
   {
      switch (getPrimitiveKind()) {
#     define CDOT_INTEGER(Name, BW, Unsigned) \
         case Name: return true;
#     include "Primitive.def"

         default:
            return false;
      }
   }

   bool isIntOrBoxedInt() const
   {
      return isIntegerType() || isBoxedInteger();
   }

   bool isBoxedFloating() const
   {
      switch (getPrimitiveKind()) {
#     define CDOT_FP(Name, BW)                \
         case Name: return true;
#     include "Primitive.def"

         default:
            return false;
      }
   }

   bool isFPOrBoxedFP() const
   {
      return isFPType() || isBoxedFloating();
   }

   bool isBoxedChar() const
   {
      return getPrimitiveKind() == Char;
   }

   bool isInt8OrChar() const
   {
      return isInt8Ty() || isBoxedChar();
   }

   bool isBoxedBool() const
   {
      return getPrimitiveKind() == Bool;
   }

   bool isInt1OrBool() const
   {
      return isInt1Ty() || isBoxedBool();
   }

   bool isNumeric() const
   {
      return isIntegerType() || isFPType() || isRawEnum();
   }

   bool isDependentType() const;

   sema::TemplateArgList const& getTemplateArgs() const;
   bool hasTemplateArgs() const;

   llvm::StringRef getClassName() const;
   ast::RecordDecl *getRecord() const;
   bool isBoxedPrimitive() const;

   std::string toString() const;
   std::string toUniqueString() const;

   QualType getPointeeType() const;

   unsigned short getAlignment() const;
   size_t getSize() const;

   size_t getMemberSize() const;
   unsigned short getMemberAlignment() const;


   TypeID getTypeID() const
   {
      return Bits.id;
   }

   PointerType *getPointerTo(ast::ASTContext &Ctx) const;

   bool isStringRepresentable() const;
   bool isSelfComparable() const;
   bool isHashable() const;

   bool isBoxedEquivOf(Type const* ty) const;
   bool isUnboxedEquivOf(Type const* ty) const;

   bool isUnsigned() const;
   unsigned int getBitwidth() const;

   bool isPtrSizedInt() const
   {
      return isIntegerType() && getBitwidth() == sizeof(void*) * 8;
   }

   bool isInt64Ty(bool Unsigned = false) const
   {
      return isIntegerType() && getBitwidth() == 64 && isUnsigned() == Unsigned;
   }

   bool isInt8Ty(bool Unsigned = false) const
   {
      return isIntegerType() && getBitwidth() == 8 && isUnsigned() == Unsigned;
   }

   bool isInt1Ty(bool Unsigned = false) const
   {
      return isIntegerType() && getBitwidth() == 1 && isUnsigned() == Unsigned;
   }

   bool isIntNTy(unsigned n, bool Unsigned = false) const
   {
      return isIntegerType() && getBitwidth() == n && isUnsigned() == Unsigned;
   }

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
   Type(TypeID id)
   {
      Bits.id = id;
   }

   struct TypeBits {
      TypeID id : 8;
   };

   enum { TypeUsedBits = 8 };

   struct IntegerTypeBits {
      unsigned : TypeUsedBits;

      unsigned bitwidth : 8;
      bool isUnsigned   : 1;
   };

   struct FPTypeBits {
      unsigned : TypeUsedBits;

      unsigned short precision : 8;
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

      BoxedPrimitive primitiveType : 16;
   };

   union {
      TypeBits Bits;
      IntegerTypeBits IntBits;
      FPTypeBits FPBits;
      ArrayTypeBits ArrayBits;
      FunctionTypeBits FuncBits;
      RecordTypeBits RecordBits;
   };
};

class QualType {
private:
#ifndef NDEBUG
   class QualTypeValue {
   public:
      QualTypeValue(Type *Ptr, unsigned Int = 0)
         : Pointer(Ptr), Int(Int)
      {}

      Type *getPointer() const
      {
         return Pointer;
      }

      void setPointer(Type *Pointer)
      {
         QualTypeValue::Pointer = Pointer;
      }

      unsigned int getInt() const
      {
         return Int;
      }

      void setInt(unsigned int Int)
      {
         QualTypeValue::Int = Int;
      }

      void *getOpaqueValue() const
      {
         uintptr_t opaqueVal = uintptr_t(Pointer);
         opaqueVal |= Int;

         return (void*)opaqueVal;
      }

      friend bool operator==(QualTypeValue const& lhs, QualTypeValue const &rhs)
      {
         return lhs.Pointer == rhs.Pointer && lhs.Int == rhs.Int;
      }

      friend bool operator!=(QualTypeValue const& lhs, QualTypeValue const &rhs)
      {
         return !(lhs == rhs);
      }

      static QualTypeValue getFromOpaqueValue(void *val)
      {
         auto ptr = (uintptr_t)val & (~0llu << TypeAlignmentInBits);
         auto quals = (uintptr_t)val & ~(~0llu << TypeAlignmentInBits);

         return QualTypeValue((Type*)ptr, unsigned(quals));
      }

   private:
      Type *Pointer;
      unsigned Int;
   };

   using ValueTy = QualTypeValue;

#else
   using ValueTy = llvm::PointerIntPair<Type*, TypeAlignmentInBits>;
#endif

   ValueTy Value;

   enum class FromOpaque {
      Placeholder
   };

   QualType(void *Ptr, FromOpaque)
      : Value(ValueTy::getFromOpaqueValue(Ptr))
   {}

public:
   QualType()
      : Value(nullptr)
   {}

   QualType(Type *ty,
            bool isLvalue = false,
            bool isConst = false)
      : Value(ty)
   {
      this->isLvalue(isLvalue);
      this->isConst(isConst);
   }

   QualType(Type *ty,
            Qualifiers quals)
      : Value(ty, quals.getRaw())
   {

   }

   QualType(Type *ty,
            unsigned quals)
      : Value(ty, quals)
   {

   }

   bool operator==(const QualType& rhs) const
   {
      return Value == rhs.Value;
   }

   bool operator!=(const QualType& rhs) const
   {
      return !operator==(rhs);
   }

   bool isLvalue() const
   {
      return (Value.getInt() & Q_LValue) != 0;
   }

   bool isConst() const
   {
      return (Value.getInt() & Q_Const) != 0;
   }

   void isLvalue(bool lval)
   {
      if (lval)
         Value.setInt(Value.getInt() | Q_LValue);
      else
         Value.setInt(Value.getInt() & ~Q_LValue);
   }

   void isConst(bool C)
   {
      if (C)
         Value.setInt(Value.getInt() | Q_Const);
      else
         Value.setInt(Value.getInt() & ~Q_Const);
   }

   bool needsLvalueToRvalueConv() const
   {
      return isLvalue() && (**this)->needsLvalueToRvalueConv();
   }

   std::string toString() const
   {
      return (**this)->toString();
   }

   QualType getCanonicalType(ast::ASTContext &Ctx) const
   {
      return (**this)->getCanonicalType(Ctx);
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

} // namespace cdot

namespace llvm {

template<class T> struct simplify_type;

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

} // namespace llvm

namespace cdot {

class VoidType : public Type {
protected:
   VoidType()
      : Type(TypeID::VoidTypeID)
   {}

public:
   std::string toString() const
   {
      return "Void";
   }

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, 0);
   }

   static void Profile(llvm::FoldingSetNodeID &ID, int)
   {

   }

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::VoidTypeID;
   }

   friend class ast::ASTContext;
};

class AutoType : public Type {
protected:
   AutoType()
      : Type(TypeID::AutoTypeID)
   {

   }

public:
   std::string toString() const
   {
      return "<inferred>";
   }

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, 0);
   }

   static void Profile(llvm::FoldingSetNodeID &ID, int)
   {

   }

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::AutoTypeID;
   }

   friend class ast::ASTContext;
};

class PrimitiveType : public Type {
public:
   static inline bool classof(Type const* T) {
      switch(T->getTypeID()) {
         case TypeID::IntegerTypeID:
         case TypeID::FPTypeID:
            return true;
         default:
            return false;
      }
   }

protected:
   PrimitiveType(TypeID id)
      : Type(id)
   {}
};

class IntegerType : public PrimitiveType {
protected:
   explicit IntegerType(unsigned int bw = sizeof(int*) * 8,
                        bool isUnsigned = false)
      : PrimitiveType(TypeID::IntegerTypeID)
   {
      IntBits.bitwidth = bw;
      IntBits.isUnsigned = isUnsigned;
   }

public:
   unsigned int getBitwidth() const { return IntBits.bitwidth; }
   bool isUnsigned() const { return IntBits.isUnsigned; }

   std::string toString() const
   {
      std::string s;
      if (isUnsigned())
         s += 'u';
      else
         s += 'i';

      s += std::to_string(getBitwidth());

      return s;
   }

   size_t getSize() const
   {
      return getBitwidth() == 1 ? 1 : size_t(getBitwidth()) / 8;
   }

   unsigned short getAlignment() const
   {
      return getBitwidth() == 1 ? (unsigned short)1
                                : (unsigned short)(getBitwidth() / 8);
   }

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, getBitwidth(), isUnsigned());
   }

   static void Profile(llvm::FoldingSetNodeID &ID, unsigned bits,
                       bool isUnsigned) {
      ID.AddInteger(bits);
      ID.AddBoolean(isUnsigned);
   }

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::IntegerTypeID;
   }

   friend class ast::ASTContext;
};

class FPType : public PrimitiveType {
protected:
   explicit FPType(unsigned short precision = 64)
      : PrimitiveType(TypeID::FPTypeID)
   {
      FPBits.precision = precision;
   }

public:
   unsigned short getPrecision() const
   {
      return FPBits.precision;
   }

   size_t getSize() const
   {
      return size_t(getPrecision()) / 8;
   }

   unsigned short getAlignment() const
   {
      return (unsigned short)(getPrecision() / 8);
   }

   std::string toString() const
   {
      return getPrecision() == 64 ? "f64" : "f32";
   }

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, getPrecision());
   }

   static void Profile(llvm::FoldingSetNodeID &ID, unsigned short precision)
   {
      ID.AddInteger(precision);
   }

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::FPTypeID;
   }

   friend class ast::ASTContext;
};

class PointerType: public Type, public llvm::FoldingSetNode {
protected:
   explicit PointerType(QualType pointee)
      : Type(TypeID::PointerTypeID), pointeeType(pointee)
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

   std::string toString() const
   {
      std::string s = pointeeType.toString();
      s += "*";

      return s;
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
   const QualType pointeeType;
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
         case TypeID::InferredArrayTypeID:
            return true;
         default:
            return false;
      }
   }

   friend class ast::ASTContext;

   std::string toString() const;

   size_t getSize() const
   {
      return elementType->getSize() * numElements;
   }

   unsigned short getAlignment() const
   {
      return elementType->getAlignment();
   }

protected:
   ArrayType(QualType elementType, size_t numElements)
      : Type(TypeID::ArrayTypeID), elementType(elementType),
        numElements(numElements)
   {}

   ArrayType(TypeID typeID, QualType elementType)
      : Type(typeID), elementType(elementType),
        numElements(0)
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

class InferredArrayType: public ArrayType {
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
      return T->getTypeID() == TypeID::InferredArrayTypeID;
   }

   friend class ast::ASTContext;

private:
   InferredArrayType(QualType elementType, ast::Expression *DependentExpr)
      : ArrayType(TypeID::InferredArrayTypeID, elementType),
        DependentExpr(DependentExpr)
   {}

   ast::Expression *DependentExpr;
};

class FunctionType : public Type, public llvm::FoldingSetNode {
public:
   enum ExtFlags : unsigned {
      None         = 0,
      FunctionPtr  = 0x1,
      Vararg       = FunctionPtr << 1,
      CStyleVararg = Vararg << 1,
      NoThrow      = CStyleVararg << 1,
   };

private:
   FunctionType(QualType returnType,
                llvm::ArrayRef<QualType> argTypes,
                ExtFlags flags)
      : Type(TypeID::FunctionTypeID),
        returnType(returnType), argTypes(argTypes.begin(), argTypes.end())
   {
      FuncBits.flags = flags;
   }

public:
   QualType getReturnType() const
   {
      return returnType;
   }

   llvm::ArrayRef<QualType> getArgTypes() const
   {
      return argTypes;
   }

   bool isRawFunctionTy() const { return (FuncBits.flags & FunctionPtr) != 0; }
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

   std::string toString() const;

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, getReturnType(), getArgTypes(), FuncBits.flags);
   }

   static void Profile(llvm::FoldingSetNodeID &ID, QualType returnType,
                       llvm::ArrayRef<QualType> argTypes,
                       unsigned flags) {
      ID.AddPointer(returnType.getAsOpaquePtr());
      for (auto ty : argTypes)
         ID.AddPointer(ty.getAsOpaquePtr());

      ID.AddInteger(flags);
   }

   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::FunctionTypeID;
   }

   friend class ast::ASTContext;

protected:
   QualType returnType;
   std::vector<QualType> argTypes;
};

class TupleType: public Type, public llvm::FoldingSetNode {
private:
   explicit TupleType(llvm::ArrayRef<QualType> containedTypes)
      : Type(TypeID::TupleTypeID), containedTypes(containedTypes.begin(),
                                                  containedTypes.end())
   {}

public:
   QualType getContainedType(size_t i) const
   {
      return containedTypes[i];
   }

   llvm::ArrayRef<QualType> getContainedTypes() const
   {
      return containedTypes;
   }

   size_t getArity() const { return containedTypes.size(); }

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

   std::string toString() const;

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

protected:
   std::vector<QualType> containedTypes;
};

namespace ast {
class RecordDecl;
} // namespace ast

class ObjectType : public Type, public llvm::FoldingSetNode {
protected:
   explicit ObjectType(ast::RecordDecl *record,
                       BoxedPrimitive primitive = BoxedPrimitive::BP_None)
      : Type(TypeID::ObjectTypeID), Rec(record)
   {
      RecordBits.primitiveType = primitive;
   }

   explicit ObjectType(TypeID typeID, ast::RecordDecl *record)
      : Type(typeID), Rec(record)
   {}

   ast::RecordDecl *Rec;

public:
   bool isRawEnum() const;

   sema::TemplateArgList const& getTemplateArgs() const;
   bool hasTemplateArgs() const;

   bool isBoxedEquivOf(Type const* other) const;

   std::string toString() const;

   ast::RecordDecl *getRecord() const
   {
      return Rec;
   }

   unsigned short getAlignment() const;
   size_t getSize() const;

   BoxedPrimitive getPrimitiveKind() const
   {
      return RecordBits.primitiveType;
   }

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      Profile(ID, getRecord());
   }

   static void Profile(llvm::FoldingSetNodeID &ID, ast::RecordDecl *R)
   {
      ID.AddPointer(R);
   }

   static bool classof(ObjectType const*) { return true; }
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
class InconcreteObjectType: public ObjectType {
protected:
   InconcreteObjectType(ast::RecordDecl *record,
                        sema::TemplateArgList *templateArgs)
      : ObjectType(TypeID::InconcreteObjectTypeID, record),
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
      return T->getTypeID() == TypeID::InconcreteObjectTypeID;
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

class GenericType : public Type, public llvm::FoldingSetNode {
protected:
   GenericType(QualType actualType, llvm::StringRef genericTypeName)
      : Type(TypeID::GenericTypeID),
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

   std::string toString() const
   {
      return genericTypeName;
   }

   std::string toUniqueString() const
   {
      std::string s;
      s += genericTypeName;
      s += ": ";
      s += actualType->toUniqueString();

      return s;
   }

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
   std::string toString() const { return forType->toString(); }

   size_t getSize() const { llvm_unreachable("should never be a member"); }

   unsigned short getAlignment() const
   { llvm_unreachable("should never be a member"); }

protected:
   explicit MetaType(QualType forType)
      : Type(TypeID::MetaTypeID), forType(forType)
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
      : Type(TypeID::NamespaceTypeID), NS(NS)
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
      : Type(TypeID::TypedefTypeID), td(td)
   {

   }

   ast::TypedefDecl *td;
};

class UnknownAnyType: public Type {
public:
   static bool classof(Type const* T)
   {
      return T->getTypeID() == TypeID::UnknownAnyTypeID;
   }

   friend class ast::ASTContext;

private:
   UnknownAnyType()
      : Type(TypeID::UnknownAnyTypeID)
   {}
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
