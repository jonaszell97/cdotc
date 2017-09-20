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


class CompoundStmt;
class ClassDecl;
class Function;

using std::pair;
using std::unordered_map;
using std::unordered_multimap;

namespace cdot {
   
   class PointerType;
   class Type;
   class ObjectType;
   class ObjectType;

   struct Argument;
   
   namespace cl {
      class Class;
      class Enum;
      struct EnumCase;
   }

   struct Variable {
      AccessModifier access;
      Type* type;
      string declaredNamespace;
   };

   struct Typedef {
      AccessModifier access;
      Type* aliasedType;
   };
}

using namespace cdot;
using cdot::cl::EnumCase;

using std::string;

class SymbolTable {
public:
   static cdot::cl::Class* declareClass(
      AccessModifier am,
      string class_name,
      ObjectType *extends,
      std::vector<ObjectType *> implements,
      std::vector<ObjectType *> generics,
      ClassDecl *decl,
      bool is_abstract
   );
   static cdot::cl::Class* declareClass(
      AccessModifier am,
      string,
      std::vector<ObjectType *>,
      std::vector<ObjectType *>,
      bool,
      ClassDecl *
   );

   static cdot::cl::Enum* declareEnum(
      AccessModifier am,
      string& name,
      std::vector<ObjectType*>& conformsTo,
      std::vector<ObjectType*>& generics
   );

   static void declareTypedef(string, Type *, AccessModifier access = AccessModifier::PUBLIC);
   static void declareNamespace(string &);
   static void declareVariable(string &, Type *, AccessModifier access, string& declaredNamespace);
   static void declareFunction(string &, std::unique_ptr<Function> &&);

   static void declareTemporaryAlias(string &, string &);
   static void clearTemporaryAliases();

   static bool isNamespace(string &);

   static bool hasClass(string&);
   static bool hasClass(string&, std::vector<string> &);

   static bool hasTypedef(string&);
   static bool hasTypedef(string&, std::vector<string> &);

   static bool hasVariable(string&);
   static bool hasVariable(string&, std::vector<string> &);

   static size_t numFunctionsWithName(string &funcName);

   static cdot::cl::Class* getClass(string);
   static cdot::cl::Class* getClass(string, std::vector<string> &);

   static Typedef getTypedef(string&);
   static Typedef getTypedef(string&, std::vector<string> &);

   static Variable& getVariable(string &);
   static pair<Variable&, string> getVariable(string &, std::vector<string> &);

   static void setVariable(string& name, Type* ty);

   static pair<unordered_multimap<string, std::unique_ptr<Function>>::iterator,
      unordered_multimap<string, std::unique_ptr<Function>>::iterator> getFunction(string &);
   static pair<unordered_multimap<string, std::unique_ptr<Function>>::iterator,
      unordered_multimap<string, std::unique_ptr<Function>>::iterator> getFunction(string &, std::vector<string> &);

   static string mangleVariable(string &, Type *);
   static string mangleVariable(string &, size_t = 0);
   static string mangleFunction(string &, std::vector<Argument> &args);

   static string mangleMethod(string &, string &, std::vector<Argument> &args);

   static void resolveTypedef(Type *&, std::vector<string> &);

private:
   SymbolTable() = default;

   static std::unordered_map<string, std::unique_ptr<cdot::cl::Class>> classes;
   static std::unordered_map<string, Typedef> typedefs;
   static std::unordered_map<string, Variable> variables;
   static std::unordered_multimap<string, std::unique_ptr<Function>> functions;
   static std::vector<string> namespaces;
   static unordered_map<string, string> TemporaryAliases;
};


#endif //CDOT_SYMBOLTABLE_H
