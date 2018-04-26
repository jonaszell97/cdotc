//
// Created by Jonas Zell on 25.03.18.
//

#include "DeclarationName.h"

#include "AST/ASTContext.h"
#include "Message/Diagnostics.h"
#include "Sema/Template.h"

#include <llvm/ADT/FoldingSet.h>

namespace cdot {

class DeclarationNameInfo: public llvm::FoldingSetNode {
   DeclarationName::DeclarationKind Kind;
   union {
      QualType Ty;
      const IdentifierInfo *II;
      uintptr_t Data1;
   };

   union {
      uintptr_t Data2;
      const sema::FinalTemplateArgumentList *ArgList;
      BlockScope *Scope;
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

   BlockScope *getBlockScope() const
   {
      assert(Kind == DeclarationName::LocalVarName);
      return Scope;
   }

   DeclarationName::AccessorKind getAccessorKind() const
   {
      assert(Kind == DeclarationName::AccessorName);
      return AccKind;
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

QualType DeclarationName::getConstructorType() const
{
   if (getStoredKind() == StoredInitializerName)
      return QualType::getFromOpaquePtr(
         reinterpret_cast<void*>(Val & ~PtrMask));

   if (getDeclarationKind() == BaseConstructorName)
      return getDeclInfo()->getType();

   return QualType();
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

const IdentifierInfo* DeclarationName::getAccessorName() const
{
   if (getDeclarationKind() == AccessorName)
      return &getDeclInfo()->getIdentifierInfo();

   return nullptr;
}

const IdentifierInfo* DeclarationName::getMacroName() const
{
   if (getDeclarationKind() == MacroName)
      return &getDeclInfo()->getIdentifierInfo();

   return nullptr;
}

DeclarationName::AccessorKind DeclarationName::getAccessorKind() const
{
   if (getDeclarationKind() == AccessorName)
      return getDeclInfo()->getAccessorKind();

   llvm_unreachable("not an accessor name!");
}

unsigned DeclarationName::getClosureArgumentIdx() const
{
   if (getDeclarationKind() == ClosureArgumentName)
      return getDeclInfo()->getClosureArgumentIdx();

   llvm_unreachable("not a closure argument name!");
}

DeclarationName DeclarationName::getInstantiationName() const
{
   if (getDeclarationKind() == InstantiationName)
      return getDeclInfo()->getDeclName();

   return nullptr;
}

const sema::FinalTemplateArgumentList*
DeclarationName::getInstantiationArgs() const
{
   if (getDeclarationKind() == InstantiationName)
      return getDeclInfo()->getArgList();

   return nullptr;
}

QualType DeclarationName::getConversionOperatorType() const
{
   if (getDeclarationKind() == ConversionOperatorName)
      return getDeclInfo()->getType();

   return QualType();
}

QualType DeclarationName::getExtendedType() const
{
   if (getDeclarationKind() == ExtensionName)
      return getDeclInfo()->getType();

   return QualType();
}

DeclarationName DeclarationName::getPackExpansionName() const
{
   if (getDeclarationKind() == PackExpansionName)
      return getDeclInfo()->getDeclName();

   return DeclarationName();
}

unsigned DeclarationName::getPackExpansionIndex() const
{
   if (getDeclarationKind() == PackExpansionName)
      return getDeclInfo()->getPackExpansionIndex();

   return unsigned(-1);
}

DeclarationName DeclarationName::getLocalVarName() const
{
   if (getDeclarationKind() == LocalVarName)
      return getDeclInfo()->getDeclName();

   return DeclarationName();
}

BlockScope* DeclarationName::getLocalVarScope() const
{
   if (getDeclarationKind() == LocalVarName)
      return getDeclInfo()->getBlockScope();

   return nullptr;
}

DeclarationName DeclarationName::getDeclaredOperatorName() const
{
   if (getDeclarationKind() == OperatorDeclName)
      return getDeclInfo()->getDeclName();

   return DeclarationName();
}

int DeclarationName::compare(const DeclarationName &RHS) const
{
   auto OwnKind = getDeclarationKind();
   auto OtherKind = RHS.getDeclarationKind();

   if (OwnKind != OtherKind)
      return (OwnKind < OtherKind) ? -1 : 1;

   switch (getDeclarationKind()) {
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
   }
}

DeclarationName DeclarationName::getManglingName() const
{
   if (getDeclarationKind() == InstantiationName)
      return getDeclInfo()->getDeclName();

   if (getDeclarationKind() == OperatorDeclName)
      return getDeclInfo()->getDeclName();

   if (getDeclarationKind() == MacroName)
      return getDeclInfo()->getIdentifierInfo();

   return *this;
}

using FoldingSetTy = llvm::FoldingSet<DeclarationNameInfo>;

DeclarationNameTable::DeclarationNameTable(ast::ASTContext &Ctx)
   : FoldingSetPtr(new FoldingSetTy()),
     Ctx(Ctx),
     ErrorName(getSpecialName(DeclarationName::ErrorName, 0, 0))
{

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

DeclarationName DeclarationNameTable::getClosureArgumentName(unsigned ArgNo)
{
   return getSpecialName(DeclarationName::ClosureArgumentName,
                         (uintptr_t)ArgNo);
}

DeclarationName DeclarationNameTable::getLocalVarName(DeclarationName Name,
                                                      BlockScope *Scope) {
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


} // namespace cdot