////
//// Created by Jonas Zell on 07.07.17.
////
//
//#ifndef CDOT_SYMBOLTABLE_H
//#define CDOT_SYMBOLTABLE_H
//
//#include <string>
//#include <unordered_map>
//#include <vector>
//#include <llvm/ADT/SmallString.h>
//
//#include "../Variant/Type/Generic.h"
//#include "../Variant/Type/QualType.h"
//#include "../Util.h"
//
//using std::pair;
//using std::unordered_map;
//
//namespace cdot {
//
//class PointerType;
//class Type;
//class ObjectType;
//
//struct Argument;
//
//namespace ast {
//   class CompoundStmt;
//   class ClassDecl;
//   class StructDecl;
//   class EnumDecl;
//   class UnionDecl;
//   class ProtocolDecl;
//   class AstNode;
//   class TypeRef;
//   class LocalVarDecl;
//   class FieldDecl;
//   class AliasDecl;
//   class FunctionDecl;
//   class MethodDecl;
//   class TypedefDecl;
//   class RecordDecl;
//   class NamespaceDecl;
//   class CallableDecl;
//   class NamedDecl;
//} // namespace ast
//
//namespace module {
//   class Module;
//} // namespace module
//
//using namespace cdot::ast;
//
//struct Variable {
//   Variable(llvm::StringRef name, AccessModifier access, const QualType &type,
//            NamespaceDecl *declarationNamespace, const SourceLocation &loc,
//            bool isTypeDependant, ast::Statement *decl)
//      : name(name), access(access), type(type),
//        declarationNamespace(declarationNamespace),
//        loc(loc), isTypeDependant(isTypeDependant),
//        isGlobal(true), decl(decl)
//   { }
//
//   const string &getName() const
//   {
//      return name;
//   }
//
//   AccessModifier getAccess() const
//   {
//      return access;
//   }
//
//   const QualType &getType() const
//   {
//      return type;
//   }
//
//   const SourceLocation &getLoc() const
//   {
//      return loc;
//   }
//
//   bool isIsTypeDependant() const
//   {
//      return isTypeDependant;
//   }
//
//   bool isIsGlobal() const
//   {
//      return isGlobal;
//   }
//
//   Statement *getDecl() const
//   {
//      return decl;
//   }
//
//   NamespaceDecl *getDeclarationNamespace() const
//   {
//      return declarationNamespace;
//   }
//
//   string name;
//   AccessModifier access;
//   QualType type;
//   NamespaceDecl *declarationNamespace;
//   SourceLocation loc;
//
//   bool isTypeDependant;
//   bool isGlobal;
//
//   ast::Statement *decl;
//};
//
//class SymbolTableEntry {
//public:
//   enum Kind : unsigned char {
//      RecordID,
//      TypedefID,
//      VariableID,
//      FunctionID,
//      NamespaceID,
//      AliasID, // Must come last!
//   };
//
//   SymbolTableEntry(RecordDecl *rec) : kind(RecordID), record(rec) {}
//   SymbolTableEntry(TypedefDecl *td) : kind(TypedefID), td(td) {}
//   SymbolTableEntry(AliasDecl *alias) : kind(AliasID), alias(alias) {}
//   SymbolTableEntry(Variable *var)
//      : kind(VariableID), variable(var) {}
//   SymbolTableEntry(FunctionDecl *func) : kind(FunctionID), function(func) {}
//   SymbolTableEntry(NamespaceDecl *NS)
//      : kind(NamespaceID), NS(NS) {}
//
//   Kind getKind() const { return kind; }
//   ast::RecordDecl *getRecord() const { return record; }
//   TypedefDecl *getTypedef() const { return td; }
//   Variable *getVariable() const { return variable; }
//   FunctionDecl* getFunction() const { return function; }
//   NamespaceDecl *getNamespace() const { return NS; }
//   AliasDecl *getAlias() const { return alias; }
//   NamedDecl *getNamedDecl() const { return decl; }
//
//   bool isRecord()   const { return kind == RecordID; }
//   bool isTypedef()  const { return kind == TypedefID; }
//   bool isVariable() const { return kind == VariableID; }
//   bool isFunction() const { return kind == FunctionID; }
//   bool isAlias()    const { return kind == AliasID; }
//
//   bool isDuplicate() const { return duplicate; }
//
//   void setDuplicate(bool duplicate)
//   {
//      SymbolTableEntry::duplicate = duplicate;
//   }
//
//private:
//   Kind kind;
//
//   union {
//      ast::RecordDecl *record;
//      TypedefDecl *td;
//      Variable *variable;
//      NamespaceDecl *NS;
//      FunctionDecl* function;
//      AliasDecl* alias;
//
//      NamedDecl *decl;
//   };
//
//   bool duplicate : 1;
//};
//
//class SymbolTable {
//public:
//   SymbolTable()
//   {
//   }
//
//   size_t getNamespaceId(llvm::StringRef nsName) const;
//   NamespaceDecl *getNamespace(size_t id) const
//   {
//      auto it = NamespaceIDs.find(id);
//      if (it == NamespaceIDs.end())
//         return nullptr;
//
//      return it->second;
//   }
//
//   NamespaceDecl *getNamespace(llvm::StringRef name) const;
//
//   const llvm::StringMap<NamespaceDecl *> &getNamespaces() const
//   {
//      return Namespaces;
//   }
//
//   SymbolTableEntry const* duplicateEntry(llvm::StringRef newName,
//                                          SymbolTableEntry const* Entry) {
//      SymbolTableEntry cpy(*Entry);
//      cpy.setDuplicate(true);
//
//      return &Entries.try_emplace(newName, cpy).first->getValue();
//   }
//
//   SymbolTableEntry const* declareRecord(RecordDecl *R);
//   SymbolTableEntry const* declareRecord(llvm::StringRef name, RecordDecl *R);
//
//   SymbolTableEntry const *declareTypedef(TypedefDecl *decl);
//   SymbolTableEntry const *declareAlias(ast::AliasDecl *decl);
//
//   SymbolTableEntry const *declareTypedef(llvm::StringRef name,
//                                          TypedefDecl *decl);
//
//   SymbolTableEntry const *declareAlias(llvm::StringRef name,
//                                        ast::AliasDecl *decl);
//
//   size_t declareNamespace(ast::NamespaceDecl *decl);
//
//   SymbolTableEntry* declareVariable(llvm::StringRef varName,
//                                     const QualType &varType,
//                                     AccessModifier access,
//                                     bool typeDependant,
//                                     size_t nsId,
//                                     ast::Statement *decl,
//                                     const SourceLocation &loc = {});
//
//   SymbolTableEntry* declareVariable(llvm::StringRef varName,
//                                     const QualType &varType,
//                                     AccessModifier access,
//                                     bool typeDependant,
//                                     NamespaceDecl *ns,
//                                     ast::Statement *decl,
//                                     const SourceLocation &loc = {});
//
//   SymbolTableEntry* declareFunction(llvm::StringRef funcName,
//                                     ast::FunctionDecl *func);
//
//   SymbolTableEntry* declareFunction(ast::FunctionDecl *func);
//
//   size_t isNamespace(llvm::StringRef name,
//                      llvm::ArrayRef<size_t> imports = {},
//                      NamespaceDecl *ns = {}) const;
//
//   bool hasClass(llvm::StringRef name,
//                 llvm::ArrayRef<size_t> imports = {},
//                 NamespaceDecl *ns = {}) const;
//
//   bool hasStruct(llvm::StringRef name,
//                  llvm::ArrayRef<size_t> imports = {},
//                  NamespaceDecl *ns = {}) const;
//
//   bool hasProtocol(llvm::StringRef name,
//                    llvm::ArrayRef<size_t> imports = {},
//                    NamespaceDecl *ns = {}) const;
//
//   bool hasUnion(llvm::StringRef name,
//                 llvm::ArrayRef<size_t> imports = {},
//                 NamespaceDecl *ns = {}) const;
//
//   bool hasEnum(llvm::StringRef name,
//                llvm::ArrayRef<size_t> imports = {},
//                NamespaceDecl *ns = {}) const;
//
//   bool hasRecord(llvm::StringRef name,
//                  llvm::ArrayRef<size_t> imports = {},
//                  NamespaceDecl *ns = {}) const;
//
//   bool hasTypedef(llvm::StringRef name,
//                   llvm::ArrayRef<size_t> imports = {},
//                   NamespaceDecl *ns = {}) const;
//
//   bool hasVariable(llvm::StringRef name,
//                    llvm::ArrayRef<size_t> imports = {},
//                    NamespaceDecl *ns = {}) const;
//
//   size_t numFunctionsWithName(llvm::StringRef funcName) const;
//
//   ast::ClassDecl *getClass(llvm::StringRef name,
//                            llvm::ArrayRef<size_t> imports = {},
//                            NamespaceDecl *ns = {}) const;
//
//   ast::StructDecl* getStruct(llvm::StringRef name,
//                              llvm::ArrayRef<size_t> imports = {},
//                              NamespaceDecl *ns = {}) const;
//
//   ast::ProtocolDecl* getProtocol(llvm::StringRef name,
//                                  llvm::ArrayRef<size_t> imports = {},
//                                  NamespaceDecl *ns = {}) const;
//
//   ast::UnionDecl* getUnion(llvm::StringRef name,
//                            llvm::ArrayRef<size_t> imports = {},
//                            NamespaceDecl *ns = {}) const;
//
//   ast::EnumDecl* getEnum(llvm::StringRef name,
//                          llvm::ArrayRef<size_t> imports = {},
//                          NamespaceDecl *ns = {}) const;
//
//   ast::RecordDecl *getRecord(llvm::StringRef recordName,
//                              llvm::ArrayRef<size_t> imports = {},
//                              NamespaceDecl *ns = {}) const;
//
//   ast::FunctionDecl *getAnyFn(llvm::StringRef withName,
//                               llvm::ArrayRef<size_t> imports = {},
//                               NamespaceDecl *ns = {}) const;
//
//   TypedefDecl *getTypedef(llvm::StringRef name,
//                           llvm::ArrayRef<size_t> imports = {},
//                           NamespaceDecl *ns = {}) const;
//
//   llvm::ArrayRef<AliasDecl*> getAliases(llvm::StringRef name,
//                                         llvm::ArrayRef<size_t> imports,
//                                         NamespaceDecl *ns = {}) const;
//
//   Variable *getVariable(llvm::StringRef name,
//                         llvm::ArrayRef<size_t> imports = {},
//                         NamespaceDecl *ns = {}) const;
//
//   void setVariable(llvm::StringRef name,
//                    QualType ty) const;
//
//   const bool hasFunction(llvm::StringRef name,
//                          llvm::ArrayRef<size_t> imports = {},
//                          NamespaceDecl *ns = {}) const;
//
//   llvm::ArrayRef<CallableDecl*>
//   getFunctionOverloads(llvm::StringRef name,
//                        llvm::ArrayRef<size_t> imports = { },
//                        NamespaceDecl *ns = {});
//
//   enum LookupKind {
//      LK_Full,
//      LK_Records,
//      LK_Functions,
//      LK_Variables
//   };
//
//   struct LookupResult {
//      enum LookupResultKind {
//         LRK_Nothing,
//
//         LRK_Record,
//         LRK_Class,
//         LRK_Enum,
//         LRK_Struct,
//         LRK_Protocol,
//         LRK_Union,
//
//         LRK_Function,
//         LRK_GlobalVariable,
//         LRK_Typedef,
//
//         LRK_Alias,
//
//         LRK_Namespace,
//      };
//
//      bool isRecord() const;
//      bool isFunction() const { return kind == LRK_Function; }
//      bool isGlobalVariable() const { return kind == LRK_GlobalVariable; }
//      bool isTypedef() const { return kind == LRK_Typedef; }
//      bool isNamespace() const { return kind == LRK_Namespace; }
//      bool isAlias() const { return kind == LRK_Alias; }
//      size_t getNamespaceID() const { return namespaceID; }
//      size_t getSpecificity() const { return specificity; }
//
//      LookupResultKind kind;
//      size_t namespaceID = 0;
//
//      union {
//         SymbolTableEntry const* Entry;
//         ast::NamespaceDecl *NS;
//      };
//
//      size_t specificity = 0;
//   };
//
//   SymbolTableEntry const* findEntry(llvm::StringRef symbolName) const;
//   LookupResult lookup(llvm::StringRef symbolName,
//                       llvm::ArrayRef<size_t> inNamespaces,
//                       NamespaceDecl *NSStack,
//                       LookupKind kind = LK_Full) const;
//
//   const llvm::StringMap<SymbolTableEntry> &getEntries() const
//   {
//      return Entries;
//   }
//
//private:
//   static size_t lastNamespaceId;
//
//   SymbolTableEntry* addEntry(llvm::StringRef name, SymbolTableEntry &&entry);
//
//   llvm::StringMap<SymbolTableEntry> Entries;
//   llvm::StringMap<NamespaceDecl*> Namespaces;
//   std::unordered_map<size_t, NamespaceDecl*> NamespaceIDs;
//
//   llvm::StringMap<string> Aliases;
//
//   NamespaceDecl const* lookupNamespace(llvm::StringRef name) const;
//
//   template<class T>
//   T const* findSingleEntry(llvm::StringRef needle,
//                            llvm::StringMap<T> const& haystack) const {
//      auto it = Aliases.find(needle);
//      if (it != Aliases.end())
//         return findSingleEntry(it->second, haystack);
//
//      auto it2 = haystack.find(needle);
//      if (it2 != haystack.end())
//         return &it2->second;
//
//      return nullptr;
//   }
//
//   template<class T>
//   T const* lookupInNamespaces(llvm::StringRef needle,
//                               llvm::StringMap<T> const& haystack,
//                               NamespaceDecl *NSStack,
//                               llvm::ArrayRef<size_t> imports) const;
//};
//
//} // namespace cdot
//
//#endif //CDOT_SYMBOLTABLE_H
