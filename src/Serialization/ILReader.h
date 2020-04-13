#ifndef CDOT_ILREADER_H
#define CDOT_ILREADER_H

#include "AST/Type.h"
#include "ASTReaderInternals.h"
#include "Basic/DeclarationName.h"
#include "IL/ILBuilder.h"
#include "Lex/SourceLocation.h"
#include "Support/LLVM.h"

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/APFloat.h>
#include <llvm/ADT/APSInt.h>
#include <llvm/Bitcode/BitstreamReader.h>
#include <llvm/Support/OnDiskHashTable.h>

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
   class ILGenPass;
   class SemaPass;
} // namespace ast

namespace serial {

class ASTReader;
class ILRecordReader;
class ILSymbolTableLookupTrait;
class ModuleReader;

class ILReader {
public:
   using RecordData     = SmallVector<uint64_t, 64>;
   using RecordDataImpl = SmallVectorImpl<uint64_t>;
   using HashTable      =
      llvm::OnDiskIterableChainedHashTable<ILSymbolTableLookupTrait>;

   friend class ILRecordReader;
   friend class ModuleReader;

private:
   /// The current module reader
   ASTReader &Reader;

   /// Cursor into the bitstream
   llvm::BitstreamCursor Stream;

   /// Offsets of values, indexed by ID
   const uint32_t *ValueOffsets = nullptr;

   /// The IL Context of the current compilation
   il::Context &ILCtx;

   /// The module we are adding read IL symbols to
   il::Module *ILMod = nullptr;

   /// The ILGen object.
   ast::ILGenPass &ILGen;

   /// The IL Builder used to create values
   il::ILBuilder Builder;

   /// The function currently being read
   il::Function *CurrentFn = nullptr;

   /// The symbol table of the module
   HashTable *SymTab = nullptr;

   struct UnfinishedBasicBlock {
      UnfinishedBasicBlock(il::BasicBlock &B,
                           SmallVector<unsigned, 4> &&ArgIDs,
                           SmallVector<unsigned, 4> &&InstIDs)
         : B(B), ArgIDs(std::move(ArgIDs)), InstIDs(std::move(InstIDs))
      { }

      il::BasicBlock &B;
      SmallVector<unsigned, 4> ArgIDs;
      SmallVector<unsigned, 4> InstIDs;
   };

   /// Basic blocks that are not done deserializing.
   SmallVector<UnfinishedBasicBlock, 0> UnfinishedBBs;

   /// Values that have already been loaded from the chain.
   ///
   /// When the pointer at index I is non-NULL, the value with
   /// ID = (I + 1) has already been loaded
   std::vector<il::Value*> ValuesLoaded;
   unsigned LocalNumDecls = 0;
   unsigned BaseValueIndex = 0;

   /// A vector to add read globals to, if not null.
   SmallVectorImpl<il::GlobalObject*> *ReadGlobals = nullptr;

   /* Statistics */

   /// Number of global variables read from the module file
   unsigned NumGlobalVariables = 0;

   /// Number of functions read from the module file
   unsigned NumFunctions = 0;

   /// Number of instructions read from the module file
   unsigned NumInstructions = 0;

   /// Number of constants read from the module file
   unsigned NumConstants = 0;

   void ReadValueRecord(unsigned ID);

   unsigned getTotalNumValues() { return (unsigned)ValuesLoaded.size(); }

   il::GlobalVariable *readGlobalVariable(ILRecordReader &Record);
   il::Constant *readConstant(ILRecordReader &Record, unsigned Code);

   il::Function *readFunction(ILRecordReader &Record, unsigned Code);
   il::BasicBlock *readBasicBlock(ILRecordReader &Record, unsigned ID);
   il::Argument *readArgument(ILRecordReader &Record);
   il::Instruction *readInstruction(ILRecordReader &Record, unsigned Code);

   il::BasicBlock *readFunctionEntryBlock(unsigned ID,
                                          SmallVectorImpl<unsigned> &InstIDs);

   void finalizeBasicBlocks();

   struct FunctionRAII {
      FunctionRAII(ILReader &Reader, il::Function *F)
         : Reader(Reader), F(Reader.CurrentFn)
      {
         Reader.CurrentFn = F;
      }

      ~FunctionRAII()
      {
         Reader.CurrentFn = F;
      }

   private:
      ILReader &Reader;
      il::Function *F;
   };

   ReadResult readValueBlock();
   ReadResult readValueSymTab(uint32_t TblOffset, llvm::StringRef Blob);

public:
   ILReader(ast::SemaPass &Sema, ast::ASTContext &Context, ASTReader &Reader,
            const llvm::BitstreamCursor &Stream, il::Context &ILCtx);

   ~ILReader();

   /// Lazily read a function body.
   void readFunctionBody(il::Function &F, llvm::ArrayRef<unsigned> BlockIDs,
                         llvm::ArrayRef<unsigned> EntryInstIDs,
                         unsigned Linkage);

   /// Lazily read a global definition.
   void readGlobalInitializer(il::GlobalVariable &G, unsigned InitID,
                              unsigned Linkage);

   /// Lazily read a function or global variable.
   void readLazyGlobal(il::GlobalObject &GO);

   HashTable *getSymbolTable() const { return SymTab; }

   il::Module *getModule() { return ILMod; }
   void setModule(il::Module *M);

   /// Main entry point to deserialize the IL module
   ReadResult ReadILModule(llvm::BitstreamCursor &Stream);

   /// Main entry point to deserialize the IL module, eagerly deserializing
   /// globals and functions
   ReadResult ReadILModuleEager();

   /// Read a type from the current position in the given record, which
   /// was read from the given AST file.
   QualType readType(const RecordData &Record, unsigned &Idx);

   /// Reads a declaration from the given position in a record in the
   /// given module.
   ast::Decl *ReadDecl(const RecordData &R, unsigned &I);

   /// Reads a value at the given position
   il::Value *readValue(const RecordData &R, unsigned &Idx);

   unsigned ReadValueID(const RecordData &R, unsigned &Idx)
   {
      return (unsigned)R[Idx++];
   }

   il::Value *GetValue(unsigned ID);
   il::GlobalObject *GetGlobalObject(StringRef Name);

   /// Reads an identifier at the given position
   IdentifierInfo *GetIdentifierInfo(const RecordData &Record,
                                     unsigned &Idx);

   /// Read a source location
   SourceLocation ReadSourceLocation(const RecordDataImpl &Record,
                                     unsigned &Idx);

   /// Read an integral value
   llvm::APInt ReadAPInt(const RecordData &Record, unsigned &Idx);

   /// Read a signed integral value
   llvm::APSInt ReadAPSInt(const RecordData &Record, unsigned &Idx);

   /// Read a floating-point value
   llvm::APFloat ReadAPFloat(const RecordData &Record,
                             const llvm::fltSemantics &Sem, unsigned &Idx);

   // Read a string
   std::string ReadString(const RecordData &Record, unsigned &Idx);

   // Skip a string
   static void SkipString(const RecordData &Record, unsigned &Idx)
   {
      Idx += Record[Idx] + 1;
   }

   struct EagerRAII {
      EagerRAII(ILReader &R, SmallVectorImpl<il::GlobalObject*> &ReadGlobals)
         : R(R), Previous(R.ReadGlobals)
      {
         R.ReadGlobals = &ReadGlobals;
      }

      ~EagerRAII()
      {
         R.ReadGlobals = Previous;
      }

   private:
      ILReader &R;
      SmallVectorImpl<il::GlobalObject*> *Previous;
   };
};

/// An object for streaming information from a record.
class ILRecordReader {
   ILReader *Reader;
   unsigned Idx = 0;
   ILReader::RecordData Record;

   using RecordData     = ILReader::RecordData;
   using RecordDataImpl = ILReader::RecordDataImpl;

public:
   /// Construct an ASTRecordReader that uses the default encoding scheme.
   explicit ILRecordReader(ILReader &Reader) : Reader(&Reader) {}

   /// Reads a record with id AbbrevID from Cursor, resetting the
   /// internal state.
   unsigned readRecord(llvm::BitstreamCursor &Cursor, unsigned AbbrevID);

   /// The current position in this record.
   unsigned getIdx() const { return Idx; }

   /// The length of this record.
   size_t size() const { return Record.size(); }

   /// An arbitrary index in this record.
   const uint64_t &operator[](size_t N) { return Record[N]; }

   /// The last element in this record.
   const uint64_t &back() const { return Record.back(); }

   /// Returns the current value in this record, and advances to the
   /// next value.
   const uint64_t &readInt() { return Record[Idx++]; }

   /// Reads a bool and advances to the next value
   bool readBool() { return static_cast<bool>(Record[Idx++]); }

   /// Returns the current value in this record, without advancing.
   const uint64_t &peekInt() { return Record[Idx]; }

   /// Skips the specified number of values.
   void skipInts(unsigned N) { Idx += N; }

   template<class EnumTy>
   EnumTy readEnum()
   {
      return static_cast<EnumTy>(Record[Idx++]);
   }

   /// Reads a value
   il::Value *readValue()
   {
      return Reader->readValue(Record, Idx);
   }

   unsigned readValueID()
   {
      return Reader->ReadValueID(Record, Idx);
   }

   /// Reads a value as a specific type
   template<class T>
   T *readValueAs()
   {
      return support::cast_or_null<T>(Reader->readValue(Record, Idx));
   }

   /// Read a type from the current position in the record.
   QualType readType()
   {
      return Reader->readType(Record, Idx);
   }

   /// Reads a declaration from the given position in a record in the
   /// given module, advancing Idx.
   ast::Decl *readDecl()
   {
      return Reader->ReadDecl(Record, Idx);
   }

   /// Reads a declaration from the given position in the record,
   /// advancing Idx.
   ///
   /// \returns The declaration read from this location, casted to the given
   /// result type.
   template<typename T>
   T *readDeclAs()
   {
      return support::cast_or_null<T>(Reader->ReadDecl(Record, Idx));
   }

   IdentifierInfo *getIdentifierInfo()
   {
      return Reader->GetIdentifierInfo(Record, Idx);
   }

   /// Read a declaration name, advancing Idx.
   DeclarationName readDeclarationName();

   /// Read a source location, advancing Idx.
   SourceLocation readSourceLocation()
   {
      return Reader->ReadSourceLocation(Record, Idx);
   }

   /// Read an integral value, advancing Idx.
   llvm::APInt readAPInt()
   {
      return Reader->ReadAPInt(Record, Idx);
   }

   /// Read a signed integral value, advancing Idx.
   llvm::APSInt readAPSInt()
   {
      return Reader->ReadAPSInt(Record, Idx);
   }

   /// Read a floating-point value, advancing Idx.
   llvm::APFloat readAPFloat(const llvm::fltSemantics &Sem)
   {
      return Reader->ReadAPFloat(Record, Sem, Idx);
   }

   /// Read a string, advancing Idx.
   std::string readString()
   {
      return Reader->ReadString(Record, Idx);
   }
};

class ILSymbolTableLookupTrait {
   ILReader &Reader;

public:
   using external_key_type = llvm::StringRef;
   using internal_key_type = llvm::StringRef;

   using data_type         = il::Value*;
   using data_type_ref     = data_type;

   using hash_value_type   = unsigned;
   using offset_type       = unsigned;

   ILSymbolTableLookupTrait(ILReader &Reader)
      : Reader(Reader)
   { }

   static bool EqualKey(const internal_key_type& a, const internal_key_type& b)
   {
      return a == b;
   }

   static internal_key_type GetInternalKey(const external_key_type &Name)
   {
      return Name;
   }

   static hash_value_type ComputeHash(const internal_key_type& a);

   static
   std::pair<unsigned, unsigned> ReadKeyDataLength(const unsigned char*& d)
   {
      using namespace llvm::support;
      auto KeyLen = endian::readNext<uint32_t, little, unaligned>(d);
      auto DataLen = endian::readNext<uint32_t, little, unaligned>(d);

      return { KeyLen, DataLen };
   }

   static internal_key_type ReadKey(const unsigned char* d, unsigned KeyLen)
   {
      return llvm::StringRef(reinterpret_cast<const char*>(d), KeyLen);
   }

   data_type ReadData(const internal_key_type&,
                      const unsigned char* d,
                      unsigned) {
      using namespace llvm::support;
      auto ID = endian::readNext<uint32_t, little, unaligned>(d);

      return Reader.GetValue(ID);
   }
};

} // namespace serial
} // namespace cdot

#endif //CDOT_ILREADER_H
