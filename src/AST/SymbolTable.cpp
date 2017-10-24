//
// Created by Jonas Zell on 07.07.17.
//

#include "SymbolTable.h"
#include "../Util.h"
#include "Passes/SemanticAnalysis/Record/Class.h"
#include "Passes/SemanticAnalysis/Record/Union.h"
#include "Passes/SemanticAnalysis/Record/Enum.h"
#include "Statement/Declaration/Class/ClassDecl.h"
#include "Passes/SemanticAnalysis/Function.h"
#include "../Variant/Type/VoidType.h"

std::unordered_map<string, Typedef> SymbolTable::typedefs = {
   { "Void", Typedef { AccessModifier::PUBLIC, VoidType::get() } }
};

std::unordered_map<string, Variable> SymbolTable::variables = {};
std::unordered_multimap<string, Function::UniquePtr> SymbolTable::functions = {};
std::unordered_map<string, Record*> SymbolTable::Records;
std::vector<string> SymbolTable::namespaces = {};
unordered_map<string, string> SymbolTable::TemporaryAliases;

Typedef* SymbolTable::resolveTypedef(
   const string &typedefName,
   Type& ts,
   std::vector<string> &namespaces)
{
   string name = typedefName;
   if (TemporaryAliases.find(typedefName) != TemporaryAliases.end()) {
      name = TemporaryAliases[typedefName];
   }

   for (const auto& ns : namespaces) {
      auto fullName = ns + name;
      if (hasTypedef(fullName)) {
         return &typedefs[fullName];
      }
   }

   return nullptr;
}

string SymbolTable::mangleVariable(
   const string &id,
   size_t scope)
{
   return "_V" + std::to_string(id.length()) + id + std::to_string(scope);
}

string SymbolTable::mangleFunction(
   const string &id,
   std::vector<Argument> &args)
{
   string symbol = "_F" + std::to_string(id.length()) + id;
   for (auto arg : args) {
      auto str =  arg.type.toString();
      symbol += std::to_string(str.length()) + str;
   }

   return symbol;
}

string SymbolTable::mangleMethod(
   const string &class_name,
   const string &name,
   std::vector<Argument> &args)
{
   string symbol = class_name + "." + name;
   for (const auto& arg : args) {
      auto str =  arg.type.toString();
      symbol += std::to_string(str.length()) + str;
   }

   return symbol;
}

void SymbolTable::declareTemporaryAlias(
   const string &alias, const string &aliasee)
{
   TemporaryAliases.emplace(alias, aliasee);
}

void SymbolTable::clearTemporaryAliases()
{
   TemporaryAliases.clear();
}

void SymbolTable::declareNamespace(const string &name)
{
   if (std::find(namespaces.begin(), namespaces.end(), name) == namespaces.end()) {
      namespaces.push_back(name);
   }
}

bool SymbolTable::isNamespace(const string &name)
{
   if(std::find(namespaces.begin(), namespaces.end(), name) != namespaces.end() ||
      cdot::isBuilitinNamespace(name))
   {
      return true;
   }

   if (TemporaryAliases.find(name) != TemporaryAliases.end()) {
      if (isNamespace(TemporaryAliases[name])) {
         return true;
      }
   }

   return false;
}

void SymbolTable::declareVariable(
   const string &name,
   Type& type,
   AccessModifier access,
   const string& declaredNamespace,
   AstNode *decl)
{
   if (variables.find(name) != variables.end()) {
      variables[name] = Variable{ access, type, declaredNamespace, decl };
   }
   else {
      variables.emplace(name, Variable{ access, type, declaredNamespace, decl });
   }

   if (name.find('.') == string::npos) {
      variables.emplace("Global." + name, Variable{ access, type, declaredNamespace, decl });
   }
}

void SymbolTable::declareFunction(
   const string &name,
   Function::UniquePtr &&fun)
{
   functions.emplace(name, std::move(fun));
}

void SymbolTable::declareUnion(Union *union_)
{
   Records.emplace(union_->getName(), union_);
}

pair<SymbolTable::FunctionIterator, SymbolTable::FunctionIterator>
SymbolTable::getFunction(const string &name)
{
   if (TemporaryAliases.find(name) != TemporaryAliases.end()) {
      return getFunction(TemporaryAliases[name]);
   }

   return functions.equal_range(name);
}

pair<SymbolTable::FunctionIterator, SymbolTable::FunctionIterator>
SymbolTable::getFunction(
   const string &name,
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

bool SymbolTable::hasVariable(const string& name)
{
   if (variables.find(name) != variables.end()) {
      return true;
   }

   if (TemporaryAliases.find(name) != TemporaryAliases.end()) {
      if (hasVariable(TemporaryAliases[name])) {
         return true;
      }
   }

   return false;
}

bool SymbolTable::hasVariable(const string& name, std::vector<string> &namespaces)
{
   for (const auto& ns : namespaces) {
      string fullName = ns + name;
      if (hasVariable(fullName)) {
         return true;
      }
   }

   return hasVariable(name);
}

size_t SymbolTable::numFunctionsWithName(const string &funcName)
{
   auto overloads = functions.equal_range(funcName);
   return std::distance(overloads.first, overloads.second);
}

Variable& SymbolTable::getVariable(const string &name)
{
   if (variables.find(name) != variables.end()) {
      return variables[name];
   }

   if (TemporaryAliases.find(name) != TemporaryAliases.end()) {
      return getVariable(TemporaryAliases[name]);
   }

   llvm_unreachable("Call hasVariable first!");
}

pair<Variable&, string> SymbolTable::getVariable(
   const string &name,
   std::vector<string> &namespaces)
{
   for (const auto& ns : namespaces) {
      string fullName = ns + name;
      if (hasVariable(fullName)) {
         return { getVariable(fullName), fullName };
      }
   }

   llvm_unreachable("Call hasVariable first!");
}

void SymbolTable::setVariable(
   const string &name,
   BuiltinType *ty)
{
   *variables[name].type = ty;
}

void SymbolTable::declareClass(
   Class *cl)
{
   declareNamespace(cl->getName());
   Records.emplace(cl->getName(), cl);
   Records.emplace("Global." + cl->getName(), cl);
}

void SymbolTable::declareEnum(
   Enum *en)
{
   declareNamespace(en->getName());
   Records.emplace(en->getName(), en);
   Records.emplace("Global." + en->getName(), en);
}

void SymbolTable::declareTypedef(
   const string& alias,
   BuiltinType* originTy,
   const std::vector<GenericConstraint>& generics,
   AccessModifier access,
   AstNode *decl)
{
   typedefs.emplace(alias, Typedef{ access, originTy, generics, decl });
   if (originTy->isObject()) {
      namespaces.push_back(alias);
   }

   if (access == AccessModifier::PUBLIC && alias.find('.') == string::npos) {
      declareTypedef("Global." + alias, originTy, generics, access, decl);
   }
}

cdot::cl::Class* SymbolTable::getClass(const string &name)
{
   auto rec = getRecord(name);
   assert(rec->isNonUnion());

   return rec->getAs<Class>();
}

namespace {
   size_t getSpecificity(const string& className) {
      return std::count(className.begin(), className.end(), '.');
   }
}

cdot::cl::Class* SymbolTable::getClass(
   const string &name, std::vector<string> &namespaces)
{
   auto rec = getRecord(name, namespaces);
   assert(rec->isNonUnion());

   return rec->getAs<Class>();
}

cdot::cl::Union* SymbolTable::getUnion(const string &name)
{
   auto rec = getRecord(name);
   assert(rec->isUnion());

   return rec->getAs<Union>();
}

cdot::cl::Union* SymbolTable::getUnion(
   const string &name, std::vector<string> &namespaces)
{
   auto rec = getRecord(name, namespaces);
   assert(rec->isUnion());

   return rec->getAs<Union>();
}

bool SymbolTable::hasRecord(const string &name)
{
   return getRecord(name) != nullptr;
}

bool SymbolTable::hasRecord(const string &name, std::vector<string> &namespaces)
{
   return getRecord(name, namespaces) != nullptr;
}

Record* SymbolTable::getRecord(const string &name)
{
   if (Records.find(name) != Records.end()) {
      return Records[name];
   }

   if (TemporaryAliases.find(name) != TemporaryAliases.end()) {
      return getRecord(TemporaryAliases[name]);
   }

   if (typedefs.find(name) != typedefs.end()) {
      return Records[typedefs[name].aliasedType->getClassName()];
   }

   return nullptr;
}

Record* SymbolTable::getRecord(const string &name, std::vector<string> &namespaces)
{
   pair<Record*, size_t> match = { nullptr, 0 };
   for (const auto& ns : namespaces) {
      auto fullName = ns + name;
      if (hasRecord(fullName)) {
         auto cl = getRecord(fullName);
         auto spec = getSpecificity(cl->getName());

         if (spec >= match.second) {
            match = { cl, spec };
         }
      }
   }

   if (match.first != nullptr) {
      return match.first;
   }

   return nullptr;
}

const std::unordered_map<string, Record*>&
SymbolTable::getRecords()
{
   return Records;
}

Typedef SymbolTable::getTypedef(const string& name) {
   if (typedefs.find(name) != typedefs.end()) {
      auto td = typedefs[name];
      return Typedef{ td.access, td.aliasedType };
   }

   if (TemporaryAliases.find(name) != TemporaryAliases.end()) {
      return getTypedef(TemporaryAliases[name]);
   }

   llvm_unreachable("Call hasTypedef first!");
}

Typedef SymbolTable::getTypedef(
   const string& name, std::vector<string> &namespaces) {
   for (const auto& ns : namespaces) {
      auto fullName = ns + name;
      if (hasTypedef(fullName)) {
         return getTypedef(fullName);
      }
   }

   llvm_unreachable("Call hasTypedef first!");
}

bool SymbolTable::hasClass(const string& name) {
   auto cl = getRecord(name);
   return cl && cl->isNonUnion();
}

bool SymbolTable::hasUnion(const string& name) {
   auto cl = getRecord(name);
   return cl && cl->isUnion();
}

bool SymbolTable::hasUnion(
   const string &name, std::vector<string> &namespaces)
{
   auto rec = getRecord(name, namespaces);
   return rec != nullptr && rec->isUnion();
}

bool SymbolTable::hasClass(
   const string& name, std::vector<string> &namespaces)
{
   auto cl = getRecord(name, namespaces);
   return cl && cl->isNonUnion();
}

bool SymbolTable::hasTypedef(const string& name)
{
   if (typedefs.find(name) != typedefs.end()) {
      return true;
   }

   if (TemporaryAliases.find(name) != TemporaryAliases.end()) {
      if (hasTypedef(TemporaryAliases[name])) {
         return true;
      }
   }

   return false;
}

bool SymbolTable::hasTypedef(
   const string& name, std::vector<string> &namespaces)
{
   for (const auto& ns : namespaces) {
      auto fullName = ns + name;
      if (hasTypedef(fullName)) {
         return true;
      }
   }

   return false;
}