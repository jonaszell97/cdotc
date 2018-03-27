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
   ValueType(Context &Ctx, Type *ty)
      : Ty(ty), Ctx(Ctx)
   {

   }

   ValueType(Context &Ctx, QualType ty, bool isReference = false)
      : Ty(ty), Ctx(Ctx)
   {

   }

   ValueType(ValueType const& that)
      : Ty(that.Ty),
        Ctx(that.Ctx)
   {}

   ValueType(ValueType &&that) noexcept
      : Ty(that.Ty),
        Ctx(that.Ctx)
   {}

   ValueType &operator=(ValueType const& that)
   {
      assert(&Ctx == &that.Ctx);
      Ty = that.Ty;

      return *this;
   }

   ValueType &operator=(ValueType &&that) noexcept
   {
      assert(&Ctx == &that.Ctx);
      Ty = that.Ty;

      return *this;
   }

   ValueType &operator=(QualType const& that)
   {
      Ty = that;
      return *this;
   }

   Context &getCtx() const
   {
      return Ctx;
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
   Context &Ctx;
};

class Value {
public:
   enum TypeID : unsigned char {
#     define CDOT_ALL(name) name##ID,
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
   unsigned Flags : 24;
   unsigned SubclassData : 32;

   SourceLocation loc;

   ValueType type;
   std::string name;

   Use *uses;
   MDSet *metaData;

   enum Flag {

   };

private:
   void setNameNoCheck(llvm::StringRef name) { this->name = name.str(); }

public:
   static bool classof(Value const* T) { return true; }

   void deleteValue();
   void checkIfStillInUse();

   TypeID getTypeID() const { return id; }
   ValueType getType() const { return type; }

   void setFlag(Flag f, bool value);
   bool getFlag(Flag f) const;

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
