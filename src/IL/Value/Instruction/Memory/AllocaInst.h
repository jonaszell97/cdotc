//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_ALLOCAINST_H
#define CDOT_ALLOCAINST_H


#include "../Instruction.h"

namespace cdot {
namespace il {

class AggregateType;

class AllocaInst: public Instruction {
public:
   AllocaInst(Type *ty,
              BasicBlock *parent,
              unsigned alignment = 0,
              bool heap = false);

   AllocaInst(Type *ty,
              BasicBlock *parent,
              size_t allocSize,
              unsigned alignment = 0,
              bool heap = false);

   unsigned int getAlignment() const;
   bool isHeapAlloca() const;

   size_t getAllocSize() const { return allocSize; }

private:
   size_t allocSize = 1;

   enum Flags : unsigned short {
      Heap = Instruction::Flags::Initializer << 1,
   };

public:
   static bool classof(AllocaInst const* T) { return true; }
   static inline bool classof(Value const* T) {
     switch(T->getTypeID()) {
        case AllocaInstID:
           return true;
        default:
           return false;
     }
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_ALLOCAINST_H
