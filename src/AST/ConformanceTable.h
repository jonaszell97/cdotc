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
   class ExtensionDecl;
   class ConstraintSet;
} // namespace ast

enum class ConformanceKind : unsigned char {
   /// \brief The conformance does not exist.
   None        = 0,

   /// \brief The conformance is implicit, e.g. an enum's conformance to
   /// Equatable.
   Implicit    = 1,

   /// \brief The conformance was explicitly declared in the code.
   Explicit    = 2,

   /// \brief The conformance was inherited from a superclass or a protocol.
   Inherited   = 4,

   /// \brief The conformance is dependent on a constraint. Only applicable to
   /// templates or protocols with unresolved constraints.
   Conditional = 5,
};

class Conformance {
   ConformanceKind Kind;
   ast::ProtocolDecl *Proto;

   // The constraints of this conformance (only for conditional conformances).
   ast::ConstraintSet *constraints;

public:
   Conformance(ConformanceKind Kind, ast::ProtocolDecl *Proto,
               ast::ConstraintSet *constraints = nullptr)
      : Kind(Kind), Proto(Proto), constraints(constraints)
   { }

   ConformanceKind getKind() const { return Kind; }
   ast::ProtocolDecl *getProto() const { return Proto; }
   ast::ConstraintSet *getConstraints() const { return constraints; }

   bool isConditional() const { return Kind == ConformanceKind::Conditional; }
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
   ~ConformanceTable();

   ConformanceKind lookupConformance(ast::RecordDecl *Decl,
                                     ast::ProtocolDecl *P) const;

   bool conformsTo(ast::RecordDecl *Decl, ast::ProtocolDecl *P) const;

   bool addConformance(ast::ASTContext &C,
                       ConformanceKind Kind,
                       ast::RecordDecl *Decl,
                       ast::ProtocolDecl *P,
                       ast::ConstraintSet *constraints = nullptr,
                       Conformance **NewConf = nullptr);

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
