//
// Created by Jonas Zell on 10.10.17.
//

#include "Record.h"

#include "Class.h"
#include "../Function.h"
#include "../../../SymbolTable.h"

#include "../../../Statement/Declaration/Template/RecordTemplateDecl.h"
#include "../../../Statement/Declaration/Class/RecordDecl.h"
#include "../../../Expression/TypeRef.h"

#include "../../../../Variant/Type/GenericType.h"
#include "../../../../Message/Diagnostics.h"
#include "../../CodeGen/CodeGen.h"

using namespace cdot::diag;

namespace cdot {
namespace cl {

size_t Record::lastTypeID = 0;

Record::Record(
   const string &name,
   AccessModifier access,
   const SourceLocation &loc,
   RecordDecl *decl) :
     recordName(name),
     declarationNamespace(name.substr(0, name.rfind('.'))),
     typeID(lastTypeID++), loc(loc),
     decl(decl)
{

}

bool Record::isDeclared() const
{
   return decl->isDeclaration();
}

void Record::isTemplate(
   RecordTemplate *Template, TemplateArgList *templateArgs)
{
   this->Template = Template;
   this->templateArgs = templateArgs;
}

AccessModifier Record::getAccess() const
{
   return access;
}

bool Record::isPrivate() const
{
   return access == AccessModifier::PRIVATE;
}

bool Record::isProtected() const
{
   return access == AccessModifier::PROTECTED;
}

llvm::Type* Record::getSelfType() const
{
   return CodeGen::getStructTy(recordName);
}

string Record::getTypeName() const
{
   if (isStruct()) {
      return "struct";
   }
   if (isProtocol()) {
      return "protocol";
   }
   if (isNonUnion()) {
      return "class";
   }
   if (isEnum()) {
      return "enum";
   }
   if (isUnion()) {
      return "union";
   }

   llvm_unreachable(0);
}

int Record::getNameSelector() const
{
   if (isNonUnion()) {
      return 0;
   }
   if (isStruct()) {
      return 1;
   }
   if (isEnum()) {
      return 2;
   }
   if (isUnion()) {
      return 3;
   }
   if (isProtocol()) {
      return 4;
   }

   llvm_unreachable(0);
}

const std::vector<Typedef> &Record::getTypedefs() const
{
   return typedefs;
}

void Record::setTypedefs(const std::vector<Typedef> &typedefs)
{
   Record::typedefs = typedefs;
}

void Record::declareTypedef(
   BuiltinType *ty, const string &alias,
   const std::vector<TemplateConstraint> &templateArgs,
   AccessModifier access, AstNode *decl)
{
   typedefs.emplace_back(ty, alias, templateArgs, access, decl);
}

bool Record::conformsTo(const string &name) const
{
   auto findFunc = [name](Class *cl) {
      return cl->getName() == name;
   };

   return std::find_if(conforms_to.begin(),
                       conforms_to.end(),
                       findFunc) != conforms_to.end();
}

void Record::addConformance(Class *cl)
{
   assert(cl != this && "circular conformance");

   conforms_to.insert(cl);
   for (const auto &byProxy : cl->getConformances()) {
      addConformance(byProxy);
   }
}

std::set<Class*>& Record::getConformances()
{
   return conforms_to;
}

bool Record::isTemplated() const
{
   return Template != nullptr;
}

const TemplateArg& Record::getTemplateArg(const string &name) const
{
   assert(Template && "not a templated class!");

   size_t i = 0;
   for (const auto &constraint : Template->constraints) {
      if (constraint.genericTypeName == name) {
         assert(templateArgs->get().size() > i);
         return templateArgs->get()[i];
      }

      ++i;
   }

   llvm_unreachable("template arg does not exist");
}

const std::vector<TemplateArg>& Record::getTemplateArgs() const
{
   assert(Template && "not a templated class!");
   return templateArgs->get();
}

RecordTemplate* Record::getTemplate()
{
   assert(Template && "not a templated class!");
   return Template;
}

bool Record::hasInnerRecord(const string &inner) const
{
   auto innerCl = SymbolTable::getRecord(inner);
   if(util::in_vector(innerRecords, innerCl)) {
      return true;
   }

   for (const auto& innerDecl : innerRecords) {
      if (innerDecl->hasInnerRecord(inner)) {
         return true;
      }
   }

   return false;
}

RecordDecl *Record::getDecl() const
{
   return decl;
}

void Record::setDecl(RecordDecl *decl)
{
   Record::decl = decl;
}

const unordered_multimap<string, std::shared_ptr<Method>> &
Record::getMethods() const
{
   return methods;
}

void Record::setMethods(
   const unordered_multimap<string, std::shared_ptr<Method>> &methods)
{
   Record::methods = methods;
}


Method* Record::getMethod(const string &method_name)
{
   for (auto& method : methods) {
      if (method.second->getName() == method_name
          || method.second->getMangledName() == method_name) {
         return method.second.get();
      }
   }

   return nullptr;
}

Method* Record::getMethod(unsigned id)
{
   for (auto& method : methods) {
      if (method.second->getMethodID() == id) {
         return method.second.get();
      }
   }

   return nullptr;
}

void Record::declareMethodAlias(const string &name,
                                const string &mangledOriginal) {
   for (const auto& method : methods) {
      if (method.second->getMangledName() == mangledOriginal) {
         methods.emplace(name, method.second);
         break;
      }
   }

   llvm_unreachable("aliased method does not exist");
}

void RecordTemplate::addInstantiation(std::shared_ptr<Statement> &&inst)
{
   if (!outerTemplate) {
      decl->addInstantiation(std::move(inst));
      return;
   }

   auto current = outerTemplate;
   while (current->Template->outerTemplate) {
      current = current->Template->outerTemplate;
   }

   current->Template->decl->addInstantiation(std::move(inst));
}

namespace {

string getOperatorEqualsName(Record *rec)
{
   string res(rec->getName());
   auto len = res.length();

   res += ".infix ==";
   res += std::to_string(len);
   res += rec->getName();

   return res;
}

} // anonymous namespace

Method* Record::declareMethod(const string &methodName,
                              const Type& ret_type,
                              AccessModifier access,
                              std::vector<Argument>&& args,
                              bool isStatic,
                              MethodDecl* decl,
                              SourceLocation loc) {
   auto isInitializer = methodName == "init";

   auto mangledName = SymbolTable::mangleMethod(recordName, methodName, args);
   checkDuplicateMethodDeclaration((AstNode*)decl, methodName, mangledName);

   auto method = std::make_shared<Method>(methodName, ret_type, access,
                                          std::move(args), isStatic,
                                          decl, loc, methods.size());

   checkTemplateArgCompatability(*method);

   method->isInitializer(isInitializer);
   method->owningClass = this;

   method->setMangledName(mangledName);

   if (ret_type->needsStructReturn()) {
      method->hasStructReturn(true);
   }

   auto ptr = method.get();
   methods.emplace(methodName, std::move(method));

   for (const auto &m : methods) {
      int i = 3;
   }

   if (mangledName == getOperatorEqualsName(this)) {
      operatorEquals = ptr;
   }

   return ptr;
}

Property* Record::declareProperty(const string &propName,
                                  const Type &ty,
                                  bool isStatic,
                                  Method *getter,
                                  Method *setter,
                                  string &&newValName,
                                  PropDecl *decl) {
   return &properties.emplace(propName,
                              Property(propName, ty, isStatic, getter,
                                       setter, this, move(newValName),
                                       decl)).first->second;
}

Method *Record::declareMethodTemplate(MethodTemplate *Template)
{
   auto method = std::make_shared<Method>(Template);
   method->owningClass = this;
   method->setMangledName(SymbolTable::mangleMethod(recordName,
                                                    method->getName(),
                                                    method->getArguments()));

   checkDuplicateMethodDeclaration((AstNode*)Template->methodDecl,
                                   Template->funcName,
                                   method->getMangledName());

   checkTemplateArgCompatability(*method);

   auto ptr = method.get();
   methods.emplace(method->getName(), std::move(method));

   return ptr;
}

namespace {

void throwTemplateArgError(Method &newMethod, Method &method)
{
   auto NewDecl = newMethod.isTemplate()
                  ? (AstNode*)newMethod.getMethodTemplate()->methodDecl
                  : (AstNode*)newMethod.getDeclaration();
   auto PrevDecl = method.isTemplate()
                   ? (AstNode*)method.getMethodTemplate()->methodDecl
                   : (AstNode*)method.getDeclaration();

   diag::err(err_overload_generic_params)
      << 1 /*method*/
      << NewDecl << diag::cont;
   diag::note(note_overload_generic_params)
      << PrevDecl << diag::term;
}

}

void Record::checkTemplateArgCompatability(
   Method &newMethod)
{
   // method overloads must have the same template arguments
   auto overloads = methods.equal_range(newMethod.getName());
   if (overloads.first == overloads.second) {
      return;
   }

   auto &method = *overloads.first->second;
   if (newMethod.isTemplate() != method.isTemplate()) {
      throwTemplateArgError(newMethod, method);
   }
   if (!newMethod.isTemplate()) {
      return;
   }

   auto &NewTemplate = *newMethod.getTemplate();
   auto &PrevTemplate = *method.getTemplate();

   if (NewTemplate.constraints.size() != PrevTemplate.constraints.size()) {
      throwTemplateArgError(newMethod, method);
   }

   size_t i = 0;
   for (const auto &Constraint : NewTemplate.constraints) {
      if (Constraint != PrevTemplate.constraints[i]) {
         throwTemplateArgError(newMethod, method);
      }

      ++i;
   }
}

void Record::checkDuplicateMethodDeclaration(
   AstNode *decl, const string &name, const string &mangledName)
{
   auto overloads = methods.equal_range(name);
   for (auto it = overloads.first; it != overloads.second; ++it) {
      if (it->second->getMangledName() == mangledName) {
         diag::err(err_duplicate_method)
            << 1 /*method*/
            << name << decl
            << diag::cont;
         diag::note(note_duplicate_method)
            << it->second->getTemplateOrMethodDecl()
            << diag::term;
      }
   }
}

bool Record::hasMethodWithName(const string &name) const
{
   auto range = methods.equal_range(name);
   return range.first != range.second;
}

bool Record::hasMethodTemplate(const string &name) const
{
   auto range = methods.equal_range(name);
   return range.first != range.second && range.first->second->isTemplate();
}

bool Record::hasProperty(const string &name) const
{
   return properties.find(name) != properties.end();
}

Property* Record::getProperty(const string &name)
{
   auto it = properties.find(name);
   if (it != properties.end()) {
      return &it->second;
   }

   return nullptr;
}

unordered_map<string, Property>& Record::getProperties()
{
   return properties;
}

auto Record::getMethodTemplates(const string &name)
   -> decltype(methods.equal_range(""))
{
   auto range = methods.equal_range(name);
   assert(range.first != range.second && range.first->second->isTemplate());

   return range;
}

std::vector<TemplateConstraint>& Record::getMethodConstraints(
   const string &forMethod)
{
   auto range = methods.equal_range(forMethod);
   assert(range.first != range.second && "call hasMethodWithName first");
   assert(range.first->second->isTemplate() && "call hasMethodTemplate first");

   return range.first->second->getTemplate()->constraints;
}

const ExtensionConstraint* Record::checkConstraints(
   Method *method,
   BuiltinType *caller) const
{
   llvm_unreachable("should only be called on classes");
}

bool Record::checkConstraint(
   const ExtensionConstraint &constr,
   BuiltinType *&caller) const
{
   llvm_unreachable("should only be called on classes");
}

namespace {
bool isBetterMatchThan(
   bool& strict,
   CallCompatability &best,
   CallCompatability &candidate)
{
   if (!candidate.isCompatible()) {
      return false;
   }
   if (strict) {
      return candidate.perfectMatch && !best.isCompatible();
   }
   if (!best.isCompatible()) {
      return true;
   }

   return candidate.castPenalty < best.castPenalty;
}
}

void Record::addImplicitConformance(ImplicitConformanceKind kind,
                                    std::vector<string> &protocolMethods) {
   Type returnType;
   Method* method;

   switch (kind) {
      case ImplicitConformanceKind::StringRepresentable: {
         *returnType = ObjectType::get("String");
         method = declareMethod(
            "infix as String",
            returnType,
            AccessModifier::PUBLIC,
            {},
            false,
            nullptr,
            SourceLocation()
         );

         break;
      }
      case ImplicitConformanceKind::Equatable: {
         *returnType = ObjectType::get("Bool");
         method = declareMethod(
            "infix ==",
            returnType,
            AccessModifier::PUBLIC,
            { Argument("", Type(ObjectType::get(recordName))) },
            false,
            nullptr,
            SourceLocation()
         );

         break;
      }
      case ImplicitConformanceKind::Hashable: {
         *returnType = ObjectType::get("UInt64");
         method = declareMethod(
            "hashCode",
            returnType,
            AccessModifier::PUBLIC,
            {},
            false,
            nullptr,
            SourceLocation()
         );

         break;
      }
   }

   method->is_protocol_method = true;
   implicitConformances.emplace_back(kind, method);
   protocolMethods.push_back(method->getMangledName());
}

Method* Record::getImplicitConformance(ImplicitConformanceKind kind)
{
   for (const auto &Impl : implicitConformances) {
      if (Impl.kind == kind) {
         return Impl.method;
      }
   }

   return nullptr;
}

Method *Record::getOperatorEquals() const
{
   return operatorEquals;
}

void Record::setOperatorEquals(Method *operatorEquals)
{
   Record::operatorEquals = operatorEquals;
}

Property::Property(const string &name,
                   const Type &ty,
                   bool isStatic,
                   Method *getter,
                   Method *setter,
                   Record *record,
                   string &&newValName,
                   PropDecl *decl)
   : name(name), type(ty), getter(getter), setter(setter), record(record),
     is_static(isStatic), decl(decl), newValName(move(newValName))
{

}

bool Property::hasGetter() const
{
   return getter != nullptr;
}

bool Property::hasSetter() const
{
   return setter != nullptr;
}

const string &Property::getName() const
{
   return name;
}

void Property::setName(const string &name)
{
   Property::name = name;
}

const Type &Property::getType() const
{
   return type;
}

void Property::setType(const Type &ty)
{
   Property::type = ty;
}

Method *Property::getGetter() const
{
   return getter;
}

void Property::setGetter(Method *getter)
{
   Property::getter = getter;
}

Method *Property::getSetter() const
{
   return setter;
}

void Property::setSetter(Method *setter)
{
   Property::setter = setter;
}

Record *Property::getRecord() const
{
   return record;
}

void Property::setRecord(Record *record)
{
   Property::record = record;
}

bool Property::isStatic() const
{
   return is_static;
}

void Property::isStatic(bool is_static)
{
   Property::is_static = is_static;
}

PropDecl *Property::getDecl() const
{
   return decl;
}

void Property::setDecl(PropDecl *decl)
{
   Property::decl = decl;
}

const string &Property::getNewValName() const
{
   return newValName;
}

void Property::setNewValName(const string &newValName)
{
   Property::newValName = newValName;
}

const string &Property::getNewValBinding() const
{
   return newValBinding;
}

void Property::setNewValBinding(const string &newValBinding)
{
   Property::newValBinding = newValBinding;
}

}
}