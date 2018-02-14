//
// Created by Jonas Zell on 09.01.18.
//

#ifndef CDOT_AST_SERIALIZE_H
#define CDOT_AST_SERIALIZE_H

#include "Support/Casting.h"
#include "lex/SourceLocation.h"

#include <memory>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Support/MemoryBuffer.h>

namespace cdot {

namespace ast {
   class Statement;
   class CallableDecl;
   class PropDecl;
   class AliasDecl;
   class FieldDecl;
   class EnumCaseDecl;
   class RecordDecl;
   class SemaPass;
} // namespace ast

namespace module {
   class Module;
} // namespace module

namespace serial {

class BitstreamReader;
class ASTSerializerImpl;

class ASTSerializer {
public:
   enum Options : unsigned {
      Opt_None = 0,
      Opt_SkipDefinitions = 1,
   };

   ASTSerializer(Options opts = Opt_None);
   ~ASTSerializer();

   void serialize(ast::Statement* const& stmt,
                  llvm::raw_ostream &out);

   void serialize(ast::Statement const* stmt,
                  llvm::raw_ostream &out);

   void serialize(ast::RecordDecl const* stmt,
                  bool skipDefinitions,
                  bool fullName,
                  llvm::raw_ostream &out);

   void serialize(ast::CallableDecl const* stmt,
                  bool skipBody,
                  llvm::raw_ostream &out);

   void serialize(ast::PropDecl const* stmt,
                  bool skipBody,
                  llvm::raw_ostream &out);

   void serialize(ast::AliasDecl const* stmt,
                  bool skipBody,
                  llvm::raw_ostream &out);

   void serialize(ast::FieldDecl const* stmt,
                  bool skipDefaultVal,
                  llvm::raw_ostream &out);

   void serialize(ast::EnumCaseDecl const* stmt,
                  bool skipCaseVal,
                  llvm::raw_ostream &out);

private:
   ASTSerializerImpl *pImpl;
};

class ASTDeserializerImpl;

class ASTDeserializer {
public:
   explicit ASTDeserializer(ast::SemaPass &SP,
                            llvm::MemoryBuffer &in,
                            module::Module *M = nullptr,
                            SourceLocation loc = {});

   explicit ASTDeserializer(ast::SemaPass &SP,
                            BitstreamReader &Reader,
                            module::Module *M = nullptr,
                            SourceLocation loc = {});
   ~ASTDeserializer();

   ptrdiff_t getOffset() const;

   ast::Statement* Read();

   template<class T>
   T* ReadSpecific()
   {
      auto stmt = Read();
      return support::cast<T>(stmt);
   }

private:
   BitstreamReader *Reader = nullptr;
   ASTDeserializerImpl *pImpl;
};

} // namespace serial
} // namespace cdot

#endif //CDOT_AST_SERIALIZE_H
