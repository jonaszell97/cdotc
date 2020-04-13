#ifndef CDOT_ASTREADER_H
#define CDOT_ASTREADER_H

#include "cdotc/AST/ASTContext.h"
#include "cdotc/AST/Decl.h"
#include "cdotc/AST/Expression.h"
#include "cdotc/AST/Statement.h"
#include "cdotc/Lex/SourceLocation.h"
#include "cdotc/Support/LLVM.h"

#include <llvm/Bitcode/BitstreamReader.h>

#include <queue>

namespace cdot {
namespace fs {
class FileManager;
} // namespace fs

namespace il {
class Constant;
} // namespace il

namespace ast {
class SemaPass;
class RecordDecl;
} // namespace ast

namespace serial {
namespace reader {
struct DeclContextLookupTable;
class ConformanceLookupTrait;
} // namespace reader

class ASTRecordReader;
class ILReader;
class IncrementalCompilationManager;
class ModuleReader;
class LazyFunctionInfo;

enum ReadResult : unsigned char;

class ASTReader {
public:
   using RecordData = SmallVector<uint64_t, 64>;
   using RecordDataImpl = SmallVectorImpl<uint64_t>;

   friend class ASTDeclReader;
   friend class ASTRecordReader;
   friend class ILReader;
   friend class IncrementalCompilationManager;
   friend class ModuleReader;
   friend class reader::ConformanceLookupTrait;

private:
   /// The module reader object.
   ModuleReader& Reader;

   /// The Sema instance.
   ast::SemaPass& Sema;

   /// The AST Context.
   ast::ASTContext& Context;

   /// The source file manager.
   fs::FileManager& FileMgr;

   /// The reader for declarations, if we're reading a cache file.
   ASTReader* DeclReader = nullptr;

   /// The location where the module file will be considered as
   /// imported from. For non-module AST types it should be invalid.
   SourceLocation CurrentImportLoc;

   /// Map from a DeclContext to its lookup tables.
   llvm::DenseMap<const ast::DeclContext*, reader::DeclContextLookupTable>
       Lookups;

   /// Declarations that have already been loaded from the chain.
   ///
   /// When the pointer at index I is non-NULL, the declaration with ID
   /// = I + 1 has already been loaded.
   std::vector<ast::Decl*> DeclsLoaded;

   /// Mapping from loaded decls to the ID they were loaded with
   llvm::DenseMap<ast::Decl*, unsigned> DeclIDMap;

   /// DeclsCursor - This is a cursor to the start of the DECLS_BLOCK block. It
   /// has read all the abbreviations at the start of the block and is ready to
   /// jump around with these in context.
   llvm::BitstreamCursor DeclsCursor;

   /// Types that have already been loaded from the chain.
   ///
   /// When the pointer at index I is non-NULL, the type with
   /// ID = (I + 1) << FastQual::Width has already been loaded
   std::vector<QualType> TypesLoaded;

   /// Blob containing the conformance data.
   StringRef ConformanceData;

   /// Actually a pointer to the on-disk hash table containing the module's
   /// conformances.
   void* ConformanceTable = nullptr;

   /// Actually a pointer to the on-disk hash table containing the module's
   /// instantiations.
   void* InstantiationTable = nullptr;

   /// Map from decls to the lexical context they should be added to when
   /// they are deserialized.
   llvm::DenseMap<unsigned, ast::DeclContext*> DeclContextMap;

   /// The number of declarations in this AST file.
   unsigned LocalNumDecls = 0;

   /// Offset of each declaration within the bitstream, indexed
   /// by the declaration ID (-1).
   const uint32_t* DeclOffsets = nullptr;

   /// Base declaration ID for declarations local to this module.
   unsigned BaseDeclID = 0;

   /// Number of operator and precedence group decls
   unsigned NumOperatorPrecedenceDecls = 0;

   /// Operator and precedence group IDs
   const uint32_t* OpPrecedenceData = nullptr;

   /// Lazy function infos, needed for eager deserializing.
   std::queue<std::pair<ast::CallableDecl*, LazyFunctionInfo*>> LazyFnInfos;

   /// Scopes read from the file, indexed by ID.
   std::vector<Scope*> LoadedScopes;

   /// The number of scopes in this AST file.
   unsigned LocalNumScopes = 0;

   /// Offset of each scope within the bitstream, indexed
   /// by the declaration ID (-1).
   const uint32_t* ScopeOffsets = nullptr;

   /// Base scope ID for scopes local to this module.
   unsigned BaseScopeID = 0;

   // === Types ===

   /// The number of types in this AST file.
   unsigned LocalNumTypes = 0;

   /// Offset of each type within the bitstream, indexed by the
   /// type ID, or the representation of a Type*.
   const uint32_t* TypeOffsets = nullptr;

   /// Base type ID for types local to this module as represented in
   /// the global type ID space.
   unsigned BaseTypeIndex = 0;

   /// The ID assigned to the module being read.
   unsigned MainModuleID = 0;

   /// The Decl IDs for the Sema/Lexical DeclContext of a Decl that has
   /// been loaded but its DeclContext was not set yet.
   struct PendingDeclContextInfo {
      ast::Decl* D;
      unsigned SemaDC;
      unsigned LexicalDC;
   };

   /// The set of Decls that have been loaded but their DeclContexts are
   /// not set yet.
   ///
   /// The DeclContexts for these Decls will be set once recursive loading has
   /// been completed.
   std::deque<PendingDeclContextInfo> PendingDeclContextInfos;

public:
   struct DeclUpdate {
      DeclUpdate(ast::Decl* D, unsigned ID, unsigned* DeclIDs) noexcept
          : D(D), ID(ID), DeclIDs(DeclIDs)
      {
      }

      ~DeclUpdate() { delete[] DeclIDs; }

      DeclUpdate(const DeclUpdate&) = delete;
      DeclUpdate& operator=(const DeclUpdate&) = delete;

      DeclUpdate(DeclUpdate&& Other) noexcept
          : D(Other.D), ID(Other.ID), DeclIDs(Other.DeclIDs)
      {
         Other.DeclIDs = nullptr;
      }

      DeclUpdate& operator=(DeclUpdate&& Other) noexcept
      {
         this->~DeclUpdate();
         new (this) DeclUpdate(std::move(Other));
         return *this;
      }

      ast::Decl* D;
      unsigned ID;
      unsigned* DeclIDs;
   };

   template<class... Ts>
   void addDeclUpdate(ast::Decl* D, unsigned ID, Ts... DeclIDs)
   {
      DeclUpdates.emplace(D, ID, new unsigned[sizeof...(Ts)]{DeclIDs...});
   }

   struct UnfinishedDecl {
      UnfinishedDecl(ast::NamedDecl* ND, RecordData&& Record, unsigned Idx)
          : ND(ND), Record(std::move(Record)), Idx(Idx)
      {
      }

      UnfinishedDecl(const UnfinishedDecl&) = delete;
      UnfinishedDecl& operator=(const UnfinishedDecl&) = delete;

      UnfinishedDecl(UnfinishedDecl&& Other) noexcept
          : ND(Other.ND), Record(std::move(Other.Record)), Idx(Other.Idx)
      {
      }

      UnfinishedDecl& operator=(UnfinishedDecl&& Other) noexcept
      {
         this->~UnfinishedDecl();
         new (this) UnfinishedDecl(std::move(Other));
         return *this;
      }

      ast::NamedDecl* ND;
      RecordData Record;
      unsigned Idx;
   };

   void addUnfinishedDecl(ast::NamedDecl* ND, RecordData&& Record, unsigned Idx)
   {
      UnfinishedDecls.emplace(ND, std::move(Record), Idx);
   }

   void registerInstantiationScope(ast::NamedDecl* Inst, unsigned ID)
   {
      InstScopeMap[Inst] = ID;
   }

   void addLazyFnInfo(ast::CallableDecl* C, LazyFunctionInfo* Inf)
   {
      LazyFnInfos.emplace(C, Inf);
   }

   const llvm::DenseMap<ast::NamedDecl*, unsigned int>& getInstScopeMap() const
   {
      return InstScopeMap;
   }

   void addDeclToContext(ast::Decl* D, ast::DeclContext* Ctx);

private:
   /// Declarations that need additional Decl references set on them that
   /// might not be available during initial deserialization
   std::queue<DeclUpdate> DeclUpdates;

   /// Declarations that still need some deserialization done on them, for
   /// example templates or instantiations that need their declarations
   /// deserialized immediately.
   std::queue<UnfinishedDecl> UnfinishedDecls;

   /// When reading a Stmt tree, Stmt operands are placed in this stack.
   SmallVector<ast::Statement*, 16> StmtStack;

   /// Map from instantiations to the scope they were instantiated in.
   llvm::DenseMap<ast::NamedDecl*, unsigned> InstScopeMap;

   /// What kind of records we are reading.
   enum ReadingKind { Read_None, Read_Decl, Read_Type, Read_Stmt };

   /// What kind of records we are reading.
   ReadingKind ReadingKind = Read_None;

   /// True if we're reading a Decl right now.
   bool ReadingDecl = false;

   /// RAII object to change the reading kind.
   class ReadingKindTracker {
      ASTReader& Reader;
      enum ReadingKind PrevKind;

   public:
      ReadingKindTracker(enum ReadingKind newKind, ASTReader& reader)
          : Reader(reader), PrevKind(Reader.ReadingKind)
      {
         Reader.ReadingKind = newKind;
      }

      ReadingKindTracker(const ReadingKindTracker&) = delete;
      ReadingKindTracker& operator=(const ReadingKindTracker&) = delete;
      ~ReadingKindTracker() { Reader.ReadingKind = PrevKind; }
   };

   ReadResult ReadASTBlock(llvm::BitstreamCursor& Stream);
   void ReadDeclsEager(ArrayRef<unsigned> Decls);

   void ReadOperatorPrecedenceGroups();

   ReadResult ReadConformanceTable(RecordDataImpl& Record, StringRef Blob);
   ReadResult ReadInstantiationTable(RecordDataImpl& Record, StringRef Blob);
   ReadResult ReadConformanceBlock(llvm::BitstreamCursor& Stream);

   /// Read the record that describes the lexical contents of a DC.
   bool ReadLexicalDeclContextStorage(uint64_t Offset, ast::DeclContext* DC);

   /// Read the record that describes the visible contents of a DC.
   bool ReadVisibleDeclContextStorage(uint64_t Offset, ast::DeclContext* DC,
                                      unsigned ID);

   void addPendingDeclContextInfo(ast::Decl* D, unsigned SemaDC,
                                  unsigned LexicalDC)
   {
      assert(D);
      PendingDeclContextInfo Info = {D, SemaDC, LexicalDC};
      PendingDeclContextInfos.push_back(Info);
   }

   void finalizeUnfinishedDecls();

   /// Produce an error diagnostic and return true.
   ///
   /// This routine should only be used for fatal errors that have to
   /// do with non-routine failures (e.g., corrupted AST file).
   void Error(StringRef Msg) const;
   void Error(unsigned DiagID, StringRef Arg1 = StringRef(),
              StringRef Arg2 = StringRef()) const;

public:
   explicit ASTReader(ModuleReader& Reader);
   ASTReader(ModuleReader& Reader, ASTReader& DeclReader);

   ASTReader(const ASTReader&) = delete;
   ASTReader& operator=(const ASTReader&) = delete;

   ~ASTReader();

   ModuleReader& getReader() const { return Reader; }
   const llvm::BitstreamCursor& getDeclsCursor() const { return DeclsCursor; }

   fs::FileManager& getFileMgr() const { return FileMgr; }
   ast::SemaPass& getSema() const { return Sema; }
   ast::ASTContext& getContext() const { return Context; }
   ast::ModuleDecl* getReadModule();

   IdentifierTable& getIdentifierTable() const
   {
      return Context.getIdentifiers();
   }

   void ReadConformances(unsigned ID);

private:
   bool FindExternalVisibleDeclsByName(const ast::DeclContext* DC,
                                       DeclarationName Name);

public:
   /// Returns the number of declarations found in the chain.
   unsigned getTotalNumDecls() const
   {
      return static_cast<unsigned>(DeclsLoaded.size());
   }

   /// Returns the number of scopes found in the chain.
   unsigned getTotalNumScopes() const
   {
      return static_cast<unsigned>(LoadedScopes.size());
   }

   unsigned DeclCursorForID(unsigned ID);
   unsigned ScopeCursorForID(unsigned ID);

   void LoadedDecl(unsigned Index, ast::Decl* D);

   /// Read a type from the current position in the given record, which
   /// was read from the given AST file.
   QualType readType(const RecordData& Record, unsigned& Idx);
   QualType getLocalType(unsigned ID);

   QualType readTypeRecord(unsigned ID);
   unsigned TypeCursorForIndex(unsigned Index);

   /// Resolve a type ID into a type, potentially building a new
   /// type.
   QualType GetType(unsigned ID);

   /// Returns the number of types found in the chain.
   unsigned getTotalNumTypes() const
   {
      return static_cast<unsigned>(TypesLoaded.size());
   }

   /// Resolve a declaration ID into a declaration, potentially
   /// building a new declaration.
   ast::Decl* GetDecl(unsigned ID);

   ast::Decl* ReadDeclRecord(unsigned ID);

   /// Reads a declaration ID from the given position in a record in the
   /// given module.
   ///
   /// \returns The declaration ID read from the record, adjusted to a global
   /// ID.
   unsigned ReadDeclID(const RecordData& Record, unsigned& Idx);

   /// Reads a declaration from the given position in a record in the
   /// given module.
   ast::Decl* ReadDecl(const RecordData& R, unsigned& I)
   {
      return GetDecl(ReadDeclID(R, I));
   }

   /// Returns true if the declaration described by this ID has already been
   /// deserialized.
   bool IsDeclLoaded(unsigned ID);

   /// Reads a declaration from the given position in a record in the
   /// given module.
   ///
   /// \returns The declaration read from this location, casted to the given
   /// result type.
   template<typename T> T* ReadDeclAs(const RecordData& R, unsigned& I)
   {
      return support::cast_or_null<T>(GetDecl(ReadDeclID(R, I)));
   }

   IdentifierInfo* GetIdentifierInfo(const RecordData& Record, unsigned& Idx);

   IdentifierInfo* getLocalIdentifier(unsigned ID);

   Scope* getScope(unsigned ID);
   Scope* ReadScopeRecord(unsigned ID);

   /// Read a declaration name.
   DeclarationName ReadDeclarationName(const RecordData& Record, unsigned& Idx);

   /// Read a nested name specifier.
   NestedNameSpecifier* ReadNestedNameSpec(const RecordData& Record,
                                           unsigned& Idx);

   /// Read a nested name specifier.
   NestedNameSpecifierWithLoc*
   ReadNestedNameSpecWithLoc(const RecordData& Record, unsigned& Idx);

   /// Read a template argument.
   sema::TemplateArgument ReadTemplateArgument(const RecordData& Record,
                                               unsigned& Idx);

   /// Read a template argument array.
   sema::FinalTemplateArgumentList*
   ReadTemplateArgumentList(const RecordData& Record, unsigned& Idx);

   /// Read a source location
   SourceLocation ReadSourceLocation(const RecordDataImpl& Record,
                                     unsigned& Idx);

   /// Read a source range
   SourceRange ReadSourceRange(const RecordData& Record, unsigned& Idx);

   /// Read an integral value
   llvm::APInt ReadAPInt(const RecordData& Record, unsigned& Idx);

   /// Read a signed integral value
   llvm::APSInt ReadAPSInt(const RecordData& Record, unsigned& Idx);

   /// Read a floating-point value
   llvm::APFloat ReadAPFloat(const RecordData& Record,
                             const llvm::fltSemantics& Sem, unsigned& Idx);

   // Read a string
   static std::string ReadString(const RecordData& Record, unsigned& Idx);

   // Skip a string
   static void SkipString(const RecordData& Record, unsigned& Idx)
   {
      Idx += Record[Idx] + 1;
   }

   /// Reads attributes from the current stream position.
   void ReadAttributes(ASTRecordReader& Record, SmallVectorImpl<Attr*>& Attrs,
                       bool* FoundInterestingAttr = nullptr);

   /// Reads a statement.
   ast::Statement* ReadStmt();

   /// Reads an expression.
   ast::Expression* ReadExpr();

   /// Reads a sub-statement operand during statement reading.
   ast::Statement* ReadSubStmt()
   {
      assert(ReadingKind == Read_Stmt
             && "Should be called only during statement reading!");

      // Subexpressions are stored from last to first, so the next Stmt we need
      // is at the back of the stack.
      assert(!StmtStack.empty() && "Read too many sub-statements!");
      return StmtStack.pop_back_val();
   }

   ast::Statement* ReadStmtFromStream(llvm::BitstreamCursor& Cursor);

   /// Reads a sub-expression operand during statement reading.
   ast::Expression* ReadSubExpr();

   /// Reads a token out of a record.
   lex::Token ReadToken(const RecordDataImpl& Record, unsigned& Idx);

   /// Reads a scope from the record.
   Scope* ReadScope(const RecordDataImpl& Record, unsigned& Idx);

   /// Get a module from its ID.
   Module* GetModule(unsigned ID);
};

/// An object for streaming information from a record.
class ASTRecordReader {
   ASTReader* Reader;
   unsigned Idx = 0;
   ASTReader::RecordData Record;

   using RecordData = ASTReader::RecordData;
   using RecordDataImpl = ASTReader::RecordDataImpl;

   friend class ASTReader;

public:
   /// Construct an ASTRecordReader that uses the default encoding scheme.
   explicit ASTRecordReader(ASTReader& Reader) : Reader(&Reader) {}

   ASTRecordReader(ASTReader* Reader, unsigned int Idx, RecordData&& Record)
       : Reader(Reader), Idx(Idx), Record(std::move(Record))
   {
   }

   ASTRecordReader(ASTRecordReader&& Other) noexcept
       : Reader(Other.Reader), Idx(Other.Idx), Record(std::move(Other.Record))
   {
      Other.Idx = 0;
      Other.Record.clear();
   }

   ASTReader* getReader() const { return Reader; }
   RecordData& getRecordData() { return Record; }

   /// Reads a record with id AbbrevID from Cursor, resetting the
   /// internal state.
   unsigned readRecord(llvm::BitstreamCursor& Cursor, unsigned AbbrevID);

   /// The current position in this record.
   unsigned getIdx() const { return Idx; }

   /// The length of this record.
   size_t size() const { return Record.size(); }

   /// An arbitrary index in this record.
   const uint64_t& operator[](size_t N) { return Record[N]; }

   /// The last element in this record.
   const uint64_t& back() const { return Record.back(); }

   /// Returns the current value in this record, and advances to the
   /// next value.
   const uint64_t& readInt() { return Record[Idx++]; }

   /// Reads a bool and advances to the next value
   bool readBool() { return static_cast<bool>(Record[Idx++]); }

   /// Returns the current value in this record, without advancing.
   const uint64_t& peekInt() { return Record[Idx]; }

   /// Skips the specified number of values.
   void skipInts(unsigned N) { Idx += N; }

   template<class EnumTy> EnumTy readEnum()
   {
      return static_cast<EnumTy>(Record[Idx++]);
   }

   /// Reads a statement.
   ast::Statement* readStmt() { return Reader->ReadStmt(); }

   /// Reads an expression.
   ast::Expression* readExpr() { return Reader->ReadExpr(); }

   /// Reads a sub-statement operand during statement reading.
   ast::Statement* readSubStmt() { return Reader->ReadSubStmt(); }

   /// Reads a sub-expression operand during statement reading.
   ast::Expression* readSubExpr() { return Reader->ReadSubExpr(); }

   /// Read a type from the current position in the record.
   QualType readType() { return Reader->readType(Record, Idx); }

   ast::SourceType readSourceType()
   {
      switch (Reader->ReadingKind) {
      case ASTReader::ReadingKind::Read_None:
         llvm_unreachable("should not be called when reading nothing");
      case ASTReader::ReadingKind::Read_Decl:
      case ASTReader::ReadingKind::Read_Type:
         return ast::SourceType(readExpr(), readType());
      case ASTReader::ReadingKind::Read_Stmt:
         return ast::SourceType(readSubExpr(), readType());
      }
   }

   /// Reads a declaration ID from the given position in this record.
   ///
   /// \returns The declaration ID read from the record, adjusted to a global
   /// ID.
   unsigned readDeclID() { return Reader->ReadDeclID(Record, Idx); }

   /// Reads a declaration from the given position in a record in the
   /// given module, advancing Idx.
   ast::Decl* readDecl() { return Reader->ReadDecl(Record, Idx); }

   /// Reads a declaration from the given position in the record,
   /// advancing Idx.
   ///
   /// \returns The declaration read from this location, casted to the given
   /// result type.
   template<typename T> T* readDeclAs()
   {
      return Reader->ReadDeclAs<T>(Record, Idx);
   }

   /// Read a reference to a module.
   Module* readModule();

   IdentifierInfo* getIdentifierInfo()
   {
      return Reader->GetIdentifierInfo(Record, Idx);
   }

   /// Read a declaration name, advancing Idx.
   DeclarationName readDeclarationName()
   {
      return Reader->ReadDeclarationName(Record, Idx);
   }

   /// Read a nested name specifier.
   NestedNameSpecifier* readNestedNameSpec()
   {
      return Reader->ReadNestedNameSpec(Record, Idx);
   }

   /// Read a template argument, advancing Idx.
   sema::TemplateArgument readTemplateArgument()
   {
      return Reader->ReadTemplateArgument(Record, Idx);
   }

   /// Read a template argument array, advancing Idx.
   sema::FinalTemplateArgumentList* readTemplateArgumentList()
   {
      return Reader->ReadTemplateArgumentList(Record, Idx);
   }

   /// Read a source location, advancing Idx.
   SourceLocation readSourceLocation()
   {
      return Reader->ReadSourceLocation(Record, Idx);
   }

   /// Read a source range, advancing Idx.
   SourceRange readSourceRange()
   {
      return Reader->ReadSourceRange(Record, Idx);
   }

   /// Read an integral value, advancing Idx.
   llvm::APInt readAPInt() { return Reader->ReadAPInt(Record, Idx); }

   /// Read a signed integral value, advancing Idx.
   llvm::APSInt readAPSInt() { return Reader->ReadAPSInt(Record, Idx); }

   /// Read a floating-point value, advancing Idx.
   llvm::APFloat readAPFloat(const llvm::fltSemantics& Sem)
   {
      return Reader->ReadAPFloat(Record, Sem, Idx);
   }

   /// Read a string, advancing Idx.
   std::string readString() { return Reader->ReadString(Record, Idx); }

   /// Reads attributes from the current stream position, advancing Idx.
   void readAttributes(SmallVectorImpl<Attr*>& Attrs,
                       bool* FoundInterestingAttr = nullptr)
   {
      return Reader->ReadAttributes(*this, Attrs, FoundInterestingAttr);
   }

   /// Reads a token out of a record, advancing Idx.
   lex::Token readToken() { return Reader->ReadToken(Record, Idx); }

   /// Reads an IL constant from the record.
   il::Constant* readILConstant();

   /// Reads a scope from the record.
   Scope* ReadScope();
};

/// Helper class that saves the current stream position and
/// then restores it when destroyed.
struct SavedStreamPosition {
   explicit SavedStreamPosition(llvm::BitstreamCursor& Cursor)
       : Cursor(Cursor), Offset(Cursor.GetCurrentBitNo())
   {
   }

   ~SavedStreamPosition() { Cursor.JumpToBit(Offset); }

private:
   llvm::BitstreamCursor& Cursor;
   uint64_t Offset;
};

} // namespace serial
} // namespace cdot

#endif // CDOT_ASTREADER_H
