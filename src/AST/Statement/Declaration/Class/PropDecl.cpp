//
// Created by Jonas Zell on 14.11.17.
//

#include "PropDecl.h"

#include "../../Block/CompoundStmt.h"
#include "../../../Passes/SemanticAnalysis/Record/Record.h"

PropDecl::PropDecl(string &&propName,
                   std::shared_ptr<TypeRef> &&type,
                   AccessModifier access,
                   bool isStatic,
                   bool hasDefinition,
                   std::shared_ptr<CompoundStmt> &&getter,
                   std::shared_ptr<CompoundStmt> &&setter,
                   string &&newValName)
   : RecordSubDecl(std::move(propName), access, isStatic),
     type(std::move(type)), has_definition(hasDefinition),
     getterBody(getter), setterBody(setter), newValName(move(newValName))
{

}

std::vector<std::shared_ptr<AstNode>> PropDecl::get_children()
{
   std::vector<std::shared_ptr<AstNode>> children;
   if (hasGetter()) {
      children.push_back(getterBody);
   }
   if (hasSetter()) {
      children.push_back(setterBody);
   }

   return children;
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
   return getterBody != nullptr;
}

bool PropDecl::hasSetter() const
{
   return setterBody != nullptr;
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
