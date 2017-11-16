//
// Created by Jonas Zell on 14.11.17.
//

#include "RecordSubDecl.h"

RecordSubDecl::RecordSubDecl(std::string &&name,
                             AccessModifier access,
                             bool isStatic)
   : name(std::move(name)), access(access), is_static(isStatic)
{

}

const string &RecordSubDecl::getName() const
{
   return name;
}

void RecordSubDecl::setName(const string &name)
{
   RecordSubDecl::name = name;
}

AccessModifier RecordSubDecl::getAccess() const
{
   return access;
}

void RecordSubDecl::setAccess(AccessModifier access)
{
   RecordSubDecl::access = access;
}

bool RecordSubDecl::isStatic() const
{
   return is_static;
}

void RecordSubDecl::isStatic(bool is_static)
{
   RecordSubDecl::is_static = is_static;
}

cl::Record *RecordSubDecl::getRecord() const
{
   return record;
}

void RecordSubDecl::setRecord(cl::Record *record)
{
   RecordSubDecl::record = record;
}
