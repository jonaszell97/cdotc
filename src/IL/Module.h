//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_MODULE_H
#define CDOT_MODULE_H

#include "AST/DeclDenseMapInfo.h"
#include "IL/Function.h"
#include "IL/GlobalVariable.h"
#include "IL/SymbolTableList.h"
#include "IL/ValueSymbolTable.h"

#include <llvm/ADT/DenseSet.h>

namespace llvm {
   class raw_ostream;
   class MemoryBuffer;
} // namespace llvm

namespace cdot {

class CompilationUnit;

namespace il {

class VTable;
class PTable;
class TypeInfo;
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

   using RecordList           = llvm::DenseSet<ast::RecordDecl*>;
   using TypeInfoMap          = llvm::DenseMap<ast::RecordDecl*,
                                               GlobalVariable*>;

   using VTableMap            = llvm::DenseMap<ast::RecordDecl*,
                                               GlobalVariable*>;

   using PTableMap            = llvm::DenseMap<ast::RecordDecl*,
                                               GlobalVariable*>;

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

   void addRecord(ast::RecordDecl *R);
   const RecordList &getRecords() const { return Records; }

   GlobalVariable *getTypeInfo(ast::RecordDecl *R) const;
   GlobalVariable *getVTable(ast::RecordDecl *R) const;
   GlobalVariable *getPTable(ast::RecordDecl *R) const;

   void addTypeInfo(ast::RecordDecl *R, GlobalVariable *TI);
   void addVTable(ast::RecordDecl *R, GlobalVariable *VT);
   void addPTable(ast::RecordDecl *R, GlobalVariable *PT);

   ValueSymbolTable* getFunSymTab() const
   { return Functions.getSymTab().get(); }

   ValueSymbolTable* getGlobSymTab() const
   { return GlobalVariables.getSymTab().get(); }

   FunctionList const& getFuncList() const { return Functions; }
   GlobalList const& getGlobalList() const { return GlobalVariables; }

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

   Function *getGlobalInitFn() const { return globalInitFn; }

   void setGlobalInitFn(Function *globalInitFn)
   { Module::globalInitFn = globalInitFn; }

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
   FunctionList Functions;
   GlobalList GlobalVariables;
   RecordList Records;

   TypeInfoMap TIMap;
   VTableMap   VTMap;
   PTableMap   PTMap;

   Context &Ctx;

   size_t fileID;
   std::string fileName;
   std::string path;

   Function *globalInitFn = nullptr;
};

} // namespace il
} // namespace cdot


#endif //CDOT_MODULE_H
