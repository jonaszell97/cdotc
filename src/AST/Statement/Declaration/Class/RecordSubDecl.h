//
// Created by Jonas Zell on 14.11.17.
//

#ifndef CDOT_RECORDSUBDECL_H
#define CDOT_RECORDSUBDECL_H

#include "../../Statement.h"

namespace cdot {
namespace ast {

class StaticExpr;
class TypeRef;

class RecordSubDecl: public Statement {
protected:
   RecordSubDecl(
      NodeType typeID,
      std::string &&name,
      AccessModifier access,
      bool isStatic
   ) : Statement(typeID), name(std::move(name)), access(access),
      is_static(isStatic)
   {}

   std::string name;
   AccessModifier access;
   bool is_static;

   cdot::cl::Record *record = nullptr;

   static bool classof(AstNode const* T)
   {
      switch (T->getTypeID()) {
         case AstNode::FieldDeclID:
         case AstNode::PropDeclID:
         case AstNode::AssociatedTypeDeclID:
            return true;
         default:
            return false;
      }
   }

public:
   const string &getName() const
   {
      return name;
   }

   AccessModifier getAccess() const
   {
      return access;
   }

   void setAccess(AccessModifier access)
   {
      RecordSubDecl::access = access;
   }

   bool isStatic() const
   {
      return is_static;
   }

   void setRecord(cl::Record *record)
   {
      RecordSubDecl::record = record;
   }

   cl::Record *getRecord() const
   {
      return record;
   }
};

class AssociatedTypeDecl: public RecordSubDecl {
public:
   AssociatedTypeDecl(std::string &&protoSpec,
                      string &&name,
                      std::vector<std::shared_ptr<StaticExpr>> &&constraints,
                      std::shared_ptr<TypeRef> &&actualType)
      : RecordSubDecl(AssociatedTypeDeclID, move(name),
                      (AccessModifier)0, false),
        protocolSpecifier(move(protoSpec)),
        constraints(move(constraints)), actualType(move(actualType))
   {}

   const std::vector<std::shared_ptr<StaticExpr>> &getConstraints() const
   {
      return constraints;
   }

   const std::shared_ptr<TypeRef> &getActualType() const
   {
      return actualType;
   }

   void setActualType(const std::shared_ptr<TypeRef> &actualType)
   {
      AssociatedTypeDecl::actualType = actualType;
   }

   llvm::StringRef getProtocolSpecifier() const
   {
      return protocolSpecifier;
   }

   static bool classof(AstNode const *T)
   {
      return T->getTypeID() == AssociatedTypeDeclID;
   }

private:
   std::string protocolSpecifier;
   std::vector<std::shared_ptr<StaticExpr>> constraints;
   std::shared_ptr<TypeRef> actualType;
};

} // namespace ast
} // namespace cdot

#endif //CDOT_RECORDSUBDECL_H
