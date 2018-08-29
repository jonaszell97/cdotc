//
// Created by Jonas Zell on 25.03.18.
//

#include "DeclarationName.h"

#include "AST/ASTContext.h"
#include "AST/Decl.h"
#include "Message/Diagnostics.h"
#include "Module/Module.h"
#include "NestedNameSpecifier.h"
#include "Sema/Template.h"

#include <llvm/ADT/FoldingSet.h>

namespace cdot {

class DeclarationNameInfo: public llvm::FoldingSetNode {
   DeclarationName::DeclarationKind Kind;
   union {
      QualType Ty;
      const IdentifierInfo *II;
      uintptr_t Data1;
      DeclarationName::SubscriptKind SubKind;
   };

   union {
      uintptr_t Data2;
      const sema::FinalTemplateArgumentList *ArgList;
      unsigned Scope;
      DeclarationName::AccessorKind AccKind;
   };

public:
   DeclarationNameInfo(DeclarationName::DeclarationKind Kind, uintptr_t Data1,
                       uintptr_t Data2)
      : Kind(Kind), Data1(Data1), Data2(Data2)
   {}

   DeclarationName::DeclarationKind getKind() const
   {
      return Kind;
   }

   bool holdsType() const
   {
      switch (Kind) {
      case DeclarationName::ConversionOperatorName:
      case DeclarationName::ExtensionName:
      case DeclarationName::BaseConstructorName:
         return true;
      default:
         return false;
      }
   }

   bool holdsIdent() const
   {
      switch (Kind) {
      case DeclarationName::InfixOperatorName:
      case DeclarationName::PrefixOperatorName:
      case DeclarationName::PostfixOperatorName:
      case DeclarationName::AccessorName:
      case DeclarationName::MacroName:
      case DeclarationName::OperatorDeclName:
      case DeclarationName::LocalVarName:
         return true;
      default:
         return false;
      }
   }

   bool holdsDeclName() const
   {
      switch (Kind) {
      case DeclarationName::InstantiationName:
      case DeclarationName::LocalVarName:
      case DeclarationName::PackExpansionName:
      case DeclarationName::OperatorDeclName:
         return true;
      default:
         return false;
      }
   }

   bool holdsTemplateArgs() const
   {
      switch (Kind) {
      case DeclarationName::InstantiationName:
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
      assert(holdsIdent() && "does not hold an identifier!");
      return *II;
   }

   DeclarationName getDeclName() const
   {
      assert(holdsDeclName() && "does not hold a decl name!");
      return DeclarationName::getFromOpaquePtr((void*)Data1);
   }

   const sema::FinalTemplateArgumentList *getArgList() const
   {
      assert(holdsTemplateArgs() && "does not hold template args!");
      return ArgList;
   }

   unsigned getPackExpansionIndex() const
   {
      assert(Kind == DeclarationName::PackExpansionName);
      return (unsigned)Data2;
   }

   unsigned getClosureArgumentIdx() const
   {
      assert(Kind == DeclarationName::ClosureArgumentName);
      return (unsigned)Data1;
   }

   uintptr_t getData1() const { return Data1; }
   uintptr_t getData2() const { return Data2; }

   unsigned getBlockScope() const
   {
      assert(Kind == DeclarationName::LocalVarName);
      return Scope;
   }

   DeclarationName::AccessorKind getAccessorKind() const
   {
      assert(Kind == DeclarationName::AccessorName);
      return AccKind;
   }

   DeclarationName::SubscriptKind getSubscriptKind() const
   {
      assert(Kind == DeclarationName::SubscriptName);
      return SubKind;
   }

   static void Profile(llvm::FoldingSetNodeID &ID,
                       DeclarationName::DeclarationKind Kind,
                       uintptr_t Data1, uintptr_t Data2) {
      ID.AddInteger(Kind);
      ID.AddInteger(Data1);
      ID.AddInteger(Data2);
   }

   void Profile(llvm::FoldingSetNodeID &ID)
   {
      return Profile(ID, Kind, Data1, Data2);
   }
};

DeclarationName::DeclarationName(DeclarationNameInfo *DNI)
   : Val(reinterpret_cast<uintptr_t>(DNI))
{
   assert((Val & PtrMask) == 0 && "not sufficiently aligned!");
   Val |= OtherStoredName;
}

DeclarationName::DeclarationKind DeclarationName::getKind() const
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

IdentifierInfo* DeclarationName::getIdentifierInfo() const
{
   switch (getKind()) {
   case DeclarationName::NormalIdentifier:
      return reinterpret_cast<IdentifierInfo*>(Val);
   case DeclarationName::InfixOperatorName:
   case DeclarationName::PrefixOperatorName:
   case DeclarationName::PostfixOperatorName:
   case DeclarationName::MacroName:
      return const_cast<IdentifierInfo*>(&getDeclInfo()->getIdentifierInfo());
   case DeclarationName::LocalVarName:
      return getLocalVarName().getIdentifierInfo();
   case DeclarationName::OperatorDeclName:
      return getDeclaredOperatorName().getIdentifierInfo();
   case DeclarationName::InstantiationName:
      return getInstantiationName().getIdentifierInfo();
   default:
      return nullptr;
   }
}

QualType DeclarationName::getConstructorType() const
{
   if (getStoredKind() == StoredInitializerName)
      return QualType::getFromOpaquePtr(
         reinterpret_cast<void*>(Val & ~PtrMask));

   if (getKind() == BaseConstructorName)
      return getDeclInfo()->getType();

   return QualType();
}

const IdentifierInfo* DeclarationName::getInfixOperatorName() const
{
   if (getKind() == InfixOperatorName)
      return &getDeclInfo()->getIdentifierInfo();

   return nullptr;
}

const IdentifierInfo* DeclarationName::getPrefixOperatorName() const
{
   if (getKind() == PrefixOperatorName)
      return &getDeclInfo()->getIdentifierInfo();

   return nullptr;
}

const IdentifierInfo* DeclarationName::getPostfixOperatorName() const
{
   if (getKind() == PostfixOperatorName)
      return &getDeclInfo()->getIdentifierInfo();

   return nullptr;
}

const IdentifierInfo* DeclarationName::getAccessorName() const
{
   if (getKind() == AccessorName)
      return &getDeclInfo()->getIdentifierInfo();

   return nullptr;
}

const IdentifierInfo* DeclarationName::getMacroName() const
{
   if (getKind() == MacroName)
      return &getDeclInfo()->getIdentifierInfo();

   return nullptr;
}

DeclarationName::AccessorKind DeclarationName::getAccessorKind() const
{
   if (getKind() == AccessorName)
      return getDeclInfo()->getAccessorKind();

   llvm_unreachable("not an accessor name!");
}

DeclarationName::SubscriptKind DeclarationName::getSubscriptKind() const
{
   if (getKind() == SubscriptName)
      return getDeclInfo()->getSubscriptKind();

   llvm_unreachable("not a subscript name!");
}

unsigned DeclarationName::getClosureArgumentIdx() const
{
   if (getKind() == ClosureArgumentName)
      return getDeclInfo()->getClosureArgumentIdx();

   llvm_unreachable("not a closure argument name!");
}

DeclarationName DeclarationName::getInstantiationName() const
{
   if (getKind() == InstantiationName)
      return getDeclInfo()->getDeclName();

   return nullptr;
}

const sema::FinalTemplateArgumentList*
DeclarationName::getInstantiationArgs() const
{
   if (getKind() == InstantiationName)
      return getDeclInfo()->getArgList();

   return nullptr;
}

QualType DeclarationName::getConversionOperatorType() const
{
   if (getKind() == ConversionOperatorName)
      return getDeclInfo()->getType();

   return QualType();
}

QualType DeclarationName::getExtendedType() const
{
   if (getKind() == ExtensionName)
      return getDeclInfo()->getType();

   return QualType();
}

DeclarationName DeclarationName::getPackExpansionName() const
{
   if (getKind() == PackExpansionName)
      return getDeclInfo()->getDeclName();

   return DeclarationName();
}

unsigned DeclarationName::getPackExpansionIndex() const
{
   if (getKind() == PackExpansionName)
      return getDeclInfo()->getPackExpansionIndex();

   return unsigned(-1);
}

DeclarationName DeclarationName::getLocalVarName() const
{
   if (getKind() == LocalVarName)
      return getDeclInfo()->getDeclName();

   return DeclarationName();
}

unsigned DeclarationName::getLocalVarScope() const
{
   if (getKind() == LocalVarName)
      return getDeclInfo()->getBlockScope();

   return 0;
}

DeclarationName DeclarationName::getDeclaredOperatorName() const
{
   if (getKind() == OperatorDeclName)
      return getDeclInfo()->getDeclName();

   return DeclarationName();
}

int DeclarationName::compare(const DeclarationName &RHS) const
{
   auto OwnKind = getKind();
   auto OtherKind = RHS.getKind();

   if (OwnKind != OtherKind)
      return (OwnKind < OtherKind) ? -1 : 1;

   switch (getKind()) {
   case NormalIdentifier:
      return getIdentifierInfo()->getIdentifier().compare(
         RHS.getIdentifierInfo()->getIdentifier());
   case InfixOperatorName:
      return getInfixOperatorName()->getIdentifier().compare(
         RHS.getIdentifierInfo()->getIdentifier());
   case PrefixOperatorName:
      return getPrefixOperatorName()->getIdentifier().compare(
         RHS.getIdentifierInfo()->getIdentifier());
   case PostfixOperatorName:
      return getPostfixOperatorName()->getIdentifier().compare(
         RHS.getIdentifierInfo()->getIdentifier());
   case InstantiationName:
      return getInstantiationName().compare(RHS.getInstantiationName());
   default:
      return (int)(getDeclInfo()->getData1() - RHS.getDeclInfo()->getData1());
   }
}

void DeclarationName::dump() const
{
   print(llvm::errs());
}

void DeclarationName::print(llvm::raw_ostream &OS) const
{
   if (!*this) {
      OS << "<invalid name>";
      return;
   }

   switch (getKind()) {
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
   case ExtensionName:
      OS << getExtendedType();
      break;
   case ConstructorName:
   case BaseConstructorName:
      OS << "init";
      break;
   case DestructorName:
      OS << "deinit";
      break;
   case PackExpansionName:
      OS << getPackExpansionName() << getPackExpansionIndex();
      break;
   case LocalVarName:
      OS << getLocalVarName();
      break;
   case OperatorDeclName:
      OS << getDeclaredOperatorName();
      break;
   case AccessorName: {
      auto AccKind = getAccessorKind();
      switch (AccKind) {
      case Getter:
         OS << "get ";
         break;
      case Setter:
         OS << "set ";
         break;
      }

      OS << getAccessorName()->getIdentifier();
      break;
   }
   case ClosureArgumentName:
      OS << "$" << getClosureArgumentIdx();
      break;
   case MacroName:
      OS << getMacroName()->getIdentifier() << "!";
      break;
   case InstantiationName:
      OS << getInstantiationName() << *getInstantiationArgs();
      break;
   case ErrorName:
      OS << "<invalid name>";
      break;
   case SubscriptName:
      OS << "subscript";
      break;
   }
}

std::string DeclarationName::toString() const
{
   std::string str;
   {
      llvm::raw_string_ostream OS(str);
      OS << *this;
   }

   return str;
}

DeclarationName DeclarationName::getManglingName() const
{
   if (getKind() == InstantiationName)
      return getDeclInfo()->getDeclName();

   if (getKind() == OperatorDeclName)
      return getDeclInfo()->getDeclName();

   if (getKind() == MacroName)
      return getDeclInfo()->getIdentifierInfo();

   return *this;
}

using FoldingSetTy = llvm::FoldingSet<DeclarationNameInfo>;
using NameSpecSetTy = llvm::FoldingSet<NestedNameSpecifier>;

DeclarationNameTable::DeclarationNameTable(ast::ASTContext &Ctx)
   : FoldingSetPtr(new FoldingSetTy()),
     Ctx(Ctx),
     ErrorName(getSpecialName(DeclarationName::ErrorName, 0, 0)),
     NestedNameSpecifiers(new NameSpecSetTy())
{

}

DeclarationNameTable::~DeclarationNameTable()
{
   delete reinterpret_cast<FoldingSetTy*>(FoldingSetPtr);
   delete reinterpret_cast<NameSpecSetTy*>(NestedNameSpecifiers);
}

DeclarationName
DeclarationNameTable::getIdentifiedName(DeclarationName::DeclarationKind Kind,
                                        const cdot::IdentifierInfo &II) {
   switch (Kind) {
   case DeclarationName::NormalIdentifier:
      return getNormalIdentifier(II);
   case DeclarationName::PrefixOperatorName:
      return getPrefixOperatorName(II);
   case DeclarationName::PostfixOperatorName:
      return getPostfixOperatorName(II);
   case DeclarationName::InfixOperatorName:
      return getInfixOperatorName(II);
   case DeclarationName::MacroName:
      return getMacroName(II);
   default:
      llvm_unreachable("not an identified name");
   }
}

DeclarationName
DeclarationNameTable::getTypedName(DeclarationName::DeclarationKind Kind,
                                   QualType Ty) {
   switch (Kind) {
   case DeclarationName::ConstructorName:
   case DeclarationName::BaseConstructorName:
      return getConstructorName(Ty, Kind == DeclarationName::ConstructorName);
   case DeclarationName::DestructorName:
      return getDestructorName(Ty);
   case DeclarationName::ExtensionName:
      return getExtensionName(Ty);
   case DeclarationName::ConversionOperatorName:
      return getConversionOperatorName(Ty);
   default:
      llvm_unreachable("not a typed name");
   }
}

DeclarationName
DeclarationNameTable::getNormalIdentifier(const IdentifierInfo &II)
{
   return DeclarationName(II);
}

DeclarationName
DeclarationNameTable::getConstructorName(QualType ConstructedType,
                                         bool IsCompleteCtor) {
   if (IsCompleteCtor)
      return DeclarationName(ConstructedType,
                             DeclarationName::ConstructorName);

   return getSpecialName(DeclarationName::BaseConstructorName,
                         (uintptr_t)ConstructedType.getAsOpaquePtr());
}

DeclarationName
DeclarationNameTable::getDestructorName(QualType DestructedType)
{
   return DeclarationName(DestructedType, DeclarationName::DestructorName);
}

DeclarationName
DeclarationNameTable::getOperatorDeclName(DeclarationName OpName)
{
   return getSpecialName(DeclarationName::OperatorDeclName,
                         (uintptr_t)OpName.getAsOpaquePtr());
}

DeclarationName DeclarationNameTable::getErrorName()
{
   return ErrorName;
}

DeclarationName
DeclarationNameTable::getSpecialName(DeclarationName::DeclarationKind Kind,
                                     uintptr_t Data1, uintptr_t Data2) {
   auto &FS = *reinterpret_cast<FoldingSetTy*>(FoldingSetPtr);

   llvm::FoldingSetNodeID ID;
   DeclarationNameInfo::Profile(ID, Kind, Data1, Data2);

   void *InsertPos;
   if (auto *Ptr = FS.FindNodeOrInsertPos(ID, InsertPos)) {
      return DeclarationName(Ptr);
   }

   auto *Info = new(Ctx) DeclarationNameInfo(Kind, Data1, Data2);
   FS.InsertNode(Info, InsertPos);

   return DeclarationName(Info);
}

DeclarationName
DeclarationNameTable::getInfixOperatorName(const IdentifierInfo &II)
{
   return getSpecialName(DeclarationName::InfixOperatorName,
                         reinterpret_cast<uintptr_t>(&II));
}

DeclarationName
DeclarationNameTable::getPrefixOperatorName(const IdentifierInfo &II)
{
   return getSpecialName(DeclarationName::PrefixOperatorName,
                         reinterpret_cast<uintptr_t>(&II));
}

DeclarationName
DeclarationNameTable::getPostfixOperatorName(const IdentifierInfo &II)
{
   return getSpecialName(DeclarationName::PostfixOperatorName,
                         reinterpret_cast<uintptr_t>(&II));
}

DeclarationName
DeclarationNameTable::getMacroName(const IdentifierInfo &II)
{
   return getSpecialName(DeclarationName::MacroName,
                         reinterpret_cast<uintptr_t>(&II));
}

DeclarationName
DeclarationNameTable::getConversionOperatorName(QualType Ty)
{
   return getSpecialName(DeclarationName::ConversionOperatorName,
                         (uintptr_t)Ty.getAsOpaquePtr());
}

DeclarationName DeclarationNameTable::getExtensionName(QualType ExtendedType)
{
   return getSpecialName(DeclarationName::ExtensionName,
                         (uintptr_t)ExtendedType.getAsOpaquePtr());
}

DeclarationName
DeclarationNameTable::getSubscriptName(DeclarationName::SubscriptKind Kind)
{
   return getSpecialName(DeclarationName::SubscriptName, (uintptr_t)Kind);
}

DeclarationName DeclarationNameTable::getClosureArgumentName(unsigned ArgNo)
{
   return getSpecialName(DeclarationName::ClosureArgumentName,
                         (uintptr_t)ArgNo);
}

DeclarationName DeclarationNameTable::getLocalVarName(DeclarationName Name,
                                                      unsigned Scope) {
   return getSpecialName(DeclarationName::LocalVarName,
                         (uintptr_t)Name.getAsOpaquePtr(),
                         (uintptr_t)Scope);
}

DeclarationName DeclarationNameTable::getPackExpansionName(DeclarationName Name,
                                                           unsigned idx) {
   return getSpecialName(DeclarationName::PackExpansionName,
                         (uintptr_t)Name.getAsOpaquePtr(),
                         idx);
}

DeclarationName
DeclarationNameTable::getAccessorName(const IdentifierInfo &II,
                                      DeclarationName::AccessorKind Kind) {
   return getSpecialName(DeclarationName::AccessorName, (uintptr_t)&II, Kind);
}

DeclarationName
DeclarationNameTable::getInstantiationName(DeclarationName Name,
                                           const sema::FinalTemplateArgumentList
                                                                     &ArgList) {
   auto &FS = *reinterpret_cast<FoldingSetTy*>(FoldingSetPtr);

   llvm::FoldingSetNodeID ID;
   DeclarationNameInfo::Profile(
      ID, DeclarationName::InstantiationName,
      reinterpret_cast<uintptr_t>(Name.getAsOpaquePtr()),
      reinterpret_cast<uintptr_t>(&ArgList));

   void *InsertPos;
   if (auto *Ptr = FS.FindNodeOrInsertPos(ID, InsertPos)) {
      return DeclarationName(Ptr);
   }

   auto *Info = new(Ctx) DeclarationNameInfo(
      DeclarationName::InstantiationName,
      reinterpret_cast<uintptr_t>(Name.getAsOpaquePtr()),
      reinterpret_cast<uintptr_t>(&ArgList));

   FS.InsertNode(Info, InsertPos);

   return DeclarationName(Info);
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

diag::DiagnosticBuilder &operator<<(diag::DiagnosticBuilder &builder,
                                    IdentifierInfo *II) {
   return builder << (II ? II->getIdentifier() : "");
}

} // namespace cdot