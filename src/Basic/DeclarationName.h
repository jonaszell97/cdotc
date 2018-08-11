//
// Created by Jonas Zell on 25.03.18.
//

#ifndef CDOT_DECLARATIONNAME_H
#define CDOT_DECLARATIONNAME_H

#include "AST/Type.h"
#include "IdentifierInfo.h"
#include "Lex/SourceLocation.h"

namespace cdot {

namespace sema {
   class FinalTemplateArgumentList;
} // namespace sema

class DeclarationNameTable;
class DeclarationNameInfo;
class NestedNameSpecifier;

class DeclarationName {
public:
   enum : uintptr_t { PtrMask = 0x3u };
   enum DeclarationKind: unsigned char {
      NormalIdentifier,
      ConstructorName,      // complete constructor
      BaseConstructorName,
      DestructorName,
      InfixOperatorName,
      PrefixOperatorName,
      PostfixOperatorName,
      OperatorDeclName,
      ConversionOperatorName,
      InstantiationName,
      ExtensionName,
      PackExpansionName,
      LocalVarName,
      AccessorName,
      ClosureArgumentName,
      MacroName,
      SubscriptName,
      ErrorName,
   };

   enum AccessorKind : unsigned char {
      Getter, Setter,
   };

   enum class SubscriptKind : unsigned char {
      Getter, Setter, General,
   };

   DeclarationName() : Val(0) {}

   DeclarationName(const IdentifierInfo &Info) : Val((uintptr_t)&Info)
   {
      assert(!(Val & PtrMask) && "pointer not sufficiently aligned!");
      (void)K;
   }

   DeclarationName(const IdentifierInfo *Info) : Val((uintptr_t)Info)
   {
      assert(!(Val & PtrMask) && "pointer not sufficiently aligned!");
   }

   bool isSimpleIdentifier() const
   {
      return *this && getStoredKind() == StoredIdentifier;
   }

   template <std::size_t StrLen>
   bool isStr(const char (&Str)[StrLen]) const
   {
      return isSimpleIdentifier() && getIdentifierInfo()->isStr(Str);
   }

   IdentifierInfo *getIdentifierInfo() const;

   QualType getConstructorType() const;
   QualType getDestructorType() const
   {
      if (getStoredKind() == StoredDeinitializerName)
         return QualType::getFromOpaquePtr(
            reinterpret_cast<void*>(Val & ~PtrMask));

      return QualType();
   }

   const IdentifierInfo* getInfixOperatorName() const;
   const IdentifierInfo* getPrefixOperatorName() const;
   const IdentifierInfo* getPostfixOperatorName() const;

   const IdentifierInfo *getAccessorName() const;
   AccessorKind getAccessorKind() const;

   SubscriptKind getSubscriptKind() const;

   unsigned getClosureArgumentIdx() const;

   DeclarationName getDeclaredOperatorName() const;

   const IdentifierInfo *getMacroName() const;

   DeclarationName getPackExpansionName() const;
   unsigned getPackExpansionIndex() const;

   DeclarationName getLocalVarName() const;
   unsigned getLocalVarScope() const;

   DeclarationName getInstantiationName() const;
   const sema::FinalTemplateArgumentList *getInstantiationArgs() const;

   QualType getConversionOperatorType() const;
   QualType getExtendedType() const;

   bool isErrorName() const { return getKind() == ErrorName; }

   DeclarationKind getKind() const;

   friend bool operator==(const DeclarationName &LHS,
                          const DeclarationName &RHS) {
      return LHS.Val == RHS.Val;
   }

   friend bool operator!=(const DeclarationName &LHS,
                          const DeclarationName &RHS) {
      return LHS.Val != RHS.Val;
   }

   explicit operator bool() const
   {
      return Val != 0;
   }

   void *getAsOpaquePtr() const { return (void*)Val; }
   static DeclarationName getFromOpaquePtr(void *Ptr)
   {
      DeclarationName DN;
      DN.Val = reinterpret_cast<uintptr_t>(Ptr);

      return DN;
   }

   static DeclarationName getEmptyMarker()
   {
      DeclarationName DN;
      DN.Val = (uintptr_t)-1;

      return DN;
   }

   static DeclarationName getTombstoneMarker()
   {
      DeclarationName DN;
      DN.Val = (uintptr_t)-2;

      return DN;
   }

   DeclarationName getManglingName() const;

   int compare(const DeclarationName &RHS) const;

   void dump() const;
   void print(llvm::raw_ostream &OS) const;

   friend class DeclarationNameTable;

private:
   enum StoredKind: unsigned char {
      StoredIdentifier        = 0x0,
      StoredInitializerName   = 0x1,
      StoredDeinitializerName = 0x2,
      OtherStoredName         = 0x3,
   };

   StoredKind getStoredKind() const
   {
      return static_cast<StoredKind>(Val & PtrMask);
   }

   DeclarationName(QualType Ty, DeclarationKind Kind)
      : Val(reinterpret_cast<uintptr_t>(Ty.getAsOpaquePtr()))
   {
      if (Kind == ConstructorName)
         Val |= StoredInitializerName;
      else {
         assert(Kind == DestructorName && "bad stored kind");
         Val |= StoredDeinitializerName;
      }
   }

   explicit DeclarationName(DeclarationNameInfo *DNI);

   DeclarationNameInfo *getDeclInfo() const
   {
      return reinterpret_cast<DeclarationNameInfo*>(Val & ~PtrMask);
   }

   union {
      uintptr_t Val;
      StoredKind K;
   };
};

inline int operator>(const DeclarationName &LHS,
                     const DeclarationName &RHS) {
   return LHS.compare(RHS) > 0;
}

inline int operator>=(const DeclarationName &LHS,
                      const DeclarationName &RHS) {
   return LHS.compare(RHS) >= 0;
}

inline int operator<(const DeclarationName &LHS,
                     const DeclarationName &RHS) {
   return LHS.compare(RHS) < 0;
}

inline int operator<=(const DeclarationName &LHS,
                      const DeclarationName &RHS) {
   return LHS.compare(RHS) <= 0;
}

inline llvm::raw_ostream &operator<<(llvm::raw_ostream &OS, DeclarationName DN)
{
   DN.print(OS);
   return OS;
}

} // namespace cdot

namespace llvm {

/// Define DenseMapInfo so that DeclarationNames can be used as keys
/// in DenseMap and DenseSets.
template<>
struct DenseMapInfo<cdot::DeclarationName> {
   static inline cdot::DeclarationName getEmptyKey()
   {
      return cdot::DeclarationName::getEmptyMarker();
   }

   static inline cdot::DeclarationName getTombstoneKey()
   {
      return cdot::DeclarationName::getTombstoneMarker();
   }

   static unsigned getHashValue(cdot::DeclarationName Name)
   {
      return DenseMapInfo<void*>::getHashValue(Name.getAsOpaquePtr());
   }

   static inline bool isEqual(cdot::DeclarationName LHS,
                              cdot::DeclarationName RHS) {
      return LHS == RHS;
   }
};

template <typename T>
struct PointerLikeTypeTraits;

template<>
struct PointerLikeTypeTraits< ::cdot::DeclarationName> {
public:
   static inline void *getAsVoidPointer(::cdot::DeclarationName N)
   {
      return N.getAsOpaquePtr();
   }

   static inline ::cdot::DeclarationName getFromVoidPointer(void *P)
   {
      return ::cdot::DeclarationName::getFromOpaquePtr(P);
   }

   enum { NumLowBitsAvailable = 0 };
};

template <>
struct isPodLike<cdot::DeclarationName> { static const bool value = true; };

} // namespace llvm

namespace cdot {

class DeclarationNameTable {
   // actually llvm::FoldingSet<DeclarationNameInfo>*
   void *FoldingSetPtr;
   ast::ASTContext &Ctx;
   DeclarationName ErrorName;

   // actually a llvm::FoldingSet<NestedNameSpecifier>*
   void *NestedNameSpecifiers;

public:
   explicit DeclarationNameTable(ast::ASTContext &Ctx);
   ~DeclarationNameTable();

   friend class NestedNameSpecifier;
   friend class NestedNameSpecifierWithLoc;

   DeclarationName getIdentifiedName(DeclarationName::DeclarationKind Kind,
                                     const IdentifierInfo &II);

   DeclarationName getTypedName(DeclarationName::DeclarationKind Kind,
                                QualType Ty);

   DeclarationName getNormalIdentifier(const IdentifierInfo &II);
   DeclarationName getConstructorName(QualType ConstructedType,
                                      bool IsCompleteCtor = true);
   DeclarationName getDestructorName(QualType DestructedType);

   DeclarationName getInfixOperatorName(const IdentifierInfo &II);
   DeclarationName getPrefixOperatorName(const IdentifierInfo &II);
   DeclarationName getPostfixOperatorName(const IdentifierInfo &II);
   DeclarationName getConversionOperatorName(QualType ConversionType);

   DeclarationName getMacroName(const IdentifierInfo &II);

   DeclarationName getPackExpansionName(DeclarationName Name,
                                        unsigned idx);

   DeclarationName getAccessorName(const IdentifierInfo &II,
                                   DeclarationName::AccessorKind Kind);

   DeclarationName getClosureArgumentName(unsigned ArgNo);

   DeclarationName getLocalVarName(DeclarationName Name,
                                   unsigned ScopeID);

   DeclarationName getExtensionName(QualType ExtendedType);
   DeclarationName getSubscriptName(DeclarationName::SubscriptKind Kind);

   DeclarationName getInstantiationName(
                              DeclarationName BaseName,
                              const sema::FinalTemplateArgumentList &argList);

   DeclarationName getOperatorDeclName(DeclarationName OpName);
   DeclarationName getErrorName();

   DeclarationName getSpecialName(DeclarationName::DeclarationKind Kind,
                                  uintptr_t Data1, uintptr_t Data2 = 0);
};

diag::DiagnosticBuilder &operator<<(diag::DiagnosticBuilder &builder,
                                    const DeclarationName &DN);

diag::DiagnosticBuilder &operator<<(diag::DiagnosticBuilder &builder,
                                    IdentifierInfo *II);

diag::DiagnosticBuilder &operator<<(diag::DiagnosticBuilder &builder,
                                    NestedNameSpecifier *Name);

llvm::raw_ostream &operator<<(llvm::raw_ostream &OS,
                              NestedNameSpecifier *Name);

} // namespace cdot

#endif //CDOT_DECLARATIONNAME_H
