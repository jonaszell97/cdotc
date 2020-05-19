#ifndef CDOT_ASTREADERINTERNALS_H
#define CDOT_ASTREADERINTERNALS_H

#include "cdotc/Serialization/ASTCommon.h"

#include <llvm/ADT/DenseSet.h>
#include <llvm/ADT/SmallVector.h>
#include <llvm/Support/OnDiskHashTable.h>

#ifndef NDEBUG
#define WRITE_SYNC_POINT Record.push_back(0x901abf218);
#define READ_SYNC_POINT assert(Record.readInt() == 0x901abf218);
#else
#define WRITE_SYNC_POINT
#define READ_SYNC_POINT
#endif

namespace cdot {
namespace serial {

class ASTReader;
class ModuleReader;

/// The result of reading the control block of an AST file, which
/// can fail for various reasons.
enum ReadResult : unsigned char {
   /// The control block was read successfully. Aside from failures,
   /// the AST file is safe to read into the current context.
   Success,

   /// The AST file itself appears corrupted.
   Failure,

   /// The AST file was missing.
   Missing,

   /// The AST file is out-of-date relative to its input files,
   /// and needs to be regenerated.
   OutOfDate,

   /// The AST file was written by a different version of Clang.
   VersionMismatch,

   /// The AST file was writtten with a different language/target
   /// configuration.
   ConfigurationMismatch,

   /// The AST file has errors.
   HadErrors
};

namespace reader {

struct DeclContextLookupTable;

/// Class that performs name lookup into a DeclContext stored
/// in an AST file.
class ASTDeclContextNameLookupTrait {
   ASTReader& Reader;

public:
   // Maximum number of lookup tables we allow before condensing the tables.
   static const int MaxTables = 4;

   /// The lookup result is a list of global declaration IDs.
   using data_type = llvm::SmallVector<unsigned, 4>;

   struct data_type_builder {
      data_type& Data;
      llvm::DenseSet<unsigned> Found;

      data_type_builder(data_type& D) : Data(D) {}

      void insert(unsigned ID)
      {
         // Just use a linear scan unless we have more than a few IDs.
         if (Found.empty() && !Data.empty()) {
            if (Data.size() <= 4) {
               for (auto I : Found)
                  if (I == ID)
                     return;
               Data.push_back(ID);
               return;
            }

            // Switch to tracking found IDs in the set.
            Found.insert(Data.begin(), Data.end());
         }

         if (Found.insert(ID).second)
            Data.push_back(ID);
      }
   };

   using hash_value_type = unsigned;
   using offset_type = unsigned;

   using external_key_type = DeclarationName;
   using internal_key_type = DeclarationName;

   explicit ASTDeclContextNameLookupTrait(ASTReader& Reader) : Reader(Reader) {}

   static bool EqualKey(const internal_key_type& a, const internal_key_type& b)
   {
      return a == b;
   }

   static hash_value_type ComputeHash(const internal_key_type& Key)
   {
      return serial::ComputeHash(Key);
   }

   static internal_key_type GetInternalKey(const external_key_type& Name)
   {
      return Name;
   }

   static external_key_type GetExternalKey(const internal_key_type& Name)
   {
      return Name;
   }

   static std::pair<unsigned, unsigned>
   ReadKeyDataLength(const unsigned char*& d);

   internal_key_type ReadKey(const unsigned char* d, unsigned);

   void ReadDataInto(internal_key_type, const unsigned char* d,
                     unsigned DataLen, data_type_builder& Val);

   data_type ReadData(internal_key_type Key, const unsigned char* d,
                      unsigned DataLen)
   {
      data_type data;
      data_type_builder builder(data);
      ReadDataInto(Key, d, DataLen, builder);

      return data;
   }

   static void MergeDataInto(const data_type& From, data_type_builder& To)
   {
      To.Data.reserve(To.Data.size() + From.size());
      for (unsigned ID : From)
         To.insert(ID);
   }
};

struct DeclContextLookupTable {
   llvm::OnDiskChainedHashTable<ASTDeclContextNameLookupTrait>* Table = nullptr;
};

using HashTable
    = llvm::OnDiskIterableChainedHashTable<ASTDeclContextNameLookupTrait>;

class ASTIdentifierLookupTraitBase {
public:
   using external_key_type = llvm::StringRef;
   using internal_key_type = llvm::StringRef;

   using hash_value_type = unsigned;
   using offset_type = unsigned;

   static bool EqualKey(const internal_key_type& a, const internal_key_type& b)
   {
      return a == b;
   }

   static hash_value_type ComputeHash(const internal_key_type& a);

   static std::pair<unsigned, unsigned>
   ReadKeyDataLength(const unsigned char*& d);

   static internal_key_type ReadKey(const unsigned char* d, unsigned n);
};

/// Class that performs lookup for an identifier stored in an AST file.
class ASTIdentifierLookupTrait : public ASTIdentifierLookupTraitBase {
   ModuleReader& Reader;

   // If we know the IdentifierInfo in advance, it is here and we will
   // not build a new one. Used when deserializing information about an
   // identifier that was constructed before the AST file was read.
   IdentifierInfo* KnownII;

public:
   using data_type = IdentifierInfo*;

   ASTIdentifierLookupTrait(ModuleReader& Reader, IdentifierInfo* II = nullptr)
       : Reader(Reader), KnownII(II)
   {
   }

   data_type ReadData(const internal_key_type& k, const unsigned char* d,
                      unsigned DataLen);

   unsigned ReadIdentifierID(const unsigned char* d);
   ModuleReader& getReader() const { return Reader; }
};

/// The on-disk hash table used to contain information about
/// all of the identifiers in the program.
using ASTIdentifierLookupTable
    = llvm::OnDiskIterableChainedHashTable<ASTIdentifierLookupTrait>;

class ConformanceLookupTrait {
   ASTReader& Reader;

public:
   using external_key_type = uint32_t;
   using internal_key_type = uint32_t;

   /// Doesn't actually return anything.
   using data_type = void;

   using hash_value_type = uint32_t;
   using offset_type = uint32_t;

   explicit ConformanceLookupTrait(ASTReader& Reader) : Reader(Reader) {}

   static bool EqualKey(const internal_key_type& a, const internal_key_type& b)
   {
      return a == b;
   }

   static internal_key_type GetInternalKey(const external_key_type& Key)
   {
      return Key;
   }

   static external_key_type GetExternalKey(const internal_key_type& Key)
   {
      return Key;
   }

   static hash_value_type ComputeHash(const internal_key_type& a) { return a; }

   static std::pair<unsigned, unsigned>
   ReadKeyDataLength(const unsigned char*& d)
   {
      return {4, 4};
   }

   static internal_key_type ReadKey(const unsigned char* d, unsigned n);

   data_type ReadData(const internal_key_type& k, const unsigned char* d,
                      unsigned DataLen);
};

class InstantiationTableLookupTrait {
   ASTReader& Reader;

public:
   using external_key_type = StringRef;
   using internal_key_type = StringRef;

   using data_type = ast::NamedDecl*;

   using hash_value_type = unsigned;
   using offset_type = unsigned;

   explicit InstantiationTableLookupTrait(ASTReader& Reader) : Reader(Reader) {}

   static bool EqualKey(const internal_key_type& a, const internal_key_type& b)
   {
      return a == b;
   }

   static internal_key_type GetInternalKey(const external_key_type& Key)
   {
      return Key;
   }

   static external_key_type GetExternalKey(const internal_key_type& Key)
   {
      return Key;
   }

   static hash_value_type ComputeHash(const internal_key_type& a);

   static std::pair<unsigned, unsigned>
   ReadKeyDataLength(const unsigned char*& d);

   static internal_key_type ReadKey(const unsigned char* d, unsigned n);

   data_type ReadData(const internal_key_type& k, const unsigned char* d,
                      unsigned DataLen);
};

} // namespace reader
} // namespace serial
} // namespace cdot

#endif // CDOT_ASTREADERINTERNALS_H
