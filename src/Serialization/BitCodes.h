#ifndef CDOT_ASTBITCODES_H
#define CDOT_ASTBITCODES_H

#include "AST/Decl.h"
#include "AST/Statement.h"

#include <llvm/Bitcode/BitCodes.h>

namespace cdot {
namespace serial {

/// Describes the various kinds of blocks that occur within
/// an module file.
enum BlockIDs {
   /// The control block, which contains all of the
   /// information that needs to be validated prior to committing
   /// to loading the module file.
   CONTROL_BLOCK_ID = llvm::bitc::FIRST_APPLICATION_BLOCKID,

      /// The block of configuration options, used to check that
      /// a module is being used in a configuration compatible with the
      /// configuration in which it was built.
      ///
      /// This block is part of the control block.
      OPTIONS_BLOCK_ID,

      /// Block containing information about a (sub-)module.
      MODULE_BLOCK_ID,

   /// The block containing information about the file manager of the module.
   FILE_MANAGER_BLOCK_ID,

   /// Control block of a cache file.
   CACHE_CONTROL_BLOCK_ID,

      /// Block containing the declarations of a cache file.
      CACHE_FILE_BLOCK_ID,

   /// The AST block, which acts as a container around the
   /// full AST block.
   AST_BLOCK_ID,

   /// The block containing the definitions of all of the
   /// types and decls used within the module file.
   DECL_TYPES_BLOCK_ID,

   /// The block containing the type, value and decl offsets
   OFFSET_BLOCK_ID,

   /// The block containing offset ranges for a cache file.
   OFFSET_RANGE_BLOCK_ID,

   /// The block containing the modules conformances,
   CONFORMANCE_BLOCK_ID,

   /// The block containing the serialized IL for the module.
   IL_MODULE_BLOCK_ID,

   /// The block containg all identifiers used in the AST and IL
   IDENTIFIER_BLOCK_ID,

   /// The binary data of the static library for this
   /// module.
   STATIC_LIB_BLOCK_ID,
};

/// Record types that appear in the static library block
enum StaticLibRecordTypes {
   /// The binary data of the static library for this
   /// module.
   STATIC_LIB_DATA = 0,
};

/// Record types that occur within the control block.
enum ControlRecordTypes {
   /// Information about a cached file.
   CACHE_FILE = 0,

   /// Information about file dependencies.
   DEPENDENCY_DATA = 1,
};

/// Record types that occur within the file manager block.
enum FileManagerRecordTypes {
   /// The macro expansion locations of the file manager.
   MACRO_EXPANSIONS = 0,

   /// Information about the source files used to create the module.
   SOURCE_FILES,
};

/// Record types that occur within the module block.
enum ModuleBlockRecordTypes {
   /// module file metadata, including the module file version number
   /// and information about the compiler used to build this module file.
   METADATA = 0,

   /// Record code for the list of other module files imported by
   /// this module file.
   IMPORTS,

   /// Implicitly imported modules.
   IMPLICIT_IMPORTS,

   /// Files used to create this module.
   INPUT_FILES,

   /// Record code for the module name.
   MODULE_NAME,

   /// Record code for the module build directory.
   MODULE_DIRECTORY,

   /// The module's AST decl.
   MODULE_DECL,
};

/// Record types that occur within the options block inside
/// the control block.
enum OptionsRecordTypes {
   /// Record code for the language options table.
   ///
   /// The record with this code contains the contents of the
   /// LangOptions structure. We serialize the entire contents of
   /// the structure, and let the reader decide which options are
   /// actually important to check.
   LANGUAGE_OPTIONS = 0,

   /// Record code for the target options table.
   TARGET_OPTIONS,
};

/// Record types that occur within the AST block
enum ASTRecordTypes {
   /// Serialized representation of the global decl context, containing all
   /// of the module's declarations
   GLOBAL_DECL_CONTEXT = 0,

   /// The modules operator and precedence group decls
   OPERATOR_PRECEDENCE_DECLS,

   /// Table of template instantiations in the module.
   INSTANTIATION_TABLE,

   /// Name lookup table for cache files.
   CACHE_LOOKUP_TABLE,
};

/// Record types that occur within the conformance block
enum ConformanceRecordTypes {
   /// Table of conformances for all records in the module.
   CONFORMANCE_TABLE = 0,

   /// Blob of data representing the conformances.
   CONFORMANCE_DATA,
};

/// Record types that occur directly within the IL module block
enum ILRecordTypes {
   /// Meta information about the IL module (e.g. module name, file name,
   /// creation date, etc.)
   IL_METADATA = 0,

   /// An on-disk hash table of the module's visible symbols
   IL_SYMBOL_TABLE,
};

/// Record types used in the identifier block
enum IdentifierRecordTypes {
   /// Offsets of all identifiers, indexed by identifier ID
   IDENTIFIER_OFFSET = 0,

   /// Table containing the module's identifier data
   IDENTIFIER_TABLE,
};

enum OffsetBlockRecordTypes {
   /// Offsets of all decl definitions, indexed by decl ID
   DECL_OFFSET = 0,

   /// Offsets of all scopes, indexed by scope ID.
   SCOPE_OFFSET,

   /// Offsets of all type definitions, indexed by type ID
   TYPE_OFFSET,

   /// Offsets of all emitted values, indexed by their ID
   IL_VALUE_OFFSETS,
};

enum OffsetRangeRecordTypes {
   /// Offset ranges.
   OFFSET_RANGES = 0,
};

enum StmtKinds {
   STMT_NULL_PTR = ast::Statement::_lastASTNodeID,
   STMT_REF_PTR,
   STMT_STOP,
};

enum DeclKinds {
   /// A reference to a declaration from an imported module.
   DECL_EXTERNAL = 0,

   /// A reference to a declaration from another cache file.
   DECL_CACHED,

   /// Lexical decl context contents.
   DECL_CONTEXT_LEXICAL,

   /// Visible decl context contents.
   DECL_CONTEXT_VISIBLE,

#  define CDOT_DECL(NAME) DECL_##NAME,
#  include "AST/Decl.def"
};

} // namespace serial
} // namespace cdot

#endif //CDOT_ASTBITCODES_H
