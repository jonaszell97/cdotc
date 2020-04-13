#ifndef CDOT_STMTORDECL_H
#define CDOT_STMTORDECL_H

#include "DeclDenseMapInfo.h"
#include "Support/Config.h"

#include <llvm/ADT/PointerUnion.h>

namespace cdot {

struct SourceLocation;
struct SourceRange;

struct StmtOrDecl {
   using UnionType = llvm::PointerUnion<ast::Decl*, ast::Statement*>;

   CDOT_LLDB_STEP_OVER
   /*implicit*/ StmtOrDecl(ast::Statement *S) : Union(S) {}

   CDOT_LLDB_STEP_OVER
   /*implicit*/ StmtOrDecl(ast::Decl *D) : Union(D) {}

   CDOT_LLDB_STEP_OVER
   /*implicit*/ StmtOrDecl(std::nullptr_t) : Union((ast::Statement*)nullptr) {}

   CDOT_LLDB_STEP_OVER
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

   bool isTypeDependent() const;
   void setTypeDependent(bool b) const;

   bool isValueDependent() const;
   void setValueDependent(bool b) const;

   bool isDependent() const { return isTypeDependent() || isValueDependent(); }

   void copyStatusFlags(StmtOrDecl Other) const;

   bool isNull() const { return Union.isNull(); }
   operator bool() const { return !isNull(); }

   void *getAsOpaquePtr() const
   {
      return Union.getOpaqueValue();
   }

   static StmtOrDecl getFromOpaquePtr(void *Ptr)
   {
      StmtOrDecl D;
      D.Union = UnionType::getFromOpaqueValue(Ptr);

      return D;
   }

private:
   llvm::PointerUnion<ast::Decl*, ast::Statement*> Union;
};

} // namespace cdot

namespace llvm {

template<>
struct PointerLikeTypeTraits<::cdot::StmtOrDecl> {
public:
   static inline void *getAsVoidPointer(::cdot::StmtOrDecl P)
   {
      return P.getAsOpaquePtr();
   }

   static inline ::cdot::StmtOrDecl getFromVoidPointer(void *P)
   {
      return ::cdot::StmtOrDecl::getFromOpaquePtr(P);
   }

   enum { NumLowBitsAvailable = 0 };
};

} // namespace llvm

#endif //CDOT_STMTORDECL_H
