//
// Created by Jonas Zell on 22.11.17.
//

#ifndef CDOT_USE_H
#define CDOT_USE_H

#include <cstddef>
#include <iterator>

namespace cdot {
namespace il {

class Value;

class Use {
public:
   explicit Use(Value *user) : User(user), Next(nullptr), Prev(nullptr)
   {

   }

   void addUseAtEnd(Use *use)
   {
      auto next = this;
      while (next->Next) {
         next = next->Next;
      }

      use->Prev = next;
      next->Next = use;
   }

   void remove()
   {
      if (Prev) {
         Prev->Next = Next;
      }

      if (Next) {
         Next->Prev = Prev;
      }

      delete this;
   }

   template<class UseT>
   class iterator_impl: public std::iterator<std::forward_iterator_tag, UseT*> {
   public:
      iterator_impl(UseT *use) : current(use)
      {
         while (current->Prev) {
            current = current->Prev;
         }
      }

      iterator_impl() : current() {}

      bool operator==(const iterator_impl &x) { return x.current == current; }
      bool operator!=(const iterator_impl &x) { return !operator==(x); }

      UseT *operator*()
      {
         return current;
      }

      iterator_impl &operator++() // pre-increment
      {
         current = current->Next;
         return *this;
      }

      iterator_impl operator++(int) // post-increment
      {
         auto tmp = *this;
         current = current->Next;

         return tmp;
      }

      iterator_impl &operator--() // pre-decrement
      {
         current = current->Prev;
         return *this;
      }

      iterator_impl operator--(int) // post-decrement
      {
         auto tmp = *this;
         current = current->Prev;

         return tmp;
      }

   protected:
      UseT *current;
   };

   using iterator = iterator_impl<Use>;
   using const_iterator = iterator_impl<const Use>;

   size_t count() const
   {
      auto curr = this;
      size_t cnt = 1;
      while (curr->Next) {
         ++cnt;
         curr = curr->Next;
      }

      return cnt;
   }

   iterator begin()
   {
      auto first = this;
      while (first->Prev) {
         first = first->Prev;
      }

      return { first };
   }

   const_iterator begin() const
   {
      auto first = this;
      while (first->Prev) {
         first = first->Prev;
      }

      return { first };
   }

   iterator end()
   {
      return {};
   }

   const_iterator end() const
   {
      return {};
   }

   Value *getUser()
   {
      return User;
   }

   Value* getUser() const
   {
      return User;
   }

   operator Value*()
   {
      return User;
   }

protected:
   Value *User;
   Use *Next;
   Use *Prev;
};

} // namespace il
} // namespace cdot

#endif //CDOT_USE_H
