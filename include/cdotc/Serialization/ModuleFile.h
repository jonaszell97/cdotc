#ifndef CDOT_MODULEFILE_H
#define CDOT_MODULEFILE_H

#include "cdotc/Basic/DeclarationName.h"

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/SmallPtrSet.h>
#include <llvm/Bitcode/BitstreamReader.h>

namespace cdot {
namespace ast {
class CallableDecl;
class Decl;
class DeclContext;
class NamedDecl;
class Statement;
} // namespace ast

namespace il {
class Function;
class GlobalObject;
class GlobalVariable;
} // namespace il

namespace serial {

class ModuleReader;

class ModuleFile {
   /// The module reader object.
   ModuleReader& Reader;

   /// Actually a pointer to an on-disk declaration hash table
   void* HashTablePtr;

   /// Names that we already looked up in this external table.
   SmallPtrSet<DeclarationName, 4> AlreadyLookedUp;

   /// Actually a pointer to the on-disk hash table containing the module's
   /// instantiations.
   void* InstantiationTable = nullptr;

   /// True if we already loaded all decls.
   bool LoadedAllDecls = false;

   /// Act on a loaded declaration.
   void LoadedDecl(ast::DeclContext& Ctx, ast::Decl* ReadDecl,
                   bool IgnoreInst = false);

public:
   ModuleFile(ModuleReader& Reader, void* HashTablePtr);
   ~ModuleFile();

   void PerformExternalLookup(ast::DeclContext& Ctx, DeclarationName Name);
   llvm::StringRef getLibraryBlob();

   void setInstantiationTable(void* Tbl) { InstantiationTable = Tbl; }
   ast::NamedDecl* LookupInstantiation(StringRef MangledName);

   void LoadAllDecls(ast::DeclContext& Ctx, bool IgnoreInst = false);

   ModuleFile* copy() const;
};

class LazyFunctionInfo {
   /// The module reader object.
   ModuleReader& Reader;

   /// Cursor to the body of the function.
   llvm::BitstreamCursor BodyCursor;

   /// Set once the body has been read.
   ast::Statement *ReadBody = nullptr;

public:
   LazyFunctionInfo(ModuleReader& Reader, llvm::BitstreamCursor BodyCursor);

   void loadBody(ast::CallableDecl* Fn);
};

class ILModuleFile {
   /// The module reader object.
   ModuleReader& Reader;

   /// The module's symbol table.
   void* SymTab;

public:
   ILModuleFile(ModuleReader& Reader, void* SymTab);
   ~ILModuleFile();

   il::GlobalObject* Lookup(StringRef Name);
};

class LazyILFunctionInfo {
   /// The module reader object.
   ModuleReader& Reader;

   /// The function the lazy body belongs to.
   il::Function& F;

   /// IDs of the basic blocks in the function.
   SmallVector<unsigned, 0> BlockIDs;

   /// IDs of the instructions in the entry block.
   SmallVector<unsigned, 0> EntryBlockInstIDs;

   /// The function's real linkage. Without a body, it has to be external.
   unsigned Linkage;

public:
   LazyILFunctionInfo(ModuleReader& Reader, il::Function& F,
                      SmallVector<unsigned, 0>&& BlockIDs,
                      SmallVector<unsigned, 0>&& EntryBlockInstIDs,
                      unsigned Linkage);

   void loadFunctionBody();
};

class LazyILGlobalInfo {
   /// The module reader object.
   ModuleReader& Reader;

   /// The global variable the lazy initializer belongs to.
   il::GlobalVariable& G;

   /// The ID of the initializer value.
   unsigned InitID;

   /// The global's real linkage. Without a body, it has to be external.
   unsigned Linkage;

public:
   LazyILGlobalInfo(ModuleReader& Reader, il::GlobalVariable& G,
                    unsigned InitID, unsigned Linkage);

   void loadGlobalInitializer();
};

} // namespace serial
} // namespace cdot

#endif // CDOT_MODULEFILE_H
