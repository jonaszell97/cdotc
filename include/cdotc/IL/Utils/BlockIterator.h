#ifndef CDOT_BLOCKITERATOR_H
#define CDOT_BLOCKITERATOR_H

#include "cdotc/IL/BasicBlock.h"
#include "cdotc/IL/Instructions.h"

#include <iterator>

namespace cdot {
namespace il {

template<class Ptr, class UseIt = decltype(Ptr()->use_begin())>
class PredIterator : public std::iterator<std::forward_iterator_tag, Ptr> {
public:
   PredIterator() = default;

   explicit PredIterator(Ptr ptr) : It(ptr->use_begin())
   {
      advanceToNextTerminator();
   }

   PredIterator(Ptr ptr, bool) : It(ptr->use_end()) {}

   using value_type = decltype(*Ptr());
   using reference = Ptr;

   bool operator==(const PredIterator& it) const { return It == it.It; }
   bool operator!=(const PredIterator& it) const { return !operator==(it); }

   PredIterator& operator++()
   {
      assert(!It.atEnd() && "advancing past end");

      ++It;
      advanceToNextTerminator();

      return *this;
   }

   PredIterator operator++(int)
   {
      assert(!It.atEnd() && "advancing past end");
      auto cpy = *this;
      ++*this;
      return cpy;
   }

   Ptr operator*() const
   {
      assert(!It.atEnd() && "dereferencing end iterator");
      return support::cast<TerminatorInst>(It->getUser())->getParent();
   }

   Ptr operator->() const { return operator*(); }

private:
   UseIt It;

   void advanceToNextTerminator()
   {
      while (!It.atEnd() && !support::isa<TerminatorInst>(It->getUser()))
         ++It;
   }
};

using pred_iterator = PredIterator<BasicBlock*>;
using const_pred_iterator = PredIterator<const BasicBlock*>;

inline pred_iterator pred_begin(BasicBlock* BB) { return pred_iterator(BB); }
inline pred_iterator pred_end(BasicBlock* BB)
{
   return pred_iterator(BB, true);
}

inline const_pred_iterator pred_begin(const BasicBlock* BB)
{
   return const_pred_iterator(BB);
}

inline const_pred_iterator pred_end(const BasicBlock* BB)
{
   return const_pred_iterator(BB, true);
}

using pred_iterator_range = llvm::iterator_range<pred_iterator>;
using const_pred_iterator_range = llvm::iterator_range<const_pred_iterator>;

inline pred_iterator_range getPredecessors(BasicBlock* BB)
{
   return pred_iterator_range(pred_begin(BB), pred_end(BB));
}

inline const_pred_iterator_range getPredecessors(const BasicBlock* BB)
{
   return const_pred_iterator_range(pred_begin(BB), pred_end(BB));
}

inline pred_iterator_range getPredecessors(BasicBlock& BB)
{
   return getPredecessors(&BB);
}

inline const_pred_iterator_range getPredecessors(const BasicBlock& BB)
{
   return getPredecessors(&BB);
}

template<class Ptr, class Term = decltype(Ptr()->getTerminator())>
class SuccIterator : public std::iterator<std::forward_iterator_tag, Ptr> {
public:
   explicit SuccIterator(Ptr BB) : TermInst(BB->getTerminator()), idx(0) {}

   SuccIterator(Ptr BB, bool) : TermInst(BB->getTerminator())
   {
      if (TermInst)
         idx = TermInst->getNumSuccessors();
      else
         idx = 0;
   }

   using value_type = decltype(*Ptr());
   using reference = Ptr;

   bool operator==(const SuccIterator& it) const
   {
      return TermInst == it.TermInst && idx == it.idx;
   }
   bool operator!=(const SuccIterator& it) const { return !operator==(it); }

   SuccIterator& operator++()
   {
      ++idx;
      return *this;
   }
   SuccIterator operator++(int)
   {
      auto cpy = *this;
      ++idx;
      return cpy;
   }

   SuccIterator& operator--()
   {
      --idx;
      return *this;
   }
   SuccIterator operator--(int)
   {
      auto cpy = *this;
      --idx;
      return cpy;
   }

   Ptr operator*() const { return TermInst->getSuccessorAt(idx); }
   Ptr operator->() const { return operator*(); }

private:
   Term TermInst;
   size_t idx;
};

using succ_iterator = SuccIterator<BasicBlock*>;
using const_succ_iterator = SuccIterator<const BasicBlock*>;

inline succ_iterator succ_begin(BasicBlock* BB) { return succ_iterator(BB); }
inline succ_iterator succ_end(BasicBlock* BB)
{
   return succ_iterator(BB, true);
}

inline const_succ_iterator succ_begin(const BasicBlock* BB)
{
   return const_succ_iterator(BB);
}

inline const_succ_iterator succ_end(const BasicBlock* BB)
{
   return const_succ_iterator(BB, true);
}

using succ_iterator_range = llvm::iterator_range<succ_iterator>;
using const_succ_iterator_range = llvm::iterator_range<const_succ_iterator>;

inline succ_iterator_range getSuccessors(BasicBlock* BB)
{
   return succ_iterator_range(succ_begin(BB), succ_end(BB));
}

inline const_succ_iterator_range getSuccessors(const BasicBlock* BB)
{
   return const_succ_iterator_range(succ_begin(BB), succ_end(BB));
}

inline succ_iterator_range getSuccessors(BasicBlock& BB)
{
   return getSuccessors(&BB);
}

inline const_succ_iterator_range getSuccessors(const BasicBlock& BB)
{
   return getSuccessors(&BB);
}

} // namespace il
} // namespace cdot

#endif // CDOT_BLOCKITERATOR_H
