//
// Created by Jonas Zell on 27.05.18.
//

#ifndef CDOT_ASTWRITER_H
#define CDOT_ASTWRITER_H

#include "BitCodes.h"
#include "AST/Decl.h"
#include "AST/Expression.h"
#include "AST/Type.h"

#include <llvm/Bitcode/BitstreamWriter.h>

#include <queue>

namespace cdot {
namespace lex {
   struct Token;
} // namespace lex

namespace il {
   class Constant;
} // namespace il

namespace serial {

class ASTRecordWriter;
class ModuleWriter;
class ILWriter;
class IncrementalCompilationManager;

class ASTWriter {
public:
   using RecordData     = llvm::SmallVector<uint64_t, 64>;
   using RecordDataImpl = llvm::SmallVectorImpl<uint64_t>;
   using RecordDataRef  = llvm::ArrayRef<uint64_t>;

   friend class ASTDeclWriter;
   friend class ASTRecordWriter;
   friend class ILWriter;
   friend class ModuleWriter;

private:
   /// The BitstreamWriter that is being written to
   llvm::BitstreamWriter &Stream;

   /// The module writer
   ModuleWriter &Writer;

   /// The writer for declarations, if we are writing a cache file.
   ASTWriter *DeclWriter = nullptr;

   /// The source ID of the cache file we're writing, if applicable.
   unsigned SourceID = unsigned(-1);

   /// Mapping from Decls to their assigned IDs
   llvm::DenseMap<const ast::Decl*, unsigned> DeclIDMap;

   /// Next decl ID to assign
   unsigned NextDeclID = 1;

   /// Offset of each declaration in the bitstream, indexed by
   /// the declaration's ID.
   std::vector<uint32_t> DeclOffsets;

   /// Mapping from types to their assigned IDs
   llvm::DenseMap<QualType, unsigned> TypeIDMap;

   /// Offset of each type in the bitstream, indexed by
   /// the type's ID.
   std::vector<uint32_t> TypeOffsets;

   /// Next type ID to assign
   unsigned NextTypeID = 1;

   /// Offsets of sub-stmts already serialized. The offset points
   /// just after the stmt record.
   llvm::DenseMap<ast::Statement*, uint64_t> SubStmtEntries;

   /// Written record declarations, used for emitting conformances.
   SmallPtrSet<ast::RecordDecl*, 16> EmittedRecordDecls;

   /// Map from records to their offset in the conformance blob.
   llvm::DenseMap<ast::RecordDecl*, unsigned> ConformanceOffsetMap;

   /// Abbreviation for lexical DeclContexts
   unsigned DeclContextLexicalAbbrev = 0;

   /// Abbreviation for DeclContext lookup tables
   unsigned DeclContextVisibleLookupAbbrev = 0;

   /// Abbreviation for decl offsets.
   unsigned DeclOffsetAbbrev = 0;

   /// Abbreviation for type offsets.
   unsigned TypeOffsetAbbrev = 0;

   /// Abbreviation for IL value offsets.
   unsigned ValueOffsetAbbrev = 0;

   /// Abbreviation for the conformance table.
   unsigned ConformanceTableAbbrev = 0;

   /// Abbreviation for the conformance data.
   unsigned ConformanceDataAbbrev = 0;

   /// Abbreviation for the instantiation table.
   unsigned InstantiationTableAbbrev = 0;

   /// Abbreviation for operator decls.
   unsigned OperatorPredecedenceAbbrev = 0;

   /// Abbreviation for the cache lookup table.
   unsigned CacheLookupAbbrev = 0;

   void WriteOffsetAbbrevs();

   void WriteTypeAbbrevs();
   void WriteType(QualType Ty);
   void WriteTypeOffsets(unsigned Offset = 0);

   unsigned IndexForID(unsigned ID) { return ID - 1; }

public:
   explicit ASTWriter(ModuleWriter &Writer);
   ASTWriter(ModuleWriter &Writer, ASTWriter &DeclWriter);

   /// Write the full AST of the given module.
   void WriteAST(ast::ModuleDecl *M);

   /// Write AST to a cache file for the given module and source ID.
   void WriteASTCache(ast::ModuleDecl *Mod);

   ModuleWriter &getWriter() const { return Writer; }
   unsigned getSourceID() const { return SourceID; }

   uint64_t WriteDeclContextLexicalBlock(ast::ASTContext &Context,
                                         ast::DeclContext *DC);
   uint64_t WriteDeclContextVisibleBlock(ast::ASTContext &Context,
                                         ast::DeclContext *DC);

   void WriteDeclOffsets(unsigned Offset = 0);

   /// Generate an on-disk hash table for the declarations in the given context.
   uint32_t GenerateNameLookupTable(ast::DeclContext *DC,
                                    llvm::SmallVectorImpl<char> &LookupTable);

   void WriteCacheLookupTable();

   void WriteDeclAbbrevs();
   void WriteConformanceAbbrevs();

   /// Write the conformance table to the module file.
   void WriteConformanceTable();

   /// Write the conformance data to the module file.
   void WriteConformanceData();

   /// Write the instantiation table to the module file.
   void WriteInstantiationTable();

   /// Write a declaration to the module file.
   void WriteDecl(ast::ASTContext &Context, ast::Decl *D);

   /// Emit a reference to a type.
   void AddTypeRef(QualType T, RecordDataImpl &Record);

   /// Force a type to be emitted and get its ID.
   unsigned GetOrCreateTypeID(QualType T);

   /// Determine the type ID of an already-emitted type.
   unsigned getTypeID(QualType T) const;

   /// Get the ID for a module.
   unsigned GetModuleID(Module *M);

   /// Write the table containing the modules operator and precedence
   /// group decls
   void WriteOperatorPrecedenceGroupRecords();

   void addOperatorsPrecedenceGroups(ast::ModuleDecl *M,
                                     SmallVectorImpl<uint32_t> &Vec);

   /// Emit a source location.
   void AddSourceLocation(SourceLocation Loc, RecordDataImpl &Record);

   /// Emit a source range.
   void AddSourceRange(SourceRange Range, RecordDataImpl &Record);

   /// Emit a reference to an identifier.
   void AddIdentifierRef(const IdentifierInfo *II, RecordDataImpl &Record);

   /// Get the unique number used to refer to the given identifier.
   unsigned getIdentifierRef(const IdentifierInfo *II);

   /// Emit a reference to a declaration.
   void AddDeclRef(const ast::Decl *D, RecordDataImpl &Record);

   /// Force a declaration to be emitted and get its ID.
   unsigned GetDeclRef(const ast::Decl *D);

   /// Determine the declaration ID of an already-emitted
   /// declaration.
   unsigned getDeclID(const ast::Decl *D);

   /// Add a string to the given record.
   void AddString(llvm::StringRef Str, RecordDataImpl &Record);

   /// Write the given subexpression to the bitstream.
   void WriteSubStmt(ast::Statement *S);

   /// Write the given declaration name
   void AddDeclarationName(DeclarationName Name, ASTRecordWriter &Record);
};

/// An object for streaming information to a record.
class ASTRecordWriter {
   ASTWriter *Writer;
   ASTWriter::RecordDataImpl *Record;

   /// Statements that we've encountered while serializing a
   /// declaration or type.
   llvm::SmallVector<ast::Statement*, 16> StmtsToEmit;

   /// Indices of record elements that describe offsets within the
   /// bitcode. These will be converted to offsets relative to the current
   /// record when emitted.
   llvm::SmallVector<unsigned, 8> OffsetIndices;

   /// Flush all of the statements and expressions that have
   /// been added to the queue via AddStmt().
   void FlushStmts();
   void FlushSubStmts();

   void PrepareToEmit(uint64_t MyOffset)
   {
      // Convert offsets into relative form.
      for (unsigned I : OffsetIndices) {
         auto &StoredOffset = (*Record)[I];
         assert(StoredOffset < MyOffset && "invalid offset");
         if (StoredOffset)
            StoredOffset = MyOffset - StoredOffset;
      }

      OffsetIndices.clear();
   }

public:
   /// Construct a ASTRecordWriter that uses the default encoding scheme.
   ASTRecordWriter(ASTWriter &Writer, ASTWriter::RecordDataImpl &Record)
      : Writer(&Writer), Record(&Record)
   {}

   /// Construct a ASTRecordWriter that uses the same encoding scheme as another
   /// ASTRecordWriter.
   ASTRecordWriter(ASTRecordWriter &Parent, ASTWriter::RecordDataImpl &Record)
      : Writer(Parent.Writer), Record(&Record)
   {}

   /// Copying an ASTRecordWriter is almost certainly a bug.
   ASTRecordWriter(const ASTRecordWriter &) = delete;
   ASTRecordWriter &operator=(const ASTRecordWriter &) = delete;

   /// Extract the underlying record storage.
   ASTWriter::RecordDataImpl &getRecordData() const { return *Record; }

   /// Minimal vector-like interface.
   /// @{
   void push_back(uint64_t N) { Record->push_back(N); }

   template<typename InputIterator>
   void append(InputIterator begin, InputIterator end) {
      Record->append(begin, end);
   }
   bool empty() const { return Record->empty(); }
   size_t size() const { return Record->size(); }
   uint64_t &operator[](size_t N) { return (*Record)[N]; }
   void clear() { Record->clear(); }
   /// @}

   /// Emit the record to the stream, followed by its substatements, and
   /// return its offset.
   // FIXME: Allow record producers to suggest Abbrevs.
   uint64_t Emit(unsigned Code, unsigned Abbrev = 0)
   {
      uint64_t Offset = Writer->Stream.GetCurrentBitNo();
      PrepareToEmit(Offset);
      Writer->Stream.EmitRecord(Code, *Record, Abbrev);
      FlushStmts();
      return Offset;
   }

   /// Emit the record to the stream, preceded by its substatements.
   uint64_t EmitStmt(unsigned Code, unsigned Abbrev = 0)
   {
      uint64_t Offset = Writer->Stream.GetCurrentBitNo();
      FlushSubStmts();
      PrepareToEmit(Offset);
      Writer->Stream.EmitRecord(Code, *Record, Abbrev);

      return Offset;
   }

   /// Add a bit offset into the record. This will be converted into an
   /// offset relative to the current record when emitted.
   void AddOffset(uint64_t BitOffset)
   {
      OffsetIndices.push_back(Record->size());
      Record->push_back(BitOffset);
   }

   /// Add the given statement or expression to the queue of
   /// statements to emit.
   ///
   /// This routine should be used when emitting types and declarations
   /// that have expressions as part of their formulation. Once the
   /// type or declaration has been written, Emit() will write
   /// the corresponding statements just after the record.
   void AddStmt(ast::Statement *S)
   {
      StmtsToEmit.push_back(S);
   }

   /// Emit a source location.
   void AddSourceLocation(SourceLocation Loc)
   {
      return Writer->AddSourceLocation(Loc, *Record);
   }

   /// Emit a source range.
   void AddSourceRange(SourceRange Range)
   {
      return Writer->AddSourceRange(Range, *Record);
   }

   /// Emit an integral value.
   void AddAPInt(const llvm::APInt &Value);

   /// Emit a signed integral value.
   void AddAPSInt(const llvm::APSInt &Value);

   /// Emit a floating-point value.
   void AddAPFloat(const llvm::APFloat &Value);

   /// Emit a reference to an identifier.
   void AddIdentifierRef(const IdentifierInfo *II)
   {
      return Writer->AddIdentifierRef(II, *Record);
   }

   /// Emit a reference to a type.
   void AddTypeRef(QualType T)
   {
      return Writer->AddTypeRef(T, *Record);
   }

   void AddTypeRef(ast::SourceType T);

   /// Emit a reference to a declaration.
   void AddDeclRef(const ast::Decl *D)
   {
      return Writer->AddDeclRef(D, *Record);
   }

   /// Emit a Token
   void AddToken(const lex::Token &Tok);

   /// Emit a declaration name.
   void AddDeclarationName(DeclarationName Name)
   {
      return Writer->AddDeclarationName(Name, *this);
   }

   /// Emit a nested name specifier.
   void AddNestedNameSpec(NestedNameSpecifier *Name);

   /// Emit a nested name specifier.
   void AddNestedNameSpecWithLoc(NestedNameSpecifierWithLoc *Name);

   /// Emit a template argument.
   void AddTemplateArgument(const sema::ResolvedTemplateArg &Arg);

   /// Emit a template parameter list.
   void AddTemplateParameterList(
      llvm::ArrayRef<ast::TemplateParamDecl*> TemplateParams);

   /// Emit a template argument list.
   void AddTemplateArgumentList(
      const sema::FinalTemplateArgumentList &TemplateArgs);

   /// Emit a string.
   void AddString(llvm::StringRef Str)
   {
      return Writer->AddString(Str, *Record);
   }

   /// Emit a list of attributes.
   void AddAttributes(llvm::ArrayRef<const Attr*> Attrs);

   /// Emit a reference to an IL constant.
   void AddILConstant(il::Constant *C);

   /// Emit a reference to a module.
   void AddModuleRef(Module *M);
};

} // namespace serial
} // namespace cdot

#endif //CDOT_ASTWRITER_H
