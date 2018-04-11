//
// Created by Jonas Zell on 25.03.18.
//

#ifndef CDOT_DECLARATIONNAME_H
#define CDOT_DECLARATIONNAME_H

#include "AST/Type.h"
#include "IdentifierInfo.h"

namespace cdot {

namespace sema {
   class TemplateArgList;
} // namespace sema

class BlockScope;
class DeclarationNameTable;
class DeclarationNameInfo;

class DeclarationName {
public:
   enum : uintptr_t { PtrMask = 0x3u };
   enum DeclarationKind: unsigned char {
      NormalIdentifier,
      ConstructorName,
      DestructorName,
      InfixOperatorName,
      PrefixOperatorName,
      PostfixOperatorName,
      ConversionOperatorName,
      InstantiationName,
      ExtensionName,
      PackExpansionName,
      LocalVarName,
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

   IdentifierInfo *getIdentifierInfo() const
   {
      if (getStoredKind() == StoredIdentifier)
         return reinterpret_cast<IdentifierInfo*>(Val);

      return nullptr;
   }

   QualType getConstructorType() const
   {
      if (getStoredKind() == StoredInitializerName)
         return QualType::getFromOpaquePtr(
            reinterpret_cast<void*>(Val & ~PtrMask));

      return QualType();
   }

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

   DeclarationName getPackExpansionName() const;
   unsigned getPackExpansionIndex() const;

   DeclarationName getLocalVarName() const;
   BlockScope *getLocalVarScope() const;

   DeclarationName getInstantiationName() const;
   const sema::TemplateArgList *getInstantiationArgs() const;

   QualType getConversionOperatorType() const;
   QualType getExtendedType() const;

   DeclarationKind getDeclarationKind() const;

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

template <>
struct isPodLike<cdot::DeclarationName> { static const bool value = true; };

} // namespace llvm

namespace cdot {

class DeclarationNameTable {
   // actually llvm::FoldingSet<DeclarationNameInfo>*
   void *FoldingSetPtr;

   ast::ASTContext &Ctx;

public:
   explicit DeclarationNameTable(ast::ASTContext &Ctx);
   ~DeclarationNameTable();

   DeclarationName getNormalIdentifier(const IdentifierInfo &II);
   DeclarationName getConstructorName(QualType ConstructedType);
   DeclarationName getDestructorName(QualType DestructedType);

   DeclarationName getInfixOperatorName(const IdentifierInfo &II);
   DeclarationName getPrefixOperatorName(const IdentifierInfo &II);
   DeclarationName getPostfixOperatorName(const IdentifierInfo &II);
   DeclarationName getConversionOperatorName(QualType ConversionType);

   DeclarationName getPackExpansionName(DeclarationName Name,
                                        unsigned idx);

   DeclarationName getLocalVarName(DeclarationName Name,
                                   BlockScope *Scope);

   DeclarationName getExtensionName(QualType ExtendedType);

   DeclarationName getInstantiationName(DeclarationName BaseName,
                                        const sema::TemplateArgList &argList);

   DeclarationName getSpecialName(DeclarationName::DeclarationKind Kind,
                                  uintptr_t Data1, uintptr_t Data2 = 0);
};

diag::DiagnosticBuilder &operator<<(diag::DiagnosticBuilder &builder,
                                    const DeclarationName &DN);

inline diag::DiagnosticBuilder &operator<<(diag::DiagnosticBuilder &builder,
                                           IdentifierInfo *II) {
   return builder << DeclarationName(II);
}

} // namespace cdot

#endif //CDOT_DECLARATIONNAME_H
