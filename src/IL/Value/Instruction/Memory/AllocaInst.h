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
              bool heap = false,
              const std::string &name = "",
              const SourceLocation &loc = {});

   unsigned int getAlignment() const;
   bool isHeapAlloca() const;

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
