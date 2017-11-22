//
// Created by Jonas Zell on 22.11.17.
//

#ifndef CDOT_OPERANDBUNDLEUSER_H
#define CDOT_OPERANDBUNDLEUSER_H

#include "Operator/OperatorInst.h"

namespace cdot {
namespace il {

class MultiOperandInst: public OperatorInst {
public:
   friend class OperatorInst;

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

private:
   unsigned getNumOperandsImpl() const { return numOperands; }

   Value *getOperandImpl(unsigned idx)
   {
      assert(idx < numOperands);
      return Operands[idx];
   }

   void setOperandImpl(unsigned idx, Value *V)
   {
      assert(idx < numOperands);
      Operands[idx] = V;
   }

   op_iterator op_begin_impl()
   {
      return &Operands[0];
   }

   op_iterator op_end_impl()
   {
      return Operands + numOperands;
   }

   op_const_iterator op_begin_impl() const
   {
      return &Operands[0];
   }

   op_const_iterator op_end_impl() const
   {
      return Operands + numOperands;
   }

protected:
   explicit MultiOperandInst(TypeID id,
                             llvm::ArrayRef<Value*> operands,
                             Type *resultType,
                             BasicBlock *parent,
                             const std::string &name = "",
                             const SourceLocation &loc = {})
      : OperatorInst(id, resultType, parent, name, loc),
        Operands(new Value*[operands.empty() ? 1 : operands.size()]),
        numOperands((unsigned)operands.size()) {

      unsigned i = 0;
      for (const auto &op : operands) {
         Operands[i] = op;
         op->addUse(this);

         ++i;
      }
   }

   ~MultiOperandInst() {
      delete[] Operands;
   }

   unsigned numOperands;
   Value **Operands;
};

} // namespace il
} // namespace cdot

#endif //CDOT_OPERANDBUNDLEUSER_H
