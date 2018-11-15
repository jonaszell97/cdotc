//
// Created by Jonas Zell on 05.01.18.
//

#ifndef CDOT_MANGLE_H
#define CDOT_MANGLE_H

#include "AST/DeclDenseMapInfo.h"
#include "Support/LLVM.h"

#include <llvm/ADT/ArrayRef.h>
#include <llvm/ADT/DenseMap.h>

namespace cdot {

class QualType;

namespace sema {
   class FinalTemplateArgumentList;
} // namespace sema

namespace ast {
   class DeclContext;
   class SemaPass;
   class CallableDecl;
   class FunctionDecl;
   class MethodDecl;
   class RecordDecl;
   class NamedDecl;
   class Decl;
} // namespace ast

class SymbolMangler {
public:
   explicit SymbolMangler(ast::SemaPass &SP) : SP(SP)
   {}

   void mangle(const ast::NamedDecl *ND, llvm::raw_ostream &OS) const;
   void manglePrefix(const ast::DeclContext *DC, llvm::raw_ostream &OS) const;
   void manglePrefix(const ast::DeclContext *DC,
                     const sema::FinalTemplateArgumentList &TemplateArgs,
                     llvm::raw_ostream &OS) const;

   void mangleVTable(const ast::RecordDecl *R, llvm::raw_ostream &OS) const;
   void manglePTable(const ast::RecordDecl *R, const ast::ProtocolDecl *P,
                     llvm::raw_ostream &OS) const;

   void mangleProtocolStub(const ast::NamedDecl *D, std::string &Buf) const;

   void mangleTypeInfo(const QualType &T, llvm::raw_ostream &OS) const;
   void mangleTypeName(const QualType &T, llvm::raw_ostream &OS) const;

   void mangleDefaultDeinitializer(ast::RecordDecl *R,
                                   llvm::raw_ostream &OS) const;

   llvm::StringRef getPrefix(const ast::DeclContext *DC) const;

private:
   ast::SemaPass &SP;
   mutable llvm::DenseMap<const ast::Decl*, std::string> Cache;
   mutable llvm::DenseMap<const ast::DeclContext*, std::string> PrefixCache;
};

} // namespace cdot

#endif //CDOT_MANGLE_H
