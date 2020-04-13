#ifndef CDOT_CONDITIONALITERATOR_H
#define CDOT_CONDITIONALITERATOR_H

#include <iterator>
#include <llvm/ADT/ArrayRef.h>

namespace cdot {

template<class Orig, class Dest, Dest (*Projector)(Orig const&)>
class ArrayView {
public:
   ArrayView(llvm::ArrayRef<Orig> arr)
      : arr(arr)
   { }

private:
   llvm::ArrayRef<Orig> arr;

public:
   class iterator {
   public:
      typedef Dest value_type;
      typedef Dest reference;
      typedef void pointer;
      typedef ptrdiff_t difference_type;
      typedef std::random_access_iterator_tag iterator_category;

      iterator(Orig const* Ptr)
         : Ptr(Ptr)
      { }

   private:
      Orig const* Ptr;

   public:
      Dest operator*() const
      { return Projector(*Ptr); }

      Dest operator->() const
      { return operator*(); }

      iterator operator++()
      {
         ++Ptr;
         return *this;
      }

      iterator operator++(int)
      { return iterator(Ptr++); }

      iterator operator+(difference_type diff) const
      { return iterator(Ptr + diff); }

      iterator operator-(difference_type diff) const
      { return iterator(Ptr - diff); }

      iterator &operator+=(difference_type diff)
      {
         Ptr += diff;
         return *this;
      }

      iterator &operator-=(difference_type diff)
      {
         Ptr -= diff;
         return *this;
      }

      Dest operator[](difference_type diff) const
      {
         return Projector(Ptr[diff]);
      }

      bool operator==(iterator const &it) const
      { return Ptr == it.Ptr; }

      bool operator!=(iterator const &it) const
      { return !operator==(it); }

      bool operator>(iterator const &it) const
      { return Ptr > it.Ptr; }

      bool operator>=(iterator const &it) const
      { return Ptr >= it.Ptr; }

      bool operator<(iterator const &it) const
      { return Ptr < it.Ptr; }

      bool operator<=(iterator const &it) const
      {
         return Ptr <= it.Ptr;
      }
   };

public:
   iterator begin() const { return iterator(arr.begin()); }
   iterator end()   const { return iterator(arr.end()); }

   bool empty()  const { return arr.empty(); }
   size_t size() const { return arr.size(); }

   Dest operator[](unsigned idx) const { return arr[idx]; }
   Dest front() const { return Projector(arr.front()); }
   Dest back()  const { return Projector(arr.back()); }

   ArrayView drop_back(unsigned count = 1) const
   {
      return ArrayView(arr.drop_back(count));
   }

   ArrayView slice(unsigned start) const
   {
      return ArrayView(arr.slice(start));
   }

   ArrayView slice(unsigned start, unsigned length) const
   {
      return ArrayView(arr.slice(start, length));
   }
};

template<class Orig, class Dest, Dest* (*Projector)(Orig*)>
class FilteredArrayView {
public:
   FilteredArrayView(llvm::ArrayRef<Orig*> arr)
      : arr(arr)
   { }

private:
   llvm::ArrayRef<Orig*> arr;

public:
   class iterator {
   public:
      typedef Dest value_type;
      typedef Dest reference;
      typedef void pointer;
      typedef ptrdiff_t difference_type;
      typedef std::forward_iterator_tag iterator_category;

      iterator(Orig* const* Ptr, Orig* const* End)
         : Ptr(Ptr), End(End)
      {
         advanceUntilNotNull();
      }

   private:
      Orig* const* Ptr;
      Orig* const* End;

      void advanceUntilNotNull()
      {
         while (Ptr != End && !Projector(*Ptr))
            ++Ptr;
      }

   public:
      Dest* operator*() const
      { return Projector(*Ptr); }

      Dest* operator->() const
      { return operator*(); }

      iterator operator++()
      {
         ++Ptr;
         advanceUntilNotNull();

         return *this;
      }

      iterator operator++(int)
      {
         auto it = iterator(Ptr++);
         advanceUntilNotNull();

         return it;
      }

      iterator operator+(difference_type diff) const
      { return iterator(Ptr + diff); }

      iterator &operator+=(difference_type diff)
      {
         Ptr += diff;
         advanceUntilNotNull();

         return *this;
      }

      bool operator==(iterator const &it) const
      { return Ptr == it.Ptr; }

      bool operator!=(iterator const &it) const
      { return !operator==(it); }

      bool operator>(iterator const &it) const
      { return Ptr > it.Ptr; }

      bool operator>=(iterator const &it) const
      { return Ptr >= it.Ptr; }

      bool operator<(iterator const &it) const
      { return Ptr < it.Ptr; }

      bool operator<=(iterator const &it) const
      { return Ptr <= it.Ptr; }
   };

public:
   iterator begin() const { return iterator(arr.begin(), arr.end()); }
   iterator end()   const { return iterator(arr.end(), arr.end()); }

   bool empty()  const { return arr.empty(); }
   size_t size() const { return arr.size(); }

   Dest* operator[](unsigned idx) const { return arr[idx]; }
   Dest* front() const { return Projector(arr.front()); }
   Dest* back()  const { return Projector(arr.back()); }

   FilteredArrayView drop_back(unsigned count = 1) const
   {
      return FilteredArrayView(arr.drop_back(count));
   }

   FilteredArrayView slice(unsigned start) const
   {
      return FilteredArrayView(arr.slice(start));
   }

   FilteredArrayView slice(unsigned start, unsigned length) const
   {
      return FilteredArrayView(arr.slice(start, length));
   }
};

} // namespace cdot

#endif //CDOT_CONDITIONALITERATOR_H
