//
// Created by Jonas Zell on 06.08.18.
//

#ifndef CDOT_NESTEDNAMESPECIFIER_H
#define CDOT_NESTEDNAMESPECIFIER_H

#include "DeclarationName.h"

#include <llvm/Support/TrailingObjects.h>

namespace cdot {

class Module;

namespace sema {
   class FinalTemplateArgumentList;
} // namespace sema

class LLVM_ALIGNAS(8) NestedNameSpecifier: public llvm::FoldingSetNode {
public:
   enum Kind : uint8_t {
      /// \brief An ordinary, unresolved identifier.
      Identifier,

      /// \brief A namespace reference.
      Namespace,

      /// \brief A type.
      Type,

      /// \brief A template parameter.
      TemplateParam,

      /// \brief An associated type.
      AssociatedType,

      /// \brief A dependent alias.
      Alias,

      /// \brief A module reference.
      Module,

      /// \brief A dependent template argument list.
      TemplateArgList,
   };

private:
   /// The previous name in the chain and the name kind.
   llvm::PointerIntPair<NestedNameSpecifier*, 3, Kind> PreviousAndKind;

   /// The opaque data of this name specifier, dependent on the name kind.
   void *Data = nullptr;

   /// \brief get or create a uniqued name specifier.
   NestedNameSpecifier(IdentifierInfo *Ident,
                       NestedNameSpecifier *Previous);

   /// \brief get or create a uniqued name specifier.
   NestedNameSpecifier(ast::NamespaceDecl *NS,
                       NestedNameSpecifier *Previous);

   /// \brief get or create a uniqued name specifier.
   NestedNameSpecifier(QualType Type,
                       NestedNameSpecifier *Previous);

   /// \brief get or create a uniqued name specifier.
   NestedNameSpecifier(ast::TemplateParamDecl *TP,
                       NestedNameSpecifier *Previous);

   /// \brief get or create a uniqued name specifier.
   NestedNameSpecifier(ast::AssociatedTypeDecl *AT,
                       NestedNameSpecifier *Previous);

   /// \brief get or create a uniqued name specifier.
   NestedNameSpecifier(ast::AliasDecl *Alias,
                       NestedNameSpecifier *Previous);

   /// \brief get or create a uniqued name specifier.
   NestedNameSpecifier(cdot::Module *M,
                       NestedNameSpecifier *Previous);

   /// \brief get or create a uniqued name specifier.
   NestedNameSpecifier(sema::FinalTemplateArgumentList *TemplateArgs,
                       NestedNameSpecifier *Previous);

public:
   /// \brief get or create a uniqued name specifier.
   static NestedNameSpecifier *Create(DeclarationNameTable &Tbl,
                                      IdentifierInfo *Ident,
                                      NestedNameSpecifier *Previous = nullptr);

   /// \brief get or create a uniqued name specifier.
   static NestedNameSpecifier *Create(DeclarationNameTable &Tbl,
                                      ast::NamespaceDecl *NS,
                                      NestedNameSpecifier *Previous = nullptr);

   /// \brief get or create a uniqued name specifier.
   static NestedNameSpecifier *Create(DeclarationNameTable &Tbl,
                                      QualType Type,
                                      NestedNameSpecifier *Previous = nullptr);

   /// \brief get or create a uniqued name specifier.
   static NestedNameSpecifier *Create(DeclarationNameTable &Tbl,
                                      ast::TemplateParamDecl *TP,
                                      NestedNameSpecifier *Previous = nullptr);

   /// \brief get or create a uniqued name specifier.
   static NestedNameSpecifier *Create(DeclarationNameTable &Tbl,
                                      ast::AssociatedTypeDecl *AT,
                                      NestedNameSpecifier *Previous = nullptr);

   /// \brief get or create a uniqued name specifier.
   static NestedNameSpecifier *Create(DeclarationNameTable &Tbl,
                                      ast::AliasDecl *Alias,
                                      NestedNameSpecifier *Previous = nullptr);

   /// \brief get or create a uniqued name specifier.
   static NestedNameSpecifier *Create(DeclarationNameTable &Tbl,
                                      cdot::Module *M,
                                      NestedNameSpecifier *Previous = nullptr);

   /// \brief get or create a uniqued name specifier.
   static NestedNameSpecifier *Create(DeclarationNameTable &Tbl,
                                      sema::FinalTemplateArgumentList *TemplateArgs,
                                      NestedNameSpecifier *Previous = nullptr);

   /// \brief Profile a name specifier to the given ID.
   static void Profile(llvm::FoldingSetNodeID &ID, Kind K,
                       NestedNameSpecifier *Previous, void *Data);

   /// \brief Profile this name specifier to the given ID.
   void Profile(llvm::FoldingSetNodeID &ID) const;

   /// \brief print this name specifier to stderr.
   void dump() const;

   /// \brief print this name specifier to \param OS.
   void print(llvm::raw_ostream &OS) const;

   /// \return the name kind of this name specifier segment.
   Kind getKind() const { return PreviousAndKind.getInt(); }

   /// \return the previous segment in this name specifier.
   NestedNameSpecifier *getPrevious() const
   {
      return PreviousAndKind.getPointer();
   }

   /// \return true iff this name specifier is dependent on a template
   // parameter.
   bool isDependent() const;

    /// \return true iff any name in this nested specifier is dependent.
   bool isAnyNameDependent() const;

   /// \return the 'depth' of this name, i.e. the number of names that come
   /// before it plus one.
   unsigned depth() const;

   /// \return the name identifier, if there is one.
   IdentifierInfo *getIdentifier() const;

   /// \return the namespace decl, if there is one.
   ast::NamespaceDecl *getNamespace() const;

   /// \return the type, if there is one.
   QualType getType() const;

   /// \return the template parameter.
   ast::TemplateParamDecl *getParam() const;

   /// \return the associated type.
   ast::AssociatedTypeDecl *getAssociatedType() const;

   /// \return the alias.
   ast::AliasDecl *getAlias() const;

   /// \return the module ref.
   cdot::Module *getModule() const;

   /// \return the template argument list.
   sema::FinalTemplateArgumentList *getTemplateArgs() const;
};

class NestedNameSpecifierWithLoc final:
                     TrailingObjects<NestedNameSpecifierWithLoc, SourceRange> {
   NestedNameSpecifierWithLoc(NestedNameSpecifier *Name,
                              ArrayRef<SourceRange> Locs);

   NestedNameSpecifierWithLoc(NestedNameSpecifierWithLoc *Prev,
                              NestedNameSpecifier *NewName,
                              SourceRange NewLoc);

   /// The nested name specifier.
   NestedNameSpecifier *NameSpec;

   /// The number of available source ranges.
   unsigned NumSourceRanges;

public:
   static NestedNameSpecifierWithLoc *Create(DeclarationNameTable &Tbl,
                                             NestedNameSpecifier *Name,
                                             ArrayRef<SourceRange> Locs);

   static NestedNameSpecifierWithLoc *Create(DeclarationNameTable &Tbl,
                                             NestedNameSpecifierWithLoc *Prev,
                                             NestedNameSpecifier *NewName,
                                             SourceRange NewLoc);

   friend TrailingObjects;

   /// \return the underlying name specifier.
   NestedNameSpecifier *getNameSpec() const { return NameSpec; }

   /// \return the source ranges of this name specifier.
   ArrayRef<SourceRange> getSourceRanges() const
   {
      return { getTrailingObjects<SourceRange>(), NumSourceRanges };
   }

   /// \return the source range at index \param idx.
   SourceRange getSourceRange(unsigned i) const;

   /// \return the source range of the entire nested name.
   SourceRange getFullRange() const;
};

} // namespace cdot

#endif //CDOT_NESTEDNAMESPECIFIER_H
