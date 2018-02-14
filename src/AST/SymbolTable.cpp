////
//// Created by Jonas Zell on 07.07.17.
////
//
//#include "SymbolTable.h"
//
//#include <sstream>
//#include <llvm/Support/raw_ostream.h>
//
//#include "../Message/Diagnostics.h"
//#include "Passes/Declaration/DeclPass.h"
//
//#include "Expression/TypeRef.h"
//#include "Expression/StaticExpr.h"
//
//#include "Statement/Declaration/CallableDecl.h"
//#include "Statement/Declaration/NamespaceDecl.h"
//#include "Statement/Declaration/Class/RecordDecl.h"
//#include "Statement/Declaration/Class/MethodDecl.h"
//#include "Statement/Declaration/TypedefDecl.h"
//
//#include "../Support/Casting.h"
//#include "../Variant/Type/VoidType.h"
//#include "../Variant/Type/GenericType.h"
//#include "Passes/PrettyPrint/PrettyPrinter.h"
//#include "../Variant/Type/IntegerType.h"
//#include "../Variant/Type/FPType.h"
//
//using namespace cdot::diag;
//using namespace cdot::support;
//
//using std::ostringstream;
//
//namespace cdot {
//
//size_t SymbolTable::lastNamespaceId = 0;
//
//size_t SymbolTable::getNamespaceId(llvm::StringRef nsName) const
//{
//   auto res = Namespaces.find(nsName);
//   if (res == Namespaces.end()) return 0;
//
//   return res->second->getNamespaceId();
//}
//
//size_t SymbolTable::declareNamespace(ast::NamespaceDecl *decl)
//{
//   auto it = Namespaces.find(decl->getName());
//   if (it != Namespaces.end()) {
//      return it->second->getNamespaceId();
//   }
//
//   auto id = lastNamespaceId++;
//   auto pos = Namespaces.try_emplace(decl->getName(), decl);
//
//   NamespaceIDs.emplace(id, pos.first->second);
//   return id;
//}
//
//NamespaceDecl const* SymbolTable::lookupNamespace(llvm::StringRef name) const
//{
//   auto it = Aliases.find(name);
//   if (it != Aliases.end())
//      return lookupNamespace(it->second);
//
//   auto it2 = Namespaces.find(name);
//   if (it2 != Namespaces.end())
//      return it2->second;
//
//   return nullptr;
//}
//
//size_t SymbolTable::isNamespace(llvm::StringRef name,
//                                llvm::ArrayRef<size_t> imports,
//                                NamespaceDecl *currentNamespace) const {
//   auto ns = lookupInNamespaces(name, Namespaces, currentNamespace, imports);
//   return ns ? (*ns)->getNamespaceId() : 0;
//}
//
//SymbolTableEntry* SymbolTable::declareVariable(llvm::StringRef name,
//                                               const QualType &type,
//                                               AccessModifier access,
//                                               bool typeDependant,
//                                               size_t nsId,
//                                               ast::Statement *decl,
//                                               const SourceLocation &loc) {
//   return addEntry(name, { new Variable(name, access, type,
//                                        getNamespace(nsId),
//                                        loc, typeDependant, decl) });
//}
//
//SymbolTableEntry* SymbolTable::declareVariable(llvm::StringRef name,
//                                               const QualType &type,
//                                               AccessModifier access,
//                                               bool typeDependant,
//                                               NamespaceDecl *ns,
//                                               ast::Statement *decl,
//                                               const SourceLocation &loc) {
//   return addEntry(name, { new Variable(name, access, type,
//                                        ns, loc, typeDependant, decl) });
//}
//
//SymbolTableEntry* SymbolTable::declareFunction(llvm::StringRef name,
//                                               FunctionDecl *F)
//{
//   llvm_unreachable("");
//}
//
//SymbolTableEntry* SymbolTable::declareFunction(ast::FunctionDecl *F)
//{
//   return declareFunction(F->getName(), F);
//}
//
//const bool SymbolTable::hasFunction(llvm::StringRef name,
//                                    llvm::ArrayRef<size_t> imports,
//                                    NamespaceDecl *ns) const {
//   auto res = lookup(name, imports, ns, LookupKind::LK_Functions);
//   return res.Entry && res.Entry->getKind() == SymbolTableEntry::FunctionID;
//}
//
//llvm::ArrayRef<CallableDecl*>
//SymbolTable::getFunctionOverloads(llvm::StringRef name,
//                                  llvm::ArrayRef<size_t> imports,
//                                  NamespaceDecl *namespaces) {
//   llvm_unreachable("");
//}
//
//bool SymbolTable::hasVariable(llvm::StringRef name,
//                              llvm::ArrayRef<size_t> imports,
//                              NamespaceDecl *namespaces) const {
//   auto res = lookup(name, imports, namespaces, LookupKind::LK_Variables);
//   return res.isGlobalVariable();
//}
//
//size_t SymbolTable::numFunctionsWithName(llvm::StringRef funcName) const
//{
//   auto Entry = Entries.find(funcName);
//   if (Entry == Entries.end()
//       || Entry->second.getKind() != SymbolTableEntry::FunctionID) {
//      return 0;
//   }
//
//   return Entry->second.getFunction()->getOverloads().size();
//}
//
//Variable *SymbolTable::getVariable(llvm::StringRef name,
//                                   llvm::ArrayRef<size_t> imports,
//                                   NamespaceDecl *namespaces) const {
//   auto res = lookup(name, imports, namespaces, LookupKind::LK_Variables);
//   if (!res.isGlobalVariable())
//      return nullptr;
//
//   return res.Entry->getVariable();
//}
//
//void SymbolTable::setVariable(llvm::StringRef name, QualType ty) const
//{
//   auto var = getVariable(name);
//   *var->type = *ty;
//}
//
//SymbolTableEntry const* SymbolTable::declareRecord(RecordDecl *R)
//{
//   return addEntry(R->getName(), { R });
//}
//
//SymbolTableEntry const* SymbolTable::declareRecord(llvm::StringRef name,
//                                                   RecordDecl *R) {
//   return addEntry(name, { R });
//}
//
//SymbolTableEntry const* SymbolTable::declareTypedef(TypedefDecl *decl)
//{
//   return declareTypedef(decl->getName(), decl);
//}
//
//SymbolTableEntry const* SymbolTable::declareTypedef(llvm::StringRef name,
//                                                    TypedefDecl *decl) {
//   return addEntry(name, { decl });
//}
//
//SymbolTableEntry const* SymbolTable::declareAlias(ast::AliasDecl *decl)
//{
//   return declareAlias(decl->getName(), decl);
//}
//
//SymbolTableEntry const* SymbolTable::declareAlias(llvm::StringRef name,
//                                                  ast::AliasDecl *decl) {
//   auto prev = lookup(name, {}, {});
//   if (prev.isAlias()) {
//      prev.Entry->getAlias()->addOverload(decl);
//      return prev.Entry;
//   }
//
//   return addEntry(name, decl);
//}
//
//#define SYMTAB_GET_X(Name) \
//   Name##Decl*                                                                 \
//   SymbolTable::get##Name(llvm::StringRef name,                                \
//                          llvm::ArrayRef<size_t> imports,                      \
//                          NamespaceDecl *namespaces) const {                   \
//      auto res = lookup(name, imports, namespaces, LookupKind::LK_Records);    \
//      if (res.isRecord()) {                                                    \
//         if (auto Cl = dyn_cast<Name##Decl>(res.Entry->getRecord())) {         \
//            return Cl;                                                         \
//         }                                                                     \
//      }                                                                        \
//      return nullptr;                                                          \
//   }
//
//SYMTAB_GET_X(Class)
//SYMTAB_GET_X(Union)
//SYMTAB_GET_X(Struct)
//SYMTAB_GET_X(Protocol)
//SYMTAB_GET_X(Enum)
//
//#undef SYMTAB_GET_X
//
//bool SymbolTable::hasRecord(llvm::StringRef name,
//                            llvm::ArrayRef<size_t> imports,
//                            NamespaceDecl *namespaces) const {
//   auto res = lookup(name, imports, namespaces, LookupKind::LK_Records);
//   return res.isRecord();
//}
//
//RecordDecl *SymbolTable::getRecord(llvm::StringRef name,
//                                   llvm::ArrayRef<size_t> imports,
//                                   NamespaceDecl *namespaces) const {
//   auto res = lookup(name, imports, namespaces, LookupKind::LK_Records);
//   if (res.isRecord()) {
//      return res.Entry->getRecord();
//   }
//
//   return nullptr;
//}
//
//FunctionDecl *SymbolTable::getAnyFn(llvm::StringRef withName,
//                                    llvm::ArrayRef<size_t> imports,
//                                    NamespaceDecl *namespaces) const {
//   auto res = lookup(withName, imports, namespaces, LookupKind::LK_Functions);
//   if (!res.isFunction()) {
//      return nullptr;
//   }
//
//   return res.Entry->getFunction();
//}
//
//TypedefDecl *SymbolTable::getTypedef(llvm::StringRef name,
//                                     llvm::ArrayRef<size_t> imports,
//                                     NamespaceDecl *namespaces) const {
//   auto res = lookup(name, imports, namespaces, LookupKind::LK_Full);
//   return res.isTypedef() ? res.Entry->getTypedef() : nullptr;
//}
//
//llvm::ArrayRef<AliasDecl*>
//SymbolTable::getAliases(llvm::StringRef name,
//                        llvm::ArrayRef<size_t> imports,
//                        NamespaceDecl *ns) const {
//   auto res = lookup(name, imports, ns, LookupKind::LK_Full);
//   if (!res.isAlias())
//      return {};
//
//   return res.Entry->getAlias()->getOverloads();
//}
//
//#define SYMTAB_HAS_X(Name)                                                \
//   bool SymbolTable::has##Name(llvm::StringRef name,                      \
//                               llvm::ArrayRef<size_t> imports,            \
//                               NamespaceDecl *namespaces) const {         \
//      auto rec = getRecord(name, imports, namespaces);                    \
//      return rec && rec->is##Name();                                      \
//   }
//
//SYMTAB_HAS_X(Union)
//SYMTAB_HAS_X(Class)
//SYMTAB_HAS_X(Enum)
//SYMTAB_HAS_X(Struct)
//SYMTAB_HAS_X(Protocol)
//
//#undef SYMTAB_HAS_X
//
//bool SymbolTable::hasTypedef(llvm::StringRef name,
//                             llvm::ArrayRef<size_t> imports,
//                             NamespaceDecl *namespaces) const {
//   auto res = lookup(name, imports, namespaces, LookupKind::LK_Full);
//   return res.isTypedef();
//}
//
//SymbolTableEntry* SymbolTable::addEntry(llvm::StringRef name,
//                                        SymbolTableEntry &&entry) {
//   auto it = Entries.find(name);
//
//   if (it != Entries.end()) {
//      if (entry.getKind() == SymbolTableEntry::VariableID) {
//         it->second = entry;
//      }
//      else {
//         SourceLocation loc;
//         SourceLocation prevLoc;
//
//         switch (entry.getKind()) {
//            case SymbolTableEntry::FunctionID:
//               loc = entry.getFunction()->getSourceLoc();
//               break;
//            case SymbolTableEntry::VariableID:
//               loc = entry.getVariable()->loc;
//               break;
//            case SymbolTableEntry::TypedefID:
//               loc = entry.getTypedef()->getSourceLoc();
//               break;
//            case SymbolTableEntry::RecordID:
//               loc = entry.getRecord()->getSourceLoc();
//               break;
//            case SymbolTableEntry::AliasID:
//               loc = entry.getAlias()->getSourceLoc();
//               break;
//            default:
//               break;
//         }
//         switch (it->second.getKind()) {
//            case SymbolTableEntry::FunctionID:
//               prevLoc = it->second.getFunction()->getSourceLoc();
//               break;
//            case SymbolTableEntry::VariableID:
//               prevLoc = it->second.getVariable()->loc;
//               break;
//            case SymbolTableEntry::TypedefID:
//               prevLoc = it->second.getTypedef()->getSourceLoc();
//               break;
//            case SymbolTableEntry::RecordID:
//               prevLoc = it->second.getRecord()->getSourceLoc();
//               break;
//            case SymbolTableEntry::AliasID:
//               prevLoc = it->second.getAlias()->getSourceLoc();
//               break;
//            default:
//               break;
//         }
//
//         auto err = diag::err(err_generic_error) << "duplicate declaration of "
//                                                    + name;
//
//         if (loc)
//            err << loc;
//
//         if (prevLoc) {
//            err << diag::cont;
//            diag::note(note_generic_note) << "previous declaration was here"
//                                          << prevLoc << diag::term;
//         }
//         else {
//            err << diag::term;
//         }
//      }
//
//      return &it->second;
//   }
//   else {
//      auto res = Entries.try_emplace(name, entry);
//      return &res.first->second;
//   }
//}
//
//bool SymbolTable::LookupResult::isRecord() const
//{
//   return kind == LRK_Record || kind == LRK_Class || kind == LRK_Struct
//          || kind == LRK_Enum || kind == LRK_Union || kind == LRK_Protocol;
//}
//
//SymbolTableEntry const* SymbolTable::findEntry(llvm::StringRef symbolName) const
//{
//   auto it = Aliases.find(symbolName);
//   if (it != Aliases.end())
//      return findEntry(it->second);
//
//   auto it2 = Entries.find(symbolName);
//   if (it2 != Entries.end())
//      return &it2->second;
//
//   return nullptr;
//}
//
//SymbolTable::LookupResult
//SymbolTable::lookup(llvm::StringRef symbolName,
//                    llvm::ArrayRef<size_t> imports,
//                    NamespaceDecl *currentNamespace,
//                    LookupKind kind) const {
//   LookupResult result { LookupResult::LRK_Nothing };
//   auto EntryPtr = lookupInNamespaces(symbolName, Entries, currentNamespace,
//                                      imports);
//
//   if (!EntryPtr) {
//      if (auto ID = isNamespace(symbolName, imports, currentNamespace)) {
//         result.kind = LookupResult::LRK_Namespace;
//         result.NS = getNamespace(ID);
//      }
//
//      return result;
//   }
//
//   auto &Entry = *EntryPtr;
//   result.Entry = EntryPtr;
//
//   bool lookupRecords = kind == LK_Full || kind == LK_Records;
//   if (lookupRecords) {
//      if (Entry.getKind() == SymbolTableEntry::RecordID) {
//         auto rec = Entry.getRecord();
//         if (rec->isEnum()) {
//            result.kind = LookupResult::LRK_Enum;
//         }
//         else if (rec->isStruct()) {
//            result.kind = LookupResult::LRK_Struct;
//         }
//         else if (rec->isProtocol()) {
//            result.kind = LookupResult::LRK_Protocol;
//         }
//         else if (rec->isUnion()) {
//            result.kind = LookupResult::LRK_Union;
//         }
//         else {
//            assert(rec->isClass());
//            result.kind = LookupResult::LRK_Class;
//         }
//
//         return result;
//      }
//   }
//
//   bool lookupFns = kind == LK_Full || kind == LK_Functions;
//   if (lookupFns) {
//      if (Entry.getKind() == SymbolTableEntry::FunctionID) {
//         result.kind = LookupResult::LRK_Function;
//         return result;
//      }
//   }
//
//   bool lookupVars = kind == LK_Full || kind == LK_Variables;
//   if (lookupVars) {
//      if (Entry.getKind() == SymbolTableEntry::VariableID) {
//         result.kind = LookupResult::LRK_GlobalVariable;
//         return result;
//      }
//   }
//
//   if (Entry.getKind() == SymbolTableEntry::TypedefID) {
//      result.kind = LookupResult::LRK_Typedef;
//      return result;
//   }
//
//   if (Entry.getKind() == SymbolTableEntry::AliasID) {
//      result.kind = LookupResult::LRK_Alias;
//      return result;
//   }
//
//   if (auto NS = getNamespace(symbolName)) {
//      result.kind = LookupResult::LRK_Namespace;
//      result.NS = NS;
//   }
//
//   return result;
//}
//
//template<class T>
//T const* SymbolTable::lookupInNamespaces(llvm::StringRef needle,
//                                         llvm::StringMap<T> const& haystack,
//                                         NamespaceDecl *currentNamespace,
//                                         llvm::ArrayRef<size_t> imports) const {
//   auto EntryPtr = findSingleEntry(needle, haystack);
//   bool foundInNamespace = false;
//   llvm::SmallString<128> ScratchBuf;
//
//   while (currentNamespace) {
//      ScratchBuf += currentNamespace->getName();
//      ScratchBuf += '.';
//      ScratchBuf += needle;
//
//      auto NextEntry = findSingleEntry(ScratchBuf.str(), haystack);
//      if (NextEntry) {
//         EntryPtr = NextEntry;
//         foundInNamespace = true;
//         break;
//      }
//
//      ScratchBuf.clear();
//      currentNamespace = currentNamespace->getOuterNamespace();
//   }
//
//   if (!foundInNamespace) {
//      for (auto id : imports) {
//         auto ns = NamespaceIDs.find(id);
//         if (ns == NamespaceIDs.end())
//            continue;
//
//         ScratchBuf += ns->second->getName();
//         ScratchBuf += '.';
//         ScratchBuf += needle;
//
//         auto NextEntry = findSingleEntry(ScratchBuf.str(), haystack);
//         if (NextEntry) {
//            EntryPtr = NextEntry;
//            break;
//         }
//
//         ScratchBuf.clear();
//      }
//   }
//
//   return EntryPtr;
//}
//
//template SymbolTableEntry const* SymbolTable::lookupInNamespaces(
//                              llvm::StringRef needle,
//                              llvm::StringMap<SymbolTableEntry> const& haystack,
//                              NamespaceDecl *NSStack,
//                              llvm::ArrayRef<size_t> imports) const;
//
//template NamespaceDecl const* SymbolTable::lookupInNamespaces(
//                              llvm::StringRef needle,
//                              llvm::StringMap<NamespaceDecl> const&haystack,
//                              NamespaceDecl *NSStack,
//                              llvm::ArrayRef<size_t> imports) const;
//
//} // namespace cdot