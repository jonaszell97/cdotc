//
// Created by Jonas Zell on 25.03.18.
//

#include "DeclarationName.h"
#include "AST/ASTContext.h"
#include "Message/Diagnostics.h"

#include <llvm/ADT/FoldingSet.h>

namespace cdot {

class DeclarationNameInfo: public llvm::FoldingSetNode {
   DeclarationName::DeclarationKind Kind;
   union {
      QualType Ty;
      const IdentifierInfo *II;
      uintptr_t Opaque;
   };

public:
   DeclarationNameInfo(DeclarationName::DeclarationKind Kind, QualType Ty)
      : Kind(Kind), Ty(Ty)
   {}

   DeclarationNameInfo(DeclarationName::DeclarationKind Kind,
                       const IdentifierInfo &II)
      : Kind(Kind), II(&II)
   {}

   DeclarationName::DeclarationKind getKind() const
   {
      return Kind;
   }

   bool holdsType() const
   {
      switch (Kind) {
      case DeclarationName::ConversionOperatorName:
         return true;
      default:
         return false;
      }
   }

   QualType getType() const
   {
      assert(holdsType() && "name does not hold a type!");
      return Ty;
   }

   const IdentifierInfo &getIdentifierInfo() const
   {
      assert(!holdsType() && "does not hold an identifier");
      return *II;
   }

   static void Profile(llvm::FoldingSetNodeID &ID,
                       DeclarationName::DeclarationKind Kind, uintptr_t Ptr) {
      ID.AddInteger(Kind);
      ID.AddInteger(Ptr);
   }

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      return Profile(ID, Kind, Opaque);
   }
};

DeclarationName::DeclarationName(DeclarationNameInfo *DNI)
   : Val(reinterpret_cast<uintptr_t>(DNI))
{
   assert((Val & PtrMask) == 0 && "not sufficiently aligned!");
   Val |= OtherStoredName;
}

DeclarationName::DeclarationKind DeclarationName::getDeclarationKind() const
{
   switch (getStoredKind()) {
   case StoredIdentifier:
      return NormalIdentifier;
   case StoredInitializerName:
      return ConstructorName;
   case StoredDeinitializerName:
      return DestructorName;
   case OtherStoredName:
      return getDeclInfo()->getKind();
   }
}

const IdentifierInfo* DeclarationName::getInfixOperatorName() const
{
   if (getDeclarationKind() == InfixOperatorName)
      return &getDeclInfo()->getIdentifierInfo();

   return nullptr;
}

const IdentifierInfo* DeclarationName::getPrefixOperatorName() const
{
   if (getDeclarationKind() == PrefixOperatorName)
      return &getDeclInfo()->getIdentifierInfo();

   return nullptr;
}

const IdentifierInfo* DeclarationName::getPostfixOperatorName() const
{
   if (getDeclarationKind() == PostfixOperatorName)
      return &getDeclInfo()->getIdentifierInfo();

   return nullptr;
}

QualType DeclarationName::getConversionOperatorType() const
{
   if (getDeclarationKind() == ConversionOperatorName)
      return getDeclInfo()->getType();

   return QualType();
}

int DeclarationName::compare(const DeclarationName &RHS) const
{
   auto OwnKind = getDeclarationKind();
   auto OtherKind = RHS.getDeclarationKind();

   if (OwnKind != OtherKind)
      return (OwnKind < OtherKind) ? -1 : 1;

   switch (getDeclarationKind()) {
   case NormalIdentifier:
   case InfixOperatorName:
   case PrefixOperatorName:
   case PostfixOperatorName:
      return getIdentifierInfo()->getIdentifier().compare(
         RHS.getIdentifierInfo()->getIdentifier());
   default:
      return (int)((uintptr_t)getDeclInfo()->getType().getAsOpaquePtr()
                   - (uintptr_t)RHS.getDeclInfo()->getType().getAsOpaquePtr());
   }
}

void DeclarationName::dump() const
{
   print(llvm::errs());
}

void DeclarationName::print(llvm::raw_ostream &OS) const
{
   assert(*this && "invalid declaration name");
   switch (getDeclarationKind()) {
   case NormalIdentifier:
      OS << getIdentifierInfo()->getIdentifier();
      break;
   case InfixOperatorName:
      OS << "infix " << getInfixOperatorName()->getIdentifier();
      break;
   case PrefixOperatorName:
      OS << "prefix " << getPrefixOperatorName()->getIdentifier();
      break;
   case PostfixOperatorName:
      OS << "postfix " << getPostfixOperatorName()->getIdentifier();
      break;
   case ConversionOperatorName:
      OS << "as " << getConversionOperatorType();
      break;
   case ConstructorName:
      OS << "init";
      break;
   case DestructorName:
      OS << "deinit";
      break;
   }
}

using FoldingSetTy = llvm::FoldingSet<DeclarationNameInfo>;

DeclarationNameTable::DeclarationNameTable(ast::ASTContext &Ctx) : Ctx(Ctx)
{
   FoldingSetPtr = new FoldingSetTy();
}

DeclarationNameTable::~DeclarationNameTable()
{
   delete reinterpret_cast<FoldingSetTy*>(FoldingSetPtr);
}

DeclarationName
DeclarationNameTable::getNormalIdentifier(const IdentifierInfo &II)
{
   return DeclarationName(II);
}

DeclarationName
DeclarationNameTable::getConstructorName(QualType ConstructedType)
{
   return DeclarationName(ConstructedType, DeclarationName::ConstructorName);
}

DeclarationName
DeclarationNameTable::getDestructorName(QualType DestructedType)
{
   return DeclarationName(DestructedType, DeclarationName::DestructorName);
}

DeclarationName
DeclarationNameTable::getSpecialName(DeclarationName::DeclarationKind Kind,
                                     const cdot::IdentifierInfo &II) {
   auto &FS = *reinterpret_cast<FoldingSetTy*>(FoldingSetPtr);

   llvm::FoldingSetNodeID ID;
   DeclarationNameInfo::Profile(ID, Kind, reinterpret_cast<uintptr_t>(&II));

   void *InsertPos;
   if (auto *Ptr = FS.FindNodeOrInsertPos(ID, InsertPos)) {
      return DeclarationName(Ptr);
   }

   auto *Info = new(Ctx) DeclarationNameInfo(Kind, II);
   FS.InsertNode(Info, InsertPos);

   return DeclarationName(Info);
}

DeclarationName
DeclarationNameTable::getSpecialName(DeclarationName::DeclarationKind Kind,
                                     QualType Ty) {
   auto &FS = *reinterpret_cast<FoldingSetTy*>(FoldingSetPtr);

   llvm::FoldingSetNodeID ID;
   DeclarationNameInfo::Profile(
      ID, Kind, reinterpret_cast<uintptr_t>(Ty.getAsOpaquePtr()));

   void *InsertPos;
   if (auto *Ptr = FS.FindNodeOrInsertPos(ID, InsertPos)) {
      return DeclarationName(Ptr);
   }

   auto *Info = new(Ctx) DeclarationNameInfo(Kind, Ty);
   FS.InsertNode(Info, InsertPos);

   return DeclarationName(Info);
}

DeclarationName
DeclarationNameTable::getInfixOperatorName(const IdentifierInfo &II)
{
   return getSpecialName(DeclarationName::InfixOperatorName, II);
}

DeclarationName
DeclarationNameTable::getPrefixOperatorName(const IdentifierInfo &II)
{
   return getSpecialName(DeclarationName::PrefixOperatorName, II);
}

DeclarationName
DeclarationNameTable::getPostfixOperatorName(const IdentifierInfo &II)
{
   return getSpecialName(DeclarationName::PostfixOperatorName, II);
}

DeclarationName
DeclarationNameTable::getConversionOperatorName(QualType Ty)
{
   return getSpecialName(DeclarationName::ConversionOperatorName, Ty);
}

diag::DiagnosticBuilder &operator<<(diag::DiagnosticBuilder &builder,
                                    const DeclarationName &DN) {
   std::string s;
   {
      llvm::raw_string_ostream OS(s);
      DN.print(OS);
   }

   return builder << s;
}


} // namespace cdot