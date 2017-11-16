//
// Created by Jonas Zell on 03.11.17.
//

#include "RecordDecl.h"

#include "../../../../Variant/Type/Generic.h"
#include "../../../Passes/SemanticAnalysis/Record/Record.h"

using std::move;

RecordDecl::RecordDecl(
   AccessModifier am, string &&recordName,
   std::vector<std::shared_ptr<TypeRef>> &&conformsTo,
   std::vector<std::shared_ptr<Statement>> &&methods,
   std::vector<std::shared_ptr<TypedefDecl>> &&typedefs,
   std::vector<std::shared_ptr<PropDecl>> &&properties,
   std::vector<Statement::SharedPtr> &&innerDeclarations)
       : am(am), recordName(move(recordName)), conformsTo(move(conformsTo)),
         methods(move(methods)), typedefs(move(typedefs)),
         innerDeclarations(move(innerDeclarations)),
         properties(move(properties))
{

}

const std::vector<std::shared_ptr<Statement>> &RecordDecl::getMethods() const
{
   return methods;
}

void RecordDecl::setMethods(
   const std::vector<std::shared_ptr<Statement>> &methods)
{
   RecordDecl::methods = methods;
}

std::vector<std::shared_ptr<TypedefDecl>> &RecordDecl::getTypedefs()
{
   return typedefs;
}

void
RecordDecl::setTypedefs(
   const std::vector<std::shared_ptr<TypedefDecl>> &typedefs)
{
   RecordDecl::typedefs = typedefs;
}

std::vector<Statement::SharedPtr> &
RecordDecl::getInnerDeclarations()
{
   return innerDeclarations;
}

void RecordDecl::setInnerDeclarations(
   const std::vector<Statement::SharedPtr> &innerDeclarations)
{
   RecordDecl::innerDeclarations = innerDeclarations;
}

AccessModifier RecordDecl::getAm() const
{
   return am;
}

void RecordDecl::setAm(AccessModifier am)
{
   RecordDecl::am = am;
}

const string &RecordDecl::getRecordName() const
{
   return recordName;
}

void RecordDecl::setRecordName(const string &recordName)
{
   RecordDecl::recordName = recordName;
}

size_t RecordDecl::getNamespaceLength() const
{
   return namespaceLength;
}

void RecordDecl::setNamespaceLength(size_t namespaceLength)
{
   RecordDecl::namespaceLength = namespaceLength;
}

const std::vector<std::shared_ptr<TypeRef>> &RecordDecl::getConformsTo() const
{
   return conformsTo;
}

void RecordDecl::setConformsTo(
   const std::vector<std::shared_ptr<TypeRef>> &conformsTo)
{
   RecordDecl::conformsTo = conformsTo;
}

bool RecordDecl::isVisited() const
{
   return record != nullptr;
}

const std::vector<std::shared_ptr<PropDecl>> &RecordDecl::getProperties() const
{
   return properties;
}

void RecordDecl::setProperties(
   const std::vector<std::shared_ptr<PropDecl>> &properties)
{
   RecordDecl::properties = properties;
}

Record *RecordDecl::getRecord() const
{
   return record;
}

void RecordDecl::setRecord(Record *record)
{
   RecordDecl::record = record;
}
