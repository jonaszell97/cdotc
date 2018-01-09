//
// Created by Jonas Zell on 07.07.17.
//

#ifndef CDOT_SYMBOLTABLE_H
#define CDOT_SYMBOLTABLE_H

#include <string>
#include <unordered_map>
#include <vector>

#include "../Variant/Type/Generic.h"
#include "../Variant/Type/QualType.h"
#include "../Util.h"

using std::pair;
using std::unordered_map;

namespace cdot {

class PointerType;
class Type;
class ObjectType;
class ObjectType;

struct Argument;

namespace cl {

class Class;
class Struct;
class Protocol;
class Enum;
class Union;
class Record;
class AssociatedType;

struct EnumCase;

} // namespace cl

namespace ast {

class CompoundStmt;
class ClassDecl;
class AstNode;
class TypeRef;
class Function;
class LocalVarDecl;
class FieldDecl;
class AliasDecl;
class FunctionDecl;
class MethodDecl;

} // namespace ast

using namespace cdot::ast;

struct Variable {
   Variable(const string &name, AccessModifier access, const QualType &type,
            size_t declaredNamespace, const SourceLocation &loc,
            bool isTypeDependant, ast::Statement *decl)
      : name(name), access(access), type(type),
        declaredNamespace(declaredNamespace),
        loc(loc), isTypeDependant(isTypeDependant),
        isGlobal(true), decl(decl)
   { }

   const string &getName() const
   {
      return name;
   }

   AccessModifier getAccess() const
   {
      return access;
   }

   const QualType &getType() const
   {
      return type;
   }

   size_t getDeclaredNamespace() const
   {
      return declaredNamespace;
   }

   const SourceLocation &getLoc() const
   {
      return loc;
   }

   bool isIsTypeDependant() const
   {
      return isTypeDependant;
   }

   bool isIsGlobal() const
   {
      return isGlobal;
   }

   Statement *getDecl() const
   {
      return decl;
   }

   string name;
   AccessModifier access;
   QualType type;
   size_t declaredNamespace;
   SourceLocation loc;

   bool isTypeDependant;
   bool isGlobal;

   ast::Statement *decl;
};

struct Typedef {
   Typedef(
      Type *ty, llvm::StringRef alias,
      std::vector<TemplateParameter> &&params,
      AccessModifier access = AccessModifier::PUBLIC,
      const SourceLocation &loc = {}) : access(access), alias(alias),
                                        aliasedType(ty),
                                        templateParams(std::move(params)),
                                        loc(loc)
   {

   }

   Typedef() = default;

   AccessModifier getAccess() const
   {
      return access;
   }

   const string &getAlias() const
   {
      return alias;
   }

   Type *getAliasedType() const
   {
      return aliasedType;
   }

   const std::vector<TemplateParameter> &getTemplateParams() const
   {
      return templateParams;
   }

   const SourceLocation &getLoc() const
   {
      return loc;
   }

   AccessModifier access;
   string alias;
   Type *aliasedType;
   std::vector<TemplateParameter> templateParams;
   SourceLocation loc;
};

struct Alias {
public:
   Alias(string &&name,
         std::vector<TemplateParameter> &&templateParams,
         std::vector<std::shared_ptr<StaticExpr>> &&constraints,
         std::shared_ptr<StaticExpr> &&aliasExpr,
         ast::AliasDecl *decl)
      : name(move(name)), templateParams(move(templateParams)),
        constraints(move(constraints)), aliasExpr(move(aliasExpr)), decl(decl)
   { }

private:
   std::string name;
   std::vector<TemplateParameter> templateParams;
   std::vector<std::shared_ptr<StaticExpr>> constraints;

   std::shared_ptr<StaticExpr> aliasExpr;
   ast::AliasDecl *decl;

   Variant Val;
   bool typeDependant = false;

public:
   const string &getName() const
   {
      return name;
   }

   std::vector<TemplateParameter> &getTemplateParams()
   {
      return templateParams;
   }

   std::vector<TemplateParameter> const& getTemplateParams() const
   {
      return templateParams;
   }

   const std::vector<std::shared_ptr<StaticExpr>> &getConstraints() const
   {
      return constraints;
   }

   const std::shared_ptr<StaticExpr> &getAliasExpr() const
   {
      return aliasExpr;
   }

   AliasDecl *getDecl() const
   {
      return decl;
   }

   const Variant &getVal() const
   {
      return Val;
   }

   void setVal(Variant &&Val)
   {
      Alias::Val = std::move(Val);
   }

   bool isTypeDependant() const
   {
      return typeDependant;
   }

   void setIsTypeDependant(bool isTypeDependant)
   {
      Alias::typeDependant = isTypeDependant;
   }
};

struct Namespace {
   Namespace(size_t id, string &&name, bool isAnonymous,
             Namespace *parentNamespace)
      : id(id), name(move(name)), isAnonymous(isAnonymous),
        parentNamespace(parentNamespace)
   {

   }

   size_t getId() const
   {
      return id;
   }

   const string &getName() const
   {
      return name;
   }

   bool isIsAnonymous() const
   {
      return isAnonymous;
   }

   Namespace *getParentNamespace() const
   {
      return parentNamespace;
   }

   size_t id;
   std::string name;
   bool isAnonymous;
   Namespace *parentNamespace;

   static size_t lastId;
};

using namespace cdot;
using namespace cdot::cl;

using std::string;

class SymbolTableEntry {
public:
   enum Kind {
      RecordID,
      TypedefID,
      VariableID,
      FunctionID,
      NamespaceID,
      AliasID,
   };

   SymbolTableEntry(Record *rec) : kind(RecordID), record(rec) {}
   SymbolTableEntry(Typedef *td) : kind(TypedefID), td(td) {}
   SymbolTableEntry(Alias *alias) : kind(AliasID), aliases{alias} {}
   SymbolTableEntry(Variable *var)
      : kind(VariableID), variable(var) {}
   SymbolTableEntry(Function *func)
      : kind(FunctionID), functions{func} {}
   SymbolTableEntry(Namespace *NS)
      : kind(NamespaceID), NS(NS) {}
   SymbolTableEntry()
      : kind(FunctionID), functions{} {}

   SymbolTableEntry(const SymbolTableEntry &) = delete;
   SymbolTableEntry &operator=(const SymbolTableEntry &) = delete;

   SymbolTableEntry(SymbolTableEntry &&);
   SymbolTableEntry &operator=(SymbolTableEntry &&);
   ~SymbolTableEntry() { destroyValue(); }

   Kind getKind() const { return kind; }
   Record *getRecord() const { return record; }
   Typedef *getTypedef() const { return td; }
   Variable *getVariable() const { return variable; }
   std::vector<Function *> &getFunctions() { return functions; }
   Namespace *getNamespace() const { return NS; }
   std::vector<Alias*> &getAliases() { return aliases; }

private:
   void destroyValue();

   Kind kind;

   union {
      Record *record;
      Typedef *td;
      Variable *variable;
      Namespace *NS;
      std::vector<Function*> functions;
      std::vector<Alias*> aliases;
   };
};

class SymbolTable {
public:
   static void addBuiltins();

   static size_t getNamespaceId(llvm::StringRef nsName);
   static Namespace *getNamespace(size_t id)
   {
      auto it = NamespaceIDs.find(id);
      if (it == NamespaceIDs.end())
         return nullptr;

      return it->second;
   }

   static Namespace *getNamespace(llvm::StringRef name)
   {
      return getNamespace(getNamespaceId(name));
   }

   static const string& getNamespaceName(size_t id);

   static SymbolTableEntry* declareRecord(Record *cl);
   static SymbolTableEntry* declareRecord(llvm::StringRef name, Record *cl);

   static SymbolTableEntry *ForwardDeclareTypedef(llvm::StringRef alias,
                                                  AccessModifier access,
                                                  SourceLocation loc);

   static SymbolTableEntry* declareTypedef(
                           llvm::StringRef alias,
                           Type* originTy,
                           std::vector<TemplateParameter> &&generics = {},
                           AccessModifier access = AccessModifier::PUBLIC,
                           const SourceLocation &loc = {});

   static SymbolTableEntry*
   declareAlias(llvm::StringRef name,
                std::vector<TemplateParameter> &&templateParams,
                std::vector<std::shared_ptr<StaticExpr>> &&constraints,
                std::shared_ptr<StaticExpr> &&aliasExpr,
                ast::AliasDecl *decl);

   static size_t declareNamespace(llvm::StringRef name,
                                  bool isAnonymous = false,
                                  Namespace *parentNamespace = nullptr);

   static SymbolTableEntry* declareVariable(llvm::StringRef varName,
                                            const QualType &varType,
                                            AccessModifier access,
                                            bool typeDependant,
                                            size_t nsId,
                                            ast::Statement *decl,
                                            const SourceLocation &loc = {});

   static SymbolTableEntry* declareFunction(llvm::StringRef funcName,
                                            ast::Function *func);

   static SymbolTableEntry* declareFunction(ast::Function *func);

   static void checkTemplateArgCompatability(ast::Function &newFunc);

   static void checkDuplicateFunctionDeclaration(const SourceLocation &loc,
                                                 llvm::StringRef name,
                                                 llvm::StringRef mangledName);

   static void declareAlias(const llvm::Twine &alias,
                            llvm::StringRef aliasee);

   static size_t isNamespace(llvm::StringRef name,
                             llvm::ArrayRef<size_t> imports = {},
                             llvm::ArrayRef<size_t> ns = {});

   static bool hasClass(llvm::StringRef name,
                        llvm::ArrayRef<size_t> imports = {},
                        llvm::ArrayRef<size_t> ns = {});

   static bool hasStruct(llvm::StringRef name,
                         llvm::ArrayRef<size_t> imports = {},
                         llvm::ArrayRef<size_t> ns = {});

   static bool hasProtocol(llvm::StringRef name,
                           llvm::ArrayRef<size_t> imports = {},
                          llvm::ArrayRef<size_t> ns = {});

   static bool hasUnion(llvm::StringRef name,
                        llvm::ArrayRef<size_t> imports = {},
                        llvm::ArrayRef<size_t> ns = {});

   static bool hasEnum(llvm::StringRef name,
                       llvm::ArrayRef<size_t> imports = {},
                       llvm::ArrayRef<size_t> ns = {});

   static bool hasRecord(llvm::StringRef name,
                         llvm::ArrayRef<size_t> imports = {},
                         llvm::ArrayRef<size_t> ns = {});

   static bool hasFunctionTemplate(llvm::StringRef name,
                                   llvm::ArrayRef<size_t> imports = {},
                                   llvm::ArrayRef<size_t> ns = {});

   static bool hasTypedef(llvm::StringRef name,
                          llvm::ArrayRef<size_t> imports = {},
                          llvm::ArrayRef<size_t> ns = {});

   static bool hasVariable(llvm::StringRef name,
                           llvm::ArrayRef<size_t> imports = {},
                           llvm::ArrayRef<size_t> ns = {});

   static size_t numFunctionsWithName(llvm::StringRef funcName);

   static cdot::cl::Class* getClass(llvm::StringRef name,
                                    llvm::ArrayRef<size_t> imports = {},
                                    llvm::ArrayRef<size_t> ns = {});

   static cdot::cl::Struct* getStruct(llvm::StringRef name,
                                      llvm::ArrayRef<size_t> imports = {},
                                     llvm::ArrayRef<size_t> ns = {});

   static cdot::cl::Protocol* getProtocol(llvm::StringRef name,
                                          llvm::ArrayRef<size_t> imports = {},
                                          llvm::ArrayRef<size_t> ns = {});

   static cdot::cl::Union* getUnion(llvm::StringRef name,
                                    llvm::ArrayRef<size_t> imports = {},
                                    llvm::ArrayRef<size_t> ns = {});

   static cdot::cl::Enum* getEnum(llvm::StringRef name,
                                  llvm::ArrayRef<size_t> imports = {},
                                  llvm::ArrayRef<size_t> ns = {});

   static Record *getRecord(llvm::StringRef recordName,
                            llvm::ArrayRef<size_t> imports = {},
                            llvm::ArrayRef<size_t> ns = {});

   static ast::Function *getAnyFn(llvm::StringRef withName,
                                  llvm::ArrayRef<size_t> imports = {},
                                  llvm::ArrayRef<size_t> ns = {});

   static bool isTemplatedFunction(llvm::StringRef funcName);
   static const std::vector<TemplateParameter> &getConstraints(
      llvm::StringRef forFunction);

   static Typedef *getTypedef(llvm::StringRef name,
                              llvm::ArrayRef<size_t> imports = {},
                              llvm::ArrayRef<size_t> ns = {});

   static llvm::ArrayRef<Alias*> getAliases(llvm::StringRef name,
                                            llvm::ArrayRef<size_t> imports,
                                            llvm::ArrayRef<size_t> ns);

   static Variable *getVariable(llvm::StringRef name,
                                llvm::ArrayRef<size_t> imports = {},
                                llvm::ArrayRef<size_t> ns = {});

   static void setVariable(llvm::StringRef name,
                           QualType ty);

   static const bool hasFunction(llvm::StringRef name,
                                 llvm::ArrayRef<size_t> imports = {},
                                 llvm::ArrayRef<size_t> ns = {});

   static llvm::ArrayRef<Function*> getFunctionOverloads(
                                          llvm::StringRef name,
                                          llvm::ArrayRef<size_t> imports = { },
                                          llvm::ArrayRef<size_t> ns = { });

   enum LookupKind {
      LK_Full,
      LK_Records,
      LK_Functions,
      LK_Variables
   };

   struct LookupResult {
      enum LookupResultKind {
         LRK_Nothing,

         LRK_Record,
         LRK_Class,
         LRK_Enum,
         LRK_Struct,
         LRK_Protocol,
         LRK_Union,

         LRK_Function,
         LRK_GlobalVariable,
         LRK_Typedef,

         LRK_Alias,

         LRK_Namespace,
      };

      bool isRecord() const;
      bool isFunction() const { return kind == LRK_Function; }
      bool isGlobalVariable() const { return kind == LRK_GlobalVariable; }
      bool isTypedef() const { return kind == LRK_Typedef; }
      bool isNamespace() const { return kind == LRK_Namespace; }
      bool isAlias() const { return kind == LRK_Alias; }
      size_t getNamespaceID() const { return namespaceID; }
      size_t getSpecificity() const { return specificity; }

      LookupResultKind kind;
      size_t namespaceID = 0;

      union {
         SymbolTableEntry *Entry;
         Namespace *NS;
      };

      size_t specificity = 0;
   };

   static SymbolTableEntry *findEntry(llvm::StringRef symbolName);
   static LookupResult lookup(llvm::StringRef symbolName,
                              llvm::ArrayRef<size_t> inNamespaces,
                              llvm::ArrayRef<size_t> NSStack,
                              LookupKind kind = LK_Full);

private:
   SymbolTable() = default;

public:

   static const llvm::StringMap<SymbolTableEntry> &getEntries()
   {
      return Entries;
   }

private:
   static SymbolTableEntry*
   addEntry(llvm::StringRef name, SymbolTableEntry &&entry);

   static llvm::StringMap<SymbolTableEntry> Entries;
   static llvm::StringMap<Namespace> Namespaces;
   static std::unordered_map<size_t, Namespace*> NamespaceIDs;

   static llvm::StringMap<string> Aliases;

   static Namespace *lookupNamespace(llvm::StringRef name);

   template<class T>
   static T *findSingleEntry(llvm::StringRef needle,
                             llvm::StringMap<T> &haystack) {
      auto it = Aliases.find(needle);
      if (it != Aliases.end())
         return findSingleEntry(it->second, haystack);

      auto it2 = haystack.find(needle);
      if (it2 != haystack.end())
         return &it2->second;

      return nullptr;
   }

   template<class T>
   static T *lookupInNamespaces(llvm::StringRef needle,
                                llvm::StringMap<T> &haystack,
                                llvm::ArrayRef<size_t> NSStack,
                                llvm::ArrayRef<size_t> imports) {
      auto EntryPtr = findSingleEntry(needle, haystack);
      bool foundInNamespace = false;
      llvm::SmallString<128> ScratchBuf;

      for (auto id : NSStack) {
         auto ns = NamespaceIDs.find(id);
         if (ns == NamespaceIDs.end())
            continue;

         ScratchBuf += ns->second->name;
         ScratchBuf += '.';
         ScratchBuf += needle;

         auto NextEntry = findSingleEntry(ScratchBuf.str(), haystack);
         if (NextEntry) {
            EntryPtr = NextEntry;
            foundInNamespace = true;
            break;
         }

         ScratchBuf.clear();
      }

      if (!foundInNamespace) {
         for (auto id : imports) {
            auto ns = NamespaceIDs.find(id);
            if (ns == NamespaceIDs.end())
               continue;

            ScratchBuf += ns->second->name;
            ScratchBuf += '.';
            ScratchBuf += needle;

            auto NextEntry = findSingleEntry(ScratchBuf.str(), haystack);
            if (NextEntry) {
               EntryPtr = NextEntry;
               break;
            }

            ScratchBuf.clear();
         }
      }

      return EntryPtr;
   }
};

} // namespace cdot

#endif //CDOT_SYMBOLTABLE_H
