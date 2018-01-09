//
// Created by Jonas Zell on 03.11.17.
//

#include "RecordDecl.h"

#include "../../../../Variant/Type/Generic.h"

#include "../../../Passes/SemanticAnalysis/Record/Class.h"
#include "../../../Passes/SemanticAnalysis/Record/Protocol.h"
#include "../../../Passes/SemanticAnalysis/Record/Enum.h"
#include "../../../Passes/SemanticAnalysis/Record/Union.h"

using std::move;

namespace cdot {
namespace ast {

RecordDecl::RecordDecl(
   NodeType typeID,
   AccessModifier am,
   string &&recordName,
   std::vector<std::shared_ptr<TypeRef>> &&conformsTo,
   std::vector<std::shared_ptr<FieldDecl>> &&fields,
   std::vector<std::shared_ptr<MethodDecl>> &&methods,
   std::vector<std::shared_ptr<TypedefDecl>> &&typedefs,
   std::vector<std::shared_ptr<PropDecl>> &&properties,
   std::vector<std::shared_ptr<AssociatedTypeDecl>> &&associatedTypes,
   std::vector<TemplateParameter> &&templateParams,
   std::vector<std::shared_ptr<StaticExpr>> &&Constraints,
   std::vector<std::shared_ptr<RecordDecl>> &&innerDeclarations,
   std::vector<std::shared_ptr<StaticStmt>> &&staticStatements)
   : Statement(typeID), am(am), recordName(move(recordName)),
     conformsTo(move(conformsTo)), fields(move(fields)),
     methods(move(methods)), typedefs(move(typedefs)),
     properties(move(properties)), associatedTypes(move(associatedTypes)),
     templateParams(move(templateParams)),
     Constraints(move(Constraints)),
     innerDeclarations(move(innerDeclarations)),
     staticStatements(move(staticStatements))
{

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

std::vector<std::shared_ptr<RecordDecl>> &
RecordDecl::getInnerDeclarations()
{
   return innerDeclarations;
}

void RecordDecl::setInnerDeclarations(
   const std::vector<std::shared_ptr<RecordDecl>> &innerDeclarations)
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

ClassDecl::ClassDecl(
   string &&class_name,
   std::vector<std::shared_ptr<FieldDecl>> &&fields,
   std::vector<std::shared_ptr<MethodDecl>> &&methods,
   std::vector<std::shared_ptr<ConstrDecl>> &&constr,
   std::vector<std::shared_ptr<TypedefDecl>> &&typedefs,
   std::vector<std::shared_ptr<PropDecl>> &&properties,
   std::vector<std::shared_ptr<AssociatedTypeDecl>> &&associatedTypes,
   std::vector<TemplateParameter> &&templateParams,
   std::vector<std::shared_ptr<StaticExpr>> &&Constraints,
   AccessModifier am,
   bool is_abstract,
   bool isStruct,
   std::shared_ptr<TypeRef> &&extends,
   std::vector<std::shared_ptr<TypeRef>> &&implements,
   std::shared_ptr<DestrDecl> &&destr,
   std::vector<std::shared_ptr<RecordDecl>> &&innerDeclarations,
   std::vector<std::shared_ptr<StaticStmt>> &&staticStatements)
   : RecordDecl(
         ClassDeclID,
         am,
         move(class_name),
         move(implements),
         move(fields),
         move(methods),
         move(typedefs),
         move(properties),
         move(associatedTypes),
         move(templateParams),
         move(Constraints),
         move(innerDeclarations),
         move(staticStatements)
      ),
     parentClass(move(extends)),
     is_abstract(is_abstract),
     is_struct(isStruct),
     constructors(move(constr)),
     destructor(move(destr))
{

}

ClassDecl::ClassDecl(
   string &&className,
   std::vector<std::shared_ptr<FieldDecl>> &&fields,
   std::vector<std::shared_ptr<MethodDecl>> &&methods,
   std::vector<std::shared_ptr<ConstrDecl>> &&constructors,
   std::vector<std::shared_ptr<TypedefDecl>> &&typedefs,
   std::vector<std::shared_ptr<PropDecl>> &&properties,
   std::vector<std::shared_ptr<AssociatedTypeDecl>> &&associatedTypes,
   std::vector<TemplateParameter> &&templateParams,
   std::vector<std::shared_ptr<StaticExpr>> &&Constraints,
   AccessModifier am,
   std::vector<std::shared_ptr<TypeRef>> &&conformsTo,
   std::shared_ptr<DestrDecl> &&destr,
   std::vector<std::shared_ptr<RecordDecl>> &&innerDeclarations,
   std::vector<std::shared_ptr<StaticStmt>> &&staticStatements)
   : RecordDecl(
         ClassDeclID,
         am,
         move(className),
         move(conformsTo),
         move(fields),
         move(methods),
         move(typedefs),
         move(properties),
         move(associatedTypes),
         move(templateParams),
         move(Constraints),
         move(innerDeclarations),
         move(staticStatements)
      ),
     is_abstract(true),
     constructors(move(constructors)),
     destructor(move(destr))
{

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

Class *ClassDecl::getDeclaredClass() const
{
   return record->getAs<Class>();
}

EnumDecl::EnumDecl(AccessModifier am,
                   string &&enumName,
                   std::shared_ptr<TypeRef> &&rawType,
                   std::vector<std::shared_ptr<FieldDecl>> &&fields,
                   std::vector<std::shared_ptr<MethodDecl>> &&methods,
                   std::vector<std::shared_ptr<PropDecl>> &&properties,
                   std::vector<std::shared_ptr<AssociatedTypeDecl>>
                                                            &&associatedTypes,
                   std::vector<TemplateParameter> &&templateParams,
                   std::vector<std::shared_ptr<StaticExpr>> &&Constraints,
                   std::vector<std::shared_ptr<TypeRef>> &&conformsTo,
                   std::vector<std::shared_ptr<EnumCaseDecl>> &&cases,
                   std::vector<std::shared_ptr<RecordDecl>> &&innerDeclarations,
                   std::vector<std::shared_ptr<StaticStmt>> &&staticStatements)
   : RecordDecl(
         EnumDeclID,
         am,
         std::move(enumName),
         std::move(conformsTo),
         move(fields),
         std::move(methods),
         {},
         move(properties),
         move(associatedTypes),
         move(templateParams),
         move(Constraints),
         std::move(innerDeclarations),
         move(staticStatements)
      ),
     cases(move(cases)),
     rawType(move(rawType))
{

}

cl::Enum* EnumDecl::getDeclaredEnum() const
{
   return record->getAs<Enum>();
}

UnionDecl::UnionDecl(string &&name,
                     UnionTypes &&types,
                     bool isConst,
                     std::vector<std::shared_ptr<FieldDecl>> &&fields,
                     std::vector<std::shared_ptr<MethodDecl>> &&methods,
                     std::vector<std::shared_ptr<TypedefDecl>> &&typedefs,
                     std::vector<std::shared_ptr<PropDecl>> &&properties,
                     std::vector<std::shared_ptr<AssociatedTypeDecl>>
                                                            &&associatedTypes,

                     std::vector<TemplateParameter> &&templateParams,
                     std::vector<std::shared_ptr<StaticExpr>> &&Constraints,
                     std::vector<std::shared_ptr<RecordDecl>> &&innerDecls,
                     std::vector<std::shared_ptr<StaticStmt>> &&staticStatements)
   : RecordDecl(UnionDeclID, AccessModifier::PUBLIC, move(name), {},
                move(fields),
                move(methods), move(typedefs), move(properties),
                move(associatedTypes),
                move(templateParams), move(Constraints),
                move(innerDecls), move(staticStatements)),
     containedTypes(move(types)),
     is_const(isConst)
{

}

Union* UnionDecl::getDeclaredUnion() const
{
   return support::cast<Union>(record);
}

} // namespace ast
} // namespace cdot