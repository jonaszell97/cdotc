//
// Created by Jonas Zell on 23.11.17.
//

#ifndef CDOT_SYMBOLTABLELIST_H
#define CDOT_SYMBOLTABLELIST_H

#include <llvm/ADT/StringRef.h>
#include <llvm/ADT/ilist.h>
#include <llvm/ADT/simple_ilist.h>
#include <llvm/Support/Casting.h>

#include "ValueSymbolTable.h"

namespace cdot {
namespace il {

class Context;
class Module;
class GlobalVariable;
class AggregateType;
class Function;
class Argument;
class BasicBlock;
class Instruction;
class ValueSymbolTable;

}
}

namespace cdot {
namespace il {

template<class ValueSubClass>
struct ValueOwner {};

#define CDOT_VALUE_OWNER(Owner, ValueSubClass) \
   template<> struct ValueOwner<ValueSubClass> { using type = Owner; }

CDOT_VALUE_OWNER(Context, Module);
CDOT_VALUE_OWNER(Module, AggregateType);
CDOT_VALUE_OWNER(Module, GlobalVariable);
CDOT_VALUE_OWNER(Module, Function);
CDOT_VALUE_OWNER(Function, BasicBlock);
CDOT_VALUE_OWNER(BasicBlock, Argument);
CDOT_VALUE_OWNER(BasicBlock, Instruction);

template <class T>
class SymbolTableList {
public:
   using OwnerType      = typename ValueOwner<T>::type;
   using ListTy         = llvm::iplist_impl<llvm::simple_ilist<T>,
                                            llvm::ilist_traits<T>>;
   using iterator       = typename ListTy::iterator;
   using const_iterator = typename ListTy::const_iterator;
   using const_reference = typename ListTy::const_reference;
   using reference = typename ListTy::reference;

   explicit SymbolTableList(OwnerType *Owner)
      : Owner(Owner), SymTab(std::make_shared<ValueSymbolTable>())
   { }

   explicit SymbolTableList(OwnerType *Owner,
                            std::shared_ptr<ValueSymbolTable> const& SymTab)
      : Owner(Owner), SymTab(SymTab)
   { }

   explicit SymbolTableList(SymbolTableList &&List)
      : Owner(List.Owner), SymTab(List.SymTab), ilist(std::move(List.ilist))
   {

   }

   SymbolTableList& operator=(SymbolTableList &&List)
   {
      if (this != &List) {
         delete SymTab;

         Owner = List.Owner;
         SymTab = std::move(List.SymTab);
         ilist = std::move(List.ilist);
      }

      return *this;
   }

   OwnerType *getOwner() const
   { return Owner; }

   std::shared_ptr<ValueSymbolTable> const& getSymTab() const
   {
      return SymTab;
   }

   void push_back(T *Val)
   {
      Val->setParent(Owner);
      SymTab->insertValue(Val);
      ilist.push_back(Val);
   }

   iterator insert(iterator it, T *Val)
   {
      Val->setParent(Owner);
      SymTab->insertValue(Val);
      return ilist.insert(it, Val);
   }

   iterator erase(iterator it)
   {
      SymTab->removeValue(&*it);
      return ilist.erase(it);
   }

   iterator erase(T *Val)
   {
      SymTab->removeValue(Val);
      for (auto it = ilist.begin(); it != ilist.end(); ++it) {
         if (&*it == Val) {
            return ilist.erase(it);
         }
      }

      llvm_unreachable("value not in list");
   }

   void remove(T* Val)
   {
      SymTab->removeValue(Val);
      ilist.remove(Val->getIterator());
   }

   void remove(iterator it)
   {
      SymTab->removeValue(&*it);
      ilist.remove(it);
   }

   T *getNextNode(T &ptr)
   {
      return ilist.getNextNode(ptr);
   }

   T *getPrevNode(T &ptr)
   {
      return ilist.getPrevNode(ptr);
   }

   T* getNextNode(T const& ptr) const
   {
      return ilist.getNextNode(const_cast<T&>(ptr));
   }

   T* getPrevNode(T const& ptr) const
   {
      return ilist.getPrevNode(const_cast<T&>(ptr));
   }

   size_t size() const { return empty() ? 0 : ilist.size(); }
   bool empty() const { return ilist.empty(); }

   const T& operator[](size_t idx) const
   {
      auto it = begin();
      for (;;) {
         if (!idx) {
            break;
         }

         ++it;
         --idx;
      }

      return *it;
   }

   T& operator[](size_t idx)
   {
      auto it = begin();
      for (;;) {
         if (!idx) {
            break;
         }

         ++it;
         --idx;
      }

      return *it;
   }

   T* find(llvm::StringRef name) const
   {
      auto val = SymTab->lookup(name);
      return val ? llvm::dyn_cast<T>(val) : nullptr;
   }

   iterator begin() { return ilist.begin(); }
   iterator end() { return ilist.end(); }

   const_iterator begin() const { return ilist.begin(); }
   const_iterator end()   const { return ilist.end(); }

   reference front() { return ilist.front(); }
   reference back()  { return ilist.back(); }

   const_reference front() const { return ilist.front(); }
   const_reference back()  const { return ilist.back(); }

private:
   OwnerType *Owner;
   std::shared_ptr<ValueSymbolTable> SymTab;
   ListTy ilist;
};

} // namespace il
} // namespace cdot


#endif //CDOT_SYMBOLTABLELIST_H
