//
// Created by Jonas Zell on 22.11.17.
//

#ifndef CDOT_OPERANDBUNDLEUSER_H
#define CDOT_OPERANDBUNDLEUSER_H

#include "Operator/OperatorInst.h"

#include <llvm/ADT/ArrayRef.h>

namespace cdot {
namespace il {

class MultiOperandInst {
public:
   static bool classof(Value const *T)
   {
      switch (T->getTypeID()) {
         case Value::CallInstID:
         case Value::InvokeInstID:
            return true;
         default:
            return false;
      }
   }

   llvm::ArrayRef<Value const*> getOperands() const
   {
      return { Operands, numOperands };
   }

   friend class Instruction;

protected:
   explicit MultiOperandInst(llvm::ArrayRef<Value*> operands)
      : numOperands((unsigned)operands.size()),
        Operands(new Value*[operands.empty() ? 1 : operands.size()])
   {

      unsigned i = 0;
      for (const auto &op : operands) {
         Operands[i] = op;
         ++i;
      }
   }

   explicit MultiOperandInst(unsigned NumOperands)
      : numOperands(NumOperands), Operands(new Value*[NumOperands])
   {}

   ~MultiOperandInst() {
      delete[] Operands;
   }

   unsigned numOperands;
   Value **Operands;
};

} // namespace il
} // namespace cdot

#endif //CDOT_OPERANDBUNDLEUSER_H
