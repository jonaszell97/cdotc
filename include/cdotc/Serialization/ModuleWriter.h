#ifndef CDOT_MODULEWRITER_H
#define CDOT_MODULEWRITER_H

#include "cdotc/AST/Type.h"
#include "cdotc/Basic/IdentifierInfo.h"
#include "cdotc/Serialization/ASTWriter.h"
#include "cdotc/Serialization/ILWriter.h"

#include <llvm/ADT/DenseMap.h>
#include <llvm/Bitcode/BitstreamWriter.h>

#include <queue>

namespace cdot {

class Module;

namespace il {
class Value;
} // namespace il

namespace serial {

class ASTRecordWriter;
class IncrementalCompilationManager;

class ModuleWriter {
public:
   using RecordData = SmallVector<uint64_t, 64>;
   using RecordDataImpl = SmallVectorImpl<uint64_t>;
   using RecordDataRef = ArrayRef<uint64_t>;

   explicit ModuleWriter(CompilerInstance& CI, llvm::BitstreamWriter& Stream,
                         unsigned SourceID = unsigned(-1),
                         IncrementalCompilationManager* IncMgr = nullptr);

   explicit ModuleWriter(CompilerInstance& CI, llvm::BitstreamWriter& Stream,
                         ModuleWriter& MainWriter,
                         IncrementalCompilationManager* IncMgr = nullptr);

   struct ValueToEmit {
      enum Kind : unsigned {
         Type,
         Decl,
         ILValue,
      };

      ValueToEmit(QualType Ty) : Val(Ty.getAsOpaquePtr()), K(Type) {}
      ValueToEmit(ast::Decl* D) : Val(D), K(Decl) {}
      ValueToEmit(const il::Value* V)
          : Val(const_cast<il::Value*>(V)), K(ILValue)
      {
      }

      bool isType() const { return K == Type; }
      bool isILValue() const { return K == ILValue; }
      bool isDecl() const { return K == Decl; }

      operator QualType() const
      {
         assert(isType());
         return QualType::getFromOpaquePtr(Val);
      }

      operator ast::Decl*() const
      {
         assert(isDecl());
         return reinterpret_cast<ast::Decl*>(Val);
      }

      operator il::Value*() const
      {
         assert(isILValue());
         return reinterpret_cast<il::Value*>(Val);
      }

   private:
      void* Val;
      Kind K : 2;
   };

   friend class ASTWriter;
   friend class ASTRecordWriter;
   friend class ILWriter;
   friend class IncrementalCompilationManager;

private:
   /// The compiler instance
   CompilerInstance& CI;

   /// The BitstreamWriter that is being written to
   llvm::BitstreamWriter& Stream;

public:
   /// The AST writer
   serial::ASTWriter ASTWriter;

   /// The IL writer
   serial::ILWriter ILWriter;

private:
   /// The source ID of the cache file being written, if applicable.
   unsigned SourceID = unsigned(-1);

   /// The values to emit
   std::queue<ValueToEmit> ValuesToEmit;

   /// The module we're currently writing
   Module* Mod = nullptr;

   /// Non-null if we're writing an incremental compilation cache file.
   IncrementalCompilationManager* IncMgr = nullptr;

   /// Mapping from identifiers to their assigned IDs
   llvm::DenseMap<const IdentifierInfo*, unsigned> IdentIDMap;

   /// Offsets of each of the identifier IDs into the identifier
   /// table.
   std::vector<uint32_t> IdentifierOffsets;

   /// Next identifier ID to assign
   unsigned NextIdentID = 1;

   /// IDs assigned to modules while writing.
   llvm::DenseMap<Module*, unsigned> ModuleIDs;

   /// Next module ID to assign.
   unsigned NextModuleID = 1;

   /// Abbreviation for module metadata.
   unsigned ModuleMetadataAbbrevCode = 0;

   /* Statistics */

   /// The number of statements written to the AST file.
   unsigned NumStatements = 0;

   /// The number of instantiations written to the AST file.
   unsigned NumInstantiations = 0;

   /// Number of OperatorDecls and PrecedenceGroupDecls written.
   unsigned NumOpPGDecls = 0;

   /// Time during construction of this writer.
   long long StartTime = 0;

   /// Set to true once we're done writing the module file.
   bool DoneWriting = false;

   void WriteBlockInfoBlock();
   void WriteControlBlock(ast::ASTContext& Ctx);
   void WriteModuleInfo(Module* M);
   void WriteCacheControlBlock();
   void WriteIdentifierBlock();
   void WriteValuesTypesAndDecls();
   void WriteValuesAndTypes();
   void WriteStaticLibraryData(llvm::MemoryBuffer* Buf);
   void WriteFileManagerBlock();

   /// Emit a reference to an identifier.
   void AddIdentifierRef(const IdentifierInfo* II, RecordDataImpl& Record);

   /// Get the unique number used to refer to the given identifier.
   unsigned getIdentifierRef(const IdentifierInfo* II);

public:
   /// Main entry point to emit a module to a file.
   void WriteModule(Module* Mod, llvm::MemoryBuffer* LibBuf = nullptr);

   /// Write a cache file.
   void WriteCacheFile();

   void printStatistics() const;

   void sanityCheck();

   bool incremental() const { return IncMgr != nullptr; }

   CompilerInstance& getCompilerInstance() const { return CI; }
   unsigned int getSourceID() const { return SourceID; }

   unsigned getSourceIDForDecl(const ast::Decl* D);

   /// Note that the identifier II occurs at the given offset
   /// within the identifier table.
   void SetIdentifierOffset(const IdentifierInfo* II, uint32_t Offset);

   /// Returns the number of exported symbols in this module.
   unsigned getNumExternallyVisibleSymbols()
   {
      return ILWriter.getNumVisibleSymbols();
   }

   Module* getModuleBeingWritten() const { return Mod; }
};

} // namespace serial
} // namespace cdot

#endif // CDOT_MODULEWRITER_H
