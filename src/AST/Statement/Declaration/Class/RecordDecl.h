//
// Created by Jonas Zell on 03.11.17.
//

#ifndef CDOT_RECORDDECL_H
#define CDOT_RECORDDECL_H

#include "../../Statement.h"
#include "../../../Passes/SemanticAnalysis/Record/Record.h"

namespace cdot {

namespace cl {

struct ExtensionConstraint {
   enum ConstraintKind {
      CONFORMANCE,
      TYPE_EQUALITY,
      TYPE_INEQUALITY,
      DEFAULT_CONSTRUCTIBLE,
      IS_STRUCT,
      IS_CLASS,
      IS_PROTOCOL,
      IS_ENUM
   };

   string constrainedGenericTypeName;
   std::shared_ptr<TypeRef> typeConstraint = nullptr;
   ConstraintKind kind;

   string reportFailure() const;
};

} // namespace cl

namespace ast {

class FieldDecl;
class PropDecl;
class MethodDecl;
class ConstrDecl;
class TypedefDecl;
class DestrDecl;
class TypeRef;

class RecordDecl: public Statement {
public:
   typedef std::shared_ptr<RecordDecl> SharedPtr;

   static bool classof(AstNode const* T)
   {
      switch (T->getTypeID()) {
#      define CDOT_ASTNODE(Name) \
          case Name##ID:
#      define CDOT_INCLUDE_RECORD_DECL
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

      std::vector<std::shared_ptr<Statement>> &&methods,
      std::vector<std::shared_ptr<TypedefDecl>> &&typedefs,
      std::vector<std::shared_ptr<PropDecl>> &&properties,

      std::vector<Statement::SharedPtr> &&innerDeclarations
   );

   AccessModifier am;
   string recordName;
   size_t namespaceLength;

   std::vector<std::shared_ptr<TypeRef>> conformsTo;

   std::vector<std::shared_ptr<Statement>> methods;
   std::vector<std::shared_ptr<TypedefDecl>> typedefs;
   std::vector<std::shared_ptr<PropDecl>> properties;

   std::vector<Statement::SharedPtr> innerDeclarations;

   Record* record = nullptr;

public:
   AccessModifier getAm() const;
   void setAm(AccessModifier am);

   const string &getRecordName() const;
   void setRecordName(const string &recordName);

   size_t getNamespaceLength() const;
   void setNamespaceLength(size_t namespaceLength);

   const std::vector<std::shared_ptr<TypeRef>> &getConformsTo() const;
   void setConformsTo(const std::vector<std::shared_ptr<TypeRef>> &conformsTo);

   const std::vector<std::shared_ptr<Statement>> &getMethods() const;
   void setMethods(const std::vector<std::shared_ptr<Statement>> &methods);

   std::vector<std::shared_ptr<TypedefDecl>> &getTypedefs();
   void setTypedefs(const std::vector<std::shared_ptr<TypedefDecl>> &typedefs);

   const std::vector<std::shared_ptr<PropDecl>> &getProperties() const;
   void setProperties(const std::vector<std::shared_ptr<PropDecl>> &properties);

   std::vector<Statement::SharedPtr> &getInnerDeclarations();
   void setInnerDeclarations(
      const std::vector<Statement::SharedPtr> &innerDeclarations);

   bool isVisited() const;

   Record *getRecord() const;
   void setRecord(Record *record);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_RECORDDECL_H
