//
// Created by Jonas Zell on 14.11.17.
//

#include "PropDecl.h"

#include "../../Block/CompoundStmt.h"
#include "../../../Passes/SemanticAnalysis/Record/Record.h"

namespace cdot {
namespace ast {

PropDecl::PropDecl(string &&propName,
                   std::shared_ptr<TypeRef> &&type,
                   AccessModifier access,
                   bool isStatic,
                   bool hasDefinition,
                   bool hasGetter,
                   bool hasSetter,
                   std::shared_ptr<CompoundStmt> &&getter,
                   std::shared_ptr<CompoundStmt> &&setter,
                   string &&newValName)
   : RecordSubDecl(PropDeclID, std::move(propName), access, isStatic),
     type(std::move(type)), has_definition(hasDefinition),
     getterBody(getter), setterBody(setter), newValName(move(newValName)),
     getter(hasGetter), setter(hasSetter)
{

}

const std::shared_ptr<TypeRef> &PropDecl::getType() const
{
   return type;
}

void PropDecl::setType(const std::shared_ptr<TypeRef> &type)
{
   PropDecl::type = type;
}

bool PropDecl::hasGetter() const
{
   return getter;
}

bool PropDecl::hasSetter() const
{
   return setter;
}

const std::shared_ptr<CompoundStmt> &PropDecl::getGetterBody() const
{
   return getterBody;
}

void PropDecl::setGetterBody(const std::shared_ptr<CompoundStmt> &getterBody)
{
   PropDecl::getterBody = getterBody;
}

const std::shared_ptr<CompoundStmt> &PropDecl::getSetterBody() const
{
   return setterBody;
}

void PropDecl::setSetterBody(const std::shared_ptr<CompoundStmt> &setterBody)
{
   PropDecl::setterBody = setterBody;
}

cl::Property *PropDecl::getProp() const
{
   return prop;
}

void PropDecl::setProp(cl::Property *prop)
{
   PropDecl::prop = prop;
}

bool PropDecl::hasDefinition() const
{
   return has_definition;
}

void PropDecl::hasDefinition(bool has_definition)
{
   PropDecl::has_definition = has_definition;
}

string &PropDecl::getNewValName()
{
   return newValName;
}

void PropDecl::setNewValName(const string &newValName)
{
   PropDecl::newValName = newValName;
}


} // namespace ast
} // namespace cdot