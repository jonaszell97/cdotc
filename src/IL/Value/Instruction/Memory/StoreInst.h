//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_STOREINST_H
#define CDOT_STOREINST_H


#include "../Instruction.h"

namespace cdot {
namespace il {

class StoreInst: public Instruction {
public:
   StoreInst(Value *dst,
             Value *src,
             BasicBlock *parent,
             const std::string &name = "",
             const SourceLocation &loc = {});

   bool useMemCpy() const;

   Value *getDst() const;
   Value *getSrc() const;

protected:
   Value *dst;
   Value *src;

public:
   static bool classof(StoreInst const* T) { return true; }
   static inline bool classof(Value const* T) {
      switch(T->getTypeID()) {
         case StoreInstID:
            return true;
         default:
            return false;
      }
   }
};

class LoadInst: public Instruction {
public:
   explicit LoadInst(Value *target,
                     BasicBlock *parent,
                     const std::string &name = "",
                     const SourceLocation &loc = {});

   Value *getTarget() const;

protected:
   Value *target;

public:
   static bool classof(StoreInst const* T) { return true; }
   static inline bool classof(Value const* T) {
      switch(T->getTypeID()) {
         case LoadInstID:
            return true;
         default:
            return false;
      }
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_STOREINST_H
