//
// Created by Jonas Zell on 01.08.17.
//

#ifndef CDOT_TYPEDEFDECL_H
#define CDOT_TYPEDEFDECL_H

#include "../Statement.h"
#include "../../../Variant/Type/Generic.h"

class TypeRef;
enum class AccessModifier : unsigned int;

class TypedefDecl : public Statement {
public:
   TypedefDecl(
      AccessModifier access,
      string&& alias,
      std::shared_ptr<TypeRef>&& origin,
      std::vector<TemplateConstraint>&& generics
   );

   typedef std::shared_ptr<TypedefDecl> SharedPtr;
   typedef std::unique_ptr<TypedefDecl> UniquePtr;

   std::vector<AstNode::SharedPtr> get_children() override;

   NodeType get_type() override {
      return NodeType::TYPEDEF_DECL;
   }

   ASTNODE_ACCEPT_PASSES
   ADD_FRIEND_PASSES

protected:
   AccessModifier access;
   string alias;
   std::shared_ptr<TypeRef> origin;

   std::vector<TemplateConstraint> templateArgs;

   cl::Record *record = nullptr;

public:
   AccessModifier getAccess() const;
   void setAccess(AccessModifier access);

   const string &getAlias() const;
   void setAlias(const string &alias);

   const std::shared_ptr<TypeRef> &getOrigin() const;
   void setOrigin(const std::shared_ptr<TypeRef> &origin);

   std::vector<TemplateConstraint> &getTemplateArgs();
   void setTemplateArgs(const std::vector<TemplateConstraint> &templateArgs);

   cl::Record *getRecord() const;
   void setRecord(cl::Record *record);
};


#endif //CDOT_TYPEDEFDECL_H
