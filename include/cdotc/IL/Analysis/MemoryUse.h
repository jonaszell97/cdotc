#ifndef CDOT_BORROW_H
#define CDOT_BORROW_H

#include "cdotc/IL/Analysis/MemoryLocation.h"
#include "cdotc/Lex/SourceLocation.h"

#include <llvm/ADT/ArrayRef.h>

namespace cdot {
namespace il {

struct MemoryRestriction {
   enum RestrictionKind : unsigned char {
      None = 0,
      MutableBorrow = 1,                            // prevent mutable borrows
      ImmutableBorrow = 2,                          // prevent immutable borrows
      Move = 4,                                     // prevent moves
      Use = MutableBorrow | ImmutableBorrow | Move, // prevent any sort of use
   };

private:
   RestrictionKind Kind;
   MemoryLocation RestrictedMem;

public:
   MemoryRestriction(RestrictionKind Kind, const MemoryLocation& RestrictedMem)
       : Kind(Kind), RestrictedMem(RestrictedMem)
   {
   }

   RestrictionKind getKind() const { return Kind; }
   MemoryLocation getRestrictedMem() const { return RestrictedMem; }
};

inline MemoryRestriction::RestrictionKind
operator|(MemoryRestriction::RestrictionKind LHS,
          MemoryRestriction::RestrictionKind RHS)
{
   return (MemoryRestriction::RestrictionKind)((unsigned)LHS | (unsigned)RHS);
}

inline MemoryRestriction::RestrictionKind
operator&(MemoryRestriction::RestrictionKind LHS,
          MemoryRestriction::RestrictionKind RHS)
{
   return (MemoryRestriction::RestrictionKind)((unsigned)LHS & (unsigned)RHS);
}

struct MemoryUse {
   enum Kind : unsigned char {
      ImmutableBorrow,
      MutableBorrow,
      Move,
   };

private:
   Kind UseKind;
   SourceRange Loc;
   MemoryLocation AccessedMem;
   llvm::ArrayRef<MemoryRestriction> Restrictions;

public:
   MemoryUse(Kind UseKind, SourceRange Loc, MemoryLocation AccessedMem,
             llvm::ArrayRef<MemoryRestriction> Restrictions = {})
       : UseKind(UseKind), Loc(Loc), AccessedMem(AccessedMem),
         Restrictions(Restrictions)
   {
   }

   bool isMutableBorrow() const { return UseKind == MutableBorrow; }
   bool isImmutableBorrow() const { return UseKind == ImmutableBorrow; }
   bool isMove() const { return UseKind == Move; }

   Kind getUseKind() const { return UseKind; }
   SourceRange getSourceLoc() const { return Loc; }
   MemoryLocation getAccessedMem() const { return AccessedMem; }
   llvm::ArrayRef<MemoryRestriction> getRestrictions() const
   {
      return Restrictions;
   }
};

} // namespace il
} // namespace cdot

#endif // CDOT_BORROW_H
