//
// Created by Jonas Zell on 07.07.17.
//

#include "SymbolTable.h"
#include "../Util.h"
#include "Passes/StaticAnalysis/Class.h"
#include "Passes/StaticAnalysis/Enum.h"
#include "Statement/Declaration/Class/ClassDecl.h"
#include "Passes/StaticAnalysis/Function.h"
#include "../Variant/Type/VoidType.h"

std::unordered_map<string, Typedef> SymbolTable::typedefs = {
   { "Void", Typedef { AccessModifier::PUBLIC, new VoidType } }
};

std::unordered_map<string, Variable> SymbolTable::variables = {};
std::unordered_multimap<string, Function::UniquePtr> SymbolTable::functions = {};
std::unordered_map<string, std::unique_ptr<cdot::cl::Class>> SymbolTable::classes = {};
std::vector<string> SymbolTable::namespaces = {};
unordered_map<string, string> SymbolTable::TemporaryAliases;

void SymbolTable::resolveTypedef(Type *&ts, std::vector<string> &namespaces)
{
   if (isa<PointerType>(ts)) {
      return resolveTypedef(cast<PointerType>(ts)->getPointeeType(), namespaces);
   }
   if (!isa<ObjectType>(ts)) {
      return;
   }

   auto asObj = cast<ObjectType>(ts);
   auto& className = asObj->getClassName();

   if (TemporaryAliases.find(className) != TemporaryAliases.end()) {
      className = TemporaryAliases[className];
   }

   for (const auto& ns : namespaces) {
      auto fullName = ns + className;
      if (hasTypedef(fullName)) {
         auto backup = asObj;
         ts = getTypedef(fullName).aliasedType;

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

string SymbolTable::mangleFunction(string &id, std::vector<Argument> &args) {
   string symbol = "_F" + std::to_string(id.length()) + id;
   for (auto arg : args) {
      auto str =  arg.type->toString();
      symbol += std::to_string(str.length()) + str;
   }

   return symbol;
}


string SymbolTable::mangleMethod(string &class_name, string &name, std::vector<Argument> &args) {
   string symbol = class_name + "." + name;
   for (const auto& arg : args) {
      auto str =  arg.type->toString();
      symbol += std::to_string(str.length()) + str;
   }

   return symbol;
}

void SymbolTable::declareTemporaryAlias(string &alias, string &aliasee) {
   TemporaryAliases.emplace(alias, aliasee);
}

void SymbolTable::clearTemporaryAliases() {
   TemporaryAliases.clear();
}

void SymbolTable::declareNamespace(string &name) {
   if (std::find(namespaces.begin(), namespaces.end(), name) == namespaces.end()) {
      namespaces.push_back(name);
   }
}

bool SymbolTable::isNamespace(string &name) {
   if( std::find(namespaces.begin(), namespaces.end(), name) != namespaces.end() ||
      cdot::isBuilitinNamespace(name))
   {
      return true;
   }

   if (TemporaryAliases.find(name) != TemporaryAliases.end()) {
      if (isNamespace(TemporaryAliases[name])) {
         name = TemporaryAliases[name];
         return true;
      }
   }

   return false;
}

void SymbolTable::declareVariable(string &name, Type *type, AccessModifier access, string& declaredNamespace) {
   if (variables.find(name) != variables.end()) {
      variables[name] = Variable{ access, type, declaredNamespace };
   }
   else {
      variables.emplace(name, Variable{ access, type, declaredNamespace });
   }
}

void SymbolTable::declareFunction(string &name, Function::UniquePtr &&fun) {
   functions.emplace(name, std::move(fun));
}

pair<SymbolTable::FunctionIterator, SymbolTable::FunctionIterator> SymbolTable::getFunction(string &name)
{
   if (TemporaryAliases.find(name) != TemporaryAliases.end()) {
      name = TemporaryAliases[name];
      return getFunction(name);
   }

   return functions.equal_range(name);
}

pair<SymbolTable::FunctionIterator, SymbolTable::FunctionIterator> SymbolTable::getFunction(
   string &name,
   std::vector<string> &namespaces)
{
   for (const auto& ns : namespaces) {
      if (functions.find(ns + name) != functions.end()) {
         return functions.equal_range(ns + name);
      }
   }

   auto overloads = functions.equal_range(name);
   if (overloads.first == overloads.second && TemporaryAliases.find(name) != TemporaryAliases.end()) {
      return getFunction(name);
   }

   return overloads;
}

bool SymbolTable::hasVariable(string& name) {
   if (variables.find(name) != variables.end()) {
      return true;
   }

   if (TemporaryAliases.find(name) != TemporaryAliases.end()) {
      if (hasVariable(TemporaryAliases[name])) {
         name = TemporaryAliases[name];
         return true;
      }
   }

   return false;
}

bool SymbolTable::hasVariable(string& name, std::vector<string> &namespaces) {
   for (const auto& ns : namespaces) {
      string fullName = ns + name;
      if (hasVariable(fullName)) {
         return true;
      }
   }

   return hasVariable(name);
}

size_t SymbolTable::numFunctionsWithName(string &funcName)
{
   auto overloads = functions.equal_range(funcName);
   return std::distance(overloads.first, overloads.second);
}

Variable& SymbolTable::getVariable(string &name) {
   if (variables.find(name) != variables.end()) {
      return variables[name];
   }

   if (TemporaryAliases.find(name) != TemporaryAliases.end()) {
      name = TemporaryAliases[name];
      return getVariable(name);
   }

   llvm_unreachable("Call hasVariable first!");
}

pair<Variable&, string> SymbolTable::getVariable(string &name, std::vector<string> &namespaces) {
   for (const auto& ns : namespaces) {
      string fullName = ns + name;
      if (hasVariable(fullName)) {
         return { getVariable(fullName), fullName };
      }
   }

   llvm_unreachable("Call hasVariable first!");
}

void SymbolTable::setVariable(
   string &name,
   Type *ty)
{
   variables[name].type = ty;
}

cdot::cl::Class* SymbolTable::declareClass(
   AccessModifier am,
   string class_name,
   ObjectType *extends,
   std::vector<ObjectType *> conformsTo,
   std::vector<ObjectType *> generics,
   ClassDecl *decl,
   bool is_abstract)
{
   auto cl = std::make_unique<cdot::cl::Class>(am, class_name, extends, conformsTo, generics, decl, is_abstract);
   auto ptr = cl.get();

   declareNamespace(class_name);
   classes.emplace(class_name, std::move(cl));

   return ptr;
}

cdot::cl::Class* SymbolTable::declareClass(
   AccessModifier am,
   string structName,
   std::vector<ObjectType *> conformsTo,
   std::vector<ObjectType *> generics,
   bool isProtocol,
   ClassDecl *decl)
{
   auto cl = std::make_unique<cdot::cl::Class>(am, structName, conformsTo, generics, isProtocol, decl);
   auto ptr = cl.get();

   declareNamespace(structName);
   classes.emplace(structName, std::move(cl));

   return ptr;
}

cdot::cl::Enum* SymbolTable::declareEnum(
   AccessModifier am,
   string &name,
   std::vector<ObjectType *> &conformsTo,
   std::vector<ObjectType *> &generics)
{
   auto en = std::make_unique<cdot::cl::Enum>(am, name, conformsTo, generics);
   auto ptr = en.get();

   declareNamespace(name);
   classes.emplace(name, std::move(en));

   return ptr;
}

void SymbolTable::declareTypedef(string alias, Type *origin, AccessModifier access) {
   typedefs.emplace(alias, Typedef{ access, origin });

   if (origin->isObject()) {
      namespaces.push_back(alias);
   }
}

cdot::cl::Class* SymbolTable::getClass(string name) {
   if (classes.find(name) != classes.end()) {
      return classes[name].get();
   }

   if (TemporaryAliases.find(name) != TemporaryAliases.end()) {
      name = TemporaryAliases[name];
      return getClass(name);
   }

   if (typedefs.find(name) != typedefs.end()) {
      return classes[typedefs[name].aliasedType->getClassName()].get();
   }

   llvm_unreachable("Call hasClass first!");
}

namespace {
   size_t getSpecificity(const string& className) {
      return std::count(className.begin(), className.end(), '.');
   }
}

cdot::cl::Class* SymbolTable::getClass(string name, std::vector<string> &namespaces)
{
   pair<Class*, size_t> match = { nullptr, 0 };
   for (const auto& ns : namespaces) {
      auto fullName = ns + name;
      if (hasClass(fullName)) {
         auto cl = getClass(fullName);
         auto spec = getSpecificity(cl->getName());

         if (spec >= match.second) {
            match = { cl, spec };
         }
      }
   }

   if (match.first != nullptr) {
      return match.first;
   }

   llvm_unreachable("Call hasClass first!");
}

Typedef SymbolTable::getTypedef(string& name) {
   if (typedefs.find(name) != typedefs.end()) {
      auto td = typedefs[name];
      return Typedef{ td.access, td.aliasedType->deepCopy() };
   }

   if (TemporaryAliases.find(name) != TemporaryAliases.end()) {
      name = TemporaryAliases[name];
      return getTypedef(name);
   }

   llvm_unreachable("Call hasTypedef first!");
}

Typedef SymbolTable::getTypedef(string& name, std::vector<string> &namespaces) {
   for (const auto& ns : namespaces) {
      auto fullName = ns + name;
      if (hasTypedef(fullName)) {
         return getTypedef(fullName);
      }
   }

   llvm_unreachable("Call hasTypedef first!");
}

bool SymbolTable::hasClass(string& name) {
   if (classes.find(name) != classes.end()) {
      return true;
   }

   if (TemporaryAliases.find(name) != TemporaryAliases.end()) {
      if (hasClass(TemporaryAliases[name])) {
         name = TemporaryAliases[name];
         return true;
      }
   }

   if (hasTypedef(name)) {
      auto td = getTypedef(name);
      return td.aliasedType->isObject();
   }

   return false;
}

bool SymbolTable::hasClass(string& name, std::vector<string> &namespaces) {
   for (const auto &ns : namespaces) {
      auto fullName = ns + name;
      if (hasClass(fullName)) {
         return true;
      }
   }

   return false;
}

bool SymbolTable::hasTypedef(string& name) {
   if (typedefs.find(name) != typedefs.end()) {
      return true;
   }

   if (TemporaryAliases.find(name) != TemporaryAliases.end()) {
      if (hasTypedef(TemporaryAliases[name])) {
         name = TemporaryAliases[name];
         return true;
      }
   }

   return false;
}

bool SymbolTable::hasTypedef(string& name, std::vector<string> &namespaces) {
   for (const auto& ns : namespaces) {
      auto fullName = ns + name;
      if (hasTypedef(fullName)) {
         return true;
      }
   }

   return false;
}