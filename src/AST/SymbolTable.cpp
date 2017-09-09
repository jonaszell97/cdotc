//
// Created by Jonas Zell on 07.07.17.
//

#include "SymbolTable.h"
#include "../Util.h"
#include "Visitor/StaticAnalysis/Class.h"
#include "Visitor/StaticAnalysis/Enum.h"
#include "Statement/Declaration/Class/ClassDecl.h"
#include "Visitor/StaticAnalysis/Function.h"

std::unordered_map<string, Type*> SymbolTable::typedefs = {};
std::unordered_map<string, Type*> SymbolTable::variables = {};
std::unordered_multimap<string, Function::UniquePtr> SymbolTable::functions = {};
std::unordered_map<string, std::unique_ptr<cdot::cl::Class>> SymbolTable::classes = {};
std::vector<string> SymbolTable::namespaces = {};

void SymbolTable::resolveTypedef(Type *&ts, std::vector<string> &namespaces) {
   if (isa<PointerType>(ts)) {
      return resolveTypedef(cast<PointerType>(ts)->getPointeeType(), namespaces);
   }
   if (!isa<ObjectType>(ts)) {
      return;
   }

   auto asObj = cast<ObjectType>(ts);
   auto& className = asObj->getClassName();

   for (const auto& ns : namespaces) {
      if (hasTypedef(ns + className)) {
         auto backup = asObj;
         ts = getTypedef(ns + className);

         Type::CopyProperties(backup, ts);
         break;
      }
   }
}

string SymbolTable::mangleVariable(string &id, size_t scope) {
   return "_V" + std::to_string(id.length()) + id + std::to_string(scope);
}

string SymbolTable::mangleVariable(string &id, Type *type) {
   return "_V" + std::to_string(id.length()) + id + "_" + type->toString();
}

string SymbolTable::mangleFunction(string &id, std::vector<Type *> &args) {
   string symbol = "_F" + std::to_string(id.length()) + id;
   for (auto arg : args) {
      auto str =  arg->toString();
      symbol += std::to_string(str.length()) + str;
   }

   return symbol;
}


string SymbolTable::mangleMethod(string &class_name, string &name, std::vector<Type *> &args) {
   string symbol = class_name + "." + name;
   for (const auto& arg : args) {
      auto str =  arg->toString();
      symbol += std::to_string(str.length()) + str;
   }

   return symbol;
}

void SymbolTable::declareNamespace(string &name) {
   if (std::find(namespaces.begin(), namespaces.end(), name) == namespaces.end()) {
      namespaces.push_back(name);
   }
}

bool SymbolTable::isNamespace(string &name) {
   return std::find(namespaces.begin(), namespaces.end(), name) != namespaces.end() ||
      cdot::isBuilitinNamespace(name);
}

void SymbolTable::declareVariable(string &name, Type *type) {
   if (variables.find(name) != variables.end()) {
      variables[name] = type;
   }
   else {
      variables.emplace(name, type);
   }
}

void SymbolTable::declareFunction(string &name, Function::UniquePtr &&fun) {
   functions.emplace(name, std::move(fun));
}

pair<unordered_multimap<string, Function::UniquePtr>::iterator,
   unordered_multimap<string, Function::UniquePtr>::iterator> SymbolTable::getFunction(string &name, string prefix)
{
   if (functions.find(prefix + name) != functions.end()) {
      return functions.equal_range(prefix + name);
   }

   return functions.equal_range(name);
}

pair<unordered_multimap<string, Function::UniquePtr>::iterator,
   unordered_multimap<string, Function::UniquePtr>::iterator> SymbolTable::getFunction(string &name)
{
   return functions.equal_range(name);
}

pair<unordered_multimap<string, Function::UniquePtr>::iterator,
   unordered_multimap<string, Function::UniquePtr>::iterator> SymbolTable::getFunction(string &name,
   std::vector<string> &
   namespaces)
{
   for (const auto& ns : namespaces) {
      if (functions.find(ns + name) != functions.end()) {
         return functions.equal_range(ns + name);
      }
   }

   return functions.equal_range(name);
}

bool SymbolTable::hasVariable(string name, string prefix) {
   if (variables.find(prefix + name) != variables.end()) {
      return true;
   }

   if (variables.find(name) != variables.end()) {
      return true;
   }

   return false;
}

bool SymbolTable::hasVariable(string name) {
   if (variables.find(name) != variables.end()) {
      return true;
   }

   return false;
}

bool SymbolTable::hasVariable(string name, std::vector<string> &namespaces) {
   for (const auto& ns : namespaces) {
      if (hasVariable(name, ns)) {
         return true;
      }
   }

   return false;
}

pair<Type*, string> SymbolTable::getVariable(string &name, string prefix) {
   if (variables.find(prefix + name) != variables.end()) {
      return { variables[prefix + name], prefix + name };
   }

   if (variables.find(name) != variables.end()) {
      return { variables[name], name };
   }

   assert(false && "Check first!");
}

Type* SymbolTable::getVariable(string &name) {
   if (variables.find(name) != variables.end()) {
      return variables[name];
   }

   assert(false && "Check first!");
}

pair<Type*, string> SymbolTable::getVariable(string &name, std::vector<string> &namespaces) {
   for (const auto& ns : namespaces) {
      if (hasVariable(name, ns)) {
         return getVariable(name, ns);
      }
   }

   assert(false && "Check first!");
}

cdot::cl::Class* SymbolTable::declareClass(
   string class_name,
   ObjectType *extends,
   std::vector<ObjectType *> conformsTo,
   std::vector<ObjectType *> generics,
   ClassDecl *decl,
   bool is_abstract
) {
   auto cl = std::make_unique<cdot::cl::Class>(class_name, extends, conformsTo, generics, decl, is_abstract);
   auto ptr = cl.get();

   declareNamespace(class_name);
   classes.emplace(class_name, std::move(cl));

   return ptr;
}

cdot::cl::Class* SymbolTable::declareClass(
   string structName,
   std::vector<ObjectType *> conformsTo,
   std::vector<ObjectType *> generics,
   bool isProtocol,
   ClassDecl *decl)
{
   auto cl = std::make_unique<cdot::cl::Class>(structName, conformsTo, generics, isProtocol, decl);
   auto ptr = cl.get();

   declareNamespace(structName);
   classes.emplace(structName, std::move(cl));

   return ptr;
}

cdot::cl::Enum* SymbolTable::declareEnum(
   string &name,
   std::vector<ObjectType *> &conformsTo,
   std::vector<ObjectType *> &generics)
{
   auto en = std::make_unique<cdot::cl::Enum>(name, conformsTo, generics);
   auto ptr = en.get();

   declareNamespace(name);
   classes.emplace(name, std::move(en));

   return ptr;
}

void SymbolTable::declareTypedef(string alias, Type *origin) {
   typedefs.emplace(alias, origin);
}

cdot::cl::Class* SymbolTable::getClass(string name, string prefix) {
   if (classes.find(prefix + name) != classes.end()) {
      return classes[prefix + name].get();
   }

   if (classes.find(name) != classes.end()) {
      return classes[name].get();
   }

   llvm_unreachable("Call hasClass first!");
}

cdot::cl::Class* SymbolTable::getClass(string name) {
   if (classes.find(name) != classes.end()) {
      return classes[name].get();
   }

   llvm_unreachable("Call hasClass first!");
}

cdot::cl::Class* SymbolTable::getClass(string name, std::vector<string> &namespaces) {
   for (const auto& ns : namespaces) {
      if (hasClass(name, ns)) {
         return getClass(name, ns);
      }
   }

   llvm_unreachable("Call hasClass first!");
}

Type* SymbolTable::getTypedef(string name, string prefix) {
   if (typedefs.find(prefix + name) != typedefs.end()) {
      return typedefs[prefix + name];
   }

   if (typedefs.find(name) != typedefs.end()) {
      return typedefs[name];
   }

   assert(false && "Dont call without checking");
}

Type* SymbolTable::getTypedef(string name) {
   if (typedefs.find(name) != typedefs.end()) {
      return typedefs[name]->deepCopy();
   }

   assert(false && "Dont call without checking");
}

Type* SymbolTable::getTypedef(string name, std::vector<string> &namespaces) {
   for (const auto& ns : namespaces) {
      if (hasTypedef(name, ns)) {
         return getTypedef(name, ns);
      }
   }

   assert(false && "Check first!");
}

bool SymbolTable::hasClass(string name, string prefix) {
   if (classes.find(prefix + name) != classes.end()) {
      return true;
   }

   if (classes.find(name) != classes.end()) {
      return true;
   }

   return false;
}

bool SymbolTable::hasClass(string name) {
   if (classes.find(name) != classes.end()) {
      return true;
   }

   return false;
}

bool SymbolTable::hasClass(string name, std::vector<string> &namespaces) {
   for (const auto& ns : namespaces) {
      if (hasClass(name, ns)) {
         return true;
      }
   }

   return false;
}

bool SymbolTable::hasTypedef(string name, string prefix) {
   if (typedefs.find(prefix + name) != typedefs.end()) {
      return true;
   }

   if (typedefs.find(name) != typedefs.end()) {
      return true;
   }

   return false;
}

bool SymbolTable::hasTypedef(string name) {
   if (typedefs.find(name) != typedefs.end()) {
      return true;
   }

   return false;
}

bool SymbolTable::hasTypedef(string name, std::vector<string> &namespaces) {
   for (const auto& ns : namespaces) {
      if (hasTypedef(name, ns)) {
         return true;
      }
   }

   return false;
}