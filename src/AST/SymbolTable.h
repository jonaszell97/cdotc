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
#include "../Variant/Type/Type.h"


class CompoundStmt;
class ClassDecl;
class Function;
class AstNode;

using std::pair;
using std::unordered_map;
using std::unordered_multimap;

namespace cdot {
   class PointerType;
   class BuiltinType;
   class ObjectType;
   class ObjectType;

   struct Argument;
   
   namespace cl {
      class Class;
      class Enum;
      class Union;
      class Record;
      struct EnumCase;
   }

   struct Variable {
      AccessModifier access;
      Type type;
      string declaredNamespace;
      AstNode *decl;
   };

   struct Typedef {
      AccessModifier access;
      BuiltinType *aliasedType;
      std::vector<GenericConstraint> generics;
      AstNode *decl;
   };
}

using namespace cdot;
using namespace cdot::cl;

using std::string;

class SymbolTable {
   typedef unordered_multimap<string, std::unique_ptr<Function>>::iterator FunctionIterator;
public:
   static void declareClass(
      Class *cl
   );

   static void declareEnum(
      Enum *en
   );

   static void declareTypedef(
      const string& alias,
      BuiltinType* originTy,
      const std::vector<GenericConstraint>& generics = {},
      AccessModifier access = AccessModifier::PUBLIC,
      AstNode *decl = nullptr
   );

   static void declareNamespace(const string &);
   static void declareVariable(
      const string &varName,
      Type &varType,
      AccessModifier access,
      const string& declaredNamespace,
      AstNode *decl
   );

   static void declareFunction(
      const string &funcName,
      std::unique_ptr<Function> &&func
   );

   static void declareUnion(
      Union *union_
   );

   static void declareTemporaryAlias(const string &, const string &);
   static void clearTemporaryAliases();

   static bool isNamespace(const string &);

   static bool hasClass(const string&);
   static bool hasClass(const string&, std::vector<string> &);

   static bool hasUnion(const string&);
   static bool hasUnion(const string&, std::vector<string> &);

   static bool hasRecord(const string&);
   static bool hasRecord(const string&, std::vector<string> &);

   static bool hasTypedef(const string&);
   static bool hasTypedef(const string&, std::vector<string> &);

   static bool hasVariable(const string&);
   static bool hasVariable(const string&, std::vector<string> &);

   static size_t numFunctionsWithName(const string &funcName);

   static cdot::cl::Class* getClass(const string&);
   static cdot::cl::Class* getClass(const string&, std::vector<string> &);

   static cdot::cl::Union* getUnion(const string&);
   static cdot::cl::Union* getUnion(const string&, std::vector<string> &);

   static Record *getRecord(const string &recordName);
   static Record *getRecord(const string &recordName, std::vector<string> &ns);

   static Typedef getTypedef(const string&);
   static Typedef getTypedef(const string&, std::vector<string> &);

   static Variable& getVariable(const string &);
   static pair<Variable&, string> getVariable(const string &, std::vector<string> &);

   static void setVariable(const string& name, BuiltinType* ty);

   static pair<FunctionIterator, FunctionIterator> getFunction(const string &);
   static pair<FunctionIterator, FunctionIterator> getFunction(const string &, std::vector<string> &);

   static string mangleVariable(const string &, size_t = 0);
   static string mangleFunction(const string &, std::vector<Argument> &args);

   static string mangleMethod(
      const string &className,
      const string &methodName,
      std::vector<Argument> &args
   );

   static Typedef* resolveTypedef(
      const string &typedefName,
      Type& target,
      std::vector<string> &importedNS
   );

   static const unordered_map<string, Record*>& getRecords();

private:
   SymbolTable() = default;

   static unordered_map<string, Record*> Records;

   static unordered_map<string, Typedef> typedefs;
   static unordered_map<string, Variable> variables;
   static std::unordered_multimap<string, std::unique_ptr<Function>> functions;
   static std::vector<string> namespaces;
   static unordered_map<string, string> TemporaryAliases;
};


#endif //CDOT_SYMBOLTABLE_H
