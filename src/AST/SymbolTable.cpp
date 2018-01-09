//
// Created by Jonas Zell on 07.07.17.
//

#include "SymbolTable.h"

#include <sstream>
#include <llvm/Support/raw_ostream.h>

#include "../Message/Diagnostics.h"
#include "Passes/Declaration/DeclPass.h"

#include "Passes/SemanticAnalysis/Record/Class.h"
#include "Passes/SemanticAnalysis/Record/Protocol.h"
#include "Passes/SemanticAnalysis/Record/Union.h"
#include "Passes/SemanticAnalysis/Record/Enum.h"
#include "Passes/SemanticAnalysis/Function.h"

#include "Expression/TypeRef.h"
#include "Expression/StaticExpr.h"
#include "Statement/Declaration/CallableDecl.h"
#include "Statement/Declaration/Class/RecordDecl.h"
#include "Statement/Declaration/Class/MethodDecl.h"
#include "Statement/Declaration/TypedefDecl.h"

#include "../Support/Casting.h"
#include "../Variant/Type/VoidType.h"
#include "../Variant/Type/GenericType.h"
#include "Passes/PrettyPrint/PrettyPrinter.h"

using namespace cdot::diag;
using namespace cdot::support;

using std::ostringstream;
using ast::Function;

namespace cdot {

std::unordered_map<size_t, Namespace*> SymbolTable::NamespaceIDs;
llvm::StringMap<string> SymbolTable::Aliases;

llvm::StringMap<Namespace> SymbolTable::Namespaces;
llvm::StringMap<SymbolTableEntry> SymbolTable::Entries;

size_t Namespace::lastId = 1;

void SymbolTable::addBuiltins()
{
   declareTypedef("Void", VoidType::get());

   declareNamespace("Builtin");
   declareNamespace("Builtin.Primitive");
   declareNamespace("Global");
}

size_t SymbolTable::getNamespaceId(llvm::StringRef nsName)
{
   auto res = Namespaces.find(nsName);
   if (res == Namespaces.end()) return 0;

   return res->second.getId();
}

const string& SymbolTable::getNamespaceName(size_t id)
{
   return NamespaceIDs[id]->name;
}

void SymbolTable::declareAlias(const llvm::Twine &alias,
                               llvm::StringRef aliasee) {
   Aliases.try_emplace(alias.str(), aliasee);
}

size_t SymbolTable::declareNamespace(llvm::StringRef name,
                                     bool isAnonymous,
                                     Namespace *parentNamespace) {
   auto it = Namespaces.find(name);
   if (it != Namespaces.end()) {
      return it->second.getId();
   }

   auto id = Namespace::lastId++;
   auto pos = Namespaces.try_emplace(name, id, string(name),
                                     isAnonymous, parentNamespace);

   NamespaceIDs.emplace(id, &pos.first->second);
   return id;
}

Namespace* SymbolTable::lookupNamespace(llvm::StringRef name)
{
   auto it = Aliases.find(name);
   if (it != Aliases.end())
      return lookupNamespace(it->second);

   auto it2 = Namespaces.find(name);
   if (it2 != Namespaces.end())
      return &it2->second;

   return nullptr;
}

size_t SymbolTable::isNamespace(llvm::StringRef name,
                                llvm::ArrayRef<size_t> imports,
                                llvm::ArrayRef<size_t> NSStack) {
//   llvm::SmallString<128> fullNS(name);
//   auto NS = lookupNamespace(fullNS.str());
//   if (NS && !NS->isAnonymous)
//      return NS->id;
//
//   for (const auto &id : ns) {
//      fullNS.clear();
//
//      fullNS += getNamespaceName(id);
//      fullNS += '.';
//      fullNS += name;
//
//      NS = lookupNamespace(fullNS.str());
//      if (NS && !NS->isAnonymous)
//         return NS->id;
//   }
//
//   for (const auto &id : ns) {
//      fullNS.clear();
//
//      fullNS += getNamespaceName(id);
//      fullNS += '.';
//      fullNS += name;
//
//      NS = lookupNamespace(fullNS.str());
//      if (NS && !NS->isAnonymous)
//         return NS->id;
//   }
//
//   return 0;
   auto ns = lookupInNamespaces(name, Namespaces, NSStack, imports);
   return ns ? ns->getId() : 0;
}

SymbolTableEntry* SymbolTable::declareVariable(llvm::StringRef name,
                                               const QualType &type,
                                               AccessModifier access,
                                               bool typeDependant,
                                               size_t nsId,
                                               ast::Statement *decl,
                                               const SourceLocation &loc) {
   return addEntry(name, { new Variable(name, access, type, nsId, loc,
                                        typeDependant, decl) });
}

namespace {

void throwTemplateArgError(Function &newFunc, Function &func)
{
   diag::err(err_overload_generic_params)
      << 1 /*method*/
      << newFunc.getSourceLoc() << diag::cont;
   diag::note(note_overload_generic_params)
      << func.getSourceLoc() << diag::term;
}

}

void SymbolTable::checkTemplateArgCompatability(Function &newFunc)
{
   // function overloads must have the same template arguments
   auto overloads = getFunctionOverloads(newFunc.getName());
   if (overloads.empty()) {
      return;
   }

   auto &func = *overloads.front();
   if (newFunc.isTemplate() != func.isTemplate()) {
      throwTemplateArgError(newFunc, func);
   }
   if (!newFunc.isTemplate()) {
      return;
   }

   auto &NewParams = newFunc.getTemplateParams();
   auto &PrevParams = func.getTemplateParams();

   if (NewParams.size() != PrevParams.size()) {
      throwTemplateArgError(newFunc, func);
   }

   size_t i = 0;
   for (const auto &Constraint : NewParams) {
      if (Constraint != PrevParams[i]) {
         throwTemplateArgError(newFunc, func);
      }

      ++i;
   }
}

void SymbolTable::checkDuplicateFunctionDeclaration(const SourceLocation &loc,
                                                    llvm::StringRef name,
                                                    llvm::StringRef mangledName) {
   auto overloads = getFunctionOverloads(name);
   for (const auto &func : overloads) {
      if (func->getLinkageName() == mangledName) {
         diag::err(err_duplicate_method)
            << 1 /*method*/
            << name << loc
            << diag::cont;
         diag::note(note_duplicate_method)
            << func->getSourceLoc()
            << diag::term;
      }
   }
}

SymbolTableEntry* SymbolTable::declareFunction(llvm::StringRef name,
                                               Function *func) {
//   checkTemplateArgCompatability(*func);
//   checkDuplicateFunctionDeclaration(func->getSourceLoc(), name,
//                                     func->getLinkageName());

   declareNamespace(name);

   auto it = Entries.find(name);
   if (it == Entries.end()) {
      auto res = Entries.try_emplace(name, SymbolTableEntry{ func });
      return &res.first->second;
   }
   else {
      it->second.getFunctions().push_back(func);
      return &it->second;
   }
}

SymbolTableEntry* SymbolTable::declareFunction(ast::Function *func)
{
   return declareFunction(func->getName(), func);
}

const bool SymbolTable::hasFunction(llvm::StringRef name,
                                    llvm::ArrayRef<size_t> imports,
                                    llvm::ArrayRef<size_t> ns) {
   auto res = lookup(name, imports, ns, LookupKind::LK_Functions);
   return res.Entry && res.Entry->getKind() == SymbolTableEntry::FunctionID;
}

llvm::ArrayRef<Function*>
SymbolTable::getFunctionOverloads(llvm::StringRef name,
                                  llvm::ArrayRef<size_t> imports,
                                  llvm::ArrayRef<size_t> namespaces) {
   auto res = lookup(name, imports, namespaces, LookupKind::LK_Functions);
   if (res.kind == LookupResult::LRK_Nothing) {
      auto Entry = SymbolTableEntry();
      auto it = Entries.try_emplace(name, std::move(Entry));

      return it.first->getValue().getFunctions();
   }

   assert(res.isFunction());
   return res.Entry->getFunctions();
}

bool SymbolTable::hasVariable(llvm::StringRef name,
                              llvm::ArrayRef<size_t> imports,
                              llvm::ArrayRef<size_t> namespaces) {
   auto res = lookup(name, imports, namespaces, LookupKind::LK_Variables);
   return res.isGlobalVariable();
}

size_t SymbolTable::numFunctionsWithName(llvm::StringRef funcName)
{
   auto Entry = Entries.find(funcName);
   if (Entry == Entries.end()
       || Entry->second.getKind() != SymbolTableEntry::FunctionID) {
      return 0;
   }

   return Entry->second.getFunctions().size();
}

Variable *SymbolTable::getVariable(llvm::StringRef name,
                                   llvm::ArrayRef<size_t> imports,
                                   llvm::ArrayRef<size_t> namespaces) {
   auto res = lookup(name, imports, namespaces, LookupKind::LK_Variables);
   if (!res.isGlobalVariable())
      return nullptr;

   return res.Entry->getVariable();
}

void SymbolTable::setVariable(llvm::StringRef name, QualType ty)
{
   auto var = getVariable(name);
   *var->type = *ty;
}

SymbolTableEntry* SymbolTable::declareRecord(Record *cl)
{
   if (hasRecord(cl->getName()))
      diag::err(err_generic_error)
         << ("duplicate declaration of"
            + cl->getName()).str()
         << cl->getSourceLoc() << diag::term;

   declareNamespace(cl->getName());
   return addEntry(cl->getName(), { cl });
}

SymbolTableEntry* SymbolTable::declareRecord(llvm::StringRef name, Record *cl)
{
   declareNamespace(name);
   return addEntry(name, { cl });
}

SymbolTableEntry* SymbolTable::ForwardDeclareTypedef(llvm::StringRef alias,
                                                     AccessModifier access,
                                                     SourceLocation loc) {
   return addEntry(alias, { new Typedef(nullptr, alias, {}, access, loc) });
}

SymbolTableEntry* SymbolTable::declareTypedef(
                                 llvm::StringRef alias,
                                 Type *originTy,
                                 std::vector<TemplateParameter> &&constr,
                                 AccessModifier access,
                                 const SourceLocation &loc) {
   return addEntry(alias, { new Typedef(originTy, alias, move(constr),
                                        access, loc) });
}

SymbolTableEntry*
SymbolTable::declareAlias(llvm::StringRef name,
                          std::vector<TemplateParameter> &&templateParams,
                          std::vector<std::shared_ptr<StaticExpr>>&&constraints,
                          std::shared_ptr<StaticExpr> &&aliasExpr,
                          ast::AliasDecl *decl) {
   auto prev = lookup(name, {}, {});
   if (prev.isAlias()) {
      prev.Entry->getAliases()
          .push_back(new Alias(string(name), move(templateParams),
                               move(constraints), move(aliasExpr),
                               decl));

      return prev.Entry;
   }

   return addEntry(name, { new Alias(string(name), move(templateParams),
                                     move(constraints), move(aliasExpr),
                                     decl) });
}

#define SYMTAB_GET_X(Name) \
   cdot::cl::Name *SymbolTable::get##Name(llvm::StringRef name,                \
                                      llvm::ArrayRef<size_t> imports,          \
                                      llvm::ArrayRef<size_t> namespaces) {     \
      auto res = lookup(name, imports, namespaces, LookupKind::LK_Records);    \
      if (res.isRecord()) {                                                    \
         if (auto Cl = dyn_cast<Name>(res.Entry->getRecord())) {               \
            return Cl;                                                         \
         }                                                                     \
      }                                                                        \
      return nullptr;                                                          \
   }

SYMTAB_GET_X(Class)
SYMTAB_GET_X(Union)
SYMTAB_GET_X(Struct)
SYMTAB_GET_X(Protocol)
SYMTAB_GET_X(Enum)

#undef SYMTAB_GET_X

bool SymbolTable::hasRecord(llvm::StringRef name,
                            llvm::ArrayRef<size_t> imports,
                            llvm::ArrayRef<size_t> namespaces) {
   auto res = lookup(name, imports, namespaces, LookupKind::LK_Records);
   return res.isRecord();
}

bool SymbolTable::hasFunctionTemplate(llvm::StringRef name,
                                      llvm::ArrayRef<size_t> imports,
                                      llvm::ArrayRef<size_t> namespaces) {
   auto res = lookup(name, imports, namespaces, LookupKind::LK_Functions);
   return res.isFunction() && res.Entry->getFunctions().front()->isTemplate();
}

Record *SymbolTable::getRecord(llvm::StringRef name,
                               llvm::ArrayRef<size_t> imports,
                               llvm::ArrayRef<size_t> namespaces) {
   auto res = lookup(name, imports, namespaces, LookupKind::LK_Records);
   if (res.isRecord()) {
      return res.Entry->getRecord();
   }

   return nullptr;
}

Function *SymbolTable::getAnyFn(llvm::StringRef withName,
                                llvm::ArrayRef<size_t> imports,
                                llvm::ArrayRef<size_t> namespaces) {
   auto res = lookup(withName, imports, namespaces, LookupKind::LK_Functions);
   if (!res.isFunction()) {
      return nullptr;
   }

   return res.Entry->getFunctions().front();
}

bool SymbolTable::isTemplatedFunction(llvm::StringRef funcName)
{
   auto res = lookup(funcName, {}, LookupKind::LK_Functions);
   if (!res.isFunction()) {
      return false;
   }

   return res.Entry->getFunctions().front()->isTemplate();
}

const std::vector<TemplateParameter> &SymbolTable::getConstraints(
                                                    llvm::StringRef forFunction) {
   auto fun = getAnyFn(forFunction);
   return fun->getTemplateParams();
}

Typedef *SymbolTable::getTypedef(llvm::StringRef name,
                                 llvm::ArrayRef<size_t> imports,
                                 llvm::ArrayRef<size_t> namespaces) {
   auto res = lookup(name, imports, namespaces, LookupKind::LK_Full);
   return res.isTypedef() ? res.Entry->getTypedef() : nullptr;
}

llvm::ArrayRef<Alias*> SymbolTable::getAliases(llvm::StringRef name,
                                               llvm::ArrayRef<size_t> imports,
                                               llvm::ArrayRef<size_t> ns) {
   auto res = lookup(name, imports, ns, LookupKind::LK_Full);
   if (!res.isAlias())
      return {};

   return res.Entry->getAliases();
}

#define SYMTAB_HAS_X(Name)                                              \
   bool SymbolTable::has##Name(llvm::StringRef name,                    \
                               llvm::ArrayRef<size_t> imports,          \
                               llvm::ArrayRef<size_t> namespaces) {     \
      auto rec = getRecord(name, imports, namespaces);                  \
      return rec && rec->is##Name();                                    \
   }

SYMTAB_HAS_X(Union)
SYMTAB_HAS_X(Class)
SYMTAB_HAS_X(Enum)
SYMTAB_HAS_X(Struct)
SYMTAB_HAS_X(Protocol)

#undef SYMTAB_HAS_X

bool SymbolTable::hasTypedef(llvm::StringRef name,
                             llvm::ArrayRef<size_t> imports,
                             llvm::ArrayRef<size_t> namespaces) {
   auto res = lookup(name, imports, namespaces, LookupKind::LK_Full);
   return res.isTypedef();
}

SymbolTableEntry* SymbolTable::addEntry(llvm::StringRef name,
                                        SymbolTableEntry &&entry) {
   auto it = Entries.find(name);

   if (it != Entries.end()) {
      if (entry.getKind() == SymbolTableEntry::VariableID) {
         Entries[name] = std::move(entry);
      }
      else {
         SourceLocation loc;
         SourceLocation prevLoc;

         switch (entry.getKind()) {
            case SymbolTableEntry::FunctionID:
               loc = entry.getFunctions().front()->getSourceLoc();
               break;
            case SymbolTableEntry::VariableID:
               loc = entry.getVariable()->loc;
               break;
            case SymbolTableEntry::TypedefID:
               loc = entry.getTypedef()->loc;
               break;
            case SymbolTableEntry::RecordID:
               loc = entry.getRecord()->getSourceLoc();
               break;
            case SymbolTableEntry::AliasID:
               loc = entry.getAliases().front()->getDecl()->getSourceLoc();
               break;
            default:
               break;
         }
         switch (it->second.getKind()) {
            case SymbolTableEntry::FunctionID:
               prevLoc = it->second.getFunctions().front()->getSourceLoc();
               break;
            case SymbolTableEntry::VariableID:
               prevLoc = it->second.getVariable()->loc;
               break;
            case SymbolTableEntry::TypedefID:
               prevLoc = it->second.getTypedef()->loc;
               break;
            case SymbolTableEntry::RecordID:
               prevLoc = it->second.getRecord()->getSourceLoc();
               break;
            case SymbolTableEntry::AliasID:
               prevLoc = it->second.getAliases().front()->getDecl()
                           ->getSourceLoc();
               break;
            default:
               break;
         }

         auto err = diag::err(err_generic_error) << "duplicate declaration of "
                                                    + name;

         if (loc)
            err << loc;

         if (prevLoc) {
            err << diag::cont;
            diag::note(note_generic_note) << "previous declaration was here"
                                          << prevLoc << diag::term;
         }
         else {
            err << diag::term;
         }
      }

      return &it->second;
   }
   else {
      auto res = Entries.try_emplace(name, std::move(entry));
      return &res.first->second;
   }
}

SymbolTableEntry::SymbolTableEntry(SymbolTableEntry &&x)
{
   kind = x.kind;
   switch (x.kind) {
      case FunctionID:
         new (&functions) std::vector<Function*>(move(x.functions));
         x.functions.clear();
         break;
      case VariableID:
         variable = x.variable;
         x.variable = nullptr;
         break;
      case TypedefID:
         td = x.td;
         x.td = nullptr;
         break;
      case RecordID:
         record = x.record;
         x.record = nullptr;
         break;
      case AliasID:
         new (&aliases) std::vector<Alias*>(move(x.aliases));
         x.aliases.clear();
         break;
      default:
         break;
   }
}

SymbolTableEntry&
SymbolTableEntry::operator=(SymbolTableEntry &&x)
{
   if (this != &x) {
      destroyValue();
      new(this) SymbolTableEntry(std::move(x));
   }

   return *this;
}

void SymbolTableEntry::destroyValue()
{
   switch (kind) {
      case FunctionID: functions.~vector(); break;
      case VariableID: delete variable; break;
      case TypedefID: delete td; break;
      case RecordID: delete record; break;
      case AliasID: aliases.~vector(); break;
      case NamespaceID: break;
   }
}

bool SymbolTable::LookupResult::isRecord() const
{
   return kind == LRK_Record || kind == LRK_Class || kind == LRK_Struct
          || kind == LRK_Enum || kind == LRK_Union || kind == LRK_Protocol;
}

SymbolTableEntry* SymbolTable::findEntry(llvm::StringRef symbolName)
{
   auto it = Aliases.find(symbolName);
   if (it != Aliases.end())
      return findEntry(it->second);

   auto it2 = Entries.find(symbolName);
   if (it2 != Entries.end())
      return &it2->second;

   return nullptr;
}

SymbolTable::LookupResult
SymbolTable::lookup(llvm::StringRef symbolName,
                    llvm::ArrayRef<size_t> imports,
                    llvm::ArrayRef<size_t> NSStack,
                    LookupKind kind) {
   auto firstPeriod = symbolName.find_first_of('.');
   if (firstPeriod != string::npos
       && symbolName.substr(0, firstPeriod) == "Global") {
      return lookup(symbolName.substr(firstPeriod + 1), {}, kind);
   }

   LookupResult result { LookupResult::LRK_Nothing };
   auto EntryPtr = lookupInNamespaces(symbolName, Entries, NSStack, imports);

   if (!EntryPtr) {
      if (auto NS = getNamespace(symbolName)) {
         result.kind = LookupResult::LRK_Namespace;
         result.NS = NS;
      }

      return result;
   }

   auto &Entry = *EntryPtr;
   result.Entry = EntryPtr;

   bool lookupRecords = kind == LK_Full || kind == LK_Records;
   if (lookupRecords) {
      if (Entry.getKind() == SymbolTableEntry::RecordID) {
         auto rec = Entry.getRecord();
         if (rec->isEnum()) {
            result.kind = LookupResult::LRK_Enum;
         }
         else if (rec->isStruct()) {
            result.kind = LookupResult::LRK_Struct;
         }
         else if (rec->isProtocol()) {
            result.kind = LookupResult::LRK_Protocol;
         }
         else if (rec->isUnion()) {
            result.kind = LookupResult::LRK_Union;
         }
         else {
            assert(rec->isClass());
            result.kind = LookupResult::LRK_Class;
         }

         return result;
      }
   }

   bool lookupFns = kind == LK_Full || kind == LK_Functions;
   if (lookupFns) {
      if (Entry.getKind() == SymbolTableEntry::FunctionID) {
         result.kind = LookupResult::LRK_Function;
         return result;
      }
   }

   bool lookupVars = kind == LK_Full || kind == LK_Variables;
   if (lookupVars) {
      if (Entry.getKind() == SymbolTableEntry::VariableID) {
         result.kind = LookupResult::LRK_GlobalVariable;
         return result;
      }
   }

   if (Entry.getKind() == SymbolTableEntry::TypedefID) {
      result.kind = LookupResult::LRK_Typedef;
      if (kind == LK_Records && Entry.getTypedef()->aliasedType->isObjectTy())
         return lookup(Entry.getTypedef()->aliasedType->getClassName(),
                       NSStack, kind);

      return result;
   }

   if (Entry.getKind() == SymbolTableEntry::AliasID) {
      result.kind = LookupResult::LRK_Alias;
      return result;
   }

   if (auto NS = getNamespace(symbolName)) {
      result.kind = LookupResult::LRK_Namespace;
      result.NS = NS;
   }

   return result;
}

} // namespace cdot