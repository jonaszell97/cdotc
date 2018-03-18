//
// Created by Jonas Zell on 13.03.18.
//

#ifndef CDOT_CONTINUATIONPOINT_H
#define CDOT_CONTINUATIONPOINT_H

#include "AST/Statement.h"
#include "AST/NamedDecl.h"

#include <llvm/ADT/PointerUnion.h>

namespace cdot {

class Scope;

namespace ast {

class Decl;
class Statement;
class DeclContext;
class SemaPass;

struct ContinuationPoint {
      ContinuationPoint(SemaPass &SP, Statement *Stmt);
      ContinuationPoint(SemaPass &SP, Decl *D);

      bool isStmt() const { return StmtOrDecl.is<Statement*>() != 0; }
      Statement *getStmt() const { return StmtOrDecl.get<Statement*>(); }
      Decl *getDecl() const { return StmtOrDecl.get<Decl*>(); }
      Scope *getScope() const { return scope; }
      DeclContext *getDeclContext() const { return declContext; }

   private:
      llvm::PointerUnion<Statement*, Decl*> StmtOrDecl;
      Scope *scope;
      DeclContext *declContext;
   };

} // namespace ast
} // namespace cdot

#endif //CDOT_CONTINUATIONPOINT_H
