//
// Created by Jonas Zell on 07.07.17.
//

#ifndef CDOT_SYMBOLTABLE_H
#define CDOT_SYMBOLTABLE_H

#include <string>
#include <unordered_map>
#include <iostream>
#include <map>
#include <vector>
#include "../Util.h"
#include "../Variant/Type/QualType.h"

using std::pair;
using std::unordered_map;
using std::unordered_multimap;

namespace cdot {

class PointerType;
class Type;
class ObjectType;
class ObjectType;
class TemplateArgList;

struct Argument;

namespace cl {

class Class;
class Enum;
class Union;
class Record;
struct EnumCase;
struct RecordTemplate;
struct CallableTemplate;

} // namespace cl

namespace ast {

class CompoundStmt;
class ClassDecl;
class AstNode;
class TypeRef;
class Function;

} // namespace ast

using namespace cdot::ast;

struct Variable {
   AccessModifier access;
   QualType type;
   string declaredNamespace;
   AstNode *decl;
};

struct Typedef {
   Typedef(
      Type *ty, const string &alias,
      const std::vector<TemplateConstraint> &templateArgs = {},
      AccessModifier access = AccessModifier::PUBLIC,
      AstNode *decl = nullptr) : aliasedType(ty), alias(alias),
                                 generics(templateArgs),
                                 access(access), decl(decl)
   {

   }

   Typedef() = default;

   AccessModifier access;
   string alias;
   Type *aliasedType;
   std::vector<TemplateConstraint> generics;
   AstNode *decl;
};
}

using namespace cdot;
using namespace cdot::cl;

using std::string;

class SymbolTable {
   typedef unordered_multimap<string, std::unique_ptr<ast::Function>>::iterator
      FunctionIterator;
public:
   static void declareRecord(
      Record *cl
   );

   static void declareRecord(
      const string &name,
      Record *cl
   );

   static RecordTemplate *declareRecordTemplate(
      RecordTemplate &&templ
   );

   static void declareFunctionTemplate(
      CallableTemplate *templ
   );

   static void declareClass(
      Class *cl
   );

   static void declareClass(
      const string &name,
      Class *cl
   );

   static void declareEnum(
      Enum *en
   );

   static void declareEnum(
      const string &name,
      Enum *en
   );

   static void declareTypedef(
      const string& alias,
      Type* originTy,
      const std::vector<TemplateConstraint>& generics = {},
      AccessModifier access = AccessModifier::PUBLIC,
      AstNode *decl = nullptr
   );

   static void declareNamespace(const string &);
   static void declareVariable(
      const string &varName,
      const QualType &varType,
      AccessModifier access,
      const string& declaredNamespace,
      AstNode *decl
   );

   static void declareFunction(
      const string &funcName,
      std::unique_ptr<ast::Function> &&func
   );

   static void checkTemplateArgCompatability(
      ast::Function &newFunc
   );

   static void checkDuplicateFunctionDeclaration(
      AstNode *decl,
      const string &name,
      const string &mangledName
   );

   static void declareUnion(
      Union *union_
   );

   static void declareTemporaryAlias(const string &, const string &);
   static void clearTemporaryAliases();

   static bool isNamespace(const string &);

   static bool hasClass(const string&);
   static bool hasClass(const string&, const std::vector<string> &);

   static bool hasUnion(const string&);
   static bool hasUnion(const string&, const std::vector<string> &);

   static bool hasRecord(const string&);
   static bool hasRecord(const string&, const std::vector<string> &);
   static bool hasRecord(const string& name,
                         std::vector<TemplateArg> &args,
                         const std::vector<string> &importedNamespaces);

   static bool hasRecordTemplate(const string &name);
   static bool hasRecordTemplate(const string&, const std::vector<string> &);

   static bool hasFunctionTemplate(const string &name);
   static bool hasFunctionTemplate(const string&, const std::vector<string> &);

   static bool hasTypedef(const string&);
   static bool hasTypedef(const string&, const std::vector<string> &);

   static bool hasVariable(const string&);
   static bool hasVariable(const string&, const std::vector<string> &);

   static size_t numFunctionsWithName(const string &funcName);

   static cdot::cl::Class* getClass(const string&);
   static cdot::cl::Class* getClass(const string&, const std::vector<string> &);

   static cdot::cl::Union* getUnion(const string&);
   static cdot::cl::Union* getUnion(const string&, const std::vector<string> &);

   static Record *getRecord(const string &recordName);
   static Record *getRecord(const string &recordName,
                            const std::vector<string> &ns);
   static Record *getRecord(const string &recordName,
                            TemplateArgList *argList,
                            const std::vector<string> &ns = {},
                            bool *isNew = nullptr);

   static ast::Function *getAnyFn(const string &withName,
                                  const std::vector<string> &ns);

   static RecordTemplate *getRecordTemplate(const string &name);
   static RecordTemplate *getRecordTemplate(const string &name,
                                            const std::vector<string> &ns);

   static CallableTemplate &getFunctionTemplate(const string &name);
   static CallableTemplate &getFunctionTemplate(const string &name,
                                                const std::vector<string> &ns);

   static bool isTemplatedFunction(const string &funcName);
   static std::vector<TemplateConstraint> &getConstraints(
      const string &forFunction);

   static Typedef getTypedef(const string&);
   static Typedef getTypedef(const string&, const std::vector<string> &);

   static Variable& getVariable(const string &);
   static pair<Variable&, string> getVariable(const string &,
                                              const std::vector<string> &);

   static void setVariable(const string& name, Type* ty);

   static pair<FunctionIterator, FunctionIterator> getFunction(const string &);
   static pair<FunctionIterator, FunctionIterator> getFunction(
      const string &, const std::vector<string> &);

   static string mangleVariable(const string &, size_t = 0);
   static string mangleFunction(const string &, std::vector<Argument> &args);
   static string mangleFunction(const string &,
                                std::vector<std::shared_ptr<TypeRef>> &args);

   static string mangleMethod(
      const string &className,
      const string &methodName,
      std::vector<Argument> &args,
      bool resolveSelf = false
   );

   static Typedef& resolveTypedef(
      const string &typedefName,
      std::vector<string> &importedNS
   );

   static const unordered_map<string, Record*>& getRecords();

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

         LRK_RecordTemplate,
         LRK_FunctionTemplate
      };

      bool isRecord() const;
      bool isFunction() const;
      bool isGlobalVariable() const;
      bool isRecordTemplate() const;
      bool isFunctionTemplate() const;

      LookupResultKind kind;
      union {
         Record *record = nullptr;
         Class *cl;
         Enum *en;
         Union *un;
         ast::Function *fun;
         RecordTemplate *recordTemplate;
         CallableTemplate *callableTemplate;
      };
   };

   static LookupResult lookup(const string &symbolName,
                              const std::vector<string> &inNamespaces,
                              LookupKind kind = LK_Full);

private:
   SymbolTable() = default;

   static unordered_map<string, Record*> Records;
   static unordered_map<string, RecordTemplate> RecordTemplates;

   static unordered_map<string, Typedef> typedefs;
   static unordered_map<string, Variable> variables;
   static std::unordered_multimap<string, std::unique_ptr<ast::Function>>
      functions;
   static std::vector<string> namespaces;
   static unordered_map<string, string> TemporaryAliases;
};


#endif //CDOT_SYMBOLTABLE_H
