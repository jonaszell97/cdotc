//
// Created by Jonas Zell on 09.04.18.
//

#ifndef CDOT_STMTORDECL_H
#define CDOT_STMTORDECL_H

#include "DeclDenseMapInfo.h"
#include <llvm/ADT/PointerUnion.h>

namespace cdot {

struct SourceLocation;
struct SourceRange;

struct StmtOrDecl {
   /*implicit*/ StmtOrDecl(ast::Statement *S) : Union(S) {}
   /*implicit*/ StmtOrDecl(ast::Decl *D) : Union(D) {}

   /*implicit*/ StmtOrDecl(std::nullptr_t) : Union((ast::Statement*)nullptr) {}
   StmtOrDecl() : Union((ast::Decl*)nullptr) {}

   SourceLocation getSourceLoc() const;
   SourceRange getSourceRange() const;

   ast::Decl *asDecl() const { return Union.dyn_cast<ast::Decl*>(); }
   ast::Statement *asStatement() const
   { return Union.dyn_cast<ast::Statement*>(); }

   ast::Decl *getDecl() const { return Union.get<ast::Decl*>(); }
   ast::Statement *getStatement() const
   { return Union.get<ast::Statement*>(); }

   bool isInvalid() const;
   void setIsInvalid(bool b) const;

   void copyStatusFlags(StmtOrDecl Other) const;

   bool isNull() const { return Union.isNull(); }
   operator bool() const { return !isNull(); }

private:
   llvm::PointerUnion<ast::Decl*, ast::Statement*> Union;
};

} // namespace cdot

#endif //CDOT_STMTORDECL_H
