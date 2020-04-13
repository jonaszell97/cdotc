#include "NestedNameSpecifier.h"

#include "AST/ASTContext.h"
#include "AST/Decl.h"
#include "Message/Diagnostics.h"
#include "Module/Module.h"

#include <llvm/Support/raw_ostream.h>

using namespace cdot;

NestedNameSpecifier::NestedNameSpecifier(QualType Type,
                                         NestedNameSpecifier *Previous)
   : PreviousAndKind(Previous, Kind::Type), Data(Type.getAsOpaquePtr())
{

}

NestedNameSpecifier::NestedNameSpecifier(ast::NamespaceDecl *NS,
                                         NestedNameSpecifier *Previous)
   : PreviousAndKind(Previous, Kind::Namespace), Data(NS)
{

}

NestedNameSpecifier::NestedNameSpecifier(IdentifierInfo *Ident,
                                         NestedNameSpecifier *Previous)
   : PreviousAndKind(Previous, Kind::Identifier), Data(Ident)
{

}

NestedNameSpecifier::NestedNameSpecifier(ast::TemplateParamDecl *TP,
                                         NestedNameSpecifier *Previous)
   : PreviousAndKind(Previous, Kind::TemplateParam), Data(TP)
{

}

NestedNameSpecifier::NestedNameSpecifier(ast::AssociatedTypeDecl *AT,
                                         NestedNameSpecifier *Previous)
   : PreviousAndKind(Previous, Kind::AssociatedType), Data(AT)
{

}

NestedNameSpecifier::NestedNameSpecifier(ast::AliasDecl *Alias,
                                         NestedNameSpecifier *Previous)
   : PreviousAndKind(Previous, Kind::Alias), Data(Alias)
{

}

NestedNameSpecifier::NestedNameSpecifier(cdot::Module *M,
                                         NestedNameSpecifier *Previous)
   : PreviousAndKind(Previous, Kind::Module), Data(M)
{

}

NestedNameSpecifier::NestedNameSpecifier(sema::FinalTemplateArgumentList*TemplateArgs,
                                         NestedNameSpecifier *Previous)
   : PreviousAndKind(Previous, Kind::TemplateArgList), Data(TemplateArgs)
{

}

using NameSpecSetTy = llvm::FoldingSet<NestedNameSpecifier>;

NestedNameSpecifier* NestedNameSpecifier::Create(DeclarationNameTable &Tbl,
                                                 QualType Type,
                                                 NestedNameSpecifier *Previous){
   llvm::FoldingSetNodeID ID;
   NestedNameSpecifier::Profile(ID, NestedNameSpecifier::Type, Previous,
                                Type.getAsOpaquePtr());

   auto *Set = reinterpret_cast<NameSpecSetTy*>(Tbl.NestedNameSpecifiers);
   void *InsertPos;
   if (auto *Name = Set->FindNodeOrInsertPos(ID, InsertPos))
      return Name;

   auto *Name = new(Tbl.Ctx) NestedNameSpecifier(Type, Previous);
   Set->InsertNode(Name, InsertPos);

   return Name;
}

NestedNameSpecifier* NestedNameSpecifier::Create(DeclarationNameTable &Tbl,
                                                 ast::NamespaceDecl *NS,
                                                 NestedNameSpecifier *Previous){
   llvm::FoldingSetNodeID ID;
   NestedNameSpecifier::Profile(ID, NestedNameSpecifier::Namespace,
                                Previous, NS);

   auto *Set = reinterpret_cast<NameSpecSetTy*>(Tbl.NestedNameSpecifiers);
   void *InsertPos;
   if (auto *Name = Set->FindNodeOrInsertPos(ID, InsertPos))
      return Name;

   auto *Name = new(Tbl.Ctx) NestedNameSpecifier(NS, Previous);
   Set->InsertNode(Name, InsertPos);

   return Name;
}

NestedNameSpecifier* NestedNameSpecifier::Create(DeclarationNameTable &Tbl,
                                                 IdentifierInfo *Ident,
                                                 NestedNameSpecifier *Previous){
   llvm::FoldingSetNodeID ID;
   NestedNameSpecifier::Profile(ID, NestedNameSpecifier::Identifier,
                                Previous, Ident);

   auto *Set = reinterpret_cast<NameSpecSetTy*>(Tbl.NestedNameSpecifiers);
   void *InsertPos;
   if (auto *Name = Set->FindNodeOrInsertPos(ID, InsertPos))
      return Name;

   auto *Name = new(Tbl.Ctx) NestedNameSpecifier(Ident, Previous);
   Set->InsertNode(Name, InsertPos);

   return Name;
}

NestedNameSpecifier* NestedNameSpecifier::Create(DeclarationNameTable &Tbl,
                                                 ast::TemplateParamDecl *TP,
                                                 NestedNameSpecifier *Previous){
   llvm::FoldingSetNodeID ID;
   NestedNameSpecifier::Profile(ID, NestedNameSpecifier::TemplateParam,
                                Previous, TP);

   auto *Set = reinterpret_cast<NameSpecSetTy*>(Tbl.NestedNameSpecifiers);
   void *InsertPos;
   if (auto *Name = Set->FindNodeOrInsertPos(ID, InsertPos))
      return Name;

   auto *Name = new(Tbl.Ctx) NestedNameSpecifier(TP, Previous);
   Set->InsertNode(Name, InsertPos);

   return Name;
}

NestedNameSpecifier* NestedNameSpecifier::Create(DeclarationNameTable &Tbl,
                                                 ast::AssociatedTypeDecl *AT,
                                                 NestedNameSpecifier *Previous){
   llvm::FoldingSetNodeID ID;
   NestedNameSpecifier::Profile(ID, NestedNameSpecifier::AssociatedType,
                                Previous, AT);

   auto *Set = reinterpret_cast<NameSpecSetTy*>(Tbl.NestedNameSpecifiers);
   void *InsertPos;
   if (auto *Name = Set->FindNodeOrInsertPos(ID, InsertPos))
      return Name;

   auto *Name = new(Tbl.Ctx) NestedNameSpecifier(AT, Previous);
   Set->InsertNode(Name, InsertPos);

   return Name;
}

NestedNameSpecifier* NestedNameSpecifier::Create(DeclarationNameTable &Tbl,
                                                 ast::AliasDecl *Alias,
                                                 NestedNameSpecifier *Previous){
   llvm::FoldingSetNodeID ID;
   NestedNameSpecifier::Profile(ID, NestedNameSpecifier::Alias,
                                Previous, Alias);

   auto *Set = reinterpret_cast<NameSpecSetTy*>(Tbl.NestedNameSpecifiers);
   void *InsertPos;
   if (auto *Name = Set->FindNodeOrInsertPos(ID, InsertPos))
      return Name;

   auto *Name = new(Tbl.Ctx) NestedNameSpecifier(Alias, Previous);
   Set->InsertNode(Name, InsertPos);

   return Name;
}

NestedNameSpecifier* NestedNameSpecifier::Create(DeclarationNameTable &Tbl,
                                                 cdot::Module *M,
                                                 NestedNameSpecifier *Previous){
   llvm::FoldingSetNodeID ID;
   NestedNameSpecifier::Profile(ID, NestedNameSpecifier::Module, Previous, M);

   auto *Set = reinterpret_cast<NameSpecSetTy*>(Tbl.NestedNameSpecifiers);
   void *InsertPos;
   if (auto *Name = Set->FindNodeOrInsertPos(ID, InsertPos))
      return Name;

   auto *Name = new(Tbl.Ctx) NestedNameSpecifier(M, Previous);
   Set->InsertNode(Name, InsertPos);

   return Name;
}

NestedNameSpecifier* NestedNameSpecifier::Create(DeclarationNameTable &Tbl,
                                                 sema::FinalTemplateArgumentList *TemplateArgs,
                                                 NestedNameSpecifier *Previous){
   llvm::FoldingSetNodeID ID;
   NestedNameSpecifier::Profile(ID, NestedNameSpecifier::TemplateArgList,
                                Previous, TemplateArgs);

   auto *Set = reinterpret_cast<NameSpecSetTy*>(Tbl.NestedNameSpecifiers);
   void *InsertPos;
   if (auto *Name = Set->FindNodeOrInsertPos(ID, InsertPos))
      return Name;

   auto *Name = new(Tbl.Ctx) NestedNameSpecifier(TemplateArgs, Previous);
   Set->InsertNode(Name, InsertPos);

   return Name;
}

void NestedNameSpecifier::Profile(llvm::FoldingSetNodeID &ID) const
{
   Profile(ID, getKind(), getPrevious(), Data);
}

void NestedNameSpecifier::Profile(llvm::FoldingSetNodeID &ID,
                                  Kind K,
                                  NestedNameSpecifier *Previous,
                                  void *Data) {
   ID.AddInteger(K);
   ID.AddPointer(Previous);
   ID.AddPointer(Data);
}

void NestedNameSpecifier::dump() const
{
   print(llvm::errs());
}

void NestedNameSpecifier::print(llvm::raw_ostream &OS) const
{
   SmallVector<const NestedNameSpecifier*, 4> Names{ this };
   auto *Prev = getPrevious();
   while (Prev) {
      Names.push_back(Prev);
      Prev = Prev->getPrevious();
   }

   unsigned i = 0;
   for (auto it = Names.rbegin(), end = Names.rend(); it != end; ++it) {
      if (i++ != 0 && (*it)->getKind() != TemplateArgList)
         OS << ".";

      switch ((*it)->getKind()) {
      case NestedNameSpecifier::Identifier:
         OS << (*it)->getIdentifier()->getIdentifier();
         break;
      case NestedNameSpecifier::Type:
         OS << (*it)->getType();
         break;
      case NestedNameSpecifier::Namespace:
         OS << (*it)->getNamespace()->getDeclName();
         break;
      case NestedNameSpecifier::TemplateParam:
         OS << (*it)->getParam()->getDeclName();
         break;
      case NestedNameSpecifier::AssociatedType:
         OS << (*it)->getAssociatedType()->getDeclName();
         break;
      case NestedNameSpecifier::Alias:
         OS << (*it)->getAlias()->getDeclName();
         break;
      case NestedNameSpecifier::Module:
         OS << (*it)->getModule()->getFullName();
         break;
      case NestedNameSpecifier::TemplateArgList:
         OS << *(*it)->getTemplateArgs();
         break;
      }
   }
}

bool NestedNameSpecifier::isDependent() const
{
   switch (getKind()) {
   case NestedNameSpecifier::Identifier:
   case NestedNameSpecifier::Namespace:
   case NestedNameSpecifier::Module:
      return false;
   case NestedNameSpecifier::Type:
      return getType()->isDependentType();
   case NestedNameSpecifier::AssociatedType: {
      return false;
   }
   case NestedNameSpecifier::TemplateParam:
   case NestedNameSpecifier::TemplateArgList:
      return true;
   case NestedNameSpecifier::Alias:
      return getAlias()->isTemplateOrInTemplate();
   }
}

bool NestedNameSpecifier::isAnyNameDependent() const
{
   if (isDependent())
      return true;

   if (auto *Prev = getPrevious())
      return Prev->isAnyNameDependent();

   return false;
}

unsigned NestedNameSpecifier::depth() const
{
   auto *Curr = this;
   unsigned i = 0;

   while (Curr) {
      ++i;
      Curr = Curr->getPrevious();
   }

   return i;
}

IdentifierInfo* NestedNameSpecifier::getIdentifier() const
{
   assert(getKind() == Kind::Identifier && "not an identifier!");
   return reinterpret_cast<IdentifierInfo*>(Data);
}

ast::NamespaceDecl* NestedNameSpecifier::getNamespace() const
{
   assert(getKind() == Kind::Namespace && "not a namespace!");
   return reinterpret_cast<ast::NamespaceDecl*>(Data);
}

QualType NestedNameSpecifier::getType() const
{
   assert(getKind() == Kind::Type && "not a type!");
   return QualType::getFromOpaquePtr(Data);
}

ast::TemplateParamDecl* NestedNameSpecifier::getParam() const
{
   assert(getKind() == Kind::TemplateParam && "not a template param!");
   return reinterpret_cast<ast::TemplateParamDecl*>(Data);
}

ast::AssociatedTypeDecl* NestedNameSpecifier::getAssociatedType() const
{
   assert(getKind() == Kind::AssociatedType && "not an associated type!");
   return reinterpret_cast<ast::AssociatedTypeDecl*>(Data);
}

ast::AliasDecl* NestedNameSpecifier::getAlias() const
{
   assert(getKind() == Kind::Alias && "not an alias!");
   return reinterpret_cast<ast::AliasDecl*>(Data);
}

cdot::Module* NestedNameSpecifier::getModule() const
{
   assert(getKind() == Kind::Module && "not an associated type!");
   return reinterpret_cast<cdot::Module*>(Data);
}

sema::FinalTemplateArgumentList* NestedNameSpecifier::getTemplateArgs() const
{
   assert(getKind() == Kind::TemplateArgList && "not an template arg list!");
   return reinterpret_cast<sema::FinalTemplateArgumentList*>(Data);
}

NestedNameSpecifierWithLoc::NestedNameSpecifierWithLoc(
                                             NestedNameSpecifier *Name,
                                             ArrayRef<SourceRange> Locs)
   : NameSpec(Name), NumSourceRanges((unsigned)Locs.size())
{
   std::copy(Locs.begin(), Locs.end(), getTrailingObjects<SourceRange>());
}

NestedNameSpecifierWithLoc::NestedNameSpecifierWithLoc(
                                             NestedNameSpecifierWithLoc *Prev,
                                             NestedNameSpecifier *NewName,
                                             SourceRange NewLoc)
   : NameSpec(NewName),
     NumSourceRanges(Prev->NumSourceRanges + 1)
{
   auto Locs = Prev->getSourceRanges();
   std::copy(Locs.begin(), Locs.end(), getTrailingObjects<SourceRange>());
   *(getTrailingObjects<SourceRange>() + NumSourceRanges - 1) = NewLoc;
}

NestedNameSpecifierWithLoc*
NestedNameSpecifierWithLoc::Create(DeclarationNameTable &Tbl,
                                   NestedNameSpecifier *Name,
                                   ArrayRef<SourceRange> Locs) {
   assert(Locs.size() == Name->depth() && "wrong amount of source ranges!");

   void *Mem = Tbl.Ctx.Allocate(totalSizeToAlloc<SourceRange>(Locs.size()),
                                alignof(NestedNameSpecifierWithLoc));

   return new(Mem) NestedNameSpecifierWithLoc(Name, Locs);
}

NestedNameSpecifierWithLoc*
NestedNameSpecifierWithLoc::Create(DeclarationNameTable &Tbl,
                                   NestedNameSpecifierWithLoc *Prev,
                                   NestedNameSpecifier *NewName,
                                   SourceRange NewLoc) {
   auto Locs = Prev->getSourceRanges();
   void *Mem = Tbl.Ctx.Allocate(totalSizeToAlloc<SourceRange>(Locs.size() + 1),
                                alignof(NestedNameSpecifierWithLoc));

   return new(Mem) NestedNameSpecifierWithLoc(Prev, NewName, NewLoc);
}

SourceRange NestedNameSpecifierWithLoc::getSourceRange(unsigned i) const
{
   assert(i < NumSourceRanges && "index out of bounds!");
   return getTrailingObjects<SourceRange>()[i];
}

SourceRange NestedNameSpecifierWithLoc::getFullRange() const
{
   auto Ranges = getSourceRanges();
   return SourceRange(Ranges.front().getStart(), Ranges.back().getEnd());
}

diag::DiagnosticBuilder &cdot::operator<<(diag::DiagnosticBuilder &builder,
                                          NestedNameSpecifier *Name) {
   std::string str;
   llvm::raw_string_ostream OS(str);
   OS << Name;

   return builder << OS.str();
}

llvm::raw_ostream &cdot::operator<<(llvm::raw_ostream &OS,
                                    NestedNameSpecifier *Name) {
   Name->print(OS);
   return OS;
}