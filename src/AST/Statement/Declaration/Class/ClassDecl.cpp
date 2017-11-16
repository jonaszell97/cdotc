//
// Created by Jonas Zell on 27.06.17.
//

#include "ClassDecl.h"
#include "ConstrDecl.h"
#include "FieldDecl.h"
#include "MethodDecl.h"

#include "../TypedefDecl.h"
#include "../../../Passes/SemanticAnalysis/Record/Class.h"

using namespace cdot::cl;
using std::move;

ClassDecl::ClassDecl(
   string &&class_name,
   std::vector<std::shared_ptr<FieldDecl>> &&fields,
   std::vector<std::shared_ptr<Statement>> &&methods,
   std::vector<std::shared_ptr<ConstrDecl>> &&constr,
   std::vector<std::shared_ptr<TypedefDecl>> &&typedefs,
   std::vector<std::shared_ptr<PropDecl>> &&properties,
   AccessModifier am,
   bool is_abstract,
   std::shared_ptr<TypeRef> &&extends,
   std::vector<std::shared_ptr<TypeRef>> &&implements,
   std::shared_ptr<DestrDecl> &&destr,
   std::vector<Statement::SharedPtr> &&innerDeclarations,
   bool isStruct)
   : RecordDecl(
         am,
         move(class_name),
         move(implements),
         move(methods),
         move(typedefs),
         move(properties),
         move(innerDeclarations)
      ),
      fields(move(fields)),
      constructors(move(constr)),
      destructor(move(destr)),
      is_abstract(is_abstract),
      parentClass(move(extends)),
     is_struct(isStruct)
{

}

ClassDecl::ClassDecl(
      string &&className,
      std::vector<std::shared_ptr<FieldDecl>> &&fields,
      std::vector<std::shared_ptr<Statement>> &&methods,
      std::vector<std::shared_ptr<ConstrDecl>> &&constructors,
      std::vector<std::shared_ptr<TypedefDecl>> &&typedefs,
      std::vector<std::shared_ptr<PropDecl>> &&properties,
      AccessModifier am,
      std::vector<std::shared_ptr<TypeRef>> &&conformsTo,
      std::shared_ptr<DestrDecl> &&destr,
      std::vector<Statement::SharedPtr> &&innerDeclarations)
   : RecordDecl(
      am,
      move(className),
      move(conformsTo),
      move(methods),
      move(typedefs),
      move(properties),
      move(innerDeclarations)
   ),
   fields(move(fields)),
   constructors(move(constructors)),
   destructor(move(destr)),
   is_abstract(true),
   is_protocol(true)
{

}

std::vector<std::shared_ptr<AstNode>> ClassDecl::get_children() {
   std::vector<std::shared_ptr<AstNode>> children;
   for (const auto& td : typedefs) {
      children.push_back(td);
   }
   for (const auto& constr : constructors) {
      children.push_back(constr);
   }
   for (const auto& field : fields) {
      children.push_back(field);
   }
   for (const auto& method : methods) {
      children.push_back(method);
   }
   for (const auto& inner : innerDeclarations) {
      children.push_back(inner);
   }

   return children;
}

void ClassDecl::setParentClass(const std::shared_ptr<TypeRef> &parentClass)
{
   ClassDecl::parentClass = parentClass;
}

bool ClassDecl::isAbstract() const
{
   return is_abstract;
}

void ClassDecl::setIsAbstract(bool is_abstract)
{
   ClassDecl::is_abstract = is_abstract;
}

bool ClassDecl::isProtocol() const
{
   return is_protocol;
}

void ClassDecl::setIsProtocol(bool is_protocol)
{
   ClassDecl::is_protocol = is_protocol;
}

bool ClassDecl::isStruct() const
{
   return is_struct;
}

void ClassDecl::setIsStruct(bool is_struct)
{
   ClassDecl::is_struct = is_struct;
}

const std::vector<std::shared_ptr<ConstrDecl>> &
ClassDecl::getConstructors() const
{
   return constructors;
}

void ClassDecl::setConstructors(
   const std::vector<std::shared_ptr<ConstrDecl>> &constructors)
{
   ClassDecl::constructors = constructors;
}

void ClassDecl::setDestructor(const std::shared_ptr<DestrDecl> &destructor)
{
   ClassDecl::destructor = destructor;
}

const std::vector<std::shared_ptr<FieldDecl>> &ClassDecl::getFields() const
{
   return fields;
}

void ClassDecl::setFields(const std::vector<std::shared_ptr<FieldDecl>> &fields)
{
   ClassDecl::fields = fields;
}

bool ClassDecl::isExplicitMemberwiseInitializer() const
{
   return explicitMemberwiseInitializer;
}

void
ClassDecl::setExplicitMemberwiseInitializer(bool explicitMemberwiseInitializer)
{
   ClassDecl::explicitMemberwiseInitializer = explicitMemberwiseInitializer;
}

const string &ClassDecl::getSelfBinding() const
{
   return selfBinding;
}

void ClassDecl::setSelfBinding(const string &selfBinding)
{
   ClassDecl::selfBinding = selfBinding;
}

Class *ClassDecl::getDeclaredClass() const
{
   return record->getAs<Class>();
}

Method *ClassDecl::getDefaultConstr() const
{
   return defaultConstr;
}

void ClassDecl::setDefaultConstr(Method *defaultConstr)
{
   ClassDecl::defaultConstr = defaultConstr;
}