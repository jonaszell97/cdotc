#ifndef CDOT_ILWRITER_H
#define CDOT_ILWRITER_H

#include "cdotc/AST/Type.h"
#include "cdotc/Basic/DeclarationName.h"
#include "cdotc/Lex/SourceLocation.h"
#include "cdotc/Support/LLVM.h"

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/DenseSet.h>
#include <llvm/ADT/SmallPtrSet.h>
#include <llvm/Bitcode/BitstreamWriter.h>

#include <queue>

namespace cdot {
namespace il {
class Argument;
class BasicBlock;
class Constant;
class Context;
class Function;
class GlobalObject;
class GlobalVariable;
class Instruction;
class Module;
class Value;
} // namespace il

namespace ast {
class Decl;
class NamedDecl;
class ILGenPass;
} // namespace ast

namespace serial {

class ASTWriter;
class ILRecordWriter;
class ModuleWriter;

class ILWriter {
public:
   using RecordData = SmallVector<uint64_t, 64>;
   using RecordDataImpl = SmallVectorImpl<uint64_t>;
   using RecordDataRef = ArrayRef<uint64_t>;

   using ValueID = uint32_t;

   friend class ModuleWriter;

private:
   /// The AST Writer object
   ASTWriter& Writer;

   /// The BitstreamWriter that is being written to
   llvm::BitstreamWriter& Stream;

   /// The IL Module we're currently writing
   const il::Module* ILMod = nullptr;

   /// The source ID of the cache file we're writing, if applicable.
   unsigned SourceID = unsigned(-1);

   /// Set of SourceLocations that belong to the cache file being written.
   llvm::DenseSet<unsigned> FileLocations;

   /// The IL Context object
   il::Context& ILCtx;

   /// The ILGen object.
   ast::ILGenPass& ILGen;

   /// Mapping from values to their assigned IDs
   llvm::DenseMap<const il::Value*, ValueID> ValueIDMap;

   /// Set of functions whose body we need to emit.
   llvm::DenseSet<const il::Value*> ExternallyVisibleFunctions;

   /// Next value ID to assign
   unsigned NextValueID = 1;

   /// Offsets of emitted values, indexed by ID
   std::vector<uint32_t> ValueOffsets;

   /// Values to be included in the symbol table
   llvm::SmallVector<const il::GlobalObject*, 32> GlobalValues;

   /// Number of global variables emitted to the module file
   unsigned NumGlobalVariables = 0;

   /// Number of functions emitted to the module file
   unsigned NumFunctions = 0;

   /// Number of instructions emitted to the module file
   unsigned NumInstructions = 0;

   /// Number of constants emitted to the module file
   unsigned NumConstants = 0;

   /// Number of externally visible symbols
   unsigned NumVisibleSymbols = 0;

public:
   ILWriter(ASTWriter& Writer, llvm::BitstreamWriter& Stream,
            il::Context& ILCtx);

   ~ILWriter();

   void writeModule(const il::Module& M);
   void writeModuleCache(const il::Module& M);
   void writeSymbolTableBlock();

   void WriteValueOffsets(unsigned Offset = 0);

   friend class ILRecordWriter;

private:
   void writeMetadataBlock();
   void writeValueBlock();

   void writeGlobalVariable(const il::GlobalVariable& G);
   void writeConstant(const il::Constant& C);

   void writeFunction(const il::Function& F);
   void writeBasicBlock(const il::BasicBlock& B);
   void writeArgument(const il::Argument& A);
   void writeInstruction(const il::Instruction& I);

   /// Emit a reference to a value.
   void AddValueRef(const il::Value* V, RecordDataImpl& Record);

public:
   /// Get or add a new value to be emitted
   unsigned GetOrCreateValueID(const il::Value* V);

   unsigned int getNumVisibleSymbols() const { return NumVisibleSymbols; }

   /// Write a value to the stream.
   void writeValue(il::Value* V);

   /// Add a declaration to be written in this file.
   void AddDeclToBeWritten(const ast::NamedDecl* D, unsigned ID);

   /// Mark a function as needing to be written.
   void AddExternallyVisibleValue(il::Value* Val);

private:
   /// Emit a reference to a type.
   void AddTypeRef(QualType T, RecordDataImpl& Record);

   /// Force a type to be emitted and get its ID.
   unsigned GetOrCreateTypeID(QualType T);

   /// Emit a reference to a declaration.
   void AddDeclRef(const ast::Decl* D, RecordDataImpl& Record);

   /// Emit a source location.
   void AddSourceLocation(SourceLocation Loc, RecordDataImpl& Record);

   /// Add a string to the given record.
   void AddString(llvm::StringRef Str, RecordDataImpl& Record);
};

/// An object for streaming information to a record.
class ILRecordWriter {
   ILWriter* Writer;
   ILWriter::RecordDataImpl* Record;

public:
   /// Construct a ILRecordWriter that uses the default encoding scheme.
   ILRecordWriter(ILWriter& Writer, ILWriter::RecordDataImpl& Record)
       : Writer(&Writer), Record(&Record)
   {
   }

   /// Copying an ILRecordWriter is almost certainly a bug.
   ILRecordWriter(const ILRecordWriter&) = delete;
   ILRecordWriter& operator=(const ILRecordWriter&) = delete;

   /// Extract the underlying record storage.
   ILWriter::RecordDataImpl& getRecordData() const { return *Record; }

   /// Minimal vector-like interface.
   /// @{
   void push_back(uint64_t N) { Record->push_back(N); }

   template<typename InputIterator>
   void append(InputIterator begin, InputIterator end)
   {
      Record->append(begin, end);
   }
   bool empty() const { return Record->empty(); }
   size_t size() const { return Record->size(); }
   uint64_t& operator[](size_t N) { return (*Record)[N]; }
   /// @}

   /// Emit the record to the stream, followed by its substatements, and
   /// return its offset.
   uint64_t Emit(unsigned Code, unsigned Abbrev = 0)
   {
      uint64_t Offset = Writer->Stream.GetCurrentBitNo();
      Writer->Stream.EmitRecord(Code, *Record, Abbrev);
      return Offset;
   }

   /// Emit a value reference
   void AddValue(const il::Value* V) { return Writer->AddValueRef(V, *Record); }

   /// Emit a source location.
   void AddSourceLocation(SourceLocation Loc)
   {
      return Writer->AddSourceLocation(Loc, *Record);
   }

   /// Emit an integral value.
   void AddAPInt(const llvm::APInt& Value);

   /// Emit a signed integral value.
   void AddAPSInt(const llvm::APSInt& Value);

   /// Emit a floating-point value.
   void AddAPFloat(const llvm::APFloat& Value);

   /// Emit a reference to an identifier.
   void AddIdentifierRef(const IdentifierInfo* II);

   /// Emit a reference to a type.
   void AddTypeRef(QualType T) { return Writer->AddTypeRef(T, *Record); }

   /// Emit a reference to a declaration.
   void AddDeclRef(const ast::Decl* D)
   {
      return Writer->AddDeclRef(D, *Record);
   }

   /// Emit a declaration name.
   void AddDeclarationName(DeclarationName Name);

   /// Emit a string.
   void AddString(llvm::StringRef Str)
   {
      return Writer->AddString(Str, *Record);
   }
};

} // namespace serial
} // namespace cdot

#endif // CDOT_ILWRITER_H
