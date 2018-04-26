//
// Created by Jonas Zell on 16.04.18.
//

#ifndef CDOT_CONFORMANCETABLE_H
#define CDOT_CONFORMANCETABLE_H

#include "AST/DeclDenseMapInfo.h"

#include <llvm/ADT/DenseMap.h>
#include <llvm/ADT/DenseSet.h>
#include <llvm/ADT/SmallPtrSet.h>
#include <llvm/ADT/TinyPtrVector.h>

namespace cdot {
namespace ast {
   class ASTContext;
} // namespace ast

enum class ConformanceKind : unsigned char {
   None = 0,
   Implicit = 1,
   Explicit = 2,
   Inherited = 4
};

class Conformance {
   ConformanceKind Kind;
   ast::ProtocolDecl *Proto;

public:
   Conformance(ConformanceKind Kind, ast::ProtocolDecl *Proto)
      : Kind(Kind), Proto(Proto)
   { }

   ConformanceKind getKind() const { return Kind; }
   ast::ProtocolDecl *getProto() const { return Proto; }
};

class ConformanceTable {
   using ConformanceList = llvm::TinyPtrVector<Conformance*>;
   using ConformanceMap  = llvm::DenseMap<ast::RecordDecl*, ConformanceList>;

   using ConformanceSet = llvm::DenseSet<ast::ProtocolDecl*>;
   using SingleOrSet    = llvm::PointerUnion<ast::ProtocolDecl*,
                                             ConformanceSet*>;

   using ConformanceSetMap = llvm::DenseMap<ast::RecordDecl*, SingleOrSet>;

   ConformanceMap Conformances;
   ConformanceSetMap TestMap;

   bool registerConformance(ast::ASTContext &C,
                            ast::RecordDecl *Rec,
                            ast::ProtocolDecl *P);

   void insertConformance(ast::RecordDecl *Rec, Conformance *Conf);

   std::pair<Conformance *const*, Conformance *const*>
   getFirstOfKind(ast::RecordDecl *R, ConformanceKind K) const;

public:
   ConformanceKind lookupConformance(ast::RecordDecl *Decl,
                                     ast::ProtocolDecl *P) const;

   bool conformsTo(ast::RecordDecl *Decl, ast::ProtocolDecl *P) const;

   bool addExplicitConformance(ast::ASTContext &C,
                               ast::RecordDecl *Decl,
                               ast::ProtocolDecl *P);

   bool addInheritedConformance(ast::ASTContext &C,
                                ast::RecordDecl *Decl,
                                ast::ProtocolDecl *P);

   bool addImplicitConformance(ast::ASTContext &C,
                               ast::RecordDecl *Decl,
                               ast::ProtocolDecl *P);

   llvm::ArrayRef<Conformance*> getAllConformances(ast::RecordDecl *Decl) const;

   template<ConformanceKind Kind>
   struct iterator {
      using pointer   = Conformance *const*;
      using reference = ast::ProtocolDecl *;

   private:
      pointer Ptr;
      pointer End;

      void moveToNext()
      {
         while (Ptr != End && ((*Ptr)->getKind() != Kind))
            ++Ptr;
      }

   public:
      iterator(pointer Ptr, pointer End) : Ptr(Ptr), End(End)
      {
         moveToNext();
      }

      iterator() : Ptr(nullptr), End(nullptr) {}

      reference operator*()  const { return (*Ptr)->getProto(); }
      reference operator->() const { return (*Ptr)->getProto(); }

      iterator &operator++() { ++Ptr; moveToNext(); return *this; }
      iterator operator++(int) { auto it = *this; ++*this; return it; }

      friend bool operator==(const iterator &lhs, const iterator &rhs)
      {
         return lhs.Ptr == rhs.Ptr;
      }

      friend bool operator!=(const iterator &lhs, const iterator &rhs)
      {
         return lhs.Ptr != rhs.Ptr;
      }
   };

   using explicit_iterator       = iterator<ConformanceKind::Explicit>;
   using explicit_iterator_range = llvm::iterator_range<explicit_iterator>;

   explicit_iterator explicit_begin(ast::RecordDecl *Rec) const;
   explicit_iterator explicit_end(ast::RecordDecl *Rec)   const;

   explicit_iterator_range getExplicitConformances(ast::RecordDecl *Rec) const
   {
      return explicit_iterator_range(explicit_begin(Rec), explicit_end(Rec));
   }

   using implicit_iterator       = iterator<ConformanceKind::Explicit>;
   using implicit_iterator_range = llvm::iterator_range<implicit_iterator>;

   implicit_iterator implicit_begin(ast::RecordDecl *Rec) const;
   implicit_iterator implicit_end(ast::RecordDecl *Rec)   const;

   implicit_iterator_range getImplicitConformances(ast::RecordDecl *Rec) const
   {
      return implicit_iterator_range(implicit_begin(Rec), implicit_end(Rec));
   }

   using inherited_iterator       = iterator<ConformanceKind::Explicit>;
   using inherited_iterator_range = llvm::iterator_range<inherited_iterator>;

   inherited_iterator inherited_begin(ast::RecordDecl *Rec) const;
   inherited_iterator inherited_end(ast::RecordDecl *Rec)   const;

   inherited_iterator_range getInheritedConformances(ast::RecordDecl *Rec) const
   {
      return inherited_iterator_range(inherited_begin(Rec), inherited_end(Rec));
   }
};

} // namespace cdot

#endif //CDOT_CONFORMANCETABLE_H
