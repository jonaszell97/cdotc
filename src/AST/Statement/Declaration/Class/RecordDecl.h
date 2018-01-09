//
// Created by Jonas Zell on 03.11.17.
//

#ifndef CDOT_RECORDDECL_H
#define CDOT_RECORDDECL_H

#include "../../Statement.h"
#include "../../../Passes/SemanticAnalysis/Record/Record.h"

namespace cdot {

struct TemplateParameter;
enum class AccessModifier : unsigned int;

namespace cl {

struct Method;

class Record;
class Enum;
class Protocol;
class Union;

} // namespace cl

namespace ast {

class FieldDecl;
class PropDecl;
class MethodDecl;
class ConstrDecl;
class TypedefDecl;
class DestrDecl;
class TypeRef;
class StaticStmt;
class StaticExpr;
class AssociatedTypeDecl;

class RecordDecl: public Statement {
public:
   typedef std::shared_ptr<RecordDecl> SharedPtr;

   static bool classof(AstNode const* T)
   {
      switch (T->getTypeID()) {
#      define CDOT_RECORD_DECL(Name) \
          case Name##ID:
#      include "../../../AstNode.def"
         return true;
         default:
            return false;
      }
   }

protected:
   RecordDecl(
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
      std::vector<std::shared_ptr<StaticStmt>> &&staticStatements
   );

   AccessModifier am;
   string recordName;
   size_t namespaceLength;

   std::vector<std::shared_ptr<TypeRef>> conformsTo;

   std::vector<std::shared_ptr<FieldDecl>> fields;
   std::vector<std::shared_ptr<MethodDecl>> methods;
   std::vector<std::shared_ptr<TypedefDecl>> typedefs;
   std::vector<std::shared_ptr<PropDecl>> properties;
   std::vector<std::shared_ptr<AssociatedTypeDecl>> associatedTypes;

   std::vector<TemplateParameter> templateParams;
   std::vector<std::shared_ptr<StaticExpr>> Constraints;

   std::vector<std::shared_ptr<RecordDecl>> innerDeclarations;
   std::vector<std::shared_ptr<StaticStmt>> staticStatements;

   Record* record = nullptr;
   RecordDecl *outerRecord = nullptr;

public:
   AccessModifier getAm() const;
   void setAm(AccessModifier am);

   const string &getRecordName() const;
   void setRecordName(const string &recordName);

   size_t getNamespaceLength() const;
   void setNamespaceLength(size_t namespaceLength);

   const std::vector<std::shared_ptr<TypeRef>> &getConformsTo() const;
   void setConformsTo(const std::vector<std::shared_ptr<TypeRef>> &conformsTo);

   const std::vector<std::shared_ptr<MethodDecl>> &getMethods() const
   {
      return methods;
   }

   std::vector<std::shared_ptr<TypedefDecl>> &getTypedefs();
   void setTypedefs(const std::vector<std::shared_ptr<TypedefDecl>> &typedefs);

   const std::vector<std::shared_ptr<PropDecl>> &getProperties() const;
   void setProperties(const std::vector<std::shared_ptr<PropDecl>> &properties);

   std::vector<std::shared_ptr<RecordDecl>> &getInnerDeclarations();
   void setInnerDeclarations(
      const std::vector<std::shared_ptr<RecordDecl>> &innerDeclarations);

   bool isVisited() const;

   Record *getRecord() const;
   void setRecord(Record *record);

   std::vector<TemplateParameter> &getTemplateParams()
   { return templateParams; }

   RecordDecl *getOuterRecord() const
   {
      return outerRecord;
   }

   void setOuterRecord(RecordDecl *outerRecord)
   {
      RecordDecl::outerRecord = outerRecord;
   }

   const std::vector<std::shared_ptr<StaticStmt>> &getStaticStatements() const
   {
      return staticStatements;
   }

   const std::vector<std::shared_ptr<StaticExpr>> &getConstraints() const
   {
      return Constraints;
   }

   const std::vector<std::shared_ptr<AssociatedTypeDecl>> &
   getAssociatedTypes() const
   {
      return associatedTypes;
   }

   const std::vector<std::shared_ptr<FieldDecl>> &getFields() const
   {
      return fields;
   }
};

struct FunctionTemplateInstantiation;

class FieldDecl;
class MethodDecl;
class ConstrDecl;
class TypedefDecl;
class DestrDecl;
class TypeRef;

using cdot::cl::ExtensionConstraint;

class ClassDecl : public RecordDecl {
public:
   // class or struct
   ClassDecl(
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
      std::vector<std::shared_ptr<TypeRef>> &&conformsTo,
      std::shared_ptr<DestrDecl> &&destr,
      std::vector<std::shared_ptr<RecordDecl>> &&innerDeclarations,
      std::vector<std::shared_ptr<StaticStmt>> &&staticStatements
   );

   // protocol
   ClassDecl(
      string &&className,
      std::vector<std::shared_ptr<FieldDecl>> &&fields,
      std::vector<std::shared_ptr<MethodDecl>> &&methods,
      std::vector<std::shared_ptr<ConstrDecl>> &&constructors,
      std::vector<std::shared_ptr<TypedefDecl>>&&typedefs,
      std::vector<std::shared_ptr<PropDecl>> &&properties,
      std::vector<std::shared_ptr<AssociatedTypeDecl>> &&associatedTypes,
      std::vector<TemplateParameter> &&templateParams,
      std::vector<std::shared_ptr<StaticExpr>> &&Constraints,
      AccessModifier am,
      std::vector<std::shared_ptr<TypeRef>> &&conformsTo,
      std::shared_ptr<DestrDecl> &&destr,
      std::vector<std::shared_ptr<RecordDecl>> &&innerDeclarations,
      std::vector<std::shared_ptr<StaticStmt>> &&staticStatements
   );

   std::shared_ptr<TypeRef> getParentClass()
   {
      return parentClass;
   }

   std::shared_ptr<DestrDecl>& getDestructor()
   {
      return destructor;
   }

   typedef std::shared_ptr<ClassDecl> SharedPtr;

   static bool classof(AstNode const* T)
   {
      return T->getTypeID() == ClassDeclID;
   }

protected:
   std::shared_ptr<TypeRef> parentClass = nullptr;

   bool is_abstract = false;
   bool is_struct = false;

   std::vector<std::shared_ptr<ConstrDecl>> constructors;
   std::shared_ptr<DestrDecl> destructor;

public:
   void setParentClass(const std::shared_ptr<TypeRef> &parentClass);

   bool isAbstract() const;
   void setIsAbstract(bool is_abstract);

   bool isStruct() const;
   void setIsStruct(bool is_struct);

   const std::vector<std::shared_ptr<ConstrDecl>> &getConstructors() const;
   void setConstructors(
      const std::vector<std::shared_ptr<ConstrDecl>> &constructors);

   void setDestructor(const std::shared_ptr<DestrDecl> &destructor);

   Class *getDeclaredClass() const;
};

class EnumCaseDecl;

class EnumDecl: public RecordDecl {
public:
   EnumDecl(
      AccessModifier am,
      string&& enumName,
      std::shared_ptr<TypeRef> &&rawType,
      std::vector<std::shared_ptr<FieldDecl>> &&fields,
      std::vector<std::shared_ptr<MethodDecl>>&& methods,
      std::vector<std::shared_ptr<PropDecl>> &&properties,
      std::vector<std::shared_ptr<AssociatedTypeDecl>> &&associatedTypes,
      std::vector<TemplateParameter> &&templateParams,
      std::vector<std::shared_ptr<StaticExpr>> &&Constraints,
      std::vector<std::shared_ptr<TypeRef>>&& conformsTo,
      std::vector<std::shared_ptr<EnumCaseDecl>>&& cases,
      std::vector<std::shared_ptr<RecordDecl>>&& innerDeclarations,
      std::vector<std::shared_ptr<StaticStmt>> &&staticStatements
   );

   typedef std::shared_ptr<EnumDecl> SharedPtr;

   static bool classof(AstNode const* T)
   {
      return T->getTypeID() == EnumDeclID;
   }

protected:
   std::vector<std::shared_ptr<EnumCaseDecl>> cases;
   std::shared_ptr<TypeRef> rawType;

   // codegen
   string selfBinding;

public:
   const std::vector<std::shared_ptr<EnumCaseDecl>> &getCases() const
   {
      return cases;
   }

   void setCases(const std::vector<std::shared_ptr<EnumCaseDecl>> &cases)
   {
      EnumDecl::cases = cases;
   }

   const std::shared_ptr<TypeRef> &getRawType() const
   {
      return rawType;
   }

   void setRawType(const std::shared_ptr<TypeRef> &rawType)
   {
      EnumDecl::rawType = rawType;
   }

   const string &getSelfBinding() const
   {
      return selfBinding;
   }

   void setSelfBinding(const string &selfBinding)
   {
      EnumDecl::selfBinding = selfBinding;
   }

   cdot::cl::Enum *getDeclaredEnum() const;
};

class UnionDecl: public RecordDecl {
public:
   typedef unordered_map<string, std::shared_ptr<TypeRef>> UnionTypes;
   typedef std::shared_ptr<UnionDecl> SharedPtr;
   typedef std::unique_ptr<UnionDecl> UniquePtr;

   UnionDecl(
      string&& name,
      UnionTypes&& types,
      bool isConst,

      std::vector<std::shared_ptr<FieldDecl>> &&fields,
      std::vector<std::shared_ptr<MethodDecl>> &&methods,
      std::vector<std::shared_ptr<TypedefDecl>> &&typedefs,
      std::vector<std::shared_ptr<PropDecl>> &&properties,
      std::vector<std::shared_ptr<AssociatedTypeDecl>> &&associatedTypes,

      std::vector<TemplateParameter> &&templateParams,
      std::vector<std::shared_ptr<StaticExpr>> &&Constraints,

      std::vector<std::shared_ptr<RecordDecl>> &&innerdecls,
      std::vector<std::shared_ptr<StaticStmt>> &&staticStatements
   );

   const UnionTypes &getContainedTypes() const {
      return containedTypes;
   }

   void setContainedTypes(const UnionTypes &containedTypes) {
      UnionDecl::containedTypes = containedTypes;
   }

   bool isConst() const { return is_const; }
   void isConst(bool is_const) { this->is_const = is_const; }

   Union *getDeclaredUnion() const;

   static bool classof(AstNode const* T)
   {
      return T->getTypeID() == UnionDeclID;
   }

protected:
   UnionTypes containedTypes;
   bool is_const = false;

   Union *declaredUnion;
};

class ProtocolDecl: public RecordDecl {
public:
   ProtocolDecl(
      AccessModifier am,
      string &&recordName,

      std::vector<std::shared_ptr<TypeRef>> &&conformsTo,

      std::vector<std::shared_ptr<MethodDecl>> &&methods,
      std::vector<std::shared_ptr<TypedefDecl>> &&typedefs,
      std::vector<std::shared_ptr<PropDecl>> &&properties,
      std::vector<std::shared_ptr<AssociatedTypeDecl>> &&associatedTypes,
      std::vector<std::shared_ptr<ConstrDecl>> &&constructors,

      std::vector<TemplateParameter> &&templateParams,
      std::vector<std::shared_ptr<StaticExpr>> &&Constraints,

      std::vector<std::shared_ptr<RecordDecl>> &&innerDeclarations,
      std::vector<std::shared_ptr<StaticStmt>> &&staticStatements)
      : RecordDecl(ProtocolDeclID, am, move(recordName), move(conformsTo),
                   {}, move(methods), move(typedefs), move(properties),
                   move(associatedTypes), move(templateParams),
                   move(Constraints),
                   move(innerDeclarations), move(staticStatements)),
        constructors(move(constructors))
   {}

   typedef std::shared_ptr<ProtocolDecl> SharedPtr;

   static bool classof(AstNode const* T)
   {
      return T->getTypeID() == ProtocolDeclID;
   }

protected:
   std::vector<std::shared_ptr<ConstrDecl>> constructors;

public:
   const std::vector<std::shared_ptr<ConstrDecl>> &getConstructors() const
   {
      return constructors;
   }
};

class ExtensionDecl: public RecordDecl {
public:
   ExtensionDecl(
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

      std::vector<std::shared_ptr<ConstrDecl>> &&initializers,
      std::vector<std::shared_ptr<RecordDecl>> &&innerDeclarations,
      std::vector<std::shared_ptr<StaticStmt>> &&staticStatements)
         : RecordDecl(ExtensionDeclID, am, move(recordName), move(conformsTo),
                      move(fields), move(methods), move(typedefs),
                      move(properties), move(associatedTypes),
                      move(templateParams), move(Constraints),
                      move(innerDeclarations), move(staticStatements)),
           initializers(move(initializers))
   {}

   typedef std::shared_ptr<ExtensionDecl> SharedPtr;

   static bool classof(AstNode const* T)
   {
      return T->getTypeID() == ExtensionDeclID;
   }

protected:
   std::vector<std::shared_ptr<ConstrDecl>> initializers;

public:
   const std::vector<std::shared_ptr<ConstrDecl>> &getInitializers() const
   { return initializers; }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_RECORDDECL_H
