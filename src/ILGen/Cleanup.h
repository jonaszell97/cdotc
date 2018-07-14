//
// Created by Jonas Zell on 27.04.18.
//

#ifndef CDOT_CLEANUPACTION_H
#define CDOT_CLEANUPACTION_H

#include <IL/Instructions.h>
#include "Basic/DiverseStack.h"

namespace cdot {
namespace ast {
   class ILGenPass;
} // namespace ast

namespace il {
   class Value;
} // namespace il

class CleanupStack;

class Cleanup {
   virtual void anchor(); // anchor this classes vtable
   unsigned AllocatedSize : 16;

protected:
   unsigned Flags : 16;

public:
   Cleanup() : AllocatedSize(0), Flags(0)
   {}

   virtual void Emit(ast::ILGenPass &ILGen) = 0;
   size_t allocated_size() const { return AllocatedSize; }

   virtual bool ignoreValue(il::Value*) { return false; }

   friend class CleanupStack;
};

class DefaultCleanup: public Cleanup {
   il::Value *Val;
   void deinitializeValue(ast::ILGenPass &ILGen, il::Value *Val);

   enum Flag {
      Ignored = 0x1,
   };

public:
   explicit DefaultCleanup(il::Value *Val)
      : Val(Val)
   {}

   bool ignoreValue(il::Value *V) override
   {
      if (Val == V) {
         Flags |= Ignored;
         return true;
      }

      return false;
   }

   void Emit(ast::ILGenPass &ILGen) override;
};

class BorrowCleanup: public Cleanup {
   il::BeginBorrowInst *BeginBorrow;
   SourceLocation EndLoc;

public:
   explicit BorrowCleanup(il::BeginBorrowInst *BeginBorrow, SourceLocation Loc)
      : BeginBorrow(BeginBorrow), EndLoc(Loc)
   {}

   void Emit(ast::ILGenPass &ILGen) override;
};

struct CleanupScope;

typedef DiverseStackImpl<Cleanup>::stable_iterator CleanupsDepth;
typedef DiverseStackImpl<Cleanup>::stable_iterator CleanupHandle;

class CleanupStack {
   ast::ILGenPass &ILGen;
   DiverseStack<Cleanup, 128> stack;
   CleanupsDepth innermostScope;

   void emitCleanups(CleanupsDepth depth, bool popCleanups = true,
                     bool emitCleanups = true);
   void endScope(CleanupsDepth depth, bool emitCleanups = true);

public:
   explicit CleanupStack(ast::ILGenPass &ILGen)
      : ILGen(ILGen), stack{}, innermostScope(stack.stable_end())
   {}

   CleanupStack(CleanupStack &&other) noexcept
      : ILGen(other.ILGen), stack(std::move(other.stack)),
        innermostScope(other.innermostScope)
   {}

   CleanupStack &operator=(CleanupStack &&other) noexcept;

   CleanupsDepth getCleanupsDepth() const { return stack.stable_begin(); }
   CleanupHandle getTopCleanup() const
   {
      assert(!stack.empty());
      return stack.stable_begin();
   }

   void emitScopeCleanups(bool popStack = true)
   {
      emitCleanups(stack.stable_end(), popStack);
   }

   template<class T, class ...Args>
   T &pushCleanup(Args&&... args)
   {
      auto &NewCleanup = stack.push<T, Args...>(std::forward<Args&&>(args)...);
      NewCleanup.AllocatedSize = sizeof(T);

      return NewCleanup;
   }

   bool ignoreValue(CleanupsDepth depth, il::Value *Val);

   void emitAllWithoutPopping()
   {
      for (auto &C : stack)
         C.Emit(ILGen);
   }

   void emitUntilWithoutPopping(CleanupsDepth depth);

   void clear()
   {
      while (!stack.empty())
         stack.pop();

      innermostScope = stack.stable_end();
   }

   friend struct CleanupScope;
};

struct CleanupScope {
protected:
   CleanupStack &Cleanups;
   CleanupsDepth depth;
   CleanupsDepth savedInnermostScope;
   bool Active = true;

   void popImpl(bool emitCleanups);

public:
   explicit CleanupScope(CleanupStack &cleanups)
      : Cleanups(cleanups), depth(cleanups.getCleanupsDepth()),
        savedInnermostScope(cleanups.innermostScope)
   {
      assert(depth.isValid());
      cleanups.stack.checkIterator(cleanups.innermostScope);
      cleanups.innermostScope = depth;
   }

   void pop()
   {
      assert(depth.isValid() && "popping a scope twice!");
      popImpl(true);
      depth = CleanupsDepth::invalid();
   }

   void popWithoutEmittingCleanups()
   {
      assert(depth.isValid() && "popping a scope twice!");
      popImpl(false);
      depth = CleanupsDepth::invalid();
   }

   void deactivate() { Active = false; }

   ~CleanupScope()
   {
      if (depth.isValid())
         popImpl(Active);
   }

   bool ignoreValue(il::Value *Val) { return Cleanups.ignoreValue(depth, Val); }
   bool isValid() const { return depth.isValid(); }
};

struct ExprCleanupScope: public CleanupScope {
   explicit ExprCleanupScope(CleanupStack &Cleanups) : CleanupScope(Cleanups)
   {}

   using CleanupScope::pop;
};

} // namespace cdot

#endif //CDOT_CLEANUPACTION_H
