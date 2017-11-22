//
// Created by Jonas Zell on 01.08.17.
//

#include "TypedefDecl.h"
#include "../../Expression/TypeRef.h"

using namespace cdot::cl;

namespace cdot {
namespace ast {

TypedefDecl::TypedefDecl(
   AccessModifier access,
   string &&alias,
   std::shared_ptr<TypeRef> &&origin,
   std::vector<TemplateConstraint> &&generics)
   : Statement(TypedefDeclID), alias(alias), origin(origin),
     templateArgs(generics), access(access)
{

}

AccessModifier TypedefDecl::getAccess() const
{
   return access;
}

void TypedefDecl::setAccess(AccessModifier access)
{
   TypedefDecl::access = access;
}

const string &TypedefDecl::getAlias() const
{
   return alias;
}

void TypedefDecl::setAlias(const string &alias)
{
   TypedefDecl::alias = alias;
}

const std::shared_ptr<TypeRef> &TypedefDecl::getOrigin() const
{
   return origin;
}

void TypedefDecl::setOrigin(const std::shared_ptr<TypeRef> &origin)
{
   TypedefDecl::origin = origin;
}

std::vector<TemplateConstraint> &TypedefDecl::getTemplateArgs()
{
   return templateArgs;
}

void TypedefDecl::setTemplateArgs(const std::vector<TemplateConstraint> &templateArgs)
{
   TypedefDecl::templateArgs = templateArgs;
}

Record *TypedefDecl::getRecord() const
{
   return record;
}

void TypedefDecl::setRecord(Record *record)
{
   TypedefDecl::record = record;
}

} // namespace ast
} // namespace cdot
