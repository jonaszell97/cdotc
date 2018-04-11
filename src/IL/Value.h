//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_VALUE_H
#define CDOT_VALUE_H

#include <cassert>
#include <string>
#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/SmallVector.h>

#include "AST/Type.h"
#include "Lex/SourceLocation.h"
#include "Support/Casting.h"

#include "Use.h"

namespace cdot {

namespace ast {
   class ASTContext;
}

class Type;

namespace il {

class MDSet;
class MetaData;
enum MDKind : unsigned;
class ValueSymbolTable;
class MDLocation;
class ILBuilder;

class CallSite;
class ImmutableCallSite;

class Context;

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

   struct FunctionBits {
      bool Throws   : 1;
      bool Declared : 1;
      bool Static   : 1;
      bool SRet     : 1;
      bool Virtual  : 1;
      bool Vararg   : 1;
      bool GlobalCtor : 1;
      bool GlobalDtor : 1;
   };

   struct GlobalVariableBits {
      bool Const           : 1;
      bool LateInitialized : 1;
   };

   struct InstructionBits {

   };

   struct AllocaInstBits {
      unsigned Alignment   : 28;
      bool Heap            : 1;
      bool CanUseSRetValue : 1;
      bool CanElideCopy    : 1;
      bool IsLocalVarDecl  : 1;
   };

   struct RetInstBits {
      bool CanUseSRetValue : 1;
   };

   union {
      unsigned SubclassData = 0;
      FunctionBits FnBits;
      GlobalVariableBits GVBits;
      InstructionBits InstBits;
      AllocaInstBits AllocaBits;
      RetInstBits RetBits;
   };

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

   void setLocation(SourceLocation location);

   SourceLocation getSourceLoc() const;

   MDSet *getMetaData() const;
   bool hasMetaData(MDKind kind) const;

   void addMetaData(MetaData *MD);

   MetaData *getMetaData(MDKind kind) const;

   template<class T>
   T* getAs()
   {
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
