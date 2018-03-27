//
// Created by Jonas Zell on 25.01.18.
//

#ifndef CDOT_MODULE_IDENTIFIERTABLE_H
#define CDOT_MODULE_IDENTIFIERTABLE_H

#include "Lex/SourceLocation.h"
#include "Support/Casting.h"

#include <llvm/ADT/StringRef.h>
#include <llvm/Support/raw_ostream.h>

namespace llvm {
   class MemoryBuffer;
} // namespace llvm

namespace cdot {

namespace ast {
   class NamedDecl;
   class SemaPass;
} // namespace ast

namespace module {

class Module;

class IdentifierTableImpl;

class IdentifierTable {
public:
   using Decl = ast::NamedDecl;

   enum QueryLevel {
      QL_None = 0, //> only used internally
      QL_Declaration, //> only type name is required, i.e. for function
      //  signatures
         QL_Interface,   //> full interface required. this includes, fields,
      //  methods, underlying typedef types, etc.
         QL_Definition,  //> full definition required. for CTFE or templates
   };

   IdentifierTable(ast::SemaPass &SP,
                   std::unique_ptr<llvm::MemoryBuffer> &&Buf,
                   SourceLocation importLoc);

   ~IdentifierTable() {}

   Decl *lookup(llvm::StringRef name,
                QueryLevel required = QL_Interface);

   template<class T>
   Decl *get(llvm::StringRef name,
             QueryLevel required = QL_Interface) {
      return support::dyn_cast<T>(lookup(name, required));
   }

private:
   static constexpr size_t pImplSize = 104;
   IdentifierTableImpl *pImplStorage;
//   std::aligned_storage<pImplSize, 8> pImplStorage;
};

void serializeModuleInterface(module::Module *M,
                              llvm::raw_ostream &out,
                              unsigned char beginLevel = 0,
                              unsigned char endLevel = 3);

} // namespace module
} // namespace cdot


#endif //CDOT_MODULE_IDENTIFIERTABLE_H
