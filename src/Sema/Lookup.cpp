
#include "cdotc/Sema/Lookup.h"

#include "cdotc/AST/Decl.h"
#include "cdotc/Basic/NestedNameSpecifier.h"
#include "cdotc/Parse/Parser.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Support/Log.h"

#include <unordered_set>

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::diag;
using namespace cdot::sema;
using namespace cdot::support;

namespace {

class LookupResolver {
private:
   enum PreparationLevel : unsigned {
      /// All macros have been resolved.
      Macros = 0x1,

      /// All static if / for declarations have been resolved.
      StaticDecls = 0x2,

      /// All using and import declarations are resolved.
      Imports = 0x4,

      /// Extensions have been matched and applied.
      ExtensionsApplied = 0x8,

      /// Implicit declarations are implemented.
      ImplicitDecls = 0x10,

      /// Associated types are fully resolved.
      AssociatedTypes = 0x20,

      /// Protocol conformances were checked.
      ProtocolConformances = 0x40,

      /// The type is ready for complete name lookup.
      Complete = ~0x0u,
   };

   /// Reference to the query context.
   QueryContext &QC;

   /// The base decl context.
   DeclContext *BaseDC;

   /// DeclContexts we have yet to visit.
   llvm::SetVector<DeclContext*> allDecls;

   /// Builtin decls that need to be resolved earlier than others.
   llvm::DenseSet<RecordDecl*> builtinDecls;

   /// Prepare a specific level of lookup.
   bool PrepareMacros(DeclContext *DC);
   bool PrepareStaticDecls(DeclContext *DC);
   bool PrepareImports(DeclContext *DC);
   bool PrepareUsings(DeclContext *DC);
   bool PrepareImplicitDecls(DeclContext *DC);

   /// Resolve name lookup in the DeclContext.
   bool FindDeclContexts(DeclContext *DC);

public:
   /// Create the lookup resolver for the base decl context.
   LookupResolver(QueryContext &QC, DeclContext *BaseDC)
      : QC(QC), BaseDC(BaseDC), currentNode(nullptr)
   {

   }

   /// D'tor.
   ~LookupResolver()
   {
      for (auto &node : nodes)
      {
         delete &node;
      }
   }

   /// Run the lookup resolver.
   bool Run(bool gatherDeclContexts = true);
};

DeclarationName getNameForProtocolExtensionDecl(ASTContext& C, NamedDecl* D, RecordDecl* Inst)
{
   auto DN = D->getDeclName();
   switch (DN.getKind()) {
   case DeclarationName::ConstructorName:
   case DeclarationName::BaseConstructorName:
      return C.getDeclNameTable().getConstructorName(
         C.getRecordType(Inst),
         DN.getKind() == DeclarationName::ConstructorName);
   case DeclarationName::DestructorName:
      return C.getDeclNameTable().getDestructorName(C.getRecordType(Inst));
   default:
      return DN;
   }
}

} // anonymous namespace

bool LookupResolver::Run(bool gatherDeclContexts)
{
   if (gatherDeclContexts) {
      if (FindDeclContexts(BaseDC)) {
         return true;
      }
   }

   // Expand macros, static declarations and imports.
   // Macros and static ifs use restricted lookup so they are safe to do here,
   // while imports can only refer to other modules.
   for (auto *DC : allDecls) {
      PrepareImports(DC);

      if (!isa<RecordDecl>(DC)) {
         PrepareUsings(DC);
         QC.Sema->updateLookupLevel(DC, LookupLevel::Complete);
      }
   }

   // Resolve associated types of protocols.
   for (auto *DC : allDecls) {
      auto *R = dyn_cast<ProtocolDecl>(DC);
      if (!R) {
         continue;
      }

      SemaPass::DeclScopeRAII DSR(*QC.Sema, DC);
      if (QC.ResolveAssociatedTypes(QC.Context.getRecordType(R))) {
         return true;
      }
   }

   // Resolve associated types of known builtin types.
   for (auto *R : builtinDecls) {
      SemaPass::DeclScopeRAII DSR(*QC.Sema, R);
      if (QC.ResolveAssociatedTypes(QC.Context.getRecordType(R))) {
         return true;
      }
   }

   // Check protocol extensions.
   for (auto *DC : allDecls) {
      auto *P = dyn_cast<ProtocolDecl>(DC);
      if (!P) {
         continue;
      }

      SemaPass::DeclScopeRAII DSR(*QC.Sema, DC);
      for (auto *Ext : P->getExtensions()) {
         if (QC.PrepareDeclInterface(Ext)) {
            return true;
         }
      }
   }

   // Check protocol conformances.
   for (auto *DC : allDecls) {
      auto *R = dyn_cast<ProtocolDecl>(DC);
      if (!R) {
         continue;
      }

      QualType T = QC.Context.getRecordType(R);
      if (QC.CheckConformances(T)) {
         return true;
      }
   }

   // Resolve associated types of other records.
   for (auto *DC : allDecls) {
      auto *R = dyn_cast<RecordDecl>(DC);
      if (!R || isa<ProtocolDecl>(R) || builtinDecls.count(R) != 0) {
         continue;
      }

      SemaPass::DeclScopeRAII DSR(*QC.Sema, DC);
      if (QC.ResolveAssociatedTypes(QC.Context.getRecordType(R))) {
         return true;
      }
   }

   // Declare implicit declarations.
   for (auto *DC : allDecls) {
      if (PrepareImplicitDecls(DC)) {
         return true;
      }
   }

   // Check conformances of other records.
   std::unordered_map<DeclContext*, std::vector<NamedDecl*>> ExtensionDecls;
   for (auto *DC : allDecls) {
      auto *R = dyn_cast<RecordDecl>(DC);
      if (!R || isa<ProtocolDecl>(R)) {
         continue;
      }

      QualType T = QC.Context.getRecordType(R);
      if (QC.CheckConformances(T, &ExtensionDecls[DC])) {
         return true;
      }

      if (QC.CheckAssociatedTypeConstraints(R)) {
         return true;
      }

      LOG(ProtocolConformances, R->getFullName(true), " ✅");
   }

   // Add additional extension decls that are not defaults.
   for (auto &[DC, Decls] : ExtensionDecls) {
      auto *Rec = cast<RecordDecl>(DC);
      // Don't add extension declarations to templates.
      if (Rec->isTemplate()) {
         continue;
      }

      QualType SelfTy = QC.Context.getRecordType(Rec);
      for (auto *ND : Decls) {
         NamedDecl *Equiv;
         if (QC.FindEquivalentDecl(Equiv, ND, Rec, SelfTy)) {
            continue;
         }

         // Declaration already implemented.
         if (Equiv) {
            continue;
         }

         // Make the declaration visible, but delay instantiation until it's
         // actually referenced.
         QC.Sema->makeDeclAvailable(
            *Rec,getNameForProtocolExtensionDecl(QC.Context, ND, Rec), ND);
      }
   }

   return false;
}

bool LookupResolver::FindDeclContexts(DeclContext *DC)
{
   if (isa<Decl>(DC) && cast<Decl>(DC)->isInvalid()) {
      return true;
   }

   PrepareMacros(DC);
   PrepareStaticDecls(DC);

   allDecls.insert(DC);

   if (auto *R = dyn_cast<RecordDecl>(DC)) {
      if (R->hasAttribute<_BuiltinAttr>()) {
         builtinDecls.insert(R);
      }
   }

   for (auto *InnerDC : DC->getDecls<DeclContext>()) {
      if (FindDeclContexts(InnerDC)) {
         return true;
      }
   }

   return false;
}

bool LookupResolver::PrepareMacros(DeclContext *DC)
{
   auto fail = false;
   for (auto* Decl : DC->getDecls<MacroExpansionDecl>()) {
      SemaPass::DeclScopeRAII DSR(*QC.Sema, Decl->getDeclContext());
      if (!QC.Sema->checkNamespaceRef(Decl))
         return true;

      DeclContext* Ctx = Decl->getDeclContext();
      if (auto* Ident = cast_or_null<IdentifierRefExpr>(Decl->getParentExpr())) {
         if (Ident->getKind() == IdentifierKind::Namespace) {
            Ctx = Ident->getNamespaceDecl();
         }
         else if (Ident->getKind() == IdentifierKind::Import) {
            Ctx = Ident->getImport();
         }
         else if (Ident->getKind() == IdentifierKind::Module) {
            Ctx = Ident->getModule();
         }
      }

      StmtOrDecl Result;
      if (QC.ExpandMacro(Result, Decl, Decl->getMacroName(), Ctx,
                         Decl->getDelim(),Decl->getTokens(),
                         (unsigned)parse::Parser::ExpansionKind::Decl)) {
         fail = true;
      }
   }

   return fail;
}

bool LookupResolver::PrepareStaticDecls(DeclContext *DC)
{
   bool fail = false;
   for (auto* Decl : DC->getDecls()) {
      switch (Decl->getKind()) {
      case Decl::StaticIfDeclID: {
         auto* If = cast<StaticIfDecl>(Decl);
         ast::Decl* Result;

         fail |= QC.ResolveStaticIf(Result, If);
         break;
      }
      case Decl::StaticForDeclID: {
         auto* For = cast<StaticForDecl>(Decl);
         ast::Decl* Result;

         fail |= QC.ResolveStaticFor(Result, For);
         break;
      }
      default:
         break;
      }
   }

   return fail;
}

bool LookupResolver::PrepareImports(DeclContext *DC)
{
   auto fail = false;
   for (auto* Decl : DC->getDecls<ImportDecl>()) {
      fail |= QC.ResolveImport(Decl);
   }

   return fail;
}

bool LookupResolver::PrepareUsings(DeclContext *DC)
{
   auto fail = false;
   for (auto* Decl : DC->getDecls<UsingDecl>()) {
      fail |= QC.ResolveUsing(Decl);
   }

   return fail;
}

bool LookupResolver::PrepareImplicitDecls(DeclContext *DC)
{
   auto *R = dyn_cast<RecordDecl>(DC);
   if (!R) {
      return false;
   }

   // Make sure implicit initializers / deinitializers are declared.
   if (QC.DeclareImplicitInitializers(R)) {
      return true;
   }

   // Resolve the initializer names and declare base ones.
   for (auto* Init : R->getDecls<InitDecl>()) {
      if (QC.AssignInitName(Init)) {
         return true;
      }
      if (auto Err = QC.CreateBaseInit(Init)) {
         return true;
      }
   }

   return false;
}

bool SemaPass::PrepareNameLookup(DeclContext *DC, bool isBaseModule)
{
   DelayRecordInstantiationsRAII Delay(*this, isBaseModule);

   LookupResolver resolver(QC, DC);
   if (resolver.Run(true, isBaseModule)) {
      return true;
   }

   if (isBaseModule) {
      if (CompleteRecordInstantiations()) {
         return true;
      }
   }

   return false;
}

bool SemaPass::CompleteRecordInstantiations()
{
   if (incompleteRecordInstantiations.empty()) {
      return false;
   }

   LookupResolver resolver(QC, DC);
   for (auto *Inst : incompleteRecordInstantiations) {
      resolver.allDecls.insert(Inst);
   }

   incompleteRecordInstantiations.clear();
   return resolver.Run(false);
}

LookupLevel SemaPass::getLookupLevel(DeclContext *DC) const
{
   auto it = lookupLevels.find(DC);
   if (it == lookupLevels.end()) {
      return LookupLevel::None;
   }

   return it->getSecond();
}

void SemaPass::updateLookupLevel(DeclContext *DC, LookupLevel newLevel)
{
   auto &ref = lookupLevels[DC];
   ref = (LookupLevel)((int)ref | (int)newLevel);
}

bool SemaPass::hasLookupLevel(DeclContext *DC, LookupLevel level) const
{
   auto it = lookupLevels.find(DC);
   if (it == lookupLevels.end()) {
      return false;
   }

   return ((int)it->getSecond() & (int)level) != 0;
}

QueryResult PrepareNameLookupQuery::run()
{
   return finish();
}

bool SingleLevelLookupResult::unique() const
{
   NamedDecl* UniqueDecl = nullptr;
   for (auto* ND : *this) {
      if (!UniqueDecl) {
         UniqueDecl = ND;
      }
      else if (ND != UniqueDecl) {
         return false;
      }
   }

   return true;
}

ast::NamedDecl* SingleLevelLookupResult::uniqueDecl() const
{
   assert(unique() && "lookup result is not unique!");
   return front();
}

bool MultiLevelLookupResult::unique() const
{
   NamedDecl* UniqueDecl = nullptr;
   for (auto* ND : allDecls()) {
      if (!UniqueDecl) {
         UniqueDecl = ND;
      }
      else if (ND != UniqueDecl) {
         return false;
      }
   }

   return true;
}

ast::NamedDecl* MultiLevelLookupResult::uniqueDecl() const
{
   assert(unique() && "lookup result is not unique!");
   return front().front();
}