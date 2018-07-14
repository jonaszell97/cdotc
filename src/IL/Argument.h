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
   Argument(ValueType type,
            ArgumentConvention Conv,
            BasicBlock *parent,
            llvm::StringRef name = "");

   Argument(const Argument &Arg, BasicBlock &Parent);

   BasicBlock *getParent() const { return parent; }
   void setParent(BasicBlock *p) { parent = p; }

   void setConvention(ArgumentConvention C) { ArgBits.Convention = C; }
   ArgumentConvention getConvention() const { return ArgBits.Convention; }

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
