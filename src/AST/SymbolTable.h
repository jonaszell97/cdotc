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
   
   namespace cl {
      class Class;
      class Enum;
      struct EnumCase;
   }
   
}

using namespace cdot;
using cdot::cl::EnumCase;

using std::string;

class SymbolTable {
public:
   static cdot::cl::Class* declareClass(string class_name, ObjectType *extends, std::vector<ObjectType *>
      implements, std::vector<ObjectType *> generics, ClassDecl *decl, bool is_abstract);
   static cdot::cl::Class* declareClass(string,std::vector<ObjectType *>, std::vector<ObjectType *>, bool, ClassDecl *);

   static cdot::cl::Enum* declareEnum(
      string& name,
      std::vector<ObjectType*>& conformsTo,
      std::vector<ObjectType*>& generics
   );

   static void declareTypedef(string, Type *);
   static void declareNamespace(string &);
   static void declareVariable(string &, Type *);
   static void declareFunction(string &, std::unique_ptr<Function> &&);

   static bool isNamespace(string &);

   static bool hasClass(string, string);
   static bool hasClass(string);
   static bool hasClass(string, std::vector<string> &);

   static bool hasTypedef(string, string);
   static bool hasTypedef(string);
   static bool hasTypedef(string, std::vector<string> &);

   static bool hasVariable(string, string);
   static bool hasVariable(string);
   static bool hasVariable(string, std::vector<string> &);

   static cdot::cl::Class* getClass(string, string);
   static cdot::cl::Class* getClass(string);
   static cdot::cl::Class* getClass(string, std::vector<string> &);

   static Type* getTypedef(string, string);
   static Type* getTypedef(string);
   static Type* getTypedef(string, std::vector<string> &);

   static pair<Type*, string> getVariable(string &, string);
   static Type* getVariable(string &);
   static pair<Type*, string> getVariable(string &, std::vector<string> &);

   static pair<unordered_multimap<string, std::unique_ptr<Function>>::iterator,
      unordered_multimap<string, std::unique_ptr<Function>>::iterator> getFunction(string &, string);
   static pair<unordered_multimap<string, std::unique_ptr<Function>>::iterator,
      unordered_multimap<string, std::unique_ptr<Function>>::iterator> getFunction(string &);
   static pair<unordered_multimap<string, std::unique_ptr<Function>>::iterator,
      unordered_multimap<string, std::unique_ptr<Function>>::iterator> getFunction(string &, std::vector<string> &);

   static string mangleVariable(string &, Type *);
   static string mangleVariable(string &, size_t = 0);
   static string mangleFunction(string &, std::vector<Type *> &);

   static string mangleMethod(string &, string &, std::vector<Type *> &);

   static void resolveTypedef(Type *&, std::vector<string> &);

private:
   SymbolTable() = default;

   static std::unordered_map<string, std::unique_ptr<cdot::cl::Class>> classes;
   static std::unordered_map<string, Type*> typedefs;
   static std::unordered_map<string, Type*> variables;
   static std::unordered_multimap<string, std::unique_ptr<Function>> functions;
   static std::vector<string> namespaces;
};


#endif //CDOT_SYMBOLTABLE_H
