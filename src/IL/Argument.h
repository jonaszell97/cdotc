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
            bool vararg,
            BasicBlock *parent,
            llvm::StringRef name = "");

   BasicBlock *getParent() const { return parent; }
   void setParent(BasicBlock *p);
   bool isVararg() const { return vararg; }

   bool isSelf() const { return self; }
   void setSelf(bool self) { Argument::self = self; }

protected:
   BasicBlock *parent;
   bool vararg = false;
   bool self = false;

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
