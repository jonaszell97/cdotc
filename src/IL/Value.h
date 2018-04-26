//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_VALUE_H
#define CDOT_VALUE_H

#include "AST/Type.h"
#include "Lex/SourceLocation.h"
#include "Support/Casting.h"
#include "Use.h"

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

class ValueType {
public:
   ValueType(Context &Ctx, Type *ty);
   ValueType(Context &Ctx, QualType ty);

   ValueType &operator=(QualType const& that)
   {
      Ty = that;
      return *this;
   }

   Context &getCtx() const
   {
      return *Ctx;
   }

   ValueType getPointerTo() const;

   ValueType getReferenceTo() const;
   void makeReference();

   /*implicit*/ operator QualType() const { return Ty; }
   /*implicit*/ operator bool()     const { return Ty; }

   Type *operator->()  const { return *Ty; }

   QualType get() const { return Ty; }

   bool operator==(const QualType &other) const { return Ty == other; }
   bool operator!=(const QualType &other) const { return !(*this == other); }

   bool operator==(const ValueType &other) const { return Ty == other.Ty; }
   bool operator!=(const ValueType &other) const { return !(*this == other); }

private:
   QualType Ty;
   Context *Ctx;
};

class Value {
public:
   enum TypeID : unsigned char {
#     define CDOT_ALL(Name) Name##ID,
#     define CDOT_ABSTRACT(Name) Name,
#     include "Instructions.def"
   };

   friend class ValueSymbolTable;
   friend class ILBuilder;

protected:
   Value(TypeID id, ValueType ty);

#  ifndef NDEBUG
   virtual
#  endif
   ~Value();

   TypeID id : 8;

   struct InstructionBits {
      bool IsSanctionedSelfUse : 1;
   };

   struct FunctionBits {
      InstructionBits InstBits;
      bool Throws   : 1;
      bool Declared : 1;
      bool Static   : 1;
      bool SRet     : 1;
      bool Virtual  : 1;
      bool Vararg   : 1;
      bool GlobalCtor   : 1;
      bool GlobalDtor   : 1;
      unsigned CtorKind : 2;
   };

   struct GlobalVariableBits {
      bool Const           : 1;
      bool LazilyInitialized : 1;
   };

   struct AllocaInstBits {
      InstructionBits InstBits;
      bool Heap            : 1;
      bool CanUseSRetValue : 1;
      bool CanElideCopy    : 1;
      bool IsLocalVarDecl  : 1;
      bool IsLet           : 1;
   };

   struct StoreInstBits {
      InstructionBits InstBits;
      bool IsInit : 1;
   };

   struct FieldRefInstBits {
      InstructionBits InstBits;
      bool IsLet : 1;
   };

   struct GEPInstBits {
      InstructionBits InstBits;
      bool IsLet : 1;
   };

   struct EnumInitInstBits {
      InstructionBits InstBits;
      bool CanUseSRetValue : 1;
      bool IsIndirect      : 1;
   };

   struct EnumExtractInstBits {
      InstructionBits InstBits;
      unsigned ArgNo  : 16;
      bool IsIndirect : 1;
      bool IsLet : 1;
   };

   struct RetInstBits {
      InstructionBits InstBits;
      bool CanUseSRetValue : 1;
   };

   struct ArgumentBits {
      bool IsSelf : 1;
      unsigned Convention : 4;
   };

   union {
      unsigned SubclassData = 0;
      FunctionBits FnBits;
      InstructionBits InstBits;
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

#  ifndef NDEBUG
   struct SizeTest {
      union {
         unsigned SubclassData = 0;
         FunctionBits FnBits;
         InstructionBits InstBits;
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

#  endif

   SourceLocation loc;

   ValueType type;
   std::string name;

   Use *uses;
   MDSet *metaData;

private:
   void setNameNoCheck(llvm::StringRef name) { this->name = name.str(); }

public:
   static bool classof(Value const* T) { return true; }

   void deleteValue();
   void checkIfStillInUse();

   TypeID getTypeID() const { return id; }
   ValueType getType() const { return type; }

   bool isSelf() const;

   using use_iterator = llvm::iterator_range<Use::iterator>;

   use_iterator getUses()
   {
      return use_iterator(use_begin(), use_end());
   }

   Use::iterator use_begin() { return uses ? uses->begin() : Use::iterator(); }
   Use::iterator use_end()   { return uses ? uses->end() : Use::iterator(); }

   Use::const_iterator use_begin() const
   {
      return uses ? uses->const_begin() : Use::const_iterator();
   }

   Use::const_iterator use_end() const
   {
      return uses ? uses->const_end() : Use::const_iterator();
   }

   Value *getSingleUser() const
   {
      if (getNumUses() != 1)
         return nullptr;

      return use_begin()->getUser();
   }

   bool isLvalue() const;
   void setIsLvalue(bool ref);

   il::Context &getCtx() const { return type.getCtx(); }
   ast::ASTContext &getASTCtx() const;

   void detachFromParent();
   void detachAndErase();

   void addUse(Value *User);
   void removeUser(Value *User);

   size_t getNumUses() const;
   bool isUnused() const;

   void replaceAllUsesWith(Value *V);

   llvm::StringRef getName() const;
   void setName(llvm::StringRef name);
   bool hasName() const;

   SourceLocation getSourceLoc() const;

   MDSet *getMetaData() const;
   bool hasMetaData(MDKind kind) const;

   void addMetaData(MetaData *MD);

   MetaData *getMetaData(MDKind kind) const;

   template<class T>
   T* getAs()
   {
      assert(T::classof(this) && "casting to incompatible type!");
      return static_cast<T*>(this);
   }

   template<class T>
   T* getAsOrNull()
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


#endif //CDOT_VALUE_H
