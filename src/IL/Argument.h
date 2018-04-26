//
// Created by Jonas Zell on 17.11.17.
//

#ifndef CDOT_ARGUMENT_H
#define CDOT_ARGUMENT_H

#include <llvm/ADT/ilist_node.h>
#include "Value.h"

namespace cdot {
namespace il {

class BasicBlock;

class Argument: public Value,
                public llvm::ilist_node_with_parent<Argument, BasicBlock> {
public:
   enum Convention {
      Copied, Owned, Borrowed,
   };

   Argument(ValueType type,
            Convention Conv,
            BasicBlock *parent,
            llvm::StringRef name = "");

   BasicBlock *getParent() const { return parent; }
   void setParent(BasicBlock *p) { parent = p; }

   Convention getConvention() const { return (Convention)ArgBits.Convention; }
   void setConvention(Convention C) { ArgBits.Convention = C; }

   bool isSelf() const { return ArgBits.IsSelf; }
   void setSelf(bool self) { ArgBits.IsSelf = self; }

   SourceLocation getSourceLoc() const { return SourceLoc; }
   void setSourceLoc(SourceLocation Loc) { SourceLoc = Loc; }

protected:
   SourceLocation SourceLoc;
   BasicBlock *parent;

public:
   static bool classof(Argument const* T) { return true; }
   static inline bool classof(Value const* T) {
     switch(T->getTypeID()) {
        case ArgumentID:
           return true;
        default:
           return false;
     }
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_ARGUMENT_H
