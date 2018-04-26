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
   class TemplateArgList;
} // namespace sema

namespace ast {
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
   void mangleVTable(const ast::RecordDecl *R, llvm::raw_ostream &OS) const;
   void mangleTypeInfo(const QualType &T, llvm::raw_ostream &OS) const;
   void mangleTypeName(const QualType &T, llvm::raw_ostream &OS) const;

private:
   ast::SemaPass &SP;
   mutable llvm::DenseMap<const ast::Decl*, std::string> Cache;
};

} // namespace cdot

#endif //CDOT_MANGLE_H
