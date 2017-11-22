//
// Created by Jonas Zell on 07.07.17.
//

#include "SymbolTable.h"

#include <sstream>

#include "../Util.h"
#include "../Message/Diagnostics.h"
#include "Passes/Declaration/DeclPass.h"

#include "Passes/SemanticAnalysis/Record/Union.h"
#include "Passes/SemanticAnalysis/Record/Enum.h"
#include "Passes/SemanticAnalysis/Function.h"

#include "Expression/TypeRef.h"
#include "Statement/Declaration/Class/ClassDecl.h"

#include "../Variant/Type/VoidType.h"
#include "../Variant/Type/GenericType.h"

using namespace cdot::diag;

using std::ostringstream;
using ast::Function;

unordered_map<string, Typedef> SymbolTable::typedefs = {
   { "Void", Typedef(VoidType::get(), "void") }
};

unordered_map<string, Variable> SymbolTable::variables;
unordered_multimap<string, Function::UniquePtr> SymbolTable::functions;
unordered_map<string, Record *> SymbolTable::Records;
unordered_map<string, RecordTemplate> SymbolTable::RecordTemplates;
std::vector<string> SymbolTable::namespaces;
unordered_map<string, string> SymbolTable::TemporaryAliases;

Typedef &SymbolTable::resolveTypedef(
   const string &typedefName,
   std::vector<string> &namespaces)
{
   string name = typedefName;
   if (TemporaryAliases.find(typedefName) != TemporaryAliases.end()) {
      name = TemporaryAliases[typedefName];
   }

   for (const auto &ns : namespaces) {
      auto fullName = ns + name;
      if (hasTypedef(fullName)) {
         return typedefs[fullName];
      }
   }

   llvm_unreachable("call hasTypedef first!");
}

string SymbolTable::mangleVariable(const string &id, size_t scope)
{
   ostringstream out;
   out << "_V" << id.length() << id << scope;

   return out.str();
}

string SymbolTable::mangleFunction(const string &id,
                                   std::vector<Argument> &args) {
   ostringstream out;
   out << "_F" << id.length() << id;
   out << util::vectorToString<Argument, '\0', '\0', '\0'>(args, true);

   return out.str();
}

string SymbolTable::mangleFunction(
   const string &name, std::vector<std::shared_ptr<TypeRef>> &args)
{
   ostringstream out;
   out << "_F" << name.length() << name;
   out << util::vectorToString<TypeRef, '\0', '\0', '\0'>(args, true);

   return out.str();
}

string SymbolTable::mangleMethod(
   const string &recordName,
   const string &name,
   std::vector<Argument> &args,
   bool resolveSelf)
{
   ostringstream out;

   out << name;
   for (const auto &arg : args) {
      if (resolveSelf && arg.type->isGenericTy()
          && arg.type->asGenericTy()->getClassName() == "Self") {
         out << recordName.length() << recordName;
      }
      else {
         auto str = arg.type.toString();
         out << str.length() << str;
      }
   }

   return out.str();
}

void SymbolTable::declareTemporaryAlias(
   const string &alias,
   const string &aliasee)
{
   TemporaryAliases.emplace(alias, aliasee);
}

void SymbolTable::clearTemporaryAliases()
{
   TemporaryAliases.clear();
}

void SymbolTable::declareNamespace(const string &name)
{
   if (std::find(namespaces.begin(), namespaces.end(), name)
       == namespaces.end()) {
      namespaces.push_back(name);
   }
}

bool SymbolTable::isNamespace(const string &name)
{
   if (std::find(namespaces.begin(), namespaces.end(), name) != namespaces.end()
       || cdot::isBuilitinNamespace(name)) {
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
   const QualType &type,
   AccessModifier access,
   const string &declaredNamespace,
   AstNode *decl)
{
   if (variables.find(name) != variables.end()) {
      variables[name] = Variable{ access, type, declaredNamespace, decl };
   }
   else {
      variables.emplace(name,
                        Variable{ access, type, declaredNamespace, decl });
   }

   if (name.find('.') == string::npos) {
      variables.emplace("Global." + name,
                        Variable{ access, type, declaredNamespace, decl });
   }
}

namespace {

void throwTemplateArgError(Function &newFunc, Function &func)
{
   auto NewDecl = newFunc.isTemplate()
                  ? (AstNode *) newFunc.getTemplate()->decl
                  : (AstNode *) newFunc.getDeclaration();
   auto PrevDecl = func.isTemplate()
                   ? (AstNode *) func.getTemplate()->decl
                   : (AstNode *) func.getDeclaration();

   diag::err(err_overload_generic_params)
      << 1 /*method*/
      << NewDecl << diag::cont;
   diag::note(note_overload_generic_params)
      << PrevDecl << diag::term;
}

}

void SymbolTable::checkTemplateArgCompatability(Function &newFunc)
{
   // function overloads must have the same template arguments
   auto overloads = functions.equal_range(newFunc.getName());
   if (overloads.first == overloads.second) {
      return;
   }

   auto &func = *overloads.first->second;
   if (newFunc.isTemplate() != func.isTemplate()) {
      throwTemplateArgError(newFunc, func);
   }
   if (!newFunc.isTemplate()) {
      return;
   }

   auto &NewTemplate = *newFunc.getTemplate();
   auto &PrevTemplate = *func.getTemplate();

   if (NewTemplate.constraints.size() != PrevTemplate.constraints.size()) {
      throwTemplateArgError(newFunc, func);
   }

   size_t i = 0;
   for (const auto &Constraint : NewTemplate.constraints) {
      if (Constraint != PrevTemplate.constraints[i]) {
         throwTemplateArgError(newFunc, func);
      }

      ++i;
   }
}

void SymbolTable::checkDuplicateFunctionDeclaration(
   AstNode *decl,
   const string &name,
   const string &mangledName)
{
   auto overloads = functions.equal_range(name);
   for (auto it = overloads.first; it != overloads.second; ++it) {
      if (it->second->getMangledName() == mangledName) {
         diag::err(err_duplicate_method)
            << 1 /*method*/
            << name << decl
            << diag::cont;
         diag::note(note_duplicate_method)
            << it->second->getTemplateOrFunctionDecl()
            << diag::term;
      }
   }
}

void SymbolTable::declareFunction(
   const string &name,
   Function::UniquePtr &&fun)
{
   checkTemplateArgCompatability(*fun);
   checkDuplicateFunctionDeclaration((AstNode *) fun->getDeclaration(), name,
                                     fun->getMangledName());

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
SymbolTable::getFunction(const string &name,
                         const std::vector<string> &namespaces) {
   for (const auto &ns : namespaces) {
      if (functions.find(ns + name) != functions.end()) {
         return functions.equal_range(ns + name);
      }
   }

   auto overloads = functions.equal_range(name);
   if (overloads.first == overloads.second
       && TemporaryAliases.find(name) != TemporaryAliases.end()) {
      return getFunction(name);
   }

   return overloads;
}

bool SymbolTable::hasVariable(const string &name)
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

bool SymbolTable::hasVariable(const string &name,
                              const std::vector<string> &namespaces) {
   for (const auto &ns : namespaces) {
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

Variable &SymbolTable::getVariable(const string &name)
{
   if (variables.find(name) != variables.end()) {
      return variables[name];
   }

   if (TemporaryAliases.find(name) != TemporaryAliases.end()) {
      return getVariable(TemporaryAliases[name]);
   }

   llvm_unreachable("Call hasVariable first!");
}

pair<Variable &, string>
SymbolTable::getVariable(const string &name,
                         const std::vector<string> &namespaces) {
   for (const auto &ns : namespaces) {
      string fullName = ns + name;
      if (hasVariable(fullName)) {
         return { getVariable(fullName), fullName };
      }
   }

   llvm_unreachable("Call hasVariable first!");
}

void SymbolTable::setVariable(const string &name, Type *ty)
{
   *variables[name].type = ty;
}

void SymbolTable::declareRecord(Record *cl)
{
   declareNamespace(cl->getName());
   Records.emplace(cl->getName(), cl);
   Records.emplace("Global." + cl->getName(), cl);
}

void SymbolTable::declareRecord(const string &name, Record *cl)
{
   declareNamespace(name);
   Records.emplace(name, cl);
   Records.emplace("Global." + name, cl);
}

RecordTemplate *SymbolTable::declareRecordTemplate(RecordTemplate &&templ)
{
   auto name = templ.recordName;
   declareNamespace(name);

   return &RecordTemplates.emplace(name, std::move(templ)).first->second;
}

void SymbolTable::declareFunctionTemplate(CallableTemplate *templ)
{
   auto fun = std::make_unique<Function>(templ);
   fun->setMangledName(mangleFunction(fun->getName(),
                                      fun->getTemplate()->args));

   checkTemplateArgCompatability(*fun);
   checkDuplicateFunctionDeclaration(fun->getTemplateOrFunctionDecl(),
                                     fun->getName(),
                                     fun->getMangledName());

   functions.emplace(fun->getName(), std::move(fun));
}

void SymbolTable::declareClass(Class *cl)
{
   declareNamespace(cl->getName());
   Records.emplace(cl->getName(), cl);
}

void SymbolTable::declareClass(const string &name, Class *cl)
{
   declareNamespace(name);
   Records.emplace(name, cl);
}

void SymbolTable::declareEnum(Enum *en)
{
   declareNamespace(en->getName());
   Records.emplace(en->getName(), en);
}

void SymbolTable::declareEnum(const string &name, Enum *en)
{
   declareNamespace(name);
   Records.emplace(name, en);
}

void SymbolTable::declareTypedef(
   const string &alias,
   Type *originTy,
   const std::vector<TemplateConstraint> &constr,
   AccessModifier access,
   AstNode *decl)
{
   typedefs.emplace(alias, Typedef(originTy, alias, constr, access, decl));
   if (originTy->isObjectTy()) {
      namespaces.push_back(alias);
   }

   if (access == AccessModifier::PUBLIC && alias.find('.') == string::npos) {
      declareTypedef("Global." + alias, originTy, constr, access, decl);
   }
}

cdot::cl::Class *SymbolTable::getClass(const string &name)
{
   auto rec = getRecord(name);
   if (!rec->isNonUnion()) {
      return nullptr;
   }

   return rec->getAs<Class>();
}

namespace {

size_t getSpecificity(const string &className)
{
   return std::count(className.begin(), className.end(), '.');
}

}

cdot::cl::Class *SymbolTable::getClass(const string &name,
                                       const std::vector<string> &namespaces) {
   auto rec = getRecord(name, namespaces);
   if (!rec || !rec->isNonUnion()) {
      return nullptr;
   }

   return rec->getAs<Class>();
}

cdot::cl::Union *SymbolTable::getUnion(const string &name)
{
   auto rec = getRecord(name);
   if (!rec || !rec->isUnion()) {
      return nullptr;
   }

   return rec->getAs<Union>();
}

cdot::cl::Union *SymbolTable::getUnion(const string &name,
                                       const std::vector<string> &namespaces) {
   auto rec = getRecord(name, namespaces);
   if (!rec || !rec->isUnion()) {
      return nullptr;
   }

   return rec->getAs<Union>();
}

bool SymbolTable::hasRecord(const string &name)
{
   return getRecord(name) != nullptr;
}

bool SymbolTable::hasRecord(const string &name,
                            const std::vector<string> &namespaces) {
   return getRecord(name, namespaces) != nullptr;
}

bool SymbolTable::hasRecord(const string &name,
                            std::vector<TemplateArg> &args,
                            const std::vector<string> &importedNamespaces) {
   if (hasRecord(name, importedNamespaces)) {
      return true;
   }

   if (RecordTemplates.find(name) != RecordTemplates.end()) {
      return true;
   }

   return false;
}

bool SymbolTable::hasRecordTemplate(const string &name)
{
   return RecordTemplates.find(name) != RecordTemplates.end();
}

bool SymbolTable::hasRecordTemplate(const string &name,
                                    const std::vector<string> &namespaces) {
   if (hasRecordTemplate(name)) {
      return true;
   }

   for (const auto &ns : namespaces) {
      if (hasRecordTemplate(ns + name)) {
         return true;
      }
   }

   return false;
}

bool SymbolTable::hasFunctionTemplate(const string &name)
{
   auto it = functions.equal_range(name);
   return it.first != it.second && it.first->second->isTemplate();
}

bool SymbolTable::hasFunctionTemplate(const string &name,
                                      const std::vector<string> &namespaces) {
   if (hasFunctionTemplate(name)) {
      return true;
   }

   for (const auto &ns : namespaces) {
      if (hasFunctionTemplate(ns + name)) {
         return true;
      }
   }

   return false;
}

Record *SymbolTable::getRecord(const string &name)
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

Record *SymbolTable::getRecord(const string &name,
                               const std::vector<string> &namespaces) {
   pair<Record *, size_t> match = { nullptr, 0 };
   for (const auto &ns : namespaces) {
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

Record *SymbolTable::getRecord(const string &recordName,
                               TemplateArgList *argList,
                               const std::vector<string> &ns,
                               bool *isNew) {
   auto it = RecordTemplates.find(recordName);
   if (it == RecordTemplates.end()) {
      return getRecord(recordName, ns);
   }

   auto &Template = it->second;
   return DeclPass::declareRecordInstantiation(
      Template,
      argList,
      isNew
   );
}

const std::unordered_map<string, Record *> &
SymbolTable::getRecords()
{
   return Records;
}

RecordTemplate *SymbolTable::getRecordTemplate(const string &name)
{
   auto it = RecordTemplates.find(name);
   assert(it != RecordTemplates.end());

   return &it->second;
}

RecordTemplate*
SymbolTable::getRecordTemplate(const string &name,
                               const std::vector<string> &namespaces) {
   auto it = RecordTemplates.find(name);
   if (it != RecordTemplates.end()) {
      return &it->second;
   }

   for (const auto &ns : namespaces) {
      it = RecordTemplates.find(ns + name);
      if (it != RecordTemplates.end()) {
         return &it->second;
      }
   }

   llvm_unreachable("call hasRecordTemplate first");
}

Function* SymbolTable::getAnyFn(const string &withName,
                               const std::vector<string> &namespaces) {
   auto it = functions.equal_range(withName);
   if (it.first != it.second) {
      return it.second->second.get();
   }

   for (const auto &ns : namespaces) {
      it = functions.equal_range(withName);
      if (it.first != it.second) {
         return it.second->second.get();
      }
   }

   return nullptr;
}

CallableTemplate &SymbolTable::getFunctionTemplate(const string &name)
{
   auto fun = getFunction(name);
   assert(fun.first != fun.second);
   assert(fun.first->second->isTemplate());

   return *fun.first->second->getTemplate();
}

CallableTemplate&
SymbolTable::getFunctionTemplate(const string &name,
                                 const std::vector<string> &namespaces) {
   auto fun = getFunction(name, namespaces);
   assert(fun.first != fun.second);
   assert(fun.first->second->isTemplate());

   return *fun.first->second->getTemplate();
}

bool SymbolTable::isTemplatedFunction(const string &funcName)
{
   auto it = functions.equal_range(funcName);
   if (it.first == it.second) {
      return false;
   }

   return it.first->second->isTemplate();
}

std::vector<TemplateConstraint> &SymbolTable::getConstraints(
   const string &forFunction)
{
   auto it = functions.equal_range(forFunction);
   assert(it.first != it.second && "call hasFunctionTemplate first");
   assert(it.first->second->isTemplate());

   return it.first->second->getTemplate()->constraints;
}

Typedef SymbolTable::getTypedef(const string &name)
{
   if (typedefs.find(name) != typedefs.end()) {
      return typedefs[name];
   }

   if (TemporaryAliases.find(name) != TemporaryAliases.end()) {
      return getTypedef(TemporaryAliases[name]);
   }

   llvm_unreachable("Call hasTypedef first!");
}

Typedef SymbolTable::getTypedef(const string &name,
                                const std::vector<string> &namespaces) {
   for (const auto &ns : namespaces) {
      auto fullName = ns + name;
      if (hasTypedef(fullName)) {
         return getTypedef(fullName);
      }
   }

   llvm_unreachable("Call hasTypedef first!");
}

bool SymbolTable::hasClass(const string &name)
{
   auto cl = getRecord(name);
   return cl && cl->isNonUnion();
}

bool SymbolTable::hasUnion(const string &name)
{
   auto cl = getRecord(name);
   return cl && cl->isUnion();
}

bool SymbolTable::hasUnion(const string &name,
                           const std::vector<string> &namespaces) {
   auto rec = getRecord(name, namespaces);
   return rec != nullptr && rec->isUnion();
}

bool SymbolTable::hasClass(const string &name,
                           const std::vector<string> &namespaces) {
   auto cl = getRecord(name, namespaces);
   return cl && cl->isNonUnion();
}

bool SymbolTable::hasTypedef(const string &name)
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

bool SymbolTable::hasTypedef(const string &name,
                             const std::vector<string> &namespaces) {
   for (const auto &ns : namespaces) {
      auto fullName = ns + name;
      if (hasTypedef(fullName)) {
         return true;
      }
   }

   return false;
}

bool SymbolTable::LookupResult::isRecord() const
{
   return kind == LRK_Record || kind == LRK_Class || kind == LRK_Struct
          || kind == LRK_Enum || kind == LRK_Union || kind == LRK_Protocol;
}

bool SymbolTable::LookupResult::isRecordTemplate() const
{
   return kind == LRK_RecordTemplate;
}

bool SymbolTable::LookupResult::isFunction() const
{
   return kind == LRK_Function;
}

bool SymbolTable::LookupResult::isFunctionTemplate() const
{
   return kind == LRK_FunctionTemplate;
}

bool SymbolTable::LookupResult::isGlobalVariable() const
{
   return kind == LRK_GlobalVariable;
}

SymbolTable::LookupResult
SymbolTable::lookup(const string &symbolName,
                    const std::vector<string> &inNamespaces,
                    LookupKind kind) {
   LookupResult result{ LookupResult::LRK_Nothing };

   bool lookupRecords = kind == LK_Full || kind == LK_Records;
   if (lookupRecords) {
      if (Record *rec = getRecord(symbolName, inNamespaces)) {
         if (rec->isEnum()) {
            result.kind = LookupResult::LRK_Enum;
            result.en = rec->getAs<Enum>();
         }
         else if (rec->isStruct()) {
            result.kind = LookupResult::LRK_Struct;
            result.cl = rec->getAs<Class>();
         }
         else if (rec->isProtocol()) {
            result.kind = LookupResult::LRK_Protocol;
            result.cl = rec->getAs<Class>();
         }
         else if (rec->isUnion()) {
            result.kind = LookupResult::LRK_Union;
            result.un = rec->getAs<Union>();
         }
         else {
            assert(rec->isClass());
            result.kind = LookupResult::LRK_Class;
            result.cl = rec->getAs<Class>();
         }

         return result;
      }
      else if (hasRecordTemplate(symbolName, inNamespaces)) {
         result.kind = LookupResult::LRK_RecordTemplate;
         result.recordTemplate = getRecordTemplate(symbolName, inNamespaces);

         return result;
      }
   }

   bool lookupFns = kind == LK_Full || kind == LK_Functions;
   if (lookupFns) {
      if (Function *fun = getAnyFn(symbolName, inNamespaces)) {
         result.kind = LookupResult::LRK_Function;
         result.fun = fun;

         return result;
      }
      else if (hasFunctionTemplate(symbolName, inNamespaces)) {
         result.kind = LookupResult::LRK_FunctionTemplate;
         result.callableTemplate = &getFunctionTemplate(symbolName,
                                                        inNamespaces);
         return result;
      }
   }

   bool lookupVars = kind == LK_Full || kind == LK_Variables;
   if (lookupVars) {
      if (hasVariable(symbolName, inNamespaces)) {
         auto var = getVariable(symbolName, inNamespaces);
         result.kind = LookupResult::LRK_GlobalVariable;

         return result;
      }
   }

   return result;
}