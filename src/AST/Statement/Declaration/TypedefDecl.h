//
// Created by Jonas Zell on 01.08.17.
//

#ifndef CDOT_TYPEDEFDECL_H
#define CDOT_TYPEDEFDECL_H

#include "../Statement.h"
#include "../../../Variant/Type/Generic.h"

namespace cdot {

struct Alias;
struct Typedef;

namespace ast {

class ConstraintExpr;
class TypeRef;

class TypedefDecl : public Statement {
public:
   TypedefDecl(
      AccessModifier access,
      string&& alias,
      std::shared_ptr<TypeRef>&& origin,
      std::vector<TemplateParameter>&& generics
   );

   Typedef *getTypedef() const
   {
      return td;
   }

   void setTypedef(Typedef *td)
   {
      TypedefDecl::td = td;
   }

   typedef std::shared_ptr<TypedefDecl> SharedPtr;

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == TypedefDeclID;
   }

protected:
   AccessModifier access;
   string alias;
   std::shared_ptr<TypeRef> origin;

   std::vector<TemplateParameter> templateParams;

   cdot::cl::Record *record = nullptr;
   Typedef *td = nullptr;

public:
   AccessModifier getAccess() const
   {
      return access;
   }

   const string &getAlias() const
   {
      return alias;
   }

   const std::shared_ptr<TypeRef> &getOrigin() const
   {
      return origin;
   }

   std::vector<TemplateParameter> &getTemplateParams()
   {
      return templateParams;
   }

   cl::Record *getRecord() const
   {
      return record;
   }

   void setRecord(cl::Record *record)
   {
      TypedefDecl::record = record;
   }
};

class AliasDecl: public Statement {
public:
   AliasDecl(string &&name,
             std::vector<TemplateParameter> &&templateParams,
             std::vector<std::shared_ptr<StaticExpr>> &&constraints,
             std::shared_ptr<StaticExpr> &&aliasExpr)
      : Statement(AliasDeclID), name(move(name)),
        templateParams(move(templateParams)),
        constraints(move(constraints)), aliasExpr(move(aliasExpr))
   { }

   static bool classof(AstNode const* T)
   {
       return T->getTypeID() == AliasDeclID;
   }

private:
   std::string name;
   std::vector<TemplateParameter> templateParams;
   std::vector<std::shared_ptr<StaticExpr>> constraints;

   std::shared_ptr<StaticExpr> aliasExpr;
   Alias *alias = nullptr;

public:
   const string &getName() const
   {
      return name;
   }

   std::vector<TemplateParameter> &getTemplateParams()
   {
      return templateParams;
   }

   std::vector<std::shared_ptr<StaticExpr>> &getConstraints()
   {
      return constraints;
   }

   std::shared_ptr<StaticExpr> &getAliasExpr()
   {
      return aliasExpr;
   }

   std::vector<TemplateParameter> const& getTemplateParams() const
   {
      return templateParams;
   }

   std::vector<std::shared_ptr<StaticExpr>> const& getConstraints() const
   {
      return constraints;
   }

   std::shared_ptr<StaticExpr> const& getAliasExpr() const
   {
      return aliasExpr;
   }

   Alias *getAlias() const
   {
      return alias;
   }

   void setAlias(Alias *alias)
   {
      AliasDecl::alias = alias;
   }
};

} // namespace ast
} // namespace cdot

#endif //CDOT_TYPEDEFDECL_H
