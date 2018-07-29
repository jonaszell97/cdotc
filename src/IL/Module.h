//
// Created by Jonas Zell on 16.11.17.
//

#ifndef CDOT_ILMODULE_H
#define CDOT_ILMODULE_H

#include "AST/DeclDenseMapInfo.h"
#include "IL/Function.h"
#include "IL/GlobalVariable.h"
#include "IL/SymbolTableList.h"
#include "IL/ValueSymbolTable.h"

#include <llvm/ADT/DenseSet.h>

namespace llvm {
   class MemoryBuffer;
   class Module;
   class raw_ostream;
} // namespace llvm

namespace cdot {
namespace serial {
   class ILModuleFile;
} // namespace serial

class CompilerInstance;

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

   ~Module();

   Function *insertFunction(Function *Fn,
                            bool OverridePrevious = false,
                            Function **Previous = nullptr);
   Function *getFunction(llvm::StringRef name);
   Function *getOwnFunction(llvm::StringRef name);

   GlobalVariable *insertGlobal(GlobalVariable *G,
                                bool OverridePrevious = false,
                                GlobalVariable **Previous = nullptr);
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
   void writeToFile(const char *FileName) const;

   bool linkInModule(std::unique_ptr<Module> &&M,
                     llvm::function_ref<void(GlobalObject*, GlobalObject*)>
                        Callback = {});

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

   llvm::StringRef getFileName() const { return fileName; }
   llvm::StringRef getPath() const { return path; }
   size_t getFileID() const { return fileID; }

   llvm::Module* getLLVMModule() const { return LLVMMod; }
   void setLLVMModule(llvm::Module* V) { LLVMMod = V; }

   bool hasExternallyVisibleSymbols()const{return HasExternallyVisibleSymbols;}
   void setHasExternallyVisibleSymbols(bool V){HasExternallyVisibleSymbols = V;}

   bool containsDeserializedValues() const { return ContainsDeserializedValues;}
   void setContainsDeserializedValues(bool V) { ContainsDeserializedValues = V;}

   bool isSynthesized() const { return Synthesized; }
   void setSynthesized(bool V) { Synthesized = V; }

   serial::ILModuleFile* getExternalLookup() const { return ExternalLookup; }
   void setExternalLookup(serial::ILModuleFile* V) { ExternalLookup = V; }

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

   serial::ILModuleFile *ExternalLookup = nullptr;

   Function *globalInitFn = nullptr;
   llvm::Module *LLVMMod = nullptr;

   bool Synthesized = false;
   bool ContainsDeserializedValues = false;
   bool HasExternallyVisibleSymbols = false;
};

} // namespace il
} // namespace cdot


#endif //CDOT_ILMODULE_H
