#ifndef CDOT_VALUE_H
#define CDOT_VALUE_H

#include "cdotc/AST/Type.h"
#include "cdotc/IL/Use.h"
#include "cdotc/Lex/SourceLocation.h"
#include "cdotc/Support/Casting.h"

#include <llvm/ADT/SmallVector.h>
#include <llvm/ADT/StringRef.h>

#include <cassert>
#include <string>

namespace cdot {
namespace ast {
class ASTContext;
} // namespace ast

class Type;

namespace il {

class MDSet;
class MetaData;
class ValueSymbolTable;
class ILBuilder;
class CallSite;
class ImmutableCallSite;
class Context;

enum MDKind : unsigned;

// Keep in sync with llvm::AtomicOrdering!
enum class MemoryOrder : unsigned {
   NotAtomic = 0,
   Relaxed = 2,
   Consume = 3,
   Acquire = 4,
   Release = 5,
   AcquireRelease = 6,
   SequentiallyConsistent = 7,
};

llvm::raw_ostream& operator<<(llvm::raw_ostream& OS, MemoryOrder MO);

class ValueType {
public:
   ValueType(Context& Ctx, QualType ty);

   ValueType& operator=(QualType T);

   Context& getCtx() const { return *Ctx; }

   ValueType getPointerTo() const;
   ValueType getReferenceTo() const;
   void makeReference();

   /*implicit*/ operator QualType() const { return Ty; }
   /*implicit*/ operator CanType() const { return Ty; }
   /*implicit*/ operator bool() const { return Ty; }

   Type* operator->() const { return *Ty; }
   QualType get() const { return Ty; }

   bool operator==(const QualType& other) const { return Ty == other; }
   bool operator!=(const QualType& other) const { return !(*this == other); }

   bool operator==(const ValueType& other) const { return Ty == other.Ty; }
   bool operator!=(const ValueType& other) const { return !(*this == other); }

private:
   QualType Ty;
   Context* Ctx;
};

class Value {
public:
   enum TypeID : unsigned char {
#define CDOT_ALL(Name) Name##ID,
#define CDOT_ABSTRACT(Name) Name,
#include "cdotc/IL/Instructions.def"
   };

   StringRef getValueKindDescription() const
   {
      return getValueKindDescription(getTypeID());
   }

   static StringRef getValueKindDescription(TypeID ID);

   friend class ValueSymbolTable;
   friend class ILBuilder;

protected:
   Value(TypeID id, ValueType ty);

#ifndef NDEBUG
   virtual
#endif
       ~Value();

   TypeID id : 8;

   struct ValueBits {
      bool Synthesized : 1;
      bool MayBeMoved : 1;
      bool IsTagged : 1;
      bool Deserialized : 1;
      bool Verified : 1;
      bool Canonicalized : 1;
      bool Optimized : 1;
   };

   struct ConstantBits {
      ValueBits InstBits;
      bool AllZeros : 1;
      bool ContainsConstantEnum : 1;
   };

   struct FunctionBits {
      ValueBits InstBits;
      bool Throws : 1;
      bool Async : 1;
      bool Declared : 1;
      bool Static : 1;
      bool SRet : 1;
      bool Virtual : 1;
      bool Deinit : 1;
      bool Vararg : 1;
      bool GlobalCtor : 1;
      bool GlobalDtor : 1;
      bool OverridePrevious : 1;
      bool Invalid : 1;
      bool ReadyForCtfe : 1;
      unsigned CtorKind : 2;
   };

   struct GlobalVariableBits {
      ValueBits InstBits;
      bool Const : 1;
      bool LazilyInitialized : 1;
      bool NeedsDeinit : 1;
      bool OverridePrevious : 1;
      bool Declared : 1;
   };

   struct AllocaInstBits {
      ValueBits InstBits;
      bool Heap : 1;
      bool CanUseSRetValue : 1;
      bool CanElideCopy : 1;
      bool IsLocalVarDecl : 1;
      bool IsLet : 1;
      bool FallibleInit : 1;
   };

   struct StoreInstBits {
      ValueBits InstBits;
      MemoryOrder memoryOrder : 3;
   };

   struct LoadInstBits {
      ValueBits InstBits;
      MemoryOrder memoryOrder : 3;
      bool IsFieldAccessLoad : 1;
   };

   struct MoveInstBits {
      ValueBits InstBits;
   };

   struct FieldRefInstBits {
      ValueBits InstBits;
      bool IsLet : 1;
   };

   struct GEPInstBits {
      ValueBits InstBits;
      bool IsLet : 1;
   };

   struct EnumInitInstBits {
      ValueBits InstBits;
      bool CanUseSRetValue : 1;
      bool IsIndirect : 1;
   };

   struct EnumExtractInstBits {
      ValueBits InstBits;
      unsigned ArgNo : 16;
      bool IsIndirect : 1;
      bool IsLet : 1;
   };

   struct RetInstBits {
      ValueBits InstBits;
      bool CanUseSRetValue : 1;
      bool IsFallibleInitNoneRet : 1;
   };

   struct YieldInstBits {
      ValueBits InstBits;
      bool IsFinalYield : 1;
   };

   struct BorrowInstBits {
      ValueBits InstBits;
      bool IsMutableBorrow : 1;
   };

   struct CallInstBits {
      ValueBits InstBits;
   };

   struct InvokeInstBits {
      ValueBits InstBits;
   };

   struct ExistentialInitInstBits {
      ValueBits InstBits;
      bool Preallocated : 1;
   };

   struct ArgumentBits {
      ValueBits InstBits;
      bool IsSelf : 1;
      bool GenericEnvironment : 1;
      ArgumentConvention Convention : 2;
   };

   union {
      unsigned SubclassData = 0;
      FunctionBits FnBits;
      ValueBits InstBits;
      ConstantBits ConstBits;
      GlobalVariableBits GVBits;
      AllocaInstBits AllocaBits;
      EnumInitInstBits EnumInitBits;
      EnumExtractInstBits EnumExtractBits;
      StoreInstBits StoreBits;
      LoadInstBits LoadBits;
      MoveInstBits MoveBits;
      FieldRefInstBits FieldRefBits;
      GEPInstBits GEPBits;
      RetInstBits RetBits;
      YieldInstBits YieldBits;
      BorrowInstBits BorrowBits;
      CallInstBits CallBits;
      InvokeInstBits InvokeBits;
      ExistentialInitInstBits ExistentialBits;
      ArgumentBits ArgBits;
   };

#ifndef NDEBUG
   struct SizeTest {
      union {
         unsigned SubclassData = 0;
         FunctionBits FnBits;
         ValueBits InstBits;
         GlobalVariableBits GVBits;
         AllocaInstBits AllocaBits;
         EnumInitInstBits EnumInitBits;
         EnumExtractInstBits EnumExtractBits;
         StoreInstBits StoreBits;
         FieldRefInstBits FieldRefBits;
         GEPInstBits GEPBits;
         RetInstBits RetBits;
         ArgumentBits ArgBits;
      };
   };

   static_assert(sizeof(SizeTest) <= sizeof(unsigned),
                 "bits don't fit into an unsigned!");

#endif

   SourceLocation loc;

   ValueType type;
   std::string name;

   Use* uses;
   MDSet* metaData;

private:
   void setNameNoCheck(llvm::StringRef name) { this->name = name.str(); }

public:
   static bool classof(Value const* T) { return true; }

   void deleteValue();
   void checkIfStillInUse();

   TypeID getTypeID() const { return id; }
   ValueType getType() const { return type; }

   unsigned getRawBits() const { return SubclassData; }
   void setRawBits(unsigned Bits) { SubclassData = Bits; }

   bool isIndexingInstruction() const;
   bool isSelf() const;

   using use_iterator = llvm::iterator_range<Use::iterator>;
   using const_use_iterator = llvm::iterator_range<Use::const_iterator>;

   use_iterator getUses() { return use_iterator(use_begin(), use_end()); }

   const_use_iterator getUses() const
   {
      return const_use_iterator(use_begin(), use_end());
   }

   Use::iterator use_begin() { return uses ? uses->begin() : Use::iterator(); }
   Use::iterator use_end() { return uses ? uses->end() : Use::iterator(); }

   Use::const_iterator use_begin() const
   {
      return uses ? uses->const_begin() : Use::const_iterator();
   }

   Use::const_iterator use_end() const
   {
      return uses ? uses->const_end() : Use::const_iterator();
   }

   Value* getSingleUser() const
   {
      if (getNumUses() != 1)
         return nullptr;

      return use_begin()->getUser();
   }

   bool isLvalue() const;
   void setIsLvalue(bool ref);

   bool isTagged() const { return InstBits.IsTagged; }
   void setTagged(bool B) { InstBits.IsTagged = B; }

   bool isAllZerosValue() const;

   il::Context& getCtx() const { return type.getCtx(); }
   ast::ASTContext& getASTCtx() const;

   void detachFromParent();
   void eraseValue();
   void detachAndErase();

   void addUse(Value* User);
   void removeUser(Value* User);
   void replaceUser(Value* User, Value* ReplaceWith);

   il::Value* ignoreBitCast();

   size_t getNumUses() const;
   bool isUnused() const;

   void replaceAllUsesWith(Value* V);

   llvm::StringRef getName() const;
   void setName(llvm::StringRef name);
   bool hasName() const;

   SourceLocation getSourceLoc() const { return loc; }
   void setSourceLoc(SourceLocation L) { loc = L; }

   MDSet* getMetaData() const;
   bool hasMetaData(MDKind kind) const;

   void addMetaData(MetaData* MD);

   MetaData* getMetaData(MDKind kind) const;

   template<class T> T* getAs()
   {
      assert(T::classof(this) && "casting to incompatible type!");
      return static_cast<T*>(this);
   }

   template<class T> T* getAsOrNull()
   {
      if (!T::classof(this)) {
         return nullptr;
      }

      return static_cast<T*>(this);
   }

   CallSite getAsCallSite();
   ImmutableCallSite getAsImmutableCallSite() const;
};

} // namespace il
} // namespace cdot

#endif // CDOT_VALUE_H
