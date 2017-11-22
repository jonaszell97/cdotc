//
// Created by Jonas Zell on 01.08.17.
//

#ifndef CDOT_TYPEDEFDECL_H
#define CDOT_TYPEDEFDECL_H

#include "../Statement.h"
#include "../../../Variant/Type/Generic.h"

namespace cdot {
namespace ast {

class TypeRef;

class TypedefDecl : public Statement {
public:
   TypedefDecl(
      AccessModifier access,
      string&& alias,
      std::shared_ptr<TypeRef>&& origin,
      std::vector<TemplateConstraint>&& generics
   );

   typedef std::shared_ptr<TypedefDecl> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == TypedefDeclID;
   }

protected:
   AccessModifier access;
   string alias;
   std::shared_ptr<TypeRef> origin;

   std::vector<TemplateConstraint> templateArgs;

   cdot::cl::Record *record = nullptr;

public:
   AccessModifier getAccess() const;
   void setAccess(AccessModifier access);

   const string &getAlias() const;
   void setAlias(const string &alias);

   const std::shared_ptr<TypeRef> &getOrigin() const;
   void setOrigin(const std::shared_ptr<TypeRef> &origin);

   std::vector<TemplateConstraint> &getTemplateArgs();
   void setTemplateArgs(const std::vector<TemplateConstraint> &templateArgs);

   cdot::cl::Record *getRecord() const;
   void setRecord(cdot::cl::Record *record);
};

} // namespace ast
} // namespace cdot

#endif //CDOT_TYPEDEFDECL_H
