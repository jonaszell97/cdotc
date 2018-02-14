//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_MODULE_H
#define CDOT_MODULE_H

#include <llvm/ADT/DenseSet.h>
#include <llvm/ADT/SmallPtrSet.h>

#include "../Value/ValueSymbolTable.h"
#include "../Value/SymbolTableList.h"

#include "../Value/Function/Function.h"
#include "../Value/GlobalVariable.h"
#include "../Value/Record/AggregateType.h"

namespace llvm {
   class raw_ostream;
   class MemoryBuffer;
} // namespace llvm

namespace cdot {

class CompilationUnit;

namespace il {

class ConstantString;
class Context;

class Module {
public:
   using FunctionList         = SymbolTableList<Function>;
   using func_iterator        = FunctionList::iterator;
   using func_const_iterator  = FunctionList::const_iterator;

   using GlobalList           = SymbolTableList<GlobalVariable>;
   using glob_iterator        = GlobalList::iterator;
   using glob_const_iterator  = GlobalList::const_iterator;

   using TypeList             = SymbolTableList<AggregateType>;
   using type_iterator        = TypeList::iterator;
   using type_const_iterator  = TypeList::const_iterator;

   using RefTypeList          = llvm::SmallPtrSet<AggregateType*, 4>;

   explicit Module(Context &Ctx,
                   size_t fileID = 0,
                   llvm::StringRef fileName = {},
                   llvm::StringRef path = {});

   Function *insertFunction(Function *func);
   Function *getFunction(llvm::StringRef name);
   Function *getOwnFunction(llvm::StringRef name);

   GlobalVariable *insertGlobal(GlobalVariable *global);
   GlobalVariable *getGlobal(llvm::StringRef name);
   GlobalVariable *getOwnGlobal(llvm::StringRef name);

   void insertType(AggregateType *Ty);

   ValueSymbolTable* getFunSymTab() const
   { return Functions.getSymTab().get(); }

   ValueSymbolTable* getGlobSymTab() const
   { return GlobalVariables.getSymTab().get(); }

   AggregateType *getType(llvm::StringRef name);
   bool addTypeReference(AggregateType *ty);
   bool addTypeReference(llvm::StringRef name);

   const RefTypeList &getReferencedTypes() const { return ReferencedTypes; }

   FunctionList const& getFuncList() const { return Functions; }
   GlobalList const& getGlobalList() const { return GlobalVariables; }
   TypeList const& getTypeList()     const { return Types; }

   FunctionList& getFuncList() { return Functions; }
   GlobalList& getGlobalList() { return GlobalVariables; }

   Context &getContext() const;

   void dump() const;
   void writeTo(llvm::raw_ostream &out) const;

   void serializeTo(llvm::raw_ostream &out) const;

   func_iterator begin() { return Functions.begin(); }
   func_iterator end() { return Functions.end(); }
   func_const_iterator begin() const { return Functions.begin(); }
   func_const_iterator end() const { return Functions.end(); }

   glob_iterator glob_begin() { return GlobalVariables.begin(); }
   glob_iterator glob_end() { return GlobalVariables.end(); }
   glob_const_iterator glob_begin() const { return GlobalVariables.begin(); }
   glob_const_iterator glob_end() const { return GlobalVariables.end(); }

   type_iterator type_begin() { return Types.begin(); }
   type_iterator type_end() { return Types.end(); }
   type_const_iterator type_begin() const { return Types.begin(); }
   type_const_iterator type_end() const { return Types.end(); }

   Function *getGlobalInitFn() const { return globalInitFn; }

   void setGlobalInitFn(Function *globalInitFn)
   { Module::globalInitFn = globalInitFn; }

   static TypeList Module::*getSublistAccess(AggregateType*)
   {
      return &Module::Types;
   }

   static FunctionList Module::*getSublistAccess(Function*)
   {
      return &Module::Functions;
   }

   static GlobalList Module::*getSublistAccess(GlobalVariable*)
   {
      return &Module::GlobalVariables;
   }

   llvm::StringRef getFileName() const
   {
      return fileName;
   }

   llvm::StringRef getPath() const
   {
      return path;
   }

   size_t getFileID() const
   {
      return fileID;
   }

protected:
   TypeList Types;
   FunctionList Functions;
   GlobalList GlobalVariables;

   RefTypeList ReferencedTypes;

   Context &Ctx;

   size_t fileID;
   std::string fileName;
   std::string path;

   Function *globalInitFn = nullptr;
};

} // namespace il
} // namespace cdot


#endif //CDOT_MODULE_H
