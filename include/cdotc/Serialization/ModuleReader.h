#ifndef CDOT_MODULEREADER_H
#define CDOT_MODULEREADER_H

#include "cdotc/Serialization/ASTReader.h"
#include "cdotc/Serialization/ILReader.h"

namespace cdot {

class Module;

namespace serial {

namespace reader {
class ASTDeclContextNameLookupTrait;
} // namespace reader

class ASTRecordReader;
class IncrementalCompilationManager;
class ModuleFile;
class ILModuleFile;
class LazyFunctionInfo;
class LazyILFunctionInfo;
class LazyILGlobalInfo;

class ModuleReader {
public:
   using RecordData = SmallVector<uint64_t, 64>;
   using RecordDataImpl = SmallVectorImpl<uint64_t>;

   friend class ASTReader;
   friend class ASTRecordReader;
   friend class ILReader;
   friend class IncrementalCompilationManager;
   friend class ModuleFile;
   friend class ILModuleFile;
   friend class LazyFunctionInfo;
   friend class LazyILFunctionInfo;
   friend class LazyILGlobalInfo;
   friend class reader::ASTDeclContextNameLookupTrait;

private:
   /// The current compiler instance
   CompilerInstance& CI;

   /// The location this module was loaded from
   SourceRange ImportLoc;

   /// The location assigned to this module for diagnostics
   SourceLocation DiagLoc;

   /// The module we are reading
   Module* Mod = nullptr;

   /// Non-null if we're writing an incremental compilation cache file.
   IncrementalCompilationManager* IncMgr = nullptr;

   /// The AST Reader
   serial::ASTReader ASTReader;

   /// The IL Reader
   serial::ILReader ILReader;

   /// The module reader for identifiers and types, if this is a cache file
   /// reader.
   ModuleReader* IdentTypeReader = nullptr;

   /// The hash table of the loaded module.
   reader::HashTable* ModTbl = nullptr;

   /// The main bitstream cursor
   llvm::BitstreamCursor Stream;

   /// The temporary IL module we read declarations from a cache file into.
   std::unique_ptr<il::Module> TmpMod;

   /// The source ID of the cache file being read, if applicable.
   unsigned SourceID = unsigned(-1);

   /// The blob containing the data for this module's static library.
   StringRef StaticLibBlob;

   /// The blob containing the data for this file's dependencies.
   StringRef DependencyBlob;

   /// Identifier IDs of modules that were imported by the module we're reading
   SmallVector<unsigned, 0> ImportedModuleIdents;

   /// Identifier IDs of modules that were imported by the module we're reading
   SmallVector<SmallVector<unsigned, 0>, 0> ImplicitlyImportedModuleIdents;

   /// Map from modules to their AST decl IDs.
   llvm::DenseMap<Module*, unsigned> ModuleDeclMap;

   /// Substitutions for source ids that are deserialized. These are used if
   /// we can find the files used to create this module on disk.
   llvm::DenseMap<unsigned, int> SourceIDSubstitutionOffsets;
   llvm::DenseMap<unsigned, unsigned> SourceIDSubstitutions;

   /// A vector containing identifiers that have already been
   /// loaded.
   ///
   /// If the pointer at index I is non-NULL, then it refers to the
   /// IdentifierInfo for the identifier with ID=I+1 that has already
   /// been loaded.
   std::vector<IdentifierInfo*> IdentifiersLoaded;

   /// The number of lookups into identifier tables.
   unsigned NumIdentifierLookups = 0;

   /// The number of lookups into identifier tables that succeed.
   unsigned NumIdentifierLookupHits = 0;

   // === Identifiers ===

   /// The number of identifiers in this AST file.
   unsigned LocalNumIdentifiers = 0;

   /// Offsets into the identifier table data.
   ///
   /// This array is indexed by the identifier ID (-1), and provides
   /// the offset into IdentifierTableData where the string data is
   /// stored.
   const uint32_t* IdentifierOffsets = nullptr;

   /// Base identifier ID for identifiers local to this module.
   unsigned BaseIdentifierID = 0;

   /// Actual data for the on-disk hash table of identifiers.
   ///
   /// This pointer points into a memory buffer, where the on-disk hash
   /// table for identifiers actually lives.
   const char* IdentifierTableData = nullptr;

   /// A pointer to an on-disk hash table of opaque type
   /// IdentifierHashTable.
   void* IdentifierLookupTable = nullptr;

   /// Modules, indexed by their ID.
   llvm::DenseMap<unsigned, Module*> Modules;

   /* Statistics */

   /// The number of statements (and expressions) de-serialized
   /// from the chain.
   unsigned NumStatementsRead = 0;

   /// Total number of decls read.
   unsigned NumDeclsRead = 0;

   /// Total number of types read.
   unsigned NumTypesRead = 0;

   /// Total number of identifiers read.
   unsigned NumIdentsRead = 0;

   /// Total number of IL Values read.
   unsigned NumILValuesRead = 0;

   /// Time during construction of this reader.
   long long StartTime = 0;

   ReadResult ReadControlBlock(llvm::BitstreamCursor& Stream);
   Module* ReadModuleBlock(llvm::BitstreamCursor& Stream,
                           Module* ParentModule = nullptr);

   ReadResult ReadCacheControlBlock();
   ReadResult ReadFileManagerBlock(llvm::BitstreamCursor& Stream);
   ReadResult ReadOptionsBlock();
   ReadResult ReadIdentifierBlock(llvm::BitstreamCursor& Cursor);
   ReadResult ReadOffsetsBlock(llvm::BitstreamCursor& Stream);
   ReadResult ReadDeclsTypesValuesBlock(llvm::BitstreamCursor& Stream);
   ReadResult ReadStaticLibBlock();
   ReadResult ReadOffsetRangeBlock(llvm::BitstreamCursor& Stream,
                                   IncrementalCompilationManager& Mgr,
                                   StringRef FileName);

   void LoadModuleImports();
   void LoadModuleImports(StringRef FileName);

   static bool ParseLanguageOptions(const RecordData& Record, bool Complain,
                                    bool AllowCompatibleDifferences);

   static bool ParseTargetOptions(const RecordData& Record, bool Complain,
                                  bool AllowCompatibleDifferences);

   /// Produce an error diagnostic and return true.
   ///
   /// This routine should only be used for fatal errors that have to
   /// do with non-routine failures (e.g., corrupted AST file).
   void Error(StringRef Msg) const;
   void Error(unsigned DiagID, StringRef Arg1 = StringRef(),
              StringRef Arg2 = StringRef()) const;

   /// ReadBlockAbbrevs - Enter a subblock of the specified BlockID with the
   /// specified cursor.  Read the abbreviations that are at the top of the
   /// block and then leave the cursor pointing into the block.
   static bool ReadBlockAbbrevs(llvm::BitstreamCursor& Cursor,
                                unsigned BlockID);

   IdentifierInfo* DecodeIdentifierInfo(unsigned ID);

   IdentifierInfo* GetIdentifierInfo(const RecordData& Record, unsigned& Idx)
   {
      return DecodeIdentifierInfo(Record[Idx++]);
   }

   IdentifierInfo* getLocalIdentifier(unsigned LocalID);
   IdentifierInfo* get(StringRef Name);

public:
   ModuleReader(CompilerInstance& CI, SourceRange ImportLoc,
                SourceLocation DiagLoc, llvm::BitstreamCursor Cursor);

   ModuleReader(CompilerInstance& CI, llvm::BitstreamCursor Cursor);

   ModuleReader(CompilerInstance& CI, llvm::BitstreamCursor Cursor,
                ModuleReader& MainReader);

   ~ModuleReader();

   ModuleReader(const ModuleReader&) = delete;
   ModuleReader& operator=(const ModuleReader&) = delete;

   Module* ReadModule();

   /// Finalize the read cache file.
   void FinalizeCacheFile(IncrementalCompilationManager& Mgr, Module* Mod,
                          StringRef FileName);

   /// Print statistics about the module being read.
   void printStatistics() const;

   bool incremental() const { return IncMgr != nullptr; }

   /// Returns the ID a declaration was loaded with, or 0 if it is not from
   /// this module.
   unsigned GetDeclID(ast::Decl* D);

   void SetIdentifierInfo(unsigned ID, IdentifierInfo* II);

   /// Returns the number of identifiers found in the chain.
   unsigned getTotalNumIdentifiers() const
   {
      return static_cast<unsigned>(IdentifiersLoaded.size());
   }

   CompilerInstance& getCompilerInstance() const { return CI; }

   struct IdentTypeReaderRAII {
      IdentTypeReaderRAII(ModuleReader& R, ModuleReader& IdentReader)
          : R(R), Previous(R.IdentTypeReader)
      {
         R.IdentTypeReader = &IdentReader;
      }

      ~IdentTypeReaderRAII() { R.IdentTypeReader = Previous; }

   private:
      ModuleReader& R;
      ModuleReader* Previous;
   };
};

} // namespace serial
} // namespace cdot

#endif // CDOT_MODULEREADER_H
