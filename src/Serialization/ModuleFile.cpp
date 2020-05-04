#include "cdotc/Serialization/ModuleFile.h"

#include "cdotc/IL/Module.h"
#include "cdotc/ILGen/ILGenPass.h"
#include "cdotc/Query/QueryContext.h"
#include "cdotc/Sema/SemaPass.h"
#include "cdotc/Sema/TemplateInstantiator.h"
#include "cdotc/Serialization/ASTReader.h"
#include "cdotc/Serialization/ModuleReader.h"

using namespace cdot;
using namespace cdot::ast;
using namespace cdot::serial;
using namespace cdot::support;

ModuleFile::ModuleFile(ModuleReader& Reader, void* HashTablePtr)
    : Reader(Reader), HashTablePtr(HashTablePtr)
{
}

ModuleFile::~ModuleFile() {}

static void addDeclToContext(SemaPass& Sema, DeclContext& Ctx, Decl* D)
{
   if (auto ND = support::cast_or_null<NamedDecl>(D)) {
      Sema.makeDeclAvailable(Ctx, ND, true);

//      if (auto *Ext = dyn_cast<ExtensionDecl>(&Ctx)) {
//         Sema.makeDeclAvailable(*Ext->getExtendedRecord(), ND, true);
//      }
   }
}

void ModuleFile::PerformExternalLookup(DeclContext& Ctx, DeclarationName Name)
{
   if (LoadedAllDecls)
      return;

   if (!AlreadyLookedUp.insert(Name).second)
      return;

   auto* Tbl = reinterpret_cast<reader::HashTable*>(HashTablePtr);
   if (Tbl) {
      auto It = Tbl->find(Name);

      reader::ASTDeclContextNameLookupTrait Trait(Reader.ASTReader);
      auto IDs = Trait.ReadData(Name, It.getDataPtr(), It.getDataLen());

      for (auto ID : IDs) {
         auto ReadDecl = Reader.ASTReader.GetDecl(ID);
         if (ReadDecl) {
            LoadedDecl(Ctx, ReadDecl);
         }
      }
   }

   if (auto *R = dyn_cast<RecordDecl>(&Ctx)) {
      for (auto *Ext : R->getExtensions()) {
         if (auto *MF = Ext->getModFile()) {
            MF->PerformExternalLookup(*Ext, Name);
         }
      }
   }
}

static bool isInInstantiation(DeclContext& CtxRef, NamedDecl*& Inst)
{
   auto* Ctx = &CtxRef;
   while (Ctx) {
      auto* ND = dyn_cast<NamedDecl>(Ctx);
      if (ND && ND->isInstantiation()) {
         Inst = ND;
         return true;
      }

      Ctx = Ctx->getParentCtx();
   }

   return false;
}

void ModuleFile::LoadedDecl(DeclContext& Ctx, Decl* ReadDecl, bool IgnoreInst)
{
   // these decls are immediately made visible
   switch (ReadDecl->getKind()) {
   case Decl::OperatorDeclID:
   case Decl::PrecedenceGroupDeclID:
   case Decl::EnumCaseDeclID:
   case Decl::FuncArgDeclID:
   case Decl::TemplateParamDeclID:
      return;
   case Decl::FieldDeclID:
      if (!ReadDecl->isStatic()) {
         return;
      }

      break;
   default:
      break;
   }

   if (auto M = dyn_cast<MethodDecl>(ReadDecl)) {
      if (M->isProtocolDefaultImpl() && isa<ProtocolDecl>(&Ctx))
         return;
   }

   auto& Sema = Reader.CI.getSema();

   // if this context is an instantiation, we need to instantiate the decl
   // first
   NamedDecl* Inst = nullptr;
   if (!IgnoreInst && isInInstantiation(Ctx, Inst)) {
      if (Inst->isImportedInstantiation()) {
         addDeclToContext(Sema, Ctx, ReadDecl);
         return;
      }
      if (ReadDecl->getDeclContext() == &Ctx) {
         addDeclToContext(Sema, Ctx, ReadDecl);
         return;
      }
      if (auto* M = dyn_cast<MethodDecl>(ReadDecl)) {
         if (M->isSubscript() || M->isProperty())
            return;
      }

      SemaPass::DeclScopeRAII DSR(Sema, &Ctx);
      auto InstResult = Sema.getInstantiator().InstantiateDecl(
          {}, ReadDecl, Inst->getTemplateArgs());

      if (InstResult) {
         Sema.QC.PrepareDeclInterface(InstResult.get());
      }

      return;
   }

   addDeclToContext(Sema, Ctx, ReadDecl);
}

void ModuleFile::LoadAllDecls(DeclContext& Ctx, bool IgnoreInst)
{
   if (LoadedAllDecls)
      return;

   LoadedAllDecls = true;

   auto* Tbl = reinterpret_cast<reader::HashTable*>(HashTablePtr);
   auto it = Tbl->data_begin();
   auto end = Tbl->data_end();

   while (it != end) {
      for (unsigned ID : *it) {
         auto* D = Reader.ASTReader.GetDecl(ID);
         if (!D) {
            continue;
         }

         if (auto* ND = dyn_cast<NamedDecl>(D)) {
            if (AlreadyLookedUp.find(ND->getDeclName())
                != AlreadyLookedUp.end())
               continue;
         }

         LoadedDecl(Ctx, D, IgnoreInst);
      }

      ++it;
   }
}

NamedDecl* ModuleFile::LookupInstantiation(StringRef MangledName)
{
   using HashTable = llvm::OnDiskIterableChainedHashTable<
       reader::InstantiationTableLookupTrait>;

   if (!InstantiationTable)
      return nullptr;

   auto* Tbl = reinterpret_cast<HashTable*>(InstantiationTable);
   auto It = Tbl->find(MangledName);

   if (It == Tbl->end())
      return nullptr;

   reader::InstantiationTableLookupTrait Trait(Reader.ASTReader);
   return Trait.ReadData(MangledName, It.getDataPtr(), It.getDataLen());
}

llvm::StringRef ModuleFile::getLibraryBlob() { return Reader.StaticLibBlob; }

ModuleFile* ModuleFile::copy() const
{
   auto& Ctx = Reader.CI.getContext();
   auto* MF = new (Ctx) ModuleFile(Reader, HashTablePtr);
   MF->InstantiationTable = InstantiationTable;
   MF->LoadedAllDecls = LoadedAllDecls;
   MF->AlreadyLookedUp = AlreadyLookedUp;

   return MF;
}

LazyFunctionInfo::LazyFunctionInfo(ModuleReader& Reader,
                                   llvm::BitstreamCursor BodyCursor)
    : Reader(Reader), BodyCursor(BodyCursor)
{
}

void LazyFunctionInfo::loadBody(CallableDecl* Fn)
{
   if (BodyRead)
      return;

   BodyRead = true;
   Fn->setBody(Reader.ASTReader.ReadStmtFromStream(BodyCursor));
}

LazyILFunctionInfo::LazyILFunctionInfo(
    ModuleReader& Reader, il::Function& F, SmallVector<unsigned, 0>&& BlockIDs,
    SmallVector<unsigned, 0>&& EntryBlockInstIDs, unsigned Linkage)
    : Reader(Reader), F(F), BlockIDs(move(BlockIDs)),
      EntryBlockInstIDs(move(EntryBlockInstIDs)), Linkage(Linkage)
{
}

void LazyILFunctionInfo::loadFunctionBody()
{
   if (!F.isDeclared())
      return;

   Reader.ILReader.readFunctionBody(F, BlockIDs, EntryBlockInstIDs, Linkage);
}

LazyILGlobalInfo::LazyILGlobalInfo(ModuleReader& Reader, il::GlobalVariable& G,
                                   unsigned InitID, unsigned Linkage)
    : Reader(Reader), G(G), InitID(InitID), Linkage(Linkage)
{
}

void LazyILGlobalInfo::loadGlobalInitializer()
{
   if (G.hasInitializer())
      return;

   Reader.ILReader.readGlobalInitializer(G, InitID, Linkage);
}

ILModuleFile::ILModuleFile(ModuleReader& Reader, void* SymTab)
    : Reader(Reader), SymTab(SymTab)
{
}

il::GlobalObject* ILModuleFile::Lookup(StringRef Name)
{
   using HashTable = ILReader::HashTable;
   auto* Tbl = reinterpret_cast<HashTable*>(SymTab);

   auto It = Tbl->find(Name);
   if (It == Tbl->end())
      return nullptr;

   ILSymbolTableLookupTrait Trait(Reader.ILReader);
   auto* Val = Trait.ReadData(Name, It.getDataPtr(), It.getDataLen());

   return cast<il::GlobalObject>(Val);
}

ILModuleFile::~ILModuleFile() {}