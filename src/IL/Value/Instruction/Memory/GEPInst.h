//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_GEPINST_H
#define CDOT_GEPINST_H

#include "../Instruction.h"

namespace cdot {
namespace il {

class AggregateType;

class GEPInst: public Instruction {
public:
   GEPInst(Value *val,
           size_t idx,
           BasicBlock *parent,
           const std::string &name = "",
           const SourceLocation &loc = {});

   GEPInst(AggregateType *AggrTy,
           Value *val,
           size_t idx,
           BasicBlock *parent,
           const std::string &name = "",
           const SourceLocation &loc = {});

   bool isStructGEP() const;
   size_t getIndex() const;
   Value *getVal() const;

protected:
   AggregateType *AggrTy;
   Value *val;
   size_t index;

public:
   static bool classof(Value const* T)
   {
      switch(T->getTypeID()) {
         case GEPInstID:
         case TupleExtractInstID:
            return true;
         default:
            return false;
      }
   }
};

class TupleExtractInst: public GEPInst {
public:
   TupleExtractInst(Value *val,
                    size_t idx,
                    BasicBlock *parent,
                    const std::string &name = "",
                    const SourceLocation &loc = {});

   static bool classof(Value const* T)
   {
      return T->getTypeID() == TupleExtractInstID;
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_GEPINST_H
