//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_CONSTANTEXPR_H
#define CDOT_CONSTANTEXPR_H


#include "../Instruction/Instruction.h"

namespace cdot {
namespace il {

class Constant;

class ConstantExpr: public Instruction {
protected:
   ConstantExpr(TypeID id,
                Type *ty,
                BasicBlock *parent,
                const std::string &name = "",
                const SourceLocation &loc = {});

   static inline bool classof(Value const* T) {
      switch (T->getTypeID()) {
#     define CDOT_INSTRUCTION(Name) \
         case Name##ID:
#     define CDOT_INCLUDE_CONSTEXPRS
#     include "../Instructions.def"
            return true;
         default:
            return false;
      }
   }
};

class ConstantBitCastInst: public ConstantExpr {
public:
   ConstantBitCastInst(Constant *Val,
                       Type *toType,
                       BasicBlock *parent,
                       const std::string &name = "",
                       const SourceLocation &loc = {});

protected:
   Constant *target;

public:
   static bool classof(Value const* T)
   {
      return T->getTypeID() == ConstantBitCastInstID;
   }
};

} // namespace il
} // namespace cdot


#endif //CDOT_CONSTANTEXPR_H
