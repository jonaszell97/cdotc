#ifndef CDOT_CONSTANT_H
#define CDOT_CONSTANT_H

#include "Value.h"

namespace cdot {
namespace il {

class Constant: public Value {
public:
   friend class Value; // for handleReplacement

   static bool classof(Value const* T)
   {
      auto kind = T->getTypeID();
      return kind > _firstConstant && kind < _lastConstant;
   }

   Value *getOperand(unsigned idx) const;
   unsigned getNumOperands() const;
   void setOperand(unsigned idx, Constant *val);

   using op_iterator          = Constant**;
   using op_const_iterator    = Constant* const*;
   using iterator_range       = llvm::iterator_range<op_iterator>;
   using const_iterator_range = llvm::iterator_range<op_const_iterator>;

   op_iterator op_begin();
   op_iterator op_end();

   op_const_iterator op_begin() const;
   op_const_iterator op_end() const;

   op_iterator op_begin_impl() { return nullptr; }
   op_iterator op_end_impl()  { return op_begin() + getNumOperands(); }

   op_const_iterator op_begin_impl() const
   { return const_cast<Constant*>(this)->op_begin_impl(); }
   op_const_iterator op_end_impl() const
   { return op_begin() + getNumOperands(); }

   iterator_range getOperands()
   {
      return iterator_range(op_begin(), op_end());
   }

   const_iterator_range getOperands() const
   {
      return const_iterator_range(op_begin(), op_end());
   }

   unsigned getNumOperandsImpl() const { return 0; }

   void replaceOperand(Constant *Prev, Constant *New);

   bool isAllZerosValue() const { return ConstBits.AllZeros; }
   bool containsConstantEnum() const { return ConstBits.ContainsConstantEnum; }

protected:
   Constant(TypeID id, ValueType ty)
      : Value(id, ty)
   {}

private:
   void handleReplacement(Value *with);
};

llvm::raw_ostream &operator<<(llvm::raw_ostream &OS, il::Constant &C);

} // namespace il
} // namespace cdot


#endif //CDOT_CONSTANT_H
